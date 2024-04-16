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
 * 3G PS Signal Data Type definitions for the URLC Interface
 **************************************************************************/

#if !defined(URLC_SIG_H)
#define       URLC_SIG_H

/***************************************************************************
* Nested Include Files
***************************************************************************/

#include <system.h>
#include <uas_asn.h>
#include <ups_typ.h>
#include <urltypes.h>
#include <ucstypes.h>

/** \defgroup SigUrlc URLC Signalling Interface
 * \ingroup Prd3gDmProtoStack
 * \ingroup Sig
 *
 * This section defines the signal interface between the Radio Link
 * Control (URLC) task and the higher layers for Mediatek's 3GPP Protocol Stack.
 * This interface is referred to as the URLC interface. The document describes
 * the signals sent between these entities in terms of data structures and data values,
 * and provides a summary of the purpose of each signal, when it may be expected to occur,
 * and how it relates to other signals on the same interface.
 *
 * <B> The context of the URLC interface is illustrated in the following diagram.</B>
 * \subsection PSInterfaceFigure Figure
 * \image html URLC_Interface.png
 *
 * The URLC interface is responsible for the transfer of user data between the URLC and the
 * higher layer entities (URRC, UPDCP, UBMC and the Circuit Switched Data Interface (CSDI)).
 *
 *
 * The signal definitions are based on the recommendations defined in Reference [3G TS 25.322]
 * with additions to fill any areas not covered by these recommendations.
 *
 *
 * @{
 */

/***************************************************************************
*   Manifest Constants
***************************************************************************/

/** Defines maximum number of CSD SDUs per TTI */
#define URLC_MAX_CSD_SDUS_PER_TTI       4

/** Defines maximum CSD SDU size  */
#define URLC_MAX_CSD_SDU_SIZE_BYTES    \
    ((UCS_MAX_URLC_SDU_OFFSET_BYTES) + \
     (UCS_RTFDP_MAX_NUM_BYTES * URLC_MAX_CSD_SDUS_PER_TTI))

/** URLC_DEBUG_LENGTH_TRUNCATED (top bit) is set to indicate that the PDU length
 * (UrlcDebugMacDataReq and UrlcDebugMacDataInd) has been truncated to exclude
 * the data, so that the length indicates the original PDU length, but only the
 * header is was logged
 */
#define URLC_DEBUG_LENGTH_TRUNCATED     0x8000

/***************************************************************************
*   Macro Functions
***************************************************************************/

/** Refers to Message Unit Identifier which is an identity of the RLC SDU used
 * to indicate which RLC SDU refers to
 */

#define URLC_MUI_MODULUS                (USHRT_MAX + 1)

/** Identifies next RLC SDU */
#define URLC_MUI_NEXT(mui)     ((mui + 1) % URLC_MUI_MODULUS)

/** Identifies previous RLC SDU */
#define URLC_MUI_PREV(mui)          \
    ((mui + URLC_MUI_MODULUS - 1) % \
     URLC_MUI_MODULUS)

/* =============================================================================
 * Macros to set/get fields of UrlcDebugMacDataReq/UrlcDebugMacDataInd
 * =============================================================================
 */

/** The MSB of bearer indicates that the data within the PDU has already been
 * deciphered
 */
#define URLC_DEBUG_BEARER_UNCIPHERED     0x80

/** Bit mask to identify radio bearer in debug signal */
#define URLC_DEBUG_BEARER_ID             0x7F

/** Size of HFN (note MSB of HFN is packed in liModeKsiHfn) */
#define URLC_DEBUG_HFNI_OCTETS           3

/* Masks for liModeKsiHfn
 *          7       6       5       4       3       2       1       0
 *      --------|-------|-------|-------|-------|-------|-------|--------
 *      | LI15  |   Mode (0-2)  |  KSI (0-6,8-14=Key,7,15=None) |  HFN  |
 *      --------|-------|-------|-------|-------|-------|-------|--------
 */

/** Bit mask to define length indicater size in debug signal */
#define URLC_DEBUG_MASK_LI               0x80

/** Bit mask to define radio bearer mode in debug signal */
#define URLC_DEBUG_MASK_MODE             0x60

/** Bit mask to define KSI value in debug signal */
#define URLC_DEBUG_MASK_KSI              0x1E

/** Bit mask to define HFN value in debug signal */
#define URLC_DEBUG_MASK_HFN_25           0x01

/* Macros to get liModeKsiHfn */
/** Gets length indicater size from debug signal See \ref liModeKsiHfn. */
#define URLC_DEBUG_GET_LI(b)         ((b & URLC_DEBUG_MASK_LI) >> 7)

/** Gets radio bearer mode from debug signal See \ref liModeKsiHfn */
#define URLC_DEBUG_GET_MODE(b)       ((b & URLC_DEBUG_MASK_MODE) >> 5)

/** Gets KSI value from debug signal See \ref liModeKsiHfn  */
#define URLC_DEBUG_GET_KSI(b)        ((b & URLC_DEBUG_MASK_KSI) >> 1)

/** Gets HFN value from debug signal See \ref liModeKsiHfn */
#define URLC_DEBUG_GET_HFN_25(b)     (b & URLC_DEBUG_MASK_HFN_25)

/** Macros to set liModeKsiHfn parameters. liModeKsiHfn parameters are:
 *
 *  li : FALSE = 7 bit LIs, TRUE = 15 bit LIs
 *
 *  m  : Mode (0=TM, 1=UM, 2=AM)
 *
 *  k  : KSI  (0-6=CS KSI, 8-15= PS KSI, FF=Unciphered)
 *
 *  h  : HFN (MSB of HFN)
 */

/* Lint code to prevent warnings when 0 or 1 is used with the MACRO
 */
#define URLC_DEBUG_LI_MODE_KSI_HFN(li, m, k, h) \
    (((li & 1) << 7) | ((m & 3) << 5) |         \
     ((k & 0x0f) << 1) | ((h >> 24) & 1))

/** Macros to set bits 24:17 of HFN */
#define URLC_DEBUG_HFN_0(hfn)        ((hfn >> 16) & 0xFF)

/** Macros to set bits 16:9 of HFN */
#define URLC_DEBUG_HFN_1(hfn)        ((hfn >>  8) & 0xFF)

/** Macros to set bits 8:1 of HFN */
#define URLC_DEBUG_HFN_2(hfn)        (hfn        & 0xFF)

/** Maximum range of txCount VT(DAT) */
#define URLC_DEBUG_TX_STATUS_MAX_NUM      0x1F

/** Maximum number of SDUs which can be confirmed or discarded in one signal */
/* Setting this value to 16 causes poor data rate? */
#define URLC_MAX_SDUS_PER_CNF             4

/** Maximum number of SDUs which can be passed to upper layers in one signal */
#define URLC_MAX_SDUS_PER_IND             4

#if defined(UPS_ENABLE_RLC_DL_SIGNAL_FLOW_CNTRL)

/** Maximum number of signals sent by RLC to PDCP in a single TTI */
# define URL_SIG_FLOW_CONTROL_MAX_SIG     4
#endif /* UPS_ENABLE_RLC_DL_SIGNAL_FLOW_CNTRL */

/** Macros to set txStatus (txCount=n, txStatus=c) */
#define URLC_DEBUG_SET_TX_STATUS(n, c)        (((n & 0x1F) << 3) | (c & 0x7))

/** Set txStatus cause on debugging signal sent to RlcDec */
#define URLC_DEBUG_GET_TX_STATUS_CAUSE(s)     (s & 0x07)

/** Set txStatus count on debugging signal sent to RlcDec */
#define URLC_DEBUG_GET_TX_STATUS_COUNT(s)     (s >> 3)

/** Defines the maximum number of octets required to hold an uplink transport
 * block list and uplink pduInfo including security key
 */
#define URLC_DEBUG_MAC_DATA_REQ_MAX_DATA                         \
    (sizeof(UrlcDebugUlPduInfo) * UPS_MAX_UL_NO_OF_TB_PER_TTI) + \
    UUL_MAX_TB_LIST_DATA_OCTETS + (SECURITY_KEY_LENGTH * UPS_KSI_ARRAY_SIZE)

/** Defines the maximum number of octets required to hold a downlink transport
 * block list and downlink pduInfo including security key
 */
#define URLC_DEBUG_MAC_DATA_IND_MAX_DATA                         \
    (sizeof(UrlcDebugDlPduInfo) * UPS_MAX_DL_NO_OF_TB_PER_TTI) + \
    UDL_MAX_TB_LIST_DATA_OCTETS + (SECURITY_KEY_LENGTH * UPS_KSI_ARRAY_SIZE)

/***************************************************************************
*   Types
***************************************************************************/

/** This is used for diagnostics to indicate the reason for an uplink SDU discard*/
typedef enum UrlcUlDiscardReasonTag
{
    URLC_DISCARD_TIMER_EXPIRY = 0,               /**< Timer for this SDU has expired */

    /** RLC has reached maximum attempts to transmit a reset Pdu */
    URLC_DISCARD_MAX_ATTEMPTS = 1,

    /** Discards uplink SDU due to RLC reset */
    URLC_DISCARD_RLC_RESET_OCCURED = 2,

    /** Discard an uplink SDU because of re-establishment of radio bearer */
    URLC_DISCARD_RE_ESTABLISH = 3,

    /** Discard an uplink SDU due to release of radio bearer */
    URLC_DISCARD_RELEASE = 4,

    /** Discard an uplink SDU because buffer is full*/
    URLC_DISCARD_BUFFER_FULL = 5
}
UrlcUlDiscardReason;

/** This is used for diagnostics to indicate the reason for an uplink SDU discard */
typedef enum UrlcSduTxStatusTag
{
    /** The whole SDU has not been transmitted */
    URLC_SDU_UNTRANSMITTED = 0,

    /** At least one AMD PDU containing a segment of the SDU has been transmitted */
    URLC_SDU_PARTIALLY_TRANSMITTED = 1,

    /** All AMD PDUs containing segments of the SDU and the Length Indicator indicating
     * the end of the SDU have been transmitted
     */
    URLC_SDU_FULLY_TRANSMITTED = 2
}
UrlcSduTxStatus;

/** This is used to indicate the status of a RLC SDU */
typedef enum UrlcSduStatusTag
{
    /** Status of SDU is OK */
    URLC_SDU_STATUS_OK = 0,

    /** Indicates that SDU has CRC error */
    URLC_SDU_STATUS_CRC_ERROR = 1,

    /** Indicates error in sequence number */
    URLC_SDU_STATUS_SEQUENCE_ERROR = 2,

    /** Used on synchronous TM bearers to indicate there was a failure to receive any
    * data at all in layer 1 (BTFD failure, TFCI decode failure) due to fading or lack
    * of layer 1 synchronisation
    */
    URLC_SDU_STATUS_NO_FRAME = 3,

    /** PDU has a parse error, discard it and record the error, so that all
     * subsequent PDUs are discarded until able to resynchronise on a LI
     */
    URLC_SDU_STATUS_ILLEGAL_PDU_FORMAT = 4
}
UrlcSduStatus;

/** Indicates why a bearer is unable to transmit. This enumerator is used to indicate to
 * upper layer why a radio bearer cannot transmit data.
 */
typedef enum UrlcXoffCauseTag
{
    /** Indicates the buffer is full */
    URLC_XOFF_BUFFER_FULL = 0,

    /** Indicates radio bearer is susppended */
    URLC_XOFF_SUSPENDED = 1,

    /** Indicates radio bearer is halted */
    URLC_XOFF_HALTED = 2,

    /** Indicates radio bearer is stopped */
    URLC_XOFF_STOPPED = 3
}
UrlcXoffCause;

/** This parameter indicates how the memory of the data of PDU was created,
 *and therefore how it should be handled and freed by the receiving task
 */
typedef enum UrlcDataUsageTag
{
    /** Data points to const or static memory and must not be freed */
    URLC_DATA_USAGE_PRESERVED = 0,

    /** Memory was allocated from the kernel memory manager */
    URLC_DATA_USAGE_KERNEL = 1,

    /** Memory was allocated from the traffic memory manager */
    URLC_DATA_USAGE_TMM = 2,

    /** URLC_DATA_USAGE_CS obsolete */
    URLC_DATA_USAGE_AMR = 4
}
UrlcDataUsage;

/** This parameter describes the contents of the UrlcSdu. This struct contains
 * the octet aligned SDU data
 */
typedef struct UrlcSduTag
{
    /** Indicates the bit offset, from the most significant bit of the first
    * octet of the data block, where the most significant bit of used data starts,
    * in the range 0 to 65535
    */
    Int16 bitOffset;

    /** Indicates the size of the used data, in bits, from bitOffset, which accepts
    * a range 0 to 65535, however currently the maximum known SDU size is 1502 octets
    * of data plus 3 octets of header
    */
    Int16 bitLength;

#if defined(L2_ENABLE_LOOPBACK_SENDS_SIGNAL)
    /** Indicates the size of the UL SDU for loopback. Currently only used in UM */
    Int16 loopbackBitLength;
#endif
    /** Indicates the size of the used data, in bits, from bitOffset, which accepts a range
    * 0 to 65535, however currently the maximum known SDU size is 1502 octets of data plus
    * 3 octets of header; these parameters are only used internally
    */
    UrlcDataUsage dataUsage;

    /** It is a pointer to a TMM data block . The most significant bit of used data starts at
     * bitOffset bits after the most significant bit of the first octet, and continues for
     * bitLength bits. The size of the data may vary with the type of signal, but shall not be
     * larger than 8192 octets (65535 bits). When bitLength is 0, data may be a NULL pointer.
     */
    Int8 * data_p;
}
UrlcSdu;

/** Contains the cell information related to this message. This primitive defines
 * cell information */
typedef struct UrlcCellInfoTag
{
    /** Identifies either the SFN or CFN that this data was received in.
    * Note that in the case of this message being received over several frames,
    the frame number is the first frame in which this message was received.
    */
    UFrameNumber frameNumber;

    /** Indicate which FDD cell this data is from and is used by URRC to
     * determine if BCH data is from the serving or a neighbour cell
     */
    UUARFCN                 uarfcn_DL;

    /** UmtsMode: FDD or TDD128
\assoc UNION \ref cellIdentification */
    UmtsMode                umtsMode;

    /** Identifies cell by primary scambling code for FDD and by cell parameters ID for TDD128 */
    CellIdentificationUnion cellIdentification;
}
UrlcCellInfo;

/** Containing the SDU confirmation data. Carries information of SDU that
 * has benn transmitted successfuly the message (messageUnitIdentifier, throughput in bps) */
typedef struct UrlcAmDataCnfSduTag
{
    /** Message Unit Identifier is an identity of the RLC SDU, which is used
     * to identify this message
     */
    UrlcMessageUnitIdentifier messageUnitIdentifier;

#if defined(ENABLE_STATS_FOR_DATA_CNF)
    /** Represents the throughput in bps of the data sent by URLC to UMAC this
     * allows getting some statistics during runtime
     */
    Int32 bpsTxToMac;

    /** Gives the throughput in bps of the data sent by URLC to UMAC
     * and acknowledged by UTRAN
     */
    Int32 bpsAckedByUtran;
#endif
}
UrlcAmDataCnfSdu;

/** Contains the individual SDUs. This Struct carries individual downlink
 * SDUs and their related information.
 */
typedef struct UrlcAmDataIndSduTag
{
    /** Contains the cell related information relating to this message */
    UrlcCellInfo cellInfo;

    /** Diagnostics info, indicating SN range used to reassemble SDU */
    struct
    {
        /** Identifies the sequence number of the PDU containing the first
         * segment of the SDU
         */
        UrlSequenceNumber start;

        /** Identifies the sequence number of the PDU containing the last
         * segment of the SDU
         */
        UrlSequenceNumber end;
    }                                       snRange;

    /** Describes the contents of the UrlcSdu */
    UrlcSdu rlcSdu;
}
UrlcAmDataIndSdu;

/** Indicates the detailed information of the SDUs which are discarded. This primitive
 *  indicates the reason why SDU is discarded and defines transmission status .
 */
typedef struct UrlcUlSduDiscardTag
{
    /** Message Unit Identifier is an identity of the RLC SDU, which is used
     * to identify this message
     */
    UrlcMessageUnitIdentifier messageUnitIdentifier;

    /** Indicates the reason why the SDU is discarded */
    UrlcUlDiscardReason       discardReason;

    /** Indicates the SDU transmission status */
    UrlcSduTxStatus           txStatus;
}
UrlcUlSduDiscard;

/** \defgroup SigUrlcULData Processing uplink Data Primitives
 * Primitives exchanged between URLC and higher
 * layers for data transmission.
 * This section defines primitives exchanged between URLC and upper
 * layers to transmit data using TM/UM/AM modes.
 * @{
 *
 */

/** This Primitive requests transmission of data on TM radio bearer.
 * This signal is sent to the URLC by the higher layers to send data (SDU)
 * on a specified radio bearer using transparent mode.
 */
typedef struct UrlcTmDataReqTag
{
    /** Identifies the radio bearer */
    BearerIdentity bearerIdentity;

    /** Contains the bit aligned SDU data */
    UrlcSdu        rlcSdu;
}
UrlcTmDataReq;

/** This primitive Requests URLC transmission of AMR data. This signal is used to send
 * AMR packet to TM RLC for transmission.
 */
typedef struct UrlcAmrTmDataReqTag
{
    /** Contains the Class A of AMR packet */
    struct
    {
        /** Defines radio bearer used to transmit class A AMR data where
         * RB_INVALID indicates radio bearer is not used
         */
        BearerIdentity bearerIdentity;

        /** Indicates the length in bit of packet where 0 means Purge buffer */
        Int16 bitLength;

        /** Contains the real data of Class A */
        Int8 data [UCS_AMR_MAX_NUM_A_BYTES];
    } classA;

    /** Contains the Class B of AMR packet */
    struct
    {
        /** Defines radio bearer used to transmit class B AMR data where
         * RB_INVALID means radio bearer not used
         */
        BearerIdentity bearerIdentity;

        /** Indicates the length in bit of packet where 0 means Purge buffer */
        Int16 bitLength;

        /** Contains the real data of Class B */
        Int8 data [UCS_AMR_MAX_NUM_B_BYTES];
    } classB;

    /** Contains the Class C of AMR packet */
    struct
    {
        /** Defines radio bearer used to transmit class C AMR data where
         * RB_INVALID means radio bearer not used
         */
        BearerIdentity bearerIdentity;                      /* RB_INVALID=Unused */

        /** Indicates the length in bit of packet where 0 means Purge buffer */
        Int16 bitLength;

        /** Contains the real data of Class C */
        Int8 data [UCS_AMR_MAX_NUM_C_BYTES];
    } classC;
}
UrlcAmrTmDataReq;

/** @} */ /* End of SigUrlcULData group */

/** This parameter describes the contents of the UrlcCsdSdu. This struct contains
 * the octet aligned SDU data relating to CSD.
 */
typedef struct UrlcCsdSduTag
{
    /** Indicates the bit offset, from the most significant bit of the first octet
     * of the data block, where the most significant bit of used data starts, in the
     * range 0 to 65535
     */
    Int16 bitOffset;

    /** Indicates the size of the used data, in bits, from bitOffset, which accepts a
     * range 0 to 65535, however currently the maximum known SDU size is 1502 octets of
     * data plus 3 octets of header
     */
    Int16 bitLength;

    /** Indicates how memory was allocated for this SDU data.
       * these parameters are only used internally
       */
    UrlcDataUsage dataUsage;

    /** It is a pointer to a TMM data block . The most significant bit of used data starts at
     * bitOffset bits after the most significant bit of the first octet, and continues for
     * bitLength bits. The size of the data may vary with the type of signal, but shall not be
     * larger than 8192 octets (65535 bits). When bitLength is 0, data may be a NULL pointer.
     */
    Int8 * data_p;
}
UrlcCsdSdu;

/** \defgroup SigUrlcULData Processing uplink Data Primitives
 * Primitives exchanged between URLC and higher
 * layers for data transmission.
 * This section defines primitives exchanged between URLC and upper
 * layers to transmit data using TM/UM/AM modes.
 * @{
 *
 */

/** This signal Requests URLC transmission of CSD data. This signal is used to send
 * CSD packet to TM RLC.
 */
typedef struct UrlcCsdTmDataReqTag
{
    /** Identifies radio bearer on which data is requested to be transdmitted */
    BearerIdentity bearerIdentity;

    /** Indicates number of SDUs for transmission
\assoc ARRAY \ref sdu */
    Int8           numberOfSdus;

    /** Contains the SDUs requested for transmission by CSD */
    UrlcCsdSdu     sdu [URLC_MAX_CSD_SDUS_PER_TTI];
}
UrlcCsdTmDataReq;

/** @} */ /* End of SigUrlcULData group */

/** \defgroup SigUrlcDLData Processing Downlink Data Primitives
 * This section defines the primitives exchanged between URLC and
 * higher layer to process dowlink data.
 * This signal is sent to the higher layers from URLC to indicate
 * reception of data on specified radio bearer.
 * @{
 */

/** This primitive indicates reception of data. This signal is sent to the higher layers,
 * except URRC from the URLC to indicate that transparent mode data has been received
 * on the specified radio bearer.
 */
typedef struct UrlcTmDataIndTag
{
    /** Identifies radio bearer on which data is received */
    BearerIdentity bearerIdentity;

    /** Contains cell information related to this message */

    UrlcCellInfo   cellInfo;

    /** Indicates the status of the rlcSdu */
    UrlcSduStatus  sduStatus;

    /** Indicates the type of transport channel which the SDU was received on.
     * This is only relevant to BCCH data which can either be received on BCH or FACH.
     */
    UTransportChannelType trChType;

    /** Contains the bit aligned SDU data which can be empty when the sduStatus
     * indicates an error
     */
    UrlcSdu rlcSdu;
}
UrlcTmDataInd;

/** @} */ /* End of SigUrlcDLData group */

/** \defgroup SigUrlcULData Processing uplink Data Primitives
 * Primitives exchanged between URLC and higher
 * layers for data transmission.
 * This section defines primitives exchanged between URLC and upper
 * layers to transmit data using TM/UM/AM modes.
 * @{
 *
 */

/** This Primitive requests transmission of data on UM radio bearer.
 * This signal is sent to the URLC by the higher layers to send data (SDU)
 * on a specified radio bearer using unacknowledge mode.
 */
typedef struct UrlcUmDataReqTag
{
    /** Identifies radio bearer on which data is requested for transmission */
    BearerIdentity bearerIdentity;

    /** Indicates if an indication should be sent to the higher layers
     * when all of the SDU data has been processed (transmitted or discarded)
     */
    Boolean txStatusRequired;

    /** Carries SDU data to be transmitted, where its length must be
     * octet aligned but its offset may be bit aligned
     */
    UrlcSdu rlcSdu;
}
UrlcUmDataReq;

/** This primitive indicates that all of the SDU data has been transmitted.
 * This signal is sent to the higher layers, from the URLC, to indicate that
 * all of the data of an unacknowledged mode SDU, which required a transmission
 * status, has been transmitted or discarded (note that as UM SDUs do not have an
 * associated message unit identifier, its not possible to use UrlcUlSduDiscardInd).
 */
typedef struct UrlcUmDataTransmittedIndTag
{
    /** Identifies the radio bearer on which SDU has been transmitted */
    BearerIdentity bearerIdentity;
}
UrlcUmDataTransmittedInd;

/** @} */ /* End of SigUrlcULData group */

/** \defgroup SigUrlcDLData Processing Downlink Data Primitives
 * This section defines the primitives exchanged between URLC and
 * higher layer to process dowlink data.
 * This signal is sent to the higher layers from URLC to indicate
 * reception of data on specified radio bearer.
 * @{
 */

/** This signal is sent to the higher layers, except URRC from the URLC to indicate that
 * unacknowledged mode data has been received on the specified radio bearer
 */
typedef struct UrlcUmDataIndTag
{
    /** Identifies radio bearer that carries data */
    BearerIdentity bearerIdentity;

    /** Contains the cell related information relating to this message */
    UrlcCellInfo   cellInfo;

    /** Contains the octet aligned SDU data */
    UrlcSdu        rlcSdu;
}
UrlcUmDataInd;

/** @} */ /* End of SigUrlcDLData group */

/** \defgroup SigUrlcULData Processing uplink Data Primitives
 * Primitives exchanged between URLC and higher
 * layers for data transmission.
 * This section defines primitives exchanged between URLC and upper
 * layers to transmit data using TM/UM/AM modes.
 * @{
 *
 */

/** This Primitive requests transmission of data on AM radio bearer.
 * This signal is sent to the URLC by the higher layers to send data (SDU)
 * on a specified radio bearer using acknowledge mode.
 */
typedef struct UrlcAmDataReqTag
{
    /** Identifies radio bearer on which data is requested for transmission */
    BearerIdentity bearerIdentity;

    /** Message Unit Identifier is an identity of the RLC SDU, which is used
     * to identify this message
     */
    UrlcMessageUnitIdentifier messageUnitIdentifier;

    /** Indicates if the upper layer requires a UrlcAmDataCnf to confirm when the SDU
     * has been transmitted successfully or a UrlcUlSduDiscardInd when transmission of
     * the SDU failed
     */
    Boolean confirmationRequired;

    /** Contains the SDU data, where its length must be octet aligned but its offset
     * may be bit aligned
     */
    UrlcSdu rlcSdu;
}
UrlcAmDataReq;

/** This signal is sent to the higher layers by URLC to confirm that the message
 * identified by messageUnitIdentifier has been transmitted successfully.
 * This primitive is sent as a response to UrlcAmDataReq to confirm when the SDU
 * has been transmitted successfully.
 * This is only sent if the confirmationRequired flag in the corresponding
 * UrlcAmDataReq signal is set to TRUE.
 */
typedef struct UrlcAmDataCnfTag
{
    /** Identifies the radio bearer */
    BearerIdentity bearerIdentity;

    /** Identifies the number of SDUs being confirmed in this message
\assoc ARRAY \ref sdu */
    Int8           numberSdus;

    /** Array containing the SDU confirmation data. The length of this array is
     * configurable via compile directive URLC_MAX_SDUS_PER_CNF.
     */
    UrlcAmDataCnfSdu sdu [URLC_MAX_SDUS_PER_CNF];
}
UrlcAmDataCnf;

/** @} */ /* End of SigUrlcULData group */

/** \defgroup SigUrlcDLData Processing Downlink Data Primitives
 * This section defines the primitives exchanged between URLC and
 * higher layer to process dowlink data.
 * This signal is sent to the higher layers from URLC to indicate
 * reception of data on specified radio bearer.
 * @{
 */

/** This message is used by the AM RLC entity to deliver to upper
 * layers an RLC SDU that has been received.
 * This signal is sent to the higher layers from URLC to indicate
 * reception of data on specified radio bearer. The message can carry
 * up to URLC_MAX_SDUS_PER_IND downlink SDUs.
 */
typedef struct UrlcAmDataIndTag
{
    /** Defines bearer identity on which data is received */
    BearerIdentity bearerIdentity;

    /** Indicates the number of SDUs carried by the message
\assoc ARRAY \ref sdu */
    Int8           numberSdus;

    /** indicates if RSP is required */
    Boolean        rspRequired;

    /** Contains the individual SDUs. It is an array whose length is configurable via
     * the compile directive URLC_MAX_SDUS_PER_IND.
     */
    UrlcAmDataIndSdu sdu [URLC_MAX_SDUS_PER_IND];
}
UrlcAmDataInd;

/** Indicates that a number of downlink AM SDUs have been discarded. This signal notifies
 * higher layers that a number of received AM SDUs have been discarded.
 */
typedef struct UrlcAmDlSduDiscardIndTag
{
    /**  Identifies the radio bearer that has discarded a message */
    BearerIdentity bearerIdentity;

    /** Indicates how many downlink SDUs were discarded, where known (1=Unknown\2-15=Known).
     * In release<B> 99 of 25.322 </B>, it is possible to use the MRW SUFI with one entry to discard
     * all SDUs represented in PDUs, up-to and including the specified sequence number/length
     * indicator, which may be one or more.
     */
    Int8 numDiscardedSdus;
}
UrlcAmDlSduDiscardInd;

/** @} */ /* End of SigUrlcDLData group */

/** \defgroup SigUrlcULData Processing uplink Data Primitives
 * Primitives exchanged between URLC and higher
 * layers for data transmission.
 * This section defines primitives exchanged between URLC and upper
 * layers to transmit data using TM/UM/AM modes.
 * @{
 *
 */

/** This primitive indicates an uplink message has been discarded. This signal is sent to the
 * higher layers by URLC to indicate that the uplink message,identified by messageUnitIdentifier,
 * has not been transmitted successfully and has been discarded.
 */
typedef struct UrlcUlSduDiscardIndTag
{
    /** Identifies the radio bearer that has discarded a message */
    BearerIdentity   bearerIdentity;

    /** Indicates how many SDUs are discarded
\assoc ARRAY \ref sdu */
    Int8             numberSdus;

    /** Indicates the detailed information of the SDUs which are discarded */
    UrlcUlSduDiscard sdu [URLC_MAX_SDUS_PER_IND];
}
UrlcUlSduDiscardInd;

/** This primitive indicates that a radio bearer is unable to transmit.
 * This signal is sent to the higher layers by URLC to indicate when a bearer is
 * unable to transmit data and indicate the cause.
*/
typedef struct UrlcXoffIndTag
{
    /** Identifies the radio bearer that is unable to transmit */
    BearerIdentity bearerIdentity;

    /** Indicate why a bearer is unable to transmit */
    UrlcXoffCause  xoffCause;
}
UrlcXoffInd;

/** This signal is sent to the higher layers by URLC to indicate when a
 * bearer becomes able to transmit data again
 */
typedef struct UrlcXonIndTag
{
    /** Identifies the radio bearer that is able to transmit again */
    BearerIdentity bearerIdentity;
}
UrlcXonInd;

/** @} */ /* End of SigUrlcULData group */

/** \defgroup SigUrlcConfig Processing Pending Configuration Primitives
 * This section defines the primitives sent to URRC by URLC for processing
 * a pending configuration.
 * These primitives indicate to URRC that a pending change in uplink/Dowlink
 * configuration should be actioned .
 *
 * @{
 */

/** This signal is sent to the URRC from URLC to indicate that the pending configuration
 * change has been executed in lower layers for the downlink. This signal is sent, at defined
 * activation time, to process a pending downlink configuration. Once this signal has been
 * received, URRC can send the configuration confirm signal.
 */
typedef struct UrlcUlConfigChangeIndTag
{
    /** Indicates activation of a specified uplink pending configuration */
    Int8             activationTimeIndex;

    /** Indicates the full uplink COUNT-C including the HFN at the activation time */
    UmacTmCountcInfo ulHfnCurrent;
}
UrlcUlConfigChangeInd;

/** This signal is sent to the URRC from URLC to indicate that the pending configuration
 * change has been executed in lower layers for the downlink. This signal is sent, at defined
 * activation time, to process a pending downlink configuration. Once this signal has been
 * received, URRC can send the configuration confirm signal.
 */
typedef struct UrlcDlConfigChangeIndTag
{
    /** Indicates activation of a specified uplink pending configuration */
    Int8             activationTimeIndex;

    /** Indicates the full downlink COUNT-C including the HFN at the activation time */
    UmacTmCountcInfo dlHfnCurrent;
}
UrlcDlConfigChangeInd;

#if defined (ENABLE_TRAFFIC_SHAPING)
/** Contains the data shared between RLC and upper layer for each buffer status update*/
typedef struct UrlcTsBufferStatusDataTag
{
    /** Identifies the radio bearer */
    BearerIdentity bearerIdentity;

    /** The buffer status for this bearer*/
    Int32          bufferStatusInBits;
}
UrlcTsBufferStatusData;

/** This signal is sent from upper layer to request buffer status reporting from RLC.
 *  The request signal also holds buffer status information for the data currently stored
 *  by upper layer (i.e. in the traffic shaping module).
 */
typedef struct UrlcTsBufferStatusReqTag
{
    /** Identifies the number of buffer status reported in this message */
    Int8                    numberBufferStatus;

    /** Array containing the buffer status data. The length of this array is
     * configurable via asn1 compile directive maxRABsetup.
     */
    UrlcTsBufferStatusData  upperLayerBSData[maxRABsetup];
}
UrlcTsBufferStatusReq;

/** This signal is sent by RLC to upper layer to update with its buffer status.
 *  Buffer status is computed for data awaiting for first transmission. Data held for
 *  retransmission are excluded from this reporting.
 */
typedef struct UrlcTsBufferStatusCnfTag
{
    /** Identifies the number of buffer status reported in this message */
    Int8                    numberBufferStatus;

    /** Array containing the buffer status data. The length of this array is
     * configurable via asn1 compile directive maxRABsetup.
     */
    UrlcTsBufferStatusData  rlcFirstTransBSData[maxRABsetup];
}
UrlcTsBufferStatusCnf;
#endif

/** @} */ /* End of SigUrlcConfig group */

/** Defines information related to Debug signal that is sent to RlcDec.
 * Debug signal that is sent to RlcDec is packed to minimise its size.
 * Signal structure is as follows :
 *
 *      Number of transport blocks
 *      Byte length (of data)
 *      Array of "Number of transport blocks" PDU info's
 *      Array of PDU data
 *      If any PDU is ciphered
 *          Array of 14 cipher keys (ksi's 1-14)
 *      Endif
 *
 *  <B> Figure below shows how Uplink PDU info is carried on a debug
 * signal. </B>
 *
 * \image html Uplink_PDU_Info.PNG "Figure: UplinkPduInfo Contents "
 */

/*                                     Bit
*          7       6       5       4       3       2       1       0
*      --------|-------|-------|-------|-------|-------|-------|--------
*      |                                                               |
*      -                   Bit Offset (0 - 27952)                      -
*      |                                                               |
*      --------|-------|-------|-------|-------|-------|-------|--------
*      |                                                               |
*      -                   Bit Length  (0 - 27952)                     -
*      |                                                               |
*      --------|-------|-------|-------|-------|-------|-------|--------
*      |Unciphr|               Bearer (0-35, 7F=Unknown)               |
*      --------|-------|-------|-------|-------|-------|-------|--------
*      | LI15  |   Mode (0-2)  | KSI (0=6,8-14=Key, 7,15=None) |       |
*      --------|-------|-------|-------|-------|-------|-------|       -
*      |                                                               |
*      -                                                               -
*      |                  HFN  (UM=25 bits, AM=20 bits)                |
*      -                                                               -
*      |                                                               |
*      --------|-------|-------|-------|-------|-------|-------|--------
*      |           TX count (1-30, 31=31..128) |       TX Cause        |
*      --------|-------|-------|-------|-------|-------|-------|--------
*/

typedef struct UrlcDebugUlPduInfoTag
{
    /** Indicates the bit offset, from the most significant bit of the first
    * octet of the data block.
    */
    Int16 bitOffset;

    /** Defines length of PDU and MSB of bitLength Indicates that the length has
     * been truncated to exclude data, using URLC_DEBUG_LENGTH_TRUNCATED
     */
    Int16 bitLength;

    /** MSB bearerIdentity Indicates data has already been deciphered */
    Int8  bearerIdentity;

    /** Indicates if length indicater is 15 bits and defines mode of
     * radio bearer, KSI and MSB of HFN
     */
    Int8 liModeKsiHfn;

    /** Defines the remaining value of HFN */
    Int8 hfn [URLC_DEBUG_HFNI_OCTETS];

    /** Defines txCount and txCause */
    Int8 status;
} UrlcDebugUlPduInfo;

/** \defgroup SigUrlcDebug Debugging Primitives
 * This section defines primitives used for debugging purpose.
 * These primitives defines informations that help for diagnostic by filling
 * in a global copy of a signal structure and logs it. Hence giving visibility
 * for purpose of debugging.
 *
 * Debug signals that are sent to RlcDec are packed to minimise their size
 * the parameters length indicaters, radio bearer mode, KSI and MSB of HFN are
 * orgonised as shown in Figure below:
 *\subsection liModeKsiHfn Fig
 *\image html liModeKsiHfn_mask.PNG
 *
 * @{
 */

/** This signal forms the visible portion of a mechanism that provides a coder
 * friendly all-purpose debug logger. This function fills in a global copy
 * of the UrlcMacDataReq structure and logs it giving visibility
 * for purpose of debugging.
 */
typedef struct UrlcDebugMacDataReqTag
{
    /** Defines number of transport blocks */
    Int8 numberOfTransportBlocks;

    /** Contiguous byte array containing array of Number of transport
     * blocks PDU info, Data array and array of 14 cipher keys (ksi 1-14)
     * if any PDU is ciphered and this is Genie length association.
\assoc ARRAY \ref data */
    Int16 byteLength;

    /** Represents an array of PduInfo and data +  */
    Int8  data [URLC_DEBUG_MAC_DATA_REQ_MAX_DATA];
} UrlcDebugMacDataReq;

/** @} */ /* End of SigUrlcDebug group */

/** UrlAmTxCause indicates why UL PDUs were transmitted, but as they must be
 * packed into 3 bits of UrlcDebugUlPduInfo status, as data and status PDUs are
 * normally mutually exclusive (except piggyback status), the 3 least significant
 * bits of data and status causes overlay each other :
 *      Data   causes : 0000 - 0111
 *      Status causes : 1000 - 1111
 */
typedef enum UrlAmTxCauseTag
{
    /* Data PDU transmit causes */
    URL_AM_TXD_CAUSE_UNTRANSMITTED       = 0,       /**< Transmitted for first time */
    URL_AM_TXD_CAUSE_NACK                = 1,       /**< NACK received */
    URL_AM_TXD_CAUSE_TIMER_POLL          = 2,       /**< Poll timer expiry */
    URL_AM_TXD_CAUSE_TIMER_PERIODIC_POLL = 3,       /**< Periocic poll timer expiry */
    URL_AM_TXD_CAUSE_PROHIBITED_POLL     = 4,       /**< Poll trigger was deferred
                                                     * during poll prohibit timer
                                                     */
    URL_AM_TXD_CAUSE_UNUSED_5            = 5,       /**< Cause unused */
    URL_AM_TXD_CAUSE_UNUSED_6            = 6,       /**< Cause unused */
    URL_AM_TXD_CAUSE_UNUSED_7            = 7,       /**< Cause unused */

    /* Status PDU transmit causes */
    URL_AM_TXS_CAUSE_TIMER_PERIODIC_STATUS = 8,     /**< Periodic status timer */
    URL_AM_TXS_CAUSE_MISSING_PDU           = 9,     /**< Missing PDU detected */
    URL_AM_TXS_CAUSE_POLLED                = 10,    /**< DL poll detected */
    URL_AM_TXS_CAUSE_SDU_DISCARD           = 11,    /**< UL SDU discarded */
    URL_AM_TXS_CAUSE_WINDOW                = 12,    /**< UL Window SUFI */
    URL_AM_TXS_CAUSE_RESET                 = 13,    /**< UL RESET/RESET_ACK PDU */
    URL_AM_TXS_CAUSE_PADDING               = 14,    /**< UL Padding control PDU */
    URL_AM_TXS_CAUSE_MAC_HS_REQ            = 15,    /**< DL UmacHsRequestStatusInd received */

    URL_AM_TX_CAUSE_ILLEGAL                = 16     /**< Values MUST NOT exceed
                                                     * range :
                                                     *      Data   : 0000 - 0111
                                                     *      Status : 1000 - 1111
                                                     */
} UrlAmTxCause;

/** Defines information related to Debug signal that is sent to RlcDec.
 * Debug signal that is sent to RlcDec is packed to minimise its size.
 * Signal structure is as follows :
 *
 *      Number of transport blocks
 *      Byte length (of data)
 *      Array of "Number of transport blocks" PDU info's
 *      Array of PDU data
 *      If any PDU is ciphered
 *          Array of 14 cipher keys (ksi's 1-14)
 *      Endif
 *
 *  <B> Figure below shows how Downlink PDU info is carried on a debug
 * signal. </B>
 * \image html Downlink_PDU_Info.png "Figure: DownlinkPduInfo Contents "
 *
 *
 */

/*
*
*                                     Bit
*          7       6       5       4       3       2       1       0
*      --------|-------|-------|-------|-------|-------|-------|--------
*      |                                                               |
*      -                   Bit Offset (0 - 27952)                      -
*      |                                                               |
*      --------|-------|-------|-------|-------|-------|-------|--------
*      |                                                               |
*      -                   Bit Length  (0 - 27952)                     -
*      |                                                               |
*      --------|-------|-------|-------|-------|-------|-------|--------
*      |Unciphr|               Bearer (0-35, 7F=Unknown)               |
*      --------|-------|-------|-------|-------|-------|-------|--------
*      | LI15  |   Mode (0-2)  | KSI (0=6,8-14=Key, 7,15=None) |       |
*      --------|-------|-------|-------|-------|-------|-------|       -
*      |                                                               |
*      -                                                               -
*      |                              HFN                              |
*      -                                                               -
*      |                                                               |
*      --------|-------|-------|-------|-------|-------|-------|--------
*      |                            Status                             |
*      --------|-------|-------|-------|-------|-------|-------|--------
*/
typedef struct UrlcDebugDlPduInfoTag
{
    /** Indicates the bit offset, from the most significant bit of the first
     * octet of the data block
     */
    Int16 bitOffset;

    /** MSB: Indicates that the length has been truncated to exclude data, use
    * URLC_DEBUG_LENGTH_TRUNCATED */
    Int16 bitLength;

    /** MSB of this field indicates data has already been deciphered */
    Int8  bearerIdentity;

    /** Indicates if length indicater is 15 bits and defines mode of
     * radio bearer, KSI and MSB of HFN
     */
    Int8 liModeKsiHfn;

    /** Defines the remaining value of HFN */
    Int8 hfn [URLC_DEBUG_HFNI_OCTETS];

    /** Defines txStatus */
    Int8 status;
} UrlcDebugDlPduInfo;

/** \defgroup SigUrlcDebug Debugging Primitives
 * This section defines primitives used for debugging purpose.
 * These primitives defines informations that help for diagnostic.
 *
 * @{
 */

/** This signal forms the visible portion of a mechanism that provides a coder
 * friendly all-purpose debug logger. This function fills in a global copy
 * of the UrlcMacDataInd structure and logs it giving visibility
 * for purpose of debugging.
 */
typedef struct UrlcDebugMacDataIndTag
{
    /** Number of transport blocks */
    Int8  numberOfTransportBlocks;

    /** Contiguous array of PDUs, array of security keys, array of data and
     * this is Genie length association
\assoc ARRAY \ref data */
    Int16 byteLength;

    /** Represents an array of data within the signal */
    Int8  data [URLC_DEBUG_MAC_DATA_IND_MAX_DATA];
} UrlcDebugMacDataInd;

/** @} */ /* End of SigUrlcDebug group */

/** Logging DlPduStatus when URLC rejects a PDU in
 * UmacDlPduListInfoRsp by setting cipherInfo.dstData_p to NULL
 */
typedef enum UrlcDebugDlPduStatusTag
{
    /** Indicates NO CRC error */
    URLC_DEBUG_DL_OK = UDL_PDU_STATUS_CRC_OK,

    /** Indicates SDU has CRC error */
    URLC_DEBUG_DL_CRC_ERROR = UDL_PDU_STATUS_CRC_ERROR,

    /** Indicates SDU has problem in MAC header */
    URLC_DEBUG_DL_MAC_ERROR = UDL_PDU_STATUS_MAC_ERROR,

    /** Unknown bearer */
    URLC_DEBUG_DL_INVALID_BEARER = 4,

    /** Invalid PDU type */
    URLC_DEBUG_DL_FORMAT_ERROR = 5,

    /** Invalid downlink sequence number */
    URLC_DEBUG_DL_SEQUENCE_ERROR = 6,

    /** Bearer stopped */
    URLC_DEBUG_DL_STOPPED = 7,

    /** Bearer halted */
    URLC_DEBUG_DL_HALTED = 8,

    /** RLC in wrong state */
    URLC_DEBUG_DL_WRONG_STATE = 9,

    /** SN outside RX window */
    URLC_DEBUG_DL_OUTSIDE_WINDOW = 10,

    /** SN already received */
    URLC_DEBUG_DL_DUPLICATE = 11,

    /** Unknown SUFI type */
    URLC_DEBUG_DL_UNKNOWN_SUFI = 12,

    /** NACK'd DL SN not available */
    URLC_DEBUG_DL_NACK_ERROR = 13,

    /** Inconsistent status indication */
    URLC_DEBUG_DL_INCONSISTENT_STATUS = 14,

    /** Timer is not active */
    URLC_DEBUG_DL_TIMER_NOT_RUNNING = 15,

    /** Reception of obsolete SUFI */
    URLC_DEBUG_DL_OBSOLETE_SUFI = 16,

    /** Loop not closed on loopback bearer */
    URLC_DEBUG_DL_LOOP_NOT_CLOSED = 17,

    /** Zero length PDU */
    URLC_DEBUG_DL_PDU_EMPTY = 18,

    /** Buffer full */
    URLC_DEBUG_DL_BUFFER_FULL = 19,

    /** PDU deleted by an RESET/RESET_ACK PDU */
    URLC_DEBUG_DL_RESET_DISCARD = 20,

    /** Maximum allowed number of SDUs is exceeded */
    URLC_DEBUG_DL_MAX_SDU_EXCEEDED = 21,

    /** Store the MRW so it can be processed when
     * PDU mrwSn is received or processed (Never passed to RLCDec)
     */
    URLC_DEBUG_DL_MRW_STORED = 0xFD,

    /** The MRW is a retransmission and the original has been
     * processed (Never passed to RLCDec)
     */
    URLC_DEBUG_DL_MRW_RETX = 0xFE,

    /** The received MRW is the same as the stored MRW so do not
     * process any further (Never passed to RLCDec)
     */
    URLC_DEBUG_DL_MRW_RETX_AND_STORED = 0xFF
} UrlcDebugDlPduStatus;

typedef enum UrlcDebugParamModeTag
{
    URLC_DEBUG_PARAM_NULL                    = 0,
    URLC_DEBUG_PARAM_SENDMRW_SET_UL_CONFIG   = 1,
    URLC_DEBUG_PARAM_SENDMRW_SEND_MRW        = 2,
    URLC_DEBUG_PARAM_SENDMRW_IDLE_TO_PENDING = 3,  /* Temporary for HD53085 */
    URLC_DEBUG_PARAM_SENDMRW_STATE_TRANSMIT  = 4,  /* Temporary for HD53085 */
    URLC_DEBUG_PARAM_SENDMRW_REASSIGNED_MRW  = 5,  /* Temporary for HD53085 */
    URLC_DEBUG_PARAM_SENDMRW_RETRY_STATUS    = 6,  /* Temporary for HD53085 */
    /** MRW timer expiry. */
    URLC_DEBUG_PARAM_SENDMRW_TIMEOUT_MRW     = 7,  /* Temporary for HD53085 */
    /** freeing memory*/
    URLC_DEBUG_PARAM_SENDMRW_DEALLOC_MRW     = 8,  /* Temporary for HD53085 */
    /** SDU exceeds MAX SDU size. */
    URLC_DEBUG_PARAM_DL_MAX_SDU_EXCEEDED     = 9
} UrlcDebugParamMode;

/** This signal forms the visible portion of a mechanism that provides a coder
 * friendly all-purpose debug logger. This function fills in a global copy of the signal
 * structure and logs it giving visibility of the event in a Genie log.
 * \note When the event occurs the coder can call UrlcDebugParam with the defined mode
 * and up to four parameters of information.
 */
typedef struct UrlcDebugParamTag
{
    /** Indicates type of debug */
    UrlcDebugParamMode mode;

    /** Optional parameters that can be output with the signal depending on what
     * information is required for each message reason
     */
    Int32 param1;

    /** Optional parameters that can be output with the signal */
    Int32 param2;

    /** Optional parameters that can be output with the signal */
    Int32 param3;

    /** Optional parameters that can be output with the signal */
    Int32 param4;
}UrlcDebugParam;

/** @} */ /* End of SigUrlc group */

#endif /* URLC_SIG_H */
/* END OF FILE URLC_SIG.H */
