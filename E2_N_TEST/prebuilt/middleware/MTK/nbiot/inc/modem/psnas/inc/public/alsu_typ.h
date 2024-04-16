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
 *  File Description :                                                   */
/** \file
 *      types used by alsu_sig.h
 **************************************************************************/

#ifndef ALSU_TYP_H
#define ALSU_TYP_H

#include <system.h>
 
/****************************************************************************
 * Nested Include Files
 ****************************************************************************/
 
 /***************************************************************************
 * Manifest Constants
 **************************************************************************/
#define SIM_UICC_PBID_LENGTH            14
 
 /****************************************************************************
 * Macros
 ****************************************************************************/

/****************************************************************************
 * Types
 ****************************************************************************/
 
 /*-----------------29/12/08 14:33------------------------------------
 ** New structure for 3G phonebooks synchronisation
 * -----------------------------------------------------------------*/
 
/** This structure is able to hold the phonebook ID. */
typedef struct SimUiccPbidTag
{
    /** Phonebook ID value*/
    Int8     value[SIM_UICC_PBID_LENGTH];
} SimUiccPbid;

/** Store the synchronisation info before sent them to AB*/
typedef struct SimUiccPbSyncInfoTag
{
    /** Whether the next fields are about ADN GLB*/
    Boolean                         globalPhoneBook;
    /** The phonebook change counter*/
    Int16                           changeCounter;
    /** True when the change counter has change*/
    Boolean                         changeCounterHasChanged;
    /** True when the change counter value has been readed (i.e changeCounter field is relevant)*/
    Boolean                         changeCounterInit;
    /** The phonebook ID*/
    SimUiccPbid                     pbid;
    /** True when the phonebook ID has change*/
    Boolean                         pbidHasChanged;
    /** True when the phonebook ID value has been readed (i.e pbid field is relevant)*/
    Boolean                         pbidInit;
    /** The phonebook previous UID*/
    Int16                           puid;
    /** True when the phonebook previous UID has change*/
    Boolean                         puidHasChanged;
    /** True when the phonebook previous UID value has been readed (i.e puid field is relevant)*/
    Boolean                         puidInit;
} SimUiccPbSyncInfo;

/** Store the synchronisation info before sent them to AB*/
typedef struct SimUiccPbExtEmailnfoTag
{
    /** Indicates the used email record in the (u)sim */
    Int16                           usedEmailRecords;
    /** Indicates the total email record in the (u)sim */
    Int16                           maxEmailRecords;
     /** Indicates the used ext record in the (u)sim */
    Int16                           usedExtRecords;
    /** Indicates the total ext record in the (u)sim */
    Int16                           maxExtRecords;
  
} SimUiccPbExtEmailnfo;
 
#endif
/* define  ALSU_TYP_H*/

/* END OF FILE */
