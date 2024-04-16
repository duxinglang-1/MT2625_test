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
 * Header file for vggnsigo.c
 **************************************************************************/

#ifndef RVGNSIGO_H
#define RVGNSIGO_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>
#include <kitqid.h>
#include <rvutil.h>
#include <rvsystem.h>
#include <abem_sig.h>

#if defined (FEA_PHONEBOOK)
#include <ablm_sig.h>
#endif /* FEA_PHONEBOOK */

#include <absh_sig.h>
#include <rvgndata.h>
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

void vgSigApexShChangeControlReq  (TaskId taskCtrl,
                                    ProcId proc,
                                     Boolean isImmediate,
                                      const VgmuxChannelNumber entity);
void vgSigApexLmDialNumStatusReq  (const VgmuxChannelNumber entity);
void vgSigApexLmFindDialNumReq    (const VgmuxChannelNumber entity);
void vgSigApexLmReadDialNumReq    (const VgmuxChannelNumber entity);
void vgSigApexLmWriteDialNumReq   (const VgmuxChannelNumber entity);
void vgSigApexLmDeleteDialNumReq  (const VgmuxChannelNumber entity);
void vgSigApexSimReadSpnReq       (const VgmuxChannelNumber entity);
void vgSigApexSimPinFunctionReq   (const VgmuxChannelNumber entity);
void vgSigApexSimChvFunctionReq   (const VgmuxChannelNumber entity);



void vgSigApexMmWriteMobileIdReq(const VgmuxChannelNumber entity);

void vgSigApexMmReadMobileIdReq(const VgmuxChannelNumber entity);



#if defined (FEA_SIMLOCK)
void vgSigApexSimMepReq           (const VgmuxChannelNumber entity);
void vgSigApexSimMepStatusReq     (const VgmuxChannelNumber entity);
#endif /* FEA_SIMLOCK */

void vgSigApexEmPlmnTestReq       (const VgmuxChannelNumber entity);


void vgSigApexLmFixedDialReq      (const VgmuxChannelNumber entity);
void vgSigApexLmGetAlphaReq       (const VgmuxChannelNumber entity);
void vgSigApexLmPhoneBookStatusReq   (const VgmuxChannelNumber entity);
void vgSigApexLmHiddenKeyFunctionReq (const VgmuxChannelNumber entity);
void vgSigApexLmReadGrpReq           (const VgmuxChannelNumber entity);
void vgSigApexLmReadGasReq           (const VgmuxChannelNumber entity);
void vgSigApexLmDeleteGasReq         (const VgmuxChannelNumber entity);
void vgSigApexLmClearGasReq          (const VgmuxChannelNumber entity);
void vgSigApexLmListGasReq           (const VgmuxChannelNumber entity);
void vgSigApexLmWriteGasReq          (const VgmuxChannelNumber entity);
void vgSigApexLmWriteGrpReq          (const VgmuxChannelNumber entity);
void vgSigApexLmReadAnrReq           (const VgmuxChannelNumber entity);
void vgSigApexLmWriteAnrReq          (const VgmuxChannelNumber entity);
void vgSigApexLmReadSneReq           (const VgmuxChannelNumber entity);
void vgSigApexLmWriteSneReq          (const VgmuxChannelNumber entity);
void vgSigApexLmReadEmailReq         (const VgmuxChannelNumber entity);
void vgSigApexLmWriteEmailReq        (const VgmuxChannelNumber entity);
void vgSigApexLmDeleteGrpReq         (const VgmuxChannelNumber entity);
void vgSigApexLmDeleteAnrReq         (const VgmuxChannelNumber entity);
void vgSigApexLmDeleteSneReq         (const VgmuxChannelNumber entity);
void vgSigApexLmDeleteEmailReq       (const VgmuxChannelNumber entity);
void vgSigApexLmGetPbSyncInfoReq     (const VgmuxChannelNumber entity);
void vgSigApexLmGetSyncStatusReq     (const VgmuxChannelNumber entity);
void vgSigApexLmReadRecordUidReq     (const VgmuxChannelNumber entity);
void vgSigApexLmReadAasReq           (const VgmuxChannelNumber entity);
void vgSigApexLmWriteAasReq          (const VgmuxChannelNumber entity);
void vgSigApexLmDeleteAasReq         (const VgmuxChannelNumber entity);
void vgSigApexLmListAasReq           (const VgmuxChannelNumber entity);
void vgSigApexLmClearAasReq          (const VgmuxChannelNumber entity);
void vgSigApexLmBarredDialReq        (const VgmuxChannelNumber entity);

void vgSigApexEmuSimCfgReq   (const VgmuxChannelNumber entity);

void vgSigApexGlWriteFeatureConfigReq(const VgmuxChannelNumber entity);

void vgSigN1CdEnterReq  (const VgmuxChannelNumber entity);
void vgSigN1CdExitReq   (const VgmuxChannelNumber entity);
void vgSigN1CdRfTestReq (const VgmuxChannelNumber entity);
void vgSigN1CdIdcTestReq(const VgmuxChannelNumber entity);
void vgSigN1TstAlgTuningSetReq (const VgmuxChannelNumber entity);
#endif  /* RVGNSIGO_H */

/* END OF FILE */
