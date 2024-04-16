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
 ***************************************************************************
 * Device Layer - Serial Port Abstraction Layer - Interface header file.
 **************************************************************************/
/** \file
 * This file contains the public software interface for this layer. This is
 * platform-independent in concept but clients may adapt, say, the set of
 * abstract ports to suit their hardware and software.
 *
 *
 *           THIS CODE MUST BE KEPT PLATFORM INDEPENDENT
 *
 **************************************************************************/

/***************************************************************************
 * This defines a client-facing functional interface.
 ***************************************************************************/

#if defined (UPGRADE_NOT_FOR_RELEASE)
/***************************************************************************
 * This file forms part of the CMD Core Devices Product.
 * Changes should be reviewed by a relevant member of si_mmdevices.
 ***************************************************************************/
#endif

#ifndef DLSPALIF_H
#define DLSPALIF_H


/***************************************************************************
 * Essential Include Files
 ***************************************************************************/
#include <system.h>  /* for types. int16 etc */



/** \defgroup PrdDevicesSpalApi SPAL API
 * \ingroup PrdDevicesSpal
 * \ingroup Api
 * Serial Port Abstraction Layer (library) API.
 *
 * The SPAL provides a platform-independent API to serial ports.  It is
 * a thin 'dumb' library only intended to simplify application code and
 * provide portability of serial applications across platforms.
 *
 * It is 'dumb' in the sense that it does not attempt any intelligent policing
 * of the requests made of it.  The code is platform agnostic but
 * programmers using it needs to understand the limitations of the physical
 * hardware they are programming.
 * Attempts to use one device twice <b>will</b> be accepted.
 *
 * <hr>
 * <b> "Stacks" and "Ports" </b>
 *
 * The entities above and below the SPAL are termed 'Stack' and 'Port'
 * respectively.
 * <dl>
 * <dt> <b>Stack</b> </dt>
 * <dd> is the 'application' attaching to and using the SPALs virtual
 * port(s). Typically a data stack but doesn't have to be; 'Stack' is just a
 * convenient term to use here.
 * <dt> <b>Port</b> </dt>
 * <dd> is the physical connector (<b>not</b> the UART) and the
 * string of devices (probably including a UART) between the MCU and it.
 * e.g. In the case where one UART may drive one of two physical connectors (via
 * a multiplexer or similar) there are <b>two</b> Ports.
 * </dl>
 *
 * <hr>
 * <b> Platform independence and 'redundant' parameters </b>
 *
 * This interface definition is the generic superset of platform requirements
 * and thus may include items that are not relevant to the current platform
 * at the current time.
 *
 * For example, the interface allows for DMA to be specified for a platform
 * that does not support it. It is the responsibility of the programmer to
 * avoid such scenarios - not for the interface definition to exclude it.
 * In this way, interface changes are minimised as platforms develop.
 *
 */


/** \addtogroup PrdDevicesSpalApi
 * @{
 */

/** Port Identifiers.
 * This list is the superset of Ports that SPAL knows about.
 * In the event of needing to add ports this list may be extended and a
 * corresponding section in the device configuration (dlspal_<plat>.h) must
 * be added.
 *
 * It is not advisable to remove entries.
 */
typedef enum DlSpalPortIdTag
{
  SPAL_PORT_WIRE_1 = 0,  /**< First of any wired RS232 ports  */
                         /*   NB "=0" as enum is used as an index. */
  SPAL_PORT_WIRE_2,      /**< 2nd of any wired RS232 ports    */
  SPAL_PORT_WIRE_3,      /**< 3rd of any wired RS232 ports    */
  SPAL_PORT_WIRE_4,      /**< 4th of any wired RS232 ports    */
  SPAL_PORT_IRDA_1,      /**< First of any SIR IrDA ports     */
  SPAL_PORT_IRDA_2,      /**< 2nd IrDA port.
                          * Having two IrDA ports is an unlikely configuration.
                          * SPAL_PORT_IRDA_2 is more likely to be
                          * an alternate connection
                          * for the one IrDA port, especially where an IrDA
                          * device is a plug-on to an RS232 port. */

  SPAL_PORT_BLUETOOTH_1, /**< First of any Bluetooth ports     */
  /* Further entries go here */

  /* List must end with these: */
  SPAL_NUM_PORTS,        /**< Only used internally to the SPAL  */
  SPAL_NO_PORT           /**< Used for disconnection (ie route to 'no port')  */
}
DlSpalPortId;


/** Stack identfiers.
 * This list is the superset of Stacks that SPAL recognises.
 * This list may be extended safely as it only provides an identifier to share
 * between any resource manager and the stacks.
 *
 * It is not advisable to remove entries.
 */

typedef enum DlSpalStackIdTag
{
  SPAL_STACK_EMMI = 0,      /**< Genie  */
                            /*   NB "=0" as enum is used as an index. */
  SPAL_STACK_VGMUX,         /**< AT  */
  SPAL_STACK_IRDA,          /**< IrDA Stack  */
  SPAL_STACK_MAPAL,         /**< [disused]  */
  SPAL_STACK_BT,            /**< Bluetooth  */
  SPAL_STACK_DOM,           /**< Debug Output Module  */
  SPAL_STACK_RAMLOG,        /**< Ram Logger  */
  SPAL_STACK_GENERIC_DEBUG, /**< Informal debug during development  */
  /* Further entries go here */

  /* List must end with this: */
  SPAL_NUM_STACKS           /**< Only used internally to the SPAL  */
}
DlSpalStackId;

#if defined (UPGRADE_NOT_FOR_RELEASE)
  /*
   * SPAL_STACK_GENERIC_DEBUG is intended for use with short-term engineering
   * debugging where it is temporarily useful to print, stream, or whatever
   * bytestream data or text via a spare SPAL port.  A real example is when
   * debugging USB Genie and signals cannot be traced then simple print's
   * are sent out via the vacated Genie SPAL port to a PC with hyperterminal.
   * Example code to support such use can be found in usbstack.c - copy and
   * adapt that code if you want to use this route - but do use the common
   * SPAL_STACK_GENERIC_DEBUG identifier (to flag runtime conflicts, and
   * clutter).
   */
#endif


/** Baud rates. */
typedef enum DlSpalBaudRateTag
{
  SPAL_BAUD_1200,        /**< Baud rate (included for completeness - in practice 9600 is the slowest rate supported) */
  SPAL_BAUD_2400,        /**< Baud rate */
  SPAL_BAUD_4800,        /**< Baud rate */
  SPAL_BAUD_9600,        /**< Baud rate (typically 9600 the slowest rate supported) */
  SPAL_BAUD_14400,       /**< Baud rate */
  SPAL_BAUD_19200,       /**< Baud rate */
  SPAL_BAUD_28800,       /**< Baud rate */
  SPAL_BAUD_38400,       /**< Baud rate */
  SPAL_BAUD_57600,       /**< Baud rate (Rates above 56K typically require DMA support on the hardware) */
  SPAL_BAUD_76800,       /**< Baud rate */
  SPAL_BAUD_115200,      /**< Baud rate (115k is a typical 'moderate speed' rate. */
  SPAL_BAUD_230400,      /**< Baud rate (Rates in excess of 115k begin to require a significant proportion of the target device MIPs */
  SPAL_BAUD_460800,      /**< Baud rate */
  SPAL_BAUD_576000,      /**< Baud rate (These high Baud rates are related to MIR and FIR IrDA transports) */
  SPAL_BAUD_921600,      /**< Baud rate */
  SPAL_BAUD_1152000,     /**< Baud rate */
  SPAL_BAUD_4000000,     /**< Baud rate */
  SPAL_NUM_OF_BAUD_RATES /**< Counter of the number of Baud rate tokens */
}
DlSpalBaudRate;


/** Parity options. */
typedef enum DlSpalParityTag
{
  /** No parity */
  SPAL_NO_PARITY,
  /** Odd parity */
  SPAL_ODD_PARITY,
  /** Even parity */
  SPAL_EVEN_PARITY
}
DlSpalParity;


/** Routing Alternates (RX & TX).
 * For some target hardware there may be alternative ways of routing the
 * receive and transmit signals.  In order for the user to specify this these
 * generic codes are provided.
 *
 * This will be hardware dependent and some combinations
 * of routes and ports are may not be physically possible.
 */
typedef enum DlSpalRxTxRouteTag
{
  /** Normal case - whatever that relates to in the hardware */
  SPAL_RXTX_STD,
  /** First alternative routing */
  SPAL_RXTX_ALT_1,
  /** Second alternative routing */
  SPAL_RXTX_ALT_2
}
DlSpalRxTxRoute;


/** Routing Alternates (CTS).
 * For some target hardware there may be alternative ways of routing the
 * CTS signal.  In order for the user to specify this these
 * generic codes are provided.
 *
 * This will be hardware dependent and some combinations
 * of routes and ports are may not be physically possible.
 */
typedef enum DlSpalCtsRouteTag
{
  /** Normal case - whatever that relates to in the hardware */
  SPAL_CTS_STD,
  /** An alternative routing */
  SPAL_CTS_ALT_1
}
DlSpalCtsRoute;


/** Routing Alternates (DTR).
 * For some target hardware there may be alternative ways of routing the
 * DTR signal.  In order for the user to specify this these
 * generic codes are provided.
 *
 * This will be hardware dependent and some combinations
 * of routes and ports are may not be physically possible.
 */
typedef enum DlSpalDtrRouteTag
{
  /** Normal case - whatever that relates to in the hardware */
  SPAL_DTR_STD,
  /** An alternative routing */
  SPAL_DTR_ALT_1,
  /** A 'null' routing */
  SPAL_DTR_NOT_ROUTED
}
DlSpalDtrRoute;


/** Buffer mechanism.
 * The SPAL Ports may offer various options regarding buffering. These will
 * be from this set: None, DMA, FIFO.
 * */
typedef enum DlSpalBufferTypeTag
{
  SPAL_BUFF_NONE,   /**< No buffering - single bytes only. */
  SPAL_BUFF_DMA,    /**< Single characters are transferred by hardware into a memory buffer. */
  SPAL_BUFF_HW_FIFO /**< The UART hardware incorporates a short (eg 16 byte) buffer. */
}
DlSpalBufferType;


/** Buffering method.
 * "Buffering" can take many forms and, especially with DMA, may be used
 * in different ways. It is assumed that the buffer will
 * cause an interrupt when it reaches a certain threshold or trigger point.
 * This setting indicates how the <i>next</i> trigger point is determined
 * from one event to the next.
 *
 * The choice of which of these to use will be affected by the nature of the
 * protocol in use for the data being transferred.  If data is arriving in
 * deterministic packets of a known length then SET may be the better choice.
 * If the data flow is not packetised but an erratic ccontinuus flow, then
 * PUSHAHEAD is more suitable.
 *
 * <b>SET</b> requires the use of DlSpalSetNextRxInterrupt[Step]()
 *
 * <b>PUSHAHEAD</b> is useful when the data rate is quite high.
 * It may not be precise but will keep the trigger points at regular intervals.
 */
typedef enum DlSpalBufferIntMethodTag
{
  SPAL_BUFFINT_SET,      /**< Explicitly set an absolute trigger point */
  SPAL_BUFFINT_PUSHAHEAD /**< Reading the buffer advances the trigger point */
}
DlSpalBufferIntMethod;


/** Connection Specification.
 * The set of parameters that defines a connection, including serial port
 * basics (Baud rate, parity, etc.) and interrupt routine linkages.
 */
typedef struct DlSpalConnectionSpecTag
{
  DlSpalBaudRate  baudRate;  /**< Baud rate */
  DlSpalParity    parity;    /**< Odd, Even or No Parity setting */
  Int8            dataBits;  /**< Number of data bits (6 - 8) */
  Int8            stopBits;  /**< Number of stop bits (1 or 2) */

  DlSpalRxTxRoute routeRxTx; /**< Routing selection for the RxTx lines */

  Boolean        supportRtsCtsRi; /**< Indicates that the Port needs to support RTS, CTS, RI */
  Boolean        hardwareRts;     /**< Indicates that the RTS needs to be handled automatically */
  DlSpalCtsRoute routeCts;        /**< CTS routing option */

  Boolean        supportDtr;      /**< Indicates that the Port needs to support DTR */
  DlSpalDtrRoute routeDtr;        /**< DTR routing option */

  /* Interrupt callbacks
   * Not named Rx/Tx because that implies the RX/TX interrupts from the UART
   * whereas these are generically data-is-ready-so-read-it and
   * transmitter-is-empty-so-send-more-data. The whole point is that you
   * don't know what device is behind this.
   *
   * The 'status event' is typically an error but not exclusively so. The
   * low-level basics are handled by the ISR within SPAL (e.g. resetting the
   * interrupt bit in the port, etc.) but it may be that higher level code
   * wants to act on the information.  Therefore it is passed up as a set of
   * flags in the call-back function definition. See SPAL_EVENT_* defines below.
   *
   * A PORT has, generically, three int sources (Rx,Tx,Status). DTR is not
   * generic in this sense but needs to be presented in some way. It isn't
   * always well supported by hardware and so is handled as a special case...
   */

  /** Rx ISR callback.
   * Pointer to callback function, used when data is ready
   * to be processed in the incoming buffer. */
  void (*dataReadyIntCallback)  ( void );

  /** Tx ISR callback.
   * Pointer to callback function, used when data can be
   * added to the outgoing buffer */
  void (*dataSentIntCallback)   ( void );

  /** Status Event callback.
   * Pointer to callback function, used to manage control
   * events and errors occurring at the port (except DTR).
   * The parameter is a bitwise field made up of event flags from the
   * SPAL_EVENT_* set e.g. SPAL_EVENT_SIG_INT
   */
  void (*statusEventIntCallback)( Int16 eventSummary );

  /** DTR Callback.
   * Pointer to callback function, used to manage DTR activity */
  void (*specialDtrIntCallback) ( void );


  DlSpalBufferType      bufferType;       /**< NONE, DMA or FIFO */
  DlSpalBufferIntMethod bufferIntMethod;  /**< SET or PUSHAHEAD */

  /** The distance ahead to set the buffer trigger.
   * Primarily used for the _PUSHAHEAD method but also used as the
   * initial setting for the _SET method.
   */
  Int16                 bufferInterval;

}
DlSpalConnectionSpec;


/** Type to describe a 'location' in a data buffer. */
typedef Int32 DlSpalDataLocation;

/** App-to-SPAL connection handle. */
typedef Int8 DlSpalHandle;

/** Invalidity value for DlSpalHandle variables. */
#define SPAL_INVALID_HANDLE  SPAL_NUM_PORTS


/***************************************************************************
 * Define:       SPAL_EVENT_*
 * Interface:    SPAL Serial Port Interface
 * Description:
 * Bit-pattern codes indicating combination of error/status activity on
 * a port. Set by port interrupt ISR, used by higher level interrupt handling
 * functions. Any variables set using these code are only valid during the
 * interrupt handling phase.
 * The _ERROR case is an OR of the FE/BE/PE bits. It is present to support
 * existing code that didn't check which error had occurred.
 ***************************************************************************/
#define SPAL_EVENT_SIG_INT   0x01 /**< an [RTS|DTR] interrupt has just occurred */
#define SPAL_EVENT_SIG_STATE 0x02 /**< ie bit 1 is set high if [RTS|DTR] is high */
#define SPAL_EVENT_OVERRUN   0x04 /**< an overrun error has occurred */
#define SPAL_EVENT_ERROR     0x08 /**< a [frame|parity|break] error has occurred */
#define SPAL_EVENT_FE        0x10 /**<   a frame error has occurred  */
#define SPAL_EVENT_PE        0x20 /**<   a parity error has occurred */
#define SPAL_EVENT_BE        0x40 /**<   a break error has occurred  */




/***************************************************************************
 * Function Prototypes
 ***************************************************************************/

/** Initialise the SPAL device layer.
 *
 * This function is the normal safe initialisation function to prepare SPAL
 * for use at startup.  Provided SPAL is not already initialised it calls
 * DlSpalForceInitialise to perform the physical initialisation.
 *
 * \return This function has no return value.
 */
void DlSpalInitialise( void );


/***************************************************************************
 * DlSpalForceUpdateHwConfig
 *
 * Other information:
 *   In some condition, need to force configure the spal port
 *
 ***************************************************************************/
void DlSpalForceUpdateHwConfig ( DlSpalHandle h, DlSpalConnectionSpec spec );

/** Assign a Stack to a Port.
 *
 * This is used to assign stack X to physical connector (i.e. port) Y.  There
 * is no actual connection at this point but the information is retained until
 * the stack subsequently opens the port.
 *
 * Subsequent calls overwrite existing settings, and a call with the
 * Port identifier set to SPAL_NO_PORT will remove any previous assignment.
 * (The stack must close the connection prior to de-assignment).
 *
 * This function is designed to be called by a port manager module.
 * However, frequently the system does not included a central routing manager
 * so this function is called directy by the application requesting the
 * routing.  (This is the case for current TTPCom source code).
 *
 * It <b>must</b> be called before the Stack opens the Port.
 *
 * \return Returns TRUE if successful.
 */
Boolean DlSpalPortRouting(
  DlSpalStackId stack,          /**< [in] Identifier of data stack to be routed to a port */
  DlSpalPortId connection_point /**< [in] Identifier of that port */
  );


/** Open the link between the stack and it's assigned port.
 *
 * This function opens the Port previously assigned to the Stack.
 * The relevant Port was specified by DlSpalPortRouting.
 * A subsequent call to DlSpalPortConfiguration() is MANDATORY.
 *
 * The parameter is perhaps a little unexpected, but is a consequence of
 * allowing the routing function to be isolated from the Stack.
 *
 * The function returns a handle that is used in all subsequent calls to
 * identify the port to the SPAL layer.
 *
 * <b>NOTE:</b> The 'handle' that is returned implies a concept that
 * <i>should</i> allow for multiple connections
 * from the same stack (to different ports), but it does not.
 * This is because the stack-to-port
 * assignment and the link-open requests come from software modules
 * that don't share data and we are left with the 'stack ID' as the
 * only common key. Thus there is a 1:1 relationship between stack ID and
 * handle. The 'handle' mechanism is retained in case this design should ever
 * change, in which case the API changes will be minimised.
 *
 * \return Returns a 'handle' or SPAL_INVALID_HANDLE on failure.
 */
DlSpalHandle DlSpalOpenPortFor(
  DlSpalStackId stack_identifier /**< [in] Identity of the Stack making the call */
  );


/** Fill a local configuration structure with default data.
 *
 * This is a convenience function that sets the fields in the configuration
 * structure (to be passed to the port configuration function) to the
 * defaults set within SPAL.  This means that unused fields may be
 * safely ignored.
 *
 * \return This function has no return value.
 */
void DlSpalSetSpecToDefault(
  DlSpalConnectionSpec *spec /**< [out] Pointer to local structure to be filled with default values */
  );


/** Query SPAL for the port's level of buffer support.
 * This function returns the best buffering option available for the
 * specified link.
 *
 * \return Returns the best DlSpalBufferType buffering option
 */
DlSpalBufferType DlSpalQuerySupportedBuffering(
  DlSpalHandle h /**< [in] Handle identifying the Stack-Port connection */
  );


/** Configure (or Re-Configure) the port parameters.
 *
 * This configures, or re-configures, the port provided to the stack.
 * The configuration structure includes:
 * <ul>
 * <li> Baud rate
 * <li> Number of data bits, stop bits
 * <li> Parity on/off
 * <li> Control line support requirements (RTS, CTS, DTR, etc)
 * <li> Buffering parameters
 * <li> Call-backs for interrupts
 * </ul>
 *
 * \return This function has no return value.
 */
void DlSpalPortConfiguration(
    DlSpalHandle h,           /**< [in] Handle identifying the Stack-Port connection */
    DlSpalConnectionSpec spec /**< [in] Set of configuration parameters */
    );

/** Transmit from a stack's buffer.
 *
 * This function transmits (or takes for subsequent transmission) the data
 * pointed to by the caller.  The return value is how many bytes were actually
 * taken - this may be less than the ideal 'length' because of internal
 * bottlenecks etc.
 *
 * The data pointed to is copied within SPAL, either into a hardware-related
 * buffer (e.g. for DMA) or actually into the UART hardware (e.g a hardware
 * FIFO). Thus, when the function returns the data in the buffer may be discarded,
 * however, the caller must handle the case where not all of the data
 * is copied (the return value is less than 'length').
 *
 * \return Returns the number of bytes actually processed.
 */
Int16 DlSpalTxFromBuffer(
  DlSpalHandle h,        /**< [in] Handle identifying the Stack-Port connection */
  Int8 * data_start_ptr, /**< [in] Pointer to first byte of data */
  Int16 length           /**< [in] Number of bytes */
  );


/** Transfer received bytes into a stack's buffer.
 *
 * This function transfers data from the internal SPAL Rx buffer into the
 * memory buffer pointed to by the caller.  No more than max_length bytes
 * are copied.
 * The return value is how many bytes were actually
 * copied - this may be less than expected.
 *
 * \return Returns the number of bytes actually processed.
 */
Int16 DlSpalRxIntoBuffer (
  DlSpalHandle h,      /**< [in] Handle identifying the Stack-Port connection */
  Int8 * buffer_start, /**< [in] Pointer to first byte of buffer */
  Int16 max_length     /**< [in] Number of bytes free in buffer */
  );


/** Transfer received bytes into a stack's <i>circular</i> buffer.
 *
 * This function transfers data from the internal SPAL Rx buffer into the
 * memory buffer pointed to by the caller.  The stack's buffer in this case
 * is treated as circular.  The stack provide start, end, and current pointers
 * - once the end is reached the next byte is transferred to the start
 * point and so on.
 * The return value is how many bytes were actually
 * copied - this may be less than expected.
 *
 * \return Returns the number of bytes actually processed.
 */
Int16 DlSpalRxIntoCircularBuffer (
  DlSpalHandle h,   /**< [in] Handle identifying the Stack-Port connection */
  Int8 * dataPtr,   /**< [in] Pointer to 'next free' byte in buffer */
  Int8 * bufStart,  /**< [in] Pointer to first byte of buffer */
  Int8 * bufEnd     /**< [in] Pointer to last byte of buffer */
  );


/** Transmit one byte.
 *
 * Single-byte case of the generic transmit function.  Return is the same, for
 * consistency, but only having one byte to send it will be 1 on success or
 * 0 on failure. Note the data is passed by parameter, not a pointer as was
 * the case for the generic function.
 *
 * \return Returns the number of bytes actually processed.
 */
Int16 DlSpalTxByte(
  DlSpalHandle h, /**< [in] Handle identifying the Stack-Port connection */
  Int8 datum      /**< [in] Byte to be transmitted */
  );


/** Read one byte from the SPAL.
 *
 * Single-byte case of the generic receive function.  Return is the same for
 * consistency but only having one byte to send it will be 1 on success or
 * 0 on failure. Note the data is passed by pointer.
 *
 * \return Returns the number of bytes actually processed.
 */
Int16 DlSpalRxByte(
  DlSpalHandle h,  /**< [in] Handle identifying the Stack-Port connection */
  Int8 *datum      /**< [in] pointer to where byte should be copied */
  );


/** Set RI.
 * Sets the Ring Indication modem control line.
 * \return This function has no return value.
 */
void DlSpalSetRi(
  DlSpalHandle h /**< [in] Handle identifying the Stack-Port connection */
  );

/** Set DCD.
 * Sets the DCD modem control line.
 * \return This function has no return value.
 */
void DlSpalSetDcd(
  DlSpalHandle h /**< [in] Handle identifying the Stack-Port connection */
  );

/** Set CTS.
 * Sets the Clear To Send modem control line.
 * \return This function has no return value.
 */
void DlSpalSetCts(
  DlSpalHandle h /**< [in] Handle identifying the Stack-Port connection */
  );

/** Set DSR.
 * Sets the Data Set Ready modem control line.
 * \return This function has no return value.
 */
void DlSpalSetDsr(
  DlSpalHandle h /**< [in] Handle identifying the Stack-Port connection */
  );

/** Clear RI.
 * Clears the Ring Indication modem control line.
 * \return This function has no return value.
 */
void DlSpalClearRi(
  DlSpalHandle h /**< [in] Handle identifying the Stack-Port connection */
  );

/** Clear DCD.
 * Clears the DCD modem control line.
 * \return This function has no return value.
 */
void DlSpalClearDcd(
  DlSpalHandle h /**< [in] Handle identifying the Stack-Port connection */
  );

/** Clear CTS.
 * Clears the Clear To Send modem control line.
 * \return This function has no return value.
 */
void DlSpalClearCts(
  DlSpalHandle h /**< [in] Handle identifying the Stack-Port connection */
  );

/** Clear DSR.
 * Clears the Data Set Ready modem control line.
 * \return This function has no return value.
 */
void DlSpalClearDsr(
  DlSpalHandle h /**< [in] Handle identifying the Stack-Port connection */
  );


/** Read the state of DTR.
 * Reads the state of the input modem control line.
 * The return is a numeric 0/1, as opposed to a Boolean, for historic
 * reasons of backwards compatibility.
 * \return Returns 0 or 1 according to the state of the input signal.
 */
Int8 DlSpalGetDtr(
  DlSpalHandle h /**< [in] Handle identifying the Stack-Port connection */
  );

/** Read the state of RTS.
 * Reads the state of the input modem control line.
 * The return is a numeric 0/1, as opposed to a Boolean, for historic
 * reasons of backwards compatibility.
 * \return Returns 0 or 1 according to the state of the input signal.
 */
Int8 DlSpalGetRts(
  DlSpalHandle h /**< [in] Handle identifying the Stack-Port connection */
  );



/** Reset the Rx buffer.
 * Re-initialises the receive buffer - obviously only applicable to ports
 * that are buffered.
 *
 * \return This function has no return value.
 */
void DlSpalResetRxBuffer(
  DlSpalHandle h /**< [in] Handle identifying the Stack-Port connection */
  );

/** Set trigger point in Rx buffer.
 * Sets the point at which a new Rx interrupt should be generated - i.e. when
 * the incoming data reaches this point in the buffer. This location is in
 * the same units as that returned by DlSpalGetCurrentRxDataLocation().
 * Only applicable to ports that are buffered.
 *
 * \return This function has no return value.
 */
void DlSpalSetNextRxInterrupt(
  DlSpalHandle h, /**< [in] Handle identifying the Stack-Port connection */
  DlSpalDataLocation location /**< [in] Trigger point */
  );


/** Advance the trigger point in Rx buffer.
 * Sets the point at which a new Rx interrupt should be generated
 * <b>relative</b> to the current data location
 * (<i>not</i> relative to the last trigger point). This location is in
 * the same units as that returned by DlSpalGetCurrentRxDataLocation().
 * Only applicable to ports that are buffered.
 *
 * \return This function has no return value.
 */
void DlSpalSetNextRxInterruptStep(
  DlSpalHandle h, /**< [in] Handle identifying the Stack-Port connection */
  DlSpalDataLocation step  /**< [in] Increment in trigger point */
  );


/** Read the current position in Rx buffer.
 *
 * Returns the 'location' of the start of the data in the receive data
 * buffer.  This is useful to the caller if it subsequently wants to set
 * an interrupt to occur at a particular point further along in the buffer.
 * Only applicable to ports that are buffered.
 *
 * \return Returns the current position.
 */
DlSpalDataLocation DlSpalGetCurrentRxDataLocation(
  DlSpalHandle h /**< [in] Handle identifying the Stack-Port connection */
  );


/** Enable Tx interrupt.
 * Enables the transmit interrupt associated with this serial port data pipe.
 * \return This function has no return value.
 */
void DlSpalEnableTxBufferReadyInt(
  DlSpalHandle h /**< [in] Handle identifying the Stack-Port connection */
  );

/** Disable Tx interrupt.
 * Disables the transmit interrupt associated with this serial port data pipe.
 * \return This function has no return value.
 */
void DlSpalDisableTxBufferReadyInt(
  DlSpalHandle h /**< [in] Handle identifying the Stack-Port connection */
  );

/** Enable Rx interrupt chain.
 * Enables the receive interrupts associated with this serial port data pipe.
 * Depending on the hardware architecture there may be more than one
 * interrupt involved in the path from UART to MCU
 * (for example: UART-to-DMA and DMA-to-MCU).
 * This function affects all the interrupts in the chain.
 * \return This function has no return value.
 */
void DlSpalEnableRxDataReadyInt(
  DlSpalHandle h /**< [in] Handle identifying the Stack-Port connection */
  );

/** Disable Rx interrupt chain.
 * Disables the receive interrupts associated with this serial port data pipe.
 * Depending on the hardware architecture there may be more than one
 * interrupt involved in the path from UART to MCU
 * (for example: UART-to-DMA and DMA-to-MCU).
 * This function affects all the interrupts in the chain.
 * \return This function has no return value.
 */
void DlSpalDisableRxDataReadyInt(
  DlSpalHandle h /**< [in] Handle identifying the Stack-Port connection */
  );

/** Enable Rx-buffer interrupt.
 * Enables the receive interrupt as seen by the MCU only.
 * Any other interrupts that may be further down the chain are unaffected.
 *
 * \return This function has no return value.
 */
void DlSpalEnableRxBufferIntOnly(
  DlSpalHandle h /**< [in] Handle identifying the Stack-Port connection */
  );

/** Disable Rx-buffer interrupt.
 * Disables the receive interrupt as seen by the MCU only.
 * Any other interrupts that may be further down the chain are unaffected.
 * (e.g for a DMA buffered system this allows interrupts from the port to
 * trigger the DMA while, but the DMA can't interrupt the host).
 *
 * \return This function has no return value.
 */
void DlSpalDisableRxBufferIntOnly(
  DlSpalHandle h /**< [in] Handle identifying the Stack-Port connection */
  );


/** Enable status interrupt.
 * Enables status interrupts associated with this serial port data pipe.
 * (RTS, Parity/Framing errors etc.)
 *
 * \return This function has no return value.
 */
void DlSpalEnableStatusEventInt(
  DlSpalHandle h /**< [in] Handle identifying the Stack-Port connection */
  );

/** Disable status interrupt.
 * Disables status interrupts associated with this serial port data pipe.
 * (RTS, Parity/Framing errors etc.)
 *
 * \return This function has no return value.
 */
void DlSpalDisableStatusEventInt(
  DlSpalHandle h /**< [in] Handle identifying the Stack-Port connection */
  );

/** Enable DTR interrupt.
 * Enables DTR interrupts.
 * \return This function has no return value.
 */
void DlSpalEnableDtrInt(
  DlSpalHandle h /**< [in] Handle identifying the Stack-Port connection */
  );

/** Disable DTR interrupt.
 * Disables DTR interrupts.
 * \return This function has no return value.
 */
void DlSpalDisableDtrInt(
  DlSpalHandle h /**< [in] Handle identifying the Stack-Port connection */
  );


/** Close the link between the stack and it's assigned port.
 *
 * This function shuts down the link. Any further use needs a call to
 * DlSpalOpenPortFor() and a complete re-initialisation.
 *
 * <b>Note:</b> The port cannot be re-used by another application until it
 * has also been de-routed, as well as closed.
 * Until then, this stack is still assigned the port - it just isn't active.
 *
 * \return This function has no return value.
 */
void DlSpalClosePort(
  DlSpalHandle h /**< [in] Handle identifying the Stack-Port connection */
  );


#if !defined (ON_PC)
/** Forcibly initialise the SPAL device layer.
 *
 * This function is usually once called internally by DlSpalInitialise()
 * on startup to initialise SPAL.
 * However, this function can be called externally
 * <b>for debugging purposes only</b>.
 * This is so that a debug application can force the use
 * of a specific SPAL port without needing to know about existing applications
 * that are currently using that port.
 * It should be expected that applications will crash if this is used.
 *
 * \return This function has no return value.
 */
void DlSpalForceInitialise( void );
#endif

/** Abort SPAL to enable High Speed Logging.
 *
 * This is a debug-only function used to move SPAL off the hardware
 * required for diagnostic High Speed Logging.
 * This disables interupts etc. effectively removing the port from
 * under the stack (it won't know about this, but without interrupts
 * it won't be doing anything).
 *
 * It should be expected that applications may crash if this is used.
 *
 * <b>One shot and NOT recoverable. </b>
 *
 * \return This function has no return value.
 */
void DlSpalOverrideForLogging( void );


/** Disable all interrupts. 
 * 
 * 
 */
void DlSpalDisableInt( DlSpalHandle h );

 /** @} */ /* End of Doxygen group */

#endif /* DLSPALIF_H */

/* END OF FILE */
