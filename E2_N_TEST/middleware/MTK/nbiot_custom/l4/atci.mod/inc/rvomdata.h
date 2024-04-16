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
 * RVOMDATA.H
 * Operations manager global data.  Only included by vgdata.h
 **************************************************************************/

#ifndef RVOMDATA_H
#define RVOMDATA_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>

#if !defined(MNL3_TYP_H)
#if !defined (MTK_NBIOT_CUSTOM_BUILD)
#   include <mnl3_typ.h>
#endif
#endif
#if !defined(RVSYSTEM_H)
#  include <rvsystem.h>
#endif

/** TODO: Remove all references to ABCC **/
#include <abcc_typ.h>

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/* Call Id Definitions */

/* MIN_USER_CALL_ID, MAX_USER_CALL_ID defined in abcc_typ.h */

#define MAX_POSSIBLE_CS_CONNECTIONS    (MAX_USER_CALL_ID - MIN_USER_CALL_ID + 1)

#  define MIN_PSD_USER_CALL_ID         (MAX_POSSIBLE_CS_CONNECTIONS + 1)
#  define MAX_PSD_USER_CALL_ID         (MAX_POSSIBLE_CS_CONNECTIONS + 1)
#  define MAX_POSSIBLE_PSD_CONNECTIONS (MAX_PSD_USER_CALL_ID - \
                                        MIN_PSD_USER_CALL_ID + 1)


#define MAX_POSSIBLE_CC_CONNECTIONS    (MAX_POSSIBLE_CS_CONNECTIONS + \
                                        MAX_POSSIBLE_PSD_CONNECTIONS)

#define VG_PHONE_BOOK_CALL_ID        (MAX_POSSIBLE_CC_CONNECTIONS + VG_SMS_LMGETALPHA_NUMBER_OF_CALL_IDS + 1)
#define VG_MAX_USER_CALL_ID          (VG_PHONE_BOOK_CALL_ID)

/***************************************************************************
 * Type Definitions
 **************************************************************************/

typedef enum CiCallControlStatesTag
{
  ALL_IDLE,
  PSD_ONLY,
  END_STATE
} CiCallControlStates_t;

typedef enum ConnectionStateTag
{
  CONNECTION_OFF_LINE,
  CONNECTION_DIALLING,
  CONNECTION_CONNECTING,
  CONNECTION_ON_LINE,
  CONNECTION_DISCONNECTING,
  CONNECTION_INCOMING,
  NUM_OF_CONNECTION_STATES
} ConnectionState_t;

/** Connection Class Type.
 * This enumeration is used to specify a connection type used by the CI Task's
 * Operation Manager. This includes call types but also NVRAM and STK
 * connections that can be controlled.
 */
typedef enum ConnectionClassTag
{
  
#if defined (FEA_PPP)  
  /** A Point-to-Point Protocol (PPP) connection class
   * used for managing call bearer combinations in the Operations Manager. */
  PPP_CONNECTION = 0,
#endif /* FEA_PPP */

  /** A Packet Transport (PT) connection class
   * used for managing call bearer combinations in the Operations Manager. */
  PT_CONNECTION = 1,
  /** Marks the end of the call related connection classes. */
  END_OF_CALL_CONTROL_CLASSES = PT_CONNECTION + 1,
  /** Additional connection type for a PSD connection which has no data connection.
   *  This is required for AT*MDRIND unsolicited event only.
   */
  PSD_NONE_CONNECTION = 2,
  /** An NVRAM connection class used for managing read/write access to data. */
  NVRAM_CONNECTION,
  /** An STK connection class used for managing SIM Toolkit Sessions. */
  STK_CONNECTION,
#ifdef ENABLE_AP_BRIDGE_FEATURE
  /**An AP Bridge connection class used for transferring user data for AP Bridge.*/
  APBRIDGE_CONNECTION,
  /** Marks the end of the connection class enumeration. */
  CONNECTION_TERMINATOR = APBRIDGE_CONNECTION + 1
#else
  /** Marks the end of the connection class enumeration. */
  CONNECTION_TERMINATOR = STK_CONNECTION + 1
#endif  
} ConnectionClass_t;


typedef struct DialStringTag
{
  Char              number[MAX_CALLED_BCD_NO_LENGTH + NULL_TERMINATOR_LENGTH];
  ConnectionClass_t type;
}
DialString;

typedef struct ConnectionInformationTag
{
  ConnectionClass_t vgClass;
  ConnectionState_t vgState;
  Int16             vgIdent;
} ConnectionInformation_t;


typedef struct ChannelStateTag
{
  Int8     dcd;
  Int8     ri;
  Boolean  aBreak;
  Int8     breakLength;
  Boolean  assigned;
  Boolean  isDisconnecting;
} ChannelState_t;

/***************************************************************************
 *  Macros
 **************************************************************************/

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

#endif  /* RVOMDATA_H */

/* END OF FILE */



