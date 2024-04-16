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
 * vgmssigo.h
 * SMS output signals.
 **************************************************************************/

#ifndef RVMSSIGO_H
#define RVMSSIGO_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>
#include <rvsystem.h>

#if !defined (ABEM_SIG_H)
#include <abem_sig.h>
#endif

#if defined (FEA_PHONEBOOK)
#if !defined (ABLM_SIG_H)
#include <ablm_sig.h>
#endif
#endif /* FEA_PHONEBOOK */

#if !defined (ABSH_SIG_H)
#include <absh_sig.h>
#endif
#if !defined (RVMSDATA_H)
#include <rvmsdata.h>
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

#if defined (FEA_PHONEBOOK)
ResultCode_t vgSmsSigOutApexLmGetAlphaReq       (const VgmuxChannelNumber entity, Int16 callId, const DialledBcdNum *srcDialledNum);
#endif /* FEA_PHONEBOOK */

ResultCode_t vgSmsSigOutApexDeleteSms           (const VgmuxChannelNumber entity, Int8 idx);
ResultCode_t vgSmsSigOutApexDeleteSmsr           (const VgmuxChannelNumber entity, Int8 idx);

ResultCode_t vgSmsSigOutApexReadSmsNoReadState  (const VgmuxChannelNumber entity, Int8 idx, SmsSimAccessType readType);
ResultCode_t vgSmsSigOutApexReadSmsSetReadState (const VgmuxChannelNumber entity, Int8 idx, SmsSimAccessType readType);
ResultCode_t vgSmsSigOutApexReadSmsNoData       (const VgmuxChannelNumber entity, SmsSimAccessType readType);

ResultCode_t vgSmsSigOutApexReadSmsrNoReadState (const VgmuxChannelNumber entity, Int8 idx, SmsSimAccessType readType);
ResultCode_t vgSmsSigOutApexReadSmsrSetReadState(const VgmuxChannelNumber entity, Int8 idx, SmsSimAccessType readType);
ResultCode_t vgSmsSigOutApexReadSmsrNoData      (const VgmuxChannelNumber entity, SmsSimAccessType readType);

ResultCode_t vgSmsSigOutApexReadSmspReq         (const VgmuxChannelNumber entity, VgSmspState smspState);
ResultCode_t vgSmsSigOutApexWriteSmspReq        (const VgmuxChannelNumber entity, const ApexSmReadSmspCnf *signal_p);
ResultCode_t vgSmsSigOutApexSendSmsNormal       (const VgmuxChannelNumber entity, const CiapSms *sig);
ResultCode_t vgSmsSigOutApexSendSmsTpdu         (const VgmuxChannelNumber entity, const TsSubmitReq *sig);
ResultCode_t vgSmsSigOutApexStoreSmsNormal      (const VgmuxChannelNumber entity, const CiapSms *sig, Int8 recordNumber);
ResultCode_t vgSmsSigOutApexStoreSmsTpdu        (const VgmuxChannelNumber entity, SimSmTpduType tpduType, const SimSmTpdu *sig);
ResultCode_t vgSmsSigOutApexSendCommandSmsTpdu  (const VgmuxChannelNumber entity, const TsCommandReq *sig);
ResultCode_t vgSmsSigOutApexSendCommandSmsNormal(const VgmuxChannelNumber entity, const CiapSms *sms);
ResultCode_t vgSmsSigOutApexSendFromSim         (const VgmuxChannelNumber entity);
ResultCode_t vgSmsSigOutApexSmStatusReq         (const VgmuxChannelNumber entity);
ResultCode_t vgSmsSigOutApexStoreLocReq         (const VgmuxChannelNumber entity, Int16 loc);
ResultCode_t vgSmsSigOutApexSetLocStatusReq     (const VgmuxChannelNumber entity, Int16 loc, SimSmRecordStatus status);
ResultCode_t vgSmsSigOutApexSmDeliveryWithReportRspTpdu(    const VgmuxChannelNumber entity, 
                                                            const TsDeliverReportReq *sig);
  

void         vgSigApexSmDeleteReq               (const VgmuxChannelNumber entity);
void         vgSigApexSmDeleteSmsrReq           (const VgmuxChannelNumber entity);
void         vgSigApexSmReadReq                 (const VgmuxChannelNumber entity);
void         vgSigApexSmReadSmsrReq             (const VgmuxChannelNumber entity);
void         vgSigApexSmReadSmspReq             (const VgmuxChannelNumber entity);
void         vgSigApexSmWriteSmspReq            (const VgmuxChannelNumber entity);
void         vgSigApexSmSendReq                 (const VgmuxChannelNumber entity);
void         vgSigApexSmStoreReq                (const VgmuxChannelNumber entity);
void         vgApexSendSmDeliveryWithReportRsp  (const VgmuxChannelNumber entity);
void         vgSigApexSmCommandReq              (const VgmuxChannelNumber entity);
void         vgSigApexSmSendFromSimReq          (const VgmuxChannelNumber entity);
void         vgSigApexSmSendMoreReq             (const VgmuxChannelNumber entity);
void         vgSigApexSmStatusReq               (const VgmuxChannelNumber entity);
void         vgSigApexSmReadNoDataReq           (const VgmuxChannelNumber entity);
void         vgSigApexSmReadSmsrNoDataReq       (const VgmuxChannelNumber entity);
void         vgSigApexSmStoreLocReq             (const VgmuxChannelNumber entity);
void         vgSigApexSmSetLocStatusReq         (const VgmuxChannelNumber entity);
void         vgSigApexSmStoreRsp                (const VgmuxChannelNumber entity);
void         vgSigApexSmSmsrStoreRsp            (const VgmuxChannelNumber entity);
#endif  /* RVMSSIGO_H */

/* END OF FILE */
