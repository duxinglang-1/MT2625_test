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
 *  .
 **************************************************************************/

#define MODULE_NAME "ABSICUST"

#include  <system.h>

/******************************************************************************
 * Include Files
 *****************************************************************************/
#include <string.h>

#if !defined (KERNEL_H)
#   include  "kernel.h"
#endif

#if !defined (ALSI_SIG_H)
#include <alsi_sig.h>
#endif

#if !defined (ABSICUST_H)
#   include <absicust.h>
#endif


#if defined (FEA_SIM_IMSI_LOCK)
/* default value for SoftBank
ImsiValidItem imsiValidItem[] = {
                    {"4402092"},
                };

ImsiValidRange imsiValidRange[] = {
                    {"4402000", "4402049"},
                };
*/

/* default value for RJIL
ImsiValidItem imsiValidItem[] = {
                    {"405840"},
                };

ImsiValidRange imsiValidRange[] = {
                    {"405854", "405874"},
                };
*/

/* default value for test*/
ImsiValidItem imsiValidItem[] = {
                    {"001010123"},
                    {"001020123"},
                    {"0123456789"},
                    {"0123000"},
                };

ImsiValidRange imsiValidRange[] = {
                    {"0123456000", "0123456300"},
                    {"0123188", "0123300"},
                };

#if defined(FEA_SIM_IMSI_LOCK_CONTROL)
Boolean imsiLockEnabled = TRUE;

/* To enable or disable the IMSI lock feature. */
void setImsiLockState(Boolean enabled)
{
    imsiLockEnabled = enabled;
}

/* Check if the IMSI lock feature is enabled. */
Boolean getImsiLockState(void)
{
    return imsiLockEnabled;
}
#endif

/****************************************************************************
*
* Function:    absiImsiConvertToString
*
* Parameters:  imsiStr: output, string.
               imsi_p : input, which is read from the SIM.
*
* Returns:     Nothing
*
* Description: Convert the imsi to string type.
*
****************************************************************************/
void absiImsiConvertToString(Char* imsiStr, Imsi *imsi_p)
{
    Int8                     digit;
    Int8                     decDigit;
    Int8                     val8 = 0;

    val8 = imsi_p->contents[0];

    decDigit = (val8 >> 4) & 0x0F;
    if (decDigit != 0x0F)
    {
      *(imsiStr++) = (Char) (decDigit + '0');
    }

    for ( digit = 1; digit < imsi_p->length ; digit++)
    {
      val8 = imsi_p->contents[digit];
      decDigit = val8 & 0x0F;
      if (decDigit != 0x0F)
      {
        *(imsiStr++) = (Char) (decDigit + '0');
      }

      decDigit = (val8 >> 4) & 0x0F;
      if (decDigit != 0x0F)
      {
         *(imsiStr++) = (Char) (decDigit + '0');
      }
    }

    *imsiStr = '\0';
}

/****************************************************************************
*
* Function:    absiImsiItemCheck
*
* Parameters:  none
*
* Returns:     Boolean, TURE: the IMSI is valid, FALSE: the IMSI is invalid.
*
* Description: Compare IMSI with imsiValidItem.
*
****************************************************************************/
Boolean absiImsiItemCheck(Char *imsiStr)
{
    Int8 validImsiNum;
    Int8 imsiItem = 0;
    Int8 checkBit = 0;
    Char validNum;
    Char checkNum;

    /* Calculate how many vaild imsi we need to compare */
    validImsiNum = sizeof(imsiValidItem)/sizeof(ImsiValidItem);

    for(imsiItem = 0; imsiItem < validImsiNum; imsiItem++)
    {
        checkBit = 0;

        do
        {
            validNum = imsiValidItem[imsiItem].contents[checkBit];
            checkNum = imsiStr[checkBit];
            if(validNum == '\0')
            {
                return TRUE;
            }
            checkBit++;
        }while(validNum == checkNum);

    }

    return FALSE;
}

/****************************************************************************
*
* Function:    absiImsiRangeCheck
*
* Parameters:  none
*
* Returns:     Boolean, TURE: the IMSI is valid, FALSE: the IMSI is invalid.
*
* Description: Check if the IMSI is in the range.
*
****************************************************************************/
Boolean absiImsiRangeCheck(Char *imsiStr)
{
    Int8    validImsiNum;
    Int8    imsiItem = 0;
    Int8    checkBit = 0;
    Char    lowerNum;
    Char    upperNum;
    Char    checkNum;
    Boolean lowerValid = FALSE;
    Boolean upperValid = FALSE;

    /* Calculate how many vaild imsi we need to compare */
    validImsiNum = sizeof(imsiValidRange)/sizeof(ImsiValidRange);

    for(imsiItem = 0; imsiItem < validImsiNum; imsiItem++)
    {
        checkBit = 0;
        lowerValid = FALSE;
        upperValid = FALSE;

        do
        {
            lowerNum = imsiValidRange[imsiItem].contents_start[checkBit];
            checkNum = imsiStr[checkBit];
            if((checkNum > lowerNum) ||
               (lowerNum == '\0'))
            {
                lowerValid = TRUE;
            }
            checkBit++;
        }while((checkNum >= lowerNum) && (lowerValid == FALSE));

        if(lowerValid == TRUE)
        {
            checkBit = 0;
            do
            {
                upperNum = imsiValidRange[imsiItem].contents_end[checkBit];
                checkNum = imsiStr[checkBit];
                if((checkNum < upperNum) ||
                   (upperNum == '\0'))
                {
                    return TRUE;
                }
                checkBit++;
            }while(checkNum <= upperNum);
        }
    }

    return FALSE;
}
#endif

/****************************************************************************
*
* Function:    absiDoMepImsiCheck
*
* Parameters:  imsi_p, structure containing IMSI.
*
* Returns:     Boolean, TURE: the IMSI is valid, FALSE: the IMSI is invalid.
*
* Description: Check if the input IMSI is valid.
*
****************************************************************************/
Boolean absiDoMepImsiCheck(Imsi *imsi_p)
{
#if defined (FEA_SIM_IMSI_LOCK)
    Char    imsiStr[MAX_IMSI_CONTENT_LENGTH];
    Boolean retult = FALSE;

#if defined (FEA_SIM_IMSI_LOCK_CONTROL)
    if(imsiLockEnabled == FALSE)
    {
        retult = TRUE;
    }
    else
#endif
    {
        absiImsiConvertToString(imsiStr, imsi_p);

        if((absiImsiItemCheck(imsiStr) == TRUE) ||
           (absiImsiRangeCheck(imsiStr) == TRUE))
        {
            retult = TRUE;
        }
    }

    return retult;
#else
    return TRUE;
#endif
}

/* END OF FILE */
