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
 * Types definition for Sim Lock Sub System.
 **************************************************************************/

#ifndef RVSLDATA_H
#define RVSLDATA_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>
#include <rvcfndata.h>
#if !defined(ALSI_SIG_H)
#include "alsi_sig.h"      /* PS AL to SIM Manager Signal Definitions   */
#endif

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/* Constants for the +CRSM command and +CSIM command */
#define VG_SIM_MAX_MSG_SIZE           (SIM_MAX_MSG_SIZE)
#define VG_CRSM_COMMAND_READ_BINARY   (0xB0)    /* 176 */
#define VG_CRSM_COMMAND_READ_RECORD   (0xB2)    /* 178 */
#define VG_CRSM_COMMAND_UPDATE_BINARY (0xD6)    /* 214 */
#define VG_CRSM_COMMAND_UPDATE_RECORD (0xDC)    /* 220 */
#define VG_CRSM_COMMAND_GET_RESPONSE  (0xC0)    /* 192 */
#define VG_CRSM_COMMAND_STATUS        (0xF2)    /* 242 */
#define VG_CRSM_COMMAND_SELECT        (0xA4)    /* 164 */

#define VG_CRSM_CLASS_GSM_APP         (0xA0)
#define VG_CRSM_CLASS_3G_APP          (0x00)
#define VG_CRSM_CLASS_STATUS          (0x80)

#define VG_CRSM_COMMAND_HEADER_SIZE   (5)
#define VG_CRSM_COMMAND_FILE_ID_SIZE  (2)
#define VG_CRSM_MAX_COMMAND_DATA      ((VG_SIM_MAX_MSG_SIZE) - (VG_CRSM_COMMAND_HEADER_SIZE))

#define VG_CRSM_MAX_COMMAND_STRING    (VG_CRSM_MAX_COMMAND_DATA * 2)

#define VG_CRSM_MAX_PATHID_LENGTH     (MAX_SELECTION_PATH_LEN - 2)
#define VG_CRSM_MAX_PATHID_CHARS      (VG_CRSM_MAX_PATHID_LENGTH * 2)
#define VG_CSIM_COMMAND_LENGTH        (VG_SIM_MAX_MSG_SIZE)
#define VG_CSIM_MAX_COMMAND_STRING    (VG_CSIM_COMMAND_LENGTH * 2)

#define SIM_LOG_CHAN_MIN_DFNAME_LEN (1)      /* Min length in bytes */
#define SIM_LOG_CHAN_MAX_DFNAME_LEN (16)     /* Max length in bytes */

#define SIM_LOG_CHAN_MIN_DFNAME_STR (SIM_LOG_CHAN_MIN_DFNAME_LEN * 2)
#define SIM_LOG_CHAN_MAX_DFNAME_STR (SIM_LOG_CHAN_MAX_DFNAME_LEN * 2)

#define SIM_LOG_CHAN_MAX_CMD_LEN      (VG_SIM_MAX_MSG_SIZE)
#define SIM_LOG_CHAN_MAX_CMD_STR      (SIM_LOG_CHAN_MAX_CMD_LEN * 2)

#define SIM_LOG_CHAN_MAX_SESSION_NUM    20
#define SIM_LOG_CHAN_BASIC_SESSION_NUM  4
#define SIM_LOG_CHAN_EXTEND_MASK        0x40

#define VG_MUAPP_INVALID_INDEX        (0xFF)

#if defined (SIM_EMULATION_ON)
#define VG_MUSIMEMUW_FILE_ID_LEN       (2)
#define VG_MUSIMEMUW_FILE_ID_STR_LEN   ((VG_MUSIMEMUW_FILE_ID_LEN*2) + 1)
#define VG_MUSIMEMUW_FILE_DATA_LEN     (SIM_MAX_EMU_FILE_WRITE_DATA_LENGTH)
#define VG_MUSIMEMUW_FILE_DATA_STR_LEN ((VG_MUSIMEMUW_FILE_DATA_LEN*2) + 1)
#endif /* SIM_EMULATION_ON */

/***************************************************************************
 * Type Definitions
 **************************************************************************/

/* SIM information definitions */

typedef enum VgSimStateTag
{
  VG_SIM_NOT_READY = 0,
  VG_SIM_PIN,
  VG_SIM_PUK,
  VG_SIM_READY,
  VG_SIM_PIN2,
  VG_SIM_PUK2,
  VG_SIM_UPIN,
  VG_SIM_UPUK,
  NUM_OF_VG_SIM_STATES

}
VgSimState;

typedef enum VgPinStateTag
{
  VG_PIN_DISABLED = 0,
  VG_PIN_ENABLED,
  VG_UPIN_ENABLED = 10,
  VG_UPIN_ENABLED_USED = 11,
  NUM_OF_VG_PIN_STATES

}
VgPinState;


typedef enum VgPinAccessModeTag
{
  VG_ENABLE_PIN,
  VG_DISABLE_PIN,
  VG_CHANGE_PIN,
  VG_VERIFY_PIN,
  VG_QUERY_PIN_STATUS,
  VG_NOT_USE_UPIN,
  VG_DISABLE_PIN_USE_UPIN,
  NUM_OF_VG_PIN_MODE,
} VgPinAccessMode;

typedef enum VgPinAccessLevelTag
{
  VG_PIN,
  VG_PIN2,
  VG_UPIN,
  NUM_OF_VG_PIN_LEVEL
} VgPinAccessLevel;

typedef enum VgSimInsertedStateTag
{
  VG_SIM_INSERTED_STATE_UNKNOWN = 0,
  VG_SIM_NOT_INSERTED,
  VG_SIM_INSERTED,
  NUM_OF_VG_SIM_INSERTED_STATES
}
VgSimInsertedState;

typedef struct SimInfoTag
{
  Boolean               cardIsUicc;
  Boolean               pinEnabled;  /*could be PIN application or universal PIN */
  SimUiccKeyRefValue    pin1KeyRef;  /*key reference for PIN1 (or application PIN)*/
  SimUiccKeyRefValue    pin2KeyRef;  /*key reference for PIN2 (also called local PIN)*/
  Boolean               verifyUniversalPin;  /*whether to verify universal PIN or the application PIN*/
  Boolean               universalPinSupportedByCard; 
  Int8                  pinNumRemainingRetrys;
  Int8                  pin2NumRemainingRetrys;
  Int8                  unblockPinNumRemainingRetrys;
  Int8                  unblockPin2NumRemainingRetrys;
  Boolean               pin2Verified;
  Boolean               plmnModeBitSetToZero;
  SimUiccPinStatus      pin1Status;          
  SimUiccPinStatus      pin2Status;
  SimUiccPinStatus      universalPinStatus;
  SimUiccPinStatus      unblockPin1Status;
  SimUiccPinStatus      unblockPin2Status;
  SimUiccPinStatus      unblockUniversalPinStatus;
  SimPhase              phase;
  Boolean               plmnSelector;     /*TRUE if EF PLMN SEL present on the SIM*/
  Boolean               userPlmnSelector; /*TRUE if EF PLMN w ACT present on the SIM/USIM*/
  Boolean               operatorPlmnSelector;  /*TRUE if EF OPLMN w ACT present on the SIM/USIM*/
  Boolean               hplmnSelector;   /*TRUE if EF HPLMN w ACT present on the SIM/USIM*/
  Boolean               ici;             /*TRUE if ICI file present on the SIM*/
  Boolean               oci;             /*TRUE if OCI file present on the SIM*/
  Imsi                  imsi;
  SimIccid              iccid;
#if defined (FEA_PHONEBOOK)
  Boolean               fdnIsEnabled;
  Boolean               bdnIsEnabled;
#endif
  Boolean               disablePin1Allowed;
#if defined (ENABLE_DUAL_SIM_SOLUTION)
  SimHolderType          currentSimHolder;
  Int8                   numSimHolders;
#endif
  ServiceTableType       serviceTableType;
  ServiceTable           serviceTable; 
  SimPlmn                hplmn;   
} VgSimInfo;


/* For the +CSIM and +CRSM commands */
typedef struct VgGenSimAccess
{
  Int16      length;
  Int8       commandData[VG_SIM_MAX_MSG_SIZE];
  Int16      efId;      /* job 101589  changed to Int16*/
  Int16      dirId;
  Int16      rootDirId;
  Int16      commandRef;
  /* job134856: add handling for <pathid> field */
  SimUiccPath path;
} VgGenSimAccess_t;

/* For the +CCHO and +CCHCcommands */
typedef struct VgUiccLogicChanInfoTag
{
    /** session identifier */
    Int8        sessionId;
    /** The length of DFname */
    Int16       dfNameLength;
    /** Holds the DFname data */
    Int16       dfName[SIM_MAX_DF_NAME_SIZE];
}VgUiccLogicChanInfo;

/* For the +CGLA/+CRLA commands */
typedef struct VgSimLogivalChannelAccess
{
  Int16      length;
  Int8       commandData[VG_SIM_MAX_MSG_SIZE];
  Int8       sessionId;
  /* for the +CRLA */
  Int16      efId;
  Int16      dirId;
  Int16      rootDirId;
  Int16      commandRef;
  SimUiccPath path;
} VgSimLogivalChannelAccess_t;

typedef enum VgMSMIEventTag
{
  VG_MSMI_HOLDER   = 0, /* SIM card holder state changed   */
  VG_MSMI_CARD     = 1  /* SIM card presence changed      */
}
VgMSMIEventType;

typedef enum VgSlCardmodeValueTag
{
  VG_SL_CARDMODE_UNKNOWN   = 0, /* Card type is unknown */
  VG_SL_CARDMODE_SIM       = 1, /* Card type is SIM     */
  VG_SL_CARDMODE_USIM      = 2  /* Card type is USIM    */
}
VgSlCardmodeValue;

/** The MNON command operation*/
typedef enum VgMnonOperationTag
{
    /** Read all the network operation name*/
    VG_MNON_OP_READ_ALL,
    /** Read only the NON at specified index*/
    VG_MNON_OP_READ_INDEX,
    /** Test command to know the maximum allowed index*/
    VG_MNON_OP_TEST
}VgMnonOperation;

/** Some data specifics to MNON command*/
typedef struct VgMnonDataTag
{
    /** The current command operation*/
    VgMnonOperation operation;
    /** start index for the interval*/
    Int8            startIntIndex;
    /** stop index for the interval*/
    Int8            stopIntIndex;
    /** Whether a intereval has been already printed*/
    Boolean         intPrinted;
}VgMnonData;

typedef enum VgMuappModeTag
{
    /** Active the Application session for the given AID Index.*/
    VG_MUAPP_ACTIVE_AID_SESSION = 0,
    /** Activate the Application session state unsolicited indication.*/
    VG_MUAPP_ACTIVE_SESSION_STATE_IND,
    /** Deactivate the Application session state unsolicited indication.*/
    VG_MUAPP_DEACTIVE_SESSION_STATE_IND,
    /** Give the current Application session state Indication status.*/
    VG_MUAPP_GET_SESSION_STATE_IND,
    /** Last mode that can be input with the MUAPP command*/
    VG_MUAPP_INVALID_MODE,
    /** Range operation*/
    VG_MUAPP_RANGE,
    /** Query operation*/
    VG_MUAPP_QUERY
}
VgMuappMode;

/** Defines the avalaibles application state
*/
typedef enum VgMuappAppStateTag
{
    /** The application is not running.*/
    VG_MUAPP_APP_STOPPED  = 0,
    /** The application is running*/
    VG_MUAPP_APP_RUNNING  = 1,
}
VgMuappAppState;

/** Define a MUAPP application state changed event.
*/
typedef struct VgMuappEventTag
{
    /** The EF(DIR) application index*/
    Int8            index;
    /** The application state*/
    VgMuappAppState state;
    /** Info about application*/
    SimUiccAidLabel aid;
}
VgMuappEvent;

/** Data used byt the *MUAPP command
*/
typedef struct VgMuappDataTag
{
    /** Currently executed operation*/
    VgMuappMode     muappMode;
    /** Temporary stored index*/
    Int8            index;
}VgMuappData;


/***************************************************************************
 *  Macros
 **************************************************************************/

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

#endif  /* RVSLDATA_H */

/* END OF FILE */

