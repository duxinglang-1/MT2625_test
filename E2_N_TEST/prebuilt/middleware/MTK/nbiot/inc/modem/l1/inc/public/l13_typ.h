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
 **************************************************************************/
/** \file
 * Types common to mmxx_sig.h rr_sig.h, mph_sig.h and cphy_sig.h.
 **************************************************************************/

#ifndef L13_TYP_H
#define L13_TYP_H

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

#if !defined (L123_TYP_H)
#include     <l123_typ.h>
#endif

/****************************************************************************
 * Manifest Constants
 ****************************************************************************/

/* can be 124, 174 or 374 */
#define LOWEST_SUPPORTED_PGSM_ARFCN          1
#define HIGHEST_SUPPORTED_PGSM_ARFCN       124
#define LOWEST_SUPPORTED_DCS_ARFCN         512
#define HIGHEST_SUPPORTED_DCS_ARFCN        885
#define LOWEST_SUPPORTED_EGSM_ARFCN        975
#define HIGHEST_SUPPORTED_EGSM_ARFCN      1023

#define NUM_PGSM_CHANNELS                  124
#define NUM_EGSM_CHANNELS                  174
#define NUM_DCS_CHANNELS                   374
#define NUM_PCS_CHANNELS                   299  /* Moved from l1types.h for consistency*/
#define NUM_GSM850_CHANNELS                124  /* Added for GSM850 support */
#define NUM_GSM750_CHANNELS                 74  /* Added for GSM750 support */

#if defined (UPGRADE_TRIBAND_OPERATION)
/* Tri-band operation includes an extra 124 channels for GSM850, allowing GSM850, (E)GSM900 and either DCS or PCS to work in parallel */
#if defined (UPGRADE_GSM400)
#   define NUM_MBAND_CHANNELS              742  /* Max number of MBAND channels with 35 in 450 band and 35 in 480 band */
#else
#   define NUM_MBAND_CHANNELS              672  /* Max number of MBAND channels */
#endif
#else
/* Dual-band operation, allowing either GSM850 and PCS1900 or (E)GSM and DCS1800 in parallel */
#if defined (UPGRADE_GSM400)
#   define NUM_MBAND_CHANNELS              618  /* Max number of MBAND channels with 35 in 450 band and 35 in 480 band */
#else
#   define NUM_MBAND_CHANNELS              548  /* Max number of MBAND channels */
#endif
#endif

/* Additions for GSM400 */
#define LOWEST_SUPPORTED_GSM450_ARFCN      259
#define HIGHEST_SUPPORTED_GSM450_ARFCN     293

#define LOWEST_SUPPORTED_GSM480_ARFCN      306
#define HIGHEST_SUPPORTED_GSM480_ARFCN     340

/* Additions for GSM850 */
#define LOWEST_SUPPORTED_GSM850_ARFCN      128
#define HIGHEST_SUPPORTED_GSM850_ARFCN     251

/* Additions for GSM750 */
#define LOWEST_SUPPORTED_GSM750_ARFCN      438
#define HIGHEST_SUPPORTED_GSM750_ARFCN     511

#define NUM_RF_CHANNELS     NUM_MBAND_CHANNELS

#define INVALID_ARFCN                   0xffff

#define LOWEST_SUPPORTED_PCS_ARFCN         512
#define HIGHEST_SUPPORTED_PCS_ARFCN        810
#define DCS_PCS_DOWNSHIFT                   48
#define EGSM_DOWNSHIFT                      58

#define OUT_OF_RANGE_ARFCN              0xfffe  /* Must be different from INVALID_ARFCN */

/* maximum number of rf channels for fixed array dimensioning */
#if defined(UPGRADE_ENHANCED_QUAD_BAND)
#define MAX_NUM_RF_CHANNELS  (NUM_EGSM_CHANNELS + NUM_DCS_CHANNELS + NUM_PCS_CHANNELS + NUM_GSM850_CHANNELS)
#else
#define MAX_NUM_RF_CHANNELS NUM_MBAND_CHANNELS
#endif

#define     ARFCN_LIST_SIZE         128

#define     MAX_BA_CHANNELS         32

#define     MAX_EXT_FREQ_LIST       64
#define     MAX_INT_ARFCN_LIST      32
#define     MAX_INT_HOP_LIST         4

#define     MAX_NUM_SEARCH_REPORTS       40  /* start off with 40 for MBAND */
#define     L1_MEAS_AVER_TIME            2 /* the number of MEASs we do average on */
#define     L1_MEAS_MAX_ARFCN_NUM        48/* the max number of arfcn pass the first filter, this value must be larger than MAX_NUM_SEARCH_REPORTS*/



#if defined (UPGRADE_AMR_PS) || defined (UPGRADE_AMR)
#define MAX_AMR_ACS_SIZE        4
#define MAX_AMR_THRESHOLD_SIZE  3
#endif

/****************************************************************************
 * Types
 ****************************************************************************/

/** TCH Loopback modes; The different types of ETSI loopback mode
 */
typedef enum TchLoopbackModeTag
{
    /** TCH loopback is off */
    TCH_LOOPBACK_OFF,
    /** TCH loopback A*/
    TCH_LOOPBACK_MODE_A,
    /** TCH loopback B*/
    TCH_LOOPBACK_MODE_B,
    /** TCH loopback C*/
    TCH_LOOPBACK_MODE_C,
    /** TCH loopback D*/
    TCH_LOOPBACK_MODE_D,
    /** TCH loopback E*/
    TCH_LOOPBACK_MODE_E,
    /** TCH loopback F*/
    TCH_LOOPBACK_MODE_F,
    /** TCH loopback I*/
    TCH_LOOPBACK_MODE_I
}
TchLoopbackMode;


/** Observed Time Difference.
 * This type is used during cell handover to store the Observed Time
 * Difference.
 */
typedef SignedInt32 Otd;


/** Handover synchronisation.
 * This describes the possible different types of synchronisation that
 * can be used for handover.  These are described in ETSI specification
 * 44.018, section 3.4.4.
 */
typedef enum SynchronisationTag
{
    /** Non-synchronised.
     * Channel Rx is activated immediately, but the DCCH and optionally SACCH
     * are filled with access bursts until reception of a PHYSICAL INFORMATION
     * message to give proper TA, after which normal Tx starts.
     */
    NON_SYNCHRONISED            = 0,

    /** Finely-synchronsied.
     * TA must be supplied in handover command.  1 radio block of access
     * bursts maybe sent on DCCH and optionally on SACCH prior to the normal
     * channel Rx and Tx starting.  The handover command indicates if the
     * RACH messages are optional or not.
     */
    SYNCHRONISED                = 1,

    /** Pre-synchronised.
     * Similar to finely synchronised, but the TA may be absent, in which a
     * default value of 1 is used.
     */
    PRE_SYNCHRONISED            = 2,

    /** Pseudo-synchronised.
     * Similar to finely synchronised, but the TA is computed from the real
     * time difference value given in the HANDOVER COMMAND.
     */
    PSEUDO_SYNCHRONISED         = 3
}
Synchronisation;


/** Difference PCH reception modes.
 * This enumerator is used in IDLE state to indicate which paging mode the MS
 * should be using. The choices are Normal (only paging messages for the
 * specified sub-channel are decoded), Extended (as for Normal, but messages
 * in the next-but-one sub-channel are also decoded), Reorganise (all paging
 * messages received are decoded), Same as Before (revert to previous mode)
 * and No PCH (no paging messages are decoded).
 */
typedef enum PageModeTag
{
    /**
     * Only paging messages for the specified sub-channel are decoded
     */
    PG_MODE_NORMAL,

    /**
     * As for #PG_MODE_NORMAL, but messages in the next-but-one
     * sub-channel are also decoded
     */
    PG_MODE_EXTENDED,

    PG_MODE_REORGANISE,   /**< All CCCH blocks */
    PG_SAME_AS_BEFORE,    /**< Use previous mode */
    PG_MODE_NON_DRX,      /**< GPRS non-DRX mode to pick up all PCH */
    PG_MODE_NO_PCH,       /**< No paging messages are received or decoded */
    PG_INVALID
}
PageMode;


/** Physical channel types.
 * This enumerator is used during dedicated channel setup to specify channel type:
 * whether Traffic-plus-Associated Control or Standalone Dedicated Control and if
 * so which variants of these
 */
typedef enum ChannelTypeTag
{
    TCH_F_PLUS_ACCH             = 1, /**< Full rate channel with associated control */
    TCH_H_PLUS_ACCH             = 2, /**< Half rate channel with associated control */
    SDCCH_4                     = 4, /**< Standalone dedicated control channel */
    SDCCH_8                     = 8  /**< Standalone dedicated control channel */
}
ChannelType;


/** Ciphering algorithms.
 * This enumerator is used when ciphering has been enabled to list the
 * ciphering algorithm to be used.
 */
typedef enum CipherAlgorithmTag
{
    /** Algorithm A5-1; algorithm used in the GSM ciphering process */
    ALGORITHM_A5_1              =   0,
    /** Algorithm A5-2; algorithm used in the GSM ciphering process,
     * this algorithm is simpler than A5/1 */
    ALGORITHM_A5_2              =   1,
    /** Algorithm A5-3 */
    ALGORITHM_A5_3              =   2,
    /** Algorithm A5-4 */
    ALGORITHM_A5_4              =   3,
    /** Algorithm A5-5 */
    ALGORITHM_A5_5              =   4,
    /** Algorithm A5-6 */
    ALGORITHM_A5_6              =   5,
    /** Algorithm A5-7 */
    ALGORITHM_A5_7              =   6
}
CipherAlgorithm;

/* Use to identify in L1Systemdata which algorithm is currently loaded */
typedef enum CipherAlgorithmLoadedTag
{
    ALGORITHM_INIT  = 0,       /* L1SystemData initialised to 0 */
    ALGORITHM_NONE,
    GSM_ALGORITHM_A5_1,
    GSM_ALGORITHM_A5_2,
    GSM_ALGORITHM_A5_3,
    GSM_ALGORITHM_A5_4,
    GSM_ALGORITHM_A5_5,
    GSM_ALGORITHM_A5_6,
    GSM_ALGORITHM_A5_7,
    GPRS_ALGORITHM_GEA_1,
    GPRS_ALGORITHM_GEA_2,
    GPRS_ALGORITHM_GEA_3,
    GPRS_ALGORITHM_GEA_4,
    GPRS_ALGORITHM_GEA_5,
    GPRS_ALGORITHM_GEA_6,
    GPRS_ALGORITHM_GEA_7
}
CipherAlgorithmLoaded;


/** Mode of a dedicated channel.
 * This enumerator is used during dedicated channel setup to specify channel
 * mode (signaling-only, speech or data), mode version and data rate.
 *
 * \see #ChannelMode2Tag
 */
typedef enum ChannelModeTag
{
    /** Signalling only */
    CHM_SIGNALLING              =   0x00,
    /** Speech full rate version 1 */
    CHM_SPEECH_FULL_RATE        =   0x01,
    /** Speech full rate version 2 */
    CHM_SPEECH_FULL_RATE_VER2   =   0x21,
    /** Speech full rate version 3 */
    CHM_SPEECH_FULL_RATE_VER3   =   0x41,
    /** Speech half rate version 1 */
    CHM_SPEECH_HALF_RATE        =   0x05,
    /** Speech half rate version 2 */
    CHM_SPEECH_HALF_RATE_VER2   =   0x25,
    /** Speech half rate version 3 */
    CHM_SPEECH_HALF_RATE_VER3   =   0x45,
    /** Data, 14.4 kbit/s radio interface rate */
    CHM_DATA_14400              =   0x0f,
    /** Data, 9.6 kbit/s radio interface rate */
    CHM_DATA_9600               =   0x03,
    /** Data, 4.8 kbit/s radio interface rate */
    CHM_DATA_4800_FULL          =   0x0B,
    /** Data, 2.4 kbit/s full rate */
    CHM_DATA_2400_FULL          =   0x13,

    /* The 4800_HALF codepoint does not correspond to CHM coding in 04.08 as
     * it clashes with 14.4 data. It must be set to some unused codepoint.
     */
    /** Data, 4.8 kbit/s half rate */
    CHM_DATA_4800_HALF          =   0x02,
    /** Data, 2.4 kbit/s half rate */
    CHM_DATA_2400_HALF          =   0x17
}
ChannelMode;


/** Cell Broadcast control.
 * This enumerator tells Layer 1 to accept or ignore broadcast data from the
 * selected cell, or alternatively to skip receiving it for a certain number
 * of slots.
 */
typedef enum CbControlTag
{
    CB_OFF,             /**< Stop CBCH reception */
    CB_ON,              /**< Start CBCH reception */
    CB_SKIP,
    CB_SKIP_N_SLOTS     /**< Skip some number of slots */
}
CbControl;

/** Arfcn bitmap.
 * This gives a list of valid ARFCNs and is used by MPH signal exchanges
 * involving BCCH search or measurements. The list is a bitmap of 1024
 * bits where channels are mapped according to their ARFCNs.
 *
 * The channels of the various bands are mapped directly to bits 0 - 1023
 * as follows:
 *
 * <TABLE>
 *  <TR><TD>Arfcn Band</TD><TD>Bits               </TD></TR>
 *  <TR><TD>P-GSM     </TD><TD>1 - 124            </TD></TR>
 *  <TR><TD>E-GSM     </TD><TD>0 - 124, 975 - 1023</TD></TR>
 *  <TR><TD>GSM850    </TD><TD>128 - 251          </TD></TR>
 *  <TR><TD>GSM450    </TD><TD>259 - 293          </TD></TR>
 *  <TR><TD>GSM480    </TD><TD>306 - 340          </TD></TR>
 *  <TR><TD>DCS1800   </TD><TD>512 - 885          </TD></TR>
 *  <TR><TD>PCS1900   </TD><TD>512 - 810          </TD></TR>
 * </TABLE>
 */
typedef struct ArfcnListTag
{
    Int8        list[ARFCN_LIST_SIZE];
}
ArfcnList;


/** BA list representation.
 * This structure is used to specify a list of neighbour-cell ARFCNs.  The
 * baInd field is used to indicate which BA list this is, and is helpful
 * in coordinating measurement reports if and when the BA list is changed.
 */
typedef struct BcchAllocationTag
{
    Int8       numChannels;             /**< Number of channels in array[]
\assoc ARRAY \ref array */
    Int8       baInd;                   /**< BA list identifier */
    Arfcn      array [MAX_BA_CHANNELS]; /**< List of ARFCNs */
}
BcchAllocation;


/** \defgroup CfgGsmBandModes  GSM Band Modes
 * \ingroup PrdGSMProtoStack
 * \ingroup Cfg
 * Types and enumerations relating to MS band control.
 * These types control the bands in which the MS may operate, as well as
 * controlling the auto-band selection implementation.
 * @{
 */

/** Bands
 * This type is used to index band specific configuration information.
 * \internal RR uses these to index cell information from an array dimensioned
 *           to #NUM_BANDS. Therefore they must be kept in the same order.
 */
typedef enum BandTag
{
    /** GSM band */
    GSM_BAND        = 0,
    /** DCS band */
    DCS_BAND        = 1,
    /** PCS band */
    PCS_BAND        = 2,
    /** EGSM band */
    EGSM_BAND       = 3,
    /** GSM 450 band */
    GSM_450_BAND    = 4,
    /** GSM 480 band */
    GSM_480_BAND    = 5,
    /** GSM 850 band */
    GSM_850_BAND    = 6,
#if defined (UPGRADE_GSM750) || defined(UPGRADE_TWGGE_NRW)
    /** GSM 750 band */
    GSM_750_BAND    = 7,
#endif
    /** The number of bands supported by Layer 1 and RR */
    NUM_BANDS,
#if defined (UPGRADE_3G)
# if defined (UPGRADE_LTE)
    /** LTE BAND; used by MM */
    LTE_BAND        = 0xFD,
# endif
    /** UMTS band; used by MM */
    UMTS_BAND       = 0xFE,
#endif
    /** Invalid band */
    INVALID_BAND    = 0xFF
}
Band;

/** Primary GSM. Arfcns 1 - 124 */
#define PGSM_BAND_BIT           0x0001

/** DCS. Arfcns 512 - 885 */
#define DCS_BAND_BIT            0x0002

/** PCS. Arfcns 512 - 810 */
#define PCS_BAND_BIT            0x0004

/** Additional extended GSM channels. Arfcns 0, 975 - 1023 */
#define EGSM_EXTRA_BAND_BIT     0x0008

/** Extended GSM.  Arfcns 0 - 124, 975 - 1023 */
#define EGSM_BAND_BITS          (PGSM_BAND_BIT + EGSM_EXTRA_BAND_BIT)

/** Only 450Mhz supported in GSM400 band. Arfcns 259 - 293 */
#define GSM450_EXTRA_BAND_BIT   0x0010

/** Only 480Mhz supported in GSM400 band. Arfcns 306 - 340 */
#define GSM480_EXTRA_BAND_BIT   0x0020

/** Both GSM400 bands supported i.e. 450_480. */
#define GSM400_EXTRA_BAND_BITS  (GSM450_EXTRA_BAND_BIT + GSM480_EXTRA_BAND_BIT)

/** GSM 850. Arfcns 128 - 251 */
#define GSM850_BAND_BIT         0x0040

/** GSM750 supported.  Arfcns 438 - 511. */
#define GSM750_BAND_BIT         0x0080

/* UMTS supported. */
#define UMTS_BAND_BIT           0x0100

#if defined (UPGRADE_LTE)
/* LTE Supported. */
#define LTE_BAND_BIT            0x0200

/** Used to disable auto-band select.
 * When present, this locks the MS to a given band or set of bands.
 */
#define BAND_LOCK_BIT           0x0400
#else
/** Used to disable auto-band select.
 * When present, this locks the MS to a given band or set of bands.
 */
#define BAND_LOCK_BIT           0x0200
#endif /* UPGRADE_LTE */

/** Bitmap for indicating supported bands.
 * This is a bitmap showing which bands are supported by the MS. Note that
 * whilst the actual bit assignments may change, the interface is specified
 * by the definitions, which will remain constant.
 *
 * Band modes are often used in interpreting of #Arfcn values; since the number
 * scheme for PCS and DCS may overlap, knowledge of the operating \a BandMode
 * can be used to resolve and ambiguity.
 */
typedef enum BandModeTag
{
    NO_BAND_MODE        = 0,
    PGSM_MODE           = PGSM_BAND_BIT,                                                                /**< Autoband enabled */
    EGSM_MODE           = EGSM_BAND_BITS,                                                               /**< Autoband enabled */
    DCS_MODE            = DCS_BAND_BIT,                                                                 /**< Autoband enabled */
    PGSM_DCS_MODE       = PGSM_BAND_BIT  + DCS_BAND_BIT,                                                /**< Autoband enabled */
    EGSM_DCS_MODE       = EGSM_BAND_BITS + DCS_BAND_BIT,                                                /**< Autoband enabled */
    PCS_MODE            = PCS_BAND_BIT,                                                                 /**< Autoband enabled */
    PGSM_PCS_MODE       = PGSM_BAND_BIT  + PCS_BAND_BIT,                                                /**< Autoband enabled */
    EGSM_PCS_MODE       = EGSM_BAND_BITS + PCS_BAND_BIT,                                                /**< Autoband enabled */

    PGSM_PCS_MODE_LOCK  = PGSM_BAND_BIT  + PCS_BAND_BIT + BAND_LOCK_BIT,                                /**< LOCK the MS to PGSM/PCS mode. Autoband DISABLED. For use in testing and 900/1900 countries only */
    EGSM_PCS_MODE_LOCK  = EGSM_BAND_BITS + PCS_BAND_BIT + BAND_LOCK_BIT,                                /**< LOCK the MS to EGSM/PCS mode. Autoband DISABLED. For use in testing and 900/1900 countries only */
    EGSM_DCS_MODE_LOCK  = EGSM_BAND_BITS + DCS_BAND_BIT + BAND_LOCK_BIT,                                /**< LOCK the MS to EGSM/DCS mode. Autoband DISABLED. For use in testing and 900/1800 countries only */
    DCS_MODE_LOCK       = DCS_BAND_BIT + BAND_LOCK_BIT,                                                 /**< LOCK the MS to DCS mode. Autoband DISABLED. For use in testing only */
    PCS_MODE_LOCK       = PCS_BAND_BIT + BAND_LOCK_BIT,                                                 /**< LOCK the MS to PCS mode. Autoband DISABLED. For use in testing only */
    EGSM_MODE_LOCK      = EGSM_BAND_BITS + BAND_LOCK_BIT,                                               /**< LOCK the MS to EGSM mode. Autoband DISABLED. For use in testing only */

    /* Band mode for GSM850 support */
    GSM850_MODE         = GSM850_BAND_BIT,                                                              /**< Autoband enabled */
    GSM850_PCS_MODE     = GSM850_BAND_BIT + PCS_BAND_BIT,                                               /**< Autoband enabled */
    GSM850_PCS_MODE_LOCK = GSM850_BAND_BIT + PCS_BAND_BIT + BAND_LOCK_BIT,                              /**< LOCK the MS to GSM850/PCS1900 mode. */

    GSM850_DCS_MODE        = GSM850_BAND_BIT + DCS_BAND_BIT,                                            /**< Autoband enabled */
    GSM850_MODE_LOCK       = GSM850_BAND_BIT + BAND_LOCK_BIT,                                           /**< LOCK the MS to GSM850 mode. Autoband DISABLED. For use in testing only */
    GSM850_DCS_MODE_LOCK   = GSM850_BAND_BIT + DCS_BAND_BIT + BAND_LOCK_BIT,                            /**< LOCK the MS to GSM850/DCS1800 mode. Autoband DISABLED. For use in testing and 850/1800 countries only */

#if defined (UPGRADE_GSM400)
    GSM850_GSM450_MODE      = GSM850_BAND_BIT + GSM450_EXTRA_BAND_BIT,
    GSM850_GSM450_PCS_MODE  = GSM850_BAND_BIT + GSM450_EXTRA_BAND_BIT + PCS_BAND_BIT,                   /**< Autoband enabled */
    GSM850_GSM480_MODE      = GSM850_BAND_BIT + GSM480_EXTRA_BAND_BIT,                                  /**< Autoband enabled */
    GSM850_GSM480_PCS_MODE  = GSM850_BAND_BIT + GSM480_EXTRA_BAND_BIT + PCS_BAND_BIT,                   /**< Autoband enabled */

    GSM850_GSM400_MODE      = GSM850_BAND_BIT + GSM450_EXTRA_BAND_BIT + GSM480_EXTRA_BAND_BIT,                    /**< Autoband enabled */
    GSM850_GSM400_PCS_MODE  = GSM850_BAND_BIT + GSM450_EXTRA_BAND_BIT + GSM480_EXTRA_BAND_BIT + PCS_BAND_BIT,     /**< Autoband enabled */

    /* The extra band modes for GSM400 support. First, the GSM450 band */
    PGSM450_MODE        = PGSM_BAND_BIT + GSM450_EXTRA_BAND_BIT,                                        /**< Autoband enabled */
    EGSM450_MODE        = EGSM_BAND_BITS + GSM450_EXTRA_BAND_BIT,                                       /**< Autoband enabled */
    PGSM450_DCS_MODE    = PGSM_BAND_BIT + GSM450_EXTRA_BAND_BIT + DCS_BAND_BIT,                         /**< Autoband enabled */
    EGSM450_DCS_MODE    = EGSM_BAND_BITS + GSM450_EXTRA_BAND_BIT + DCS_BAND_BIT,                        /**< Autoband enabled */
    PGSM450_PCS_MODE    = PGSM_BAND_BIT + GSM450_EXTRA_BAND_BIT + PCS_BAND_BIT,                         /**< Autoband enabled */
    EGSM450_PCS_MODE    = EGSM_BAND_BITS + GSM450_EXTRA_BAND_BIT + PCS_BAND_BIT,                        /**< Autoband enabled */

    /* GSM480 band */
    PGSM480_MODE        = PGSM_BAND_BIT + GSM480_EXTRA_BAND_BIT,                                        /**< Autoband enabled */
    EGSM480_MODE        = EGSM_BAND_BITS + GSM480_EXTRA_BAND_BIT,                                       /**< Autoband enabled */
    PGSM480_DCS_MODE    = PGSM_BAND_BIT + GSM480_EXTRA_BAND_BIT + DCS_BAND_BIT,                         /**< Autoband enabled */
    EGSM480_DCS_MODE    = EGSM_BAND_BITS + GSM480_EXTRA_BAND_BIT + DCS_BAND_BIT,                        /**< Autoband enabled */
    PGSM480_PCS_MODE    = PGSM_BAND_BIT + GSM480_EXTRA_BAND_BIT + PCS_BAND_BIT,                         /**< Autoband enabled */
    EGSM480_PCS_MODE    = EGSM_BAND_BITS + GSM480_EXTRA_BAND_BIT + PCS_BAND_BIT,                        /**< Autoband enabled */

    /* GSM450 and GSM480 bands */
    PGSM400_MODE          = PGSM_BAND_BIT + GSM400_EXTRA_BAND_BITS,                                     /**< Autoband enabled */
    EGSM400_MODE          = EGSM_BAND_BITS + GSM400_EXTRA_BAND_BITS,                                    /**< Autoband enabled */
    PGSM400_DCS_MODE      = PGSM_BAND_BIT + GSM400_EXTRA_BAND_BITS + DCS_BAND_BIT,                      /**< Autoband enabled */
    EGSM400_DCS_MODE      = EGSM_BAND_BITS + GSM400_EXTRA_BAND_BITS + DCS_BAND_BIT,                     /**< Autoband enabled */
    PGSM400_PCS_MODE      = PGSM_BAND_BIT + GSM400_EXTRA_BAND_BITS + PCS_BAND_BIT,                      /**< Autoband enabled */
    EGSM400_PCS_MODE      = EGSM_BAND_BITS + GSM400_EXTRA_BAND_BITS + PCS_BAND_BIT,                     /**< Autoband enabled */

    PGSM400_PCS_MODE_LOCK  = PGSM_BAND_BIT + GSM400_EXTRA_BAND_BITS + PCS_BAND_BIT + BAND_LOCK_BIT,     /**< LOCK the MS to GSM400/PCS1900 mode. Autoband DISABLED. For use in testing and 400/1900 countries only */
    EGSM400_PCS_MODE_LOCK  = EGSM_BAND_BITS + GSM400_EXTRA_BAND_BITS + PCS_BAND_BIT + BAND_LOCK_BIT,    /**< LOCK the MS to EGSM850/PCS1900 mode. Autoband DISABLED. For use in testing and 400/1900 countries only */

#endif
#if defined (UPGRADE_3G)
    /** used by MM */
    UMTS_MODE           = UMTS_BAND_BIT,
#endif

#if defined (UPGRADE_LTE)
    /** used to identify that this is not a GSM band for an LTE and GSM build */
    LTE_MODE            = LTE_BAND_BIT,
    /** This is used to verify value is within the allowed range
     * (Note that the value should be updated if new bit is added) */
    ALL_BITS_BAND_MODE = BAND_LOCK_BIT + LTE_BAND_BIT + UMTS_BAND_BIT + GSM750_BAND_BIT + GSM850_BAND_BIT + GSM400_EXTRA_BAND_BITS + EGSM_BAND_BITS + DCS_BAND_BIT + PCS_BAND_BIT,
#else
    /** This is used to verify value is within the allowed range
     * (Note that the value should be updated if new bit is added) */
    ALL_BITS_BAND_MODE = BAND_LOCK_BIT + UMTS_BAND_BIT + GSM750_BAND_BIT + GSM850_BAND_BIT + GSM400_EXTRA_BAND_BITS + EGSM_BAND_BITS + DCS_BAND_BIT + PCS_BAND_BIT,
#endif
    /** Invalid band mode */
    INVALID_BAND_MODE   = 0xffff
}
BandMode;

/** Used thoughout the stack to define the user selected mode and active RAT
    DO NOT CHANGE THE ORDER */
typedef enum NetworkModesTag
{
    NMODE_GSM,
    NMODE_UMTS_FDD,
    NMODE_UMTS_TDD128,
    NMODE_GSM_UMTS_FDD,
    NMODE_GSM_UMTS_TDD128,
#if defined (UPGRADE_LTE)
    NMODE_LTE_FDD,                                /* To be supported in a later release */
    NMODE_LTE_TD,
    NMODE_GSM_LTE_FDD,                            /* To be supported in a later release */
    NMODE_GSM_LTE_TD,                             /* To be supported in a later release */
    NMODE_LTE_FDD_UMTS_FDD,                       /* To be supported in a later release */
    NMODE_LTE_TD_UMTS_TDD128,                     /* To be supported in a later release */
    NMODE_GSM_LTE_FDD_UMTS_FDD,                   /* To be supported in a later release */
    NMODE_GSM_LTE_TD_UMTS_TDD128,                 /* To be supported in a later release */
    NMODE_LTE,
    NMODE_GSM_LTE,                                /* To be supported in a later release */
    NMODE_LTE_UMTS_FDD,                           /* To be supported in a later release */
    NMODE_LTE_UMTS_TDD128,                        /* To be supported in a later release */
    NMODE_GSM_LTE_UMTS_FDD,                       /* To be supported in a later release */
    NMODE_GSM_LTE_UMTS_TDD128,                    /* To be supported in a later release */
#endif /* UPGRADE_LTE */
    NMODE_NULL         /**< for init only */
}
NetworkModes;

#if defined(UPGRADE_3G_TDD128)
#define TDD_RAT 1
#else
#define TDD_RAT 0
#endif

#if defined(UPGRADE_3G_FDD)
#define FDD_RAT 1
#else
#define FDD_RAT 0
#endif

#if defined(UPGRADE_GGE)
#define GSM_RAT 1
#else /* !(UPGRADE_GGE) */
#define GSM_RAT 0
#endif /* !(UPGRADE_GGE) */

#if defined (UPGRADE_LTE)
#define LTE_TD_RAT 1
#define LTE_FDD_RAT 1
#define LTE_RAT 1
#define NUMBER_OF_SUPPORTED_RATS  (GSM_RAT+FDD_RAT+TDD_RAT+LTE_TD_RAT+LTE_FDD_RAT+LTE_RAT)
#else /* (UPGRADE_LTE) */
#define NUMBER_OF_SUPPORTED_RATS  (GSM_RAT+FDD_RAT+TDD_RAT)
#endif /* (UPGRADE_LTE) */


#define NMODE_GSM_OFFSET            (0)
#define NMODE_UMTS_FDD_OFFSET       (GSM_RAT)
#define NMODE_UMTS_TDD128_OFFSET    (GSM_RAT+FDD_RAT)
#if defined (UPGRADE_LTE)
#define NMODE_LTE_TD_OFFSET         (GSM_RAT+FDD_RAT+TDD_RAT)
#define NMODE_LTE_FDD_OFFSET        (GSM_RAT+FDD_RAT+TDD_RAT+LTE_TD_RAT)
#define NMODE_LTE_OFFSET            (GSM_RAT+FDD_RAT+TDD_RAT+LTE_TD_RAT+LTE_FDD_RAT)
#define NMODE_NULL_OFFSET           (GSM_RAT+FDD_RAT+TDD_RAT+LTE_TD_RAT+LTE_FDD_RAT+LTE_RAT)
#else
#define NMODE_NULL_OFFSET           (GSM_RAT+FDD_RAT+TDD_RAT)
#endif /* UPGRADE_LTE */
/** @} */ /* End of CfgGSMBandModes group */

#endif
/* END OF FILE */
