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
 **************************************************************************
 * File Description
 * ----------------
 *
 * Header file for Voyager Multiplexer DTE Interface for the 430/R/no platform
 * Original code taken from vgmxdt30.h, rvmxdtrp.h and rvmxdtrp4.h
 *
 **************************************************************************/
#ifndef VGMXDTECFG_H
#define VGMXDTECFG_H

#include <system.h>

#include <plkmirq.h>     /* for ReenableIrq() */

/* make sure there is exactly one platform defined and pull in the
 * appropriate interrupt code
 */

#if defined ( TTC_PLATFORM_S9C ) || defined ( TTC_PLATFORM_S9M )
#  if !defined(VG_MUX_DTE_IMPLEMENTED)
#    include <dlcoreirq.h>
#    define VG_MUX_DTE_IMPLEMENTED
#  else
#    error "Multiple implementation of multiplexer DTE code"
#  endif
#endif

#if defined (TTC_PLATFORM_RPV4_MCU)
#  if !defined(VG_MUX_DTE_IMPLEMENTED)
#    define VG_MUX_DTE_IMPLEMENTED
#  else
#    error "Multiple implementation of multiplexer DTE code"
#  endif
#endif

#if defined (ENABLE_NO_PLATFORM)
#  if !defined (VG_MUX_DTE_IMPLEMENTED)
#    define VG_MUX_DTE_IMPLEMENTED
#  else
#    error "Multiple implementation of multiplexer DTE code"
#  endif
#endif /* End of #if defined (ENABLE_NO_PLATFORM) */

#include <dlspalif.h>    /* for SPAL interface definition */


/******************************************************************************/
/* Information about how the platform is configured                                */
/******************************************************************************/

/* These are used for internal 'hold' buffers. */
#define VG_MUX_DTE_DMA_TX_BUFF_SIZE 256
#define VG_MUX_DTE_DMA_RX_BUFF_SIZE 256

/******************************************************************************/
/* Global buffer pointers                                                     */
/******************************************************************************/

/* The Receive holding buffer */

#define VG_MUX_RX_HOLD_BUFFER_SIZE (VG_MUX_DTE_DMA_RX_BUFF_SIZE + VG_MUX_DTE_DMA_TX_BUFF_SIZE + 1)
extern Int8  vgmuxDmaRxHoldBuffer [VG_MUX_RX_HOLD_BUFFER_SIZE];

/* The number of characters in the receive holding buffer */
/* May only be modified either from the RX Interrupt or   */
/* when the RX Interrupt is disabled                      */

extern volatile Int16 vgmuxDmaRxNchars;

/* The next character to retrieve from the receive holding buffer */

extern volatile Int16 vgmuxDmaRxNextChar;

/* The variable which indicates whether a transmission */
/* is in progress                                      */

extern volatile Boolean vgmuxDteTxInProgress;

/******************************************************************************/
/* Disabling and enabling interrupts                                          */
/******************************************************************************/

#if !defined (TTC_PLATFORM_RPV4_MCU)
/* Disable all system interrupts
 */
#define VG_MUX_DTE_DISABLE_SYS_INTS   KiDisableInterrupts()
/* Enable all system interrupts
*/
#define VG_MUX_DTE_ENABLE_SYS_INTS    KiEnableInterrupts()
#endif
/******************************************************************************/
/* Reception of Data                                                          */
/******************************************************************************/

/* The macro used to retrieve the next character */
/* implementation will vary by hardware type     */

#define VG_MUX_DTE_RX_CHAR(bYTE) (bYTE = vgmuxDmaRxHoldBuffer[vgmuxDmaRxNextChar++])

/* the following routine retrieves a pointer to up */
/* to maxChars characters received on the serial   */
/* line. NCH is set to the number of characters    */
/* returned and dATA_P is set to point to the first*/
/* such character. This pointer remains valid until*/
/* the next call to VG_MUX_DTE_RX_CHAR or          */
/* VG_MUX_DTE_RX_N_CHARS or the return of the RX   */
/* interrupt routine                               */

#define VG_MUX_DTE_RX_N_CHARS(dATA_P, nCH, mAXcHARS)            \
        (nCH) = vgmuxDmaRxNchars - vgmuxDmaRxNextChar;          \
        if ((nCH) > (mAXcHARS))                                 \
        {                                                       \
          (nCH) = (mAXcHARS);                                   \
        }                                                       \
        (dATA_P) = &vgmuxDmaRxHoldBuffer[vgmuxDmaRxNextChar];   \
        vgmuxDmaRxNextChar += (nCH)

/* The macro used to get error information for the byte */

#define VG_MUX_DTE_RX_ERROR(bYTE) (bYTE) = 0

/* the macro used to determine whether any unprocessed */
/* characters are in the buffer                        */

#define VG_MUX_DTE_RX_BUFFER_IS_EMPTY (vgmuxDmaRxNextChar >= vgmuxDmaRxNchars)

/******************************************************************************/
/* Serial Lines                                                               */
/******************************************************************************/

/* A 'handle' to identify the link to the SPAL.
 * Not re-entrant, but then there is only one physical port so we won't be
 * opening more than one instance. It's only necessary to have this here
 * because of these macro's - an alternative would be to encapsulate this
 * all in vgmxdte.c
 */
extern DlSpalHandle  muxPortHandle;

/* Set the state of the RI line                       */
#define VG_MUX_DTE_SET_RI   DlSpalSetRi( muxPortHandle )
#define VG_MUX_DTE_CLEAR_RI DlSpalClearRi( muxPortHandle )

/* Set the state of the DCD line    */
/* DCD is not standardly connected  */
#define VG_MUX_DTE_SET_DCD   DlSpalSetDcd( muxPortHandle )
#define VG_MUX_DTE_CLEAR_DCD DlSpalClearDcd( muxPortHandle )

/* Set the state of the CTS line */
/* should not be used if hardware flow control is enabled */

#if defined (TTC_PLATFORM_RPV4_MCU)
#  define VG_MUX_DTE_SET_CTS    DlSpalClearCts( muxPortHandle )   /* Un-Pause */
#  define VG_MUX_DTE_CLEAR_CTS  DlSpalSetCts( muxPortHandle )     /* Pause    */
#else  /* TTC_PLATFORM_RPV4_MCU */
#  define VG_MUX_DTE_SET_CTS    DlSpalSetCts( muxPortHandle )     /* Un-pause */
#  define VG_MUX_DTE_CLEAR_CTS  DlSpalClearCts( muxPortHandle )   /* Pause    */
#endif /* TTC_PLATFORM_RPV4_MCU */

/* Set the state of the DSR line */
#define VG_MUX_DTE_SET_DSR
#define VG_MUX_DTE_CLEAR_DSR

/* Read the state of the DTR line */
#define VG_MUX_DTE_READ_DTR DlSpalGetDtr( muxPortHandle )

/* read the state of the RTS line                                  */
#define VG_MUX_DTE_READ_RTS   DlSpalGetRts( muxPortHandle )

/* break status */
#define VG_MUX_DTE_READ_BREAK   0
#define VG_MUX_DTE_SET_BREAK
#define VG_MUX_DTE_CLEAR_BREAK

/*****************************************************************************/
/* Global Data                                                               */
/*****************************************************************************/

Boolean vgDteifWasInterruptError( void );

#endif /* !VGMXDTECFG_H         */

