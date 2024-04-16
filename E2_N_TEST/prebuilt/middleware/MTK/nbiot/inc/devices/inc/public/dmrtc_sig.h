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
 **************************************************************************/
/** \file
 * Device Management - Real-Time-Clock signalling interface definitions.
 ***************************************************************************/

#ifndef DMRTC_SIG_H
#define DMRTC_SIG_H

/** \addtogroup PrdDevicesRtcSignalApi
 * \ingroup SigDevices
 * Definition of the Real Time Clock DM signalling interface.
 * The following modules define the Real Time Clock DM signalling interface.
 * The RTC DM signalling interface defines a control interface to the
 * application layer, and a managment interface between the RTC DM task, the
 * DM task, and the RTC platform depenendent device driver.
 * @{
 */

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <dmrtc_typ.h>  /* For ACLK type definitions. */

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/***************************************************************************
 * Typed Constants
 **************************************************************************/

/***************************************************************************
 * Type Definitions
 **************************************************************************/

/** \defgroup PrdDevicesRtcControlSignalling \
 *  Real Time Clock Control.
 *  \ingroup PrdDevicesRtcSignalling
 *  Signalling interface used to control the Real Time Clock.
 *  The Real Time Clock control signalling interface interfaces between
 *  the application layer and the RTC DM task. It supports requests to
 *  configure and read the RTC date and time, and requests to set and clear
 *  alarms.
 *  @{
 */

/** Configure \e dmRtcDateAndTimeInd generation characteristics.
 *  The \e dmRtcSetDisplayPeriodReq signal configures the destination
 *  task and generation rate of \e dmRtcDateAndTimeInd signals.
 */
typedef struct DmRtcSetDisplayPeriodReqTag
{
  /** dmRtcDateAndTimeInd destination task.
   */
  TaskId            taskId;

  /** dmRtcDateAndTimeInd generation interval (s).
   */
  Int16             interval;
}
DmRtcSetDisplayPeriodReq;

/** Date and time indicator.
 *  The \e dmRtcDateAndTimeInd signal indicates the data and time,
 *  and the number of alarms that are currently set. The signal
 *  generation rate and destination task is configured via the
 *  \e dmRtcSetDisplayPeriodReq signal.
 */
typedef struct DmRtcDateAndTimeIndTag
{
  /** Source task of this signal.
   */
  TaskId            taskId;

  /** Date and time.
   */
  RtcDateAndTime    dateAndTime;

  /** Number of alarms currently set.
   */
  Int8              numAlarmsSet;
}
DmRtcDateAndTimeInd;

/** RTC status indicator.
 *  The \e dmRtcStatusInd signal indicates that an error condition has
 *  occurred.
 */
typedef struct DmRtcStatusIndTag
{
  /** Source task of this signal.
   */
  TaskId            taskId;

  /** Number of alarms currently set.
   */
  Int8              numAlarmsSet;

  /** RTC status.
   */
  RtcStatus         status;
}
DmRtcStatusInd;

/** Read the date and time.
 *  The \e dmRtcReadDateAndTimeReq signal requests the date and time.
 */
typedef struct DmRtcReadDateAndTimeReqTag
{
  /** Task for confirmation signal.
   */
  TaskId            taskId;

  /** Request command reference, returned in the confirmation signal.
   */
  Int16             commandRef;

  /** Requested format for the reponse date and time.
   */
  RtcFormat         format;
}
DmRtcReadDateAndTimeReq;

/** Return the date and time.
 *  The \e dmRtcReadDateAndTimeCnf signal returns the date and time in
 *  response to a dmRtcReadDateAndTimeReq request.
 */
typedef struct DmRtcReadDateAndTimeCnfTag
{
  /** Command reference from the requesting signal.
   */
  Int16             commandRef;

  /** Source task of this signal.
   */
  TaskId            taskId;

  /** RTC status.
   */
  RtcStatus         status;

  /** Date and time.
   */
  RtcDateAndTime    dateAndTime;
}
DmRtcReadDateAndTimeCnf;

/** Set the date and time.
 *  The \e dmRtcSetDateAndTimeReq signal sets the date and time.
 */
typedef struct DmRtcSetDateAndTimeReqTag
{
  /** Task for confirmation signal.
   */
  TaskId            taskId;

  /** Date and time.
   */
  RtcDateAndTime    dateAndTime;
}
DmRtcSetDateAndTimeReq;

/** Confirmation of date and time set request.
 *  The \e dmRtcSetDateAndTimeCnf signal returns confirmation of a
 *  dmRtcReadDateAndTimeReq request.
 */
typedef struct DmRtcSetDateAndTimeCnfTag
{
  /** Source task of this signal.
   */
  TaskId            taskId;

  /** RTC status.
   */
  RtcStatus         status;
}
DmRtcSetDateAndTimeCnf;

/** Alarm indicator.
 *  The \e dmRtcAlarmInd signal indicates that an alarm has occurred.
 *  occurred.
 */
typedef struct DmRtcAlarmIndTag
{
  /** Source task of this signal.
   */
  TaskId            taskId;

  /** Details of the alarm.
   */
  AclkAlarmDetails  alarmDetails;
}
DmRtcAlarmInd;

/** Clear an alarm.
 *  The \e dmRtcClearAlarmReq signal clears (deletes) an existing alarm.
 */
typedef struct DmRtcClearAlarmReqTag
{
  /** Task for confirmation signal.
   */
  TaskId            taskId;

  /** Reference of alarm to clear.
   */
  Int16             alarmRef;
}
DmRtcClearAlarmReq;

/** Confirmation of an alarm clear request.
 *  The \e dmRtcClearAlarmCnf signal returns confirmation of a
 *  dmRtcClearAlarmReq request.
 */
typedef struct DmRtcClearAlarmCnfTag
{
  /** Source task of this signal.
   */
  TaskId            taskId;

  /** RTC status.
   */
  RtcStatus         status;
}
DmRtcClearAlarmCnf;

/** Read an alarm.
 *  The \e dmRtcReadAlarmReq signal reads an existing alarm.
 */
typedef struct DmRtcReadAlarmReqTag
{
  /** Task for confirmation signal.
   */
  TaskId            taskId;

  /** Reference of alarm to read.
   */
  Int16             alarmRef;
}
DmRtcReadAlarmReq;

/** Confirmation of an alarm read request.
 *  The \e dmRtcReadAlarmCnf signal returns confirmation of a
 *  dmRtcReadAlarmReq request.
 */
typedef struct DmRtcReadAlarmCnfTag
{
  /** Source task of this signal.
   */
  TaskId            taskId;

  /** RTC status.
   */
  RtcStatus         status;

  /** Details of the alarm.
   */
  AclkAlarmDetails  alarm;
}
DmRtcReadAlarmCnf;

/** Set an alarm.
 *  The \e dmRtcSetAlarmReq signal sets a new alarm.
 */
typedef struct DmRtcSetAlarmReqTag
{
  /** Task for confirmation signal.
   */
  TaskId            taskId;

  /** Reference to alarm (set to ALARM_REF_NEW).
   */
  Int16             alarmRef;

  /** Details of the alarm.
   */
  AclkAlarmDetails  alarmDetails;
}
DmRtcSetAlarmReq;

/** Confirmation of an alarm set request.
 *  The \e dmRtcSetAlarmCnf signal returns confirmation of a
 *  dmRtcSetAlarmReq request.
 */
typedef struct DmRtcSetAlarmCnfTag
{
  /** Source task of this signal.
   */
  TaskId            taskId;

  /** RTC status.
   */
  RtcStatus         status;
}
DmRtcSetAlarmCnf;

/** Set the daylight saving.
 *  The \e dmRtcSetDaylightSavingReq signal sets the daylight saving.
 */
typedef struct DmRtcSetDaylightSavingReqTag
{
  /** Task for confirmation signal.
   */
  TaskId            taskId;

  /** Daylight saving to be applied.
   */
  RtcDaylightSaving daylightSaving;
}
DmRtcSetDaylightSavingReq;

/** Confirmation of a daylight saving set request.
 *  The \e dmRtcSetDaylightSavingCnf signal returns confirmation of a
 *  dmRtcSetDaylightSavingReq request.
 */
typedef struct DmRtcSetDaylightSavingCnfTag
{
  /** RTC status.
   */
  RtcStatus         status;
}
DmRtcSetDaylightSavingCnf;

/** Set the time zone.
 *  The \e dmRtcSetTimeZoneReq signal sets the time zone.
 */
typedef struct DmRtcSetTimeZoneReqTag
{
  /** Task for confirmation signal.
   */
  TaskId            taskId;

  /** Time zone to be applied.
   */
  RtcDisplacement   timeZone;
}
DmRtcSetTimeZoneReq;

/** Confirmation of a time zone set request.
 *  The \e dmRtcSetTimeZoneCnf signal returns confirmation of a
 *  dmRtcSetTimeZoneReq request.
 */
typedef struct DmRtcSetTimeZoneCnfTag
{
  /** RTC status.
   */
  RtcStatus         status;
}
DmRtcSetTimeZoneCnf;

/** Read the time zone.
 *  The \e dmRtcReadTimeZoneReq signal reads the time zone.
 */
typedef struct DmRtcReadTimeZoneReqTag
{
  /** Task for confirmation signal.
   */
  TaskId            taskId;
}
DmRtcReadTimeZoneReq;

/** Confirmation of a time zone read request.
 *  The \e dmRtcReadTimeZoneCnf signal returns confirmation of a
 *  dmRtcReadTimeZoneReq request.
 */
typedef struct DmRtcReadTimeZoneCnfTag
{
  /** Current time zone.
   */
  RtcDisplacement   timeZone;

  /** RTC status.
   */
  RtcStatus         status;
}
DmRtcReadTimeZoneCnf;

/** @}
 * End of PrdDevicesRtcControlSignalling group.
 */

/** \defgroup PrdDevicesRtcManagementSignalling \
 *  Real Time Clock Management.
 *  \ingroup PrdDevicesRtcSignalling
 *  Signalling interface used to manage the Real Time Clock operation.
 *  The Real Time Clock management signalling interface interfaces between
 *  the RTC DM task, and the DM task, and the RTC platform depenendent device
 *  driver, to manage the operation the RTC product.
 *  @{
 */

/** RTC read request.
 *  The \e dmDdRtcReadReq signal requests the current RTC setting.
 */
typedef struct DmDdRtcReadReqTag
{
  /** Task for confirmation signal.
   */
  TaskId          taskId;

  /** Request confirmation reference.
   */
  Int16           commandRef;

  /** Format for the returned data.
   */
  RtcFormat       format;
}
DmDdRtcReadReq;

/** RTC read confirmation.
 *  The \e dmDdRtcReadCnf signal returns confirmation of a
 *  \e dmDdRtcReadReq request.
 */
typedef struct DmDdRtcReadCnfTag
{
  /** Command reference of the requesting signal.
   */
  Int16           commandRef;

  /** RTC status.
   */
  RtcStatus       status;

  /** RTC date and time.
   */
  RtcDateAndTime  dateAndTime;
}
DmDdRtcReadCnf;

/** RTC set request.
 *  The \e dmDdRtcSetReq signal sets the RTC.
 */
typedef struct DmDdRtcSetReqTag
{
  /** Task for confirmation signal.
   */
  TaskId          taskId;

  /** RTC date and time.
   */
  RtcDateAndTime  dateAndTime;
}
DmDdRtcSetReq;

/** RTC set confirmation.
 *  The \e dmDdRtcSetCnf signal returns confirmation of a
 *  \e dmDdRtcSetReq request.
 */
typedef struct DmDdRtcSetCnfTag
{
  /** RTC status.
   */
  RtcStatus       status;
}
DmDdRtcSetCnf;

/** RTC status request.
 *  The \e dmDdRtcStatusReq signal requests the RTC status.
 */
typedef struct DmDdRtcStatusReqTag
{
  /** Task for confirmation signal.
   */
  TaskId          taskId;
}
DmDdRtcStatusReq;

/** RTC status indication.
 *  The \e dmDdRtcStatusInd signal indicates the RTC status and may be
 *  generated in response to a \e dmDdRtcStatusInd request, or as an
 *  unsolicted indication due to, for instance an error condition.
 */
typedef struct DmDdRtcStatusIndTag
{
  /** RTC status.
   */
  RtcStatus       status;

  /** Number of alarms currently set.
   */
  Int8            numberOfAlarmsSet;
}
DmDdRtcStatusInd;

/** RTC set alarm request.
 *  The \e dmDdRtcAlarmSetReq signal sets the RTC alarm.
 */
typedef struct DmDdRtcAlarmSetReqTag
{
  /** Task for confirmation signal.
   */
  TaskId          taskId;

  /** Date and time of the alarm.
   */
  RtcDateAndTime  dateAndTime;

  /** Request command reference (returned in the confirmation).
   */
  Int16           commandRef;

  /** /e TRUE if the MS should power-up on alarm.
   */
  Boolean         powerOnRequired;
}
DmDdRtcAlarmSetReq;

/** RTC set alarm confirmation.
 *  The \e dmDdRtcAlarmSetCnf signal returns confirmation of a
 *  \e dmDdRtcAlarmSetReq request.
 */
typedef struct DmDdRtcAlarmSetCnfTag
{
  /** Command reference of requesting signal.
   */
  Int16           commandRef;

  /** RTC status.
   */
  RtcStatus       status;
}
DmDdRtcAlarmSetCnf;

/** RTC clear alarm request.
 *  The \e dmDdRtcAlarmClearReq signal clears the RTC alarm.
 */
typedef struct DmDdRtcAlarmClearReqTag
{
  /** Task for confirmation signal.
   */
  TaskId          taskId;

  /** Request command reference (returned in the confirmation).
   */
  Int16           commandRef;
}
DmDdRtcAlarmClearReq;

/** RTC clear alarm confirmation.
 *  The \e dmDdRtcAlarmClearCnf signal returns confirmation of a
 *  \e dmDdRtcAlarmClearReq request.
 */
typedef struct DmDdRtcAlarmClearCnfTag
{
  /** Command reference of requesting signal.
   */
  Int16           commandRef;

  /** RTC status.
   */
  RtcStatus       status;
}
DmDdRtcAlarmClearCnf;

/** RTC alarm indication.
 *  The \e dmDdRtcAlarmInd signal indicates that the RTC alarm has
 *  occurred. This signal will typically be generated by the RTC device
 *  driver ISR, and sent to the RTC DM task.
 */
typedef struct DmDdRtcAlarmIndTag
{
  /** Source task of this signal.
   */
  TaskId           taskId;

  /** RTC status.
   */
  RtcStatus        status;
}
DmDdRtcAlarmInd;

/** @}
 * End of PrdDevicesRtcManagementSignalling group.
 */

/** @}
 * End of PrdDevicesRtcSignalApi group.
 */

/** \cond
 */

/* Structure not used in RTC signalling interface. */

typedef struct PdRtcAlarmIndTag
{
  TaskId           taskId;
  RtcStatus        status;
}
PdRtcAlarmInd;

/** \endcond
 */

#endif
/* END OF FILE */

