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
 *    Type definitions for UCS (CSDI) which may be required in all UCS modules.
 **************************************************************************/

#if !defined (UCSTYPES_H)
#define       UCSTYPES_H

/***************************************************************************
* Nested Include Files
***************************************************************************/

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/* See 26.101-320 for these numbers */
#define UCS_AMR_NUM_SID_A_BITS            39u
#define UCS_AMR_MAX_NUM_A_BITS            81u
#define UCS_AMR_MAX_NUM_B_BITS            103u
#define UCS_AMR_MAX_NUM_C_BITS            60u
#define UCS_AMR_MAX_NUM_A_BYTES           BITS_TO_INT8S (UCS_AMR_MAX_NUM_A_BITS)
#define UCS_AMR_MAX_NUM_B_BYTES           BITS_TO_INT8S (UCS_AMR_MAX_NUM_B_BITS)
#define UCS_AMR_MAX_NUM_C_BYTES           BITS_TO_INT8S (UCS_AMR_MAX_NUM_C_BITS)

/* UCS_AMR_MAX_NUM_BYTES must be the largest of the three */
#define UCS_AMR_MAX_NUM_BYTES             UCS_AMR_MAX_NUM_B_BYTES

#define UCS_RTFDP_MAX_NUM_BITS            640u
#define UCS_RTFDP_MAX_NUM_BYTES           BITS_TO_INT8S (UCS_RTFDP_MAX_NUM_BITS)
#define UCS_MAX_URLC_SDU_OFFSET_BYTES     4

#define UCS_DATA_MAX_NUM_BYTES            (UCS_RTFDP_MAX_NUM_BYTES > \
                                           UCS_AMR_MAX_NUM_BYTES ? UCS_RTFDP_MAX_NUM_BYTES : UCS_AMR_MAX_NUM_BYTES)

#define UCS_MAX_CSD_SDUS_PER_TTI          4

/***************************************************************************
*   Macro Functions
***************************************************************************/

/***************************************************************************
 * Types
 ***************************************************************************/

typedef enum UcsAmrBitTypesTag
{
    UCS_AMR_A_BITS = 0,
    UCS_AMR_B_BITS = 1,
    UCS_AMR_C_BITS = 2
}
UcsAmrBitTypes;

/**UcsRabIdentity is an Int8
 * (the ex-asn.1 type has ansi 41 clutter, which we do no want)
 */
typedef Int8 UcsRabIdentity;

/** Specifies the type of RAB */
typedef enum UcsRabTypeTag
{
    /** The type of RAB is AMR */
    UCS_RAB_TYPE_AMR,

    /** The type of RAB is CSD */
    UCS_RAB_TYPE_CSD
}
UcsRabType;

/** Indicates the status of the AMR data */
typedef enum UcsAmrDataStatusTag
{
    /** The frame is OK */
    UCS_AMR_DATA_STATUS_OK,

    /** The frame is bad with CRC error */
    UCS_AMR_DATA_STATUS_CRC_ERROR,

    /** there is no frame */
    UCS_AMR_DATA_STATUS_NO_FRAME,

    /** Status unknown */
    UCS_AMR_DATA_STATUS_UNKNOWN
}
UcsAmrDataStatus;

/***************************************************************************
 * Typed Constants
 ***************************************************************************/

#endif /* UCSTYPES_H */
/* End of file */
