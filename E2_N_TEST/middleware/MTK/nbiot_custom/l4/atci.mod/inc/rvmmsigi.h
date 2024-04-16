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
 * RVMMSIGI.H
 * Header file for rvmmsigi.c
 **************************************************************************/

#ifndef RVMMSIGI_H
#define RVMMSIGI_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>
#include <ki_sigbuf.h>
#include <rvutil.h>
#include <abem_sig.h>

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

void vgSigApexMmReadPlmnSelCnf   (const SignalBuffer *signalBuffer,
                                   const VgmuxChannelNumber entity);
void vgSigApexMmWritePlmnSelCnf  (const SignalBuffer *signalBuffer,
                                   const VgmuxChannelNumber entity);
void vgSigApexMmPlmnListCnf      (const SignalBuffer *signalBuffer,
                                   const VgmuxChannelNumber entity);
void vgSigApexMmAbortPlmnListCnf      (const SignalBuffer *signalBuffer,
                                   const VgmuxChannelNumber entity);
void vgSigApexMmPlmnSelectCnf    (const SignalBuffer *signalBuffer,
                                   const VgmuxChannelNumber entity);
void vgSigApexMmDeregisterCnf    (const SignalBuffer *signalBuffer,
                                   const VgmuxChannelNumber entity);
void vgSigApexAbemPlmnTestCnf    (const SignalBuffer *signalBuffer,
                                   const VgmuxChannelNumber entity);
void vgSigApexMmRssiInd          (const SignalBuffer *signalBuffer);
void vgSigApexMmBandInd          (const SignalBuffer *signalBuffer);
#if defined (ENABLE_AT_ENG_MODE)
void vgSigApexMmCipherInd        (const SignalBuffer *signalBuffer);
#endif
void vgSigApexMmNetworkInfoInd   (const SignalBuffer *signalBuffer,
                                   const VgmuxChannelNumber entity);
void vgSigApexMmNetworkStateInd  (const SignalBuffer *signalBuffer);
void vgSigAclkReadTimeZoneCnf    (const SignalBuffer *signalBuffer,
                                   const VgmuxChannelNumber entity);
void vgSigAclkSetTimeZoneCnf     (const SignalBuffer *signalBuffer,
                                   const VgmuxChannelNumber entity);
void vgApexMmReadBandModeCnf     (const SignalBuffer *signalBuffer,
                                   const VgmuxChannelNumber entity);
void vgApexMmWritePwonOptionsCnf (const SignalBuffer *signalBuffer,
                                  const VgmuxChannelNumber entity);
void vgApexMmReadPwonOptionsCnf  (const SignalBuffer *signalBuffer,
                                  const VgmuxChannelNumber entity);
#if defined(UPGRADE_MTNET)
void vgApexMmSuspendCnf          (const SignalBuffer *signalBuffer,
                                  const VgmuxChannelNumber entity);
void vgApexMmResumeCnf          (const SignalBuffer *signalBuffer,
                                  const VgmuxChannelNumber entity);
#endif

void vgSigApexMmCsconInd (const SignalBuffer *signalBuffer);

void vgSigApexMmOosaStatusInd (const SignalBuffer *signalBuffer);
                        
void vgApexMmLockArfcnCnf(const SignalBuffer * signalBuffer,
                        const VgmuxChannelNumber entity);
void vgApexMmSearchBandListCnf (const SignalBuffer       *signalBuffer,
                                   const VgmuxChannelNumber entity);                        
void vgApexMmUeStatsCnf (const SignalBuffer       *signalBuffer,
                              const VgmuxChannelNumber entity);

void vgApexMmLocCellInfoCnf (const SignalBuffer       *signalBuffer,
                              const VgmuxChannelNumber entity);

void vgApexMmSetEdrxCnf (  const SignalBuffer       *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgApexMmReadEdrxCnf (  const SignalBuffer       *signalBuffer,
                                        const VgmuxChannelNumber entity);

void vgApexWriteIotOptCfgCnf(  const SignalBuffer       *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgApexReadIotOptCfgCnf(  const SignalBuffer       *signalBuffer,
                                        const VgmuxChannelNumber entity);

void vgApexWritePsmConfCnf(  const SignalBuffer       *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgApexReadPsmConfCnf(  const SignalBuffer       *signalBuffer,
                                        const VgmuxChannelNumber entity);

void vgApexWriteAttachPdnConfCnf(  const SignalBuffer       *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgApexReadAttachPdnConfCnf(  const SignalBuffer       *signalBuffer,
                                        const VgmuxChannelNumber entity);
#if defined (FEA_NFM)
void vgApexReadNfmCnf(  const SignalBuffer       *signalBuffer,
                              const VgmuxChannelNumber entity);
void vgApexWriteNfmCnf(  const SignalBuffer       *signalBuffer,
                               const VgmuxChannelNumber entity);

void vgApexReadNfmConfCnf(  const SignalBuffer       *signalBuffer,
                              const VgmuxChannelNumber entity);
void vgApexWriteNfmConfCnf(  const SignalBuffer       *signalBuffer,
                               const VgmuxChannelNumber entity);
void vgApexReadNfmStatusCnf(  const SignalBuffer       *signalBuffer,
                              const VgmuxChannelNumber entity);
#endif

#if defined (FEA_RPM)
void vgSigApexMmRpmInfoInd(  const SignalBuffer       *signalBuffer);
void vgApexReadRpmCnf(  const SignalBuffer       *signalBuffer,
                              const VgmuxChannelNumber entity);
#endif
void vgApexMmDisableHplmnSearchCnf (const SignalBuffer       *signalBuffer,
                                   const VgmuxChannelNumber entity);



#endif  /* vgMmSIGI_H */

/* END OF FILE */
