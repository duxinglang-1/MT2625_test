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
 * Header file for vgslsigo.c
 **************************************************************************/

#ifndef RVSLSIGO_H
#define RVSLSIGO_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>

#if !defined (RVUTIL_H)
#  include <rvutil.h>
#endif

/***************************************************************************
 * Manifest Constants
 **************************************************************************/
#define STATUS_BYTES_LENGTH  2
/***************************************************************************
 * Type Definitions
 **************************************************************************/

/***************************************************************************
 *  Macros
 **************************************************************************/

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/
void vgSigApexSimGetPinRsp(const VgmuxChannelNumber entity);

void vgSigApexSimGetChvRsp(const VgmuxChannelNumber entity);

void vgSigApexPmModeChangeReq(const VgmuxChannelNumber entity);

#if defined (ENABLE_DUAL_SIM_SOLUTION)
void vgSigApexSimSelectReq(const VgmuxChannelNumber entity);
#endif

void vgSigApexPmPowerGoingDownRsp (void);

void vgSigApexSimGenAccessReq (const VgmuxChannelNumber entity);

void vgSigApexSimLogicalChannelAccessReq(const VgmuxChannelNumber entity);

void vgSigApexSimUsimAppStartReq(const VgmuxChannelNumber entity);

void vgSigApexSimReadDirReq(const VgmuxChannelNumber entity);

void vgSigApexSimReadMsisdnReq(const VgmuxChannelNumber entity);

void vgSigApexTerminateSessionReq(const VgmuxChannelNumber entity);

void vgSigApexListPnnReq (const VgmuxChannelNumber entity);

void vgSigApexListOplReq (const VgmuxChannelNumber entity);

void vgSigApexReadSimParamReq (const VgmuxChannelNumber entity);

void vgSigApexReadSystemMwiReq(const VgmuxChannelNumber entity);

ResultCode_t vgSendApexSimGenAccessReq (const VgmuxChannelNumber entity,
                                        Int16 length, const Int8 *src_p,
                                        Int16 efId, Int16 dirId,
                                        Int16 rootDirId,
                                        /* job134856: add handling for <pathid> field */
                                        const Int8 *path_p,
                                        Int8 pathLength);
                /* job 101859 changed to Id's to Int16's and  */
                /* added rootDirId */

ResultCode_t vgSendApexSimLogicalChannelAccessReq (const VgmuxChannelNumber entity,
                                        Int16 length,
                                        Int8  sessionId,
                                        const Int8 *src_p,
                                        Int16 efId,
                                        Int16 dirId,
                                        Int16 rootDirId,
                                        const Int8 *path_p,
                                        Int8 pathLength);

#if defined (SIM_EMULATION_ON)
void vgSigAlsiWriteUsimEmuFileReq (const VgmuxChannelNumber entity);
#endif /* SIM_EMULATION_ON */

void vgSigApexSimCsimLockReq      (const VgmuxChannelNumber entity);

void vgSigApexSimOpenLogicalChannelReq      (const VgmuxChannelNumber entity);
void vgSigApexSimCloseLogicalChannelReq      (const VgmuxChannelNumber entity);

#endif  /* RVSLSIGO_H */

/* END OF FILE */

