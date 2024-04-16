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
 *  Utility functions for voyager for IPv6 address conversion
 **************************************************************************/

#define MODULE_NAME "RVUTV6AD"


/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>

#include <stddef.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <rvsystem.h>

#if defined (VG_UTV6AD_UNIT_TEST)
/* avoid pulling in rvcrdata.h */
#define RVCRDATA_H
enum DummyResultCode
{
  RESULT_CODE_OK,
  VG_CME_UTV6AD_MORE_THAN_ONE_DOUBLE_COLON,
  VG_CME_UTV6AD_IPV4_ADDRESS_OFF_END,
  VG_CME_UTV6AD_DOTTED_DECIMAL_OUTSIDE_IPV4,
  VG_CME_UTV6AD_IPV4_ERR_BYTE_VALUE_OVERFLOW,
  VG_CME_UTV6AD_IPV4_ERR_EMPTY_BYTE,
  VG_CME_UTV6AD_IPV4_ERR_BYTE_VALUE_TOO_LARGE,
  VG_CME_UTV6AD_BYTE_PAIR_TOO_LARGE,
  VG_CME_UTV6AD_IPV4_ADDRESS_TOO_SHORT_OR_BAD_CHARS,
  VG_CME_UTV6AD_IPV6_ADDRESS_TOO_SHORT_OR_BAD_CHARS,
  VG_CME_UTV6AD_BYTE_PAIR_VALUE_OVERFLOW,
  VG_CME_UTV6AD_LEADING_SINGLE_COLON,
  VG_CME_UTV6AD_TRAILING_SINGLE_COLON,
  VG_CME_UTV6AD_IPV4_EMBEDDED_NONFINALLY,
  VG_CME_UTV6AD_BUFFER_TOO_SMALL,
  VG_CME_UTV6AD_TRAILING_JUNK,
  VG_TOTAL_NUMBER_CODES
};
#endif
#include <rvutv6ad.h>

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/* These maximum lengths are used only in this file.  It is not yet
   clear how exact they should relate to the values defined in
   pdp_typ.h and other places.  The complex tangle of #if defined
   values used makes this non-obvious. */
#define VG_UTV6AD_IPV4_BIN_LEN 4u
#define VG_UTV6AD_IPV6_BIN_LEN 16u

/* The textual lengths include the final nul.  The value for
   VG_UTV6AD_IPV6_MAX_TEXT_LEN allows for an embedded ipv4 address. */
#define VG_UTV6AD_IPV6_MAX_TEXT_LEN 46u
#define VG_UTV6AD_IPV4_MAX_TEXT_LEN 16u
#define VG_UTV6AD_IPV6_MAX_DOTTED_DECIMAL_TEXT_LEN 64u

/***************************************************************************
 * Types
 ***************************************************************************/

/***************************************************************************
 * Variables
 ***************************************************************************/


/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/
/***************************************************************************
 * Macros
 ***************************************************************************/

/***************************************************************************
 * Local Functions
 ***************************************************************************/



#if defined (VG_UTV6AD_DEBUG) || defined (VG_UTV6AD_UNIT_TEST)
static void printBinAddr (const Int8 binAddr[VG_UTV6AD_IPV6_BIN_LEN]);
#endif

/****************************************************************************
 *
 * Function: vgUtv6adIpv4TextToBin -- convert an ipv4 address to
 * binary form.
 *
 * Parameters: textualAddr should contain the nul-terminated textual
 * form.  binAddr will be filled in with the binary address; it must
 * be at least VG_UTV6AD_IPV4_BIN_LEN bytes long.  endptr_p will be
 * set to point to the input character where the routine stopped
 * converting the input; in case of failure, this will the first
 * invalid character.  If and only if the string contains just a
 * single complete and correct address, then VG_UTV6AD_ERR_OK will be
 * returned and *endptr_p will be 0.
 *
 * Returns:     error code.
 *
 * Description:
 *
 ****************************************************************************/
VgUtv6adConvErr
vgUtv6adIpv4TextToBin (const Char *textualAddr,
                     Char **endptr_p,
                     Int8 binAddr[])
{
  const Char *addr = textualAddr;
  Char *endptr = PNULL;
  Int8 binPos = 0;
  VgUtv6adConvErr err;

  err = VG_UTV6AD_ERR_OK;
#if defined (VG_UTV6AD_DEBUG)
  fprintf (stderr, "looking for ipv4 addr in %s\n", textualAddr);
#endif
  memset (binAddr, 0, VG_UTV6AD_IPV4_BIN_LEN);
  do
  {
    Int32 byte = 0;

    errno = 0;
    if (err == VG_UTV6AD_ERR_OK)
    {
      byte = (Int32)strtoul ((const char *)addr, (char **)&endptr, 10);
    }
    if (errno == ERANGE)
    {
      err = VG_UTV6AD_ERR_IPV4_ERR_BYTE_VALUE_OVERFLOW;
    }
    if ((err == VG_UTV6AD_ERR_OK) && (addr == endptr) && (addr != PNULL) && (*addr == '.'))
    {
      /* strtoul may or may not have detected this case */
      err = VG_UTV6AD_ERR_IPV4_ERR_EMPTY_BYTE;
    }
    if (err == VG_UTV6AD_ERR_OK && byte >= 0x100)
    {
      err = VG_UTV6AD_ERR_IPV4_ERR_BYTE_VALUE_TOO_LARGE;
    }
    if ((err == VG_UTV6AD_ERR_OK) && (endptr != PNULL))
    {
      binAddr[binPos] = (Int8)byte;
      binPos += 1;
      addr = endptr + 1;
    }
  } while ((err == VG_UTV6AD_ERR_OK) && (binPos < VG_UTV6AD_IPV4_BIN_LEN) && (endptr != PNULL) && (*endptr == '.'));

  if (err == VG_UTV6AD_ERR_OK && binPos != VG_UTV6AD_IPV4_BIN_LEN)
  {
    err = VG_UTV6AD_ERR_IPV4_ADDRESS_TOO_SHORT_OR_BAD_CHARS;
  }

  *endptr_p = endptr;

  return (err);
}

/****************************************************************************
 *
 * Function:    vgUtv6adIpv6TextToBin -- convert an ipv6 address to binary form
 *
 * Parameters:  textualAddr should contain the nul-terminated
 * textual form.  binAddr will be filled in with the binary address;
 * it must be at least VG_UTV6AD_IPV6_BIN_LEN bytes long.  endptr_p
 * will be set to point to the input character where the routine
 * stopped converting the input; in case of failure, this will the
 * first invalid character.  If allow3gppDottedDecimal is set, the
 * 3gpp form, which allows dotted-decimal beyond embedded ipv4
 * addresses, is accepted.  If and only if the string contains just a
 * single complete and correct address, then VG_UTV6AD_ERR_OK will be
 * returned and *endptr_p will be 0.
 * Note that the AT+CGPIAF settings are not required as this function can
 * handle the compressed and non-compressed zeros, and also leading zeros
 * present or not present.
 *
 * Returns:     error code
 *
 * Description:
 *
 ****************************************************************************/
VgUtv6adConvErr
vgUtv6adIpv6TextToBin (const Char *textualAddr,
                       Char **endptr_p,
                       Int8 binAddr[],
                       Boolean allow3gppDottedDecimal,
                       Boolean allowNonfinalEmbeddedIpv4)
{
  const Char *addr = textualAddr;
  Char *endptr = PNULL;
  VgUtv6adConvErr err;
  Int8 binPos = 0;
  Int8 fillBinPos = VG_MAX_UINT8;

  err = VG_UTV6AD_ERR_OK;
  memset (binAddr, 0, VG_UTV6AD_IPV6_BIN_LEN);
  if (textualAddr[0] == ':' && textualAddr[1] != ':')
  {
    err = VG_UTV6AD_ERR_LEADING_SINGLE_COLON;
    endptr = (Char *) &textualAddr[1];
  }
  if (err == VG_UTV6AD_ERR_OK)
  {
    do
    {
      Int32 bibyte = 0;

      /* check for double colon */
      if ((addr > textualAddr) && (*(addr-1) == ':') && (*addr == ':'))
      {
        if (fillBinPos == VG_MAX_UINT8)
        {
          /* First double colon -- remember where it occurred in the
             binary form */
          fillBinPos = binPos;
        }
        else
        {
          err = VG_UTV6AD_ERR_MORE_THAN_ONE_DOUBLE_COLON;
        }
      }
      if (err == VG_UTV6AD_ERR_OK)
      {
        errno = 0;
        bibyte = strtoul ((const char *)addr, (char **)&endptr, 16);
        if (errno == ERANGE)
        {
          err = VG_UTV6AD_ERR_BYTE_PAIR_VALUE_OVERFLOW;
        }
      }
      if (err == VG_UTV6AD_ERR_OK)
      {
        if ((endptr != PNULL) && (*endptr == '.'))
        {
#if defined (VG_UTV6AD_DEBUG)
          fprintf (stderr, "found . at start of %s\n", endptr);
#endif
          /* It seems as though we've found an embedded ipv4 address.
             Have another try from the same place, this time working
             through the whole ipv4 address, starting at addr. */
          if ((binPos < VG_UTV6AD_IPV6_BIN_LEN - VG_UTV6AD_IPV4_BIN_LEN) &&
              (fillBinPos == VG_MAX_UINT8) &&
              (!allowNonfinalEmbeddedIpv4) &&
              (!allow3gppDottedDecimal))
          {
            err = VG_UTV6AD_ERR_IPV4_EMBEDDED_NONFINALLY;
          }
          if (binPos > VG_UTV6AD_IPV6_BIN_LEN - VG_UTV6AD_IPV4_BIN_LEN)
          {
            err = VG_UTV6AD_ERR_IPV4_ADDRESS_OFF_END;
          }
          if (err == VG_UTV6AD_ERR_OK)
          {
            err = vgUtv6adIpv4TextToBin (addr, &endptr, &binAddr[binPos]);
            /* An ipv4 address must not be followed by a . unless
               we're allowing the 3gpp form. */
            if (err == VG_UTV6AD_ERR_OK &&
                ! allow3gppDottedDecimal && *endptr == '.')
            {
              err = VG_UTV6AD_ERR_DOTTED_DECIMAL_OUTSIDE_IPV4;
            }
#if defined (VG_UTV6AD_DEBUG)
            fprintf (stderr, "ipv4 addr processed, err %u, at start of %s\n",
                     err, addr);
#endif
          }
          if (err == VG_UTV6AD_ERR_OK)
          {
            binPos += VG_UTV6AD_IPV4_BIN_LEN;
          }
          if (endptr != PNULL)
          {
            addr = endptr + 1;
          }
        }
        else
        {
          if (bibyte >= 0x10000)
          {
            err = VG_UTV6AD_ERR_BYTE_PAIR_TOO_LARGE;
          }
          if (err == VG_UTV6AD_ERR_OK)
          {
            binAddr[binPos] = (Int8)(bibyte >> BITS_PER_INT8);
            binAddr[binPos+1] = (Int8)(bibyte & 0xff);
            binPos += 2;
          }
          if (endptr != PNULL)
          {
            addr = endptr + 1;
          }
        }
      }
      /* In the loop test, allow '.' as well, to allow 3gpp's eccentric
         style of writing ipv6 addresses, dotted decimal like ipv4. */
    }
    while (err == VG_UTV6AD_ERR_OK && binPos < VG_UTV6AD_IPV6_BIN_LEN &&
           ((endptr!=PNULL) && (*endptr == ':' || (allow3gppDottedDecimal && (*endptr == '.')))));
  }

  if ((endptr != PNULL) &&
      (err == VG_UTV6AD_ERR_OK) && (endptr >= textualAddr + 2u) &&
      (endptr[-2] != ':') && (endptr[-1] == ':') && (endptr[0] != ':'))
  {
    err = VG_UTV6AD_ERR_TRAILING_SINGLE_COLON;
  }
  if (err == VG_UTV6AD_ERR_OK)
  {
    if (fillBinPos == VG_MAX_UINT8)
    {
      /* No double colons -- check the address is full-length, but no
         longer. */
      if (binPos != VG_UTV6AD_IPV6_BIN_LEN)
      {
        err = VG_UTV6AD_ERR_IPV6_ADDRESS_TOO_SHORT_OR_BAD_CHARS;
      }
    }
    else
    {
      /* Double colon occurred.  Shift the part of the binary address
         which occurred to the right of it, and fill the gap with
         zeroes. */
      Int8 numFillBytes = VG_UTV6AD_IPV6_BIN_LEN - binPos;
#if defined (VG_UTV6AD_DEBUG)
      printBinAddr (binAddr);
      fputc ('\n', stdout);
      fprintf (stderr, " double colon found, shifting right by %u at %u\n",
               numFillBytes, fillBinPos);
#endif
      memmove (&binAddr[fillBinPos + numFillBytes],
               &binAddr[fillBinPos],
               VG_UTV6AD_IPV6_BIN_LEN - numFillBytes - fillBinPos);
      memset (&binAddr[fillBinPos], 0, numFillBytes);
    }
  }
  *endptr_p = endptr;
  return (err);
}

/****************************************************************************
 *
 * Function: vgUtv6adIpv4BinToText -- convert an ipv4 address to
 * textual form.
 *
 * Parameters: textualAddrMaxSize is the amount of space available at
 * textualAddr; textualAddr will be filled in with the textual
 * address.
 *
 * Returns:     error code
 *
 * Description:
 *
 ****************************************************************************/
VgUtv6adConvErr
vgUtv6adIpv4BinToText (const Int8 binAddr[],
                       Int8 textualAddrMaxSize,
                       Char *textualAddr)
{
  VgUtv6adConvErr err;

  err = VG_UTV6AD_ERR_OK;
  WarnCheck (textualAddrMaxSize >= VG_UTV6AD_IPV4_MAX_TEXT_LEN,
             textualAddrMaxSize, VG_UTV6AD_IPV4_MAX_TEXT_LEN, 0);
  if (textualAddrMaxSize < VG_UTV6AD_IPV4_MAX_TEXT_LEN)
  {
    err = VG_UTV6AD_ERR_BUFFER_TOO_SMALL;
  }
  else
  {
    snprintf ((char *)textualAddr,
             VG_UTV6AD_IPV4_MAX_TEXT_LEN,
             "%u.%u.%u.%u",
             (unsigned) binAddr[0],
             (unsigned) binAddr[1],
             (unsigned) binAddr[2],
             (unsigned) binAddr[3]);
  }
  return (err);
}

/****************************************************************************
 *
 * Function: vgUtv6adIpv6BinToColonHexText -- convert an ipv6 address to
 * textual form.
 *
 * Parameters: textualAddrMaxSize is the amount of space available at
 * textualAddr; textualAddr will be filled in with the textual
 * address.  
 *
 * If includeLeadingZeros, then leading zeros are included in an address
 * field.
 *
 * If compressZeros is true, then the longest sequence of
 * alternating : and 0 will be reduced to ::
 *
 * Returns: error code
 *
 * Description:
 *
 ****************************************************************************/
VgUtv6adConvErr
vgUtv6adIpv6BinToColonHexText (const Int8 binAddr[],
                            Int8 textualAddrMaxSize,
                            Char *textualAddr,
                            Boolean includeLeadingZeros,
                            Boolean compressZeros)
{
  VgUtv6adConvErr err;

  err = VG_UTV6AD_ERR_OK;
  WarnCheck (textualAddrMaxSize >= VG_UTV6AD_IPV6_MAX_TEXT_LEN,
             textualAddrMaxSize, VG_UTV6AD_IPV6_MAX_TEXT_LEN, 0);
  if (textualAddrMaxSize < VG_UTV6AD_IPV6_MAX_TEXT_LEN)
  {
    err = VG_UTV6AD_ERR_BUFFER_TOO_SMALL;
  }
  else
  {
    /* Check for the particular prefixes indicating ipv4 addresses
       embedded at the end.  It would seem very natural to write
       2002:192.168.33.1 for a 6to4 address, but people don't, and the
       rfc doesn't allow for it either -- so don't here, tho' we allow
       it optionally when converting from text.  Also, IPv4-compatible
       IPv6 addresses are obsolescent, so don't support them.  Note
       that if we did support such addresses, we'd need some
       special-case code below, to avoid compressing any leading 0 of
       the ipv4 address; because here we only generated ipv4 addresses
       preceded by ffff, compressing cannot affect such a leading 0. */
    if (memcmp (binAddr, "\0\0\0\0\0\0\0\0\0\0\xff\xff", 12u) == 0)
    {
      snprintf ((char *)textualAddr,
                VG_UTV6AD_IPV6_MAX_TEXT_LEN,
                "%x:%x:%x:%x:%x:%x:%u.%u.%u.%u",
                binAddr[0] << BITS_PER_INT8 | binAddr[1],
                binAddr[2] << BITS_PER_INT8 | binAddr[3],
                binAddr[4] << BITS_PER_INT8 | binAddr[5],
                binAddr[6] << BITS_PER_INT8 | binAddr[7],
                binAddr[8] << BITS_PER_INT8 | binAddr[9],
                binAddr[10] << BITS_PER_INT8 | binAddr[11],
                (unsigned) binAddr[12], (unsigned) binAddr[13],
                (unsigned) binAddr[14], (unsigned) binAddr[15]);
    }
    else
    {
      if (includeLeadingZeros)
      {
        /* Must include any leading zeros. */
        /* rfc 3513 uses only uppercase letters as hex digits so we will
         * use them here
         */
        snprintf ((char *)textualAddr,
                  VG_UTV6AD_IPV6_MAX_TEXT_LEN,
                  "%04X:%04X:%04X:%04X:%04X:%04X:%04X:%04X",
                  binAddr[0] << BITS_PER_INT8 | binAddr[1],
                  binAddr[2] << BITS_PER_INT8 | binAddr[3],
                  binAddr[4] << BITS_PER_INT8 | binAddr[5],
                  binAddr[6] << BITS_PER_INT8 | binAddr[7],
                  binAddr[8] << BITS_PER_INT8 | binAddr[9],
                  binAddr[10] << BITS_PER_INT8 | binAddr[11],
                  binAddr[12] << BITS_PER_INT8 | binAddr[13],
                  binAddr[14] << BITS_PER_INT8 | binAddr[15]);

        /* Compress the zeros. */
        if (compressZeros)
        {
          Int8 numZeroSets;
          Int8 maxZeroSets;
          Int8 zeroStrLen;
          Int8 maxZeroStrLen;
          Char *textualAddr_p;
          Char *zeroStartPointInTextualAddr_p;
          Char *tempTextualAddr_p;

          maxZeroSets = 0;
          maxZeroStrLen = 0;
          zeroStartPointInTextualAddr_p = textualAddr;
          
          /* We move the pointer on 5 characters at a time - as we have fixed each
           * display string to 4 bytes.  5 characters to take account of the ":".
           */
          for (textualAddr_p = textualAddr;
               (Int8)(textualAddr_p - textualAddr) < strlen((char *)textualAddr) ;
               textualAddr_p += 5)
          {
            tempTextualAddr_p = textualAddr_p;
            numZeroSets = 0;
            zeroStrLen = 0;

            /* Keep going for strings of 0000:0000: */            
            while ((*tempTextualAddr_p != '\0') &&
                   (strncmp((char *)tempTextualAddr_p, "0000", 4) == 0))
            {
              numZeroSets++;
              zeroStrLen+=4;

              if (*(tempTextualAddr_p + 4) == '\0')
              {
                /* This is the end of the line - so this is as long as the string
                 * of zeros is.
                 */
                tempTextualAddr_p += 4;
              }
              else
              {
                /* Must be a ':' character - so shift the pointer on 1 and add
                 * 1 to the overall string length
                 */
                tempTextualAddr_p += 5;
                zeroStrLen++;
              }                
            }

            /* Check if we got any more sets of zeros than last time */
            if (numZeroSets > maxZeroSets)
            {
              maxZeroSets = numZeroSets;
              zeroStartPointInTextualAddr_p = textualAddr_p;
              maxZeroStrLen = zeroStrLen;
            }

            textualAddr_p = tempTextualAddr_p;
          }
               
          if (maxZeroSets > 0)
          {
            /* We found some zero sets and we have the information for the
             * longest one.
             */
            if (*(zeroStartPointInTextualAddr_p + maxZeroStrLen) == '\0')
            {
              if (zeroStartPointInTextualAddr_p == textualAddr)
              {
                /* The whole string was full of 0's so replace it with
                 * "::\0"
                 * Length is 3 characters to cause "\0" at end of the string.
                 */
                strncpy ((char *)textualAddr, "::", 3);
              }
              else
              {
                /* The end of the string of '0's is the end of the whole string
                 * so just put a ":\0" in the right place
                 * Length is 2 characters to cause "\0" at end of the string.
                 */
                strncpy ((char *)zeroStartPointInTextualAddr_p, ":", 2);
              }              
            }
            else
            {
              /* We have a string of 0000's which ends somewhere in the middle
               * of the text string.
               */
              if (zeroStartPointInTextualAddr_p == textualAddr)
              {
                /* The string or 0's starts at the beginning.  Copy the data back to
                 * remove the 0's making sure to copy the \0 too.  Copy the data
                 * back offset by one to make room for the ":" at the beginning.
                 */
                memmove (&(zeroStartPointInTextualAddr_p [1]),
                         &(zeroStartPointInTextualAddr_p[maxZeroStrLen - 1]),
                         (strlen ((const char *)textualAddr) - maxZeroStrLen) + 2);
                *zeroStartPointInTextualAddr_p = ':';
              }
              else
              {
                /* The 0's are in the middle of the string somewhere.
                 * Copy the data back to remove the 0's making sure 
                 * to copy the \0 too.
                 */
                memmove (&(zeroStartPointInTextualAddr_p[0]),
                         &(zeroStartPointInTextualAddr_p[maxZeroStrLen - 1]),
                         (strlen ((const char *)textualAddr) - (zeroStartPointInTextualAddr_p - textualAddr) - maxZeroStrLen) + 2);
              }
            }
          }
        }
      }
      else
      {
        /* We must strip the leading zeros */
        snprintf ((char *)textualAddr,
                  VG_UTV6AD_IPV6_MAX_TEXT_LEN,
                  "%X:%X:%X:%X:%X:%X:%X:%X",
                  binAddr[0] << BITS_PER_INT8 | binAddr[1],
                  binAddr[2] << BITS_PER_INT8 | binAddr[3],
                  binAddr[4] << BITS_PER_INT8 | binAddr[5],
                  binAddr[6] << BITS_PER_INT8 | binAddr[7],
                  binAddr[8] << BITS_PER_INT8 | binAddr[9],
                  binAddr[10] << BITS_PER_INT8 | binAddr[11],
                  binAddr[12] << BITS_PER_INT8 | binAddr[13],
                  binAddr[14] << BITS_PER_INT8 | binAddr[15]);

        /* One might perhaps try compressing a run of zeros by looking at the
           binary form of the address, looking for successive zero byte
           pairs.  But this doesn't make things much simpler, if at all,
           since then the sprintf calls become messier.  So work on the
           textual form. */
        if (compressZeros)
        {
          Int8 maxZeroLen;
          Int8 zeroLen;
          Char *textualAddr_p;
          Char *maxZeroInTextualAddr_p;

          maxZeroLen = 0;
          maxZeroInTextualAddr_p = textualAddr;
          for (textualAddr_p = textualAddr;
               *textualAddr_p != '\0';
               ++textualAddr_p)
          {
            zeroLen = (Int8)strspn ((const char *)textualAddr_p, ":0");
            /* Look for a longer string of zeroes and colons than we've
               found so far.  The sequence of :0:0:... must start with :, or
               be at the start of the string. */
            if (zeroLen > maxZeroLen &&
                (*textualAddr_p == ':' || textualAddr_p == textualAddr))
            {
              maxZeroLen = zeroLen;
              maxZeroInTextualAddr_p = textualAddr_p;
            }
          }
#if defined (VG_UTV6AD_DEBUG)
          fprintf (stderr, "Compressing zeros in %s\n", textualAddr);
          fprintf (stderr, "                   %*s%.*s\n",
                   maxZeroInTextualAddr_p - textualAddr, "",
                   maxZeroLen, "******************************************");
#endif
          if (maxZeroLen > 2u)
          {
            Int8 oldLength = (Int8)strlen ((const char *)textualAddr);
#if defined (VG_UTV6AD_DEBUG)
            fprintf (stderr,
                     "moving to pos %u from pos %u, %u bytes\n",
                     maxZeroInTextualAddr_p + 1u - textualAddr,
                     maxZeroInTextualAddr_p + maxZeroLen - 1u - textualAddr,
                     textualAddr + oldLength + 2u -
                     (maxZeroInTextualAddr_p + maxZeroLen));
#endif
            /* Move the trailing piece over the zeroes-and-colons. */
            memmove (maxZeroInTextualAddr_p + 1u,
                     maxZeroInTextualAddr_p + maxZeroLen - 1u,
                     textualAddr + oldLength + 2u -
                     (maxZeroInTextualAddr_p + maxZeroLen));
            /* When the compressing is medial, we don't need the next
               memset, but we do need it if the compressing is initial or
               final, otherwise the string would incorrectly start with 0:
               or end with :0 (respectively). */
            memset (maxZeroInTextualAddr_p, ':', 2u);
          }
        }

      }
    }
  }
  return (err);
}

/****************************************************************************
 *
 * Function: vgUtv6adIpv6BinToDottedDecimalText -- convert an ipv6
 *           full address to the special 3gpp textual form.
 *
 * Parameters: textualAddrMaxSize is the amount of space available at
 *             textualAddr; textualAddr will be filled in with the textual
 *             address.
 *
 * Returns:     error code
 *
 * Description:
 *
 ****************************************************************************/
VgUtv6adConvErr
vgUtv6adIpv6BinToDottedDecimalText (const Int8 binAddr[],
                                Int8 textualAddrMaxSize,
                                Char *textualAddr)
{
  VgUtv6adConvErr err;

  err = VG_UTV6AD_ERR_OK;
  WarnCheck (textualAddrMaxSize >= VG_UTV6AD_IPV6_MAX_DOTTED_DECIMAL_TEXT_LEN,
             textualAddrMaxSize, VG_UTV6AD_IPV6_MAX_DOTTED_DECIMAL_TEXT_LEN, 0);
  if (textualAddrMaxSize < VG_UTV6AD_IPV6_MAX_DOTTED_DECIMAL_TEXT_LEN)
  {
    err = VG_UTV6AD_ERR_BUFFER_TOO_SMALL;
  }
  else
  {
    /* This form is used in some 3gpp AT commands.  I don't know why
       they avoided the conventional ipv6 form. */
    snprintf ((char *)textualAddr, 
             VG_UTV6AD_IPV6_MAX_DOTTED_DECIMAL_TEXT_LEN,
             "%u.%u.%u.%u.%u.%u.%u.%u.%u.%u.%u.%u.%u.%u.%u.%u",
             (unsigned) binAddr[0], (unsigned) binAddr[1],
             (unsigned) binAddr[2], (unsigned) binAddr[3],
             (unsigned) binAddr[4], (unsigned) binAddr[5],
             (unsigned) binAddr[6], (unsigned) binAddr[7],
             (unsigned) binAddr[8], (unsigned) binAddr[9],
             (unsigned) binAddr[10], (unsigned) binAddr[11],
             (unsigned) binAddr[12], (unsigned) binAddr[13],
             (unsigned) binAddr[14], (unsigned) binAddr[15]);
  }
  return (err);
}


#if defined (VG_UTV6AD_DEBUG) || defined (VG_UTV6AD_UNIT_TEST)
/* printBinAddr - output an address for debug */
static void
printBinAddr (const Int8 binAddr[VG_UTV6AD_IPV6_BIN_LEN])
{
  Char textualAddr[VG_UTV6AD_IPV6_MAX_TEXT_LEN] = {0};
  vgUtv6adIpv6BinToColonHexText (binAddr,
                                 VG_UTV6AD_IPV6_MAX_TEXT_LEN,
                                 textualAddr, 0, 0);
  printf ("%s", textualAddr);
}
#endif

#if defined (VG_UTV6AD_UNIT_TEST)

/* The remainder of this file is a standalone command-line program
   which tests the routines above */

static const char *ipAddrConvErrTexts[] =
{
  "OK",
  "MORE_THAN_ONE_DOUBLE_COLON",
  "IPV4_ADDRESS_OFF_END",
  "DOTTED_DECIMAL_OUTSIDE_IPV4",
  "IPV4_ERR_BYTE_VALUE_OVERFLOW",
  "IPV4_ERR_EMPTY_BYTE",
  "IPV4_ERR_BYTE_VALUE_TOO_LARGE",
  "BYTE_PAIR_TOO_LARGE",
  "V4_ADDRESS_TOO_SHORT_OR_BAD_CHARS",
  "V6_ADDRESS_TOO_SHORT_OR_BAD_CHARS",
  "BYTE_PAIR_VALUE_OVERFLOW",
  "LEADING_SINGLE_COLON",
  "TRAILING_SINGLE_COLON",
  "IPV4_EMBEDDED_NONFINALLY",
  "VG_UTV6AD_ERR_BUFFER_TOO_SMALL"
};

static void testIpv6Conv (void)
{
  Int8 binAddr[VG_UTV6AD_IPV6_BIN_LEN] = {0};
#if 0
  Int8 spare[VG_UTV6AD_IPV6_BIN_LEN] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
#endif
  Int8 repeatBinAddr[VG_UTV6AD_IPV6_BIN_LEN] = {0};
  Char textAddr[VG_UTV6AD_IPV6_MAX_TEXT_LEN] = {0};
  Int8 testCaseNum;
  VgUtv6adConvErr err;
  VgUtv6adConvErr err1;
  int ok;
  const struct {
    const Char *textAddr;
#if 0
    const Int8 binAddr[VG_UTV6AD_IPV6_BIN_LEN];
#endif
    int allow3gppDottedDecimal;
    int allowNonfinalEmbeddedIpv4;
    VgUtv6adConvErr err;
  }
  testCases[] =
  {
    /* valid cases */
#if 1
    { "1:2:3:4:5:6:7:8", 0, 1, VG_UTV6AD_ERR_OK },
    { "1:2:3:4:5::7:8", 0, 1, VG_UTV6AD_ERR_OK },
    /* rfc 1884 and rfc 3513 say `The use of "::" indicates multiple
       groups of 16-bits of zeros.'  It's not clear whether 0 is a
       valid multiplier.  Thus the next is treated as 1:2:3:4:5:6:7::
       with a trailing extraneous 8, rather than 1:2:3:4:5:6:7:8.
       There are three plausible interpretations of the rfc: `::'
       represents at least 0, or 1, or 2 groups of 16-bits of
       zeros. */
    { "1:2:3:4:5:6:7::8", 0, 1, VG_UTV6AD_ERR_OK },
    /* In the next I treat the :: as one pair of zero bytes, and the
       :8 as extraneous. */
    { "1::2:3:4:5:6:7:8", 0, 1, VG_UTV6AD_ERR_OK },
#endif
    { "1:0:0:4:0:0:0:8", 0, 1, VG_UTV6AD_ERR_OK },
#if 1
    { "1::4:0:0:0:8",
#if 0
      {0,1,0,0,0,0,0,4,0,0,0,0,0,0,0,8 },
#endif
      0, 1, VG_UTV6AD_ERR_OK },
    { "1:0:0:4::8", 0, 1, VG_UTV6AD_ERR_OK },
    { "fe80::", 0, 1, VG_UTV6AD_ERR_OK },
    { "FE80::", 0, 1, VG_UTV6AD_ERR_OK },
    { "::1234", 0, 1, VG_UTV6AD_ERR_OK },
    { "fe80::1234", 0, 1, VG_UTV6AD_ERR_OK },
    { "::", 0, 1, VG_UTV6AD_ERR_OK },
    { "::1", 0, 1, VG_UTV6AD_ERR_OK },
    { "2002::1234:abcd", 0, 1, VG_UTV6AD_ERR_OK },
    { "a:b:c:d:e:f::", 0, 1, VG_UTV6AD_ERR_OK },
    { "1.2.3.4::", 0, 1, VG_UTV6AD_ERR_OK },
    { "1.2.3.4::", 0, 0, VG_UTV6AD_ERR_IPV4_EMBEDDED_NONFINALLY },
    { "::1.2.3.4", 0, 1, VG_UTV6AD_ERR_OK },
    { "::1.2.3.4", 0, 0, VG_UTV6AD_ERR_OK },
    { "1:2::7:8", 0, 0, VG_UTV6AD_ERR_OK },
    { "abc:1.2.3.4::fe", 0, 1, VG_UTV6AD_ERR_OK },
    { "abc::1.2.3.4:fe", 0, 1, VG_UTV6AD_ERR_OK },
    { "1.2.3.4.5.6.7.8.9.0.10.11.12.13.14.15", 1, 1, VG_UTV6AD_ERR_OK },
    { "255.254.253.252:251.250.249.248:248.249.250.251:252.253.254.255", 0, 1, VG_UTV6AD_ERR_OK },
    { "::1", 0, 1, VG_UTV6AD_ERR_OK },
    { "2002:192.88.99.1::", 0, 1, VG_UTV6AD_ERR_OK },
    { "2002:192.88.99.1::", 0, 0, VG_UTV6AD_ERR_IPV4_EMBEDDED_NONFINALLY },
    /* some edge and invalid cases */
    { "::1:2.3.4.5.6", 0, 1, VG_UTV6AD_ERR_DOTTED_DECIMAL_OUTSIDE_IPV4 },
    /* not clear about the next. */
    { "::1:2.3.4.5.6", 1, 1, VG_UTV6AD_ERR_OK },
    /* Is the next an error? */
    { ":2:3:4:5:6:7:8", 0, 1, VG_UTV6AD_ERR_LEADING_SINGLE_COLON },
    /* Is the next an error? */
    { "1:2:3:4:5:6:7:", 0, 1, VG_UTV6AD_ERR_TRAILING_SINGLE_COLON },
    { "1:2:3:4:5:6:7:z", 0, 1, VG_UTV6AD_ERR_TRAILING_SINGLE_COLON },
    { ":", 0, 1, VG_UTV6AD_ERR_LEADING_SINGLE_COLON },
    { ".", 0, 1, VG_UTV6AD_ERR_IPV4_ERR_EMPTY_BYTE },
    { ".", 0, 0, VG_UTV6AD_ERR_IPV4_EMBEDDED_NONFINALLY },
    { "::1:256.3.4.5.6", 1, 1, VG_UTV6AD_ERR_IPV4_ERR_BYTE_VALUE_TOO_LARGE },
    { "255.254.253.252.251.250.249.256.248.249.250.251.252.253.254.255", 1, 1, VG_UTV6AD_ERR_IPV4_ERR_BYTE_VALUE_TOO_LARGE },
    { "sdfiugfduigfdihugldf", 0, 1, VG_UTV6AD_ERR_IPV6_ADDRESS_TOO_SHORT_OR_BAD_CHARS },
    { "::-5:-6:-7:-8", 0, 1, VG_UTV6AD_ERR_BYTE_PAIR_TOO_LARGE },
    { "-5.-6.-7.-8::", 0, 1, VG_UTV6AD_ERR_IPV4_ERR_BYTE_VALUE_TOO_LARGE },
    { "::1junk", 0, 1, VG_UTV6AD_ERR_OK },
    { "1::junk", 0, 1, VG_UTV6AD_ERR_OK },
    { "1::2junk", 0, 1, VG_UTV6AD_ERR_OK },
    { "::1::2", 0, 1, VG_UTV6AD_ERR_MORE_THAN_ONE_DOUBLE_COLON },
    { ":1::2::", 0, 1, VG_UTV6AD_ERR_LEADING_SINGLE_COLON },
    { "1::2::", 0, 1, VG_UTV6AD_ERR_MORE_THAN_ONE_DOUBLE_COLON },
    { "::1:2.3.456", 0, 1, VG_UTV6AD_ERR_IPV4_ERR_BYTE_VALUE_TOO_LARGE },
    { "::1:2.3.256", 0, 1, VG_UTV6AD_ERR_IPV4_ERR_BYTE_VALUE_TOO_LARGE },
    { "::1:23456", 0, 1, VG_UTV6AD_ERR_BYTE_PAIR_TOO_LARGE },
    { "::1:ffff", 0, 1, VG_UTV6AD_ERR_OK },
    { "::ffff:1.2.3.4", 0, 1, VG_UTV6AD_ERR_OK },
    { "::1.2.3.4", 0, 1, VG_UTV6AD_ERR_OK },
    /* FIXME: in the next, the 0 is discarded on reconversion */
    { "::0.2.3.4", 0, 1, VG_UTV6AD_ERR_OK },
    { "::.2.3.4", 0, 1, VG_UTV6AD_ERR_IPV4_ERR_EMPTY_BYTE },
    { "::1:10000", 0, 1, VG_UTV6AD_ERR_BYTE_PAIR_TOO_LARGE },
    { "1:2", 0, 1, VG_UTV6AD_ERR_IPV6_ADDRESS_TOO_SHORT_OR_BAD_CHARS },
    { "1:2:3:4:5:6:7:8.9.10.11", 0, 1, VG_UTV6AD_ERR_IPV4_ADDRESS_OFF_END },
    { "1:2:3:4:5:6:..10.11", 0, 1, VG_UTV6AD_ERR_IPV4_ERR_EMPTY_BYTE },
    { "1:::8", 0, 1, VG_UTV6AD_ERR_MORE_THAN_ONE_DOUBLE_COLON },
    { "::1:10000000000000000000000000000000000000000000000000000", 0, 1, VG_UTV6AD_ERR_BYTE_PAIR_VALUE_OVERFLOW },
    { "1.10000000000000000000000000000000000000000000000000000.3.4::", 0, 1, VG_UTV6AD_ERR_IPV4_ERR_BYTE_VALUE_OVERFLOW },
    { "::1:1ffffffffffffffff", 0, 1, VG_UTV6AD_ERR_BYTE_PAIR_VALUE_OVERFLOW },
    { "::..............:::::::::::::::::::::::::", 0, 1, VG_UTV6AD_ERR_IPV4_ERR_EMPTY_BYTE },
    /* Note that \x9C is the character code for British Pound and \x24 for US Dollar Symbol. */
    { "-!\"\x9C\x24%^&*(){}:@~[];'#,.<>?/", 0, 1, VG_UTV6AD_ERR_IPV6_ADDRESS_TOO_SHORT_OR_BAD_CHARS },
    { "ffff:FFFF:eeee:EEEE:ffff:FFFF:eeee:EEEE:ffff:FFFF:eeee:EEEE", 0, 1, VG_UTV6AD_ERR_OK },
    { ":ffff:FFFF:eeee:EEEE:ffff:FFFF:eeee:EEEE:ffff:FFFF:eeee:EEEE", 0, 1, VG_UTV6AD_ERR_LEADING_SINGLE_COLON },
    { "\r\t\a\f\xab\v\b\n", 0, 1, VG_UTV6AD_ERR_IPV6_ADDRESS_TOO_SHORT_OR_BAD_CHARS },
    { "", 0, 1, VG_UTV6AD_ERR_IPV6_ADDRESS_TOO_SHORT_OR_BAD_CHARS },
#endif
  };

  for (testCaseNum = 0;
       testCaseNum < sizeof (testCases)/sizeof (testCases[0]);
       ++testCaseNum)
  {
    Char *endptr;

    memset (binAddr, 0, VG_UTV6AD_IPV6_BIN_LEN);
    err = vgUtv6adIpv6TextToBin (testCases[testCaseNum].textAddr,
                                 &endptr,
                                 binAddr,
                                 testCases[testCaseNum].allow3gppDottedDecimal,
                                 testCases[testCaseNum].allowNonfinalEmbeddedIpv4);
#if 0
    printBinAddr (testCases[testCaseNum].binAddr);
    fputs ("->", stdout);
    printBinAddr (binAddr);
    fputc ('\n', stdout);
    assert (memcmp (binAddr, testCases[testCaseNum].binAddr, VG_UTV6AD_IPV6_BIN_LEN) == 0);
#endif
    memset (textAddr, 0, VG_UTV6AD_IPV6_MAX_TEXT_LEN);
    err1 = vgUtv6adIpv6BinToColonHexText (binAddr, VG_UTV6AD_IPV6_MAX_TEXT_LEN, textAddr, 0, 1);
    if (err1 != VG_UTV6AD_ERR_OK)
    {
      printf ("warn: bin to text fails (%u %s) for %s\n",
              err1,
              ipAddrConvErrTexts [err1],
              testCases[testCaseNum].textAddr);
    }
     printf ("%s: ", err == testCases[testCaseNum].err ? "PASS" : "FAIL");
    if (err == VG_UTV6AD_ERR_OK)
    {
      printf ("%-40s %s\n",
              testCases[testCaseNum].textAddr, textAddr);
    }
    else
    {
      printf ("%-40s -- invalid (%u %s) at %c\n      %*s^\n",
              testCases[testCaseNum].textAddr,
              err,
              ipAddrConvErrTexts [err],
              *endptr == '\0' ? '$' : *endptr,
              endptr - testCases[testCaseNum].textAddr, "");
    }
#if 0
    fputs ("BB: ", stdout);
    printBinAddr (binAddr);
    fputc ('\n', stdout);
#endif
    if (err == VG_UTV6AD_ERR_OK)
    {
      memset (repeatBinAddr, 0, VG_UTV6AD_IPV6_BIN_LEN);
#if 0
      fputs ("BC: ", stdout);
      printBinAddr (binAddr);
      fputc ('\n', stdout);
#if 0
      fputs ("GD: ", stdout);
      printBinAddr (spare);
      fputc ('\n', stdout);
#endif
#endif
      err = vgUtv6adIpv6TextToBin (textAddr, &endptr, repeatBinAddr, 0, 1);
    }
#if 0
    fputs ("GD: ", stdout);
    printBinAddr (spare);
    fputc ('\n', stdout);
    fputs ("CC: ", stdout);
    printBinAddr (binAddr);
    fputc ('\n', stdout);
#endif
    if (err == VG_UTV6AD_ERR_OK)
    {
      ok = memcmp (binAddr, repeatBinAddr, VG_UTV6AD_IPV6_BIN_LEN) == 0;
      if (ok)
      {
        printf ("PASS: reconversion to binary for %s\n",
                testCases[testCaseNum].textAddr);
      }
      else
      {
        printf ("FAIL: reconversion to binary fails for %s ",
                testCases[testCaseNum].textAddr);
        printBinAddr (binAddr);
        fputs ("->", stdout);
        printBinAddr (repeatBinAddr);
        fputc ('\n', stdout);
      }
    }
    if (err == VG_UTV6AD_ERR_OK)
    {
      Char dottyDecimalText[VG_UTV6AD_IPV6_MAX_DOTTED_DECIMAL_TEXT_LEN] = {0};
      Int8 repeatBisBinAddr[VG_UTV6AD_IPV6_BIN_LEN] = {0};
      /* check the dotty decimal conversion */
      err1 =
        vgUtv6adIpv6BinToDottedDecimalText (binAddr,
                                            VG_UTV6AD_IPV6_MAX_DOTTED_DECIMAL_TEXT_LEN,
                                            dottyDecimalText);
      if (err1 != VG_UTV6AD_ERR_OK)
      {
        printf ("warn: dotty conversion to text fails (%u %s) for %s\n",
                err1,
                ipAddrConvErrTexts [err1],
                dottyDecimalText);
      }
      err = vgUtv6adIpv6TextToBin (dottyDecimalText, &endptr, repeatBisBinAddr, 1, 1);
      ok = memcmp (binAddr, repeatBisBinAddr, VG_UTV6AD_IPV6_BIN_LEN) == 0 &&
        *endptr == '\0' &&
        err == VG_UTV6AD_ERR_OK;
      if (ok)
      {
        printf ("PASS: dotty reconversion to binary for %s\n",
                dottyDecimalText);
      }
      else
      {
        printf ("FAIL: dotty reconversion to binary fails (%u %s) for %s ",
                err,
                ipAddrConvErrTexts [err],
                dottyDecimalText);
        printBinAddr (binAddr);
        fputs ("->", stdout);
        printBinAddr (repeatBisBinAddr);
        fputc ('\n', stdout);
      }
    }
    fflush (stdout);
  }
}

static void testIpv4Conv (void)
{
  Int8 binAddr[VG_UTV6AD_IPV4_BIN_LEN] = {0};
  Int8 repeatBinAddr[VG_UTV6AD_IPV4_BIN_LEN] = {0};
  Char textAddr[VG_UTV6AD_IPV4_MAX_TEXT_LEN] = {0};
  Int8 testCaseNum;
  VgUtv6adConvErr err;
  VgUtv6adConvErr err1;
  int ok;
  const struct {
    const char *textAddr;
    VgUtv6adConvErr err;
  }
  testCases[] =
  {
    /* valid cases */
    { "0.0.0.0", VG_UTV6AD_ERR_OK },
    { "127.0.0.1", VG_UTV6AD_ERR_OK },
    { "1.2.3.4", VG_UTV6AD_ERR_OK },
    { "255.255.255.255", VG_UTV6AD_ERR_OK },
    /* some edge and invalid cases */
    { "...", VG_UTV6AD_ERR_IPV4_ERR_EMPTY_BYTE },
    { "1.2.3.4.5", VG_UTV6AD_ERR_OK },
    { "1.2.3.4junk", VG_UTV6AD_ERR_OK },
    { "1.2.3junk", VG_UTV6AD_ERR_IPV4_ADDRESS_TOO_SHORT_OR_BAD_CHARS },
    { "1.2.3.456", VG_UTV6AD_ERR_IPV4_ERR_BYTE_VALUE_TOO_LARGE },
    { "1.2.3.10000000000000000000000000000000", VG_UTV6AD_ERR_IPV4_ERR_BYTE_VALUE_OVERFLOW },
    { ".10000000000000000000000000000000", VG_UTV6AD_ERR_IPV4_ERR_EMPTY_BYTE },
    { "$%^%%$", VG_UTV6AD_ERR_IPV4_ADDRESS_TOO_SHORT_OR_BAD_CHARS },
    { "", VG_UTV6AD_ERR_IPV4_ADDRESS_TOO_SHORT_OR_BAD_CHARS },
  };

  for (testCaseNum = 0;
       testCaseNum < sizeof (testCases)/sizeof (testCases[0]);
       ++testCaseNum)
  {
    Char *endptr;

    memset (binAddr, 0, VG_UTV6AD_IPV4_BIN_LEN);
    /* Convert text to bin */
    err = vgUtv6adIpv4TextToBin (testCases[testCaseNum].textAddr,
                                 &endptr,
                                 binAddr);
    memset (textAddr, 0, VG_UTV6AD_IPV4_MAX_TEXT_LEN);
    /* and then the bin back to text */
    err1 = vgUtv6adIpv4BinToText (binAddr, VG_UTV6AD_IPV4_MAX_TEXT_LEN, textAddr);
    if (err1 != VG_UTV6AD_ERR_OK)
    {
      printf ("warn: bin to text fails (%u %s) for %s\n",
              err1,
              ipAddrConvErrTexts [err1],
              testCases[testCaseNum].textAddr);
    }
    printf ("%s: ", err == testCases[testCaseNum].err ? "PASS" : "FAIL");
    if (err == VG_UTV6AD_ERR_OK)
    {
      printf ("%-40s %s\n",
              testCases[testCaseNum].textAddr, textAddr);
    }
    else
    {
      printf ("%-40s -- invalid (%u %s) at %c\n      %*s^\n",
              testCases[testCaseNum].textAddr,
              err,
              ipAddrConvErrTexts [err],
              *endptr == '\0' ? '$' : *endptr,
              endptr - testCases[testCaseNum].textAddr, "");
    }
    /* For the valid textual forms, convert to binary again, and check
       we got the same as the first time. */
    if (err == VG_UTV6AD_ERR_OK)
    {
      memset (repeatBinAddr, 0, VG_UTV6AD_IPV4_BIN_LEN);
      err = vgUtv6adIpv4TextToBin (textAddr, &endptr, repeatBinAddr);
      if (err == VG_UTV6AD_ERR_OK)
      {
        ok = memcmp (binAddr, repeatBinAddr, VG_UTV6AD_IPV4_BIN_LEN) == 0;
        if (ok)
        {
          printf ("PASS: reconversion to binary for %s\n",
                  testCases[testCaseNum].textAddr);
        }
        else
        {
          printf ("FAIL: reconversion to binary, cmp fails for %s",
                  testCases[testCaseNum].textAddr);
          printBinAddr (binAddr);
          fputs ("->", stdout);
          printBinAddr (repeatBinAddr);
          fputc ('\n', stdout);
        }
      }
      else
      {
#if 0
        printf ("warn: comparison not tested (%u %s) for invalid %s\n",
                err,
                ipAddrConvErrTexts [err],
                testCases[testCaseNum].textAddr);
#endif
      }
    }
    else
    {
#if 0
      printf ("warn: reconversion not tested (%u %s) for invalid %s\n",
              err,
              ipAddrConvErrTexts [err],
              testCases[testCaseNum].textAddr);
#endif
    }
    fflush (stdout);
  }
}

int main (void)
{
  testIpv6Conv ();
  testIpv4Conv ();
  return 0;
}

/* Local Variables: */
/* compile-command: "gcc -DVG_UTV6AD_UNIT_TEST -DUPGRADE_GPRS -I../../../../../sys/sys.typ/api/inc -I../../api/shinc -I../../../../../sys/sys.typ/api/shinc -I../../api/inc -I. -Wall -Werror -ansi -pedantic -g -O -Wall -Wshadow -Wpointer-arith -Wbad-function-cast -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Wcast-align -Wstrict-prototypes -Wwrite-strings -fno-guess-branch-probability rvutv6ad.c && a.exe" */
/* c-basic-offset: 2 */
/* End: */
#endif

/* END OF FILE */



