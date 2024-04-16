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
 * RVMMTIM.C
 * Common sub-system handling all the timer code.
 **************************************************************************/

#define MODULE_NAME "RVOMTIME"

/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <gkitimer.h>
#include <vgmx_sig.h>
#include <ttconfig.h>
#include <rvdata.h>
#include <rvomtime.h>
#include <rvcmxut.h>
#include <rvcmxut.h>
#include <rvstk.h>
#include <rvpfut.h>
#include <rvcimxut.h>
#include <rvccut.h>
#include <rvccsigi.h>
#if defined (ENABLE_AT_ENG_MODE)
#include <rvemut.h>
#endif
#include <rvcimux.h>
#include <rvcmux.h>
#include <rvslut.h>
#if defined (DEVELOPMENT_VERSION)
#include <rvtsut.h>
#endif
#include <rvmspars.h>
#include <rvpdsigi.h>
#include <rvmmsigi.h>
#if defined (ENABLE_RAVEN_TIMER_DEBUG)
#include <stdio.h>
#endif
#ifdef ENABLE_AP_BRIDGE_FEATURE
#include <rvapsigi.h>
#endif

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/***************************************************************************
 * Signal definitions
 ***************************************************************************/

/***************************************************************************
 * Types
 ***************************************************************************/

/***************************************************************************
 * Variables
 ***************************************************************************/

/* time period in milli seconds */
static Int32 currentTimerMilli [NUM_OF_VG_CI_TIMERS];

static KiTimer  vgCiTimer [NUM_OF_VG_CI_TIMERS];

typedef void (*VgExpiryProc)(const VgmuxChannelNumber entity);
typedef struct TimerExpiryFuncTag
{
    VgCiTimerId  timerId;
    VgExpiryProc procFunc;
    Boolean      requiresActiveEntity;
} TimerExpiryFunc;

static const TimerExpiryFunc timerExpiryFunc[] =
{
    {TIMER_TYPE_STK_CNF_CONFIRM,      vgStkTimerExpiry,                   TRUE  },
    {TIMER_TYPE_STK_TONE,             vgCiSTKToneTimerExpired,            TRUE  },
    {TIMER_TYPE_CONNECT,              vgCcCONNECTTimerExpiry,             TRUE  },
#if defined (FEA_MT_PDN_ACT)    
    {TIMER_TYPE_RINGING,              vgRINGINGTimerExpiry,               TRUE  },
#endif /* FEA_MT_PDN_ACT */
    {TIMER_TYPE_PORT_SETTING_CHANGE,  vgMuxPORTSETTINGCHANGETimerExpiry,  TRUE  },
    {TIMER_TYPE_CMUX_ACTIVATION,      vgMuxCMUXACTIVATIONTimerExpiry,     TRUE  },
    {TIMER_TYPE_DROPPED,              vgCcDROPPEDTimerExpiry,             TRUE  },
    {TIMER_TYPE_SMS_MSG,              vgMsSMSMsgTimerExpiry,              TRUE  },
    {TIMER_TYPE_SMS_TR2M,             vgMsSMSTr2mTimerExpiry,             TRUE  },
    {TIMER_TYPE_SMS_CMMS,             vgMsSMSCmmsTimerExpiry,             TRUE  },
    {TIMER_TYPE_SIGNAL_DELAY,         vgSignalDelayTimerExpiry,           TRUE  },
#if defined (FEA_PPP)      
    {TIMER_TYPE_PSD_LOOPBACK,         vgPdLoopbackTimerExpiry,            TRUE  },
#endif /* FEA_PPP */      
#if defined (ENABLE_AT_ENG_MODE)
    {TIMER_TYPE_EM_PERIODIC,          vgEmTimerExpiry,                    TRUE  },
#endif
#ifdef ENABLE_AP_BRIDGE_FEATURE
    {TIMER_TYPE_APB_DATA_MODE_REACTIVE, vgApbDataModeReactiveTimerExpiry, TRUE  },
#endif
};

#define NUM_TIMER_EXPIRY_FUNC (sizeof(timerExpiryFunc) / sizeof(TimerExpiryFunc))

/***************************************************************************
 * Macros
 ***************************************************************************/

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

/***************************************************************************
 * Local Functions
 ***************************************************************************/

/* EXPORTED FUNCTIONS */
/****************************************************************************
 *
 * Function:    vgCiLocalTimerInit
 *
 * Parameters:  void
 *
 * Returns:     void
 *
 * Description: Init timeout period
 *
 ****************************************************************************/

void vgCiLocalTimerInit (void)
{
  currentTimerMilli[TIMER_TYPE_STK_CNF_CONFIRM]     = (Int32)10000;   /* stk timeout 10 seconds*/
  currentTimerMilli[TIMER_TYPE_STK_TONE]            = (Int32)5000;    /* stk tone timeout 5 seconds*/
  currentTimerMilli[TIMER_TYPE_CONNECT]             = (Int32)60000;   /* connect timeout */
  currentTimerMilli[TIMER_TYPE_RINGING]             = (Int32)1000;    /* ring timeout */
  currentTimerMilli[TIMER_TYPE_DROPPED]             = (Int32)12000;   /* drop timeout */
  currentTimerMilli[TIMER_TYPE_PORT_SETTING_CHANGE] = (Int32)60;      /* ICF or IPR setting delay */
  currentTimerMilli[TIMER_TYPE_CMUX_ACTIVATION]     = (Int32)60;      /* CMUX - activation of 27.010 MUX delay */
  currentTimerMilli[TIMER_TYPE_SMS_MSG]             = (Int32)1000;    /* SMS MSG wait timer */
  currentTimerMilli[TIMER_TYPE_SMS_TR2M]            = (Int32)12000;   /* SMS MSG TR2M timer */
  currentTimerMilli[TIMER_TYPE_SMS_CMMS]            = (Int32)3000;    /* SMS MSG CMMS timer */
  currentTimerMilli[TIMER_TYPE_SIGNAL_DELAY]        = (Int32)100;     /* SMS Unsolicited busy timer */
#if defined (FEA_PPP)  
  currentTimerMilli[TIMER_TYPE_PSD_LOOPBACK]       = (Int32)50;       /* PSD loopback timer */
#endif /* FEA_PPP */
#if defined (ENABLE_AT_ENG_MODE)
  currentTimerMilli[TIMER_TYPE_EM_PERIODIC]         = (Int32)500;     /* Engineering mode periodic timer */
#endif /* ENABLE_AT_ENG_MODE */
#ifdef ENABLE_AP_BRIDGE_FEATURE
  currentTimerMilli[TIMER_TYPE_APB_DATA_MODE_REACTIVE] = (Int32)50;    /* AP Bridge data mode reactive timer */
#endif
}

/*--------------------------------------------------------------------------
 *
 * Function:    getTimeOutPeriod
 *
 * Parameters:  ciTimer - timer identifier
 *
 * Returns:     Int32 - time-out period
 *
 * Description: Returns the current time-out period for the specified timer
 *
 *-------------------------------------------------------------------------*/

Int32 getTimeOutPeriod (VgCiTimerId  ciTimer)
{
  return (currentTimerMilli [ciTimer]);
}

/*--------------------------------------------------------------------------
 *
 * Function:    isTimerRunning
 *
 * Parameters:  ciTimer - timer identifier
 *
 * Returns:     Boolean - whether time is running
 *
 * Description: Checks timer to records to determine of specified timer
 *              is currently running and we are expecting a timer expiry
 *
 *-------------------------------------------------------------------------*/

Boolean isTimerRunning (VgCiTimerId timerId)
{
  Boolean result = TRUE;

  if (vgCiTimer[timerId].timerId == KI_TIMER_NOT_RUNNING)
  {
    result = FALSE;
  }

  return (result);
}

/*--------------------------------------------------------------------------
 *
 * Function:    timerInitialise
 *
 * Parameters:  none
 *
 * Returns:     nothing
 *
 * Description: Initialises records of all timers that can be used
 *
 *-------------------------------------------------------------------------*/

void timerInitialise (void)
{
  Int8 index;

  for (index = 0; index < NUM_OF_VG_CI_TIMERS; index++)
  {
    vgCiTimer[index].timeoutPeriod = 0;
    vgCiTimer[index].myTaskId      = VG_CI_TASK_ID;
    vgCiTimer[index].timerId       = KI_TIMER_NOT_RUNNING;
    vgCiTimer[index].userValue     = index;
  }
}

/*--------------------------------------------------------------------------
 *
 * Function:    setTimeOutPeriod
 *
 * Parameters:  milli   - number of milliseconds for timer
 *              ciTimer - timer identifier
 *
 * Returns:     nothing
 *
 * Description: Sets the number of milliseconds that should occur between starting a
 *              timer and receiving an indication of its subsequent expiry
 *
 *-------------------------------------------------------------------------*/

void setTimeOutPeriod (Int32 milli, VgCiTimerId ciTimer)
{
  currentTimerMilli [ciTimer] = milli;
#if defined (ENABLE_RAVEN_TIMER_DEBUG)
  printf ("timer (%u) set timeout (%ld)", ciTimer, milli);
#endif
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgCiStopTimer
 *
 * Parameters:  timerId - timer identifier to stop
 *
 * Returns:     nothing
 *
 * Description: Stops the specified timer so that it no longer expires
 *
 *-------------------------------------------------------------------------*/

void vgCiStopTimer (VgCiTimerId timerId)
{
  if (vgCiTimer[timerId].timerId != KI_TIMER_NOT_RUNNING)
  {
#if defined (ENABLE_RAVEN_TIMER_DEBUG)
    printf ("timer (%u) stop; entity (%u); timeout (%ld)",
             timerId,
              vgCiTimer[timerId].userValue,
               currentTimerMilli[timerId]);
#endif
    KiStopTimer(&vgCiTimer[timerId]);
  }
}


/*--------------------------------------------------------------------------
 *
 * Function:    vgCiStartTimer
 *
 * Parameters:  timerId - timer identifier to stop
 *              entity  - mux channel
 *
 * Returns:     Boolean - whether timer was set up
 *
 * Description: Starts a timer off which will later expire and be processed
 *              by the CI Task
 *
 *-------------------------------------------------------------------------*/

Boolean vgCiStartTimer (VgCiTimerId timerId,
                         const VgmuxChannelNumber entity)
{
  Int32    ticks;
  Boolean  status = TRUE;

  vgCiStopTimer (timerId);

  ticks = MILLISECONDS_TO_TICKS (getTimeOutPeriod (timerId));

  /* if the timeperiod is greater than 0 then schedule a timeout */
  if ( ticks > 0 )
  {
    /* set up the ki timer buffer and schedule a timer */
    vgCiTimer[timerId].timeoutPeriod = ticks;
    vgCiTimer[timerId].userValue = (KiTimerUserValue)entity;
#if defined (ENABLE_RAVEN_TIMER_DEBUG)
    printf ("timer (%u) start; entity (%u); timeout (%ld)",
             timerId,
              entity,
               currentTimerMilli [timerId]);
#endif
    KiStartTimer (&vgCiTimer[timerId]);
  }
  else
  {
    /*lint -save -e666 disables warning relating to passing a function to macro */
    /* It is ok to disable this warning as 'getTimeOutPeriod' does not alter value */
    WarnCheck((ticks != 0), timerId, getTimeOutPeriod (timerId), ticks);
    /*lint -restore */
    status = FALSE;
  }

  return (status);
}

/*--------------------------------------------------------------------------
 *
 * Function:    vgCiProcessTimerExpiry
 *
 * Parameters:  in:  Void
 *
 * Returns:     Void
 * Description: Processes timer expiry signals.  If an expiry is received for
 * a timer which has been stopped then the signal is ignored.  The timer id
 * is a combination of the task id which initiated the timer and the timer
 * identity.
 *
 * Design spec:
 *
 *-------------------------------------------------------------------------*/

void vgCiProcessTimerExpiry (Int16 userValue, TimerId timerId)
{

    Int32    aloop;
    Boolean found = FALSE;

    for(    aloop = 0;
            (aloop < NUM_TIMER_EXPIRY_FUNC) && (found == FALSE);
            aloop++ )
    {
        /* Check that the timer has not expired.  When stop timer is called the
         * recorded timer id is reset to TIMER NOT RUNNING */
        if (vgCiTimer[timerExpiryFunc[aloop].timerId].timerId == timerId)
        {
            vgCiTimer[timerExpiryFunc[aloop].timerId].timerId = KI_TIMER_NOT_RUNNING;

            if (timerExpiryFunc[aloop].requiresActiveEntity)
            {
                if (isEntityActive((VgmuxChannelNumber)userValue))
                {
#if defined (ENABLE_RAVEN_TIMER_DEBUG)
                    printf( "timer (%u) expired; entity (%u); timeout (%ld)",
                            timerExpiryFunc[aloop].timerId,
                            userValue,
                            currentTimerMilli[timerExpiryFunc[aloop].timerId]);
#endif
                    (timerExpiryFunc[aloop].procFunc) ((VgmuxChannelNumber)userValue);
                }
                else
                {
#if defined (ENABLE_RAVEN_TIMER_DEBUG)
                    printf( "timer (%u) expiry ignored; entity (%u); timeout (%ld)",
                            timerExpiryFunc[aloop].timerId,
                            userValue,
                            currentTimerMilli[timerExpiryFunc[aloop].timerId]);
#endif
                    /* timer expiry is ignored because initiating entity is not enabled */
                    WarnParam(   timerExpiryFunc[aloop].timerId,
                                 userValue,
                                 currentTimerMilli[timerExpiryFunc[aloop].timerId]);
                }
            }
            else
            {
#if defined (ENABLE_RAVEN_TIMER_DEBUG)
                printf( "timer (%u) expired; entity (%u); timeout (%ld)",
                        timerExpiryFunc[aloop].timerId,
                        userValue,
                        currentTimerMilli[timerExpiryFunc[aloop].timerId]);
#endif
                (timerExpiryFunc[aloop].procFunc) ((VgmuxChannelNumber)userValue);
            }
            found = TRUE;
        }
    }
}

/***************************************************************************
 * Processes
 ***************************************************************************/

/* END OF FILE */

