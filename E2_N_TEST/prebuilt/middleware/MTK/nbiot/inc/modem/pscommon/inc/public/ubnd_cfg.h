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
 * File Description:
 *
 * 3G Band configuration
 **************************************************************************/
/** \file
 * Multiband Primitive Data Type definitions
 *************************************************************************/

#if !defined (UBND_CFG_H)
#define       UBND_CFG_H
# if defined (UPGRADE_3G)

/***************************************************************************
* Nested Include Files
***************************************************************************/


/***************************************************************************
*   Manifest Constants
***************************************************************************/

/** The maximum number of supported bands. */
#if defined (UPGRADE_FDD_MULTIBAND)
#define MAX_NUM_FDD_BANDS 3
#else
#define MAX_NUM_FDD_BANDS 1
#endif

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
#define MAX_NUM_TDD128_BANDS 3
#define NUM_USED_TDD128_BANDS 3 /* for MSC */
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */

/** Supported Bands bit masks
 * Bit map indicating which UMTS (FDD) bands are supported .
 */

/** Band 1 bit mask allocation. */
#define FDD_BAND_1_BIT         0x0001
/** Band 2 bit mask allocation. */
#define FDD_BAND_2_BIT         0x0002
/** Band 3 bit mask allocation. */
#define FDD_BAND_3_BIT         0x0004
/** Band 4 bit mask allocation. */
#define FDD_BAND_4_BIT         0x0008
/** Band 5 bit mask allocation. */
#define FDD_BAND_5_BIT         0x0010
/** Band 6 bit mask allocation. */
#define FDD_BAND_6_BIT         0x0020
/** Band 7 bit mask allocation. */
#define FDD_BAND_7_BIT         0x0040
/** Band 8 bit mask allocation. */
#define FDD_BAND_8_BIT         0x0080
/** Band 9 bit mask allocation. */
#define FDD_BAND_9_BIT         0x0100
/** Band 10 bit mask allocation. */
#define FDD_BAND_10_BIT        0x0200
/** used to indicate currently in GSM mode. */
#define GSM_BAND_BIT           0x1000

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** Supported Bands bit masks
 * Bit mask allocation indicating which UMTS (TDD128) bands are supported .
 */
/** Band A LOWER bit mask allocation. */
#define TDD128_BAND_A_LOWER_BIT         0x0001
/** Band A UPPER bit mask allocation. */
#define TDD128_BAND_A_UPPER_BIT         0x0002
/** Band B LOWER bit mask allocation. */
#define TDD128_BAND_B_LOWER_BIT         0x0004
/** Band B UPPER bit mask allocation. */
#define TDD128_BAND_B_UPPER_BIT         0x0008
/** Band C bit mask allocation. */
#define TDD128_BAND_C_BIT               0x00010
/** Band 19 bit mask allocation. */
#define TDD128_BAND_F_BIT               0x00020
/** Band 23 bit mask allocation. */
#define TDD128_BAND_E_BIT               0x00040
/** Band D bit mask allocation. */
#define TDD128_BAND_D_BIT               0x00080

#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */


/** Frequency bands.
 * Min and max values of UARFCN for the bands as defined in section
 * 5.4.4 in 25.101.  and TxRx Separation as defined in same.
 */

/** FDD Band 1 min UARFCN. */
#define MIN_UARFCN_IN_BAND_1   10562
/** FDD Band 1 max UARFCN. */
#define MAX_UARFCN_IN_BAND_1   10838
/** FDD Band 1 number of UARFCNs. */
#define NUM_UARFCN_IN_BAND_1   ((MAX_UARFCN_IN_BAND_1 + 1) - MIN_UARFCN_IN_BAND_1)
/** FDD Band 1 TxTx Separation. */
#define TXRX_SEPARATION_BAND_1 950

/** FDD Band 2 min UARFCN. */
#define MIN_UARFCN_IN_BAND_2   9662
/** FDD Band 2 max UARFCN. */
#define MAX_UARFCN_IN_BAND_2   9938
/** FDD Band 2 number of UARFCNs. */
#define TXRX_SEPARATION_BAND_2 400
/** FDD Band 2 min additional UARFCN. */
#define MIN_ADD_UARFCN_IN_BAND_2   412
/** FDD Band 2 max additional UARFCN. */
#define MAX_ADD_UARFCN_IN_BAND_2   687
/** FDD Band 2 number of additional UARFCNs. */
#define NUM_ADD_UARFCN_IN_BAND_2   12

/** FDD Band 3 min UARFCN. */
#define MIN_UARFCN_IN_BAND_3   1162
/** FDD Band 3 max UARFCN. */
#define MAX_UARFCN_IN_BAND_3   1513
/** FDD Band 3 number of UARFCNs. */
#define TXRX_SEPARATION_BAND_3 225

/** FDD Band 4 min UARFCN. */
#define MIN_UARFCN_IN_BAND_4   1537
/** FDD Band 4 max UARFCN. */
#define MAX_UARFCN_IN_BAND_4   1738
/** FDD Band 4 number of UARFCNs. */
#define TXRX_SEPARATION_BAND_4 225
/** FDD Band 4 min additional UARFCN. */
#define MIN_ADD_UARFCN_IN_BAND_4   1887
/** FDD Band 4 max additional UARFCN. */
#define MAX_ADD_UARFCN_IN_BAND_4   2087
/** FDD Band 4 number of additional UARFCNs. */
#define NUM_ADD_UARFCN_IN_BAND_4   9

/** FDD Band 5 min UARFCN. */
#define MIN_UARFCN_IN_BAND_5   4357
/** FDD Band 5 max UARFCN. */
#define MAX_UARFCN_IN_BAND_5   4458
/** FDD Band 5 number of UARFCNs. */
#define TXRX_SEPARATION_BAND_5 225

/** FDD Band 6 min UARFCN. */
#define MIN_UARFCN_IN_BAND_6   4387
/** FDD Band 6 max UARFCN. */
#define MAX_UARFCN_IN_BAND_6   4413
/** FDD Band 6 number of UARFCNs. */
#define TXRX_SEPARATION_BAND_6 225

/** FDD Band 7 min UARFCN. */
#define MIN_UARFCN_IN_BAND_7   2237
/** FDD Band 7 max UARFCN. */
#define MAX_UARFCN_IN_BAND_7   2563
/** FDD Band 7 number of UARFCNs. */
#define TXRX_SEPARATION_BAND_7 225
/** FDD Band 7 min additional UARFCN. */
#define MIN_ADD_UARFCN_IN_BAND_7   2587
/** FDD Band 7 max additional UARFCN. */
#define MAX_ADD_UARFCN_IN_BAND_7   2912
/** FDD Band 7 number of additional UARFCNs. */
#define NUM_ADD_UARFCN_IN_BAND_7   14

/** FDD Band 8 min UARFCN. */
#define MIN_UARFCN_IN_BAND_8   2937
/** FDD Band 8 max UARFCN. */
#define MAX_UARFCN_IN_BAND_8   3088
/** FDD Band 8 number of UARFCNs. */
#define TXRX_SEPARATION_BAND_8 225

/** FDD Band 9 min UARFCN. */
#define MIN_UARFCN_IN_BAND_9   9237
/** FDD Band 9 max UARFCN. */
#define MAX_UARFCN_IN_BAND_9   9387
/** FDD Band 9 number of UARFCNs. */
#define TXRX_SEPARATION_BAND_9 475


/** Band 7 not supported by UE */
#define FDD_VALID_BAND_MASK    0x01BF


#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** Frequency bands.
 * Min and max values of UARFCN for the bands as defined
 * in 25.102 for TDD128
 */
/** TDD128 Band A LOWER min UARFCN. */
#define MIN_UARFCN_IN_TDD128_BAND_A_LOWER   9504 /* 9500 for MSC */
/** TDD128 Band A LOWER max UARFCN. */
#define MAX_UARFCN_IN_TDD128_BAND_A_LOWER   9596 /* 9600 for MSC */
/** TDD128 Band A LOWER number of UARFCNs. */
#define NUM_UARFCN_IN_TDD128_BAND_A_LOWER   ((MAX_UARFCN_IN_TDD128_BAND_A_LOWER + 1) - MIN_UARFCN_IN_TDD128_BAND_A_LOWER)

/** TDD128 Band A UPPER min UARFCN. */
#define MIN_UARFCN_IN_TDD128_BAND_A_UPPER   10054 /* 10050 for MSC */
/** TDD128 Band A UPPER max UARFCN. */
#define MAX_UARFCN_IN_TDD128_BAND_A_UPPER   10121 /* 10125 for MSC */
/** TDD128 Band A UPPER number of UARFCNs. */
#define NUM_UARFCN_IN_TDD128_BAND_A_UPPER   ((MAX_UARFCN_IN_TDD128_BAND_A_UPPER + 1) - MIN_UARFCN_IN_TDD128_BAND_A_UPPER)

/** TDD128 Band B LOWER min UARFCN. */
#define MIN_UARFCN_IN_TDD128_BAND_B_LOWER   9254 /* 9250 for MSC */
/** TDD128 Band B LOWER max UARFCN. */
#define MAX_UARFCN_IN_TDD128_BAND_B_LOWER   9546 /* 9550 for MSC */
/** TDD128 Band B LOWER number of UARFCNs. */
#define NUM_UARFCN_IN_TDD128_BAND_B_LOWER   ((MAX_UARFCN_IN_TDD128_BAND_B_LOWER + 1) - MIN_UARFCN_IN_TDD128_BAND_B_LOWER)

/** TDD128 Band B UPPER min UARFCN. */
#define MIN_UARFCN_IN_TDD128_BAND_B_UPPER   9654 /* 9650 for MSC */
/** TDD128 Band B UPPER max UARFCN. */
#define MAX_UARFCN_IN_TDD128_BAND_B_UPPER   9946 /* 9950 for MSC */
/** TDD Band B UPPER number of UARFCNs. */
#define NUM_UARFCN_IN_TDD128_BAND_B_UPPER   ((MAX_UARFCN_IN_TDD128_BAND_B_UPPER + 1) - MIN_UARFCN_IN_TDD128_BAND_B_UPPER)

/** TDD128 Band C min UARFCN. */
#define MIN_UARFCN_IN_TDD128_BAND_C         9554 /* 9550 for MSC */
/** TDD128 Band 19 max UARFCN. */
#define MAX_UARFCN_IN_TDD128_BAND_C         9596 /* 9650 for MSC */
/** TDD128 Band 19 number of UARFCNs. */
#define NUM_UARFCN_IN_TDD128_BAND_C         ((MAX_UARFCN_IN_TDD128_BAND_C + 1) - MIN_UARFCN_IN_TDD128_BAND_C)

/** TDD128 Band 19 min UARFCN. */
#define MIN_UARFCN_IN_TDD128_BAND_F         9404 /* 9400 for MSC */
/** TDD128 Band 19 max UARFCN. */
#define MAX_UARFCN_IN_TDD128_BAND_F         9596 /* 9600 for MSC */
/** TDD128 Band 19 number of UARFCNs. */
#define NUM_UARFCN_IN_TDD128_BAND_F         ((MAX_UARFCN_IN_TDD128_BAND_F + 1) - MIN_UARFCN_IN_TDD128_BAND_F)

/** TDD128 Band 23 min UARFCN. */
#define MIN_UARFCN_IN_TDD128_BAND_E         11504 /* 11500 for MSC */
/** TDD128 Band 23 max UARFCN. */
#define MAX_UARFCN_IN_TDD128_BAND_E         11996 /* 12000 for MSC */
/** TDD128 Band 23 number of UARFCNs. */
#define NUM_UARFCN_IN_TDD128_BAND_E         ((MAX_UARFCN_IN_TDD128_BAND_E + 1) - MIN_UARFCN_IN_TDD128_BAND_E)

#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */

/***************************************************************************
*   Macro Functions
***************************************************************************/

/***************************************************************************
*   Types
***************************************************************************/

/** FDD Multiband identifier.
 * Used to reference a given band.
 */

typedef enum FddBandTag
{
    /** FDD band 1 */
    FDD_BAND_1             = 0,
    /** FDD band 2 */
    FDD_BAND_2             = 1,
    /** FDD band 3 */
    FDD_BAND_3             = 2,
    /** FDD band 4 */
    FDD_BAND_4             = 3,
    /** FDD band 5 */
    FDD_BAND_5             = 4,
    /** FDD band 6 */
    FDD_BAND_6             = 5,
    /** FDD band 7 */
    FDD_BAND_7             = 6,
    /** FDD band 8 */
    FDD_BAND_8             = 7,
    /** FDD band 9 */
    FDD_BAND_9             = 8,
    /** Number of FDD bands */
    NUM_FDD_BANDS,
    /** Unvalid FDD band */
    INVALID_FDD_BAND       = 0xFF
}
FddBand;

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** TDD128 Multiband identifier.
 * Used to reference a given band.
 */
typedef enum Tdd128BandTag
{
    TDD128_BAND_A_LOWER        = 0,
    TDD128_BAND_A_UPPER        = 1,
    TDD128_BAND_B_LOWER        = 2,
    TDD128_BAND_B_UPPER        = 3,
    TDD128_BAND_C              = 4,
    TDD128_BAND_F              = 5,
    TDD128_BAND_E              = 6,
    TDD128_BAND_D              = 7,    
    NUM_TDD128_BANDS,
    INVALID_TDD128_BAND        = 0xFF
}
Tdd128Band;

#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */

/** FDD Multiband bitmasks.
 * Used to reference a given band in a bitmask.
 */
typedef Int16 FddBandMode;

#define FDD_BAND_1_MODE  (FddBandMode)(FDD_BAND_1_BIT)
#define FDD_BAND_2_MODE  (FddBandMode)(FDD_BAND_2_BIT)
#define FDD_BAND_3_MODE  (FddBandMode)(FDD_BAND_3_BIT)
#define FDD_BAND_4_MODE  (FddBandMode)(FDD_BAND_4_BIT)
#define FDD_BAND_5_MODE  (FddBandMode)(FDD_BAND_5_BIT)
#define FDD_BAND_6_MODE  (FddBandMode)(FDD_BAND_6_BIT)
#define FDD_BAND_7_MODE  (FddBandMode)(FDD_BAND_7_BIT)
#define FDD_BAND_8_MODE  (FddBandMode)(FDD_BAND_8_BIT)
#define FDD_BAND_9_MODE  (FddBandMode)(FDD_BAND_9_BIT)
#define GSM_BAND_MODE    (FddBandMode)(GSM_BAND_BIT)

#define INVALID_FDD_BAND_MODE  (FddBandMode)(0xffff)

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE) || defined(UPGRADE_TWGGE_NRW)
/** TDD128 Multiband bitmasks.
 * Used to reference a given band in a bitmask.
 */
typedef Int16 Tdd128BandMode;

#define TDD128_BAND_A_LOWER_MODE  (Tdd128BandMode)(TDD128_BAND_A_LOWER_BIT)
#define TDD128_BAND_A_UPPER_MODE  (Tdd128BandMode)(TDD128_BAND_A_UPPER_BIT)
#define TDD128_BAND_B_LOWER_MODE  (Tdd128BandMode)(TDD128_BAND_B_LOWER_BIT)
#define TDD128_BAND_B_UPPER_MODE  (Tdd128BandMode)(TDD128_BAND_B_UPPER_BIT)
#define TDD128_BAND_C_MODE        (Tdd128BandMode)(TDD128_BAND_C_BIT)
#define TDD128_BAND_F_MODE        (Tdd128BandMode)(TDD128_BAND_F_BIT)
#define TDD128_BAND_E_MODE        (Tdd128BandMode)(TDD128_BAND_E_BIT)
#define TDD128_BAND_D_MODE        (Tdd128BandMode)(TDD128_BAND_D_BIT)

#define INVALID_TDD128_BAND_MODE  (Tdd128BandMode)(0xffff)


#define TDD128_VALID_BAND_MASK (Tdd128BandMode)(TDD128_BAND_A_UPPER_MODE | TDD128_BAND_F_MODE | TDD128_BAND_E_MODE)

#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */


/** FDD Multiband region bitmasks.
 * Used to define region in terms of FddBandMode bits.
 */
typedef Int16 FddRegion;

/** FDD power class.
 * Used to indicate the power class of an FDD band.
 */
typedef struct FddBandPowerClassTag
{
    /** Indicated Band. */
    FddBand         band;

    /** Indicates the power class of the band.*/
    UUE_PowerClass  powerClass;
}
FddBandPowerClass;

/** FDD power classes.
 * Defines the powerClass of all supported bands. Bands not supported
 * by L1 should be ignorred.
 */
typedef struct FddBandInfoTag
{
#if defined (UPGRADE_FDD_MULTIBAND)
    /** Indicates the number of bands. */
    Int16              numBandPowerClass;
    /** Array of band power classes. */
    FddBandPowerClass  powerClass [MAX_NUM_FDD_BANDS];
#else
    FddBandPowerClass  powerClass;
#endif
}
FddBandInfo;

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
typedef Int16 Tdd128Region;

typedef struct Tdd128BandPowerClassTag
{
   /** ue_PowerClass is in the range 1 to 4.
    * However, only power classes 2 and 3 are applicable for TDD128. */
   Tdd128Band        band;
   UUE_PowerClass    ue_PowerClass;
}
Tdd128BandPowerClass;

typedef struct Tdd128BandInfoTag
{
    /** Indicates the number of bands. */
    Int16    numBandPowerClass;

    /** Array of band power classes. */
    Tdd128BandPowerClass    powerClass [MAX_NUM_TDD128_BANDS];
}
Tdd128BandInfo;
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */

typedef union UmtsBandUnionTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    FddBand     fdd;
#endif /* UPGRADE_3G_FDD  || UPGRADE_3G_DOUBLE_DUAL_MODE */
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    Tdd128Band  tdd128;
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */
}
UmtsBandunion;

# endif /* (UPGRADE_3G) */

#endif /* UBND_CFG_H */


/* END OF FILE */

