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
 * Header file for rvpdsigo.c.
 **************************************************************************/

#ifndef RVPDSIGO_H
#define RVPDSIGO_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>
#include <ki_sigbuf.h>
#include <rvsystem.h>
#include <rvutil.h>
#include <abpd_sig.h>
#include <absm_sig.h>
#include <gpcntr.h>

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

void vgCiApnToNetworkAPN            (TextualAccessPointName inAPN,
                                     AccessPointName        *outAPN);

void vgApexSmReadRouteReq           (const VgmuxChannelNumber entity);
void vgApexSmWriteRouteReq          (const VgmuxChannelNumber entity);

void vgApexAbpdChannelAllocReq      (const VgmuxChannelNumber entity);
void vgApexAbpdChannelFreeReq       (const VgmuxChannelNumber entity,
                                     const VgmuxChannelNumber entityToFree);

void vgApexAbpdPsdAttachReq         (const VgmuxChannelNumber entity);
void vgApexAbpdPsdDetachReq         (const VgmuxChannelNumber entity);

void vgApexAbpdDialReq              (const VgmuxChannelNumber entity);
void vgApexAbpdActivateDataConnReq  (const VgmuxChannelNumber entity);
void vgApexAbpdConnectRsp           (const VgmuxChannelNumber entity);
void vgApexAbpdConnectRej           (const VgmuxChannelNumber entity);
void vgApexAbpdHangupReq            (const VgmuxChannelNumber entity);
#if defined (FEA_QOS_TFT)
void vgApexAbpdPsdModifyReq         (const VgmuxChannelNumber entity);
#endif

#if defined (FEA_MT_PDN_ACT)
void vgApexAbpdSetupRsp             (const VgmuxChannelNumber entity);
#endif /* FEA_MT_PDN_ACT */

void vgApexAbpdApnReadReq           (const VgmuxChannelNumber entity);
void vgApexAbpdApnWriteReq          (const VgmuxChannelNumber entity);

void vgApexAbpdReportCounterReq     (const VgmuxChannelNumber entity);
void vgApexAbpdResetCounterReq      (const VgmuxChannelNumber entity);

#if defined (FEA_UPDIR)
void vgApexAbpdSetUpdirInfoReq (const VgmuxChannelNumber entity);
void vgApexAbpdResetUpdirInfoReq (const VgmuxChannelNumber entity);
#endif
void vgApexAbpdPppConfigReq         (const VgmuxChannelNumber entity,
                                      const Boolean updateMode,
                                       const Int32 index,
                                        const Int32 finalValue,
                                         const Int32 initValue,
                                          const Int8 tries);

void vgApexAbpdPppConfigAuthReq     (const VgmuxChannelNumber entity,
                                     const Boolean updateMode,
                                     const Int32 value);
void vgApexAbpdPppLoopbackReq       (const VgmuxChannelNumber entity,
                                     const Boolean enable);
#if defined (FEA_ACL)
void vgApexAbpdSetAclReq            (const VgmuxChannelNumber entity);
void vgApexAbpdListAclReq           (const VgmuxChannelNumber entity);
void vgApexAbpdWriteAclReq          (const VgmuxChannelNumber entity);
void vgApexAbpdDeleteAclReq         (const VgmuxChannelNumber entity);
void vgApexAbpdAclStatusReq         (const VgmuxChannelNumber entity);
#endif
void vgApexAbpdWriteRelAssistReq (const VgmuxChannelNumber entity);
void vgApexAbpdReadRelAssistReq (const VgmuxChannelNumber entity);
void vgApexAbpdReadApnDataTypeReq (const VgmuxChannelNumber entity);
void vgApexAbpdWriteApnDataTypeReq (const VgmuxChannelNumber entity);
void vgApexAbpdTransmitDataInd(const VgmuxChannelNumber entity);

# if defined (UPGRADE_GGE)
void vgSigRlcmacDataReq (Int8                **dataMemory,
                          Int16               dataSize,
                           LlcSapi             sapi,
                            PeakThroughput      peakThroughput,
                             RadioPriorityLevel  rpl);

void vgSigRlcmacUnitDataReq (Int8                **dataMemory,
                              Int16               dataSize,
                               LlcSapi             sapi,
                                PeakThroughput      peakThroughput,
                                 RadioPriorityLevel  rpl);
#endif /* UPGRADE_GGE */
#endif  /* RVGPSIGO_H */

/* END OF FILE */

