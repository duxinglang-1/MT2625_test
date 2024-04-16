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
 *  File Description :
 *
 *  This file contains the function prototypes of the high-level routines
 *  to encode and decode SMS messages.
 *
 *  List of routines:
 *
 *    PackSmsSubmit
 *    PackSmsCommand
 *    BuildTsDeliverInd
 *    BuildTsStatusReport
 **************************************************************************/

#ifndef SMENCDEC_H
#define SMENCDEC_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <ts_sig.h>
#include <alsi_sig.h>
#include <smrl_sig.h>

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/***************************************************************************
 * Typed Constants
 **************************************************************************/

/***************************************************************************
 * Type Definitions
 **************************************************************************/
/*
Set of SMS message types see GSM 03.40 9.2.2. and 9.2.3.1
Note that the ordering of bits in 9.2.3.1 is stated as bit 0 followed by
bit 1. We assume the bit numbers are correct, but the ordering is wrong.
Hence bit0=1, bit1=0 we assume to be bit pattern xxxxxx01 with meaning
SMS-SUBMIT-REPORT in direction SC to MS.
*/
typedef enum SmsMessageToMsTag
{
   SMS_DELIVER,
   SMS_SUBMIT_REPORT,
   SMS_STATUS_REPORT,

   SMS_INVALID_MSG_TYPE = 0xFF         /* decode error */
}
SmsMessageToMs;

typedef enum SmsMessagesToScTag
{
   SMS_DELIVER_REPORT,
   SMS_SUBMIT,
   SMS_COMMAND
}
SmsMessagesToSc;

/***************************************************************************
 *  Macros
 **************************************************************************/

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/
Int8
EncodeSmOctetFormat (Int8 *buffer, Int8 msgLen, const Int8 *msgData);

Int8
EncodeSmDefFormat (Int8 *buffer, Int8 msgLen, const Int8 *msgData, Boolean udhPresent);

void
DecodeSmOctetFormat (Int8 *buffer, Int8 *msgLen, Int8 *msgData, Int8 maxSize);

void
DecodeSmDefFormat (Int8 *buffer, Int8 *msgLen, Int8 *msgData, Boolean udhPresent);

void
UnpackSmsSubmitReport (RpUserDataElement *pUserData,
                       TsReportInd *pTsReportInd);

Int16
PackSmsSubmit (const TsSubmitReq *pTsSubmitReq, Int8 *basePtr, Int16 maxSize);

Int16
PackSmsCommand (const TsCommandReq *pTsCommandReq, Int8 *basePtr, Int16 maxSize);

void
BuildTsDeliverInd (Int8 *basePtr, TsDeliverInd *pTsDeliverInd);

void
BuildTsStatusReport (Int8 *basePtr,
                     TsStatusReportInd *pTsStatusReportInd);

Int16 
PackTsDeliverReportError (TsDeliverReportReq *pTsDeliverReportReq,
                              Int8 *basePtr, Int16 maxSize);

void DecodeRawSmsAddress    (SmsAddress    *smsAddress,
                             RawSmsAddress *rawSmsAddress);

void EncodeRawSmsAddress    (RawSmsAddress *rawSmsAddress,
                             SmsAddress    *smsAddress);

Int16
PackTsDeliverReportAck (TsDeliverReportReq *pTsDeliverReportReq,
                        Int8 *basePtr, Int16 maxSize);

void
BuildTsCommandReq (Int8 *basePtr, TsCommandReq *pTsCommandReq);

void
BuildTsSubmitReq (Int8 *basePtr, TsSubmitReq *pTsSubmitReq, Boolean force8BitDecode, Boolean useRawDcs);

Int16
PackSmsDeliver (const TsDeliverInd *pTsDeliverInd,
                Int8         *basePtr,
                Int16        maxSize,
                Boolean      useRawDcsVal);  /* used in RP-ACK for SMS-PP DD */

Int16 Encode8BitData (Int8 *buffer, Int16 msgLen, const Int8 *msgData, Int16 maxChars);
void EncodeSmsDataCodingScheme (Int8 *rawData, const SmsDataCoding *dataCoding);
void DecodeSmsDataCodingScheme (Int8  rawData, SmsDataCoding *dataCoding);

Int8 SmEncodeFirstOctet(    SimSmTpduType type,
                            const SimSmTpdu *shortMessageTpdu);

Int16 PackSmsStatusReport(  const TsStatusReportInd *tsStatusReportInd,
                            Int8                    *dataPtr,
                            Int16                   maxSize);
#endif

/* END OF FILE */
