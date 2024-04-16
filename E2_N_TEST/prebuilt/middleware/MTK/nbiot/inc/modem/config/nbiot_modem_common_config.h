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
 * File Description:
 *
 *    Common configuration file for NB-IoT modem.
 *
 **************************************************************************/

#if !defined (NBIOT_MODEM_COMMON_CONFIG_H)
#define       NBIOT_MODEM_COMMON_CONFIG_H

/***************************************************************************
 * Table of contents:
 * 1. Permanent configuration items
 * 2. Temporary configuration items
 ***************************************************************************/



/***************************************************************************
 * 1. Permanent configuration items
 ***************************************************************************/

/* -------------------------------------------------------------------------
 *
 * Description : Define to switch on the use of the FAST signal interrupt
 *               queue functionality.
 *
 * When defined, The fast signal interrupt queue mechanism will be used.
 *
 * When not defined, The standard GKI send signal mechanism will be used.
 *
 * Restrictions : None
 *
 * Owner : MTK08904, Stuart Bennett
 *
 * References : None
 *
 * ---------------------------------------------------------------------- */

#define N1_USE_FAST_SIG_INT_QUEUE

/* -------------------------------------------------------------------------
* Description : Flag for combined procedures.
*
* When defined, combined procedures are in use
*
* When not defined, combined procedures are not in use
*
* Restrictions : Note! When this flag is activated, EMM combined procedures are
*                implemented according 3GPP release 13
*
* Owner : L4/EMM
*
*
* References : None
*
*
* ------------------------------------------------------------------------- */
#define FEA_COMBINED_PROCEDURES

/* -------------------------------------------------------------------------
* Description : Flag for activating LPP functionality.
*
* When defined, LPP is activated
*
* When not defined, LPP functionality is not taken into use
*
* Restrictions : -
*
* Owner : L4/LPP
*
*
* References : None
*
*
* ------------------------------------------------------------------------- */

#if defined(MTK_AGNSS_SUPL2_ENABLE)
#define FEA_LPP_SUPPORT
#define __LTE_OTDOA_SUPPORT__
#define __LTE_ECID_SUPPORT__
#endif
/* -------------------------------------------------------------------------
* Description : Flag for SimLock code removal.
*
* When defined, SimLock feature is active
*
* When not defined, SimLock related code will not be executed
*
* Restrictions : -
*
* Owner : L4/sim
*
*
* References : None
*
*
* ------------------------------------------------------------------------- */
#define FEA_SIMLOCK

/* -------------------------------------------------------------------------
* Description : Flag for PPP code removal.
*
* When defined, PPP feature is active
*
* When not defined, PPP related code will not be executed
*
* Restrictions : -
*
* Owner : L4
*
*
* References : None
*
*
* ------------------------------------------------------------------------- */
/* #define FEA_PPP */

/* -------------------------------------------------------------------------
* Description : Flag for MUX ring indicator status.
*
* When defined, MUX ring indicator status is support
*
* When not defined, MUX ring indicator status related code will not be executed
*
* Restrictions : -
*
* Owner : MUX
*
*
* References : None
*
*
* ------------------------------------------------------------------------- */
/* #define FEA_MUX_RING_INDICATOR */

/* -------------------------------------------------------------------------
* Description : Flag for Phonebook code removal.
*
* When defined, Phonebook feature is active
*
* When not defined, Phonebook related code will not be executed
*
* Restrictions : -
*
* Owner : Sim
*
*
*References : None
*
*
* ------------------------------------------------------------------------- */
/*#define FEA_PHONEBOOK*/
#if !defined( FEA_PHONEBOOK ) && !defined(ABSH_EXCLUDE_LM)
#define ABSH_EXCLUDE_LM
#endif /*  FEA_PHONEBOOK */

/* -------------------------------------------------------------------------
* Description : Flag for ACL code removal.
*
* When defined, APN Control List feature is active
*
* When not defined, APN Control List related code will not be executed
*
* Restrictions : -
*
* Owner : L4/SIM
*
*
* References : None
*
*
* ------------------------------------------------------------------------- */
/* #define FEA_ACL */


/* -------------------------------------------------------------------------
* Description : Flag for MT PDN Activation code removal.
*
* When defined, Mobile terminated PDN activation feature is active
*
* When not defined, Mobile terminated PDN activation related code will not be executed
*
* Restrictions : -
*
* Owner : L4
*
*
*References : None
*
*
* ------------------------------------------------------------------------- */
/*#define FEA_MT_PDN_ACT*/

/* -------------------------------------------------------------------------
* Description : Flag for QOS TFT code removal.
*
* When defined, QOS TFT feature is active
*
* When not defined, QOS TFT related code will not be executed
*
* Restrictions : -
*
* Owner : L4
*
*
*References : None
*
*
* ------------------------------------------------------------------------- */
#define FEA_QOS_TFT

/* -------------------------------------------------------------------------
* Description : Flag for dedicated bearer code removal.
*
* When defined, dedicated bearer feature is active
*
* When not defined, dedicated bearer related code will not be executed
*
* Restrictions : -
*
* Owner : L4
*
*
*References : None
*
*
* ------------------------------------------------------------------------- */
/* #define FEA_DEDICATED_BEARER */

/* -------------------------------------------------------------------------
* Description : Flag for A SLASH code removal.
*
* When defined, A SLASH feature is active
*
* When not defined, A SLASH related code will not be executed
*
* Restrictions : -
*
* Owner : L4
*
*
*References : None
*
*
* ------------------------------------------------------------------------- */
/*#define FEA_A_SLASH*/

/* -------------------------------------------------------------------------
* Description : Flag for SS functionality.
*
* When defined, SS code is executed.
*
* When not defined, SS related code will not be executed.
*
* Restrictions : -
*
* Owner : L4
*
*
*References : None
*
*
* ------------------------------------------------------------------------- */
/*#define FEA_SS*/

/* -------------------------------------------------------------------------
* Description : Flag for SMS test loop.
*
* When defined, SMS test loop code is executed.
*
* When not defined, SMS test loop code is not executed.
*
* Restrictions : -
*
* Owner : L4
*
*
*References : None
*
*
* ------------------------------------------------------------------------- */
#define FEA_SMS_TEST_LOOP

/* -------------------------------------------------------------------------
* Description : Flag for GPIO modem control.
*
* When defined, GPIO modem control code is executed.
*
* When not defined, GPIO modem control code is not executed.
*
* Restrictions : -
*
* Owner : L4
*
*
*References : None
*
*
* ------------------------------------------------------------------------- */
#define FEA_GPIO_MODEM_CONTROL

/* -------------------------------------------------------------------------
* Description : Flag for operator names.
*
* When defined, SE.13 operator names are in use.
*
* When not defined, SE.13 operator names are excluded.
*
* Restrictions : Note if this flag is enabled, then
*                a huge (>40 kB) table is taken into builds.
*
* Owner : L4
*
*
*References : None
*
*
* ------------------------------------------------------------------------- */
/*#define FEA_OPERATOR_NAMES*/

/* -------------------------------------------------------------------------
* Description : Flag for country initials.
*
* When defined, SE.13 country initials are in use.
*
* When not defined, SE.13 country initials are excluded.
*
* Restrictions : -
*
* Owner : L4
*
*
*References : None
*
*
* ------------------------------------------------------------------------- */
/*#define FEA_COUNTRY_INITIALS*/

/* -------------------------------------------------------------------------
* Description : Flag for 3GPP release 14.
*
* When defined, release 14 features are activated.
*
* When not defined, release 14 features are excluded.
*
* Restrictions : -
*
* Owner :
*
*
*References : None
*
*
* ------------------------------------------------------------------------- */
/*#define FEA_REL14*/

/* -------------------------------------------------------------------------
* Description : Flag for enabling/disabling decicated bearers.
*
* When defined,     Decicated bearers are in use
*
* When not defined, Decicated bearers are not in use
*
* Restrictions : -
*
*
* Owner : ABPD owner
*
* References : None
*
* ------------------------------------------------------------------------- */
/*#define FEA_DEDICATED_BEARERS*/

/***************************************************************************
 * 2. Temporary configuration items
 ***************************************************************************/


/* ---------------------------------------------------------------------- */
/* This is a temporary flag and should be removed once no longer needed!  */
/* ---------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * Description :
 *
 * When defined, ...
 *
 * When not defined, ...
 *
 * Restrictions :
 *
 * Can be enabled only if FEA_REL14 is defined
 * Shall be disabled in if...
 *
 * etc.
 *
 *
 * Owner :
 *
 * ...
 *
 * References :
 *
 * Can refer e.g. to 3GPP Spec/CR, operator/customer requirement,
 * CQ item, internal development milestone etc.
 *
 *
 * Conditions for removal :
 * This temporary flag can be removed when ....
 *
 * ------------------------------------------------------------------------- */

/* #define FEA_TEMP_ERATURE */

/* -------------------------------------------------------------------------
 *
 * Description : This flag is used to fake service. Stack is cut from
 *               EMM - ERRC interface and messages from ERRC are
 *               hardcoded so that EMM and layers above it are handled
 *               like device is in service.
 *
 * Restrictions : None
 *
 * Owner : MTK13577, Marko Kukkohovi
 *
 * References : None
 *
 * ---------------------------------------------------------------------- */
/* #define FEA_TEMP_APMD_FAKE_SERVICE */

/* -------------------------------------------------------------------------
 * Description : Temporary hacks needed in FPGA full stack call integration
 *
 * When defined, temporary hacks needed in FPGA full stack call
 *               integration are included in the build
 *
 * When not defined, temporary hacks needed in FPGA full stack call
 *               integration are not included in the build
 *
 * Restrictions : -
 *
 * Owner : Hannu Huusko
 *
 * NOTE: This flag enables the hacks only in ERRC SW. Additional modifications
 *        may be needed in other parts of the modem SW.
 *
 * Conditions for removal :
 * This temporary flag can be removed when the hacks are not needed any more.
 *
 * ------------------------------------------------------------------------- */

/* #define FEA_TEMP_ERRC_FPGA_CALL_INTEGRATION_HACKS */

/* -------------------------------------------------------------------------
 * Description : Temporary flag to add code for T2F_F2T HW error interrupt issue removal
 *
 * When defined, temporary workaround to hw error issue is applied
 *
 * When not defined,
 *
 * Restrictions : -
 *
 * Owner : David Metcalf, Eduard Oliver
 *
 * NOTE: None
 *
 * Conditions for removal : Noe
 *
 * ------------------------------------------------------------------------- */

#define FEA_TEMP_L1_T2F_F2T_HW_ERROR_WORKAROUND

/* -------------------------------------------------------------------------
 * Description : Temporary flag to change L1 SW tick behavior to match to 3GPP R14
 *               requirements.
 *
 * When defined, new tick design will be applied
 *
 * When not defined, old tick design is in use
 *
 * Restrictions : -
 *
 * Owner : Jukka Toivanen, Hujian Xu
 *
 * NOTE: None
 *
 * Conditions for removal : Noe
 *
 * ------------------------------------------------------------------------- */

#define FEA_TEMP_L1_RX_TO_TX_TRANSITION_REDESIGN


/* -------------------------------------------------------------------------
* Description : Temporary flag for L1 serving cell measurements via 240khz path
*
* When defined, this flag enables serving cell measurements via 240khz path
* for verification purposes. When serving cell is measured via both 238khz and
* 240khz path it is easy to verify if neighbor cell measurements are working as
* supposed. Flag can be disabled once product verification is in such phase that
* this is not needed any more.
*
* When not defined, serving cell is measured only via 238khz path
*
* Restrictions : -
*
* Owner : mtk09541 Antti Tormanen
*
*
* Conditions for removal :
* This temporary flag can be removed after verification
*
* ------------------------------------------------------------------------- */
#define FEA_TEMP_N1_CM_MEASURE_SERVING_CELL_VIA_240KHZ_PATH

/* -------------------------------------------------------------------------
 * Description : Enables functionality that is under development in the L1
 *               cell search sequencer
 *
 * When defined, partially tested/untested code is included in the build
 *
 * When not defined, code under development is not included in the build
 *
 * Restrictions : -
 *
 * Owner : Raija Kylmanen mtk13613
 *
 *
 * Conditions for removal :
 * This temporary flag can be removed when the functionality has been verified.
 *
 * ------------------------------------------------------------------------- */

//#define FEA_TEMP_L1_CS_SEQ_DEVELOPMENT

/* -------------------------------------------------------------------------
 * Description : Temporary flag for NB-IoT code removal
 *
 * When defined, temporary flag used when GSM application is neeeded
 *
 * When not defined, temporary flag removed only USIM application available
 *
 * Restrictions : -
 *
 * Owner : Marko Kukkohovi
 *
 * NOTE: None
 *
 * Conditions for removal : None
 *
 * ------------------------------------------------------------------------- */

/* #define FEA_TEMP_OBSOLETE_SIM_MANAGER */


/* -------------------------------------------------------------------------
 * Description : Temporary flag for enabling SMS abnormal cases functionality 24.301 ch.5.5.1.2.6A
 *
 * When defined, SMS abnormal cases functionality is enabled.
 *
 * When not defined, SMS abnormal cases functionality is not enabled.
 *
 * Restrictions : -
 *
 * Owner : Marko Kukkohovi
 *
 * NOTE: None
 *
 * Conditions for removal : When NW/testcases support SMS abnormal cases.
 *
 * ------------------------------------------------------------------------- */

/* #define FEA_TEMP_EMM_SMS_ABNORMAL_CASES */

/* -------------------------------------------------------------------------
 *
 * Description : This flag is used to enable hardwired control plane user
 *               data loopback.
 *               When enabled, DBM loops all downlink data received over
 *               control plane (ESM_DATA_TRANSPORT) back to uplink.
 *               The data will be looped unchanged.
 *
 * Restrictions : For testing purposes only
 *
 * Owner : DBM layer responsible
 *
 * Conditions for removal :
 * This temporary flag can be removed when the hack is not needed any more.
 *
 * ---------------------------------------------------------------------- */
/* #define FEA_TEMP_HARDWIRED_CP_LOOPBACK */


/* -------------------------------------------------------------------------
 * Description : Enables functionality that is under development in the Modem
 *               Power State Controller (PSC)
 *
 * When defined, partially tested/untested PSC code is included in the build
 *
 * When not defined, PSC code under development is not included in the build
 *
 * Restrictions : PSC functionality is disabled for all PC-based
 *                simulation builds
 *
 * Owner : Jonathan Plews
 *
 *
 * Conditions for removal :
 * This temporary flag can be removed when the functionality has been verified.
 *
 * ------------------------------------------------------------------------- */

#if !defined(MTK_NBIOT_SIMULATION_BUILD) && !defined(MTK_NBIOT_COSIM_BUILD) && !defined(NB_PC_HW_SIM) && !defined(NB_PC_L1_NET_SIM)
#define FEA_TEMP_PSC_DEVELOPMENT
#endif /* MTK_NBIOT_SIMULATION_BUILD */

/* -------------------------------------------------------------------------
 * Description : Temporary flag for MUX phase one retention
 *
 * When defined, MUX enables CSCI and single port channel retention
 *
 * When not defined, MUX disables CSCI and single port channel retention
 *
 * Restrictions : requires  FEA_TEMP_PSC_DEVELOPMENT to be enabled.
 *
 * Owner : Marko Kukkohovi
 *
 * NOTE: None
 *
 * Conditions for removal : When deep sleep functionality has been verified.
 *
 * ------------------------------------------------------------------------- */

#if defined(FEA_TEMP_PSC_DEVELOPMENT)
#define UPGRADE_MUX_RETENTION
#endif

/* -------------------------------------------------------------------------
 * Description : Temporary flag for MUX phase two retention
 *
 * When defined, MUX enables 27010 retention
 *
 * When not defined, enabling of 27010 prevents deep sleep. The retention is
 * also prevented when the single port connection is on data mode.
 *
 * Restrictions : requires  FEA_TEMP_PSC_DEVELOPMENT and UPGRADE_MUX_RETENTION
 * to be enabled.
 *
 * Owner : Marko Kukkohovi
 *
 * NOTE: This flag requires that UPGRADE_MUX_RETENTION and
 * FEA_TEMP_PSC_DEVELOPMENT are ON.
 *
 * Conditions for removal : When deep sleep functionality has been verified.
 *
 * ------------------------------------------------------------------------- */

#if defined(FEA_TEMP_PSC_DEVELOPMENT) && defined(UPGRADE_MUX_RETENTION)
#define UPGRADE_MUX_RETENTION_P2
#endif

/* -------------------------------------------------------------------------
 * Description : Temporary flag for enabling NB-IoT SIM Emulation
 *
 * When defined, used SIM emulation according previous flag config
 *
 * When not defined, SIM emulation is always disabled
 *
 * Restrictions : -
 *
 * Owner : Marko Kukkohovi
 *
 * NOTE: None
 *
 * Conditions for removal : None
 *
 * ------------------------------------------------------------------------- */

/* #define FEA_TEMP_INCLUDE_SIM_EMULATION */

/* -------------------------------------------------------------------------
* Description : Temporary flag for RLM and RX manager optimization.
*
* Restrictions : -
*
* Owner : Margit Kangas
*
* Conditions for removal :
* This temporary flag can be removed when the functionality has been verified.
*
* ------------------------------------------------------------------------- */
/* #define FEA_TEMP_RLM_TO_SUSPEND */

/* -------------------------------------------------------------------------
 * Description : Disables RF control interface from AFC to RF. == New filtered AFC is not send to RF/DFE
 *
 * Restrictions : -
 *
 * Owner : Mika Heikkinen
 *
 * Conditions for removal :
 * This temporary flag can be removed when the functionality has been verified.
 *
 * ------------------------------------------------------------------------- */
#if defined(MTK_NBIOT_TARGET_BUILD)
  /* #define AFC_DISABLE_RF_CONTROL_IF_SEND */
#endif

/* -------------------------------------------------------------------------
 * Description : Disables DSP control interface from AFC to DSP. == No AFC related measurement controls send to DSP
 *
 * Restrictions : -
 *
 * Owner : Mika Heikkinen
 *
 * Conditions for removal :
 * This temporary flag can be removed when the functionality has been verified.
 *
 * ------------------------------------------------------------------------- */

/*#define AFC_DISABLE_DSP_CONTROL_IF_SEND*/

/* -------------------------------------------------------------------------
* Description : Temporary flag for OTDOA implementation and integration
*
* When defined, L1 OTDOA sequencer is active
*
* When not defined, L1 OTDOA sequencer and related code will not be executed
*
* Restrictions : -
*
* Owner : Teemu Olkkola
*
*
* Conditions for removal :
* This temporary flag can be removed when OTDOA functionality has been tested
* on such level it can be enabled as default.
*
* ------------------------------------------------------------------------- */
/* #define FEA_TEMP_L1_OTDOA */

/* -------------------------------------------------------------------------
* Description : flag for enabling an interface between MD sleep driver and PSC
*
* When defined,  N1 uses PSC api functions to control its states
*
* When not defined, N1 uses do not use PSC api functions
*
* Restrictions : -
*
* Owner : Matti Ylitarvas
*
*
 * NOTE: None
 *
 * Conditions for removal : None
 *
 * ------------------------------------------------------------------------- */
 #define FEA_ENABLE_N1_MD_SLEEP_DRIVER_PSC_API

/* -------------------------------------------------------------------------
* Description : flag for allowing the deep sleep of L1
*
* When defined,  deep sleep is allowed by L1
*
* When not defined, deep sleep is not allowed by L1
*
* Restrictions : -
*
* Owner : Matti Ylitarvas
*
*
 * NOTE: None
 *
 * Conditions for removal : None
 *
 * ------------------------------------------------------------------------- */

#define FEA_N1_DEEP_SLEEP_ALLOWED

/* -------------------------------------------------------------------------
* Description : flag for disabling GCF test mode
*
* When defined,  GCF test mode disabled (normal operation)
*
* When not defined, GCF test mode enabled (for GCF testing only)
*
* Restrictions : -
*
* Owner : Steve Barrett
*
*
 * NOTE: None
 *
 * Conditions for removal : None
 *
 * ------------------------------------------------------------------------- */

#if !defined (DISABLE_ATCI_LTE_GCF_SUPPORT)
#define DISABLE_ATCI_LTE_GCF_SUPPORT
#endif

/* -------------------------------------------------------------------------
 *
 * Description :  flag for controlling the STUB of BG PLMN search.
 *                     if STUB is enabled, PS shall not trigger RA but stay in idle state, and L1 shall
 *                     trigger BG PLMN search when enter idle state with pre-defined parameters.
 * When defined. this feature is switched on
 * When not defined. this featue is switched off
 *
 * Restrictions : None
 *
 * Owner : Yingqi Liu
 *
 * References : None
 *
 * ---------------------------------------------------------------------- */
/* #define N1_BG_PLMN_SEARCH_STUB */

/* -------------------------------------------------------------------------
* Description :
* When defined,  support subscribe signal activity (for psc integration)
*
* When not defined, related code will not be included(normal mode)
*
* Restrictions : Requires  FEA_TEMP_PSC_DEVELOPMENT to be enabled.
*
* Owner : Richard Cui
*
* NOTE: None
*
* Conditions for removal : After psc integration done, it should be removed
*
* ------------------------------------------------------------------------- */

#if defined(FEA_TEMP_PSC_DEVELOPMENT)
#define KI_ENABLE_SUBSCRIBE_EVENT_SIGNAL
#endif /* FEA_TEMP_PSC_DEVELOPMENT */

/* -------------------------------------------------------------------------
* Description :
* When defined,  support subscribe timer expiry activity (for psc integration)
*
* When not defined, related code will not be included(normal mode)
*
* Restrictions : Requires  FEA_TEMP_PSC_DEVELOPMENT to be enabled.
*
* Owner : Richard Cui
*
* NOTE: None
*
* Conditions for removal : After psc integration done, it should be removed
*
* ------------------------------------------------------------------------- */

#if defined(FEA_TEMP_PSC_DEVELOPMENT)
#define KI_ENABLE_SUBSCRIBE_EVENT_TIMER
#endif /* FEA_TEMP_PSC_DEVELOPMENT */

/* -------------------------------------------------------------------------
* Description :
* When defined,  SIG_INTIALISE will be sent to all task when bootup to notify the
*                bootup reason (for psc integration)
*
* When not defined, related code will not be included(normal mode)
*
* Restrictions : -
*
* Owner : Richard Cui
*
* NOTE: None
*
* Conditions for removal : After psc integration done, it should be removed
*
* ------------------------------------------------------------------------- */
/* #define KI_ENABLE_BOOTUP_NOTIFY */

/* -------------------------------------------------------------------------
* Description :
* When defined,  active timer before deep sleep  will be saved into retention memory
*              next bootup will re-active these timers
*
* When not defined, related code will not be included(normal mode)
*
* Restrictions : Requires  FEA_TEMP_PSC_DEVELOPMENT to be enabled.
*
* Owner : Richard Cui
*
* NOTE: None
*
* Conditions for removal : After psc integration done, it should be removed
*
* ------------------------------------------------------------------------- */

#if defined(FEA_TEMP_PSC_DEVELOPMENT)
#define TEMP_TIME_SAVING_DEVELOPMENT
#endif /* FEA_TEMP_PSC_DEVELOPMENT */

/* -------------------------------------------------------------------------
 * Description : Prevent entry to light sleep (during development phase)
 *
 * When defined, HAL sleep manager will be prevented from entering light sleep
 * by applying a suitable sleep lock.
 *
 * When not defined, HAL sleep manager will be allowed to enter light sleep.
 *
 * Restrictions : -
 *
 * Owner : Jonathan Plews
 *
 *
 * Conditions for removal :
 * This temporary flag can be removed when all sleep functionality has been
 * verified.
 *
 * ------------------------------------------------------------------------- */

/* Disable now by default. Use AT+SM=LOCK to disable LIGHT and DEEP sleep
#define FEA_TEMP_DISABLE_LIGHT_SLEEP
*/


/* -------------------------------------------------------------------------
 * Description : Prevent entry to deep sleep (during development phase)
 *
 * When defined, HAL sleep manager will be prevented from entering deep sleep
 * by applying a suitable sleep lock.
 *
 * When not defined, HAL sleep manager will be allowed to enter deep sleep.
 *
 * Restrictions : -
 *
 * Owner : Jonathan Plews
 *
 *
 * Conditions for removal :
 * This temporary flag can be removed when all sleep functionality has been
 * verified.
 *
 * ------------------------------------------------------------------------- */

/* Disable now by default. Use AT+SM=LOCK to disable LIGHT and DEEP sleep
#define FEA_TEMP_DISABLE_DEEP_SLEEP
*/

/* -------------------------------------------------------------------------
* Description : flag for enabling time drifing compensation when L1 wakes up from sleep.
*
* When defined,  Compensation will do every nth time of ten according to bitmap below.
* Default values are set to:
*                            #define COMPENSATE_1M92_TICKS        -1
*                            #define COMPENSATION_BITMAP          0x2B5  -> 1-0-1-0-1-1-0-1-0-1
*
* When not defined, we will not do the compensation.
*
* Restrictions : -
*
* Owner : Matti Ylitarvas
*
*
 * NOTE: None
 *
 * Conditions for removal : None
 *
 * ------------------------------------------------------------------------- */

#define FEA_N1_SLEEP_COMPENSATE_TIME_DRIFTING


/* -------------------------------------------------------------------------
* Description : flag for enabling L1-EL2 interface for corrected Scheduling
*               Request handling.
*
* When defined, updated L1-EL2 interface is used.
*
* Restrictions : -
*
* Owner : Antti Karjalainen
*
* NOTE: None
*
* Conditions for removal :
* Can be removed when updated L1-EL2 interface functionality has been verified.
*
* ------------------------------------------------------------------------- */
#define FEA_TEMP_NPHY_TRANSMIT_RSP

/* -------------------------------------------------------------------------
* Description : flag for enabling t-reordering timer workaround
*
* When defined,  RLC does not start the t-reordering timer if we do not detect the new RX SN gap
* When not defined, RLC will start the t-reordering timer if vrH > vrR
*
* Restrictions : -
*
* Owner : yulan.liu
*
*
 * NOTE: None
 *
 * Conditions for removal : None
 *
 * ------------------------------------------------------------------------- */
#ifndef UE_SIMULATOR
//#define GCF_EL2_RLC_T_REORDERING_WORKAROUND
#endif /* UE_SIMULATOR */

/* -------------------------------------------------------------------------
* Description : This flag is needed for syncing interface change to
*               ESM-EMM unit data ind message.
*
* When defined,  If this flag is enabled data messages in ESM-EMM interface
*               use l3Message as pointer instead of static array.
*
* Restrictions : -
*
* Owner : Jutta Liuska
*
*
 * NOTE: None
 *
 * Conditions for removal : When functionality has been tested properly.
 *
 * ------------------------------------------------------------------------- */
/* #define FEA_TEMP_EMM_ESM_INTERFACE_CHANGE */

/* -------------------------------------------------------------------------
 * Description : Flag for changing NAS message delivery in ERRC-EMM interface
 *               indication messages.
 *
 * When defined, NAS message in ERRC-EMM indication messages is allocated
 * dynamically.
 *
 * When not defined, NAS message in ERRC-EMM indication messages is in
 * static array.
 *
 * Restrictions : -
 *
 * Owner : Hannu Huusko
 *
 * NOTE: None
 *
 * Conditions for removal :
 * Can be removed when updated ERRC-EMM interface functionality has been verified.
 *
 * ------------------------------------------------------------------------- */
#define FEA_TEMP_ERRC_EMM_NAS_MESSAGE_IF_IND

/* -------------------------------------------------------------------------
 * Description : Flag for changing NAS message delivery in ERRC-EMM interface
 *               request messages.
 *
 * When defined, NAS message in ERRC-EMM request messages is allocated
 * dynamically.
 *
 * When not defined, NAS message in ERRC-EMM request messages is in
 * static array.
 *
 * Restrictions : -
 *
 * Owner : Hannu Huusko
 *
 * NOTE: None
 *
 * Conditions for removal :
 * Can be removed when updated ERRC-EMM interface functionality has been verified.
 *
 * ------------------------------------------------------------------------- */
#define FEA_TEMP_ERRC_EMM_NAS_MESSAGE_IF_REQ

/* -------------------------------------------------------------------------
 * Description : Flag for changing NAS message delivery in EMM-ESM interface
 *               request messages.
 *
 * When defined, NAS message in EMM-ESM request messages is allocated
 * dynamically.
 *
 * When not defined, NAS message in EMM-ESM request messages is in
 * static array.
 *
 * Restrictions : -
 *
 * Owner : Marko Akselin
 *
 * NOTE: None
 *
 * Conditions for removal :
 * Can be removed when updated ERRC-EMM-ESM interfaces functionality has been verified.
 *
 * ------------------------------------------------------------------------- */
/* #define FEA_TEMP_EMM_ESM_INTERFACE_CHANGE */

/* -------------------------------------------------------------------------
* Description : flag for enabling automatic NPHY_SIGNAL_LEVEL_IND message
*               sending from L1 to ABMM.
*
* When defined, updated L1-ABMM interface is used.
*
* Restrictions : -
*
* Owner : Pekka Karjaluoto mtk13612
*
* NOTE: None
*
* Conditions for removal :
* Can be removed when updated L1-ABMM interface functionality has been verified.
*
* ------------------------------------------------------------------------- */
#define FEA_TEMP_NPHY_SIGNAL_LEVEL_IND

/* -------------------------------------------------------------------------
* Description :
* When defined,  ULS driver will shut down mdsys off pd when no trace is active.
*
*
* When not defined, no effect
*
* Restrictions : Requires  FEA_TEMP_PSC_DEVELOPMENT to be enabled.
*
* Owner : Jude Fang
*
* NOTE: None
*
* Conditions for removal : After psc integration done, it should be removed
*
* ------------------------------------------------------------------------- */

#if defined(FEA_TEMP_PSC_DEVELOPMENT)
#define TEMP_ULS_LOW_POWER_DEVELOPMENT
#endif /* FEA_TEMP_PSC_DEVELOPMENT */

/* -------------------------------------------------------------------------
* Description :
* When defined,  EL2 will deliver only two data packets to upper layers and
*                then EL2_TASK is put to sleep. This gives upper layers time
*                to process the received DL data packets.
*
*
* When not defined, no effect
*
* Restrictions : -
*
* Owner : Tero Miettinen
*
* NOTE: None
*
* Conditions for removal : After feature is tested to be working or replaced
*                          by some other feature.
*
* ------------------------------------------------------------------------- */
#define FEA_TEMP_DL_RLC_SDU_DELIVERY

/* -------------------------------------------------------------------------
* Description :
* When defined,  Access technology check is done for user- and operator
*                controlled PLMN lists.
*
*
* When not defined, All PLMN in the list are preferred
*
* Restrictions : -
*
* Owner : Harri Mntyniemi
*
* NOTE: None
*
* Conditions for removal : When this feature is required.
*
* ------------------------------------------------------------------------- */
/*#define FEA_TEMP_ACCESS_TECHNOLOGY_CHECK*/

/* -------------------------------------------------------------------------
* Description : Temporary flag for L2 Tput statistics
*
* When defined, L1 grant and L2 RLC effective data volume on UL/DL will be
* accumulated
*
* When not defined, no such Tput statistics
*
* Restrictions : -
*
* Owner : Alfonso Zhou
*
*
* Conditions for removal :
* This temporary flag can be removed when a corresponding function is available
* through AT command.
* ------------------------------------------------------------------------- */
#define FEA_TEMP_EXTRA_L2_TPUT_TRACING

/* -------------------------------------------------------------------------
* Description : Temporary flag for DCN feature in case if this feature will be
*               cherry picked before all REL14 features are activated.
*
* When defined, Dedicated Core Network feature is activated.
*
* When not defined, Dedicated Core Network feature is excluded.
*
* Restrictions : Feature is activate if FEA_REL14 is activated
*
*
* Owner : EMM
*
*References : 3GPP TS 24.301 (release 14) chapters 8.2.1.15, 8.2.16.3,
*             8.2.26.18 and 9.9.3.48. 3GPP TS 24.008 chapter 10.5.5.35
*
* ------------------------------------------------------------------------- */
#ifdef FEA_REL14
#define FEA_TEMP_REL14_DCN
#endif

/* -------------------------------------------------------------------------
* Description : Temporary flag for Security functions HASH feature in case if
*               this feature will be cherry picked before all REL14 features
*               are activated.
*
* When defined, HASH feature in Security Mode Command procedure is activated.
*
* When not defined, HASH feature in Security Mode Command procedure is excluded.
*
* Restrictions : Feature is activated if FEA_REL14 is activated
*
*
* Owner : EMM
*
*References : 3GPP TS 24.301 (release 14) chapters 5.4.3.2, 5.4.3.3,
*             and 5.4.3.4.
*
* ------------------------------------------------------------------------- */
#ifdef FEA_REL14
#define FEA_TEMP_REL14_HASH
#endif

/* -------------------------------------------------------------------------
* Description : Temporary flag for Restriction on use of enhanced coverage
*
* When defined, Restriction on use of enhanced coverage is activated.
*
* When not defined, Restriction on use of enhanced coverage is excluded.
*
* Restrictions : Feature is activated if FEA_REL14 is activated
*
*
* Owner : EMM
*
*References : 3GPP TS 24.301 (release 14) chapters 5.3.16, 5.5.1.2.2,
*             and 5.5.1.2.4.
*
* ------------------------------------------------------------------------- */
#ifdef FEA_REL14
#define FEA_TEMP_REL14_EMM_REST_EC
#endif

/* -------------------------------------------------------------------------
* Description : Temporary flag for EMM Rel14 common changes
*
* When defined, EMM R14 common changes are activated
*
* When not defined, EMM R14 common changes are excluded.
*
* Restrictions : Feature is activated if FEA_REL14 is activated
*
* Owner : EMM
*
*References : 3GPP TS 24.301 (release 14)
*
* ------------------------------------------------------------------------- */
#ifdef FEA_REL14
#define FEA_TEMP_REL14_EMM_COMMON
#endif

/* -------------------------------------------------------------------------
* Description : Temporary flag for CP data congestion control
*
* When defined, EMM CP data congestion control feature is activated
*
* When not defined, EMM CP data congestion control changes are excluded.
*
* Restrictions : Feature is activated if FEA_REL14 is activated
*
* Owner : EMM
*
*References : 3GPP TS 24.301 (release 14)
*
* ------------------------------------------------------------------------- */
#ifdef FEA_REL14
/* #define FEA_TEMP_REL14_EMM_CP_CONGESTION_CONTROL */
#endif

/* -------------------------------------------------------------------------
* Description : Flag for temporarily hiding new Call control implementation
*
* When defined, ABPD asks PDN connection modification from SIM before connection activation.
*
* When not defined, ABPD starts PDN connection activation without SIM modification.
*
* Restrictions : -
*
* Owner : ABPD
*
* ------------------------------------------------------------------------- */
/*#define FEA_TEMP_PDN_CONN_SIM_MODIFICATION*/

/* -------------------------------------------------------------------------
* Description : Temporary flag for handling Session Management Request for UE configured for dual priority.
*
* When defined, UE can send PDN CONNECTIVITY REQUEST message  with 'MS is not configured for NAS signalling low priority'
* if timer T3396 is running for same APN and running for reject cause #26.
*
* When not defined, UE rejects new PDN connectivity request for a  specific  APN if T3396 is running.
*
* Restrictions : Feature is activated if FEA_REL14 is activated
*
*
* Owner : ABPD
*
*References : 3GPP TS 24.301 (release 14) chapters 6.5.5.
*
*
* ------------------------------------------------------------------------- */
#define FEA_TEMP_REL14_ABPD_DUAL_PRIORITY

/* -------------------------------------------------------------------------
* Description :
* When defined, NBMM DL memory handling is improved in such way that lifecycle
*               of SIMA memory blocks is shortened. This is expected to improve
*               NBMM DL memory utilisation in scenarios where UE buffers
*               received RLC PDU's in RLC Rx window.
*
* Restrictions : -
*
* Owner : Antti Karjalainen
*
* NOTE: None
*
* Conditions for removal : After feature is tested to be working or replaced
*                          by some other feature.
*
* ------------------------------------------------------------------------- */
#define FEA_TEMP_DL_NBMM_REDESIGN

/* -------------------------------------------------------------------------
* Description : Temporary flag for UL flow control. This flag is common for
*               DBM and EL2.
*
* When defined, UL flow control is enabled
*
* When not defined, no UL flow control
*
* Restrictions : -
*
* Owner : Tero Miettinen
*
*
* Conditions for removal : After feature is tested to be working or replaced
*                          by some other feature.
* ------------------------------------------------------------------------- */
#define FEA_TEMP_DBM_EL2_UL_FLOW_CONTROL

/* -------------------------------------------------------------------------

* Description : flag for enabling SR delay for RRC release ACKworkaround
*
* When defined,  if the Rlc status report for RRC release trigger the BSR, we will dealay th SR
*
* Restrictions : -
*
* Owner : yulan.liu
*
*
 * NOTE: None
 *
 * Conditions for removal : None
 *
 * ------------------------------------------------------------------------- */
#define SR_DELAY_FOR_RRC_RELEASE_ACK_WORKAROUND

/* -------------------------------------------------------------------------
* Description : flag for enabling SR delay when have conflict between PRACH and NPUSCH
*
* When defined,  when have have conflict between PRACH and NPUSCH, L1 will cancel the SR
*
* and provide the time to L2 for next SR
*
* Restrictions : -
*
* Owner : yulan.liu
*
*
 * NOTE: None
 *
 * Conditions for removal : None
 *
 * ------------------------------------------------------------------------- */
#define SR_DELAY_PRACH_CONFILCT_WORKAROUND


/* -------------------------------------------------------------------------
* Description : For enabling accelarated AFC reaction when fast frequency changes(temperature critical periods).
*
* Pros: Calculated cumulative frequency error should follow real frequency error more agile.
* Cons: Can cause fluctuation in DSP measurement due too aggressive control(may need parameter polishing).

* Restrictions : -
*
* Owner : Mika Heikkinen
*
*
* Conditions for removal :
* When functionality desing is complete.
*
* ------------------------------------------------------------------------- */
#define AFC_ACCELARATED_ALFALOOP
/*
* Description :2HARQ FEATURE for testing
*
* When defined, Connected state will support for up to 2 harq processes;
*
* When not defined, Connected state will support only 1 harq processes.
*
* Restrictions : -
*
* Owner : Ron Zhang
*
*
* Conditions for add :
* This  flag can be added only when 2HARQ feature shall be tested.
*
* ------------------------------------------------------------------------- */
//#define FEA_2HARQ

/* -------------------------------------------------------------------------
* Description : Temporary flag for ECID positioning implementation and integration
*
* When defined, L1 ECID measurements are active
*
* When not defined, L1 ECID related code will not be executed
*
* Restrictions : -
*
* Owner : Teemu Olkkola
*
*
* Conditions for removal :
* This temporary flag can be removed when ECID functionality has been tested
* on such level it can be enabled as default.
*
* ------------------------------------------------------------------------- */
#define FEA_TEMP_L1_ECID

/* -------------------------------------------------------------------------
* Description : Temporary flag for NDI toggling after RA compatible modification
*
* When defined, L1 could compatible for most NDI toggling issue
*
* When not defined, L1 modification is not enabled.
*
* Restrictions : -
*
* Owner : Jay.Jiang
*
*
* Conditions for removal :
* This temporary flag can be removed when the modification is considered as no side effect.
*
* ------------------------------------------------------------------------- */
#define FEA_TEMP_L1_NDI_TOGGLE_COMP

/* -------------------------------------------------------------------------
 * Description : Flag for enabling ERRC operator check functions.
 *
 * When defined, operator check functions are available.
 *
 * When not defined, operator check functions are not available.
 *
 * Restrictions : -
 *
 * Owner : Hannu Huusko
 *
 * NOTE: None
 *
 * Conditions for removal :
 * Can be removed if operator check is taken into use or it is known not to be
 * needed.
 *
 * ------------------------------------------------------------------------- */
/* #define FEA_TEMP_ERRC_OPERATOR_CHECK */

/* -------------------------------------------------------------------------
* Description : Temporary flag for PDN access control.
*               PDN access control includes handling of T3396, Back-off timer
*               and various ESM causes received from NW that require the
*               device to apply back-off algorithms for APN+PLMN combination.
*
* When defined, PDN access control will apply.
*
* When not defined, PDN access control will not apply
*
* Restrictions : -
*
* Owner : ABPD owner
*
*
* Conditions for removal : After feature is tested to be working or replaced
*                          by some other feature.
* ------------------------------------------------------------------------- */
#define FEA_TEMP_PDN_ACCESS_CONTROL

/* -------------------------------------------------------------------------
* Description : Flag for enabling of Control Plane ROHC on ESM/EMM.
*
* When defined, CP ROHC functionality is available.
*
* When not defined, CP ROHC functionality is not available.
*
* Restrictions : -
*
* Owner : ESM
*
*
*References : None
*
*
* ------------------------------------------------------------------------- */
/* #define FEA_CP_ROHC */

/* -------------------------------------------------------------------------
* Description : Flag for enabling of User Plane ROHC on PDCP.
*
* When defined, UP ROHC functionality is available.
*
* When not defined, UP ROHC functionality is not available.
*
* Restrictions : -
*
* Owner : Shanlei.Kan
*
*
*References : None
*
*
* ------------------------------------------------------------------------- */
/* #if !defined(FEA_UP_ROHC)&&!defined(FPGA_ENV)//FPGA build does not support ROHC due to VRAM restrictions
#define FEA_UP_ROHC
#endif */

/* -------------------------------------------------------------------------
* Description : Flag for enabling interface for receiving and reporting TA and idleState to SIMAT.
*
* When defined, L1 could enable SIMAT TA reproting.
*
* When not defined, L1 not support SIMAT TA reporting.
*
* Restrictions : -
*
* Owner : Ron.Zhang
*
*
*References : None
*
*
* ------------------------------------------------------------------------- */

//#define FEA_SIMAT_TA
/* -------------------------------------------------------------------------
* Description : Flag for enabling NB2 2HARQ test with ZTE eNB when FEA_2HARQ is opened.
*
* When defined, If FEA_2HARQ is enabled, L1 could directly enable 2HARQ scheduling no matter what is the 2HARQ configuration.
*
* When not defined, If FEA_2HARQ is enabled,L1 2HARQ shceduling on or off depends on 2HARQ configuration from eNB.
*
* Restrictions : -
*
* Owner : Ron.Zhang
*
*
*References : None
*
*
* ------------------------------------------------------------------------- */

//#define TEMP_FEA_2HARQ_ZTE

/* -------------------------------------------------------------------------
* Description : Flag for temp workaround for NB2 2HARQ test.
*
* When defined, open the workaround used for supporting for 2harq test.
*
* When not defined, disable the workaround used for supporting for 2harq test.
*
* Restrictions : -
*
* Owner : Ron.Zhang
*
*
*References : None
*
*
* ------------------------------------------------------------------------- */

//#define TEMP_FEA_2HARQ_MTK

/* -------------------------------------------------------------------------
* Description : Flag for enabling CS TD buffer dumping.
*
*
* Restrictions : Shall not enable the flag unless required.
*
* Owner : Jacky.Wu
*
*
*References : None
*
*
* ------------------------------------------------------------------------- */

//#define DUMP_TD_BUFFER_40MS


/* -------------------------------------------------------------------------
* Description : Flag for temperature compemsation
*
* When defined, enable temperature compensation.
*
* Restrictions : -
*
* Owner : Asher.Huang
*
*
*References : None
*
*
* ------------------------------------------------------------------------- */
#define TEMP_FEA_TFC



/* -------------------------------------------------------------------------
* Description : Flag for temperature compemsation only in ics
*
* When defined, enable temperature compensation only in ics.
*
* Restrictions : -
*
* Owner : Albus.Yuan
*
*
*References : None
*
*
* ------------------------------------------------------------------------- */
// Enable TFC for BC66_NA with NTC driver VIO18 version
#if defined(MTK_BC66_NA_QORVO_ENABLE) && defined(NBIOT_THERM_REF_POWER_VIO18)
//#define TEMP_FEA_TFC_ONLY_IN_ICS
#else
#define TEMP_FEA_TFC_ONLY_IN_ICS
#endif

/* -------------------------------------------------------------------------
* Description : Flag for use AFC calibration temperature as the TFC start point
*
* When defined, use the temperature record in AFC calibration as the TFC start point.
*
* Restrictions : -
*
* Owner : Alex.Zhu
*
*
*References : None
*
*
* ------------------------------------------------------------------------- */
#define TEMP_FEA_TFC_USE_AFC_CAL_TEMP_AS_START_POINT

/* -------------------------------------------------------------------------
* Description : OTFC related macro
*
* When defined, the OTFC feature is applyed
*
*
* Restrictions : Only on TSX crystal
*
* Owner : OTFC related owner
*
*
* References : None
*
* ------------------------------------------------------------------------- */
//#define TEMP_FEA_OTFC


#define FEA_DTAG
/* -------------------------------------------------------------------------

* Description : Flag for Deutsche Telekom Network Friendly Mode(NFM) feature.
*
* When defined, NFM features(start timer and operator specific back-off timer)
*               can be enabled
*
* When not defined, NFM features are not in use.
*
* Restrictions : -
*
* Owner : Marko Kukkohovi
*
*
*References : None
*
*
* ------------------------------------------------------------------------- */
#ifdef FEA_DTAG
#define FEA_NFM
#endif /* FEA_DTAG */

/* -------------------------------------------------------------------------
* Description : Temporary flag for retx msg5 when grant received and the
* whole message5 have been sent but no status report received.
*
* When defined, message 5 retx optimization enabled.
*
* When not defined, no message 5 retx optimization
*
* Restrictions : -
*
* Owner : Andrew.Shi
*
*
* Conditions for removal : After feature is tested to be working or replaced
*                          by some other feature.
* ------------------------------------------------------------------------- */
#define FEA_TEMP_EL2_MSG5_RETX_OPT

/* -------------------------------------------------------------------------
* Description : Flag for L1 customer HSL traces
*
* When defined, enables the trace changes for L1 customer HSL traces.
*
* Restrictions : Enabling the flag needs to be synced with tools (Wenfu)
*
* Owner : Jari Korva, Juha Tuomainen
*
* References : None
*
* ------------------------------------------------------------------------- */
#define FEA_TEMP_L1_CUSTOMER_HSL_TRACES

/* -------------------------------------------------------------------------
* Description : Temporary flag for eSIM testing
*
* When defined, Emulation SIM could be enabled by AT*MEMUSIM. Because there is no
*
* card slot in the module with embeded SIM card.
*
* Restrictions : -
*
* Owner : SIM
*
* References : None
*
* ------------------------------------------------------------------------- */
#ifdef MTK_NBIOT_TARGET_BUILD
#define FEA_TEMP_ESIM_EMULATION
#endif

/* -------------------------------------------------------------------------
* Description : Flag for optimization for high speed connected RA.
*
* When defined, enable optimization.
*
* Restrictions : -
*
* Owner : Hujian.Xu
*
*
*References : None
*
*
* ------------------------------------------------------------------------- */
#define TEMP_FEA_HIGH_SPEED_CS_RA_OPT

/* -------------------------------------------------------------------------
* Description : Flag for Tx timing loop
*
* When defined, enables the feature of Tx timing loop.
*
* Restrictions : None
*
* Owner : Yingqi Liu
*
* References : None
*
* ------------------------------------------------------------------------- */
#define FEA_TEMP_L1_TX_TIMING_LOOP

/* -------------------------------------------------------------------------
* Description : Flag for Deutsche Telekom/TMO Radio Policy Manager (RPM) feature.
*
* When defined, RPM features(limit max number of attach retry, pdn connectivity reqest per hour)
*               can be enabled
*
* When not defined, RPM features are not in use.
*
* Restrictions : -
*
* Owner : Ziquan YU
*
*
* References : None
*
*
* ------------------------------------------------------------------------- */
#define FEA_RPM

/* -------------------------------------------------------------------------
* Description : Flag for UDP Packet Delivery Indication Request (UPDIR) feature.
*
* When defined, UPDIR features  can be enabled
*
* When not defined, UPDIR features are not in use.
*
* Restrictions : -
*
* Owner : Anson Feng
*
*
* References : None
*
*
* ------------------------------------------------------------------------- */
#define FEA_UPDIR

/* -------------------------------------------------------------------------
* Description : Flag for LGUPLUS Initial Subsription feature.
*
* When defined, hardcoded SMS-PP Download request will be sent to unsubscribed
* LGUPLUS USIM, then BIP will be used for subscription.
*
* Restrictions : -
*
* Owner : Qiming Li
*
* References : None
*
* ------------------------------------------------------------------------- */
#define FEA_KOR_LGUP

/* -------------------------------------------------------------------------
* Description : configure SSS in MIB/SIB1 CM to fater AFC loop
*
* When defined, enables the feature of  SSS in MIB/SIB1 CM.
*
* Restrictions : None
*
* Owner : Scott.Song
*
* References : None
*
* ------------------------------------------------------------------------- */
#define N1_ALG_CM_SSS_ENABLE_CFG

/* -------------------------------------------------------------------------
* Description : Temporary flag for trigger RLF when continues invalid status PDU received.
*
* When defined, RLF will be triggered if continues invalid status PDU received.
*
* When not defined, RLF will not be triggered
*
* Restrictions : -
*
* Owner : Andrew.Shi
*
*
* Conditions for removal : After feature is tested to be working or replaced
*                          by some other feature.
* ------------------------------------------------------------------------- */
#define FEA_TEMP_EL2_INVALID_STATUS_PDU_TRIGGER_RLF

/* -------------------------------------------------------------------------
* Description : Crash dumps via HSL/printf.
*
* Due to some historical reasons, L1 was dumping crash information using both
* HSL and printf. However, currently only printf is useful as UART output is
* saved to CoreDump file by Genie. HSL traces end up nowhere as ULS buffer
* is not dumped at the end of the process.
*
* Restrictions : -
*
* Owner : jari.korva
*
* Conditions for removal of CODE/flags: Confirm if UART/HSL is used for dumping
*                                       and remove the obselete code.
* ------------------------------------------------------------------------- */
//#define FEA_TEMP_L1_CRASH_DUMP_HSL
//#define FEA_TEMP_L1_CRASH_DUMP_PRINTF
#define FEA_TEMP_L1_CRASH_DUMP_PRINTF_NEW

/* -------------------------------------------------------------------------
* Description : Flag for enabling CC scheme 3 in L1
*
* When defined, takes CC scheme 3 implementation into the build. Note that the
* feature is enabled/disabled by a flag in n1_sq_npdxxh.h even when this flag
* is defined here.
*
* Restrictions : -
*
* Owner : Juha Tuomainen
*
*
*References : None
*
*
* ------------------------------------------------------------------------- */
//#define FEA_TEMP_L1_CC_SCHEME_3

/* -------------------------------------------------------------------------
* Description :
*
* When defined, enhanced random access power control are available.
*
* When not defined, disable enhanced random access power control.
*
* Restrictions : -
*
* Owner : Hujian Xu
*
*
* References : None
*
*
* ------------------------------------------------------------------------- */

#define FEA_TEMP_ENHANCED_RA_PWR_CTRL

/* -------------------------------------------------------------------------
* Description : Flag for NPSS 2-frame combine mode
*
* When defined, enables the feature of NPSS 2-frame combine mode.
*
* Restrictions : None
*
* Owner : Jacky Wu
*
* References : None
*
* ------------------------------------------------------------------------- */
#define FEA_PSS_2_FRAME_MODE

/* -------------------------------------------------------------------------
* Description :
*
* When defined, coveralge level selection according to history
*
*
* Restrictions : -
*
* Owner : Hujian Xu
*
*
* References : None
*
*
* ------------------------------------------------------------------------- */

//#define FEA_TEMP_CE_LEVEL_HISTORY

/* -------------------------------------------------------------------------
* Description : Flag for Enable ack transmitted RLC SDU when RLF/reestablishment
*
* When defined, enables the feature of Enable ack transmitted RLC SDU
*
* Restrictions : None
*
* Owner : Andrew.Shi
*
* References : None
*
* ------------------------------------------------------------------------- */
#define FEA_EL2_ACK_TRANSMITTED_SDUS


/* -------------------------------------------------------------------------
* Description : Flag for moving signal out of critical path
*
* When defined, some signal is called from SFH at end of subframe tick
*
* Restrictions : None
*
* Owner : Yingqi Liu
*
* References : None
*
* ------------------------------------------------------------------------- */
#define FEA_TEMP_L1_DELAYED_SIGNAL


/* -------------------------------------------------------------------------
* Description : Flag for moving measurement reporting out of critical path
*
* When defined, measurement reporting is called from SFH at end of subframe tick
*
* Restrictions : None
*
* Owner : Juha Tuomainen
*
* References : None
*
* ------------------------------------------------------------------------- */
#define FEA_TEMP_L1_DELAYED_MEAS_REPORTING



/* -------------------------------------------------------------------------
* Description :
*
* When defined, workaround for unreasonalble NPRACH prameters
*
*
* Restrictions : -
*
* Owner : Hujian Xu
*
*
* References : None
*
*
* ------------------------------------------------------------------------- */

//#define FEA_TEMP_UNREASONALBLE_PRACH_PARAM_WORKAROUND
#endif /* NBIOT_MODEM_COMMON_CONFIG_H */
