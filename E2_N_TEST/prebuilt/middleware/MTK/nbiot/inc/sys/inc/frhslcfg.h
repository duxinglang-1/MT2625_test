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
 * \ingroup CfgCBEcorePLFservicesFrHsl
 * \ingroup CfgCBEcorePLFdriversFrHsl
 * \brief This is the header file to configure the FrHsl module.
 * This file contains default definitions of platform-specific macros to
 * customize the HSL hardware, interface and logging routines.  All macros
 * defined here can be overridden in a platform-specific header file.
 */

#ifndef FRHSLCFG_H
#define FRHSLCFG_H

/*******************************************************************************
** Includes
*******************************************************************************/
#if !defined (FRHSL_H)
# error This file must only be included directly from FRHSL.H
#endif

#include <system.h>
#include <frcfg.h>


/*******************************************************************************
** Platform specific include files
** Conditionally include any platform specific files here. These can override
** the configurable defines and macros in this file.
*******************************************************************************/
/* Only one of these configurations is selected  */
/*                                           */
/* For PC simulation env */
#if defined (FR_PC_ENABLE_HSL_LOB_FILE_OUT)
#    include <frpchsl.h>
#endif
/* For ULS h/w env */
#if defined (USE_ULS)
# include <frulscfg.h>
#endif
/* For GKI log */
#include <frgkicfg.h>

#include <frhslgrp.h>
#include <frprimelog.h>

#ifdef USE_FILTER_PRECHECK_FUNCTION
int16_t nbiot_trace_pre_check(int16_t GrpId);
#endif

/* Note: For users to fulfill the filterID, please take the way like this:
 *    (( HslLogModule_*** ) | (HslLogPriority_n ))
 */


/*******************************************************************************
** Config
*******************************************************************************/

/** \addtogroup CfgCBEcorePLFdriversFrHsl
 * @{ */


/*******************************************************************************
** Constants
*******************************************************************************/

/*******************************************************************************
* Define      : FR_HSL_BASE_ADDR
* Group       : Framework, HSL, Config
* Description : The base address for the Core HSL Controller.
*******************************************************************************/
#if !defined (FR_HSL_BASE_ADDR)
# error FR_HSL_BASE_ADDR needs to be defined
#endif

#if !defined (FR_HSL_ADDR_OFFSET_SHIFT)
/** Shift the address offsets left by this number of bits for every
 *  register in the HSL Controller.
 *  Shift the address offsets left by this number of bits for every
 *  register in the HSL Controller.  This may be required on certain platforms
 *  which can not provide 2-byte bus accesses to the HSL.
 */
# define FR_HSL_ADDR_OFFSET_SHIFT   (0)
#endif

/** Initial value to set the HSL config register to when initialising HSL.
 * The initial value for the HSL config register may need to be modified
 * for different platforms.  The only field anticipated to vary is the
 * clock divider.
 */
#if !defined (FR_HSL_INIT_CONFIG)
# define FR_HSL_INIT_CONFIG     (FR_HSL_CONFIG_PORT_ENABLE_BIT | \
                                 FR_HSL_CONFIG_TCU_LOG_BIT |     \
                                 FR_HSL_CONFIG_SW_LOG_BIT |      \
                                 FR_HSL_CONFIG_CLK_DIVIDE_1_BITS)
#endif

/** Initial value to set the HSL SwEnable0 register to when initialising HSL.
 * The HSL SwEnable0 register configures which multi-word software messages
 * are filtered out by the hardware when the top two bits of the message header
 * are 00.  The register may be altered at run-time if necessary.
 */
#if !defined (FR_HSL_INIT_SW_ENABLE_0)
# define FR_HSL_INIT_SW_ENABLE_0 0x3FFF
#endif

/** Initial value to set the HSL SwEnable1 register to when initialising HSL.
 * Same as SwEnable0 for message headers with the top two bits equal to 01.
 */
#if !defined (FR_HSL_INIT_SW_ENABLE_1)
# define FR_HSL_INIT_SW_ENABLE_1 0x3FFF
#endif

/** Initial value to set the HSL SwEnable1 register to when initialising HSL.
 * Same as SwEnable0 for message headers with the top two bits equal to 10.
 */
#if !defined (FR_HSL_INIT_SW_ENABLE_2)
# define FR_HSL_INIT_SW_ENABLE_2 0x3FFF
#endif

/** Initial value to set the HSL SwEnable1 register to when initialising HSL.
 * Same as SwEnable0 for message headers with the top two bits equal to 11.
 */
#if !defined (FR_HSL_INIT_SW_ENABLE_3)
# define FR_HSL_INIT_SW_ENABLE_3 0x3FFF
#endif

/** @} */ /* End of addtogroup CfgCBEcorePLFdriversFrHsl */

/*******************************************************************************
** Macros
*******************************************************************************/

/** \addtogroup CfgCBEcorePLFservicesFrHsl
 * @{ */


/** Macro used to read the data in a 16 bit wide register in the HSL controller.
 * Macro used to read the data in a 16 bit wide register in the HSL controller.
 * \param Int32 rEGaDDR = Absolute address of the register to access.
 * \returns the 16 bit value stored in the given register.
 */
#if !defined (M_FrHslReadInt16Reg)
# define M_FrHslReadInt16Reg(rEGaDDR)          (* (volatile Int16*) (rEGaDDR))
#endif

/** Macro used to write to registers in the HSL controller.
 *  Macro used to write to registers in the HSL controller.
 *  \param Int32 rEGaDDR = Absolute address of the register to access.
 *  \param Int16 dATA = Data to write to the given address.
 */
#if !defined (M_FrHslWriteInt16Reg)
# define M_FrHslWriteInt16Reg(rEGaDDR, dATA)   (* (volatile Int16*) (rEGaDDR)) = (Int16)(dATA)
#endif

/** Macro that can be redefined to allow extra initialisation before
 *  the HSL controller initialisation begins.
 *  Macro that can be redefined to allow extra initialisation before
 *  the HSL controller initialisation begins.
 */
#if !defined (M_FrHslInitialiseBegin)
# define M_FrHslInitialiseBegin()
#endif

/** Macro that can be redefined to allow extra initialisation after
 *  the HSL controller initialisation ends.
 *  Macro that can be redefined to allow extra initialisation after
 *  the HSL controller initialisation ends.
 */
#if !defined (M_FrHslInitialiseEnd)
# define M_FrHslInitialiseEnd()
#endif

/** Macro that can be redefined to allow code to be added at the
 *  start of the HSL flow control interrupt.
 *  Macro that can be redefined to allow code to be added at the
 *  start of the HSL flow control interrupt.
 */
#if !defined (M_FrHslFlowControlIntBegin)
# define M_FrHslFlowControlIntBegin()
#endif

/** Macro that can be redefined to allow code to be added at the
 *  end of the HSL flow control interrupt.
 *  Macro that can be redefined to allow code to be added at the
 *  end of the HSL flow control interrupt.
 */
#if !defined (M_FrHslFlowControlIntEnd)
# define M_FrHslFlowControlIntEnd()
#endif


/** Macro for writing One HSL Body element.
 *  Macro for writing One HSL Body element.
 */
#define M_FrHslWriteSwMsgBody1_zeroPad(aRG1) \
    M_FrHslWriteSwMsgBody(aRG1, 0);

/** Macro for writing Two HSL Body elements.
 *  Macro for writing Two HSL Body elements.
 */
#define M_FrHslWriteSwMsgBody2(aRG1, aRG2) \
    M_FrHslWriteSwMsgBody(aRG1, aRG2);

/** Macro for writing Three HSL Body elements.
 *  Macro for writing Three HSL Body elements.
 */
#define M_FrHslWriteSwMsgBody3(aRG1, aRG2, aRG3) \
    M_FrHslWriteSwMsgBody2(aRG1,aRG2);         \
    M_FrHslWriteSwMsgBody1_zeroPad(aRG3);

/** Macro for writing Four HSL Body elements.
 *  Macro for writing Four HSL Body elements.
 */
#define M_FrHslWriteSwMsgBody4(aRG1, aRG2, aRG3, aRG4) \
    M_FrHslWriteSwMsgBody2(aRG1,aRG2);         \
    M_FrHslWriteSwMsgBody2(aRG3,aRG4);

/** Macro for writing Five HSL Body elements.
 *  Macro for writing Five HSL Body elements.
 */
#define M_FrHslWriteSwMsgBody5(aRG1, aRG2, aRG3, aRG4, aRG5) \
    M_FrHslWriteSwMsgBody4(aRG1,aRG2, aRG3,aRG4);            \
    M_FrHslWriteSwMsgBody1_zeroPad(aRG5);

/** Macro for writing Six HSL Body elements.
 *  Macro for writing six HSL Body elements.
 */
#define M_FrHslWriteSwMsgBody6(aRG1, aRG2, aRG3, aRG4, aRG5, aRG6) \
    M_FrHslWriteSwMsgBody4(aRG1,aRG2, aRG3,aRG4);                  \
    M_FrHslWriteSwMsgBody2(aRG5,aRG6);

/** Macro for writing Seven HSL Body elements.
 *  Macro for writing Seven HSL Body elements.
 */
#define M_FrHslWriteSwMsgBody7(aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7) \
    M_FrHslWriteSwMsgBody6(aRG1,aRG2, aRG3, aRG4, aRG5, aRG6);         \
    M_FrHslWriteSwMsgBody1_zeroPad(aRG7);


/** Macro for writing Eight HSL Body elements.
 *  Macro for writing Eight HSL Body elements.
 */
#define M_FrHslWriteSwMsgBody8(aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8) \
    M_FrHslWriteSwMsgBody6(aRG1, aRG2, aRG3, aRG4, aRG5, aRG6);          \
    M_FrHslWriteSwMsgBody2(aRG7,aRG8);

/** Macro for writing Nine HSL Body elements.
 *  Macro for writing Nine HSL Body elements.
 */
#define M_FrHslWriteSwMsgBody9(aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, \
                               aRG9) \
    M_FrHslWriteSwMsgBody8(aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8);    \
    M_FrHslWriteSwMsgBody1_zeroPad(aRG9);

/** Macro for writing Ten HSL Body elements.
 *  Macro for writing Ten HSL Body elements.
 */
#define M_FrHslWriteSwMsgBody10(aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, \
                                aRG9, aRG10)                                    \
    M_FrHslWriteSwMsgBody8(aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8);     \
    M_FrHslWriteSwMsgBody2(aRG9, aRG10);

/** Macro for writing Eleven HSL Body elements.
 *  Macro for writing Eleven HSL Body elements.
 */
#define M_FrHslWriteSwMsgBody11(aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, \
                                aRG9, aRG10, aRG11)                             \
    M_FrHslWriteSwMsgBody10(aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,     \
                            aRG9, aRG10);                                       \
    M_FrHslWriteSwMsgBody1_zeroPad(aRG11);

/** Macro for writing Twelve HSL Body elements.
 *  Macro for writing Twelve HSL Body elements.
 */
#define M_FrHslWriteSwMsgBody12(aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, \
                                aRG9, aRG10, aRG11, aRG12)                      \
    M_FrHslWriteSwMsgBody10(aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,     \
                            aRG9, aRG10);                                \
    M_FrHslWriteSwMsgBody2(aRG11, aRG12);

/** Macro for writing Thirteen HSL Body elements.
 *  Macro for writing Thirteen HSL Body elements.
 */
#define M_FrHslWriteSwMsgBody13(aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, \
                                aRG9, aRG10, aRG11, aRG12, aRG13)               \
    M_FrHslWriteSwMsgBody12(aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,     \
                            aRG9, aRG10, aRG11, aRG12);                         \
    M_FrHslWriteSwMsgBody1_zeroPad(aRG13);

/** Macro for writing Fourteen HSL Body elements.
 *  Macro for writing Fourteen HSL Body elements.
 */
#define M_FrHslWriteSwMsgBody14(aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, \
                                aRG9, aRG10, aRG11, aRG12, aRG13, aRG14)        \
    M_FrHslWriteSwMsgBody12(aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,     \
                            aRG9, aRG10, aRG11, aRG12);                  \
    M_FrHslWriteSwMsgBody2(aRG13, aRG14);

/** Macro for writing Fifteen HSL Body elements.
 *  Macro for writing Fifteen HSL Body elements.
 */
#define M_FrHslWriteSwMsgBody15(aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8, \
                                aRG9, aRG10, aRG11, aRG12, aRG13, aRG14, aRG15) \
    M_FrHslWriteSwMsgBody14(aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,     \
                            aRG9, aRG10, aRG11, aRG12, aRG13, aRG14);           \
    M_FrHslWriteSwMsgBody1_zeroPad(aRG15);

/** Macro for writing Sixteen HSL Body elements.
 *  Macro for writing Sixteen HSL Body elements.
 */
#define M_FrHslWriteSwMsgBody16(aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,        \
                                aRG9, aRG10, aRG11, aRG12, aRG13, aRG14, aRG15, aRG16) \
    M_FrHslWriteSwMsgBody14(aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,            \
                            aRG9, aRG10, aRG11, aRG12, aRG13, aRG14);           \
    M_FrHslWriteSwMsgBody2(aRG15, aRG16);

/** \def M_FrHslDebugSniffMsg()
 * Macro to aid debugging by calling some function after each log point.
 * This macro is expanded after every log point and allows for small amounts
 * of debug to be frequently applied if needed.  By default the macro
 * does nothing.

 * If the macro \a M_FrHslDebugSniffFunction() is defined and no definition
 * for \a M_FrHslDebugSniffMsg() is already defined, \a M_FrHslDebugSniffMsg()
 * will be defined to a call \a M_FrHslDebugSniffFunction() as a function with
 * the following prototype, and conditionally log up to 4 values:
 *
 * \code
 *  Boolean func(Int16 *a, Int16 *b, Int16 *c, Int16 *c);
 * \endcode
 *
 * If the called function returns TRUE, the values of \a a, \a b, \a c and
 * \a d will be dumped in a LG_DEBUG_GENERAL logpoint.  If the function
 * returns FALSE, nothing is logged.
 *
 * \warning Carefully defining the macro M_FrHslDebugSniffMsg() makes it
 *          possible to 'sniff' out extra information to debug.  However,
 *          this macro is expanded for each log point and so considerable
 *          loading can be added to the target if used haplessly.
 */
# if !defined(M_FrHslDebugSniffMsg) && defined(M_FrHslDebugSniffFunction)
#  define M_FrHslDebugSniffMsg()                            \
        {                                                   \
            Int16 a, b, c, d;                               \
            if(M_FrHslDebugSniffFunction(&a, &b, &c, &d))   \
            {                                               \
                M_FrHslWriteSwMsgHeader(LG_DEBUG_GENERAL);  \
                M_FrHslWriteSwMsgBody(4);                   \
                M_FrHslWriteSwMsgBody3(a,b,c);              \
                M_FrHslWriteSwMsgLastWord(d);               \
            }                                               \
        }

/* Define prototype for the function */
Boolean M_FrHslDebugSniffFunction (Int16 *a, Int16 *b, Int16 *c, Int16 *d);

# elif !defined(M_FrHslDebugSniffMsg)
#  define M_FrHslDebugSniffMsg()
# endif

/** Write a 0 Byte message to HSL (just a message header, no data).
  * Write a 0 Byte message to HSL (just a message header, no data).
  * \param Int16 hDR = Message header word.
  */
#  if !defined (M_FrHsl0ByteMsg)
#   define M_FrHsl0ByteMsg(hDR,mDFlOGiD)                 \
         {                                      \
            M_FrHslWriteSwMsgSingle(hDR);       \
            M_FrHslDebugSniffMsg();             \
         }
#  endif

/** Write a 1 Byte message to HSL.
 *  Write a 1 Byte message to HSL.
 *  \param Int16 hDR = Message header word.
 *  \param Int16 aRG1 = First byte of the message.
 */
#  if !defined (M_FrHsl1ByteMsg)
#   define M_FrHsl1ByteMsg(hDR, mDFlOGiD,aRG1)  \
         {                                      \
             M_FrHslWriteSwMsgHeader(hDR, 1);   \
             M_FrHslWriteSwMsgLastWord(aRG1, 0);\
             M_FrHslDebugSniffMsg();            \
         }
#  endif

/** Write a 2 Byte message to HSL.
 *  Write a 2 Byte message to HSL.
 *  \param Int16 hDR = Message header word.
 *  \param Int16 aRG1 = First byte of the message.
 *  \param Int16 aRG2 = Second byte of the message.
 */
#  if !defined (M_FrHsl2ByteMsg)
#   define M_FrHsl2ByteMsg(hDR, mDFlOGiD, aRG1, aRG2)     \
         {                                          \
             M_FrHslWriteSwMsgHeader(hDR,2);        \
             M_FrHslWriteSwMsgLastWord(aRG1, aRG2); \
             M_FrHslDebugSniffMsg();                \
         }
#  endif

/** Write a 3 Byte message to HSL.
 *  Write a 3 Byte message to HSL.
 *  \param Int16 hDR = Message header word.
 *  \param Int16 aRG1 = First byte of the message.
 *  \param Int16 aRG2 = Second byte of the message.
 *  \param Int16 aRG3 = Third byte of the message.
 */
#  if !defined (M_FrHsl3ByteMsg)
#   define M_FrHsl3ByteMsg(hDR, mDFlOGiD, aRG1, aRG2, aRG3)   \
         {                                          \
             M_FrHslWriteSwMsgHeader(hDR,3);        \
             M_FrHslWriteSwMsgBody2(aRG1, aRG2);    \
             M_FrHslWriteSwMsgLastWord(aRG3, 0);    \
             M_FrHslDebugSniffMsg();                \
         }
#  endif

/** Write a 4 Byte message to HSL.
 *  Write a 4 Byte message to HSL.
 *  \param Int16 hDR = Message header word.
 *  \param Int16 aRG1 = First byte of the message.
 *  \param Int16 aRG2 = Second byte of the message.
 *  \param Int16 aRG3 = Third byte of the message.
 *  \param Int16 aRG4 = Fourth byte of the message.
 */
#  if !defined (M_FrHsl4ByteMsg)
#   define M_FrHsl4ByteMsg(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4)    \
         {                                                 \
             M_FrHslWriteSwMsgHeader(hDR, 4);              \
             M_FrHslWriteSwMsgBody2(aRG1, aRG2);           \
             M_FrHslWriteSwMsgLastWord(aRG3, aRG4);        \
             M_FrHslDebugSniffMsg();                       \
         }
#  endif

/** Write a 5 Byte message to HSL.
 *  Write a 5 Byte message to HSL.
 *  \param Int16 hDR = Message header word.
 *  \param Int16 aRG1 = First byte of the message.
 *  \param Int16 aRG2 = Second byte of the message.
 *  \param Int16 aRG3 = Third of the message.
 *  \param Int16 aRG4 = Fourth byte of the message.
 *  \param Int16 aRG5 = Fifth byte of the message.
 */
#  if !defined (M_FrHsl5ByteMsg)
#   define M_FrHsl5ByteMsg(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5)   \
         {                                                      \
             M_FrHslWriteSwMsgHeader(hDR, 5);                   \
             M_FrHslWriteSwMsgBody4(aRG1, aRG2, aRG3, aRG4);    \
             M_FrHslWriteSwMsgLastWord(aRG5, 0);                \
             M_FrHslDebugSniffMsg();                            \
         }
#  endif

/** Write a 6 Byte message to HSL.
 *  Write a 6 Byte message to HSL.
 *  \param Int16 hDR = Message header word.
 *  \param Int16 aRG1 = First byte of the message.
 *  \param Int16 aRG2 = Second byte of the message.
 *  \param Int16 aRG3 = Third byte of the message.
 *  \param Int16 aRG4 = Fourth byte of the message.
 *  \param Int16 aRG5 = Fifth byte of the message.
 *  \param Int16 aRG6 = Sixth byte of the message.
 */
#  if !defined (M_FrHsl6ByteMsg)
#   define M_FrHsl6ByteMsg(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6)    \
         {                                                             \
             M_FrHslWriteSwMsgHeader(hDR, 6);                          \
             M_FrHslWriteSwMsgBody4(aRG1, aRG2, aRG3, aRG4);           \
             M_FrHslWriteSwMsgLastWord(aRG5, aRG6);                    \
             M_FrHslDebugSniffMsg();                                   \
         }
#  endif

/** Write a 7 Byte message to HSL.
 *  Write a 7 Byte message to HSL.
 *  \param Int16 hDR = Message header word.
 *  \param Int16 aRG1 = First byte of the message.
 *  \param Int16 aRG2 = Second byte of the message.
 *  \param Int16 aRG3 = Third byte of the message.
 *  \param Int16 aRG4 = Fourth byte of the message.
 *  \param Int16 aRG5 = Fifth byte of the message.
 *  \param Int16 aRG6 = Sixth byte of the message.
 *  \param Int16 aRG7 = Seventh byte of the message.
 */
#  if !defined (M_FrHsl7ByteMsg)
#   define M_FrHsl7ByteMsg(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7)     \
         {                                                                    \
             M_FrHslWriteSwMsgHeader(hDR, 7);                                 \
             M_FrHslWriteSwMsgBody6(aRG1, aRG2, aRG3, aRG4, aRG5, aRG6);      \
             M_FrHslWriteSwMsgLastWord(aRG7, 0);                              \
             M_FrHslDebugSniffMsg();                                          \
         }
#  endif


/** Write a 8 Byte message to HSL.
 *  Write a 8 Byte message to HSL.
 *  \param Int16 hDR = Message header word.
 *  \param Int16 aRG1 = First byte of the message.
 *  \param Int16 aRG2 = Second byte of the message.
 *  \param Int16 aRG3 = Third byte of the message.
 *  \param Int16 aRG4 = Fourth byte of the message.
 *  \param Int16 aRG5 = Fifth byte of the message.
 *  \param Int16 aRG6 = Sixth byte of the message.
 *  \param Int16 aRG7 = Seventh byte of the message.
 *  \param Int16 aRG8 = Eighth byte of the message.
 */
#  if !defined (M_FrHsl8ByteMsg)
#   define M_FrHsl8ByteMsg(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8)   \
         {                                                                        \
             M_FrHslWriteSwMsgHeader(hDR, 8);                                     \
             M_FrHslWriteSwMsgBody6(aRG1, aRG2, aRG3, aRG4, aRG5, aRG6);          \
             M_FrHslWriteSwMsgLastWord(aRG7, aRG8);                               \
             M_FrHslDebugSniffMsg();                                              \
         }
#  endif

/** Write a 9 Byte message to HSL.
 *  Write a 9 Byte message to HSL.
 *  \param Int16 hDR = Message header word.
 *  \param Int16 aRG1 = First byte of the message.
 *  \param Int16 aRG2 = Second byte of the message.
 *  \param Int16 aRG3 = Third byte of the message.
 *  \param Int16 aRG4 = Fourth byte of the message.
 *  \param Int16 aRG5 = Fifth byte of the message.
 *  \param Int16 aRG6 = Sixth byte of the message.
 *  \param Int16 aRG7 = Seventh byte of the message.
 *  \param Int16 aRG8 = Eighth byte of the message.
 *  \param Int16 aRG9 = Nineth byte of the message.
 */
#  if !defined (M_FrHsl9ByteMsg)
#   define M_FrHsl9ByteMsg(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,           \
                                aRG9)                                                     \
         {                                                                                \
             M_FrHslWriteSwMsgHeader(hDR, 9);                                             \
             /*lint -save -e681 stops LINT complaining about the loop never executing */  \
             while(!M_FrHslFifoEmpty());                                                  \
             /*lint -restore */                                                           \
             M_FrHslWriteSwMsgBody8(aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8);      \
             M_FrHslWriteSwMsgLastWord(aRG9, 0);                                          \
             M_FrHslDebugSniffMsg();                                                      \
         }
#  endif

/** Write a 10 Byte message to HSL.
 *  Write a 10 Byte message to HSL.
 *  \param Int16 hDR = Message header word.
 *  \param Int16 aRG1 = First byte of the message.
 *  \param Int16 aRG2 = Second byte of the message.
 *  \param Int16 aRG3 = Third byte of the message.
 *  \param Int16 aRG4 = Fourth byte of the message.
 *  \param Int16 aRG5 = Fifth byte of the message.
 *  \param Int16 aRG6 = Sixth byte of the message.
 *  \param Int16 aRG7 = Seventh byte of the message.
 *  \param Int16 aRG8 = Eighth byte of the message.
 *  \param Int16 aRG9 = Nineth byte of the message.
 *  \param Int16 aRG10 = Tenth byte of the message.
 */
#  if !defined (M_FrHsl10ByteMsg)
#   define M_FrHsl10ByteMsg(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,         \
                                 aRG9, aRG10)                                            \
         {                                                                               \
             M_FrHslWriteSwMsgHeader(hDR, 10);                                           \
             /*lint -save -e681 stops LINT complaining about the loop never executing */ \
             while(!M_FrHslFifoEmpty());                                                 \
             /*lint -restore */                                                          \
             M_FrHslWriteSwMsgBody8(aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8);     \
             M_FrHslWriteSwMsgLastWord(aRG9, aRG10);                                     \
             M_FrHslDebugSniffMsg();                                                     \
         }
#  endif

/** Write a 11 Byte message to HSL.
 *  Write a 11 Byte message to HSL.
 *  \param Int16 hDR = Message header word.
 *  \param Int16 aRG1 = First byte of the message.
 *  \param Int16 aRG2 = Second byte of the message.
 *  \param Int16 aRG3 = Third byte of the message.
 *  \param Int16 aRG4 = Fourth byte of the message.
 *  \param Int16 aRG5 = Fifth byte of the message.
 *  \param Int16 aRG6 = Sixth byte of the message.
 *  \param Int16 aRG7 = Seventh byte of the message.
 *  \param Int16 aRG8 = Eighth byte of the message.
 *  \param Int16 aRG9 = Nineth byte of the message.
 *  \param Int16 aRG10 = Tenth byte of the message.
 *  \param Int16 aRG11 = Eleventh byte of the message.
 */
#  if !defined (M_FrHsl11ByteMsg)
#   define M_FrHsl11ByteMsg(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,         \
                                 aRG9, aRG10, aRG11)                                     \
         {                                                                               \
             M_FrHslWriteSwMsgHeader(hDR, 11);                                           \
             /*lint -save -e681 stops LINT complaining about the loop never executing */ \
             while(!M_FrHslFifoEmpty());                                                 \
             /*lint -restore */                                                          \
             M_FrHslWriteSwMsgBody10(aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,     \
                                         aRG9, aRG10);                                   \
             M_FrHslWriteSwMsgLastWord(aRG11, 0);                                        \
             M_FrHslDebugSniffMsg();                                                     \
         }
#  endif

/** Write a 12 Byte message to HSL.
 *  Write a 12 Byte message to HSL.
 *  \param Int16 hDR = Message header word.
 *  \param Int16 aRG1 = First byte of the message.
 *  \param Int16 aRG2 = Second byte of the message.
 *  \param Int16 aRG3 = Third byte of the message.
 *  \param Int16 aRG4 = Fourth byte of the message.
 *  \param Int16 aRG5 = Fifth byte of the message.
 *  \param Int16 aRG6 = Sixth byte of the message.
 *  \param Int16 aRG7 = Seventh byte of the message.
 *  \param Int16 aRG8 = Eighth byte of the message.
 *  \param Int16 aRG9 = Nineth byte of the message.
 *  \param Int16 aRG10 = Tenth byte of the message.
 *  \param Int16 aRG11 = Eleventh byte of the message.
 *  \param Int16 aRG12 = Twelveth of the message.
 */
#  if !defined (M_FrHsl12ByteMsg)
#   define M_FrHsl12ByteMsg(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,         \
                                 aRG9, aRG10, aRG11, aRG12)                              \
         {                                                                               \
             M_FrHslWriteSwMsgHeader(hDR, 12);                                           \
             /*lint -save -e681 stops LINT complaining about the loop never executing */ \
             while(!M_FrHslFifoEmpty());                                                 \
             /*lint -restore */                                                          \
             M_FrHslWriteSwMsgBody10(aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,     \
                                       aRG9, aRG10);                                     \
             M_FrHslWriteSwMsgLastWord(aRG11, aRG12);                                    \
             M_FrHslDebugSniffMsg();                                                     \
         }
#  endif

/** Write a 13 Byte message to HSL.
 *  Write a 13 Byte message to HSL.
 *  \param Int16 hDR = Message header word.
 *  \param Int16 aRG1 = First byte of the message.
 *  \param Int16 aRG2 = Second byte of the message.
 *  \param Int16 aRG3 = Third byte of the message.
 *  \param Int16 aRG4 = Fourth byte of the message.
 *  \param Int16 aRG5 = Fifth byte of the message.
 *  \param Int16 aRG6 = Sixth byte of the message.
 *  \param Int16 aRG7 = Seventh byte of the message.
 *  \param Int16 aRG8 = Eighth byte of the message.
 *  \param Int16 aRG9 = Nineth byte of the message.
 *  \param Int16 aRG10 = Tenth byte of the message.
 *  \param Int16 aRG11 = Eleventh byte of the message.
 *  \param Int16 aRG12 = Twelveth of the message.
 *  \param Int16 aRG13 = Thirteenth byte of the message.
 */
#  if !defined (M_FrHsl13ByteMsg)
#   define M_FrHsl13ByteMsg(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,         \
                                 aRG9, aRG10, aRG11, aRG12, aRG13)                       \
         {                                                                               \
             M_FrHslWriteSwMsgHeader(hDR, 13);                                           \
             M_FrHslWriteSwMsgBody12(aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,     \
                                       aRG9, aRG10, aRG11, aRG12);                       \
             /*lint -save -e681 stops LINT complaining about the loop never executing */ \
             while(!M_FrHslFifoEmpty());                                                 \
             /*lint -restore */                                                          \
             M_FrHslWriteSwMsgLastWord(aRG13, 0);                                        \
             M_FrHslDebugSniffMsg();                                                     \
         }
#  endif


/** Write a 14 Byte message to HSL.
 *  Write a 14 Byte message to HSL.
 *  \param Int16 hDR = Message header word.
 *  \param Int16 aRG1 = First byte of the message.
 *  \param Int16 aRG2 = Second byte of the message.
 *  \param Int16 aRG3 = Third byte of the message.
 *  \param Int16 aRG4 = Fourth byte of the message.
 *  \param Int16 aRG5 = Fifth byte of the message.
 *  \param Int16 aRG6 = Sixth byte of the message.
 *  \param Int16 aRG7 = Seventh byte of the message.
 *  \param Int16 aRG8 = Eighth byte of the message.
 *  \param Int16 aRG9 = Nineth byte of the message.
 *  \param Int16 aRG10 = Tenth byte of the message.
 *  \param Int16 aRG11 = Eleventh byte of the message.
 *  \param Int16 aRG12 = Twelveth of the message.
 *  \param Int16 aRG13 = Thirteenth byte of the message.
 *  \param Int16 aRG14 = Fourteenth byte of the message.
 */
#  if !defined (M_FrHsl14ByteMsg)
#   define M_FrHsl14ByteMsg(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,         \
                                 aRG9, aRG10, aRG11, aRG12, aRG13, aRG14)                \
         {                                                                               \
             M_FrHslWriteSwMsgHeader(hDR, 14);                                           \
             M_FrHslWriteSwMsgBody12(aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,     \
                                       aRG9, aRG10, aRG11, aRG12);                       \
             /*lint -save -e681 stops LINT complaining about the loop never executing */ \
             while(!M_FrHslFifoEmpty());                                                 \
             /*lint -restore */                                                          \
             M_FrHslWriteSwMsgLastWord(aRG13, aRG14);                                    \
             M_FrHslDebugSniffMsg();                                                     \
         }
#  endif


/** Write a 15 Byte message to HSL.
 *  Write a 15 Byte message to HSL.
 *  \param Int16 hDR = Message header word.
 *  \param Int16 aRG1 = First byte of the message.
 *  \param Int16 aRG2 = Second byte of the message.
 *  \param Int16 aRG3 = Third byte of the message.
 *  \param Int16 aRG4 = Fourth byte of the message.
 *  \param Int16 aRG5 = Fifth byte of the message.
 *  \param Int16 aRG6 = Sixth byte of the message.
 *  \param Int16 aRG7 = Seventh byte of the message.
 *  \param Int16 aRG8 = Eighth byte of the message.
 *  \param Int16 aRG9 = Nineth byte of the message.
 *  \param Int16 aRG10 = Tenth byte of the message.
 *  \param Int16 aRG11 = Eleventh byte of the message.
 *  \param Int16 aRG12 = Twelveth of the message.
 *  \param Int16 aRG13 = Thirteenth byte of the message.
 *  \param Int16 aRG14 = Fourteenth byte of the message.
 *  \param Int16 aRG15 = Fifteenth byte of the message.
 */
#  if !defined (M_FrHsl15ByteMsg)
#   define M_FrHsl15ByteMsg(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,         \
                                 aRG9, aRG10, aRG11, aRG12, aRG13, aRG14, aRG15)         \
         {                                                                               \
             M_FrHslWriteSwMsgHeader(hDR, 15);                                           \
             M_FrHslWriteSwMsgBody14(aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,     \
                                       aRG9, aRG10, aRG11, aRG12, aRG13, aRG14);         \
             /*lint -save -e681 stops LINT complaining about the loop never executing */ \
             while(!M_FrHslFifoEmpty());                                                 \
             /*lint -restore */                                                          \
             M_FrHslWriteSwMsgLastWord(aRG15, 0);                                        \
             M_FrHslDebugSniffMsg();                                                     \
         }
#  endif


/** Write a 16 Byte message to HSL.
 *  Write a 16 Byte message to HSL.
 *  \param Int16 hDR = Message header word.
 *  \param Int16 aRG1 = First byte of the message.
 *  \param Int16 aRG2 = Second byte of the message.
 *  \param Int16 aRG3 = Third byte of the message.
 *  \param Int16 aRG4 = Fourth byte of the message.
 *  \param Int16 aRG5 = Fifth byte of the message.
 *  \param Int16 aRG6 = Sixth byte of the message.
 *  \param Int16 aRG7 = Seventh byte of the message.
 *  \param Int16 aRG8 = Eighth byte of the message.
 *  \param Int16 aRG9 = Nineth byte of the message.
 *  \param Int16 aRG10 = Tenth byte of the message.
 *  \param Int16 aRG11 = Eleventh byte of the message.
 *  \param Int16 aRG12 = Twelveth of the message.
 *  \param Int16 aRG13 = Thirteenth byte of the message.
 *  \param Int16 aRG14 = Fourteenth byte of the message.
 *  \param Int16 aRG15 = Fifteenth byte of the message.
 *  \param Int16 aRG16 = Sixteenth of the message.
 */
#  if !defined (M_FrHsl16ByteMsg)
#   define M_FrHsl16ByteMsg(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,          \
                                 aRG9, aRG10, aRG11, aRG12, aRG13, aRG14, aRG15, aRG16)   \
         {                                                                                \
             M_FrHslWriteSwMsgHeader(hDR, 16);                                           \
             M_FrHslWriteSwMsgBody14(aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,     \
                                       aRG9, aRG10, aRG11, aRG12, aRG13, aRG14);         \
             /*lint -save -e681 stops LINT complaining about the loop never executing */ \
             while(!M_FrHslFifoEmpty());                                                 \
             /*lint -restore */                                                          \
             M_FrHslWriteSwMsgLastWord(aRG15, aRG16);                                    \
             M_FrHslDebugSniffMsg();                                                      \
         }
#  endif

/** Write a message that is an array of Int16s.
 *  Write a message that is an array of Int16s.
 *  \param Int16 hDR = Message header word.
 *  \param Int16 nUM = Number of Int16s in the array.
 *  \param Int16 *aRRAY = Array of nUM Int16s.
 */
//#  if !defined (M_FrHslInt16ArrayMsg)
#    define M_FrHslInt16ArrayMsg(hDR, nUM, aRRAY)                             \
             {                                                                \
                 Int16        nToCopy = nUM;                                  \
                 const Int16 *aRRAYpTR = aRRAY;                               \
                                                                              \
                 /* +2 for Address */                                         \
                 M_FrHslWriteSwMsgHeader(hDR, nToCopy + 2);                   \
                 if (nToCopy == 0)                                            \
                 {   /* Write address */                                      \
                     M_FrHslWriteSwMsgLastWord( (Int16)(((Int32)aRRAYpTR) >> 16),       \
                                               (Int16)(((Int32)aRRAYpTR) & 0xFFFF) );   \
                 }                                                            \
                 else                                                         \
                 {   /* Write address */                                      \
                     M_FrHslWriteSwMsgBody( (Int16)(((Int32)aRRAYpTR) >> 16),           \
                                            (Int16)(((Int32)aRRAYpTR) & 0xFFFF) );      \
                     /* Write Data */                                         \
                     while(nToCopy > 2)                                       \
                     {                                                        \
                       M_FrHslThrottle (nToCopy);                             \
                       M_FrHslWriteSwMsgBody(aRRAYpTR[0],aRRAYpTR[1]);        \
                       nToCopy -= 2;                                          \
                       aRRAYpTR += 2;                                         \
                     }                                                        \
                                                                              \
                     if(nToCopy == 2)                                         \
                     {                                                        \
                         M_FrHslWriteSwMsgLastWord(aRRAYpTR[0],aRRAYpTR[1]);  \
                     }                                                        \
                     else                                                     \
                     {                                                        \
                         M_FrHslWriteSwMsgLastWord(aRRAYpTR[0],0);            \
                     }                                                        \
                 }                                                            \
                 M_FrHslDebugSniffMsg();                                      \
             }


//#define M_FrHslInt16ArrayMsg(hDR, nUM, aRRAY)   FrHslInt16ArrayMsg(hDR, nUM, aRRAY)
//#  endif

/**  Write a message that is an array of Int32s.
 *   Write a message that is an array of Int32s.
 *  \param Int16 hDR = Message header word.
 *  \param Int16 nUM = Number of Int32s in the array.
 *  \param Int32 *aRRAY = Array of nUM Int32s.
 */
#  if !defined (M_FrHslInt32ArrayMsg)
#    define M_FrHslInt32ArrayMsg(hDR, nUM, aRRAY)                             \
             {                                                                \
                 Int16        nToCopy = (Int16) nUM;                          \
                 const Int32 *aRRAYpTR = (const Int32 *) aRRAY;               \
                 Int32        dATA;                                           \
                                                                              \
                 /* +2 for Address, * 2 for 16 bit words */                   \
                 M_FrHslWriteSwMsgHeader(hDR, (nToCopy * 2) + 2);             \
                 if (nToCopy == 0)                                            \
                 {   /* Write address */                                      \
                     M_FrHslWriteSwMsgLastWord( (Int16)(((Int32) aRRAYpTR) >> 16),      \
                                                (Int16)(((Int32) aRRAYpTR) & 0xFFFF) ); \
                 }                                                            \
                 else                                                         \
                 {   /* Write address */                                      \
                     M_FrHslWriteSwMsgBody( (Int16)(((Int32) aRRAYpTR) >> 16),          \
                                            (Int16)(((Int32) aRRAYpTR) & 0xFFFF) );     \
                     /* Write Data */                                         \
                     while (nToCopy > 1)                                      \
                     {                                                        \
                         dATA = *aRRAYpTR;                                    \
                         M_FrHslThrottle(nToCopy * 2);                        \
                         M_FrHslWriteSwMsgBody((dATA >> 16), dATA);           \
                         nToCopy--;                                           \
                         aRRAYpTR++;                                          \
                     }                                                        \
                     dATA = *aRRAYpTR;                                        \
                     M_FrHslWriteSwMsgLastWord((dATA >> 16), dATA);           \
                 }                                                            \
                 M_FrHslDebugSniffMsg();                                      \
             }
#  endif /* M_FrHslInt32ArrayMsg */

/** Write a string message
 *  Write a string message
 *  \param Int16  hDR = Message header word.
 *  \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
 *  \param Int16  *pDATA = Array of lENGTH MS_U16s.
 */
# if !defined (M_FrHslString)
#   define M_FrHslString(hDR, gROUPiD, formatSpecifier, aRRAY)      \
           if( M_FrHslIsLogPointEnabled(hDR,gROUPiD) )              \
           {                                                        \
              while(!M_FrHslFifoEmpty());                           \
              FrHslString(hDR, aRRAY);                              \
           }
# endif /* M_FrHslString */

/** Macro to control the HSL Fifo write rate.
 *  Macro to control the HSL Fifo write rate.
 *  \param wRITTENcOUNTER = Count of the number of values written to the
 *                          HSL FIFO for the current log point.
 */
# if !defined (M_FrHslThrottle)
#   define M_FrHslThrottle(wRITTENcOUNTER)
# endif /* M_FrHslThrottle */


/*******************************************************************************
* Alternative macros
*******************************************************************************/


/** \def ENABLE_GATE_ON_ARRAY_LOGS
 *  Allows individual HSL messages (of array data) to be enabled/disabled on
 *  a case-by-case basis.
 *
 *  The switch ENABLE_GATE_ON_ARRAY_LOGS allows individual HSL
 *  messages (of array data) to be enabled/disabled on a
 *  case-by-case basis. This functionality differs from using
 *  the HSL hardware filter to enable/disable output of certain
 *  log messages, because the latter does not impact on the
 *  MIPS overhead associated with logging.
 *  ENABLE_GATE_ON_ARRAY_LOGS is intended for use during
 *  development when it may be appropriate to reduce the MIPS
 *  consumed by logging relatively large arrays of data. When
 *  enabled, every array log requires a definition of the form
 *  ENABLE_logHeader (where "logHeader" is the message's log
 *  header name/id) to be defined. The definition must assign
 *  the value 0 to disable the log message or non-zero to enable
 *  it. NOTE that it is not possible to allow the pre-processor
 *  to completely eliminate disabled log points. We therefore
 *  use the construct "if ((ENABLE_ ## hDR)) {}" to perform the
 *  gating operation and rely on the compiler to optimise (to
 *  nothing) any code that would otherwise have been generated
 *  by this log macro. In framework the call to the Logging macro
 *  should be conditionally compiled in the application code so
 *  as to not rely on the tools to remove unnecessary code. This
 *  also aids clarity when try to find out what a conditional
 *  compile affects.
 */



/** \def UPGRADE_FR_HSL_VERSION2
 *  This define is used to enable the HSL version 2 functionality
 *  This define is used to enable the HSL version 2 functionality.
 *  HSL version 2 provides a new set of HSL logging macros to ease the addition of new HSL log
 *  points. The log point header ids are automatically generated via a pre-preprocessor called
 *  silox.exe. This utility will parse header/unit files to detect that the correct syntax is used
 *  and insert IDs according to a defined set of rules. Two types of IDs are currently suported.
 *  One set of ID is for log point statements which will always be logged e.g. M_FrHslPrintfx().
 *  Another set of ID are for array logging statements and macros based on M_FrHslFilterPrintf().
 *  The define ENABLE_GATE_ON_ARRAY_LOGS is used to provide compile time enabling/disabling of the
 *  filtered log points.
 *  Filtering per log point is applied on all M_FrHslFilterPrintfx() and M_FrHslArray16(),
 *  M_FrHslShArray16(),M_FrHslArray32() macros.
 */


/** \def FR_HSL_V2_UNITIALISED_LOG_ID
 *  Log point id assigned when using MDF to capture a new log point.
 *  Log point id assigned when using MDF to capture a new log point.
 *  This value should be used when adding a log point with a new MDF
 *  compatible log point id but using HSL version 2 macros.
 *  e.g. M_FrHslPrintf1(FR_HSL_V2_UNITIALISED_LOG_ID, NEW_MDF_COMPAT_ID,"NewMdfCompaqId: %#.4x", arg1);
 *  NEW_MDF_COMPAT_ID is the new log point added for MDF to be able to decode it.
 */
#define FR_HSL_V2_UNITIALISED_LOG_ID ((Int16)0x0000)

/** \def FR_HSL_V2_UNPROCESSED_LOG_ID
 *  Log point id assigned when an HSL version 2 statement has not been processed by silox.exe.
 *  Log point id assigned when an HSL version 2 statement has not been processed by silox.exe.
 *  This ID is used as part of stub functions which will print a default HSL log message when
 *  new HSL log points have been added but have not been processed by silox.exe.
 */
#define FR_HSL_V2_UNPROCESSED_LOG_ID ((Int16)0x0002)

#ifndef LG_DEBUG_1
# define    LG_DEBUG_1                  (0x0000)
#endif /* LG_DEBUG_1 */

/* Default HSL group id inserted by silox when none provided. */
#ifndef DEFAULT_GROUP
# define DEFAULT_GROUP                 (0x0001)
#endif /* DEFAULT_GROUP */

# define M_FrHslBypassCompileTimeFiltering(hDR, mDFlOGiD)                                                                       \
   ( mDFlOGiD == LG_DEBUG_1 ) /* Output LG_DEBUG_1 (not classified) logs by default */

/* Until this is implemented make it dummy */
# define M_FrHslIsLogPointEnabledRuntime(hDR, mDFlOGiD) TRUE
//# define M_FrHslIsLogPointEnabledRuntime(hDR, mDFlOGiD) FrHslDynamicLogPointTest((hDR), (mDFlOGiD))
//#define M_FrHslIsSpecialFilterGrp(mDFlOGiD) TRUE
//#define M_FrHslPriorityNumFromSpecialFilterGrp(mDFlOGiD) TRUE
/** \def M_FrHslIsLogPointEnabledCompileTime
 * Macro to test if a log point is enabled, "executed" at compile-time (no runtime overhead) */
# define M_FrHslIsLogPointEnabledCompileTime(hDR, mDFlOGiD)                                                                     \
    ( /* Bypass Filtering */                                                                                                    \
            M_FrHslBypassCompileTimeFiltering(hDR, mDFlOGiD) ||                                                                 \
      /* Global Priority */ (                                                                                                   \
            !M_FrHslIsSpecialFilterGrp(mDFlOGiD) &&                                                                             \
            (0 != ( (mDFlOGiD) & M_FrHslFilterPriorityMaskFromBitmap(HSL_FILTER_GLOBAL_PRIORITY))) ) ||                         \
      /* Group-level Priority */ (                                                                                              \
            M_FrHslIsSpecialFilterGrp(mDFlOGiD) &&                                                                              \
            (0 != ( (mDFlOGiD) & M_FrHslFilterPriorityMaskFromNum( M_FrHslPriorityNumFromSpecialFilterGrp( (mDFlOGiD) ))) ) )   \
    )

/** \def M_FrHslFilterUsingDynamicFiltering
 * Should the global M_FrHslIsLogPointEnabled macro fallback to M_FrHslIsLogPointEnabledDynamic ?
 */
# define M_FrHslFilterUsingDynamicFiltering(hDR, mDFlOGiD)                                                                      \
    (FALSE)

/** \def M_FrHslIsLogPointEnabled
 *  Macro to test if a log point is enabled or not (at compile time).
 *  \param Int16 hDR Log point ID to check if it is currently enabled or not.
 *  \param Int16 mDFlOGiD Log point Group
 */
#if defined (NBIOT_TRACE_FILTER)
#include <nbiot_trace.h>
#ifdef USE_FILTER_PRECHECK_FUNCTION
#define M_FrHslIsLogPointEnabled(hDR, mDFlOGiD)  nbiot_trace_pre_check( mDFlOGiD )
#else
#define M_FrHslIsLogPointEnabled(hDR, mDFlOGiD)  NBIOT_TRACE_PRE_CHECK( mDFlOGiD )
#endif //USE_FILTER_PRECHECK_FUNCTION
#else
#define M_FrHslIsLogPointEnabled(hDR, mDFlOGiD)                                                                                 \
    ( M_FrHslFilterUsingDynamicFiltering(hDR, mDFlOGiD) ? ( M_FrHslIsLogPointEnabledDynamic(hDR, mDFlOGiD) ) : (M_FrHslIsLogPointEnabledCompileTime(hDR, mDFlOGiD)) )
#endif
/** \def M_FrHslIsLogPointEnabledDynamic
 *  Macro to test if a log point is enabled or not (at compile time AND runtime)
 *  \param Int16 hDR Log point ID to check if it is currently enabled or not.
 *  \param Int16 mDFlOGiD Log point Group
 *
 *  CompileTime version has higher priority, then comes Runtime version.
 */
#define M_FrHslIsLogPointEnabledDynamic(hDR, mDFlOGiD)                                                                          \
    ( M_FrHslIsLogPointEnabledCompileTime(hDR, mDFlOGiD) && M_FrHslIsLogPointEnabledRuntime(hDR, mDFlOGiD) )

/* No filtering applied on FrHslPrintx() macros, so the macros always resolve to their previous forms */

/** \def FR_HSL_PRINTF
 *  HSL log are directly outputed on a text file
 */
#if defined(FR_HSL_PRINTF)
extern void FrPcHslPrintf(const char *fmt, ...);

# define M_FrHslPrintf0(hDR,mDFlOGiD,fORMATsPECIFER)                                                   FrPcHslPrintf(fORMATsPECIFER)
# define M_FrHslPrintf1(hDR,mDFlOGiD,fORMATsPECIFER, aRG1)                                             FrPcHslPrintf(fORMATsPECIFER,aRG1)
# define M_FrHslPrintf2(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2)                                       FrPcHslPrintf(fORMATsPECIFER,aRG1,aRG2)
# define M_FrHslPrintf3(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3)                                 FrPcHslPrintf(fORMATsPECIFER, aRG1, aRG2, aRG3)
# define M_FrHslPrintf4(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4)                           FrPcHslPrintf(fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4)
# define M_FrHslPrintf5(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5)                     FrPcHslPrintf(fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5)
# define M_FrHslPrintf6(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6)               FrPcHslPrintf(fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6)
# define M_FrHslPrintf7(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7)         FrPcHslPrintf(fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7)
# define M_FrHslPrintf8(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8)   FrPcHslPrintf(fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8)
# define M_FrHslPrintf9(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                      \
                                                     aRG9 )                                                               \
                      FrPcHslPrintf(fORMATsPECIFER, aRG1,aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                        \
                                                     aRG9)

# define M_FrHslPrintf10(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                      aRG9, aRG10 )                                                       \
                      FrPcHslPrintf(fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                       \
                                                      aRG9, aRG10 )

# define M_FrHslPrintf11(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                      aRG9, aRG10, aRG11 )                                                \
                      FrPcHslPrintf(fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                       \
                                                      aRG9, aRG10, aRG11 )

# define M_FrHslPrintf12(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                      aRG9, aRG10, aRG11, aRG12 )                                         \
                      FrPcHslPrintf(fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                       \
                                                      aRG9, aRG10, aRG11 )

# define M_FrHslPrintf13(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                      aRG9, aRG10, aRG11, aRG12, aRG13 )                                  \
                      FrPcHslPrintf(fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                       \
                                                      aRG9, aRG10, aRG11, aRG12, aRG13 )

# define M_FrHslPrintf14(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                      aRG9, aRG10, aRG11, aRG12, aRG13, aRG14 )                           \
                      FrPcHslPrintf(fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                       \
                                                      aRG9, aRG10, aRG11, aRG12, aRG13, aRG14 )

# define M_FrHslPrintf15(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                      aRG9, aRG10, aRG11, aRG12, aRG13, aRG14, aRG15 )                    \
                      FrPcHslPrintf(fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                       \
                                                      aRG9, aRG10, aRG11, aRG12, aRG13, aRG14, aRG15 )

# define M_FrHslPrintf16(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                      aRG9, aRG10, aRG11, aRG12, aRG13, aRG14, aRG15, aRG16 )             \
                      FrPcHslPrintf(fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                       \
                                                      aRG9, aRG10, aRG11, aRG12, aRG13, aRG14, aRG15, aRG16 )

#else /* FR_HSL_PRINTF */

#if defined (ULS_FUNCTION_PROFILER)
                                 
#else                                            
/** Write a 0 argument message to HSL (just a message header, no data).
  * Write a 0 argument message to HSL (just a message header, no data). Equivalent
  * Equivalent to former LG_MES_0 macro.
  * \param Int16 hDR = Autogenerated Log point ID.
  * \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  */

# define M_FrHslPrintf0(hDR,mDFlOGiD,fORMATsPECIFER)                      \
           if ( M_FrPrimeLogFilter(mDFlOGiD) )                       \
           {                                                         \
             M_FrPrimeLogMsg1((Int32)hDR<<16);                       \
           }\
           if(M_FrPrimeLogFilterHslCheck(mDFlOGiD) )\
           {\
               if ( M_FrHslIsLogPointEnabled(hDR, M_FrPrimeLogGetHslMdfId(mDFlOGiD) ) )                 \
               {                                                              \
                 M_FrHsl0ByteMsg(hDR,M_FrPrimeLogGetHslMdfId(mDFlOGiD));                                        \
               }\
           }

/** Write a 1 argument message to HSL (just a message header, no data).
  * Write a 1 argument message to HSL (just a message header, no data).
  * Equivalent to former LG_MES_1 macro.
  * \param Int16 hDR = Autogenerated Log point ID.
  * \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param Int16 aRG1 = argument value to log
  */

# define M_FrHslPrintf1(hDR,mDFlOGiD,fORMATsPECIFER, aRG1)                \
           if ( M_FrPrimeLogFilter(mDFlOGiD) )                       \
           {                                                         \
             M_FrPrimeLogMsg1(((Int32)hDR<<16)|(aRG1));                       \
           }\
           if(M_FrPrimeLogFilterHslCheck(mDFlOGiD))\
           {\
               if ( M_FrHslIsLogPointEnabled(hDR, M_FrPrimeLogGetHslMdfId(mDFlOGiD) ))                 \
               {                                                              \
                 M_FrHsl1ByteMsg(hDR, M_FrPrimeLogGetHslMdfId(mDFlOGiD), aRG1);                                  \
               }\
           }

/** Write a 2 argument message to HSL (just a message header, no data).
  * Write a 2 argument message to HSL (just a message header, no data).
  * Equivalent to former LG_MES_2 macro.
  * \param Int16 hDR = Autogenerated Log point ID.
  * \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param Int16 aRG1 = first argument value to log
  * \param Int16 aRG2 = second argument value to log
  */

# define M_FrHslPrintf2(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2)          \
           M_FrPrimeLogCheckInterface(mDFlOGiD);\
           if ( M_FrHslIsLogPointEnabled(hDR, mDFlOGiD) )                 \
           {                                                              \
             M_FrHsl2ByteMsg(hDR, mDFlOGiD, aRG1, aRG2);                            \
           }

/** Write a 3 argument message to HSL (just a message header, no data).
  * Write a 3 argument message to HSL (just a message header, no data).
  * Equivalent to former LG_MES_3 macro.
  * \param Int16 hDR = Autogenerated Log point ID.
  * \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param Int16 aRG1 = first argument value to log
  * \param Int16 aRG2 = second argument value to log
  * \param Int16 aRG3 = third argument value to log
  */

/** Write a 1 argument message to HSL (just a message header, no data).
  * Write a 1 argument message to HSL (just a message header, no data).
  * Equivalent to former LG_MES_1 macro.
  * \param Int16 hDR = Autogenerated Log point ID.
  * \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param Int16 aRG1 = argument value to log
  */

# define M_FrHslPrintf3(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3)    \
           if ( M_FrPrimeLogFilter(mDFlOGiD) )                       \
           {                                                         \
             M_FrPrimeLogMsg2(((Int32)hDR<<16)|(aRG1), ((Int32)(aRG2)<<16)|(aRG3)); \
           }\
           if(M_FrPrimeLogFilterHslCheck(mDFlOGiD))\
           {\
               if ( M_FrHslIsLogPointEnabled(hDR, M_FrPrimeLogGetHslMdfId(mDFlOGiD) ))                 \
               {                                                              \
                 M_FrHsl3ByteMsg(hDR, M_FrPrimeLogGetHslMdfId(mDFlOGiD), aRG1, aRG2, aRG3);                      \
               }\
           }

/** Write a 4 argument message to HSL (just a message header, no data).
  * Write a 4 argument message to HSL (just a message header, no data).
  * Equivalent to former LG_MES_4 macro.
  * \param Int16 hDR = Autogenerated Log point ID.
  * \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param Int16 aRG1 = first argument value to log
  * \param Int16 aRG2 = second argument value to log
  * \param Int16 aRG3 = third argument value to log
  * \param Int16 aRG4 = fourth argument value to log
  */
# define M_FrHslPrintf4(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4)    \
           M_FrPrimeLogCheckInterface(mDFlOGiD);\
           if ( M_FrHslIsLogPointEnabled(hDR, mDFlOGiD) )                       \
           {                                                                    \
             M_FrHsl4ByteMsg(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4);                      \
           }

/** Write a 5 argument message to HSL (just a message header, no data).
  * Write a 5 argument message to HSL (just a message header, no data).
  * \param Int16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_5 macro.
  * \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param Int16 aRG1 = first argument value to log
  * \param Int16 aRG2 = second argument value to log
  * \param Int16 aRG3 = third argument value to log
  * \param Int16 aRG4 = fourth argument value to log
  * \param Int16 aRG5 = fifth argument value to log
  */
# define M_FrHslPrintf5(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5)    \
           if ( M_FrPrimeLogFilter(mDFlOGiD) )                       \
           {                                                         \
             M_FrPrimeLogMsg3(((Int32)hDR<<16)|(aRG1), ((Int32)(aRG2)<<16)|(aRG3), ((Int32)(aRG4)<<16)|(aRG5));  \
           }\
           if(M_FrPrimeLogFilterHslCheck(mDFlOGiD))\
           {\
               if ( M_FrHslIsLogPointEnabled(hDR, M_FrPrimeLogGetHslMdfId(mDFlOGiD)) )                 \
               {                                                              \
                 M_FrHsl5ByteMsg(hDR, M_FrPrimeLogGetHslMdfId(mDFlOGiD), \
                        aRG1, aRG2, aRG3, aRG4, aRG5);     \
               }\
           }




/** Write a 6 argument message to HSL (just a message header, no data).
  * Write a 6 argument message to HSL (just a message header, no data).
  * \param Int16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_6 macro.
  * \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param Int16 aRG1 = first argument value to log
  * \param Int16 aRG2 = second argument value to log
  * \param Int16 aRG3 = third argument value to log
  * \param Int16 aRG4 = fourth argument value to log
  * \param Int16 aRG5 = fifth argument value to log
  * \param Int16 aRG6 = sixth argument value to log
  */
# define M_FrHslPrintf6(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6)    \
           M_FrPrimeLogCheckInterface(mDFlOGiD);\
           if ( M_FrHslIsLogPointEnabled(hDR, mDFlOGiD) )                                   \
           {                                                                                \
             M_FrHsl6ByteMsg(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6);                      \
           }

/** Write a 7 argument message to HSL (just a message header, no data).
  * Write a 7 argument message to HSL (just a message header, no data).
  * \param Int16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_6 macro.
  * \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param Int16 aRG1 = first argument value to log
  * \param Int16 aRG2 = second argument value to log
  * \param Int16 aRG3 = third argument value to log
  * \param Int16 aRG4 = fourth argument value to log
  * \param Int16 aRG5 = fifth argument value to log
  * \param Int16 aRG6 = sixth argument value to log
  * \param Int16 aRG7 = seventh argument value to log
  */

# define M_FrHslPrintf7(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7)    \
           if ( M_FrPrimeLogFilter(mDFlOGiD) )                       \
           {                                                         \
             M_FrPrimeLogMsg45(((Int32)hDR<<16)|(aRG1), ((Int32)(aRG2)<<16)|(aRG3), \
                ((Int32)(aRG4)<<16)|(aRG5), ((Int32)(aRG6)<<16)|(aRG7), 0, 4);                      \
           }\
           if(M_FrPrimeLogFilterHslCheck(mDFlOGiD))\
           {\
               if ( M_FrHslIsLogPointEnabled(hDR, M_FrPrimeLogGetHslMdfId(mDFlOGiD)) )                 \
               {                                                              \
                  M_FrHsl7ByteMsg(hDR, M_FrPrimeLogGetHslMdfId(mDFlOGiD), \
                                aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7);                      \
               }\
           }

/** Write a 8 argument message to HSL (just a message header, no data).
  * Write a 8 argument message to HSL (just a message header, no data).
  * \param Int16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_6 macro.
  * \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param Int16 aRG1 = first argument value to log
  * \param Int16 aRG2 = second argument value to log
  * \param Int16 aRG3 = third argument value to log
  * \param Int16 aRG4 = fourth argument value to log
  * \param Int16 aRG5 = fifth argument value to log
  * \param Int16 aRG6 = sixth argument value to log
  * \param Int16 aRG7 = seventh argument value to log
  * \param Int16 aRG8 = eighth argument value to log
  */
# define M_FrHslPrintf8(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8)    \
           M_FrPrimeLogCheckInterface(mDFlOGiD);\
           if ( M_FrHslIsLogPointEnabled(hDR, mDFlOGiD) )                                               \
           {                                                                                            \
             M_FrHsl8ByteMsg(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8);            \
           }

/** Write a 9 argument message to HSL (just a message header, no data).
  * Write a 9 argument message to HSL (just a message header, no data).
  * \param Int16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_6 macro.
  * \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param Int16 aRG1 = first argument value to log
  * \param Int16 aRG2 = second argument value to log
  * \param Int16 aRG3 = third argument value to log
  * \param Int16 aRG4 = fourth argument value to log
  * \param Int16 aRG5 = fifth argument value to log
  * \param Int16 aRG6 = sixth argument value to log
  * \param Int16 aRG7 = seventh argument value to log
  * \param Int16 aRG8 = eighth argument value to log
  * \param Int16 aRG9 = nineth argument value to log
  */
# define M_FrHslPrintf9(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                      \
                                                     aRG9 )                                                               \
           if ( M_FrPrimeLogFilter(mDFlOGiD) )                       \
           {                                                         \
              M_FrPrimeLogMsg45(((Int32)hDR<<16)|(aRG1),((Int32)(aRG2)<<16)|(aRG3), \
                ((Int32)(aRG4)<<16)|(aRG5), ((Int32)(aRG6)<<16)|(aRG7), ((Int32)(aRG8)<<16)|(aRG9), 5); \
           }\
           if(M_FrPrimeLogFilterHslCheck(mDFlOGiD))\
           {\
               if ( M_FrHslIsLogPointEnabled(hDR, M_FrPrimeLogGetHslMdfId(mDFlOGiD)) )                 \
               {                                                              \
                  M_FrHsl9ByteMsg(hDR, M_FrPrimeLogGetHslMdfId(mDFlOGiD), \
                                                    aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                                         \
                                                     aRG9 );                                                              \
               }\
           }

/** Write a 10 argument message to HSL (just a message header, no data).
  * Write a 10 argument message to HSL (just a message header, no data).
  * \param Int16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_6 macro.
  * \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param Int16 aRG1 = first argument value to log
  * \param Int16 aRG2 = second argument value to log
  * \param Int16 aRG3 = third argument value to log
  * \param Int16 aRG4 = fourth argument value to log
  * \param Int16 aRG5 = fifth argument value to log
  * \param Int16 aRG6 = sixth argument value to log
  * \param Int16 aRG7 = seventh argument value to log
  * \param Int16 aRG8 = eighth argument value to log
  * \param Int16 aRG9 = nineth argument value to log
  * \param Int16 aRG10 = tenth argument value to log
  */
# define M_FrHslPrintf10(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                      aRG9, aRG10 )                                                       \
           M_FrPrimeLogCheckInterface(mDFlOGiD);\
           if ( M_FrHslIsLogPointEnabled(hDR, mDFlOGiD) )                                                                 \
           {                                                                                                              \
                                M_FrHsl10ByteMsg(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                     aRG9, aRG10 );                                                       \
           }

/** Write a 11 argument message to HSL (just a message header, no data).
  * Write a 11 argument message to HSL (just a message header, no data).
  * \param Int16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_6 macro.
  * \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param Int16 aRG1 = first argument value to log
  * \param Int16 aRG2 = second argument value to log
  * \param Int16 aRG3 = third argument value to log
  * \param Int16 aRG4 = fourth argument value to log
  * \param Int16 aRG5 = fifth argument value to log
  * \param Int16 aRG6 = sixth argument value to log
  * \param Int16 aRG7 = seventh argument value to log
  * \param Int16 aRG8 = eighth argument value to log
  * \param Int16 aRG9 = nineth argument value to log
  * \param Int16 aRG10 = tenth argument value to log
  * \param Int16 aRG11 = eleventh argument value to log
  */
# define M_FrHslPrintf11(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                      aRG9, aRG10, aRG11 )                                                \
           if ( M_FrHslIsLogPointEnabled(hDR, mDFlOGiD) )                                                                 \
           {                                                                                                              \
                                M_FrHsl11ByteMsg(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                     aRG9, aRG10, aRG11 );                                                \
           }

/** Write a 12 argument message to HSL (just a message header, no data).
  * Write a 12 argument message to HSL (just a message header, no data).
  * \param Int16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_6 macro.
  * \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param Int16 aRG1 = first argument value to log
  * \param Int16 aRG2 = second argument value to log
  * \param Int16 aRG3 = third argument value to log
  * \param Int16 aRG4 = fourth argument value to log
  * \param Int16 aRG5 = fifth argument value to log
  * \param Int16 aRG6 = sixth argument value to log
  * \param Int16 aRG7 = seventh argument value to log
  * \param Int16 aRG8 = eighth argument value to log
  * \param Int16 aRG9 = nineth argument value to log
  * \param Int16 aRG10 = tenth argument value to log
  * \param Int16 aRG11 = eleventh argument value to log
  * \param Int16 aRG12 = twelveth argument value to log
  */
# define M_FrHslPrintf12(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                      aRG9, aRG10, aRG11, aRG12 )                                         \
           M_FrPrimeLogCheckInterface(mDFlOGiD);\
           if ( M_FrHslIsLogPointEnabled(hDR, mDFlOGiD) )                                                                 \
           {                                                                                                              \
                                M_FrHsl12ByteMsg(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                     aRG9, aRG10, aRG11, aRG12 );                                         \
           }

/** Write a 13 argument message to HSL (just a message header, no data).
  * Write a 13 argument message to HSL (just a message header, no data).
  * \param Int16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_6 macro.
  * \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param Int16 aRG1 = first argument value to log
  * \param Int16 aRG2 = second argument value to log
  * \param Int16 aRG3 = third argument value to log
  * \param Int16 aRG4 = fourth argument value to log
  * \param Int16 aRG5 = fifth argument value to log
  * \param Int16 aRG6 = sixth argument value to log
  * \param Int16 aRG7 = seventh argument value to log
  * \param Int16 aRG8 = eighth argument value to log
  * \param Int16 aRG9 = nineth argument value to log
  * \param Int16 aRG10 = tenth argument value to log
  * \param Int16 aRG11 = eleventh argument value to log
  * \param Int16 aRG12 = twelveth argument value to log
  * \param Int16 aRG13 = thirteenth argument value to log
  */
# define M_FrHslPrintf13(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                      aRG9, aRG10, aRG11, aRG12, aRG13 )                                  \
           if ( M_FrHslIsLogPointEnabled(hDR, mDFlOGiD) )                                                                 \
           {                                                                                                              \
                                M_FrHsl13ByteMsg(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                     aRG9, aRG10, aRG11, aRG12, aRG13 );                                  \
           }

/** Write a 14 argument message to HSL (just a message header, no data).
  * Write a 14 argument message to HSL (just a message header, no data).
  * \param Int16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_6 macro.
  * \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param Int16 aRG1 = first argument value to log
  * \param Int16 aRG2 = second argument value to log
  * \param Int16 aRG3 = third argument value to log
  * \param Int16 aRG4 = fourth argument value to log
  * \param Int16 aRG5 = fifth argument value to log
  * \param Int16 aRG6 = sixth argument value to log
  * \param Int16 aRG7 = seventh argument value to log
  * \param Int16 aRG8 = eighth argument value to log
  * \param Int16 aRG9 = nineth argument value to log
  * \param Int16 aRG10 = tenth argument value to log
  * \param Int16 aRG11 = eleventh argument value to log
  * \param Int16 aRG12 = twelveth argument value to log
  * \param Int16 aRG13 = thirteenth argument value to log
  * \param Int16 aRG14 = fourteenth argument value to log
  */
# define M_FrHslPrintf14(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                      aRG9, aRG10, aRG11, aRG12, aRG13, aRG14 )                           \
           M_FrPrimeLogCheckInterface(mDFlOGiD);\
           if ( M_FrHslIsLogPointEnabled(hDR, mDFlOGiD) )                                                                 \
           {                                                                                                              \
                                M_FrHsl14ByteMsg(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                     aRG9, aRG10, aRG11, aRG12, aRG13, aRG14 );                           \
           }

/** Write a 15 argument message to HSL (just a message header, no data).
  * Write a 15 argument message to HSL (just a message header, no data).
  * \param Int16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_6 macro.
  * \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param Int16 aRG1 = first argument value to log
  * \param Int16 aRG2 = second argument value to log
  * \param Int16 aRG3 = third argument value to log
  * \param Int16 aRG4 = fourth argument value to log
  * \param Int16 aRG5 = fifth argument value to log
  * \param Int16 aRG6 = sixth argument value to log
  * \param Int16 aRG7 = seventh argument value to log
  * \param Int16 aRG8 = eighth argument value to log
  * \param Int16 aRG9 = nineth argument value to log
  * \param Int16 aRG10 = tenth argument value to log
  * \param Int16 aRG11 = eleventh argument value to log
  * \param Int16 aRG12 = twelveth argument value to log
  * \param Int16 aRG13 = thirteenth argument value to log
  * \param Int16 aRG14 = fourteenth argument value to log
  * \param Int16 aRG15 = fifteenth argument value to log
  */
# define M_FrHslPrintf15(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                      aRG9, aRG10, aRG11, aRG12, aRG13, aRG14, aRG15 )                    \
           if ( M_FrHslIsLogPointEnabled(hDR, mDFlOGiD) )                                                                 \
           {                                                                                                              \
                                M_FrHsl15ByteMsg(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                     aRG9, aRG10, aRG11, aRG12, aRG13, aRG14, aRG15 );                    \
           }

/** Write a 16 argument message to HSL (just a message header, no data).
  * Write a 16 argument message to HSL (just a message header, no data).
  * \param Int16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_6 macro.
  * \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param Int16 aRG1 = first argument value to log
  * \param Int16 aRG2 = second argument value to log
  * \param Int16 aRG3 = third argument value to log
  * \param Int16 aRG4 = fourth argument value to log
  * \param Int16 aRG5 = fifth argument value to log
  * \param Int16 aRG6 = sixth argument value to log
  * \param Int16 aRG7 = seventh argument value to log
  * \param Int16 aRG8 = eighth argument value to log
  * \param Int16 aRG9 = nineth argument value to log
  * \param Int16 aRG10 = tenth argument value to log
  * \param Int16 aRG11 = eleventh argument value to log
  * \param Int16 aRG12 = twelveth argument value to log
  * \param Int16 aRG13 = thirteenth argument value to log
  * \param Int16 aRG14 = fourteenth argument value to log
  * \param Int16 aRG15 = fifteenth argument value to log
  * \param Int16 aRG16 = sixteenth argument value to log
  */
# define M_FrHslPrintf16(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                      aRG9, aRG10, aRG11, aRG12, aRG13, aRG14, aRG15, aRG16 )             \
           M_FrPrimeLogCheckInterface(mDFlOGiD);\
           if ( M_FrHslIsLogPointEnabled(hDR, mDFlOGiD) )                                                                 \
           {                                                                                                              \
                                M_FrHsl16ByteMsg(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                     aRG9, aRG10, aRG11, aRG12, aRG13, aRG14, aRG15, aRG16 );             \
           }

#endif /* FR_HSL_PRINTF */


/** Write a message that is an array of Int16s.
 *  Write a message that is an array of Int16s. Equivalent to former LG_INT16_DATA macro.
 *  \param Int16 hDR = Message header word.
 *  \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
 *  \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
 *  \param Int16 lENGTH = Number of Int16s in the array.
 *  \param Int16 *pDATA = Array of lENGTH Int16s.
 */
#if defined(FR_HSL_PRINTF)
# define M_FrHslArray16(hDR, mDFlOGiD, fORMATsPECIFER, lENGTH, pDATA)                                   \
                  FrPcHslPrintf(fORMATsPECIFER);
#else
# define M_FrHslArray16(hDR, mDFlOGiD, fORMATsPECIFER, lENGTH, pDATA)                                   \
           if ( M_FrPrimeLogFilter(mDFlOGiD) )                       \
           {                                                         \
             M_FrPrimeLogArray16(hDR, lENGTH, pDATA );                      \
           }\
           if(M_FrPrimeLogFilterHslCheck(mDFlOGiD))\
           {\
               if ( M_FrHslIsLogPointEnabled(hDR, M_FrPrimeLogGetHslMdfId(mDFlOGiD)) )                                              \
               {                                                                                           \
                   M_FrHslInt16ArrayMsg((hDR), (lENGTH), (pDATA));                                         \
               }\
           }
#endif

/** Write a message that captures an instance of a structure.
 *  \param Int16 hDR = Message header word.
 *  \param Int16 gROUPiD Allows filtering per log point group.
 *  \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
 *  \param Type tYPE C/C++ type of this structure.
 *  \param Int16 *pARRAY = Pointer to structure instance to log.
 *  We specify an extra Int16 to log if there's an odd number of bytes in this structure.
 *  Back on the PC when we decode this structure, we have access to DWARF information for this
 *  which will allow to find out if there's an odd number of bytes or not.
 */
#define M_FrHslTypedef(hDR, gROUPiD, fORMATsPECIFER, tYPE, pARRAY)                            \
{                                                                                             \
  M_FrHslArray16((hDR), (gROUPiD), fORMATsPECIFER, (sizeof(tYPE)/sizeof(Int16)) + sizeof(tYPE)%sizeof(Int16), (pARRAY)); \
}


/** Write a message that is an array of Int16s from shared memory. Filter applied at run time.
 *  Write a message that is an array of Int16s from shared memory. Filter applied at run time.
 *  Equivalent to former LG_MES_6 macro. In PLF this is exactly the same as LG_INT16_DATA.
 *  \param Int16 hDR = Message header word.
 *  \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
 *  \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
 *  \param Int16 lENGTH = Number of Int16s in the array.
 *  \param Int16 *pDATA = Array of lENGTH Int16s.
 */
#if defined(FR_HSL_PRINTF)
# define M_FrHslShArray16(hDR, mDFlOGiD, fORMATsPECIFER, lENGTH, pDATA)                                 \
                  FrPcHslPrintf(fORMATsPECIFER);
#else
# define M_FrHslShArray16(hDR, mDFlOGiD, fORMATsPECIFER, lENGTH, pDATA)                                 \
            if ( M_FrHslIsLogPointEnabled(hDR, mDFlOGiD) )                                              \
            {                                                                                           \
                M_FrHslInt16ArrayMsg((hDR), (lENGTH), (pDATA));                                         \
            }
#endif

/**  Write a message that is an array of Int32s. Filter applied at run time.
 *   Write a message that is an array of Int32s. Filter applied at run time.
 *  \param Int16 hDR = Message header word.
 *   Equivalent to former LG_INT32_DATA macro.
 *  \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
 *  \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
 *  \param Int16 lENGTH = Number of Int32s in the array.
 *  \param Int32 *pDATA = Array of lENGTH Int32s.
 *  \param Int16 lASTwORDeMPTY = Logging indicator for the last 16 bit word - if set to 1 the last 16 bit word is not logged.
 */
#if defined(FR_HSL_PRINTF)
# define M_FrHslArray32(hDR, mDFlOGiD, fORMATsPECIFER, lENGTH, pDATA)                                   \
                  FrPcHslPrintf(fORMATsPECIFER);
#else
# define M_FrHslArray32(hDR, mDFlOGiD, fORMATsPECIFER, lENGTH, pDATA)                                   \
            if ( M_FrHslIsLogPointEnabled(hDR, mDFlOGiD) )                                              \
            {                                                                                           \
                M_FrHslInt32ArrayMsg((hDR), (lENGTH), (pDATA));                                         \
            }
#endif

#endif //(ULS_FUNCTION_PROFILER)

/** \def M_FrHslStopAcquisition()
 *  Send a specific (fixed) HSL log ID to automatically stop HSL probe acquisition
 *  note1: Under FR_HSL_PRINTF flags these macro are meanless
 */
#if defined(FR_HSL_PRINTF)
      # define M_FrHslStopAcquisition()
#else
      # define M_FrHslStopAcquisition()  M_FrHsl0ByteMsg(0xE, HSL_SYS | HslPriority_0)
#endif

/** @} */ /* End of addtogroup CfgCBEcorePLFservicesFrHsl */

/** Using Variadic operator to match any number of arguments. 
    this is supported in the gnu c99 standard which is used on nbiot project.
    This macro will provide a clearer message where file hasn't been parsed
    by silox.exe.  Where it is not defined, a compiler error will still be present. */    
#if (__STDC_VERSION__ >= 199901L)
#define M_FrHslPrintf(...) \
This log point appears in a new file which is not part of silo_db.sox; Must run silox.exe on this file.
#endif 


#endif /* FRHSLCFG_H */
/* END OF FILE */
