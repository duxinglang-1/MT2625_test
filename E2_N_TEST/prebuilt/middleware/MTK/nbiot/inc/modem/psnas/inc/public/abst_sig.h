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
 *  File Description :                                                   */
/** \file
 *      SIM Application Toolkit APEX interface
 **************************************************************************/

#ifndef ABST_SIG_H
#define ABST_SIG_H

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/
#if !defined (ABST_TYP_H)
#  include <abst_typ.h>
#endif

#if !defined (ALSA_SIG_H)
#  include <alsa_sig.h>
#endif

#if !defined (ALSI_SIG_H)
#  include <alsi_sig.h>
#endif

# if !defined (ALSA_SIG_H)
#   include <alsa_sig.h>
# endif

#  include <abpd_sig.h>

/** \defgroup SigApexSt  ABST Signalling Interface
 * \ingroup NasApex
 * \ingroup Sig
 * ABST Signalling Interface is the interface between the SIMAT Task
 * and SAT Procedure in the Application Background Layer.
 * @{
 */
/****************************************************************************
 * Macros
 ****************************************************************************/
/** Defines the body of the ApexStCcStatusMessage in relation to Call Control.*/
typedef struct SimatCcStatusIndMessageTag
{
  /** Unique command reference */
  Int32                       simatCommandRef;
  /** Indicates the status to allow each peer entity to get into the correct state */
  SimatCcStatus               status;
  /** Defines the type of any response data returned */
  SimatCallControlInfoType    type;
} SimatCcStatusIndMessage;


/** This is the message body of the signal sent to request the default
 * terminal profile from the SIMAT Task. Contains the command reference
 * of requested signal.
 */
typedef struct ApexStConfigureReqMessageTag
{
  /** Unique command reference */
  Int32                       simatCommandRef;
} ApexStConfigureReqMessage;

/** Determines the outcome of the ApexStConfigureReq
 * and if the response is positive this structure contains the
 * terminal profile supported by the ME
 */
typedef struct ApexStConfigureCnfMessageTag
{
  /** Unique command reference */
  Int32                       simatCommandRef;
  /** Indicates the result of checking terminal profile */
  SimatTerminalProfileCheck   result;
  /** The list of USAT facilities that are supported by the ME.*/
  SimTerminalProfile          terminalProfile;
} ApexStConfigureCnfMessage;

/** Contains the call set up details.
 * This is the message body of the signal sent by the background
 * layer containing details of a call to be set-up by the SIMAT task.
 */
typedef struct ApexStSetUpCallIndMessageTag
{
  /** Command reference */
  Int32                       simatCommandRef;
  /** Call set-up details  */
  SimatCcCallSetUp            ccCallSetUp;
  /** Indicates if alpha Id is present
\assoc PRESENT \ref alphaId */
  Boolean                     alphaIdPresent;
  /** Alpha identifier for the proactive command */
  SimatAlphaIdentifier        alphaId;
} ApexStSetUpCallIndMessage;

/** Determines whether call set up was successful or if it failed.*/
typedef struct ApexStSetUpCallRspMessageTag
{
  /** Command reference */
  Int32                       simatCommandRef;
  /** Determines the outcome of the call set up request */
  SimatCcResult               result;
} ApexStSetUpCallRspMessage;

/** Contains the SS set up details.
 * This is the message body of the signal sent from the background
 * layer containing details of an SS operation to be sent.
 */
typedef struct ApexStSetUpSsIndMessageTag
{
  /** Command reference */
  Int32                       simatCommandRef;
  /** Details of SS operation  */
  SimatCcSsOperation          ccSsOperation;
  /** Alpha identifier for the proactive command */
  SimatAlphaIdentifier        alphaId;
  /** Indicates if alpha Id is present */
  Boolean                     alphaIdPresent;
} ApexStSetUpSsIndMessage;


/** Determines whether SS set-up was successful or if it failed.*/
typedef struct ApexStSetUpSsRspMessageTag
{
  /** Command reference */
  Int32                       simatCommandRef;
  /** Determines the outcome of the SS set up request */
  SimatCcResult               result;
} ApexStSetUpSsRspMessage;

/** Contains the USSD set up details.
 * This is the message body of the signal sent from background layer
 * containing details of a USSD to be sent.
 */
typedef struct ApexStSetUpUssdIndMessageTag
{
  /** Command reference */
  Int32                       simatCommandRef;
  /** Call control set-up USSD procedure */
  SimatCcUssdOperation        ccUssdOperation;
   /** Alpha identifier for the proactive command */
  SimatAlphaIdentifier        alphaId;
  /** Indicates if alpha Id is present */
  Boolean                     alphaIdPresent;
} ApexStSetUpUssdIndMessage;

/** Determines whether USSD set-up was successful or if it failed.*/
typedef struct ApexStSetUpUssdRspMessageTag
{
  /** Command reference */
  Int32                       simatCommandRef;
  /** Determines the outcome of the SS set up request */
  SimatCcResult               result;

} ApexStSetUpUssdRspMessage;

/** \defgroup SigApexStSignals  ABST Primitives
 * Primitives exchanged between  SIMAT and the Application Layer.
 * This section defines primitives exchanged between SIMAT and the Application Layer.
 *
 * @{
 */

/****************************************************************************
 * Signal Types
 ****************************************************************************/
/** Request a data download from the SIMAT.
 * Background Layer request to transparently pass a Short
 * Message received from the network via the
 * SMS Point-to-point mechanism to the SIM.
 */
typedef struct ApexStSmsPpDownloadIndTag
{
  /** The address of sender (Background Layer ) */
  TaskId                      taskId;
  /** Contains the contents of the TsDeliverInd signal */
  AlsaSmsPpDownloadReq        messageBody;
} ApexStSmsPpDownloadInd;

/** Determines the outcome of the ApexStSmsPpDownloadInd request.
 * Confirmation or not that a Short Message
 * received from the network via the SMS Point-to-point
 * mechanism has been passed to the SIM.
 */
typedef struct ApexStSmsPpDownloadRspTag
{
  /** The address of sender (SIMAT_TASK_ID) */
  TaskId                      taskId;
  /** Contains the contents of AlsaSmsPpDownloadCnf */
  AlsaSmsPpDownloadCnf        messageBody;
} ApexStSmsPpDownloadRsp;

/** Informs registered tasks that call control has taken place. */
typedef struct ApexStCcStatusIndTag
{
  /** AL_TASK_ID */
  TaskId                      taskId;
  /** Contains the ApexStCcStatusMessage data */
  SimatCcStatusIndMessage     messageBody;
  /** Indicates if alpha identifier is present */
  Boolean                     alphaIdPresent;
  /** Holds alpha identifier identifier data */
  SimatAlphaIdentifier        alphaId;
} ApexStCcStatusInd;

/** Indicates data has changed.
 * This signal provides an indication that data on the SIM has changed,
 * and must now be updated.
 * This may be a complete reset, or may just update the files
 * which have changed.
 */
typedef struct ApexStRefreshReqTag
{
  /** The sender task (SIMAT_TASK_ID) */
  TaskId                      taskId;
  /** Contains information of files to be refreshed */
  AlsaRefreshInd              messageBody;
} ApexStRefreshReq;

/** Returns the response data to the SAT */
typedef struct ApexStRefreshCnfTag
{
  /** The sender task */
  TaskId                      taskId;
  /** Holds the response data */
  AlsaRefreshRsp              messageBody;
} ApexStRefreshCnf;

/** Request to returns the Terminal Profile Data for the SIM
 * Toolkit entity.
 */
typedef struct ApexStConfigureReqTag
{
  /** ID of task sending signal */
  TaskId                      taskId;
  /** Contains the command reference of requested signal */
  ApexStConfigureReqMessage   messageBody;
} ApexStConfigureReq;

/** Reports SIM application toolkit handset capability.
 * This signal is sent as response to a ApexStConfigureReq.
 * It returns the Terminal Profile Data for the SIM
 * Toolkit entity
 */
typedef struct ApexStConfigureCnfTag
{
  /** The sender address (SIMAT_TASK_ID) */
  TaskId                      taskId;
  /**  Supplies the default terminal profile used by SIMAT Task. */
  ApexStConfigureCnfMessage   messageBody;
} ApexStConfigureCnf;

/** The ApexStCcCallSetUpReq function is passed to the SAT
 * from the Background Layer to allow the SIM to perform Call Control.
 * This signal  contains details of a call which must be set up.
 * This number may originate from the user or the SIM itself.
 */
typedef struct ApexStCcCallSetUpReqTag
{
  /** The sender address (AL_TASK_ID) */
  TaskId                      taskId;
  /** Contains the details of a call to be set up */
  AlsaCcCallSetUpReq          messageBody;
} ApexStCcCallSetUpReq;

/** The ApexStCcCallSetUpCnf signal is a confirmation whether
 * a call can be set up or not, and decides whether the handset
 * should bar, permit or replace a dialled number.
 */
typedef struct ApexStCcCallSetUpCnfTag
{
  /** The sender address (SIMAT_TASK_ID)*/
  TaskId                      taskId;
  /** The outcome of call set up request */
  AlsaCcCallSetUpCnf          messageBody;
} ApexStCcCallSetUpCnf;

/** The ApexStCcSsOperationReq signal is passed to the SAT Foreground
 * Layer for passing back down to the SIM and deals with a
 * Supplementary Service request.
 */
typedef struct ApexStCcSsOperationReqTag
{
  /** Te sender address (AL_TASK_ID) */
  TaskId                      taskId;
  /** Contains the details of a SS operation to be set up */
  AlsaCcSsOperationReq        messageBody;
} ApexStCcSsOperationReq;

/** The ApexStCcSsOperationCnf signal is a confirmation that
 * Supplementary Service has been validated.
 */
typedef struct ApexStCcSsOperationCnfTag
{
  /** The sender address (SIMAT_TASK_ID)*/
  TaskId                      taskId;
  /** The outcome of SS set up request */
  AlsaCcSsOperationCnf        messageBody;
} ApexStCcSsOperationCnf;


/** The ApexStCcUssdStringReq signal is passed to the SAT Foreground
 * Layer for passing back down to the SIM and deals with a
 * USSD string.
 */
typedef struct ApexStCcUssdStringReqTag
{
  /** The sender address (AL_TASK_ID) */
  TaskId                      taskId;
  /** Contains the details of a SS operation to be set up */
  AlsaCcUssdStringReq        messageBody;
} ApexStCcUssdStringReq;

/** The ApexStCcUssdOperationCnf signal is a confirmation that
 * Supplementary Service has been validated.
 */
typedef struct ApexStCcUssdStringCnfTag
{
  /** The sender address (SIMAT_TASK_ID)*/
  TaskId                      taskId;
  /** The outcome of SS set up request */
  AlsaCcUssdStringCnf        messageBody;
} ApexStCcUssdStringCnf;

/** The ApexStCcPdpContextReq function is passed to the SAT
 * from the Background Layer to allow the SIM to perform GPRS Call Control.
 * This signal  contains details of the context to be activated
 * This  may originate from the user or the SIM itself for an OPEN CHANNEL.
 */
typedef struct ApexStCcPdpContextReqTag
{
  /** The sender address (AL_TASK_ID) */
  TaskId                      taskId;
  /** Contains the details of the context to be activated*/
  AlsaCcPdpContextReq          messageBody;
  /** PDN channel peer */
  ChannelPeer                  pdnChannelPeer;
  /** PDN call control enabled or not */
  Boolean                      pdnCcEnabled;
  /* connId of PDN */
  Int8                         connId;
} ApexStCcPdpContextReq;

/** The ApexStCcPdpContextCnf signal is a confirmation whether
 * a context can be activated or not, and decides whether the context is
 * barred, permitted or modified .
 */
typedef struct ApexStCcPdpContextCnfTag
{
  /** The sender address (AL_TASK_ID) */
  TaskId                      taskId;
  /** Contains the details of the context to be activated*/
  AlsaCcPdpContextCnf          messageBody;
} ApexStCcPdpContextCnf;

/** The ApexStCcPdnContextReq function is passed to the SAT
 * from the Background Layer to allow the SIM to perform PDN Call Control.
 * This signal  contains details of the context to be activated
 * This  may originate from the user or the SIM itself for an OPEN CHANNEL.
 */
typedef struct ApexStCcPdnContextReqTag
{
  /** The sender address (AL_TASK_ID) */
  TaskId                      taskId;
  /** Contains the details of the context to be activated*/
  AlsaCcPdnContextReq          messageBody;
  /** PDN channel peer */
  ChannelPeer                  pdnChannelPeer;
  /** PDN call control enabled or not */
  Boolean                      pdnCcEnabled;
  /* connId of PDN */
  Int8                         connId;
} ApexStCcPdnContextReq;

/** The ApexStCcPdpContextCnf signal is a confirmation whether
 * a context can be activated or not, and decides whether the context is
 * barred, permitted or modified .
 */
typedef struct ApexStCcPdnContextCnfTag
{
  /** The sender address (AL_TASK_ID) */
  TaskId                      taskId;
  /** Contains the details of the context to be activated*/
  AlsaCcPdnContextCnf          messageBody;
} ApexStCcPdnContextCnf;

/** Request the SAT to set up a new call.
 * This signal contains the new call details, supplied
 * by the Background Layer to the SAT.
 */
typedef struct ApexStSetUpCallIndTag
{
  /** The address of sender (AL_TASK_ID) */
  TaskId                      taskId;
  /** Contains the call set up details */
  ApexStSetUpCallIndMessage   messageBody;
} ApexStSetUpCallInd;

/* TODO remove if possible. Used by nbiot_custom. */

/** The signal is passed from the SAT to the Background Layer
 * to confirm whether the set up call attempt was successful or not.
 * This signal is returned to the Background Layer as a
 * result of the set-up call attempt. If the SAT cannot set
 * up the call because it is busy, a negative response is returned.
 */
typedef struct ApexStSetUpCallRspTag
{
  /** The address of sender(SIMAT_TASK_ID) */
  TaskId                      taskId;
  /** Determines whether call set up was successful or not */
  ApexStSetUpCallRspMessage   messageBody;
} ApexStSetUpCallRsp;

/** Requests SIMAT to set up a Supplementary Service.
 * Contains the Supplementary Service details to be
 * passed to the SAT to allow the SAT to drive the ABSS interface.
 */
typedef struct ApexStSetUpSsIndTag
{
  /** The address of sender */
  TaskId                      taskId;
  /** Contains the Supplementary Service details. */
  ApexStSetUpSsIndMessage     messageBody;
} ApexStSetUpSsInd;

/* TODO remove if possible. Used by nbiot_custom. */

/** This signal is returned to the Background Layer as a
 * result of the set-up SS attempt. If the SAT cannot set
 * up the SS because it is busy, a negative response is returned.
 */
typedef struct ApexStSetUpSsRspTag
{
  /** The address of sender (SIMAT_TASK_ID) */
  TaskId                      taskId;
  /** Determines whether SS set up was successful or not */
  ApexStSetUpSsRspMessage     messageBody;
} ApexStSetUpSsRsp;

/** The ApexStCancelCommandReq  signal is passed from
 * the controlling task to the Background layer to
 * indicate whether the user decided to interrupt the proactive command.
 */
typedef struct ApexStCancelCommandReqTag
{
  /** The address of sender (SIMAT_TASK_ID) */
  TaskId                  taskId;
  /** Specifies the type of alpha identifier */
  SimatAlphaIdType        type;
  /** Indicates whether the user requested to cancel the
   * proactive command, terminate session or not cancel
   */
  SimatCommandCancelType  cancelCommand;
  /** Indicates whether or not the icon was displayed successfully */
  Boolean                 failedToDisplayIcon;
} ApexStCancelCommandReq;

/** This signal is passed from the background layer to the SIMAT task
 * to indicate whether the user decided to interrupt the proactive command.
 * This signal is a copy of the ApexStCancelCommandReq signal .
 */
typedef   ApexStCancelCommandReq   ApexStCancelCommandInd;

/** This signal is the response to the ApexStCancelCommandInd.*/
typedef struct ApexStCancelCommandRspTag
{
  /** The address of sender (SIMAT_TASK_ID) */
  TaskId                taskId;
} ApexStCancelCommandRsp;

/** The ApexStCancelCommandCnf is the response to the ApexStCancelCommandReq.*/
typedef   ApexStCancelCommandRsp   ApexStCancelCommandCnf;

/* Proactive command setup call acknowledge apex signals.*/

/** This signal is sent to all CC-registered tasks by
 * the Background Layer. This signal informs each registered
 * task that the SAT is waiting for a user response.
 *
 * An alpha ID and icon ID are optionally supplied.
 */
typedef struct ApexStCallSetupGetAckIndTag
{
  /** The sending task ID ( TASK_BL_ID ) */
  TaskId                taskId;
  /** Indicates if alpha identifier is present */
  Boolean               alphaIdPresent;
  /** Determines the alpha ID */
  SimatAlphaIdentifier  alphaId;
  /** Destination address to which the call is set up */
  SimatAddress          address;
   /** Indicates if icon ID is present */
  Boolean               iconIdPresent;
  /** Indicates the icon Id related to this call */
  SimatIconIdentifier   iconId;
  /**  if text formatting information is present */
  Boolean                       textAttsPresent;
  /** text formatting attributes */
  SimatTextAttributes           textAttributes;
  /** Indicates if it is call control */
  Boolean               ccBySim;     /*job 101219*/
} ApexStCallSetupGetAckInd;

/** This signal is sent by one of the registered tasks
 * when the user acknowledges the call set up proactive command.
 * The first registered task to respond is the only signal of this
 * type accepted.
 */
typedef struct ApexStCallSetupGetAckRspTag
{
  /** The sending task ID */
  TaskId                taskId;
  /** User response */
  SimatUserFeedback     userFeedback;
  /** Icon could not be displayed */
  Boolean               failedToDisplayIcon;
} ApexStCallSetupGetAckRsp;

/** This signal is sent back to the SAT Controlling
 * Task to confirm that the user (by whatever means) has responded.
 */
typedef ApexStCallSetupGetAckRsp ApexStCallSetupGetAckCnf;

/** Indicates the download has failed.
 * This signal Sends indication to all registered FL saying data
 * download failed.
 */
typedef struct ApexStDownloadErrorIndTag
{
  /** The source address */
  TaskId                taskId;
  /** Defines the error occures when downloading data */
  SimatDownloadError    downloadError;
} ApexStDownloadErrorInd;

/**  SIMAT notifies the Background Layer a command cannot be processed.
 * If the SIM cannot process a command the AlsaFetchFailInd signal is sent to the SIMAT.
 * The ApexStInformSimAppErrReq signal is subsequently sent to the Background Layer.
 */
typedef struct ApexStInformSimAppErrReqTag
{
  /** The address of sender (SIMAT_TASK_ID) */
  TaskId                taskId;
  /** Status of command */
  SimCommandStatus      cmdStatus;
  /** SIM status */
  SimSwStatusCode       simStatus;
  /** Status of SIM Application Toolkit command processed */
  SimatCommandStatus    simatCmdStatus;
} ApexStInformSimAppErrReq;

/** The Background Layer uses the ApexStInformSimAppErrInd signal
 * to inform all controlling tasks that a SIM error has occurred.
 */
typedef struct ApexStInformSimAppErrIndTag
{
  /** The source address (AL_TASK_ID) */
  TaskId                taskId;
  /** Status of command */
  SimCommandStatus      cmdStatus;
  /** SIM status */
  SimSwStatusCode       simStatus;
  /** Status of SIM Application Toolkit command processed */
  SimatCommandStatus    simatCmdStatus;
} ApexStInformSimAppErrInd;

/** This signal instructs the Background Layer to inform
 * all controlling tasks that an alpha ID (arising from an
 * Supplementary Service or SMS SAT transaction should be
 * displayed). An icon ID is optionally supplied.
 */
typedef struct ApexStDisplayAlphaIdReqTag
{
  /** The address of sender (SIMAT_TASK_ID) */
  TaskId                taskId;
  /** Indicates if alpha identifier is present */
  Boolean               alphaIdPresent;
  /** Specifies the type of alpha identifier */
  SimatAlphaIdType      type;
  /** Indicates the alpha identifier */
  SimatAlphaIdentifier  alphaId;
  /** Indicates if icon identifier is present */
  Boolean               iconIdPresent;
  /** Indicates the icon identifier */
  SimatIconIdentifier   iconId;
  /** if text formatting information for the text string was included
   * in the proactive command   */
  Boolean                       textAttsPresent;
  /** text formatting attributes */
  SimatTextAttributes           textAttributes;
} ApexStDisplayAlphaIdReq;

/** This signal is passed to all controlling tasks containing the
 * alpha ID to be displayed. An icon ID is optionally supplied
 */
typedef ApexStDisplayAlphaIdReq ApexStDisplayAlphaIdInd;


/*----------------------------------------------------
 * MO SM Control signals  SIM TOOLKIT CLASS III
 * --------------------------------------------------*/

/** The signal ApexStSmControlInd contains the service
 * centre address and the address of the recipient of the SMS under control.
 */
typedef struct ApexStSmControlIndTag
{
  /* Sender's tak ID */
  TaskId                      taskId;
  /** Holds AlsaMoSmControlReq message */
  AlsaMoSmControlReq          messageBody;
} ApexStSmControlInd;

/** The signal ApexStSmControlRsp contains
 * the response from the SIM regarding the status of
 * the SMS under control.
 */
typedef struct ApexStSmControlRspTag
{
  /** SIMAT_TASK_ID */
  TaskId                      taskId;
  /** Holds AlsaMoSmControlCnf message */
  AlsaMoSmControlCnf          messageBody;
} ApexStSmControlRsp;

/** Requests SIMAT to set up a USSD.
 * Contains the USSD details to be
 * passed to the SAT to allow the SAT to drive the ABSS interface.
 */
typedef struct ApexStSetUpUssdIndTag
{
  TaskId                      taskId;
  /** Details of USSD . */
  ApexStSetUpUssdIndMessage     messageBody;
} ApexStSetUpUssdInd;

/** This signal is returned to the Background Layer as a
 * result of the set-up USSD attempt. If the SAT cannot set
 * up the USSD, a negative response is returned.
 */
typedef struct ApexStSetUpUssdRspTag
{
  TaskId                      taskId;
  /** Determines whether SS set up was successful or not */
  ApexStSetUpUssdRspMessage     messageBody;
} ApexStSetUpUssdRsp;

/** Not Used */
typedef struct ApexStSmSendIndTag
{
  /** Sender's task ID */
  TaskId                      taskId;
  /** Command reference */
  Int32                       simatCommandRef;
} ApexStSmSendInd;

/** Not used */
typedef struct ApexStSmSendRspTag
{
  /** Sender's task ID */
  TaskId                      taskId;
  /** Command reference */
  Int32                       simatCommandRef;
  /** Result of request */
  SimatMoSmResult             result;
} ApexStSmSendRsp;

/** Not used */
typedef struct ApexStSmStatusIndTag
{
  /** Sender's task ID */
  TaskId                      taskId;
} ApexStSmStatusInd;

/** @} */ /* End of SigApexStSignals group */
/** @} */ /* End of SigApexSt group */
#endif
/* END OF FILE */
