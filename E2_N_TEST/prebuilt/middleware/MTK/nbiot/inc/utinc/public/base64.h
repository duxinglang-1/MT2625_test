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
 * Interface and types for BASE64 variants encoding/decoding operations.
 *
 * The variants differ in the alphabets used to represent numeric values.
 *
 * BASE64_ALPHABET_BASIC       - RFC3548, (Implemented)
 * BASE64_ALPHABET_URLFILESAFE - RFC3548, (Implemented)
 * BASE64_ALPHABET_MODIFIED    - RFC3501, (Implemented)
 *
 * More details:
 *
 * http://www.ietf.org/rfc/rfc2045.txt
 * http://www.ietf.org/rfc/rfc3501.txt
 * http://www.ietf.org/rfc/rfc3548.txt
 *
 ******************************************************************************
 *
 * BASE64_ALPHABET_BASIC
 *
 *      Value Encoding  Value Encoding  Value Encoding  Value Encoding
 *         0 A            17 R            34 i            51 z
 *         1 B            18 S            35 j            52 0
 *         2 C            19 T            36 k            53 1
 *         3 D            20 U            37 l            54 2
 *         4 E            21 V            38 m            55 3
 *         5 F            22 W            39 n            56 4
 *         6 G            23 X            40 o            57 5
 *         7 H            24 Y            41 p            58 6
 *         8 I            25 Z            42 q            59 7
 *         9 J            26 a            43 r            60 8
 *        10 K            27 b            44 s            61 9
 *        11 L            28 c            45 t            62 +
 *        12 M            29 d            46 u            63 /
 *        13 N            30 e            47 v
 *        14 O            31 f            48 w         (pad) =
 *        15 P            32 g            49 x
 *        16 Q            33 h            50 y
 *
 ******************************************************************************
 *
 * BASE64_ALPHABET_URLFILESAFE
 *
 *   Value Encoding  Value Encoding  Value Encoding  Value Encoding
 *      0 A            17 R            34 i            51 z
 *      1 B            18 S            35 j            52 0
 *      2 C            19 T            36 k            53 1
 *      3 D            20 U            37 l            54 2
 *      4 E            21 V            38 m            55 3
 *      5 F            22 W            39 n            56 4
 *      6 G            23 X            40 o            57 5
 *      7 H            24 Y            41 p            58 6
 *      8 I            25 Z            42 q            59 7
 *      9 J            26 a            43 r            60 8
 *     10 K            27 b            44 s            61 9
 *     11 L            28 c            45 t            62 - (minus)
 *     12 M            29 d            46 u            63 _ (understrike)
 *     13 N            30 e            47 v
 *     14 O            31 f            48 w         (pad) =
 *     15 P            32 g            49 x
 *     16 Q            33 h            50 y
 *
 *******************************************************************************
 *
 * BASE64_ALPHABET_MODIFIED
 *
 *      Value Encoding  Value Encoding  Value Encoding  Value Encoding
 *         0 A            17 R            34 i            51 z
 *         1 B            18 S            35 j            52 0
 *         2 C            19 T            36 k            53 1
 *         3 D            20 U            37 l            54 2
 *         4 E            21 V            38 m            55 3
 *         5 F            22 W            39 n            56 4
 *         6 G            23 X            40 o            57 5
 *         7 H            24 Y            41 p            58 6
 *         8 I            25 Z            42 q            59 7
 *         9 J            26 a            43 r            60 8
 *        10 K            27 b            44 s            61 9
 *        11 L            28 c            45 t            62 +
 *        12 M            29 d            46 u            63 ,
 *        13 N            30 e            47 v
 *        14 O            31 f            48 w         (pad) =
 *        15 P            32 g            49 x
 *        16 Q            33 h            50 y
 **************************************************************************/

#ifndef BASE64_H
#define BASE64_H

#if !defined (KERNEL_H)
#include <kernel.h>
#endif

/******************************************************************************
 * Manifest Constants
 *****************************************************************************/

#define B00000011   3
#define B00001111  15
#define B00110000  48
#define B00111100  60
#define B00111111  63
#define B11000000 192
#define B11110000 240
#define B11111100 252

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Type Definitions
 *****************************************************************************/

typedef enum Base64ResultTag
{
  BASE64_RESULT_NULL,        /* 00 */
  BASE64_OK,                 /* 01 */
  BASE64_INPUT_INVALID,      /* 02 */
  BASE64_ZERO_POINTER,       /* 03 */
  BASE64_BUFFER_SIZE,        /* 04 */
  BASE64_NO_DYN_MEMORY,      /* 05 */
  BASE64_ALPHABET,           /* 06 */
  BASE64_INPUT_BYTE_COUNT,   /* 07 */
  BASE64_MULTIPLE_OF_4,      /* 08 */
  BASE64_OUT_BUFFER_SIZE,    /* 09 */
  BASE64_SIZE_UNEQUAL,       /* 10 */
  BASE64_LARGER_THAN_BUFFER  /* 11 */
}
Base64Result;

typedef enum Base64AlphabetTag
{
  BASE64_ALPHABET_NULL,
  BASE64_ALPHABET_BASIC,       /* Most common - See RFC 3548 */
  BASE64_ALPHABET_URLFILESAFE, /* Make URL and FILENAME handling safe - See RFC3548 */
  BASE64_ALPHABET_MODIFIED     /* Used in IMAP - See RFC 3501, RFC3548 */
}
Base64Alphabet;

/******************************************************************************
 * Variable Declarations
 *****************************************************************************/

/******************************************************************************
 * Global Function Declarations
 *****************************************************************************/



#if defined (PC_CONSOLE_FUNCTION_TEST)

/*
  Only public under test condition to make functions accessible from
  the testprogram.
*/

extern Base64Result base64Encode(Base64Alphabet const alphabet,
                                 Int8 *const buffer_p,
                                 Int16 const bufferMaxSize,
                                 Int8 const validBytes);

extern Base64Result base64Decode(Base64Alphabet const alphabet,
                                 Int8 *const buffer_p,
                                 Int16 const bufferMaxSize,
                                 Int8 *const validBytes_p);

#endif

extern Base64Result base64EncodeBuffer(Base64Alphabet const alphabet,
                                       Int8 *const inBuff_p,
                                       Int32 const inBuffMaxSize,
                                       Int32 const inBuffBytes,
                                       Int8 *const outBuff_p,
                                       Int32 const outBuffMaxSize,
                                       Int32 *const generatedBytes_p);

extern Base64Result base64DecodeBuffer(Base64Alphabet const alphabet,
                                       Int8 *const inBuff_p,
                                       Int32 const inBuffMaxSize,
                                       Int32 const inBuffBytes,
                                       Int8 *const outBuff_p,
                                       Int32 const outBuffMaxSize,
                                       Int32 *const generatedBytes_p);


#endif

