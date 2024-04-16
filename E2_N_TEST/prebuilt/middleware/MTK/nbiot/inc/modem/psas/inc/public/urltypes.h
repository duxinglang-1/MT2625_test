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
 *
 *    Type definitions for URLC which may be required in all URLC modules.
 **************************************************************************/

#if !defined (URLTYPES_H)
#define       URLTYPES_H

/***************************************************************************
* Nested Include Files
***************************************************************************/

#include <system.h>
#include <ups_typ.h>
#include <l3_typ.h>     /* Needed for TcUmtsTestLoopMode in UrlLoopbackInfo */
#include <ki_typ.h>

#if defined (UPGRADE_GENERIC_MUX)
#include <ps_offsets.h>
#endif /* UPGRADE_GENERIC_MUX */

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/* Bearer identity masks used on UMAC_DATA_IND */
#define URL_BEARER_PROCESSED_MASK          0x80
#define URL_BEARER_ID_MASK                 0x7F

/* PDU element sizes */
#define URL_UM_PDU_HEADER_BITS             8    /* Size of UM PDU header
                                                 * (bits)
                                                 */
#define URL_UM_PDU_HEADER_OCTETS           1    /* Size of UM PDU header
                                                 * (octets)
                                                 */
#define URL_UM_SN_BITS                     7    /* Size of UM sequence numbers
                                                 * (bits)
                                                 */
#define URL_UM_SN_MODULUS                  128  /* UM sequence number modulus:
                                                 *      7 bits=0-127
                                                 */
#if defined (UPGRADE_3G_MBMS) && defined (UPGRADE_3G_TDD128)
#define URL_UM_SN_MODULUS_MINUS_ONE        127  /* UM sequence number modulus minus one:
                                                 *      7 bits=0-126
                                                 */
#endif /* UPGRADE_3G_MBMS && UPGRADE_3G_TDD128 */
#define URL_AM_PDU_DATA_HEADER_OCTETS      2    /* Size of control PDU header
                                                 * (octets)
                                                 */
#define URL_AM_PDU_DATA_HEADER_BITS        16   /* Size of control PDU header
                                                 * (bits)
                                                 */
#define URL_AM_PDU_CONTROL_HEADER_BITS     4    /* Size of control PDU header
                                                 * (bits)
                                                 */
#define URL_AM_SN_BITS                     12    /* Size of AM sequence numbers
                                                 * (bits)
                                                 */
#define URL_AM_SN_MODULUS                  4096 /* AM sequence number modulus:
                                                 *      12 bits=0-4095
                                                 */
#define URL_AM_SN_MODULUS_MINUS_ONE        4095 /* AM sequence number modulus minus one:
                                                 *      12 bits=0-4094
                                                 */
#define URL_UM_HFNI_BITS                   25   /* Size of HFNI field (bits) */
#define URL_AM_HFNI_BITS                   20   /* Size of HFNI field (bits) */

/* PDU header manipulation */
#define URL_PDU_E_BITS                     1
#define URL_PDU_E_MASK                     0x01
#define URL_PDU_LI7_MASK                   0xFE
#define URL_PDU_LI15_HIGH_MASK             0xFF
#define URL_PDU_LI15_LOW_MASK              URL_PDU_LI7_MASK

/* Length indicators sizes */
#define URL_PDU_LI7_BITS                   7
#define URL_PDU_LI7_OCTETS                 BITS_TO_INT8S (URL_PDU_LI7_BITS)
#define URL_PDU_LI15_BITS                  15
#define URL_PDU_LI15_OCTETS                BITS_TO_INT8S (URL_PDU_LI15_BITS)

/* PDU length indicator values */
#define URL_PDU_E_LI                       1                /* More LI's */
#define URL_PDU_E_DATA                     0                /* Last LI */
#define URL_PDU_LI7_EXACT                  (Int8 )  0x00
#define URL_PDU_LI15_EXACT                 (Int16)0x0000
#define URL_PDU_LI15_ONE_SHORT             (Int16)0x7FFB
#define URL_PDU_LI7_START                  (Int8 )  0x7C
#define URL_PDU_LI15_START                 (Int16)0x7FFC
#define URL_PDU_LI7_RESERVED               (Int8 )  0x7D
#define URL_PDU_LI15_RESERVED              (Int16)0x7FFD
#define URL_PDU_LI7_PIGGYBACK_STATUS       (Int8 )  0x7E
#define URL_PDU_LI15_PIGGYBACK_STATUS      (Int16)0x7FFE
#define URL_PDU_LI7_PAD                    (Int8 )  0x7F
#define URL_PDU_LI15_PAD                   (Int16)0x7FFF

#define URL_PDU_MINIMUM_USABLE_OCTETS      1 /* Minimum number of octets for a
                                             * PDU to be usable
                                             */
#define URL_SDU_ONE_SHORT_OCTETS           -1 /* Size of remaining SDU data that
                                             * causes the use of the ONE_SHORT
                                             * length indicator
                                             */

/* Maximum value for AM SUFI lengths */
#define URL_AM_SUFI_MAX_LIST_LENGTH        15 /* Max number of (SN, L) entries
                                             * in a LIST SUFI
                                             *  0    = Illegal
                                             *  1-15 = 1-15 entries
                                             */
#define URL_AM_SUFI_MAX_BITMAP_LENGTH      16 /* Max number of octets in a
                                             * BITMAP SUFI
                                             *  0-15 = 1-16 octets
                                             */
#define URL_AM_SUFI_MAX_RLIST_LENGTH       15 /* Max number of CW's in a
                                             * RLIST SUFI:
                                             *  0    = Only FSN is present
                                             *  1-15 = 1-15 CW's
                                             */
#define URL_AM_SUFI_MAX_MRW_LENGTH         15 /* Max number of SN_MWR's in a
                                             * MRW SUFI
                                             *  0    = 1 SN_MRW, where the
                                             *         discarded SDU extends
                                             *         above configured tx
                                             *         window
                                             *  1-15 = 1-15 SN_MRW's
                                             */

/* Macros to define the sizes of AM control PDU bodies, excuding the header */
#define URL_AM_SUFI_NO_MORE_BITS           (URL_AM_SUFI_TYPE_BITS)
#define URL_AM_SUFI_WINDOW_BITS \
    ((URL_AM_SUFI_TYPE_BITS) +  \
     (URL_AM_SUFI_SN_BITS))
#define URL_AM_SUFI_ACK_BITS   \
    ((URL_AM_SUFI_TYPE_BITS) + \
     (URL_AM_SUFI_SN_BITS))
#define URL_AM_SUFI_LIST_BITS(len)     \
    ((URL_AM_SUFI_TYPE_BITS) +         \
     (URL_AM_SUFI_LENGTH_BITS) +       \
     ((len) * ((URL_AM_SUFI_SN_BITS) + \
               (URL_AM_SUFI_L_BITS)    \
              )                        \
     )                                 \
    )
#define URL_AM_SUFI_BITMAP_BITS(len)       \
    ((URL_AM_SUFI_TYPE_BITS) +             \
     (URL_AM_SUFI_LENGTH_BITS) +           \
     (URL_AM_SUFI_SN_BITS) +               \
     (BITS_TO_INT8S (len) * BITS_PER_INT8) \
    )
#define URL_AM_SUFI_RLIST_BITS(len)                                      \
    ((URL_AM_SUFI_TYPE_BITS) +                                           \
     (URL_AM_SUFI_LENGTH_BITS) +                                         \
     (URL_AM_SUFI_SN_BITS) +                                             \
     ((len) *                                                            \
                                          ((URL_AM_SUFI_CW_VALUE_BITS) + \
                                           (URL_AM_SUFI_CW_E_BITS)       \
                                          )                              \
     )                                                                   \
    )
#define URL_AM_SUFI_MRW_BITS(len)                                           \
    ((URL_AM_SUFI_TYPE_BITS) +                                              \
     (URL_AM_SUFI_LENGTH_BITS) +                                            \
     (URL_AM_SUFI_N_BITS) +                                                 \
     (((len) == 0)                                                          \
                                          ? (URL_AM_SUFI_SN_BITS)           \
                                          : ((len) * (URL_AM_SUFI_SN_BITS)) \
     )                                                                      \
    )
#define URL_AM_SUFI_MRW_ACK_BITS \
    ((URL_AM_SUFI_TYPE_BITS) +   \
     (URL_AM_SUFI_N_BITS) +      \
     (URL_AM_SUFI_SN_BITS)       \
    )
#define URL_AM_RESET_BITS                  (URL_AM_HFNI_BITS)
#define URL_AM_RESET_ACK_BITS              (URL_AM_HFNI_BITS)

/* Maximum data length values (one less than lowest possible LI value) */
#define URL_PDU_MAX_DATA_OCTETS_LI7        ((URL_PDU_LI7_START)-1)
#define URL_PDU_MAX_DATA_OCTETS_LI15       ((URL_PDU_LI15_ONE_SHORT)-1)

/* Maximum allowable PDU sizes */
#define URL_UM_MAX_7_BIT_PDU_OCTETS        125
#define URL_UM_MAX_15_BIT_PDU_OCTETS \
    ((URL_UM_PDU_HEADER_OCTETS) +    \
     (URL_PDU_LI15_OCTETS) +         \
     (URL_PDU_MAX_DATA_OCTETS_LI15)  \
    )
#define URL_AM_MAX_7_BIT_PDU_OCTETS        126
#define URL_AM_MAX_15_BIT_PDU_OCTETS                        \
    ((BITS_TO_INT8S (URL_AM_PDU_DATA_HEADER_BITS)) +        \
     (URL_PDU_LI15_OCTETS) + (URL_PDU_MAX_DATA_OCTETS_LI15) \
    )

/* Maximum size of a control PDU data is defined by the maximum size of the
 * largest possible control PDU type, which, in Release 99 of 25.322, is the
 * LIST SUFI
 */
#define URL_AM_CTRL_MAX_DATA_OCTETS                                    \
    BITS_TO_INT8S (URL_AM_PDU_CONTROL_HEADER_BITS +                    \
                   URL_AM_SUFI_LIST_BITS (URL_AM_SUFI_MAX_LIST_LENGTH) \
                  )

/* When a bit aligned PDU needs octet aligning, it needs to be bit copied into
 * a buffer which is large enough to hold all octets of the unaligned data +
 * two extra octet for aligment, then copy the data into into the 2nd octet
 * of the buffer, so that it can be aligned into the 1st octet
 */
#define URL_BIT_ALIGNMENT_SAFETY_ZONE      2

/* Maximum number of cipher configurations per bearer */
#define URL_MAX_CIPHER_CONFIGS             3

/***************************************************************************
*   Macro Functions
***************************************************************************/

/* PDU single octet field access macros */
#define URL_PDU_E_GET(octet)                (octet & URL_PDU_E_MASK)
#define URL_PDU_E_SET(octet, e)  \
    (octet & URL_PDU_LI7_MASK) | \
    (e ? URL_PDU_E_LI : URL_PDU_E_DATA)
#define URL_UM_PDU_SN_GET(octet)            (octet >> 1)
#define URL_AM_PDU_SN_GET(word)             ((word >> 3) & 0x0FFF)

#define URL_UM_NEXT_SN(sn)                  ((sn + 1) % URL_UM_SN_MODULUS)
#define URL_UM_PREV_SN(sn)                  ((sn + URL_UM_SN_MODULUS - 1) % URL_UM_SN_MODULUS)
#define URL_AM_NEXT_SN(sn)                  ((sn + 1) % URL_AM_SN_MODULUS)
#define URL_AM_PREV_SN(sn)                  ((sn + URL_AM_SN_MODULUS - 1) % URL_AM_SN_MODULUS)

/* Constant which defines an illegal sequence number */
#define URL_SN_UNDEFINED     0xFFFF

/* Macros to access the userId field of timers, which has the following
 * structure:
 *   MSB                                                        LSB
 *   15  14  13  12  11  10  9   8   7   6   5   4   3   2   1   0
 * |-------------------------------|---------------|---------------|
 * |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * |-------------------------------|---------------|---------------|
 * |Unused |    Bearer identity    |   Optional    |Timer identity |
 *                                    Timer data
 *
 * Timers belonging to UrlMain will have Bearer identity == RB_NUM_BEARERS
 */
#define URL_TIMER_SET(userId, b, t, d) \
    (userId) = (KiTimerUserValue)      \
               (                       \
        ((b & 0xFF) << 8) |            \
        ((d & 0x0F) << 4) |            \
        (t & 0x0F)                     \
               )
#define URL_TIMER_BEARER_GET(userId)        ((userId >> 8) & 0xFF)
#define URL_TIMER_DATA_SET(userId, d) \
    (userId) = (userId & 0xFF0F) |    \
               ((d & 0x0F) << 4)
#define URL_TIMER_DATA_GET(userId)          ((userId >> 4) & 0x000F)
#define URL_TIMER_ID_SET(userId, t) \
    (userId) = (userId & 0xFFF0) |  \
               (t & 0x0F)
#define URL_TIMER_ID_GET(userId)            (userId & 0x000F)

/* UM hyper rate Frame Number (HFN) */
#define URL_UM_HFN_MODULUS     0x02000000           /* 25 bits */
#define URL_UM_NEXT_HFN(hfn)                ((hfn + 1) % (URL_UM_HFN_MODULUS))
#define URL_UM_PREV_HFN(hfn)              \
    (((hfn) + (URL_UM_HFN_MODULUS - 1)) % \
     (URL_UM_HFN_MODULUS))

/* As a UM COUNT-C is HFN=25 MSB's/SN=7 LSB's and a startValue must set the 20
 * MSB's of the HFN, then to convert startValue into a UM HFN, it needs to be
 * shifted << 32-(20-7)=5, with LSB's of HFN set to 0
 */
#define URL_UM_START_TO_HFN(startValue)     ((UCOUNT_C)(startValue) << 5)

/* AM hyper rate Frame Number (HFN) */
#define URL_AM_HFN_MODULUS     0x0100000             /* 20 bits */
#define URL_AM_NEXT_HFN(hfn)                ((hfn + 1) % (URL_AM_HFN_MODULUS))
#define URL_AM_PREV_HFN(hfn)              \
    (((hfn) + (URL_AM_HFN_MODULUS - 1)) % \
     (URL_AM_HFN_MODULUS))

/* As a AM COUNT-C is HFN=20 MSB's/SN=12 LSB's and a startValue must set the 20
 * MSB's of the HFN, then startValue translates directly into HFN
 */
#define URL_AM_START_TO_HFN(startValue)     (UCOUNT_C)(startValue)

/* Constant which defines when a HFN is not defined by a cipher configiration */
#define URL_HFN_UNDEFINED      0xFFFFFFFF

/* COUNT-C, made from HFN and SN */
#define URL_UM_COUNT_C(hfn, sn)        \
    (((UCOUNT_C)(hfn) & 0x01FFFFFF) << \
     URL_UM_SN_BITS) | (sn)
#define URL_UM_GET_COUNT_C_HFN(countC) \
    ((UrlHyperFrameNumber)(countC) >>  \
     URL_UM_SN_BITS)
#define URL_UM_GET_COUNT_C_SN(countC)       ((UrlSequenceNumber)(countC) & 0x07F)
#define URL_AM_COUNT_C(hfn, sn)        \
    (((UCOUNT_C)(hfn) & 0x000FFFFF) << \
     URL_AM_SN_BITS) | (sn)
#define URL_AM_GET_COUNT_C_HFN(countC) \
    ((UrlHyperFrameNumber)(countC) >>  \
     URL_AM_SN_BITS)
#define URL_AM_GET_COUNT_C_SN(countC)       ((UrlSequenceNumber)(countC) & 0xFFF)

/* Mask defining which COUNT-C bits are significant to URRC, so that any COUNT-C
 * change which alters these bits should be informed to URRC in a CrlcCountCInd
 */
#define URL_COUNT_C_IND_MASK            0xFFFFFF80

/* Definitions of bits within the UrlDataSegmentType octet :
 *        MSB                                                     LSB
 *     |-------------------------------|-------------------------------|
 *     |   7   |   6   |   5   |   4   |   3   |   2   |   1   |   0   |
 *     |-------------------------------|-------------------------------|
 *     | LI 15 |Unused |  Pad  |Status |Length | Short | Exact | Start |
 *
 *  where :
 *
 *      Start   : START LI present flag
 *      Exact   : EXACT LI present flag
 *      Short   : ONE SHORT LI present flag
 *      Length  : Length LI present flag
 *      Status  : Piggyback status present flag
 *      Pad     : Pad LI present flag
 *      LI 15   : 0 =  7 bit LI's, 1 = 15 bit LI's
 */
#define URL_SEG_LI_START                0x01
#define URL_SEG_LI_EXACT                0x02
#define URL_SEG_LI_ONE_SHORT            0x04
#define URL_SEG_LI_LENGTH               0x08
#define URL_SEG_LI_PIGGYBACK_STATUS     0x10
#define URL_SEG_LI_PAD                  0x20
#define URL_SEG_LI_RESERVED             0x40
#define URL_SEG_LI_7                    0x00
#define URL_SEG_LI_15                   0x80

/* Special values of UrlDataSegmentType */
#define URL_SEG_TYPE_UNKNOWN            0xFF    /* PDU not parsed yet */
#define URL_SEG_TYPE_ERROR                                                 \
    (URL_SEG_LI_PAD | URL_SEG_LI_PIGGYBACK_STATUS | URL_SEG_LI_ONE_SHORT | \
     URL_SEG_LI_EXACT | URL_SEG_LI_START)

/* Values for throughput statistics */
#define BITS_PER_128INT8                (BITS_PER_INT8 * 128)
#define BITS_PER_256INT8                (BITS_PER_INT8 * 256)
#define BITS_PER_512INT8                (BITS_PER_INT8 * 512)
#define BITS_PER_1024INT8               (BITS_PER_INT8 * 1024)

/***************************************************************************
 * Types
 ***************************************************************************/

/** Used to identify a message */
typedef Int16 UrlcMessageUnitIdentifier;

/** Indicates RLC SN  */
typedef Int16 UrlSequenceNumber;
typedef Int16 UrlLengthIndicator;
typedef Int32 UrlHyperFrameNumber;
typedef Int8 UrlDataSegmentType;                                /* See above */

/* Loopback information */
typedef struct UrlLoopbackInfoTag
{
    Int8  numberOfSduSizes;                                 /* 1-MAX_UL_TF */
#if defined (UPGRADE_3G_MBMS) && defined (UPGRADE_3G_TDD128)
    TcUmtsTestLoopMode  loopMode;
    Int32               loopMode3SduCount; /* SDU counter for loopback MODE_3 */ 
#endif /* UPGRADE_3G_MBMS && UPGRADE_3G_TDD128 */
    Int16 sduBits [UPS_MAX_UL_TF];                      /* 0-65535 */
} UrlLoopbackInfo;

/* Note that to avoid comparison problems, the SN and HFN of the first enty must
 * track the bottom of the window VT(A)/VR(R)
 */
typedef struct UrlCipherConfigTag
{
    KeySeqId ksi;                           /* Cipher key identity :
                                             *   Bits 0-3 : 0-6 = KSI
                                             *                7 = UPS_NO_KEY_SET
                                             *   Bit    4 : 0 = CS, 1 = PS
                                             *
                                             *   0xFF = UPS_INVALID_KSI
                                             */
    UCipheringAlgorithm_r7 algorithm;       /* Algorithm UEA0, UEA1 or UEA2 */
    UrlSequenceNumber      sn;              /* SN when key becomes active */
    UrlHyperFrameNumber    hfn;             /* HFN that starts at SN or
                                             * URL_HFN_UNDEFINED
                                             */
} UrlCipherConfig;

/* Type for an array of cipher configurations */
typedef UrlCipherConfig UrlCipherConfigList [URL_MAX_CIPHER_CONFIGS];

/* Structure to store the COUNT-C and KSI of the youngest PDU */
typedef struct UrlCountcInfoTag
{
    UCOUNT_C countC;                    /* COUNT-C */
    KeySeqId ksi;                       /* Cipher key identity */
} UrlCountcInfo;

/* As the maximum number of LIs per PDU is not specified, this structure allows
 * optimisation of DL data PDU parsing, by avoiding the need to extract the LIs
 * twice (once to count them and again to process them) without a prohibitively
 * large array; once the initial array is full, another linked array can be
 * allocated ...
 */
typedef struct UrlLengthIndicatorListTag
{
    UrlLengthIndicator li [URL_AM_SUFI_MAX_LIST_LENGTH];
    struct UrlLengthIndicatorListTag * next_p;
} UrlLengthIndicatorList;

#if defined (ENABLE_UPLANE_STATISTICS)

/* SDU size counters for throughput statistics
 */
typedef struct UrlSduSizeCountersTag
{
    /* sum of SDU sizes (bytes) this value must be stored in bytes to avoid
     * wrapping
     */
    Int32 sumSize;
    Int32 minSduBits;               /* smallest SDU size (bits) */
    Int32 maxSduBits;               /* largest SDU size (bits) */
    Int32 numSdusSize0000To0127;    /* num. SDUs with 0<=size<128 bytes */
    Int32 numSdusSize0128To0255;    /* num. SDUs with 128<=size<256 bytes */
    Int32 numSdusSize0256To0511;    /* num. SDUs with 256<=size<512 bytes */
    Int32 numSdusSize0512To1023;    /* num. SDUs with 512<=size<1024 bytes */
    Int32 numSdusSize1024OrMore;    /* num. SDUs with 1024<=size bytes */
} UrlSduSizeCounters;

/* PDU discard counters for throughput statistics
 */
typedef struct UrlPduDiscardCountersTag
{
    Int32 numDuplicatedPdus;            /* num. duplicated PDUs */
    Int32 numInconsistentPdus;          /* num. parse error PDUs */
    Int32 numOtherDiscardedPdus;        /* num. other error PDUs */
} UrlPduDiscardCounters;

/* Status PDU statistics
 */
typedef struct UrlStatusPduStatisticsTag
{
    Int32 numStatusPdus;     /* num. status PDUs */
    Int32 listSufiCount;     /* num. LIST SUFIs */
    Int32 bitmapSufiCount;   /* num. BITMAP SUFIs */
    Int32 rlistSufiCount;    /* num. RLIST SUFIs */
    Int32 mrwSufiCount;      /* num. MRW SUFIs */
    Int32 mrwAckSufiCount;   /* num. MRW-ACK SUFIs */
    Int32 nackdSnCount;      /* num. NACKd SNs */
} UrlStatusPduStatistics;

/* PDU counters for throughput statistics
 */
typedef struct UrlPduStatisticsTag
{
    UrlStatusPduStatistics statusPdus;
    Int32                  numDataPdus;
    Int32                  numResetPdus;
    Int32                  numResetAckPdus;
    Int32                  numStatusPaddingPdus;
    Int32                  maxLis;             /* max LIs per PDU */
    Int32                  pollReqCount;
} UrlPduStatistics;

/* uplink statistics values
 */
typedef struct UrlUplinkStatisticsTag
{
    KernelTicks        startTick;               /* time since establishment */
    UrlSduSizeCounters sduSize;
    UrlPduStatistics   pduStats;
    Int32              numDiscardedSdus;        /* discarded SDUs */
} UrlUplinkStatistics;

/* downlink statistics values
 */
typedef struct UrlDownlinkStatisticsTag
{
    KernelTicks           startTick;      /* time since establishment */
    UrlSduSizeCounters    sduSize;
    UrlPduStatistics      pduStats;
    UrlPduDiscardCounters discardedPdus;   /* only needed for downlink */
    Int32                 reestablishmentCount;   /* reestablishments */
} UrlDownlinkStatistics;
#endif /* ENABLE_UPLANE_STATISTICS */

/***************************************************************************
 * Typed Constants
 ***************************************************************************/

/* Bearer release cause */
typedef enum UrlBearerReleaseCauseTag
{
    URL_BEARER_RELEASE = 0,
    URL_BEARER_RE_ESTABLISHMENT = 1
} UrlBearerReleaseCause;

/* Function usage */
typedef enum UrlUsageTag
{
    URL_USAGE_FOREGROUND = 0,                       /* Processing a signal */
    URL_USAGE_BACKGROUND = 1                        /* Low priority background
                                                     * operation
                                                     */
} UrlUsage;

#endif /* URLTYPES_H */
/* END OF UrlTypes.H */
