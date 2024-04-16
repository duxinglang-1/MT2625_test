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
 *  Definition of signals exchanged between the LLC layer and
 *  the RLC/MAC layer.
 *
 * The Service Access Point the RLC/MAC layer provides its services
 * is called as "GRR" (GRR-SAP) by ETSI (GSM 04.07, 04.60 and 04.64).
 * The signals conveying GRR primitives are prefixed with RLCMAC
 * rather than GRR to distinguish the RLC/MAC signals from ones
 * designated to GPRS Radio Resource (RR/GRR) entity.
 *
 * ETSI Primitive   | GKI Signal (Generic Kernel Interface)
 * ---------------------------------------------------------
 * GRR-DATA-REQ     | RLCMAC_DATA_REQ
 * GRR-DATA-IND     | RLCMAC_DATA_IND
 * GRR-UNITDATA-REQ | RLCMAC_UNITDATA_REQ
 * GRR-UNITDATA-IND | RLCMAC_UNITDATA_IND
 *       -          | RLCMAC_DATACFG_REQ
 *       -          | RLCMAC_RD_STATUS_IND
 **************************************************************************/

#ifndef RLC_SIG_H
#define RLC_SIG_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

/* TTP includes */
/* GPRS includes */
#include <gpllc.h>

#if !defined (GPSAPI_H)
#include <gpsapi.h>
#endif

#if !defined (GPQOS_H)
#include <gpqos.h>
#endif

/** \defgroup SigGprlcmac GPRLCMAC Signalling Interface
 * \ingroup GSMProtoStack
 * \ingroup Sig
 * Signalling interface between the LLC and RLC/MAC layers.
 * Definition of signals exchanged between the LLC layer and
 * the RLC/MAC layer.
 * This interface is essentially for the transfer of acknowledged and
 * unacknowledged LLC PDUs to and from the network.
 *
 * The context of GPRLCMAC signalling interface is shown in the Diagram below.
 *
 * \image html GSM_Interface.png
 * @{
 */

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/***************************************************************************
 * Typed Constants
 **************************************************************************/

/***************************************************************************
 * Types
 **************************************************************************/

/** Request to send a PDU to the network.
 * This signal is sent by LLC to RLC/MAC to send a LLC frame to the network
 * in RLC/MAC acknowledged mode.
 */
typedef struct RlcmacDataReqTag
{
    /** Holds next segment; not needed or used; it should be removed after some further investigations */
    SignalBuffer   next;

    /**  Indicates the DLCI requesting the transmission */
    LlcSapi        sapi;

    /** Specifies a QoS parameter to RLC/MAC */
    PeakThroughput peakThroughput;

    /** Indicates what priority level RLC/MAC should use;
     * LLC passes this value to RLC/MAC for each PDU sent on an LLC link
     */
    RadioPriorityLevel radioPriority;

    /** Indicates the Packet Flow Identifier to RLC/MAC,
     * where the value GPRS_PFI_NOT_SUPPORTED means that the feature is not used
     */
    PacketFlowId packetFlowId;

    /** Indicates if the frame transmission was triggered
     * as an implicit page response. This field is used by GMM normally for cell
     * updates or for page responses, and the reason is indicated in this parameter.
     */
    TriggerCause triggerCause;

    /**  Indicates that the frame is not contain neither ACK nor
     * SACK acknowledgement
     */
    Boolean notAckSack;

    /** LLC frame to be sent to the network */
    LlPdu   pdu;
}
RlcmacDataReq;

/** Indicates that an PDU has been received from the network.
 * This signal is sent by RLC/MAC to LLC to deliver a LLC frame, sent to the MS
 * by the network. RLC/MAC allocates Traffic Memory Manager block for the frame
 * in this signal with GpMemAlloc and leaves free space in front of the frame
 * data for possible TCP/IP header decompression in SNDCP.
 * RLC/MAC is informed about maximum block offset and
 * length with #RlcmacDataCfgReq.
 */

typedef struct RlcmacDataIndTag
{
    /** Received PDU from the network */
    LlPdu pdu;
}
RlcmacDataInd;

/** Request to send an PDU in unacknowledged mode to the peer entity.
 * This signal is sent by LLC to RLC/MAC to send a LLC frame to the network
 * in RLC/MAC unacknowledged mode.
 */
typedef struct RlcmacDataReqTag
RlcmacUnitDataReq;

/** Indication an LLC UI frame has been received from the peer entity.
 * This signal is sent by RLC/MAC to LLC to deliver an unacknowledged LLC frame,
 * sent to the MS by the network in RLC/MAC unacknowledged mode.
 */
typedef struct RlcmacDataIndTag
RlcmacUnitDataInd;

/** Traffic Memory Manager configuration request.
 * This signal is sent to RLC/MAC to configure Traffic Memory Manager block
 * allocation in RLC/MAC. RLC/MAC allocates length bytes of memory to
 * LLC frames with GpMemAlloc and leaves offset bytes of free space
 * in front of each frame data.
 * The required block size depends on the LLC mode, unacknowledged or acknowledged.
 * RLC-MAC unacknowledged is always LLC unacknowledged data but that does not apply
 * to acknowledged data. LLC informs RLC-MAC of the maximum of the two sizes.
 *
 * There is no response to this signal.
 */
typedef struct RlcmacDataCfgReqTag
{
    /** Indicates to RLC/MAC number of bytes of free space to leave in front of each frame data */
    Int16 offset;

    /** Indicates to RLC/MAC number of bytes of memory to allocate for LLC frames */
    Int16 length;
}
RlcmacDataCfgReq;

/** Request to flush all queued data.
 * This signal is sent by LLC to request RLC/MAC to flush all queued data,
 * when: <UL>
 * <LI> TLLI is unassigned (GPRS Detach) or
 * <LI> Reset XID parameter is received (Inter-SGSN RA Update).
 *</UL>
 */
typedef EmptySignal
RlcmacFlushReq;

/***************************************************************************
 * Functions
 **************************************************************************/

/** @} */ /* End of SigGprlcmac group */

#endif

/* END OF FILE */
