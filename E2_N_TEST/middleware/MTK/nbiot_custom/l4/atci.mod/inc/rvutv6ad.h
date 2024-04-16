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
 * RVUTV6AD.H
 * Header file for rvutv6ad.c
 **************************************************************************/

#ifndef RVUTV6AD_H
#define RVUTV6AD_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>


#include <rvcrdata.h>

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/***************************************************************************
 * Type Definitions
 **************************************************************************/

typedef enum VgUtv6adConvErrTag
{
  VG_UTV6AD_ERR_OK = RESULT_CODE_OK,
  VG_UTV6AD_ERR_MORE_THAN_ONE_DOUBLE_COLON = VG_CME_UTV6AD_MORE_THAN_ONE_DOUBLE_COLON,
  VG_UTV6AD_ERR_IPV4_ADDRESS_OFF_END = VG_CME_UTV6AD_IPV4_ADDRESS_OFF_END,
  VG_UTV6AD_ERR_DOTTED_DECIMAL_OUTSIDE_IPV4 = VG_CME_UTV6AD_DOTTED_DECIMAL_OUTSIDE_IPV4,
  VG_UTV6AD_ERR_IPV4_ERR_BYTE_VALUE_OVERFLOW = VG_CME_UTV6AD_IPV4_ERR_BYTE_VALUE_OVERFLOW,
  VG_UTV6AD_ERR_IPV4_ERR_EMPTY_BYTE = VG_CME_UTV6AD_IPV4_ERR_EMPTY_BYTE,
  VG_UTV6AD_ERR_IPV4_ERR_BYTE_VALUE_TOO_LARGE = VG_CME_UTV6AD_IPV4_ERR_BYTE_VALUE_TOO_LARGE,
  VG_UTV6AD_ERR_BYTE_PAIR_TOO_LARGE = VG_CME_UTV6AD_BYTE_PAIR_TOO_LARGE,
  VG_UTV6AD_ERR_IPV4_ADDRESS_TOO_SHORT_OR_BAD_CHARS = VG_CME_UTV6AD_IPV4_ADDRESS_TOO_SHORT_OR_BAD_CHARS,
  VG_UTV6AD_ERR_IPV6_ADDRESS_TOO_SHORT_OR_BAD_CHARS = VG_CME_UTV6AD_IPV6_ADDRESS_TOO_SHORT_OR_BAD_CHARS,
  VG_UTV6AD_ERR_BYTE_PAIR_VALUE_OVERFLOW = VG_CME_UTV6AD_BYTE_PAIR_VALUE_OVERFLOW,
  VG_UTV6AD_ERR_LEADING_SINGLE_COLON = VG_CME_UTV6AD_LEADING_SINGLE_COLON,
  VG_UTV6AD_ERR_TRAILING_SINGLE_COLON = VG_CME_UTV6AD_TRAILING_SINGLE_COLON,
  VG_UTV6AD_ERR_IPV4_EMBEDDED_NONFINALLY = VG_CME_UTV6AD_IPV4_EMBEDDED_NONFINALLY,
  VG_UTV6AD_ERR_BUFFER_TOO_SMALL = VG_CME_UTV6AD_BUFFER_TOO_SMALL,
  /* The next is not used by the conversion code, but is intended for
     use by callers in case in which it is an error to have any
     characters other than the address in the input string: i.e. where
     endptr is pointing to something other than a nul byte.  The
     rvutv6ad routines do not regard this as an error, since there are
     cases in which this is valid. */
  VG_UTV6AD_ERR_TRAILING_JUNK = VG_CME_UTV6AD_TRAILING_JUNK,
  VG_UTV6AD_ERR_LAST = VG_TOTAL_NUMBER_CODES
} 
VgUtv6adConvErr;

/***************************************************************************
 *  Macros
 **************************************************************************/

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

extern VgUtv6adConvErr 
vgUtv6adIpv4TextToBin (const Char *textualAddr,
                       Char **endptr_p,
                       Int8 binAddr[]);

extern VgUtv6adConvErr 
vgUtv6adIpv6TextToBin (const Char *textualAddr,
                       Char **endptr_p,
                       Int8 binAddr[],
                       Boolean allow3gppDottedDecimal,
                       Boolean allowNonfinalEmbeddedIpv4);

extern VgUtv6adConvErr 
vgUtv6adIpv6BinToDottedDecimalText (const Int8 binAddr[],
                                    Int8 textualAddrMaxSize,
                                    Char *textualAddr);

extern VgUtv6adConvErr 
vgUtv6adIpv6BinToText (const Int8 binAddr[],
                       Int8 textualAddrMaxSize,
                       Char *textualAddr,
                       Boolean includeLeadingZeros,
                       Boolean compressZeros);

extern VgUtv6adConvErr
vgUtv6adIpv6BinToColonHexText (const Int8 binAddr[],
                            Int8 textualAddrMaxSize,
                            Char *textualAddr,
                            Boolean includeLeadingZeros,
                            Boolean compressZeros);

extern VgUtv6adConvErr 
vgUtv6adIpv4BinToText (const Int8 binAddr[],
                       Int8 textualAddrMaxSize,
                       Char *textualAddr);
#endif

/* END OF FILE */
