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

#ifndef PSC_API_H
#define PSC_API_H


/****************************************************************************
 * Include Files
 ****************************************************************************/

#include <system.h>
#include "nbiot_modem_common_config.h"

/****************************************************************************
 *  Defines
 *  CR WSAP00068585 Params Overhead  for deepsleep  in  public file
 *  2018.2.1  for shorten deep sleep restore timing: 5000->1000
 */
#define PSC_WORST_CASE_WAKE_UP_TIME_MS      1000

/****************************************************************************
 * Defines
 ****************************************************************************/

/* Modem entity sleep-related actions
 * NOTE: Further actions will be added to this enumeration in the future so
 *       modem entities should be implemented with this in mind (eg provide
 *       default handling for "unknown" actions)
 */
typedef enum PscSleepActionTag
{
    PSC_SLEEP_ACTION_SLEEP, /* Preserve modem entity context in preparation for deep sleep.*/
    PSC_SLEEP_ACTION_WAKEUP /* Restore modem entity context for warm wake-up*/
}
PscSleepAction;


/* System power states */
typedef enum PscPowerStateTag
{
    PSC_PWR_STATE_EDRX,
    PSC_PWR_STATE_OOS,
    PSC_PWR_STATE_PSM,
    PSC_PWR_STATE_INACTIVE,
    PSC_PWR_STATE_NO_SERVICE
}
PscPowerState;


/* PSC client identifiers */
typedef enum PscClientIdTag
{
    PSC_CLIENT_ABMM,
    PSC_CLIENT_ABPD,
    PSC_CLIENT_AP_BRIDGE,
    PSC_CLIENT_ATCI,
    PSC_CLIENT_EMM,
    PSC_CLIENT_ERRC,
    PSC_CLIENT_ESM,
    PSC_CLIENT_GKI_TIMER,
    PSC_CLIENT_L1,
    PSC_CLIENT_MUX,
    PSC_CLIENT_USIM,
    PSC_CLIENT_ULS,
    PSC_CLIENT_SIMAT
}
PscClientId;


/* Modem entity sleep callback function pointer type.
 * A modem entity which requires sleep-related functionality should implement a
 * function of this type. The function should perform any necessary sleep-related
 * action specified by the sleepAction parameter.
 *
 * The PSC will call each registered sleep callback function with the appropriate
 * sleepAction parameter value when a sleep-related action needs to be performed.
 * The PSC also provides the system power state as parameter powerState for possible
 * use within the modem entity.
 */
typedef void (*PscSleepCb)(PscSleepAction sleepAction,
                           PscPowerState powerState);


/* Reason for wake-up when power is restored */
typedef enum PscWakeupReasonTag
{
    PSC_COLD_BOOT,
    PSC_DEEP_SLEEP
}
PscWakeupReason;


/***************************************************************************
 * Data structures
 ***************************************************************************/

/* PSC deep sleep constraints are used to prevent deep sleep entry
 * in one or several power states.
 */
typedef enum PscConstraintTag
{
    PSC_CONSTRAINT_ALL,
    PSC_CONSTRAINT_EDRX,
    PSC_CONSTRAINT_PSM,
    PSC_CONSTRAINT_OOS,
    PSC_CONSTRAINT_INACTIVE,
    PSC_CONSTRAINT_NO_SERVICE
}
PscConstraint;


/***************************************************************************
 * Function prototypes
 ***************************************************************************/

/**
 * Description: Used to indicate start and end of system power state (PSM,
 *              eDRX silence period or OOS) to PSC.
 *
 *              NOTE : Two versions of the API are provided. The first (without
 *                     the "_INT" suffix) is to be used for calls from a GKI task
 *                     execution context. The second (with the "_INT" suffix)
 *                     is to be used for calls from a GKI interrupt execution
 *                     context.
 *
 * Parameters:  powerState - system power state being started/ended
 *
 *              isStart    - indicates start (true) or end (false) of
 *                           system power state
 *
 */
void psc_set_power_state(PscPowerState powerState, bool isStart);

void psc_set_power_state_INT(PscPowerState powerState, bool isStart);


/**
 * Description: Set or clear deep sleep constraint for modem entity in PSC
 *
 *              NOTE: By default there are no active constraints in place,
 *                    allowing deep sleep entry in all power states.
 *
 * Parameters:  clientId   - ID of modem entity
 *
 *              constraint - deep sleep constraint for modem entity
 *
 *              isSet      - indicates if constraint is being set (true) or
 *                           cleared (false)
 *
 */
void psc_set_constraint(PscClientId clientId, PscConstraint constraint, bool isSet);

void psc_set_constraint_INT(PscClientId clientId, PscConstraint constraint, bool isSet);


/**
 * Description: Registers the modem entity sleep callback function with the PSC.
 *              The PSC will call each registered sleep callback function when
 *              a sleep-related action needs to be performed.
 *
 * Parameters:  clientId   - ID of modem entity
 *
 *              sleepCb - pointer to sleep callback function for modem entity.
 *                        This function performs a sleep-related action within
 *                        the modem entity as specified by its sleepAction parameter.
 *
 *                        NOTE: GKI-based modem entities may need to internally
 *                              convert this synchronous (function-based)
 *                              call to an asynchronous (signal-based)
 *                              call to prevent disruption of any ongoing
 *                              processing.
 *
 */
void psc_register_sleep_callback(PscClientId clientId,
                                 PscSleepCb sleepCb);


/**
 * Description: Reserve active lock for modem entity in PSC. Deep sleep is
 *              only possible after all active locks are released.
 *
 *              NOTE: The purpose of the active lock is not to prevent sleep, but
 *                    to ensure all registered entities prepare for sleep. Use
 *                    psc_set_constraint() to prevent sleep.
 *
 * Parameters:  clientId   - ID of modem entity
 *
 */
void psc_reserve_active_lock(PscClientId clientId);


/**
 * Description: Release active lock for modem entity in PSC. This indicates that
 *              the modem entity is prepared for deep sleep. Deep sleep is only
 *              possible after all active locks are released.
 *
 *              NOTE: PSC will call sleep callback function for all modem
 *                    entities that have reserved an active lock using
 *                    psc_reserve_active_lock(). When its callback function is
 *                    invoked with sleepAction = PSC_SLEEP_ACTION_SLEEP, the
 *                    modem entity is expected to release its active lock using
 *                    psc_release_active_lock().
 *
 * Parameters:  clientId   - ID of modem entity
 *
 */
void psc_release_active_lock(PscClientId clientId);


/**
 * Description: Returns reason for wake-up when power is restored
 *
 * Return:      Reason for wake-up
 */
PscWakeupReason  psc_get_wakeup_reason(void);


/**
 * Description: Returns sleep opportunity duration
 *
 * Return:      Sleep opportunity in milliseconds
 *
 */
uint32_t psc_get_sleep_opportunity_duration(void);



#endif /* PSC_API_H */
