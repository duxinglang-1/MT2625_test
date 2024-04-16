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
 *      types common to ts_sig.h and smrl_sig.h
 **************************************************************************/

#ifndef TL_TYP_H
#define TL_TYP_H

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

#if !defined (SML3_TYP_H)
#include     <sml3_typ.h>
#endif

#if !defined (SITL_TYP_H)
#include     <sitl_typ.h>
#endif

/****************************************************************************
 * Macros
 ****************************************************************************/

#define     NO_RP_CAUSE_DIAGNOSTIC      0

/* MAX_TPDU_INFO_LENGTH
**
** Maximum amount of user data associated with an RP message. GSM 24.011,
** section 7.3 gives the following sizes for the various RP message types:
**
**    RP-DATA   233
**    RP-ACK    234
**    RP-ERROR  234
**
** One byte is IE identifier second byte is length indicator
*/
#define     MAX_TPDU_INFO_LENGTH        232

/* SMS_MAX_RAW_ADDRESS_DATA
**
** This is the maximum amount of data that can be transfered in either an
** RP-Originator Address or RP-Destination Address (excluding the length
** byte).
*/
#define     SMS_MAX_RAW_ADDRESS_DATA   11

/****************************************************************************
 * Types
 ****************************************************************************/

/** Informs the originating MS of the outcome of a short message submitted to an SME */
typedef enum SmsStatusOfReportTag
{
   /** Successfully delivered to the Short Message Entity (SME) */
   TRANSFER_OK                  =   0,
   /** The Service Centre (SC) was not able to forward the message to the SME */
   TRANSFER_ERROR
}
SmsStatusOfReport;

/** Identifies the cause of faillure SMS transfer */
typedef struct RpCauseElementTag
{
    /** Gives the reason why a short message transfer attempt fails */
    GsmCause                        cause;
    /** Diagnostic information */
    Int8                            diagnostic;
}
RpCauseElement;

/** RP user data element */
typedef struct RpUserDataElementTag
{
    /** The length of TPDU information element 
\assoc ARRAY \ref tpduInformation */
    Int16                           tpduInfoLength;
    /** TPDU information element */
    Int8                            tpduInformation[MAX_TPDU_INFO_LENGTH];
}
RpUserDataElement;
/** RP user data */
typedef struct RpUserDataTag
{
    /** Length indicator 
\assoc ARRAY \ref data */
    Int8                            length;
    /** Holds TPDU  */
    Int8                            data[MAX_TPDU_INFO_LENGTH - 2];
}
RpUserData;

/** Used to hold a raw RP-Destination Address or RP-Originator Address in both cases 
 * it represents the Service Centre address
 */
typedef struct RawSmsAddressTag
{
    /** The length of data 
\assoc ARRAY \ref data */
    Int8     length;
    /** Contains the service centre address */
    Int8     data[SMS_MAX_RAW_ADDRESS_DATA];

} RawSmsAddress;

#endif
/* END OF FILE */







































