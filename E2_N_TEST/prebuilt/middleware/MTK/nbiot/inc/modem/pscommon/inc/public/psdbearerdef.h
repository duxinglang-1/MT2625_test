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
 * Definition of PSD Bearer ID Values.
 * NSAPI Values for 2G/3G and EPS Bearer IDs for LTE.
 * \note PSD Bearer Id must always be of type Int8.
 ***************************************************************************/

#ifndef PSDBEARERDEF_H
#define PSDBEARERDEF_H

/**** NESTED INCLUDE FILES *************************************************/

/***************************************************************************
*   Manifest Constants
***************************************************************************/

/** Unassigned (used in LTE 24.007/11.2.3.1.5) */
#define PSD_BEARER_ID_UNASSIGNED    (0)
/** Reserved (used in LTE 24.007/11.2.3.1.5) */
#define PSD_BEARER_ID_RESERVED1     (1)
/** Reserved (used in LTE 24.007/11.2.3.1.5) */
#define PSD_BEARER_ID_RESERVED2     (2)
/** Reserved (used in LTE 24.007/11.2.3.1.5) */
#define PSD_BEARER_ID_RESERVED3     (3)
/** Reserved (used in LTE 24.007/11.2.3.1.5) */
#define PSD_BEARER_ID_RESERVED4     (4)

/** Reserved */
#define PSD_BEARER_ID_EXT       (0)
/** Reserved */
#define PSD_BEARER_ID_PTM_M     (1)
/** Reserved */
#define PSD_BEARER_ID_2         (2)
/** Reserved */
#define PSD_BEARER_ID_3         (3)
/** Reserved */
#define PSD_BEARER_ID_4         (4)
/** NSAPI/EPS Bearer ID 5 */
#define PSD_BEARER_ID_5         (5)
/** NSAPI/EPS Bearer ID 6 */
#define PSD_BEARER_ID_6         (6)
/** NSAPI/EPS Bearer ID 7 */
#define PSD_BEARER_ID_7         (7)
/** NSAPI/EPS Bearer ID 8 */
#define PSD_BEARER_ID_8         (8)
/** NSAPI/EPS Bearer ID 9 */
#define PSD_BEARER_ID_9         (9)
/** NSAPI/EPS Bearer ID 10 */
#define PSD_BEARER_ID_10        (10)
/** NSAPI/EPS Bearer ID 11 */
#define PSD_BEARER_ID_11        (11)
/** NSAPI/EPS Bearer ID 12 */
#define PSD_BEARER_ID_12        (12)
/** NSAPI/EPS Bearer ID 13 */
#define PSD_BEARER_ID_13        (13)
/** NSAPI/EPS Bearer ID 14 */
#define PSD_BEARER_ID_14        (14)
/** NSAPI/EPS Bearer ID 15 */
#define PSD_BEARER_ID_15        (15)
#if defined (UPGRADE_3G)
# if defined (UPGRADE_3G_MBMS)
#  if defined (UPGRADE_3G_TDD128)
/** MBMS LSAPI_1*/
#define MBMS_PSD_BEARER_ID_1    (16)
/** MBMS LSAPI_2*/
#define MBMS_PSD_BEARER_ID_2    (17)
#  endif /* (UPGRADE_3G_TDD128) */
# endif /* (UPGRADE_3G_MBMS) */
#endif /* (UPGRADE_3G) */
/** For MT activations */
#define PSD_BEARER_ID_PENDING_ALLOCATION  (0xFE)

#if defined (UPGRADE_3G) && defined (UPGRADE_3G_MBMS) && defined (UPGRADE_3G_TDD128)
/** Maximum number of PSD bearers supported in this build.  This is a restriction
 * within the build to save memory resources.  It must not exceed
 * the MAX_POSSIBLE_NUM_PSD_BEARERS.
 */
#define MAX_NUM_PSD_BEARERS 13  
/** Maximum possible number of PSD bearers permitted by the 3GPP specifications */
#define MAX_POSSIBLE_NUM_PSD_BEARERS  13 /* could be max of 13 (5-15 inclusive + 16-17 for MBMS) */
#else
/** Maximum number of PSD bearers supported in this build.  This is a restriction
 * within the build to save memory resources.  It must not exceed
 * the MAX_POSSIBLE_NUM_PSD_BEARERS.
 */
#define MAX_NUM_PSD_BEARERS 11
/** Maximum possible number of PSD bearers permitted by the 3GPP specifications */
#define MAX_POSSIBLE_NUM_PSD_BEARERS  11 /* could be max of 11 (5-15 inclusive) */
#endif

#if (MAX_NUM_PSD_BEARERS > MAX_POSSIBLE_NUM_PSD_BEARERS)
  #error "MAX_NUM_PSD_BEARERS value is NOT supported"
#endif

/** For non-MBMS related PSD bearers, the maximum bearer ID value is 15 */
#define MAX_PSD_BEARER_ID           (PSD_BEARER_ID_15)
#define MAX_NUM_PSD_BEARER_IDS      (MAX_PSD_BEARER_ID+1)

/** Minimum useable PSD Beaerer ID - this is used to subtract from
 * a specific PSD bearer ID value for indexing tables.
 */
#define MIN_USEABLE_PSD_BEARER_ID   (PSD_BEARER_ID_5)

/** Maximum number of PSD bearer contexts that can be preserved in NVD RAM over deep sleep
 * For now the number of contexts is fixed to 4
 * TODO: This shall be changed to maximum number of supported bearers when the
 * value is agreed.
 */
#define MAX_NUM_OF_STORED_CONTEXTS 4
#endif

/* END OF FILE */
