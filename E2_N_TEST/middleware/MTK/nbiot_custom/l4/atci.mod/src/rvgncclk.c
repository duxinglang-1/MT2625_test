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
 * Handler for AT Command AT+CCLK which configures the clock
 **************************************************************************/

#define MODULE_NAME "RVGNCCLK"


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
#if !defined (RVGNSIGO_H)
#  include <rvgnsigo.h>
#endif
#if !defined (RVGNCCLK_H)
#  include <rvgncclk.h>
#endif
#if !defined (RVCRHAND_H)
#  include <rvcrhand.h>
#endif
#include "hal_rtc_internal.h"
 
/***************************************************************************
 * Signal definitions
 ***************************************************************************/

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

#define CCLK_DATA_AND_TIME_LENGTH (20)

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

static Boolean vgCharsToInt8 (Char *charA,
                               Int8 *result);
#if defined(HAL_RTC_MODULE_ENABLED)

static Boolean vgTimeStringToRtcTime (Char *inString,
                                       hal_rtc_time_t *rtc,
                                       signed char *pTime_zone);
#endif

/***************************************************************************
 * Type Definitions
 ***************************************************************************/

/***************************************************************************
 * Local Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
*
* Function:    vgCharsToInt8
*
* Parameters:  *charA   - pointer to array of chars holding integer value
*              *result  - integer conversion of charA
*
* Returns:     Boolean  - TRUE if conversion successful.
*                       - FALSE otherwise
*
* Description: Converts a string of into an integer
*
**************************************************************************/

Boolean vgCharsToInt8 (Char *charA, Int8 *result)
{
  Boolean conversion = TRUE;
  Char    *charB = charA + 1;
  Int8    intA = 0;
  Int8    intB = 0;

  if ((*charA >= '0') && (*charA <= '9'))
  {
    intA = *charA - '0';
  }
  else
  {
    conversion = FALSE;
  }

  if ((conversion == TRUE) && (*charB >= '0') && (*charB <= '9'))
  {
    intB = *charB - '0';
  }
  else
  {
    conversion = FALSE;
  }

  if (conversion == TRUE)
  {
    *result = (intA * 10) + intB;
  }
  else
  {
    *result = 0;  /* reset */
  }

  return (conversion);
}


/*--------------------------------------------------------------------------
*
* Function:    vgTimeStringToRtcTime
*
* Parameters:  *inString    - pointer to array of chars holding time string
*              *rtc         - real time clock data (type RtcDateAndTime)
*
* Returns:     Boolean      - TRUE if conversion successful.
*                           - FALSE otherwise
*
* Description: Converts a string of format "yy/MM/dd,hh:mm:ss+-zz" (where
*              characters indicate the last two digits of year,month,day,
*              hour,minute,second and time zone) into a type described by
*              structure RtcDateAndTime i.e. integer format
**************************************************************************/
#if defined(HAL_RTC_MODULE_ENABLED)

Boolean vgTimeStringToRtcTime (Char *inString, hal_rtc_time_t *rtc,signed char *pTime_zone)
{
    
      Boolean success    = TRUE;     /* TRUE if whole time string conversion OK */
    
    
      Int8    cIndex;        /* Loop counter */
      Int8    yearChk[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
      Int8    dateTime[7];
    
      if (vgStrLen (inString) == CCLK_DATA_AND_TIME_LENGTH)
      {
        /* check for dividers */
        if (((inString[ 2] == '/') &&
             (inString[ 5] == '/') &&
             (inString[ 8] == ',') &&
             (inString[11] == ':') &&
             (inString[14] == ':') &&
            ((inString[17] == '+') || (inString[17] == '-'))))
        {
          /* convert time and date values */
          for (cIndex = 0; cIndex < 7; cIndex++ )
          {
            if (vgCharsToInt8 ((inString + (cIndex * 3)), &dateTime[cIndex]) == FALSE)
            {
              success = FALSE;
            }
          }
    
          if (success == TRUE)
          {
    
            /* date */
            rtc->rtc_year     = dateTime[0];
            rtc->rtc_mon      = dateTime[1];
            rtc->rtc_day      = dateTime[2];
            rtc->rtc_hour     = dateTime[3];
            rtc->rtc_min      = dateTime[4];
            rtc->rtc_sec      = dateTime[5];
            rtc->rtc_week     = 0;
            if(rtc->rtc_day > yearChk[rtc->rtc_mon-1])
            {
                return FALSE;
            }
            /* format */
            if(inString[17] == '+')
            {
               *pTime_zone = dateTime[6]*4;      
            }
            else
            {
               *pTime_zone =0 - dateTime[6]*4 ;
            }
          }
        }
        else /* invalid dividers */
        {
          success = FALSE;
        }
      }
      else /* string length incorrect */
      {
        success = FALSE;
      }
    
      return (success);
}
#endif

/***************************************************************************
 * Global Functions
 ***************************************************************************/

/*--------------------------------------------------------------------------
*
* Function:    vgGnCCLK
*
* Parameters:  commandBuffer_p - pointer to command line string
*              entity          - mux channel number
*
* Returns:     Resultcode_t    - result of function
*
* Description: Executes the +CCLK AT command which allows the user to
*              set the real-time clock of the ME.
*
*              AT+CCLK=<time> - set command where
*              <time>:  string type value; format is "yy/MM/dd,hh:mm:ss+-zz"
*                       where characters indicate the last two digits of
*                       year,month,day,hour,minute,second and time zone.
*                       The time zone is expressed in quarters of an hour
*                       between the local time and GMT; range (-47...+48)
*                       eg. 6th May 1994 22:10:00 GMT+2 hours equals
*                       "94/05/06,22:10:00+08"
*-------------------------------------------------------------------------*/
ResultCode_t vgGnCCLK (CommandLine_t *commandBuffer_p,
                        const VgmuxChannelNumber entity)
{
  ResultCode_t       result = RESULT_CODE_OK;
#if defined(HAL_RTC_MODULE_ENABLED)
  Char               timeString[STRING_LENGTH_40 + NULL_TERMINATOR_LENGTH] = {0};
  Int16              timeStringLength = 0;
  hal_rtc_time_t rtc_time;
  signed char  pTime_zone;
  hal_rtc_status_t temp;

  switch (getExtendedOperation (commandBuffer_p))
  {
    case EXTENDED_RANGE:    /* AT+CCLK=? */
    {
      break;
    }
    case EXTENDED_ASSIGN:   /* AT+CCLK=  */
    { /* now get the time string */
      if (getExtendedString (commandBuffer_p,
                              timeString,
                               STRING_LENGTH_40,
                                &timeStringLength) == TRUE)
      {
        if (vgTimeStringToRtcTime (&timeString[0], &rtc_time,&pTime_zone) == TRUE)
        { /* Send request to set real time clock */
          if((HAL_RTC_STATUS_OK == hal_rtc_set_time(&rtc_time))&&(HAL_RTC_STATUS_OK == hal_rtc_set_quarter_hour(pTime_zone)))
          {
              result = RESULT_CODE_OK;
          }
          else
          {
              result = VG_CME_OPERATION_FAILED;
          }

        }
        else
        { /* no parameter so error */
          result = VG_CME_INVALID_INPUT_VALUE;
        }
      }
      else
      {
        result = RESULT_CODE_ERROR;
      }
      break;
    }
    case EXTENDED_QUERY:    /* AT+CCLK?  */
    {
      result = RESULT_CODE_OK;
      hal_rtc_get_time(&rtc_time);
      hal_rtc_get_quarter_hour(&pTime_zone);
      vgPutNewLine (entity);
      if(rtc_time.rtc_year < 100 && rtc_time.rtc_year >9)
      {
          vgPrintf (entity, (const Char *)"+CCLK:20%d/%02d/%02d,%02d:%02d:%02dGMT%+d"
            ,rtc_time.rtc_year,rtc_time.rtc_mon,rtc_time.rtc_day,rtc_time.rtc_hour,rtc_time.rtc_min,rtc_time.rtc_sec,pTime_zone/4);
      }
      else if(rtc_time.rtc_year <= 9)
      {
          vgPrintf (entity, (const Char *)"+CCLK:200%d/%02d/%02d,%02d:%02d:%02dGMT%+d"
            ,rtc_time.rtc_year,rtc_time.rtc_mon,rtc_time.rtc_day,rtc_time.rtc_hour,rtc_time.rtc_min,rtc_time.rtc_sec,pTime_zone/4);      
      }
      else
      {
          vgPrintf (entity, (const Char *)"+CCLK:2%d/%02d/%02d,%02d:%02d:%02dGMT%+d"
            ,rtc_time.rtc_year,rtc_time.rtc_mon,rtc_time.rtc_day,rtc_time.rtc_hour,rtc_time.rtc_min,rtc_time.rtc_sec,pTime_zone/4);      
      }
      vgPutNewLine (entity);      
      break;
    }
    case EXTENDED_ACTION:   /* AT+CCLK   */
    default:
    {
      result = RESULT_CODE_ERROR;
      break;
    }
  }
#endif
  return (result);
}
/***************************************************************************
 * Processes
 ***************************************************************************/

/* END OF FILE */


