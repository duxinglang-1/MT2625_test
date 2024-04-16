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
 *************************************************************************/
/** \file
 * EMM - LTE Mobility Management Public Data Types
 *************************************************************************/

#if !defined (EMM_PUBLIC_TYP_H)
#define       EMM_PUBLIC_TYP_H

/***************************************************************************
* Nested Include Files
***************************************************************************/

/***************************************************************************
 *   Manifest Constants
 ***************************************************************************/

/***************************************************************************
*   Macro Functions
***************************************************************************/

/***************************************************************************
*   Types
***************************************************************************/

#define KEY_ASME_MAX_SIZE          32
#define NAS_COUNT_MAX_SIZE          4
#define MNC_MCC_DIGIT_LENGTH        3

/** Key KASME is the output from a successful EPS AKA procedure. */
typedef struct EutraKeyAsmeTag
{
    Int8       length;
    Int8       data[KEY_ASME_MAX_SIZE];
}
EutraKeyAsme;

/** The EPS update status is stored on the USIM or NVRAM if service 85 is not
 * supported. It is changed only after an attach, n/w initiated detach,
 * authentication, TAU, service request or paging for EPS services using IMSI.
 */
typedef enum EpsUpdateStatusTag
{
    /** UPDATED */
    EU1_EPS_UPDATED,
    /** NOT_UPDATED */
    EU2_EPS_NOT_UPDATED,
    /** ROAMING NOT ALLOWED */
    EU3_EPS_ROAMING_NOT_ALLOWED
}
EpsUpdateStatus;

/** This structure contains a KAsme and its associated KSI. */
typedef struct EutraKAsmeDataTag
{
    /** the associated key set identifier for the KASME */
    Int8          asmeKeySetId;
    /**  output from a successful EPS AKA procedure. */
    EutraKeyAsme       keyAsme;
}
EutraKAsmeData;

/** Each new KASME has a set of two associated NAS counts for uplink and downlink NAS messages.
 * NAS Counts are initialised when the KASME is created and after the NAS SMC message for that
 * security context is sent, the relevant NAS COUNT is increased each time a NAS message
 * sent/received.
 */
typedef struct EutraNasCountTag
{
    Int8       length;
    Int8       data[NAS_COUNT_MAX_SIZE];
}
EutraNasCount;

#endif /* EMM_PUBLIC_TYP_H */

/* END OF FILE */

