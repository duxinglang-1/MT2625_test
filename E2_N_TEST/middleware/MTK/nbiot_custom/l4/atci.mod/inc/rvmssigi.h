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
 * vgmssigi.h
 * SMS input signal handlers
 **************************************************************************/

#ifndef RVMSSIGI_H
#define RVMSSIGI_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>

#if !defined (RVUTIL_H)
#include <rvutil.h>
#endif
#if !defined (ABEM_SIG_H)
#include <abem_sig.h>
#endif

#if defined (FEA_PHONEBOOK)
#if !defined (ABLM_SIG_H)
#include <ablm_sig.h>
#endif
#endif /* FEA_PHONEBOOK */

#if !defined (ABSM_SIG_H)
#include <absm_sig.h>
#endif
#if !defined (ABSH_SIG_H)
#include <absh_sig.h>
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

/* confirmation signals */
void vgSmsSigInApexSmDeleteCnf       (const VgmuxChannelNumber entity, const ApexSmDeleteCnf      *signal_p);
void vgSmsSigInApexSmDeleteSmsrCnf    (const VgmuxChannelNumber entity, const ApexSmDeleteSmsrCnf  *signal_p);
void vgSmsSigInApexSmReadCnf         (const VgmuxChannelNumber entity, const ApexSmReadCnf        *signal_p);
void vgSmsSigInApexSmReadSmsrCnf      (const VgmuxChannelNumber entity, const ApexSmReadSmsrCnf    *signal_p);
void vgSmsSigInApexSmSendCnf         (const VgmuxChannelNumber entity, const ApexSmSendCnf        *signal_p);
void vgSmsSigInApexSmStoreCnf        (const VgmuxChannelNumber entity, const ApexSmStoreCnf       *signal_p);
void vgSmsSigInApexSmSendFromSimCnf  (const VgmuxChannelNumber entity, const ApexSmSendFromSimCnf *signal_p);
void vgSmsSigInApexSmReadSmspCnf     (const VgmuxChannelNumber entity, const ApexSmReadSmspCnf    *signal_p);
void vgSmsSigInApexSmWriteSmspCnf    (const VgmuxChannelNumber entity, const ApexSmWriteSmspCnf   *signal_p);
void vgSmsSigInApexSmCommandCnf      (const VgmuxChannelNumber entity, const ApexSmCommandCnf     *signal_p);
void vgSmsSigInApexSmStatusCnf       (const VgmuxChannelNumber entity, const ApexSmStatusCnf      *signal_p);
void vgSmsSigInApexSmReadNoDataCnf   (const VgmuxChannelNumber entity, const ApexSmReadCnf        *signal_p);
void vgSmsSigInApexSmReadSmsrNoDataCnf(const VgmuxChannelNumber entity, const ApexSmReadSmsrCnf    *signal_p);
void vgSmsSigInApexSmStoreLocCnf     (const VgmuxChannelNumber entity, const ApexSmStoreLocCnf    *signal_p);
void vgSmsSigInApexSmSetLocStatusCnf (const VgmuxChannelNumber entity, const ApexSmSetLocStatusCnf *signal_p);
void vgSmsSigInApexSmStkInfoInd      (const VgmuxChannelNumber entity, const ApexSmStkInfoInd      *signal_p);

#if defined (FEA_PHONEBOOK)
void vgSmsSigInApexLmGetAlphaCnf     (const VgmuxChannelNumber entity, const ApexLmGetAlphaCnf    *signal_p);
#endif /* FEA_PHONEBOOK */

/* indication signals */
void vgSmsSigInApexSmMsgReceivedInd  (const ApexSmMsgReceivedInd   *signal_p );
void vgSmsSigInApexSmDeliveryInd     (const ApexSmDeliveryInd      *signal_p );
void vgSmsSigInApexSmReadyInd        (const ApexSmReadyInd         *signal_p );
void vgSmsSigInApexSmStatusReportInd (const ApexSmStatusReportInd  *signal_p );
void vgSmsSigInApexSmSmsrStoreInd    (const ApexSmSmsrStoreInd     *signal_p );
void vgSmsSigInApexSmRecordChangedInd(const ApexSmRecordChangedInd *signal_p );
void vgSmsSigInApexSmStatusChangedInd(const ApexSmStatusChangedInd *signal_p );
void vgSmsSigInApexSmSrDeletedInd    (const ApexSmSrDeletedInd     *signal_p );
void vgSmsSigInApexSmStoreInd        (const ApexSmStoreInd         *signal_p );
void vgSmsSigInApexTransactonEndInd  ( const ApexSmTransactionEndInd *signal_p);

#endif  /* RVMSSIGI_H */

/* END OF FILE */
