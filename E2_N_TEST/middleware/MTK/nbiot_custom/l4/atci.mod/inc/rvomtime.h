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
 * RVOMTIME.H
 * Header file for timer code
 **************************************************************************/

#ifndef RVOMTIME_H
#define RVOMTIME_H

/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <rvsystem.h>

/*   Exported Type definitions 
**   If a new timer is added, please add the corresponding time period
**   to currentTimerMilli[NUM_OF_VG_CI_TIMERS] in file rvomtime.h
*/

typedef enum VgCiTimerIdTag
{
  TIMER_TYPE_STK_CNF_CONFIRM,
  TIMER_TYPE_STK_TONE,
  TIMER_TYPE_CONNECT,
  TIMER_TYPE_RINGING,
  TIMER_TYPE_DROPPED,
  TIMER_TYPE_PORT_SETTING_CHANGE,
  TIMER_TYPE_CMUX_ACTIVATION,
  TIMER_TYPE_SMS_MSG,
  TIMER_TYPE_SMS_TR2M,
  TIMER_TYPE_SMS_CMMS,
  TIMER_TYPE_SIGNAL_DELAY,

#if defined (FEA_PPP)
  TIMER_TYPE_PSD_LOOPBACK,
#endif /* FEA_PPP */  

#if defined (ENABLE_AT_ENG_MODE)
  TIMER_TYPE_EM_PERIODIC,
#endif
#ifdef ENABLE_AP_BRIDGE_FEATURE
  TIMER_TYPE_APB_DATA_MODE_REACTIVE,
#endif
  NUM_OF_VG_CI_TIMERS
}
VgCiTimerId;

/* Exported Function Prototypes */

void setTimeOutPeriod (Int32  milli, VgCiTimerId  ciTimer);
void vgCiLocalTimerInit   (void);
void vgCiStopTimer (VgCiTimerId timerId);
void vgCiStopAllEntityTimers (const VgmuxChannelNumber entity);
Boolean vgCiStartTimer(VgCiTimerId timerId,
                        const VgmuxChannelNumber entity);
void vgCiProcessTimerExpiry (Int16 userValue, TimerId timerId);
void timerInitialise (void);
Boolean isTimerRunning (VgCiTimerId timerId);
Int32 getTimeOutPeriod (VgCiTimerId ciTimer);

#endif

