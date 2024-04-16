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
 *      types common to ts_sig.h smrl_sig.h and alsi_sig.h
 **************************************************************************/

#ifndef SITL_TYP_H
#define SITL_TYP_H

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

#include <system.h>

/****************************************************************************
 * Macros
 ****************************************************************************/

#define     SMS_MAX_ADDR_LEN            20
#define     TIMESTAMP_SIZE              14
#define     SMS_MAX_CMD_LEN             157
#define     SMS_MAX_SMSR_DATA_LEN       143
#define     SMS_MAX_MSG_LEN             160
#define     SMS_MAX_PDU_DATA_LEN        140

/****************************************************************************
 * Types
 ****************************************************************************/

/** Specifies the type of number */
typedef enum TypeOfNumberTag
{
   /** Unknown */
   NUM_UNKNOWN                      =   0,
   /** International number */
   NUM_INTERNATIONAL,
   /** National number */
   NUM_NATIONAL,
   /** Network specific number */
   NUM_NETWORK_SPEC,
   /** Subscriber number */
   NUM_SUBSCRIBER,
   /** Alphanumeric, (coded according to GSM TS 03.38 7 bit default alphabet)*/
   NUM_ALPHANUMERIC,
   /** Abbreviated number */
   NUM_ABBREVIATED,
   /** Reserved for extension */
   NUM_RESERVED
}
TypeOfNumber;

/** Indicates to which numbering plan an associated number belongs.*/
typedef enum NumberingPlanTag
{
   /** Unknown */
   PLAN_UNKNOWN                     =   0,
   /** ISDN/telephone numbering plan (E.164/E.163)*/
   PLAN_ISDN                        =   1,
   /** Data numbering plan (X.121)*/
   PLAN_DATA                        =   3,
   /** Telex numbering plan */
   PLAN_TELEX                       =   4,
   /** National numbering plan */
   PLAN_NATIONAL                    =   8,
   /** Private numbering plan */
   PLAN_PRIVATE                     =   9,
   /** ERMES numbering plan (ETSI DE/PS 3 01 3) */
   PLAN_ERMES                       =   10,
   /** Reserved for extension */
   PLAN_RESERVED                    =   15
}
NumberingPlan;

/** Reports the reason for failure to transfer or process a short message */
typedef enum TpFailureCauseTag
{
   /*
   ** Protocol Id Errors
   */
   /** Telematic interworking not supported */
   TP_TELEMATIC_NOT_SUPPORTED        =   0x80,
   /** Short message Type 0 not supported */
   TP_MSG_TYPE0_NOT_SUPPORTED,
   /** Cannot replace short message */
   TP_CANT_REPLACE_SHORT_MSG,
   /** Unspecified TP-PID error */
   TP_UNSPECIFIED_TPPID_ERROR        =   0x8F,

   /*
   ** Data Coding Scheme Errors
   */
   /** Data coding scheme (alphabet) not supported */
   TP_ALPHABET_NOT_SUPPORTED         =   0x90,
   /** Message class not supported */
   TP_MSG_CLASS_NOT_SUPPORTED,
   /** Unspecified TP-DCS error */
   TP_UNSPECIFIED_DATA_CODING_ERROR  =   0x9F,

   /*
   ** Command Errors
   */
   /** Command cannot be actioned */
   TP_CANT_ACTION_COMMAND            =   0xA0,
   /** Command unsupported */
   TP_UNSUPPORTED_COMMAND,
   /** Unspecified TP-Command error */
   TP_UNSPECIFIED_COMMAND_ERROR      =   0xAF,

   /*
   ** Other Errors
   */
   /** TPDU not supported */
   TP_TPDU_NOT_SUPPORTED             =   0xB0,

   /** SC busy */
   TP_SC_BUSY                        =   0xC0,
   /** No SC subscription */
   TP_NO_SC_SUBSCRIPTION,
   /** SC system failure */
   TP_SC_SYSTEM_FAILURE,
   /** Invalid SME address */
   TP_INVALID_SME_ADDRESS,
   /** Destination SME barred */
   TP_DESTINATION_SME_BARRED,
   /** SM Rejected-Duplicate SM */
   TP_SM_REJECTED_DUPLICATE_SM,

   /** SIM SMS storage full */
   TP_SIM_STORAGE_FULL               =   0xD0,
   /** No SMS storage capability in SIM */
   TP_SIM_NO_SMS_CAPABILITY,
   /** REDUNDANT Do not use */
   TP_SIM_NO_CAPACITY                =   0xD1,   
   /** Error in MS */
   TP_ERROR_IN_MS,
   /** Memory Capacity Exceeded */
   TP_MEMORY_CAPACITY_EXCEEDED,
   /** SIM Application Toolkit Busy */
   TP_SIM_APPLICATION_TOOLKIT_BUSY,
   /** SIM data download error */
   TP_SIM_DATA_DOWNLOAD_ERROR,

   /** Unspecified error cause */
   TP_UNSPECIFIED_ERROR              =   0xFF,

   /** Internal code */
   TP_NO_ERROR                       =   0x100   
}
TpFailureCause;

/** Array of type Int8 */ 
typedef Int8
SmsTimeStamp[TIMESTAMP_SIZE];

/** SMS address element */
typedef struct SmsAddressTag
{
   /** Is an integer representation of the number of useful semi octets 
    * within the Address Value field, for instance excludes any semi octet 
    * containing only fill bits
    
\assoc ARRAY \ref addressValue */
   Int8                             length;
   /** Specifies the type of number */
   TypeOfNumber                     typeOfNumber;
   /** Indicates to which numbering plan an associated number belongs.*/
   NumberingPlan                    numberingPlan;
   /** Address data */
   Int8                             addressValue[SMS_MAX_ADDR_LEN];
}
SmsAddress;

#endif
/* END OF FILE */





































