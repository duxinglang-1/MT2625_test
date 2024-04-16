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
 * Header file for vgslsigi.c
 **************************************************************************/

#ifndef RVSLSIGI_H
#define RVSLSIGI_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#if !defined (SYSTEM_H)
#  include <system.h>
#endif
#if !defined (KI_SIGBUF_H)
#include <ki_sigbuf.h>
#endif
#if !defined (RVUTIL_H)
#  include <rvutil.h>
#endif

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/***************************************************************************
 * Type Definitions
 **************************************************************************/
typedef struct responseT
{
    Int8  flag;
    Int16 sw;
    Int32 length;
    Int8  res_data[256];
}response;

/***************************************************************************
 *  Macros
 **************************************************************************/

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/
void vgSigApexSimAppStartedInd (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity);

void vgSigApexSimAppStoppedInd (const SignalBuffer *signalBuffer,
                                const VgmuxChannelNumber entity);

void vgSigApexSimGetChvInd    (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity);

void vgSigApexSimGetPinInd    (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity);

void vgSigApexSimChvFunctionInd (const SignalBuffer *signalBuffer,
                                  const VgmuxChannelNumber entity);

void vgSigApexPmModeChangeCnf (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity);

void vgSigApexSimOkInd        (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity);

void vgSigApexSimNokInd       (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity);

void vgSigApexSimFaultInd     (const SignalBuffer *signalBuffer);

void vgSigApexSimGenAccessCnf (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity);

void vgSigApexSimLogicalChannelAccessCnf (const SignalBuffer *signalBuffer,
                                          const VgmuxChannelNumber entity);

void vgSigApexSimRawApduAccessCnf (const SignalBuffer *signalBuffer,
                                   const VgmuxChannelNumber entity);

void vgSigApexSimReadSimParamCnf (const SignalBuffer *signalBuffer,
                                  const VgmuxChannelNumber entity);

void vgSigApexEmuSimCnf (const SignalBuffer *signalBuffer,
                           const VgmuxChannelNumber entity);

void vgSigApexListPnnCnf (const SignalBuffer *signalBuffer,
                          const VgmuxChannelNumber entity);

void vgSigApexListOplCnf (const SignalBuffer *signalBuffer,
                          const VgmuxChannelNumber entity);


#if defined (ENABLE_DUAL_SIM_SOLUTION)
void vgSigApexSimSelectCnf    (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity);

void vgSigApexSimHolderInfoCnf(const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity);
#endif /* ENABLE_DUAL_SIM_SOLUTION */

void  vgSigApexSimCsimLockCnf (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity);

void  vgSigApexSimCsimLockInd (const SignalBuffer *signalBuffer,
                               const VgmuxChannelNumber entity);

void vgSigApexSimReadDirCnf    (const SignalBuffer *signalBuffer,
                                  const VgmuxChannelNumber entity);

void vgSigApexSimReadMsisdnCnf (    const SignalBuffer *signalBuffer,
                                const VgmuxChannelNumber entity);

void vgSigApexSimUsimAppStartCnf(   const SignalBuffer *signalBuffer,
                                    const VgmuxChannelNumber entity);

Char *vgConvFromBcdToText (BcdNumberType numType,
                        const Bcd *bcdData_p,
                         Int8 bcdDataLen,
                          Char *textDialNum_p);

#if defined (SIM_EMULATION_ON)
void vgSigAlsiWriteUsimEmuFileCnf(  const SignalBuffer *signalBuffer,
                                    const VgmuxChannelNumber entity);
#endif /* SIM_EMULATION_ON */

void vgSigApexSimOpenLogicalChannelCnf (   const SignalBuffer *signalBuffer,
                                    const VgmuxChannelNumber entity);

void vgSigApexSimCloseLogicalChannelCnf (   const SignalBuffer *signalBuffer,
                                    const VgmuxChannelNumber entity);

#endif  /* RVSLSIGI_H */

/* END OF FILE */
