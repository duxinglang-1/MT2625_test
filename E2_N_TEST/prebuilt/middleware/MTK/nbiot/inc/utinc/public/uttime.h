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
 * Function Header for Time Handling Utility Functions
 **************************************************************************/


#ifndef UTTIME_H
#define UTTIME_H

#include <system.h>

#if defined (__cplusplus)
extern "C" {
#endif

/***************************************************************************
 * Macros
 **************************************************************************/

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

#define UT_JANUARY 1
#define UT_FEBRUARY 2
#define UT_MARCH 3
#define UT_APRIL 4
#define UT_MAY 5
#define UT_JUNE 6
#define UT_JULY 7
#define UT_AUGUST 8
#define UT_SEPTEMBER 9
#define UT_OCTOBER 10
#define UT_NOVEMBER 11
#define UT_DECEMBER 12

#define UT_SECONDS_PER_MINUTE   ((Int8) 60)
#define UT_MINUTES_PER_HOUR     ((Int8) 60)
#define UT_HOURS_PER_DAY        ((Int8) 24)
#define UT_MONTHS_PER_YEAR      ((Int8) 12)
#define UT_SECONDS_PER_HOUR     (UT_MINUTES_PER_HOUR * UT_SECONDS_PER_MINUTE)
#define UT_SECONDS_PER_DAY      (UT_SECONDS_PER_HOUR * UT_HOURS_PER_DAY)

#define MAX_RTC_DAYS 397  /* 40-bit ctr,  (2E40 * 31.25uS / 60*60*24)*/
#define MAX_RTC_SECS (MAX_RTC_DAYS * UT_SECONDS_PER_DAY)

/***************************************************************************
 * RTC types
 **************************************************************************/

/***************************************************************************
 * Type Definitions
 **************************************************************************/

/** \addtogroup PrdDevicesRtcDataTypes
 * Data Types used by the Real Time Clock.
 * The following data types are used to support the Real Time Clock.
 * @{
 */

/** RTC data format (set or read).
 */
typedef enum RtcFormatTag
{
  RTC_FORMAT_LOCAL,     /**< RTC data contains the local time. */
  RTC_FORMAT_GMT,       /**< RTC data contains Greenwich Mean Time. */
  RTC_FORMAT_DATE_ONLY, /**< RTC data date field only is valid. */
  RTC_FORMAT_INVALID    /**< All RTC data fields are invalid. */
}
RtcFormat;

/** RTC command status result.
 */
typedef enum RtcCommandStatusTag
{
  /** Command succeeded. */
  RTC_CMD_SUCCESS         = 0,
  /** Command succeeded, NVM cached time and RTC will resynchronised. */
  RTC_CMD_RESYNC_REQUIRED,
  /** Setting of the current time has resulted in an alarm being lost
   *  because it is now in the past or out-of-range. */
  RTC_CMD_ALARM_LOST,
  /** Command failed due to a software applied error condition. */
  RTC_CMD_FAIL_SW,
  /** Command failed due to a hardware applied error condition. */
  RTC_CMD_FAIL_HW,
  /** Command failed because the alarm time is in the past. */
  RTC_CMD_FAIL_ALARM_IN_PAST,
  /** Command failed because the RTC time base is invalid. */
  RTC_CMD_FAIL_BASE_TIME_INVALID
}
RtcCommandStatus;

/** RTC status.
 */
typedef enum RtcStatusTag
{
  RTC_STATUS_OK,                /**< RTS status okay. */
  RTC_STATUS_ALL_ALARMS_IN_USE, /**< All alarms are in use. */
  RTC_STATUS_NOT_SET,           /**< RTC is not set. */
  RTC_STATUS_INVALID,           /**< RTC status is invalid. */
  RTC_ALARM_MISSED,             /**< Alarm has been lost for some reason. */
  RTC_STATUS_NOT_READY,         /**< RTC is not ready. */
  RTC_STATUS_RESET,             /**< RTC has been reset due to some error. */
  RTC_STATUS_FAILURE            /**< RTC is not functioning correctly. */
}
RtcStatus;

/** RTC date (set or read).
 */
typedef struct RtcDateTag
{
  Int16 year;  /**< RTC year. */
  Int8  month; /**< RTC month (1-12). */
  Int8  day;   /**< RTC day of month (1-31). */
}
RtcDate;

/** RTC time (set or read).
 */
typedef struct RtcTimeTag
{
  Int8 hours;   /**< RTC hours (0-23). */
  Int8 minutes; /**< RTC minutes (0-59). */
  Int8 seconds; /**< RTC secinds (0-59). */
}
RtcTime;

/** RTC displacement (offset) direction from GMT.
 */
typedef enum RtcDispFormatTag
{
  RTC_DISP_FORMAT_POS,    /**< Positive offset from UTC. */
  RTC_DISP_FORMAT_NEG,    /**< Negative offset from UTC. */
  RTC_DISP_FORMAT_INVALID /**< Invalid displacement format. */
}
RtcDispFormat;

/** RTC displacement (offset) from GMT.
 */
typedef struct RtcDisplacementTag
{
  RtcDispFormat format; /**< Direction of offset from GMT. */
  RtcTime       offset; /**< Absolute time offset from GMT. */
}
RtcDisplacement;

/** RTC daylight saving (set or read).
 */
typedef enum RtcDaylightSavingTag
{
  RTC_DAYLIGHT_SAVING_NONE = 0, /**< No daylight saving. */
  RTC_DAYLIGHT_SAVING_ONE_HR,   /**< Daylight saving of one hour. */
  RTC_DAYLIGHT_SAVING_TWO_HRS,  /**< Daylight saving of two hours. */
  RTC_DAYLIGHT_SAVING_INVALID   /**< Daylight saving is invalid. */
}
RtcDaylightSaving;

/** RTC date and time (set or read).
 */
typedef struct RtcDateAndTimeTag
{
  RtcDate           date;           /**< RTC date. */
  RtcTime           time;           /**< RTC time. */
  RtcFormat         format;         /**< RTC format. */
  RtcDisplacement   timeZone;       /**< RTC applied time zone. */
  RtcDaylightSaving daylightSaving; /**< RTC applied daylight saving. */
}
RtcDateAndTime;

/** @}.
 * End of PrdDevicesRtcDataTypes add to group.
 */



/***************************************************************************
 * Typed Constants
 **************************************************************************/

extern const RtcTime oneHour;
extern const RtcTime twoHours;
extern const RtcDate oneDay;
extern const RtcDate oneWeek;
extern const RtcDate twentyEightDays;
extern const RtcDate oneMonth;

 /***************************************************************************
 * Type Definitions
 **************************************************************************/

/***************************************************************************
 *  Macros
 **************************************************************************/

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

extern void       utUnixToCurrentTime (Int32 elapsedTimeInSeconds,
                                            RtcDateAndTime * dateAndTime_p);

extern void        utDateAdd          (RtcDateAndTime *dateAndTime_p,
                                            RtcDate *add_p);

extern void        utDateSub          (RtcDateAndTime *dateAndTime_p,
                                            RtcDate *sub_p);

extern void        utDateAddDays      (RtcDateAndTime *dateAndTime_p,
                                            Int32 days);

extern void        utTimeAdd         (RtcDateAndTime *dateAndTime_p,
                                            RtcTime        *addTime_p);
extern void        utTimeSub         (RtcDateAndTime *dateAndTime_p,
                                            RtcTime        *subTime_p);
extern SignedInt16 utCmpTime         (RtcTime        *time1_p,
                                            RtcTime        *time2_p);
extern SignedInt16 utCmpDate         (RtcDate        *date1_p,
                                            RtcDate        *date2_p);
extern SignedInt16 utCmpDateAndTime  (RtcDateAndTime *dt1_p,
                                            RtcDateAndTime *dt2_p);
extern Int8        utDaysInMonth     (RtcDate *date_p);

extern SignedInt32 utTimeDiffSeconds (RtcDateAndTime *dateAndTime1_p,
                                            RtcDateAndTime *dateAndTime2_p);
extern void        utConvSecsToTime  (Int32 seconds, RtcTime *timeValue_p);

extern Boolean     utTimeLocalToGmt  (RtcDateAndTime *dateAndTime_p);

extern Boolean     utTimeGmtToLocal  (RtcDateAndTime *dateAndTime_p );

/** Converts a UTCTime string from e.g. x509 certificates to an RtcDateAndTime representation, returns TRUE on success */
extern Boolean     utUTCTimeToRtcTime(unsigned char* utcTime, Int16 utcTimeLen, RtcDateAndTime* dateAndTime_p);

/** Converts a GeneralizedTime string from e.g. x509 certificates to an RtcDateAndTime representation, returns TRUE on success */
extern Boolean     utGeneralizedTimeToRtcTime(unsigned char* generalizedTime, Int16 generalizedTimeLen, RtcDateAndTime* dateAndTime_p);


#if defined (DEVELOPMENT_VERSION)
extern void        utTestHarness     (void);
#endif  /*DEVELOPMENT_VERSION*/

#if defined (__cplusplus)
}
#endif

#endif

/* END OF FILE */
