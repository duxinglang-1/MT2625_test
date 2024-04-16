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
 * File Description                                                      */
/** \file
 * This file contains all enum,struct and union type definitions
 * used within the supplementary service unit in the background
 * task.
 **************************************************************************/
#ifndef ABSS_TYP_H
#define ABSS_TYP_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#if !defined(APPLAYER_H)
#   include <applayer.h>
#endif

#if !defined(MNXX_SIG_H)
#    include <mnxx_sig.h>
#endif

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/* #define LIFE_THE_UNIVERSE_AND_EVERYTHING 42 */

/***************************************************************************
 * Typed Constants
 **************************************************************************/

/* const char alphabet[] = "ABCDEF...."; */


/***************************************************************************
 * Type Definitions
 **************************************************************************/
#define   MAXNUMOFCUG                 10    /* see GSM4.80 */
#define   MAXCUGINTELOCKLEN           4     /* see GSM4.80 */
/*FR9712-1798*/
#define   MAXBASICSERVICEGROUPLIST    70    /* see GSN4.80 */
#define   MAXBASICSERVICEGROUPS       13    /* see GSN4.80 */
#define   MAX_ADDR_LEN                20    /* see GSM4.80 */
#define   MAX_ISDN_ADDR_LEN           9     /* see GSM4.80 */
#define   MAX_ISDN_SUBADDR_LEN        21    /* see GSM4.80 */
#define   MAX_USS_DATA_LEN            (MAX_FACILITY_DATA_LENGTH - 20 )
#define   MAX_USS_DATA_CHARS          ((MAX_FACILITY_DATA_LENGTH - 20 ) * 8) / 7
                                      /* 7-bit data FR9807-2675*/
#define   MAX_SSRAW_DATA_LEN          (MAX_FACILITY_DATA_LENGTH - 20 )
                                      /* 20 = sizeof(linkedInvokeComponent + 8)*/
#define   MAXCHARGEBANDS              7
#define   MAXPASSWORDLEN              4

#define MAX_CNAP_NAME_LENGTH          81

#if defined (UPGRADE_AGPS)
/* MOLR request definitions */
#define   MAX_NUM_PRIVATE_EXTENSIONS  10
#define   MAX_EXTERNAL_ADDRESS_SIZE   20
#define   MAX_MLC_NUMBER_SIZE          9
#define   MAX_ASSISTANCE_DATA_SIZE    38
#define   MAX_EXTENSION_TYPE_SIZE     16  /* Look at IOC for Private MAP extensions */
#define   MAX_EXTENSION_ID_SIZE       16  /* Section 17.7.11  29.002 */


/* MOLR-result definitions */
#define   MAX_LOCATION_ESTIMATE_STRING_SIZE      20
#define   MAX_DECIPHERING_KEYS_STRING_SIZE       15
#define   MAX_ADD_LOCATION_ESTIMATE_STRING_SIZE  91

/* locationNofification definitions */
#define   MAX_LCS_NAME_STRING_LENGTH             63
#define   MAX_LCS_CODEWORD_SIZE                  20

   /* UPGRADE_AGPS */
#endif

#define MAX_NUM_CCBS_FEATURES       5
#define MAX_B_SUBSCRIBER_SUBADDRESS_LEN 21
  /* The ASN.1 for the subscriber number says, rather confusingly,
     OCTET STRING ( SIZE (1 .. 20 ) ) ( SIZE (1 .. 9 ) ).  My reading
     is that the first is just redundant (presumably the result of
     mindless expansion of a type definition), and the type should be
     read as follows: a string of 1 to 20 octets, restricted to have
     size between 1 and 9: i.e. a string of 1 to 9 octets.  */
#define MAX_B_SUBSCRIBER_NUMBER_LEN     9





#if defined (UPGRADE_AGPS)

/*
 * Types used in
 * LCS-MOLR
 * message
 */


/* Implicit Enumeration */
typedef enum SsLcsGADShapesBitMaskTag
{
  ELLIPSOID_POINT                                         = 0x01,
  ELLIPSOID_POINT_WITH_UNCERTAINTY_CIRCLE                 = 0x02,
  ELLIPSOID_POINT_WITH_UNCERTAINTY_ELLIPSE                = 0x04,
  POLYGON                                                 = 0x08,
  ELLIPSOID_POINT_WITH_ALTITUDE                           = 0x10,
  ELLIPSOID_POINT_WITH_ALTITUDE_AND_UNCERTAINTY_ELLIPSOID = 0x20,
  ELLIPSOID_ARC                                           = 0x40
}
SsLcsGADShapesBitMask;


typedef enum SsLcsMolrTypeTag
{
    LOCATION_ESTIMATE   = 0,
    ASSISTANCE_DATA     = 1,
    DECIPHERING_KEYS    = 2
}
SsLcsMolrType;

typedef enum SsLcsLocationMethodTag
{
    MS_BASED_EOTD       = 0,
    MS_ASSISTED_EOTD    = 1,
    ASSISTED_GPS        = 2
}
SsLcsLocationMethod;


typedef enum SsLcsResponseTimeCatagoryTag
{
    LOW_DELAY           = 0,
    DELAY_TOLERANT      = 1
}
SsLcsResponseTimeCatagory;


typedef enum SsLcsPosnFailDiagnosticTag
{
    CONGESTION                                      = 0,
    INSUFFICIENT_RESOURCES                          = 1,
    INSUFFICIENT_MEASUREMENT_DATA                   = 2,
    INCONSISTENT_MEASUREMENT_DATA                   = 3,
    LOCATION_PROCEDURE_NOT_COMPLETED                = 4,
    LOCATION_PROCEDURE_NOT_SUPPORTED_BY_TARGET_MS   = 5,
    QOS_NOT_ATTAINABLE                              = 6,
    POSITION_METHOD_NOT_AVAILABLE_IN_NETWORK        = 7,
    POSITION_METHOD_NOT_AVAILABLE_IN_LOCATION_AREA  = 8
}
SsLcsPosnFailDiagnostic;



/* MOLR types */
typedef struct SsLcsOptLocationMethodTag
{
    Boolean                 present;
    SsLcsLocationMethod     method;
}
SsLcsOptLocationMethod;

typedef struct SsLcsOptAccuracyTag
{
  Boolean               present;
  Int8                  accuracy; /* IMPLICIT OCTECT STRING - SIZE 1 */
}
SsLcsOptAccuracy;

typedef struct SsLcsOptResponseTimeTag
{
  Boolean                     present;
  SsLcsResponseTimeCatagory   responseTimeCatagory; /* ENUMERATED */
}
SsLcsOptResponseTime;


typedef struct SsLcsExtensionIdTag
{
   Int8         n;   /* OCTET STRING  - should not exceed 16 octets */
   Int8         data[MAX_EXTENSION_ID_SIZE];
}
SsLcsExtensionId;

typedef struct SsLcsExtensionTypeTag
{
   Int8         n;  /* OCTET STRING  - should not exceed 16 octets */
   Int8         data[MAX_EXTENSION_TYPE_SIZE];
}
SsLcsExtensionType;


typedef struct SsLcsPrivateExtListDataTag
{
   Boolean                  extTypePresent;
   SsLcsExtensionType       extType;
   SsLcsExtensionId         extId;
}
SsLcsPrivateExtListData;


typedef struct SsLcsPrivateExtensionListTag
{
   Int8                       n; /* 1 to maxNumOfPrivateExtensions */
   SsLcsPrivateExtListData      data[MAX_NUM_PRIVATE_EXTENSIONS];
}
SsLcsPrivateExtensionList;


typedef struct SsLcsPcsExtensionsTag
{
   Int8                      _dummy_; /* Not used */
}
SsLcsPcsExtensions;

typedef struct SsLcsExtensionContainerTag
{
   Boolean                        privateExtensionListPresent;
   SsLcsPrivateExtensionList      privateExtensionList;
   Boolean                        pcs_ExtensionsPresent;
   SsLcsPcsExtensions             pcs_Extensions;
}
SsLcsExtensionContainer;


typedef struct SsLcsOptExtensionContainerTag
{
   Boolean                        present;
   SsLcsExtensionContainer        data;
}
SsLcsOptExtensionContainer;


typedef struct  SsLcsQosTag
{
   SsLcsOptAccuracy            horizontalAccuracy;    /* optional octet string */
   Boolean                     verticalCoordReq;      /* IMPLICIT NULL OPTIONAL */
   SsLcsOptAccuracy            verticalAccuracy;      /* optional octet string */
   SsLcsOptResponseTime        responseTime;          /* IMPLICIT SEQUENCE */
   SsLcsOptExtensionContainer  optExtensionContainer; /* IMPLICIT SEQUENCE */
}
SsLcsQos;


typedef struct  SsLcsOptQosTag
{
   Boolean         present;
   SsLcsQos        lcsQos;
}
SsLcsOptQos;


typedef struct SsLcsOptExternalAddressTag
{
   Boolean     present;
   Int8        addressLength;
   Int8        externalAddress[MAX_EXTERNAL_ADDRESS_SIZE];  /* Max size is 20 */
}
SsLcsOptExternalAddress;


typedef struct SsLcsOptClientExternalIdTag
{
   Boolean                     present;
   SsLcsOptExternalAddress     optExternalAddress;
   SsLcsOptExtensionContainer  optExtensionContainer;    /* IMPLICIT SEQUENCE */
}
SsLcsOptClientExternalId;


typedef struct SsLcsOptMlcNumberTag
{
   Boolean       present;
   Int8          mlcNumberLength;
   Int8          mlcNumber[MAX_MLC_NUMBER_SIZE];
}
SsLcsOptMlcNumber;


typedef struct SsLcsOptGpsAssistanceDataTag
{
   Boolean       present;
   Int8          assistanceDataLength;
   Int8          assistanceData[MAX_ASSISTANCE_DATA_SIZE];
}
SsLcsOptGpsAssistanceData;

typedef struct SsLcsOptSupportedGADShapesTag
{
   Boolean       present;
   Int16         supportedGADShapesBitMask;     /*IMPLICIT BIT STRING */
}
SsLcsOptSupportedGADShapes;


typedef struct SsLcsOptLcsServiceTypeIdTag
{
   Boolean       present;
   Int8          lcsServiceTypeId;
}
SsLcsOptLcsServiceTypeId;


typedef struct SsLcsMOLRDataTag
{
   SsLcsMolrType               lcsMolrType;
   SsLcsOptLocationMethod      lcsOptLocationMethod;
   SsLcsOptQos                 lcsOptQos;
   SsLcsOptClientExternalId    lcsOptClientExternalId;
   SsLcsOptMlcNumber           lcsOptMlcNumber;
   SsLcsOptGpsAssistanceData   lcsOptGpsAssistanceData;
   SsLcsOptSupportedGADShapes  lcsOptSupportedGADShapes;
   SsLcsOptLcsServiceTypeId    lcsOptLcsServiceTypeId;
}
SsLcsMOLRData;



/*
 * MOLR result definitions
 */
typedef struct SsLcsOptLocationEstimateResultTag
{
   Boolean        present;
   Int8           length;
   Int8           data[MAX_LOCATION_ESTIMATE_STRING_SIZE]; /* IMPLICIT OCTET STRING */
}
SsLcsOptLocationEstimateResult;

typedef struct SsLcsOptDecipheringKeysResultTag
{
   Boolean        present;
   Int8           length;
   Int8           data[MAX_DECIPHERING_KEYS_STRING_SIZE]; /* Fixed size of 15 bytes  IMPLICIT OCTET STRING */
}
SsLcsOptDecipheringKeysResult;

typedef struct SsLcsOptAddLocationEstimateResultTag
{
   Boolean        present;
   Int8           length;
   Int8           data[MAX_ADD_LOCATION_ESTIMATE_STRING_SIZE]; /* IMPLICIT OCTET STRING */
}
SsLcsOptAddLocationEstimateResult;


typedef struct SsLcsMOLRResultInfoTag
{
   SsLcsOptLocationEstimateResult        optLocEstimateResult;
   SsLcsOptDecipheringKeysResult         optDecipherKeysResult;
   SsLcsOptAddLocationEstimateResult     optAddLocEstimateResult;
}
SsLcsMOLRResultInfo;



/*
 * MOLR error definition
 */
typedef struct SsLcsOptPosnFailDiagnosticTag
{
   Boolean                       diagnosticPresent;
   SsLcsPosnFailDiagnostic       diagnostic;
}
SsLcsOptPosnFailDiagnostic;


typedef struct SsLcsPositionMethodFailureTag
{
   SsLcsOptPosnFailDiagnostic    posnFailDiagnostic;       /* Enum */
   SsLcsOptExtensionContainer    optExtensionContainer;    /* IMPLICIT SEQUENCE */
}
SsLcsPositionMethodFailure;





/*
 * Types used in
 * LCS-LOCATION_NOTIFICATION_IND
 * message
 */

/* Implicit Enumeration */
typedef enum SsLcsNotificationTypeTag
{
   NOTIFY_LOCATION_ALLOWED                               = 0,
   NOTIFY_AND_VERIFY_LOCATION_ALLOWED_IF_NO_RESPONSE     = 1,
   NOTIFY_AND_VERIFY_LOCATION_NOT_ALLOWED_IF_NO_RESPONSE = 2,
   LOCATION_NOT_ALLOWED                                  = 3
}
SsLcsNotificationType;

/* Implicit Enumeration */
typedef enum SsLcsLocationEstimateTypeTag
{
   CURRENT_LOCATION               = 0,
   CURRENT_OR_LAST_KNOWN_LOCATION = 1,
   INITIAL_LOCATION               = 2,
   ACTIVATE_DEFERRED_LOCATION     = 3,
   CANCEL_DEFERRED_LOCATION       = 4
}
SsLcsLocationEstimateType;

typedef struct SsLcsLocationTypeTag
{
   SsLcsLocationEstimateType    locEstimateType;
   Boolean                      deferredLocationTypePresent;
   Int16                        deferredLocationType;
}
SsLcsLocationType;


/* Implicit Enumeration */
typedef enum SsLcsVerificationResponseTag
{
   PERMISSION_DENIED   =  0,
   PERMISSION_GRANTED  =  1
}
SsLcsVerificationResponse;


typedef enum SsLcsFormatIndicatorTag
{
    FORMAT_INDICATOR_LOGICAL_NAME       =  0,
    FORMAT_INDICATOR_EMAIL_ADDRESS      =  1,
    FORMAT_INDICATOR_MSISDN             =  2,
    FORMAT_INDICATOR_URL                =  3,
    FORMAT_INDICATOR_SIP_URL            =  4
}
SsLcsFormatIndicator;


typedef struct SsLcsLocationNotificationResTag
{
   Boolean                      present;
   SsLcsVerificationResponse    verificationResponse;
}
SsLcsLocationNotificationRes;

typedef struct SsLcsDataCodingSchemeTag
{
   Int8        scheme;
}
SsLcsDataCodingScheme;

typedef struct SsLcsNameStringTag
{
   Int8        length;
   Int8        name[MAX_LCS_NAME_STRING_LENGTH];
}
SsLcsNameString;


typedef struct SsLcsOptFormatIndicatorTag
{
    Boolean               present;
    SsLcsFormatIndicator  formatIndicator;
}
SsLcsOptFormatIndicator;


typedef struct SsLcsOptClientNameTag
{
    Boolean                     present;
    SsLcsDataCodingScheme       dataCodingScheme;
    SsLcsNameString             nameString;
    SsLcsOptFormatIndicator     optFormatIndicator;
}
SsLcsOptClientName;


typedef struct SsLcsOptRequestorIDTag
{
    Boolean                     present;
    SsLcsDataCodingScheme       dataCodingScheme;
    SsLcsNameString             requestorIdString;
    SsLcsOptFormatIndicator     optFormatIndicator;
}
SsLcsOptRequestorID;


typedef struct SsLcsOptCodewordTag
{
    Boolean                     present;
    SsLcsDataCodingScheme       dataCodingScheme;
    Int8                        length;
    Int8                        codewordString[MAX_LCS_CODEWORD_SIZE];
}
SsLcsOptCodeword;


typedef struct SsLcsOptServiceTypeIdTag
{
    Boolean                     present;
    SignedInt8                  serviceTypeId;
}
SsLcsOptServiceTypeId;


typedef struct SsLcsLocNotifyInfoTag
{
    SsLcsNotificationType       notifyType;
    SsLcsLocationType           locationType;
    SsLcsOptClientExternalId    lcsOptClientExternalId;
    SsLcsOptClientName          lcsOptClientName;
    SsLcsOptRequestorID         lcsOptRequestorId;
    SsLcsOptCodeword            lcsOptCodeword;
    SsLcsOptServiceTypeId       lcsOptServiceTypeId;
}
SsLcsLocNotifyInfo;



  /* UPGRADE_AGPS */
#endif





/***************************************************************************
 *  Macros
 **************************************************************************/

#define MAKE_MO_INVOKE_HANDLE(i)        (((i)&0x00FF)|0x0000)
#define MAKE_VG_MO_INVOKE_HANDLE(i)     (((i)&0x00FF)|0x0100)



/***************************************************************************
 * Data Enum Definitions
 **************************************************************************/
/** Supplementary service (SS) */
typedef enum SsCodeTag
{
  /** All supplementary services */
  SS_ALL                       =0x00,
  /** SS all line identification */
  SS_ALL_LINE_IDENT            =0x10,
  /** Calling line identification presentation */
  SS_CLIP                      =0x11,
  /** Calling line identification restriction */
  SS_CLIR                      =0x12,
  /** Connected line identification presentation */
  SS_COLP                      =0x13,
  /** Connected Line Identification Restriction */
  SS_COLR                      =0x14,
  /** Malicious Caller Identification;
   * is a facility which enables a phone call recipient to mark a phone call as malicious
   */
  SS_MCI                       =0x15,
  /** Calling Name Presentation */
  SS_CNAP                      =0x19,
  /** All Call Forwarding services, including Call Deflection */
  SS_ALL_FORWARDING            =0x20,
  /** Call Forwarding Unconditional (cfu), i.e, a call to a mobile subscriber is immediately
   * diverted in the MSC to another number given by the command cfu.
   */
  SS_CFU                       =0x21,
  /** All conditional call forwarding */
  SS_ALL_COND_FORWARDING       =0x28,
  /** Call Forwarding Busy (cfb), i.e, a call to a number is diverted in the MSC to another
   * number given by the command cfb only if the called user is busy
   */
  SS_CFB                       =0x29,
  /** Call Forwarding No Reply (cfnr), i.e, a call to a mobile subscriber is diverted in
   * the MSC to another number given by the command cfnr only if the called party does not react
   */
  SS_CFNRY                     =0x2A,
  /**  Call Forwarding on mobile subscriber Not Reachable */
  SS_CFNRC                     =0x2B,
  /** It permits to a calling user A, encountering a busy destination user B, to "camp-on"
   * to the busy user. This means that the call is indicated to user B and kept in a waiting
   * state until user B reacts to the indication, rather than being released due to the busy condition.
   */
  SS_ALL_CALL_OFFERING         =0x30,
  /** Call Transfer; it enables a user A to transform an existing call (user A - user B)
   * into a new call between user B and a user C selected by user A
   */
  SS_CT                        =0x31,

  SS_MAH                       =0x32,
  /** The Call Completion supplementary services are Completion of Calls to Busy Subscribers
   * (SS-CCBS) and Completion of Calls on No Reply (SS-CCNR)
   */
  SS_ALL_CALL_COMPLETE         =0x40,
  /** Call Waiting; It permits a served user while being busy with one or more calls to
   * be informed of an incoming call from a new user by an indication
   */
  SS_CW                        =0x41,
  /** Call Hold; it allows the served user (holding user), which may be the originally calling or the
   * called user, to interrupt communications on an existing call and then subsequently,
   * if desired, re-establish (i.e retrieve) communications with the held user */
  SS_HOLD                      =0x42,
  /** Completion of Calls to Busy Subscriber */
  SS_CCBS                      =0x43,
  /* see 29002-390 for definitions of these SS-Codes (in binary!) */
  /** The Multicall supplementary service enables a mobile subscriber to have  several
   * simultaneous CS calls, each call using its own dedicated bearer
   */
  SS_MULTICALL                 =0x45,
  SS_ALL_MULTIPARTY            =0x50,
  /** Multiparty: Supplementary Service for speech conference service */
  SS_MULTIPTY                  =0x51,
  SS_ALL_COMM_OF_INT           =0x60,
  /** Closed User Groups who can only make calls and receive calls from members within the group */
  SS_CUG                       =0x61,
  /** All supplementary services related to charging */
  SS_ALL_CHARGING              =0x70,
  /** Advice of Charge Information supplementary service
   * permits the MS (Mobile Station) to display an accurate estimate
   * of the size of the bill, which will eventually be levied in the HPLMN
   */
  SS_AOCI                      =0x71,
  /** Advice of Charge Charging supplementary service; a supplementary
   * service that may be subscribed to enable a user to invoke AoC
   */
  SS_AOCC                      =0x72,
  SS_ALL_ADDITIONAL_INFO_TFR   =0x80,
  /** User-to-User Signalling;
   * the UUS supplementary service allows the served subscriber
   * to send/receive a limited amount of subscriber generated
   * information to/from another user in association with a call
   * to the user
   */
  SS_UUS                       =0x81,
  /** Barring all outgoing and incoming calls */
  SS_ALL_CALL_RESTRICTION      =0x90,
  /** Barring outgoing call */
  SS_BARRING_OF_OUTGOING       =0x91,
  /** Barring of All Outgoing Calls supplementary service;
   * A cellular supplementary service, that when invoked sets
   * flags in the network databases to stop a subscriber making outgoing calls
   */
  SS_BAOC                      =0x92,
  /** Barring all outgoing international calls */
  SS_BOIC                      =0x93,
  /** Barring all outgoing calls except those directed
   * to the Home PLMN Country */
  SS_BOIC_EXHC                 =0x94,
  /** Barring the incoming call */
  SS_BARRING_OF_INCOMING       =0x99,
  /** Barring all incoming calls */
  SS_BAIC                      =0x9A,
  /** Barring all incoming calls while roaming */
  SS_BICROAM                  =0x9B,
#if defined (UPGRADE_AGPS)
  /** all LCS privacy exception classes */
  SS_ALL_LCS_PRIVACY_EXCEPTION = 0xB0,
  /** Allow location by any LCS client */
  SS_LCS_UNIVERSAL             = 0xB1,
  /** allow location by any value added LCS client to which
   * a call is established from the target MS */
  SS_LCS_CALL_RELATED          = 0xB2,
  /** allow location by designated external value added LCS clients */
  SS_LCS_CALL_UNRELATED        = 0xB3,
  /** allow location by designated PLMN operator LCS clients */
  SS_LCS_PLMN_OPERATOR         = 0xB4,
  /** all Mobile Originating location request classes */
  SS_LCS_ALL_MOLR_SS           = 0xC0,
  /** allow an MS to request its own location */
  SS_LCS_BASIC_SELF_LOCATION   = 0xC1,
  /** allow an MS to perform self location without interaction with
   * the PLMN for a pre-determined period of time */
  SS_LCS_AUTONOMOUS_SELF_LOCATION  = 0xC2,
  /** allow an MS to request transfer of its location to another LCS client */
  SS_LCS_TRANSFER_TO_THIRD_PARTY   = 0xC3,
   /* UPGRADE_AGPS */
#endif
  /** PLMN specific  */
  SS_ALL_PLMN_SPECIFIC        =0xF0,
  /** PLMN specific 1 */
  SS_PLMN_SPECIFIC_1          =0xF1,
  /** PLMN specific 2 */
  SS_PLMN_SPECIFIC_2          =0xF2,
  /** PLMN specific 3 */
  SS_PLMN_SPECIFIC_3          =0xF3,
  /** PLMN specific 4 */
  SS_PLMN_SPECIFIC_4          =0xF4,
  /** PLMN specific 5 */
  SS_PLMN_SPECIFIC_5          =0xF5,
  /** PLMN specific 6 */
  SS_PLMN_SPECIFIC_6          =0xF6,
  /** PLMN specific 7 */
  SS_PLMN_SPECIFIC_7          =0xF7,
  /** PLMN specific 8 */
  SS_PLMN_SPECIFIC_8          =0xF8,
  /** PLMN specific 9 */
  SS_PLMN_SPECIFIC_9          =0xF9,
  /** PLMN specific 10 */
  SS_PLMN_SPECIFIC_A          =0xFA,
  /** PLMN specific 11 */
  SS_PLMN_SPECIFIC_B          =0xFB,
  /** PLMN specific 12 */
  SS_PLMN_SPECIFIC_C          =0xFC,
  /** PLMN specific 13 */
  SS_PLMN_SPECIFIC_D          =0xFD,
  /** PLMN specific 14 */
  SS_PLMN_SPECIFIC_E          =0xFE,
  /** PLMN specific 15 */
  SS_PLMN_SPECIFIC_F          =0xFF
}
SsCode;

typedef enum SsIntraCugOptionTag
{
  CUG_NO_RESTRICTIONS     = 0,
  CUG_IC_CALLBARRED       = 1,
  CUG_OG_CALLBARRED       = 2
}
SsIntraCugOption;

typedef enum SsInterCugRestrictionsTag
{
  CUG_ONLY_FACILITY          = 0x00,
  CUG_WITH_OUTGOING_ACCESS  = 0x01,
  CUG_WITH_INCOMING_ACCESS  = 0x02,
  CUG_WITH_OG_AND_IC_ACCES  = 0x03
}
SsInterCugRestrictions;

typedef enum BasicServiceTypeTag
{
  TELE_SERVICE              = 0,
  BEARER_SERVICE            = 1
}
BasicServiceType;

typedef enum  BasicServiceCodeTag
{
  BS_ALL                    = 0x00,
  BS_ALL_DATACDA_SERVICES    = 0x10,
  BS_DATACDA_300BPS          = 0x11,
  BS_DATACDA_1200BPS        = 0x12,
  BS_DATACDA_1200_75BPS      = 0x13,
  BS_DATACDA_2400BPS        = 0x14,
  BS_DATACDA_4800BPS        = 0x15,
  BS_DATACDA_9600BPS        = 0x16,
  BS_ALL_DATACDS_SERVICES    = 0x18,
  BS_DATACDS_1200BPS        = 0x1A,
  BS_DATACDS_2400BPS        = 0x1C,
  BS_DATACDS_4800BPS        = 0x1D,
  BS_DATACDS_9600BPS        = 0x1E,
  BS_ALL_PADACCESSCA_SERVICES  = 0x20,
  BS_PADACCESSCA_300BPS      = 0x21,
  BS_PADACCESSCA_1200BPS    = 0x22,
  BS_PADACCESSCA_1200_75BPS  = 0x23,
  BS_PADACCESSCA_2400BPS    = 0x24,
  BS_PADACCESSCA_4800BPS    = 0x25,
  BS_PADACCESSCA_9600BPS    = 0x26,
  BS_ALL_DATAPDS_SERVICES    = 0x28,
  BS_DATAPDS_2400BPS        = 0x2C,
  BS_DATAPDS_4800BPS        = 0x2D,
  BS_DATAPDS_9600BPS        = 0x2E,
  BS_ALL_ALT_SPEECH_DATACDA  = 0x30,
  BS_ALL_ALT_SPEECH_DATACDS  = 0x38,
  BS_ALL_SPEECH_THEN_DATA_CDA  = 0x40,
  BS_ALL_SPEECH_THEN_DATA_CDS  = 0x48,
  BS_ALL_DATACIRCUIT_ASYNC  = 0x50,
  BS_ALL_ASYNC_SERVICES      = 0x60,
  BS_ALL_DATACIRCUIT_SYNC    = 0x58,
  BS_ALL_SYNC_SERVICES      = 0x68,
  BS_ALL_PLMN_SPECIFIC_BS    = 0xD0,
  BS_PLMN_SPECIFIC_BS_1      = 0xD1,
  BS_PLMN_SPECIFIC_BS_2      = 0xD2,
  BS_PLMN_SPECIFIC_BS_3      = 0xD3,
  BS_PLMN_SPECIFIC_BS_4      = 0xD4,
  BS_PLMN_SPECIFIC_BS_5      = 0xD5,
  BS_PLMN_SPECIFIC_BS_6      = 0xD6,
  BS_PLMN_SPECIFIC_BS_7      = 0xD7,
  BS_PLMN_SPECIFIC_BS_8      = 0xD8,
  BS_PLMN_SPECIFIC_BS_9      = 0xD9,
  BS_PLMN_SPECIFIC_BS_A      = 0xDA,
  BS_PLMN_SPECIFIC_BS_B      = 0xDB,
  BS_PLMN_SPECIFIC_BS_C      = 0xDC,
  BS_PLMN_SPECIFIC_BS_D      = 0xDD,
  BS_PLMN_SPECIFIC_BS_E      = 0xDE,
  BS_PLMN_SPECIFIC_BS_F      = 0xDF,

  TS_ALL                    = 0x00,
  TS_ALL_SPEECH_TX_SERVICES  = 0x10,
  TS_TELEPHONY              = 0x11,
  TS_EMERGENCY_CALLS        = 0x12,
  TS_ALL_SHORT_MSG_SERVICES  = 0x20,
  TS_SHORT_MSG_MT_PP        = 0x21,
  TS_SHORT_MSG_MO_PP        = 0x22,
  TS_VIDEOTEX               = 0x40,
  TS_TELETEX                = 0x50,
  TS_ALL_FAX_TX_SERVICES    = 0x60,
  TS_FAX_G3_AND_ALTSPEECH    = 0x61,
  TS_AUTOMATIC_FAX_GROUP3    = 0x62,
  TS_FAX_GROUP4              = 0x63,
  TS_ALL_DATA_TELESERVICES  = 0x70,
  TS_ALL_EXCEPT_SMS         = 0x80,
  TS_AUX_SPEECH             = 0xD0,
  TS_AUX_TELEPHONY          = 0xD1,
  TS_PLMN_SPECIFIC_BS_2      = 0xD2,
  TS_PLMN_SPECIFIC_BS_3      = 0xD3,
  TS_PLMN_SPECIFIC_BS_4      = 0xD4,
  TS_PLMN_SPECIFIC_BS_5      = 0xD5,
  TS_PLMN_SPECIFIC_BS_6      = 0xD6,
  TS_PLMN_SPECIFIC_BS_7      = 0xD7,
  TS_PLMN_SPECIFIC_BS_8      = 0xD8,
  TS_PLMN_SPECIFIC_BS_9      = 0xD9,
  TS_PLMN_SPECIFIC_BS_A      = 0xDA,
  TS_PLMN_SPECIFIC_BS_B      = 0xDB,
  TS_PLMN_SPECIFIC_BS_C      = 0xDC,
  TS_PLMN_SPECIFIC_BS_D      = 0xDD,
  TS_PLMN_SPECIFIC_BS_E      = 0xDE,
  TS_PLMN_SPECIFIC_BS_F      = 0xDF
}
BasicServiceCode;

/** The status of a supplementary service feature
 * (Call Forwarding or Call Barring).
 * These enumerated values should be used as masks
 */
typedef enum SsStatusTag
{
  /** Active Status */
  SS_ACTIVE                    =0x01,
  /** SS Registered  */
  SS_REGISTERED                =0x02,
  /** Provision established */
  SS_PROVISIONED              =0x04,
  /** Not operative (dormant status) */
  SS_QUIESCENT                =0x08
}
SsStatus;

typedef enum SsForwardOptionsTag
{
  /* these enumerated values should be used as masks */
  SS_NOTIFY_FWD_PARTY          =0x80,
  SS_NOTIFY_CALLED_PARTY      =0x20,
  SS_MS_NOT_REACHABLE          =0x00,
  SS_MS_BUSY                  =0x04,
  SS_NO_REPLY                  =0x08,
  SS_UNCONDITIONAL            =0x0C
}
SsForwardOptions;

/** SS Guidance information element */
typedef enum SsGuidanceInfoTag
{
  /** Enter password (This information is used for checking of the old password) */
  ENTER_PSWD                =0x00,
  /** Enter new password (this information is used during password registration
   * for the request of the first new password)  */
  ENTER_NEW_PSWD            =0x01,
  /** Enter new password again (this information is used during password
   * registration for the request of the new password again for verification) */
  ENTER_NEW_PSWD_AGAIN      =0x02,
  /** Bad password try again */
  BAD_PSWD_TRY_AGAIN        =0x03,
  /** Bad password format try again */
  BAD_PSWD_FORMAT_TRY_AGAIN  =0x04
}
SsGuidanceInfo;

/** Supplementary service operation code element */
typedef enum SsOperationCodeTag
{
  /** register a supplementary service for a particular subscriber */
  MO_REGISTER_SS            =10,
  /** delete a supplementary service that was entered for a particular
   * subscriber using registerSS */
  MO_ERASE_SS               =11,
  /** activate a supplementary service for a particular subscriber  */
  MO_ACTIVATE_SS            =12,
  /** switches off a supplementary service for a particular subscriber  */
  MO_DEACTIVATE_SS          =13,
  /** allows the state of a single supplementary service to be queried
   * for a particular subscriber in the HLR
   */
  MO_INTERROGATE_SS         =14,
  /** SS-Notify */
  MT_NOTIFY_SS              =16,
  /**  used to create or change a password for a supplementary service */
  MO_REGISTER_PWD_SS        =17,
  /** The HLR sends this message if the subscriber wants to change
   * his current password or modify or activate a supplementary service
   */
  MT_GETPASSWORD_SS         =18,
  /** provide a means to support non-GSM standardized supplementary services */
  MO_PROC_USS_DATA          =19,
  /** Forward check ind */
  MT_FWDCHECK_IND_SS        =38,
  /** Process USS request */
  MO_PROC_USS_REQUEST       =59,
  /** USS request */
  MT_USS_REQUEST            =60,
  /** USS notify */
  MT_USS_NOTIFY             =61,
#if defined (UPGRADE_AGPS)
  MO_LCS_MOLR               =115,
  MT_LCS_LOCATION_NOTIFY    =116,
#endif
  /** Forward CLOSED USER GROUP info SS */
  MO_FWD_CUG_INFO_SS        =120,
  /** Split multi party SS */
  MO_SPLIT_MPTY_SS          =121,
  /** Retrieve multi party SS */
  MO_RETRIEVE_MPTY_SS       =122,
  /** Hold multi party SS */
  MO_HOLD_MPTY_SS           =123,
  /** Build multi party SS */
  MO_BUILD_MPTY_SS          =124,
  /** Enable to invoke AoC (Advice of Charge) */
  MT_FWD_CHARGE_ADVICE_SS   =125,
  /** Explicit Call Transfer (ECT) supplementary services */
  MO_ECT_SS                 =126,
  /** SS Operation is null */
  SS_OPERATION_NULL         = 0x7FFF
}
SsOperationCode;

/** Supplementary service error code element */
typedef enum SsErrorCodeTag
{
  /** Error code is null */
  SS_ERROR_CODE_NULL           =0,
  /** Unknown subscriber */
  SS_UNKNOWN_SUBSCRIBER        =1,
  /** illegal subscriber */
  SS_ILLEGAL_SUBSCRIBER        =9,
  /** Bearer service not provisioned */
  SS_BRERSERV_NOT_PROV        =10,
  /** Teleservice not provisioned */
  SS_TELESERV_NOT_PROV        =11,
  /** Illegal equipment */
  SS_ILLEGAL_EQUIPMENT        =12,
  /** Call is barred */
  SS_CALL_BARRED              =13,
  /** operation not allowed */
  SS_ILLEGAL_OPERATION        =16,
  /** Status error */
  SS_ERROR_STATUS              =17,
  /** SS not available in network  */
  SS_NOT_AVAILABLE            =18,
  /** SS subscription violation */
  SS_SUBS_VIOLATION            =19,
  /** Service incompatibility */
  SS_INCOMPATIBILITY          =20,
  /** Facility not supported */
  SS_FACILITY_NOT_SUPPORTED    =21,
  /** Absent subscriber */
  SS_ABSENT_SUBSCRIBER        =27,
  /** System failure */
  SS_SYSTEM_FAILURE            =34,
  /** Data is missing */
  SS_DATA_MISSING              =35,
  /** Unexpected data value */
  SS_UNEXPECTED_DATA_VALUE    =36,
  /** Password registration failure */
  SS_PWD_REGISTRATION_FAILURE  =37,
  /** Negative password check */
  SS_NEGATIVE_PWD_CHECK        =38,
  /** Password Attempts Violation. (subscriber
   * enters a wrong "password" consecutive times.
   */
  SS_NUMOF_PWD_ATTEMPT_VIOL    =43,
#if defined (UPGRADE_AGPS)
  /** Position method failure at a location service execution */
  SS_POSITION_METHOD_FAILURE   =54,
#endif
  /** SS error indicating unknown SS data coding of alphabet */
  SS_UNKNOWN_ALPHABET          =71,
  /** USSD Busy(Already ss / ussd is ongoing). */
  SS_USSD_BUSY                =72,
  /** SS maximum multiparty calls is exceeded */
  SS_MAXMPTY_CALLS_EXCEEDED   =126,
  /** Resources not available */
  SS_RESOURCES_NOT_AVAILABLE  =127
}
SsErrorCode;

/** The call barring cause */
typedef enum SsCallBarCauseTag
{
  /** Activate call barring */
  SS_BARRING_SERV_ACTIVE      = 0,
  /** Operator Determined Barring  */
  SS_OPERATOR_BARRING          = 1
}
SsCallBarCause;

/** Override category element */
typedef enum OverrideCategoryTag
{
  /** Override enabled */
  SS_OVERRIDE_ENABLED         = 0,
  /** Override disabled */
  SS_OVERRIDE_DISABLED        = 1
}
OverrideCategory;
/** SS violation element */
typedef enum SsViolUnionTypeTag
{
  /** Override type */
  SS_OVERRIDE_TYPE            = 1,
  /** CLI restriction */
  SS_CLI_RESTRICTION_TYPE     = 2
}
SsViolUnionType;

typedef Boolean SsAbsentSubscriber;

typedef enum SsSystemFailureTag
{
  SS_SYSFAIL_PLMN              =0,
  SS_SYSFAIL_HLR              =1,
  SS_SYSFAIL_VLR              =2,
  SS_SYSFAIL_PVLR              =3,
  SS_SYSFAIL_CTRL_MSC          =4,
  SS_SYSFAIL_VMSC              =5,
  SS_SYSFAIL_EIR              =6,
  SS_SYSFAIL_RSS              =7
}
SsSystemFailure;

typedef enum SsPwdRegFailureTag
{
  /** */
  SSPWD_UNDETERMINED          =0,
  SSPWD_INVALID_FORMAT        =1,
  SSPWD_NEW_MISMATCH          =2
}
SsPwdRegFailure;

typedef enum SsProblemTagTag
{
  /** General problem (abnormal event detected by the peer) */
  SS_GENERAL_TAG              =0x0,     /* 0x80 & 0x1F */
  /** Invoke problem */
  SS_INVOKE_TAG                =0x1,     /* 0x81 & 0x1F */
  /** Return result problem ( response rejected by the peer) */
  SS_RETURN_RESULT_TAG        =0x2,     /* 0x82 & 0x1F */
  /** Return error problem (response rejected by the peer) */
  SS_RETURN_ERROR_TAG          =0x3      /* 0x83 & 0x1F */
}
SsProblemTag;

/** Supplementary service problem code element */
typedef enum SsProblemCodeTag
{
  /** General problem unrecognised operation (service not supported) */
  SSGEN_UNRECOGNISED          =0x00,
  /** General problem mistyped parameter */
  SSGEN_MISTYPED              =0x01,
  /** General problem bad structure */
  SSGEN_BAD_STRUCTURE          =0x02,
  /** Invoke problem duplicated invoke ID */
  SSINV_DUPLICATE_ID          =0x00,
  /** Invoke problem unrecognised invoke ID */
  SSINV_UNRECOG_ID            =0x01,
  /** Invoke problem mistyped parameter */
  SSINV_MISTYPED              =0x02,
  /** Invoke problem resource limitation */
  SSINV_RESOURCE_LIMIT        =0x03,
  /** Invoke problem initiating release */
  SSINV_INITIATE_RELEASE      =0x04,
  /** Invoke problem unrecognised linked ID
   * (abnormal event received from the peer)
   */
  SSINV_UNRECOG_LINK          =0x05,
  /** Invoke problem linked response unexpected */
  SSINV_LINK_RSP_UNEXP        =0x06,
  /** Invoke problem unexpected linked operation */
  SSINV_UNEXP_LINK_OP          =0x07,

  /** Return result unrecognised invoke ID */
  SSRETRES_UNRECOG_ID          =0x00,
  /** Return result unexpected */
  SSRETRES_UNEXPECTED          =0x01,
  /** Return result mistyped parameter */
  SSRETRES_MISTYPED            =0x02,

  /** Return error unrecognised invoke ID */
  SSRETERR_UNRECOG_ID          =0x00,
  /** Return error unexpected */
  SSRETERR_UNEXPECTED          =0x01,
  /** Return error unrecognised error */
  SSRETERR_UNRECOG_ERR        =0x02,
  /** Return error unexpected error */
  SSRETERR_UNEXP_ERR          =0x03,
  /** Return error mistyped parameter */
  SSRETERR_MISTYPED            =0x04
}
SsProblemCode;

/* The type of SS result */
typedef enum SsResultParameterTypeTag
{
  SS_NO_DATA                    =0,
  /** SS forwarding information */
  SS_FORWARD_INFO                ,
  /** SS call barring information */
  SS_CALL_BARRING_INFO           ,
  /** SS CUG information */
  SS_CUG_INFO                    ,
  /** SS data information */
  SS_INFO_DATA                   ,
  /** SS subscriber status */
  SS_STATUS                      ,
  /** SS forward to number */
  SS_FWD_TO_NUMBER               ,
  SS_BS_GROUP_LIST               ,
  /** SS forward feature list */
  SS_FWD_FEATURE_LIST            ,
  /** USS user data*/
  SS_USS_USER_DATA               ,
  SS_USS_USER_ARG                ,
  SS_RAW_DATA                    ,
  SS_PASSWORD                    ,
  SS_CLIR_INFO
#if defined (UPGRADE_AGPS)
  ,  SS_LCS_MOLR
#endif
}
SsResultParameterType;

typedef enum SsFieOperationTypeTag
{
  SSCT_INVOKE                  =0x01,
  SSCT_RETURN_RESULT          =0x02,
  SSCT_RETURN_ERROR            =0x03,
  SSCT_REJECT                  =0x04,
  SSCT_CORRUPT                =0xFF
}
SsFieOperationType;

typedef enum SsNotificationTag
{
  SS_INCOMING_IS_FWDED_CALL    =0x01,
  SS_INCOMING_FWDED_TO_C      =0x02  ,
  SS_OUTGOING_FWDED_TO_C      =0x04
}
SsNotification;

typedef enum SsCallOnHoldIndicatorTag
{
  SS_CALL_RETRIEVED            =0,
  SS_CALL_ON_HOLD              =1
}
SsCallOnHoldIndicator;

typedef enum PipeTypeTag
{
  MO_PIPE                     =0,
  MT_PIPE                     =1
}PipeType;

typedef enum SsPipeStateTag
{
  SS_PIPE_CLOSED,
  SS_PIPE_OPEN
}
SsPipeState;

/** SS cancel reason parameter */
typedef enum SsCancelReasonTag
{
  /** There is no pipe available */
  SS_NO_PIPES                  =0,
  /** There are other transactions in progress */
  SS_ONGOING_TRANSACTIONS,
  /** mnssRejInd is sent as cancelInd */
  SS_MNSS_REJECT,
  /** There is no controlling task */
  SS_NO_CONTROL,
  /** Invalid invoke ID */
  SS_INVALID_INVOKE_ID
}
SsCancelReason;

typedef enum SsReasonTag     /*job 111331*/
{
  SS_REPLACED_WITH_SS          =0,
  SS_REPLACED_WITH_CALL,
  SS_REPLACED_WITH_USSD,
  SS_BARRED,
  USSD_REPLACED_WITH_USSD,
  USSD_REPLACED_WITH_SS,
  USSD_REPLACED_WITH_CALL,
  USSD_BARRED
}
SsReason;

typedef enum SsEctCallStateTag
{
  SS_ECT_CALL_ALERTING            =0,
  SS_ECT_CALL_ACTIVE              =1
}
SsEctCallState;

typedef enum SsEctRdnTypeTag {
  ECT_ALLOWED_ADDR        = 0,
  ECT_PRES_RESTRICTED     = 1,
  ECT_NO_NA_INTERNETWORK  = 2,
  ECT_PRES_RESTRICT_ADDR  = 3
} SsEctRdnType;

/***************************************************************************
 * Data Struct Definitions
 **************************************************************************/
/** The invokeHandle is a 16-bit value */
typedef Int16  InvokeHandle;

/** Determines the value of handle if present */
typedef struct OptInvokeHandleTag
{
  /** Indicates if handle value is present */
  Boolean        present;
  /** handle value */
  InvokeHandle   handle;
}
OptInvokeHandle;

typedef struct InvokeComponentTag
{
  Int8                          componentTypeTag;
  Int8                          componentLen    ;
  Int8                          invokeIdTag     ;
  Int8                          invokdeIdLen    ;
  Int8                          invokeId        ;
  Int8                          opCodeTag       ;
  Int8                          opCodeLen       ;
  Int8                          opCode          ;
}
InvokeComponent;

typedef struct LinkedInvokeComponentTag
{
  Int8                          componentTypeTag;
  Int8                          componentLen    ;
  Int8                          invokeIdTag     ;
  Int8                          invokeIdLen     ;
  Int8                          invokeId        ;
  Int8                          linkIdTag       ;
  Int8                          linkIdLen       ;
  Int8                          linkId          ;
  Int8                          opCodeTag       ;
  Int8                          opCodeLen       ;
  Int8                          opCode          ;
}
LinkedInvokeComponent;


typedef struct ReturnResultComponentTag
{
  Int8                          componentTypeTag;
  Int8                          componentLen    ;
  Int8                          invokeIdTag     ;
  Int8                          invokeIdLen     ;
  Int8                          invokeId        ;
  Int8                           sequenceTag     ;
  Int8                          sequenceLen     ;
  Int8                          opCodeTag       ;
  Int8                          opCodeLen       ;
  Int8                          opCode          ;
}
ReturnResultComponent;

typedef struct ReturnResultNullComponentTag
{
  Int8                          componentTypeTag;
  Int8                          componentLen    ;
  Int8                          invokeIdTag     ;
  Int8                          invokeIdLen     ;
  Int8                          invokeId        ;
}
ReturnResultNullComponent;

typedef struct ReturnErrorComponentTag
{
  Int8                          componentTypeTag;
  Int8                          componentLen    ;
  Int8                          invokeIdTag     ;
  Int8                          invokeIdLen     ;
  Int8                          invokeId        ;
  Int8                          errorCodeTag    ;
  Int8                          errorCodeLen    ;
  Int8                          errorCode       ;
}
ReturnErrorComponent;

typedef struct RejectComponentTag
{
  Int8                          componentTypeTag;
  Int8                          componentLen    ;
  Int8                          invokeIdTag     ;
  Int8                          invokeIdLen     ;
  Int8                          invokeId        ;
  Int8                          problemCodeTag  ;
  Int8                          problemCodeLen  ;
  Int8                          problemCode     ;
}
RejectComponent;

typedef struct RejectNullComponentTag
{
  Int8                          componentTypeTag;
  Int8                          componentLen    ;
  Int8                          nullTag         ;
  Int8                          nullLen         ;
  Int8                          problemCodeTag  ;
  Int8                          problemCodeLen  ;
  Int8                          problemCode     ;
}
RejectNullComponent;

typedef struct OptNoReplyCondTimeTag
{
/** \assoc PRESENT \ref time */
  Boolean                       present;
  SignedInt8                    time;  /* do not change this to a 2 byte entity*/
}
OptNoReplyCondTime;

typedef struct OptSsForwardOptionsTag
{
/** \assoc PRESENT \ref options */
  Boolean                       present;
  SsForwardOptions              options;
}
OptSsForwardOptions;

/** Supplementary service password element */
typedef struct SsPasswordTag
{
  /** The length of password
\assoc ARRAY \ref password */
  Int8       length;
  /** Holds the password */
  Char       password[MAXPASSWORDLEN];
}
SsPassword, OptSsPassword;

/** CUG index element.
 * The calling user may specify a CUG index to indicate that
 * service to a particular CUG is required. If no CUG index is
 * given then the preferential CUG (if specified) is used by the network.
 */
typedef struct OptSsCugIndexTag
{
  /** Indicates if CUG index is present
\assoc PRESENT \ref value */
  Boolean                        present;
  /** CUG index which is assigned by the network to identify a CUG. */
  Int16                          value  ;
}
OptSsCugIndex;

typedef Int16 EnFwdChargeAdvice;

typedef struct OptChargeInfoTag
{
/** \assoc PRESENT \ref value */
  Boolean                        present;
  EnFwdChargeAdvice              value  ;
}
OptChargeInfo;

/** Supplementary service element */
typedef struct OptSsNotificationTag
{
  /** Indicates if SS notification is present
\assoc PRESENT \ref notification */
  Boolean                        present;
  /** SS notification */
  SsNotification                 notification;
}
OptSsNotification;

/* GSM4.80 Register RESULT forwardedToNumber SIZE(1..9)*/
typedef struct SsFwdToNumberRspTag
{
  Int8                          length;
  Int8                          string[MAX_ISDN_ADDR_LEN];
}
SsFwdToNumberRsp, OptSsFwdToNumberRsp;

/** GSM4.80 Register ARGUMENT forwardedToNumber SIZE(1 to 20) */
typedef struct SsFwdToNumberReqTag
{
  /** Length of number */
  Int8                          length;
  /** Holds the forwarding number */
  Int8                          string[MAX_ADDR_LEN];
}
SsFwdToNumberReq, OptSsFwdToNumberReq;

/** The forwarding sub address element */
typedef struct SsFwdToSubAddrTag
{
  /** The length of forwarding sub address
\assoc ARRAY \ref string */
  Int8                          length;
  /** The forwarding sub address */
  Int8                          string[MAX_ISDN_SUBADDR_LEN];
}
SsFwdToSubAddr, OptSsFwdToSubAddr;

typedef struct OptSsCallOnHoldIndTag
{
  Boolean                       present;
  SsCallOnHoldIndicator         callHoldInd;
}
OptSsCallOnHoldInd;

typedef struct OptSsStatusTag
{
/** \assoc PRESENT \ref status */
  Boolean                       present;
  SsStatus                      status;
}
OptSsStatus;

typedef struct OptSsCodeTag
{
/** \assoc PRESENT \ref code */
  Boolean                       present;
  SsCode                        code;
}
OptSsCode;

typedef Int16 SsPrefCugInd;

typedef struct OptSsPrefCugIndTag
{
/** \assoc PRESENT \ref value */
  Boolean                        present;
  SsPrefCugInd                  value;
}
OptSsPrefCugInd;

typedef struct OptLinkedIdTag
{
/** \assoc PRESENT \ref id */
  Boolean                        present;
  Int8                          id;
}
OptLinkedId;

typedef enum CliRestrictionOptTag
{
    SS_CLIR_PERMANENT                = 0,
    SS_CLIR_TEMP_DEFAULT_RESTR       = 1,
    SS_CLIR_TEMP_DEFAULT_ALLWD       = 2
}
CliRestrictionOpt;


typedef struct CcbsIndexOptTag
{
  Boolean present;
  Int8 ccbsIndex;
}
CcbsIndexOpt;

typedef struct BSubscriberNumberOptTag
{
  Boolean present;
  Int8 bSubscriberNumber[MAX_B_SUBSCRIBER_NUMBER_LEN];
}
BSubscriberNumberOpt;

typedef struct BSubscriberSubaddressOptTag
{
  Boolean present;
  Int8 bSubscriberSubaddress[MAX_B_SUBSCRIBER_SUBADDRESS_LEN];
}
BSubscriberSubaddressOpt;

/** SsBasicServiceType */
typedef Int8 SsBasicServiceType;
/** SsBasicServiceCode */
typedef Int8 SsBasicServiceCode;

/** Supplementary service basic service element */
typedef struct SsBasicServiceTag
{
  SsBasicServiceType            type;  /**< BasicServiceType */
  SsBasicServiceCode            code;  /**< BasicServiceCode */
}
SsBasicService;
/** The optional basic service element */
typedef struct OptBasicServiceTag
{
  /** Indicates if SS basic sevice is present
\assoc PRESENT \ref optbs */
  Boolean                        present;
  /** The SS basic service */
  SsBasicService                optbs;
}
OptBasicService;

typedef struct CcbsFeatureTag
{
  CcbsIndexOpt ccbsIndexOpt;
  BSubscriberNumberOpt bSubscriberNumberOpt;
  BSubscriberSubaddressOpt bSubscriberSubaddressOpt;
  OptBasicService optBasicService;
}
CcbsFeature;

/** Defines priority level element if present */
typedef struct PriorityLevelOptTag
{
  /** Indicates if priority level is present */
  Boolean present;
  /** Priority level */
  PriorityLevel priorityLevel;
}
PriorityLevelOpt;

typedef struct CcbsFeatureListTag
{
  Int8 length;
  CcbsFeature ccbsFeature[MAX_NUM_CCBS_FEATURES];
}
CcbsFeatureList;

typedef struct NbrSBOptTag
{
  Boolean present;
  Int8 nbrSB;
}
NbrSBOpt;

typedef struct NbrUserOptTag
{
  Boolean present;
  Int8 nbrUser;
}
NbrUserOpt;

typedef struct NbrSNOptTag
{
  Boolean present;
  Int8 nbrSN;
}
NbrSNOpt;


typedef struct CliRestrictionInfoTag
{
  SsStatus             ssStatus;
  /* It seems that the cliRestrictionOpt was made optional in
     24080-350, compared with some version of 04.08.  Therefore, to
     avoid disturbing existing code, we do not use the usual structure
     with a `present' flag and the data. */
  Boolean           cliRestrictionOptPresent;
  CliRestrictionOpt    cliRestrictionOpt;
  /* At release 1999 (see 24080-350) the CliRestrictionInfo in the
     ASN.1 is renamed GenericServiceInfo.  To avoid messing up old
     code, we leave the name unchanged, although it is now rather
     confusing, since the following items don't have anything to do
     with CLI restrictions. */
  PriorityLevelOpt  maximumEntitledPriorityOpt;
  PriorityLevelOpt  defaultPriorityOpt;
  CcbsFeatureList   ccbsFeatureList;
  NbrSBOpt      nbrSBOpt;
  NbrUserOpt        nbrUserOpt;
  NbrSNOpt      nbrSNOpt;
}
CliRestrictionInfo;

/** Supplementary service violation */
typedef struct SsSubsViolationTag
{
  /** Type of subscriber violation
 \assoc UNION \ref code */
  SsViolUnionType               type;
  union
  {
    /** CLI restriction option */
    CliRestrictionOpt             cliRestrictionOpt;
    /** Determines override */
    OverrideCategory              override;
  }
  code;
}
SsSubsViolation, SsSubscriptionOption;

typedef struct OptSsSubsOptionTag
{
/** \assoc PRESENT \ref option */
  Boolean                        present;
  SsSubscriptionOption           option ;
}
OptSsSubsOption;

typedef struct SsBasicServiceGroupListTag
{
  Int8                          length;
  SsBasicService                bs[MAXBASICSERVICEGROUPLIST];
}
SsBasicServiceGroupList;

typedef struct SsCugFeatureTag
{
  OptBasicService               optBasicService;
  OptSsPrefCugInd               optSsPrefCugInd;
  SsInterCugRestrictions        ssInterCugRestrictions;
}
SsCugFeature;

typedef struct SsCallBarFeatureTag
{
  OptBasicService                optBasicService;
  OptSsStatus                    optSsStatus    ;
}
SsCallBarFeature;

typedef struct SsCugFeatureListTag
{
  Int8                          length;
  SsCugFeature                  cugFeature[MAXBASICSERVICEGROUPS];
}
SsCugFeatureList;

typedef struct SsCallBarFeatureListTag
{
  Int8                          length;
  SsCallBarFeature              feature[MAXBASICSERVICEGROUPS];
}
SsCallBarFeatureList;

typedef struct SsCugSubscriptionTag
{
  Int16                         cugIndex;
  Int8                          cugInterLock[MAXCUGINTELOCKLEN];
  SsIntraCugOption              intraCugOption;
  SsBasicServiceGroupList       bsList;
}
SsCugSubscription;

typedef struct SsFwdFeatureTag
{
  OptBasicService               optBasicService;
  OptSsStatus                   optSsStatus;
  OptSsFwdToNumberRsp           optFwdToNumberRsp;
  OptSsFwdToSubAddr             optFwdToSubAddr;
  OptSsForwardOptions           optFwdOptions;
  OptNoReplyCondTime            optNoReplyCondTime;
}
SsFwdFeature;

typedef struct SsCugSubscriptionListTag
{
  Int8                          length;
  SsCugSubscription             cugSubscription[MAXNUMOFCUG];
}
SsCugSubscriptionList;

typedef struct SsFwdFeatureListTag
{
/** \assoc ARRAY \ref feature */
  Int8                          length;
  SsFwdFeature                  feature[MAXBASICSERVICEGROUPS];
}
SsFwdFeatureList;

typedef struct SsCugInfoTag
{
  SsCugSubscriptionList         subsList;
  SsCugFeatureList              ftrList;
}
SsCugInfo;

#if 0
typedef struct SsCugGroupDescTag
{
  Char              cugName[DISPLAY_WIDTH-1];
  SignedInt16       cugIndex;
} SsCugGroupDesc;
#endif

typedef struct SsForwardInfoTag
{
  OptSsCode                     optSsCode;
  SsFwdFeatureList              fwdFeatureList;
}
SsForwardInfo;

typedef struct SsIncompatibilityTag
{
  OptSsCode                      optSsCode       ;
  OptBasicService                optBasicService ;
  OptSsStatus                    optSsStatus     ;
}
SsIncompatibility;


typedef struct SsCallBarringInfoTag
{
  OptSsCode                      optSsCode ;
  SsCallBarFeatureList           callBarFeatureList ;
}
SsCallBarringInfo;

typedef struct SsInfoDataTag
{
  OptSsCode                      optSsCode        ;
  OptSsStatus                    optSsStatus      ;
  OptSsSubsOption                optSsSubsOption  ;
  SsBasicServiceGroupList        bsList ;
  PriorityLevelOpt      defaultPriorityOpt;
  NbrUserOpt            nbrUserOpt;
}
SsInfoData;

typedef struct SsIntrgStatusTag
{
  SsStatus                      ssStatus ;
}
SsIntrgStatus;

typedef struct UssUserDataTag
{
/** \assoc ARRAY \ref data */
  Int8                          length   ;
  Int8                          data[MAX_USS_DATA_LEN];
}
UssUserData;

typedef struct UssdStringTag
{
/** \assoc ARRAY \ref data */
  Int8                          length   ;
  Int8                          data[MAX_USS_DATA_LEN];
}
UssdString;

/** SsDecodeScheme */
typedef Int8 SsDecodeScheme;

/** USS user argument element */
typedef struct UssUserArgTag
{
  /** Data coding scheme */
  SsDecodeScheme                dataCodingScheme ;
  /** USSD argument string */
  UssdString                    argString        ;
}
UssUserArg;

typedef struct SsRawDataTag
{
  Int8                          length;
  Int8                          data[MAX_SSRAW_DATA_LEN];
}
SsRawData;

/** SS forward closed user group information */
typedef struct SsFwdCugInfoTag
{
  /** Indicates CUG index.
   * The CUG supplementary service enables users to form groups to and from
   * which access is restricted
   */
  OptSsCugIndex                  optCugIndex ;
  /** Indicates if suppression of a preferential CUG is present */
  Boolean                        suppressPrefCugPresent ;
  /** Indicates if suppression of the OA capability  is present */
  Boolean                        suppressOAPresent;
}
SsFwdCugInfo;

/** The SS forward closed user group information element */
typedef struct OptSsFwdCugInfoTag
{
  /** Indicates if CUG is set*/
  Boolean                present;
  /** CUG group ref no (0 to MAXNUMOFCUG-1) */
  Int8                   cugGroupNo;
  /** Defines the SS forward closed user group information. */
  SsFwdCugInfo           ssFwdCugInfo;
}
OptSsFwdCugInfo;

typedef struct SsFwdChargeAdviceTag
{
  SsCode                        ssCode   ;
  OptChargeInfo                 optChargeInfo[MAXCHARGEBANDS] ;
}
SsFwdChargeAdvice;

/** SS error parameter type element */
typedef enum SsErrorParmTypeTag
{
  /** NO error */
  SSERR_NONE              = 0,
  /** Call barred */
  SSERR_CALLBARCAUSE,
  /** Status error */
  SSERR_STATUS,
  /** Subscriber violation */
  SSERR_SUBSVIOLATION,
  /** Subscriber is absent */
  SSERR_ABSENTSUBSCRIBER,
  /** System failure */
  SSERR_SYSTEMFAILURE,
  /** Password registration failure */
  SSERR_PWDREGFAILURE,
  /** Incompatibility of service */
  SSERR_INCOMPATIBILITY
#if defined (UPGRADE_AGPS)
  , SSERR_POSNMETHODFAILURE  /**< Position method failure at a location service execution */
#endif
}
SsErrorParmType;

/** Supplementary service error parameter element */
typedef union SsErrorParameterTag
{
  /** Dummy variable */
  Int8                        dummy            ;
  /** Determines why the call is barred */
  SsCallBarCause              callBarCause     ;
  /** Status of a supplementary service */
  SsStatus                    status           ;
  /** Subscriber violation */
  SsSubsViolation              subsViolation    ;
  /** Absent violation */
  SsAbsentSubscriber          absentSubscriber ;
  /** System failure */
  SsSystemFailure              systemFailure    ;
  /** Password registration failure */
  SsPwdRegFailure              pwdRegFailure    ;
  /** Incompatibility */
  SsIncompatibility            incompatibility  ;
#if defined (UPGRADE_AGPS)
  /** Position method failure at a location service execution */
  SsLcsPositionMethodFailure   positionMethodFailure;
#endif
}
SsErrorParameter;

/** SS reject problem element */
typedef struct SsRejectProblemTag
{
  /** SS problem code */
  SsProblemCode                code     ;
  /** Problem */
  SsProblemTag                tag      ;
}
SsRejectProblem;

/** SS error code data element */
typedef struct SsErrorCodeDataTag
{
  /** Indicates the SS error */
  SsErrorCode                 code            ;
  /** Indicates the type of SS error
\assoc UNION \ref parameter */
  SsErrorParmType             type            ;
  /** Indicates the parameter of SS error */
  SsErrorParameter            parameter       ;
}
SsErrorCodeData;

/** ss result operation code element */
typedef struct OptSsResultOpCodeTag
{
  /** Indicates if operation code is present
\assoc PRESENT \ref operCode */
  Boolean                     present         ;
  /** Determines the operation code */
  SsOperationCode             operCode        ;
}
OptSsResultOpCode;

typedef enum  AbssComponentTypeTag
{
  ABSS_COMP_INVOKE = 1,
  ABSS_COMP_ERROR,
  ABSS_COMP_REJECT,
  ABSS_COMP_RESULT,
  ABSS_COMP_NULL
}
AbssComponentType;

/** Component data element */
typedef union AbssComponentDataTag
{
  /** Invoke operation code */
  SsOperationCode             invOperCode     ;
  /** Determines the result */
  OptSsResultOpCode           optResult       ;
  /** Indicates the error */
  SsErrorCodeData             error           ;
  /** Reject error */
  SsRejectProblem             problem         ;
}
AbssComponentData;

typedef struct SsFieParsedHeaderTag
{
  SsFieOperationType          operationType   ;
  OptLinkedId                 optInvokeId     ;
  OptLinkedId                 optLinkedId     ;

/** \assoc UNION \ref compData */
  AbssComponentType           compType        ;  /* This is for TVIEW only */
  AbssComponentData           compData        ;

  Int8                        parOffset       ;
  Int8                        componentLen    ;  /* Length of the component */
}
SsFieParsedHeader;


typedef struct PresentAllowedAddrTag  {
  Char partyNumber [20];
  Char partyNumberSubaddress[21];
} PresentAllowedAddr;

typedef struct PresentRestrictAddrTag {
  Char partyNumber [20];
  Char partyNumberSubaddress[21];
} PresentRestrictAddrTag;

typedef union SsRdnValueTag {
  PresentAllowedAddr     presentAllowedAddr;
  Int8                   dummy1;  /* Allow for union association */
  Int8                   dummy2;  /* Allow for union association */
  PresentRestrictAddrTag presentRestrictAddr;
} SsRdnValue;

typedef struct OptSsEctRdnTag {
  /** \assoc PRESENT \ref ssEctRdnType
   *  \assoc PRESENT \ref ssRdnValue
   */
  Boolean      present;
  /** \assoc UNION \ref ssRdnValue */
  SsEctRdnType ssEctRdnType;
  SsRdnValue   ssRdnValue;
} OptSsEctRdn;

typedef struct OptSsEctIndTag {
  Boolean                      present       ;
  SsEctCallState               ssEctCallState;
  OptSsEctRdn                  optSsEctRdn   ;
} OptSsEctInd;


typedef struct UssdCnapStringTag
{
  Int8                          length   ;
  Int8                          data[MAX_CNAP_NAME_LENGTH];
}
UssdCnapString;

/** Specifies the type of calling name presentation (CNAP) */
typedef enum  CnapSsNameTypeTag
{
  /** Calling Name Info is allowed */
  CNAP_NAME_PRESENT_ALLOWED = 0,
  /** Presentation Indicator value is restricted */
  CNAP_PRESENT_RESTRICTED = 1,
  /** Name is unavailable */
  CNAP_NAME_UNAVAILABLE = 2,
  /** Name is present but restricted */
  CNAP_NAME_PRESENT_RESTRICTED = 3
} CnapSsNameType;

typedef struct CnapNameSetTag
{
  SsDecodeScheme  dataDecodingScheme;
  Int16           lengthInChars;
  UssdCnapString  nameString;
} CnapNameSet;

typedef union CnapSsNameValTag
{
/*  Boolean     present; */
  CnapNameSet namePresentAllowed;
  Int8        dummy1;  /* Allow for union association */
  Int8        dummy2;  /* Allow for union association */
  CnapNameSet namePresentRestricted;
}CnapSsNameVal;

/** Provides information on supplementary service (SS)CNAP */
typedef struct CnapSsNameTag
{
  /** Whether or not the information about SS cnap is present
   *  \assoc PRESENT \ref ssNameType
   *  \assoc PRESENT \ref ssNameVal
   */
  Boolean         present;
  /** Specifies the type of CNAP name
\assoc UNION \ref ssNameVal */
  CnapSsNameType  ssNameType;
  /** Determines the information on the specified CNAP name */
  CnapSsNameVal   ssNameVal;
} CnapSsName;

/** The calling name information of the calling party includes either the calling name
 * identity or an indication of privacy or unavailability
 */
typedef struct OptCnapSsNameIndTag
{
  /** Indicates if the calling name is present */
  Boolean present;
  /** SS calling name information */
  CnapSsName  ssCallingName;
} OptCnapSsNameInd;


typedef struct AlertingPatternOptTag
{
  Boolean present;
  Int8 alertingPattern;
}
AlertingPatternOpt;

typedef enum MulticallIndicatorTag
{
  NBR_SN_EXCEEDED = 0,
  NBR_USER_EXCEEDED = 1
}
MulticallIndicator;

typedef struct MulticallIndicatorOptTag
{
  Boolean present;
  MulticallIndicator multicallIndicator;
}
MulticallIndicatorOpt;


typedef struct SsNotifyArgTag
{
  OptSsCode                    optSsCode    ;
  OptSsStatus                  optSsStatus  ;
  OptSsNotification            optSsNotification     ;
  Boolean                      callsWaitingIndicator ;
  OptSsCallOnHoldInd           optSsCallOnHoldInd    ;
  Boolean                      mptyIndicator         ;
  OptSsCugIndex                optCugIndex           ;
  Boolean                      clirSuppressionRejected ;
  OptSsEctInd                  optSsEctInd           ;
  OptCnapSsNameInd             optCnapSsNameInd;
  CcbsFeatureList       ccbsFeatureList;
  AlertingPatternOpt        alertingPatternOpt;
  MulticallIndicatorOpt     multicallIndicatorOpt;
}
SsNotifyArg;


typedef union SsComponentUnionTag
{
  InvokeComponent              inv      ;
  LinkedInvokeComponent        linkinv  ;
  ReturnResultComponent        result   ;
  ReturnErrorComponent        err      ;
  RejectComponent              rej      ;
  RejectNullComponent         rejNull  ;
}
SsComponentUnion;

typedef union SsResultDataTag
{
  Int8                        dummy            ; /* for TView assocation */
  SsForwardInfo                forwardInfo      ;
  SsCallBarringInfo            callBarringInfo  ;
  SsCugInfo                    cugInfo          ;
  SsInfoData                  infoData         ;
  SsStatus                    status           ;
  SsFwdToNumberRsp            fwdToNumberRsp   ;
  SsBasicServiceGroupList      bsGroupList      ;
  SsFwdFeatureList            fwdFeatureList   ;
  UssUserData                  userData         ;
  UssUserArg                  userArg          ;
  SsRawData                    rawData          ;
  SsPassword              password     ;
  CliRestrictionInfo          cliRestrictionInfo;
#if defined (UPGRADE_AGPS)
  SsLcsMOLRResultInfo         molrResultInfo;
#endif
}
SsResultData;

typedef enum AsnDecodeErrorTag
{ /* The asn template decode routine returns one of the */
  /* following values */
  ASNERR_NONE               = 0,
  ASNERR_TAG_MISMATCH,        /* when the tag encountered is unexpected    */
  ASNERR_DATA_LENGTH_RANGE,   /* when the content len is outside the range */
                              /* minlen .. maxlen as specified for each    */
                              /* primitive                                 */
  ASNERR_SIZE_INCONSISTENT,   /* the length of content implies crossing    */
                              /* packet boundary. This error is returned   */
                              /* when the asn packet is parsed             */
  ASNERR_SEQOF_ARRAY_TOOBIG,  /* When a sequence of is being decode, a max */
                              /* value is specified in the template table  */
                              /* which corresponds to the max value in the */
                              /* asn definition of the packet. If this is  */
                              /* exceeded then the decoding is aborted with*/
                              /* this error code                           */
  ASNERR_SEQOF_ARRAY_TOOSMALL,/* When a sequence of is being decode, a min */
                              /* value is specified in the template table  */
                              /* which corresponds to the min value in the */
                              /* asn definition of the packet. If this is  */
                              /* not reached then the decoding is aborted  */
                              /* with this error code                      */
  ASNERR_ITEM_MISSING,        /* A mandatory item is missing               */
  ASNERR_NO_CHOICE_MATCH,     /* This error code is returned when a choice */
                              /* selection which is mandatory resulted in  */
                              /* no selections being made                  */
  ASNERR_CORRUPT_TEMPLATE,    /* this occurs when a template opcode is not */
                              /* recognised. This may occur if the template*/
                              /* pointer has overrun.                      */
  ASNERR_NO_END_OF_CONTENT,   /* end of content missing                    */
  ASNERR_SEQ_CONTENT_OVERFLOW,
  ASNERR_SEQOF_CONTENT_OVERFLOW,
                              /* when a seq of is being decoded, if the    */
                              /* pointer crosses over the content data     */
                              /* boundary this error is returned, it       */
                              /* data corruption                           */
  ASNERR_CORRUPT_FIE,
  ASNERR_PASSWORD_TOO_LONG,   /* a password is longer than the structure it should be stored in */
#if defined (UPGRADE_AGPS)
  ASNERR_LCS_LOC_ESTIMATE_TOO_LONG,
  ASNERR_LCS_DECIPHERING_KEYS_TOO_LONG,
  ASNERR_LCS_ADD_LOC_ESTIMATE_TOO_LONG,
  ASNERR_LCS_CLIENT_EXTERNAL_ADDRESS_TOO_LONG,
  ASNERR_LCS_NAME_STRING_LENGTH_TOO_LONG,
  ASNERR_LCS_CODEWORD_TOO_LONG,
#endif
  ASNERR_NULL
} AsnDecodeError;

/** SS error code passed from AB to AF */
typedef enum SsErrorTypeTag
{
    /** No error present */
    SS_NO_ERROR,
    /** Time out */
    SS_TIMED_OUT,
    /** Operation error */
    SS_OPERATION_ERROR
} SsErrorType;





/* for emacs users: this file uses indentation by 2 spaces */
/* Local Variables: */
/* c-basic-offset: 2 */
/* End: */

#endif

/* END OF FILE */

