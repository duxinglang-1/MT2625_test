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
 * File Description:                                                     */
/** \file
 * definition of NSAPI
 **************************************************************************/

#ifndef GPNSAPI_H
#define GPNSAPI_H

/**** NESTED INCLUDE FILES *************************************************/

/** NSAPIs are allocated dynamically by the SM user. An NSAPI identifies one PDP
 * connection in the GPRS protocol stack. NSAPIs 2, 3 and 4 are reserved for
 * future use. Dynamic allocation therefore starts at NSAPI 5.
 * ref. GSM 04.08/10.5.6.2), 04.65/7.2
 */

typedef enum NsapiTag
{
    /** Reserved */
    GPRS_NSAPI_EXT = 0,
    /** Reserved */
    GPRS_NSAPI_PTM_M = 1,
    /** Reserved */
    GPRS_NSAPI_2 = 2,
    /** Reserved */
    GPRS_NSAPI_3 = 3,
    /** Reserved */
    GPRS_NSAPI_4 = 4,
    /** NSAPI 5 */
    GPRS_NSAPI_5 = 5,
    /** NSAPI 6 */
    GPRS_NSAPI_6 = 6,
    /** NSAPI 7 */
    GPRS_NSAPI_7 = 7,
    /** NSAPI 8 */
    GPRS_NSAPI_8 = 8,
    /** NSAPI 9 */
    GPRS_NSAPI_9 = 9,
    /** NSAPI 10 */
    GPRS_NSAPI_10 = 10,
    /** NSAPI 11 */
    GPRS_NSAPI_11 = 11,
    /** NSAPI 12 */
    GPRS_NSAPI_12 = 12,
    /** NSAPI 13 */
    GPRS_NSAPI_13 = 13,
    /** NSAPI 14 */
    GPRS_NSAPI_14 = 14,
    /** NSAPI 15 */
    GPRS_NSAPI_15 = 15,
#if defined (UPGRADE_3G)
# if defined (UPGRADE_3G_MBMS)
#  if defined (UPGRADE_3G_TDD128)
    /** MBMS LSAPI_1*/
    MBMS_LSAPI_1 = 16,
    /** MBMS LSAPI_2*/    
    MBMS_LSAPI_2 = 17,
#  endif /* (UPGRADE_3G_TDD128) */
# endif /* (UPGRADE_3G_MBMS) */
#endif /* (UPGRADE_3G) */
    /** For MT activations */
    GPRS_NSAPI_PENDING_ALLOCATION = 0xFE,  
    /** Unallocated NSAPI */
    GPRS_NSAPI_UNALLOCATED = 0xFF
}
Nsapi;

#if defined (UPGRADE_3G) && defined (UPGRADE_3G_MBMS) && defined (UPGRADE_3G_TDD128)
# if defined (DATA_IN_SIGNAL)
#define MAX_NUM_NSAPIS 5  /* could be max of 11 (5-15 inclusive) */
# else
#define MAX_NUM_NSAPIS 9  /* could be max of 11 (5-15 inclusive) */
# endif
#else
# if defined (DATA_IN_SIGNAL)
#define MAX_NUM_NSAPIS 3  /* could be max of 11 (5-15 inclusive) */
# else
#define MAX_NUM_NSAPIS 7  /* could be max of 11 (5-15 inclusive) */
# endif
#endif
#endif

/* END OF FILE */
