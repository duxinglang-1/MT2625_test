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
 * Header file for Voyager Multiplexer DTE Interface
 **************************************************************************/

#ifndef VGMXDTE_H
#define VGMXDTE_H

#include <cimux_sig.h>
#if defined (UPGRADE_NASMDL2)
#include <muxframe_typ.h>
#endif

/* pull in the correct platform code
 */
#if defined(ON_PC)
#  if !defined(VG_MUX_DTE_IMPLEMENTED)
#    include <vgmxdtpc.h>
#  endif
#else
#  include <vgmxdtecfg.h>   /* target platforms */
#endif

/*******************************************************/
/* Initialisation                                      */
/*******************************************************/

extern void vgmuxDteInitialise(void);

/*******************************************************/
/* Transmit                                            */
/*******************************************************/
#if !defined (UPGRADE_GENERIC_MUX)
/* Maximum size of a transmitted buffer */

#define VG_MUX_DTE_TX_BUFF_SIZE 256

/* The type for the transmit interrupt routine         */
/* Called in interrupt mode                            */

typedef Int16 (*VgmuxDteTxInterrupt) (
               Int8 buffer[VG_MUX_DTE_TX_BUFF_SIZE]);
#else
typedef Boolean (*VgmuxDteTxInterrupt) (
                 MuxFrame **txMuxFrame);
#endif
/* The variable which indicates whether a transmission */
/* is in progress                                      */

extern volatile Boolean vgmuxDteTxInProgress;

/* Routine to indicate that more output is available   */
/* Called from normal mode to indicate that additional */
/* transmit data is available. It is always safe to    */
/* call this routine even if a transmission is in      */
/* progress.                                           */

void vgmuxDteTxDataReadyReq(Boolean isInterrupt);

/* Routine to pause or restart transmission            */
/* This will stop the transfer of data but does not    */
/* affect any serial control lines                     */

void vgmuxDteTxPauseReq(Boolean pauseTx);

/*******************************************************/
/* Receive                                             */
/*******************************************************/

/* The macro used to retrieve the next character */
/* implementation will vary by hardware type     */

/* #define VG_MUX_DTE_RX_CHAR(bYTE) */

/* The macro used to get error information for the byte */

/* #define VG_MUX_DTE_RX_ERROR(bYTE) */


/* the following routine retrieves a pointer to up */
/* to maxChars characters received on the serial   */
/* line. NCH is set to the number of characters    */
/* returned and dATA_P is set to point to the first*/
/* such character. This pointer remains valid until*/
/* the next call to VG_MUX_DTE_RX_CHAR or          */
/* VG_MUX_DTE_RX_N_CHARS or the return of the RX   */
/* interrupt routine                               */

/* #define VG_MUX_DTE_RX_N_CHARS(dATA_P, nCH, mAXcHARS) */

/* the macro used to determine whether any unprocessed */
/* characters are in the buffer                        */

/* #define VG_MUX_DTE_RX_BUFFER_IS_EMPTY */

/* the interrupt routine which is called when data is  */
/* available on the UART. Called in Interrupt mode.    */

typedef void (*VgmuxDteRxInterrupt)(void);

/* Routine to pause reception of data. This will stop  */
/* any more data being received and will stop          */
/* the DTE transmitting data using the control lines   */
/* if hardware flow control is enabled                 */

void vgmuxDteRxPauseReq(Boolean pauseTx);

/* Set the number of received characters after which   */
/* a serial interrupt should be generated              */

/* ***** WARNING *****                                 */
/* Use of this routine will cause some unknown number  */
/* of characters to be lost on the serial line. You    */
/* should Restrict its use to occasions where this     */
/* does not cause a problem.                           */
/* ***** WARNING *****                                 */

void vgmuxDteRxCharacterWindowReq(Int8 window);

/* Force a Poll of the receive character buffer        */
/* This will cause the receive interrupt routine to    */
/* be called with any characters in the buffer         */

void vgmuxDteRxPollReq(void);

/* Routine to return a suitable receive window based */
/* on the current serial line speed                  */

Int8 vgmuxDteLineSpeedToWindow(void);

/*******************************************************/
/* Control Lines                                       */
/*******************************************************/
/* the type representing the serial line signal interrupt */

typedef void (*VgmuxDteSignalInterrupt)(void);

/*******************************************************/
/* Flow Control                                        */
/*******************************************************/

/* The flow control settings for the serial line          */
/* Treat as read only and set using vgmuxDteSetFlowControl*/
#if !defined (UPGRADE_GENERIC_MUX)

extern VgmxFlowCtrlInfo vgmuxDteFlowControl;

void vgmuxDteSetFlowControl(const VgmxFlowCtrlInfo *flowControl_p);
/*******************************************************/
/* Serial Line settings                                */
/*******************************************************/
/* The speed of the serial line                           */
/* Treat as read only and set using vgmuxDteSetPortRate   */

extern VgmxPortRate vgmuxDtePortRate;

void vgmuxDteSetPortRate(VgmxPortRate portRate);

/* the character format for the serial line               */
/* Treat as read only and set using vgmuxDteSetCharFormat */

extern VgmxCharFormatInfo vgmuxDteCharFormat;

void vgmuxDteSetCharFormat(const VgmxCharFormatInfo *charFormat_p);
#else
extern FlowCtrl vgmuxDteFlowControl;

void vgmuxDteSetFlowControl(const FlowCtrl *flowControl_p);

extern PortSpeed vgmuxDtePortRate;

void vgmuxDteSetPortRate(PortSpeed portSpeed);
#endif
/* indicator as to whether the serial line is enabled     */
/* Treat as read only and set using vgmuxDteEnable        */

extern Boolean vgmuxSerialLineIsEnabled;

void vgmuxDteEnable(Boolean enable);


/* used to set/read the autobaud flag from the null mux task
 */
void    vgmuxDteSetAutoBaudFlag( Boolean flagValue );
/* added for job101287 */
Boolean vgmuxDteGetAutoBaudFlag (void);



/*****************************************************************************
 * Function:    vgDteReconfigSerialPort
 *
 * Parameters:  None
 *
 * Returns:     Nothing
 *
 * Description: Re-Configure the serial port according to the current settings
 *
 ****************************************************************************/
void vgDteReconfigSerialPort(void);

/*******************************************************/
/* Error Handling                                      */
/*******************************************************/

/* type for the Error interrupt */

typedef void (*VgmuxDteErrorInterrupt)(void);

/*******************************************************/
/* Enabling and disabling Interrupts                   */
/*******************************************************/

/* enable or disable all serial line interrupts */

void vgmuxDteInterruptsReq(Boolean enableOn);

/* special version for TMM use */

void vgDsDteifTmmInterruptReq (Boolean enableOn);


/*******************************************************/
/* Defining Interrupt Routines                         */
/*******************************************************/

/* define call back routines for the serial line interface */

void vgmuxDteDefineRoutines(VgmuxDteRxInterrupt     rx,
                            VgmuxDteTxInterrupt     tx,
                            VgmuxDteSignalInterrupt signal,
                            VgmuxDteErrorInterrupt  error);


/*******************************************************/
/* Window values by line speed                         */
/*******************************************************/
#define VGMUX_RX_WINDOW_MAX     128

#define VGMUX_RX_WINDOW_921600  VGMUX_RX_WINDOW_MAX
#define VGMUX_RX_WINDOW_460800  VGMUX_RX_WINDOW_MAX
#define VGMUX_RX_WINDOW_230400  VGMUX_RX_WINDOW_MAX
#define VGMUX_RX_WINDOW_115200  VGMUX_RX_WINDOW_MAX
#define VGMUX_RX_WINDOW_57600   64
#define VGMUX_RX_WINDOW_38400   40
#define VGMUX_RX_WINDOW_28800   30
#define VGMUX_RX_WINDOW_19200   20
#define VGMUX_RX_WINDOW_14400   16
#define VGMUX_RX_WINDOW_9600    10
#define VGMUX_RX_WINDOW_4800    6
#define VGMUX_RX_WINDOW_2400    4
#define VGMUX_RX_WINDOW_1200    4
#define VGMUX_RX_WINDOW_300     4
#define VGMUX_RX_WINDOW_MIN     1


/*****************************************************************************/
/* Global Data                                                               */
/*****************************************************************************/

#endif /* !defined(VGMXDEFS_H) */

