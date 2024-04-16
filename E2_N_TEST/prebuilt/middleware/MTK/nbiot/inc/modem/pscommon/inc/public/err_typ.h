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
 *  File Description :                                                   */
/** \file
 *     Defines the Err type.
 **************************************************************************/

#ifndef ERR_TYP_H
#define ERR_TYP_H

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

#if !defined (SYSTEM_H)
#include <system.h>
#endif


/****************************************************************************
 * Macros
 ****************************************************************************/


/****************************************************************************
 * Types
 ****************************************************************************/

/*** Base enum Codes ******************************************/
/*
** base codes can be used where several units have their own (mutually
** exclusive) sets of constants, but need to be combined into a larger
** set of enumerated constants.
*/
typedef enum BaseTag
{
     /* system-wide signals/errors */
    SY_BASE                     =   0,
    /* Layer 1 */
    L1_BASE                     =   0x0100,
    /* Layer 2 */
    L2_BASE                     =   0x0200,
    /* Common L3 */
    L3_BASE                     =   0x0300,
    /* Radio Resource */
    RR_BASE                     =   0x0400,
    /* Mobility Management */
    MM_BASE                     =   0x0500,
    /* Call Control */
    CC_BASE                     =   0x0600,
    /* Supplementary Services */
    SS_BASE                     =   0x0700,
    /* Short Message Service */
    SMS_BASE                    =   0x0800,
    /* Application */
    AL_BASE                     =   0x0900,
    /* SIM Interface */
    SI_BASE                     =   0x0a00,
    /* Kernel Interface */
    KI_BASE                     =   0x0b00,
    /* Test */
    T_BASE                      =   0x0c00,
    /* Cell broadcast */
    /* Cell broadcast */
    CB_BASE                     =   0x0d00,
    /* SNDCP */
    SN_BASE                     =   0x0e00
}
Base;

/** Specifies the error */
typedef enum ErrTag
{
    /** Success */
    SUCCESS                     =   SY_BASE,
    /** value for trivial functions with only one failure mode */
    FAILED  = 1,
    /** value is used explicitly in DOS batch files */
    WARNING = 2,
    /** value used to indicate that a TLV type IE has
     * zero length (needed for LSA Identity where a zero
     * length IE means that the IE is not present */
    ZERO_LENGTH_IE,
    /** errors for L3Message.status.errorCode */
    INVALID_MESSAGE_TYPE        =   L3_BASE,
    /** Message too short */
    MESSAGE_TOO_SHORT,
    /** Invalid transaction identifier */
    INVALID_TRANSACTION_IDENTIFIER,
    SKIP_INDICATOR_NOT_ZERO,
    /** Out of sequence IEs */
    IE_SEQUENCE_ERROR,
    /** Repeated IEs */
    DUPLICATED_IE,

    UNRECOGNISED_IE_COMP_REQ,
    /** IEIs unknown in the message */
    UNRECOGNISED_IE,
    /** Syntactically incorrect optional IEs*/
    OPTIONAL_IE_CONTENT_ERROR,
    /** One of the data or constant fields
     * contained an invalid value. */
    MANDATORY_IE_CONTENT_ERROR,
    /** There is not enough raw message left
     * to contain all of the IE.*/
    MANDATORY_IE_MISSING,
    /** Invalid protocol discriminator */
    INVALID_PROTOCOL_DISCRIMINATOR,

#if defined (UPGRADE_NASMDL2)
    /** Invalid procedure transaction identity */
    INVALID_PTI,
    /** Invalid EPS bearer identity */
    INVALID_EBI,
    /** Invalid value in security header type */
    INVALID_SECURITY_HEADER_TYPE,

    /** Semantic error in the TFT operation */
    SEMANTIC_ERROR_IN_TFT_OPERATION,
    /** Syntactical error in the TFT operation */
    SYNTACTICAL_ERROR_IN_TFT_OPERATION,
    /** Semantic error in packet filters */
    SEMANTIC_ERROR_IN_PACKET_FILTERS,
    /** Syntactical error in packet filters */
    SYNTACTICAL_ERROR_IN_PACKET_FILTERS,
#endif /* UPGRADE_NASMDL2 */

    /** Unexpected signal */
    UNEXPECTED_SIGNAL           =   RR_BASE,
    /** Unecognised signal */
    UNRECOGNISED_SIGNAL,
    /** Unknown state in RR */
    UNKNOWN_STATE_IN_RR,
    /** Unexpected L1 error */
    UNEXPECTED_L1_ERROR,
    /** RR-MM synchronisation error */
    RR_MM_SYNCHRONISATION_ERROR,
    /** Message type uncompatible with state */
    MSG_TYPE_NOT_COMPAT_WITH_STATE,
    /** Unexpected L2 error */
    UNEXPECTED_L2_ERROR,
    MA_NOT_KNOWN_FOR_CBCH,
    /** Message not required */
    MESSAGE_NOT_REQUIRED,

    /** Unexpected MN event for this state */
    CC_UNEXPECTED_MN_EVENT_FOR_THIS_STATE = CC_BASE,
    /** Unexpected MM event for this state */
    CC_UNEXPECTED_MM_EVENT_FOR_THIS_STATE,
    /** Unexpected MM event */
    CC_UNEXPECTED_MM_EVENT,
    /** Unexpected timer event for this state */
    CC_UNEXPECTED_TIMER_EVENT_FOR_THIS_STATE,
    /** Unexpected message in this state */
    CC_UNEXPECTED_MESSAGE_IN_THIS_STATE,
    /** Unexpected message in U0 */
    CC_UNEXPECTED_MESSAGE_IN_U0,
    /** Unexpected message in U0_1 */
    CC_UNEXPECTED_MESSAGE_IN_U0_1,
    /** Invalid timer reference */
    CC_INVALID_TIMER_REFERENCE,
    /** Unknown transaction identifier */
    CC_INVALID_TI,
    /** CC unkown message */
    CC_UNKNOWN_MESSAGE,
    /** CC unkown primitive */
    CC_UNKNOWN_PRIMITIVE,
    /** CC programming error */
    CC_PROGRAMMING_ERROR,

    /** Unexpected event for this state */
    SS_UNEXPECTED_MN_EVENT_FOR_THIS_STATE = SS_BASE,
    /** Unexpected event for this state */
    SS_UNEXPECTED_MM_EVENT_FOR_THIS_STATE,

    /* MM error codes */
    /** Unexpected event for this state */
    MM_UNEXPECTED_EVENT_FOR_THIS_STATE = MM_BASE,
    /** MM unexpected message */
    MM_UNEXPECTED_MESSAGE,
    /** Programming error */
    MM_PROGRAMMING_ERROR,

    /** Fatal kernel error */
    FATAL_KERNEL_ERROR              =   KI_BASE,
    /** Out of memory */
    OUT_OF_MEMORY,

    /** The message is not wanted after checking the requirement */
    CB_ERR_MESSAGE_NOT_WANTED       =   CB_BASE,
    /** Bad header format */
    CB_ERR_BAD_HEADER,
    /** CB Bad page number */
    CB_ERR_BAD_PAGE_NUMBER,
    /** CB Message not found */
    CB_ERR_NO_MESSAGE_FOUND,
    /** CB Unknown state */
    CB_ERR_UNKNOWN_STATE,
    /** CB bad schedule message header */
    CB_ERR_BAD_SCHED_HEADER,
    /** SNDCP is not able to select a DCOMP value for the data
     * compression being negotiated (all 16 DCOMP values are in use)
     */
    SN_NO_MORE_DCOMP_VALUES         =   SN_BASE,
    /** SNDCP is not able to select a PCOMP value for the header
     * compression being negotiated (all 16 PCOMP values are in use)
     */
    SN_NO_MORE_PCOMP_VALUES,
    /** SNDCP is not able to select an entity number for the header
     * compression being negotiated (all 32 entities are in use)
     */
    SN_NO_MORE_HDR_ENTITY_NOS,
    /** SNDCP is not able to select an entity number for the data
     * compression being negotiated (all 32 entities are in use)
     */
    SN_NO_MORE_DATA_ENTITY_NOS,
    /** Last error */
    LAST_ERROR
}
Err;

#endif
/* END OF FILE */














