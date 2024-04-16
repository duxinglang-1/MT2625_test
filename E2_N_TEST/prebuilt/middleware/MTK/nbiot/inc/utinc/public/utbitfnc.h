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
 *  This file contains the function prototypes of the bit manipulation routines
 *  available as utility functions to all TTPCom code.
 **************************************************************************/

#ifndef UTBITFNC_H
#define UTBITFNC_H

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

/***************************************************************************
 *  Macros
 **************************************************************************/

extern const   Int8    utilOffsetLengthMask [BITS_PER_INT8][BITS_PER_INT8+1];

/* Macro to get bits of a bitmap array, which rely on the fact that the start
 * of the bitmap is octet aligned
 */
#define UTIL_BITMAP_GET(bitmap,offset)                                      \
    (((bitmap)[(offset)/BITS_PER_INT8] & (0x80>>((offset)%BITS_PER_INT8))) ? 1 : 0)

/* position of bit within character */
#define BIT_CHAR(bit)         ((bit) / CHAR_BIT)

/* array index for character containing bit */
#define BIT_IN_CHAR(bit)      (1 << (CHAR_BIT - 1 - ((bit)  % CHAR_BIT)))


/***************************************************************************
 *  Inline functions
 **************************************************************************/
 
/******************************************************************************
  *
  * Function     : UtilBitmapSet
  *
  * Scope        : GLOBAL
  *
  * Parameters   : bitmap                    Octet aligned, left justified, array
  *                                          of octets, containing boolean flags,
  *                                          where:  bit 0 = MSB of octet 1
  *                                                  bit 7 = MSB of octet 1
  *                                                  bit 8 = MSB of octet 2
  *                                                  bit 15= MSB of octet 2
  *                                                  ...
  *                offset                    Bit number to set
  *                value                     Value to set
  *
  * Returns      : None
  *
  * Description  : Set the value of one bit in an octet aligned bitmap
  *
  *****************************************************************************/

static __inline void  UtilBitmapSet (Int8       bitmap [],
                                      Int16      offset,
                                      Boolean    value)
{
    if (value)                                        
    {                                             
        bitmap[BIT_CHAR(offset)] |= BIT_IN_CHAR(offset); 
    }                                             
    else                                          
    {                                             
        Int8 mask;                                
                                                      
        /* create a mask to zero out desired bit */ 
        mask =  BIT_IN_CHAR(offset);                   
        mask = ~mask;                               
                                                        
        bitmap[BIT_CHAR(offset)] &= mask;               
    }   
}

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/
Int16
UtEncode7BitPackedData (Int8 *buffer, Int16 msgLen, const Int8 *msgData, Int16 maxChars);

Int16
UtDecode7BitPackedData (Int8 *destData, const Int8 *sourceData, Int16 destLength, Int16 sourceLength);

void UtilEncodeBitField (
      Int8   *dest_p,
      Int32  *destBitOffset,
      Int8   numOfBits,
      Int64  value);

Int8 UtilExtract8BitField (
      Int8   * const source_p,
      Int32  * const sourceBitOffset,
      const Int8   numOfBits);

Int16 UtilExtract16BitField (
      Int8   * const source_p,
      Int32  * const sourceBitOffset,
      const Int8   numOfBits);

Int32 UtilExtract32BitField (
      Int8   * const source_p,
      Int32  * const sourceBitOffset,
      const Int8   numOfBits);

Int64 UtilExtract64BitField (
      Int8   * const source_p,
      Int32  * const sourceBitOffset,
      const Int8   numOfBits);

#if defined(ENABLE_UP_MIPS_TEST)
  #if defined(UPGRADE_NOT_FOR_RELEASE)
/* UmipsUtilBitCopy may or may not be mapped to UtilBitCopy depening on
 * command line options */
#define UtilBitCopy(s, sbo, sbl, d, dbo) UmipsUtilBitCopy(s, sbo, sbl, d, dbo)
void UmipsUtilBitCopy (
      Int8   *source_p,
      Int32  sourceBitOffset,
      Int32  sourceBitLength,
      Int8   *destination_p,
      Int32  destinationBitOffset);
  #endif /* UPGRADE_NOT_FOR_RELEASE */
#else
void UtilBitCopy (
      Int8   *source_p,
      Int32  sourceBitOffset,
      Int32  sourceBitLength,
      Int8   *destination_p,
      Int32  destinationBitOffset);
#endif /* ENABLE_UP_MIPS_TEST */

void UtilBitMove (Int8    *source_p,
                  Int32   sourceBitOffset,
                  Int32   bitLength,
                  Int8    *destination_p,
                  Int32   destinationBitOffset);

Boolean IsSysMem(Int8 *source_p);
#endif

/* END OF FILE */
