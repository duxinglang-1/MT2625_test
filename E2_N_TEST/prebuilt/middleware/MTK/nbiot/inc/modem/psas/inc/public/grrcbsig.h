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
 *      GRR-CB Interface
 **************************************************************************/

#ifndef GRRCBSIG_H
#define GRRCBSIG_H

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

#if !defined (SYSTEM_H)
#include     <system.h>
#endif

#if !defined (L13_TYP_H)
#include     <l13_typ.h>
#endif

#if !defined (L123_TYP_H)
#include     <l123_typ.h>
#endif

#if !defined (L3_TYP_H)
#include     <l3_typ.h>
#endif

#if !defined (SIR_TYP_H)
#include     <sir_typ.h>
#endif

/** \defgroup SigCb  CB Signalling Interface
 * \ingroup Prd3gDmProtoStack
 * \ingroup Sig
 * CB Signalling Interface.
 * This section defines the GRR-CB Interface and L2-CB Interface.
 *
 * The context of CB interface is shown in the Figure below.
 *
 * \image html GSM_Interface.png
 * @{
 */

/****************************************************************************
 * Macros
 ****************************************************************************/

/****************************************************************************
 * Types
 ****************************************************************************/

/** \defgroup SigCbgrr   GRR-CB Interface
 * The GRRCB signal interface allows (G)RR to control the operation of the Cell
 * Broadcast task.
 * @{
 */

/****************************************************************************
 * Signal types
 ****************************************************************************/

/** Request RR to enable/disable or to skip a message.
 * The signal has three purposes:<UL>
 * <LI> Enabling (cbControl = CB_ON) and disabling (cbControl = CB_OFF) receiving of CB messages
 * <LI> Power saving by not receiving the last 3 blocks of a message, which is not required
 * (cbControl = CB_SKIP)
 * Power saving by not receiving a number of complete CB messages which are not required
 * (cbControl=CB_SKIP_N_SLOTS).  In this case, the parameter numSlotsToSkip will indicate how many
 * complete messages will not be received.
 */
typedef struct GrrCbControlReqTag
{
    /** Type of the request, either on, off, or skip */
    CbControl cbControl;

    /** Number of slots to skip.
     * This is only valid if #CB_SKIP_N_SLOTS is specified in \a cbControl.
     */
    Int8 numSlotsToSkip;
}
GrrCbControlReq;

/** Cell change indication.
 * This signal provides the CB with information on the new cell when a cell
 * change occurs
 */
typedef struct GrrCbCellChangeIndTag
{
    /** Whether or not CB is supported in the new cell */
    Boolean cbSupported;

    /** Indicates the ARFCN of the new cell */
    Arfcn   arfcn;

    /** The Location Area Code for the new cell */
    Lai     lai;

    /** Identity of the new cell */
    CellId  cellId;
}
GrrCbCellChangeInd;

/** @} */ /* End of SigCbgrr group */
/** @} */ /* End of SigCb group */
#endif

/* END OF FILE */

