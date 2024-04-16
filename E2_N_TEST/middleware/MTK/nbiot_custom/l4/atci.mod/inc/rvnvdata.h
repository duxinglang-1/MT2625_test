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
 * RV data types for storage in NVRAM 
 **************************************************************************/

#ifndef RVNVDATA_H
#define RVNVDATA_H

#include <abem_typ.h>

#if !defined (ABPD_SIG_H)
#include <abpd_sig.h>
#endif

#if !defined (RVMMDATA_H)
#include <rvmmdata.h>
#endif

#if !defined (RVMXDATA_H)
#include <rvmxdata.h>
#endif

#if !defined (RVOMDATA_H)
#include <rvomdata.h>
#endif

#if !defined (RVCHDATA_H)
#include <rvchdata.h>
#endif

#if !defined (RVPROF_H)
#  include <rvprof.h>
#endif

#if !defined (RVGNDATA_H)
#  include <rvgndata.h>
#endif

#if !defined (RVGPDATA_H)
# include <rvgpdata.h>
#endif

#if !defined (RVGPDATA_H)
# include <rvsldata.h>
#endif

/***************************************************************************
 * Nested Include Files
 **************************************************************************/


/***************************************************************************
 * Type Definitions
 **************************************************************************/
typedef struct EntityNvContext_Tag
{
 CmuxCmdParams        cmuxCmdParams;
 Int8                 nvrmReadfProfileFlag;
 Int8                 nvrmWriteProfileFlag;
 ProfileContext_t     entityProfileData;
 Boolean              gprsContextConnectionActive;
 Boolean              smsEnableExtraUnsol;
 CommandId_t          currentAtCommand;
}EntityNvContext_t;

typedef struct GeneralNvGenericContext_Tag
{
  VgMFtrCfgData     vgMFtrCfgData;
}GeneralNvGenericContext_t;


typedef struct SimLockNvContext_Tag
{
  Boolean             powerUpProtoStack;
  Boolean             powerUpSim;
  VgSimInsertedState  currentSimInsertedState;
}SimLockNvContext_t;

typedef struct StkNvGenericContext_Tag
{
  Boolean registeredForStk[CI_MAX_ENTITIES];
  VgmuxChannelNumber registeredStkEntity;
  
}StkNvGenericContext_t;


typedef struct MobilityNvContext_Tag
{
  VgCOPSData              vgCOPSData;
  VgCSCONData             vgCSCONData;
  VgMOOSAINDData          vgMOOSAINDData;
}MobilityNv_t;

typedef struct GprsNvGeneric_Tag
{
  AbpdApn               currDefaultApn;
}GprsNvGeneric_t;


typedef struct MuxContextTag
{
  EntityState_t      entityState[CI_MAX_ENTITIES];
  Boolean            atciRegisteredWithABPRocedures;
  Boolean            atciHaveReadPhoneFunctionality;
  Boolean            isExternalChannel[CI_MAX_ENTITIES];
} MuxContext_t;

typedef struct OpmanGenericContextTag
{
  Boolean        peripheralControlBuffer [CONNECTION_TERMINATOR];
  Boolean        channelNeedsEnabling [CI_MAX_ENTITIES];
  ChannelState_t channelState[CI_MAX_ENTITIES];
  Int8           numberOfEnabledChannels;
  Int8           channelEnableIndChannelNumber[CI_MAX_ENTITIES];
} OpmanGenericContext_t;

typedef struct ChManagerContextTag
{
  Boolean         isImmediate;
  VgChangeControl vgControl [NUMBER_CHANGE_CONTROL_ELEMENTS];
} ChManagerContext_t;

typedef struct CidNvInfoTag
{
   Boolean      isActive;
   Int8         connId;
}CidNvInfo_t;

/* if this strucutre or the sub-structures of this data store is changed then the 
routines for reading/writing content to NVRAM may need  to be changed */
typedef struct RvNvramGenericDataTag
{

    MuxContext_t              muxContextData;
    OpmanGenericContext_t     opmanGenericData;
    ProfileGenericContext_t   profileGenericData;
    ChManagerContext_t        chmanData;
    MobilityNv_t              mobilityData;
    SimLockNvContext_t        simLockData;
    StkNvGenericContext_t     stkData;
    GeneralNvGenericContext_t generalGenericData;
    GprsNvGeneric_t           pdContextData;
    EntityNvContext_t         entityNvData[ATCI_MAX_NUM_ENABLED_AT_CHANNELS]; 
    CidNvInfo_t               cidInfo[MAX_NUMBER_OF_CIDS];
}
RvNvramGenericData;

typedef enum RvNvDataTypeTag
{
    RV_MUX_CONTEXT_DATA,
    RV_OPMAN_DATA,
    RV_PROFILE_DATA,
    RV_CHMAN_DATA,
    RV_MOBILITY_DATA,
    RV_SIMLOCK_DATA,
    RV_STK_DATA,
    RV_GENERAL_DATA,
    RV_PD_CONTEXT_DATA,
    RV_ENTITY_SPECIFIC_DATA,
    RV_CID_DATA
}
RvNvDataType;

/***************************************************************************
 *  Macros
 **************************************************************************/

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/
 
#endif

