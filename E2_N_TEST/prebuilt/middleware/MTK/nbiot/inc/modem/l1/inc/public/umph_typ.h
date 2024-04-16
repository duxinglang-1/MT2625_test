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
 * Types used in Umph primitives for dual mode
 **************************************************************************/

#ifndef UMPH_TYP_H
#define UMPH_TYP_H

#if defined (UPGRADE_GGE)
# if defined (UPGRADE_3G) || defined (UPGRADE_LTE)
/***************************************************************************
 * Nested Include Files
 **************************************************************************/
#if defined(EXCLUDE_TTPCOM_3GPS)
# include <dummy_ups.h>
#else
# include <uas_asn.h>
# include <cphy_sig.h>
#endif
#include <mph_typ.h> /* Types needed for handover command */

/***************************************************************************
 * Manifest Constants
 **************************************************************************/
#if defined(CPHY_MAX_FREQUENCIES_PER_RSSI_SCAN)
#define UMPH_MAX_UMTS_FREQUENCIES_PER_RSSI_SCAN CPHY_MAX_FREQUENCIES_PER_RSSI_SCAN
#else
#define UMPH_MAX_UMTS_FREQUENCIES_PER_RSSI_SCAN 32
#endif

#if defined(CPHY_MAX_FREQUENCIES_PER_RSSI_SCAN)
#define UMPH_MAX_FDD_FREQUENCIES_PER_RSSI_SCAN CPHY_MAX_FREQUENCIES_PER_RSSI_SCAN
#else
#define UMPH_MAX_FDD_FREQUENCIES_PER_RSSI_SCAN 32
#endif
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
#if defined(CPHY_MAX_FREQUENCIES_PER_RSSI_SCAN)
#define UMPH_MAX_TDD128_FREQUENCIES_PER_RSSI_SCAN CPHY_MAX_FREQUENCIES_PER_RSSI_SCAN
#else
#define UMPH_MAX_TDD128_FREQUENCIES_PER_RSSI_SCAN 32
#endif
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */

#define MAX_FREQ_BANDS               2
#define UMAX_NUM_UMTS_NCELLS         32
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
#define UMAX_NUM_MEAS_FDD_NCELLS     32
#endif
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
#define UMAX_NUM_MEAS_TDD128_NCELLS  54
#endif
#define UMAX_NUM_GSM_CELLS           6

#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
#define UMAX_NUM_MEAS_FDD_UTRANS     3
#endif
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
#define UMAX_NUM_MEAS_TDD128_UTRANS  9
#endif

#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
#define INVALID_FDD_SCRAMBLING_CODE 0xffff
#endif /* UPGRADE_3G_FDD  || UPGRADE_3G_DOUBLE_DUAL_MODE */

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
#define INVALID_TDD128_CELL_PARAMETER_ID 0xff
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */

/***************************************************************************
 * Type Definitions
 **************************************************************************/
typedef UUARFCN UArfcn;

#define INVALID_UUARFCN 0xFFFF

/** SIB Decoding control
 *  Turn SIB decode on/off
 */
typedef enum USysInfoBlockControlTag
{
    USIB_OFF  = 0,
    USIB_ALL  = 1
}
USysInfoBlockControl;

/** UMTS Neighbour cell measurements
 *  Turn on/off UMTS monitoring of neighbour cells
 */
typedef enum UmtsNcellMeasCtrlTag
{
    UMTS_NCELL_MEAS_OFF = 0,
    UMTS_NCELL_MEAS_ON  = 1
}
UmtsNcellMeasCtrl;

/** Abort 3G Neighbour cell measurements
 *  ALL, RSSI Scan or Find Cell.
 */
typedef enum UmtsAbortCtrlTag
{
    UMTS_ABORT_ALL           = 0,
    UMTS_ABORT_RSSI_SCAN     = 1,
    UMTS_ABORT_FIND_CELL     = 2,
    UMTS_ABORT_ONGOING_OPS   = 3
}
UmtsAbortCtrl;

/** PDU Decode Status based on CRC check
 */
typedef enum UmtsDlPduStatusTag
{
    UMTS_DL_PDU_STATUS_CRC_OK       = 0,    /**< CRC for this PDU okay */
    UMTS_DL_PDU_STATUS_CRC_ERROR    = 1,    /**< CRC for this PDU failed */
    UMTS_DL_PDU_STATUS_UNUSED                /**< he associated structure is not being used */
}
UmtsDlPduStatus;

/** Handover from Utran Command
 *  Used the main /a L1GsmHandoverReq handover data type
 */
typedef L1GsmHandoverReq UHandoverFromUtranCommand;

#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
/** Utran FDD Cells on given UARFCN
 *  Neighbour cell data for FDD cells per Uarfcn
 */
typedef struct UFddUtranElementTag
{
     UArfcn                  uArfcn;
/** \assoc ARRAY \ref primaryScramblingCode */
     Int16                   numPrimaryScramblingCodes;
     /* txDiversityIndicMapBit - Bit 0 (set to '1') indicates if Tx-diversity is on for the cell
     ** defined in primaryScramblingCode[0]. Bit 1 indicates psc[1] .. etc */
     Int32                   txDiversityIndicMap;
     UPrimaryScramblingCode  primaryScramblingCode [UMAX_NUM_UMTS_NCELLS];
}
UFddUtranElement;

/** FDD cells information to perform measurements */
typedef struct FddUtranInfoTag
{
/** \assoc ARRAY \ref fddUtranElement */
    Int8                    numFddUtrans;
    UFddUtranElement        fddUtranElement [UMAX_NUM_MEAS_FDD_UTRANS];
}
FddUtranInfo;

typedef struct FddCellMeasurementTag
{
    /** Ec/No measurement
     * This is the Ec/No measurement for the CPICH
     * in 1/8dB steps (e.g. -80 = -10dB).
     * */

    SignedInt16             cpichEcN0;

    /** RSCP measurement
     * This is the RSCP measurement for the CPICH
     * in 1/8dBm steps (e.g. -80 = -10dBm).
     * */

    SignedInt16             cpichRscp;

}
FddCellMeasurement;

typedef struct FddNCellMeasurementsTag
{

/** \assoc ARRAY \ref index
\assoc ARRAY \ref cpichEcN0
\assoc ARRAY \ref cpichRscp */
    Int8                    numMeas;

    /* For identifying the cell for the reported measurement */
    Int8                    index[UMAX_NUM_MEAS_FDD_NCELLS];

    /** Ec/No measurement
     * This is the Ec/No measurement for the CPICH
     * in 1/8dB steps (e.g. -80 = -10dB).
     * */

    SignedInt16             cpichEcN0[UMAX_NUM_MEAS_FDD_NCELLS];

    /** RSCP measurement
     * This is the RSCP measurement for the CPICH
     * in 1/8dBm steps (e.g. -80 = -10dBm).
     * */

    SignedInt16             cpichRscp[UMAX_NUM_MEAS_FDD_NCELLS];

    SignedInt16             rssi[UMAX_NUM_MEAS_FDD_UTRANS];
}
FddNCellMeasurements;
#endif /* UPGRADE_3G_FDD  || UPGRADE_3G_DOUBLE_DUAL_MODE */

#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)

/** Utran TDD128 Cells on given UARFCN
 *  Neighbour cell data for TDD128 cells per Uarfcn
 */
typedef struct UTdd128UtranElementTag
{
    UArfcn                       uArfcn;
    /** \assoc ARRAY \ref cellParametersID */
    Int16                        numCellParametersIDs;
    /* tstdIndicMap  - Bit 0 (set to '1') indicates if tstd is on for the cell
    ** defined in cellParametersID[0]. Bit 1 indicates cpi[1] .. etc */
    Int32                        tstdIndicMap;
    /* sctdIndicMap  - Bit 0 (set to '1') indicates if sctd is on for the cell
    ** defined in cellParametersID[0]. Bit 1 indicates cpi[1] .. etc */
    Int32                        sctdIndicMap;
    UCellParametersID            cellParametersID[UMAX_NUM_UMTS_NCELLS];

}
UTdd128UtranElement;

/** TDD128 cells information to perform measurements */
typedef struct Tdd128UtranInfoTag
{
/** \assoc ARRAY \ref tdd128UtranElement */
    Int8                   numTdd128Utrans;

    /* UTRAN elements */
    UTdd128UtranElement    tdd128UtranElement [UMAX_NUM_MEAS_TDD128_UTRANS];
}
Tdd128UtranInfo;

typedef struct Tdd128TxDiversityTag
{
    /* TS transmit diversity is used in the cell or not. */
    Boolean     useTstd;

    /* SC transmit diversity is used in the cell or not. */
    Boolean     useSctd;
}
Tdd128TxDiversity;

typedef struct Tdd128CellMeasurementTag
{
    /** RSCP measurement */

    SignedInt16     pccpchRscp;
}
Tdd128CellMeasurement;


typedef struct Tdd128NcellDetectedMeasTag
{
    Boolean                      cellDetected;
    UCellParametersID            cellParametersID;
    /** RSCP measurement */
    SignedInt16                  pccpchRscp;
}
Tdd128NcellDetectedMeas;

typedef struct Tdd128NCellMeasurementsTag
{
/** \assoc ARRAY \ref index
\assoc ARRAY \ref pccpchRscp */
    Int8                    numMeas;

    /* For identifying the cell for the reported measurement */
    Int16                   index[UMAX_NUM_MEAS_TDD128_NCELLS];

    /** RSCP measurement */

    SignedInt16             pccpchRscp[UMAX_NUM_MEAS_TDD128_NCELLS];

    /** Except for the above cells, strongest detected Ncell on each uarfcn. */
    Tdd128NcellDetectedMeas ncellDetectedMeas[UMAX_NUM_MEAS_TDD128_UTRANS];

    SignedInt16             rssi[UMAX_NUM_MEAS_TDD128_UTRANS];
}
Tdd128NCellMeasurements;

#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */

/** Indicates the Cell Identity for FDD or TDD128. */
typedef union UmtsCellIdentityUnionTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
     /* The Primary Scrambling Code */
     UPrimaryScramblingCode    primaryScramblingCode;
#endif /* UPGRADE_3G_FDD  || UPGRADE_3G_DOUBLE_DUAL_MODE */
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
     /* The Cell Parameters ID */
     UCellParametersID         cellParametersID;
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */
}
UmtsCellIdentityUnion;

typedef union UmtsCellMeasurementUnionTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
     /* The measurement result for Fdd */
     FddCellMeasurement        fddCellMeasurement;
#endif /* UPGRADE_3G_FDD  || UPGRADE_3G_DOUBLE_DUAL_MODE */
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
     /* The measurement result for Tdd128 */
     Tdd128CellMeasurement     tdd128CellMeasurement;
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */
}
UmtsCellMeasurementUnion;

typedef union UmtsUtranInfoUnionTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
     /* The FDD information to perform measurements */
     FddUtranInfo               fddUtranInfo;
#endif /* UPGRADE_3G_FDD  || UPGRADE_3G_DOUBLE_DUAL_MODE */
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
     /* The TDD128 information to perform measurements */
     Tdd128UtranInfo          tdd128UtranInfo;
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */
}
UmtsUtranInfoUnion;

typedef union UmtsNCellMeasurementsUnionTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
     /* The measurement result for Fdd */
     FddNCellMeasurements        fddNCellMeasurements;
#endif /* UPGRADE_3G_FDD  || UPGRADE_3G_DOUBLE_DUAL_MODE */
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
     /* The measurement result for Tdd128 */
     Tdd128NCellMeasurements     tdd128NCellMeasurements;
#endif /* UPGRADE_3G_TDD128  || UPGRADE_3G_DOUBLE_DUAL_MODE */
}
UmtsNCellMeasurementsUnion;


/** RSSI scan for cell selection.
 * GRR sends this signal to trigger a full scan of the UTRAN RSSI.
 * This signal is sent when GRR initiates Inter-RAT cell selection
 * or a PLMN search procedure.
 *
 * If numFreq greater than 0, layer 1 ust scan the frequencies defined
 * in teh freqList. If numFreq equal to 0 than layer 1 must scan all
 * frequencies in between minFreqInBand and MaxFreqInBand.
 *
 * When performing measurements on neighbouring cells,
 * if L1 is in Idle or Packet Idle mode, L1 will continue to pick up
 * paging on the current serving cell.
 */

typedef struct UmphUmtsRssiScanReqTag
{
    /**  Number of Frequencies
     * The number of frequencies to search in the freqList array

\assoc ARRAY \ref freqList */
    Int16    numFreq;

    /**  Frequencies List
     * frequencies to search as defined by numFreq
     */
    UArfcn   freqList      [UMPH_MAX_UMTS_FREQUENCIES_PER_RSSI_SCAN];

    /**
     * If numFreq equal to zero the following fields are valid. */

    /**  Minimum Frequency In Band
     * First Frequency to search in band
     */
    UArfcn   minFreqInBand;
    /**  Maximum Frequency In Band
     * Last Frequency to search in band
     */
    UArfcn   maxFreqInBand;
}
UmphUmtsRssiScanReq;

/** Measurement indication.
 * This is sent by layer 1 in reponse to a #UmphUmtsRssiScanReq.
 * This signal contains the RSSI measurements and the corresponding frequencies.
 * The utraRssiReport array is sorted in descending RSSI strength order.
 *
 */

typedef struct UmphUmtsRssiScanCnfTag
{
    /**  Number of Measurements returned
     * This is the number of measurements returned in the utraRssiReport array

\assoc ARRAY \ref utraRssiReport */
    Int16           numMeas;

    /** Number of frequencies that have been given a higher ranking by L1.
     * This can indicate that results \a utra_RssiReport[0] to
     * \a utra_RssiReport[numOfRankedFreqs - 1] have been identified as being
     * most likely to contain a UMTS carrier.  This is only applicable to band
     * scans where the corresponding \a UmphUmtsRssiScanReq.numOfFreqs is zero,
     * and then only if L1 supports some filtering or carrier
     * identification scheme.  It should be zero if not used or not applicable
     * to the scan method.
     */
    Int16           numOfRankedFreqs;

    /**  Inter-RAT RSSI Measurement Report
     * reported
     */

    UtraRssiReport  utraRssiReport [URRC_CSRR_MAX_FREQS];
}
UmphUmtsRssiScanCnf;

/** Find Cell Request
 * This signal is sent by GRR to GSM L1 to find the strongest cell in the
 * frequency specified by uArfcn. This signal requests L1 to synchronise
 * to and camp onto the strongest UTRAN cell for the requested uArfcn.
 * If a cell has been found, L1 will return a #UmphUmtsFindCellCnf
 * indicating the primary scrambling code of the cell
 *
 * If L1 is unable to find a cell for the requested uArfcn,
 * then L1 will return a UmphUmtsFindCellCnf with an indication
 * that the cell was not found.
 */

typedef struct UmphUmtsFindCellReqTag
{
    /** UArfcn to scan for cells
     * */
    UArfcn  uArfcn;
}
UmphUmtsFindCellReq;

/** Find Cell Confimation
 * L1 sends GRR this signal in response to a #UmphUmtsFindCellReq
 *
 * L1 indicates whether it successfully camped onto this cell and
 * if it was successful, reports the primary scrambling code or cell
 * parameters ID.
 */

typedef struct UmphUmtsFindCellCnfTag
{
    /** Cell Detected
     * Indicates if cell detected or not,
     * */
    Boolean                     cellDetected;

    /** Indicate it's for FDD or TDD128
\assoc UNION \ref umtsCellIdentity
\assoc UNION \ref umtsCellMeasurement */
    UmtsMode                    umtsMode;

    /* The identity of the cell detected */
    UmtsCellIdentityUnion       umtsCellIdentity;

    /* The measurement of the cell detected */
    UmtsCellMeasurementUnion    umtsCellMeasurement;
}
UmphUmtsFindCellCnf;

/** SIB Decode Indication
 * This signal is sent from L1 to GRR to return MIB's and SIB'd
 * received on the current UMTS cell as requested in
 * #UmphUmtsSibDecodeReq
 */

typedef struct UmphUmtsSibUnitDataIndTag
{
    Int16                    sfn;
    UArfcn                   uArfcn;
    /** Indicate it's for FDD or TDD128
\assoc UNION \ref umtsCellIdentity */
    UmtsMode                 umtsMode;
    /* Cell identity ex: the primary scrambling code or Cell parameters ID */
    UmtsCellIdentityUnion    umtsCellIdentity;
    /* CRC status for this PDU */
    UmtsDlPduStatus          status;
    Int8                     data [UPS_BCH_TB_SIZE_OCTETS];
}
UmphUmtsSibUnitDataInd;

/** Find Next Cell Confimation
 * L1 sends GRR this signal in response to a #UmphUmtsFindNextCellReq
 *
 * L1 indicates whether it successfully camped onto this cell and
 * if it was successful, reports the primary scrambling code as
 * with #UmphUmtsFindCellCnf
 */

typedef UmphUmtsFindCellCnf UmphUmtsFindNextCellCnf;

/** Neighbour Cell Measurement Report
 * This signal is used to report UMTS neighbour cell measurements to GRR.
 *
 * UmphFddNcellMeasInd is used to report 3G ncell info to L3. The index is used as follows:-
 * for fddUtranElement[0] the index runs from 0 .. numPrimaryScramblingCodes where 32 cells are possible.
 * for fddUtranElement[1] the index runs from 32 to (32 + numPrimaryScramblingCodes) where 32 cells are possible.
 * for fddUtranElement[2] the index runs from 64 to (64 + numPrimaryScramblingCodes) where 32 cells are possible.
 *
 * For example, the 3G cell defined by fddUtranElement[1], primaryScramblingCode[2], the index will be set to 34.
 *
 * If the 'numPrimaryScramblingCodes' is set to zero, it indicates a wideband RSSI measurememt reported in the RSSI field.
 *
 * For example, RSSI only for fddUtranElement[1] is reported in rssi[1]. RSSI is reported for all UARFCN's requested.
 * stack will determine if value should be used based on 'numPrimaryScramblingCodes' requested for that UARFCN
 *
 */
typedef struct UmphUmtsNcellMeasIndTag
{
    Int8        seqNum;

    /** Indicate it's for FDD or TDD128
\assoc UNION \ref umtsNCellMeasurements */
    UmtsMode                      umtsMode;
    UmtsNCellMeasurementsUnion    umtsNCellMeasurements;
}
UmphUmtsNcellMeasInd;

#endif /* UPGRADE_3G || UPGRADE_LTE */
#endif /* UPGRADE_GGE */
#endif



/* END OF FILE */
