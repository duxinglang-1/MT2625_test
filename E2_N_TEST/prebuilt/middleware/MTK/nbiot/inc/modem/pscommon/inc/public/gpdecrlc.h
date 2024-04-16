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
 *  File Description :
 *
 *      GPRS decode utility declarations for RLC/MAC downlink messages
 *
 **************************************************************************/

#if !defined (GPDECRLC_H)
#define GPDECRLC_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <gprlctyp.h>

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/***************************************************************************
 * Typed Constants
 **************************************************************************/

/***************************************************************************
 * Types
 **************************************************************************/

/***************************************************************************
 *  Signal Definitions
 **************************************************************************/

/***************************************************************************
 *  FUNCTION PROTOTYPES
 *
 *  General notes on parameters:
 *
 *  In:     *rawMsg_p:  pointer to first octet of the control message content
 *                      of an rlc/mac downlink message.  The message may have
 *                      been received in either one, or in some cases two,
 *                      downlink rlc/mac control blocks, as defined in
 *                      04.60 v8.0.0 10.3.1.  Where the message was split over
 *                      2 rlc/mac control blocks, the 'control message
 *                      content' of the two are assumed to have been
 *                      concatenated into a single contiguous block of memory.
 *                      The message type (04.60 11.2.0.1) is therefore located
 *                      in the 6 most significant bits of *rawMsg_p, and the
 *                      remaining message content, as defined in CSN.1 in
 *                      04.60 11.2, follows on contiguously.
 *
 * In:      msgLen:     length of the message to be decoded, in octets.  Where
 *                      the message was split over 2 rlc/mac control blocks
 *                      this is in the range 40-44(??).  Where the message
 *                      occupies a single rlc/mac control block
 *                      this is in the range 20-22.  (The decode utilities
 *                      need this in order to check that the message is
 *                      correctly padded, and also to guard against 'run-off'
 *                      when decoding structures which repeat indefinitely.
 *                      In some cases where two separate functions are used
 *                      to decode the complete message, this parameter is
 *                      omitted for the first function.)
 *
 * In or Out: *bitOffset_p:     offset (in bits) from *rawMsg_p at which
 *                      decoding is to start.  This parameter is only
 *                      relevant where two separate functions are used
 *                      to decode the complete message: it is output by
 *                      the first function to indicate where decoding
 *                      stopped, and must be preserved by the calling task
 *                      and input to the second function.
 *
 * Out or i/o: xxxx_p:  pointer to the appropriate a data structure of the
 *                      appropriate type for the message being decoded.  This
 *                      is normally listed as an output parameter.  In some
 *                      cases it is identified in the function header as an
 *                      i/o parameter: this implies that the calling task
 *                      is expected to supply default values for
 *                      optional fields within this data structure.  These
 *                      will be returned unchanged to the caller in the
 *                      event that the field is absent.  (This mechanism may
 *                      be used in some cases to avoid the need for an
 *                      explicit 'field present' flag.)
 *
 **************************************************************************/

DecodeResult
GpDecPacketAccessReject           (const Int8               *rawMsg_p,
                                   Int8                     msgLen,
                                   PacketAccessReject       *par_p);

DecodeResult
GpDecPacketDownlinkAssignment     (const Int8               *rawMsg_p,
                                   Int16                    *bitOffset_p,
                                   RlcMacMsgHeader          *rmmh_p);

DecodeResult
GpDecPacketDownlinkAssignmentBody (const Int8               *rawMsg_p,
                                   Int8                     msgLen,
                                   Int16                    *bitOffset_p,
                                   PacketDnlinkAssignment   *pda_p);

DecodeResult
GpDecPacketDnlinkDummyCtrlBlock   (const Int8                 *rawMsg_p,
                                   PacketDnlinkDummyCtrlBlock *pddcb_p,
                                   Int16                       lenInBits);

DecodeResult
GpDecPacketPagingRequest          (const Int8               *rawMsg_p,
                                   PacketPagingRequest      *ppr_p,
                                   Int16                     lenInBits);

DecodeResult
GpDecPacketPdchRelease            (const Int8               *rawMsg_p,
                                   PacketPdchRelease        *ppr_p,
                                   Int16                     lenInBits);

DecodeResult
GpDecPacketPollingRequest         (const Int8               *rawMsg_p,
                                   PacketPollingRequest     *ppr_p,
                                   Int16                     lenInBits);

DecodeResult
GpDecPacketPwrCtrlTa              (const Int8               *rawMsg_p,
                                   Int16                    *bitOffset_p,
                                   RlcMacMsgHeader          *rmmh_p);

DecodeResult
GpDecPacketPwrCtrlTaBody          (const Int8               *rawMsg_p,
                                   Int16                    *bitOffset_p,
                                   PacketPwrCtrlTa          *ppct_p,
                                   Int16                     lenInBits);

DecodeResult
GpDecPacketQueueingNotification   (const Int8                 *rawMsg_p,
                                   PacketQueueingNotification *pqn_p,
                                   Int16                       lenInBits);

DecodeResult
GpDecPacketTbfRelease             (const Int8                 *rawMsg_p,
                                   PacketTbfRelease           *ptr_p,
                                   Int16                       lenInBits);

DecodeResult
GpDecPacketUplinkAckNack          (const Int8               *rawMsg_p,
                                   Int16                    *bitOffset_p,
                                   RlcMacMsgHeader          *rmmh_p);

DecodeResult
GpDecPacketUplinkAckNackBody      (const Int8               *rawMsg_p,
                                   Int8                     msgLen,
                                   Int16                    *bitOffset_p,
                                   PacketUplinkAckNack      *puan_p);

DecodeResult
GpDecPacketUplinkAssignment       (const Int8               *rawMsg_p,
                                   Int16                    *bitOffset_p,
                                   RlcMacMsgHeader          *rmmh_p);

DecodeResult
GpDecPacketUplinkAssignmentBody   (const Int8               *rawMsg_p,
                                   Int8                     msgLen,
                                   Int16                    *bitOffset_p,
                                   PacketUplinkAssignment   *pua_p);

DecodeResult
GpDecPacketTimeslotRecfg          (const Int8               *rawMsg_p,
                                   Int16                    *bitOffset_p,
                                   RlcMacMsgHeader          *rmmh_p);

DecodeResult
GpDecPacketTimeslotRecfgBody      (const Int8               *rawMsg_p,
                                   Int8                     msgLen,
                                   Int16                    *bitOffset_p,
                                   PacketTimeslotRecfg      *ptr_p);

#if defined (INCLUDE_GPRS_FIXED_ALLOC_IN_L3)
DecodeResult
GpDecFixedAllocBitmap             (const Int8               *rawMsg_p,
                                   Int16                    msgLenBits,
                                   Int16                    *bitOffset_p,
                                   FixedAlloc               *fixed_p);
#endif

#if defined (UPGRADE_EDGE)
DecodeResult
GpDecEgprsPktUplinkAckNackBody    (const Int8               *rawMsg_p,
                                   Int8                     msgLen,
                                   Int16                    *bitOffset_p,
                                   PacketUplinkAckNack      *puan_p);

DecodeResult
GpDecEgprsPktUplinkAssignmentBody (const Int8               *rawMsg_p,
                                   Int8                     msgLen,
                                   Int16                    *bitOffset_p,
                                   PacketUplinkAssignment   *pua_p);

DecodeResult
GpDecEgprsPktDnlinkAssignmentBody (const Int8               *rawMsg_p,
                                   Int8                     msgLen,
                                   Int16                    *bitOffset_p,
                                   PacketDnlinkAssignment   *pda_p);

DecodeResult
GpDecEgprsPktTsRecfgBody (const   Int8                *rawMsg_p,
                                  Int16               *bitOffset_p,
                                  PacketTimeslotRecfg *ptr_p,
                                  Int16                msgLen);

#endif /* UPGRADE_EDGE */
#endif

/* END OF FILE */
