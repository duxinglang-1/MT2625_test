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
 * Application-provided 'types' for ANRM2 module.
 **************************************************************************/
/* This file should be included in ANRM2 module only.
 * Only the NVRAM manager needs know about everyone's types
 **************************************************************************/

#ifndef ANRM2TYP_H
#define ANRM2TYP_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#if !defined (ANRM2BASE_H)
#  include <anrm2base.h>
#endif


#if !defined (AFNV_TYP_H)
#  include <afnv_typ.h>
#endif

#include <dmnvmcfg.h>  /* replaces abnv_typ.h */

#if defined (UPGRADE_XO_ATC) || defined(UPGRADE_TWGGE_NRW)
# include <u1xoatcbg.h>
#endif

/***************************************************************************
 * Manifest Constants
 **************************************************************************/
/* none */

/***************************************************************************
 * Typed Constants
 **************************************************************************/
/* none */

/***************************************************************************
 * Type Definitions
 **************************************************************************/
/* none */

typedef enum Anrm2DataNameTag      /* Must match order in NramDataUnion    */
{
  NRAM2_NULL = 0,
#  include <afnv_adn.h>

#  include <abnv_adn.h>

  /* add new data areas (which are not files) here ... */
#if defined (UPGRADE_XO_ATC) || defined(UPGRADE_TWGGE_NRW)
    NRAM2_XO_ATC_DATA,
#endif

  NUM_OF_NRAM2
}
Anrm2DataName;


typedef union Anrm2DataUnionTag
{
  Int32 anrmDummy;

#  include <afnv_adu.h>

#  include <abnv_adu.h>

#if defined (UPGRADE_XO_ATC) || defined(UPGRADE_TWGGE_NRW)
  U1XoAtcNvData u1XoAtcNvData;
#endif

}
Anrm2DataUnion;


#endif /*Include Guard*/

