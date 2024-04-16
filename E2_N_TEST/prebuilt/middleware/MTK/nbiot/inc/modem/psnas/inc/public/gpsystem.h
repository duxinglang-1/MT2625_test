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
 * File Description:
 *
 * General GPRS settings
 **************************************************************************/

#ifndef GPSYSTEM_H
#define GPSYSTEM_H

/**** NESTED INCLUDE FILES *************************************************/

#include <system.h>
#include <uthdcomp.h>
#include <gpllc.h>
#include <snpdu.h>
#include <npdu_typ.h>

/**** CONSTANTS ************************************************************/

#define SNDCP_DEFAULT_VERSION_NUMBER    0
#define SNDCP_VERSION_NUMBER            0

#if defined (UPGRADE_3G)
#if !defined (UPGRADE_GGE)
/* In 3G single mode, only LLC_SAPI_NOT_ASSIGNED is valid */
#define MAX_NUM_SAPIS  1
#else
/* In 3G dual mode, LLC_SAPI_NOT_ASSIGNED + 4 sapis are valid */
#define MAX_NUM_SAPIS  5
#endif /* UPGRADE_GGE */
#else
#define MAX_NUM_SAPIS  4  /* 4 SAPIs used by SNDCP */
#endif /* UPGRADE_3G */

#define MAX_RX_OFFSET        (MAX_LL_HEADER_LEN - MIN_LL_HEADER_LEN \
                              + RFC1144_HEADER_SIZE)

#if defined (UPGRADE_3G)
/* Need to define a minimum offset for NPDU. */
#define MIN_RX_OFFSET  3
#else
#define MIN_RX_OFFSET  0
#endif

#if defined (DATA_IN_SIGNAL)
# if MAX_RX_BUFFER_OFFSET < MAX_RX_OFFSET
#  error Need to increase MAX_RX_BUFFER_OFFSET!
# endif
#endif

/* pragma settings */

#if defined GP_USE_PRAGMAS

#pragma argsused
#pragma warn -aus
#pragma warn -par

#endif


/* compiler manipulation */

#define STATIC static


#endif

/* END OF FILE */
