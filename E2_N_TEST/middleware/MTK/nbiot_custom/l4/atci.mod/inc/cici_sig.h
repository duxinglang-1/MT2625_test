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
 * Signal definitions for signals sent by the CI to itself.
 **************************************************************************/

#ifndef CICI_SIG_H
#define CICI_SIG_H

/** \defgroup SigCiex CIEX Signalling Interface
 * \ingroup Sig
 * \ingroup PrdMW
 * CIEX signalling interface.
 * This section covers the:<UL>
 * <LI> CI-CI Signalling Interface
 * <LI> CI-MUX Signalling Interface
 * <LI> CI-RTFDP Signalling Interface
 * </UL>
 * The context of CIEX is shown in the Diagram below.
 *
 * \image html NOCI_Interface.png
 * @{
 */



/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <rvcrdata.h>
#include <rvomdata.h>
#include <rvsystem.h>
#if defined (UPGRADE_GENERIC_MUX)
#include <cimux_typ.h>
#else
#include <vgmx_typ.h>
#endif
#include <rvmxdata.h>
#include <rvcfndata.h>
/** \defgroup SigCiCi CI-CI Signalling Interface
 * \ingroup Sig
 * \ingroup SigCiex
 * Internal CI Task signals that are sent from/to the CI Task only.
 * These signals are used to expose the inner working of the CI Task, such as
 * the current running AT command and the current data entry mode.
 *
 * @{
 */
/***************************************************************************
 * Manifest Constants
 **************************************************************************/
#if defined (UPGRADE_NASMDL2)
#if defined (ENABLE_LONG_AT_CMD_RSP)
#define VG_MAX_AT_DATA_IN_SIGNAL_LENGTH  (500)
                                        /**< The maximum length of data that can be sent to the
                                         * Command Response Manager in the signal #CirmDataInd. */

#define VG_PRINTF_CONV_BUFFER     (COMMAND_LINE_SIZE) /* 4200 */
                                          /**< Max temporary buffer length to hold strings which are
                                            * constructed for AT response or unsolicited event output 
                                            * to the Command Response Manager */
#else
#define VG_MAX_AT_DATA_IN_SIGNAL_LENGTH  (75)
                                        /**< The maximum length of data that can be sent to the
                                         * Command Response Manager in the signal #CirmDataInd. */

#define VG_PRINTF_CONV_BUFFER     (VG_MAX_AT_DATA_IN_SIGNAL_LENGTH * 8)
                                          /**< Max temporary buffer length to hold strings which are
                                            * constructed for AT response or unsolicited event output 
                                            * to the Command Response Manager */
    
#endif

#define SOLICITED_MAX_NUM_ITEMS   (20)  /**< The maximum number of signals that an entity can
                                         * register to receive at any time. */
#endif /* UPGRADE_NASMDL2 */

/***************************************************************************
 * Typed Constants
 **************************************************************************/

/***************************************************************************
 * Type Definitions
 **************************************************************************/

/** Class of Tranmission Data.
 * This enumeration is used in the #CirmDataInd signal to specify the type of
 * of data present. When received by the Command Response Manager the data is
 * processed according to the transmission data class. */
typedef enum TxDataClassTag
{
  ECHO_DATA,         /**< AT command echo data, the command Response Manager
                      * sends the data directly on to the multiplexer if echo
                      * is enabled. */
  DATA,              /**< AT command data, the command Response Manager sends
                      * the data directly on to the multiplexer. */
  RESULT_CODE,       /**< Result code type, the command Response Manager creates
                      * a result code string based on the current result code
                      * formatting configuration. The manager sends another
                      * CirmDataInd signal containing the actual data to be
                      * transmitted. */
  LINE_DATA          /**< Assigns a data channel for GSM or GPRS data
                      * connections (specified by pendingConnection value). */
} TxDataClass_t;

/** Run AT command indication.
 * This signal is used to indicate the current AT command string that is being
 * processed by the CI Task. When a carriage return character is detected on
 * an AT command channel the AT prefix is removed and the resulting string is
 * ready to process. This signal is sent to the CI Task in order to initiate
 * processing of the command string. The string is then compared to all
 * supported AT commands and when/if matched the relevant actions are
 * performed. Any concatenated AT commands will also be included in the
 * contents of this signal. If another AT command is detected in this command
 * string then this signal is sent again.
 */
typedef struct CiRunAtCommandIndTag
{
  VgmuxChannelNumber entity;           /**< The AT command channel entity which is processing an
                                        * AT command string. */
  ShortCommandLine_t activeAtCommand;  /**< The AT command string that will be processed when the CI
                                        * Task receives this signal.  Truncated to 200 bytes */
} CiRunAtCommandInd;

/** Empty Transmission Cache indication.
 * The CI Task has been informed by the active multiplexer, through the
 * #VgmuxXonInd signal, to start sending it data once again. On receipt of
 * this signal the CI Task will send all data stored in it's transmission
 * cache.
 */
typedef struct CiEmptyTxCacheIndTag
{
  VgmuxChannelNumber  entity;  /**< The multiplexer channel whose data transmission cache is to be
                                * sent. */
} CiEmptyTxCacheInd;

/** Data Entry Mode indication.
 * This signal is used by the CI Task to switch between data entry modes. For
 * example after entering an AT command to send an SMS, we are prompted to
 * enter a message. The CI Task on receipt of this signal treats any data
 * entered on this channel as SMS message content.
 */
typedef struct CiDataEntryModeIndTag
{
  VgDataEntryMode    dataEntryMode;  /**< The data entry mode to switch to. */
  VgmuxChannelNumber entity;         /**< The AT command channel entity on which to apply a change
                                      * in data entry mode. */
} CiDataEntryModeInd;

/** User Profile loaded indication.
 * This signal is used by the CI Task to notify User profile data has been loaded from NVRAM or reset to defaults
 * for the given entity channel to all ATCI sub-system.
 */
typedef struct CiUserProfLoadedIndTag
{
  VgmuxChannelNumber  entity;  /**< The multiplexer channel on which User profile data has been loaded. */
} CiUserProfLoadedInd;


/** Solicited signal Registration indication.
 * This signal is used by the CI Task to update it's list of signals that it
 * expected to receive for a particular channel. For example, if an AT
 * command sends a request signal and expects a confirmation signal then this
 * signal is used inform the CI Task of the confirmation signal.
 */
typedef struct VgCiSsRegistrationIndTag
{
  VgmuxChannelNumber   entity;   /**< The multiplexer channel which is expecting the specified
                                  * signal. */
  VoyagerSubsystems_t  ssCode;   /**< The CI Task sub-system that should process the specified
                                  * signal. */
  SignalId             signalId; /**< The signal ID for the expected signal. */
} VgCiSsRegistrationInd;

/** Response Manager Data Indication.
 * This signal is used by the CI Task to send data to the command Response
 * Manager. Depending on the class of transmission data specified the
 * Response Manager will take appropriate action.
 */
typedef struct CirmDataIndTag
{
  VgmuxChannelNumber  channelNumber;      /**< The multiplexer channel for which the data is
                                           * intended for. */
  Boolean             postfix;            /**< TRUE is a <cr><lf> postfix is required for the data.
                                           */
  Boolean             prefix;             /**< TRUE is a <cr><lf> prefix is required for the data.
                                           */
  ResultCode_t        resultCode;         /**< The result code to transmit in enumerated form. */
  ConnectionClass_t   pendingConnection;  /**< TRUE if a data session is to be established after
                                           * transmitting the specified data. The manager will
                                           * send another CirmDataInd signal containing line data
                                           * for the data session using the details provided in this
                                           * signal. */
  Boolean             taskInitiated;      /**< TRUE if the data contained in the signal has not
                                           * been generated by the user. This is used when a WAP CSD
                                           * connection has been established by request of another
                                           * task. The associated result codes generated when
                                           * establishing this connection must not be transmitted
                                           * and so this flag is used to note that they must be
                                           * intercepted.
                                           */
  CommandId_t         commandId;          /**< The AT command (if any) that was running when the
                                           * data in this signal was generated. */
  TxDataClass_t       classOfData;        /**< The class of data contained within the signal. */
  Boolean             forceTransmit;      /**< Fource output of string regardless of current mode of channel. */
  Int16               length;             /**< The length of data supplied in the data field. 
\assoc ARRAY \ref data */
                                          /** The actual data to be transmitted (if the class of
                                           * data is ECHO_DATA, DATA). */
  Int8                data [VG_MAX_AT_DATA_IN_SIGNAL_LENGTH];
  Boolean             isUrc;              /**< Indicates if data is URC */
  Boolean             isSmsUrc;           /**< Indicates if data is SMS URC */

} CirmDataInd; 

/** Signal for CI to send to itself when CFUN state changes so it can be caught by other subsystems
 * so that they are aware of the state change and can initialise any protocol stack relevant parameters.
 */
typedef struct CiCfunSetIndTag
{
  VgmuxChannelNumber      entity;    /**< The active multiplexer channel */
  VgCFUNType              cfunMode; /**< current cfun state */
} CiCfunSetInd;

#if defined(UPGRADE_MTNET)
typedef struct CiResetIndTag
{
  VgmuxChannelNumber      entity;    /**< The active multiplexer channel */
} CiResetInd;
#endif

#if defined (ENABLE_L23_DEBUG)
/** Signal for CI to send the debug information 
 */
typedef struct CiDebugIndTag
{
  SignalId                receiveSigType;   /**< The Signal type received */
  Int32                   requestStatus;    /**< The SM ready state */
  Boolean                 unsolicitedChannelBusy;   /**< The unsolicitedChannel state */
} CiDebugInd;
#endif

#if defined (ENABLE_ATCI_UNIT_TEST)


/**
 * This signal is used to request ATCI to do deep sleep preparations
 * or do deep sleep wake-up tasks
 */
typedef struct CiUnitTestSleepModeIndTag
{
  Boolean          wakeUp;
}
CiUnitTestSleepModeInd; 

/**
 * This signal is used as response to Sleep mode request 
 with information on success/failure of request
 */
typedef struct CiUnitTestSleepModeRspTag
{
  Boolean          success;
 /* ToDo: add data to be able to check content */ 
}
CiUnitTestSleepModeRsp; 

#endif
/***************************************************************************
 *  Macros
 **************************************************************************/

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

/** @} */ /* End of SigCiCi group */
/** @} */ /* End of SigCiex group */

#endif /* CICI_SIG_H */

