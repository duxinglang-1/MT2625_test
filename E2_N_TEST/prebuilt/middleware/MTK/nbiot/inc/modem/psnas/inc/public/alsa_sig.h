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
 *      application layer - sim manager interface for SIM Application Toolkit
 *      related functionality
 **************************************************************************/

#ifndef ALSA_SIG_H
#define ALSA_SIG_H

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

#if !defined (SYSTEM_H)
#include     <system.h>
#endif

#if !defined (ALSA_TYP_H)
#include     <alsa_typ.h>
#endif


/** \defgroup SigAlsa  ALSA Signalling Interface
 * \ingroup Prd3gDmNas
 * \ingroup Sig
 * ALSA Signalling Interface is the interface between USIM Manager and
 * SAT Foreground Layer interface.
 * The USIM Manager passes signals to and from the SIMAT using the ALSA interface.
 * This interface defines the communication mechanism between the
 * USIM Manager and the Application Layer, and deals specifically
 * with the SIM Application Toolkit implementation.
 *
 * <B> The context of the ALSA interface is illustrated in the following diagram.</B>
 *
 * \image html SAT_Software_Interfaces.png
 *
 * @{
 */

/** \defgroup SigAlsaSignals  ALSA Primitives
 * Primitives exchanged between USIM Manager and SIMAT Task.
 * This section defines primitives exchanged between USIM Manager and SIMAT Task.
 *
 * @{
 */
/****************************************************************************
 * Signal types
 ****************************************************************************/
/** Indication that the ME is to display a text message on the screen,
 * indicating both the text string format and the priority of the message.
 * The command qualifier information holds the SimatDisplayTextQual structure,
 * which indicates to the application layer when the text message should be
 * removed from the screen (as a consequence of a user MMI action, or following
 * a predetermined delay) and also identifies the priority level of that text message.
 */

typedef struct AlsaDisplayTextIndTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /** The comprehension required */
    Boolean                       partialComprehen;
    /** The missing information */
    Boolean                       missingInformation;
    /** Command qualifier for the SIMAT display text */
    SimatDisplayTextQual          commandQual;
    /** Text message to be displayed */
    SimatTextString               textString;
    /** Whether or not icon data available */
    Boolean                       iconIdPresent;
    /** Icon Id data */
    SimatIconIdentifier           iconId;
    /** Immediate response (Whether the terminal shall immediately send the response) */
    Boolean                       immediateResponse;
    /** If set to false the ME defaults to a duration provided at
     * the discretion of the ME manufacturer.
     */
    Boolean                       durationPresent;
    SimatTimeUnit                 durationUnit;
    Int8                          durationValue;
    /** if text formatting information for the text string was included in the
     * proactive command
\assoc PRESENT \ref textAttributes */
     Boolean                      textAttsPresent;
    /** text formatting attributes */
    SimatTextAttributes           textAttributes;
}
AlsaDisplayTextInd;

/** Returns the response data for the Display Text proactive SIM command.*/
typedef struct AlsaDisplayTextRspTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /** Identifies the outcome of the command to be relayed to the SIM.  */
    SimatGeneralResult            generalResult;
    /** Indicates whether a problem was encountered with the ME in
     * attempting to perform the proactive command.

\assoc PRESENT \ref meProblem */
    Boolean                       meProblemPresent;
    /** Provide  additional information why ME currently unable to process command*/
    SimatMeProblemAddInfo         meProblem;
}
AlsaDisplayTextRsp;

/** Indication that text is to be displayed and the user is to
 * enter a single character in response to the text message.
 * The signal specifies if help information is available for the user to request and
 * the valid character set for user input as part of the command qualifier data.
 */
typedef struct AlsaGetInkeyIndTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /** The comprehension required */
    Boolean                       partialComprehen;
    /** The missing information */
    Boolean                       missingInformation;
    /** Command qualifier for the SIMAT get inkey */
    SimatGetInkeyQual             commandQual;
    /** Text message to be displayed */
    SimatTextString               textString;
    /** Whether or not icon data available */
    Boolean                       iconIdPresent;
    /** Icon Id data */
    SimatIconIdentifier           iconId;
    /** If set to false the ME defaults to a duration provided at
     * the discretion of the ME manufacturer. */
    Boolean                       durationPresent;
    SimatTimeUnit                 durationUnit;
    Int8                          durationValue;
    /** if text formatting information for the text string was included
     * in the proactive command
\assoc PRESENT \ref textAttributes */
    Boolean                       textAttsPresent;
    /** text formatting attributes */
    SimatTextAttributes           textAttributes;

}
AlsaGetInkeyInd;

/** Returns the response data for the Get Inkey proactive SIM command.*/
typedef struct AlsaGetInkeyRspTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /** Identifies the outcome of the command to be relayed to the SIM. */
    SimatGeneralResult            generalResult;
    SimatTextString               keyDigit;
    /** Indicates whether a problem was encountered with the ME in
     * attempting to perform the proactive command.

\assoc PRESENT \ref meProblem */
    Boolean                       meProblemPresent;
    /** Determines the ME problem */
    SimatMeProblemAddInfo         meProblem;
    /** Total display duration, see 102 223, section 6.8.22*/
    Boolean                       durationPresent;
    SimatTimeUnit                 durationUnit;
    Int8                          durationValue;
}
AlsaGetInkeyRsp;

/** Indication that text is to be displayed and the user is to enter a response string.
 * The commandQual structure holds the command qualifier details for the proactive
 * command, indicating the availability of help information, the valid character
 * set for the text, the packing format, and level of visibility of the response
 * string to the user.
 */
typedef struct AlsaGetInputIndTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /** The comprehension required */
    Boolean                       partialComprehen;
    /** The missing information */
    Boolean                       missingInformation;
    /** Command qualifier for the SIMAT get input */
    SimatGetInputQual             commandQual;
    /** Text string to be displayed */
    SimatTextString               textString;
    /** The length of response to be entered by user */
    SimatResponseLength           responseLength;
    /** Whether the default text string is present
\assoc PRESENT \ref defaultTextString */
    Boolean                       defaultTextStringPresent;
    /** The default text string */
    SimatTextString               defaultTextString;
    /** Whether or not icon data available */
    Boolean                       iconIdPresent;
    /** Icon Id data */
    SimatIconIdentifier           iconId;
    /** if text formatting information for the text string was included
     * in the proactive command
\assoc PRESENT \ref textAttributes */
    Boolean                       textAttsPresent;
    /** text formatting attributes */
    SimatTextAttributes           textAttributes;

}
AlsaGetInputInd;

/** Returns the response data for the Get Input proactive SIM command.*/
typedef struct AlsaGetInputRspTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /**  Identifies the outcome of the command to be relayed to the SIM. */
    SimatGeneralResult            generalResult;
    /** The returned response data */
    SimatTextString               textString;
    /** Indicates whether a problem was encountered with the ME in
     * attempting to perform the proactive command.

\assoc PRESENT \ref meProblem */
    Boolean                       meProblemPresent;
    /** Provide  additional information why ME currently unable to process command*/
    SimatMeProblemAddInfo         meProblem;
}
AlsaGetInputRsp;

/** This signal indicates that an audio tone is to be played.
 * The tone, alpha identifier and duration information may not
 * be provided by the SIM, and the relevant Boolean types indicate
 *  if the information is present or not.
 */
typedef struct AlsaPlayToneIndTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /** The comprehension required */
    Boolean                       partialComprehen;
    /** The missing information */
    Boolean                       missingInformation;
    /** Indicates if alpha Id is present
\assoc PRESENT \ref alphaId */
    Boolean                       alphaIdPresent;
    /** Only used if the SIM wishes to convey a textual meaning for
     * the tone to the user.
     */
    SimatAlphaIdentifier          alphaId;
    /** if text formatting information for the text string was included in the
     * proactive command
\assoc PRESENT \ref textAttributes */
    Boolean                       textAttsPresent;
    /** text formatting attributes */
    SimatTextAttributes           textAttributes;
    /** If tonePresent is FALSE the ME defaults to a general beep.
\assoc PRESENT \ref tone */
    Boolean                       tonePresent;
    /** Tone to be displayed */
    SimatToneType                 tone;
    /** If set to false the ME defaults to a duration provided at
     * the discretion of the ME manufacturer.

\assoc PRESENT \ref durationValue */
    Boolean                       durationPresent;
    SimatTimeUnit                 durationUnit;
    Int8                          durationValue;
    /** Whether or not icon data available */
    Boolean                       iconIdPresent;
    /** Icon Id data */
    SimatIconIdentifier           iconId;
}
AlsaPlayToneInd;

/** Returns the response data for the Play Tone proactive SIM command.*/
typedef struct AlsaPlayToneRspTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /**  Identifies the outcome of the command to be relayed to the SIM. */
    SimatGeneralResult            generalResult;
    /** Indicates whether a problem was encountered with the ME in
     * attempting to perform the proactive command.

\assoc PRESENT \ref meProblem */
    Boolean                       meProblemPresent;
    /** Provide  additional information why ME currently unable to process command*/
    SimatMeProblemAddInfo         meProblem;
}
AlsaPlayToneRsp;

/** Indication that a set of menu items has been supplied to
 * the application layer by the SIM for user navigation.
 * It may alternatively indicate that the menu is to be
 * removed from the menu structure of the ME, depending
 * on the length and value of the first item data object
 * in the Simat Structure.  Each menu item contains an identifier and a text string.
 */
typedef struct AlsaSetUpMenuIndTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /** The comprehension required */
    Boolean                       partialComprehen;
    /** The missing information */
    Boolean                       missingInformation;
    /** Command qualifier for the SIMAT set up menu */
    SimatSetUpMenuQual            commandQual;
    /** Alpha identifier for the proactive command */
    SimatAlphaIdentifier          alphaId;
    /** if text formatting information for the text string above was included
     * in the proactive command
\assoc PRESENT \ref titleTextAttributes */
    Boolean                       titleTextAttsPresent;
    /** text formatting attributes */
    SimatTextAttributes           titleTextAttributes;
    /** Menu items */
    SimatMenuItems                menuItems;
    /** Indicates whether or not NAI value is present
\assoc PRESENT \ref nai */
    Boolean                       naiPresent;
    /** Next Action Identifier value */
    SimatNextActionIndicator      nai;
    /** Icon data available */
    Boolean                       iconIdPresent;
    /** Icon Id data */
    SimatIconIdentifier           iconId;
}
AlsaSetUpMenuInd;

/** Returns the response data for the Set Up Menu proactive SIM command.*/
typedef struct AlsaSetUpMenuRspTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /**  Identifies the outcome of the command to be relayed to the SIM. */
    SimatGeneralResult            generalResult;
    /** Indicates whether a problem was encountered with the ME in
     * attempting to perform the proactive command.

\assoc PRESENT \ref meProblem */
    Boolean                       meProblemPresent;
    /** Provide  additional information why ME currently unable to process command*/
    SimatMeProblemAddInfo         meProblem;
}
AlsaSetUpMenuRsp;

/** Indication that the user is required to select an item from
 * the list of text stings provided by the SIM
 */
typedef struct AlsaSelectItemIndTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /** The comprehension required */
    Boolean                       partialComprehen;
    /** The missing information */
    Boolean                       missingInformation;
    /** Command qualifier for the SIMAT select item */
    SimatSelectItemQual           commandQual;
    /** Indicates whether or not the proactive command data
     * from the SIM contains an alpha identifier

\assoc PRESENT \ref alphaId */
    Boolean                       alphaIdPresent;
    /** Alpha identifier for the proactive command */
    SimatAlphaIdentifier          alphaId;
    /** if text formatting information for the text string above was included
     * in the proactive command    */
    Boolean                       textAttsPresent;
    /** text formatting attributes */
    SimatTextAttributes           textAttributes;
    /** The number of items in the list to choose from */
    SimatItemList                 itemList;
    /** Indicates if default Item Identifier is present
\assoc PRESENT \ref defaultItemId */
    Boolean                       defaultItemIdPresent;
    /** Default Item Identifier */
    Int8                          defaultItemId;
    /** Indicates if NAI value is present
\assoc PRESENT \ref nai */
    Boolean                       naiPresent;
    /** Next Action Identifier value */
    SimatNextActionIndicator      nai;
    /** Whether or not icon data available */
    Boolean                       iconIdPresent;
    /** Icon Id data */
    SimatIconIdentifier           iconId;
}
AlsaSelectItemInd;

/** Returns the response data for the Select Item proactive SIM command.*/
typedef struct AlsaSelectItemRspTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /**  Identifies the outcome of the command to be relayed to the SIM. */
    SimatGeneralResult            generalResult;
    /** The identifier of the item chosen */
    Int8                          chosenItemId;
    /** Indicates whether a problem was encountered with the ME in
     * attempting to perform the proactive command.

\assoc PRESENT \ref meProblem */
    Boolean                       meProblemPresent;
    /** Provide  additional information why ME currently unable to process command*/
    SimatMeProblemAddInfo         meProblem;
}
AlsaSelectItemRsp;

/** This signal indicates that a short message is to be sent to the network
 * with either user data passed transparently or in a packed text format.
 */
typedef struct AlsaSendSmIndTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /** The comprehension required */
    Boolean                       partialComprehen;
    /** The missing information */
    Boolean                       missingInformation;
    /** Identifies whether packing of the text is required by the ME. */
    SimatSendSmQual               commandQual;
    /** Indicates whether the alpha identifier is present in
     * the command data received from the SIM.

\assoc PRESENT \ref alphaId */
    Boolean                       alphaIdPresent;
    /** Alpha identifier for the proactive command */
    SimatAlphaIdentifier          alphaId;
    /** if text formatting information for the text string above was included
     * in the proactive command
\assoc PRESENT \ref textAttributes */
    Boolean                       textAttsPresent;
    /** text formatting attributes */
    SimatTextAttributes           textAttributes;
    /** Indicates whether the address data is present in
     * the command data received from the SIM.

\assoc PRESENT \ref address */
    Boolean                       addressPresent;
    /** destination address where Short Message TPDU is sent */
    SimatAddress                  address;
    /** SMS type
\assoc UNION \ref smTpdu */
    SimSmTpduType                 smType;
    /** SMS-TPDU */
    SimSmTpdu                     smTpdu;
    /** Whether or not icon data available */
    Boolean                       iconIdPresent;
    /** Icon Id data */
    SimatIconIdentifier           iconId;
}
AlsaSendSmInd;

/** Returns the response data for the Send Short Message proactive SIM command.*/
typedef struct AlsaSendSmRspTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /**  Identifies the outcome of the command to be relayed to the SIM. */
    SimatGeneralResult            generalResult;
    /** Indicates if ME problem is present
\assoc PRESENT \ref meProblem */
    Boolean                       meProblemPresent;
    /** Provide  additional information why ME currently unable to process command*/
    SimatMeProblemAddInfo         meProblem;
    /** Indicates if SMS problem is present
\assoc PRESENT \ref smsProblem */
    Boolean                       smsProblemPresent;
    /** Determines the SMS problem */
    GsmCause                      smsProblem;
    /** Indicates if call control problem is present */
    Boolean                       ccProblemPresent;
    /** Determines the call control problem */
    SimatCcProblemAddInfo         ccProblem;
}
AlsaSendSmRsp;

/** Indication that the ME is to send an SS request,
 * regardless of whether the Fixed Dialling Number service is enabled
 */
typedef struct AlsaSendSsIndTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /** The comprehension required */
    Boolean                       partialComprehen;
    /** The missing information */
    Boolean                       missingInformation;
    /** Indicates whether an alpha identifier is included in the signal.
\assoc PRESENT \ref alphaId */
    Boolean                       alphaIdPresent;
    /** Alpha identifier for the proactive command */
    SimatAlphaIdentifier          alphaId;
    /** if text formatting information for the text string above was included
     * in the proactive command
\assoc PRESENT \ref textAttributes */
    Boolean                       textAttsPresent;
    /** text formatting attributes */
    SimatTextAttributes           textAttributes;
    /** SS string */
    SimatSsString                 ssString;
    /** Whether or not icon data available */
    Boolean                       iconIdPresent;
    /** Icon Id data */
    SimatIconIdentifier           iconId;
}
AlsaSendSsInd;

/** Returns the response data for the Send SS proactive SIM command.*/
typedef struct AlsaSendSsRspTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /**  Identifies the outcome of the command to be relayed to the SIM. */
    SimatGeneralResult            generalResult;
    /** Provide the type of specific cause value as additional information
\assoc UNION \ref additInfo */
    SimatSsAdditResultType        additInfoType;
    /** Provide a specific cause value as additional information */
    SimatSsAdditResult            additInfo;
    /** Indicates if call control problem is present */
    Boolean                       ccProblemPresent;
    /** Determines the call control problem */
    SimatCcProblemAddInfo         ccProblem;
    /** Indicates if dditional result information for TRCC is present */
    Boolean                       trccInfoPresent;
    /** Additional result information for TRCC, if present */
    SimatTrccInfo                 trccInfo;
}
AlsaSendSsRsp;

/** This signal indicates that a call is to be set up by the SIM,
 * depending on the current call state. The alpha identifier,
 * bearer capability, called party sub address, and duration
 * information are optional in the command from the SIM, and
 * their presence is denoted by the relevant Boolean flags in the signal.
 */
typedef struct AlsaSetUpCallIndTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /** The comprehension required */
    Boolean                       partialComprehen;
    /** The missing information */
    Boolean                       missingInformation;
    /** Command qualifier for the SIMAT set up a call */
    SimatSetUpCallQual            commandQual;
    /** Indicates if alpha Id is present
\assoc PRESENT \ref alphaId */
    Boolean                       alphaIdPresent;
    /** Alpha identifier for the proactive command */
    SimatAlphaIdentifier          alphaId;
    /** Destination address to which the call is set up */
    SimatAddress                  address;
    /** Indicates if bearer capability is present
\assoc PRESENT \ref ccp */
    Boolean                       ccpPresent;
    /** Relevant capability configuration parameters */
    BearerCapability              ccp;
    /** Indicates if called party sub address is present
\assoc PRESENT \ref cps */
    Boolean                       cpsPresent;
    /** Called party sub address */
    CalledPartySubaddress         cps;
    /** If set to false the ME defaults to a duration provided at
     * the discretion of the ME manufacturer.

\assoc PRESENT \ref durationInMillisecs */
    Boolean                       durationPresent;
    /** DURATION in millisecs */
    Int32                         durationInMillisecs;
    /** Whether or not icon data available */
    Boolean                       iconIdPresent;
    /** Icon Id data */
    SimatIconIdentifier           iconId;
    /** if text formatting information for the text string above was included
     * in the proactive command
\assoc PRESENT \ref textAttributes */
    Boolean                       textAttsPresent;
    /** text formatting attributes */
    SimatTextAttributes           textAttributes;
    /** second alpha ID if present */
    Boolean                       csuPhaseAlphaIdPresent;
    /** second alpha ID */
    SimatAlphaIdentifier          csuPhaseAlphaId;
    /** second icon ID if present */
    Boolean                       csuPhaseIconIdPresent;
    /** second icon ID */
    SimatIconIdentifier           csuPhaseIconId;
    /** if text formatting information is present for secod alpha id
\assoc PRESENT \ref csuPhaseTextAttributes */
    Boolean                       csuPhaseTextAttsPresent;
    /** text formatting attributes for second alpha id*/
    SimatTextAttributes           csuPhaseTextAttributes;
}
AlsaSetUpCallInd;

/** Returns the response data for the Set Up Call proactive SIM command. */
typedef struct AlsaSetUpCallRspTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /**  Identifies the outcome of the command to be relayed to the SIM. */
    SimatGeneralResult            generalResult;
    /** Indicates whether a problem was encountered with the ME in
     * attempting to perform the proactive command.

\assoc PRESENT \ref meProblem */
    Boolean                       meProblemPresent;
    /** Provide  additional information why ME currently unable to process command*/
    SimatMeProblemAddInfo         meProblem;
    /** Indicates if network problem is present
\assoc PRESENT \ref netProblem */
    Boolean                       netProblemPresent;
    /** Determines the network problem */
    GsmCause                      netProblem;
    /** Indicates if call control problem is present */
    Boolean                       ccProblemPresent;
    /** Determines the call control problem */
    SimatCcProblemAddInfo         ccProblem;
    /** Indicates if dditional result information for TRCC is present */
    Boolean                       trccInfoPresent;
    /** Additional result information for TRCC, if present */
    SimatTrccInfo                 trccInfo;
}
AlsaSetUpCallRsp;

/** Indication to the ME of changes to the SIM configuration determining
 * whether SIM Initialisation, File Change Notification, or SIM Reset is
 * to be performed.
 */
typedef struct AlsaRefreshIndTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /** The comprehension required */
    Boolean                       partialComprehen;
    /** The missing information. */
    Boolean                       missingInformation;
    /** Defines which refresh mode is required. */
    SimatRefreshQual              commandQual;
    /** The list of files to be refreshed for Refresh Command
     * types File Change Notification and SIM Initialisation
     * and File Change Notification
     */
    SimatFileList                 fileInfo;
    /** Application ID of the application that should be refreshed. */
    SimatAid                      aid;
}
AlsaRefreshInd;

/** Returns the response data for the Refresh proactive SIM command.*/
typedef struct AlsaRefreshRspTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /**  Identifies the outcome of the command to be relayed to the SIM. */
    SimatGeneralResult            generalResult;
    /** Indicates whether a problem was encountered with the ME in
     * attempting to perform the proactive command.

\assoc PRESENT \ref meProblem */
    Boolean                       meProblemPresent;
    /** Provide  additional information why ME currently unable to process command*/
    SimatMeProblemAddInfo         meProblem;
}
AlsaRefreshRsp;

/** Indication that the ME is to pass the current local information
 * to the SIM.  This information is presently restricted to the LAC,
 * MNC, MCC, Cell ID of the serving cell, and the IMEI, and is returned
 * within a Terminal Response
 */
typedef struct AlsaProvideLocalInfoIndtag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /** The comprehension required */
    Boolean                       partialComprehen;
    /** The missing information */
    Boolean                       missingInformation;
    /** Identifies whether it is the Location Information or the IMEI to be provided.*/
    SimatProvLocalInfoQual        commandQual;
}
AlsaProvideLocalInfoInd;

/** Returns the response data for the Provide Local Information proactive SIM command.*/
typedef struct AlsaProvideLocalInfoRspTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /**  Identifies the outcome of the command to be relayed to the SIM. */
    SimatGeneralResult            generalResult;
    /** The type of local info for the SIMAT Provide Local Information proactive command
\assoc UNION \ref localInfo */
    SimatProvLocalInfoQual        localInfoType;
    /** Contains the Location Information or the IMEI of the ME,
     * depending on the value of the localInfoType field.
     */
    SimatProvLocalInformation     localInfo;
    /** Indicates whether a problem was encountered with the ME in
     * attempting to perform the proactive command.

\assoc PRESENT \ref meProblem */
    Boolean                       meProblemPresent;
    /** Provide  additional information why ME currently unable to process command*/
    SimatMeProblemAddInfo         meProblem;
}
AlsaProvideLocalInfoRsp;

/** Request to transparently pass a Short
 * Message received from the network via the
 * SMS Point-to-point mechanism to the SIM.
 */
typedef struct AlsaSmsPpDownloadReqTag
{
    /** Command reference */
    Int16                         simatCommandRef;
    /** Contains the contents of the TsDeliverInd signal. */
    TsDeliverInd                  deliver;
#if defined (FEA_KOR_LGUP)
    Boolean                       isLgupSubscription;
#endif
}
AlsaSmsPpDownloadReq;

/** Confirmation or not that a Short Message
 * received from the network via the SMS Point-to-point
 * mechanism has been passed to the SIM.
 * A value of zero for the responseDataLength field indicates
 * there is no response data to include in the acknowledgement
 * message to be sent back to the network by the ME.
 */
typedef struct AlsaSmsPpDownloadCnfTag
{
    /** Command reference */
    Int16                         simatCommandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus              requestStatus;
    /** The length of response data
\assoc ARRAY \ref simatSmsPpAck */
    Int8                          responseDataLength;
    /** SIM Acknowledgement Response Data */
    Int8                          simatSmsPpAck[SIMAT_MAX_SMS_PP_ACK_LEN];
#if defined (FEA_KOR_LGUP)
    Boolean                       isLgupSubscription;
#endif
}
AlsaSmsPpDownloadCnf;

/** Request to select an item from the Menu provided by the proactive SIM */
typedef struct AlsaMenuSelectionReqTag
{
    /** Command reference */
    Int16                         simatCommandRef;
    /** The item to be selected */
    Int8                          chosenItemId;
    /** Determines whether help information has been requested by
     * the user on the selected menu item.
     */
    Boolean                       helpInfoRequired;
}
AlsaMenuSelectionReq;

/** Confirmation or not that the SIM has been informed of the menu
 * selection made by the user from the menu supplied by the proactive SIM.
 * This includes an indication of whether this has resulted in a proactive
 * session being initiated on the SIM.
 */
typedef struct AlsaMenuSelectionCnfTag
{
    /** Command reference */
    Int16                         simatCommandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus              requestStatus;
    /** proactive session has started after the menu selection */
    Boolean                       proactiveSessionStarted;
}
AlsaMenuSelectionCnf;

/** Request to pass the call set up details to the SIM using the Envelope
 * (Call Control) command mechanism as part of the Call Control by SIM feature.
 */
typedef struct AlsaCcCallSetUpReqTag
{
    /** Command reference */
    Int16                         simatCommandRef;
    /** Holds the Called Party Number to which the ME is proposing setting up the call.*/
    SimatAddress                  address;
    /** Indicates if Bearer capabilities info is present */
    Boolean                       ccpPresent;/* job 101264 added presence flag */
    /** Contains the Bearer capabilities that the ME is proposing to send to the network.*/
    BearerCapability              ccp;
    /** Indicates if called party subaddress is present */
    Boolean                       cpsPresent;/* job 101264 added presence flag */
    /** Contains the called party subaddress that the ME is proposing to send to the network */
    CalledPartySubaddress         cps;
    /** location info */
    SimatLocationInfo             locationInfo;
    /** Indicates if Bearer capabilities info2 is present */
    Boolean                       ccp2Present;/* job 101264 added ccp2 data object */
    /** Contains the Bearer capabilities 2 that the ME is proposing to send to the network.*/
    BearerCapability              ccp2;
}
AlsaCcCallSetUpReq;

/** Confirmation or not that the request to set up a call has been
 * successfully passed to the SIM
 */
typedef struct AlsaCcCallSetUpCnfTag
{
    /** Command reference */
    Int16                         simatCommandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus              requestStatus;
    /** Indicates whether the ME should allow, bar or modify the intended call. */
    SimatCallControlResult        ccResult;
    /** If no response data is available from the SIM (dependent on the ccResult
     * value) the ccInfoPresent field is set to FALSE and any remaining fields
     * can be ignored.

\assoc PRESENT \ref ccInfo */
    Boolean                       ccInfoPresent;
    /** Defines whether it is related to a Call Set Up or a
     * Supplementary Service operation.

\assoc UNION \ref ccInfo */
    SimatCallControlInfoType      ccInfoType;
    /** Call control info */
    SimatCallControlInfo          ccInfo;
    /** Indicates if Alpha Id is present
\assoc PRESENT \ref alphaId */
    Boolean                       alphaIdPresent;
    /** Alpha identifier for the proactive command */
    SimatAlphaIdentifier          alphaId;
}
AlsaCcCallSetUpCnf,
/** Confirmation or not that the supplementary service control
 * string details have been successfully passed to the SIM.
 */
AlsaCcSsOperationCnf,
AlsaCcUssdStringCnf;

/** Request to pass the supplementary service control string details to the SIM
 * using the Envelope (Call Control) command mechanism as part of the Call
 * Control by SIM feature.
 */
typedef struct AlsaCcSsOperationReqTag
{
    /** Command reference */
    Int16                         simatCommandRef;
    /** SS control string */
    SimatSsString                 ssString;
    /** Provides location info */
    SimatLocationInfo             locationInfo;
}
AlsaCcSsOperationReq;

typedef struct AlsaCcUssdStringReqTag
{
    /** Command reference */
    Int16                         simatCommandRef;
    /** Boolean to indicate if this is SS or USSD string */
    SimatUssdString               ussdString;
    /** Provides location info */
    SimatLocationInfo             locationInfo;
}
AlsaCcUssdStringReq;

/** Signal passed to the Application Layer to indicate that the attempt
 * to process the Toolkit command has failed
 */
typedef struct AlsaFetchFailIndTag
{
    /** Command status */
    SimCommandStatus      cmdStatus;
    SimSwStatusCode       simStatus;       /**<  Decoded from SW words */
    SimatCommandStatus    simatCmdStatus;  /**<  SIM Toolkit Command Status */

} AlsaFetchFailInd;

/** Indication that the ME is to send an USSD request, regardless of whether
 * the Fixed Dialling Number service is enabled.
 */
typedef struct AlsaSendUssdIndTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /** The comprehension required */
    Boolean                       partialComprehen;
    /** The missing information */
    Boolean                       missingInformation;
    /** Indicates whether an alpha identifier is included in the signal.
\assoc PRESENT \ref alphaId */
    Boolean                       alphaIdPresent;
    /** Alpha identifier for the proactive command */
    SimatAlphaIdentifier          alphaId;
    /** if text formatting information for the text string above was included
     * in the proactive command    */
    Boolean                       textAttsPresent;
    /** text formatting attributes */
    SimatTextAttributes           textAttributes;
    /** USSD string */
    SimatUssdString               ussdString;
    /** Whether or not icon data available */
    Boolean                       iconIdPresent;
    /** Icon Id data */
    SimatIconIdentifier           iconId;
}
AlsaSendUssdInd;

/** Returns the response data for the Send USSD proactive SIM command.*/
typedef struct AlsaSendUssdRspTag
{
    Int32                         simatCommandRef;
    /**  Identifies the outcome of the command to be relayed to the SIM. */
    SimatGeneralResult            generalResult;
    /** Provide the type of specific cause value as additional information
\assoc UNION \ref additInfo */
    SimatUssdAdditResultType      additInfoType;
    /** Provide a specific cause value as additional information */
    SimatUssdAdditResult          additInfo;
    /** Indicates if call control problem is present */
    Boolean                       ccProblemPresent;
    /** Determines the call control problem */
    SimatCcProblemAddInfo         ccProblem;
    /** Indicates if dditional result information for TRCC is present */
    Boolean                       trccInfoPresent;
    /** Additional result information for TRCC, if present */
    SimatTrccInfo                 trccInfo;
}
AlsaSendUssdRsp;

/** This signal  supplies the list of events to monitor.
 * These are included in the SimatEventList structure.
 */
typedef struct AlsaSetUpEventListIndTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /** The comprehension required */
    Boolean                       partialComprehen;
    /** The missing information */
    Boolean                       missingInformation;
    /** Monitor relevant events */
    SimatEventList                monitorEvent;
}
AlsaSetUpEventListInd;

/** Returns the response data for the Setup Event List proactive SIM command.
 */
typedef struct AlsaSetUpEventListRspTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /**  Identifies the outcome of the command to be relayed to the SIM. */
    SimatGeneralResult            generalResult;
    /** Indicates whether a problem was encountered with the ME in
     * attempting to perform the proactive command.
     */
    Boolean                       meProblemPresent;
    /** Provide  additional information why ME currently unable to process command*/
    SimatMeProblemAddInfo         meProblem;
}
AlsaSetUpEventListRsp;

/** Signal to download a detected event to the SIM/UICC as set-up
 * by Set-up Event List proactive command.
 */
typedef struct AlsaEventDownloadReqTag
{
    /** Command reference */
    Int16                         simatCommandRef;
    /** Specifies the event type
\assoc UNION \ref event */
    SimatEventDownloadType        eventType;
    /** Event */
    SimatEventDownload            event;
}
AlsaEventDownloadReq;

/** Confirmation of success or failure to download a detected event. */
typedef struct AlsaEventDownloadCnfTag
{
    /** Command reference */
    Int16                         simatCommandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus              requestStatus;
}
AlsaEventDownloadCnf;

/** sends a MO SM CONTROL ENVELOPE to the SIM.
 * Contains the service centre address and the address of the
 * recipient of the SMS under control.
 */
typedef struct AlsaMoSmControlReqTag
{
    /** Command reference */
    Int16                         simatCommandRef;
    /** Source address */
    SimatAddress                  scAddress;
    /** Destination address */
    SimatAddress                  destAddress;
    /** Location info of the current cell */
    SimatLocationInfo             locationInfo;
}
AlsaMoSmControlReq;

/** Confirmation of the request to send a MO SM CONTROL ENVELOPE to the SIM  */
typedef struct AlsaMoSmControlCnfTag
{
    /** Command reference */
    Int16                         simatCommandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus              requestStatus;
    /** MO Short Message control result */
    SimatMoSmControlResult        mosmResult;
    /** Indicates if MO Short Message info is present */
    Boolean                       mosmInfoPresent;
    /** Indicates if MO SM source address is present
\assoc PRESENT \ref scAddress */
    Boolean                       scAddressPresent;
    /** Source address */
    SimatAddress                  scAddress;
    /** Indicates if destination address is present
\assoc PRESENT \ref destAddress */
    Boolean                       destAddressPresent;
    /** Destination address */
    SimatAddress                  destAddress;
    /** Indicates if alpha ID is present
\assoc PRESENT \ref alphaId */
    Boolean                       alphaIdPresent;
    /** Alpha identifier for the proactive command */
    SimatAlphaIdentifier          alphaId;
}
AlsaMoSmControlCnf;

/** This signal is sent to  SIM Manager requesting a new Terminal Profile
 * to be sent to the SIM.
 */
typedef struct AlsaSendTermProfileReqTag
{
    /** Command reference */
    Int16                  simatCommandRef;
    /** Identifies the features relevant to the SIMAT that the ME supports. */
    SimTerminalProfile     terminalProfile;
}
AlsaSendTermProfileReq;

/** This signal is sent as a response to AlsaSendTermProfileReq */
typedef struct AlsaSendTermProfileCnfTag
{
    /** Command reference */
    Int16                  simatCommandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus       requestStatus;
}
AlsaSendTermProfileCnf;

/* job 103179 */
/** Signal that indicates whether the ME is in idle mode or not */
typedef struct AlsaMmModeIndTag
{
    /** Indicates if MM is in idle mode */
   Boolean mmIdleMode;
}
AlsaMmModeInd;

typedef struct AlsaNetworkMeasCnfTag
{
    /* NO_SERVICE, EMERGENCY_ONLY (limited service), or NORMAL_SERVICE */
    SimatGmmServiceStatus status;
    Boolean               locInfoPresent;
    /** Location info */
    LocalLocationInfo     locInfo;
    /** Indicates if measurement report is present */
    Boolean               measReportPresent;
    /** Encoded according to 44.018 section 10.5.2.20 */
    Int8                  encodedMeasReport[ENCODED_MEAS_RESULTS_IE_SIZE];
    /** Indicates the number of BCCH ARFCN */
    Int8                  numBcchArfcn;
    /** Lists the BCCH ARFCN */
    Arfcn                 bcchArfcn[MAX_BA_CHANNELS];
    /** Indicates if timing advane is present */
    Boolean               timingAdvancePresent;
    /** Indicates if ME is in idle mode */
    Boolean               inIdleMode;
    /** Determines timing advance (only available when the ME is connected
     * to a GSM access network)
     */
    Int8                  timingAdvance;
}
 AlsaNetworkMeasCnf;


 /** This signal is used to indicate tht timer is started/stoped or
 * to request return of the valaue of a given timer.
 * Indication that the ME is to start/stop a timer or that the SIM
 * requests the ME to return the value of a given timer.
 * The action to perform is indicated by the command qualifier.
 */
typedef struct AlsaTimerManagementIndTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /** The comprehension required */
    Boolean                       partialComprehen;
    /** The missing information */
    Boolean                       missingInformation;
    /** Command qualifier */
    SimatTimerManQual             commandQual;
    /** Identifier the timer */
    SimatTimerIdentifier          timerId;
    /** Indicates if timer value in second is present */
    Boolean                       timerValInSecsPresent;
    /** Timer value in second */
    Int32                         timerValInSecs;
}
AlsaTimerManagementInd;

/** Returns the response data for the Timer Management command. */
typedef struct AlsaTimerManagementRspTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /**  Identifies the outcome of the command to be relayed to the SIM. */
    SimatGeneralResult            generalResult;
    /** Identifier the timer */
    SimatTimerIdentifier          timerId;
    /** Timer value in seconds */
    Int32                         timerValInSecs;
    /** Indicates whether a problem was encountered with the ME in
     * attempting to perform the proactive command.
     */
    Boolean                       meProblemPresent;
    /** Provide  additional information why ME currently unable to process command*/
    SimatMeProblemAddInfo         meProblem;
}
AlsaTimerManagementRsp;

/** Indicates to set up idle mode text. */
typedef struct AlsaSetUpIdleModeTextIndTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /** The comprehension required */
    Boolean                       partialComprehen;
    /** The missing information */
    Boolean                       missingInformation;
    /** Whether or not to remove the existing idle mode text */
    Boolean                       removeExistingIdleModeText;
    /** Text string displayed in idle mode */
    SimatTextString               textString;
    /** Whether or not icon data available */
    Boolean                       iconIdPresent;
    /** Icon Id data */
    SimatIconIdentifier           iconId;
    /** if text formatting information for the text string above was included
     * in the proactive command
\assoc PRESENT \ref textAttributes */
    Boolean                       textAttsPresent;
    /** text formatting attributes */
    SimatTextAttributes           textAttributes;
}
AlsaSetUpIdleModeTextInd;

/** Confirmation to the corresponding request */
typedef struct AlsaSetUpIdleModeTextRspTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /**  Identifies the outcome of the command to be relayed to the SIM. */
    SimatGeneralResult            generalResult;
    /** Indicates whether a problem was encountered with the ME in
     * attempting to perform the proactive command.
     */
    Boolean                       meProblemPresent;
    /** Provide  additional information why ME currently unable to process command*/
    SimatMeProblemAddInfo         meProblem;
}
AlsaSetUpIdleModeTextRsp;

/** Used by the SIM to send an AT command to the ME, as though
 * initiated by an attached terminal.
 */
typedef struct AlsaRunAtCommandIndTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /** The comprehension required */
    Boolean                       partialComprehen;
    /** The missing information */
    Boolean                       missingInformation;
    /** Indicates whether an alpha identifier is included in the signal */
    Boolean                       alphaIdPresent;
    /** Alpha identifier for the proactive command */
    SimatAlphaIdentifier          alphaId;
    /** AT command */
    SimatAtCommandData            atCommand;
    /** Indicates whether the SIM has requested the ME to display an icon */
    Boolean                       iconIdPresent;
    /** Icon Id data */
    SimatIconIdentifier           iconId;
    /** if text formatting information for the text string above was included
     * in the proactive command
\assoc PRESENT \ref textAttributes */
    Boolean                       textAttsPresent;
    /** text formatting attributes */
    SimatTextAttributes           textAttributes;
}
AlsaRunAtCommandInd;

/** This signal is sent as response to AlsaRunAtCommandInd.
 * Returns the response data for the Run AT Command Proactive SIM command.
 */
typedef struct AlsaRunAtCommandRspTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /**  Identifies the outcome of the command to be relayed to the SIM. */
    SimatGeneralResult            generalResult;
    /** AT response */
    SimatAtCommandData            atResponse;
    /** Indicates whether a problem was encountered with the ME in
     * attempting to perform the proactive command.
     */
    Boolean                       meProblemPresent;
    /** Provide  additional information why ME currently unable to process command*/
    SimatMeProblemAddInfo         meProblem;
}
AlsaRunAtCommandRsp;

/** this signal is sent when the SEND_DTMF command is received */
typedef struct AlsaSendDtmfIndTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /** The comprehension required */
    Boolean                       partialComprehen;
    /** The missing information */
    Boolean                       missingInformation;
    /** Indicates if Alpha id is present */
    Boolean                       alphaIdPresent;
    /** Alpha identifier for the proactive command */
    SimatAlphaIdentifier          alphaId;
    /** DTMF digits */
    SimatDtmfString               dtmfString;
    /** Whether or not icon data available */
    Boolean                       iconIdPresent;
    /** Icon Id data */
    SimatIconIdentifier           iconId;
    /** if text formatting information for the text string
     * above was included in the proactive command
\assoc PRESENT \ref textAttributes */
    Boolean                       textAttsPresent;
    /** text formatting attributes */
    SimatTextAttributes           textAttributes;
}
AlsaSendDtmfInd;

/** Returns the send DTMF response */
typedef struct AlsaSendDtmfRspTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /**  Identifies the outcome of the command to be relayed to the SIM. */
    SimatGeneralResult            generalResult;
    /** Indicates whether a problem was encountered with the ME in
     * attempting to perform the proactive command.
     */
    Boolean                       meProblemPresent;
    /** Provide  additional information why ME currently unable to process command*/
    SimatMeProblemAddInfo         meProblem;
}
AlsaSendDtmfRsp;

/** Decodes the Language notification proactive command. */
typedef struct AlsaLanguageNotificationIndTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /** The comprehension required */
    Boolean                       partialComprehen;
    /** The missing information */
    Boolean                       missingInformation;
    /** Indicates if language code is present */
    Boolean                       languageCodePresent;
    /** Language code */
    Int8                          languageCode[SIMAT_NUM_LANG_CODE_CHARS];
}
AlsaLanguageNotificationInd;

/** Confirmation to the corresponding request */
typedef struct AlsaLanguageNotificationRspTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /**  Identifies the outcome of the command to be relayed to the SIM. */
    SimatGeneralResult            generalResult;
    /** Indicates whether a problem was encountered with the ME in
     * attempting to perform the proactive command.
     */
    Boolean                       meProblemPresent;
    /** Provide  additional information why ME currently unable to process command*/
    SimatMeProblemAddInfo         meProblem;
}
AlsaLanguageNotificationRsp;

/** Timer expiry request*/
typedef struct AlsaTimerExpirationReqTag
{
    /** Command reference */
    Int16                         simatCommandRef;
    /** Identifies ID */
    SimatTimerIdentifier          timerId;
    /** Timer value in seconds */
    Int32                         timerValInSecs;
}
AlsaTimerExpirationReq;

/** Sets up timers for the SIM */
typedef struct AlsaTimerExpirationCnfTag
{
    /** Command reference */
    Int16                         simatCommandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus              requestStatus;
}
AlsaTimerExpirationCnf;

/** Icon Identifier support for reading Image instance data from SIM */
typedef struct AlsaReadImageRecReqTag
{
    /** Command reference */
    Int16                         simatCommandRef;
    /** Record of EF-IMG file containing details of all instances of one graphical image */
    Int8                          recordNum;
}
AlsaReadImageRecReq;

/** Confirmation of success or failure for reading Image instance data from SIM */
typedef struct AlsaReadImageRecCnfTag
{
    /** Command reference */
    Int16                         simatCommandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus              requestStatus;
    /** Record of IMG LF file containing image instance data */
    Int8                          recordNum;
    /** Read image */
    SimImageData                  image;
}
AlsaReadImageRecCnf;

/** Reads an instance of the Icon Image  from SIM card */
typedef struct AlsaReadImageInstDataReqTag
{
    /** Command reference */
    Int16                         simatCommandRef;
    /** Indentifies EF */
    SimEfId                       efId;
    /** Start point of data in efId */
    Int16                         offset;
    /** Max 256 bytes per efId */
    Int16                         length;
}
AlsaReadImageInstDataReq;

/** This primitive is sent as a response to AlsaReadImageInstDataReq */
typedef struct AlsaReadImageInstDataCnfTag
{
    /** Command reference */
    Int16                         simatCommandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus              requestStatus;
    /** Length of data read
\assoc ARRAY \ref data */
    Int16                         dataLength;
    /** Read image */
    SimImageInstData              data;
}
AlsaReadImageInstDataCnf;

/** Lists all the instances of the Icon Image  from SIM card */
typedef struct AlsaListImageRecReqTag
{
    /** Command reference */
    Int16                         simatCommandRef;
}
AlsaListImageRecReq;

/** This signal returns the image records. This primitive is
 * sent as a response to AlsaListImageRecReq */
typedef struct AlsaListImageRecCnfTag
{
    /** Command reference */
    Int16                         simatCommandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus              requestStatus;
    /** Number of returned records */
    Int8                          listSize;
}
AlsaListImageRecCnf;

/** Requests a browser inside a browser enabled terminal to interpret the content
 * corresponding to an URL
 */
typedef struct AlsaLaunchBrowserIndTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /** The comprehension required */
    Boolean                       partialComprehen;
    /** The missing information */
    Boolean                       missingInformation;
    /** Command qualifier for the SIMAT launch browser */
    SimatLaunchBrowserQual        commandQual;
    /** Indicates if browser Id is present */
    Boolean                       browserIdPresent;
    /** Browser identity */
    Int8                          browserId;
    /** The terminal shall request content using this URL */
    SimatUrl                      url;
    /** Indicates if bearer is present */
    Boolean                       bearerPresent;
    /** The length of bearer list */
    Int8                          bearerListSize;
    /** Specifies the type of bearer */
    SimatBearer                   bearer[SIMAT_MAX_NUM_BEARERS];
    /** Indicates if Provisioning File Reference data is present */
    Boolean                       provFRDataPresent; /* added for job104951 */
    /** Provisioning file reference data */
    SimatProvFileRefData          provFRData;        /* added for job104951 */
    /** Indicates if text string is present */
    Boolean                       textStringPresent;
    /** Gives to the terminal the name/identity of the Gateway/Proxy to be used
     * for connecting to the URL
     */
    SimatTextString               textString;
    /** Indicates if ID is present */
    Boolean                       alphaIdPresent;
    /** Alpha identifier for the proactive command */
    SimatAlphaIdentifier          alphaId;
    /** if text formatting information for the text string above was included
     * in the proactive command
\assoc PRESENT \ref textAttributes */
    Boolean                       textAttsPresent;
    /** text formatting attributes */
    SimatTextAttributes           textAttributes;
    /** Whether or not icon data available */
    Boolean                       iconIdPresent;
    /** Icon Id data */
    SimatIconIdentifier           iconId;
}
AlsaLaunchBrowserInd;

/** The TERMINAL RESPONSE for the LAUNCH BROWSER command  */
typedef struct AlsaLaunchBrowserRspTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /**  Identifies the outcome of the command to be relayed to the SIM. */
    SimatGeneralResult            generalResult;
    /** Indicates whether a problem was encountered with the ME in
     * attempting to perform the proactive command.
     */
    Boolean                       meProblemPresent;
    /** Provide  additional information why ME currently unable to process command*/
    SimatMeProblemAddInfo         meProblem;
    /** Indicates if there is a problem launching browser */
    Boolean                       browserProblemPresent;
    /** Problem code */
    SimatLauBrProblemAddInfo      browserProblem;
}
AlsaLaunchBrowserRsp;

/** This command is issued by the UICC to request a channel opening.
 * requests the terminal to open a data channel with parameters indicated in the
 * command (if class E is supported)
*/
typedef struct AlsaOpenChannelIndTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /** The comprehension required */
    Boolean                       partialComprehen;
    /** The missing information */
    Boolean                       missingInformation;
    /** Command qualifier for the SIMAT open channel */
    SimatOpenChannelQual          commandQual;
    /** Indicates if alpha Id is present */
    Boolean                       alphaIdPresent;
    /** Alpha identifier for the proactive command */
    SimatAlphaIdentifier          alphaId;
    /** if text formatting information for the text string above was included
     * in the proactive command
\assoc PRESENT \ref textAttributes */
    Boolean                       textAttsPresent;
    /** text formatting attributes */
    SimatTextAttributes           textAttributes;
    /** Whether or not icon data available */
    Boolean                       iconIdPresent;
    /** Icon Id data */
    SimatIconIdentifier           iconId;
    /** Indicates if address is present */
    Boolean                       addressPresent;
    /**  Provides information to the ME necessary to identify the local device */
    SimatAddress                  address;
    /** Indicates if called party sub address is present */
    Boolean                       cpsPresent;
    /** Called party sub address.
     * If the subaddress is not present, the terminal shall not provide a called party
     * subaddress to the network.
     */
    CalledPartySubaddress         cps;
    /** Indicates if duration1 is present */
    Boolean                       duration1Present;
    /** Indicates the duration of reconnection tries. */
    Int32                         duration1InMillisecs;
    /** Indicates if duration2 is present */
    Boolean                       duration2Present;
    /** Indicates the timeout value before the terminal releases the
     * link if there is no data exchanged on the link.
     */
    Int32                         duration2InMillisecs;
    /** Describes the bearer */
    SimatBearerDescription        bearerDesc;
    /** Buffer size */
    Int16                         bufferSize;

/** \assoc PRESENT \ref accessPointAddr */
    Boolean                       accessPointAddrPresent;
    SimatUrl                      accessPointAddr;
    /** Indicates if local address parameter is present */
    Boolean                       otherLocalAddrPresent;
    /** Specifies the local address type
\assoc UNION \ref otherLocalAddr */
    SimatOtherAddressType         otherLocalAddrType;
    /**  Provides information to the ME necessary to identify the local device */
    SimatOtherAddress             otherLocalAddr;
    /** Whether or not user logging is present */
    Boolean                       userLoginPresent;
    /** User logging */
    SimatTextString               userLogin;
    /** Whether or not user password is present */
    Boolean                       userPasswordPresent;
    /** User password */
    SimatTextString               userPassword;
    /** If the UICC/ME interface transport level is present */
    Boolean                       transportLevelPresent;
    /** The UICC/ME interface transport level */
    SimatTransportLevel           transportLevel;
    /** Indicates if destination address is present
\assoc PRESENT \ref dataDestAddr */
    Boolean                       dataDestAddrPresent;
    /** Specifies the type of other data destination address
\assoc UNION \ref dataDestAddr */
    SimatOtherAddressType         dataDestAddrType;
    /** Other data destination address */
    SimatOtherAddress             dataDestAddr;
}
AlsaOpenChannelInd;

/** This signal is sent as a response to AlsaOpenChannelInd. */
typedef struct AlsaOpenChannelRspTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /**  Identifies the outcome of the command to be relayed to the SIM. */
    SimatGeneralResult            generalResult;
    /** Indicates whether a problem was encountered with the ME in
     * attempting to perform the proactive command.
     */
    Boolean                       meProblemPresent;
    /** Provide  additional information why ME currently unable to process command*/
    SimatMeProblemAddInfo         meProblem;
    /** Indicates if network problem is present */
    Boolean                       netProblemPresent;
    /** Determines the network problem */
    GsmCause                      netProblem;
    /** Indicates if SMS problem is present */
    Boolean                       smsProblemPresent;
    /** Determines the SMS problem */
    GsmCause                      smsProblem;
    /** Indicates if bipProblem contains valid data */
    Boolean                       bipProblemPresent;
    /** Indicates the additional information for Bearer Independent Protocol element */
    SimatBipProblemAddInfo        bipProblem;
    /** Channel info */
    SimatChannelInfo              channelInfo;
    /** bearer desctiption */
    SimatBearerDescription        bearerDesc;
    /** Codes the number of bytes requested by the UICC in an OPEN CHANNEL command */
    Int16                         bufferSize;
}
AlsaOpenChannelRsp;

/** Requests the terminal to close the specified data channel */
typedef struct AlsaCloseChannelIndTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /** The comprehension required */
    Boolean                       partialComprehen;
    /** The missing information */
    Boolean                       missingInformation;
    /** Identifies the channel */
    Int8                          channelId;
    /** Indicates if alpha ID is present */
    Boolean                       alphaIdPresent;
    /** Alpha identifier for the proactive command */
    SimatAlphaIdentifier          alphaId;
    /** if text formatting information for the text string above was included
     * in the proactive command
\assoc PRESENT \ref textAttributes */
    Boolean                       textAttsPresent;
    /** text formatting attributes */
    SimatTextAttributes           textAttributes;
    /** Whether or not icon data available */
    Boolean                       iconIdPresent;
    /** Icon Id data */
    SimatIconIdentifier           iconId;
}
AlsaCloseChannelInd;

/** This signal is sent as a response to AlsaCloseChannelInd. */
typedef struct AlsaCloseChannelRspTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /**  Identifies the outcome of the command to be relayed to the SIM. */
    SimatGeneralResult            generalResult;
    /** Indicates if SMS problem is present */
    Boolean                       smsProblemPresent;
    /** Determines the SMS problem */
    GsmCause                      smsProblem;
    /** Indicates if call control problem is present */
    Boolean                       ccProblemPresent;
    /** Determines the call control problem */
    SimatCcProblemAddInfo         ccProblem;
    /** Indicates if bipProblem contains valid data */
    Boolean                       bipProblemPresent;
    /** Indicates the additional information for Bearer Independent Protocol element */
    SimatBipProblemAddInfo        bipProblem;
}
AlsaCloseChannelRsp;

/** Requests the terminal to return to the UICC data received on the specified channel*/
typedef struct AlsaReceiveDataIndTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /** The comprehension required */
    Boolean                       partialComprehen;
    /** The missing information */
    Boolean                       missingInformation;
    /** Identifiers the channel */
    Int8                          channelId;
    /** Indicates if alpha id is present */
    Boolean                       alphaIdPresent;
    /** Alpha identifier for the proactive command */
    SimatAlphaIdentifier          alphaId;
    /** if text formatting information for the text string above was included
     * in the proactive command
\assoc PRESENT \ref textAttributes */
    Boolean                       textAttsPresent;
    /** text formatting attributes */
    SimatTextAttributes           textAttributes;
    /** Whether or not icon data available */
    Boolean                       iconIdPresent;
    /** Icon Id data */
    SimatIconIdentifier           iconId;
    /** Length of channel data */
    Int8                          channelDataLength;
}
AlsaReceiveDataInd;

/** This signal is sent as a response to AlsaReceiveDataInd. */
typedef struct AlsaReceiveDataRspTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /**  Identifies the outcome of the command to be relayed to the SIM. */
    SimatGeneralResult            generalResult;
    /** Indicates if SMS problem is present */
    Boolean                       smsProblemPresent;
    /** Determines the SMS problem */
    GsmCause                      smsProblem;
    /** Indicates if call control problem is present */
    Boolean                       ccProblemPresent;
    /** Determines the call control problem */
    SimatCcProblemAddInfo         ccProblem;
    /** Indicates if bipProblem contains valid data */
    Boolean                       bipProblemPresent;
    /** Indicates the additional information for Bearer Independent Protocol element */
    SimatBipProblemAddInfo        bipProblem;
    /** The remaining data length */
    Int8                          remainingDataLength;
    /** Channel data length */
    Int8                          channelDataLength;
    /** Holds the channel data */
    Int8                          channelData[SIMAT_MAX_CHANNEL_DATA_LENGTH];

}
AlsaReceiveDataRsp;

/** Requests the terminal to send on the specified channel data provided by the UICC */
typedef struct AlsaSendDataIndTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /** The comprehension required */
    Boolean                       partialComprehen;
    /** The missing information */
    Boolean                       missingInformation;
    /** Command qualifier for the SIMAT send data */
    SimatSendDataQual             commandQual;
    /** Identifiers the channel */
    Int8                          channelId;
    /** Indicates if alpha id presents */
    Boolean                       alphaIdPresent;
    /** Alpha identifier for the proactive command */
    SimatAlphaIdentifier          alphaId;
    /** if text formatting information for the text string above was included
     * in the proactive command
\assoc PRESENT \ref textAttributes */
    Boolean                       textAttsPresent;
    /** text formatting attributes */
    SimatTextAttributes           textAttributes;
    /** Whether or not icon data available */
    Boolean                       iconIdPresent;
    /** Icon Id data */
    SimatIconIdentifier           iconId;
    /** The channel data length */
    Int8                          channelDataLength;
    /** Holds the data to be sent */
    Int8                          channelData[SIMAT_MAX_CHANNEL_DATA_LENGTH];
}
AlsaSendDataInd;

/** Returns the alsa send data response */
typedef struct AlsaSendDataRspTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /**  Identifies the outcome of the command to be relayed to the SIM. */
    SimatGeneralResult            generalResult;
    /** Indicates whether a problem was encountered with the ME in
     * attempting to perform the proactive command.
     */
    Boolean                       meProblemPresent;
    /** Provide  additional information why ME currently unable to process command*/
    SimatMeProblemAddInfo         meProblem;
    /** Indicates if SMS problem is present */
    Boolean                       smsProblemPresent;
    /** Determines the SMS problem */
    GsmCause                      smsProblem;
    /** Indicates if call control problem is present */
    Boolean                       ccProblemPresent;
    /** Determines the call control problem */
    SimatCcProblemAddInfo         ccProblem;
    /** Indicates if bipProblem contains valid data */
    Boolean                       bipProblemPresent;
    /** Indicates the additional information for Bearer Independent Protocol element */
    SimatBipProblemAddInfo        bipProblem;
    /** Channel data length */
    Int8                          channelDataLength;
}
AlsaSendDataRsp;

/** This command requests the terminal to return a Channel status
 * data object for each dedicated Channel identifier.
 */
typedef struct AlsaGetChannelStatusIndTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /** The comprehension required */
    Boolean                       partialComprehen;
    /** The missing information */
    Boolean                       missingInformation;
}
AlsaGetChannelStatusInd;

/** Returns the status of all channels */
typedef struct AlsaGetChannelStatusRspTag
{
    /** Command reference */
    Int32                         simatCommandRef;
    /**  Identifies the outcome of the command to be relayed to the SIM. */
    SimatGeneralResult            generalResult;
    /** Indicates if SMS problem is present */
    Boolean                       smsProblemPresent;
    /** Determines the SMS problem */
    GsmCause                      smsProblem;
    /** Indicates if bipProblem contains valid data */
    Boolean                       bipProblemPresent;
    /** Indicates the additional information for Bearer Independent Protocol element */
    SimatBipProblemAddInfo        bipProblem;
    /** Channel status */
    SimatChannelStatus            channelStatus;
}
AlsaGetChannelStatusRsp;

/** This command passes details of a PDP Context to pass to the USIM for permission
 * for checking GPRS Call Control Is enabled */
typedef struct AlsaCcPdpContextReqTag
{
    /** Command reference */
    Int16                         simatCommandRef;
    /** details of PDP Context activation parameters */
    SimatCcPdpContextInfo         pdpContextInfo;
    /** location info */
    SimatLocationInfo             locationInfo;
}
AlsaCcPdpContextReq;

/** This signal returns the result of the PDP Context check by the  USIM when call
 * control for GPRS is enabled  - if it has modified the new  PDP context parameters
 * are supplied in this signal  */
typedef struct AlsaCcPdpContextCnfTag
{
    /** Command reference */
    Int16                         simatCommandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus              requestStatus;
    /** The length of response data */
    SimatCallControlResult        ccResult;
    /** If no response data is available from the SIM (dependent on the ccResult
     * value) the contextInfoPresent field is set to FALSE and context
     * info can be ignored.
     */
    Boolean                       contextInfoPresent;
    /** details of PDP Context activation parameters  supplied by USIM*/
    SimatCcPdpContextInfo         pdpContextInfo;
    /** Indicates if Alpha Id is present */
    Boolean                       alphaIdPresent;
    /** Alpha identifier for the proactive command */
    SimatAlphaIdentifier          alphaId;
}
AlsaCcPdpContextCnf;

/** This command passes details of a PDN Context to pass to the USIM for permission
 * for checking EPS PDN connection parameters */
typedef struct AlsaCcPdnContextReqTag
{
    /** Command reference */
    Int16                         simatCommandRef;
    /** details of PDP Context activation parameters */
    SimatCcPdnContextInfo         pdnContextInfo;
    /** location info */
    SimatLocationInfo             locationInfo;
}
AlsaCcPdnContextReq;

/** This signal returns the result of the PDN Context check by the USIM when call
 * control for EPS PDN is enabled  - if it has modified the new PDN context parameters
 * are supplied in this signal  */
typedef struct AlsaCcPdnContextCnfTag
{
    /** Command reference */
    Int16                         simatCommandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus              requestStatus;
    /** The length of response data */
    SimatCallControlResult        ccResult;
    /** If no response data is available from the SIM (dependent on the ccResult
     * value) the contextInfoPresent field is set to FALSE and context
     * info can be ignored.
     */
    Boolean                       contextInfoPresent;
    /** details of PDP Context activation parameters  supplied by USIM*/
    SimatCcPdnContextInfo         pdnContextInfo;
    /** Indicates if Alpha Id is present */
    Boolean                       alphaIdPresent;
    /** Alpha identifier for the proactive command */
    SimatAlphaIdentifier          alphaId;
}
AlsaCcPdnContextCnf;


typedef struct AlsaInternalStopTimersIndTag
{
  /** Determines the outcome of the request.*/
  Int32         timeoutPeriod;
} AlsaInternalStopTimersInd;

/** @} */ /* End of SigAlsaSignals group */
/** @} */ /* End of SigAlsa group */

#endif
/* END OF FILE */
