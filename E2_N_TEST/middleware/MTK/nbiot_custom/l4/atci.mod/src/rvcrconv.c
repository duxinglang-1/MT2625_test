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
 * Implements handling of character set conversion routines.
 **************************************************************************/

#define MODULE_NAME "RVCRCONV"


/***************************************************************************
 * Include Files
 ***************************************************************************/

#if !defined (SYSTEM_H)
#  include <system.h>
#endif
#if !defined (RVUTIL_H)
#  include <rvutil.h>
#endif
#if !defined (RVDATA_H)
#  include <rvdata.h>
#endif
#if !defined (RVPROF_H)
#  include <rvprof.h>
#endif
#if !defined (RVCRHAND_H)
#  include <rvcrhand.h>
#endif
#if !defined(CICI_SIG_H)
#  include <cici_sig.h>
#endif
#if !defined (RVCRMAN_H)
#  include <rvcrman.h>
#endif
#if !defined (RVCRCONV_H)
#  include <rvcrconv.h>
#endif
#if !defined (UTUC_FNC_H)
#  include <utuc_fnc.h>
#endif

/***************************************************************************
 * Type Definitions
 ***************************************************************************/

typedef struct CodePagesStructTag
{
  VgCSCSMode identifier;    /* Character set enumerate                  */
  Int8       maxValue;      /* Maximum character value of character set */
  const Char *gsmMapping;   /* Conversion map from GSM alphabet         */
} CodePagesStruct;

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

static Boolean findCodePage (const VgCSCSMode codePage,
                              CodePagesStruct **cpConversion_p);
static Boolean mapCharToGsm(Int8 *gsmChar,
                            const Int8 teChar,
                            const CodePagesStruct *cpConversion_p);
static Boolean mapCharFromGsm(Int8 *teChar,
                              const Int8 gsmChar,
                              const CodePagesStruct *cpConversion_p);

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/* Conversion maps from GSM covering all characters */
static const Char TAB_8859_1_TO_GSM[] =
{
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x0A, 0x80, 0x80, 0x0D, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x20, 0x21, 0x22, 0x23, 0x02, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
  0x00, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
  0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x80, 0x80, 0x80, 0x80, 0x11,
  0x80, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
  0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0x40, 0x80, 0x01, 0x24, 0x03, 0x80, 0x5F, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x60,
  0x41, 0x41, 0x41, 0x41, 0x5B, 0x0E, 0x1C, 0x09, 0x45, 0x1F, 0x45, 0x45, 0x49, 0x49, 0x49, 0x49,
  0x80, 0x5D, 0x4F, 0x4F, 0x4F, 0x4F, 0x5C, 0x80, 0x0B, 0x55, 0x55, 0x55, 0x5E, 0x59, 0x80, 0x1E,
  0x7F, 0x61, 0x61, 0x61, 0x7B, 0x0F, 0x1D, 0x09, 0x04, 0x05, 0x65, 0x65, 0x07, 0x69, 0x69, 0x69,
  0x80, 0x7D, 0x08, 0x6F, 0x6F, 0x6F, 0x7C, 0x80, 0x0C, 0x60, 0x75, 0x75, 0x7E, 0x79, 0x80, 0x79
};

static const Char TAB_IRA_TO_GSM[] =
{
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x0A, 0x80, 0x80, 0x0D, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x20, 0x21, 0x22, 0x23, 0x02, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
  0x00, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
  0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x80, 0x80, 0x80, 0x80, 0x11,
  0x80, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
  0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x80, 0x80, 0x80, 0x80, 0x80
};

static const Char TAB_PCCP437_TO_GSM[] =
{
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x0A, 0x80, 0x80, 0x0D, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0x5F, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x20, 0x21, 0x22, 0x23, 0x02, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
  0x00, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
  0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x80, 0x80, 0x80, 0x80, 0x11,
  0x80, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
  0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x09, 0x7E, 0x05, 0x61, 0x7B, 0x7F, 0x0F, 0x09, 0x65, 0x65, 0x04, 0x69, 0x69, 0x07, 0x5B, 0x0E,
  0x1F, 0x1D, 0x1C, 0x6F, 0x7C, 0x08, 0x75, 0x06, 0x79, 0x5C, 0x5E, 0x80, 0x01, 0x03, 0x80, 0x80,
  0x61, 0x69, 0x6F, 0x75, 0x7D, 0x5D, 0x80, 0x80, 0x60, 0x80, 0x80, 0x80, 0x80, 0x40, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0x1E, 0x13, 0x80, 0x18, 0x80, 0x80, 0x80, 0x12, 0x19, 0x15, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80
};

static const Char TAB_PCDN_TO_GSM[] =
{
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x0A, 0x80, 0x80, 0x0D, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0x5F, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x20, 0x21, 0x22, 0x23, 0x02, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
  0x00, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
  0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x80, 0x80, 0x80, 0x80, 0x11,
  0x80, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
  0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x09, 0x7E, 0x05, 0x61, 0x7B, 0x7F, 0x0F, 0x09, 0x65, 0x65, 0x04, 0x69, 0x69, 0x07, 0x5B, 0x0E,
  0x1F, 0x1D, 0x1C, 0x6F, 0x7C, 0x08, 0x75, 0x06, 0x79, 0x5C, 0x5E, 0x0C, 0x01, 0x0B, 0x80, 0x80,
  0x61, 0x69, 0x6F, 0x75, 0x7D, 0x5D, 0x80, 0x80, 0x60, 0x80, 0x80, 0x80, 0x80, 0x40, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0x1E, 0x13, 0x80, 0x18, 0x80, 0x80, 0x80, 0x12, 0x19, 0x15, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80
};

/* Matches CSCS character map enumerate to conversion information */

static const CodePagesStruct cpConversion[] =
{
  { VG_AT_CSCS_IRA,    127, TAB_IRA_TO_GSM      },
  { VG_AT_CSCS_PCCP,   255, TAB_PCCP437_TO_GSM  },
  { VG_AT_CSCS_PCDN,   255, TAB_PCDN_TO_GSM     },
  { VG_AT_CSCS_8859_1, 255, TAB_8859_1_TO_GSM   }
};

/***************************************************************************
 * Variables
 ***************************************************************************/

/***************************************************************************
 * Macros
 ***************************************************************************/

/***************************************************************************
 * Local Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
 *
 * Function:        findCodePage
 *
 * Parameters:      codepage        - CSCS to get code table for.
 *                  cpConversion_p  - pointer to code table
 *
 * Returns:         Boolean - TRUE if lookup table found, false otherwise.
 *
 * Description:     Finds a lookup table for a specific cscs set.
 *-------------------------------------------------------------------------*/
static Boolean findCodePage (const VgCSCSMode codePage,
                              CodePagesStruct **cpConversion_p)
{
  Int8             index;
  Boolean          cpWasFound = FALSE;
  CodePagesStruct  *currentCodePage = (CodePagesStruct *)cpConversion;

  /* Loop through available character mappings to find match.... */
  for (index = 0; (index < VG_AT_CSCS_MAX_VAL && cpWasFound == FALSE); index++)
  {
    if (currentCodePage->identifier == codePage)
    {
      cpWasFound = TRUE;
    }
    else
    {
      if (index < (VG_AT_CSCS_MAX_VAL - 1))
      {
        currentCodePage++;
      }
    }
  }

  /* If map is found modify code page pointer accordingly.... */
  if (cpWasFound == TRUE)
  {
    *cpConversion_p = currentCodePage;
  }

  return (cpWasFound);
}

/*--------------------------------------------------------------------------
 *
 * Function:        mapCharToGsm
 *
 * Parameters:      gsmChar,
 *                  teChar,
 *                  cpConversion_p
 *
 * Returns:         Boolean - TRUE if conversion ok, FALSE if no match.
 *
 * Description:     Converts a character of the given code set to GSM.
 *-------------------------------------------------------------------------*/
static Boolean mapCharToGsm( Int8 *gsmChar,
                             const Int8 teChar,
                             const CodePagesStruct *cpConversion_p)
{
  Boolean result = TRUE;

  /* If input character within range of CH set convert, else no conversion
   * possible....
   */
  if (teChar < cpConversion_p->maxValue)
  {
    *gsmChar = cpConversion_p->gsmMapping[teChar];
  }

  else
  {
    result = FALSE;
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:        mapCharFromGsm
 *
 * Parameters:      gsmChar,
 *                  teChar,
 *                  cpConversion_p
 *
 * Returns:         Boolean - TRUE if conversion ok, FALSE if no match.
 *
 * Description:     Converts a GSM character to the given code set.
 *-------------------------------------------------------------------------*/
static Boolean mapCharFromGsm( Int8 *teChar,
                               const Int8 gsmChar,
                               const CodePagesStruct *cpConversion_p)
{
  Boolean found = FALSE;
  Int8    index = 0;

  /* Find correct CH mapping for this character.... */
  while ((index < cpConversion_p->maxValue) && (found == FALSE))
  {
    if (cpConversion_p->gsmMapping[index] == gsmChar)
    {
      *teChar = index;
      found = TRUE;
    }
    else
    {
      index++;
    }
  }

  return (found);
}

/***************************************************************************
 * Global Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
 *
 * Function:        vgMapTEToGsm
 *
 * Parameters:      outputBuffer_p
 *                  inputBuffer_p
 *                  inputBufferLen
 *                  cscsMode
 *                  entity
 *
 * Returns:         Int16 - length of converted data stored in buffer
 *
 * Description:     Converts an input string from its current character set
 *                  to the GSM charater set.
 *-------------------------------------------------------------------------*/
Int16 vgMapTEToGsm(Int8                     *outputBuffer_p,
                   const Int16              outputBufferLen,
                   const Int8               *inputBuffer_p,
                   const Int16              inputBufferLen,
                   const VgCSCSMode         cscsMode,
                   const VgmuxChannelNumber entity)
{
  Int16               outputLen = 0,
                      index,
                      *ucs2Data_p,
                      wordsConverted = 0;
  Int8                convChar;
  Int8                inputNumber[VG_CR_SYMBOL_LENGTH_UCS2 + NULL_TERMINATOR_LENGTH] = {0};
  Boolean             gsmStringOverflowed = FALSE;
  CodePagesStruct     *cpConversion_p = (CodePagesStruct *)cpConversion;

  FatalAssert(PNULL != outputBuffer_p);
  FatalAssert(PNULL != inputBuffer_p);

  if ( (PNULL != outputBuffer_p) && (PNULL != inputBuffer_p) )
  {
    switch (cscsMode)
    {
      case VG_AT_CSCS_GSM: /* GSM to GSM - no conversion required */
      {
        memcpy (outputBuffer_p, inputBuffer_p, inputBufferLen);
        outputLen = inputBufferLen;
        break;
      }

      case VG_AT_CSCS_HEX: /* HEX to GSM */
      {
        /* converts each character from two byte hexidecimal pair (0-F,0-F) */
        for (index = 0; index < inputBufferLen; index += VG_CR_SYMBOL_LENGTH_HEX)
        {
          memcpy(inputNumber, &inputBuffer_p[index], VG_CR_SYMBOL_LENGTH_HEX);
          inputNumber[VG_CR_SYMBOL_LENGTH_HEX] = '\0';
          outputBuffer_p[outputLen] = (Int8)vgAsciiToInt16(inputNumber);
          outputLen++;
        }
        break;
      }

      case VG_AT_CSCS_UCS2: /* UCS2 to GSM*/
      {
        /* Allocate enough memory to hold UCS2 words.... */
        ucs2Data_p = (Int16 *)getCrConversionBuffer(entity);

        /* Convert to ASCII to UCS2.... */
        if (ucs2Data_p != PNULL)
        {
          for (index = 0; index < inputBufferLen; index += VG_CR_SYMBOL_LENGTH_UCS2)
          {
            memcpy(inputNumber, &inputBuffer_p[index], VG_CR_SYMBOL_LENGTH_UCS2);
            inputNumber[VG_CR_SYMBOL_LENGTH_UCS2] = '\0';
            ucs2Data_p[wordsConverted] = vgAsciiToInt16(inputNumber);
            wordsConverted++;
          }

          FatalAssert((wordsConverted*2) <= VG_CR_CH_SET_CONV_BUFFER_LEN);

          /* Convert USC2 to GSM.... */
          utUcs2ToByteAlignedEGsm7Bit (ucs2Data_p,
                                        wordsConverted,
                                         outputBuffer_p,
                                          &outputLen,
                                           outputBufferLen,
                                            &gsmStringOverflowed);

          FatalAssert(gsmStringOverflowed == FALSE);
        }
        else
        {
          FatalParam(entity, 0, 0);
        }
        break;
      }

      case VG_AT_CSCS_IRA:    /* ??? to GSM - try character mapping tables */
      case VG_AT_CSCS_PCCP:
      case VG_AT_CSCS_PCDN:
      case VG_AT_CSCS_8859_1:
      {
        /* Find conversion routine.... */
        if (findCodePage (cscsMode, &cpConversion_p) == TRUE)
        {
          for (index = 0; index < inputBufferLen; index++)
          {
            if (mapCharToGsm(&convChar, inputBuffer_p[index], cpConversion_p) == TRUE)
            {
              outputBuffer_p[index] = convChar;
            }

            else
            {
              outputBuffer_p[index] = GSM_CHARACTER_SET_SIZE;
            }
          }
          outputLen = inputBufferLen;
        }
        else
        {
          /* if no conversion possible return input string */
          WarnParam(entity, cscsMode, 0);
          memcpy (outputBuffer_p, inputBuffer_p, inputBufferLen);
          outputLen = inputBufferLen;
        }
        break;
      }

      default:
      {
        /* if no conversion possible return input string */
        WarnParam(entity, cscsMode, 0);
        memcpy (outputBuffer_p, inputBuffer_p, inputBufferLen);
        outputLen = inputBufferLen;
        break;
      }
    }

    FatalAssert(outputLen <= outputBufferLen);
  }

  return (outputLen);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgMapTEToHex
 *
 * Parameters:      outputBuffer_p
 *                  inputBuffer_p
 *                  inputBufferLen
 *                  cscsMode
 *                  entity
 *
 * Returns:         Int16 - length of converted data stored in buffer
 *
 * Description:     Converts an input string from its current character set
 *                  into HEX.
 *-------------------------------------------------------------------------*/
Int16 vgMapTEToHex(Int8                     *outputBuffer_p,
                   const Int16              outputBufferLen,
                   const Int8               *inputBuffer_p,
                   const Int16              inputBufferLen,
                   const VgCSCSMode         cscsMode)
{
  Int16               outputLen = 0,
                      index;
  Int8                inputNumber[VG_CR_SYMBOL_LENGTH_HEX + NULL_TERMINATOR_LENGTH] = {0};

  PARAMETER_NOT_USED(outputBufferLen);

  FatalAssert(PNULL != outputBuffer_p);
  FatalAssert(PNULL != inputBuffer_p);

  if ( (PNULL != outputBuffer_p) && (PNULL != inputBuffer_p) )
  {
    switch (cscsMode)
    {
      case VG_AT_CSCS_UCS2: /* UCS2 to HEX */
      case VG_AT_CSCS_HEX:  /* HEX to HEX */
      {
        /* converts each two byte hexidecimal pair (0-F,0-F) to single byte */
        for (index = 0; index < inputBufferLen; index += VG_CR_SYMBOL_LENGTH_HEX)
        {
          memcpy(inputNumber, &inputBuffer_p[index], VG_CR_SYMBOL_LENGTH_HEX);
          inputNumber[VG_CR_SYMBOL_LENGTH_HEX] = '\0';
          outputBuffer_p[outputLen] = (Int8)vgAsciiToInt16(inputNumber);
          outputLen++;
        }
        break;
      }

      case VG_AT_CSCS_GSM:  /* GSM to HEX - no conversion required */
      case VG_AT_CSCS_IRA:  /* ??? to HEX - try character mapping tables */
      case VG_AT_CSCS_PCCP:
      case VG_AT_CSCS_PCDN:
      case VG_AT_CSCS_8859_1:
      {
        memcpy (outputBuffer_p, inputBuffer_p, inputBufferLen);
        outputLen = inputBufferLen;
        break;
      }

      default:
      {
        WarnParam(cscsMode, 0, 0); 
        memcpy (outputBuffer_p, inputBuffer_p, inputBufferLen);
        outputLen = inputBufferLen;
        break;
      }
    }

    FatalAssert(outputLen <= outputBufferLen);
  }

  return (outputLen);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgMapTEToAlphaId
 *
 * Parameters:      outputBuffer_p
 *                  inputBuffer_p
 *                  inputBufferLen
 *                  cscsMode
 *                  entity
 *
 * Returns:         Int16 - length of converted data stored in buffer
 *
 * Description:     Converts an input string from its current character set
 *                  into an alphaId.
 *-------------------------------------------------------------------------*/
Int8 vgMapTEToAlphaId(  Int8                      *outputBuffer_p,
                        const Int8                outputBufferLen,
                        const Int8                *inputBuffer_p,
                        const Int16               inputBufferLen,
                        const VgCSCSMode          cscsMode,
                        const VgmuxChannelNumber  entity)
{
  Int16               outputLen = 0,
                      index,
                      *ucs2Data_p,
                      wordsConverted = 0;
  Int8                inputNumber[VG_CR_SYMBOL_LENGTH_UCS2 + NULL_TERMINATOR_LENGTH];
  Boolean             ucs2StringOverflowed;

  FatalAssert(PNULL != outputBuffer_p);
  FatalAssert(PNULL != inputBuffer_p);

  if ( (PNULL != outputBuffer_p) && (PNULL != inputBuffer_p) )
  {
    switch (cscsMode)
    {
      case VG_AT_CSCS_GSM: /* GSM to alphaId */
      case VG_AT_CSCS_HEX: /* HEX to alphaId */
      case VG_AT_CSCS_IRA: /* ??? to alphaId */
      case VG_AT_CSCS_PCCP:
      case VG_AT_CSCS_PCDN:
      case VG_AT_CSCS_8859_1:
      {
        /* Allocate enough memory to hold UCS2 words.... */
        ucs2Data_p = (Int16 *)getCrConversionBuffer(entity);

        /* Convert to ASCII to UCS2.... */
        if (ucs2Data_p != PNULL)
        {
          /* Do intermediate conversions to GSM, then UCS2 then finally alpha.... */
          outputLen = vgMapTEToGsm( outputBuffer_p,
                                    outputBufferLen,
                                    inputBuffer_p,
                                    inputBufferLen,
                                    cscsMode,
                                    entity);

          utByteAlignedEGsm7BitToUcs2 (outputBuffer_p,
                                        outputLen,
                                         ucs2Data_p,
                                          &wordsConverted,
                                           (VG_CR_CH_SET_CONV_BUFFER_LEN / 2),
                                            &ucs2StringOverflowed);

          FatalAssert (ucs2StringOverflowed == FALSE);

          outputLen = utUcs2ToAlphaId(outputBuffer_p,
                                      outputBufferLen,
                                      ucs2Data_p,
                                      wordsConverted);

          resetCrConversionBuffer (entity);
        }

        else
        {
          FatalParam(entity, cscsMode, 0);
        }
        break;
      }

      case VG_AT_CSCS_UCS2: /* UCS2 to alphaId */
      {
        /* Allocate enough memory to hold UCS2 words.... */
        ucs2Data_p = (Int16 *)getCrConversionBuffer(entity);

        /* Convert to ASCII to UCS2 then to alpha ID.... */
        if (ucs2Data_p != PNULL)
        {
          for (index = 0; index < inputBufferLen; index += VG_CR_SYMBOL_LENGTH_UCS2)
          {
            memcpy(inputNumber, &inputBuffer_p[index], VG_CR_SYMBOL_LENGTH_UCS2);
            inputNumber[VG_CR_SYMBOL_LENGTH_UCS2] = '\0';
            ucs2Data_p[wordsConverted] = vgAsciiToInt16(inputNumber);
            wordsConverted++;
          }

          FatalAssert((wordsConverted*2) <= VG_CR_CH_SET_CONV_BUFFER_LEN);

          outputLen = utUcs2ToAlphaId(outputBuffer_p,
                                      outputBufferLen,
                                      ucs2Data_p,
                                      wordsConverted);

          resetCrConversionBuffer (entity);
        }
        else
        {
          FatalParam(entity, cscsMode, 0);
        }
        break;
      }

      default:
      {
        WarnParam(entity, cscsMode, 0);
        memcpy (outputBuffer_p, inputBuffer_p, inputBufferLen);
        outputLen = inputBufferLen;
        break;
      }
    }

    WarnAssert(outputLen <= outputBufferLen);
  }


  return ((Int8)outputLen);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgMapGsmToTE
 *
 * Parameters:      outputBuffer_p
 *                  inputBuffer_p
 *                  inputBufferLen
 *                  cscsMode
 *                  entity
 *
 * Returns:         Int16 - length of converted data stored in buffer
 *
 * Description:     Converts an input string from GSM format to the TE
 *                  character set.
 *-------------------------------------------------------------------------*/
Int16 vgMapGsmToTE(Int8                     *outputBuffer_p,
                   const Int16              outputBufferLen,
                   const Int8               *inputBuffer_p,
                   const Int16              inputBufferLen,
                   const VgCSCSMode         cscsMode,
                   const VgmuxChannelNumber entity)
{
  Int16               outputLen = 0,
                      index,
                      *ucs2Data_p,
                      wordsConverted;
  Int8                convChar = 0;
  CodePagesStruct     *cpConversion_p = (CodePagesStruct *)cpConversion;
  Boolean             ucs2StringOverflowed;

  PARAMETER_NOT_USED(outputBufferLen);

  FatalAssert(PNULL != outputBuffer_p);
  FatalAssert(PNULL != inputBuffer_p);

  if ( (PNULL != outputBuffer_p) && (PNULL != inputBuffer_p) )
  {

    switch (cscsMode)
    {
      case VG_AT_CSCS_GSM: /* GSM to GSM - no conversion required */
      {
        memcpy (outputBuffer_p, inputBuffer_p, inputBufferLen);
        outputLen = inputBufferLen;
        break;
      }

      case VG_AT_CSCS_HEX: /* GSM to HEX - special case */
      {
        /* converts each character into two byte hexidecimal pair (0-F,0-F) */
        for (index = 0; index < inputBufferLen; index++)
        {
          outputBuffer_p = vgOp8BitHex ((Int8)inputBuffer_p[index], outputBuffer_p);
          outputLen += VG_CR_SYMBOL_LENGTH_HEX;
        }
        break;
      }

      case VG_AT_CSCS_UCS2: /* GSM to UCS2 */
      {
        ucs2Data_p = (Int16 *)getCrConversionBuffer(entity);

        if (ucs2Data_p != PNULL)
        {
          /* Convert to UCS2.... */
          utByteAlignedEGsm7BitToUcs2 ((Int8 *)inputBuffer_p,
                                        inputBufferLen,
                                         ucs2Data_p,
                                          &wordsConverted,
                                           (VG_CR_CH_SET_CONV_BUFFER_LEN / 2),
                                            &ucs2StringOverflowed);

          FatalAssert((wordsConverted*2) <= VG_CR_CH_SET_CONV_BUFFER_LEN);

          /* Convert to ASCII.... */
          for (index = 0; index < wordsConverted; index++)
          {
            outputBuffer_p = vgOp32BitHex((Int16)ucs2Data_p[index],
                                          sizeof(Int16),
                                          outputBuffer_p);
            outputLen += VG_CR_SYMBOL_LENGTH_UCS2;
          }

          resetCrConversionBuffer (entity);
        }
        else
        {
          FatalParam(entity, cscsMode, 0);
        }
        break;
      }

      case VG_AT_CSCS_IRA:  /* GSM to ??? - try character mapping tables */
      case VG_AT_CSCS_PCCP:
      case VG_AT_CSCS_PCDN:
      case VG_AT_CSCS_8859_1:

      {
        if (findCodePage (cscsMode, &cpConversion_p) == TRUE)
        {
          for (index = 0; index < inputBufferLen; index++)
          {
            if (mapCharFromGsm(&convChar, inputBuffer_p[index], cpConversion_p) == TRUE)
            {
              outputBuffer_p[index] = convChar;
            }

            else
            {
              outputBuffer_p[index] = GSM_CHARACTER_SET_SIZE;
            }
          }
          outputLen = inputBufferLen;
        }
        else
        {
          /* if no conversion possible return input string */
          WarnParam(entity, cscsMode, 0);
          memcpy (outputBuffer_p, inputBuffer_p, inputBufferLen);
          outputLen = inputBufferLen;
        }
        break;
      }

      default:
      {
        WarnParam(entity, cscsMode, 0);
        memcpy (outputBuffer_p, inputBuffer_p, inputBufferLen);
        outputLen = inputBufferLen;
        break;
      }
    }

    FatalAssert(outputLen <= outputBufferLen);
  }

  return (outputLen);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgMapHexToTE
 *
 * Parameters:      outputBuffer_p
 *                  inputBuffer_p
 *                  inputBufferLen
 *                  cscsMode
 *                  entity
 *
 * Returns:         Int16 - length of converted data stored in buffer
 *
 * Description:     Converts an input string from HEX format to the TE
 *                  character set.
 *-------------------------------------------------------------------------*/
Int16 vgMapHexToTE(Int8                     *outputBuffer_p,
                   const Int16              outputBufferLen,
                   const Int8               *inputBuffer_p,
                   const Int16              inputBufferLen,
                   const VgCSCSMode         cscsMode)
{
  Int16               outputLen = 0,
                      index;

  PARAMETER_NOT_USED(outputBufferLen);

  FatalAssert(PNULL != outputBuffer_p);
  FatalAssert(PNULL != inputBuffer_p);

  if ( (PNULL != outputBuffer_p) && (PNULL != inputBuffer_p) )
  {
    switch (cscsMode)
    {
      case VG_AT_CSCS_UCS2: /* HEX to UCS2 */
      case VG_AT_CSCS_HEX:  /* HEX to HEX */
      {
        /* converts each byte into hexidecimal pair (0-F,0-F).... */
        for (index = 0; index < inputBufferLen; index++)
        {
          outputBuffer_p = vgOp8BitHex ((Int8)inputBuffer_p[index], outputBuffer_p);
          outputLen += VG_CR_SYMBOL_LENGTH_HEX;
        }
        break;
      }

      case VG_AT_CSCS_GSM:  /* HEX to GSM - no conversion required */
      case VG_AT_CSCS_IRA:  /* HEX to ??? - no conversion required */
      case VG_AT_CSCS_PCCP:
      case VG_AT_CSCS_PCDN:
      case VG_AT_CSCS_8859_1:
      {
        memcpy (outputBuffer_p, inputBuffer_p, inputBufferLen);
        outputLen = inputBufferLen;
        break;
      }

      default:
      {
        WarnParam(cscsMode, 0, 0);
        memcpy (outputBuffer_p, inputBuffer_p, inputBufferLen);
        outputLen = inputBufferLen;
        break;
      }
    }

    FatalAssert(outputLen <= outputBufferLen);
  }

  return (outputLen);
}

/*--------------------------------------------------------------------------
 *
 * Function:        vgMapAlphaIdToTE
 *
 * Parameters:      outputBuffer_p
 *                  inputBuffer_p
 *                  inputBufferLen
 *                  cscsMode
 *                  entity
 *
 * Returns:         Int16 - length of converted data stored in buffer
 *
 * Description:     Converts an input string from an AlphaId to the TE
 *                  character set.
 *-------------------------------------------------------------------------*/
Int16 vgMapAlphaIdToTE( Int8                      *outputBuffer_p,
                        const Int16               outputBufferLen,
                        const Int8                *inputBuffer_p,
                        const Int16               inputBufferLen,
                        const VgCSCSMode          cscsMode,
                        const VgmuxChannelNumber  entity)
{
  Int16               outputLen = 0,
                      index,
                      *ucs2Data_p,
                      wordsConverted = 0;
  Int8                convChar = 0,
                      *writePoint_p;
  Boolean             gsmStringOverflowed = FALSE;
  CodePagesStruct     *cpConversion_p = (CodePagesStruct *)cpConversion;

  FatalAssert(PNULL != outputBuffer_p);
  FatalAssert(PNULL != inputBuffer_p);

  if ( (PNULL != outputBuffer_p) && (PNULL != inputBuffer_p) )
  {
    /* Convert alpha to ucs2.  This is the intermediate stage for all conversions....*/
    ucs2Data_p = (Int16 *)getCrConversionBuffer(entity);

    if (ucs2Data_p != PNULL)
    {
      /* Convert to UCS2.... */
      wordsConverted = utAlphaIdToUcs2( ucs2Data_p,
                                        (VG_CR_CH_SET_CONV_BUFFER_LEN/2),
                                        inputBuffer_p,
                                        inputBufferLen);
      /* Check conversion length.... */
      FatalAssert((wordsConverted*2) <= VG_CR_CH_SET_CONV_BUFFER_LEN);
    }
    else
    {
      FatalParam(entity, cscsMode, 0);
    }

    /* Now convert ucs2 to required format.... */
    switch (cscsMode)
    {
      case VG_AT_CSCS_UCS2: /* alphaId to UCS2 */
      {
        writePoint_p = outputBuffer_p;

        /* Convert to ASCII.... */
        for (index = 0; index < wordsConverted; index++)
        {
          writePoint_p = vgOp32BitHex((Int16)ucs2Data_p[index],
                                      sizeof(Int16),
                                      writePoint_p);
          outputLen += VG_CR_SYMBOL_LENGTH_UCS2;
        }
        break;
      }

      case VG_AT_CSCS_GSM:  /* AlphaId to GSM - no conversion required */
      {
        /* Convert intermediate UCS2 to GSM.... */
        utUcs2ToByteAlignedEGsm7Bit (ucs2Data_p,
                                      wordsConverted,
                                       outputBuffer_p,
                                        &outputLen,
                                         outputBufferLen,
                                          &gsmStringOverflowed);

        FatalAssert(gsmStringOverflowed == FALSE);

        break;
      }

      case VG_AT_CSCS_HEX:
      {
        /* Convert intermediate UCS2 to GSM.  Note we can safely use
         * the output buffer as a temp store in this case since the GSM data
         * will be 1/2 the expected HEX conversion size hence there should be
         * enough space allocated...
         */

        utUcs2ToByteAlignedEGsm7Bit (ucs2Data_p,
                                      wordsConverted,
                                       outputBuffer_p,
                                        &outputLen,
                                         outputBufferLen,
                                          &gsmStringOverflowed);

        FatalAssert(gsmStringOverflowed == FALSE);

      /* Now convert to hex.... */
      outputLen = vgMapTEToHex( (Int8 *)getCrConversionBuffer(entity),
                                 VG_CR_CH_SET_CONV_BUFFER_LEN,
                                  outputBuffer_p,
                                   outputLen,
                                    VG_AT_CSCS_GSM);

      outputLen = vgMapHexToTE( outputBuffer_p,
                                 outputBufferLen,
                                  (Int8 *)getCrConversionBuffer(entity),
                                   outputLen,
                                    VG_AT_CSCS_HEX);
      break;
    }

      case VG_AT_CSCS_IRA:  /* alphaId to ??? - no conversion required */
      case VG_AT_CSCS_PCCP:
      case VG_AT_CSCS_PCDN:
      case VG_AT_CSCS_8859_1:
      {
        /* Convert intermediate UCS2 to GSM then to XXX.  Note we can safely use
         * the output buffer as a temp store in this case since the GSM data
         * will be a 1:1 ratio to the XXX conversion size hence there should be
         * enough space allocated...
         */

        utUcs2ToByteAlignedEGsm7Bit (ucs2Data_p,
                                      wordsConverted,
                                       outputBuffer_p,
                                        &outputLen,
                                         outputBufferLen,
                                          &gsmStringOverflowed);

        FatalAssert(gsmStringOverflowed == FALSE);

        if (findCodePage (cscsMode, &cpConversion_p) == TRUE)
        {
          for (index = 0; index < outputLen; index++)
          {
            if (mapCharFromGsm(&convChar, outputBuffer_p[index], cpConversion_p) == TRUE)
            {
              outputBuffer_p[index] = convChar;
            }
            else
            {
              outputBuffer_p[index] = GSM_CHARACTER_SET_SIZE;
            }
          }
        }
        else
        {
          /* if no conversion possible return input string */
          WarnParam(entity, cscsMode, 0);
          memcpy (outputBuffer_p, inputBuffer_p, inputBufferLen);
          outputLen = inputBufferLen;
        }
        break;
      }

      default:
      {
        WarnParam(entity, cscsMode, 0);
        break;
      }
    }

    FatalAssert(outputLen <= outputBufferLen);
  }

  resetCrConversionBuffer (entity);

  return (outputLen);
}

#if defined (FEA_PHONEBOOK)
/*--------------------------------------------------------------------------
 *
 * Function:        vgAlphaStringSearch
 *
 * Parameters:      alphaId_p           - Alpha ID to search
 *                  lenAlphaString      - Length of alpha
 *                  alphaSearchText_p   - The string to search for.
 *                  lenSearchText       - Length of search string
 *                  entity              - entity.
 *
 * Returns:         TRUE if match found, FALSE otherwise.
 *
 * Description:     Searches for an instance of an alpha string in another.
 *
 *-------------------------------------------------------------------------*/
Boolean vgAlphaStringSearch(const Int8                *alphaId_p,
                            const Int16               lenAlphaString,
                            const Int8                *alphaSearchText_p,
                            const Int16               lenSearchText,
                            const VgmuxChannelNumber  entity)
{
  Int16               i = 0,
                      convLenAlphaString = 0,
                      convLenSearchString = 0;
  Boolean             stringFound = FALSE;
  Int8                *stringBuffer_p = getCrAlphaStringBuffer(entity);
  Int8                *searchBuffer_p = getCrAlphaSearchBuffer(entity);

  FatalAssert( PNULL != stringBuffer_p );
  FatalAssert( PNULL != searchBuffer_p );

  if ( (PNULL != stringBuffer_p) &&
       (PNULL != searchBuffer_p) )
  {
    /* Convert both strings to UCS2.  This is required since this will be the
     * only common format where comparissons between all data types will be
     * possible....
     */
    convLenAlphaString = vgMapAlphaIdToTE(  stringBuffer_p,
                                            VG_CR_CH_SET_ALPHA_BUFFER_LEN,
                                            alphaId_p,
                                            lenAlphaString,
                                            VG_AT_CSCS_UCS2,
                                            entity);
    convLenSearchString = vgMapAlphaIdToTE( searchBuffer_p,
                                            VG_CR_CH_SET_ALPHA_SEARCH_LEN,
                                            alphaSearchText_p,
                                            lenSearchText,
                                            VG_AT_CSCS_UCS2,
                                            entity);


    /* Search for occurances of search text in alpha string.... */

    while(((convLenSearchString + i) <= convLenAlphaString) && (stringFound == FALSE))
    {
      if(memcmp(searchBuffer_p,
                (stringBuffer_p + i),
                convLenSearchString) == 0)
      {
        stringFound = TRUE;
      }
      else
      {
        i++;
      }
    }

  }

  resetCrAlphaSearchBuffer(entity);
  resetCrAlphaStringBuffer(entity);

  return (stringFound);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgGetMaxAlphaIdSize
 *
 * Parameters:  cscsMode - the current character set
 *
 * Returns:     maximum input length
 *
 * Description: Returns maximum input length for an AlphaId before conversion
 *
 *-------------------------------------------------------------------------*/

Int16 vgGetMaxAlphaIdSize (const VgCSCSMode cscsMode)
{
  Int16 maxSize;

  switch (cscsMode)
  {
    case VG_AT_CSCS_HEX:
    {
      maxSize = SIM_ALPHA_ID_SIZE * VG_CR_SYMBOL_LENGTH_HEX;
      break;
    }
    case VG_AT_CSCS_UCS2:
    {
      maxSize = SIM_ALPHA_ID_SIZE * VG_CR_SYMBOL_LENGTH_UCS2;
      break;
    }
    default:
    {
      maxSize = SIM_ALPHA_ID_SIZE;
      break;
    }
  }

  return (maxSize);
}
#endif /* FEA_PHONEBOOK */

/***************************************************************************
 * Processes
 ***************************************************************************/

/* END OF FILE */

