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
 * RVCCDATA.H
 * Call Control global data.  Only included by vgdata.h
 **************************************************************************/

#ifndef RVCCDATA_H
#define RVCCDATA_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

#define ESCAPE_GUARD_PERIOD   (10)

#define ATD_PAUSE_DTMF_UPPER  ('P')
#define ATD_PAUSE_DTMF_LOWER  ('p')

/***************************************************************************
 * Type Definitions
 **************************************************************************/

typedef enum VgDialNumberTypeTag
{
  /* Number format as described in GSM 04.08 section 10.5.4.7 */
  VG_DIAL_NUMBER_UNKNOWN       = 129,   /* Unknown type, IDSN format number */
  VG_DIAL_NUMBER_RESTRICTED    = 128,   /* Unknown type, unknown number format */
  VG_DIAL_NUMBER_INTERNATIONAL = 145,   /* International number type, ISDN format */
  VG_DIAL_NUMBER_NATIONAL      = 161,   /* National number type, IDSN format */
  VG_DIAL_NUMBER_NET_SPECIFIC  = 177,   /* Network specific number, ISDN format */
  VG_DIAL_NUMBER_TYPE_MAX               /* value from where no type are valid*/
} VgDialNumberType;

typedef enum CiTimerErrorTag
{
  CI_CONNECT_TIMER_EXPIRED,
  NUM_OF_TIMER_ERROR
} CiTimerError_t;

typedef enum CiCmErrorTag
{
    /* Not allow to make another call in current call state */
    CAUSE_AT_CALL_NOT_ALLOWED       =   0xF1,
    /* MM establishment fails while making outgoing call */
    CAUSE_AT_CALL_IMPOSSIBLE        =   0xFE,
    /*Lower layer connection fails */
    CAUSE_AT_CALL_LOWER_FAIL        =   0xFF
}CiCmError_t;

typedef enum CiCallReleaseTypeTag
{
  CI_CALL_RELEASE_ERROR_APEX,
  CI_CALL_RELEASE_ERROR_TIMER,
  CI_CALL_RELEASE_ERROR_GSM_CAUSE,
  NUM_OF_CALL_RELEASE_ERROR_TYPES
} CiCallReleaseType_t;

/* result of Call control on SIM */
typedef enum VgMCCSTType
{
    VG_MCCST_SMS          = 1,
    VG_MCCST_GPRS_CONTEXT = 4
}VgMCCSTType;

typedef enum VgMCCSTEventTag
{
  VG_MCCST_ALLOWED         = 0,
  VG_MCCST_MODIFIED        = 1,
  VG_MCCST_BARRED          = 2,
  VG_MCCST_CALL_REPLACED_BY_SS  = 3,
  VG_MCCST_CALL_REPLACED_BY_USSD = 4,
  VG_MCCST_SS_REPLACED_BY_USSD = 5,
  VG_MCCST_SS_REPLACED_BY_CALL = 6,
  VG_MCCST_USSD_REPLACED_BY_CALL = 7,
  VG_MCCST_USSD_REPLACED_BY_SS = 8
} VgMCCSTEventType;

/***************************************************************************
 *  Macros
 **************************************************************************/

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

#endif  /* RVCCDATA_H */

/* END OF FILE */




