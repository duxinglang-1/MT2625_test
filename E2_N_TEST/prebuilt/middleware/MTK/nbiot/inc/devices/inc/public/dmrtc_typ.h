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
 * Device Management - Real-Time-Clock ACLK type definitions.
 **************************************************************************/

#ifndef DMRTC_TYP_H
#define DMRTC_TYP_H

/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>

#include <kitqid.h>  /* GKI Task and Queue identifier defintions. */

#include <uttime.h>  /* For RTC utility type definitions. */

/***************************************************************************
 * Typed Constants
 **************************************************************************/

#define MAX_ALARM_REF_STRING_SIZE (2)
#define MAX_NUM_OF_ALARMS         (4)
#define MAX_ALARM_MSG_SIZE        (30)

/***************************************************************************
 * Type Definitions
 **************************************************************************/
typedef enum AclkAlarmPowerTag
{
  ALARM_POWER_DISABLED,
  ALARM_POWER_OFF,
  ALARM_POWER_ON,
  NUM_OF_ALARM_POWERS,
  ALARM_POWER_INVALID
}
AclkAlarmPower;

typedef enum AclkAlarmRepeatTag
{
  ALARM_REPEAT_NONE,
  ALARM_REPEAT_DAILY,
  ALARM_REPEAT_WEEKLY,
  ALARM_REPEAT_MONTHLY,
  NUM_OF_ALARM_REPEATS,
  ALARM_REPEAT_INVALID
}
AclkAlarmRepeat;

typedef enum AclkAlarmStateTag
{
  ALARM_STATE_DISABLED,
  ALARM_STATE_ENABLED,
  NUM_OF_ALARM_STATES,
  ALARM_STATE_INVALID
}
AclkAlarmState;

typedef enum AclkAlarmTypeTag
{
  ALARM_TYPE_USER = 0,
  ALARM_TYPE_MMI_ALARM,
  ALARM_TYPE_EVENT_ALARM
}
AclkAlarmType;

typedef struct AclkAlarmDetailsTag
{
  RtcDateAndTime    dateAndTime;
  AclkAlarmState    state;
  AclkAlarmRepeat   repeat;
  Int16             tone;
  AclkAlarmPower    power;
  Boolean           vibrator;
  TaskId            taskId;
  Int16             message[MAX_ALARM_MSG_SIZE + 1];
  AclkAlarmType     alarmType;
  Int8              volume;
}
AclkAlarmDetails;


typedef struct AnrmAlarmDetailsTag
{
  Boolean          present;
  AclkAlarmDetails details;
}
AnrmAlarmDetails;

typedef struct AclkDataAreaTag
{
  RtcDisplacement       rtcTimeZone;
  RtcDaylightSaving     rtcDaylightSaving;
  RtcDateAndTime        rtcAlarmDateAndTime;
  Boolean               rtcAlarmPowerOn;
  AnrmAlarmDetails      alarm[MAX_NUM_OF_ALARMS];
  Int16                 numAlarmsSet;
}
AclkDataArea;

#endif /* DMRTC_TYP_H */

/* END OF FILE */

