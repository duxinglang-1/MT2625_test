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
 *      Types common to mmxx_sig.h grr_sig.h and mmxx_sig.h
 **************************************************************************/

#ifndef MNL3_TYP_H
#define MNL3_TYP_H

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

#if !defined (SIL3_TYP_H)
#include     <sil3_typ.h>
#endif

#if !defined (SML3_TYP_H)
#include     <sml3_typ.h>
#endif

#if !defined (L23_TYP_H)
#include     <l23_typ.h>
#endif

/****************************************************************************
 * Macros
 ****************************************************************************/

#define     MAX_FACILITY_DATA_LENGTH         (L2_MESSAGE_SIZE - 2)
                                             /* 2 for the GSM_HEADER_SIZE.  It's */
                                             /* not possible to just include     */
                                             /* L3COMMON.H for GSM_HEADER_SIZE   */
                                             /* as it is not supplied            */
                                             /* to object code customers and this*/
                                             /* header gets included in a number */
                                             /* of Application Layer background  */
                                             /* files (absh_lcl.c and anrm*.c)   */
                                             /* which are supplied in source form*/
/*The MAX_CALLED_BCD_NO_LENGTH is 80 for GSM/DCS/UMTS, but only
 * 32 for PCS. Number will be truncated if too long for PCS
 * See also MAX_CALLED_PARTY_BCD_IE_SIZE in L3COMMON.H */
#define     MAX_CALLED_BCD_NO_LENGTH        80
#define     MAX_CALLED_BCD_NO_LENGTH_PCS    32
#define     MAX_CALLING_BCD_NO_LENGTH       20
#define     MAX_REDIRECTING_BCD_NO_LENGTH   32

#define     MAX_USER_TO_USER_LENGTH_SMALL   32 /* Applies to Setup message which */
                                               /* is a type TLV ranging from     */
                                               /* 3-35. Where V is the protocol  */
                                               /* descriptor (ie. 35-3=32)       */
#define     MAX_USER_TO_USER_IE_SMALL       33

#define     MAX_CAUSE_DIAGNOSTIC_LENGTH     28

/* Maximum supported pause separator in ATD command */
#define     MAX_PAUSE_SEPARATOR             10

#define     PD_GSMID_TO_L3MSGTYPE(p,i)  (((p) * 256) + (i))

#if defined (UPGRADE_REL5)
#if defined (UPGRADE_3G)
#define     MAX_NUM_SUPPORTED_CODEC_LIST    2   /* Num of RAT which should not exceed 3 */
#else
#define     MAX_NUM_SUPPORTED_CODEC_LIST    1   /* Num of RAT which should not exceed 3 */
#endif
#define     MAX_NUM_SUPPORTED_CODEC_TYPE    8
#define     MAX_NUM_CODEC_BITMAP_OCTETS     2
#define     MAX_NUM_CODEC_BITMAP_TYPES      14

#define     POLICE_EMERGENCY_SERVICE            0x01
#define     AMBULANCE_EMERGENCY_SERVICE         0x02
#define     FIRE_BRIGADE_EMERGENCY_SERVICE      0x04
#define     MARINE_GUARD_EMERGENCY_SERVICE      0x08
#define     MOUNTAIN_RESCUE_EMERGENCY_SERVICE   0x10

#endif
/****************************************************************************
 * Types
 ****************************************************************************/
/** This is present in RR_SYNC_IND, MMCC_SYNC_IND and MNCC_SYNC_IND signals to tell upper
 * layers about events at the radio resource ((G)RR) sub-layer
 */
typedef enum SyncCauseTag
{
    /** Change of the ciphering configuration */
    SYNC_CIPHERING_MODE_SET,
    /** Assignment of resources. */
    SYNC_RESOURCE_ASSIGNED,
    /** SYNC_MODE_MODIFIED will arrive (via MM) when RR sends back a
      * CHANNEL MODE MODIFY ACKNOWLEDGE. This sync can arrive when the
      * MS is doing Mobile Terminated Call Modify (the MS has sent back a
      * MODIFY COMPLETE, but the network has not yet assigned or modified
      * the radio channel. When this happens, the sync will occur.
      * SYNC_MODE_MODIFIED is also received in very early assignment
      * at some time during the CC signalling - pass on to the user
      * so that the speech connection can be made, and then use signalling info
      * from that point on to mute and unmute the speech.
      */
    SYNC_MODE_MODIFIED,
    /** Integrity setting and it is only required in WCDMA mode. */
    SYNC_INTEGRITY_PROTECTION_SET,
    /** SAPI 3 has been released */
    SYNC_SAPI3_RELEASED,
    /** Used to start T303 and it is only required in WCDMA mode. */
    SYNC_CM_SERVICE_REQ_SENT,
    /** Connection is released and it is only required in WCDMA mode. */
    SYNC_RESOURCE_RELEASED
}
SyncCause;

/** Indicates the type of diagnostic if any */
typedef enum DiagnosticsEnumTag
{
    /** No diagnostic available */
    NO_DIAGNOSTICS,
    /** Raw data */
    RAW,
    /** Type of L3 message */
    L3_MESSAGE_TYPE,
    /** Number of members in diagnostic (must be last) */
    NumberOfMembersInDiagnosticsUnion
}
DiagnosticsEnum;

/** Dual Tone Multi Frequency code set.*/
typedef enum DtmfCharacterTag
{
    /** Digit 0 */
    DTMF_0                      =   48,
    /** Digit 1 */
    DTMF_1                      =   49,
    /** Digit 2 */
    DTMF_2                      =   50,
    /** Digit 3 */
    DTMF_3                      =   51,
    /** Digit 4 */
    DTMF_4                      =   52,
    /** Digit 5 */
    DTMF_5                      =   53,
    /** Digit 6 */
    DTMF_6                      =   54,
    /** Digit 7 */
    DTMF_7                      =   55,
    /** Digit 8 */
    DTMF_8                      =   56,
    /** Digit 9 */
    DTMF_9                      =   57,
    /** A */
    DTMF_A                      =   65,
    /** B */
    DTMF_B                      =   66,
    /** C */
    DTMF_C                      =   67,
    /** D */
    DTMF_D                      =   68,
    /** * */
    DTMF_STAR                   =   42,
    /** # */
    DTMF_HASH                   =   35
}
DtmfCharacter;

/** Signal value elements */
typedef enum SignalInfoTag
{
    /** dial tone on */
    SIG_DIAL_TONE_ON            =   0,
    /** ring back tone on */
    SIG_RING_BACK_TONE_ON       =   1,
    /** intercept tone on*/
    SIG_INTERCEPT_TONE_ON       =   2,
    /** network congestion tone on */
    SIG_NET_CONG_TONE_ON        =   3,
    /** busy tone on */
    SIG_BUSY_TONE_ON            =   4,
    /** confirm tone on */
    SIG_CONFIRM_TONE_ON         =   5,
    /** answer tone on */
    SIG_ANSWER_TONE_ON          =   6,
    /** call waiting tone on */
    SIG_CALL_WAITING_TONE_ON    =   7,
    /** off-hook warning tone on */
    SIG_OFFHK_WARN_TONE_ON      =   8,
    /** tones off */
    SIG_TONES_OFF               =   63,
    /** alerting off */
    SIG_ALERTING_OFF            =   79
}
SignalInfo;

/** The purpose of the repeat indicator information element is
 * to indicate how the associated repeated information elements
 * shall be interpreted, when included in a message. The repeat
 * indicator information element is included immediately before
 * the first occurrence of the associated information element which
 * will be repeated in a message
 */
typedef enum RepeatIndicationTag
{
    /** No repeat */
    RI_NO_REPEAT                =   0,
    /** Circular for successive selection (mode 1 alternate mode 2)*/
    RI_CIRCULAR                 =   1,
    /** Support of fallback (mode 1 preferred, mode 2 selected if setup of mode 1 fails) */
    RI_FALLBACK                 =   2,
    /** According to 24.008-3b0 10.5.4.22, `3' is now reserved.  Leave
     * the old enumerator to avoid breaking old code. */
    RI_SEQUENTIAL               =   3
}
RepeatIndication;

/** Notification indicator information element */
typedef enum NotificationDescripTag
{
    /** Indicates user is suspended */
    NOTIF_USER_SUSPENDED        =   0,
    /** Indicates user is resumed*/
    NOTIF_USER_RESUMED          =   1,
    /** Indicates bearer is changed */
    NOTIF_BEARER_CHANGE         =   2
}
NotificationDescrip;

/** Call state*/
typedef enum CallStateTag /* 04.08: 10.5.4.6 */
{
    /** No call exists */
    U0_NULL                     =   0,
    /** MM connection pending */
    U0_1_MM_CONN_PENDING        =   2,
    /** Call initiated          */
    U1_CALL_INITIATED           =   1,
    /** mobile originating call proceeding */
    U3_MO_CALL_PROCEEDING       =   3,
    /** Call delivered         */
    U4_CALL_DELIVERED           =   4,
    /** call present  */
    U6_CALL_PRESENT             =   6,
    /** call received */
    U7_CALL_RECEIVED            =   7,
    /** connect request */
    U8_CONNECT_REQUEST          =   8,
    /** mobile terminating call confirmed */
    U9_MT_CALL_CONFIRMED        =   9,
    /** active */
    U10_ACTIVE_CC               =   10,
    /** disconnect request */
    U11_DISCONNECT_REQ          =   11,
    /** disconnect indication */
    U12_DISCONNECT_INDICATION   =   12,
    /** release request */
    U19_RELEASE_REQUEST         =   19,
    /** mobile originating modify */
    U26_MO_MODIFY               =   26,
    /** mobile terminating modify */
    U27_MT_MODIFY               =   27,

    /** null */
    N0_NULL                     =   0,
    /** MM connection pending */
    N0_1_MM_CONN_PENDING        =   2,
    /** call initiated  */
    N1_CALL_INITIATED           =   1,
    /** mobile originating call proceeding */
    N3_MO_CALL_PROCEEDING       =   3,
    /** Call delivered         */
    N4_CALL_DELIVERED           =   4,
    /** call present  */
    N6_CALL_PRESENT             =   6,
    /** call received */
    N7_CALL_RECEIVED            =   7,
    /** connect request */
    N8_CONNECT_REQUEST          =   8,
    /** mobile terminating call confirmed */
    N9_MT_CALL_CONFIRMED        =   9,
    /** active */
    N10_ACTIVE_CC               =   10,
    /** disconnect request */
    N11_DISCONNECT_REQ          =   11,
    /** disconnect indication */
    N12_DISCONNECT_INDICATION   =   12,
    /** release request */
    N19_RELEASE_REQUEST         =   19,
    /** mobile originating modify */
    N26_MO_MODIFY               =   26,
    /** mobile terminating modify */
    N27_MT_MODIFY               =   27,
    /** connect indication*/
    N28_CONNECT_INDICATION      =   28,
    /** Assignment pending */
    Na_ASSIGNMENT_PENDING       =   30,
    /** End of CC state */
    endCcState
}
CallState;

/** Coding standard */
typedef enum CodingStandardTag
{
    /** CCITT standardized coding */
    CODING_CCITT_Q931           =   0,
    /** Reserved for other international standards */
    CODING_OTHER_INTERNATL      =   1,
    /** National standard */
    CODING_NATIONAL             =   2,
    /** Standard defined for the network (either public or private)
      * present on the network side of the interface */
    CODING_GSM_NETWORK          =   3
}
CodingStandard;

/** High layer interpretation. */
typedef enum HlcInterpretationTag
{
    /** First (primary or only) high layer characteristics identification
     * (in octet 4) to be used in the call
     */
    USE_FIRST_HLC_ID            =   4
}
HlcInterpretation;

/** Presentation method element */
typedef enum PresentationMethodTag
{
    /** High layer protocol profile (without specification of attributes)*/
    HIGH_LAYER_PROTOCOL_PROFILE =   1
}
PresentationMethod;

/** High layer characteristics identification*/
typedef enum HighCharacteristicsIdTag
{
    /** Telephony */
    HLC_G711                    =   1,
    /** Facsimile Group 2/3 */
    HLC_T62                     =   4,
    /** Facsimile Group 4 Class I */
    HLC_T503                    =   33,
    /** Facsimile service Group 4, Classes II ad III */
    HLC_T501                    =   36,
    /** This codepoint was previously allocated for an
     * F.200-series Recommendation that has been deleted
     */
    HLC_T502                    =   40,
    /** HLC_T62_T70 */
    HLC_T62_T70                 =   49,
    /** Syntax based Videotex */
    HLC_T504                    =   50,
    /** Telex service */
    HLC_TELEX                   =   53,
    /** Message Handling Systems (MHS)*/
    HLC_X400                    =   56,
    /** OSI application */
    HLC_X200                    =   65,
    /** Reserved for management */
    HLC_MAINTENANCE             =   94,
    /** Reserved for management */
    HLC_MANAGEMENT              =   95
}
HighCharacteristicsId;

/** Presentation indicator (octet 3a)*/
typedef enum PresentationIndicatorTag
{
    /** Presentation allowed */
    PRES_ALLOWED                =   0,
    /** Presentation restricted */
    PRES_RESTRICTED             =   1,
    /** Number not available due to interworking */
    PRES_NUMBER_NOT_AVAIL       =   2
}
PresentationIndicator;

/** Indicates the type of recommendation */
typedef enum RecommendationTag
{
    /** Recommendation Q931 */
    REC_Q931                    =   0,
    /** Recommendation GSM */
    REC_GSM                     =   1,
    /** Recommendation X21  */
    REC_X21                     =   3,
    /** Recommendation X25  */
    REC_X25                     =   4
}
Recommendation;

/** Screening indicator (octet 3a)*/
typedef enum ScreeningIndicatorTag
{
    /** User-provided, not screened */
    SCREEN_USER_NOT_SCREENED    =   0,
    /** User-provided, verified and passed */
    SCREEN_USER_VERIFIED_PASS   =   1,
    /** User-provided, verified and failed */
    SCREEN_USER_VERIFIED_FAIL   =   2,
    /** Network provided */
    SCREEN_NET                  =   3
}
ScreeningIndicator;
/** Location element (octet 3) */
typedef enum LocationTag
{
    /** User */
    LOC_USER                    =   0,
    /** Private network serving the local user*/
    LOC_PRIV_LOCAL              =   1,
    /** Public network serving the local user*/
    LOC_PUB_LOCAL               =   2,
    /** LOCation transit */
    LOC_TRANSIT                 =   3,
    /** Public network serving the remote user */
    LOC_PUB_REMOTE              =   4,
    /** Private network serving the remote user*/
    LOC_PRIV_REMOTE             =   5,
    /** International */
    LOC_INTERNATIONAL           =   7,
    /** Network beyond interworking point*/
    LOC_BEYOND_IWF              =   10
}
Location;

/** Progress description (octet 4)*/
typedef enum ProgressDescriptionTag
{
    /** Call is not end-to-end PLMN/ISDN, further call
     * progress information may be available in-band */
    PROG_NOT_ENDTOEND_PLMN      =   1,
    /** Destination address in non-PLMN/ISDN */
    PROG_DEST_NON_PLMN          =   2,
    /** Origination address in non-PLMN/ISDN */
    PROG_ORIG_NON_PLMN          =   3,
    /** Call has returned to the PLMN/ISDN */
    PROG_CALL_RET_TO_PLMN       =   4,
    /** In-band information or appropriate pattern now available*/
    PROG_INBAND_INFO_AVAIL      =   8,
    /** Call is end-to-end PLMN/ISDN*/
    PROG_CALL_IS_ENDTOEND       =  32,
    /** Queueing */
    PROG_QUEUEING               =  64
}
ProgressDescription;

/** User-user protocol discriminator */
typedef enum UserProtocolDiscrimTag
{
    /** User specific protocol */
    U_PROTO_SPECIFIC            =   0,
    /** OSI high layer protocols */
    U_PROTO_OSI_HIGHER_LAYER    =   1,
    /** X.244  */
    U_PROTO_X244                =   2,
    /** IA5 characters */
    U_PROTO_IA5_CHARS           =   4,
    /** Rec.V.120 rate adaption  */
    U_PROTO_V120                =   7,
    /** user-network call control messages */
    U_PROTO_Q931                =   8
}
UserProtocolDiscrim;

/** The PD identifies the L3 protocol to which the standard layer 3 message belongs.
 * The correspondence between L3 protocols and PDs is one-to-one.
 */
typedef enum ProtocolDiscriminatorTag
{
#if defined (UPGRADE_LTE)
    /** EPS session management messages */
    PD_ESM                      =   0x02,
#endif /* UPGRADE_LTE */
    /** Call control; call related SS messages */
    PD_CC                       =   0x03,
    /** Used for GPRS Transparent Transport Protocol (GTTP) when DTM supported  */
    PD_GTTP                     =   0x04,
    /** Mobility management messages */
    PD_MM                       =   0x05,
    /** Radio resources management messages */
    PD_RR                       =   0x06,
#if defined (UPGRADE_LTE)
    /** EPS Mobility management messages */
    PD_EMM                      =   0x07,
#endif /* UPGRADE_LTE */
    /** GPRS mobility management messages */
    PD_GMM                      =   0x08, /* used for GPRS only */ /* to be removed after ATCI cleaning */
    /** SMS messages */
    PD_SMS                      =   0x09,
    /** GPRS session management messages (used for GPRS only) */
    PD_SM                       =   0x0A,
    /** Non call related SS messages */
    PD_SS                       =   0x0B, /* FR000136 */
    /** Reserved for tests procedures described in 3GPP TS 44.014 [5a] and 3GPP TS 34.109 [17a]. */
    PD_TEST                     =   0x0F,
    /** Not Used */
    PD_NON                      =   0x7F
}
ProtocolDiscriminator;

/** Hold Auxiliary State element */
typedef enum HoldAuxiliaryStateTag
{
    /** Idle */
    IDLE,
    /** Hold request */
    HOLD_REQUEST,
    /** CAll is held */
    CALL_HELD,
    /** Retrieve the request */
    RETRIEVE_REQUEST
}
HoldAuxiliaryState;
/** Multi-party Auxiliary State element */
typedef enum MptyAuxiliaryStateTag
{
    /** Idle state */
    MPTY_IDLE                   =   0,
    /** Request state */
    MPTY_REQUEST                =   1,
    /** Call in Multi-party Auxiliary State */
    CALL_IN_MPTY                =   2,
    /** Split request */
    SPLIT_REQUEST               =   3
}
MptyAuxiliaryState;

/** This enum is reserved for Non Access Stratum messages only
 * for instance CC, SMS, MM SS,etc */
typedef enum L3MessageTypeTag
{
    /* SMS MESSAGES */
    /** CP-DATA */
    CP_DATA                     = PD_GSMID_TO_L3MSGTYPE (PD_SMS, 0x01),
    /** CP-ACK; used to acknowledge the reception of a CP-DATA message.*/
    CP_ACK                      = PD_GSMID_TO_L3MSGTYPE (PD_SMS, 0x04),
    /** CP-ERROR */
    CP_ERROR                    = PD_GSMID_TO_L3MSGTYPE (PD_SMS, 0x10),

    /* TEST INTERFACE MESSAGES */
    /** Close TCH Loop command */
    CLOSE_TCH_LOOP_CMD          = PD_GSMID_TO_L3MSGTYPE (PD_TEST, 0x00),
    /** Close TCH Loop ACKNOWLEDGE */
    CLOSE_TCH_LOOP_ACK          = PD_GSMID_TO_L3MSGTYPE (PD_TEST, 0x01),
    /** OPEN LOOP COMMAND */
    OPEN_LOOP_CMD               = PD_GSMID_TO_L3MSGTYPE (PD_TEST, 0x06),
    /** OPEN LOOP ACK */
    OPEN_LOOP_ACK               = PD_GSMID_TO_L3MSGTYPE (PD_TEST, 0x07),
    /** ACTIVATE EMMI COMMAND */
    ACT_EMMI_CMD                = PD_GSMID_TO_L3MSGTYPE (PD_TEST, 0x0C),
    /** ACTIVATE EMMI ACKNOWLEDGE */
    ACT_EMMI_ACK                = PD_GSMID_TO_L3MSGTYPE (PD_TEST, 0x0D),
    /** DEACTIVATE EMMI */
    DEACT_EMMI_CMD              = PD_GSMID_TO_L3MSGTYPE (PD_TEST, 0x10),
    /** TEST INTERFACE */
    TEST_INTERFACE              = PD_GSMID_TO_L3MSGTYPE (PD_TEST, 0x14),
#if defined (UPGRADE_3G)
    /** UMTS CLOSE TEST LOOP MODE COMMAND */
    TC_UMTS_CLOSE_TEST_LOOP_MODE_CMD         = PD_GSMID_TO_L3MSGTYPE (PD_TEST, 0x40),
    /** UMTS CLOSE TEST LOOP MODE ACKNOWLEDGE */
    TC_UMTS_CLOSE_TEST_LOOP_MODE_ACK         = PD_GSMID_TO_L3MSGTYPE (PD_TEST, 0x41),
    /** UMTS OPEN TEST LOOP MODE COMMAND */
    TC_UMTS_OPEN_TEST_LOOP_MODE_CMD          = PD_GSMID_TO_L3MSGTYPE (PD_TEST, 0x42),
    /** UMTS OPEN TEST LOOP MODE ACKNOWLEDGE */
    TC_UMTS_OPEN_TEST_LOOP_MODE_ACK          = PD_GSMID_TO_L3MSGTYPE (PD_TEST, 0x43),
    /** UMTS ACTIVATE RADIO BEARER TEST LOOP MODE COMMAND */
    TC_UMTS_ACTIVATE_RB_TEST_LOOP_MODE_CMD   = PD_GSMID_TO_L3MSGTYPE (PD_TEST, 0x44),
    /** UMTS ACTIVATE RADIO BEARER TEST LOOP MODE ACKNOWLEDGE  */
    TC_UMTS_ACTIVATE_RB_TEST_LOOP_MODE_ACK   = PD_GSMID_TO_L3MSGTYPE (PD_TEST, 0x45),
    /** UMTS DEACTIVATE RADIO BEARER TEST LOOP MODE COMMAND */
    TC_UMTS_DEACTIVATE_RB_TEST_LOOP_MODE_CMD = PD_GSMID_TO_L3MSGTYPE (PD_TEST, 0x46),
    /** UMTS DEACTIVATE RADIO BEARER TEST LOOP MODE ACKNOWLEDGE  */
    TC_UMTS_DEACTIVATE_RB_TEST_LOOP_MODE_ACK = PD_GSMID_TO_L3MSGTYPE (PD_TEST, 0x47),
#if defined (UPGRADE_3G_MBMS)
# if defined (UPGRADE_3G_TDD128)
    /** UMTS TEST LOOP MODE 3 RLC SDU COUNTER REQUEST COMMAND */
    TC_UMTS_UE_TEST_LOOP_MODE3_RLC_SDU_COUNTER_REQUEST_CMD = PD_GSMID_TO_L3MSGTYPE (PD_TEST, 0x49),
    /** UMTS TEST LOOP MODE 3 RLC SDU COUNTER RESPONSE ACKNOWLEDGE */
    TC_UMTS_UE_TEST_LOOP_MODE3_RLC_SDU_COUNTER_RESPONSE_ACK = PD_GSMID_TO_L3MSGTYPE (PD_TEST, 0x4A),
# endif /* UPGRADE_3G_TDD128 */
#endif /* UPGRADE_3G_MBMS */
#endif /* UPGRADE_3G */


#if defined (UPGRADE_LTE)
    /** EPS attach request */
    EPS_ATTACH_REQ                                      = PD_GSMID_TO_L3MSGTYPE (PD_EMM, 0x41),
    /** EPS attach accept */
    EPS_ATTACH_ACCEPT                                   = PD_GSMID_TO_L3MSGTYPE (PD_EMM, 0x42),
    /** EPS attach complete */
    EPS_ATTACH_COMPLETE                                 = PD_GSMID_TO_L3MSGTYPE (PD_EMM, 0x43),
    /**EPS attach reject */
    EPS_ATTACH_REJECT                                   = PD_GSMID_TO_L3MSGTYPE (PD_EMM, 0x44),
    /** EPS detach request (Note: Converted from/to internal msg types by L3 encode/decode) */
    EPS_DETACH_REQ                                      = PD_GSMID_TO_L3MSGTYPE (PD_EMM, 0x45),
    /** EPS_detach accept */
    EPS_DETACH_ACCEPT                                   = PD_GSMID_TO_L3MSGTYPE (PD_EMM, 0x46),
    /** Tracking area update request */
    EPS_TRACKING_AREA_UPDATE_REQ                        = PD_GSMID_TO_L3MSGTYPE (PD_EMM, 0x48),
    /** Tracking area update accept */
    EPS_TRACKING_AREA_UPDATE_ACCEPT                     = PD_GSMID_TO_L3MSGTYPE (PD_EMM, 0x49),
    /** Tracking area update complete */
    EPS_TRACKING_AREA_UPDATE_COMPLETE                   = PD_GSMID_TO_L3MSGTYPE (PD_EMM, 0x4A),
    /** Tracking area update reject */
    EPS_TRACKING_AREA_UPDATE_REJECT                     = PD_GSMID_TO_L3MSGTYPE (PD_EMM, 0x4B),
    /** Extended service request */
    EPS_EXTENDED_SERVICE_REQUEST                        = PD_GSMID_TO_L3MSGTYPE (PD_EMM, 0x4C),
    /** CP service request */
    EPS_CONTROL_PLANE_SERVICE_REQUEST                   = PD_GSMID_TO_L3MSGTYPE (PD_EMM, 0x4D),
    /** Service reject */
    EPS_SERVICE_REJECT                                  = PD_GSMID_TO_L3MSGTYPE (PD_EMM, 0x4E),
    /** Service accept */
    EPS_SERVICE_ACCEPT                                  = PD_GSMID_TO_L3MSGTYPE (PD_EMM, 0x4F),
    /** GUTI reallocation command */
    EPS_GUTI_REALLOCATION_COMMAND                       = PD_GSMID_TO_L3MSGTYPE (PD_EMM, 0x50),
    /** GUTI reallocation complete */
    EPS_GUTI_REALLOCATION_COMPLETE                      = PD_GSMID_TO_L3MSGTYPE (PD_EMM, 0x51),
    /** Authentication Request */
    EPS_AUTHENTICATION_REQUEST                          = PD_GSMID_TO_L3MSGTYPE (PD_EMM, 0x52),
    /** Authentication Response */
    EPS_AUTHENTICATION_RESPONSE                         = PD_GSMID_TO_L3MSGTYPE (PD_EMM, 0x53),
    /** Authentication Reject */
    EPS_AUTHENTICATION_REJECT                           = PD_GSMID_TO_L3MSGTYPE (PD_EMM, 0x54),
    /** Authentication Failure */
    EPS_AUTHENTICATION_FAILURE                          = PD_GSMID_TO_L3MSGTYPE (PD_EMM, 0x5C),
    /** Identity request */
    EPS_IDENTITY_REQUEST                                = PD_GSMID_TO_L3MSGTYPE (PD_EMM, 0x55),
    /** Identity response */
    EPS_IDENTITY_RESPONSE                               = PD_GSMID_TO_L3MSGTYPE (PD_EMM, 0x56),
    /** EPS security mode command */
    EPS_SECURITY_MODE_COMMAND                           = PD_GSMID_TO_L3MSGTYPE (PD_EMM, 0x5D),
    /** EPS security mode complete */
    EPS_SECURITY_MODE_COMPLETE                          = PD_GSMID_TO_L3MSGTYPE (PD_EMM, 0x5E),
    /** EPS security mode reject */
    EPS_SECURITY_MODE_REJECT                            = PD_GSMID_TO_L3MSGTYPE (PD_EMM, 0x5F),
    /** EMM status */
    EPS_EMM_STATUS                                      = PD_GSMID_TO_L3MSGTYPE (PD_EMM, 0x60),
    /** EMM Information */
    EPS_EMM_INFORMATION                                 = PD_GSMID_TO_L3MSGTYPE (PD_EMM, 0x61),
    /** Downlink NAS transport */
    EPS_DOWNLINK_NAS_TRANSPORT                          = PD_GSMID_TO_L3MSGTYPE (PD_EMM, 0x62),
    /** Uplink NAS Transport */
    EPS_UPLINK_NAS_TRANSPORT                            = PD_GSMID_TO_L3MSGTYPE (PD_EMM, 0x63),
    /** CS service notification */
    EPS_CS_SERVICE_NOTIFICATION                         = PD_GSMID_TO_L3MSGTYPE (PD_EMM, 0x64),
    /**Downlink Generic NAS Transport   */
    EPS_DOWNLINK_GENERIC_NAS_TRANSPORT                  = PD_GSMID_TO_L3MSGTYPE (PD_EMM, 0x68),
    /** Uplink Generic NAS Transport */
    EPS_UPLINK_GENERIC_NAS_TRANSPORT                    = PD_GSMID_TO_L3MSGTYPE (PD_EMM, 0x69),

    /** EPS detach request - MO (Only used internally to separate MO/MT structures in Genie) */
    EPS_MO_DETACH_REQ                                   = PD_GSMID_TO_L3MSGTYPE (PD_EMM, 0xFC),
    /** EPS detach request - MT (Only used internally to separate MO/MT structures in Genie) */
    EPS_MT_DETACH_REQ                                   = PD_GSMID_TO_L3MSGTYPE (PD_EMM, 0xFD),
    /** Encoded plain NAS message in security protected message (Only used internally) */
    EPS_PLAIN_NAS_MESSAGE                               = PD_GSMID_TO_L3MSGTYPE (PD_EMM, 0xFE),

    /** Activate default EPS bearer context request */
    EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQ         = PD_GSMID_TO_L3MSGTYPE (PD_ESM, 0xC1),
    /** Activate default EPS bearer context accept */
    EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT      = PD_GSMID_TO_L3MSGTYPE (PD_ESM, 0xC2),
    /** Activate default EPS bearer context reject */
    EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJ         = PD_GSMID_TO_L3MSGTYPE (PD_ESM, 0xC3),
    /** Activate dedicated EPS bearer context request */
    EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQ       = PD_GSMID_TO_L3MSGTYPE (PD_ESM, 0xC5),
    /** Activate dedicated EPS bearer context accept */
    EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT    = PD_GSMID_TO_L3MSGTYPE (PD_ESM, 0xC6),
    /** Activate dedicated EPS bearer context reject */
    EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJ       = PD_GSMID_TO_L3MSGTYPE (PD_ESM, 0xC7),
    /** Modify EPS bearer context request */
    EPS_MODIFY_EPS_BEARER_CONTEXT_REQ                   = PD_GSMID_TO_L3MSGTYPE (PD_ESM, 0xC9),
    /** Modify EPS bearer context accept */
    EPS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT                = PD_GSMID_TO_L3MSGTYPE (PD_ESM, 0xCA),
    /** Modify EPS bearer context reject */
    EPS_MODIFY_EPS_BEARER_CONTEXT_REJ                   = PD_GSMID_TO_L3MSGTYPE (PD_ESM, 0xCB),
    /** Deactivate EPS bearer context request */
    EPS_DEACTIVATE_EPS_BEARER_CONTEXT_REQ               = PD_GSMID_TO_L3MSGTYPE (PD_ESM, 0xCD),
    /** Deactivate EPS bearer context accept */
    EPS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT            = PD_GSMID_TO_L3MSGTYPE (PD_ESM, 0xCE),
    /** PDN connectivity request */
    EPS_PDN_CONNECTIVITY_REQ                            = PD_GSMID_TO_L3MSGTYPE (PD_ESM, 0xD0),
    /** PDN connectivity reject */
    EPS_PDN_CONNECTIVITY_REJ                            = PD_GSMID_TO_L3MSGTYPE (PD_ESM, 0xD1),
    /** PDN disconnect request */
    EPS_PDN_DISCONNECT_REQ                              = PD_GSMID_TO_L3MSGTYPE (PD_ESM, 0xD2),
    /** PDN disconnect reject */
    EPS_PDN_DISCONNECT_REJ                              = PD_GSMID_TO_L3MSGTYPE (PD_ESM, 0xD3),
    /** Bearer resource allocation request */
    EPS_BEARER_RESOURCE_ALLOC_REQ                       = PD_GSMID_TO_L3MSGTYPE (PD_ESM, 0xD4),
    /** Bearer resource allocation reject */
    EPS_BEARER_RESOURCE_ALLOC_REJ                       = PD_GSMID_TO_L3MSGTYPE (PD_ESM, 0xD5),
    /** Bearer resource modification request */
    EPS_BEARER_RESOURCE_MODIFY_REQ                      = PD_GSMID_TO_L3MSGTYPE (PD_ESM, 0xD6),
    /** Bearer resource modification reject */
    EPS_BEARER_RESOURCE_MODIFY_REJ                      = PD_GSMID_TO_L3MSGTYPE (PD_ESM, 0xD7),
    /** ESM information request */
    EPS_ESM_INFO_REQ                                    = PD_GSMID_TO_L3MSGTYPE (PD_ESM, 0xD9),
    /** ESM information response */
    EPS_ESM_INFO_RSP                                    = PD_GSMID_TO_L3MSGTYPE (PD_ESM, 0xDA),
    /** ESM dummy message */
    EPS_ESM_DUMMY_MESSAGE                               = PD_GSMID_TO_L3MSGTYPE (PD_ESM, 0xDC),
    /** ESM status */
    EPS_ESM_STATUS                                      = PD_GSMID_TO_L3MSGTYPE (PD_ESM, 0xE8),
    /** ESM data transport */
    EPS_ESM_DATA_TRANSPORT                              = PD_GSMID_TO_L3MSGTYPE (PD_ESM, 0xEB),

    /** LTE CLOSE UE TEST LOOP  */
    TC_EUTRA_CLOSE_UE_TEST_LOOP                         = PD_GSMID_TO_L3MSGTYPE (PD_TEST, 0x80),
    /** LTE CLOSE UE TEST LOOP COMPLETE */
    TC_EUTRA_CLOSE_UE_TEST_LOOP_COMPLETE                = PD_GSMID_TO_L3MSGTYPE (PD_TEST, 0x81),
    /** LTE OPEN UE TEST LOOP  */
    TC_EUTRA_OPEN_UE_TEST_LOOP                          = PD_GSMID_TO_L3MSGTYPE (PD_TEST, 0x82),
    /** LTE OPEN UE TEST LOOP COMPLETE */
    TC_EUTRA_OPEN_UE_TEST_LOOP_COMPLETE                 = PD_GSMID_TO_L3MSGTYPE (PD_TEST, 0x83),
    /** LTE ACTIVATE TEST MODE  */
    TC_EUTRA_ACTIVATE_TEST_MODE                         = PD_GSMID_TO_L3MSGTYPE (PD_TEST, 0x84),
    /** LTE ACTIVATE TEST MODE COMPLETE */
    TC_EUTRA_ACTIVATE_TEST_MODE_COMPLETE                = PD_GSMID_TO_L3MSGTYPE (PD_TEST, 0x85),
    /** LTE DEACTIVATE TEST MODE */
    TC_EUTRA_DEACTIVATE_TEST_MODE                       = PD_GSMID_TO_L3MSGTYPE (PD_TEST, 0x86),
    /** LTE DEACTIVATE TEST MODE COMPLETE */
    TC_EUTRA_DEACTIVATE_TEST_MODE_COMPLETE              = PD_GSMID_TO_L3MSGTYPE (PD_TEST, 0x87),

#endif  /* UPGRADE_LTE */

    /** NON-MESSAGES; these all have bit7 = 1 to differentiate them */
    NON_L3_MESSAGE                    = PD_GSMID_TO_L3MSGTYPE (PD_NON, 0xFF)
}
L3MessageType;

/** used to fill place in message union */
typedef Int8
EmptyMemberOfUnion;

/** Provides information about the data */
typedef struct RawDataTag
{
    /** Total raw data length
\assoc ARRAY \ref data */
    Int16                           length;
    /** Lists the data */
    Int8                            data[MAX_CAUSE_DIAGNOSTIC_LENGTH];
}
RawData;

/** User supplied diagnostic */
typedef union DiagnosticsUnionTag
{
    /* used to fill place in message union */
    EmptyMemberOfUnion              noDiagnostics;
    /** Raw data to be diagnostic*/
    RawData                         raw;
    /** Indicates L3 message type */
    L3MessageType                   l3MessageType;
}
DiagnosticsUnion;

/** Used to aid the decoding of the Facility information element.
 * The SS version indicator is a type 4 information element with
 * one octet of the information element.
 * This is used in L3 messages.
 */
typedef struct SsVersionIndicatorTag
{
    /** Represents information elements of SS version indicator */
    Int8                            information;
}
SsVersionIndicatorElement,
/** SS version indicator struct that is used in MNXX signals */
SsVersionIndicator;

/** Used to transport supplementary service related information.
 * The facility Holds coded facility information element data.
 * It is a type 4 information element with a minimum
 * length of 2 octets.
 * This is used in L3 messages.
 */
typedef struct FacilityTag
{
    /** Length of facility contents
\assoc ARRAY \ref data */
    Int16                           dataLength;
    /** Facility information */
    Int8                            data[MAX_FACILITY_DATA_LENGTH];
}
FacilityElement,
/** Facility struct that is used in MNXX signals */
Facility;

/** Information transfer capability */
typedef enum LlcTransferCapabilityTag
{
    /** speech*/
    LLC_TC_SPEECH           = 0,
    /** unrestricted digital information */
    LLC_TC_UNRESTRICTED_DIG = 8,
    /** restricted digital information*/
    LLC_TC_RESTRICTED_DIG   = 9,
    /** 3.1 kHz audio*/
    LLC_TC_3100_AUDIO       = 16,
    /** 7 kHz audio*/
    LLC_TC_7000_AUDIO       = 17,
    /** video */
    LLC_TC_VIDEO            = 24
}
LlcTransferCapability;

/** Indicates mode of the transfer (Circuit or Packet) */
typedef enum LlcTransferModeTag
{
    /** circuit mode */
    LLC_TM_CIRCUIT = 0,
    /** packet mode */
    LLC_TM_PACKET  = 2
}
LlcTransferMode;

/** Indicates transfer rate information */
typedef enum LlcTransferRateTag
{
    /** This code shall be used for packet-mode calls. */
    LLC_TR_PACKET_MODE = 0,
    /** 64 kbit/s */
    LLC_TR_64K         = 16,
    /** 2 x 64 kbit/s */
    LLC_TR_2X64K       = 17,
    /** 384 kbit/s */
    LLC_TR_384K        = 19,
    /** 1536 kbit/s */
    LLC_TR_1536K       = 21,
    /** 1920 kbit/s */
    LLC_TR_1920K       = 23,
    /** Multi rate */
    LLC_TR_MULTIRATE   = 24,
    NUM_LLC_TR /* to avoid messing with commas before C 99 */
}
LlcTransferRate;

/** Indicates structure information */
typedef enum LlcStructureTag
{
    /** Default values are used */
    LLC_STRUC_DEFAULT       = 0,
    /** 8 kHz integrity */
    LLC_STRUC_8K            = 1,
    /** Service data unit integrity */
    LLC_STRUC_SRV_UNIT_DATA = 4,
    /** Unstructured */
    LLC_STRUC_UN_STRUCTURED = 7
}
LlcStructure;

/** user information layer 1 protocol */
typedef enum LlcUserInfoL1ProtocolTag
{
   /** CCITT standardized rate adaption V.110/X.30.*/
   LLC_UIL1P_V110       = 1,
   /** Recommendation G.711 m-law */
   LLC_UIL1P_G711_U_LAW = 2,
   /** Recommendation G.711 A-law */
   LLC_UIL1P_G711_A_LAW = 3,
   /** Recommendation G.721 32 kbit/s ADPCM and Recommendation I.460 */
   LLC_UIL1P_G721_I460  = 4,
   /** Recommendation G.722 and G.725 7 kHz audio*/
   LLC_UIL1P_G722_G725  = 5,
   /** Recommendation G.7xx 384 kbit/s video*/
   LLC_UIL1P_H261       = 6,
   /** Non-CCITT standardized rate adaption*/
   LLC_UIL1P_NON_CCITT  = 7,
   /** CCITT standardized rate adaption V120.*/
   LLC_UIL1P_V120       = 8,
   /** CCITT standardized rate adaption X31 HDLC flag stuffing */
   LLC_UIL1P_X31        = 9
}
LlcUserInfoL1Protocol;

/** Specifies user rate */
typedef enum LlcUserRateTag
{
  /** rate is indicated by E-bits specified in Rec I460 */
  LLC_UR_E_BITS_I460 = 0,
  /** 0.6 kbit/s */
  LLC_UR_600         = 1,
  /** 1.2 kbit/s */
  LLC_UR_1200        = 2,
   /** 2.4 kbit/s */
  LLC_UR_2400        = 3,
   /** 3.6 kbit/s */
  LLC_UR_3600        = 4,
   /** 4.8 kbit/s */
  LLC_UR_4800        = 5,
   /** 7.2 kbit/s */
  LLC_UR_7200        = 6,
   /** 8 kbit/s */
  LLC_UR_8000        = 7,
   /** 9.6 kbit/s */
  LLC_UR_9600        = 8,
   /** 14.4 kbit/s */
  LLC_UR_14400       = 9,
   /** 16 kbit/s */
  LLC_UR_16000       = 10,
   /** 19.2 kbit/s */
  LLC_UR_19200       = 11,
   /** 32 kbit/s */
  LLC_UR_32000       = 12,
   /** 38.4 kbit/s */
  LLC_UR_38400       = 13,
   /** 48 kbit/s */
  LLC_UR_48000       = 14,
   /** 56 kbit/s */
  LLC_UR_56000       = 15,
   /** 64 kbit/s */
  LLC_UR_64000       = 16,
   /** 57.6 kbit/s */
  LLC_UR_57600       = 18,
   /** 28.8 kbit/s */
  LLC_UR_28800       = 19,
   /** 24 kbit/s */
  LLC_UR_24000       = 20,
   /** 0.1345 kbit/s */
  LLC_UR_134_5       = 21,
   /** 0.1 kbit/s */
  LLC_UR_100         = 22,
   /** 0.075/1.2 kbit/s */
  LLC_UR_75_1200     = 23,
   /** 1.2/0.075 kbit/s */
  LLC_UR_1200_75     = 24,
   /** 0.050 kbit/s */
  LLC_UR_50          = 25,
   /** 0.075 kbit/s */
  LLC_UR_75          = 26,
   /** 0.110 kbit/s */
  LLC_UR_110         = 27,
   /** 0.15 kbit/s */
  LLC_UR_150         = 28,
   /** 0.2 kbit/s */
  LLC_UR_200         = 29,
   /** 0.3 kbit/s */
  LLC_UR_300         = 30,
   /** 12 kbit/s */
  LLC_UR_12000       = 31
}
LlcUserRate;

/** Indicates intermediate rate information */
typedef enum LlcIntermediateRateTag
{
  /** 8 kbit/s */
  LLC_IR_8K  = 1,
  /** 16 kbit/s */
  LLC_IR_16K = 2,
  /** 32 kbit/s */
  LLC_IR_32K = 3
}
LlcIntermediateRate;

/** V110 rate adaption element */
typedef struct LlcRateAdaptionV110Tag
{
  /** Represents Intermediate rate */
  LlcIntermediateRate               intermediateRate;
  /** Indicates if required to send data with Network Independent Clock */
  Boolean                           requiresToTxWithNic;
  /** Indicates if can accept data with Network Independent Clock
   * (for instance sender does support this optional procedure)
   */
  Boolean                           canRxWithNic;
  /** Indicates if required to send data with flow control mechanism */
  Boolean                           flowControlOnTx;
  /** Indicates if can accept data with flow control mechanism
   * (sender does support this optional procedure)
   */
  Boolean                           flowControlOnRx;
}
LlcRateAdaptionV110;

/** V120 rate adaption element */
typedef struct LlcRateAdaptionV120Tag
{
  /** Indicates whether rate adaption header included or not */
  Boolean                           isHdrPresent;
  /** Whether multiple frame establishment supported or not */
  Boolean                           multiFrameEstabSupported;
  /**Indicates if bit transparent mode of operation(0)or
   * protocol sensitive mode of operation (1)
   */
  Boolean                           protocolSensitiveMode;
  /** Logical link identifier negotiation
   * (default/full protocol negotiation)
   */
  Boolean                           fullProtocolNegotiation;
  /** Assignor/assignee */
  Boolean                           origIsAssignorOnly;
  /** Indicates if negotiation is done in-band using logical link zero */
  Boolean                           negotiationIsInBand;
}
LlcRateAdaptionV120;

/** Specifies rate adaption type */
typedef enum RateAdaptionTypeTag
{
  /** No Type */
  RATE_ADAPTION_TYPE_NONE,
  /** V.110 rate adaption*/
  RATE_ADAPTION_TYPE_V110,
  /** V.120 rate adaption*/
  RATE_ADAPTION_TYPE_V120,
  /** Unvalid rate adaption*/
  RATE_ADAPTION_TYPE_INVALID
}
RateAdaptionType;

/** Specifies type of rate adaption (none, V110 or V120) */
typedef union LlcRateAdaptionTag
{
  /** No Type */
  Int8                none;
  /** V.110 rate adaption*/
  LlcRateAdaptionV110 v110;
   /** V.120 rate adaption*/
  LlcRateAdaptionV120 v120;
}
LlcRateAdaption;

/** Number of stop bits */
typedef enum LlcNoOfStopBitsTag
{
  /** 1 bits */
  LLC_SB_1   = 1,
  /** 1.5 bits */
  LLC_SB_1_5 = 2,
  /** 2 bits */
  LLC_SB_2   = 3
}
LlcNoOfStopBits;

/** Number of data bits */
typedef enum LlcNoOfDataBitsTag
{
  /** 5 bits */
  LLC_DB_5 = 1,
  /** 7 bits */
  LLC_DB_7 = 2,
  /** 8 bits */
  LLC_DB_8 = 3
}
LlcNoOfDataBits;

/** User info L2 protocol */
typedef enum LlcUserInfoL2ProtocolTag
{
  LLC_UIL2P_BASIC_MODE     = 1,
  LLC_UIL2P_Q921           = 2,
  LLC_UIL2P_X25_LINK_LAYER = 6,
  LLC_UIL2P_X25_MULTILINK  = 7,
  LLC_UIL2P_EXT_LAPB_T71   = 8,
  LLC_UIL2P_HDLC_ARM       = 9,
  LLC_UIL2P_HDLC_NRM       = 10,
  LLC_UIL2P_HDLC_ABM       = 11,
  LLC_UIL2P_LAN_8802_2     = 12,
  LLC_UIL2P_ISO7776_DTE_DTE = 33,
  LLC_UIL2P_X75_SLP        = 13
}
LlcUserInfoL2Protocol;

/** layer 3 protocol information */
typedef enum LlcUserInfoL3ProtocolTag
{
  /** CCITT Recommendation Q.931 (I.451) */
  LLC_UIL3P_Q931          = 2,
  /** CCITT Recommendation X.25, packet layer */
  LLC_UIL3P_X25_PKT_LAYER = 6,
  /** ISO 8208 (X.25 packet level protocol for data terminal equipment) */
  LLC_UIL3P_8208          = 7,
  /** ISO 8348 (OSI connection oriented network service specific subset
   * of ISO 8208 and CCITT X.25)
   */
  LLC_UIL3P_8348          = 8,
  /** ISO 8473 (OSI connectionless service) */
  LLC_UIL3P_8473          = 9,
  /** CCITT Recommendation T.70 minimum network layer*/
  LLC_UIL3P_T70           = 10
}
LlcUserInfoL3Protocol;

/** L2 mode of operation */
typedef enum LlcL2ModeOfOperationTag
{
    NORMAL_MODE_OF_OPERATION = 1,
    EXTENDED_MODE_OF_OPERATION = 2
    /* other values are reserved */
}
LlcL2ModeOfOperation;

/** Specifies the additional l3 protocol info */
typedef enum AdditionalL3ProtocolInfoTag
{
    /** IP additional l3 protocol info (see table 4-16 of Q931) */
    IP_ADDITIONAL_L3_PROTOCOL_INFO = 0xCC,
    /** PPP additional l3 protocol info */
    PPP_ADDITIONAL_L3_PROTOCOL_INFO = 0xCF
    /** In Q931 all other values are reserved and in ETS 300 102-1 it is
     * not clear whether they can be used. */
}
AdditionalL3ProtocolInfo;

/** L3 Packet Sequence Mode Of Operation element */
typedef enum LlcL3PacketSequenceModeOfOperationTag
{
    /** Normal */
    NORMAL_PACKET_SEQUENCE_NUMBERING = 1,
    /** Extended */
    EXTENDED_PACKET_SEQUENCE_NUMBERING = 2
}
LlcL3PacketSequenceModeOfOperation;

/** L3 default packet size.
 * These values at present are quite regular, and could be
 * generated by <<.  We define an enum anyway since there is no
 * guarantee that the regularity will be preserved with future
 * versions of the standard.
 */
typedef enum LlcL3DefaultPacketSize
{
    DEFAULT_PACKET_SIZE_16 = 4,
    DEFAULT_PACKET_SIZE_32 = 5,
    DEFAULT_PACKET_SIZE_64 = 6,
    DEFAULT_PACKET_SIZE_128 = 7,
    DEFAULT_PACKET_SIZE_256 = 8,
    DEFAULT_PACKET_SIZE_512 = 9,
    DEFAULT_PACKET_SIZE_1024 = 10,
    DEFAULT_PACKET_SIZE_2048 = 11,
    DEFAULT_PACKET_SIZE_4096 = 12
}
LlcL3DefaultPacketSize;


/** see Q.931 table 4-16 for the numbers in binary.  Not all values are
   usable in gsm/umts.  24.008-2b0 10.5.4.18 says that we can use
   either the Q.931 values, as below, or for backwards compatibility
   those of ETS 300 102-1, which are 16 less. */
typedef enum LlcModemTypeTag
{
    LLC_MODEM_TYPE_MAX_ETS300102_1 = 16,
    /** Recommendation V.21 */
    LLC_MODEM_TYPE_V21 = 17,
    /** Recommendation V.22 */
    LLC_MODEM_TYPE_V22 = 18,
    /** Recommendation V.22 bis */
    LLC_MODEM_TYPE_V22BIS = 19,
    /** Recommendation V.23 */
    LLC_MODEM_TYPE_V23 = 20,
    /** Recommendation V.26 */
    LLC_MODEM_TYPE_V26 = 21,
    /** Recommendation V.26 bis */
    LLC_MODEM_TYPE_V26BIS = 22,
    /** Recommendation V.26 ter */
    LLC_MODEM_TYPE_V26TER = 23,
    /** Recommendation V.27 */
    LLC_MODEM_TYPE_V27 = 24,
    /** Recommendation V.27 bis */
    LLC_MODEM_TYPE_V27BIS = 25,
    /** Recommendation V.27 ter */
    LLC_MODEM_TYPE_V27TER = 26,
    /** Recommendation V.29 */
    LLC_MODEM_TYPE_V29 = 27,
    /** Recommendation V.32 */
    LLC_MODEM_TYPE_V32 = 28,
    /** Recommendation V.34 */
    LLC_MODEM_TYPE_V34 = 30
}
LlcModemType;

/** Low layer compatibility information element.
 * The low layer compatibility information element is transferred transparently
 * by a PLMN between the call originating entity and the addressed entity.
 * The low layer compatibility is a type 4 information element with a minimum
 * length of 2 octets and a maximum length of 15 octets.
 */
typedef struct LowLayerCompatibilityTag
{
    /** Coding standard (octet 3)  */
    CodingStandard                  codingStandard;
    /** Information transfer capability (Octeet 3)*/
    LlcTransferCapability           transferCapability;

    /** Indicates if octet 3a is present or omitted
\assoc PRESENT \ref outBandNegotiationPossible */
    Boolean                         present3a;
    /** This parameter (octet 3a) shall be present if out-band
     * negotiation is required.
     */
    Boolean                         outBandNegotiationPossible;

    /** Transfer mode (Octet 4)  */
    LlcTransferMode                 transferMode;
    /** Information transfer rate (Octet 4 */
    LlcTransferRate                 o2dTransferRate;

    /** If default values are used for all fields of octets 4a
      * then this octet shall not be included.
\assoc PRESENT \ref structure */
    Boolean                         present4a;
    /** Defines the structure */
    LlcStructure                    structure;

    /** octet 4.1 (Q.931).  If the Q.931 format is being used, then
       present4_1 may or may not be set, but present4a and present4b
       will not be set.  If the ETS 300 102-1 format is being used,
       then present4_1 will not be set, but present4a and present4b
       may or may not be set.  present4_1 should be set if and only if
       o2dTransferRate == LLC_TR_MULTIRATE (this value is not allowed
       in ETS 300 102-1).
\assoc PRESENT \ref rateMultiplier */
    Boolean                         present4_1;
    /** Determines rate multiplier */
    Int8                            rateMultiplier;

    /** If default values are used for all fields of octets 4a
      * then this octet shall not be included.
\assoc PRESENT \ref d2oTransferRate */
    Boolean                         present4b;
    LlcTransferRate                 d2oTransferRate;

    /** user information layer 1 protocol (octet 5)  */
    LlcUserInfoL1Protocol           userInfoL1Protocol;

    /** This octet may be present if userInfoL1Protocol indicates
      * either of the CCITT standardized rate adaptions V.110/X.30
      * or V.120.(octet 5a)
\assoc PRESENT \ref isAsynchronous
\assoc PRESENT \ref inBandNegotiation
\assoc PRESENT \ref userRate */
    Boolean                         present5a;
    /** Indicates whether Synchronous or asynchronous */
    Boolean                         isAsynchronous;
    /** Indicates whether in-band negotiation is possible or not */
    Boolean                         inBandNegotiation;
    /** Specifies user rate */
    LlcUserRate                     userRate;

    /** This octet is present only if userInfoL1Protocol indicates
      * CCITT standardized rate adaption V.120. (octet 5b)
\assoc UNION \ref rateAdaption */
    RateAdaptionType                present5b;
    /** Specifies rate adaption and defines the related elements */
    LlcRateAdaption                 rateAdaption;

    /** This octet may be present if userInfoL1Protocol indicates
      * either of the CCITT standardized rate adaptions V.110/X.30
      * or V.120.(octet 5c)
\assoc PRESENT \ref noOfStopBits
\assoc PRESENT \ref noOfDataBits
\assoc PRESENT \ref parityInformation */
    Boolean                         present5c;
    /** Number of stop bits (octet 5c))*/
    LlcNoOfStopBits                 noOfStopBits;
    /** Number of data bits excluding parity bit if present */
    LlcNoOfDataBits                 noOfDataBits;
    /** Parity information */
    ParityInformation               parityInformation;

    /** octet 5d
\assoc PRESENT \ref fullDuplexMode
\assoc PRESENT \ref modemType */
    Boolean                         present5d;
    /** Full or hald duplex */
    Boolean                         fullDuplexMode;
    /** Type of modem*/
    LlcModemType                    modemType;

    /** octet 6
     * There may be no L2 protocol info present in the IE
\assoc PRESENT \ref userInfoL2Protocol */
    Boolean                         userInfoL2ProtocolPresent;
    LlcUserInfoL2Protocol           userInfoL2Protocol;

    /** octet 6a, ETS 300 102-1 variant, or Q.931 variant
       with no octet 6b.  At most one of present6a and
       present6ab will be set.
\assoc PRESENT \ref l2ProtocolInfo */
    Boolean                         present6a;
    /** Indicates user information layer 2 protocol. */
    Int8                            l2ProtocolInfo;

    /** octets 6a and 6b, according to Q.931
\assoc PRESENT \ref llcL2ModeOfOperation
\assoc PRESENT \ref l2Q933Use
\assoc PRESENT \ref l2WindowSize */
    Boolean                         present6ab;
    /** Mode of operation*/
    LlcL2ModeOfOperation              llcL2ModeOfOperation;
    /** If `l2Q933Use' ever seems to have a meaningful purpose, it should
       perhaps be converted to an enum. */
    Int8                            l2Q933Use;
    Int8                            l2WindowSize; /**< 1 to 127 */

    /** octet 7
     * There may be no L3 protocol info present in the IE
\assoc PRESENT \ref userInfoL3Protocol */
    Boolean                         userInfoL3ProtocolPresent;
    /** layer 3 protocol information */
    LlcUserInfoL3Protocol           userInfoL3Protocol;

    /** octet 7a
\assoc PRESENT \ref l3ProtocolInfo */
    Boolean                         present7a;
    Int8                            l3ProtocolInfo;

    /** Note that octet 7a may be present, but present7a will _not_ be
     set if 7b is set; its meaning changes to `octet
     7a only is present'.  At most one of present7a, present7ab and
     present7abc will be set.
\assoc PRESENT \ref additionalL3ProtocolInfo */
    Boolean                         present7ab;
    /** The additional l3 protocol info */
    AdditionalL3ProtocolInfo        additionalL3ProtocolInfo;
    /** octet 7abc
\assoc PRESENT \ref llcL3PacketSequenceModeOfOperation
\assoc PRESENT \ref llcL3DefaultPacketSize
\assoc PRESENT \ref llcL3PacketWindowSize */
    Boolean                         present7abc;
    /** LlcL3PacketSequenceModeOfOperation is not to be confused with
       LlcL2ModeOfOperation or ModeOfOperation */
    LlcL3PacketSequenceModeOfOperation llcL3PacketSequenceModeOfOperation;
    /** L3 packet sequence mode of operation  */
    LlcL3DefaultPacketSize          llcL3DefaultPacketSize ;
    Int8                            llcL3PacketWindowSize; /**< 1 to 127 */

}
LowLayerCompatibilityElement,
/** Low layer compatibility struct that is used in MNXX signals */
LowLayerCompatibility;

/** Indicate high layer capabilities */
typedef struct HighLayerCompatibilityTag
{
    /** Coding standard */
    CodingStandard                  codingStandard;
    /** Indicates how the High layer characteristics
      * identification(in octet 4) should be interpreted. */
    HlcInterpretation               interpretation;
    /** Presentation method of protocol profile */
    PresentationMethod              presentationMethod;
    /** High layer characteristics identification */
    HighCharacteristicsId           highLayerCharId;
    /** Extended High layer characteristics identification */
    HighCharacteristicsId           highLayerCharIdExt;
}
HighLayerCompatibilityElement,
/** High layer compatibility struct that is used in MNXX signals */
HighLayerCompatibility;

/** Calling party BCD number information element is used
 * to identify the origin of a call.
 */
typedef struct CallingPartyBcdNumTag
{
    /** Indicates length of number contents
\assoc ARRAY \ref number */
    Int8                            numberLength;
    /** Indicates type of number */
    BcdNumberType                   type;
    /** Numbering plan identification */
    BcdNumberPlan                   numPlan;
    /** Presentation indicator */
    PresentationIndicator           presentInd;
    /** Screening indicator (octet 3a)*/
    ScreeningIndicator              screeningInd;
    /** Lists the number */
    Bcd                             number[MAX_CALLING_BCD_NO_LENGTH];
}
CallingPartyBcdNumElement,
/** Connected number element */
ConnectedNumberElement,
/** Calling party BCD number */
CallingPartyBcdNum,
/** Identify the connected party of a call. */
ConnectedNumber;

/** Redirecting party BCD number elements.
 * Identifies the redirecting party. It is a type 4 information element.
 */
typedef struct RedirectingPartyBcdNumTag
{
    /** Length of redirecting party BCD number contents */
    Int8                            numberLength;
    /** Indicates the type of the number */
    BcdNumberType                   type;
    /** Numbering plan identification */
    BcdNumberPlan                   numPlan;
    /** Presentation indicator */
    PresentationIndicator           presentInd;
    /** Screening indicator */
    ScreeningIndicator              screeningInd;
    /** Lists the number */
    Bcd                             number[MAX_REDIRECTING_BCD_NO_LENGTH];
}
RedirectingPartyBcdNum;

/** Called party BCD number information element */
typedef struct CalledPartyBcdNumTag
{
    /** Length of called party BCD number contents.
\assoc ARRAY \ref number */
    Int8                            numberLength;
    /** Indicates the type of the number */
    BcdNumberType                   type;
    /** Numbering plan identification*/
    BcdNumberPlan                   numPlan;
    /** BCD Value (04.08 10.5.4.7) */
    Bcd                             number[MAX_CALLED_BCD_NO_LENGTH];
}
CalledPartyBcdNumElement,
/** Called party BCD number struct that is used in MNXX signals */
CalledPartyBcdNum;

/**  Describes an event which has occurred during the life of a call.
 * Passes information about the call in progress. The progress indicator
 * is a type 4 information element with a length of 4 octets.
 */
typedef struct ProgressIndicatorTag
{
    /** Coding standard (octet 3)*/
    CodingStandard                  coding;
    /** Location elements */
    Location                        location;
    /** Progress description element */
    ProgressDescription             description;
}
ProgressIndicatorElement,
/** Progress indicator struct that is used in MNXX signals */
ProgressIndicator;

/** Cause information element. Describes the reason for generating
 * this messages to provide diagnostic information in the event of
 * procedural errors and to indicate the location of the cause originator.
 */
typedef struct CauseTag
{
    /** Defines cause Coding standard */
    CodingStandard                  coding;
    /** Location of the cause originat */
    Location                        location;
    /** Type of recommendation */
    Recommendation                  recommendation;
    /** Cause values */
    GsmCause                        value;
    /** Diagnostic type
\assoc UNION \ref diagnostics */
    DiagnosticsEnum                 diagnosticsType;
    /** User supplied diagnostic */
    DiagnosticsUnion                diagnostics;
    /** Contains the protocol discriminator (PD) information element */
    ProtocolDiscriminator           pd;
}
CauseElement,
/** Cause struct that is used in MNXX signals */
Cause;

/** Small user to user element.
 * The user-user is a type 4 information element with a minimum
 * length of 3 octets and a maximum length of either 35 or 131 octets.
 * The user-user information element is transported transparently
 * through a GSM PLMN.
 */
typedef struct SmallUserToUserTag
{
    /** Indicates the length of user-user contents
\assoc ARRAY \ref info */
    Int16                           infoLength;
    /** User-user protocol discriminator */
    UserProtocolDiscrim             protocolDisc;
    /** Lists User-user information */
    Int8                            info[MAX_USER_TO_USER_LENGTH_SMALL];
}
SmallUserUserElement,
/** Small user to user struct that is used in MNXX signals */
SmallUserUser;

/** The Call Control Capabilities information element.
 * The Call Control Capabilities is a type 4 information element
 * with a length of 3 octets
 */
typedef struct CcCapabilitiesTag
{
    /** Indicates if the mobile station supports DTMF */
    Boolean            dtmf;
    /** Indicates that the mobile station supports the
      * prolonged clearing procedure. */
    Boolean pcp;
    /** Indicates the maximum number of supported bearers. */
    Int8 maxNumberSupportedBearers;
    /** Indicates the maximum number of speech bearers. */
    Int8 maxNumberSpeechBearers;
}
CcCapabilitiesElement,
/** Call control capabilities struct that is used in MNXX signals */
CcCapabilities;

/** Provides information defining the priority level requested
 * or applied. The Priority Level IE may be included in CM_SERVICE_REQUEST,
 * CALL_PROCEEDING and SETUP messages.
 */
typedef enum PriorityLevelTag
{
    /** no priority applied */
    NO_PRIORITY_LEVEL = 0x00,
    /** call priority level 4 */
    PRIORITY_LEVEL_4  = 0x01,
    /** call priority level 3 */
    PRIORITY_LEVEL_3  = 0x02,
    /** call priority level 2*/
    PRIORITY_LEVEL_2  = 0x03,
    /** call priority level 1 */
    PRIORITY_LEVEL_1  = 0x04,
    /** call priority level 0 */
    PRIORITY_LEVEL_0  = 0x05,
    /** call priority level B */
    PRIORITY_LEVEL_B  = 0x06,
    /** call priority level A */
    PRIORITY_LEVEL_A  = 0x07
}
PriorityLevel;

/** Alerting Pattern information element.
 * The Alerting Pattern IE is a type 4 information element used to
 * allow the network to convey information related to the alert to
 * be used by the MS
 */
typedef enum AlertingPatternTag
{
    /** alerting pattern 1 */
    ALERTING_PATTERN_1 = 0x00,
    /** alerting pattern 2 */
    ALERTING_PATTERN_2 = 0x01,
    /** alerting pattern 3 */
    ALERTING_PATTERN_3 = 0x02,
    /** alerting pattern 5 */
    ALERTING_PATTERN_5 = 0x04,
    /** alerting pattern 5 */
    ALERTING_PATTERN_6 = 0x05,
    /** alerting pattern 7 */
    ALERTING_PATTERN_7 = 0x06,
    /** alerting pattern 8 */
    ALERTING_PATTERN_8 = 0x07,
    /** alerting pattern 9 */
    ALERTING_PATTERN_9 = 0x08
}
AlertingPattern;


/** StreamIdentifier of type Int8 */
typedef Int8 StreamIdentifier;

/** Cause of No CLI information element.
 * The Cause of No CLI is a type 4 information element used to
 * provide the mobile station the detailed reason why Calling party
 * BCD number is not notified.
 */
typedef enum CauseOfNoCliTag
{
    /** Unavailable */
    CAUSE_OF_NO_CLI_UNAVAILABLE = 0,
    /** Reject by user */
    CAUSE_OF_NO_CLI_REJECT_BY_USER = 1,
    /** Interaction with other service */
    CAUSE_OF_NO_CLI_INTERACTION_WITH_OTHER_SERVICE = 2,
    /** Coin line/payphone */
    CAUSE_OF_NO_CLI_COIN_LINE_PAYPHONE = 3
    /* 24008-3b0: Other values shall be interpreted as "Unavailable". */
}
CauseOfNoCli;

/** NetworkCallControlCapabilities is an 8-bit bitstring, but only one
   bit is used at present */
typedef Int8 NetworkCallControlCapabilities;


#if defined (UPGRADE_REL5)

/** Codec type element (Values according to 26-103) */
typedef enum CodecTypeTag
{
    GSM_FR              = 0x00,     /**< GSM FULL RATE */
    GSM_HR              = 0x01,     /**< GSM HALF RATE */
    GSM_EFR             = 0x02,     /**< GSM ENHANCED FULL RATE */
    FR_AMR              = 0x03,     /**< FULL RATE ADAPTIVE MULTI-RATE */
    HR_AMR              = 0x04,     /**< HALF RATE ADAPTIVE MULTI-RATE */
#if defined (UPGRADE_3G)
    UMTS_AMR            = 0x05,     /**< UMTS ADAPTIVE MULTI-RATE */
    UMTS_AMR_2          = 0x06,     /**< UMTS  ADAPTIVE MULTI-RATE 2 */
    TDMA_EFR            = 0x07,     /**< TDMA ENHANCED FULL RATE */
    PDC_EFR             = 0x08,     /**< PDC ENHANCED FULL RATE */
#endif
    FR_AMR_WB           = 0x09,     /**< FULL RATE ADAPTIVE MULTI-RATE WIDE BAND */
#if defined (UPGRADE_3G)
    UMTS_AMR_WB         = 0x0a,     /**< UMTS ADAPTIVE MULTI-RATE WIDE BAND */
#endif
    OHR_AMR             = 0x0b,     /**< 8PSK HALF RATE ADAPTIVE MULTI-RATE WIDE BAND */
    OFR_AMR_WB          = 0x0c,     /**< 8PSK FULL RATE ADAPTIVE MULTI-RATE WIDE BAND */
    OHR_AMR_WB          = 0x0d,     /**< 8PSK HALF RATE ADAPTIVE MULTI-RATE WIDE BAND */
    MU_ME_DUMMY         = 0xff      /**< Reserved for MuMe dummy codec type*/
} CodecType;

/** System identification.
 * Values according to 28.062
 */
typedef enum SystemIdentifierTag
{
    GSM_RADIO_ACCESS_TECHNOLOGY  = 0x00,
    TDMA_RADIO_ACCESS_TECHNOLOGY = 0x01,    /* TIA/EIA-136 */
    CDMA_RADIO_ACCESS_TECHNOLOGY = 0x02,    /* TIA/EIA-95  */
    UMTS_RADIO_ACCESS_TECHNOLOGY = 0x04
} SystemIdentifier;

/** Supported codec element */
typedef struct SupportedCodecTag
{
    /** Identifier the system */
    SystemIdentifier   sysId;
    /** Indicates the number of codec type */
    Int8               numCodecType;
    /** Holds codec type */
    CodecType          codecType[MAX_NUM_SUPPORTED_CODEC_TYPE];
} SupportedCodec;

/** Supported codec information element.
 * The purpose of the Supported Codec List information element is to
 * provide the network with information about the speech codecs
 * supported by the mobile.
 */
typedef struct SupportedCodecListTag
{
    Int8               numSupportedCodec;         /**< number of radio access technologies */
    /** Lists the supported codec */
    SupportedCodec     supportedCodec[MAX_NUM_SUPPORTED_CODEC_LIST];
} SupportedCodecList;

#endif

#endif

/* END OF FILE */

