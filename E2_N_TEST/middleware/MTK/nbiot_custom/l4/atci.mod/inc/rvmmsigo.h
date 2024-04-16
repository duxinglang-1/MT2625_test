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
 * vgMmSIGO.H
 * Header file for vggnsigo.c
 **************************************************************************/

#ifndef RVMMSIGO_H
#define RVMMSIGO_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>
#include <rvutil.h>
#include <abmm_sig.h>
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

void vgSigApexMmReadPlmnSelReq  (Int16 startField,
                                  Int8 numEntriesDesired,
                                   AbmmPlmnSelector plmnSelector,
                                   const VgmuxChannelNumber entity);
void vgSigApexMmWritePlmnSelReq (const VgmuxChannelNumber entity);
void vgSigApexMmPlmnListReq     (const VgmuxChannelNumber entity);
void vgSigApexMmAbortPlmnListReq     (const VgmuxChannelNumber entity);
void vgSigApexMmPlmnSelectReq   (const VgmuxChannelNumber entity);
void vgSigApexMmDeregisterReq   (const VgmuxChannelNumber entity);
void vgSigApexEmPlmnTestReq     (const VgmuxChannelNumber entity);
void vgSigAclkReadTimeZoneReq   (const VgmuxChannelNumber entity);
void vgSigAclkSetTimeZoneReq    (const VgmuxChannelNumber entity);
void vgApexMmReadBandModeReq    (const VgmuxChannelNumber entity);
void vgApexMmWritePwonOptionsReq(const VgmuxChannelNumber entity);
void vgApexMmReadPwonOptionsReq (const VgmuxChannelNumber entity);
#if defined(UPGRADE_MTNET)
void vgApexMmSuspendReq         (const VgmuxChannelNumber entity);
void  vgApexMmResumeReq         (const VgmuxChannelNumber entity);
#endif

void vgApexMmSetEdrxReq(const VgmuxChannelNumber entity);
void vgApexMmReadEdrxReq(const VgmuxChannelNumber entity);
void vgApexWriteIotOptCfgReq(const VgmuxChannelNumber entity);
void vgApexReadIotOptCfgReq(const VgmuxChannelNumber entity);
void vgApexWritePsmConfReq(const VgmuxChannelNumber entity);
void vgApexReadPsmConfReq(const VgmuxChannelNumber entity);
void vgApexWriteAttachPdnConfReq(const VgmuxChannelNumber entity);
void vgApexReadAttachPdnConfReq(const VgmuxChannelNumber entity);
#if defined (FEA_NFM)
void vgApexReadNfmReq(const VgmuxChannelNumber entity);
void vgApexWriteNfmReq(const VgmuxChannelNumber entity);
void vgApexReadNfmConfReq(const VgmuxChannelNumber entity);
void vgApexWriteNfmConfReq(const VgmuxChannelNumber entity);
void vgApexReadNfmStatusReq(const VgmuxChannelNumber entity);
#endif
#if defined (FEA_RPM)
void vgApexReadRpmReq(const VgmuxChannelNumber entity);
#endif

void vgApexMmLockArfcnReq(const VgmuxChannelNumber entity);
void vgApexMmUeStatsReq (const VgmuxChannelNumber entity);
void vgApexMmLocCellInfoReq (const VgmuxChannelNumber entity);
void vgApexMmSearchBandListReq (const VgmuxChannelNumber entity);
void vgApexMmSetEhplmnReq (const VgmuxChannelNumber entity);
void vgApexMmDisableHplmnSearchReq(const VgmuxChannelNumber entity);

#endif  /* vgMmSIGO_H */

/* END OF FILE */

