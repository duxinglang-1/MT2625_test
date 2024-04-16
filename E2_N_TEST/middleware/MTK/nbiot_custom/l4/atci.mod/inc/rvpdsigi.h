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
 * Header file for rvpdsigi.c.
 * builds.
 **************************************************************************/

#ifndef RVPDSIGI_H
#define RVPDSIGI_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>
#include <ki_sigbuf.h>
#include <rvsystem.h>
#include <rvutil.h>
#include <abpd_sig.h>
#include <abmm_sig.h>
#include <absm_sig.h>

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
#if defined (FEA_MT_PDN_ACT)
Boolean vgPdIncomingPdpContextActivation (const VgmuxChannelNumber entity);
Boolean vgPdMayAnswerWithA (const VgmuxChannelNumber entity);
ResultCode_t vgPdAnswer (const VgmuxChannelNumber entity,
                         Int32 cid,
                         AbpdPdpConnType connectionType);
ResultCode_t vgPdReject (const VgmuxChannelNumber entity);
void vgPdRINGINGTimerExpiry (const VgmuxChannelNumber entity);
#endif /* FEA_MT_PDN_ACT */

/********************************************************
 * New ABPD functions - replace the functions above
 ********************************************************/

#if defined (FEA_QOS_TFT) 
VgEREPModifyReason vgPdGetPsdModifyReason (QualityOfService *originalQos,
                                    QualityOfService *currentQos,
                                    TrafficFlowTemplate *originalTft,
                                    TrafficFlowTemplate *currentTft);
#endif /* FEA_QOS_TFT */

void vgPDNAddrIntToDisplayStr (PdnType pdnType,
                        PdnAddress inAddr,
                        TextualPdnAddress *outAddr,
                        const VgmuxChannelNumber entity);

void vgIpv6PDNAddrIntToStr (PdnAddress inAddr, 
                       TextualPdnAddress *outAddr,
                       Int8 offset,
                       const VgmuxChannelNumber entity);

ResultCode_t
vgColonHexStringToIpv6PDPAddr (const TextualPdnAddress *inAddr, PdnAddress *outAddr,
                               Boolean checkTrailingJunk);

void vgNetworkAPNToCiAPN        (AccessPointName inAPN,
                                 TextualAccessPointName *outAPN);
void vgSetGsmCauseCallReleaseError (const GsmCause errorCode,
                                     const VgmuxChannelNumber entity);

ResultCode_t vgStringToPDNAddr  (PdnType                 pdnType,
                                 const TextualPdnAddress *inAddr,
                                 PdnAddress              *outAddr);

void vgApexSmReadRouteCnf    (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity);
void vgApexSmWriteRouteCnf   (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity);

void vgApexAbpdChannelAllocCnf (const SignalBuffer *signalBuffer,
                                     const VgmuxChannelNumber entity);

void vgApexAbpdPsdAttachCnf (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity);
void vgApexAbpdPsdDetachCnf (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity);


void vgApexAbpdAttachDefBearerActInd (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity);

void vgApexAbpdContextInd (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity);
void vgApexAbpdConnectInd (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity);
void vgApexAbpdErrorInd (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity);
void vgApexAbpdConnectingInd (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity);
void vgApexAbpdConnectedInd (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity);
void vgApexAbpdPsdBearerStatusInd (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity);
void vgApexAbpdDisconnectedInd (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity);
void vgApexAbpdBusyInd (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity);
void vgApexAbpdActivateDataConnCnf (const SignalBuffer *signalBuffer,
                                    const VgmuxChannelNumber entity);

#if defined (FEA_QOS_TFT)
void vgApexAbpdPsdModifyCnf (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity);
void vgApexAbpdPsdModifyRej (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity);

void vgApexAbpdPsdModifyInd (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity);
#endif /* FEA_QOS_TFT */

#if defined (FEA_MT_PDN_ACT)
void vgApexAbpdSetupInd     (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity);
#endif /* FEA_MT_PDN_ACT */

void vgApexAbpdApnReadCnf (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity);
void vgApexAbpdApnWriteCnf (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity);

void vgApexAbpdReportCounterCnf (const SignalBuffer *signalBuffer,
                            const VgmuxChannelNumber entity);

#if defined (FEA_UPDIR)
void vgApexAbpdSetUpdirInfoCnf (const SignalBuffer *signalBuffer,
                            const VgmuxChannelNumber entity);

void vgApexAbpdUpdirInd (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity);
#endif
void vgApexAbpdPppConfigCnf     (const SignalBuffer *signalBuffer,
                                 const VgmuxChannelNumber entity);
void vgApexAbpdPppConfigAuthCnf (const SignalBuffer *signalBuffer,
                                 const VgmuxChannelNumber entity);
void vgApexAbpdPppLoopbackCnf (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity);
void vgPdLoopbackTimerExpiry (const VgmuxChannelNumber entity);
#if defined (FEA_ACL)

void vgApexAbpdSetAclCnf     (const SignalBuffer *signalBuffer,
                              const VgmuxChannelNumber entity);
void vgApexAbpdListAclCnf    (const SignalBuffer *signalBuffer,
                              const VgmuxChannelNumber entity);
void vgApexAbpdWriteAclCnf   (const SignalBuffer *signalBuffer,
                              const VgmuxChannelNumber entity);
void vgApexAbpdDeleteAclCnf  (const SignalBuffer *signalBuffer,
                              const VgmuxChannelNumber entity);
void vgApexAbpdAclStatusCnf (const SignalBuffer *signalBuffer,
                             const VgmuxChannelNumber entity);
#endif
void vgApexAbpdStkInfoInd   (const SignalBuffer *signalBuffer,
                             const VgmuxChannelNumber entity);
void vgApexAbpdWriteRelAssistCnf (const SignalBuffer *signalBuffer,
                             const VgmuxChannelNumber entity);
void vgApexAbpdReadRelAssistCnf (const SignalBuffer *signalBuffer,
                             const VgmuxChannelNumber entity);

void vgApexAbpdReadApnDataTypeCnf (const SignalBuffer *signalBuffer,
                             const VgmuxChannelNumber entity);

void vgApexAbpdWriteApnDataTypeCnf (const SignalBuffer *signalBuffer,
                             const VgmuxChannelNumber entity);

void vgApexAbpdApnUlRateControlInd (const SignalBuffer *signalBuffer,
                             const VgmuxChannelNumber entity);

void vgApexAbpdPlmnUlRateControlInd (const SignalBuffer *signalBuffer,
                             const VgmuxChannelNumber entity);

void vgApexAbpdPacketDiscardInd (const SignalBuffer *signalBuffer,
                             const VgmuxChannelNumber entity);

void vgApexAbpdMtuInd           (const SignalBuffer *signalBuffer,
                             const VgmuxChannelNumber entity);

#if !defined (USE_BMM_ALLOC_MEMORY)
void vgUtMemAboveHwmInd (const SignalBuffer *signalBuffer,
                         const VgmuxChannelNumber entity);
void vgUtMemBelowLwmInd (const SignalBuffer *signalBuffer,
                         const VgmuxChannelNumber entity);
#endif  /* !USE_BMM_ALLOC_MEMORY */                         
#endif  /* RVGPSIGI_H */

/* END OF FILE */

