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
 * vgmsprnt.c
 * Print functions for SMS subsystem
 **************************************************************************/


/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>

#ifndef RVMSPRNT_H
#define RVMSPRNT_H

#include <gkisig.h>

#if !defined (RVDATA_H)
#  include <rvdata.h>
#endif
#if !defined (RVUTIL_H)
#  include <rvutil.h>
#endif
#if !defined (RVMSDATA_H)
#  include <rvmsdata.h>
#endif

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/


/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

/***************************************************************************
 * Type Definitions
 ***************************************************************************/


/***************************************************************************
 * Variables
 ***************************************************************************/


/***************************************************************************
 * Local Functions
 ***************************************************************************/








/***************************************************************************
 * Global Functions
 ***************************************************************************/

void vgSmsPrintf                                   ( const VgmuxChannelNumber entity, Char* fmt, ... );


void vgSmsPrintAlphaIdTextMode                     ( const VgmuxChannelNumber entity, const SimAlphaIdentifier *alphaId_p );
void vgSmsPrintAlphaIdPduMode                      ( const VgmuxChannelNumber entity, const SimAlphaIdentifier *alphaId_p );

void vgSmsPrintSca                                 ( const VgmuxChannelNumber  entity );
void vgSmsPrintValidityPeriod                      ( const VgmuxChannelNumber  entity );

void vgSmsPrintSmsTimestamp                        ( const VgmuxChannelNumber entity, const Int8 *data );

void vgSmsPrintSmsAddressTextMode                  ( const VgmuxChannelNumber entity, const SmsAddress *scAddr );
void vgSmsPrintSmsAddressPduMode                   ( const VgmuxChannelNumber entity, const SmsAddress *scAddr );

void vgSmsPrintPdu                                 ( const VgmuxChannelNumber entity, const Int8 tpduData[], Int16 tpduLength, const SmsAddress *scAddr );


void vgSmsPrintUnsolicitedSmDeliveryIndWithAlphaId ( const VgmuxChannelNumber entity, const ApexSmDeliveryInd *smDeliveryInd, const VgSmsAlphaId *alphaId );
void vgSmsPrintUnsolicitedSmReadyInd               ( const VgmuxChannelNumber entity, const ApexSmReadyInd *smSimState_p );
void vgSmsPrintUnsolicitedSmStatusReport           ( const VgmuxChannelNumber entity, const ApexSmStatusReportInd *smStatusReport );
void vgSmsPrintUnsolicitedSmMsgReceivedInd         ( const VgmuxChannelNumber entity, AbsmMemAreaType messageLoc, Int16 recordNumber );
void vgSmsPrintUnsolicitedSmStatusReportInd        ( const VgmuxChannelNumber entity, Int16 recordNumber );

void vgSmsFlush                                    ( const VgmuxChannelNumber entity );
void vgSmsPrintNum                                 ( const VgmuxChannelNumber entity, Int16 );
void vgSmsPuts                                     ( const VgmuxChannelNumber entity, const Char *s );
void vgSmsPutc                                     ( const VgmuxChannelNumber entity, Char c );
void vgSmsPutsWithLen                              ( const VgmuxChannelNumber entity, const Char *s, Int16 len );
void vgSmsPrintNewline                             ( const VgmuxChannelNumber entity );
void vgSmsPrintInt16                               ( const VgmuxChannelNumber entity, Int16 val );
void vgSmsPrint8BitHex                             ( const VgmuxChannelNumber entity, Int8 val );

void vgSmsPrintSmsInPduMode                        ( const VgmuxChannelNumber entity, SimSmtpduSubmitDeliverType  submitOrDeliver, const VgDisplaySmsParam *sms_p, const VgSmsAlphaId *alphaId );
void vgSmsPrintSmsInTextMode                       ( const VgmuxChannelNumber entity, SimSmtpduSubmitDeliverType  submitOrDeliver, const VgDisplaySmsParam *sms_p, const VgSmsAlphaId *alphaId );
void vgSmsPrintSmsrInPduMode                       ( const VgmuxChannelNumber entity, const VgDisplaySmsSrParam  *display_p );
void vgSmsPrintSmsrInTextMode                      ( const VgmuxChannelNumber entity, const VgDisplaySmsSrParam  *display_p );

#endif /* RVMSPRNT_H */

/* END OF FILE */

