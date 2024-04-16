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
 *  File Description :
 *
 *  This file contains the function prototypes of the UCS2/GSM conversion
 * functions.
 **************************************************************************/

#ifndef UTUC_FNC_H
#define UTUC_FNC_H


/***************************************************************************
 * Nested Include Files
 **************************************************************************/

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/***************************************************************************
 * Typed Constants
 **************************************************************************/

/***************************************************************************
 * Type Definitions
 **************************************************************************/

typedef enum AlphaIdCodingSchemeTag
{
  OCTET_ALIGNED_GSM_7BIT,
  DOUBLE_OCTET_UCS2 = 0x80,
  SINGLE_OCTET_BASE_POINTER_COMBINED,
  DOUBLE_OCTET_BASE_POINTER_COMBINED
} AlphaIdCodingScheme;

/*-----------------27/06/2002 11:23-----------------
 * New Typded for Extended Character Support
 * --------------------------------------------------*/
typedef enum GsmCharTypeTag
{
  NON_GSM7BIT      	= 0,
  GSM7BIT	    	= 1,
  EXTENDED_GSM7BIT	= 2
}GsmCharType;

/***************************************************************************
 *  Macros
 **************************************************************************/
#define GSM_CHARACTER_SET_SIZE          0x80

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/
#if 0
/* utUcs2StringToDebug function not in use */
extern Char *
utUcs2StringToDebug ( const Int16 * strUcs2_p );
#endif
extern Int16
utLengthAlphaIdToUcs2( const Int8 *alphaId,
                       Int16 alphaIdLen );

extern Int16
utUcs2ToAlphaId ( Int8        *alphaId_p,
                  Int16       maxAlphaIdLen,
                  const Int16 *strSrc_p,
                  Int16       srcLen );

extern Int16
utAlphaIdToUcs2 ( Int16      *strDst_p,
                  Int16      maxDstLen,
                  const Int8 *alphaId,
                  Int16      alphaIdLen );

/* extern Boolean
utIsUcs2StringAllGsm ( const Int16 *ucs2String_p,
                       Int16       length);   */

extern GsmCharType
utIsUcs2StringAllEGsm(const Int16 *ucs2Str_p, Int16 ucs2Length);



/* 0204-16329 Modified For Extended Character Support
 * - offset used to calculate number of extra bytes
 *   for each Extended Character */
/* extern Int16
utUcs2ToByteAlignedGsm7Bit ( Int8        *gsmData_p,
                             const Int16 *ucs2String_p,
                             Int16       length,
                             Int16       *offset_p);   */

extern GsmCharType
utUcs2ToByteAlignedEGsm7Bit(const Int16 *ucs2Str_p,
                            Int16 ucs2Length,
                            Int8 *gsmStr_p,
                            Int16 *gsmLength_p,
                            Int16 maxGsmLength,
                            Boolean *gsmBufferTooSmall_p);


extern Int16
utUcs2ToChar ( Char * charStr,
               const Int16 *ucs2String,
               Int16 length);

/* extern Int16
utByteAlignedGsm7BitToUcs2 ( Int16      *ucs2String,
                             const Int8 *gsmData,
                             Int16      length); */

extern GsmCharType
utByteAlignedEGsm7BitToUcs2(const Int8 *gsmStr_p,
                            Int16 gsmDataLength,
                            Int16 *ucs2Str_p,
                            Int16 *ucs2Length_p,
                            Int16 maxUcs2Len,
                            Boolean *ucs2BufferTooSmall_p);

/*0210-20704 */
extern GsmCharType
utSmsByteAlignedEGsm7BitToUcs2(const Int8 *gsmStr_p,
                               Int16 gsmDataLength,
                               Int16 *ucs2Str_p,
                               Int16 *ucs2Length_p,
                               Int16 maxUcs2Len,
                               Boolean *ucs2BufferTooSmall_p,
                               Int16 *numConverted_p);

extern Int16
utByteAlignedGsm7BitToChar ( Char *charString,
                             const Int8 * gsmData,
                             Int16 length);

extern void
utDoubleOctetToUcs2 ( Int16      *ucs2String,
                      const Int8 *srcData_p,
                      Int16      length);

extern void
utUcs2ToDoubleOctet ( Int8        *dst_p,
                      const Int16 *ucs2String,
                      Int16       length);



/* 0204-16329 mapGsmToUcs2 modified for Extended Character Support
 * - gsmString_p pointer to gsmData Buffer
 * - index_p pointer to referrence of array
 * - Length length of gsmString
 */
/*extern Int16
mapGsmToUcs2(const Int8 *gsmString_p, Int16 *index_p, Int16 length); */

extern GsmCharType
mapEGsmToUcs2(Int8 gsm1, Int8 gsm2, Int16 *ucs2_p);


/* 0204-16329 mapUcsToGsm modified for Extended Character Support
 * - gsmData_p pointer to gsm data buffer.
 * - index_p pointer to refference of array
 * - ucs2Char current Ucs2 Character
 * - gsmDataLength length of Gsm Buffer
 * - offSet_p used to calculate extra bytes for each extended character,
 */

/*-----------------17/06/2002 12:27-----------------
 * 0206-18260 modified Length to a pointer to gsmDataLength,
 * --------------------------------------------------*/
/* 0206-18260 Changed gsmDataLength to pointer */
/* extern Int8
mapUcs2ToGsm (Int8 *gsmData_p, Int16 *index_p, Int16 ucs2Char, Int16 *gsmDataLength, Int16 *offset_p ); */

extern GsmCharType
mapUcs2ToEGsm(Int16 ucs2, Int8 *gsm1_p, Int8 *gsm2_p);

extern Char
mapUcs2ToChar ( Int16 ucs2Char );

/* 0204-16329 utUcs2ToGsmLength - new function to get length of Gsm String from Ucs2String,
 *  maybe different due to new Extended Character Support
 *  - gsmData_p pointer to gsm string buffer.
 *  - ucs2String_p pointer to the Ucs2 String buffer
 *  - length size of Ucs2 String.
 *  - returns length of Gsm String including Extended Character Bytes.0
 *
 */

/* extern Int16 utUcs2ToGsmLength(Int8 *gsmData_p, const Int16 *ucs2String_p, Int16 length); */
extern GsmCharType
utGetEGsmLengthOfUcs2(const Int16 *ucs2Str_p, Int16 ucs2Length, Int16 *gsmLength_p);


extern SignedInt16 utCompareUcs2 ( Int16 *firstUcs2Str, Int16 firstLen, Int16 *secondUcs2Str, Int16 secondLen, Boolean doExactMatch);

#endif /* UTUC_FNC_H */
