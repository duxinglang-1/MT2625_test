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
 * Declaration of ATCI deep sleep functions.
 **************************************************************************/

#ifndef RVSLEEP_H
#define RVSLEEP_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/
#include <rvdata.h>
#include <psc_api.h>
#include <rvcimxut.h>

/***************************************************************************
 * Manifest Constants
 **************************************************************************/
  /* NVDM group name for ATCI Generic Data */
#define RV_NVRAM_GENERIC_DATA_GROUP    "RV_GEN_GROUP"
#define RV_NVRAM_ENTITY_DATA_GROUP     "RV_ENTITY_GROUP"
#define RV_NVRAM_PSD_CONN_DATA_GROUP   "RV_PSDCON_GROUP"

  /* NVDM data item name for ATCI Generic Data */
#define RV_NVRAM_MUX_CONTEXT_DATA     "RV_DATA_ITEM_MUX_CONTEXT_DATA"
#define RV_NVRAM_OPMAN_DATA           "RV_DATA_ITEM_OPMAN_DATA"           
#define RV_NVRAM_PROFILE_DATA         "RV_DATA_ITEM_PROFILE_DATA"
#define RV_NVRAM_CHMAN_DATA           "RV_DATA_ITEM_CHMAN_DATA"
#define RV_NVRAM_MOBILITY_DATA        "RV_DATA_ITEM_MOBILITY_DATA"
#define RV_NVRAM_SIMLOCK_DATA         "RV_DATA_ITEM_SIMLOCK_DATA"
#define RV_NVRAM_STK_DATA             "RV_DATA_ITEM_STK_DATA"
#define RV_NVRAM_GENERAL_DATA         "RV_DATA_ITEM_GENERAL_DATA"
#define RV_NVRAM_PD_CONTEXT_DATA      "RV_DATA_ITEM_PD_CONTEXT_DATA"
#define RV_NVRAM_ACTIVE_ENTITY_1      "RV_DATA_ITEM_ENTITY_DATA_1"
#define RV_NVRAM_ACTIVE_ENTITY_2      "RV_DATA_ITEM_ENTITY_DATA_2"
#define RV_NVRAM_ACTIVE_ENTITY_3      "RV_DATA_ITEM_ENTITY_DATA_3"
#define RV_NVRAM_ACTIVE_ENTITY_4      "RV_DATA_ITEM_ENTITY_DATA_4"
#define RV_NVRAM_ACTIVE_ENTITY_5      "RV_DATA_ITEM_ENTITY_DATA_5"
#define RV_NVRAM_ACTIVE_ENTITY_6      "RV_DATA_ITEM_ENTITY_DATA_6"
#define RV_NVRAM_ACTIVE_ENTITY_7      "RV_DATA_ITEM_ENTITY_DATA_7"
#define RV_NVRAM_ACTIVE_ENTITY_8      "RV_DATA_ITEM_ENTITY_DATA_8"
#define RV_NVRAM_ACTIVE_ENTITY_9      "RV_DATA_ITEM_ENTITY_DATA_9"
#define RV_NVRAM_ACTIVE_ENTITY_10     "RV_DATA_ITEM_ENTITY_DATA_10"
#define RV_NVRAM_ACTIVE_ENTITY_11     "RV_DATA_ITEM_ENTITY_DATA_11"

#define RV_NVRAM_PSD_CONN_DATA        "RV_DATA_ITEM_PSD_CONN_DATA"

/***************************************************************************
* Type Definitions
**************************************************************************/
typedef enum RvNvramResultTag
{
    RV_NVRAM_RESULT_OK,
    RV_NVRAM_RESULT_FAILED
} RvNvramResult;



/***************************************************************************
 *  Macros
**************************************************************************/

/***************************************************************************
 *  Function Prototypes
**************************************************************************/

void RvInitialiseNVRamMemStore(void);

void RvDeepSleepCb (PscSleepAction sleepAction, PscPowerState powerState);


Boolean RvPrepareForSleep(void);

void RvGoToSleep(void);
 
void RvWakeUpFromSleep(void);

void RvWakeUpCompleteCheck(void);

void RvGenerateWakeupCompleteUrc(void);

Boolean VgSleepManInterfaceController(  const SignalBuffer *signal_p,
                                              const VgmuxChannelNumber entity);

#if defined (ENABLE_ATCI_UNIT_TEST)
void vgResetAllAtciStructures(void);
#endif

#endif 


