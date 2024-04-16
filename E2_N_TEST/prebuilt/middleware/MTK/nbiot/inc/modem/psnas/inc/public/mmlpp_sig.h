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
 * File Description:                                                      */

/** \file
 * Eutra/Utra PS Signal Data Type definitions for the EMMLPP Interface
 **************************************************************************/

#ifndef MMLPP_SIG_H
#define MMLPP_SIG_H

#ifdef FEA_LPP_SUPPORT
/***************************************************************************
* Nested Include Files
***************************************************************************/

#include <system.h>
#include <l3gp_typ.h>
#include <l3_typ.h>

/* This document specifies the signal interface between Eps Mobility Management
 * (EMM) and LTE Position Protocol (LPP).
 * The interface between EMM and LPP is known as the EMMLPP interface.
 * It describes the signals sent between these two entities in terms of data
 * structures and data values, and provides a summary of the purpose of each
 * signal, when it may be expected to occur, and how it relates to other
 * signals on the same interface.
 * This interface is designed to support LTE.
 *
 */

/***************************************************************************
*   Manifest Constants
***************************************************************************/

/***************************************************************************
*   Macro Functions
***************************************************************************/

/***************************************************************************
*   Types
***************************************************************************/
/** \defgroup SigEmmLppSignals  EMMLPP Primitives
 * Primitives exchanged between EMM and LPP.
 * This section defines primitives exchanged between EMM and LPP
 *
 * @{
 *
 */

typedef enum ProtocolNameTag
{
    EMM_LPP_NO_PROTOCOL = 0,
    EMM_LPP_LPP_PDU =  1,
    EMM_LPP_LCS_PDU = 2,
}
LppProtocol;

/** This signal is sent from EMM to LPP to inform LPP about the cell parameters
 */
typedef struct MmLppCellInfoIndTag
{
    Mcc                    mcc;
    Mnc                    mnc;
    Boolean                isThreeDigitMnc;
    EutraAsn_CellIdentity  cellId;
}
MmLppCellInfoInd;

/** This signal is sent from LPP to EMM to inform EMM that calculating
 *    is ongoing.
 */
typedef struct MmLppActiveStartReqTag
{
    /** Empty signal */
    Int8                    empty;
}
MmLppActiveStartReq;

/** This signal is sent from LPP to EMM to inform EMM that calculating
 *    has stopped.
 */
typedef struct MmLppActiveStopReqTag
{
    /** Empty signal */
    Int8                    empty;
}
MmLppActiveStopReq;

/** This signal is sent from LPP to EMM when LPP/LCP needs
 *    to send data to network
 */
typedef struct MmLppDataReqTag
{
    LppProtocol                 protocol;
    Int16                       length;
    Int8                        *data;
}
MmLppDataReq;

/** This signal is sent from EMM to LPP to inform LPP that EMM has
 *   handled MmLppDataReq message
 */
typedef struct MmLppDataCnfTag
{
    Boolean                    success;
}
MmLppDataCnf;

/** This signal is sent from EMM to LPP when EMM receives LPP/LCS data
 *   from network
 */
typedef struct MmLppDataIndTag
{
    LppProtocol                 protocol;
    Int16                       length;
    Int8                        *data;
}
MmLppDataInd;



/** @} */ /* End of SigEmmLppSignals group */

#endif /* FEA_LPP_SUPPORT */
#endif /* MMLPP_SIG_H */

/* END OF FILE */

