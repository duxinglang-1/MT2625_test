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
 * 3G L1 Primitive Data Type definitions for the UPHY Interface
 *************************************************************************/

#if !defined (U1_TYP_H)
#define       U1_TYP_H

#include <system.h>
#include <u1_cfg.h>
#include <uas_asn.h>

/***************************************************************************
* Nested Include Files
***************************************************************************/


/***************************************************************************
*   Macro Functions
***************************************************************************/

/** Defines the maximum number of octets required to hold an uplink transport
 * block list, allowing optional alignment padding per transport block, to
 * ensure that the start of each transport block is word aligned, so that the
 * physical layer can avoid bit copies.
 */
#define UUL_MAX_TB_LIST_DATA_OCTETS     \
    (BITS_TO_INT8S(UPS_MAX_UL_TB_BITS) + \
     (UPS_TRANSPORT_BLOCK_ALIGNMENT_IN_OCTETS * \
        UPS_MAX_UL_NO_OF_TB_PER_TTI \
      ) \
    )

/** Defines the maximum number of octets required to hold a downlink transport
 * block list, allowing optional alignment padding per transport block, to
 * ensure that the start of each transport block is word aligned, so that the
 * physical layer can avoid bit copies. Note that in DL we also add CRC bits
 * per TB, as in test loop mode 2 these may be appended to each TB.
 */
#define UDL_MAX_TB_LIST_DATA_OCTETS     \
    (BITS_TO_INT8S(UPS_MAX_DL_TB_BITS) + \
     ((UPS_TRANSPORT_BLOCK_ALIGNMENT_IN_OCTETS + \
       UPS_MAX_CRC_SIZE_IN_OCTETS) * \
        UPS_MAX_DL_NO_OF_TB_PER_TTI \
      ) \
    )

#if defined(UPGRADE_REL5) /* && defined (UPLANE_INTEGRATION_TEST) */
# if defined(UPGRADE_3G_HSDPA)
/** Defines the maximum number of MAC-hs PDUs in octets. */
#  define UDL_HS_MAX_MAC_HS_PDU_DATA_OCTETS  \
      (BITS_TO_INT8S(UPS_HS_DSCH_MAX_BITS_PER_TTI_FOR_PS))
# endif /* UPGRADE_3G_HSDPA */
#endif /* UPGRADE_REL5  && UPLANE_INTEGRATION_TEST */

/** Defines the maximum number of bearers which can be put into loopback mode.
 *  (Moved from Manifest Constant area so could be assigned to RB_NUM_BEARERS)
 */
#define UPS_MAX_LOOPBACK_BEARERS    RB_NUM_BEARERS

/***************************************************************************
*   Types
***************************************************************************/

/** Indicates the radio bearer identifier. RB1 to RB32 is configured by the
 * network.
 */
typedef enum BearerIdentityTag
{
    RB_0            =  0, /**< SRB0 on CCCH (UL:TM DL:UM) */
    RB_1            =  1, /**< SRB1 on (UL:UM DL:UM) */
    RB_2            =  2, /**< SRB2 on (UL:AM DL:AM) */
    /** SRB3 on (UL:AM DL:AM) for high priority NAS signalling */
    RB_3            =  3,
    /** SRB4 on (UL:AM DL:AM) for low priority NAS signalling, which is optional */
    RB_4            =  4,
    RB_5            =  5, /**< Radio Bearer 5, configured by the network. */
    RB_6            =  6, /**< Radio Bearer 6, configured by the network. */
    RB_7            =  7, /**< Radio Bearer 7, configured by the network. */
    RB_8            =  8, /**< Radio Bearer 8, configured by the network. */
    RB_9            =  9, /**< Radio Bearer 9, configured by the network. */
    RB_10           = 10, /**< Radio Bearer 10, configured by the network. */
    RB_11           = 11, /**< Radio Bearer 11, configured by the network. */
    RB_12           = 12, /**< Radio Bearer 12, configured by the network. */
    RB_13           = 13, /**< Radio Bearer 13, configured by the network. */
    RB_14           = 14, /**< Radio Bearer 14, configured by the network. */
    RB_15           = 15, /**< Radio Bearer 15, configured by the network. */
    RB_16           = 16, /**< Radio Bearer 16, configured by the network. */
    RB_17           = 17, /**< Radio Bearer 17, configured by the network. */
    RB_18           = 18, /**< Radio Bearer 18, configured by the network. */
    RB_19           = 19, /**< Radio Bearer 19, configured by the network. */
    RB_20           = 20, /**< Radio Bearer 20, configured by the network. */
    RB_21           = 21, /**< Radio Bearer 21, configured by the network. */
    RB_22           = 22, /**< Radio Bearer 22, configured by the network. */
    RB_23           = 23, /**< Radio Bearer 23, configured by the network. */
    RB_24           = 24, /**< Radio Bearer 24, configured by the network. */
    RB_25           = 25, /**< Radio Bearer 25, configured by the network. */
    RB_26           = 26, /**< Radio Bearer 26, configured by the network. */
    RB_27           = 27, /**< Radio Bearer 27, configured by the network. */
    RB_28           = 28, /**< Radio Bearer 28, configured by the network. */
    RB_29           = 29, /**< Radio Bearer 29, configured by the network. */
    RB_30           = 30, /**< Radio Bearer 30, configured by the network. */
    RB_31           = 31, /**< Radio Bearer 31, configured by the network. */
    RB_32           = 32, /**< Radio Bearer 32, configured by the network. */

    /** Internal Bearer for BCCH, which can't be referenced by the network. */
    RB_BCCH         = 33,
    /** Internal Bearer for PCCH, which can't be referenced by the network. */
    RB_PCCH         = 34,
    /** Internal Bearer for CTCH, which can't be referenced by the network. */
    RB_CTCH         = 35,

#if defined (UPGRADE_3G_MBMS) && defined (UPGRADE_3G_TDD128)
    /** Internal Bearer for MCCH, which can't be referenced by the network. */
    RB_MCCH         = 36,
    /** Internal Bearer for MSCH_01, which can't be referenced by the network. */
    RB_MSCH_01      = 37,
    /** Internal Bearer for MSCH_02, which can't be referenced by the network. */
    RB_MSCH_02      = 38,
    /** Internal Bearer for MTCH_01, which can't be referenced by the network. */
    RB_MTCH_01      = 39,
    /** Internal Bearer for MTCH_02, which can't be referenced by the network. */
    RB_MTCH_02      = 40,
    /** Internal Bearer for MTCH_03, which can't be referenced by the network. */
    RB_MTCH_03      = 41,
    /** Internal Bearer for MTCH_04, which can't be referenced by the network. */
    RB_MTCH_04      = 42,
    /** Internal Bearer for MTCH_05, which can't be referenced by the network. */
    RB_MTCH_05      = 43,
    /** Internal Bearer for MTCH_06, which can't be referenced by the network. */
    RB_MTCH_06      = 44,
    /** Internal Bearer for MTCH_07, which can't be referenced by the network. */
    RB_MTCH_07      = 45,
    /** Internal Bearer for MTCH_08, which can't be referenced by the network. */
    RB_MTCH_08      = 46,
    /** Internal Bearer for MTCH_09, which can't be referenced by the network. */
    RB_MTCH_09      = 47,
    /** Internal Bearer for MTCH_10, which can't be referenced by the network. */
    RB_MTCH_10      = 48,
    /** Internal Bearer for MTCH_11, which can't be referenced by the network. */
    RB_MTCH_11      = 49,
    /** Internal Bearer for MTCH_12, which can't be referenced by the network. */
    RB_MTCH_12      = 50,
    /** Internal Bearer for MTCH_13, which can't be referenced by the network. */
    RB_MTCH_13      = 51,
    /** Internal Bearer for MTCH_14, which can't be referenced by the network. */
    RB_MTCH_14      = 52,
    /** Internal Bearer for MTCH_15, which can't be referenced by the network. */
    RB_MTCH_15      = 53,
    /** Internal Bearer for MTCH_16, which can't be referenced by the network. */
    RB_MTCH_16      = 54,
    /** Internal Bearer for MTCH_17, which can't be referenced by the network. */
    RB_MTCH_17      = 55,
    /** Internal Bearer for MTCH_18, which can't be referenced by the network. */
    RB_MTCH_18      = 56,
    /** Internal Bearer for MTCH_19, which can't be referenced by the network. */
    RB_MTCH_19      = 57,
    /** Internal Bearer for MTCH_20, which can't be referenced by the network. */
    RB_MTCH_20      = 58,
    /** Internal Bearer for MTCH_21, which can't be referenced by the network. */
    RB_MTCH_21      = 59,
    /** Internal Bearer for MTCH_22, which can't be referenced by the network. */
    RB_MTCH_22      = 60,
    /** Internal Bearer for MTCH_23, which can't be referenced by the network. */
    RB_MTCH_23      = 61,
    /** Internal Bearer for MTCH_24, which can't be referenced by the network. */
    RB_MTCH_24      = 62,
    /** Internal Bearer for MTCH_25, which can't be referenced by the network. */
    RB_MTCH_25      = 63,
    /** Internal Bearer for MTCH_26, which can't be referenced by the network. */
    RB_MTCH_26      = 64,
    /** Internal Bearer for MTCH_27, which can't be referenced by the network. */
    RB_MTCH_27      = 65,
    /** Internal Bearer for MTCH_28, which can't be referenced by the network. */
    RB_MTCH_28      = 66,
    /** Internal Bearer for MTCH_29, which can't be referenced by the network. */
    RB_MTCH_29      = 67,
    /** Internal Bearer for MTCH_30, which can't be referenced by the network. */
    RB_MTCH_30      = 68,
    /** Internal Bearer for MTCH_31, which can't be referenced by the network. */
    RB_MTCH_31      = 69,
    /** Internal Bearer for MTCH_32, which can't be referenced by the network. */
    RB_MTCH_32      = 70,
#endif /* UPGRADE_3G_MBMS && UPGRADE_3G_TDD128 */

    RB_NUM_BEARERS, /**< Number of Radio Bearers */

    /** RB_INVALID is used by MAC to indicate a transport block whose CRC was
     * ok, but couldn't be parsed.
     */
    RB_INVALID      = 0xFF
}
BearerIdentity;

/** Defines GSM System information message type number.  */
typedef Int8        GsmControlBlockType;

/** Defines the GSM physical block as an array of octets. */
typedef Int8        GsmPhysicalBlock [CPHY_GSM_L1_PHYSICAL_BLOCK_SIZE];

/** <tt>GSM BSIC</tt> is used by UEs to distinguish different Base Stations, and
 * it is composed of 3-bit <tt>Network Colour Code (NCC)</tt> and 3-bit <tt>
 * Base station Colour Code (BCC)</tt>. <tt>NCC</tt> occupies the upper nibble
 * of an octet whereas the lower nibble is occupied by <tt>BCC</tt>.
 */
typedef Int8        GsmBsic;

/** Defines the RSSI (received signal strength) of the signal in GSM. */
typedef SignedInt16 GsmRssiLevel;

/** Defines the Level 2 scheduling information, which is also referred to as
 * <tt> inband scheduling message </tt>. This inband scheduling message is sent
 * on the CTCH together with the actual CB messages (as in CBS-GSM).
 * \note For more information refer to <tt> [3G TS 25.925 section 6.3.6.4]</tt>
 */
typedef struct UCBS_DRX_Level2InformationTag
{
    /** Indicates the SFN of when the last BMC scheduled message was received
     * and is used as a reference point for the \c receptionPeriodOffset.
     * \note If the TTI used was greater than 10ms then the SFN corresponds to
     * the first frame number that this message was received in.
     */
    Int16   /* 0 to 4095 */  referenceSfn;

    /** Indicates the number of reception periods with in a CBS Schedule Period.
     */
    Int16   /* 0 to 256 */   numberOfReceptionPeriods;

    /** Indicates the CTCH-BS indexes to be decoded. This is known after
     * reading the BMC Schedule Message.
     */
    Int16   /* 1 to 511 */   receptionPeriodOffset [CBS_RECEPTION_PERIOD_OFFSET_ARRAY_SIZE];
}
UCBS_DRX_Level2Information;

/** Defines the activation time. */
typedef struct ActivationTimeTag
{
    /* A boolean flag to determine if a CFN has been provided by the network (TRUE)
           or to use current UE CFN (FALSE).  */
    Boolean                    cfnSignalled;

    /** If \c now = FALSE then the activation time is at a specified CFN. */
    UActivationTime            cfn;
}
ActivationTime;

#if defined (UPGRADE_3G_MBMS)
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** Defines the activation time for MBMS. */
typedef struct ActivationTimeForMbmsTag
{
    /* A boolean flag to determine if a CFN has been provided by the network (TRUE)
           or to use current UE CFN (FALSE).  */
    Boolean                    cfnSignalled;

    /** If \c now = FALSE then the activation time is at a specified SFN. */
    Int16                      sfn;
}
ActivationTimeForMbms;
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */
#endif /* UPGRADE_3G_MBMS */

/** Defines either a SFN or a CFN. */
typedef union SfnOrCfnTag
{
    /** This member indicates the System Frame Number. */
    Int16                      sfn;

    /** This member indicates the Connection Frame Number. */
    Int8                       cfn;
}
SfnOrCfn;

/** Defines the frame number in idle/connected state. */
typedef struct UFrameNumberTag
{
    /** A boolean flag to define if the frame number should be given in SFN/CFN.
     * If it is TRUE then the frame number shall be given in CFN, and if it
     * FALSe then the frame number shall be given in SFN.

\assoc UNION \ref sfnOrCfn */
    Boolean                    connected;

    /** This member indicates the frame number in CFN/SFN. */
    SfnOrCfn                   sfnOrCfn;
}
UFrameNumber;

/** Indicates if the CRC of a received PDU was verified successfully or not.
 * It may also indicate is the associated structure is used or not.
 */
typedef enum UDlPduStatusTag
{
    /** CRC for this PDU okay. */
    UDL_PDU_STATUS_CRC_OK               = 0,
    /** CRC for this PDU failed. */
    UDL_PDU_STATUS_CRC_ERROR            = 1,
    /** Error at the MAC level. */
    UDL_PDU_STATUS_MAC_ERROR            = 2,
    /** Error for entire frame. */
    UDL_PDU_STATUS_MAC_NO_FRAME_NOT_USED      = 3, /* No longer used in MAC/RLC */

    /* The following values are used by RLC when it rejects a PDU in
     * UmacDlPduListInfoRsp by setting cipherInfo.dstData_p to NULL
     */

    /** Unknown bearer */
    UDL_PDU_STATUS_INVALID_BEARER       = 4,
    /** RLC in wrong state */
    UDL_PDU_STATUS_WRONG_STATE          = 5,
    /** Bearer stopped */
    UDL_PDU_STATUS_STOPPED              = 6,
    /** Bearer halted */
    UDL_PDU_STATUS_HALTED               = 7,
    /** Zero length PDU */
    UDL_PDU_STATUS_PDU_EMPTY            = 8,
    /** SN already available */
    UDL_PDU_STATUS_SN_DUPLICATE         = 9,
    /** SN outside RX window */
    UDL_PDU_STATUS_SN_OUTSIDE_WINDOW    = 10,
    /** Loop not closed on loopback bearer */
    UDL_PDU_STATUS_LOOP_NOT_CLOSED      = 11,
    /** Buffer full */
    UDL_PDU_STATUS_BUFFER_FULL          = 12,
    /** MAX SDU size exceeded */
    UDL_PDU_STATUS_MAX_SDU_EXCEEDED     = 13,

    /** The associated structure is not being used. */
    UDL_PDU_STATUS_UNUSED
} UDlPduStatus;

/** Indicates the type of transport channel. */
typedef enum UTransportChannelTypeTag
{
    /** Broadcast Transport Channel */
    UBCH    = 0,
    /** Paging Transport Channel */
    UPCH    = 1,
    /** FACH Transport Channel */
    UFACH   = 2,
    /** Dedicated Transport Channel */
    UDCH    = 3,
    /** Random Access Tranport Channel */
    URACH   = 4,
    /** High Speed Dedicated Shared Transport Channel */
    UHSDSCH = 5,
    /** Enhanced Dedicated Transport Channel */
    UEDCH = 6,
    /** Total number of transport channel types */
    NUM_TR_CHAN_TYPES
}
UTransportChannelType;

/** Defines the start and end of each transport block (or RLC PDU) and its
 * associated data (transport channel and TFI).
 */
typedef struct UDlPduListInfoTag
{
    /** Indicates UBCH, UPCH, UFACH or UDCH. */
    UTransportChannelType           trChType;

    /** Indicates a transport channel identity between MAC and PHY (and it is
     * in the range 1 to 32), but a radio bearer identity between RLC and MAC
     * (and is set to a value from the \c BearerIdentity enumeration). */
    Int8                            identity;

    /** Indicates an array of PDU bit offsets, from the end of the preceding
     * PDU in the list, where the most significant bit of used data starts, in
     * the range 0 to 255.
     */
    Int8                            bitOffset;

    /** Indicates an array of PDU bit lengths, in the range 0 to 65535. The bit
     * length for a transport block may only be zero in CELL_DCH. If this
     * transport channel has been specified as a channel requiring a quality
     * target, and no other transport channels have data to send, a "dummy TB"
     * may be specified to be transmitted by MAC, whose only useful information
     * is the CRC result.
     */
    Int16                           bitLength;

    /** Indicates the status of the PDU. */
    UDlPduStatus                    pduStatus;
}
UDlPduListInfo;

typedef struct U1PhyHsDlDataListTag
{
    /** The number of elements in \a pduInfo[] that are populated. */
    Int8           numberOfTransportBlocks;

    /** Array giving PDU status information. */
    UDlPduListInfo pduListInfo[1];
}U1PhyHsDlDataList;

/** Defines the downlink pdu data as an array of octets to hold downlink
 * transport blocks.
 */
typedef Int8 UDlPduListData [UDL_MAX_TB_LIST_DATA_OCTETS];

/** Defines the transport block set and contains a list of one or more
 * contiguous transport blocks, where each transport block must start on a
 * 16-bit word aligned address. To minimise copying of data this data type is
 * also used for RLC PDUs on the MAC interface, so that only the offsets and
 * lengths need to be modified.
 *
 * The below figure illustrates the PDU list on its journey up the protocol
 * stack:
 *
 * \image html prd3gdownlinkpdulist.png "Fig: Example of PDU list going up the stack in the downlink"
 * \image latex prd3gdownlinkpdulist.eps "Example of PDU list going up the stack in the downlink" width=12cm
 *
 * \note the alignment padding between transport blocks may be 8-bit, 16-bit or
 * 32-bit, depending on hardware requirements. The protocol stack and L1 use
 * the configurable parameter <tt> TRANSPORT_BLOCK_ALIGNMENT_IN_OCTETS </tt> to
 * define alignment.
 *
 */
typedef struct UDlPduListTag
{
    /** Indicates the number of transport blocks in this TTI.
\assoc ARRAY \ref pduListInfo */
    Int8                            numberOfTransportBlocks;

    /** Indicates the start and end of each transport block (or RLC PDU) and
     * its associated data (transport channel and TFI).
     */
    UDlPduListInfo                  pduListInfo [UPS_MAX_DL_NO_OF_TB_PER_TTI];

    /* byteLength is to used as a Genie association, to enable it to display
     * only the used area of the pduList. For example, for a list containing
     * two PDU's, it would need to be set to :
     *        bitOffset[ 0 ] + bitLength[ 0 ]
     *      + bitOffset[ 1 ] + bitLength[ 1 ]
     * It is also used to copy the entire data block where required.
     */

#if !defined (DATA_IN_SIGNAL)
    /** Indicates that data is finished with and ready for reuse.
     * This allows safe recycling when the data pointer is statically
     * allocated.
     *
     * \note This only exists when compiling without  DATA_IN_SIGNAL defined.
     */
    Boolean                         isConsumed;
#endif /* !DATA_IN_SIGNAL */

    /** With DATA_IN_SIGNAL this limits the data displayed by Genie.
     * Its value should be set to contain the value of
     * <tt> ((bitOffset+bitLength+7)/8). </tt>
     *
     * \note This also forces the next field to be word aligned as
     *       required by some Phy implementations.

\assoc ARRAY \ref data */
    Int16                           byteLength;

    /** Indicates an array of data containing transport blocks. The size of the
     * array varies with the product data class and includes padding octet(s)
     * per transport block, to ensure that the start of each transport block is
     * word aligned at the UPHY interface according to the configurable
     * parameter \c TRANSPORT_BLOCK_ALIGNMENT_IN_OCTETS.
     *
     * <TABLE>
     * <TR><TD><B>Data Class (kbps)</B></TD> <TD><B>Max bits per TTI</B></TD> <TD><B>Max blocks per TTI</B></TD> <TD><B>Padding bits</B></TD> <TD><B>Total bits</B></TD></TR>
     * <TR><TD>32                      </TD> <TD>640                </TD>     <TD>4                    </TD>     <TD>4*8=32         </TD>     <TD>672          </TD></TR>
     * <TR><TD>64                      </TD> <TD>3840               </TD>     <TD>8                    </TD>     <TD>8*8=64         </TD>     <TD>3904         </TD></TR>
     * <TR><TD>128                     </TD> <TD>3840               </TD>     <TD>8                    </TD>     <TD>8*8=64         </TD>     <TD>3904         </TD></TR>
     * <TR><TD>384                     </TD> <TD>6400               </TD>     <TD>16                   </TD>     <TD>16*8=128       </TD>     <TD>6528         </TD></TR>
     * </TABLE>
     */
    UDlPduListData                  data;

    /* The data array must be the *very* last element in this structure, so that
     * its size can be reduced to be as large as needed, not a default maximum;
     * it may not be allocated if no data is sent
     */
} UDlPduList;

/** Defines the start and end of each transport block (or RLC PDU) and its
 * associated data (transport channel and TFI).
 */
typedef struct UUlPduListInfoTag
{
    /** Indicates either UDCH (if associated with \c PhyDataReq primitive) or
     * URACH (if associated with \c PhyAccessReq primitive).
     */
    UTransportChannelType           trChType;

    /** Indicates a transport channel identity between MAC and PHY (and it
     * is in the range 1 to 32), but a radio bearer identity between RLC and
     * MAC (and is set to a value from the \c BearerIdentity enumeration). */
    Int8                            identity;

    /** Indicates an array of PDU bit offsets, from the end of the preceding
     * PDU in the list, where the most significant bit of used data starts, in
     * the range 0 to 255.
     */
    Int8                            bitOffset;

    /** Indicates an array of PDU bit lengths, in the range 0 to 65535. The bit
     * length for a transport block may only be zero in CELL_DCH. If this
     * transport channel has been specified as a channel requiring a quality
     * target, and no other transport channels have data to send, a "dummy TB"
     * may be specified to be transmitted by MAC, whose only useful information
     * is the CRC result.
     */
    Int16                           bitLength;

    /** Indicates the Transport Format Index in a Transport Format Set, and it
     * is in the range 0 to 31.
     */
    Int8                            transportFormatIndicator;
}
UUlPduListInfo;

/** Defines the uplink pdu data as an array of octets to hold uplink transport
 * blocks, allowing optional alignment padding per transport block, to ensure
 * that the start of each transport block is word aligned, so that the physical
 * layer can avoid bit copies.
 */
typedef Int8 UUlPduListData [UUL_MAX_TB_LIST_DATA_OCTETS];

/** Defines the transport block set and contains a list of one or more
 * contiguous transport blocks, where each transport block must start on a
 * 16-bit word aligned address. To minimise copying of data this data type is
 * also used for RLC PDUs on the MAC interface, so that only the offsets and
 * lengths need to be modified.
 *
 * The below figure illustrates the PDU list on its journey down the protocol
 * stack:
 *
 * \image html prd3guplinkpdulist.png "Fig: Example of PDU list going down the stack in the uplink"
 * \image latex prd3guplinkpdulist.eps "Example of PDU list going down the stack in the uplink" width=12cm
 *
 * \note the alignment padding between transport blocks may be 8-bit, 16-bit or
 * 32-bit, depending on hardware requirements. The protocol stack and L1 use
 * the configurable parameter <tt> TRANSPORT_BLOCK_ALIGNMENT_IN_OCTETS </tt> to
 * define alignment.
 */
typedef struct UUlPduListTag
{
    /** Indicates the number of transport blocks in this TTI.
\assoc ARRAY \ref pduListInfo */
    Int8                            numberOfTransportBlocks;

    /** Indicates the start and end of each transport block (or RLC PDU) and
     * its associated data (transport channel and TFI).
     */
    UUlPduListInfo                  pduListInfo [UPS_MAX_UL_NO_OF_TB_PER_TTI];

#if !defined (DATA_IN_SIGNAL)
    /** Indicates that data is finished with and ready for another write by the
     * creator. This is useful when the data pointer is statically allocated
     */
    Boolean                         isConsumed;
#endif /* !DATA_IN_SIGNAL */

    /* byteLength is to used as a Genie association, to enable it to display
     * only the used area of the data. For example, for a list containing two
     * PDU's, it would need to be set to :
     *        bitOffset[ 0 ] + bitLength[ 0 ] + bitOffset[ 1 ] + bitLength[ 1 ]
     * It is also used to copy the entire data block where required.
     */
    /** This member only exists when compiled for DATA_IN_SIGNAL, and is used
     * by Genie to limit the amount of data displayed by Genie; it does not
     * stop Genie from displaying data before the offset. Its value should be
     * set to contain the value of <tt> ((bitOffset+bitLength+7)/8). </tt>

\assoc ARRAY \ref data */
    Int16                           byteLength;

    /** Indicates an array of data containing transport blocks. The size of the
     * array varies with the product data class and includes padding octet(s)
     * per transport block, to ensure that the start of each transport block is
     * word aligned at the UPHY interface according to the configurable
     * parameter \c TRANSPORT_BLOCK_ALIGNMENT_IN_OCTETS.
     *
     * <TABLE>
     * <TR><TD><B>Data Class (kbps)</B></TD> <TD><B>Max bits per TTI</B></TD> <TD><B>Max blocks per TTI</B></TD> <TD><B>Padding bits</B></TD> <TD><B>Total bits</B></TD></TR>
     * <TR><TD>32                      </TD> <TD>640                </TD>     <TD>4                    </TD>     <TD>4*8=32         </TD>     <TD>672          </TD></TR>
     * <TR><TD>64                      </TD> <TD>3840               </TD>     <TD>8                    </TD>     <TD>8*8=64         </TD>     <TD>3904         </TD></TR>
     * <TR><TD>128                     </TD> <TD>3840               </TD>     <TD>8                    </TD>     <TD>8*8=64         </TD>     <TD>3904         </TD></TR>
     * <TR><TD>384                     </TD> <TD>6400               </TD>     <TD>16                   </TD>     <TD>16*8=128       </TD>     <TD>6528         </TD></TR>
     * </TABLE>
     */

    UUlPduListData                  data;

    /* The data array must be the *very* last element in this structure, so that
     * its size can be reduced to be as large as needed, not a default maximum;
     * it may not be allocated if no data is sent
     */
} UUlPduList;

/** Defines the TGPS parameters. */
typedef struct UTGPSActiveConfigParamsTag
{
    /** Indicates the CFN at which TGPS should be activated. */
    UTGCFN                          activateAtTgcfn;

    /** Indicates the Transmission Gap Patter Sequence Identifier. */
    UTGPSI                          tgpsi;

    /** Indicates the compressed mode configuration parameters. For more
     * information refer to <tt> [3G TS 25.331 section 10.3.6.33] </tt> */
    UTGPS_ConfigurationParams       compressedModeConfig;
}
UTGPSActiveConfigParams;

typedef struct UTGPSDeactiveConfigParamsTag
{
    /** Indicates if CFN at which TGPS deactivated is present. */
    Boolean                         deactivateAtTgcfnPresent;
    /** Indicates the CFN at which TGPS should be deactivated. */
    UTGCFN                          deactivateAtTgcfn;

    /** Indicates the Transmission Gap Patter Sequence Identifier. */
    UTGPSI                          tgpsi;
}
UTGPSDeactiveConfigParams;

/** Defines the UMTS operating mode . */
typedef enum UmtsModeTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Compliance with 3GPP FDD mode. */
    UMTS_FDD,
#endif
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Compliance with TDD128 mode. */
    UMTS_TDD128,
#endif
    UMTS_INVALID
}
UmtsMode;

#if defined (UPGRADE_3G)
/** Indicates FDD/TDD128 cell information*/
typedef union CellIdentificationUnionTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
   /** Indicates which FDD cell this data is from and is used by
    * RRC to determine if BCH data is from the serving or a neighbour cell and
    * it ranges from 0 to 511.
    */
   UPrimaryScramblingCode     primaryScramblingCode;
#endif
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
   /** Specifies the cell parameters Id of the cell for TDD128 */
   UCellParametersID          cellParametersId;
#endif
   /** To avoid 2G single mode build fail*/
   Int8                       dummy;
}CellIdentificationUnion;
#endif /* (UPGRADE_3G) */

typedef enum RatActivateModeTag
{
    NO_ACTIVATE_NEEDED,
    ACTIVATE_RAT,
    SWITCH_RAT
}
RatActivateMode;

#if defined(UPGRADE_3G_HSUPA)
/** SET0 in 25.212 section 4.8.4.1
 */
typedef enum EdchSfTag
{
    EDCH_SF_256,
    EDCH_SF_128,
    EDCH_SF_64,
    EDCH_SF_32,
    EDCH_SF_16,
    EDCH_SF_8,
    EDCH_SF_4,
    EDCH_SF_4x2,
    EDCH_SF_2x2,
    EDCH_SF_4x2_2x2,
    EDCH_NUM_SF,
    EDCH_SF_INVALID
}
EdchSf;
#endif /*UPGRADE_3G_HSUPA*/

#endif  /* U1_TYP_H */


/* END OF FILE */
