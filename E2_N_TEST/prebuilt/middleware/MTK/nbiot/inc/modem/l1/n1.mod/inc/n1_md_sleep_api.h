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
#ifndef __N1_MD_SLEEP_API__
#define __N1_MD_SLEEP_API__

/** @file n1_md_sleep_api.h
 **************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
** Macros
*******************************************************************************/

/*****************************************************************************
* Enum
*****************************************************************************/

/*******************************************************************************
** Global Data
*******************************************************************************/

/*******************************************************************************
** Local Data
*******************************************************************************/

/*******************************************************************************
** Local Functions
*******************************************************************************/

/*******************************************************************************
** Global Functions
*******************************************************************************/

/**
* @brief       This function calculates and returns a sleep opportunity lenght of N1.
* @param       Pointer to opportunity length of sleep of N1 
* @return      Is the remain time valid or not.*/

bool N1MdSleepGetRemainSleepOpportunityLength(uint32_t *sleepOpportunity_p);

/**
* @brief       This function indicates deep sleep 32K K convergence done.
* @param       NA
* @return      deep sleep 32K K convergence done or not.*/

bool N1MdDeepSleepCaliConvergeDone(void);


/**
* @brief       Power off MDSYS PD (MDSYS off) power domain before ligth/deep sleeps */
void N1MdSleepPowerMdsysPDOff(void);

/**
* @brief       Power on MDSYS  PD (MDSYS off) power domain after ligth sleep */
void N1MdSleepPowerMdsysPDOn(void);

/**
* @brief       This function forces power down of MDSYS PD (MDSYS off)  power domain*/
void N1MdSleepForcePowerMdsysPDOff(void);

/**
* @brief       tihs function forces Power on of MDSYS PD (MDSYS off) power domain */
void N1MdSleepForcePowerMdsysPDOn(void);

#ifdef __cplusplus
}
#endif
 
#endif /* __N1_MD_SLEEP_API__ */
