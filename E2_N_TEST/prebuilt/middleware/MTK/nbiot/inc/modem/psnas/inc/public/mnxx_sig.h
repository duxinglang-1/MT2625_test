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
 *      Cc, Ss and Cb - application interface
 *      Sms Cm - Rl interface
 **************************************************************************/

#ifndef MNXX_SIG_H
#define MNXX_SIG_H

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

#if !defined (MNL3_TYP_H)
#include     <mnl3_typ.h>
#endif

#if !defined (SITS_TYP_H)
#include     <sits_typ.h>
#endif

#if !defined (L3_TYP_H)
#include     <l3_typ.h>
#endif

/** \defgroup SigMNxx  MNxx Signalling Interface
 * \ingroup Prd3gDmNas
 * \ingroup Sig
 *
 * This section covers signals exchanged between CC, SS and CB - Application layer (AL)
 * interface and SMS-CM - SM-RL interface.
 * This section toutches the MNCC, MNSS, MNCB and MNSM interfaces which are respectively
 * interfaces between: <UL>
 * <LI> CC and Application layer
 * <LI> SS and Application layer
 * <LI> CB and Application layer
 * <LI> SMS-CM and SM-RL layer </UL>
 *
 * <B> The context of the MNxx interface is illustrated in the following diagram.</B>
 * \subsection UsimManagerFigure Figure
 * \image html MMxx_Interface.png "Figure: MNxx Interface"
 * @{
 */
/****************************************************************************
 * Macros
 ****************************************************************************/
/** The maximum length of SMS message */
#define     MAX_SMS_MESSAGE_LENGTH      255

/****************************************************************************
 * Types
 ****************************************************************************/

/** The internal state of a Supplementary Services protocol state machine in the CM sub-layer
*/
typedef enum SsStateTag
{
    /** Idle */
    SS0_IDLE                    =   0,
    /** Connection is pending */
    SS1_MM_CONN_PENDING         =   1,
    /** SS signalling transaction established */
    SS2_CONNECTED               =   2
}
SsState;

/** Indicates CC capability */
typedef enum CcCapabilityBitsTag
{
    /** No capability */
    NO_CAPABILITY               =   0x00,
    /**  DTMF capability */
    DTMF_CAPABILITY             =   0x01
}
CcCapabilityBits;

/**
allocated by the Application or the Protocol Stack
Used by CC and SS
*/
typedef Int16
CallReference;

/** SMS message element */
typedef struct SmsMessageTag
{
    /** Indicates the length of the data
\assoc ARRAY \ref data */
    Int16                           length;
    /** Lists the data contents*/
    Int8                            data[MAX_SMS_MESSAGE_LENGTH];
}
SmsMessage;

/****************************************************************************
 * Signal types
 ****************************************************************************/

/** \defgroup SigMNxxCc  MNCC Primitives
 * Primitives exchanged between CC and AL
 * This section defines primitives exchanged between CC and AL
 *
 * @{
 */

/*****************************************************************************
* AL - CC primitives
*****************************************************************************/

/** Request to send a SETUP or EMERGENCY SETUP message to initiate Mobile
 * originating establishment of either a normal or an emergency call.
 * To originate a call the application layer sends an MnccSetupReq signal to
 * the CC task. This signal must be the first sent using a new callReference.
 * The tv field is not supplied by the application, but is filled in by the CC
 * router function and used within the CC entity.
 * If the application is originating an emergency call then itIsAnEmergencyCall
 * is set TRUE. No optional fields are effective for emergency calls.
 * The called number is mandatory for all calls.
 *
 * If the set up is successful the application receives an MnccCallProcInd.
 * If a call cannot be set up the Application Layer receives either an MnccRejInd
 * or an MnccRelInd.
 */

typedef struct MnccSetupReqTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
#if defined (UPGRADE_3G_FDD) && defined (UPGRADE_REL7)
    /** If the application is originating an emergency call this
     * parameter is set to true.
\assoc PRESENT \ref emergencyServiceCategory */
    Boolean                         itIsAnEmergencyCall;
    /** Emergency category rec 24.008 9.3.8.4
     * This information element shall be included if the emergency category is available from the SIM/USIM or the mobile station.
     * If this information element is included, it shall indicate the selected emergency call category.
     * If the element is not included, the network shall by default assume a non-specific emergency call.
     */
    Int8                            emergencyServiceCategoryPresent;
    Int8                            emergencyServiceCategory;
#else /* !(UPGRADE_3G_FDD) || !(UPGRADE_REL7) */
    /** If the application is originating an emergency call this
     * parameter is set to true.
     */
    Boolean                         itIsAnEmergencyCall;
#endif /* !(UPGRADE_3G_FDD) || !(UPGRADE_REL7) */
    /** Used to identify the called party. The called party BCD number
     * is a type 4 information element with a minimum length of 3 octets
     * and a maximum length of 43 octets. For PCS 1900 the maximum length
     * is 19 octets.
     */
    CalledPartyBcdNum               calledBcdNumber;
    /** Describes a bearer service */
    BearerCapability                bearerServiceReq1;
    /** optional parts
\assoc PRESENT \ref bearerServiceReq2Optional */
    Boolean                         bearerServiceReq2Present;
    /** Indicates if facility information element is present
\assoc PRESENT \ref facilityOptional */
    Boolean                         facilityPresent;
    /** Indicates if calling party sub address is present
\assoc PRESENT \ref callingSubNoOptional */
    Boolean                         callingSubNoPresent;
    /** Indicates if Called party subaddress is present
\assoc PRESENT \ref calledSubNoOptional */
    Boolean                         calledSubNoPresent;
    /** Indicates if low layer compatibility mode1 is present
\assoc PRESENT \ref lowLayerCompat1Optional */
    Boolean                         lowCompatMode1Present;
    /** Indicates if low layer compatibility mode2 is present
\assoc PRESENT \ref lowLayerCompatMode2Optional */
    Boolean                         lowCompatMode2Present;
    /** Indicates if high layer compatibility mode1 is present
\assoc PRESENT \ref highLayerCompat1Optional */
    Boolean                         highCompatMode1Present;
    /** Indicates if high layer compatibility mode2 is present
\assoc PRESENT \ref highLayerCompatMode2Optional */
    Boolean                         highCompatMode2Present;
    /** Indicates if user to user is present
\assoc PRESENT \ref userToUserOptional */
    Boolean                         userToUserPresent;
    /** Indicates if ss version indicator is present
\assoc PRESENT \ref ssVersionIndicatorOptional */
    Boolean                         ssVersionIndicatorPresent;
    /**  Indicate how the associated repeated information elements
     * shall be interpreted.
     */
    RepeatIndication                repeatIndication;
    /** Transports supplementary service related information. */
    Facility                        facilityOptional;
    /** Identifies a subaddress associated with the origin of a call. */
    CallingPartySubaddress          callingSubNoOptional;
    /** Identifies the called party. */
    CalledPartySubaddress           calledSubNoOptional;
    /** Provide a means which should be used for compatibility checking
     * by an addressed entity.
     */
    LowLayerCompatibility           lowLayerCompat1Optional;
    /** The high layer compatibility is a type 4 information element with a
     * minimum length of 2 octets and a maximum length of 5 octets.
     */
    HighLayerCompatibility          highLayerCompat1Optional;
    /** Convey information between the mobile station and the
     * remote ISDN user.*/
    SmallUserUser                   userToUserOptional;
    /** Describe a bearer service */
    BearerCapability                bearerServiceReq2Optional;
    /** Provide a means which should be used for compatibility checking
     * by an addressed entity.
     */
    LowLayerCompatibility           lowLayerCompatMode2Optional;
    /** The high layer compatibility is a type 4 information element with a
     * minimum length of 2 octets and a maximum length of 5 octets.
     */
    HighLayerCompatibility          highLayerCompatMode2Optional;
    /** This specifies the SS version used to encode and decode
     * Supplementary Services Facilities */
    SsVersionIndicator              ssVersionIndicatorOptional;
    /** Indicates if CLIR suppression information element present */
    Boolean                         clirSuppressionPresent;
    /** Indicates if CLIR invocation information element present */
    Boolean                         clirInvocationPresent;
    /** Indicates if call control capabilities present */
    Boolean                         ccCapabilitiesPresent;
    /** Identifies the call control capabilities of the mobile station.*/
    CcCapabilities                  ccCapabilitiesOptional;
    /** Indicates if stream identifier is present */
    Boolean                         streamIdentifierPresent;
    /** represents stream identifier */
    StreamIdentifier                streamIdentifierOptional;
#if defined (UPGRADE_REL5)
    /** Indicates if supported codec is present */
    Boolean                         supportedCodecListPresent;
    /** Provides the network with information about the speech
     * codecs supported by the mobile. */
    SupportedCodecList              supportedCodecListOptional;
#endif
}
MnccSetupReq;

/** The application sends this signal to indicate to the network
 * that the user has answered the call. May be sent directly after
 * an MnccCallConfReq if alerting the called party is not required.
 */
typedef struct MnccSetupRspTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
    /** Indicates if facility information element is present
\assoc PRESENT \ref facilityOptional */
    Boolean                         facilityPresent;
    /** Indicates if connected subadress is present
\assoc PRESENT \ref connectedSubaddressOptional */
    Boolean                         connectedSubaddressPresent;
    /** Indicates if user to user is present
\assoc PRESENT \ref userToUserOptional */
    Boolean                         userToUserPresent;
    /** Indicates if ss version indicator is present
\assoc PRESENT \ref ssVersionIndicatorOptional */
    Boolean                         ssVersionIndicatorPresent;
    /** Transports supplementary service related information. */
    Facility                        facilityOptional;
    /** Identifies a subaddress associated with the connected party of a call.*/
    ConnectedSubaddress             connectedSubaddressOptional;
    /** Used to convey information between the mobile station and
     * the remote ISDN user.
     */
    UserUserElement                 userToUserOptional;
    /** This specifies the SS version used to encode and decode
     * Supplementary Services Facilities */
    SsVersionIndicator              ssVersionIndicatorOptional;
    /** Indicates if stream identifier is present */
    Boolean                         streamIdentifierPresent;
    /** represents stream identifier */
    StreamIdentifier                streamIdentifierOptional;
}
MnccSetupRsp;

/** The MnccSetupInd event is received by the application when a mobile
 * terminated call is being established. The application can treat this
 * signal in a number of ways depending on the caller number, bearer
 * capability etc. To complete the call an MnccCallConfReq must be sent.
 * In addition, if the signalInfoPresent field is TRUE, an MnccAlertReq
 * signal must be sent, as this is early assignment of a traffic channel.
 */
typedef struct MnccSetupIndTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
    /** Describe the current status of a call */
    CallState                       callState;
    /** The TI allows to distinguish up to 16 different
     * bi-directional messages flows for a given PD and a given SAP
     */
    Int8                            transactionId;  /* job 100548 added to pass event download info to STK task */
    /** Used to identify which side of the interface initiated
     * the transaction (a message has a TI flag set to 0 when
     * it belongs to transaction initiated by its sender, and to 1 otherwise.
     */
    Boolean                         tiFlag;
    /** Indicates if facility information element is present
\assoc PRESENT \ref facilityOptional */
    Boolean                         facilityPresent;
    /** Indicates if calling party sub address is present
\assoc PRESENT \ref callingSubNoOptional */
    Boolean                         callingSubNoPresent;
    /** Indicates if Called party subaddress is present
\assoc PRESENT \ref calledSubNoOptional */
    Boolean                         calledSubNoPresent;
    /** Indicates if low layer compatibility mode1 is present
\assoc PRESENT \ref lowLayerCompatMode1Optional */
    Boolean                         lowCompatMode1Present;
    /** Indicates if high layer compatibility mode1 is present
\assoc PRESENT \ref highLayerCompatMode1Optional */
    Boolean                         highCompatMode1Present;
    /** Indicates if low layer compatibility mode2 is present
\assoc PRESENT \ref lowLayerCompatMode2Optional */
    Boolean                         lowCompatMode2Present;
    /** Indicates if high layer compatibility mode2 is present
\assoc PRESENT \ref highLayerCompatMode2Optional */
    Boolean                         highCompatMode2Present;
    /** Indicates if bearer service Req1 is present
\assoc PRESENT \ref bearerSvcReq1Optional */
    Boolean                         bearerSvcReq1Present;
    /** optional part
\assoc PRESENT \ref bearerSvcReq2Optional */
    Boolean                         bearerSvcReq2Present;
    /** Indicates if user to user is present
\assoc PRESENT \ref userToUserOptional */
    Boolean                         userToUserPresent;
    /** Indicates if progress indicator is present.
     * This information element may be included by the network:<ul>
     * <li> in order to pass information about the call in progress,
     * e.g., in the event of interworking; and/or
     * <li> to make the mobile station attach the user connection
     * for speech </ul>

\assoc PRESENT \ref progressIndicatorOptional */
    Boolean                         progressIndicatorPresent;
    /** Indicates if signal information is present.
\assoc PRESENT \ref signalOptional */
    Boolean                         signalInfoPresent;
    /** Indicates if Calling party BCD number is present
\assoc PRESENT \ref callingBcdNoOptional */
    Boolean                         callingBcdNoPresent;
    /** Indicates if Called party BCD number is present
\assoc PRESENT \ref calledBcdNoOptional */
    Boolean                         calledBcdNoPresent;
    /** Indicates if Redirecting party BCD number is present */
    Boolean                         redirectingPartyBcdNumPresent;
    /** Indicates if Redirecting party subaddress is present */
    Boolean                         redirectingPartySubAddrPresent;
    /** Indicates if priority level is present */
    Boolean                         priorityLevelPresent;
    /** Indicates if alerting pattern is present */
    Boolean                         alertingPatternPresent;
    /** Indicates if Cause of No CLI  is present */
    Boolean                         causeOfNoCliPresent;
    /** Indicates if network call control capabilities is present */
    Boolean                         networkCallControlCapabilitiesPresent;
#if defined (UPGRADE_REL5)
    Boolean                         backupBearerSvcReqPresent;
#endif
    /**  Indicate how the associated repeated information elements
     * shall be interpreted.
     */
    RepeatIndication                repeatIndication;
    /** Transports supplementary service related information. */
    Facility                        facilityOptional;
    /** Identifies a subaddress associated with the origin of a call. */
    CallingPartySubaddress          callingSubNoOptional;
    /** Identifies the subaddress of the called party of a call. */
    CalledPartySubaddress           calledSubNoOptional;
    /** Provide a means which should be used for compatibility checking
     * by an addressed entity.
     */
    LowLayerCompatibility           lowLayerCompatMode1Optional;
    /** The high layer compatibility is a type 4 information element with a
     * minimum length of 2 octets and a maximum length of 5 octets.
     */
    HighLayerCompatibility          highLayerCompatMode1Optional;
    /** Provide a means which should be used for compatibility checking
     * by an addressed entity.
     */
    LowLayerCompatibility           lowLayerCompatMode2Optional;
    /** The high layer compatibility is a type 4 information element with a
     * minimum length of 2 octets and a maximum length of 5 octets.
     */
    HighLayerCompatibility          highLayerCompatMode2Optional;
    /** Describes a bearer service */
    BearerCapability                bearerSvcReq1Optional;
    /** Describes a bearer service */
    BearerCapability                bearerSvcReq2Optional;
    /** Describes an event which has occurred during the life of a call.*/
    ProgressIndicator               progressIndicatorOptional;
    /** Signal information allows the network to convey information
     * to a user regarding tones and alerting signals
     */
    SignalInfo                      signalOptional;
    /** Identifies the origin of a call */
    CallingPartyBcdNum              callingBcdNoOptional;
    /** Used to identify the called party. The called party BCD number
     * is a type 4 information element with a minimum length of 3 octets
     * and a maximum length of 43 octets. For PCS 1900 the maximum length
     * is 19 octets.
     */
    CalledPartyBcdNum               calledBcdNoOptional;
    /** Convey information between the mobile station and the
     * remote ISDN user.*/
    SmallUserUser                   userToUserOptional;
    /** Identifies the redirecting party */
    RedirectingPartyBcdNum          redirectingPartyBcdNum;
    /** Identifies a subaddress associated with the redirecting party*/
    RedirectingPartySubAddress      redirectingPartySubAddress;
    /** Provide information defining the priority level requested or applied */
    PriorityLevel                   priorityLevel;
    /** Allows the network to convey information related to the alert to
     * be used by the MS. */
    AlertingPattern                 alertingPattern;
    /** Provides the mobile station the detailed reason why Calling party
     * BCD number is not notified.
     */
    CauseOfNoCli                    causeOfNoCliOptional;
    /** Included by the network to indicate its call control capabilities
     * if the network supports multicall and there are no other ongoing calls
     * to the MS.
     */
    NetworkCallControlCapabilities  networkCallControlCapabilitiesOptional;
#if defined (UPGRADE_REL5)
    /** Indicates a requested service to a MS in case a complete description
     * of the bearer service by a bearer capability IE is not available
     */
    BackupBearerCapability          backupBearerSvcReqOptional;
#endif
}
MnccSetupInd;

/** This signal informs the application that, for a mobile originated call,
 * a channel has been assigned and may be connected. The call is active so
 * the data/speech path may be connected.
 */
typedef struct MnccSetupCnfTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
    /** Describe the current status of a call */
    CallState                       callState;
    /** The TI allows to distinguish up to 16 different
     * bi-directional messages flows for a given PD and a given SAP
     */
    Int8                            transactionId;  /* job 100548 added to pass event download info to STK task */
    /** Used to identify which side of the interface initiated
     * the transaction (a message has a TI flag set to 0 when
     * it belongs to transaction initiated by its sender, and to 1 otherwise.
     */
    Boolean                         tiFlag;
    /** Indicates if facility information element is present
\assoc PRESENT \ref facilityOptional */
    Boolean                         facilityPresent;
    /** Indicates if progress indicator is present.
     * This information element may be included by the network:<ul>
     * <li> in order to pass information about the call in progress,
     * e.g., in the event of interworking; and/or
     * <li> to make the mobile station attach the user connection
     * for speech </ul>

\assoc PRESENT \ref progressIndicatorOptional */
    Boolean                         progressIndicatorPresent;
    /** Indicates if connected number is present
\assoc PRESENT \ref connectedNumberOptional */
    Boolean                         connectedNumberPresent;
    /** Indicates if connected subadress is present
\assoc PRESENT \ref connectedSubaddressOptional */
    Boolean                         connectedSubaddressPresent;
    /** Indicates if user to user is present
\assoc PRESENT \ref userToUserOptional */
    Boolean                         userToUserPresent;
    /** Transports supplementary service related information. */
    Facility                        facilityOptional;
    /** Describes an event which has occurred during the life of a call.*/
    ProgressIndicator               progressIndicatorOptional;
    /** Identifies the connected party of a call.*/
    ConnectedNumber                 connectedNumberOptional;
    /** Identifies a subaddress associated with the connected party of a call.*/
    ConnectedSubaddress             connectedSubaddressOptional;
    /** Convey information between the mobile station and the
     * remote ISDN user.*/
    UserUserElement                 userToUserOptional;
}
MnccSetupCnf;

/** This signal informs the application that, for a mobile terminated call,
 * a channel has been assigned and may be connected. The call is active
 * so the data/speech path may be connected.
 */

typedef struct MnccSetupCompleteIndTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
    /** Describe the current status of a call */
    CallState                       callState;
}
MnccSetupCompleteInd;

/** The application may reject an incoming call due to some incompatibility.
 * Request to reject a Mobile terminated call if the call is refused or if
 * the call cannot be accepted, e.g., because of missing compatibility.
 *
 * This signal is sent in response to the setup indication. */
typedef struct MnccRejReqTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
    /** Describe the reason for rejecting an incoming call. */
    Cause                           cause;
    /** Indicates if facility information element is present
\assoc PRESENT \ref facilityOptional */
    Boolean                         facilityPresent;
    /** Indicates if user to user is present
\assoc PRESENT \ref userToUserOptional */
    Boolean                         userToUserPresent;
    /** Indicates if ss version indicator is present
\assoc PRESENT \ref ssVersionIndicatorOptional */
    Boolean                         ssVersionIndicatorPresent;
    /** Transports supplementary service related information. */
    Facility                        facilityOptional;
    /** Convey information between the mobile station and the
     * remote ISDN user.*/
    UserUserElement                 userToUserOptional;
    /** This specifies the SS version used to encode and decode
     * Supplementary Services Facilities */
    SsVersionIndicator              ssVersionIndicatorOptional;
}
MnccRejReq;

/*
 * 0206-18596 added facilityPresent and facilityOptional
 * and created new type for MnssRejInd
 */

/** If a connection for a mobile originating call cannot be provided
 * the application receives this signal.
 * Indication that the Mobile originated call has been rejected, e.g.
 * if the MM connection cannot be provided or if the call establishment
 * initiation has been rejected by the network.
 */
typedef struct MnxxRejIndTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
    /** Describe the current status of a call */
    CallState                       callState;
    /** Describe the reason for generating this messages. */
    Cause                           cause;
    /** Indicates if facility information element is present */
    Boolean                         facilityPresent;
    /** Transports supplementary service related information. */
    Facility                        facilityOptional;
}
MnccRejInd;

/** The application confirms a compatible incoming call by sending
 * this signal. An alternative bearer capability may be offered to
 * the remote station.*/
typedef struct MnccCallConfReqTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
    /** Indicates if bearer service Req1 is present
\assoc PRESENT \ref bearerSvcReq1Optional */
    Boolean                         bearerSvcReq1Present;
    /** Indicates if bearer service Req2 is present
\assoc PRESENT \ref bearerSvcReq2Optional */
    Boolean                         bearerSvcReq2Present;
    /**  Indicate how the associated repeated information elements
     * shall be interpreted.
     */
    RepeatIndication                repeatIndication;
    /** Describe the reason for sending this message. */
    Cause                           cause;
    /** Describes a bearer service */
    BearerCapability                bearerSvcReq1Optional;
    /** Describes a bearer service */
    BearerCapability                bearerSvcReq2Optional;
    /** Indicates if call control capabilities present */
    Boolean                         ccCapabilitiesPresent;
    /** Identifies the call control capabilities of the mobile station.*/
    CcCapabilities                  ccCapabilitiesOptional;
    /** Indicates if stream identifier is present */
    Boolean                         streamIdentifierPresent;
    /** Represents stream identifier */
    StreamIdentifier                streamIdentifierOptional;
#if defined (UPGRADE_REL5)
    /** Indicates if supported codec is present */
    Boolean                         supportedCodecListPresent;
    /** Provides the network with information about the speech
     * codecs supported by the mobile. */
    SupportedCodecList              supportedCodecListOptional;
#endif
}
MnccCallConfReq;

/** Indication that the network has initiated call establishment.
 * Indication to the Mobile originating user that call establishment
 * has been initiated in the Network and no more call establishment
 * information will be accepted by the Network.
 */
typedef struct MnccCallProceedingIndTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
    /** Describe the current status of a call */
    CallState                       callState;
    /** Indicates if bearer service Req1 is present
\assoc PRESENT \ref bearerSvcReq1Optional */
    Boolean                         bearerSvcReq1Present;
    /** Indicates if bearer service Req2 is present
\assoc PRESENT \ref bearerSvcReq2Optional */
    Boolean                         bearerSvcReq2Present;
    /** Indicates if facility information element is present
\assoc PRESENT \ref facilityOptional */
    Boolean                         facilityPresent;
    /** Indicates if progress indicator is present.
\assoc PRESENT \ref progressIndicatorOptional */
    Boolean                         progressIndicatorPresent;
    /**  Indicate how the associated repeated information elements
     * shall be interpreted.
     */
    RepeatIndication                repeatIndication;
    /** Describes a bearer service */
    BearerCapability                bearerSvcReq1Optional;
    /** Describes a bearer service */
    BearerCapability                bearerSvcReq2Optional;
    /** Transports supplementary service related information. */
    Facility                        facilityOptional;
    /** Describes an event which has occurred during the life of a call.*/
    ProgressIndicator               progressIndicatorOptional;
}
MnccCallProceedingInd;

/** Indication that network interworking is in progress during mobile
 * originated call establishment.
 * Indication to the Mobile user that a PROGRESS message or a message
 * containing a progress IE has been received, e.g., because the call
 * is progressing in the PLMN/ISDN environment, or because the call has
 * left the PLMN/ISDN environment, or because in band tones/announcement
 * are available.
 */
typedef struct MnccCallProgressIndTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
    /** Describe the current status of a call */
    CallState                       callState;
    /** Describes an event which has occurred during the life of a call.*/
    ProgressIndicator               progressIndicator;
    /** Indicates if user to user is present
\assoc PRESENT \ref userToUserOptional */
    Boolean                         userToUserPresent;
    /** Used to convey information between the mobile station and
     * the remote ISDN user.
     */
    UserUserElement                 userToUserOptional;
}
MnccCallProgressInd;

/** Indication to the network that the user is being alerted.
 * Request to send an ALERTING message from the called Mobile
 * user to the remote calling user to indicate that user alerting has
 * been initiated.
 *
 * To be sent after an MnccCallConfReq has been sent.
 */
typedef struct MnccAlertReqTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
    /** Indicates if facility information element is present
\assoc PRESENT \ref facilityOptional */
    Boolean                         facilityPresent;
    /** Indicates if user to user is present
\assoc PRESENT \ref userToUserOptional */
    Boolean                         userToUserPresent;
    /** Indicates if ss version indicator is present
\assoc PRESENT \ref ssVersionIndicatorOptional */
    Boolean                         ssVersionIndicatorPresent;
    /** Transports supplementary service related information. */
    Facility                        facilityOptional;
    /** Used to convey information between the mobile station and
     * the remote ISDN user.
     */
    UserUserElement                 userToUserOptional;
    /** This specifies the SS version used to encode and decode
     * Supplementary Services Facilities */
    SsVersionIndicator              ssVersionIndicatorOptional;
}
MnccAlertReq;

/** Received during mobile originating call establishment
 * when the remote station has begun to alert the user.
 * Indication of the receipt of an ALERTING message,
 * alerting to the remote called user has been initiated.
 */
typedef struct MnccAlertIndTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
    /** Describe the current status of a call */
    CallState                       callState;
    /** Indicates if facility information element is present
\assoc PRESENT \ref facilityOptional */
    Boolean                         facilityPresent;
    /** Indicates if progress indicator is present.
\assoc PRESENT \ref progressIndicatorOptional */
    Boolean                         progressIndicatorPresent;
    /** Indicates if user to user is present
\assoc PRESENT \ref userToUserOptional */
    Boolean                         userToUserPresent;
    /** Transports supplementary service related information. */
    Facility                        facilityOptional;
    /** Describes an event which has occurred during the life of a call.*/
    ProgressIndicator               progressIndicatorOptional;
    /** Used to convey information between the mobile station and
     * the remote ISDN user.
     */
    UserUserElement                 userToUserOptional;
}
MnccAlertInd;

/** Used to send information to the remote user regarding the current call.
* The notification can be suspend, resume or bearer change.
* Send information pertaining to a call, such as user suspended, to
* the Network by the Mobile user.
*/
typedef struct MnccNotifyReqTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
    /** Indicate information pertaining to a call */
    NotificationDescrip             notificationDescrip;
}
MnccNotifyReq;

/** Information from the remote user regarding the current call.
 * Indication to the Mobile user that information pertaining to
 * a call, such as remote user suspended, has been received from the Network.
*/
typedef struct MnccNotifyIndTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
    /** Describe the current status of a call */
    CallState                       callState;
    /** Indicate information pertaining to a call */
    NotificationDescrip             notificationDescrip;
}
MnccNotifyInd;

/** This primitive send a DISCONNECT message to the Network
 * in order to clear the end to end connection.
 * Sent once only by the application to disconnect the call.
 *
 * May not be sent directly after receiving an MnccSetupInd or after
 * receiving an MnccDiscInd.
 */
typedef struct MnccDiscReqTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
    /** Describe the reason for sending this message. */
    Cause                           cause;
    /** Indicates if facility information element is present
\assoc PRESENT \ref facilityOptional */
    Boolean                         facilityPresent;
    /** Indicates if user to user is present
\assoc PRESENT \ref userToUserOptional */
    Boolean                         userToUserPresent;
    /** Indicates if ss version indicator is present
\assoc PRESENT \ref ssVersionIndicatorOptional */
    Boolean                         ssVersionIndicatorPresent;
    /** Transports supplementary service related information. */
    Facility                        facilityOptional;
    /** Used to convey information between the mobile station and
     * the remote ISDN user.
     */
    UserUserElement                 userToUserOptional;
    /** This specifies the SS version used to encode and decode
     * Supplementary Services Facilities */
    SsVersionIndicator              ssVersionIndicatorOptional;
}
MnccDiscReq;

/** Received by the application after the remote user has initialised clearing.
 * Indication of reception of a DISCONNECT message, by which the Network indicates
 * that the end to end connection is cleared.
 */

typedef struct MnccDiscIndTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
    /** Describe the current status of a call */
    CallState                       callState;
    /** Describe the reason for sending this message. */
    Cause                           cause;
    /** Indicates if facility information element is present
\assoc PRESENT \ref facilityOptional */
    Boolean                         facilityPresent;
    /** Indicates if progress indicator is present.
\assoc PRESENT \ref progressIndicatorOptional */
    Boolean                         progressIndicatorPresent;
    /** Indicates if user to user is present
\assoc PRESENT \ref userToUserOptional */
    Boolean                         userToUserPresent;
    /** Transports supplementary service related information. */
    Facility                        facilityOptional;
    /** Describes an event which has occurred during the life of a call.*/
    ProgressIndicator               progressIndicatorOptional;
    /** Used to convey information between the mobile station and
     * the remote ISDN user.
     */
    UserUserElement                 userToUserOptional;
}
MnccDiscInd;

/** Sent by the application to inform (G)MM and the network
 * that they can release their connections.
 * This primitive send a RELEASE message to inform the Network that the user
 * intends to release the call reference and the corresponding MM connection so
 * that the Network can release its MM connection and the correspondent call reference.
 *
 * Sent after receiving a disc ind.
 */

typedef struct MnccRelReqTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
    /** Indicates if facility information element is present
\assoc PRESENT \ref facilityOptional */
    Boolean                         facilityPresent;
    /** Indicates if user to user is present
\assoc PRESENT \ref userToUserOptional */
    Boolean                         userToUserPresent;
    /** Indicates if ss version indicator is present
\assoc PRESENT \ref ssVersionIndicatorOptional */
    Boolean                         ssVersionIndicatorPresent;
    /** Describe the reason for sending this message. */
    Cause                           cause;
    /** Transports supplementary service related information. */
    Facility                        facilityOptional;
    /** Used to convey information between the mobile station and
     * the remote ISDN user.
     */
    UserUserElement                 userToUserOptional;
    /** This specifies the SS version used to encode and decode
     * Supplementary Services Facilities */
    SsVersionIndicator              ssVersionIndicatorOptional;
}
MnccRelReq;

/** Indicates that a RELEASE message has been received and the Network
 * intends to release its MM connection. Indication to the application that
 * it should dispose of the call reference and disconnect the data/speech path,
 * as the call no longer exists.
 */
typedef struct MnccRelIndTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
    /** Describe the current status of a call */
    CallState                       callState;
    /** Indicates if facility information element is present
\assoc PRESENT \ref facilityOptional */
    Boolean                         facilityPresent;
    /** Indicates if user to user is present
\assoc PRESENT \ref userToUserOptional */
    Boolean                         userToUserPresent;
    /** Describe the reason for initiating call clearing */
    Cause                           cause1;
    /** Describe the Abnormal cases for initiating call clearing */
    Cause                           cause2;
    /** Transports supplementary service related information. */
    Facility                        facilityOptional;
    /** Used to convey information between the mobile station and
     * the remote ISDN user.
     */
    UserUserElement                 userToUserOptional;
}
MnccRelInd;

/** Confirmation of the Mobile user's request to release the MM
 * connection and call reference in the Network.
 * Indication to the application that it should dispose of the
 * call reference and disconnect the data/speech path.
 * Received only if the application requested the release.
 */
typedef struct MnccRelCnfTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
    /** Describe the current status of a call */
    CallState                       callState;
    /** Indicates if facility information element is present
\assoc PRESENT \ref facilityOptional */
    Boolean                         facilityPresent;
    /** Indicates if user to user is present
\assoc PRESENT \ref userToUserOptional */
    Boolean                         userToUserPresent;
    /** Reason for the release */
    Cause                           cause1;
    /** Additional reason for the release */
    Cause                           cause2;
    /** Transports supplementary service related information. */
    Facility                        facilityOptional;
    /** Used to convey information between the mobile station and
     * the remote ISDN user.
     */
    UserUserElement                 userToUserOptional;
}
MnccRelCnf;

/** Request to transport a facility IE for a call related supplementary service invocation.
 * Send facility information for call related supplementary services.
*/
typedef struct MnccFacilityReqTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
    /** Transports supplementary service related information. */
    Facility                        facility;
    /** Indicates if ss version indicator is present
\assoc PRESENT \ref ssVersionIndicatorOptional */
    Boolean                         ssVersionIndicatorPresent;
    /** This specifies the SS version used to encode and decode
     * Supplementary Services Facilities */
    SsVersionIndicator              ssVersionIndicatorOptional;
}
MnccFacilityReq;

/** Indication that a facility IE for a call related supplementary service
 * invocation has been received.Receive facility information for call related
 * supplementary services. */
typedef struct MnccFacilityIndTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
    /** Describe the current status of a call */
    CallState                       callState;
    /** Transports supplementary service related information. */
    Facility                        facility;
}
MnccFacilityInd;

/** Send a DTMF signal to the remote station.
 * Request to send a START DTMF message in order to start a DTMF control operation.
 */
typedef struct MnccStartDtmfReqTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
    /** Contains the digit the network should reconvert back into a DTMF tone
     * which is then applied towards the remote user.*/
    DtmfCharacter                   dtmfCharacter;
}
MnccStartDtmfReq;

/** Received after the DTMF tone has been initiated.
 * The application must time the tone from the receipt of this signal.
 *
 * If the dtmfPresent field is FALSE then the tone has not been sent and
 * the causeOptional field is present.
 *
 * DTMF operation is only available for active calls. The application must
 * maintain its own guard timer for the receipt of this confirmation.
 */
typedef struct MnccStartDtmfCnfTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
    /** Describe the current status of a call */
    CallState                       callState;
    /** Indicates if dtmfCharacterOptional is valid */
    Boolean                         dtmfPresent;
    /** Contains the digit the network should reconvert back into a DTMF tone
     * which is then applied towards the remote user.*/
    DtmfCharacter                   dtmfCharacterOptional; /* if dtmfPresent */
    /** If the dtmfPresent field is FALSE then the tone has not been sent and
     * the causeOptional field is present.
     */
    Cause                           causeOptional;         /* if !dtmfPresent */
}
MnccStartDtmfCnf;

/** After the required time the application sends this signal to stop
 * the sounding of the remote DTMF tone.
 */
typedef struct MnccStopDtmfReqTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
}
MnccStopDtmfReq;

/** Received after the DTMF signal has been stopped.
 * Upon receiving the STOP DTMF message the network will stop sending the
 * DTMF tone and return a STOP DTMF ACKNOWLEDGE message to the mobile station.
 *
 * The application must maintain its own guard timer for the receipt of this confirmation.
 */

typedef struct MnccStopDtmfCnfTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
    /** Describe the current status of a call */
    CallState                       callState;
    /** indicates ACKNOWLEDGEMENT */
    Boolean                         success;
}
MnccStopDtmfCnf;

/** Sent to start the mobile originating in call modification procedure.
 * Modification is dependent on the repeat indicator in the original setup signals.
 */
typedef struct MnccModifyReqTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
    /** Describes a bearer service */
    BearerCapability                bearerCapability;
    /** Indicates if low layer compatibility is present
\assoc PRESENT \ref lowLayerCompatibility */
    Boolean                         lowLayerCompatibilityPresent;
    /** Provide a means which should be used for compatibility checking
     * by an addressed entity.
     */
    LowLayerCompatibility           lowLayerCompatibility;
    /** Indicates if high layer compatibility is present
\assoc PRESENT \ref highLayerCompatibility */
    Boolean                         highLayerCompatibilityPresent;
    /** The high layer compatibility is a type 4 information element with a
     * minimum length of 2 octets and a maximum length of 5 octets.
     */
    HighLayerCompatibility          highLayerCompatibility;
    /** Indicates that the direction of the data call to which the
     * MODIFY message relates is opposite to the call setup direction.
     */
    Boolean                         reverseCallSetupDirectionPresent;
#if defined (UPGRADE_3G_FDD) && defined (UPGRADE_REL6)
    /* CR-860 Correction of the network initiated in-call modification
    ** Rec 24.008 9.3.13 Modify
    ** The field immediateModificationIndicatorPresent does not exist anymore
    */
#else /* !(UPGRADE_3G_FDD) || !(UPGRADE_REL6) */
    /** Indicates if immediate modification indicator is present */
    Boolean                         immediateModificationIndicatorPresent;
#endif /* !(UPGRADE_3G_FDD) || !(UPGRADE_REL6) */
}
MnccModifyReq;


/** Received after the in-call modification procedure has been completed by the network.
 * The Mobile originating in call modification has been completed.
*/
typedef struct MnccModifyCnfTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
    /** Describe the current status of a call */
    CallState                       callState;
    /** Describes a bearer service */
    BearerCapability                bearerCapability;
    Cause                           cause;  /**< NO_CAUSE or cause if reject */
    /** Indicates if low layer compatibility is present
\assoc PRESENT \ref lowLayerCompatibility */
    Boolean                         lowLayerCompatibilityPresent;
    /** Indicates if high layer compatibility is present
\assoc PRESENT \ref highLayerCompatibility */
    Boolean                         highLayerCompatibilityPresent;
    /** Provide a means which should be used for compatibility checking
     * by an addressed entity.
     */
    LowLayerCompatibility           lowLayerCompatibility;
    /** The high layer compatibility is a type 4 information element with a
     * minimum length of 2 octets and a maximum length of 5 octets.
     */
    HighLayerCompatibility          highLayerCompatibility;
    /** Indicates that the direction of the data call to which the
     * MODIFY message relates is opposite to the call setup direction.
     */
    Boolean                         reverseCallSetupDirectionPresent;
}
MnccModifyCnf;

/** Received when the mobile terminating in-call modification procedure
 * has been completed by the network.
 */
typedef struct MnccModifyIndTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
    /** Describe the current status of a call */
    CallState                       callState;
    /** Describes a bearer service */
    BearerCapability                bearerCapability;
    /** Indicates if low layer compatibility is present
\assoc PRESENT \ref lowLayerCompatibility */
    Boolean                         lowLayerCompatibilityPresent;
    /** Indicates if high layer compatibility is present
\assoc PRESENT \ref highLayerCompatibility */
    Boolean                         highLayerCompatibilityPresent;
    /** Provide a means which should be used for compatibility checking
     * by an addressed entity.
     */
    LowLayerCompatibility           lowLayerCompatibility;
    /** The high layer compatibility is a type 4 information element with a
     * minimum length of 2 octets and a maximum length of 5 octets.
     */
    HighLayerCompatibility          highLayerCompatibility;
    /** Indicates that the direction of the data call to which the
     * MODIFY message relates is opposite to the call setup direction.
     */
    Boolean                         reverseCallSetupDirectionPresent;
#if defined (UPGRADE_3G_FDD) && defined (UPGRADE_REL6)
    /* CR-860 Correction of the network initiated in-call modification
    ** Rec 24.008 9.3.13 Modify
    ** The field immediateModificationIndicatorPresent does not exist anymore
    */
#else /* !(UPGRADE_3G_FDD) || !(UPGRADE_REL6) */
    /** Indicates if immediate modification indicator is present */
    Boolean                         immediateModificationIndicatorPresent;
#endif /* !(UPGRADE_3G_FDD) || !(UPGRADE_REL6) */
}
MnccModifyInd;

/** Response to send a MODIFY COMPLETE  message to indicate Mobile
 * terminating in call modification completion by the Mobile user.
 * When the AL receives an MnccSyncInd (Channel Mode Modified), it must switch
 * in the new channel circuit, and start using this new circuit (e.g. fax) instead
 * of the old one (e.g. speech). The MnccModifyRsp triggers the sending of a MODIFY
 * COMPLETE message that tells the network that the MS will now start using the newly
 * modified channel. (The network then transmits a MODIFY COMPLETE message to the remote MS). */
typedef struct MnccModifyRspTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
    /** Describes a bearer service */
    BearerCapability                bearerCapability;
    /**
    NO_CAUSE if compatible or cause if reject
    */
    Cause                           cause;
    /** Indicates if low layer compatibility is present
\assoc PRESENT \ref lowLayerCompatibility */
    Boolean                         lowLayerCompatibilityPresent;
    /** Indicates if high layer compatibility is present
\assoc PRESENT \ref highLayerCompatibility */
    Boolean                         highLayerCompatibilityPresent;
    /** Provide a means which should be used for compatibility checking
     * by an addressed entity.
     */
    LowLayerCompatibility           lowLayerCompatibility;
    /** The high layer compatibility is a type 4 information element with a
     * minimum length of 2 octets and a maximum length of 5 octets.
     */
    HighLayerCompatibility          highLayerCompatibility;
    /** Indicates that the direction of the data call to which the
     * MODIFY message relates is opposite to the call setup
     * direction (No reverse call setup must be present if reject).
     */
    Boolean                         reverseCallSetupDirectionPresent;
}
MnccModifyRsp;

/** Received when a dedicated channel assignment has been performed and/or
 * the channel mode has been changed.
 */
typedef struct MnccSyncIndTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
    /** Describe the current status of a call */
    CallState                       callState;
    /** This is present in RR_SYNC_IND, MMCC_SYNC_IND and MNCC_SYNC_IND signals
     * to tell upper layers about events at the radio resource ((G)RR) sub-layer.
     */
    SyncCause                       syncCause;
}
MnccSyncInd;

/** Sent by the application to initiate call holding.
 * This primitive requests call holding.
 *
 * \note Hold signal is not defined in GSM.
 */
typedef struct MnccHoldReqTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
}
MnccHoldReq;

/** Sent by the application to initiate the retrieval of a held call.
 * This primitive requests the retrieval of a held call.
 *
 * \note This signal is not defined in GSM.
 */
typedef struct MnccRetrieveReqTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
}
MnccRetrieveReq;

/** Received by the application to indicate a successful hold (cause value is NO_CAUSE.)
 * This primitive is sent as a responce to MnccHoldReq.
 * When the cause value is not NO_CAUSE MnccHoldCnf indicates an unsuccessful hold request.
 */
typedef struct MnccHoldCnfTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
    /** Describe the current status of a call */
    CallState                       callState;
    /** Indicates the results of  hold request */
    Cause                           cause;
}
MnccHoldCnf;

/** Received by the application to indicate a successful retrieval
 * (cause value is NO_CAUSE).
 * This primitive is sent as a responce to MnccRetrieveReq.
 * When the cause value is not NO_CAUSE MnccRetrieveCnf indicates an unsuccessful
 * retrieve request.
 */
typedef struct MnccRetrieveCnfTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
    /** Describe the current status of a call */
    CallState                       callState;
    /** Indicates the results of retrieve request */
    Cause                           cause;
}
MnccRetrieveCnf;

/** This signal is sent when status enquiry is received from the network */
typedef struct MnccStatusRspTag
{
   /** The call reference is provided by the Application Layer and must be between
    * zero and the number of tvs (the range is 0-6).
    */
   CallReference                   callReference;
   /** The cause to send this signal */
   Cause                           causeToSend;
   /** Multi-party Auxiliary State */
   MptyAuxiliaryState              mptyState;
   /** Hold state */
   HoldAuxiliaryState              holdState;
}
MnccStatusRsp;

typedef struct MnccStatusIndReq
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
   CallReference                   callReference;
   /** The cause to send this signal */
   Cause                           causeToSend;
}
MnccStatusInd;

/** @} */ /* End of SigMNxxCc group */
/*****************************************************************************
* AL - SS primitives
*****************************************************************************/
/** \defgroup SigMNxxSs  MNSS Primitives
 * Primitives exchanged between SS and AL
 * This section defines primitives exchanged between SS and AL
 *
 * @{
 */

/*
 * 0206-18596 created new type for MnssRejInd, because MnccRejInd changed
 */
/** If a connection for a mobile originating supplementary services transaction cannot
 * be provided the application receives this signal.
 */
typedef struct MnssRejIndTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
    /** Describe the current status of a call */
    CallState                       callState;
    /** Indicates the cause */
    GsmCause                        cause;
}
MnssRejInd;

/** Sent by the application to establish a transaction for call independent supplementary services.
 * Request to send a REGISTER message in order to establish a signalling transaction for the provision
  * of call independent supplementary services. The request for a call independent supplementary service
  * invocation may be included.
  */
typedef struct MnssBeginReqTag
{
   /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
    /** Transports supplementary service related information. */
    Facility                        facility;
    /** Indicates if ss version indicator is present
\assoc PRESENT \ref ssVersionIndicatorOptional */
    Boolean                         ssVersionIndicatorPresent;
    /** This specifies the SS version used to encode and decode
     * Supplementary Services Facilities */
    SsVersionIndicator              ssVersionIndicatorOptional;
}
MnssBeginReq;

/** Received by the application to establish a transaction for call
 * independent supplementary services.
 * Receipt of a REGISTER message, a signalling transaction is established for
 * the provision of call independent supplementary services after receipt of a
 * REGISTER message. The indication of a supplementary service invocation may be included.
 */
typedef struct MnssBeginIndTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (the range is 0-6).
     */
    CallReference                   callReference;
    /** Describe the current status of a call */
    SsState                         callState;
    /** Transports supplementary service related information. */
    Facility                        facility;
}
MnssBeginInd;

/** Sent by the application to request a supplementary service transaction.
 * Request to send a FACILITY message for the provision of a call independent
 *  supplementary service invocation.
 */
typedef struct MnssFacilityReqTag
{
    /**
    call ref from BeginReq (al) or BeginInd (ps)
    */
    CallReference                   callReference;
    /** Transports supplementary service related information. */
    Facility                        facility;
}
MnssFacilityReq;

/** Received by the application to indicate a supplementary service transaction.
 * This primitive indicates the receipt of a FACILITY message for a call
 * independent supplementary service invocation.
 */
typedef struct MnssFacilityIndTag
{
    /**
    call ref from BeginReq (al) or BeginInd (ps)
    */
    CallReference                   callReference;
    /** Describe the current status of a call */
    SsState                         callState;
    /** Transports supplementary service related information. */
    Facility                        facility;
}
MnssFacilityInd;

/** Sent by the application to initiate the release of a supplementary
 * services signalling transaction.
 * This primitive send a RELEASE COMPLETE message in order to release the
 * signalling transaction. The request for transfer of a supplementary service
 * facility may be included.
 */
typedef struct MnssEndReqTag
{
    /**
    call ref from BeginReq (al) or BeginInd (ps)
    */
    CallReference                   callReference;
    /** Indicates if facility information element is present
\assoc PRESENT \ref facilityOptional */
    Boolean                         facilityPresent;
    /** Indicates the cause of release */
    Cause                           cause;
    /** Transports supplementary service related information. */
    Facility                        facilityOptional;
}
MnssEndReq;

/** Received by the application after a supplementary services transaction has
 * been released.  When this is received by the AL, it indicates that the
 * relevant transaction must be treated as finished, and its call reference
 * may be re-used. This may be due to normal or abnormal release in the protocol
 *  stack. The reason is given in the cause field.
 */
typedef struct MnssEndIndTag
{
    /** The call reference is provided by the Application Layer and must be between
     * zero and the number of tvs (from BeginReq (al) or BeginInd (ps))
     */
     CallReference                   callReference;
    /** Describe the current status of a call */
    SsState                         callState;
    /** Indicates if facility information element is present
\assoc PRESENT \ref facilityOptional */
    Boolean                         facilityPresent;
    /** Indicates the cause of release */
    Cause                           cause;
    /** Transports supplementary service related information. */
    Facility                        facilityOptional;
}
MnssEndInd;

/** @} */ /* End of SigMnxxSs group */

/*****************************************************************************
* AL - SM primitives
*****************************************************************************/
/** \defgroup SigMNxxSm  MNSM Primitives
 * Primitives exchanged between SM and AL
 * This section defines primitives exchanged between SM and AL
 *
 * @{
 */

/** The MnsmEstReq signal indicates a request by the SM-RL task to establish a
 * CM-connection for a new Mobile Originated (MO) Short Message (SM).
 * The smsMessage field in this case will hold an RP-DATA or RP-SMMA data unit.
 */
typedef struct MnsmEstReqTag
{
    /** Identifies the entity */
   Int8                         entityNum;
   /** Holds an RP-DATA or RP-SMMA data unit. */
   SmsMessage                   smsMessage;
   SmsRoute                     smsRoute;  /**< sms routing type e.g. GPRS only, CS_preferred  */

}
MnsmEstReq;

/** This signal is an indication from the SM-CM task that a CM-connection now exists.
 * This primitive indicates that a CM-connection now exists for a new incoming
 * Mobile Terminated (MT) SM.
 */
typedef struct MnsmEstIndTag
{
    /** Identifies the entity */
   Int8                         entityNum;
   /** Hold an RP-DATA data unit. */
   SmsMessage                   smsMessage;
   Boolean                      smsCipher;  /**< MT SMS arrived via GPRS or CS */
}
MnsmEstInd;

/** The MnsmDataReq signal is a request from the SM-RL task for the SM-CM task
 * to send the given RPDU (the smsMessage field) over the established CM-connection.
 * The RPDU will either be an RP-ACK or an RP-ERROR.
 */
typedef struct MnsmDataReqTag
{
    /** Identifies the entity */
   Int8                         entityNum;
   /** Holds either be an RP-ACK or an RP-ERROR.*/
   SmsMessage                   smsMessage;
}
MnsmDataReq;

/** The MnsmDataInd signal is an indication from the SM-CM task that
 * contains either an RP-ACK or RP-ERROR RPDU.
 */
typedef struct MnsmDataIndTag
{
    /** Identifies the entity */
   Int8                         entityNum;
   /** contains either an RP-ACK or RP-ERROR RPDU. */
   SmsMessage                   smsMessage;
   Boolean                      smsCipher; /**< ciphering state of MO SMS */
}
MnsmDataInd;

/** The MnsmRelReq signal is used by the SM-RL task to tell the SM-CM task
 * to release the CM-connection for this entity. Once this signal is sent the
 * entity corresponding to the given number no longer exists in the SM-RL task.
 */
typedef struct MnsmRelReqTag
{
    /** Identifies the entity */
   Int8                         entityNum;
   /** Indicates what caused the release. This will normally be set to CAUSE_RP_NO_ERROR.*/
   GsmCause                     cause;
}
MnsmRelReq;

/** The MnsmErrInd signal is used by the SM-CM task to indicate to the
 * SM-RL task that an error has occurred while trying to transport a short
 * message. This signal implies the release of the CM-connection and the deletion
 * of the corresponding entities.
 */
typedef struct MnsmErrIndTag
{
    /** Identifies the entity */
   Int8                         entityNum;
   /** Indicates what type of error occurred */
   GsmCause                     cause;
   Boolean                      smsCipher; /**< ciphering state of MO SMS         */
}
MnsmErrInd;

/** The MnsmAbortReq signal is used by SM-RL task to tell the SM-CM task
 * to release the CM-connection in abnormal cases. The entity corresponding
 * to the given entity number will no longer exist in the SM-RL task after this
 * signal has been sent.
 */
typedef struct MnsmAbortReqTag
{
    /** Identifies the entity */
   Int8                         entityNum;
   /** Indicates what caused the abort request to be sent.*/
   GsmCause                     cause;
}
MnsmAbortReq;


/*job100559 AL signals its readiness or otherwise to receive*/
/** The MnsmConfigReq is a non-GSM defined signal that is used to indicate to
 * the SMS tasks that the AL is ready to receive SMS.
 */
typedef struct MnsmConfigReqTag
{
    Boolean                     isReady;   /**< TRUE=> AL is ready to receive*/
} MnsmConfigReq;

/** @} */ /* End of MNxxSm group */

/** Indicates GSMS Route */
typedef enum SmsRouteCnfTag
{
    /**  GSMS Sent in circuit switched mode */
    GSMS_VIA_CS,
    /**  GSMS Sent in GPRS mode */
    GSMS_VIA_GPRS
#if defined (UPGRADE_3G)
    /**  GSMS Sent in UGPRS or EPS (LTE) mode */
    ,GSMS_VIA_UGPRS_OR_EPS
#endif
}
SmsRouteCnf;

/** @} */ /* End of SigMMxx group */
#endif
/* END OF FILE */
