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
 * File Description
 * ----------------
 *
 * vgmsut.h
 * Utility functions for SMS
 **************************************************************************/

#ifndef RVMSUT_H
#define RVMSUT_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>
#include <gkisig.h>
#include <rvsystem.h>

#if !defined (RVDATA_H)
#  include <rvdata.h>
#endif
#if !defined (ABSM_TYP_H)
#  include <absm_typ.h>
#endif

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/***************************************************************************
 * Type Definitions
 **************************************************************************/

/***************************************************************************
 *  Macros
 **************************************************************************/

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/
DialledBcdNum vgSmsUtilConvertSmsAddressToDialledBcdNum (const SmsAddress *smsAddress);

const Char   *vgSmsUtilGetRecordStatusString (Int16 idx);
Int8         vgSmsUtilConvertPduIdxToStatusIdx (Int8 pduIdx);

void         vgSmsOutputListOfRecordStatusStrings (const VgmuxChannelNumber entity);

Boolean      vgSmsUtilConvertPduIdxToSimAccessType (Int32 pduIdx, SmsSimAccessType *simAccessType);
Boolean      vgSmsUtilConvertStatusStringToSimAccessType (Char *statusString, SmsSimAccessType *simAccessType);
Boolean      vgSmsUtilConvertDelFlagToSimAccessType (Int32 delFlag, SmsSimAccessType *simAccessType);
ResultCode_t vgSmsUtilConvertRequestError (const VgmuxChannelNumber entity, SmRequestStatus status);
Boolean      vgSmsUtilConvertPduIdxToSmsStatus (Int32 pduIdx, SimSmRecordStatus *smsStatus);
Boolean      vgSmsUtilConvertStatusStringToSmsStatus (Char *statusString, SimSmRecordStatus *smsStatus);

/* added for job132548 */
ResultCode_t vgSmsUtilConvertNetworkRequestError (GsmCause cause);

Char         vgSmsUtilTypeOfNumberToChar (TypeOfNumber typeOfNumber);
Boolean      vgSmsUtilIsValidMessageChar (const VgmuxChannelNumber entity, Char ch);
Boolean      vgSmsUtilHasSmsStatusSimOverflowChanged (void);
Boolean      vgSmsIsNotBackwardCompatiblePduMode (const VgmuxChannelNumber entity);
Boolean      vgSmsUtilIsInTextMode (const VgmuxChannelNumber entity);
Boolean      vgSmsUtilIsExtraInformationEnabled (void);

#if defined (FEA_PHONEBOOK)
Boolean      vgSmsUtilIsAlphaIdLookupEnabled (void);
#endif /* FEA_PHONEBOOK */

Boolean      vgSmsUtilAreExtraUnsolicitedIndicationsEnabled (const VgmuxChannelNumber entity);

void         vgSmsUtilProcessSmsText (const VgmuxChannelNumber entity);
void         vgSmsUtilInitConcatSmsStruct (const VgmuxChannelNumber entity);
void         vgSmsUtilFormatConcatSmsStruct (const VgmuxChannelNumber entity, CiapSms *structure);

Boolean      vgSmsUtilEncodeTimestamp(  const Char     *stringIn,
                                        Int16           lengthIn,
                                        SmsTimeStamp    out_p);

Boolean      vgSmsUtilEncodeSmsDigits (const Char    *stringIn,
                                       Int16         lengthIn,
                                       SmsAddress    *addrOut_p);

void         vgSmsUtilSendUnsolicitedSmDeliveryIndToCrm (const VgmuxChannelNumber entity,
                                                         const ApexSmDeliveryInd *smDeliveryInd);


void         vgSmsUtilSetUnsolicitedEventHandlerFree (void);
void         vgSmsUtilSetUnsolicitedEventHandlerBusy (void);
Boolean      vgSmsUtilIsUnsolicitedChannelBusy (void);

void         vgSmsPrintListOfRecordStatusStrings (VgmuxChannelNumber entity);

const SmsAddress *vgSmsGetShortMessageSmeAddress (const VgDisplaySmsParam* sms_p);

void vgSmsConvertSmDeliveryInd( const ApexSmDeliveryInd *src, TsDeliverInd *dest );
void vgSmsConvertSmStatusReportInd( const ApexSmStatusReportInd *src, TsStatusReportInd *dest );

Boolean vgSmsUtilFormatData (const VgmuxChannelNumber entity);

SimSmRecordStatus vgSmsUtilConvertVgStatToSimSmStat( VgSmsStatus vgStat);
VgSmsStatus vgSmsUtilConvertSimSmStatToVgStat( SimSmRecordStatus smStat);

/* SMS parameters from the SIM */

void vgSmsUtilSetSca (const SimSmsParameters *smsParams);
void vgSmsUtilGetSca (SmsAddress *sca);

void vgSmsGetValidityPeriod (VpFormat     *validityPeriodFormat,
                             Int8         *validityPeriodAsValue,
                             SmsTimeStamp  validityPeriodAsTime);
void vgSmsSetValidityPeriod (const SimSmsParameters *smsParams);

ResultCode_t vgSmsFormatSmsStructFromPdu(   const VgmuxChannelNumber  entity, 
                                            Int8               *smsMessage,
                                            Int16              *smsLength,
                                            Int16               argLength,
                                            SimSmTpduType      *tpduType, 
                                            SimSmTpdu          *smTpdu, 
                                            Int8                scAddrInfoLength,
                                            Boolean             isStatusReport,
                                            Boolean             checkCompMode);

ResultCode_t vgSmsFormatDeliverReportFromPdu(   const VgmuxChannelNumber  entity, 
                                                Int8               *smsMessage,
                                                Int16              *smsLength,
                                                TsDeliverReportReq *smTpdu);

void vgSmsUtilCreatePduFromText(    VgmuxChannelNumber entity, 
                                    const CiapSms *sms, 
                                    SimSmTpduType *tpduType, 
                                    SimSmTpdu *smsTpdu);

Boolean vgSmsSetDefaultSmStatus(    SimSmTpduType tpduType, 
                                    SimSmRecordStatus *status);
#endif  /* RVMSUT_H */

/* END OF FILE */
