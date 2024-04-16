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
 * PDP definitions - packet data protocol info is used in various
 * different signals
 *
 * Based on gppdp.h
 **************************************************************************/

#ifndef PDP_TYP_H
#define PDP_TYP_H

/**** NESTED INCLUDE FILES *************************************************/

#if !defined (SYSTEM_H)
#  include <system.h>
#endif

/**** CONSTANTS ************************************************************/

/**** TYPEDEFS *************************************************************/

/** The PDP Type indicates what protocol is being used.
 * Ref. Table 10.5.155/GSM 24.008.
 */
typedef enum PdpTypeNumberTag
{
  /** X.121 address */
    PDP_X121 = 0,
  /** PDP-type PPP */
    PDP_TYPE_PPP = 1,
  /** PDP-type IHOSS */
    PDP_TYPE_OSP_IHOSS = 2,
  /** IPv4 address */
    PDP_IP_V4 = 0x21,
  /** IPv6 address */
    PDP_IP_V6 = 0x57,
   /** IPv4v6 address */
    PDP_IP_V4V6 = 0x8D
}
PdpTypeNumber;

/** PDP type organisation
 * Ref 24.008/Table 10.5.155 */
typedef enum PdpTypeOrganisationTag
{
    /** ETSI allocated address */
    PDP_ORG_ETSI_ALLOCATED = 0,
    /** IETF allocated address */
    PDP_ORG_IETF_ALLOCATED = 1,
    /** Empty PDP type */
    PDP_ORG_EMPTY = 0x0f
}
PdpTypeOrganisation;

/** PDP Address Maximum length */
#define MAX_PDP_ADDR          16

/** Packet data protocol address information element
 * The address field is an array of length bytes representing an IP or
 * X.121 or PPP or OSP_IHOSS address. We must reserve room for the
 * maximum size address possible; currently IPv6.  PdpAddress is for the
 * binary form only; if the address is as a human-readable string, then
 * TextualPdpAddress is used.
 */
typedef struct PdpAddressTag
{
  /** Indicates if PDP address is present */
    Boolean addressPresent;
  /** Length of PDP address contents
\assoc ARRAY \ref address */
    Int8    length;
  /** Address information */
    Int8    address [MAX_PDP_ADDR];
}
PdpAddress;

/** The MAX_TEXTUAL_PDP_ADDR value of 64 allows for ipv6:
 *  0000:0000:0000:0000:0000:0000:0000:0000; and ipv6 with embedded
 *  ipv4: 0000:0000:0000:0000:0000:0000:000.000.000.000; and the 3gpp
 *  dotty-decimal form for ipv6:
 *  000.000.000.000.000.000.000.000.000.000.000.000.000.000.000.000.
 *  plus the trailing string terminator '\0'
 */
#define MAX_TEXTUAL_PDP_ADDR 64

/** PDP address in textual format */
typedef struct TextualPdpAddressTag
{
    /** Address present flag */
    Boolean addressPresent;
    /** Length of address */
    Int8    length;
    /** Address data */
    Int8    address [MAX_TEXTUAL_PDP_ADDR];
}
TextualPdpAddress;

/** We need to reserve 4 bytes to store information about ipv4 address */
#define IP_ADDRESS_V4_SIZE 4
/** IPv6 adddress are 16 bytes */
#define IP_ADDRESS_V6_SIZE 16

/** Maximum permitted length of the Access Point Name (APN)
 */
#define MAX_APN_NAME 100

/** Access point name information element.
 * Used to identify the packet data network to which the GPRS user
 * wishes to connect and to notify the access point of the packet
 * data network that wishes to connect to the MS.
 * The Access point name is a label or a fully qualified domain name
 * according to DNS naming conventions.
 *
 * --- extracted from GSM 03.30 v7.10 section 9 ---
 * The APN Operator Identifier is placed after the APN Network Identifier. An
 * APN consisting of both the Network Identifier and Operator Identifier
 * corresponds to a DNS name of a GGSN and has a maximum length of 100 octets.

 * The syntax of the APN shall follow the Name Syntax defined in RFC 2181 [14]
 * and RFC 1035 [15]. The APN consists of one or more labels. Each label is
 * coded as one octet length field followed by that number of octets coded as 8
 * bit ASCII characters. Following RFC 1035 [15] the labels should consist only
 * of the alphabetic characters (A-Z and a-z), digits (0-9) and the dash (-).
 * The case of alphabetic characters is not significant. The APN is not
 * terminated by a length byte of zero.

 * For the purpose of presentation, an APN is usually displayed as a string in
 * which the labels are separated by dots (e.g. "Label1.Label2.Label3").
 * --- end of extract ---
 */
typedef struct AccessPointNameTag
{
  /** Length of access point name contents
\assoc ARRAY \ref name */
    Int8 length;
  /** Is an array of length bytes representing either a 32-bit
   * IP address or an ASCII character string that identifies
   * the GGSN (Gateway GPRS Serving Node) to which the GPRS
   * user wishes to connect
   */
    Int8 name[MAX_APN_NAME];
}
AccessPointName;

/** Textual Access Point Name element.
 * This structure is intended to hold the APN in textual form.
 */
typedef struct TextualAccessPointNameTag
{
  /** The length of APN in textual form
\assoc ARRAY \ref name */
    Int8 length;
  /** APN in textual form */
    Int8 name[MAX_APN_NAME];
}
TextualAccessPointName;


/** \note The values in here are important since this
 * element is used to code the L3 message to the network
 * for GPRS (reference 04.08, 07.60)
 */
typedef enum ConfigurationProtocolTag
{
  /** PPP for use with IP PDP type */
    CONFIG_PROTOCOL_PPP = 0,
    /** Protocol type IHOSS */
    CONFIG_PROTOCOL_OSP_IHOSS = 1
}
ConfigurationProtocol;

/** Maximum permitted length of the Protocol Configuration Options (PCO)                    */
#define MAX_PCO_OPTS 2000  /* Was 250 for PCO. For ePCO the max size is 65534 (65538-4)     */
                           /* as defined in 3GPP TS 24.301 9.9.4.26.                        */
                           /* However, this is a teoretical maximun and too big data to     */
                           /* be send as a signal. It is expected that much less is needed. */
                           /* Selected a value that is 8 times bigger than the max size     */
                           /* for PCO.                                                      */

/** Protocol configuration options information element.
 * The purpose of the protocol configuration options information element is to:<UL>
 * <LI> transfer external network protocol options associated with a PDP context activation, and
 * <LI> transfer additional (protocol) data (e.g. configuration parameters, error codes or
 * messages/events) associated with an external protocol or an application.</UL>
 *
 * The protocol configuration options field conveys a block of length bytes to
 * and from external networks. The format of data is as for a PPP information
 * packet, defined in RFC 1661,without the <I> padding </I> fields, and starting
 * with the 2-byte protocol field (MSB first). When negotiating IPCP or PAP/CHAP
 * options with the external network, data contains a frame received (from the PC PPP)
 * by PPP on the MS. The contents of data have no meaning to SM. MAX_PCO_OPTS is limited by the
 * GSM message field definition, currently 255 bytes, giving a MAX_PCO_OPTS
 * value of 252. 04.08 states that the maximum IE size is 253, including
 * header. In practice this may be reduced when the type of PPP fields being
 * exchanged are known.
 */
typedef struct ProtocolConfigOptionsTag
{
  /** Configuration protocol */
    ConfigurationProtocol   protocol;
  /** Length of protocol configuration options contents
\assoc ARRAY \ref data */
    Int16                   length;
  /** Holds a list of the configuration protocol options */
    Int8                    data [MAX_PCO_OPTS];
}
ProtocolConfigOptions;

#define MAX_USER_DATA 1500  /* Maximum size of an user data block  */

typedef struct UserDataContainerTag
{

/** \assoc ARRAY \ref userData */
    Int16                   length;
    Int8                    userData [MAX_USER_DATA];
}
UserDataContainer;


/** Serving PLMN Rate Control (3GPP TS 24.301 9.9.4.28) */
typedef struct ServingPlmnRateControlTag
{

    Int16                   servingPlmnRateControlValue;
}
ServingPlmnRateControl;

/** TFT operation code (defined in 24.008 10.5.6.12) */
typedef enum TftOperationCodeTag
{
    /** Spare */
    TFT_OPCODE_SPARE = 0,
    /** Create new TFT */
    TFT_OPCODE_CREATE_NEW = 1,
    /** Delete existing TFT */
    TFT_OPCODE_DELETE_EXISTING = 2,
    /** Add packet filters to existing TFT */
    TFT_OPCODE_ADD_PACKET_FILTERS = 3,
    /** Replace packet filters in existing TFT */
    TFT_OPCODE_REPLACE_PACKET_FILTERS = 4,
    /** Delete packet filters from existing TFT  */
    TFT_OPCODE_DELETE_PACKET_FILTERS = 5
#if defined (UPGRADE_LTE)
    /** No TFT operation */
    , TFT_OPCODE_NO_OPERATION = 6
#else /* (UPGRADE_LTE) */
#if defined (UPGRADE_3G_FDD) && defined (UPGRADE_REL7)
    /** No TFT operation */
    , TFT_OPCODE_NO_OPERATION = 6
#endif /* (UPGRADE_3G_FDD) && (UPGRADE_REL7) */
#endif /* (UPGRADE_LTE) */
}
TftOperationCode;

/** Packet filter identifier (PFI) is between 1 and 8 (27.007 10.1.3) */
#define MIN_PFI   1
/** Maximum Packet Filter Identifier value */
#if defined (UPGRADE_LTE)
#define MAX_PFI   16
#else /* UPGRADE_LTE */
#define MAX_PFI   8
#endif /* UPGRADE_LTE */

/** IPV4 remote address type.
 * Packet filter component type identifier (24.008 10.5.6.12)
 */
#define PF_COMP_TYPE_ID_IPV4_REMOTE_ADDR    0x10
/** IPV6 remote address type.
 * Packet filter component type identifier (24.008 10.5.6.12)
 */
#define PF_COMP_TYPE_ID_IPV6_REMOTE_ADDR    0x20
/** Protocol Idendifier/ Next Header type.
 * Packet filter component type identifier (24.008 10.5.6.12)
 */
#define PF_COMP_TYPE_ID_PROTID_NXT_HDR      0x30
/** Single Local Port type.
 * Packet filter component type identifier (24.008 10.5.6.12)
 */
#define PF_COMP_TYPE_ID_SNGL_LOCAL_PORT     0x40
/** Local Port Range type.
 * Packet filter component type identifier (24.008 10.5.6.12)
 */
#define PF_COMP_TYPE_ID_LOCAL_PORT_RNGE     0x41
/** Single Remote Port type.
 * Packet filter component type identifier (24.008 10.5.6.12)
 */
#define PF_COMP_TYPE_ID_SNGL_REMOTE_PORT    0x50
/** Remote Port Range type.
 * Packet filter component type identifier (24.008 10.5.6.12)
 */
#define PF_COMP_TYPE_ID_REMOTE_PORT_RNGE    0x51
/** Security Parameter Index type.
 * Packet filter component type identifier (24.008 10.5.6.12)
 */
#define PF_COMP_TYPE_ID_SEC_PAR_IND         0x60
/** Type of Service/ Traffic Class type.
 * Packet filter component type identifier (24.008 10.5.6.12)
 */
#define PF_COMP_TYPE_ID_TOS_TRFC_CLS        0x70
/** Flow label type.
 * Packet filter component type identifier (24.008 10.5.6.12)
 */
#define PF_COMP_TYPE_ID_FLOW_LBL            0x80

#if defined (UPGRADE_LTE)
/** Authorization Token parameter identifier value in the parameters list */
#define TFT_PARAM_ID_AUTH_TOKEN             0x01

/** Flow Identifier parameter identifier value in the parameters list */
#define TFT_PARAM_ID_FLOW_ID                0x02

/** Packet Filter Identifier parameter identifier value in the parameters list */
#define TFT_PARAM_ID_PF_ID                  0x03
#endif /* UPGRADE_LTE */

#if defined (UPGRADE_NASMDL2)
/** Remote address and subnet mask (27.007 10.1.3 and 24.008 10.5.6.12)
 * IPv4: "a1.a2.a3.a4.m1.m2.m3.m4" where a,m are (0-255)
 * IPv6: "a1.a2...a16.m1.m2...m16" where a,m are (0-255)
 * Should be big enough to hold an IPv6 address in text format.
 */
#define MAX_REMOTE_ADDR_AND_SUBNETMASK (2*MAX_TEXTUAL_PDP_ADDR)
#else
/** Remote address and subnet mask (27.007 10.1.3 and 24.008 10.5.6.12)
 * IPv4: "a1.a2.a3.a4.m1.m2.m3.m4" where a,m are (0-255)
 * IPv6: "a1.a2...a16.m1.m2...m16" where a,m are (0-255)
 * Note: Remote addr supports only IPv4 for now because of length limitation below
 * (to hold a string of xxx.xxx.xxx.xxx).
 * IPv6 not supported because MAX_PDP_ADDR would need to be changed to 64
 * which would require more memory system wide - and we do not support IPv6 yet.
 */
#define MAX_REMOTE_ADDR_AND_SUBNETMASK (2*MAX_PDP_ADDR)
#endif

/**
 * Encoded (numeric value) length in bytes - 8 bytes for IPv4, 32 bytes for IPv6.
 * This is the remote address AND subnet mask part of the packet filter.
 * Used for external (MS-network) representation in the IE.
 */
/** IPV4 remote address length */
#define IPV4_REMOTE_ADDR_LEN    8
/** IPV6 remote address length */
#define IPV6_REMOTE_ADDR_LEN    32

/** Remote address and subnet mask information.
 * Note: This was called 'Source address and subnet mask' Pre Rel-7.
 * Consists of dot-separated numeric (0-255) parameters on the form
 *
 * 'a1.a2.a3.a4.m1.m2.m3.m4', for IPv4 and
 *
 * 'a1.a2.a3.a4.a5.a6.a7.a8.a9.a10.a11.a12.a13.a14.a15.a16.
 *
 * m1.m2.m3.m4.m5.m6.m7.m8.m9.m10.m11.m12.m13.m14.m15.m16', for IPv6.
 */
typedef struct RemoteAddrSubnetMaskTag
{
    /** Indicates if val parameter holds valid information
\assoc PRESENT \ref length
\assoc PRESENT \ref val */
    Boolean     present;
    /** The length of data
\assoc ARRAY \ref val */
    Int8        length;
    /** Holds the Remote address and subnet Mask data */
    Int8        val [MAX_REMOTE_ADDR_AND_SUBNETMASK];
}
RemoteAddrSubnetMask;

/** Protocol number (IPv4) / next header (IPv6) (0-255)
 * (27.007 10.1.3 and 24.008 10.5.6.12)
 */
#define MAX_PROTO_NUM 255
/** Encoded length in bytes.
 * Used for external (MS-network) representation in the IE.
 */
#define PROTO_NUM_LEN 1

/** Protocol identifier/Next header type information */
typedef struct ProtocolNumNextHdrTag
{
    /** Indicates if val parameter contains valid information
\assoc PRESENT \ref val */
    Boolean     present;
    /** Specifies the IPv4 protocol identifier or IPv6 next header */
    Int8        val;
}
ProtocolNumNextHdr;

/** Local and remote port range
 * "f.t" where f, t are (0-65535)
 * (27.007 10.1.3 and 24.008 10.5.6.12)
 */
#define MAX_PORT_RANGE  12

/** Encoded (numeric value) length in bytes (2 bytes each for f,t)
 * Used for external (MS-network) representation in the IE.
 */
#define PORT_RANGE_LEN  4

/** Port range information element */
typedef struct PortRangeTag
{
    /** Indicates if val parameter contains valid information
\assoc PRESENT \ref length
\assoc PRESENT \ref val */
    Boolean     present;
    /** The length of port range
\assoc ARRAY \ref val */
    Int8        length;
    /** Port range data */
    Int8        val [MAX_PORT_RANGE];
}
PortRange;

/** Ipsec security parameter index (hex value 00000000-FFFFFFFF)
 * (27.007 10.1.3 and 24.008 10.5.6.12)
 * IPSEC_LEN is encoded length, used for external (MS-network) representation
 * in the IE.
 */
#define IPSEC_LEN   4

/** IPsec security parameter index information (27.007 10.1.3 and 24.008 10.5.6.12)
*/
typedef struct IpsecSpiTag
{
    /** Indicates if the val parameter holds valid information
\assoc PRESENT \ref val */
    Boolean     present;
    /** Holds IPsec security parameter index data */
    Int32       val;
}
IpsecSpi;

/** Type of service and mask (IPv4) / traffic class and mask (IPv6)
 * "t.m" where t,m (0-255)
 * (27.007 10.1.3 and 24.008 10.5.6.12)
 */
#define MAX_TOS_TRFCCLSS  8

/** Encoded (numeric value) length in bytes (1 byte each for t,m)
 * Used for external (MS-network) representation in the IE.
 */
#define TOS_TRFCCLSS_LEN  2

/** Type of service/Traffic class type information */
typedef struct TosTrfcClassTag
{
    /** Indicates if val parameter contains valid information
\assoc PRESENT \ref length
\assoc PRESENT \ref val */
    Boolean     present;
    /** The length of service/Traffic class contents
\assoc ARRAY \ref val */
    Int8        length;
    /** Contains the service/Traffic class data */
    Int8        val [MAX_TOS_TRFCCLSS];
}
TosTrfcClass;

/** Flow label (IPv6) (hex value 00000-FFFFF)
 * (27.007 10.1.3 and 24.008 10.5.6.12)
 */
#define MAX_FLOW_LABEL  0xFFFFF

/** Encoded length in bytes.
 * Used for external (MS-network) representation in the IE.
 */
#define FLOW_LABEL_LEN  3

/** Flow label information.
 * Flow label is valid for IPv6 only. Value ranges from 00000 to FFFFF.
 */
typedef struct FlowLabelTag
{
    /** Indicates if val parameter holds valid information
\assoc PRESENT \ref val */
    Boolean     present;
    /** Flow label data */
    Int32       val;
}
FlowLabel;

/** Maximum value for evaluation precedence index (27.007 10.1.3)
 */
#define MAX_EVAL_PREC_IND   255

#if defined (UPGRADE_LTE)
/** Packet filter direction (defined in 24.008 10.5.6.12) */
typedef enum PacketFilterDirectionTag
{
    /** Pre Rel-7 TFT filter */
    PF_DIR_PRE_REL7             = 0,
    /** Downlink only filter */
    PF_DIR_DOWNLINK_ONLY        = 1,
    /** Uplink only filter */
    PF_DIR_UPLINK_ONLY          = 2,
    /** Bidirectional filter */
    PF_DIR_BIDIRECTIONAL        = 3
}
PacketFilterDirection;
#endif /* UPGRADE_LTE */

/** Packet filter information element
 * Note: 23.060 15.3.2.0:
 * In the list of attributes 'Remote' refers to the external network entity,
 * and 'Local' to the MS.
 */
typedef struct PacketFilterTag
{
    /** Identifies each packet filter in a TFT */
    Int8                  packetFilterId;
#if defined (UPGRADE_LTE)
    /** Identifies for what traffic direction the filter applies */
    PacketFilterDirection packetFilterDirection;
#endif /* UPGRADE_LTE */
    /** Specifies the precedence for the packet filter among all packet
     * filters in all TFTs associated with this PDP address
     */
    Int8                  evalPrecedenceIndex;
    /** Remote address and subnet mask
     * Note: This was called 'Source address and subnet mask' Pre Rel-7
     */
    RemoteAddrSubnetMask  remoteAddrSubnetMask;
    /** Protocol identifier/Next header type */
    ProtocolNumNextHdr    protocolNumNextHdr;
    /** Local port range
     * Note: This was called 'Destination port range' Pre Rel-7
     */
    PortRange             localPortRange;
    /** Remote port range
     * Note: This was called 'Source port range' Pre Rel-7
     */
    PortRange             remotePortRange;
    /** IPsec security parameter index */
    IpsecSpi              ipsecSpi;
    /** Type of service/Traffic class type */
    TosTrfcClass          tosTrfcClass;
    /** Flow label type */
    FlowLabel             flowLabel;
}
PacketFilter;

/** Maximum value of packet filters in a TFT (27.007 10.1.3)
 */
#define MAX_PFS_IN_TFT  MAX_PFI

/** Maximum length of TFT excluding IEI, length, and opcode (24.008 10.5.6.12)
 */
#define MAX_TFT 254

/** Traffic Flow Template information.
 * Specifies the TFT parameters and operations for a PDP context.
 * In addition, this information element may be used to transfer
 * extra parameters to the network (e.g. the Authorization Token).
 * The TFT may contain packet filters for the downlink direction,
 * the uplink direction or packet filters that apply for both
 * directions. The packet filters determine the traffic mapping
 * to PDP contexts. The downlink packet filters shall be applied
 * by the network and the uplink packet filters shall be applied
 * by the MS. A packet filter that applies for both directions
 * shall be applied by the network as a downlink packet filter
 * and by the MS as an uplink filter.
 */
typedef struct TrafficFlowTemplateTag
{
    /** TFT operation code */
    TftOperationCode  tftOpCode;
#if defined (UPGRADE_NASMDL2)
    /** Number of packet filters.
     * Note: This is currently under NASMDL2 because old code
     * gets the number of packet filters by counting which packet
     * filters dont have PFI=0. This method was specifically used
     * for deleting individual packet filters from a TFT (without
     * deleting the whole TFT), which is not a standard AT+CGTFT
     * parameter combination (see AT Command Specification CGTFT
     * section notes).
     * New code under NASMDL2 does not reply on PFI being 0 (value
     * 0 is a valid PFI in some situations, such as in LTE Bearer
     * Resource Allocation Request message, but uses the numPacketFilters
     * below. The upper layers should also be updated to use this
     * method, i.e. using the numPacketFilters field and not have
     * gaps in packetFilterData below.
\assoc ARRAY \ref packetFilterData */
    Int8              numPacketFilters;
#endif /* UPGRADE_NASMDL2 */
    /** Holds a list of packet filters */
    PacketFilter      packetFilterData[MAX_PFS_IN_TFT];
#if defined (UPGRADE_LTE)
    /** Length of parameters list
\assoc ARRAY \ref parametersList */
    Int8              parametersListLength;
    /** Parameters list */
    Int8              parametersList[MAX_TFT];
#endif /* UPGRADE_LTE */
}
TrafficFlowTemplate;

/** Linked TI (24.008 10.5.6.7).
 * Specifies the active PDP context from which the PDP address for the
 * new PDP context could be derived by the network.
 */
typedef struct LinkedTiTag
{
    /** Indicates whether the MS or network initiated the transaction.
    * If it is set to FALSE the network initiated the transaction;
    * otherwise the MS did
    */
    Boolean     myTv;
    /** Holds a transaction value that is used to
     * identify this action within the CM sub-layer on both the
     * Mobile Station (MS) and Network side
     */
    Int8        tv;
}
LinkedTi;

#endif

/* END OF FILE */
