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
 * The application background layer call control data types
 **************************************************************************/

#ifndef ABCC_TYP_H
#define ABCC_TYP_H

/***************************************************************************
 * Nested Include Files
 ***************************************************************************/

#if !defined(APPLAYER_H)
#   include <applayer.h>
#endif

#if !defined(MMXX_SIG_H)
#if !defined (MTK_NBIOT_CUSTOM_BUILD)
#   include <mmxx_sig.h>
#endif
#endif

#if !defined(MNXX_SIG_H)
#if !defined (MTK_NBIOT_CUSTOM_BUILD)
#   include <mnxx_sig.h>
#endif
#endif

#if !defined(ALSI_SIG_H)
#   include <alsi_sig.h>
#endif

#if !defined(MNL3_TYP_H)
#   include <mnl3_typ.h>   /* FR9710-1504 - needed for CallingPartyBcdNum */
#endif

#  include <uttime.h>

#if !defined(ALSA_TYP_H)
#   include <alsa_typ.h>
#endif

#if defined (UPGRADE_CTM) || defined(UPGRADE_TWGGE_NRW)
#  include <l1ctmtyp.h>
#endif

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

#define MAX_DIALLED_NO_LENGTH  (MAX_CALLING_BCD_NO_LENGTH)

/*
 * The application layer referes to calls as UserCallId's (which is defined
 * later). The following macros define the valid range of UserCallId's
 */
 
#define MIN_USER_CALL_ID            (1)     /**< Minimum valid #UserCallId. */
#define MAX_USER_CALL_ID            (7)     /**< Maximum valid #UserCallId. */
#define NEVER_USED_USER_CALL_ID     (0)     /**< Invalid #UserCallId. */


/***************************************************************************
 * Type Definitions
 **************************************************************************/

/* local call barring prefix sizes */
#define MAX_LCB_PREFIXES          9
#define MAX_LCB_PREFIX_BYTES      5    /* max # BCD chars is twice this */

/** ABCC Advice of Charge structures */
typedef Int32 CallChargeMeter;

/** structure recording current state of Local Call Barring */
typedef struct LocalCallBarringTag
{
  /** Indicates if barring incoming call is enabled */
  Boolean           incomingBarringEnabled;
  /** Indicates if barring outcoming call is enabled */
  Boolean           outgoingBarringEnabled;
  /** Indicates if call is barred from prefix list */
  Boolean           barFromPrefixList;
  /** Number of prefixes */
  Int8              numOfPrefixes;
  /** Number of barred BCD */
  Int8              barBcdPrefix [MAX_LCB_PREFIXES] [MAX_LCB_PREFIX_BYTES];
}
LocalCallBarring;

/** NVRAM session data element. 
 * Stored in NVRAM that holds persistent data about the last 
 * call (duration and cost) and the total calls.
 */
typedef struct NvRamSessionDataTag
{
  /**   If TRUE set to new lastSessionCost value 
   * otherwise keep last session cost at current value.
   */
  CallChargeMeter lastSessionCost;
  /** The offset in secs from the start of the session */
  Int32           lastSessionDuration;  /**< in seconds */
  Int32           allSessionsDuration;  /**< in seconds */

  Int32           allIncomingSessionsDuration;  /**< in seconds */
  Int32           allOutgoingSessionsDuration;  /**< in seconds */

  Int32           lastIncomingSessionDuration;  /**< in seconds */
  Int32           lastOutgoingSessionDuration;  /**< in seconds */
}
NvRamSessionData;

/** Contains the list of subAddress information */
typedef  ConnectedSubaddressElement
SubAddress;

/** Apex defined cause types. */
typedef enum ApexCauseTypeTag
{
  /** No cause was given*/
  APEX_CAUSE_OK = 0,
  /** The called number was busy */
  APEX_CAUSE_SUBSCRIBER_BUSY,
  /** The network was congested */
  APEX_CAUSE_CONGESTION,
  /** There was no radio path available */
  APEX_CAUSE_RADIO_PATH_UNAVAIL,
  /** There was an error */
  APEX_CAUSE_ERROR,
  /** The called number was unobtainable */
  APEX_CAUSE_NUM_UNOBTAINABLE,
  /** There was an authentication failure */
  APEX_CAUSE_AUTH_FAILURE,
  /** No number was given for the call */
  APEX_CAUSE_NO_NUM_PRESENT,
  /** There was a call control failure */
  APEX_CAUSE_BL_CC_FAILURE,
  /** There was a mobility management failure */
  APEX_CAUSE_BL_MM_FAILURE,
  /** The requested channel was busy */
  APEX_CAUSE_CHANNEL_BUSY,
  /** FDN Mismatch */
  APEX_CAUSE_FDN_MISMATCH,
  /** Bearer capability fail */
  APEX_CAUSE_BC_FAIL,
  /** EMERGENCY_CALLS_ONLY */
  APEX_CAUSE_EMERGENCY_CALLS_ONLY,
  /** ACM limit exceeded */
  APEX_CAUSE_ACM_LIMIT_EXCEEDED,
  /** Hold error */
  APEX_CAUSE_HOLD_ERROR,
  /** Busy processing Reques */
  APEX_CAUSE_BUSY_PROCESSING_REQUEST,
  /** Active channel unavailable */
  APEX_CAUSE_ACTIVE_CHANNEL_UNAVAILABLE,
  /** Outgoing calls barred */
  APEX_CAUSE_OUTGOING_CALL_BARRED,
  /** Number has been blacklisted */
  APEX_CAUSE_NUMBER_BLACKLISTED,
  /** Blacklist full */
  APEX_CAUSE_BLACKLIST_FULL,
  /** Redial timer still running */
  APEX_CAUSE_REDIAL_NOT_TIMED_OUT,
  /** Channel Mode Modify fail */
  APEX_CAUSE_MODIFY_FAIL,
  /** Not in control */
  APEX_CAUSE_NOT_IN_CONTROL,
  /** No cause was given */
  APEX_CAUSE_NO_CAUSE, /*FR9809-2939*/
  /** Reestablishment barred */
  APEX_CAUSE_ERROR_REESTABLISHMENT_BARRED,
  /** STK call is emergency */
  APEX_CAUSE_SIMTOOLKIT_CALL_IS_EMERGENCY,
  /** Number barred by SIM */
  APEX_CAUSE_BARRED_BY_SIM,
  /** Number of APEX causes */
  NUM_OF_APEX_CAUSES
}
/** Apex defined cause types. */
ApexCauseType;


/** Identifier for a call.
 * The application layer referes to calls by its userCallId which is from
 * #MIN_USER_CALL_ID to #MAX_USER_CALL_ID. Once a call has been assigned a
 * \a UserCallId it remains the same for the duration of the call. The first
 * call received or sent is assigned #MIN_USER_CALL_ID.
 */
typedef  Int16  UserCallId;

/** The application layer has its own cause failure definitions. Which contain
 * generalised GSM causes and Apex specific causes. Two cause fields have been
 * included as the GSM can sometimes provided more than one cause.
 */
typedef  struct   OptCauseTag
{
  /** Indicates if more cause information is present 
\assoc PRESENT \ref cause */
  Boolean         causePresent;
  /** Specifies the type of additional cause */
  ApexCauseType   apexCauseType;
  /** Determines the cause*/
  Cause           cause;
}
OptCause;


/** Typedef of DialledBcdNum */
typedef CallingPartyBcdNum DialledBcdNum; /* FR9710-1504 */

/** Determines the dialled BCD number information if available */
typedef  struct  OptDialledBcdNumTag
{
  /** Indicates if dialled BCD number is present 
\assoc PRESENT \ref dialledBcdNum */
  Boolean           present;
  /** Dialled BCD number information */
  DialledBcdNum     dialledBcdNum;
}
OptDialledBcdNum;


/** This information element shall be included by the mobile 
 * station supporting multicall. 
 */
typedef struct OptStreamIdentifierTag
{
  /** Indicates if stream identifier is present */
  Boolean                 present;
  /** Associates a particular call with a Radio Access Bearer (RAB) */
  StreamIdentifier        streamIdentifier;
}
OptStreamIdentifier;

#if defined (UPGRADE_REL5)
/** Provide the network with information about the speech 
 * codecs supported by the mobile.
 */
typedef struct OptSupportedCodecListTag
{
    /** Indicates if supported codec list is present */
    Boolean               present;
    /** The list of the supported codec */
    SupportedCodecList    supportedCodecList;
}
OptSupportedCodecList;
#endif

/*
  The typedef's defined below are to simplify the association of present with
  the relevent data structure
*/

/** Calling party BCD number element */
typedef struct OptCallingPartyBcdNumTag
{
  /** Indicates if Calling party BCD number is present 
\assoc PRESENT \ref callingPartyBcdNum */
  Boolean               present;
  /** Identifies the origin of a call */
  CallingPartyBcdNum    callingPartyBcdNum;
}
OptCallingPartyBcdNum;

/** Called party BCD number element */
typedef struct OptCalledPartyBcdNumTag
{
  /** Indicates if called party bcd number is present 
\assoc PRESENT \ref calledPartyBcdNum */
  Boolean               present;
  /** For all bands except for PCS1900, 
   * the maximum length of this IE sent by the network shall be 13 octets.
   */
  CalledPartyBcdNum     calledPartyBcdNum;
}
OptCalledPartyBcdNum;

/**************  FDN LIST (moved to  ablm  in PHASE 2 *************************/


#if defined (UPGRADE_CTM) || defined(UPGRADE_TWGGE_NRW)
/** Cellular Text telephone Modem (CTM) Settings element */
typedef struct CtmSettingsTag
{
  /** Indicates if CTM is enabled */
  Boolean                   enableCtm;
  /** Specifies CTM text intrface */
  CtmTextInterface          ctmTextInterface;
  /** Indicates Baudot rate */
  CtmBaudotRate             ctmBaudotRate;
}
CtmSettings;
#endif /* UPGRADE_CTM */


#endif

/* for emacs users */
/* Local Variables: */
/* c-basic-offset: 2 */
/* End: */

/* END OF FILE */
