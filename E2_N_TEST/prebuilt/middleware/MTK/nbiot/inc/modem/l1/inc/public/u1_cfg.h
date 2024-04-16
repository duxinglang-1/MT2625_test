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
 * 3G L1 Configuration definitions for the UPHY Interface
 *************************************************************************/

#if !defined (U1_CFG_H)
#define       U1_CFG_H

#include <system.h>
/***************************************************************************
* Nested Include Files
***************************************************************************/

/***************************************************************************
*   Manifest Constants
***************************************************************************/

/** Defines GSM L1 physical block size in bytes. */
#define CPHY_GSM_L1_PHYSICAL_BLOCK_SIZE       23

#if defined   (UPS_CFG_DL_DATA_CLASS_32KBPS)
/** Defines the maximum sum of number of bits of all transport blocks being
 * received at an arbitrary time instant.
 */
# define UPS_MAX_DL_TB_BITS                     640
/** Defines the maximum total number of transport blocks received within TTIs
 * that end at the same time.
 */
# define UPS_MAX_DL_NO_OF_TB_PER_TTI            8
/** Defines the maximum number of simultaneous transport channels available on
 * the downlink; and it does not contain the BCH of the neighbour cell.
 */
# define UPS_MAX_DL_NO_OF_TRCH                  8
/** Defines the maximum number of transport format combinations available on
 * the downlink.
 */
# define UPS_MAX_DL_TFC                         32
/** Defines the maximum number of transport formats available on the downlink.
 */
# define UPS_MAX_DL_TF                          32
/** Defines the maximum number of AM entities that could exist. */
# define UPS_MAX_AM_ENTITIES                    4

#elif defined (UPS_CFG_DL_DATA_CLASS_64KBPS)
/** Defines the maximum sum of number of bits of all transport blocks being
 * received at an arbitrary time instant.
 */
# define UPS_MAX_DL_TB_BITS                     3840
/** Defines the maximum total number of transport blocks received within TTIs
 * that end at the same time.
 */
# define UPS_MAX_DL_NO_OF_TB_PER_TTI            8
/** Defines the maximum number of simultaneous transport channels available on
 * the downlink; and it does not contain the BCH of the neighbour cell.
 */
# define UPS_MAX_DL_NO_OF_TRCH                  8
/** Defines the maximum number of transport format combinations available on
 * the downlink.
 */
# define UPS_MAX_DL_TFC                         48
/** Defines the maximum number of transport formats available on the downlink.
 */
# define UPS_MAX_DL_TF                          64
/** Defines the maximum number of AM entities that could exist. */
# define UPS_MAX_AM_ENTITIES                    4

#elif defined (UPS_CFG_DL_DATA_CLASS_128KBPS)
/** Defines the maximum sum of number of bits of all transport blocks being
 * received at an arbitrary time instant.
 */
# define UPS_MAX_DL_TB_BITS                     3840
/** Defines the maximum total number of transport blocks received within TTIs
 * that end at the same time.
 */
# define UPS_MAX_DL_NO_OF_TB_PER_TTI            16
/** Defines the maximum number of simultaneous transport channels available on
 * the downlink; and it does not contain the BCH of the neighbour cell.
 */
# define UPS_MAX_DL_NO_OF_TRCH                  8
/** Defines the maximum number of transport format combinations available on
 * the downlink.
 */
# define UPS_MAX_DL_TFC                         96
/** Defines the maximum number of transport formats available on the downlink.
 */
# define UPS_MAX_DL_TF                          64
/** Defines the maximum number of AM entities that could exist. */
# define UPS_MAX_AM_ENTITIES                    5

#else /* 384KBPS */
/** Defines the maximum sum of number of bits of all transport
 * blocks being received at an arbitrary time instant.
 */
#if defined (UPGRADE_3G_TDD128)
# define UPS_MAX_DL_TB_BITS                     8960
#else
# if defined (UPGRADE_3G_FDD)
#  if defined(PLATFORM_G2DSP) || defined (PLATFORM_G2MCU)
#   define UPS_MAX_DL_TB_BITS                     8960
#  else
/** Defines UPS_MAX_DL_TB_BITS for G1 platform */
#   define UPS_MAX_DL_TB_BITS                     6400
#  endif
# else /* XXX: WORKAROUND, for ml1dev build */
#  if defined(PLATFORM_G2DSP) || defined (PLATFORM_G2MCU)
#   define UPS_MAX_DL_TB_BITS                     8960
#  endif
# endif
#endif


/** Defines the maximum total number of transport blocks received
 * within TTIs that end at the same time.
 */
# define UPS_MAX_DL_NO_OF_TB_PER_TTI            32
/** Defines the maximum number of simultaneous transport channels available on
 * the downlink; and it does not contain the BCH of the neighbour cell.
 */
# define UPS_MAX_DL_NO_OF_TRCH                  8
/** Defines the maximum number of transport format combinations available on
 * the downlink.
 */
# define UPS_MAX_DL_TFC                         128
/** Defines the maximum number of transport formats available on the downlink.
 */
# define UPS_MAX_DL_TF                          64
/** Defines the maximum number of AM entities that could exist. */
# define UPS_MAX_AM_ENTITIES                    6
#endif /* UPS_CFG_DL_DATA_CLASS_? */


#if defined   (UPS_CFG_UL_DATA_CLASS_32KBPS)
/** Defines the maximum number of uplink physical channels. */
# define UPS_MAX_UL_PHY_CH                      1
/** Defines the minimum spreading factor. */
# define UPS_MIN_SPREADING_FACTOR               32
/** Defines the maximum sum of number of bits of all transport
 * blocks being transmitted at an arbitrary time instant.
 */
# define UPS_MAX_UL_TB_BITS                     640
/** Defines the maximum total number of transport blocks transmitted within
 * TTIs that start at the same time in the uplink.
 */
# define UPS_MAX_UL_NO_OF_TB_PER_TTI            4
/** Defines the maximum number of simultaneous transport channels on the
 * uplink.
 */
# define UPS_MAX_UL_NO_OF_TRCH                  4
/** Defines the maximum number of tranport format combinations available on the
 * uplink.
 */
# define UPS_MAX_UL_TFC                         16
/** Defines the maximum number of tranport formats available on the uplink. */
# define UPS_MAX_UL_TF                          32

#elif defined (UPS_CFG_UL_DATA_CLASS_64KBPS)
# if defined (UPS_CFG_DL_DATA_CLASS_32KBPS)
# error Downlink data class must be >= Uplink data class
# endif /* UPS_CFG_DL_DATA_CLASS_32KBPS */
/** Defines the maximum number of uplink physical channels. */
# define UPS_MAX_UL_PHY_CH                      1
/** Defines the minimum spreading factor. */
# define UPS_MIN_SPREADING_FACTOR               16
/** Defines the maximum sum of number of bits of all transport blocks being
 * transmitted at an arbitrary time instant.
 */
# define UPS_MAX_UL_TB_BITS                     3840
/** Defines the maximum total number of transport blocks transmitted within
 * TTIs that start at the same time in the uplink.
 */
# define UPS_MAX_UL_NO_OF_TB_PER_TTI            8
/** Defines the maximum number of simultaneous transport channels on the uplink.
 */
# define UPS_MAX_UL_NO_OF_TRCH                  8
/** Defines the maximum number of tranport format combinations available on the
 * uplink.
 */
# define UPS_MAX_UL_TFC                         32
/** Defines the maximum number of tranport formats available on the uplink. */
# define UPS_MAX_UL_TF                          32

#elif defined (UPS_CFG_UL_DATA_CLASS_128KBPS)
# if defined (UPS_CFG_DL_DATA_CLASS_32KBPS) || defined (UPS_CFG_DL_DATA_CLASS_64KBPS)
# error Downlink data class must be >= Uplink data class
# endif /* UPS_CFG_DL_DATA_CLASS_32KBPS || UPS_CFG_DL_DATA_CLASS_64KBPS */
/** Defines the maximum number of uplink physical channels. */
# define UPS_MAX_UL_PHY_CH                      1
/** Defines the minimum spreading factor. */
# define UPS_MIN_SPREADING_FACTOR               8
/** Defines the maximum sum of number of bits of all transport
 * blocks being transmitted at an arbitrary time instant.
 */
# define UPS_MAX_UL_TB_BITS                     3840
/** Defines the maximum total number of transport blocks transmitted within
 * TTIs that start at the same time in the uplink.
 */
# define UPS_MAX_UL_NO_OF_TB_PER_TTI            8
/** Defines the maximum number of simultaneous transport channels on the
 * uplink.
 */
# define UPS_MAX_UL_NO_OF_TRCH                  8
/** Defines the maximum number of tranport format combinations available on the
 * uplink.
 */
# define UPS_MAX_UL_TFC                         32
/** Defines the maximum number of tranport formats available on the uplink. */
# define UPS_MAX_UL_TF                          32

#else /* 384KBPS */
# if defined (UPS_CFG_DL_DATA_CLASS_32KBPS) || defined (UPS_CFG_DL_DATA_CLASS_64KBPS) || \
     defined (UPS_CFG_DL_DATA_CLASS_128KBPS)
# error Downlink data class must be >= Uplink data class
# endif /* UPS_CFG_DL_DATA_CLASS_? */
/** Defines the maximum number of uplink physical channels. */
# define UPS_MAX_UL_PHY_CH                      1
/** Defines the minimum spreading factor. */
# define UPS_MIN_SPREADING_FACTOR               4
/** Defines the maximum sum of number of bits of all transport blocks being
 * transmitted at an arbitrary time instant.
 */
# define UPS_MAX_UL_TB_BITS                     8960

/** Defines the maximum total number of transport blocks transmitted within
 * TTIs that start at the same time in the uplink.
 */
# define UPS_MAX_UL_NO_OF_TB_PER_TTI            32

/** Defines the maximum number of simultaneous transport channels on the uplink.
 */
# define UPS_MAX_UL_NO_OF_TRCH                  8
/** Defines the maximum number of tranport format combinations available on the
 * uplink.
 */
# define UPS_MAX_UL_TFC            128

/** Defines the maximum number of tranport formats available on the uplink. */
# define UPS_MAX_UL_TF            64
#endif /* UPS_CFG_UL_DATA_CLASS_? */

#if !defined (UPS_TRANSPORT_BLOCK_ALIGNMENT_IN_OCTETS)
/** Defines how the start of each transport block must be aligned.
 * This has the following options:
 *     - 0 = No alignment
 *     - 1 = Byte aligned
 *     - 2 = Word aligned
 *     - 4 = Longword aligned
 */
#define UPS_TRANSPORT_BLOCK_ALIGNMENT_IN_OCTETS 2
#endif /* UPS_TRANSPORT_BLOCK_ALIGNMENT_IN_OCTETS */

#if !defined (UPS_MAX_RADIO_LINKS)
/** Defines the maximum number of radio links that can be supported in L1. */
# define UPS_MAX_RADIO_LINKS                    6  /* Range of 6 to 8 */
#endif /* UPS_MAX_RADIO_LINKS */

#ifdef UPGRADE_3G_HSUPA
#ifdef UPGRADE_3G_FDD
#if !defined (UPS_MAX_E_DCH_RADIO_LINKS)
/** Defines the maximum number of E-DCH radio links that can be supported in L1.
 *  The Spec. 25331 sub-caluse 10.3.10: maxEDCHRL '4'
 */
# define UPS_MAX_E_DCH_RADIO_LINKS              4
#endif /* UPS_MAX_E_DCH_RADIO_LINKS */
#endif /* UPGRADE_3G_FDD */
#endif /* UPGRADE_3G_HSUPA */

/* Maximum number of intra-freq cells the physical layer can measure */
#if !defined (UPS_MAX_MEASURED_INTRA_FREQ_CELLS)
/** Defines the maximum number of intra-frequency cells that can be measured by
 * L1.
 */
#if defined (UPGRADE_MEASUREMENT_CAPABILITY_FROM_8CELL_TO_16CELL)
/* RRC will always keep the measure buffer of the active cells even though
 * L1 had no loner measured on it. So L1 may reports 16 cells which are
 * not in active set.
 */
# define UPS_MAX_MEASURED_INTRA_FREQ_CELLS      (16 + UPS_MAX_RADIO_LINKS)
#else
/* RRC will always keep the measure buffer of the active cells even though
 * L1 had no loner measured on it. So L1 may reports 8 cells which are
 * not in active set.
 */
# define UPS_MAX_MEASURED_INTRA_FREQ_CELLS      (8 + UPS_MAX_RADIO_LINKS)
#endif /* UPGRADE_MEASUREMENT_CAPABILITY_FROM_8CELL_TO_16CELL */
#endif /* UPS_MAX_MEASURED_INTRA_FREQ_CELLS */

/** Defines the maximum CRC size in octets.
 * 24 comes from the maximum of UCRC_Size enum in rrcXXX_asn.h */
#define UPS_MAX_CRC_SIZE_IN_OCTETS          BITS_TO_INT8S(24)

/** Defines the number of slots per frame, which is 15. */
#define UPS_SLOTS_PER_FRAME             15

/** Defines the maximum stored scrambling codes. */
#define MAX_STORED_SCRAMBLING_CODES         32

#if defined(UPGRADE_TWGGE_NRW)
# if defined (UPGRADE_3G) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** Defines the maximum stored cellParametersId. */
#define MAX_STORED_CELLPARAMETERSID         32
# endif
#else
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** Defines the maximum stored cellParametersId. */
#define MAX_STORED_CELLPARAMETERSID         32
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */
#endif
/** Defines the size of an array that holds all the reception
 * periods in a CTCH-BS (specified in 33.102).
 */
#define CBS_RECEPTION_PERIOD_OFFSET_ARRAY_SIZE      256

/** Defines the maximum number of FDD inter-frequency cells. */
#define MAX_FDD_INTER_FREQUENCY_CELLS       8

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** Defines the maximum number of Downlink timeslots for TDD. */
#define MAX_DL_TIMESLOT       6
/** Defines the maximum number of Uplink timeslots for TDD. */
#define MAX_UL_TIMESLOT       6
/** Defines the maximum number of TDD128 inter-frequency cells. */
#define MAX_TDD128_INTER_FREQUENCY_CELLS       9
/** Defines the maximum number of TDD128 frequency cells. */
#define MAX_TDD128_FREQUENCY_CELLS     (MAX_TDD128_INTER_FREQUENCY_CELLS + 1)
/** Defines the maximum number of Downlink CCTrCH for TDD128. */
#define UPS_MAX_DL_NO_OF_CCTRCH 1
/** Defines the maximum number of Uplink CCTrCH for TDD128. */
#define UPS_MAX_UL_NO_OF_CCTRCH 1
/** Defines the number of channel codes in a timeslot */
#define UUL_CHANNELS_PER_SLOT_FOR_TDD128 2
/** Defines the number of subframe in a frame */
#define TDD128_SUBFRAME_PER_FRAME 2

#else /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */

/** Defines the maximum number of Downlink CCTrCH for FDD. */
#define UPS_MAX_DL_NO_OF_CCTRCH 1
/** Defines the maximum number of Uplink CCTrCH for FDD. */
#define UPS_MAX_UL_NO_OF_CCTRCH 1

#if defined(UPGRADE_TWGGE_NRW)
#define MAX_TDD128_INTER_FREQUENCY_CELLS       9
/** Defines the maximum number of TDD128 frequency cells. */
#define MAX_TDD128_FREQUENCY_CELLS     (MAX_TDD128_INTER_FREQUENCY_CELLS + 1)

#endif

#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */


/** Defines the BCH transport block size in bits. */
#define UPS_BCH_TB_SIZE_BITS      246

/** Defines the BCH transport block size in octets. */
#define UPS_BCH_TB_SIZE_OCTETS    BITS_TO_INT8S(UPS_BCH_TB_SIZE_BITS)


#if defined(UPGRADE_3G_HSDPA)

#if defined (UPGRADE_3G_FDD)
/** Defines the maximum number of MAC-D PDUs permissible in a 2ms TTI. */
#  define UPS_MAX_HS_DSCH_MACD_PDUS_IN_TTI                70
   /* FDD must have at least 50kB buffer for HSDPA */
#endif

#if defined (UPGRADE_3G_TDD128)
/** Defines the maximum number of MAC-D PDUs permissible in a 5ms TTI. */
#  define UPS_TDD128_MAX_HS_DSCH_MACD_PDUS_IN_TTI         45

#  if defined (UPS_CFG_TDD128_HS_DSCH_CATEGORY_11)
/** Defines the maximum number of bits which can be received on HS-DSCH in each
 * 5ms TTI.
 */
#define UPS_HS_DSCH_TDD_MAX_BITS_PER_TTI          11226

#  elif defined (UPS_CFG_TDD128_HS_DSCH_CATEGORY_12)
/** Defines the maximum number of bits which can be received on HS-DSCH in each
 * 5ms TTI.
 */
#define UPS_HS_DSCH_TDD_MAX_BITS_PER_TTI          11226

#  elif defined (UPS_CFG_TDD128_HS_DSCH_CATEGORY_13)
/** Defines the maximum number of bits which can be received on HS-DSCH in each
 * 5ms TTI.
 */
#define UPS_HS_DSCH_TDD_MAX_BITS_PER_TTI          14043

#  elif defined (UPS_CFG_TDD128_HS_DSCH_CATEGORY_14)
/** Defines the maximum number of bits which can be received on HS-DSCH in each
 * 5ms TTI.
 */
#define UPS_HS_DSCH_TDD_MAX_BITS_PER_TTI          14043
#  elif defined (UPS_CFG_TDD128_HS_DSCH_CATEGORY_15)
/** Defines the maximum number of bits which can be received on HS-DSCH in each
 * 5ms TTI.
 */
#define UPS_HS_DSCH_TDD_MAX_BITS_PER_TTI          14043
#  elif defined (UPS_CFG_TDD128_HS_DSCH_CATEGORY_1)
/** Defines the maximum number of bits which can be received on HS-DSCH in each
 * 5ms TTI.
 */
#define UPS_HS_DSCH_TDD_MAX_BITS_PER_TTI          2788
#  elif defined (UPS_CFG_TDD128_HS_DSCH_CATEGORY_2)
/** Defines the maximum number of bits which can be received on HS-DSCH in each
 * 5ms TTI.
 */
#define UPS_HS_DSCH_TDD_MAX_BITS_PER_TTI          2788
#  elif defined (UPS_CFG_TDD128_HS_DSCH_CATEGORY_3)
/** Defines the maximum number of bits which can be received on HS-DSCH in each
 * 5ms TTI.
 */
#define UPS_HS_DSCH_TDD_MAX_BITS_PER_TTI          2788
#  elif defined (UPS_CFG_TDD128_HS_DSCH_CATEGORY_4)
/** Defines the maximum number of bits which can be received on HS-DSCH in each
 * 5ms TTI.
 */
#define UPS_HS_DSCH_TDD_MAX_BITS_PER_TTI          5600
#  elif defined (UPS_CFG_TDD128_HS_DSCH_CATEGORY_5)
/** Defines the maximum number of bits which can be received on HS-DSCH in each
 * 5ms TTI.
 */
#define UPS_HS_DSCH_TDD_MAX_BITS_PER_TTI          5600
#  elif defined (UPS_CFG_TDD128_HS_DSCH_CATEGORY_6)
/** Defines the maximum number of bits which can be received on HS-DSCH in each
 * 5ms TTI.
 */
#define UPS_HS_DSCH_TDD_MAX_BITS_PER_TTI          5600
#  elif defined (UPS_CFG_TDD128_HS_DSCH_CATEGORY_7)
/** Defines the maximum number of bits which can be received on HS-DSCH in each
 * 5ms TTI.
 */
#define UPS_HS_DSCH_TDD_MAX_BITS_PER_TTI          8416
#  elif defined (UPS_CFG_TDD128_HS_DSCH_CATEGORY_8)
/** Defines the maximum number of bits which can be received on HS-DSCH in each
 * 5ms TTI.
 */
#define UPS_HS_DSCH_TDD_MAX_BITS_PER_TTI          8416
#  elif defined (UPS_CFG_TDD128_HS_DSCH_CATEGORY_9)
/** Defines the maximum number of bits which can be received on HS-DSCH in each
 * 5ms TTI.
 */
#define UPS_HS_DSCH_TDD_MAX_BITS_PER_TTI          8416
#  elif defined (UPS_CFG_TDD128_HS_DSCH_CATEGORY_10)
/** Defines the maximum number of bits which can be received on HS-DSCH in each
 * 5ms TTI.
 */
#define UPS_HS_DSCH_TDD_MAX_BITS_PER_TTI          11226
#  else
#   error HS-DSCH category is not defined, but UPGRADE_3G_HSDPA is on.
#  endif /* UPS_CFG_TDD128_HS_DSCH_CATEGORY_11 */
#endif /* defined (UPGRADE_3G_TDD128) */


# if defined(UPGRADE_3G_FDD)
#  if defined (UPS_CFG_HS_DSCH_CATEGORY_11)
/** Defines the maximum number of channelisation codes L1 is capable of
 * receiving simultaneously on HS-DSCH.
 */
#   define UPS_HS_DSCH_MAX_RX_CODES               5
/** Defines the minimum HS-DSCH TTI reception distance supported by L1. 1 means
 * continuous HS-DSCH reception (every TTI). 2 means L1 does not support
 * continuous HS-DSCH reception: there must be a gap of 1 TTI between each TTI
 *
where HS-DSCH data is received.
 */
#   define UPS_HS_DSCH_MIN_INTER_TTI              2
/** Defines the maximum number of bits which can be received on HS-DSCH in each
 * 2ms TTI.
 */
#   define UPS_HS_DSCH_MAX_BITS_PER_TTI           3630
/** Defines the maximum number of soft memory bits available on HS-DSCH to all
 * the HARQ processes.
 */
#   define UPS_HS_DSCH_TOTAL_SOFT_BITS            1440


#  elif defined (UPS_CFG_HS_DSCH_CATEGORY_12)
/** Defines the maximum number of channelisation codes L1 is capable of
 * receiving simultaneously on HS-DSCH.
 */
#   define UPS_HS_DSCH_MAX_RX_CODES               5
/** Defines the minimum HS-DSCH TTI reception distance supported by L1. 1 means
 * continuous HS-DSCH reception (every TTI). 2 means L1 does not support
 * continuous HS-DSCH reception: there must be a gap of 1 TTI between each TTI
 * where HS-DSCH data is received.
 */
#   define UPS_HS_DSCH_MIN_INTER_TTI              1
/** Defines the maximum number of bits which can be received on HS-DSCH in each
 * 2ms TTI.
 */
#   define UPS_HS_DSCH_MAX_BITS_PER_TTI           3630
/** Defines the maximum number of soft memory bits available on HS-DSCH to all
 * the HARQ processes.
 */
#   define UPS_HS_DSCH_TOTAL_SOFT_BITS            2880
/** Defines CATEGORY_13-15 both TDD and FDD MAX_BITS_PER_TTI only for the
 * requirement of TDD. Corresponding other FDD parameters TBD in future if needed
 */
#  elif defined (UPS_CFG_HS_DSCH_CATEGORY_13)
/** Defines the maximum number of bits which can be received on HS-DSCH in each
 * 2ms TTI.
 */
#define UPS_HS_DSCH_MAX_BITS_PER_TTI          35280

#  elif defined (UPS_CFG_HS_DSCH_CATEGORY_14)
/** Defines the maximum number of bits which can be received on HS-DSCH in each
 * 2ms TTI.
 */
#define UPS_HS_DSCH_MAX_BITS_PER_TTI          42192

#  elif defined (UPS_CFG_HS_DSCH_CATEGORY_15)
/** Defines the maximum number of bits which can be received on HS-DSCH in each
 * 2ms TTI.
 */
#define UPS_HS_DSCH_MAX_BITS_PER_TTI          23370


#  elif defined (UPS_CFG_HS_DSCH_CATEGORY_1)
/** Defines the maximum number of channelisation codes L1 is capable of
 * receiving simultaneously on HS-DSCH.
 */
#   define UPS_HS_DSCH_MAX_RX_CODES               5
/** Defines the minimum HS-DSCH TTI reception distance supported by L1. 1 means
 * continuous HS-DSCH reception (every TTI). 2 means L1 does not support
 * continuous HS-DSCH reception: there must be a gap of 1 TTI between each TTI
 * where HS-DSCH data is received.
 */
#   define UPS_HS_DSCH_MIN_INTER_TTI              3
/** Defines the maximum number of bits which can be received on HS-DSCH in each
 * 2ms TTI.
 */
#   define UPS_HS_DSCH_MAX_BITS_PER_TTI           7298
/** Defines the maximum number of soft memory bits available on HS-DSCH to all
 * the HARQ processes.
 */
#   define UPS_HS_DSCH_TOTAL_SOFT_BITS            19200


#  elif defined (UPS_CFG_HS_DSCH_CATEGORY_2)
/** Defines the maximum number of channelisation codes L1 is capable of
 * receiving simultaneously on HS-DSCH.
 */
#   define UPS_HS_DSCH_MAX_RX_CODES               5
/** Defines the minimum HS-DSCH TTI reception distance supported by L1. 1 means
 * continuous HS-DSCH reception (every TTI). 2 means L1 does not support
 * continuous HS-DSCH reception: there must be a gap of 1 TTI between each TTI
 * where HS-DSCH data is received.
 */
#   define UPS_HS_DSCH_MIN_INTER_TTI              3
/** Defines the maximum number of bits which can be received on HS-DSCH in each
 * 2ms TTI.
 */
#   define UPS_HS_DSCH_MAX_BITS_PER_TTI           7298
/** Defines the maximum number of soft memory bits available on HS-DSCH to all
 * the HARQ processes.
 */
#   define UPS_HS_DSCH_TOTAL_SOFT_BITS            28800


#  elif defined (UPS_CFG_HS_DSCH_CATEGORY_3)
/** Defines the maximum number of channelisation codes L1 is capable of
 * receiving simultaneously on HS-DSCH.
 */
#   define UPS_HS_DSCH_MAX_RX_CODES               5
/** Defines the minimum HS-DSCH TTI reception distance supported by L1. 1 means
 * continuous HS-DSCH reception (every TTI). 2 means L1 does not support
 * continuous HS-DSCH reception: there must be a gap of 1 TTI between each TTI
 * where HS-DSCH data is received.
 */
#   define UPS_HS_DSCH_MIN_INTER_TTI              2
/** Defines the maximum number of bits which can be received on HS-DSCH in each
 * 2ms TTI.
 */
#   define UPS_HS_DSCH_MAX_BITS_PER_TTI           7298
/** Defines the maximum number of soft memory bits available on HS-DSCH to all
 * the HARQ processes.
 */
#   define UPS_HS_DSCH_TOTAL_SOFT_BITS            28800


#  elif defined (UPS_CFG_HS_DSCH_CATEGORY_4)
/** Defines the maximum number of channelisation codes L1 is capable of
 * receiving simultaneously on HS-DSCH.
 */
#   define UPS_HS_DSCH_MAX_RX_CODES               5
/** Defines the minimum HS-DSCH TTI reception distance supported by L1. 1 means
 * continuous HS-DSCH reception (every TTI). 2 means L1 does not support
 * continuous HS-DSCH reception: there must be a gap of 1 TTI between each TTI
 * where HS-DSCH data is received.
 */
#   define UPS_HS_DSCH_MIN_INTER_TTI              2
/** Defines the maximum number of bits which can be received on HS-DSCH in each
 * 2ms TTI.
 */
#   define UPS_HS_DSCH_MAX_BITS_PER_TTI           7298
/** Defines the maximum number of soft memory bits available on HS-DSCH to all
 * the HARQ processes.
 */
#   define UPS_HS_DSCH_TOTAL_SOFT_BITS            38400


#  elif defined (UPS_CFG_HS_DSCH_CATEGORY_5)
/** Defines the maximum number of channelisation codes L1 is capable of
 * receiving simultaneously on HS-DSCH.
 */
#   define UPS_HS_DSCH_MAX_RX_CODES               5
/** Defines the minimum HS-DSCH TTI reception distance supported by L1. 1 means
 * continuous HS-DSCH reception (every TTI). 2 means L1 does not support
 * continuous HS-DSCH reception: there must be a gap of 1 TTI between each TTI
 * where HS-DSCH data is received.
 */
#   define UPS_HS_DSCH_MIN_INTER_TTI              1
/** Defines the maximum number of bits which can be received on HS-DSCH in each
 * 2ms TTI.
 */
#   define UPS_HS_DSCH_MAX_BITS_PER_TTI           7298
/** Defines the maximum number of soft memory bits available on HS-DSCH to all
 * the HARQ processes.
 */
#   define UPS_HS_DSCH_TOTAL_SOFT_BITS            57600


#  elif defined (UPS_CFG_HS_DSCH_CATEGORY_6)
/** Defines the maximum number of channelisation codes L1 is capable of
 * receiving simultaneously on HS-DSCH.
 */
#   define UPS_HS_DSCH_MAX_RX_CODES               5
/** Defines the minimum HS-DSCH TTI reception distance supported by L1. 1 means
 * continuous HS-DSCH reception (every TTI). 2 means L1 does not support
 * continuous HS-DSCH reception: there must be a gap of 1 TTI between each TTI
 * where HS-DSCH data is received.
 */
#   define UPS_HS_DSCH_MIN_INTER_TTI              1
/** Defines the maximum number of bits which can be received on HS-DSCH in each
 * 2ms TTI.
 */
#   define UPS_HS_DSCH_MAX_BITS_PER_TTI           7298
/** Defines the maximum number of soft memory bits available on HS-DSCH to all
 * the HARQ processes.
 */
#   define UPS_HS_DSCH_TOTAL_SOFT_BITS            67200


#  elif defined (UPS_CFG_HS_DSCH_CATEGORY_7)
/** Defines the maximum number of channelisation codes L1 is capable of
 * receiving simultaneously on HS-DSCH.
 */
#   define UPS_HS_DSCH_MAX_RX_CODES               10
/** Defines the minimum HS-DSCH TTI reception distance supported by L1. 1 means
 * continuous HS-DSCH reception (every TTI). 2 means L1 does not support
 * continuous HS-DSCH reception: there must be a gap of 1 TTI between each TTI
 * where HS-DSCH data is received.
 */
#   define UPS_HS_DSCH_MIN_INTER_TTI              1
/** Defines the maximum number of bits which can be received on HS-DSCH in each
 * 2ms TTI.
 */
#   define UPS_HS_DSCH_MAX_BITS_PER_TTI           14411
/** Defines the maximum number of soft memory bits available on HS-DSCH to all
 * the HARQ processes.
 */
#   define UPS_HS_DSCH_TOTAL_SOFT_BITS            115200


#  elif defined (UPS_CFG_HS_DSCH_CATEGORY_8)
/** Defines the maximum number of channelisation codes L1 is capable of
 * receiving simultaneously on HS-DSCH.
 */
#   define UPS_HS_DSCH_MAX_RX_CODES               10
/** Defines the minimum HS-DSCH TTI reception distance supported by L1. 1 means
 * continuous HS-DSCH reception (every TTI). 2 means L1 does not support
 * continuous HS-DSCH reception: there must be a gap of 1 TTI between each TTI
 * where HS-DSCH data is received.
 */
#   define UPS_HS_DSCH_MIN_INTER_TTI              1
/** Defines the maximum number of bits which can be received on HS-DSCH in each
 * 2ms TTI.
 */
#   define UPS_HS_DSCH_MAX_BITS_PER_TTI           14411
/** Defines the maximum number of soft memory bits available on HS-DSCH to all
 * the HARQ processes.
 */
#   define UPS_HS_DSCH_TOTAL_SOFT_BITS            134400


#  elif defined (UPS_CFG_HS_DSCH_CATEGORY_9)
/** Defines the maximum number of channelisation codes L1 is capable of
 * receiving simultaneously on HS-DSCH.
 */
#   define UPS_HS_DSCH_MAX_RX_CODES               15
/** Defines the minimum HS-DSCH TTI reception distance supported by L1. 1 means
 * continuous HS-DSCH reception (every TTI). 2 means L1 does not support
 * continuous HS-DSCH reception: there must be a gap of 1 TTI between each TTI
 * where HS-DSCH data is received.
 */
#   define UPS_HS_DSCH_MIN_INTER_TTI              1
/** Defines the maximum number of bits which can be received on HS-DSCH in each
 * 2ms TTI.
 */
#   define UPS_HS_DSCH_MAX_BITS_PER_TTI           20251
/** Defines the maximum number of soft memory bits available on HS-DSCH to all
 * the HARQ processes.
 */
#   define UPS_HS_DSCH_TOTAL_SOFT_BITS            172800


#  elif defined (UPS_CFG_HS_DSCH_CATEGORY_10)
/** Defines the maximum number of channelisation codes L1 is capable of
 * receiving simultaneously on HS-DSCH.
 */
#   define UPS_HS_DSCH_MAX_RX_CODES               15
/** Defines the minimum HS-DSCH TTI reception distance supported by L1. 1 means
 * continuous HS-DSCH reception (every TTI). 2 means L1 does not support
 * continuous HS-DSCH reception: there must be a gap of 1 TTI between each TTI
 * where HS-DSCH data is received.
 */
#   define UPS_HS_DSCH_MIN_INTER_TTI              1
/** Defines the maximum number of bits which can be received on HS-DSCH in each
 * 2ms TTI.
 */
#   define UPS_HS_DSCH_MAX_BITS_PER_TTI           27952
/** Defines the maximum number of soft memory bits available on HS-DSCH to all
 * the HARQ processes.
 */
#   define UPS_HS_DSCH_TOTAL_SOFT_BITS            172800


#  else
#   error HS-DSCH category is not defined, but UPGRADE_3G_HSDPA is on.
#  endif /* UPS_CFG_HS_DSCH_CATEGORY_11 */
#endif /* # if defined(UPGRADE_3G_FDD) */

#if defined (ENABLE_PHY_HS_TEST_HARNESS)
# if defined (G1_PHONE)
#  define MAX_FAKE_MAX_NO_MACD_PDU_PER_TTI 2
# else
#  define MAX_FAKE_MAX_NO_MACD_PDU_PER_TTI 42
# endif
# define MAX_FAKE_MACD_SDU_BITS_SIZE 12032
# if 1  /*Use HS-DSCH category for TH*/
#  define UPS_HS_DSCH_MAX_BITS_PER_TTI_FOR_TH UPS_HS_DSCH_MAX_BITS_PER_TTI
# else  /*Use TH special category to handle High Data Rates: Warning: in that case, data may be corrupted*/
#  define UPS_HS_DSCH_MAX_BITS_PER_TTI_FOR_TH ((MAX_FAKE_MAX_NO_MACD_PDU_PER_TTI*MAX_FAKE_MACD_SDU_BITS_SIZE)+21)
# endif
#endif

/** Defines the maximum number of bits which can be received on HS-DSCH in each
 *  TTI.
 */
# if defined(UPGRADE_3G_FDD) && defined (UPGRADE_3G_TDD128)
#  if (UPS_HS_DSCH_TDD_MAX_BITS_PER_TTI > UPS_HS_DSCH_MAX_BITS_PER_TTI)
    #define UPS_HS_DSCH_MAX_BITS_PER_TTI_FOR_PS         UPS_HS_DSCH_TDD_MAX_BITS_PER_TTI
#  else
    #define UPS_HS_DSCH_MAX_BITS_PER_TTI_FOR_PS         UPS_HS_DSCH_MAX_BITS_PER_TTI
#  endif
#  if (UPS_TDD128_MAX_HS_DSCH_MACD_PDUS_IN_TTI > UPS_MAX_HS_DSCH_MACD_PDUS_IN_TTI)
    #define UPS_MAX_HS_DSCH_MACD_PDUS_IN_TTI_FOR_PS     UPS_TDD128_MAX_HS_DSCH_MACD_PDUS_IN_TTI
#  else
    #define UPS_MAX_HS_DSCH_MACD_PDUS_IN_TTI_FOR_PS     UPS_MAX_HS_DSCH_MACD_PDUS_IN_TTI
#  endif
# elif defined (UPGRADE_3G_TDD128)
    #define UPS_HS_DSCH_MAX_BITS_PER_TTI_FOR_PS         UPS_HS_DSCH_TDD_MAX_BITS_PER_TTI
    #define UPS_MAX_HS_DSCH_MACD_PDUS_IN_TTI_FOR_PS     UPS_TDD128_MAX_HS_DSCH_MACD_PDUS_IN_TTI
# else  /* (UPGRADE_3G_FDD)  */
    #define UPS_HS_DSCH_MAX_BITS_PER_TTI_FOR_PS         UPS_HS_DSCH_MAX_BITS_PER_TTI
    #define UPS_MAX_HS_DSCH_MACD_PDUS_IN_TTI_FOR_PS     UPS_MAX_HS_DSCH_MACD_PDUS_IN_TTI
# endif

#endif /* UPGRADE_3G_HSDPA */

#if defined(UPGRADE_3G_HSUPA)
/** Defines the maximum total number of transport blocks transmitted within
 * TTIs per LogCh that start at the same time in the uplink.
 */
# define UPS_MAX_EDCH_MACD_PDUS_PER_LCH         64

/** Defines the maximum total number of transport blocks transmitted within
 * TTIs that start at the same time in the uplink.
 * Note: +6 is implementation only, in order to support >1 LogCh at a time, but not to have too much allocs 
 */
# define UPS_MAX_EDCH_MACD_PDUS_PER_TTI         (UPS_MAX_EDCH_MACD_PDUS_PER_LCH+6)

/** Defines the maximum number of bytes which can be transmitted on E-DCH in each
 *  TTI.
 */
# if defined (UPGRADE_3G_FDD)
#  if defined(UPGRADE_REL6)
#   if defined (UPS_CFG_E_DCH_CATEGORY_6)
#    define UPS_MAX_FDD_MAC_E_PDU_SIZE_10MS      BITS_TO_INT8S (20000)
#    define UPS_MAX_FDD_MAC_E_PDU_SIZE_2MS       BITS_TO_INT8S (11484)
#    define UPS_MAX_FDD_MAC_E_PDU_SIZE_FOR_PS    UPS_MAX_FDD_MAC_E_PDU_SIZE_10MS
#   else /*UPS_CFG_E_DCH_CATEGORY_6*/
#    warning "Unsupported E-DCH category for FDD, please update this file"
#   endif /*UPS_CFG_E_DCH_CATEGORY_6*/
#  endif /*UPGRADE_REL6*/
# endif /*UPGRADE_3G_FDD*/
# if defined (UPGRADE_3G_TDD128)
#   define UPS_MAX_TDD128_MAC_E_PDU_SIZE        BITS_TO_INT8S (11160)
# endif

# if defined (UPGRADE_3G_FDD) && defined (UPGRADE_3G_TDD128)
#  if (UPS_MAX_TDD128_MAC_E_PDU_SIZE > UPS_MAX_FDD_MAC_E_PDU_SIZE_FOR_PS)
#   define UPS_MAX_MAC_E_PDU_SIZE_FOR_PS        UPS_MAX_TDD128_MAC_E_PDU_SIZE
#  else
#   define UPS_MAX_MAC_E_PDU_SIZE_FOR_PS        UPS_MAX_FDD_MAC_E_PDU_SIZE_FOR_PS
#  endif
# elif defined (UPGRADE_3G_TDD128)
#  define UPS_MAX_MAC_E_PDU_SIZE_FOR_PS         UPS_MAX_TDD128_MAC_E_PDU_SIZE
# else  /* (UPGRADE_3G_FDD)  */
#  define UPS_MAX_MAC_E_PDU_SIZE_FOR_PS         UPS_MAX_FDD_MAC_E_PDU_SIZE_FOR_PS
# endif

# if (UPS_MAX_UL_NO_OF_TB_PER_TTI > UPS_MAX_EDCH_MACD_PDUS_PER_TTI)
#  define UPS_MAX_UL_NO_OF_TB_PER_TTI_FOR_PS    UPS_MAX_UL_NO_OF_TB_PER_TTI
# else
#  define UPS_MAX_UL_NO_OF_TB_PER_TTI_FOR_PS    UPS_MAX_EDCH_MACD_PDUS_PER_TTI
# endif
#else /*UPGRADE_3G_HSUPA*/
#  define UPS_MAX_UL_NO_OF_TB_PER_TTI_FOR_PS    UPS_MAX_UL_NO_OF_TB_PER_TTI
#endif /*UPGRADE_3G_HSUPA*/
#if defined(UPGRADE_3G_HSDPA)
# if (UPS_MAX_DL_NO_OF_TB_PER_TTI > UPS_MAX_HS_DSCH_MACD_PDUS_IN_TTI_FOR_PS)
#  define UPS_MAX_DL_NO_OF_TB_PER_TTI_FOR_PS    UPS_MAX_DL_NO_OF_TB_PER_TTI
# else
#  define UPS_MAX_DL_NO_OF_TB_PER_TTI_FOR_PS    UPS_MAX_HS_DSCH_MACD_PDUS_IN_TTI_FOR_PS
# endif
#else /*UPGRADE_3G_HSDPA*/
# define UPS_MAX_DL_NO_OF_TB_PER_TTI_FOR_PS     UPS_MAX_DL_NO_OF_TB_PER_TTI
#endif /*UPGRADE_3G_HSDPA*/
#if (UPS_MAX_UL_NO_OF_TB_PER_TTI_FOR_PS > UPS_MAX_DL_NO_OF_TB_PER_TTI_FOR_PS)
# define UPS_MAX_NO_OF_TB_PER_TTI_FOR_PS       UPS_MAX_UL_NO_OF_TB_PER_TTI_FOR_PS
#else
# define UPS_MAX_NO_OF_TB_PER_TTI_FOR_PS       UPS_MAX_DL_NO_OF_TB_PER_TTI_FOR_PS
#endif

#endif  /* U1_CFG_H */

/* END OF FILE */
