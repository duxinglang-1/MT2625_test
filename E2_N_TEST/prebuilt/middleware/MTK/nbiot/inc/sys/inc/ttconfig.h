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
 * System wide configuration parameters.
 * Do not add new items to this file unless they are system wide and
 * non-Application specific. They should be nothing to do with GSM, GPRS,
 * 3G, Bluetooth or any other application.
 **************************************************************************/

#ifndef TTCONFIG_H
#define TTCONFIG_H

/** \def ON_PC
 * Indicate that this is a PC build.
 * This conditional is used to indicate the build is for a PC simulation
 * environment. This can be used to enable code to simulate target hardware
 * or provide other features for the simulation environment. */

/****************************************************************************
 *  WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
 *
 *  Do not add new items to this file unless they are system wide
 *  and non-Application specific. They should be nothing to do with GSM,
 *  GPRS, 3G, Bluetooth or any other application.
 *
 *  WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
 ***************************************************************************/
/****************************************************************************
 * Definitions
 ****************************************************************************/

/*
** Mechanism for conditional compiling code dependant on PC/Target
** environment. Default is on target.
*/

#if !defined (GX2)
/* Detect Borland PC compiler */
#if !defined (ON_PC) && defined (__WIN32__)
#   define ON_PC
#endif
/* Detect Microsoft PC compiler */
#if !defined (ON_PC) && defined (_WIN32)
#   define ON_PC
#endif
/* Detect Linux compilers */
#if !defined (ON_PC) && defined (__linux__)
    #define ON_PC
#endif
#endif

#endif
/* END OF FILE */
