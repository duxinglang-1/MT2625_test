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
 * definitions for SN-PDP interface signals. This includes signals from PPP
 * to SNDCP
 **************************************************************************/

#ifndef SN_SIG_H
#define SN_SIG_H

/**** NESTED INCLUDE FILES *************************************************/

#include <gpsystem.h>
#include <gpnsapi.h>
#include <uthdcomp.h>
#include <gpdtcomp.h>
#include <gpllc.h>
#include <pdp_typ.h>
#include <npdu_typ.h>
#if defined (UPGRADE_3G)
# if defined (UPGRADE_3G_MBMS)
#  if defined (UPGRADE_3G_TDD128)
#include <mbms_typ.h>
#  endif /*UPGRADE_3G_TDD128*/
# endif /* UPGRADE_3G_MBMS */
#endif /* UPGRADE_3G */

/** \defgroup SigGpsn  GPSN Signalling Interface
 * \ingroup Prd3gDmNas
 * \ingroup Sig
 * Signaling interface between the PDP task and SNDCP/RABM task.
 * This section defines signals exchanged between the PDP task and SNDCP/RABM task.
 * This interface is known as GPSN.
 * The context of the GPSN interface is illustrated in this
 * \ref GmmrabmContextFigure
 * @{
 */

/**** CONSTANTS ************************************************************/


/**** TYPEDEFS *************************************************************/

/** Define all PDPs packet types here - may be duplicate values.
 * Packet types are used to communicate the header packet types for use
 * in header compressors - e.g. RFC1144 has 3 distinct packet types;
 * SNDCP needs to know from a PDP which compression
 * algorithm is being used, and the packet type (if any) of the packet.
 */
typedef enum PdpPacketTypeHdrTag
{
    /** Generic <I> no header compression </I> value */
    GPRS_PCOMP_UNCOMPRESSED = 0,

    /** RFC1144 packet type non compressed TCP/IP */
    GPRS_PCOMP_ALG_RFC1144_UNCOMP_TCPIP,
    /** RFC1144 packet type compressed TCP/IP */
    GPRS_PCOMP_ALG_RFC1144_COMP_TCPIP,
    /** PDP header compression RFC 2507 */
    GPRS_PCOMP_ALG_RFC2507_COMP,
    /** PDP header compression RFC 3095 (applicable for PDCP only) */
    GPRS_PCOMP_ALG_RFC3095_COMP
}
PdpPacketTypeHdr;

/** Define all PDPs packet types here - may be duplicate values.
 * Packet types are used to communicate the data packet types for use
 * in data compressors - e.g. V.42bis has just one packet type;
 * SNDCP needs to know from a PDP which compression
 * algorithm is being used, and the packet type (if any) of the packet.
 */
typedef enum PdpPacketTypeDataTag
{
    /** Generic <I> no data compression </I> value */
    GPRS_DCOMP_UNCOMPRESSED = 0,

    /** V.42bis packet types */
    GPRS_DCOMP_ALG_V42BIS
}
PdpPacketTypeData;

/** Header compression algorithm and its parameters.
 * \note Because PcompProfile contains the header compression types, there is no
 * reliance on new algorithm types being consecutively allocated - i.e. any
 * algorithm can be put into any array slot
 */
typedef struct PcompProfileTag
{
    /** Indicates the header compression used 
\assoc UNION \ref hcp */
    HeaderCompressionType       hct;
    /** Contains the parameters (if any) for this compression algorithm.
     * These Parameters may be the requested parameters from the proposal
     * phase of negotiation, or may be the final negotiated parameters
     * from the response phase of negotiation.
     */
    HeaderCompressionParams     hcp;
}
PcompProfile;

/** Data compression algorithm and its parameters.
 * \note Because DcompProfile contains the data compression types, there is no
 * reliance on new algorithm types being consecutively allocated - i.e. any
 * algorithm can be put into any array slot
 */
typedef struct DcompProfileTag
{
    /** Indicates the data compression used 
\assoc UNION \ref dcp */
    DataCompressionType         dct;
    /** Contains the parameters (if any) for this compression algorithm.
     * Parameters may be the requested parameters from the proposal phase
     * of negotiation, or may be the final negotiated parameters from the
     * response phase of negotiation
     */
    DataCompressionParams       dcp;
}
DcompProfile;

/** \defgroup SigGpsnSignal  GPSN primitives
 * Primitives exchanged between the PDP task and SNDCP/RABM task.
 * This section defines primitives exchanged between the PDP task and SNDCP/RABM task.
 * @{
 */

/* data transfer signals */
/** This signal is sent by the GPRS User to SNDCP/RABM to send a network-layer
 * PDU (protocol data unit) to the network using LLC in acknowledged mode (2G).
 * In the case of PPP, the network-layer PDU originates from an application
 * on the PC and is sent to SNDCP/RABM via the PPP stacks in the PC and MS. There
 * is no response to this signal. The NSAPI must have an active PDP context
 * (established by SM). The compression parameters for this NSAPI may have
 * already been set using SnXidReq/SnXidCnf. The pdpPacketTypeHdr field
 * specifies the compressed IP packet type when TCP/IP header compression is
 * being used by the PDP. The pdpPacketTypeData field specifies the data
 * compression type and/or packet type when data compression is being used
 * by the PDP.
 */
typedef struct SnDataReqTag
{
    /** NSAPI */
    Nsapi               nsapi;
    /** The header compression packet type */
    PdpPacketTypeHdr    pdpPacketTypeHdr;
    /** The data compression IP packet type */
    PdpPacketTypeData   pdpPacketTypeData;
    /** Network layer PDU to be sent to the network */
    Npdu                npdu;
    #if defined (DEVELOPMENT_VERSION)
    Int16                ip_Id;
    Int32                src_ip;
    Int32                dst_ip;      
    #endif
}
SnDataReq,
/** This signal is sent by the GPRS User to SNDCP/RABM to send a network-layer
 * PDU (protocol data unit) to the network using LLC in unacknowledged mode (2G).
 * In the case of PPP, the network-layer PDU originates from an application
 * on the PC and is sent to the MS via the PPP stack in the PC. There is no
 * response to this signal. The NSAPI must have an active PDP context
 * (established by SM). The compression parameters for this NSAPI may have
 * already been set using SnXidReq/SnXidCnf.
 */
SnUnitDataReq;

/** Indicates received data to PDP entity.
 * This signal is sent by SNDCP/RABM to the GPRS User to deliver a network-layer
 * PDU (protocol data unit) to the MS from the network. The data received
 * has been sent using confirmed transmission at the LLC layer.
 */
typedef struct SnDataIndTag
{
    /** NSAPI */
    Nsapi               nsapi;
    /** Specifies the compressed IP packet type when TCP/IP header
     * compression is being used by the PDP
     */
    PdpPacketTypeHdr    pdpPacketTypeHdr;
    /** Specifies the data compression type and/or packet type when
     * data compression is being used by the PDP
     */
    PdpPacketTypeData   pdpPacketTypeData;
    /** Received NPDU */
    Npdu                npdu;
    /** Indicates if the GPRS User should send an SnDataRsp signal
     * to SNDCP/RABM layer and is used for downlink flow controlling to
     * prevent the PPP signal queue overflow
     */
    Boolean             rspRequired;
}
SnDataInd,
/** Indicates received data to the GPRS User entity.
 * This signal is sent by SNDCP/RABM to the GPRS User to deliver a network-layer
 * PDU (protocol data unit) to the MS from the network. The data received
 * has been sent using unconfirmed transmission at the LLC layer (2G).
 */
SnUnitDataInd;

#ifdef MDL_DL_PIPELINE
/** Indicates received data to PDP entity.
 * This signal is sent by SNDCP/RABM to the GPRS User to deliver a network-layer
 * PDU (protocol data unit) to the MS from the network. The data received
 * has been sent using confirmed transmission at the LLC layer.
 */
typedef struct SnDataPipelineIndTag
{
    /** NSAPI */
    Nsapi               nsapi;
    /** Specifies the compressed IP packet type when TCP/IP header
     * compression is being used by the PDP
     */
    PdpPacketTypeHdr    pdpPacketTypeHdr;
    /** Specifies the data compression type and/or packet type when
     * data compression is being used by the PDP
     */
    PdpPacketTypeData   pdpPacketTypeData;
    /** Number of elements in the npdu list*/
    Int8                numNpdu;
    /** NPDUs list*/
    Npdu                npduList[ MAX_NPDU_PER_SIGNAL];
    /** True for UM and TM bearer*/
    Boolean             unconfirmed;
}
SnDataPipelineInd;
#endif

/* SnDataRsp : empty signal */

/* negotiation signals */

/** This signal is sent by the GPRS User to SNDCP/RABM to negotiate compression
 * parameters for a PDP context when external compression (for instance as
 * part of PC PPP) is available.
 * This signal is sent prior to sending data on the SN-SAP.  The response
 * to this signal is an SnXidCnf or an SnXidInd. The SnXidInd signal may
 * occur in the case of a re-negotiation initiated by the GPRS User which
 * crosses (coincides) with a network-initiated change of XID parameters.
 * In this case an SnXidRsp must be sent back to SNDCP/RABM.
 *
 * This signal is sent even if the PDP context does not support any compression
 * (all pcompPdp and dcompPdp flags set to FALSE). If no external compression
 * is available in the PDP, SNDCP may still negotiate for data and header
 * compression with its peer in the network(Note: No RABM here). An SnXidCnf is returned to the
 * GPRS User to indicate that no compression is to be used by the PDP
 */
typedef struct SnXidReqTag
{
    /** Identifies the PDP context */
    Nsapi               nsapi;
    /** Identifies the GPRS User task so that subsequent signals may be
     * sent to the correct task
     */
    TaskId              sourceTask;

    /** Indicates how many elements in the pcompPdp and
     * pcompRequested arrays are relevant
     
\assoc ARRAY \ref pcompPdp
\assoc ARRAY \ref pcompRequested */
    Int8                numPcompProfiles;
    /** Indicates how many elements in the dcompPdp and
     * dcompRequested arrays are relevant
     
\assoc ARRAY \ref dcompPdp
\assoc ARRAY \ref dcompRequested */
    Int8                numDcompProfiles;

    /** Whether header compression algorithm is on or off for this PDP context */
    Boolean             pcompPdp[MAX_NUM_HDR_COMP_TYPES];
    /** Requested header compresion algorithm */
    PcompProfile        pcompRequested[MAX_NUM_HDR_COMP_TYPES];

    /** Whether data compression algorithm is on or off for this PDP context */
    Boolean             dcompPdp[MAX_NUM_DATA_COMP_TYPES];
    /** Requested data compresion algorithm */
    DcompProfile        dcompRequested[MAX_NUM_DATA_COMP_TYPES];

    /** Set to TRUE if header compression can be enabled for this PDP context.
     * SNDCP will not ask the network for header compression if this
     * Boolean is set to FALSE. Its state can be changed using the GPRS
     * AT command +CGDCONT
     * \note No RABM here
     */
    Boolean             enableGlobalDcomp;
     /** Set to TRUE if data compression can be enabled for this PDP context.
     * SNDCP will not ask the network for data compression if this
     * Boolean is set to FALSE. Its state can be changed using the GPRS
     * AT command +CGDCONT
     */
    Boolean             enableGlobalPcomp;
}
SnXidReq;

/** This signal is sent by SNDCP/RABM to the GPRS User in response to an SnXidReq.
 * It contains negotiated values for compression in the specified PDP context.
 * This signal concludes XID negotiation. Note that negotiation is completed
 * in one step, i.e. a Req followed by a Cnf or an Ind followed by a Rsp,
 * depending on which side of the SNDCP connection initiates XID exchange.
 * If an option is negotiated to be used, it is implicit that it is used for
 * both directions of the link. If the negotiatedCompressionProfile matches
 * the requestedCompressionProfile given in the corresponding SnXidReq, this
 * indicates that SNDCP will leave any compression up to the external implementation.
 * PPP determines from this signal whether it must use header compression on the MS,
 * and what negotiated parameters to return to the PC PPP (during IPCP negotiation).
 * If the context activation cannot operate with a reduced set of options, it is
 * deactivated; otherwise the reduced options are accepted
 */
typedef struct SnXidCnfTag
{
    /** Identifies PDP context */
    Nsapi               nsapi;

    /** Indicates how many elements in the pcompPdp and pcompNegotiated arrays are relevant 
\assoc ARRAY \ref pcompPdp
\assoc ARRAY \ref pcompNegotiated */
    Int8                numPcompProfiles;
    /** Indicates how many elements in the dcompPdp and dcompNegotiated arrays are relevant 
\assoc ARRAY \ref dcompPdp
\assoc ARRAY \ref dcompNegotiated */
    Int8                numDcompProfiles;

    /** Whether agree or decline to accept PDP use of a header compression algorithm */
    Boolean             pcompPdp[MAX_NUM_HDR_COMP_TYPES];
    /** Negotiated header compression algorithm */
    PcompProfile        pcompNegotiated[MAX_NUM_HDR_COMP_TYPES];

    /** Whether agree or decline to accept PDP use of a data compression algorithm */
    Boolean             dcompPdp[MAX_NUM_DATA_COMP_TYPES];
    /** Negotiated data compression algorithm */
    DcompProfile        dcompNegotiated[MAX_NUM_DATA_COMP_TYPES];
}
SnXidCnf;

/** This signal is sent by SNDCP/RABM to the GPRS User. It contains network-suggested values
 * for compression in the specified PDP context. The response to this signal is an SnXidRsp
 * from the GPRS User, which concludes negotiation. PPP knows from this signal and from MS
 * and PC header compression capabilities whether to use header compression towards the
 * PC PPP
 */
typedef struct SnXidIndTag
{
    /** Identifies the PDP context */
    Nsapi               nsapi;

    /** Indicates how many elements in the pcompPdp and pcompRequested arrays are relevant 
\assoc ARRAY \ref pcompPdp
\assoc ARRAY \ref pcompRequested */
    Int8                numPcompProfiles;
    /** Indicates how many elements in the dcompPdp and dcompRequested arrays are relevant 
\assoc ARRAY \ref dcompPdp
\assoc ARRAY \ref dcompRequested */
    Int8                numDcompProfiles;

    /** Whether the header compression algorithm is on or off for this PDP context */
    Boolean             pcompPdp[MAX_NUM_HDR_COMP_TYPES];
    /** Network-suggested values for header compression */
    PcompProfile        pcompRequested[MAX_NUM_HDR_COMP_TYPES];

    /** Whether the data compression algorithm is on or off for this PDP context */
    Boolean             dcompPdp[MAX_NUM_DATA_COMP_TYPES];
    /** Network-suggested values for data compression */
    DcompProfile        dcompRequested[MAX_NUM_DATA_COMP_TYPES];
}
SnXidInd;

/** This signal is sent by the GPRS User to SNDCP/RABM in response to an SnXidInd.
 * This signal contains MS-suggested values for external compression in the
 * specified PDP context. This signal concludes XID negotiation
 */
typedef struct SnXidRspTag
{
    /** Identifies PDP context */
    Nsapi               nsapi;

    /** Indicates how many elements in the pcompPdp and pcompNegotiated arrays are relevant 
\assoc ARRAY \ref pcompPdp
\assoc ARRAY \ref pcompNegotiated */
    Int8                numPcompProfiles;
    /** Indicates how many elements in the dcompPdp and dcompNegotiated arrays are relevant 
\assoc ARRAY \ref dcompPdp
\assoc ARRAY \ref dcompNegotiated */
    Int8                numDcompProfiles;

    /** Whether agree or decline to accept SNDCP use of a header compression algorithm */
    Boolean             pcompPdp[MAX_NUM_HDR_COMP_TYPES];
    /** Negotiated header compression algorithm */
    PcompProfile        pcompNegotiated[MAX_NUM_HDR_COMP_TYPES];

    /** Whether agree or decline to accept SNDCP use of a data compression algorithm */
    Boolean             dcompPdp[MAX_NUM_DATA_COMP_TYPES];
    /** Negotiated data compression algorithm */
    DcompProfile        dcompNegotiated[MAX_NUM_DATA_COMP_TYPES];
}
SnXidRsp;


/** Configuration signal.
 * This signal enables the user to force SNDCP to negotiate for no header compression
 * and/or no data compression with its peer, regardless of the normal provision.
 * This option is intended to improve fault-finding in initial field trials when the
 * MS is connected to unknown or untested peers. If SNDCP does not receive this signal,
 * all compression available to SNDCP is set to default values
 */

typedef struct SnConfigReqTag
{
    /** Enable the user to force SNDCP to negotiate for no header compression
     * with its peer, regardless of the normal provision
     */
    Boolean             disableRfc1144HeaderCompression;
    /** Enable the user to force SNDCP to negotiate for no data compression
     * with its peer, regardless of the normal provision
     */
    Boolean             disableV42BisDataCompression;
}
SnConfigReq;

#if defined (ENABLE_TRAFFIC_SHAPING)
/** Traffic Shaping Configuration signal.
 * This signal is used to configure the traffic shaping feature.
 */
typedef struct SnTrafficShapingConfigReqTag
{
    /** Whether traffic shaping is enabled
     */
    Boolean             trafficShapingEnabled;
}
SnTrafficShapingConfigReq;
#endif

#if defined (UPGRADE_3G)
# if defined (UPGRADE_3G_MBMS)
#  if defined (UPGRADE_3G_TDD128)

/** SnMbmsActivateIndTag
 * The signal is sent to RABM by MBMS module and it notifies RABM
 * that this mbmsLsapi is allocated for certain MBMS service ID by MBMS.
 */
typedef struct SnMbmsActivateIndTag
{
  /** a local mbms service access point identity and it is associated to RAB. */
  Nsapi                       mbmsLsapi;
}
SnMbmsActivateInd;

/** SnMbmsActivateRspTag
 * RABM sends the signal to inform MBMS that the corresponding mbmsLsapi RAB has been established.
 */
typedef struct SnMbmsActivateRspTag
{
  /** a local mbms service access point identity and it is associated to RAB. */
  Nsapi                       mbmsLsapi;
}
SnMbmsActivateRsp;

/** SnMbmsDeactivateIndTag
 * MBMS sends the signal SnMbmsDeactivateInd to indicate RABM that this mbmsLsapi need to be released
 * and MBMS will erase the corresponding allocation for the mbmsLsapi.
 */
typedef struct SnMbmsDeactivateIndTag
{
  /** a local mbms service access point identity and it is associated to RAB. */
  Nsapi                       mbmsLsapi;
  /** force to deactivate RABM when it is failure to establish RABM */
  Boolean                     forced;
}
SnMbmsDeactivateInd;

/** SnMbmsDeactivateRspTag
 * This is a response to a SnMbmsDeactivateRsp signal, and indicates to MBMS that
 * this mbmsLsapi is no longer used by RABM. This LSAPI has been released.
 */
typedef struct SnMbmsDeactivateRspTag
{
  /** a local mbms service access point identity and it is associated to RAB. */
  Nsapi                       mbmsLsapi;
}
SnMbmsDeactivateRsp;

#  endif /*UPGRADE_3G_TDD128*/
# endif /* UPGRADE_3G_MBMS */
#endif /* UPGRADE_3G */

/** @} */ /* End of SigGpsnSignal group */
/** @} */ /* End of SigGpsn group */
#endif /* !SN_SIG_H */

/* END OF FILE */
