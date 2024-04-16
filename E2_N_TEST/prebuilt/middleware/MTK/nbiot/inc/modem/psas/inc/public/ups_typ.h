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
 * 3G PS Primitive Data Type definitions required by more than one interface
 *************************************************************************/

#if !defined (UPS_TYP_H)
#define       UPS_TYP_H

/***************************************************************************
*   Manifest Constants
***************************************************************************/

/** \defgroup PrdLayer1 Layer 1
 * \ingroup Prd
 * Defines Layer 1 (L1) interface.
 * Layer 1 is the part of the system that interfaces to the physical layer
 * and runs in synchronisation with the base-station to which the MS is
 * camped.  Its role is to interpret commands from the Protocol Stack and to
 * schedule actions for the physical layer to perform in order to action
 * transmission, reception and monitoring on the air-interface.
 */

/** \defgroup ProtoStackAs Access Stratum
 * \ingroup Prd
 * Defines Access Stratum (AS) interface.
 * This section provides details of the functionality of each module in the WCDMA/GSM Access Stratum.
 */

/** \defgroup Prd3gDmNas Non Access Stratum
 * \ingroup Prd
 * Defines Non Access Stratum (NAS) interface.
 * This section provides details of the functionality of each entity in the NAS.
 */

/** \defgroup PrdMW Middleware
 * \ingroup Prd
 * Defines Middleware (MW) interface.
 * This section provides details of the functionality of each module in the Middleware area.
 */

/** \defgroup Layer13G 3G Layer 1
 * \ingroup PrdLayer1
 * 3G layer 1 interface.
 */

/** \defgroup PrdGSMLayer1 GSM Layer 1
 * \ingroup PrdLayer1
 * GSM Layer 1.
 */

/** \defgroup Prd3gDmProtoStack 3G Access Stratum
 * \ingroup ProtoStackAs
 * Defines 3G Access Stratum (AS) interface.
 */

/** \defgroup GSMProtoStack 2G Access Stratum
 * \ingroup ProtoStackAs
 * Defines 2G Access Stratum (AS) interface.
 */

/** \defgroup NasApex APEX
 * \ingroup Prd3gDmNas
 * The APEX interface is the signalling interface into the top of the
 * protocol stack, and can be used to control all aspects of the stack.
 * The APEX interface is sub-divided into different groups relating to
 * different areas of functionality.
 */

/** \defgroup Cfg3gPs Protocol Stack Configuration
 * \ingroup Prd3gDmProtoStack
 * \ingroup Cfg
 * This sections defines the Protocol Stack configuration options.
 * @{
 */

#if !defined (UPS_INTRA_FREQ_FILTER_COUNT)

/** Defines the number of measurements the physical layer will send every
 * 200 ms.
 */
# define UPS_INTRA_FREQ_FILTER_COUNT           4
#endif /* UPS_INTRA_FREQ_FILTER_COUNT */

#if !defined (UPS_MAX_MEASURED_INTER_FREQ_CELLS)

/** Defines the maximum number of inter-frequency cells that can be measured by
 * L1.
 */
# define UPS_MAX_MEASURED_INTER_FREQ_CELLS     32
#endif /* UPS_MAX_MEASURED_INTER_FREQ_CELLS */

#if defined (UPGRADE_GGE)

#if !defined (UPS_MAX_MEASURED_INTER_RAT_CELLS)

/** Defines the maximum number of inter-RAT cells that can be measured by L1. */
# define UPS_MAX_MEASURED_INTER_RAT_CELLS      32
#endif /* UPS_MAX_MEASURED_INTER_RAT_CELLS */

#endif

#if defined (UPGRADE_3G_HSUPA)

/** Defines the maximum total number of transport blocks transmitted within
 * TTIs per LogCh that start at the same time in the uplink.
 */
#if !defined(UPS_MAX_EDCH_MACD_PDUS_PER_LCH)
# define UPS_MAX_EDCH_MACD_PDUS_PER_LCH               64
#endif
/** Defines the maximum total number of transport blocks transmitted within
 * TTIs that start at the same time in the uplink.
 */
#if !defined(UPS_MAX_EDCH_MACD_PDUS_PER_TTI)
# define UPS_MAX_EDCH_MACD_PDUS_PER_TTI               (UPS_MAX_EDCH_MACD_PDUS_PER_LCH+6)
#endif
/** Defines the maximum logical channels that can be multiplexed on an E-DCH
 *  transport channel (25.309 7.1.1)
 */
# define MAX_LOGICAL_CHANS_PER_EDCH_TRANSPORT_CHAN    15

# define EDCH_MAX_MACD_FLOWS                          8

/** Defines the maximum physical channels that can be used to carry E-DCH data
 */
# define EDCH_MAX_NB_EDPDCH                           4

/* Maximum possible number of reference E-TFCs (derived from ASN)           */
# define   EDCH_MAX_NUM_REF_TFCS            8

/* A TFCS subset is a group of TFCs that have a common value for spreading  */
/* factor and reference TFC.                                                */
# define   EDCH_MAX_ETFCS_SUBSETS           (EDCH_NUM_SF + EDCH_MAX_NUM_REF_TFCS)

# define   EDCH_MIN_ETFCS_TBSIZE_FDD        18
/* Maximum E-TFCS size (25.321 v690 Annex B) */
# define   EDCH_MAX_ETFCS_SIZE              128

# define   EDCH_INVALID_ETFCI               0xFF

/* Define the sizes of each pre-defined TFCS (25.321 v690 Annex B) */
#define   EDCH_ETFCS_2_0_SIZE                                  EDCH_MAX_ETFCS_SIZE
#define   EDCH_ETFCS_2_1_SIZE                                  126
#define   EDCH_ETFCS_10_0_SIZE                                 EDCH_MAX_ETFCS_SIZE
#define   EDCH_ETFCS_10_1_SIZE                                 121

/* Number of slots in an FDD frame (10ms) */
# define   EDCH_SLOTS_PER_FRAME             15

/* Number of E-DCH sub-frames (2ms) in a frame (10ms) */
# define   EDCH_NUM_SUBFR_IN_FR             5

/* Minimal number of slots in an FDD E-DCH TTI (2ms or 10ms) */
# define   EDCH_MIN_TX_SLOTS_PER_2MS_TTI_FDD    (EDCH_SLOTS_PER_FRAME/EDCH_NUM_SUBFR_IN_FR)
# define   EDCH_MIN_TX_SLOTS_PER_10MS_TTI_FDD   8

/** max number HARQ processes for all transmission type for all UMTS modes*/
# define NUM_OF_HARQ_PROCESSES      8
/** Index of Zero_Grant as defined in TS 25.331 section 10.3.6.97 */
# define EDCH_ZERO_GRANT            38
/** Index of Minimum_Grant as defined in TS 25.321 section 3.1.2.2 */
# define EDCH_MINIMUM_GRANT         0
/** Inactive HARQ process */
# define EDCH_HARQ_INACTIVE         0xFF

#endif /* UPGRADE_3G_HSUPA */

#if !defined(UPS_MINIMUM_SKIP_FRAMES)

/** Defines the minimum number of frames to skip to next required BCH frame. */
#define UPS_MINIMUM_SKIP_FRAMES     6
#endif /* UPS_MINIMUM_SKIP_FRAMES */

#if defined(UPGRADE_3G_HSDPA)

/** Defines the number of MAC-hs PDU buffers required to be available to L1. */
#define UPHY_MAX_POINTER_ASSIGN     3
#endif /* UPGRADE_3G_HSDPA */

/***************************************************************************
* 3G Build Option Checking
***************************************************************************/
#if defined(UPGRADE_3G)

#  if defined (UPGRADE_REL6)
#    if !defined(UPGRADE_REL5)
#      error For a 3GPP Release 6 build, UPGRADE_REL5 is required
#    endif /* UPGRADE_REL5 */
#  else
#    if defined (UPGRADE_REL5)
#      if !defined(UPGRADE_REL4)
#        error For a 3GPP Release 5 build, UPGRADE_REL4 is required
#      endif /* UPGRADE_REL4 */
#    endif /* UPGRADE_REL5 */
#  endif /* UPGRADE_REL6 */

#  if defined(UPGRADE_3G_HSDPA)
#    if !defined(UPGRADE_REL5)
#      error HSDPA must be built with at least 3GPP Release 5
#    endif /* UPGRADE_REL5 */
#  endif /* UPGRADE_3G_HSDPA */

#  if defined(UPGRADE_3G_HSUPA)
#    if !defined(UPGRADE_REL6)
#      error HSUPA must be built with at least 3GPP Release 6
#    endif /* UPGRADE_REL6 */
#    if !defined(UPGRADE_3G_HSDPA)
#      error HSUPA must be built with HSDPA
#    endif /* UPGRADE_3G_HSDPA */
#  endif /* UPGRADE_3G_HSUPA */
#endif

/***************************************************************************
* Nested Include Files
***************************************************************************/

#include <uas_asn.h>
#include <system.h>
#include <u1_typ.h>

/***************************************************************************
*   Manifest Constants
***************************************************************************/

/** \ingroup Cfg3gPs
 *
 * @{
 */

/** Defines the maximum stored cell identities. */
#define MAX_STORED_CELL_IDENTITIES     MAX_STORED_SCRAMBLING_CODES

/** Defines the security key length in octets. */
#define SECURITY_KEY_LENGTH            16

/** Defines the number of bits in a START value. */
#define UPS_START_VALUE_BITS           20  /* START value is 20 bits */
/** Defines the default START value. */
#define UPS_DEFAULT_START_VALUE        ((USTART_Value)0x00000000)

/** Defines the invalid START value. */
#define UPS_INVALID_START_VALUE        ((USTART_Value)0xFFFFFFFF)

/** Defines the maximum START value. */
#define UPS_MAX_START_VALUE            ((USTART_Value)((0x1 << UPS_START_VALUE_BITS) - 1))

/* Cipher KSI's
 *                                     Bit
 *          7       6       5       4       3       2       1       0
 *      --------|-------|-------|-------|-------|-------|-------|--------
 *      |            Unused             |Domain |          KSI           |
 *      --------|-------|-------|-------|-------|-------|-------|--------
 *
 * Where:   Domain: 0 = CS, 1 = PS
 *          KSI   : 0-6 = Keys, 7(UPS_NO_KEY_SET) = No key
 */

/** Defines the invalid Key Set Identifier. */
#define UPS_INVALID_KSI                0XFF

/** Defines that a key is not present in ciphering or integrity protection.
 * First 3 bits are used for KSI. */
#define UPS_NO_KEY_SET                 0x07

/** Defines the mask for identifying the KSI domain. */
#define UPS_KSI_DOMAIN_MASK            0x08

/** Defines the dimension of an array to hold all cipher keys, which for fast
 * access needs to be large enough to hold keys 0-14, although key 7 will
 * always be empty.
 */
#define UPS_KSI_ARRAY_SIZE             15       /* KSI's 0-14 */

/** To right shift the COUNT-C value of an AM entity by 12 bits to
 * extract the START value, which is the 20 MSBs of COUNT-C.
 */
#define COUNTC_TO_START(countc)             ((countc) >> 12)

/** To left shift the START value by 12 bits to fit in the 20 MSB of the COUNT-C
 * used for AM entity.
 */
#define START_TO_COUNTC(start)              ((start) << 12)

/** To mask the COUNT-C of an UM entity by 25 MSB mask to extract the
 * HFN component with 7 LSBs set to 0.
 */
#define COUNTC_MSB_MASK(x)                  ((x) & 0xFFFFFF80)

/** To right shift the COUNT-C value of an AM entity by 12 bits to extract the
 * START value, which is the 20 MSBs of COUNT-C.
 */
#define COUNTC_TO_COUNTC_MSB(countc)        COUNTC_TO_START (countc)

/** To left shift 25 MSB of COUNT-C by 7 bits to fit in UM entity COUNT-C. */
#define COUNTC_MSB_TO_COUNTC(countcmsb)     ((countcmsb) << 7)

/** To right shift TM entity HFN by 4 bits to extract the START value. */
#define TM_HFN_TO_START(hfn)                ((hfn) >> 4)

/** To left shift START value by 4 bits in to MSB of the HFN. */
#define START_TO_TM_HFN(start)              ((start) << 4)

/** Defines the maximum COUNT-I sequence number. */
#define UPS_MAX_COUNTI_SEQ_NUM       ((URRC_MessageSequenceNumber)15)

/** Defines the number of bits used for storing KSI. */
#define URRC_KEY_SET_ID_BIT_SIZE     3    /* 33.102 - 6.4.4 */

/** Defines the mask for 3 LSBs of KSI. */
#define URRC_KEY_SET_ID_BIT_MASK     ((RrcKeySequence) 0x07)

/** To unify the 4 LSBs of RRC sequence number with 28 MSBs of the HFN
 * of COUNT-I. */
#define SET_COUNTI(hfn, sn)   \
    ((((Int32) (hfn)) << 4) | \
     (((Int32) (sn)) & (Int32) (0xf)))

/** To right shift the HFN by 8 bits to extract the START value. */
#define COUNTI_HFN_TO_START_VALUE(hfn)       ((hfn) >> 8)

/** to left shift the START value by 8 bits to extract HFN. */
#define COUNTI_START_VALUE_TO_HFN(start)     ((start) << 8)

/** To return the CN domain as a string "CS"/"PS" */
#define DEBUG_DOMAIN_NAME_STRING(cn)          \
    (((cn) == UCN_DomainIdentity_cs_domain) ? \
     "CS" : "PS")

/* defines above are based on values specified in 33.102 */

/** Defines the longest TTI in frames, which is 80 ms. */
#define UPS_LONGEST_TTI_IN_FRAMES               8

/** Defines the maximum RLC SDU octets, which is used to define the maximum
 * size of an SDU data.
 * Although [3G TS 24.008] defines this to be 1520, we've witnessed SDUs which
 * exceed that on an HSDPA network.
 * The book "PPP design, implementation and debugging" states that although
 * the Maximum Receive Unit (MRU) is 1500 octets, the author has witnessed
 * implementation errors that allow MRUs of 1524, 1576 and 1600 octets
 */
#define UPS_MAX_RLC_SDU_DATA_OCTETS             1600

/** Defines the maximum number of octets required to allow space for in-sutu
 * PDCP header decompression.
 */
#if defined (PDCP_RFC2507_HC)
#define UPS_MAX_HEADER_DECOMPRESSION_OCTETS     168
#else /* PDCP_RFC2507_HC */
#define UPS_MAX_HEADER_DECOMPRESSION_OCTETS     128
#endif /* PDCP_RFC2507_HC */

/** Defines the maximum RLC SDU octets, which is used to define the size of the
 * SDU data array when the data is passed within the primitive, instead of by
 * reference. It's size comes from <tt> [3G TS 24.008] </tt> (1520) + an
 * allowance for PDCP in-situ header decompression, which although only needed
 * on the downlink on bearers owned by PDCP, its used to define both uplink and
 * dowwnlink SDU's. However as in-primitive SDU data will only be used during
 * development, its not seen as a problem
 */
#define UPS_MAX_RLC_SDU_OCTETS     \
    (UPS_MAX_RLC_SDU_DATA_OCTETS + \
     UPS_MAX_HEADER_DECOMPRESSION_OCTETS)

/* The following 3 constants are common to CSR in RRC and L1. */
#if defined (UPGRADE_NOT_FOR_RELEASE)

/* They may be made part of a "#if defined (UPGRADE_GGE)"
   only when it is established why this #define cannot be used in
   cphy_sig.h. */
#endif

/** Defines the System Information Message Type 3 in GSM. */
#define CPHY_GSM_SYS_INFO_MESSAGE_TYPE_2     0x02
#define CPHY_GSM_SYS_INFO_MESSAGE_TYPE_3     0x03

/** Defines the System Information Message Type 4 in GSM. */
#define CPHY_GSM_SYS_INFO_MESSAGE_TYPE_4     0x04

/***************************************************************************
*   Macro Functions
***************************************************************************/

/***************************************************************************
*   Types
***************************************************************************/

/* The following 4 typedef's are common to CSR in RRC and L1. */
#if defined (UPGRADE_NOT_FOR_RELEASE)

/* They may be made part of a "#if defined (UPGRADE_GGE)"
   only when it is established why this #define cannot be used in
   cphy_sig.h.*/
#endif

/** Indicates the type of logical channel. */
typedef enum UlogicalChannelTypeTag
{
    /** Broadcast Control Channel */
    UBCCH = 0,

    /** Paging Control Channel */
    UPCCH = 1,

    /** Common Control Channel */
    UCCCH = 2,

    /** Common Traffic Channel */
    UCTCH = 3,

    /** Dedicated Control/Traffic Channel */
    UDCCH_DTCH = 4
}
UlogicalChannelType;

/** A 16 octet length security key used in Ciphering and Integrity. */
typedef Int8 USecurityKey [SECURITY_KEY_LENGTH];

/** The key set identifier (KSI) is a number which is associated with the cipher
 * and integrity keys derived during authentication. The key set identifier is
 * allocated by the network and sent with the authentication request message to
 * the mobile station where it is stored together with the calculated cipher key
 * CK and integrity key IK.
 */
typedef Int8 KeySeqId;

#if defined (UPGRADE_GGE)

/** Defines the GSM cell information. */
typedef struct UGSM_CellInformationTag
{
    /** Indicates the Base Transceiver Identification Code, which consists of
     * NCC and BCC.
     */
    UBSIC           BSIC;

    /** Indicates the band of the GSM cell. */
    UFrequency_Band bandIndicator;

    /** Indicates the ARFCN of the GSM cell. */
    Int16           BCCH_ARFCN;
}
UGSM_CellInformation;
#endif

/** Defines the different codec types. For more information refer to <tt>[3G TS
 * 26.103]</tt>.
 */
typedef enum CsCodecTypeTag
{
    /** GSM Full Rate Codec Type with CoID = 00000000b */
    CS_CODEC_TYPE_FR_COID = 0,

    /** GSM Half Rate Codec Type with CoID = 00000001b */
    CS_CODEC_TYPE_HR_COID = 1,

    /** GSM Enhanced Full Rate Codec Type with CoID = 00000010b */
    CS_CODEC_TYPE_EFR_COID = 2,

    /** Full Rate Adaptive Multi-Rate Codec Type with CoID = 00000011b */
    CS_CODEC_TYPE_FR_AMR_COID = 3,

    /** Half Rate Adaptive Multi-Rate Codec Type with CoID = 00000100b */
    CS_CODEC_TYPE_HR_AMR_COID = 4,

    /** UMTS Adaptive Multi-Rate Codec Type with CoID = 00000101b */
    CS_CODEC_TYPE_UMTS_AMR_COID = 5,

    /** UMTS Adaptive Multi-Rate2 Codec Type with CoID = 00000110b */
    CS_CODEC_TYPE_UMTS_AMR_2_COID = 6,

    /** Unspecified Codec Type Codec Type with CoID = 00001111b */
    CS_CODEC_TYPE_NONE_SPECIFIED = 15
}
CsCodecType;

/* PduListData is an array of octets, containing a list of contiguous PDUs,
 * where each PDU in the list allows room for the maximum transport block size,
 * and contains a bit offset, length and status.
 * Each PDU block is filled from the top down, and allows the higher layers to
 * strip off their header information.
 * Example list, passed to RLC from MAC, containing two 10 bit PDU's,
 * where 9 bits of MAC header has already been stripped off:
 *      bitOffset[ 0 ]      = 9
 *      bitOffset[ 1 ]      = 9
 *      bitOffset[ 2 ]      = 0
 *      ...
 *      bitLength[ 0 ]      = 10
 *      bitLength[ 1 ]      = 10
 *      bitLength[ 2 ]      = 0
 *      ...
 *      data[ 0 ] PDU 1     = Bit 7 contained MSB of MAC headers of PDU 1
 *      data[ 1 ]           = Bit 7 contained LSB MAC header of PDU 1
 *                            Bit 6 contains MSB of PDU 1 data
 *      data[ 2 ]           = Bit 5 contains LSB of PDU 1 data
 *                PDU 2       Bits 4-0 contained MAC header of PDU 2
 *      data[ 3 ]           = Bits 7-4 contained MAC header of PDU 2
 *                            Bit 3 contains MSB of PDU 2 data
 *      data[ 4 ]           = Bit 2 contains LSB of PDU 2 data
 *                            Bits 1-0 unused
 */

#if defined(UPGRADE_3G_HSDPA)
/** Defines the MAC-hs PDU received on HS-DSCH. Only one data block can be
 * received each 2ms TTI.
 */
typedef struct UmacHsDataTag
{
    /** The \c data array size is the maximum MAC-hs PDU size, which is
     * dependent on the HS-DSCH capability as defined in <tt>[3G TS 25.306]</tt> 
     * (see table below).
     * The actual decoded data always starts
     * at position data[0] in this array
     * (starting with "Version Flag" bits as defined in <tt> [TS 25.321 section
     * 9.2.2]</tt>. Note that the data array always starts on a 32-bit
     * boundary.
     *
     * <TABLE>
     * <TR><TD><B>Data Class (Mbps)</B></TD> <TD><B>Max bits per TTI</B></TD> <TD><B>Max MAC-hs SDUs per TTI</B></TD></TR>
     * <TR><TD>1.2 (cat. 1)            </TD> <TD>7298                   </TD> <TD>70                            </TD></TR>
     * <TR><TD>3.6 (cat. 5)            </TD> <TD>7298                   </TD> <TD>70                            </TD></TR>
     * <TR><TD>7 (cat. 7)              </TD> <TD>14411                  </TD> <TD>70                            </TD></TR>
     * <TR><TD>10 (cat. 9)             </TD> <TD>20251                  </TD> <TD>70                            </TD></TR>
     * <TR><TD>category 10             </TD> <TD>27952                  </TD> <TD>70                            </TD></TR>
     * </TABLE>
     *
     * The constant <tt> UDL_HS_MAX_MAC_HS_PDU_DATA_OCTETS </tt> is set
     * according to HS-DSCH category using the maximum bits per TTI as given
     * above.
     * \todo TO BE CONTINUED FOR TDD128.
     */
    Int8                                    data[UDL_HS_MAX_MAC_HS_PDU_DATA_OCTETS];
}
UmacHsData;
#endif /* UPGRADE_3G_HSDPA */

/** Defines the cell for \c CbmcCellChangeInd and the corresponding Indication
 * to the application layer.
 */
typedef struct UCellDescriptionTag
{
    /** Indicates if Cell Broadcast Services are supported in this cell or not.
     */
    Boolean       cbSupported;

    /** Indicates the downlink frequency of the cell. */
    UUARFCN       uarfcn_dl;

    /** Indicates the Location Area Identifier of the cell. */
    ULAI          lai;

    /** Indicates the cell identity of the cell. */
    UCellIdentity cellIdentity;
}
UCellDescription;

/** Specifies the COUNT-C to use on both CS and PS domains when
 * \c CmacHfnAbortReq primitive is sent to MAC.
 */
typedef struct UmacTmCountcInfoTag
{
    /** Specifies the COUNT-C for the CS domain. */
    Int32 csCountC;

    /** Specifies the COUNT-C for the PS domain. */
    Int32 psCountC;
}
UmacTmCountcInfo;

/** Indicates the RLC entity type for a bearer. */
typedef enum UrlBearerModeTag
{
    /** Transparent Mode */
    URL_BEARER_MODE_TM = 0,

    /** Unacknowledged Mode */
    URL_BEARER_MODE_UM = 1,

    /** Acknowledged Mode */
    URL_BEARER_MODE_AM = 2,

    /** Number of bearer modes */
    URL_MUM_BEARER_MODES = 3
} UrlBearerMode;

/** Defines the information required by MAC for TFC selection for an
 * un-segmented transparent mode radio bearer, where MAC is allowed to
 * transmit multiple SDU's in a TTI (one per PDU), provided each SDU is the
 * same size.
*/
typedef struct UrlTmUnsegmentedTfcSelectionInfoTag
{
    /** Indicates how many SDU's of the same size (\c sduBits) are available
     * for transmission.
     */
    Int16 numSdus;

    /** Indicates the size of each SDU in bits. */
    Int16 sduBits;
} UrlTmUnsegmentedTfcSelectionInfo;

/** Defines the information required by MAC for TFC selection
 * for a segmented transparent mode radio bearer, where MAC is allowed to
 * transmit one SDU per TTI, segmented into one or more  PDU's of the same size.
 */
typedef struct UrlTmSegmentedTfcSelectionInfoTag
{
    /** Indicates the size of the next available SDU in bits. */
    Int32 sduBits;
} UrlTmSegmentedTfcSelectionInfo;

/** Defines the information required by MAC for TFC selection for a transparent
 * mode radio bearer, and its contents differ according to its configuration.
 */
typedef struct UrlTmTfcSelectionInfoTag
{
    /** A boolean flag to indicate if the bearer is configured to transmit
     * un-segmented or segmented SDU's:
     *     - FALSE: Unsegmented (one SDU per PDU, provided all the SDUs are of
     *              the same size)
     *     - TRUE: Segmented (one SDU per TTI, evenly segmented)
     
\assoc UNION \ref data */
    Boolean segmented;

    /** A data structure to define either of these:
     *     - #UrlTmUnsegmentedTfcSelectionInfo
     *     - #UrlTmSegmentedTfcSelectionInfo
     */
    union UrlTmTfcSelectionInfoDataUnionTag
    {
        /** Refer to #UrlTmUnsegmentedTfcSelectionInfo */
        UrlTmUnsegmentedTfcSelectionInfo unsegmented;

        /** Refer to #UrlTmSegmentedTfcSelectionInfo */
        UrlTmSegmentedTfcSelectionInfo segmented;
    }
    data;
}
UrlTmTfcSelectionInfo;

/** Defines the information required by MAC for TFC selection for an
 * unacknowledged mode radio bearer.
 */
typedef struct UrlUmTfcSelectionInfoTag
{
    /** Indicates an approximation of the number of bits of data available
     * for transmission, given that RLC doesn't know the PDU size that will be
     * selected by MAC; and it includes:
     *     - the total number of bits of SDU data available for transmission
     *     - the size of one length indicator for each SDU
     *     - the size of one length indicator for each SDU that requires a START
     *       LI
     */
    Int32   numBitsForTx;
}
UrlUmTfcSelectionInfo;

/** Defines the information required by MAC for TFC selection for an
 * acknowledged mode radio bearer, and its contents differ according to
 * configuration.
 */
typedef struct UrlAmTfcSelectionInfoTag
{
    /** Indicates if the bearer is configured for one or two logical channels.
     */
    Boolean twoLogicalChannels;

    /** Indicates how many bits of control PDUs are available for transmission.
     */
    Int32   numControlBitsForTx;

    /** Indicates how many bits of data PDUs are available for transmission
     * (excludes PDUs that can't be transmitted yet).
     */
    Int32   numDataBitsForTx;
}
UrlAmTfcSelectionInfo;

/** Defines the information required by MAC for TFC selection for a radio
 * bearer, and its contents vary according to the bearer mode.
 */
typedef struct UrlTfcSelectionInfoTag
{
    /** Indicates the operating mode of a bearer, which can be AM/UM/TM. 
\assoc UNION \ref info */
    UrlBearerMode mode;

    /** Holds any one of the following based on \c mode:
     *     - #UrlTmTfcSelectionInfo
     *     - #UrlUmTfcSelectionInfo
     *     - #UrlAmTfcSelectionInfo
     */
    union UrlTfcSelectionInfoUnionTag
    {
        UrlTmTfcSelectionInfo tm;
        UrlUmTfcSelectionInfo um;
        UrlAmTfcSelectionInfo am;
    }
    info;
}
UrlTfcSelectionInfo;

/** Indicates the total number of bits of data that are queued for transmission
 * on UM and AM bearers, and includes PDU that are not allowed to be
 * transmitted yet. For TM bearers this value is always set to 0.
 */
typedef Int32 UrlTrafficVolumeBits;

/** Defines the information required by MAC for TFC selection and traffic
 * volume measurements for a radio bearer, and its contents vary according to
 * the bearer mode.
*/
typedef struct UrlBufferInfoTag
{
    /** Indicates the information required by MAC for TFC selection, where its
     * content differs for each mode.
     */
    UrlTfcSelectionInfo  tfcSelection;

    /** Indicates the total number of bits of data queued for transmission. */
    UrlTrafficVolumeBits trafficVolumeBits;

#if defined (ENABLE_TRAFFIC_SHAPING)
    /** Indicates the total number of bits of data queued for transmission (available in Traffic shaping entity). */
    UrlTrafficVolumeBits extraTrafficVolumeInBits;

    /** Indicates the total number of bits of data queued for first transmission in RLC */
    Int32 untransmittedTrafficVolumeInBits;
#endif
}
UrlBufferInfo;

#if defined(UPGRADE_3G_HSUPA)
typedef struct UrlBufferInfoForEdchTag
{
    /** Indicates the information required by MAC for TFC selection for RLC control, mode independant.
     */
    Int32                   tfcSelectionCtrlBits;

    /** Indicates the information required by MAC for TFC selection for RLC data, mode independant.
     */
    Int32                   tfcSelectionDataBits;

    /** Indicates the total number of bits of data queued for transmission. */
    UrlTrafficVolumeBits    trafficVolumeBits;

#if defined (ENABLE_TRAFFIC_SHAPING)
    /** Indicates the total number of bits of data queued for transmission (available in Traffic shaping entity). */
    UrlTrafficVolumeBits    extraTrafficVolumeInBits;
#endif
}
UrlBufferInfoForEdch;

# if defined(UPGRADE_3G_FDD)
typedef struct EdchSfBoundaryTag
{
    /** Each valid SF is represented by an element in this array, the       */
    /* enumerated type EdchSf can be used as an index into the array to     */
    /* access the required SF.                                              */
    /* Each element contains the maxTBSize that is the maximum allowed by   */
    /* the corresponding SF. Unused SFs are set to 0.                       */
    Int16   maxTBSize[EDCH_NUM_SF];
}
EdchSfBoundary;


extern const Int16 umaTfc2msTable0 [EDCH_ETFCS_2_0_SIZE];
extern const Int16 umaTfc2msTable1 [EDCH_ETFCS_2_1_SIZE];
extern const Int16 umaTfc10msTable0 [EDCH_ETFCS_10_0_SIZE];
extern const Int16 umaTfc10msTable1 [EDCH_ETFCS_10_1_SIZE];
# endif /*UPGRADE_3G_FDD*/
#endif /*UPGRADE_3G_HSUPA*/
/** @} */ /* End of Cfg3gPs group */
#include <ups_cfg.h>

#endif /* UPS_TYP_H */

/* END OF FILE */

