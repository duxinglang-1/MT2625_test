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
 * RVCRHAND.H
 * Interfaces & definitions for the CR data handler.
 **************************************************************************/

#ifndef RVCRHAND_H
#define RVCRHAND_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>

#include <stdio.h>
#include <stdarg.h>

#include <rvprof.h>

 /***************************************************************************
 * Manifest Constants
 **************************************************************************/

/***************************************************************************
 * Type Definitions
 **************************************************************************/

typedef enum VgDataCodingFormatTag
{
  VG_DATA_CODED_MIN,
  VG_DATA_CODED_GSM = VG_DATA_CODED_MIN,
  VG_DATA_CODED_HEX,
  VG_DATA_CODED_MAX
} VgDataCodingFormat;

/***************************************************************************
 *  Macros
 **************************************************************************/

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

extern int _vsprintf (char *buff, const char *fmt, va_list a);

void vgPutNewLine (const VgmuxChannelNumber entity);

void vgPutc (const VgmuxChannelNumber entity, const Char putChar);

void vgPuts (const VgmuxChannelNumber entity, const Char *putString_p);

void vgPutsWithLength (const VgmuxChannelNumber  entity,
                       const Char                *putString_p,
                       const Int16               length);

void vgPutCrSpecificData (const VgmuxChannelNumber  entity,
                          const VgDataCodingFormat  codingScheme,
                          const Int8                *crData_p,
                          const Int16               crDataLength);

void vgPutCrSpecificDataCscs(   const VgmuxChannelNumber  entity,
                                const VgDataCodingFormat  codingScheme,
                                const Int8                *crData_p,
                                const Int16               crDataLength,
                                const VgCSCSMode          cscsMode);

void vgPutCrSpecificChar (const VgmuxChannelNumber  entity,
                          const VgDataCodingFormat  codingScheme,
                          const Int8                crData);

void vgPutAlphaId (const VgmuxChannelNumber  entity,
                   const Int8                *alphaId_p,
                   const Int16               alphaIdLength);

void vgPutAlphaIdUcs2(  VgmuxChannelNumber  entity,
                        const Int8         *alphaId_p,
                        Int16               alphaIdLength);

void vgPrintMissingParams (const VgmuxChannelNumber  entity,
                           const Int16               numberParams);

void vgPut8BitNum (const VgmuxChannelNumber  entity,
                   const Int8                number);
void vgPut16BitNum (const VgmuxChannelNumber  entity,
                    const Int16               number);
void vgPut32BitNum (const VgmuxChannelNumber  entity,
                    const Int32               number);

void vgPut8BitHex (const VgmuxChannelNumber  entity,
                   const Int8                number);
void vgPut16BitHex (const VgmuxChannelNumber  entity,
                    const Int16               number);
void vgPut32BitHex (const VgmuxChannelNumber  entity,
                    const Int32               number);

void vgPrintf (const VgmuxChannelNumber entity,
               const Char *fmt_p,
               ...);

void vgFlushBuffer (const VgmuxChannelNumber  entity);

#endif  /* RVCRHAND_H */

/* END OF FILE */




