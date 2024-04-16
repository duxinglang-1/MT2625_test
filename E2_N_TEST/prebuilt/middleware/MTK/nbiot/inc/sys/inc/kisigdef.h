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
 * GKI header file that defines the signal definition macro SIG_DEF. This is
 * used to extract the id and union member parts of the signal definition.
 **************************************************************************/

/*
** SIGNAL should be defined to be either TVSIGNAL or PSSIGNAL by the
** including file
*/
/*
** This file deliberately does not have an
** #if !defined (KISIGDEF_H)
** as it can be legitimately included twice
*/

#define TVSIGNAL 1
#define PSSIGNAL 2
#define SIGNAL_COUNT 3
/* Referenced Data Logging defines used in kirefsigdef.h */
#define REF_LOGGING_FUNC_PROTO 4
#define REF_LOGGING_FUNC 5
#define REF_LOGGING_SWITCH 6
#define REF_LOGGING_ASSOC 7
#define REF_LOGGING_PAYLOAD 8

/* Undefine all the signal definition macros */
#if defined (SIG_DEF)
#   undef SIG_DEF
#endif
#if defined (SIG_DEF_1_REF)     
#   undef SIG_DEF_1_REF         
#endif                          
#if defined (SIG_DEF_2_REF)     
#   undef SIG_DEF_2_REF         
#endif                          
#if defined (SIG_DEF_3_REF)     
#   undef SIG_DEF_3_REF         
#endif                          
#if defined (SIG_DEF_ARRAY_REF) 
#   undef SIG_DEF_ARRAY_REF     
#endif
#if defined (SIG_DEF_CUSTOM_REF)
#   undef SIG_DEF_CUSTOM_REF
#endif

#if SIGNAL == TVSIGNAL
#   define SIG_DEF(ENUM,UNION) UNION;
#   define SIG_DEF_1_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1) SIG_TYPE SIG_NAME;
#   define SIG_DEF_2_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1, N_2, T_2) SIG_TYPE SIG_NAME;
#   define SIG_DEF_3_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1, N_2, T_2, N_3, T_3) SIG_TYPE SIG_NAME;
#   define SIG_DEF_ARRAY_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1, S_1) SIG_TYPE SIG_NAME;
#   define SIG_DEF_CUSTOM_REF(SIG_ID, SIG_TYPE, SIG_NAME, PAYLOAD_TYPE, LOGGING_FUNC) SIG_TYPE SIG_NAME;
#endif /* SIGNAL == TVSIGNAL */

#if SIGNAL == PSSIGNAL
#   define SIG_DEF(ENUM,UNION) ENUM,
#   define SIG_DEF_1_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1) SIG_ID,
#   define SIG_DEF_2_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1, N_2, T_2) SIG_ID,
#   define SIG_DEF_3_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1, N_2, T_2, N_3, T_3) SIG_ID,
#   define SIG_DEF_ARRAY_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1, S_1) SIG_ID,
#   define SIG_DEF_CUSTOM_REF(SIG_ID, SIG_TYPE, SIG_NAME, PAYLOAD_TYPE, LOGGING_FUNC) SIG_ID,
#endif /* SIGNAL == PSSIGNAL */

#if SIGNAL == SIGNAL_COUNT
/* Generate an array whose byte size is the number of signals defined in the
 * system. */
#define SIG_DEF(ENUM,UNION) +1
#define SIG_DEF_1_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1) + 1
#define SIG_DEF_2_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1, N_2, T_2) +1
#define SIG_DEF_3_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1, N_2, T_2, N_3, T_3) +1
#define SIG_DEF_ARRAY_REF(SIG_ID, SIG_TYPE, SIG_NAME, N_1, T_1, S_1) +1
#define SIG_DEF_CUSTOM_REF(SIG_ID, SIG_TYPE, SIG_NAME, PAYLOAD_TYPE, LOGGING_FUNC) +1
#endif /* SIG_DEF_TYPE == SIGNAL_COUNT */

/* Include macro definitions for referenced data logging */
#include <kirefsigdef.h>

#if !defined (SIG_DEF)
#   error SIG_DEF must be defined
#endif

/*
** Example interface specification
**
** #if defined (INTF_EXAMPLE)
** SIG_DEF( EXAMPLE_KEYPRESS_IND  = EXAMPLE_SIGNAL_BASE, ExampleKeypressInd    exampleKeypressInd)
** SIG_DEF( EXAMPLE_POWER_OFF_REQ,                       ExamplePowerOffReq    examplePowerOffReq)
** SIG_DEF( EXAMPLE_POWER_ON_IND,                        ExamplePowerOnInd     examplePowerOnInd)
** SIG_DEF( EXAMPLE_SIM_ERROR_IND,                       ExampleSimErrorInd    exampleSimErrorInd)
** #endif
**
** Signal bases are added using the KI_BASE_DEF() macro in one of the XXsigset.h
** files (or exsigbas.h for terminal clients, exssigbas.h for silicon clients).
** The KI_BASE_DEF() macro adds _SIGNAL_BASE to the end of the parameter
** given as the base name. This is done to enforce the GKI naming convention.
**
** Note: For Referenced Data Logging example see comments in 'kirefsigdef.h'
*/

#undef SIGNAL

/*
** This file deiberately does not have a
** #endif
** as it can be legitimately included twice
*/

/* END OF FILE */
