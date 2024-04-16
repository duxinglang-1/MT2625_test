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
 *      application layer  - Sms Tl task interface
 **************************************************************************/

#ifndef TS_SIG_H
#define TS_SIG_H

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

#if !defined (SITS_TYP_H)
#include     <sits_typ.h>
#endif

#if !defined (TL_TYP_H)
#include     <tl_typ.h>
#endif

/** \defgroup SigSmsSmts SMTS Signalling Interface
 * \ingroup SigSms
 * \ingroup Sig 
 * Interface between SM-TL and Application layer.
 * @{
 */
/****************************************************************************
 * Macros
 ****************************************************************************/

/****************************************************************************
 * Defines
 ****************************************************************************/

/** The maximum length of user data */
#define  SMS_MAX_USER_DATA_LENGTH                       (140)
#define  SMS_DELIVER_REPORT_MAX_USER_DATA_LENGTH        (159)

/****************************************************************************
 * Types
 ****************************************************************************/
/** Whether the sms status report is the result of an SMS-SUBMIT or an SMS COMMAND */
typedef enum  SmsStatusReportQualTag
{
   /** The SMS STATUS REPORT is the result of a SMS SUBMIT*/
   SMS_REPORT_FROM_SUBMIT,
   /** The SMS STATUS REPORT is the result of an SMS COMMAND 
    * for instance an Enquiry
    */
   SMS_REPORT_FROM_COMMAND
}
SmsStatusReportQual;

/****************************************************************************
 * Signal types
 ****************************************************************************/
/** Reports the reception of a Mobile Terminated Short Message (SM) from a 
 * Service Centre (SC). The response to this signal is a TsDeliverReportReq.
 */
typedef struct TsDeliverIndTag
{
   /** Parameter describing the message type */
   Int8                             shortMsgId;
   /** Parameter indicating if the SME has requested a status report*/
   Boolean                          statusReportInd;
   /** Address of the SME */
   SmsAddress                       smeAddr;
   /** SMS service centre address */
   SmsAddress                       scAddr;
   /** Parameter indicating that Reply Path exists*/
   Boolean                          replyPath;
   /** Parameter identifying the above layer protocol, if any */
   SmsProtocolId                    smsProtocolId;
   /**  SMS data coding scheme */
   SmsDataCoding                    smsDataCodingScheme;
   /** Parameter indicating whether or not there are more messages to send */
   Boolean                          moreMsgsToSend;
   /** Parameter identifying time when the Service Centre (SC) received the message */
   SmsTimeStamp                     scTimeStamp;
   /** Indicates the total number of valid data bytes in the shortMsgData 
    * array (including any header data if present)
    
\assoc ARRAY \ref shortMsgData */
   Int8                             shortMsgLen;
   /** The first byte of the shortMsgData array is the User Data Header 
    * Length (UDHL), which indicates the number of bytes for the User Data Header 
    * (UDH) not including the length byte
    */
   Int8                             shortMsgData[SMS_MAX_MSG_LEN];
   /** Indicates if user data header is present */
   Boolean                          userDataHeaderPresent;
   /** It indicates whether the incoming SMS message was ciphered */
   Boolean                          smsCipher;  /* cipher status of SM */
   /* -----------------2/2/01 10:40AM-------------------*/
   /**  Need to retain knowledge of the Raw
    * value of the DCS byte as the test scripts expect
    * us to send it to the SIM Simulator exactly as
    * received in the TsDeliverInd; Networks do not
    * care as they only process the lower nibble of
    * the DCS containing the Class and Coding information
    */
   Int8                             rawDcsValue;
}
TsDeliverInd;

/** Reports the success or failure of a previous TsDeliverInd or TsStatusReportInd*/
typedef struct TsDeliverReportReqTag
{
   /** Identifies which previously received SM the deliver report is for */
   Int8                             shortMsgId;
   /** Indicates whether the SM was received correctly, if this field 
    * is equal to TRANSFER_OK
    */
   SmsStatusOfReport                statusOfReport;
   /** Indicating the reason for SMS DELIVER failure (field will be ignored) */
   TpFailureCause                   tpFailureCause;
   /** Indicates if smsProtocolId is present 
\assoc PRESENT \ref smsProtocolId */
   Boolean                          smsProtocolIdPresent;
   /** Identifies the above layer protocol, if any */
   SmsProtocolId                    smsProtocolId;
   /** Indicates if SMS data coding is present 
\assoc PRESENT \ref smsDataCodingScheme */
   Boolean                          smsDataCodingSchemePresent;
   /** Data coding */
   SmsDataCoding                    smsDataCodingScheme;
   /** Network requires PID and DCS fields to be the same as in the
    * original TsDeliverInd, as per GSM 11.14, section 7.1.1
    */
   Int8                             rawDcsValue;
   /** Indicates the total number of valid data bytes 
\assoc ARRAY \ref userData */
   Int8                             userDataLength;
   /** May comprise just the short message itself or a Header in addition to the short */ 
   Int8                             userData[SMS_DELIVER_REPORT_MAX_USER_DATA_LENGTH];
   /** Parameter indicating that the TP UD field contains a Header */
   Boolean                          userDataHeaderPresent;
   /** whether the UDL + UD parameter is present*/
   Boolean                          udlPresent;
}
TsDeliverReportReq;

/** Sends a Mobile Originated short message to a service centre*/
typedef struct TsSubmitReqTag
{
   /** Parameter identifying the SMS */
   Int8                             shortMsgId;
   /** msgRef to identify SM to Service Centre (SC) */
   Int8                             msgRef;
   /** Parameter indicating if the MS is requesting a status report*/
   Boolean                          statusReportReq;
   /** Address of the SME */
   SmsAddress                       smeAddr;
   /** SMS service centre address */
   SmsAddress                       scAddr;
   /** Parameter indicating the request for Reply Path */
   Boolean                          replyPath;
   /** Identifies the above layer protocol, if any  */
   SmsProtocolId                    smsProtocolId;
   /** Indicates the scheme used for data coding */
   SmsDataCoding                    smsDataCodingScheme;
   /** This will be used by STK application only*/
   Boolean                          useRawDcs;  
   /** This will be used by STK application only*/
   Int8                             rawDcs;     
   /** Parameter indicating whether or not the TP VP field is present*/
   VpFormat                         validityPeriodFormat;
   /** Holds one of these: relative format, enhanced format or absolute format */
   Int8                             validityPeriodAsValue;
   /** Parameter identifying the time from where the message is no longer valid*/
   SmsTimeStamp                     validityPeriodAsTime;
   /** The length of short message 
\assoc ARRAY \ref shortMsgData */
   Int8                             shortMsgLen;
   /** Holds the SMS message */
   Int8                             shortMsgData[SMS_MAX_MSG_LEN];
   /** Parameter indicating whether or not the Service Centre (SC) shall accept an SMS 
    * SUBMIT for an SM still held in the SC which has the same TP MR and 
    * the same TP DA as a previously submitted SM from the same OA
    */
   Boolean                          rejectDuplicates;
   /** Parameter indicating that the TP UD field contains a Header */
   Boolean                          userDataHeaderPresent;
   /** Indicates to SM-TL which route should be used for the SMS message
    * (GSM or GPRS)
    */
   SmsRoute                         smsRoute;  
}
TsSubmitReq;

/** Reports the success or failure of a previous TsSubmitReq, TsCommandReq or TsMemAvailReq*/
typedef struct TsReportIndTag
{
   /** Identifies which previously sent SM the report is for */
   Int8                             shortMsgId;
   /** Indicates the result of the request */
   SmsStatusOfReport                statusOfReport;
   /** Contains data indicating what type of error occurred 
    * in the relay layer of the short message service 
    */
   RpCauseElement                   rpCause;
   /** Indicates the reason for failure */
   TpFailureCause                   tpFailureCause;
   /** Contains TPDU */
   RpUserData                       rpUserData;  /* FR 15954 */
   /** Indicates whether the incoming SMS message was ciphered*/
   Boolean                          smsCipher;  /* cipher status of SM */
   /* add for mantis 144469,Indicates whether the incoming SMS was success or failure */
   Boolean                          mtSmsFailure;
}
TsReportInd;

/** Report from the service centre on the status of a short message requested in a TsCommandReq*/
typedef struct TsStatusReportIndTag
{
   /** is a short message identifier generated by the SM-RL layer 
    * of the protocol stack and refers to this short message and 
    * not the one the report is about 
    */
   Int8                             shortMsgId;
   /** Message number of SM reported on by Service Centre (SC) */
   Int8                             msgRef;
   /** Indicates if more messages available to send */
   Boolean                          moreMsgsToSend;
   /** Address of the Service Centre (SC) which holds the short message that the TsCommandReq references */
   SmsAddress                       scAddr;
   /** Address of the recipient of the previously submitted mobile originated short message */
   SmsAddress                       recipientAddr;
   /** Identifies time when the SC received the previously sent SMS SUBMIT */
   SmsTimeStamp                     scTimeStamp;
   /** Discharge time */
   SmsTimeStamp                     receptionTime; 
   /** The status of the previously sent mobile originated short message */
   SmsStatus                        smsStatus;
   /** Indicates whether the Status Report was due to a TsCommandReq or a TsSubmitReq */
   SmsStatusReportQual              statusReportQual;
   /** Indicates whether the incoming SMS report was ciphered */
   Boolean                          smsCipher;  /* cipher status of SM */
   /** Identifies the above layer protocol, if any  */
   SmsProtocolId                    smsProtocolId;
   /** Indicates the scheme used for data coding */
   SmsDataCoding                    smsDataCodingScheme;
   /** The length of SM SR message */
   Int8                             shortMsgLen;
   /** Holds the SM SR message */
   Int8                             shortMsgData[SMS_MAX_MSG_LEN];
   /** Parameter indicating that the TP UD field contains a Header */
   Boolean                          userDataHeaderPresent;
   /** Holds the TP-PI parameter */
   Int8                             piData[SMS_MAX_SMSR_DATA_LEN];
   /** number of bytes used by TP-PI parameter*/
   Int8                             piLen;
   /** whether the protocol ID parameter is present*/
   Boolean                          pidPresent ;
   /** whether the DCS parameter is present*/
   Boolean                          dcsPresent;
   /** whether the UDL + UD parameter is present*/
   Boolean                          udlPresent;
}
TsStatusReportInd;

/** Sends a command to a short message service centre.
 * This signal request to send an SMS command to the network, for example 
 * to request the status of a previously sent SMS message
 */
typedef struct TsCommandReqTag
{
   /** Is a short message identifier that must be uniquely 
    * created by the application and refers to this short 
    * message and not the one being referenced by the command
    */
   Int8                             shortMsgId;
   /** Message number to identify SM to Service Centre (SC) */
   Int8                             msgRef;
   /** Address of the Service Centre (SC) */
   SmsAddress                       scAddr;
   /** Identifies the above layer protocol, if any */
   SmsProtocolId                    smsProtocolId;
   /** Specifies which operation is to be performed on a SM */
   SmsCommand                       commandType;
   /** Message number in SC to perform command on */
   Int8                             msgNum;
   /** The length of the TP CD field in octets 
\assoc ARRAY \ref cmdData */
   Int8                             cmdDataLen;
   /** Parameter containing user data */
   Int8                             cmdData[SMS_MAX_CMD_LEN];
   /** is TRUE then a status report is requested for this TsCommandReq */
   Boolean                          statusReportReq;
   /** Indicates the destination address to which the command refers 
    * if the command is on a specific short message
    */
   SmsAddress                       smeAddr;
   /** Indicates to SM-TL which route should be used for the SMS message
    * (GSM or GPRS)
    */
   SmsRoute                         smsRoute;  
}
TsCommandReq;

/** Informs the SMS entity (service centre) that there is now storage 
 * capacity in the mobile for at least one more mobile terminated short
 *  message. Depending upon the implementation, this could mean that 
 * there is no room in the SIM (strict GSM interpretation) or that 
 * there is no general availability of memory where the mobile may 
 * store data in different forms of memory
 */
typedef struct TsMemAvailReqTag
{
   /** Is a short message identifier that must be uniquely created by the application*/
   Int8                             shortMsgId;
   /** Message number to identify SMS */
   Int8                             msgRef;
   /** Indicates to SM-TL which route should be used for the SMS message 
    * (GSM or GPRS)
    */
   SmsRoute                         smsRoute; 
}
TsMemAvailReq;

/** Provides the raw TPDU data for the TsDeliverInd preceding this signal. 
 * There is no response for this signal, the data is provided to allow the 
 * Application to write the SMS data exactly as received off-air to the SIM 
 * using the AlsiWriteRawSmReq or AlsiAddRawSmReq signals
 */
typedef struct TsRawDeliverIndTag
{
    /** Is a short message identifier generated by the SM-RL layer of the protocol stack */
    Int8                            shortMsgId;
    /** the address of the Service Centre (SC) that routed the SM to the mobile */
    RawSmsAddress                   scAddr;
    /** Contains raw TPDU data */
    RpUserDataElement               tpdu;
    /** Indicates whether the incoming SMS message was ciphered */
    Boolean                         smsCipher;  
}
TsRawDeliverInd;


/*job100559*/
/*job116776 introducing moreMessagesToSend functionality*/
/** This signal indicates that the AL is ready to start receiving messages. 
 * If moreMsgsToSend is TRUE then SM-CM should try to 
 * keep any existing dedicated connection open so the messages can be transmitted on it
 */
typedef struct TsConfigReqTag
{
  /** TRUE implies AL is ready to prcess received SMS's*/
  Boolean                           isReady; 
  /** Indicates if more messages available to send */
  Boolean                           moreMsgsToSend;
}
TsConfigReq;
/** @} */ /* End of SigSmts group */
#endif
/* END OF FILE */
