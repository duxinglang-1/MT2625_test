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
 *      types common to alsi_sig.h and mncb_sig.h
 **************************************************************************/

#ifndef CBSI_TYP_H
#define CBSI_TYP_H

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

#include <system.h>

/****************************************************************************
 * Macros
 ****************************************************************************/

#define     SIM_LANGUAGE_LIST_SIZE      10

/****************************************************************************
 * Types
 ****************************************************************************/

/** This is the list of all valid languages, applications should make sure that
 * all selected languages are valid ones, invalid languages should be disabled */
typedef enum LanguageTag
{
    /** German */
    GSM_LANG_GERMAN                 =   0x00,
    /** English*/
    GSM_LANG_ENGLISH                =   0x01,
    /** Italian */
    GSM_LANG_ITALIAN                =   0x02,
    /** French */
    GSM_LANG_FRENCH                 =   0x03,
    /** Spanish */
    GSM_LANG_SPANISH                =   0x04,
    /** Dutch */
    GSM_LANG_DUTCH                  =   0x05,
    /** Swedish */
    GSM_LANG_SWEDISH                =   0x06,
    /** Danish */
    GSM_LANG_DANISH                 =   0x07,
    /** Portuguese */
    GSM_LANG_PORTUGUESE             =   0x08,
    /** Finnish */
    GSM_LANG_FINNISH                =   0x09,
    /** Norwegian */
    GSM_LANG_NORWEGIAN              =   0x0A,
    /** Greek */
    GSM_LANG_GREEK                  =   0x0B,
    /** Turkish */
    GSM_LANG_TURKISH                =   0x0C,
    /** Hungarian */
    GSM_LANG_HUNGARIAN              =   0x0D,
    /** Polish */
    GSM_LANG_POLISH                 =   0x0E,
    /** Language unspecified */
    GSM_LANG_UNSPECIFIED            =   0x0F,
    /** Czech */
    GSM_LANG_CZECH                  =   0x20,
    /** Hebrew */
    GSM_LANG_HEBREW                 =   0x21,
    /** Arabic */
    GSM_LANG_ARABIC                 =   0x22,
    /** Russian */
    GSM_LANG_RUSSIAN                =   0x23,
    /** Icelandic */
    GSM_LANG_ICELANDIC              =   0x24,
    /** Chinese */
    GSM_LANG_CHINESE                =   0x2F,
    /** If this number changes, we need to adjust
     * pscfg parameter MAX_LP_ENTRIES accordingly,
     * that if we want to enable all available languages */
    GSM_LANG_NUMBER_OF              =   22,
    /** Empty */
    GSM_LANG_EMPTY                  =   0xff
}
Language;

typedef struct LanguagePrefListTag
{
    /** number of languages in array 'language'
\assoc ARRAY \ref language */
    Int8                            numEntries;
    /* the field number of the first language */
    Int16                           firstEntry;
    Language                        language[SIM_LANGUAGE_LIST_SIZE];
}
LanguagePrefList;


/** Message coding element */
typedef enum CbMessageCodingTag
{
    /** Default alphabet */
    CB_DEFAULT_ALPHABET,
    /** 8 bit data */
    CB_8_BIT_DATA,
    /** USC2 (16 bit) */
    CB_UCS2_ALPHABET,
    /** Reserved */
    noMoreCodings
}
CbMessageCoding;

/** Message class element */
typedef enum CbMessageClassTag
{
    /** No message class */
    CB_NO_MESSAGE_CLASS,
    /** Class 1 Default meaning: ME-specific.*/
    CB_CLASS_1,
    /** Class 2 SIM specific message. */
    CB_CLASS_2,
    /** Class 3 Default meaning: TE-specific */
    CB_CLASS_3,
    /** Class 0 */
    CB_CLASS_0,
    noClass
}
CbMessageClass;

/** Scheme indicates the intended handling of the message at the MS, the alphabet/coding, 
 * and the language (when applicable). Any reserved codings shall be assumed to be the 
 * GSM default alphabet 
 */
typedef struct CbDataCodingTag
{
    Boolean                         embeddedLanguageInd;
    /** indicates whether or not a text Message is compressed.*/
    Boolean                         compressedText;
    /** Indicates the lenguage being used */
    Language                        language;
    /** Indicates the data coding being used */
    CbMessageCoding                 messageCoding;
    /** Indicates the message class being used */
    CbMessageClass                  messageClass;

} CbDataCoding;

#endif
/* END OF FILE */







































