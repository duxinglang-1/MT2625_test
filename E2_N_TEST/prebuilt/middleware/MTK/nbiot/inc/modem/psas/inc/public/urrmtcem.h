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
 * Header file containing 3G engineering mode constants, macros,
 * structure/datatype definitions which can be exported outside the
 * RRC.
 *
 * Do not add datatypes, function declarations etc., that would result
 * in the export of RRC specific stuff outside the RRC.
 **************************************************************************/

#if !defined(URRMTCEM_H)
#define       URRMTCEM_H

/***************************************************************************
* Nested Include Files
***************************************************************************/

#include <sir_typ.h>        /* NAS CellId, Mcc, Mnc, Lai, Rai */
#include <l13_typ.h>        /* Band enum */
#include <grrmrtyp.h>       /*GprsNetworkMode*/

#if defined (ENABLE_AT_TRACE)
#include <u1_typ.h>
#include <urlc_sig.h>
#endif

/***************************************************************************
*   Manifest Constants
***************************************************************************/

/* Engineering mode control uses a 32-bit control field to turn
 * specific eng. mode information on and off.
 *
 * Bits 1 to 2 contain general engineering mode control ....
 * (based on equivalent NAS/GRR bit flags defined in
 * psinc\grrmtyp.h)
 */

#define UENG_INFO_DISABLED                           0x00000000
#define UENG_INFO_CONTINUOUS                         0x00000002
#define UENG_INFO_SINGLE_SHOT                        0x00000004

#define UENG_INFO_ON_OFF_MASK                        0x00000006

/* Bits 3 to 12 define a reporting period in seconds.
 * A zero value will indicate that the current reporting
 * period is to be left unchanged
 */

#define UENG_INFO_REPORTING_PERIOD_BIT_SHIFT         3
#define UENG_INFO_REPORTING_PERIOD_MASK              0x00001ff8

#if defined (ENABLE_AT_TRACE)

#define UENG_INFO_AT_TARCE_MODE                      0x80000000
#define UENG_INFO_AT_TARCE_ENABLE                    0x40000000

#endif

/* Bits 13 - 17 contain RRC mode/state specific control e.g
 * for which RRC mode/states do we want engineering mode
 * information (probably all)
 * IDLE, URA_PCH, CELL_PCH, CELL_FACH, CELL_DCH.
 *
 * Excludes RRC DEACTIVATED and SUSPENDED modes
 * (signals will be sent out, but will not contain
 * and valid information apart from the RRC mode/state)
 */
#define UENG_INFO_IDLE_MODE                          0x00002000
#define UENG_INFO_CONNECTED_URA_PCH                  0x00004000
#define UENG_INFO_CONNECTED_CELL_PCH                 0x00008000
#define UENG_INFO_CONNECTED_CELL_FACH                0x00010000
#define UENG_INFO_CONNECTED_CELL_DCH                 0x00020000

#define UENG_INFO_CONNECTED_STATES   \
    (UENG_INFO_CONNECTED_URA_PCH   | \
     UENG_INFO_CONNECTED_CELL_PCH  | \
     UENG_INFO_CONNECTED_CELL_FACH | \
     UENG_INFO_CONNECTED_CELL_DCH)

#define UENG_INFO_ALL_MODE_STATES \
    (UENG_INFO_CONNECTED_STATES | \
     UENG_INFO_IDLE_MODE)

#define UENG_INFO_MODE_STATE_MASK                    UENG_INFO_ALL_MODE_STATES

/* Bits 18 to 22 identify the information to be returned */
#define UENG_INFO_UE_OPERATION                       0x00040000
#define UENG_INFO_SERVING_CELL                       0x00080000
#define UENG_INFO_INTRA_FREQ_CELLS                   0x00100000
#define UENG_INFO_INTER_FREQ_CELLS                   0x00200000
#define UENG_INFO_INTER_RAT_GSM_CELLS                0x00400000

#define UENG_INFO_ALL_DATA        \
    (UENG_INFO_UE_OPERATION     | \
     UENG_INFO_SERVING_CELL     | \
     UENG_INFO_INTRA_FREQ_CELLS | \
     UENG_INFO_INTER_FREQ_CELLS | \
     UENG_INFO_INTER_RAT_GSM_CELLS)

#define UENG_INFO_DATA_MASK                          UENG_INFO_ALL_DATA

#define UENG_DEFAULT_REPORTING_PERIOD_SECONDS        30

/* Constants used to flag invalid values
 * in the engineering mode structures */
#define UENG_INVALID_SERVING_CELL_ID                 4000000000UL /* Int32 ID */
#define UENG_INVALID_CELL_ID                         0xff        /* Int8 ID  */

#define UENG_INVALID_MCC                             0xffff
#define UENG_INVALID_MNC                             0xffff
#define UENG_INVALID_LAC                             0xfffe
#define UENG_INVALID_RAC                             0x00u

#define UENG_INVALID_ARFCN                           0xffff
#define UENG_INVALID_PRIMARY_SCRAMBLING_CODE         0xffff
#define UENG_INVALID_URA_ID                          0xffff
#define UENG_INVALID_DRX_CYCLE_LENGTH                0xffff

#define UENG_INVALID_NUMBER_OF_RADIO_LINKS           0xff
#define UENG_INVALID_SRNC_IDENTITY                   0xffff
#define UENG_INVALID_S_RNTI                          0xffffffff

#define UENG_INVALID_TX_POWER                        -32768 /* SHRT_MIN */
#define UENG_INVALID_RSSI                            -32768

#define UENG_INVALID_REF_TIME_DIFFERENCE_TO_CELL     0xffff

#define UENG_INVALID_CPICH_RSCP                      -32768
#define UENG_INVALID_CPICH_EC_N0                     -32768
#define UENG_INVALID_UTRA_CARRIER_RSSI               -32768
#define UENG_INVALID_PATHLOSS                        -32768
#define UENG_INVALID_QUAL                            -32768
#define UENG_INVALID_RXLEV                           -32768

#define UENG_INVALID_GSM_CARRIER_RSSI                -32768
#define UENG_INVALID_BSIC_NCC                        0xff
#define UENG_INVALID_BSIC_BCC                        0xff

#ifdef UPGRADE_3G_TDD128

# define UENG_INVALID_CELL_PARAMETERS_ID             0xff
# define UENG_INVALID_PCCPCH_RSCP                    -32768
#endif /* UPGRADE_3G_TDD128 */

/***************************************************************************
*   Macro Functions
***************************************************************************/

#define UENG_ENCODE_REPORTING_PERIOD(cONFIGmASK, pERIOD) \
    (((cONFIGmASK)                                       \
      & ~UENG_INFO_REPORTING_PERIOD_MASK)                \
     |                                                   \
     (((pERIOD) << UENG_INFO_REPORTING_PERIOD_BIT_SHIFT) \
      &  UENG_INFO_REPORTING_PERIOD_MASK)                \
    )

#define UENG_EXTRACT_REPORTING_PERIOD(cONFIGmASK)     \
    (((cONFIGmASK) & UENG_INFO_REPORTING_PERIOD_MASK) \
     >> UENG_INFO_REPORTING_PERIOD_BIT_SHIFT)

#define UENG_RRC_IS_CONNECTED_MODE(mODEsTATE)       \
    ((mODEsTATE) == UENG_RRC_CONNECTED_URA_PCH   || \
     (mODEsTATE) == UENG_RRC_CONNECTED_CELL_PCH  || \
     (mODEsTATE) == UENG_RRC_CONNECTED_CELL_FACH || \
     (mODEsTATE) == UENG_RRC_CONNECTED_CELL_DCH)

#define UENG_RRC_MODE_NAME(mODEsTATE)                         \
    (UENG_RRC_IS_CONNECTED_MODE (mODEsTATE) ? "CONNECTED"   : \
     (mODEsTATE) == UENG_RRC_IDLE          ? "IDLE"        :  \
     (mODEsTATE) == UENG_RRC_SUSPENDED     ? "SUSPENDED"   :  \
     (mODEsTATE) == UENG_RRC_DEACTIVATED   ? "DEACTIVATED" :  \
     "??")

#define UENG_RRC_STATE_NAME(mODEsTATE)                           \
    ((mODEsTATE) == UENG_RRC_CONNECTED_URA_PCH   ? "URA PCH"   : \
     (mODEsTATE) == UENG_RRC_CONNECTED_CELL_PCH  ? "CELL PCH"  : \
     (mODEsTATE) == UENG_RRC_CONNECTED_CELL_FACH ? "CELL FACH" : \
     (mODEsTATE) == UENG_RRC_CONNECTED_CELL_DCH  ? "CELL DCH"  : \
     ((mODEsTATE) == UENG_RRC_IDLE       ||                      \
      (mODEsTATE) == UENG_RRC_SUSPENDED  ||                      \
      (mODEsTATE) == UENG_RRC_DEACTIVATED)       ? "NA"        : \
     "??")

#define UENG_SCALE_TO_DB(vALUE) \
    ((vALUE) / RRC_SYS_INFO_DB_SCALING_FACTOR)

#define UENG_CIPHERING_ALGORITHM_NAME(aLGORITHMiD)        \
    ((aLGORITHMiD) == UCipheringAlgorithm_uea0 ? "UEA0" : \
     (aLGORITHMiD) == UCipheringAlgorithm_uea1 ? "UEA1" : \
     "??")

#define UENG_GSM_BAND_INDICATOR_NAME(bANDiD)                   \
    ((bANDiD) == UFrequency_Band_dcs1800BandUsed ? "DCS1800" : \
     (bANDiD) == UFrequency_Band_pcs1900BandUsed ? "PCS1900" : \
     "??")

#define UENG_BARRED_STATUS_NAME(bARREDsTATUS)                          \
    ((bARREDsTATUS) == UENG_NOT_BARRED            ? "Not Barred"     : \
     (bARREDsTATUS) == UENG_BARRED_NO_SERVICE     ? "No Service"     : \
     (bARREDsTATUS) == UENG_BARRED_EMERGENCY_ONLY ? "Emergency Only" : \
     "??")

#define UENG_INTRA_FREQ_BARRING_TYPE_NAME(bARRINGtYPE)                                  \
    ((bARRINGtYPE) == UENG_NO_INTRA_FREQ_BARRING                ? "Freq Not Barred"   : \
     (bARRINGtYPE) == UENG_INTRA_FREQ_BARRING                   ? "Freq Barred"       : \
     (bARRINGtYPE) == UENG_INTRA_FREQ_BARRING_MAX_TBARRED_TIMER ? "Max TBarred Timer" : \
     "??")

#define UENG_ALLOWED_INDICATOR_NAME(aLLOWED)                     \
    ((aLLOWED) == UAllowedIndicator_allowed    ? "Allowed"     : \
     (aLLOWED) == UAllowedIndicator_notAllowed ? "Not Allowed" : \
     "??")

#define UENG_RESERVED_INDICATOR_NAME(rESERVED)                       \
    ((rESERVED) == UReservedIndicator_reserved    ? "Reserved"     : \
     (rESERVED) == UReservedIndicator_notReserved ? "Not Reserved" : \
     "??")

#define UENG_UT_BARRED_NAME(uTbARRED)         \
    ((uTbARRED) == UT_Barred_s10   ? "10"   : \
     (uTbARRED) == UT_Barred_s20   ? "20"   : \
     (uTbARRED) == UT_Barred_s40   ? "40"   : \
     (uTbARRED) == UT_Barred_s80   ? "80"   : \
     (uTbARRED) == UT_Barred_s160  ? "160"  : \
     (uTbARRED) == UT_Barred_s320  ? "320"  : \
     (uTbARRED) == UT_Barred_s640  ? "640"  : \
     (uTbARRED) == UT_Barred_s1280 ? "1280" : \
     "??")

/***************************************************************************
*   Types
***************************************************************************/

/**
 * Enumerated RRC mode and connected states.
 */
typedef enum UEngRrcModeStateTag
{
    /** RRC Deactivated */
    UENG_RRC_DEACTIVATED,

    /** RRC Suspended */
    UENG_RRC_SUSPENDED,

    /** RRC idle */
    UENG_RRC_IDLE,

    /** URA_PCH state in UTRA RRC connected mode (Neither DCCH nor DTCH are available in these states) */
    UENG_RRC_CONNECTED_URA_PCH,

    /** CELL_PCH state in UTRA RRC connected mode (Neither DCCH nor DTCH are available in these states) */
    UENG_RRC_CONNECTED_CELL_PCH,

    /** CELL_FACH state in UTRA RRC connected mode (DCCH and, if configured, DTCH are available in this state) */
    UENG_RRC_CONNECTED_CELL_FACH,

    /** CELL_DCH state in UTRA RRC connected mode (DCCH and, if configured, DTCH are available in this state)*/
    UENG_RRC_CONNECTED_CELL_DCH
}
UEngRrcModeState;

/** Barred status element */
typedef enum UEngBarredStatusTag
{
    /** Not Barred; UE shall treat this cell as candidate during the cell selection
     * and cell re-selection procedures in Idle mode and in Connected mode
     */
    UENG_NOT_BARRED,

    /** No Service available on this cell */
    UENG_BARRED_NO_SERVICE,

    /** The UE is permitted to select/re-select this cell only for emergency calls */
    UENG_BARRED_EMERGENCY_ONLY
}
UEngBarredStatus;

/** Engineering mode intra frequency barring type */
typedef enum UEngIntraFreqBarringTypeTag
{
    /** Not intra-frequency Barred; the UE may select another cell on the same frequency if
     * selection/re-selection criteria are fulfilled
     */
    UENG_NO_INTRA_FREQ_BARRING,

    /** Intra-frequency Barred; the UE shall not re-select a cell on the same frequency as the barred cell */
    UENG_INTRA_FREQ_BARRING,

    /** Max TBarred Timer; the UE shall after expiry of the time interval Tbarred again check whether the
     * status of the barred cell has changed */
    UENG_INTRA_FREQ_BARRING_MAX_TBARRED_TIMER
}
UEngIntraFreqBarringType;

/**
 * Cell camp information.
 */
typedef struct UEngCellCampReSelectTag
{
    /** Sintra inf */
    Boolean                  intraSearchPresent;

    SignedInt16              intraSearch;

    /** Sinter inf */
    Boolean                  interSearchPresent;

    SignedInt16              interSearch;

#if defined (UPGRADE_GGE)
    /** Sinter-RAT inf */
    Boolean                  interRatPresent;

    SignedInt16              interRatsearch;
#endif
    /** Qhysts inf */
    Int16                    hysts;

    /** Tresel inf */
    Int8                     tReselections;
}
UEngCellCampReSelect;


/**
 * Cell access restriction information.
 */
typedef struct UEngCellAccessRestrictionsTag
{
    /** Whether or not the cell is barred */
    UCellBarred              cellBarred;

    /** Indicates if cell reserved for operator use */
    UReservedIndicator       resForOpUse;

    /** Whether or not the cell is reserved for future extension */
    UReservedIndicator       resExtension;

    /** Access class barred 16-bit map */
    Int16                    accessControlClass;

    /** Intra-frequency barring type */
    UEngIntraFreqBarringType freqBarring;
}
UEngCellAccessRestrictions;

/**
 * Common Serving cell parameter information.
 */
typedef struct UEngServingCellCommonParamTag
{
    /** ARFCN downlink of the serving cell */
    Int16 arfcn_dl;

    /** URA identity; gives the identity of the UTRAN Registration Area,
     * it can be used to indicate to the UE which URA it shall use in case
     * of overlapping URAs
     */
    Int16   uraId;

    /** Whether or not Hierarchical Cell Structure (HCS) is used */
    Boolean hcsUsed;

    /** The CS domain DRX cycle length; the time interval between reading initial
     * paging information for a specific UE in CS domain */
    Int16   csDrxCycleLength;

    /** The PS domain DRX cycle length; the time interval between reading initial
     * paging information for a specific UE in PS domain */
    Int16   psDrxCycleLength;

    /** UTRAN DRX cycle length for each CN */
    Int16   utranDrxCycleLength;

    /* PS domain items  - also carried by
     * MM engineering mode signal: MmrInfoInd */

    /** Routing Area Code is a fixed length of 1 octet and identifies a routing area within a
     * location area; the RAC is part of the RAI (Routing Area Identity)
     */
    Rac             rac;

    /** Specifies the mode of GPRS network */
    GprsNetworkMode gprsNetworkMode;
}
UEngServingCellCommonParam;

/**
 * Intra-Frequency Common cell parameter information.
 */
typedef struct UEngIntraFreqCommonCellParamsTag
{
    /** Indicates if the intra-frequency cell is in the active set */
    Boolean isActive;

    /** The reference time difference to cell
     * indicates the timing difference between the primary CCPCH of the current
     * cell and the primary CCPCH of a neighbouring cell.
     */
    Int16 referenceTimeDifferenceToCell;
}
UEngIntraFreqCommonCellParams;

/**
 * Inter-Frequency Common cell parameter information.
 */
typedef struct UEngInterFreqCommonCellParamsTag
{
    /** The DL ARFCN of the inter-frequncy cell */
    Int16 arfcn_dl;

    /** The reference time difference to cell
     * indicates the timing difference between the primary CCPCH of the current
     * cell and the primary CCPCH of an inter-frequncy cell.
     */
    Int16 referenceTimeDifferenceToCell;
}
UEngInterFreqCommonCellParams;

#if defined (UPGRADE_3G_FDD)

/**
 * FDD Serving Cell measurement information.
 */
typedef struct UEngServingFddCellParamsAndMeasTag
{
    /** Primary scrambling code */
    Int16       primaryScramblingCode;

    /** CPICH (RSCP)used as a measurement quantity in the measurement report for the serving cell;
     * in units of 1/8 dBm */
    SignedInt16 cpich_RSCP;

    /** CPICH Ec/N0 used as the quality measure for determining which cell to choose when
     * performing a cell selection or mobile-initiated cell reselection; in units of 1/8 dB
     */
    SignedInt16 cpich_Ec_N0;

    /** Minimum Required Quality Level is the minimum required quality level in the cell (dB)used
     * to determine if it may select/reselect to the cell; in units of 1/8 dB i.e, (-24 to 0 )
     */
    SignedInt16 q_QualMin;

    /** Minimum Required Rx Level is the minimum required RX level in the cell (dBm)used
     * to determine if it may select/reselect to the cell; in units of 1/8 dBm
     */
    SignedInt16 q_RxlevMin;

    /** Maximum allowed UL TX power in units of 1/8 dBm */
    SignedInt16 txPowerMax;

    /** serving cell S criteria */
    SignedInt16 servingSresult;

    /** serving cell R criteria */
    SignedInt16 servingRresult;
}
UEngServingFddCellParamsAndMeas;

#endif /* UPGRADE_3G_FDD */
#if defined (UPGRADE_3G_TDD128)

/**
 * TDD128 Serving Cell measurement information.
 */
typedef struct UEngServingTdd128CellParamsAndMeasTag
{
    /** Cell parameters identity */
    Int8 cellParametersId;

    /** Received Signal Code Power, the received power on one code measured on the PCCPCH from
     * a TDD cell (The reference point for the RSCP shall be the antenna connector of the UE);
     * in units of 1/8 dB
     */
    SignedInt16 p_ccpch_RSCP;

    /** Minimum Required Rx Level is the minimum required RX level in the cell (dBm)used
     * to determine if it may select/reselect to the cell;  in units of 1/8 dBm
     */
    SignedInt16 q_RxlevMin;

    /** Maximum allowed UL TX power in units of 1/8 dBm */
    SignedInt16 txPowerMax;

    /** serving cell S criteria */
    SignedInt16 servingSresult;

    /** serving cell R criteria */
    SignedInt16 servingRresult;
}
UEngServingTdd128CellParamsAndMeas;

#endif /* UPGRADE_3G_TDD128 */
/** Engineering mode serving cell parameters and measurement */
typedef union UEngServingCellParamsAndMeasUnionTag
{
#if defined (UPGRADE_3G_FDD)
    /** FDD serving cell parameters and measurement */
    UEngServingFddCellParamsAndMeas    fdd;
#endif
#if defined (UPGRADE_3G_TDD128)
    /** TDD128 serving cell parameters and measurement */
    UEngServingTdd128CellParamsAndMeas tdd128;
#endif
}
UEngServingCellParamsAndMeasUnion;

/**
 * Engineering mode information about the serving cell.
 */
typedef struct UEngServingCellInfoTag
{
    /** Cell ID of the serving cell */
    UCellIdentity                     cellId;

    /** LAI; Identifies uniquely a location area for a GSM-MAP type of PLMN */
    Lai                               lai;

    /** Common parameters for a serving cell */
    UEngServingCellCommonParam        common;

    /** Specifies if FDD or TDD128 mode
\assoc UNION \ref mode */
    UmtsMode                          umtsMode;

    /** Mode specific paramters for a serving cell */
    UEngServingCellParamsAndMeasUnion mode;

    /** Indicates the restrictions to cell access */
    UEngCellAccessRestrictions        accessRestrictions;

    /** Barred status indicates how UE shall treat this cell */
    UEngBarredStatus                  barredStatus;

    /** Indicates the cell SIB3 inf */
    UEngCellCampReSelect              cellCampInf;
}
UEngServingCellInfo;

#if defined (UPGRADE_3G_FDD)

/**
 * Intra-Frequency FDD cell measurement information.
 */
typedef struct UEngIntraFreqFddCellParamsAndMeasTag
{
    /** Primary scrambling code */
    Int16       primaryScramblingCode;

    /** CPICH (RSCP)used as a measurement quantity in the measurement report for the serving cell;
     * in units of 1/8 dBm */
    SignedInt16 cpich_RSCP;

    /** CPICH Ec/N0 used as the quality measure for determining which cell to choose when
     * performing a cell selection or mobile-initiated cell reselection; in units of 1/8 dB
     */
    SignedInt16 cpich_Ec_N0;

    /** Path loss in units in 1/8 dB */
    SignedInt16 pathloss;

    /** Minimum Required Quality Level is the minimum required quality level in the cell (dB)used
     * to determine if it may select/reselect to the cell; in units of 1/8 dB i.e, (-24 to 0 )
     */
    SignedInt16 q_QualMin;

    /** Minimum Required Rx Level is the minimum required RX level in the cell (dBm)used
     * to determine if it may select/reselect to the cell; in units of 1/8 dBm.
     */
    SignedInt16 q_RxlevMin;

    /** Maximum allowed UL TX power in units of 1/8 dBm */
    SignedInt16 txPowerMax;

    /** This cell reselect R value */
    SignedInt16 rValue;

    /** This cell Qoffset */
    SignedInt16  qOffset;
}
UEngIntraFreqFddCellParamsAndMeas;
#endif /* UPGRADE_3G_FDD */

#if defined (UPGRADE_3G_TDD128)

/**
 * Intra-Frequency TDD128 cell measurement information.
 */
typedef struct UEngIntraFreqTdd128CellParamsAndMeasTag
{
    /** Cell Parameter identity */
    Int8 cellParametersId;             /* modified by Aaron.cai 2008-08-19*/

    /** Received Signal Code Power, the received power on one code measured on the PCCPCH from
     * a TDD cell (The reference point for the RSCP shall be the antenna connector of the UE);
     * in units of 1/8 dBm
     */
    SignedInt16 p_ccpch_RSCP;

    /** Path loss in units in 1/8 dB */
    SignedInt16 pathloss;

    /** Minimum Required Rx Level is the minimum required RX level in the cell (dBm)used
     * to determine if it may select/reselect to the cell; in units of 1/8 dBm
     */
    SignedInt16 q_RxlevMin;

    /** Maximum allowed UL TX power in units of 1/8 dBm */
    SignedInt16 txPowerMax;

    /** This cell reselect R value */
    SignedInt16 rValue;

    /** This cell Qoffset */
    SignedInt16  qOffset;
}
UEngIntraFreqTdd128CellParamsAndMeas;

#endif /* UPGRADE_3G_TDD128 */

/** Engineering mode intra-frequency cell parameters and measurement */
typedef union UEngIntraFreqCellParamsAndMeasUnionTag
{
#if defined (UPGRADE_3G_FDD)
    /** FDD intra freq cell parameters and measurement */
    UEngIntraFreqFddCellParamsAndMeas    fdd;
#endif
#if defined (UPGRADE_3G_TDD128)
    /** TDD128 intra freq cell parameters and measurement */
    UEngIntraFreqTdd128CellParamsAndMeas tdd128;
#endif
}
UEngIntraFreqCellParamsAndMeasUnion;

/**
 * Engineering mode information about
 * an Intra-Frequency FDD or TDD128 cell.
 */
typedef struct UEngIntraFreqCellInfoTag
{
    /** Cell Identity of the intra frequency cell */
    UIntraFreqCellID                    cellId;

    /** LAI; Identifies uniquely a location area for a GSM-MAP type of PLMN */
    Lai                                 lai;

    /** Common parameters for intra-frequency cell */
    UEngIntraFreqCommonCellParams       common;

    /** Specifies if the mode is FDD or TDD128
\assoc UNION \ref mode */
    UmtsMode                            umtsMode;

    /** Mode specific paramters for intra-frequency cell */
    UEngIntraFreqCellParamsAndMeasUnion mode;

    /** Indicates if access restrictions are present */
    Boolean                             accessRestrictionsPresent;

    /** Access restriction parameters used for cell selection/reselection purpose */
    UEngCellAccessRestrictions          accessRestrictions;
}
UEngIntraFreqCellInfo;

/**
 * Engineering mode information about the
 * Intra-Frequency FDD or TDD128 cells.
 */
typedef struct UEngIntraFreqCellListTag
{
    /** The number of the supplied intra-frequency neighbouring cells
\assoc ARRAY \ref cellInfo */
    Int8                  numberOfCells;

    /** The supplied intra-frequency neighbouring cells information list */
    UEngIntraFreqCellInfo cellInfo[maxCellMeas];
}
UEngIntraFreqCellList;

#if defined (UPGRADE_3G_FDD)

/**
 * Inter-Frequency FDD cell measurement information.
 */
typedef struct UEngInterFreqFddCellParamsAndMeasTag
{
    /** Primary scrambling code of the inter-frequency cell */
    Int16       primaryScramblingCode;

    /** CPICH (RSCP)used as a measurement quantity in the measurement report for the serving cell;
     * in units of 1/8 dBm */
    SignedInt16 cpich_RSCP;

    /** CPICH Ec/N0 used as the quality measure for determining which cell to choose when
     * performing a cell selection or mobile-initiated cell reselection; in units of 1/8 dB
     */
    SignedInt16 cpich_Ec_N0;

    /** The received wide band power, including thermal noise and noise generated in the receiver,
     * within the bandwidth defined by the receiver pulse shaping filter (The reference point for the
     * measurement shall be the antenna connector of the UE); it is in 1/8 dBm steps,
     * for example: -400 = -50 dBm.
     */
    SignedInt16 utra_carrierRSSI;

    /** Path lossin units in 1/8 dB*/
    SignedInt16 pathloss;

    /** Minimum Required Quality Level is the minimum required quality level in the cell (dB)used
     * to determine if it may select/reselect to the cell; in units of 1/8 dB
     */
    SignedInt16 q_QualMin;

    /** Minimum Required Rx Level is the minimum required RX level in the cell (dBm)used
     * to determine if it may select/reselect to the cell; in units of 1/8 dBm
     */
    SignedInt16 q_RxlevMin;

    /** Maximum allowed UL TX power in units of 1/8 dBm  */
    SignedInt16 txPowerMax;

    /** This cell reselect R value */
    SignedInt16 rValue;

    /** This cell Qoffset */
    SignedInt16 qOffset;
}
UEngInterFreqFddCellParamsAndMeas;
#endif /* UPGRADE_3G_FDD */

#if defined (UPGRADE_3G_TDD128)

/**
 * Inter-Frequency TDD128 cell measurement information.
 */
typedef struct UEngInterFreqTdd128CellParamsAndMeasTag
{
    /** Cell parameters identity */
    Int16 cellParametersId;

    /** Received Signal Code Power, the received power on one code measured on the PCCPCH from
     * a TDD cell (The reference point for the RSCP shall be the antenna connector of the UE);
     * in units of 1/8 dBm
     */
    SignedInt16 p_ccpch_RSCP;

    /** The received wide band power, including thermal noise and noise generated in the receiver,
      * within the bandwidth defined by the receiver pulse shaping filter (The reference point for the
      * measurement shall be the antenna connector of the UE)
      */
    SignedInt16 utra_carrierRSSI;

    /** path loss in 1/8 dB */
    SignedInt16 pathloss;

    /** Minimum Required Rx Level is the minimum required RX level in the cell (dBm)used
     * to determine if it may select/reselect to the cell; in units in 1/8 dBm
     */
    SignedInt16 q_RxlevMin;

    /** Maximum allowed UL TX power in units in 1/8 dBm */
    SignedInt16 txPowerMax;

    /** This cell reselect R value */
    SignedInt16 rValue;

    /** This cell Qoffset */
    SignedInt16 qOffset;
}
UEngInterFreqTdd128CellParamsAndMeas;

#endif /* UPGRADE_3G_TDD128 */

/** Engineering mode inter-frequency cell parameters and measurement */
typedef union UEngInterFreqCellParamsAndMeasUnionTag
{
#if defined (UPGRADE_3G_FDD)
    /** Engineering mode inter-frequency FDD cell parameters and measurement */
    UEngInterFreqFddCellParamsAndMeas    fdd;
#endif
#if defined (UPGRADE_3G_TDD128)
    /** Engineering mode inter-frequency TDD128 cell parameters and measurement */
    UEngInterFreqTdd128CellParamsAndMeas tdd128;
#endif
}
UEngInterFreqCellParamsAndMeasUnion;

/**
 * Engineering mode information about
 * an Inter-Frequency FDD or TDD128 cell.
 */
typedef struct UEngInterFreqCellInfoTag
{
    /** Cell Identity of the inter frequency cell */
    UInterFreqCellID                    cellId;

    /** LAI; Identifies uniquely a location area for a GSM-MAP type of PLMN */
    Lai                                 lai;

    /** Common parameters for inter-frequency cell */
    UEngInterFreqCommonCellParams       common;

    /** Specifies if FDD or TDD128 mode
\assoc UNION \ref mode */
    UmtsMode                            umtsMode;

    /** Mode specific paramters for intra-frequency cell */
    UEngInterFreqCellParamsAndMeasUnion mode;

    /** Indicates if access restrictions are present */
    Boolean                             accessRestrictionsPresent;

    /** Indicates the restrictions to cell access */
    UEngCellAccessRestrictions          accessRestrictions;
}
UEngInterFreqCellInfo;

/**
 * Engineering mode information about the
 * Inter-Frequency FDD and TDD128 cells.
 */
typedef struct UEngInterFreqCellListTag
{
    /** The number of the supplied inter-frequency neighbouring cells
\assoc ARRAY \ref cellInfo */
    Int8                  numberOfCells;

    /** The supplied inter-frequency neighbouring cells information list */
    UEngInterFreqCellInfo cellInfo[maxCellMeas];
}
UEngInterFreqCellList;

#if defined (UPGRADE_GGE)

/**
 * GSM cell measurement information.
 */
typedef struct UEngGsmCellMeasTag
{
    /** Received Signal Strength Indicator, the wide-band received power within the
     * relevant channel bandwidth. Measurement shall be performed on a GSM BCCH carrier;
     * the reference point for the RSSI shall be the antenna connector of the UE.
     * \note This measurement is for handover between UTRAN and GSM.
     */
    SignedInt16 gsm_carrierRSSI;

    /** Minimum Required Rx Level is the minimum required RX level in the cell (dBm)used
     * to determine if it may select/reselect to the cell
     */
    SignedInt16 q_RxlevMin;
}
UEngGsmCellMeas;

/**
 * GSM cell parameter information.
 */
typedef struct UEngBsicTag
{
    /** Network Colour Code (NCC);
     * The first/leftmost bit of the bit string contains the
     * most significant bit of the NCC
     */
    Int8 ncc;

    /** Base Station Colour Code (BCC);
    * The first/leftmost bit of the bit string contains the most
    * significant bit of the BCC
    */
    Int8 bcc;
}
UEngBsic;

/**
 * GSM cell parameter information.
 */
typedef struct UEngGsmCellEngParamsTag
{
    /** BCCH ARFCN for GSM cell */
    Int16           bcch_arfcn;

    /** GSM band indicator (Indicates how to interpret the BCCH ARFCN) */
    UFrequency_Band gsmBandIndicator;

    /** Base transceiver Station Identity Code; this is a code in GSM which is
     * broadcast on the SCH (Synchronization Channel) in order to identify the NCC
     *(Network Colour Code) and the BCC (Base Station Colour Code)
     */
    UEngBsic bsic;
}
UEngGsmCellParams;

/**
 * Engineering mode information about a  GSM cell.
 */
typedef struct UEngGsmCellEngInfoTag
{
    /** GSM cell ID */
    UInterRATCellID cellId;

    /** The Location Area Identity uniquely identifies a LA (Location Area)
     * within any PLMN (Public Land Mobile Network); it is comprised of the
     * MCC (Mobile Country Code), MNC (Mobile Network Code) and the LAC
     * (Location Area Code)
     */
    Lai               lai;

    /** Parameters of GSM cell */
    UEngGsmCellParams params;

    /** Measurement of GSM cell */
    UEngGsmCellMeas   meas;
}
UEngGsmCellInfo;

/**
 * Engineering mode information about the GSM cells.
 */
typedef struct UEngGsmCellListTag
{
    /** The number of the supplied GSM cells in the list
\assoc ARRAY \ref cellInfo */
    Int8            numberOfCells;

    /** Holds information for each GSM cell in the list */
    UEngGsmCellInfo cellInfo[maxCellMeas];
}
UEngGsmCellList;
#endif /* UPGRADE_GGE */

/**
 * GSM cell parameter information.
 */
typedef struct UEngRntiTag
{
    /** SRNC identity (The SRNC is responsible for the users mobility within
     * the UTRAN and is also the point of connection towards the CN)
     */
    Int16 srnc_Identity;

    /** Is the UE identifier which is allocated by the serving RN and is unique whithin that SRNC;
     * it is allocated to all UEs having RRC connection; S-RNTI is reallocated when the serving RNC
     * for the RRC connection is changed and de-allocated when the RRC connection is released
     */
    Int32 s_RNTI;
}
UEngRnti;

/**
 * Ciphering Information
 */
typedef struct UEngCiperingInfoTag
{
    /** Specifies ciphering algorithm (UEA0 = no ciphering , UEA1 = ciphering) */
    UCipheringAlgorithm_r7 algorithm;

    /** Indicates if ciphering algorithm is present */
    Boolean                algorithmPresent;

    /** Whether or not the ciphering is started */
    Boolean                started;
}
UEngCipheringInfo;

#if defined(UPGRADE_3G_HSDPA)

/**
 * HSDPA Information
 */
typedef struct UEngHsdpaInfoTag
{
    /** Whether or not the HSDPA is supported */
    Boolean hsdpaIsSupported;

    /** Indicates if HS-DSCH reception is ongoing */
    Boolean hsDschReception;
}
UEngHsdpaInfo;
#endif /* UPGRADE_3G_HSDPA */

/**
 * UE operation engineering mode information
 * (excluding RRC mode/state)
 */
typedef struct UEngUeInfoTag
{
    /** The UE having an RRC connection will be allocated a S-RNTI; the SRNC identifier together
     * with S-RNTI is a unique identifier of the RRC connection within a network; the combination
     * of SRNC identifier and S-RNTI is reffered to as U-RNTI, which is used on the radio interface
     */
    UEngRnti uRnti;

    /** Indicates the number of radio links for which it reports the Rx to Tx
    * time difference for the cells identified by the associated PSC.
    */
    Int8              numberOfRadioLinks;

    /** Either the filtered or raw Tx power data */
    SignedInt16       txPower;

    /** The measured power of a received signal*/
    SignedInt16       rssi;

    /** CS ciphering information */
    UEngCipheringInfo csCipheringInfo;

    /** PS ciphering information */
    UEngCipheringInfo psCipheringInfo;

#if defined(UPGRADE_3G_HSDPA)
    /** HSDPA information */
    UEngHsdpaInfo     hsdpaInfo;
#endif /* UPGRADE_3G_HSDPA */
}
UEngUeInfo;

/***************************************************************************
 * General Function Prototypes
 ***************************************************************************/
#if defined (ENABLE_AT_TRACE)
void UrrMtcEmSendRrcEngInfoIndWithAtTraceUL (BearerIdentity bearerIdentity, UrlcSdu * rlcSdu_p);
void UrrMtcEmSendRrcEngInfoIndWithAtTraceDL (BearerIdentity         bearerIdentity,
                                                          UTransportChannelType  trChType,
                                                          UrlcSdu              * rlcSdu_p);
#endif

#endif /* URRMTCEM_H */
/* END OF FILE */
