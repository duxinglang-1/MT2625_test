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
 *  This file contains all of the low level routines required
 *  to encode and decode information elements of SMS messages.
 **************************************************************************/

#ifndef SMRDWR_H
#define SMRDWR_H

/******************************************************************************
 * Include Files
 *****************************************************************************/

#if !defined (SMRL_SIG_H)
#   include  <smrl_sig.h>
#endif

/******************************************************************************
 * Manifest Constants
 *****************************************************************************/

#define INT8_ALL_ZEROS 0x00
#define INT8_ALL_ONES  0xFF

/******************************************************************************
 * Types
 *****************************************************************************/

/******************************************************************************
 * Variables
 *****************************************************************************/

/******************************************************************************
 * Macros
 *****************************************************************************/

/* Compute memory space required to store [n] values in semi-octet form */
#define SemiOctetSize(n) ((n)/2 + (n)%2)

/******************************************************************************
 * Local Functions
 ******************************************************************************/

/**** MACRO ****************************************************************/

/*
Set the field starting at the LSB specified by [bp]
within octet [op] to the value specified by [fv].
The octet being written to is assumed to have had the appropriate bits
initialised to zero (allowing logical OR operation to be used)
The value is assumed NOT to straddle an octet boundary.
The value is assumed to fit the available bit space.
See GSM 03.40 9.1.1.
*/
#define WriteBitData(op, bp, fv, bptr) bptr[op] |= (fv << bp)


/**** MACRO ****************************************************************/

/*
Write an item of data of type octet stored in [val] to the memory
area pointed to by [bp] and offset by [op]
See GSM 03.40 9.1.2.2
*/
#define WriteOctetData(op, val, bp) bp[op] = val



/**** MACRO ****************************************************************/

/*
Write an item of data of type integer stored in [val] to the memory
area pointed to by [bp] and offset by [op]
Note that it is assumed that the integer occupies 8 bits of an octet
ie does not straddle an octet boundary.
See GSM 03.40 9.1.2.1.
*/
#define WriteIntegerData(op, val, bp) bp[op] = val

/**** FUNCTION *************************************************************/
void
WriteSemiOctetData (Int8 octetPosition, const Int8 *numberList,
                    Int8 numberOfDigits, Int8 *basePtr);

/*
Convert data from a list of digits pointed to by [numberList] into
semi-octet format and write to the memory area
pointed to by [baseptr] and offset by [octetPosition].
See GSM 03.40 9.1.2.3.
*/


/**** FUNCTION *************************************************************/
Int8
WriteAddressData (Boolean use0408Format, Int8 octetPosition, const SmsAddress *addr, Int8 *basePtr);
/*
Convert the data pointed to by [addr] into one of the
Called party BCD number IE (GSM 04.08 10.5.4.7) or
a SMS address field (GSM 03.40 9.1.2.5)
and write it to the memory area pointed to by [baseptr] and offset
by [octetPosition].

The difference in format is wether the length byte written
represents the length of the element, or the number of BCD digits
encoded.

return the number of octets written
*/

/**** MACRO ****************************************************************/

/*
Read the field starting at [bitPosition] within octet
[octetPosition] and store in [fieldValue].
The value is assumed NOT to straddle an octet boundary.
See GSM 03.40 9.1.1.
*/
#define ReadBitData(octetPosition, bitPosition,             \
                     fieldSize, fieldValue, basePtr)        \
   *(fieldValue) = basePtr[octetPosition] >> bitPosition;   \
   *(fieldValue) &= ~(INT8_ALL_ONES << fieldSize);


/**** MACRO ****************************************************************/

/*
Read the octet value at [octetPosition] and return in [fieldValue]
See GSM 03.40 9.1.2.2.
*/
#define ReadOctetData(octetPosition, fieldValue, basePtr) \
   *(fieldValue) = basePtr[octetPosition];


/**** MACRO ****************************************************************/

/*
Read the integer value at [octetPosition] and return in [fieldValue]
Note that it is assumed that the integer occupies 8 bits of an octet
ie does not straddle an octet boundary.
See GSM 03.40 9.1.2.1.
*/
#define ReadIntegerData(octetPosition, fieldValue, basePtr) \
   *(fieldValue) = basePtr[octetPosition];

/**** FUNCTION *************************************************************/

Int8
ReadSemiOctetData (Int8 octetPosition, Int8 *numberList,
                   Int8 numberOfDigits, const Int8 *basePtr, Boolean checkForPad);
/*
Read data in semi-octet format and convert to
an array of octets (MSB first). See GSM 03.40 9.1.2.3.
*/


/**** FUNCTION *************************************************************/

Int8
ReadAddressData (Int8 octetPosition, SmsAddress *addr, const Int8 *basePtr);
/*
Read the data pointed to by [basePtr] and offset by [octetPosition] and
store the data in the structure pointed to by [addr].
See GSM 03.40 9.1.2.3

In this format, the length byte contains the number of BCD digits contained
within the structure. The length of the information element is deduced from
this and returned to the caller.
*/

Int8
ReadRpAddressData (Int8 octetPosition, SmsAddress *addr, const Int8 *basePtr);
/*
This routine reads address data from a RP_DATA message in the format of 04.08
10.5.4.7. In this format, the length octet represents the number of octets
following.

A length of 0 is valid and is catered for as a special case.

The routine returns the actual number of octets read
*/

#endif

/* END OF FILE */
