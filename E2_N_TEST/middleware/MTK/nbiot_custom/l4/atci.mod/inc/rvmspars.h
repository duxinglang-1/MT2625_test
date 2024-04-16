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
 * RVMSPARS.H
 * Interfaces and defintions for SMS Parser.
 **************************************************************************/

#ifndef RVMSPARS_H
#define RVMSPARS_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>

/***************************************************************************
 * Manifest Constants
 **************************************************************************/
#define VG_SMS_MSG_WAITING  (0xAA00)
/***************************************************************************
 * Type Definitions
 **************************************************************************/

typedef Boolean (*VgSMSDecodeFn)(const Int8 *elementData_p, Int8 *elementLen_p);

typedef enum VgSMSTPDUTypeTag
{
  VG_SMS_TPDU_MIN = 0,
  VG_SMS_TPDU_SUBMIT          = VG_SMS_TPDU_MIN,
  VG_SMS_TPDU_DELIVER,
  VG_SMS_TPDU_SUBMIT_REPORT,
  VG_SMS_TPDU_DELIVER_REPORT,
  VG_SMS_TPDU_STATUS_REPORT,
  VG_SMS_TPDU_MAX
} VgSMSTPDUType;

typedef enum VgSMSTPDUElementTag
{
  VG_SMS_TP_SCA,
  VG_SMS_TP_FO,
  VG_SMS_TP_MR,
  VG_SMS_TP_OA,
  VG_SMS_TP_DA,
  VG_SMS_TP_PID,
  VG_SMS_TP_DCS,
  VG_SMS_TP_VP,
  VG_SMS_TP_SCTS,
  VG_SMS_TP_UD,
  VG_SMS_TP_DT,
  VG_SMS_TP_ST,
  VG_SMS_TP_PI,
  VG_SMS_TP_RA,
  VG_SMS_TP_NULL
} VgSMSTPDUElement;

typedef enum VgVpFormatTag
{
  VPF_NOT_PRESENT = 0x00,
  VPF_ENHANCED,
  VPF_RELATIVE,
  VPF_ABSOLUTE
} VgVpFormat;


typedef struct VgSMSTPDUDecodeTag
{
  VgSMSTPDUElement  vgSMSTPDUElement;
  VgSMSDecodeFn     vgHandlerFn;
} VgSMSTPDUDecode;

/***************************************************************************
 *  Macros
 **************************************************************************/

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

void vgMsSMSMsgTimerExpiry(const VgmuxChannelNumber entity);
Boolean vgMsSMSMsgFetch(const  VgmuxChannelNumber entity,
                        Int8   msgId);
void vgMsSMSTr2mTimerExpiry (const VgmuxChannelNumber entity);
void vgMsSMSCmmsTimerExpiry (const VgmuxChannelNumber entity);

Boolean vgMsSMSIsSimResponse(const Int16 commandRef);

void vgMsSMSMsgProcess(const Int8 *efRecord_p,
                       const Int8 recordNumber);

Boolean vgCheckUDHeader(Int8 headerLen, Int8 *headerData_p,
                        Int8 smDataLen, Int8 *smData_p);

Boolean vgGetNextIE(const Int8 headerLen,
                    Int8 *headerData_p,
                    const Int8 smDataLen,
                    Int8 *smData_p,
                    const Int8 ieElement,
                    const Int8 startPos,
                    Int8 *iePosition_p,
                    Int8 *nextScanStart_p);

Boolean vgMsParsePDUForElement(const Int8             *pdu_p,
                               const VgSMSTPDUType    vgSMSTPDUType,
                               const VgSMSTPDUElement vgSMSTPDUElement,
                               Int8                   *pduOffset_p);

#endif  /* RVSSPARS_H */

/* END OF FILE */




