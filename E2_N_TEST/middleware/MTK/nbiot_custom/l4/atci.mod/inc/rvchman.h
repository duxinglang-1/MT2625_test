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
 * RVCHMAN.H
 * Change control manager interface controller exported functions
 **************************************************************************/

#ifndef RVCHMAN_H
#define RVCHMAN_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>
#include <ki_sigbuf.h>
#include <rvutil.h>

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

ResultCode_t vgChManContinueActionFlowControl(  const VgmuxChannelNumber entity,
                                                const SignalId signalId);
ResultCode_t vgChManContinueAction  (const VgmuxChannelNumber entity,
                                      const SignalId signalId);
Boolean vgChManFindElement          (const SignalId signalId,
                                      VgChangeControlElements *element);
Boolean vgChManCheckHaveControl     (const VgChangeControlElements element,
                                     const VgmuxChannelNumber entity);
Boolean vgChManGetControl           (const VgChangeControlElements element,
                                      const VgmuxChannelNumber entity);
void vgChManReleaseControl          (const VgmuxChannelNumber entity);
void vgChManReleaseControlOfElement (const VgChangeControlElements element,
                                      const VgmuxChannelNumber entity);
Boolean vgChManRegister             (const VgChangeControlElements element,
                                      const VgmuxChannelNumber entity);
void vgChManSetPendingCompletion    (const VgChangeControlElements element,
                                      const Boolean pending,
                                       const VgmuxChannelNumber entity);
ResultCode_t vgChManPostAction       (const SignalId signalId,
                                      const VgmuxChannelNumber entity,
                                       const Boolean successful);
Boolean vgChManCheckSignalDirection (const SignalId signalId,
                                      VgmuxChannelNumber *entity);

/* added for job108826  */
void vgChManRestoreVgControlEntity  (const VgChangeControlElements element);

void vgChManProcessPendingSignal    (const VgChangeControlElements element);
void vgChManInitialRegistration     (const VgmuxChannelNumber entity);
Boolean vgChManInterfaceController  (const SignalBuffer *signal_p,
                                      const VgmuxChannelNumber entity);


#endif  /* RVCHMAN_H */

/* END OF FILE */
