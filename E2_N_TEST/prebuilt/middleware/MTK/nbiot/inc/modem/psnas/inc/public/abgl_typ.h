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
 * Background general Library type definitions & constants
 **************************************************************************/

#ifndef ABGL_TYP_H
#define ABGL_TYP_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/
#include <system.h>

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/***************************************************************************
 * Typed Constants
 **************************************************************************/


/***************************************************************************
 * Type Definitions
 **************************************************************************/

/** Define the possible results for a request
 */
typedef enum AbglRequestStatusTag
{
    /** The request operation complet successfully*/
    GL_REQUEST_OK,
    /** NVRAM error occurs.*/
    GL_REQUEST_NVRAM_ERROR,
    /** ABGL isn't ready.*/
    GL_ABGL_NOT_READY,
    /** Request has failed because ABGL isn't in a valid state for it.*/
    GL_ABGL_INVALID_STATE,
    /** Request has failed because a least one parameter is invalid.*/
    GL_ABGL_INVALID_PARAMETER,
}AbglRequestStatus;

/** Supported modem mode
 */
typedef enum AbglFeatureModemModeTag
{
    /** Modem is configured to run for Modem Standalone (Default).*/
    GL_FEATURE_MODEM_MODE_STANDALONE    = 0,
    /** Modem is configured to run with MMI application.*/
    GL_FEATURE_MODEM_MODE_MMI           = 1
}AbglFeatureModemMode;

/** ME memory location type
 */
typedef enum AbglFeatureMeLocationTag
{
    /** No ME memory supported (Default).*/
    GL_FEATURE_ME_LOCATION_NONE     = 0,
    /** ME memory supported in Modem (NVRAM area).*/
    GL_FEATURE_ME_LOCATION_NVRAM    = 1
}AbglFeatureMeLocation;

/** Avalaible SIM sockets when power on
 */
typedef enum AbglSimSocketTypeTag
{
   AB_SIM_SOCKET_0 = 0,
   AB_SIM_SOCKET_1,
   AB_SIM_SOCKET_2,
   AB_SIM_SOCKET_MAX
} AbglSimSocketType;

/** The avalaible setting to store the call information
 */
typedef enum AbglFeatureCallInfoStorageTag
{
    /** Store the call information on SIM when possible. If not possible 
      * (file not present,...), the call information will be stored in ME.*/
    GL_FEATURE_SAVE_CALL_INFO_SIM   = 0,
    /** Store all the call information on ME and nothing into the SIM.*/
    GL_FEATURE_SAVE_CALL_INFO_ME    = 1,
}AbglFeatureCallInfoStorage;

/** The configurable features stored in NVRAM
 */
typedef struct AbglFeatureConfigDataAreaTag
{
    /** Selected modem mode*/
    AbglFeatureModemMode        modemMode;
    /** Selected ME storage location*/
    AbglFeatureMeLocation       meLocation;
    /** Whether SM should handle concat SM*/
    Boolean                     smHandleConcatSms;
    /** What to do with the call information*/
    AbglFeatureCallInfoStorage  callInfoStorageMode;
#if defined (ENABLE_SIM_SOCKET_SELECT)
    /** Selected SIM socket when power on*/
    AbglSimSocketType           simSocket;
#endif    
}AbglFeatureConfigDataArea;

/** Specifies the state of ABGL */
typedef enum AbglStateTag
{
    ABGL_STATE_INITIAL,          /**< Waiting NVRAM information */
    ABGL_STATE_ACTIVE,           /**< Initialisation performed, ABGL is ready */
}
AbglState;

/** Context for ABGL sub-system
 */
typedef struct AbglContextTag
{
    /** The current ABGL state */
    AbglState   state;
}AbglContext;
/***************************************************************************
 *  Macros
 **************************************************************************/


/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

#endif

/* END OF FILE */
