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
 * Header file for vggnsigi.c
 **************************************************************************/

#ifndef RVGNSIGI_H
#define RVGNSIGI_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>
#include <ki_sigbuf.h>
#include <rvsystem.h>
#include <rvutil.h>

#if defined (FEA_PHONEBOOK)
#include <ablm_sig.h>
#endif /* FEA_PHONEBOOK */

#include <absh_sig.h>

#if defined (UPGRADE_VCXO)
#include <l1vcxocfg.h>
#endif
#if defined (COARSE_TIMER)
# if defined (UPGRADE_SHMCL_SOLUTION)
#include <shm_nvramtyp.h>
# else
#include <shmconn_typ.h>
# endif
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

VgDialNumberType vgBcdNumberTypeToChar (const BcdNumberType typeOfNumber);
void vgSigApexSimReadSpnCnf            (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexSimChvFunctionCnf        (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);

#if defined (FEA_SIMLOCK)
void vgSigApexSimMepCnf                (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber  entity);
void vgSigApexSimMepStatusCnf          (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber  entity);
#if defined(FEA_NOT_SLIM_SIM_CODE)
void vgSigApexSimWriteMepNetworkIdCnf  (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber  entity);
void vgSigApexSimReadMepNetworkIdCnf   (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber  entity);
#endif
#endif /* FEA_SIMLOCK */


void vgSigAclkDateAndTimeInd           (const SignalBuffer *signalBuffer);
void vgSigApexGlWriteFeatureConfigCnf  (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexSimPinFunctionCnf        (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
#if defined (FEA_PHONEBOOK)
void vgSigApexLmDialNumStatusCnf       (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexLmHiddenKeyFunctionCnf   (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexLmFindDialNumCnf         (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexLmReadDialNumCnf         (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexLmWriteDialNumCnf        (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexLmDeleteDialNumCnf       (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexLmFixedDialCnf           (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexLmBarredDialCnf          (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexLmGetAlphaCnf            (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexLmReadGrpCnf             (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexLmReadGasCnf             (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexLmListGasCnf             (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexLmWriteGasCnf            (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexLmWriteGrpCnf            (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexLmWriteAnrCnf            (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexLmReadAnrCnf             (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexLmWriteSneCnf            (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexLmReadSneCnf             (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexLmWriteEmailCnf          (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexLmReadEmailCnf           (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexLmDeleteGrpCnf           (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexLmDeleteAnrCnf           (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexLmDeleteSneCnf           (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexLmDeleteEmailCnf         (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexLmDeleteGasCnf           (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexLmClearGasCnf            (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexLmPhoneBookStatusCnf     (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexLmGetSyncStatusCnf       (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexLmReadRecordUidCnf       (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexLmReadyIndLocal          (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexLmRecordChangedInd       (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexLmReadAasCnf             (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexLmListAasCnf             (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexLmWriteAasCnf            (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexLmDeleteAasCnf           (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexLmClearAasCnf            (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
#endif /* FEA_PHONEBOOK */

void vgSigN1CdEnterCnf                 (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigN1CdExitCnf                  (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigN1CdRfTestCnf                (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigN1CdIdcTestCnf               (const SignalBuffer *signalBuffer,
                                        const VgmuxChannelNumber entity);
void vgSigApexMmWriteMobileIdCnf( const SignalBuffer *signalBuffer,
                                    const VgmuxChannelNumber entity);

void vgSigApexMmReadMobileIdCnf( const SignalBuffer *signalBuffer,
                                    const VgmuxChannelNumber entity);
void vgSendNextMnvmgetDataItemToMux   (const VgmuxChannelNumber entity);

#if defined (COARSE_TIMER)
void vgSigConModemStatusInd (ModemCause cause);
#endif
#endif /* RVGNSIGI_H */

/* END OF FILE */
