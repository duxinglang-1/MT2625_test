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
 * Middleware Type definitions for the CONN Interface.
 **************************************************************************/

#ifndef     CONN_TYP_H
#define     CONN_TYP_H

/***************************************************************************
 * Nested Include Files
 ***************************************************************************/
#include <system.h>

/** \addtogroup SigConn
 *
 * @{
 */
/***************************************************************************
 *   Manifest Constants
 ***************************************************************************/

/***************************************************************************
 *   Macro Functions
 ***************************************************************************/

/***************************************************************************
 *   Enumeration Types
 ***************************************************************************/
/** Indicates the port type.
 * This enumeration type indicates the way data and control information are exchanged on the port.
 * Here "data" usually refers to some sort of IP packet exchange (e.g. PPP) and "control" usually
 * refers to AT command exchange.
 */
typedef enum PortTypeTag
{
    /** The port consists of a data channel with associated control channel. */
    PORT_SEPARATE_DATA_AND_CTRL = 0,

    /** The port consists of a single channel which is used for both, data and control. */
    PORT_SINGLE = 1,

    /** Number of different port types */
    NUMBER_OF_PORT_TYPES = 2
} PortType;



/** Indicates the transmission rate.
 * This enumeration type indicates the current or the requested transmission rate of a channel.
 * The type is used by the CONN interface as well as the CIMUX interface.
 * \note In case the Connection Layer drives a USB port this setting is ignored by the Connection
 * Layer. In this case the Connection Layer will report \a PORTSPEED_AUTO in the initial
 * \link #ConnInitIndTag ConnInitInd \endlink signal.
 * \note In case this type is used on the CIMUX interface and it applies to a logical 27.010
 * multiplexer channel then the setting is ignored. MUX will
 * report the current port speed of the Connection Layer in the initial \link
 * #CiMuxChannelEnableIndTag CiMuxChannelEnableInd \endlink signal.
 */
typedef enum PortSpeedTag
{
    /** Automatic detection of transmission rate. */
    PORTSPEED_AUTO                  = 0,

    /** Transmission rate of110bit/s. */
    PORTSPEED_110                  = 1,

    /** Transmission rate of 300bit/s. */
    PORTSPEED_300                 = 2,

    /** Transmission rate of 1200bit/s. */
    PORTSPEED_1200                 = 3,

    /** Transmission rate of 2400bit/s. */
    PORTSPEED_2400                 = 4,

    /** Transmission rate of 4800bit/s. */
    PORTSPEED_4800                = 5,

    /** Transmission rate of 9600bit/s. */
    PORTSPEED_9600                  = 6,

    /** Transmission rate of 19200bit/s. */
    PORTSPEED_19200                 = 7,

    /** Transmission rate of 38400bit/s. */
    PORTSPEED_38400                 = 8,

    /** Transmission rate of 57600bit/s. */
    PORTSPEED_57600                 = 9,

    /** Transmission rate of 115200bit/s. */
    PORTSPEED_115200                = 10,

    /** Transmission rate of 230400bit/s. */
    PORTSPEED_230400                = 11,

    /** Transmission rate of 460800bit/s. */
    PORTSPEED_460800                = 12,

    /** Transmission rate of 921600bit/s. */
    PORTSPEED_921600                = 13,

    /** Maximum number of baud rates supported */
    MAX_NUM_PORTSPEED
} PortSpeed;



/** Indicates the flow control type.
 * This enumeration type indicates the current or the requested flow control type of a channel.
 * The flow control type can be defined separately for uplink and downlink.
 * The type is used by the CONN interface as well as the CIMUX interface.
 * \note In case the Connection Layer drives a USB port this setting is ignored by the Connection
 * Layer. The Connection Layer will always use the USB internal flow control mechanism and report
 * \a FC_RTS_CTS in the \link #ConnInitIndTag ConnInitInd \endlink signal.
 * \note In case the Connection Layer drives a UART port and uplink flow control is set to
 * \a FC_NONE the Connection Layer may discard received data if MUX is not fast enough
 * to process the data. However, the Connection Layer must be able to receive data for at least
 * 100ms before it discards data in case MUX does not send a flow control signal
 * \link #ConnRxDataRspTag ConnRxDataRsp \endlink in time.
 *   or
 * 100ms before it discards data in case MUX does not process data in time. (if defined (UPGRADE_CONN_MDT_NB))
 */
typedef enum FlowCtrlTypeTag
{
    /** No flow control is performed. */
    FC_NONE = 0,

    /** Flow control is performed via flow control characters 0x11=XON/0x13=XOFF (in-band flow
     * control).
     * MUX filters out received flow control characters and does not pass them on.
     * \note This setting is only used on the CIMUX interface.
     */
    FC_XON_XOFF_NO_PASS = 1,

    /** Flow control is performed via the serial lines RTS/CTS (out-band flow control). */
    FC_RTS_CTS = 2,
    
    /** Number of different flow control methods available */
    NUMBER_OF_FC_METHODS = 3
} FlowCtrlType;

/***************************************************************************
 *   Structure Types
 ***************************************************************************/
/** Defines the flow control settings of a channel.
 * This structure contains the current or the requested flow control settings of a channel for
 * uplink and downlink.
 * The type is used by the CONN interface as well as the CIMUX interface.
 */
typedef struct FlowCtrlTag
{
    /** Flow Control settings in Uplink direction. */
    FlowCtrlType                uplink;

    /** Flow Control settings in Downlink direction. */
    FlowCtrlType                downlink;
} FlowCtrl;



/** Defines the communication port settings of a channel.
 * This structure contains the communication port settings.
 * The type is used by the CONN interface as well as the CIMUX interface.
 * If this type is used on the CIMUX interface and it applies to a multiplexer instance which runs
 * the 27.010 multiplexer protocol then these settings apply to a logical channel rather than the
 * Connection Level.
 * \note The character format cannot be changed and is fixed to 8 data bits/no parity bit/1 stop bit.
 */
typedef struct ComPortSettingsTag
{
    /** Transmission rate. */
    PortSpeed                   portSpeed;

    /** Flow Control settings. */
    FlowCtrl                    flowCtrl;
} ComPortSettings;



/** Uplink line status.
 * This structure contains the line status received from the peer host.
 * \note Break signal is not supported by this interface.
 */
typedef struct UlLineStatusTag
{
    /** DTR line status.
     * The DTR (Data Terminal Ready) line status (TRUE=ON, FALSE=OFF).
     */
    Boolean                     dtrOn;

    /** RTS line status.
     * The RTS (Ready To Send) line status (TRUE=ON, FALSE=OFF).
     */
    Boolean                     rtsOn;
} UlLineStatus;



/** Downlink line status.
 * This structure contains the line status transmitted to the peer host.
 * \note Break signal is not supported by this interface.
 */
typedef struct DlLineStatusTag
{
    /** DSR line status.
     * The DSR (Data Set Ready) line status (TRUE=ON, FALSE=OFF).
     */
    Boolean                     dsrOn;

    /** CTS line status.
     * The CTS (Clear To Send) line status (TRUE=ON, FALSE=OFF).
     */
    Boolean                     ctsOn;

    /** RI line status.
     * The RI (Ring Indicator) line status (TRUE=ON, FALSE=OFF).
     */
    Boolean                     riOn;

    /** DCD line status.
     * The DCD (Data Carrier Detect) line status (TRUE=ON, FALSE=OFF).
     */
    Boolean                     dcdOn;
} DlLineStatus;

/** @} */ /* End of SigConn group */

#endif /* !CONN_TYP_H */

/* End of conn_typ.h */


