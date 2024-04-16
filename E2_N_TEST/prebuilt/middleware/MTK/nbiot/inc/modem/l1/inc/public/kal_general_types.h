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
#ifndef _KAL_GENERAL_TYPES_H
#define _KAL_GENERAL_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif
/*******************************************************************************
 * Type Definitions
 *******************************************************************************/

// Fixed width integer types
#ifdef _MSC_VER
// Special type handle for enum2str codegen.
#ifdef __ENUM_GEN__
typedef signed char         S8;
typedef signed short        S16;
typedef signed long         S32;
typedef signed long long    S64;
typedef unsigned char       U8;
typedef unsigned short      U16;
typedef unsigned long       U32;
typedef unsigned long long  U64;
#else  // __ENUM_GEN__
typedef __int8              S8;
typedef __int16             S16;
typedef __int32             S32;
typedef __int64             S64;
typedef unsigned __int8     U8;
typedef unsigned __int16    U16;
typedef unsigned __int32    U32;
typedef unsigned __int64    U64;
#endif  // __ENUM_GEN__
#else  // _MSC_VER
#include <inttypes.h>
typedef int8_t              S8;
typedef int16_t             S16;
typedef int32_t             S32;
typedef int64_t             S64;
typedef uint8_t             U8;
typedef uint16_t            U16;
typedef uint32_t            U32;
typedef uint64_t            U64;
#endif

typedef char                kal_char;
typedef U16                 kal_wchar;

typedef U8                  kal_uint8;
typedef U16                 kal_uint16;
typedef U32                 kal_uint32;
typedef U64                 kal_uint64;
typedef S8                  kal_int8;
typedef S16                 kal_int16;
typedef S32                 kal_int32;
typedef S64                 kal_int64;

typedef enum
{
    KAL_FALSE,
    KAL_TRUE
} kal_bool;

/*******************************************************************************
 * Constant definition
 *******************************************************************************/
#ifndef NULL
#define NULL               0
#endif

/*******************************************************************************
 * Module, SAP, Message
 *******************************************************************************/


#ifdef __cplusplus
}
#endif

#endif

