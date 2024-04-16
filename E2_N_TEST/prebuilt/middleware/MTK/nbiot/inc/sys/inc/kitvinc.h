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
 * GKI header file for GKI signals and types need in the Signal Database.
 * Signal structure definition header files and other types needed by PC
 * test tools.
 **************************************************************************/

#ifndef KITVINC_H
#define KITVINC_H

/** \addtogroup PrdGki
 * @{
 */

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <ki_sig.h>
#include <ki_time.h>
#include <emmi_sig.h>

/***************************************************************************
 * Types used by PC test tools
 **************************************************************************/

#include "assocver.h"

/** Test Tool Data.
 * The TsuiteData is an enumeration used to define values that can be picked
 * up by any PC test tools from the pre-processed header files. Do not change
 * the name of this enumeration or any of its members as PC tools depend on
 * the current names. */
typedef enum TsuiteDataTag
{
    /** Number of signals in a base for Normal and Compact filter matrix.
     * This is only used to get the value of #MAX_NUM_SIGS_IN_BASE which
     * is only used by the #NormalFilterMatrix and #CompactFilterMatrix
     * filter matrices, both of which are not used anymore (they will be
     * removed in a future release). */
    NUM_SIGS_IN_BASE = MAX_NUM_SIGS_IN_BASE,

    /** Numerator for Kernel tick to Millisecond conversion. */
    TD_TICK_TO_MS_NUMERATOR = KI_TICK_TO_MS_NUMERATOR,

    /** Denominator for Kernel tick to Millisecond conversion. */
    TD_TICK_TO_MS_DENOMINATOR = KI_TICK_TO_MS_DENOMINATOR

}
TsuiteData;

/** @} */ /* End of PrdGki group */

#endif
/* END OF FILE */
