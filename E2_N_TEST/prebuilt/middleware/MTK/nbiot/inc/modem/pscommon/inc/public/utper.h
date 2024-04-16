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
 * File Description:
 *
 *    Header file for packed encoding rule functions
 **************************************************************************/
#ifndef UTPER_H
#define UTPER_H

/***************************************************************************
** Nested Include Files
***************************************************************************/

#include <system.h>

/***************************************************************************
 *   Manifest Constants
 ***************************************************************************/

#define DEFAULT_DECODE_BUFFER_SIZE       500
#define MAX_UNCONSTRAINED_OCTET_STRING  2048
#define MAX_UNCONSTRAINED_BIT_STRING   16384
#define MAX_ENCODE_DECODE_ERRORS_TO_REPORT 1
#define PER_MIN_DATA_ALLOC                 1

/***************************************************************************
 *   Macro Functions
 ***************************************************************************/

/***************************************************************************
 *   Types
 ***************************************************************************/

typedef struct PerDecodeBufferTag
{
    Int16 decodeBufferSize;            /* Allocated decode buffer size in bytes */
    Int16 decodedDataLength;           /* Used length of this buffer in bytes */
    struct PerDecodeBufferTag *next_p; /* Pointer to next decode buffer */
    /* data part Must be at the end of the structure */
    Int8   data [PER_MIN_DATA_ALLOC];  /* data size is allocated dynamically. */
}
PerDecodeBuffer;

typedef struct PerBufferTag
{
    Int32 encodedDataBitOffset;/* Current bit offset into encodedDataPtr */
    Int32 previousBitOffset;   /* Previous bit offset into encodedDataPtr */
    Int32 maxEncodedBitLength;    /* Total length of encoded data buffer in bits */
    Int8 *encodedData_p;       /* Pointer to the start of encoded data buffer */
    PerDecodeBuffer *head_p;   /* Pointer to initial decode buffer space */
    PerDecodeBuffer *tail_p;   /* Pointer to last decode buffer space */
    Boolean getEncodeSizeOnly; /* TRUE if the user only wants to calculate the size
                                * of the encoded PER string */
    Boolean decodedSpareValue; /* TRUE if decoded a spare value with no DEFAULT */
    Boolean spareValueSubstituted; /* TRUE if decoded a spare value and it was
                                      substituted for a DEFAULT value */
#if defined (DEVELOPMENT_VERSION)
    Boolean errorCount;        /* Errors reported to TEST_FILE_OUT */
#endif
}
PerBuffer;

typedef enum PerErrorTag
{
    PER_NO_ERROR,
    PER_LENGTH_OUT_OF_RANGE,
    PER_VALUE_OUT_OF_RANGE,
    PER_ERROR_INVALID_ENUM,
    PER_INVALID_CHOICE_TAG,
    PER_UNSUPPORTED_EXTENSION, /* Decoded data had the extension bit set */
    PER_DECODE_BUFFER_OVERRUN,
    PER_UNSUPPORTED_TYPE
}
PerError;

/***************************************************************************
 *   Typed Constants
 ***************************************************************************/

/***************************************************************************
 *  Function Prototypes
 ***************************************************************************/

/***************************************************************************
 *  Encode Functions
 ***************************************************************************/
void PerEncBoolean (PerBuffer *perBuffer_p,
                    Boolean   *value_p);

void PerEncBooleanValue (PerBuffer *perBuffer_p,
                         Boolean   value);

void PerEncConstrainedLength (PerBuffer *perBuffer_p,
                              Int16     value,
                              Int16     min,
                              Int16     max);

void PerEncUnconstrainedLength (PerBuffer *perBuffer_p,
                                Int16     value);

void PerEncInt8 (PerBuffer *perBuffer_p,
                 Int8      *value_p,
                 Int8      min,
                 Int8      max);

void PerEncInt16 (PerBuffer *perBuffer_p,
                  Int16     *value_p,
                  Int16     min,
                  Int16     max);

void PerEncInt32 (PerBuffer *perBuffer_p,
                  Int32     *value_p,
                  Int32     min,
                  Int32     max);

void PerEncInt64 (PerBuffer *perBuffer_p,
                  Int64     *value_p,
                  Int64     min,
                  Int64     max);

void PerEncSignedInt8 (PerBuffer  *perBuffer_p,
                       SignedInt8 *value_p,
                       SignedInt8 min,
                       SignedInt8 max);

void PerEncSignedInt16 (PerBuffer   *perBuffer_p,
                        SignedInt16 *value_p,
                        SignedInt16 min,
                        SignedInt16 max);

void PerEncSignedInt32 (PerBuffer   *perBuffer_p,
                        SignedInt32 *value_p,
                        SignedInt32 min,
                        SignedInt32 max);

void PerEncSignedInt64 (PerBuffer   *perBuffer_p,
                        SignedInt64 *value_p,
                        SignedInt64 min,
                        SignedInt64 max);

void PerEncFixedBitString (PerBuffer *perBuffer_p,
                           Int8      *value_p,
                           Int16     number);

void PerEncFixedOctetString (PerBuffer *perBuffer_p,
                             Int8      *value_p,
                             Int16     length);

void PerEncVariableBitString (PerBuffer *perBuffer_p,
                              Int8      *value_p,
                              Int16     length,
                              Int16     min,
                              Int16     max);

void PerEncVariableOctetString (PerBuffer *perBuffer_p,
                                Int8      *value_p,
                                Int16     length,
                                Int16     min,
                                Int16     max);

void PerEncUnconstrainedOctetString (PerBuffer *perBuffer_p,
                                     Int8      *value_p,
                                     Int16     length);

void PerEncUnconstrainedBitString (PerBuffer *perBuffer_p,
                                   Int8      *value_p,
                                   Int16     length);

void PerEncEnum (PerBuffer *perBuffer_p,
                 Int8      *value_p,
                 Int8      max);

void PerEncEnumWithExtension (PerBuffer *perBuffer_p,
                              Int8      *value_p,
                              Int8      maxValueInRoot);

void PerEncStartOfBitAlignedDataWithLength (PerBuffer *perBuffer_p);

void PerEncEndOfBitAlignedDataWithLength (PerBuffer *perBuffer_p);

void PerEncStartOfOctetAlignedDataWithLength (PerBuffer *perBuffer_p);

void PerEncEndOfOctetAlignedDataWithLength (PerBuffer *perBuffer_p);

/***************************************************************************
 *  Decode Functions
 ***************************************************************************/
PerError PerDecBoolean (PerBuffer *perBuffer_p,
                        Boolean   *value_p);

PerError PerDecConstrainedLength (PerBuffer *perBuffer_p,
                                  Int16     *value_p,
                                  Int16     min,
                                  Int16     max);

PerError PerDecUnconstrainedLength (PerBuffer *perBuffer_p,
                                    Int16     *value_p);

PerError PerDecInt8 (PerBuffer *perBuffer_p,
                     Int8      *value_p,
                     Int8      min,
                     Int8      max);

PerError PerDecInt16 (PerBuffer *perBuffer_p,
                      Int16     *value_p,
                      Int16     min,
                      Int16     max);

PerError PerDecInt32 (PerBuffer *perBuffer_p,
                      Int32     *value_p,
                      Int32     min,
                      Int32     max);

PerError PerDecInt64 (PerBuffer *perBuffer_p,
                      Int64     *value_p,
                      Int64     min,
                      Int64     max);

PerError PerDecSignedInt8 (PerBuffer  *perBuffer_p,
                           SignedInt8 *value_p,
                           SignedInt8 min,
                           SignedInt8 max);

PerError PerDecSignedInt16 (PerBuffer   *perBuffer_p,
                            SignedInt16 *value_p,
                            SignedInt16 min,
                            SignedInt16 max);

PerError PerDecSignedInt32 (PerBuffer   *perBuffer_p,
                            SignedInt32 *value_p,
                            SignedInt32 min,
                            SignedInt32 max);

PerError PerDecSignedInt64 (PerBuffer   *perBuffer_p,
                            SignedInt64 *value_p,
                            SignedInt64 min,
                            SignedInt64 max);

PerError PerDecFixedBitString (PerBuffer *perBuffer_p,
                               Int8      *value_p,
                               Int16     length);

PerError PerDecFixedOctetString (PerBuffer *perBuffer_p,
                                 Int8      *value_p,
                                 Int16     length);

PerError PerDecVariableBitString (PerBuffer *perBuffer_p,
                                  Int8      *value_p,
                                  Int16     *length_p,
                                  Int16     min,
                                  Int16     max);

PerError PerDecVariableOctetString (PerBuffer *perBuffer_p,
                                    Int8      *value_p,
                                    Int16     *length_p,
                                    Int16     min,
                                    Int16     max);

PerError PerDecUnconstrainedBitString (PerBuffer *perBuffer_p,
                                       Int8      **value_p,
                                       Int16     *length_p);

PerError PerDecUnconstrainedOctetString (PerBuffer *perBuffer_p,
                                         Int8      **value_p,
                                         Int16     *length_p);

PerError PerDecEnum (PerBuffer *perBuffer_p,
                     Int8      *value_p,
                     Int8      max);

PerError PerDecEnumWithExtension (PerBuffer *perBuffer_p,
                                  Int8      *value_p,
                                  Int8      maxValueInRoot);

PerError PerDecStartOfBitAlignedDataWithLength (PerBuffer *perBuffer_p);

void PerDecEndOfBitAlignedDataWithLength (PerBuffer *perBuffer_p);

PerError PerDecStartOfOctetAlignedDataWithLength (PerBuffer *perBuffer_p);

void PerDecEndOfOctetAlignedDataWithLength (PerBuffer *perBuffer_p);

PerError PerSkipUnconstrainedBitString (PerBuffer *perBuffer_p);

PerError PerDecPadding (PerBuffer *perBuffer_p,
                        Int8      **value_p,
                        Int16     *length_p);

#if defined (UPGRADE_NASMDL2)
Boolean PerDecExtensionPresent (PerBuffer *perBuffer_p, Int8 noOfExtensionAdditions, Int8 extension, Int32 *startOfExtensionPresenceFlags_p);

PerError PerDecSkipAllUnsupportedExtensions (PerBuffer *perBuffer_p);
#endif /* UPGRADE_NASMDL2 */

/***************************************************************************
 *  Memory Allocation Functions
 ***************************************************************************/
void PerAllocMemory (PerBuffer *perBuffer_p,
                     Int16     numberOfBytes,
                     void      **buffer_p);

void PerFreeMemory (PerBuffer *perBuffer_p);


/***************************************************************************
 *  Initialisation Functions
 ***************************************************************************/
void PerInitEncode (PerBuffer *perBuffer_p,
                    Int8      *encodedData_p,
                    Int32     maxEncodedBitLength,
                    Int16     initialDecodeSize,
                    void      **decodedData_p);

void PerInitDecode (PerBuffer *perBuffer_p,
                    Int8      *encodedData_p,
                    Int32     maxEncodedBitLength,
                    Int16     initialDecodeSize,
                    void      **decodedData_p);


#endif
