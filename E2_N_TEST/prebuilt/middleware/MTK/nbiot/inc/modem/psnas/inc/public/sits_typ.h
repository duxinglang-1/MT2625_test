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
 *      types common to ts_sig.h and alsi_sig.h
 **************************************************************************/

#ifndef SITS_TYP_H
#define SITS_TYP_H

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

#include <system.h>

/****************************************************************************
 * Types
 ****************************************************************************/
/** Sms routing type */
typedef enum SmsRouteTag
 {
     /* Don't change the order of these items as they may be  */
     /* identical to those typed in by CI user to identify    */
     /* required routing type (i.e 1 to 1 correspondance)     */
        /** GSM route via GPRS */
        GPRS_GSMS_ROUTE_GPRS,
        /** GSM route via CS */
        GPRS_GSMS_ROUTE_CS,
        /** GSM route via GPRS is preferred*/
        GPRS_GSMS_ROUTE_GPRS_PREF,
        /** GSM route via CS is preferred*/
        GPRS_GSMS_ROUTE_CS_PREF
        ,GPRS_GSMS_ROUTE_GPRS_NOT_AVAIL_BUT_PREF /**< GSM route GPRS not available but it is preferred*/
        ,TEST_LOOP_ROUTE

 } SmsRoute;

/** Specifies the type of SMS operation to perform */
typedef enum SmsCommandTag
{
   /** Enquiry relating to previously submitted short message */
   SMS_CMD_ENQUIRY              =   0,
   /** Cancel Status Report Request relating to previously submitted short message */
   SMS_CMD_CANCEL,
   /** Delete previously submitted Short Message */
   SMS_CMD_DELETE,
   /** Enable Status Report Request relating to previously submitted short message */
   SMS_CMD_ENABLE_SRR
}
SmsCommand;

/** Indicates the status of a previously submitted SMS SUBMIT and certain 
 * SMS COMMANDS for which a Status  Report has been requested
 */
typedef enum SmsStatusTag
{
   /** Short message received by the SME */
   ST_SM_RECEIVED_BY_SME        =   0,
   /** Short message forwarded by the SC to the SME */
   ST_SM_FORWARDED_TO_SME,
   /** Short message replaced by the SC */
   ST_SM_REPLACED_BY_SC,

   /** Congestion */
   ST_TA_CONGESTION             =   32,
   /** SME busy */
   ST_TA_SME_BUSY,
   /** No response from SME */
   ST_TA_NO_RESPONSE,
   /** Service rejected */
   ST_TA_SERVICE_REJ,
   /** Quality of service not available */
   ST_TA_QUAL_SER_NAV,
   /** Error in SME */
   ST_TA_ERR_IN_SME,

   /** Remote procedure error */
   ST_P_REM_PROC_ERR            =   64,
   /** Incompatible destination */
   ST_P_INCOMPAT_DEST,
   /** Connection rejected by SME */
   ST_P_CONN_REJ_BY_SME,
   /** Not obtainable */
   ST_P_NOT_OBTAINABLE,
   /** Quality of service not available */
   ST_P_QUAL_SER_NAV,
   /** No interworking available */
   ST_P_NO_IWORK_AV,
   /** SM Validity Period Expired */
   ST_P_SM_VALIDITY_EXPIRED,
   /** SM Deleted by originating SME */
   ST_P_SM_DEL_BY_ORIG_SME,
   /** SM Deleted by SC Administration */
   ST_P_SM_DEL_BY_SC_ADMIN,
   /** SM does not exist  */
   ST_P_SM_NOT_EXIST,

   /** Congestion */
   ST_TI_CONGESTION             =   96,
   /** SME busy */
   ST_TI_SME_BUSY,
   /** No response from SME */
   ST_TI_NO_RESPONSE,
   /** Service rejected */
   ST_TI_SERVICE_REJ,
   /** Quality of service not available */
   ST_TI_QUAL_SER_NAV,
   /** Error in SME */
   ST_TI_ERR_IN_SME,
   
   /** ACK */
   ST_ACK                       =   128,
   /** NACK */
   ST_NACK
}
SmsStatus;

/** This is the validity period format for a short message. 
 * TP Validity Period parameter value indicates the time period 
 * for which the short message is valid, i.e. for how long the 
 * SC shall guarantee its existence in the SC memory before delivery 
 * to the recipient has been carried out.
 */
typedef enum VpFormatTag
{
   /** No Validity Period specified */
   VP_NOT_PRESENT               =   0,
   /** Reserved */
   VP_RESERVED,
   /** Validity period is relative in integer representation and the 
    * following octet contains the TP-VP value in the range 0 to 255 
    * representing 0 to 255 seconds (A TP-VP value of zero is undefined 
    * and reserved for future use).
    */
   VP_INTEGER_FORMAT,
   /** Validity period is relative in semi-octet representation
    * and the following 3 octets contain the relative time in Hours, 
    * Minutes and Seconds giving the length of the validity period 
    * counted from when the SMS-SUBMIT is received by the SC.
    */
   VP_SEMI_OCTET
}
VpFormat;

/** Specifies a protocol Id */
typedef enum ProtocolMeaningTag
{
   /** no interworking, but SME to SME protocol */
   PM_SM_AL_PROTOCOL            = 0x00,
   /** Telematic interworking */
   PM_TELEMATIC_INTERWORK       = 0x20,
   /** Short Message functions */
   PM_SM_FUNCTIONS              = 0x40,
   /** ME functions */
   PM_ME_FUNCTIONS              = 0x60,
   /** Reserved */
   PM_RESERVED                  = 0x80,
   /** SC specific use */
   PM_SC_SPECIFIC               = 0xc0,
   PM_RAW_DATA           /**< Internal value */
}
ProtocolMeaning;

/** Data coding scheme values */
typedef enum MsgCodingTag
{
   /** GSM default alphabet 7 bits packed */
   MSG_CODING_DEFAULT           = 0x00,
   /** GSM default alphabet 8 bits */
   MSG_CODING_8BIT              = 0x01,
   /** UCS2 */
   MSG_CODING_UCS2              = 0x02,
   /** Reserved */
   MSG_CODING_RESERVED          = 0x03,
   /** Number of coding */
   MSG_CODING_NUM_OF
}
MsgCoding;

/** Message class element */
typedef enum MsgClassTag
{
   /** Class 0 */
   MSG_CLASS0                   = 0x00,
   /** Class 1 Default meaning: ME-specific.*/
   MSG_CLASS1                   = 0x01,
   /** Class 2 (U)SIM specific message */
   MSG_CLASS2                   = 0x02,
   /** Class 3 Default meaning: TE specific */
   MSG_CLASS3                   = 0x03,
   /** Class not given */
   MSG_CLASS_NOT_GIVEN,
   /** Number of classes */
   MSG_CLASS_NUM_OF
}
MsgClass;

/** Indicates different types of telematic devices */
typedef enum ProtInterWorkingTag
{
   /** implicit */
   PID_IMPLICIT                 =   0,
   /** telex (or teletex reduced to telex format)*/
   PID_TELEX,
   /** group 3 telefax */
   PID_GRP3_FAX,
   /** group 4 telefax */
   PID_GRP4_FAX,
   /** voice telephone (conversion to speech) */
   PID_VOICE,
   /** ERMES (European Radio Messaging System) */
   PID_ERMES,
   /** National Paging system (known to the SC) */
   PID_NAT_PAGING,
   /** Videotex (T.100/T.101) */
   PID_VIDEOTEX,
   /** teletex, carrier unspecified */
   PID_UNSPEC_TELETEX,
   /** teletex, in PSPDN */
   PID_PSPDN_TELETEX,
   /** teletex, in CSPDN */
   PID_CSPDN_TELETEX,
   /** teletex, in analog PSTN */
   PID_PSTN_TELETEX,
   /** teletex, in digital ISDN */
   PID_ISDN_TELETEX,
   /** UCI (Universal Computer Interface, ETSI DE/PS 3 01 3)*/
   PID_UCI,

   /** a message handling facility (known to the SC) */
   PID_MSG_HANDLING             =   16,
   /** any public X.400 based message handling system */
   PID_X400,
   /** Internet Electronic Mail */
   PID_INTERNET_EMAIL,

   /** Specific to SC 1 */
   PID_SC_SPECIFIC_1            =   24,
   /** Specific to SC 2 */
   PID_SC_SPECIFIC_2,
   /** Specific to SC 3 */
   PID_SC_SPECIFIC_3,
   /** Specific to SC 4 */
   PID_SC_SPECIFIC_4,
   /** Specific to SC 5 */
   PID_SC_SPECIFIC_5,
   /** Specific to SC 6 */
   PID_SC_SPECIFIC_6,
   /** Specific to SC 7 */
   PID_SC_SPECIFIC_7,
   /** A GSM mobile station */
   PID_GSM_MS
}
ProtInterWorking;

/** A range of values in the Protocol Identifier which allows an indication 
 * to be sent with a short message (MT or MO) that the short message is of 
 * a particular type allowing the receiving MS or the SC to replace an 
 * existing message of the same type held in the SC
 */
typedef enum ProtScCommandTag
{
   /** Short Message Type 0 */
   PID_SM_TYPE0                 =   0,
   /** Replace Short Message Type 1 */
   PID_REPLACE_SM_TYPE1,
   /** Replace Short Message Type 2 */
   PID_REPLACE_SM_TYPE2,
   /** Replace Short Message Type 3 */
   PID_REPLACE_SM_TYPE3,
   /** Replace Short Message Type 4 */
   PID_REPLACE_SM_TYPE4,
   /** Replace Short Message Type 5 */
   PID_REPLACE_SM_TYPE5,
   /** Replace Short Message Type 6 */
   PID_REPLACE_SM_TYPE6,
   /** Replace Short Message Type 7 */
   PID_REPLACE_SM_TYPE7,

   /** Return Call Message */
   PID_RETURN_CALL_MSG          = 0x1F
}
ProtScCommand;

/** Required for case when PID byte has bit7 = 0 and bit6=1 (see GSM 03.40) */
typedef enum ProtMeCommandTag
{
   /** ME De personalization Short Message */
   PID_DEPERSONALISATION_SM     = 0x3E,
   /** SIM Data download */
   PID_SIM_DATA_DOWNLOAD
}
ProtMeCommand;

/** This data type is a union used in the SmsProtocolId structure.  
 * It refers to either the higher layer protocol being used, or 
 * indicates interworking with a certain type of telematic device.  
 * The SmsProtocolId structure also contains a meaning field.  
 * This field is used to indicate which member of the union is valid.
 */
typedef union ProtocolIdTag
{
   /** meaning = PM_SM_AL_PROTOCOL */
   ProtInterWorking                 protSmAlProtocol;
   /** meaning = PM_TELEMATIC_INTERWORK */
   ProtInterWorking                 protInterWorking;
   /** meaning = PM_SM_FUNCTIONS */
   ProtScCommand                    protScCommand;
   /** meaning = PM_ME_FUNCTIONS */
   ProtMeCommand                    protMeCommand;
   /** meaning = PM_RESERVED */
   Int8                             protReserved;
   /** meaning = PM_SC_SPECIFIC */
   Int8                             protScSpecific;
   /** raw data format */
   Int8                             data;
}
ProtocolId;

/** The Protocol Identifier information element */
typedef struct SmsProtocolIdTag
{
   /** Indicates which member of the ProtocolId union is valid.
\assoc UNION \ref protocolId */
   ProtocolMeaning                  protocolMeaning;
   /** Identifies a protocol */
   ProtocolId                       protocolId;
}
SmsProtocolId;

/** Indication Type element */
typedef enum SmsIndTypeTag
{
    /** Voicemail Message Waiting */
    SMS_IND_VOICE_MESSAGE_WAITING  = 0x00,
    /** Fax Message Waiting */
    SMS_IND_FAX_MESSAGE_WAITING    = 0x01,
    /** Electronic Mail Message Waiting */
    SMS_IND_EMAIL_MESSAGE_WAITING  = 0x02,
    /** Other Message Waiting */
    SMS_IND_OTHER_MESSAGE_WAITING  = 0x03,
    /** Number of INDs */
    SMS_IND_NUM_OF_INDS

} SmsIndType;

/** Message Waiting Indication Group element */
typedef struct SmsMsgWaitingIndTag
{
    /** Whether Store Message or Discard Message */
    Boolean                         storeMessage;
    /** Whether or not indication is active */
    Boolean                         indActive;
    /** Specifies the indication type */
    SmsIndType                      indType;

} SmsMsgWaitingInd;

/** SMS data coding element */
typedef struct SmsDataCodingTag
{
   /** Specifies Data coding scheme value */
   MsgCoding                        msgCoding;
   /** Specifies the message class */
   MsgClass                         msgClass;
   /** Whether the text is compressed */
   Boolean                          compressedText;
   /** Indicates if Message Waiting Indication Group is present
\assoc PRESENT \ref msgWaitingInd */
   Boolean                          msgWaitingIndPresent;
   /** Determines Message Waiting Indication Group */
   SmsMsgWaitingInd                 msgWaitingInd;
   /** Message Marked for Automatic Deletion Group */
   Boolean                          markedForAutomaticDeletion;
}
SmsDataCoding;

#endif
/* END OF FILE */




































