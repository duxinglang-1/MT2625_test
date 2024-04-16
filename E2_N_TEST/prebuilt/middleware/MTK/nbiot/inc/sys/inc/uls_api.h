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

/*---------------------------------------------------------------------------
 | Filename:
 | ---------
 |   uls_api.h
 |
 | Project:
 | --------
 |   UMOLY
 |
 | Description:
 | ------------
 |   Unified Logging System (ULS) API Definition
 |
 | Author:
 | -------
 |   mtk08910
 |
 |---------------------------------------------------------------------------
 | $Log$
 |
 ---------------------------------------------------------------------------*/

#ifndef _ULS_API_H_
#define _ULS_API_H_


#ifdef USE_ULS
#include "uls_reg.h"
#include <kal_general_types.h>

#if defined(USE_ULS_SIMULATOR)
#include <assert.h>
#endif
/*---------------------------------------------------------------------------
 | @file uls_api.h
 |
 | @brief MediaTek Unified Logging System (ULS) API definition File
 |
 |  This API allows the integration to the ULS libraries.
 |
 ---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif
/*---------------------------------------------------------------------------
 | @defgroup ULS HW API : ULS HW API Definition
 | @{
 |
 | @brief ULS API
 |
 | This module exposes structures and functions to integrate the ULS logging HW.
 |
 ---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 | @macro ULS_READ 
 | @brief Read the contents of a ULS register.
 |
 | @param[in] uls_reg - name of ULS Register to read. 
 |
 | @return kal_uint32 data read from the specified ULS register.
 |
 ---------------------------------------------------------------------------*/
#define ULS_READ(uls_reg)			  *(volatile kal_uint32 *)(uls_reg)

/*---------------------------------------------------------------------------
 | @macro ULS_WRITE
 | @brief Write a 32bit data to the specified ULS register.
 |
 | @param[in] uls_reg - Name of the ULS Register to be updated. 
 | @param[in] data    - 32 bit data word.
 |
 ---------------------------------------------------------------------------*/
#define ULS_WRITE(uls_reg, data)	  ULS_READ(uls_reg) = (kal_uint32)(data)

#if defined(USE_ULS_SIMULATOR)
extern void uls_sim_write(kal_uint32 uls_reg, kal_uint32 data);
extern kal_uint32 uls_sim_read(kal_uint32 uls_reg);
#  undef ULS_READ
#  undef ULS_WRITE
#  define ULS_READ(uls_reg)			  uls_sim_read(uls_reg);
#  define ULS_WRITE(uls_reg, data)	  uls_sim_write(uls_reg, data);

/*---------------------------------------------------------------------------
 | Trace macros to log to ULS
 ---------------------------------------------------------------------------*/ 
#define ULS_LOG_1_DATA(v1)            ULS_WRITE (REG_ULS_SINGLE, (kal_uint32) v1)

#define ULS_LOG_2_DATA(v1, v2)        ULS_WRITE (REG_ULS_START, (kal_uint32) v1); \
                                      ULS_WRITE (REG_ULS_END,   (kal_uint32) v2)

#define ULS_LOG_3_DATA(v1, v2, v3)    ULS_WRITE (REG_ULS_START, (kal_uint32) v1); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v2); \
                                      ULS_WRITE (REG_ULS_END,   (kal_uint32) v3)

#define ULS_LOG_4_DATA(v1, v2, v3, v4)                                            \
                                      ULS_WRITE (REG_ULS_START, (kal_uint32) v1); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v2); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v3); \
                                      ULS_WRITE (REG_ULS_END,   (kal_uint32) v4)

#define ULS_LOG_5_DATA(v1, v2, v3, v4, v5)                                        \
                                      ULS_WRITE (REG_ULS_START, (kal_uint32) v1); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v2); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v3); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v4); \
                                      ULS_WRITE (REG_ULS_END,   (kal_uint32) v5)

#define ULS_LOG_6_DATA(v1, v2, v3, v4, v5, v6)                                    \
                                      ULS_WRITE (REG_ULS_START, (kal_uint32) v1); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v2); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v3); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v4); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v5); \
                                      ULS_WRITE (REG_ULS_END,   (kal_uint32) v6)

#define ULS_LOG_7_DATA(v1, v2, v3, v4, v5, v6, v7)                                \
                                      ULS_WRITE (REG_ULS_START, (kal_uint32) v1); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v2); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v3); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v4); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v5); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v6); \
                                      ULS_WRITE (REG_ULS_END,   (kal_uint32) v7)

#define ULS_LOG_8_DATA(v1, v2, v3, v4, v5, v6, v7, v8)                            \
                                      ULS_WRITE (REG_ULS_START, (kal_uint32) v1); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v2); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v3); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v4); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v5); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v6); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v7); \
                                      ULS_WRITE (REG_ULS_END,   (kal_uint32) v8)

#define ULS_LOG_9_DATA(v1, v2, v3, v4, v5, v6, v7, v8, v9)                            \
                                      ULS_WRITE (REG_ULS_START, (kal_uint32) v1); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v2); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v3); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v4); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v5); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v6); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v7); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v8); \
                                      ULS_WRITE (REG_ULS_END,   (kal_uint32) v9)

#define ULS_LOG_10_DATA(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)                            \
                                      ULS_WRITE (REG_ULS_START, (kal_uint32) v1); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v2); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v3); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v4); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v5); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v6); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v7); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v8); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v9); \
                                      ULS_WRITE (REG_ULS_END,   (kal_uint32) v10)

#define ULS_LOG_11_DATA(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)                            \
                                      ULS_WRITE (REG_ULS_START, (kal_uint32) v1); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v2); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v3); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v4); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v5); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v6); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v7); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v8); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v9); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v10); \
                                      ULS_WRITE (REG_ULS_END,   (kal_uint32) v11)

#define ULS_LOG_12_DATA(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12)                            \
                                      ULS_WRITE (REG_ULS_START, (kal_uint32) v1); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v2); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v3); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v4); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v5); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v6); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v7); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v8); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v9); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v10); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v11); \
                                      ULS_WRITE (REG_ULS_END,   (kal_uint32) v12)

#define ULS_LOG_13_DATA(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13)                            \
                                      ULS_WRITE (REG_ULS_START, (kal_uint32) v1); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v2); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v3); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v4); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v5); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v6); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v7); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v8); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v9); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v10); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v11); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v12); \
                                      ULS_WRITE (REG_ULS_END,   (kal_uint32) v13)

#define ULS_LOG_14_DATA(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14)   \
                                      ULS_WRITE (REG_ULS_START, (kal_uint32) v1); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v2); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v3); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v4); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v5); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v6); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v7); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v8); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v9); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v10); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v11); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v12); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v13); \
                                      ULS_WRITE (REG_ULS_END,   (kal_uint32) v14)

#define ULS_LOG_15_DATA(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15)   \
                                      ULS_WRITE (REG_ULS_START, (kal_uint32) v1); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v2); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v3); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v4); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v5); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v6); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v7); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v8); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v9); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v10); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v11); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v12); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v13); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v14); \
                                      ULS_WRITE (REG_ULS_END,   (kal_uint32) v15)

#define ULS_LOG_16_DATA(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16)   \
                                      ULS_WRITE (REG_ULS_START, (kal_uint32) v1); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v2); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v3); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v4); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v5); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v6); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v7); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v8); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v9); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v10); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v11); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v12); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v13); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v14); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v15); \
                                      ULS_WRITE (REG_ULS_END,   (kal_uint32) v16)

#define ULS_LOG_17_DATA(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17)   \
                                      ULS_WRITE (REG_ULS_START, (kal_uint32) v1); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v2); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v3); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v4); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v5); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v6); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v7); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v8); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v9); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v10); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v11); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v12); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v13); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v14); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v15); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v16); \
                                      ULS_WRITE (REG_ULS_END,   (kal_uint32) v17)

#define ULS_LOG_18_DATA(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18)   \
                                      ULS_WRITE (REG_ULS_START, (kal_uint32) v1); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v2); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v3); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v4); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v5); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v6); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v7); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v8); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v9); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v10); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v11); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v12); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v13); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v14); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v15); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v16); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v17); \
                                      ULS_WRITE (REG_ULS_END,   (kal_uint32) v18)

#define ULS_LOG_19_DATA(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19)   \
                                      ULS_WRITE (REG_ULS_START, (kal_uint32) v1); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v2); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v3); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v4); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v5); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v6); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v7); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v8); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v9); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v10); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v11); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v12); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v13); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v14); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v15); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v16); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v17); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v18); \
                                      ULS_WRITE (REG_ULS_END,   (kal_uint32) v19)

#define ULS_LOG_20_DATA(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20)   \
                                      ULS_WRITE (REG_ULS_START, (kal_uint32) v1); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v2); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v3); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v4); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v5); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v6); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v7); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v8); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v9); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v10); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v11); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v12); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v13); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v14); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v15); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v16); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v17); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v18); \
                                      ULS_WRITE (REG_ULS_BODY,  (kal_uint32) v19); \
                                      ULS_WRITE (REG_ULS_END,   (kal_uint32) v20)
#else

/*---------------------------------------------------------------------------
 | Trace macros to log to ULS
 ---------------------------------------------------------------------------*/ 
#define ULS_LOG_1_DATA( v1 )          ULS_WRITE ( REG_ULS_SINGLE, (kal_uint32) v1)

#define ULS_LOG_2_DATA( v1, v2)       ULS_WRITE ( REG_ULS_START, ((((kal_uint32) v1)<<16)|1) ); \
                                      ULS_WRITE ( REG_ULS_END,   (kal_uint32) v2)

#define ULS_LOG_3_DATA( v1, v2, v3)   ULS_WRITE ( REG_ULS_START, ((((kal_uint32) v1)<<16)|2) ); \
                                      ULS_WRITE ( REG_ULS_END,   ((((kal_uint32) v3)<<16)|(kal_uint16) v2) )

#define ULS_LOG_4_DATA( v1, v2, v3, v4)                                            \
                                      ULS_WRITE ( REG_ULS_START, ((((kal_uint32) v1)<<16)|3) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v3)<<16)|(kal_uint16) v2) ); \
                                      ULS_WRITE ( REG_ULS_END,   (kal_uint32) v4)

#define ULS_LOG_5_DATA( v1, v2, v3, v4, v5)                                        \
                                      ULS_WRITE ( REG_ULS_START, ((((kal_uint32) v1)<<16)|4) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v3)<<16)|(kal_uint16) v2) ); \
                                      ULS_WRITE ( REG_ULS_END,   ((((kal_uint32) v5)<<16)|(kal_uint16) v4) )

#define ULS_LOG_6_DATA( v1, v2, v3, v4, v5, v6)                                    \
                                      ULS_WRITE ( REG_ULS_START, ((((kal_uint32) v1)<<16)|5) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v3)<<16)|(kal_uint16) v2) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v5)<<16)|(kal_uint16) v4) ); \
                                      ULS_WRITE ( REG_ULS_END,   (kal_uint32) v6)

#define ULS_LOG_7_DATA( v1, v2, v3, v4, v5, v6, v7)                                \
                                      ULS_WRITE ( REG_ULS_START, ((((kal_uint32) v1)<<16)|6) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v3)<<16)|(kal_uint16) v2) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v5)<<16)|(kal_uint16) v4) ); \
                                      ULS_WRITE ( REG_ULS_END,   ((((kal_uint32) v7)<<16)|(kal_uint16) v6) )

#define ULS_LOG_8_DATA( v1, v2, v3, v4, v5, v6, v7, v8)                            \
                                      ULS_WRITE ( REG_ULS_START, ((((kal_uint32) v1)<<16)|7) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v3)<<16)|(kal_uint16) v2) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v5)<<16)|(kal_uint16) v4) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v7)<<16)|(kal_uint16) v6) ); \
                                      ULS_WRITE ( REG_ULS_END,   (kal_uint32) v8)

#define ULS_LOG_9_DATA( v1, v2, v3, v4, v5, v6, v7, v8, v9)                            \
                                      ULS_WRITE ( REG_ULS_START, ((((kal_uint32) v1)<<16)|8) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v3)<<16)|(kal_uint16) v2) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v5)<<16)|(kal_uint16) v4) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v7)<<16)|(kal_uint16) v6) ); \
                                      ULS_WRITE ( REG_ULS_END,   ((((kal_uint32) v9)<<16)|(kal_uint16) v8) )

#define ULS_LOG_10_DATA( v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)                            \
                                      ULS_WRITE ( REG_ULS_START, ((((kal_uint32) v1)<<16)|9) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v3)<<16)|(kal_uint16) v2) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v5)<<16)|(kal_uint16) v4) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v7)<<16)|(kal_uint16) v6) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v9)<<16)|(kal_uint16) v8) ); \
                                      ULS_WRITE ( REG_ULS_END,   (kal_uint32) v10)

#define ULS_LOG_11_DATA( v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)                            \
                                      ULS_WRITE ( REG_ULS_START, ((((kal_uint32) v1)<<16)|10) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v3)<<16)|(kal_uint16) v2) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v5)<<16)|(kal_uint16) v4) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v7)<<16)|(kal_uint16) v6) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v9)<<16)|(kal_uint16) v8) ); \
                                      ULS_WRITE ( REG_ULS_END,   ((((kal_uint32) v11)<<16)|(kal_uint16) v10) )

#define ULS_LOG_12_DATA( v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12)                            \
                                      ULS_WRITE ( REG_ULS_START, ((((kal_uint32) v1)<<16)|11) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v3)<<16)|(kal_uint16) v2) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v5)<<16)|(kal_uint16) v4) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v7)<<16)|(kal_uint16) v6) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v9)<<16)|(kal_uint16) v8) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v11)<<16)|(kal_uint16) v10) ); \
                                      ULS_WRITE ( REG_ULS_END,   (kal_uint32) v12)

#define ULS_LOG_13_DATA( v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13)                            \
                                      ULS_WRITE ( REG_ULS_START, ((((kal_uint32) v1)<<16)|12) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v3)<<16)|(kal_uint16) v2) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v5)<<16)|(kal_uint16) v4) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v7)<<16)|(kal_uint16) v6) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v9)<<16)|(kal_uint16) v8) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v11)<<16)|(kal_uint16) v10) ); \
                                      ULS_WRITE ( REG_ULS_END,   ((((kal_uint32) v13)<<16)|(kal_uint16) v12) )

#define ULS_LOG_14_DATA( v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14)   \
                                      ULS_WRITE ( REG_ULS_START, ((((kal_uint32) v1)<<16)|13) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v3)<<16)|(kal_uint16) v2) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v5)<<16)|(kal_uint16) v4) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v7)<<16)|(kal_uint16) v6) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v9)<<16)|(kal_uint16) v8) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v11)<<16)|(kal_uint16) v10) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v13)<<16)|(kal_uint16) v12) ); \
                                      ULS_WRITE ( REG_ULS_END,   (kal_uint32) v14)

#define ULS_LOG_15_DATA( v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15)   \
                                      ULS_WRITE ( REG_ULS_START, ((((kal_uint32) v1)<<16)|14) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v3)<<16)|(kal_uint16) v2) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v5)<<16)|(kal_uint16) v4) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v7)<<16)|(kal_uint16) v6) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v9)<<16)|(kal_uint16) v8) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v11)<<16)|(kal_uint16) v10) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v13)<<16)|(kal_uint16) v12) ); \
                                      ULS_WRITE ( REG_ULS_END,   ((((kal_uint32) v15)<<16)|(kal_uint16) v14) )

#define ULS_LOG_16_DATA( v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16)   \
                                      ULS_WRITE ( REG_ULS_START, ((((kal_uint32) v1)<<16)|15) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v3)<<16)|(kal_uint16) v2) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v5)<<16)|(kal_uint16) v4) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v7)<<16)|(kal_uint16) v6) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v9)<<16)|(kal_uint16) v8) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v11)<<16)|(kal_uint16) v10) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v13)<<16)|(kal_uint16) v12) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v15)<<16)|(kal_uint16) v14) ); \
                                      ULS_WRITE ( REG_ULS_END,   (kal_uint32) v16)

#define ULS_LOG_17_DATA( v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17)   \
                                      ULS_WRITE ( REG_ULS_START, ((((kal_uint32) v1)<<16)|16) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v3)<<16)|(kal_uint16) v2) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v5)<<16)|(kal_uint16) v4) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v7)<<16)|(kal_uint16) v6) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v9)<<16)|(kal_uint16) v8) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v11)<<16)|(kal_uint16) v10) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v13)<<16)|(kal_uint16) v12) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v15)<<16)|(kal_uint16) v14) ); \
                                      ULS_WRITE ( REG_ULS_END,   ((((kal_uint32) v17)<<16)|(kal_uint16) v16) )

#define ULS_LOG_18_DATA( v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18)   \
                                      ULS_WRITE ( REG_ULS_START, ((((kal_uint32) v1)<<16)|17) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v3)<<16)|(kal_uint16) v2) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v5)<<16)|(kal_uint16) v4) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v7)<<16)|(kal_uint16) v6) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v9)<<16)|(kal_uint16) v8) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v11)<<16)|(kal_uint16) v10) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v13)<<16)|(kal_uint16) v12) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v15)<<16)|(kal_uint16) v14) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v17)<<16)|(kal_uint16) v16) ); \
                                      ULS_WRITE ( REG_ULS_END,   (kal_uint32) v18)

#define ULS_LOG_19_DATA( v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19)   \
                                      ULS_WRITE ( REG_ULS_START, ((((kal_uint32) v1)<<16)|18) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v3)<<16)|(kal_uint16) v2) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v5)<<16)|(kal_uint16) v4) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v7)<<16)|(kal_uint16) v6) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v9)<<16)|(kal_uint16) v8) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v11)<<16)|(kal_uint16) v10) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v13)<<16)|(kal_uint16) v12) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v15)<<16)|(kal_uint16) v14) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v17)<<16)|(kal_uint16) v16) ); \
                                      ULS_WRITE ( REG_ULS_END,   ((((kal_uint32) v19)<<16)|(kal_uint16) v18) )

#define ULS_LOG_20_DATA( v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20)   \
                                      ULS_WRITE ( REG_ULS_START, ((((kal_uint32) v1)<<16)|19) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v3)<<16)|(kal_uint16) v2) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v5)<<16)|(kal_uint16) v4) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v7)<<16)|(kal_uint16) v6) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v9)<<16)|(kal_uint16) v8) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v11)<<16)|(kal_uint16) v10) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v13)<<16)|(kal_uint16) v12) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v15)<<16)|(kal_uint16) v14) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v17)<<16)|(kal_uint16) v16) ); \
                                      ULS_WRITE ( REG_ULS_BODY,  ((((kal_uint32) v19)<<16)|(kal_uint16) v18) ); \
                                      ULS_WRITE ( REG_ULS_END,   (kal_uint32) v20)

#endif

/*---------------------------------------------------------------------------
 | Timestamps
 ---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
 | @macro ULS_DISABLE_TIMESTAMP
 | @brief Disable all timestamps (global/local/compatibility)
 |
 ---------------------------------------------------------------------------*/
#define ULS_DISABLE_TIMESTAMP() ULS_WRITE (REG_ULS_TIMESTAMP_MODE, ULS_READ(REG_ULS_TIMESTAMP_MODE) | ULS_DISABLE_TIMESTAMP_MODE)

/*---------------------------------------------------------------------------
 | @macro ULS_ENABLE_TIMESTAMP
 | @brief Enables all timestamps mode
 |
 ---------------------------------------------------------------------------*/
#define ULS_ENABLE_TIMESTAMP() \
                ULS_WRITE (REG_ULS_TIMESTAMP_MODE, ULS_READ(REG_ULS_TIMESTAMP_MODE) & ~ULS_DISABLE_TIMESTAMP_MODE)

/*---------------------------------------------------------------------------
 | @macro ULS_SET_GLOBAL_TIMESTAMP
 | @brief Configures ULS to use global timestamps.
 |
 ---------------------------------------------------------------------------*/
#define ULS_SET_GLOBAL_TIMESTAMP() \
                ULS_WRITE (REG_ULS_TIMESTAMP_MODE, ULS_READ(REG_ULS_TIMESTAMP_MODE) | ULS_GLOBAL_TIMESTAMP_MODE)

/*---------------------------------------------------------------------------
 | @macro ULS_SET_LOCAL_TIMESTAMP
 | @brief Configures ULS to use local timestamps.
 | 
 | To set ULS_LOCAL_TIMESTAMP_MODE, we clear ULS_GLOBAL_TIMESTAMP_MODE bit.
 ---------------------------------------------------------------------------*/
#define ULS_SET_LOCAL_TIMESTAMP() \
                ULS_WRITE (REG_ULS_TIMESTAMP_MODE, ULS_READ(REG_ULS_TIMESTAMP_MODE) & ~ULS_GLOBAL_TIMESTAMP_MODE)

/*---------------------------------------------------------------------------
 | @macro ULS_INSERT_LOCAL_TIMESTAMP
 | @brief Insert a short 16bit timestamp to the output of ULS.
 |
 | Calls to this function has no effect when the ULS_TIMESTAMP_MODE_REG has
 | bit 1 set (Disable local interrupts) or when the timestamp mode is set to global.
 ---------------------------------------------------------------------------*/
#define ULS_INSERT_LOCAL_TIMESTAMP() ULS_WRITE (REG_ULS_TIMESTAMP_INSERT, ULS_INSERT_TIMESTAMP)

 /*---------------------------------------------------------------------------
 | @macro ULS_ADD_COMPATIBILITY_TIMESTAMP
 | @brief Insert a compatibility 32bit timestamp to the output of ULS.
 |
 | Calls to this function has no effect when the ULS_TIMESTAMP_MODE_REG has
 | bit 1 set (Disable timestamp interrupts).
 ---------------------------------------------------------------------------*/
#define ULS_ADD_COMPATIBILITY_TIMESTAMP() ULS_WRITE (REG_ULS_ADD_COMPATIBILITY_TIMESTAMP, ULS_INSERT_TIMESTAMP)

 /*---------------------------------------------------------------------------
 | ULS Control
 ---------------------------------------------------------------------------*/

 /*---------------------------------------------------------------------------
 | @macro ULS_ENABLE_LOGGING
 | @brief Enables ULS Logging
 |
 ---------------------------------------------------------------------------*/
#define ULS_ENABLE() ULS_WRITE (REG_ULS_ENABLE_LOGGING, ULS_READ(REG_ULS_ENABLE_LOGGING) | ULS_ENABLE_LOGGING)

 /*---------------------------------------------------------------------------
 | @macro ULS_ENABLE_LOGGING
 | @brief Disables ULS Logging
 |
 ---------------------------------------------------------------------------*/
#define ULS_DISABLE() ULS_WRITE (REG_ULS_ENABLE_LOGGING, ULS_READ(REG_ULS_ENABLE_LOGGING) & ~ULS_ENABLE_LOGGING)

 /*---------------------------------------------------------------------------
 | @macro ULS_HALT
 | @brief Halts ULS Logging
 |
 ---------------------------------------------------------------------------*/
#define ULS_HALT() ULS_WRITE (REG_ULS_ENABLE_LOGGING, ULS_READ(REG_ULS_ENABLE_LOGGING) | ULS_GLOBAL_HALT)

 /*---------------------------------------------------------------------------
 | @macro ULS_RESUME
 | @brief Resumes ULS Logging (from Halt mode)
 |
 ---------------------------------------------------------------------------*/
#define ULS_RESUME() ULS_WRITE (REG_ULS_ENABLE_LOGGING, ULS_READ(REG_ULS_ENABLE_LOGGING) & ~ULS_GLOBAL_HALT)

 /*---------------------------------------------------------------------------
 | @macro ULS_SOFT_RESET
 | @brief Soft Resets ULS Logging
 |
 ---------------------------------------------------------------------------*/
#define ULS_SOFT_RESET() ULS_WRITE (REG_ULS_SOFT_RESET, ULS_SOFT_RESET_VALUE)

 /*---------------------------------------------------------------------------
 | @macro ULS_FLUSH_OUTPUT_BUFFER
 | @brief Flush Output Buffer
 |
 ---------------------------------------------------------------------------*/
#define ULS_FLUSH_OUTPUT_BUFFER() ULS_WRITE (REG_ULS_OUTPUT_BUFFER_FLUSH, 1)

 /*---------------------------------------------------------------------------
 | @macro ULS_RING_BUFFER_WRITE_POINTER
 | @brief Obtains the EMI write pointer marking the end of the ring buffer.
 |
 | @return EMI write pointer (end of ring buffer) 
 ---------------------------------------------------------------------------*/
#define ULS_RING_BUFFER_WRITE_POINTER() ULS_READ (REG_ULS_DDR_COMPLETED_WRITE_POINTER)

/*---------------------------------------------------------------------------
 | @macro ULS_RING_BUFFER_READ_POINTER
 | @brief Obtains the EMI read pointer marking the start of the ring buffer. 
 |
 | @return EMI read pointer (start of buffer).
 ---------------------------------------------------------------------------*/
#define ULS_RING_BUFFER_READ_POINTER() ULS_READ (REG_ULS_DDR_READ_POINTER)

 /*---------------------------------------------------------------------------
 | @function ULS_UPDATE_RING_BUFFER_READ_POINTER
 | @brief Updates the ring buffer read pointer. 
 | @param[in] read_ptr      - new read pointer to the ring buffer
 | 
 | This function is called whenever the EMI ring buffer contents have been
 | transferred to the AP (to SDCard) or USB port.
 ---------------------------------------------------------------------------*/
#define ULS_SET_RING_BUFFER_READ_POINTER(read_ptr) \
                    ULS_WRITE (REG_ULS_DDR_READ_POINTER, (kal_uint32) read_ptr)

/*---------------------------------------------------------------------------
 | @function uls_ring_buffer_size
 | @brief Obtains the size of the data held in the EMI ring buffer. 
 |
 | @return size of the EMI ring buffer in bytes.
 ---------------------------------------------------------------------------*/
#define ULS_RING_BUFFER_SIZE() ULS_READ (REG_ULS_DDR_BYTE_COUNT)
 
 /*---------------------------------------------------------------------------
 | @function ULS_SET_RING_BUFFER_THRESHOLD
 | @brief Configures the EMI ring buffer threshold level at which an interrupt
 | will be generated.
 |
 | @param[in] uls_threshold - size in bytes which when exceeded generate an interrupt.
 |                                        
 ---------------------------------------------------------------------------*/
#define ULS_SET_RING_BUFFER_THRESHOLD(uls_threshold) \
                    ULS_WRITE (REG_ULS_DDR_BLOCK_SIZE, (kal_uint32) uls_threshold)

/*---------------------------------------------------------------------------
 | @typedef uls_status_struct
 |
 | @brief Report the status of ULS HW.
 |
 ---------------------------------------------------------------------------*/
typedef enum
{
    ULS_ACTIVE = 0,                     /**< ULS Operating normally          */
    ULS_HAULT_SW,                       /**< ULS haulted by the SW           */
    ULS_HAULT_RING_BUFFER_FULL,         /**< ULS haulted when the buffer is full */
    ULS_RING_BUFFER_FULL,               /**< EMI ring buffer has become FULL */
    ULS_RING_BUFFER_EXCEEDS_THRESHOLD,  /**< EMI ring buffer has exceed the  */
                                        /**< configured threshold limit      */
    ULS_FLUSH_IN_PROGRESS,              /**< ULS is flushing the input FIFOs */
    ULS_SOFT_RESET_IN_PROGRESS          /**< ULS is performing a soft reset  */
}
uls_status_struct;

typedef enum
{
    ULS_PRIMITIVE               = 0,
    ULS_INDEX_TRACE             = 1,
    ULS_RAW_BYTE                = 2,
    ULS_RAW_STRING_TYPE         = 3,
    ULS_DISCARD_INFO            = 4, 
    ULS_TIME_INFO               = 5,
    ULS_RTL_PRIMITIVE           = 6,
    ULS_PRIMITIVE_ADT           = 7,
/*  ULS_L1_TRACE                = 8,      Now specified within Flag Byte */
    ULS_META_CNF_PRIMITIVE      = 9,
/*  ULS_L2_TRACE                = 10      Now Specified within Flag Byte */
} uls_type;

typedef enum
{
	ULS_HEADER_SYSTEM_NULL = 0,
	ULS_HEADER_SYSTEM_FIFO_OVERFLOW,
	ULS_HEADER_SYSTEM_TS_WRAP,
	ULS_HEADER_SYSTEM_TS,
	ULS_HEADER_SYSTEM_COMPATIBILITY_TS,
	ULS_HEADER_SYSTEM_SINGLE,
	ULS_HEADER_SINGLE = 0,
	ULS_HEADER_START = 0x40,
	ULS_HEADER_BODY = 0x80,
	ULS_HEADER_END = 0xC0,
} ULS_Header_Type_e;

/*---------------------------------------------------------------------------
 | FLAG Byte definition 
---------------------------------------------------------------------------*/
#define ULS_FRAME_ENABLE	(0x80)                    // Bit 7 signals a ULS frame is to follow
#define ULS_L1_TRACE		(ULS_ENABLE|0x02);        // Signal L1 TRACE follows
#define ULS_L2_TRACE		(ULS_ENABLE|0x04);        // Signal L2 Trace follows


/*---------------------------------------------------------------------------
 | @brief uls_initialise.
 |
 | Initialise the ULS HW.
 | 
 ---------------------------------------------------------------------------*/
extern void uls_initialise(void);

/*---------------------------------------------------------------------------
 | @function uls_send_data
 | @brief Write large ammount of logging data to ULS.
 |
 | @param[in] src_address - address of the data to be logged.
 | @param[in] size        - amount of data to be logged in bytes. 
 |
 | Writes logging data to ULS. The ULS HW will either stall the process or
 | discard the data should the input ULS FIFOs become full. This is according
 | to the ULS configuration.
 ---------------------------------------------------------------------------*/
extern void uls_send_data (kal_uint32 *src_address, kal_uint32 size);


/*---------------------------------------------------------------------------
 |
 | Additions needed for current project
 |
 ---------------------------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif /* USE_ULS */
#endif /* _ULS_API_H_ */
