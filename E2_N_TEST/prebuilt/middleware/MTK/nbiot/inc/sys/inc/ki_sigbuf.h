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
 * GKI header file that defines the #SignalBuffer type.
 **************************************************************************/

#ifndef KI_SIGBUF_H
#define KI_SIGBUF_H

/** \addtogroup PrdGki
 * @{
 */

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>
#include <pssignal.h>

/***************************************************************************
 * Type Definitions
 **************************************************************************/

/*
** Signal Buffers
**
** All signals will be represented by C structures, the first field of which
** will always be a SignalId. The definition for a signal buffer is a union
** of all signals, because this would produce a vast amount of debug info,
** each module can define its own Signal union, consisting only of those
** signals actually used by the module.
** If a module does not require a union signal it should contain the line
** KI_NO_SIGNALS_USED
*/
union Signal;

#define KI_NO_SIGNALS_USED union Signal {Int8 dummy;};
/* This version is for backwards compatibility */
#define NO_SIGNALS_USED    KI_NO_SIGNALS_USED

/** Type by the GKI API to pass signal information.
 * This type is used in the GKI API to pass the user data associated
 * with a signal. The fields within this structure point into a bigger
 * underlying structure used within the GKI. */
typedef struct SignalBufferTag
{
    /** Pointer to the signal id.
     * This is a pointer rather than a direct value as it allows the
     * user to change the signal id and forward on the signal without
     * needing to inform the GKI directly (if it was a value the id in
     * the underlying structure would not get updated). */
    SignalId        *type;
    /** Pointer to the signal body.
     * The user code can either cast this directly to a specific
     * structure pointer or define a local signal union to access the
     * required member by name. */
    union Signal    *sig;
}
SignalBuffer;

/** @} */ /* End of PrdGki group */

#endif

/* END OF FILE */
