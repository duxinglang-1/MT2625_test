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
*******************************************************************************/
/** \file
 * Message structures for N1 test messages.
 ******************************************************************************/

#ifndef N1TST_SIG_H
#define N1TST_SIG_H

/*******************************************************************************
** Includes
*******************************************************************************/

#include <system.h>
#if !defined (MTK_NBIOT_CUSTOM_BUILD)
#include <n1_sth_type.h>
#else
 /** This enumeration defines the state handler top-level states. */
typedef enum N1SthStateTag
{
    N1_STH_NULL_STATE,
    N1_STH_SEARCH_STATE,
    N1_STH_IDLE_STATE,
    N1_STH_CONNECTED_STATE,
#if !defined (N1_REMOVE_CALDEV)
    N1_STH_CALDEV_STATE,
#endif
    N1_STH_NUM_STATES  
} N1SthState;
#endif
/*******************************************************************************
** Constants
*******************************************************************************/

/** Test command identifiers. Values for N1TstAlgTuningSetReq.cmd */
#define N1_TST_ALG_TUNING_AGC_FREEZE_MODE_SET  1
#define N1_TST_ALG_TUNING_DECODING_ATTEMPTS    2

/** Maximum length of parameter data */
#define MAX_N1TST_ALG_TUNING_SET_REQ_PARAM_LENGTH 12

/** Test command specific constants */
/* For N1_TST_ALG_TUNING_AGC_FREEZE_MODE_SET */
#define N1_TST_ALG_TUNING_AGC_FREEZE_MODE_SET_UNFREEZE 0
#define N1_TST_ALG_TUNING_AGC_FREEZE_MODE_SET_MEAS 1
#define N1_TST_ALG_TUNING_AGC_FREEZE_MODE_SET_USER 2
/* For N1_TST_ALG_TUNING_DECODING_ATTEMPTS */
#define N1_TST_ALG_TUNING_DECODING_ATTEMPTS_MIB  0
#define N1_TST_ALG_TUNING_DECODING_ATTEMPTS_SIB1 1
#define N1_TST_ALG_TUNING_DECODING_ATTEMPTS_SIBN 2

/*******************************************************************************
** Macros
*******************************************************************************/

/*******************************************************************************
** Typedefs
*******************************************************************************/

/** This structure is used when L1 changes top-level state. */
typedef struct N1TstStateChangeIndTag
{
    N1SthState oldState;
    N1SthState newState;
    uint16_t      sfn;

} N1TstStateChangeInd;

typedef struct N1TstAlgTuningSetReqTag
{
    /** Transaction identifier for the command. Reserved for future use. */
    uint16_t      token;
    /** Test command type */
    uint8_t       cmd;
    /** No response expected. Reserved for future use. */
    uint8_t       silent;
    /** Parameter data for comand. */
    uint8_t       param[MAX_N1TST_ALG_TUNING_SET_REQ_PARAM_LENGTH];
} N1TstAlgTuningSetReq;

/*******************************************************************************
** Global Data
*******************************************************************************/

/*******************************************************************************
** Global Function Prototypes
*******************************************************************************/

#endif

/* END OF FILE */
