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
 * External interface function prototypes for the cgstkrd module.
 **************************************************************************/

#ifndef RVSTKRD_H
#define RVSTKRD_H


#include <pssignal.h>
#include <ki_sigbuf.h>

#include <rvstk.h>

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/***************************************************************************
 * Type Definitions
 **************************************************************************/

typedef enum StkDataCodingSchemeTag
{
  STK_DCS_DEFAULT   = 0,
  STK_DCS_EIGHT_BIT = 4,
  STK_DCS_UCS2      = 8
}
StkDataCodingScheme;

typedef enum VgStkModeTag
{
  STK_MODE_PDU,
  STK_MODE_TEXT,
  STK_MODE_CMGF
} VgStkMode;

/***************************************************************************
 *  Macros
 **************************************************************************/

/***************************************************************************
 *  Variables
 **************************************************************************/

/* used by vgstktp.c to return the correct command identifier. */
extern Int32 simCommandId;

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

void vgInitialiseStkData       (void);
Int16 vgStkGetCmdId            (void);
Int32 vgStkGetSimCmd           (void);
SignalId vgStkGetCurrentSignal (void);
Boolean vgStkIsDataValid       (void);
void vgStkSetDataToInValid     (void);
void vgStkSendData             (void);
Boolean vgStkSetUpData         (const SignalBuffer signalEntity,
                                 const StkCommandId commandId,
                                  const VgmuxChannelNumber entity);
void vgStkDestroyBuffer        (void);
Boolean vgStkMenuItemPresent (Int32 thisItem);

Boolean vgStkOutputInTextMode (const VgmuxChannelNumber entity);
void vgAfsaRunAtCommandInd    (const SignalBuffer signalEntity,
                               const VgmuxChannelNumber entity);


#endif

/* END OF FILE */
