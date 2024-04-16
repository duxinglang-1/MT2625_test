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
 * Eutra L1 Common Data Types
 *************************************************************************/

#if !defined (EL1_TYP_H)
#define       EL1_TYP_H

/***************************************************************************
* Nested Include Files
***************************************************************************/

#include <system.h>

#if defined (UPGRADE_GGE)
#include <l13_typ.h>
#endif

#if defined (UPGRADE_3G)
#include <u1_typ.h>
#endif

#include "eps_typ.h" // For nbiot/platforms/pc/stubs/uphystubs.c compilation

/** \defgroup Layer1Eutra Eutra Layer 1
 * \ingroup PrdLayer1
 * Eutra layer 1 interface.
 */

/***************************************************************************
 *   Manifest Constants
 ***************************************************************************/
/** \def UDL_LTE_MAX_MAC_HS_PDU_DATA_OCTETS
 *  Maximum size of MAC HS PDU data, in octets.
 */
#define UDL_LTE_MAX_MAC_HS_PDU_DATA_OCTETS  (9422)

/** /def UPHY_MAXIMUM_ASSIGNED_LTE_POINTERS
 *  Define the maximum assigned EuphyAssignPointersReq blocks at any one time
 */
#define UPHY_MAXIMUM_ASSIGNED_LTE_POINTERS  (2)

/** \def NUMBER_UPLINK_HARQ_PROCESSES
 *  Number of uplink harq processes.
 */
#define NUMBER_UPLINK_HARQ_PROCESSES        (8)

/** \def MAX_IMSI_SIZE_DIV2
 *  Number of pairs of IMSI digits.
 */
#define MAX_IMSI_SIZE_DIV2                  (8)

/** \def EUTRA_MAX_NUM_SPECIAL_SF_CONFIGS
 *   The maximum number of different configurations for a special subframe.
 */
#define EUTRA_MAX_NUM_SPECIAL_SF_CONFIGS    (9)

/** \def NUMBER_SUBCARRIERS_PER_RB
 *   The number of subcarriers per resource block.
 */
#define NUMBER_SUBCARRIERS_PER_RB           (12)

/** \def ECPHY_MAX_REPORTS
 * Defines the maximum number of measurement reports supported on this platform. */
#define ECPHY_MAX_REPORTS                   (8)

/** \def ECPHY_MAX_FREQUENCIES_PER_RSSI_SCAN
 * Defines the maximum number of frequencies per RSSI scan supported by
 * the physical layer. It is used when a list of frequencies is given in
 * #EcphyRssiScanReq primitive during initial cell selection or PLMN search.
 */
#define ECPHY_MAX_FREQUENCIES_PER_RSSI_SCAN    (20)

/** \def ECPHY_MAX_FREQUENCIES_IN_RSSI_REPORT
 * Defines the maximum number of frequencies per frequency band for LTE.
 */
#define ECPHY_MAX_FREQUENCIES_IN_RSSI_REPORT   (20)

/** \def MIB_SIZE_IN_OCTETS
 * Defines the size of the MIB in Octets. This is a fixed size of 3 octets.
 */
#define MIB_SIZE_IN_OCTETS                           3

/** \def EL1_TDD_ULDL_CONFIG_X
 * Defines the valid values for TDD UL/DL Configuration
 */
#define EL1_TDD_ULDL_CONFIG_0                   (0)
#define EL1_TDD_ULDL_CONFIG_1                   (1)
#define EL1_TDD_ULDL_CONFIG_2                   (2)
#define EL1_TDD_ULDL_CONFIG_3                   (3)
#define EL1_TDD_ULDL_CONFIG_4                   (4)
#define EL1_TDD_ULDL_CONFIG_5                   (5)
#define EL1_TDD_ULDL_CONFIG_6                   (6)

/** \def EL1_DB_SCALING_FACTOR
 * Defines the value for DB scaling factor to get dB and dBm values in 1/8 dB(m) unit.
 */
/* 2017-01-18 Hannu Huusko: Changed from 8 to 1, because scaling is not needed in NB-IoT? */
#define EL1_DB_SCALING_FACTOR                   (1)

/***************************************************************************
*   Macro Functions
***************************************************************************/

/***************************************************************************
*   Types
***************************************************************************/
/** Enum for the diffenent types of cyclic prefix. */
typedef enum EutraCyclicPrefixIdsTag
{
    EUTRA_CYCLIC_PREFIX_NORMAL,         /**< Normal cyclic prefix, 144 or 160 Ts. */
    EUTRA_CYCLIC_PREFIX_EXTENDED,       /**< Extended cyclic prefix 512 Ts. */
    NUM_EUTRA_CYCLIC_PREFIXES           /**< The number of cyclic prefixes defined. */

} EutraCyclicPrefixIds;

/** Defines whether the configuration is to be SETUP, RELEASED or NOT CHANGED.
 *  Note SETUP can imply either an establishment or reconfiguration.
 */
typedef enum EutraConfigActionTag
{
    EUTRA_CONFIG_ACTION_RELEASE_NO_CHANGE,     /**< Configuration is released and not changed. */
    EUTRA_CONFIG_ACTION_RELEASE,               /**< Release configuration. */
    EUTRA_CONFIG_ACTION_SETUP_NO_CHANGE,       /**< Configuration is setup and not changed. */
    EUTRA_CONFIG_ACTION_SETUP                  /**< Setup or modify configuration. */
}
EutraConfigAction;

/** Defines the physical layer identity of the cell
 *  Range is 0 to 511 for FDD mode and 0 to 127 for TDD128 mode.
 */
typedef Int16 UtraPhysCellId;

/** Defines the transmission bandwidth configuration, NRB in downlink,
 *  see TS 36.101 table 5.6-1.
 *  Note changes in this enum must be reflected to enum ESysBandwidthMhz.
 */
typedef enum EutraBandwidthTag
{
    EUTRA_BANDWIDTH_6,
    EUTRA_BANDWIDTH_15,
    EUTRA_BANDWIDTH_25,
    EUTRA_BANDWIDTH_50,
    EUTRA_BANDWIDTH_75,
    EUTRA_BANDWIDTH_100,
    NUM_EUTRA_BANDWIDTH
} EutraBandwidth;

/** Defines the number of cell specific antenna ports as defined in TS 36.211, 6.2.1. */
typedef enum EutraAntennaPortsCountTag
{
    EUTRA_NUM_TX_ANTENNAS_1,
    EUTRA_NUM_TX_ANTENNAS_2,
    EUTRA_NUM_TX_ANTENNAS_4,
    EUTRA_NUM_TX_UNKNOWN
} EutraAntennaPortsCount;

/** Defines the Cyclic Prefix Length. use values of 1 and 2 to align with asn1 types */
typedef enum EutraCyclicPrefixTag
{
    EUTRA_UNKNOWN_CP   = 0,
    EUTRA_NORMAL_CP    = 1,
    EUTRA_EXTENDED_CP  = 2
} EutraCyclicPrefix;

/** transmissionMode is defined in TS 36.213 section 7.1. */
typedef enum EutraTransmissionModeTag
{
   EUTRA_TX_MODE_1_SINGLE          = 1,
   EUTRA_TX_MODE_2_OL_TX_DIVERSITY = 2,
   EUTRA_TX_MODE_3_LD_CDD          = 3,
   EUTRA_TX_MODE_4_CL_SM           = 4,
   EUTRA_TX_MODE_5_MU_MIMO         = 5,
   EUTRA_TX_MODE_6_CL_SM_RANK1     = 6,
   EUTRA_TX_MODE_7_SINGLE_BEAMFORM = 7
#if defined (UPGRADE_REL9)
   , EUTRA_TX_MODE_8_DUAL_BEAMFORM = 8
#endif /* UPGRADE_REL9 */
   ,NUM_EUTRA_TX_MODE
} EutraTransmissionMode;

/** Defines the frequency layer where the xxxxx_1 refers to the first carrier
 *  in the measurement request. E.g. for the E-UTRA case EUTRAN_FREQ_LAYER_1
 *  is the first entry EcphyInterFrequencyMeasurementReq. */
typedef enum EutraFrequencyLayerTag
{
    NO_FREQ_LAYER,
    EUTRAN_FREQ_LAYER_1,
    EUTRAN_FREQ_LAYER_2,
    EUTRAN_FREQ_LAYER_3,
    EUTRAN_FREQ_LAYER_4,
    EUTRAN_FREQ_LAYER_5,
    EUTRAN_FREQ_LAYER_6,
    EUTRAN_FREQ_LAYER_7,
    EUTRAN_FREQ_LAYER_8,
    UTRAN_FREQ_LAYER_1,
    UTRAN_FREQ_LAYER_2,
    UTRAN_FREQ_LAYER_3,
    GSM_FREQ_LAYER
}
EutraFrequencyLayer;


/** Defines the additional spectrum emission configuration,
 *  see TS 36.101 table 6.2.4-1.
 *  Range is 1 to 32.
 */
typedef Int8 EutraAdditionalSpectrumEmission;

/** Defines the C_RNTI */
typedef Int16       EutraCrnti;

/** Defines the IMSI */
typedef struct EutraImsiTag
{
    /** Defines the number of digits in the IMSI*/
    Int8             noOfDigits;

    /** Defines the imsi in BCD format starting at the top 4 bits of the 1st Octet.
     *  \note there are 2 digits per octet so the size of the used array is #noOfDigits /2.
     *  This format is same as the PER encoded IMSI in the PCCH message.
     */
    Int8             bcdDigit [MAX_IMSI_SIZE_DIV2];
}
EutraImsi;

/** Defines the S-IMSI */
typedef struct EutraStmsiTag
{
    /** Defines the MMEC part of the S-TMSI */
    Int8             mmec;

    /** Defines the M-TMSI part of the S-TMSI */
    Int32            mTMSI;
}
EutraStmsi;

/** \defgroup SigEl1EarfcnToFrequency EARFCN to Carrier Frequency Conversion
 * This section defines how the EARFCN channel number is converted to a carrier frequency.
 *
 * @{
 *
 * The carrier frequency in the uplink and downlink is designated by the E-UTRA
 * Absolute Radio Frequency Channel Number (EARFCN) in the range 0 - 65535.
 *
 * The relation between EARFCN and the carrier frequency in MHz for the downlink
 * is given by the following equation,
 *
 *    FDL = FDL_low + 0.1(NDL - NOffs-DL)
 *
 *    where FDL_low and NOffs-DL are given in the table below and NDL is the downlink EARFCN.
 *
 * The relation between EARFCN and the carrier frequency in MHz for the uplink
 * is given by the following equation:
 *
 *   FUL = FUL_low + 0.1(NUL - NOffs-UL)
 *
 *   where FUL_low and NOffs-UL are given in the following table NUL is the uplink EARFCN.
 *
 *
 * <TABLE>
 * <TR><TH>E-UTRA Operating</TH><TH>Duplex</TH><TH colspan="3">Downlink</TH><TH colspan="3">Uplink</TH></TR>
 * <TR><TH>Band</TH><TH>FDD or TDD</TH><TH>FDL_low (MHz)</TH><TH>NOffs-DL</TH><TH>Range of NDL</TH><TH>FUL_low (MHz)</TH><TH>NOffs-UL</TH><TH>Range of NUL</TH></TR>
 * <TR><TD>1</TD><TD>FDD</TD><TD>2110</TD><TD>0</TD><TD>0 - 599</TD><TD>1920</TD><TD>18000</TD><TD>18000 - 18599</TD></TR>
 * <TR><TD>2</TD><TD>FDD</TD><TD>1930</TD><TD>600</TD><TD>600 - 1199</TD><TD>1850</TD><TD>18600</TD><TD>18600 - 19199</TD></TR>
 * <TR><TD>3</TD><TD>FDD</TD><TD>1805</TD><TD>1200</TD><TD>1200 - 1949</TD><TD>1710</TD><TD>19200</TD><TD>19200 - 19949</TD></TR>
 * <TR><TD>4</TD><TD>FDD</TD><TD>2110</TD><TD>1950</TD><TD>1950 - 2399</TD><TD>1710</TD><TD>19950</TD><TD>19950 - 20399</TD></TR>
 * <TR><TD>5</TD><TD>FDD</TD><TD>869</TD><TD>2400</TD><TD>2400 - 2649</TD><TD>824</TD><TD>20400</TD><TD>20400 - 20649</TD></TR>
 * <TR><TD>6</TD><TD>FDD</TD><TD>875</TD><TD>2650</TD><TD>2650 - 2749</TD><TD>830</TD><TD>20650</TD><TD>20650 - 20749</TD></TR>
 * <TR><TD>7</TD><TD>FDD</TD><TD>2620</TD><TD>2750</TD><TD>2750 - 3449</TD><TD>2500</TD><TD>20750</TD><TD>20750 - 21449</TD></TR>
 * <TR><TD>8</TD><TD>FDD</TD><TD>925</TD><TD>3450</TD><TD>3450 - 3799</TD><TD>880</TD><TD>21450</TD><TD>21450 - 21799</TD></TR>
 * <TR><TD>9</TD><TD>FDD</TD><TD>1844.9</TD><TD>3800</TD><TD>3800 - 4149</TD><TD>1749.9</TD><TD>21800</TD><TD>21800 - 22149</TD></TR>
 * <TR><TD>10</TD><TD>FDD</TD><TD>2110</TD><TD>4150</TD><TD>4150 - 4749</TD><TD>1710</TD><TD>22150</TD><TD>22150 - 22749</TD></TR>
 * <TR><TD>11</TD><TD>FDD</TD><TD>1475.9</TD><TD>4750</TD><TD>4750 - 4949</TD><TD>1427.9</TD><TD>22750</TD><TD>22750 - 22949</TD></TR>
 * <TR><TD>12</TD><TD>FDD</TD><TD>728</TD><TD>5000</TD><TD>5000 - 5179</TD><TD>698</TD><TD>23000</TD><TD>23000 - 23179</TD></TR>
 * <TR><TD>13</TD><TD>FDD</TD><TD>746</TD><TD>5180</TD><TD>5180 - 5279</TD><TD>777</TD><TD>23180</TD><TD>23180 - 23279</TD></TR>
 * <TR><TD>14</TD><TD>FDD</TD><TD>758</TD><TD>5280</TD><TD>5280 - 5379</TD><TD>788</TD><TD>23280</TD><TD>23280 - 23379</TD></TR>
 * <TR><TD> </TD><TH></TH><TD></TD><TD></TD><TD></TD><TD></TD><TD></TD><TD></TD></TR>
 * <TR><TD>17</TD><TD>FDD</TD><TD>734</TD><TD>5730</TD><TD>5730 - 5849</TD><TD>704</TD><TD>23730</TD><TD>23730 - 23849</TD></TR>
 * <TR><TD> </TD><TH></TH><TD></TD><TD></TD><TD></TD><TD></TD><TD></TD><TD></TD></TR>
 * <TR><TD>33</TD><TD>TDD</TD><TD>1900</TD><TD>36000</TD><TD>36000 - 36199 </TD><TD>1900</TD><TD>36000</TD><TD>36000 - 36199</TD></TR>
 * <TR><TD>34</TD><TD>TDD</TD><TD>2010</TD><TD>36200</TD><TD>36200 - 36349</TD><TD>2010</TD><TD>36200</TD><TD>36200 - 36349</TD></TR>
 * <TR><TD>35</TD><TD>TDD</TD><TD>1850</TD><TD>36350</TD><TD>36350 - 36949</TD><TD>1850</TD><TD>36350</TD><TD>36350 - 36949</TD></TR>
 * <TR><TD>36</TD><TD>TDD</TD><TD>1930</TD><TD>36950</TD><TD>36950 - 37549</TD><TD>1930</TD><TD>36950</TD><TD>36950 - 37549</TD></TR>
 * <TR><TD>37</TD><TD>TDD</TD><TD>1910</TD><TD>37550</TD><TD>37550 - 37749</TD><TD>1910</TD><TD>37550</TD><TD>37550 - 37749</TD></TR>
 * <TR><TD>38</TD><TD>TDD</TD><TD>2570</TD><TD>37750</TD><TD>37750 - 38249</TD><TD>2570</TD><TD>37750</TD><TD>37750 - 38249</TD></TR>
 * <TR><TD>39</TD><TD>TDD</TD><TD>1880</TD><TD>38250</TD><TD>38250-38649</TD><TD>1880</TD><TD>38250</TD><TD>38250-38649</TD></TR>
 * <TR><TD>40</TD><TD>TDD</TD><TD>2300</TD><TD>38650</TD><TD>38650-39649</TD><TD>2300</TD><TD>38650</TD><TD>38650-39649</TD></TR>
 * </TABLE>
 *
 * NOTE:  The channel numbers that designate carrier frequencies so close to the
 * operating band edges that the carrier extends beyond the operating band edge
 * shall not be used. This implies that the first 7, 15, 25, 50, 75 and 100 channel
 * numbers at the lower operating band edge and the last 6, 14, 24, 49, 74 and 99
 * channel numbers at the upper operating band edge shall not be used for channel
 * bandwidths of 1.4, 3, 5, 10, 15 and 20 MHz respectively.
 */

/** @} */ /* End of SigEl1EarfcnToFrequency group */

/***************************************************************************
 *   Measurement type definitions
 ***************************************************************************/

/** Defines a structure which is used for the RSSI measurement reports.
 */
typedef struct EutraRssiReportTag
{
    /** The EARFCN on which the measurement was made.
     * \see \ref SigEl1EarfcnToFrequency. */
    EutraArfcn                          earfcn;

    /** The wideband RSSI measurement result for this #earfcn and is in
     * 1/8 dBm steps, for example -400 = -50dBm.
     */
    SignedInt16                         rssi;

#if defined (EL1_TEST_PLATFORM)
    /** The Bandwidth on which the measurement was made.
     * for debug use */
    EutraBandwidth                      bandwidth;
#endif
}
EutraRssiReport;

/** Defines the default neighbour cell configuration for MBSFN subframes
 *  and for the TDD UL/DL configuration.
 */
typedef enum EutraNeighCellConfigTag
{
    EUTRA_NCELL_MBSFN_NOT_SAME_AS_SERV_CELL,      /**< Not all neighbour cells have the same
                                                   *   MBSFN subframe allocation as serving cell. */
    EUTRA_NCELL_NO_MBSFN_SUBFRAMES,               /**< No MBSFN subframes are present in all neighbour cells. */

    EUTRA_NCELL_MBSFN_SAME_AS_SERV_CELL,          /**< The MBSFN subframe allocations of all neighbour cells
                                                   *   are identical to or subsets of that in the serving cell. */
    EUTRA_NCELL_DIFFERENT_TDD_UL_DL_TO_SERV_CELL  /**< Different UL/DL allocation in neighbouring cells for
                                                   *   TDD compared to the serving cell. */
}
EutraNeighCellConfig;

/** Defines a structure which is used for the RSRP and RSRQ measurement reports for each cell.
 */
typedef struct EutraCellMeasurementReportTag
{
    /** The physical Id of the cell. */
    EutraPhysCellId                     physCellId;

    /** The RSRP measurement in 1/8dBm units. */
    EutraRsrp                           rsrp;

    /** The RSRQ measurement in 1/8dB units. */
    EutraRsrq                           rsrq;
}
EutraCellMeasurementReport;


/** Defines the maximum allowed measurement bandwidth on a carrier frequency.
 *  Defined by the parameter Transmission Bandwidth Configuration "NRB" TS 36.104.
 *  The possible values are 6, 15, 25, 50, 75 and 100 resource blocks respectively.
 *  A value of 0 indicates that EL1 should use dl_bandwidth (decoded from MIB).
 */
typedef EutraBandwidth EutraAllowedMeasBandwidth;

#if defined (UPGRADE_REL9)
/** Defines a structure which is used for E-CID measurement report for the serving cell.
 */
typedef struct EutraUeRxTxDiffMeasurementReportTag
{
    /** Defines UE Rx-Tx time difference measurement result of the serving cell,
     * provided by lower layers. According to UE Rx-Tx time difference report
     * mapping in TS 36.133 */
    Int16                               ueRxTxTimeDiffResult;

    /** Indicates the current system frame number when receiving the UE Rx-Tx
     * time difference measurement results from lower layer.
     */
    Int16                               currentSFN;
}
EutraUeRxTxDiffMeasurementReport;
#endif /* UPGRADE_REL9 */


/** Defines the carrier frequency, allowedMeasBandwidth and presenceAntennaPort1
 * for an Inter-frequency carrier.
 */
typedef struct EutraInterFreqCarrierTag
{
    /** The EARFCN of the Eutra carrier.
     *  The carrier frequency is calculated from the current band.
     *  \see \ref SigEl1EarfcnToFrequency. */
    EutraArfcn                          earfcn;

    /** Defines the maximum allowed measurement bandwidth on a carrier frequency. */
    EutraAllowedMeasBandwidth           allowedMeasBandwidth;

    /** Defines whether all the neighbouring cells use Antenna Port 1.
     * When set to TRUE, the UE may assume that at least two cell-specific
     * antenna ports are used in all neighbouring cells.
     */
    Boolean                             presenceAntennaPort1;

    /** Defines the default neighbour cell configuration for MBSFN subframes
     *  and for the TDD UL/DL configuration. */
    EutraNeighCellConfig                neighCellConfig;

    /** Defines whether this carrier is higher priority than the serving cell.
     *  When highPrioritySearchMode is active EL1 must search for cells on
     *  carriers with #highPriorityCarrier set TRUE at the search rate of
     *  (60 x Nlayers) seconds. Where Nlayers is equal to the number of
     *  higher priority frequency layers.
     */
    Boolean                             highPriorityCarrier;
}
EutraInterFreqCarrier;

typedef struct EutraInterFrequencyMeasurementIndTag
{
    /** The EARFCN of the Eutra carrier in the current frequency band.
     *  \see \ref SigEl1EarfcnToFrequency. */
    EutraArfcn                          earfcn;

    /** Defines the reporting rate for this carrir in sub frame (i.e. ms). */
    Int16                               reportRateinSf;

#if defined (EL1_TEST_PLATFORM)
    /** Defines the reporting tti for test use. */
    Int16                               reportTti;
#endif

    /** Defines the number of reports in eutraCellMeasurementReport.
     * Range is 0 to #ECPHY_MAX_REPORTS.
    \assoc ARRAY \ref eutraCellMeasurementReport */
    Int8                                numberOfReports;

    /** Defines the physCellId, rsrp and rsrq for up to #ECPHY_MAX_REPORTS cells.
     *  BE CAREFULL THIS FIELD MUST BE THE LAST ONE OF THE STRUCT */
    EutraCellMeasurementReport          eutraCellMeasurementReport [ECPHY_MAX_REPORTS];
}
EutraInterFrequencyMeasurementInd;

typedef struct EutraFindCellReqTag
{
    /** Specifies the downlink EARFCN on which to find and camp on the cell.
     *  \see \ref SigEl1EarfcnToFrequency. */
    EutraArfcn                          earfcn;

    /** Defines the frequency band and is used to convert #earfcn to a carrier frequency.
     *  E-UTRA carrier frequency is calculated as defined in TS 36.101 section 5.7.3. */
    EutraFrequencyBand                  frequencyBand;
}
EutraFindCellReq;

typedef struct EutraFindCellCnfTag
{
    /** Indicates if the physical layer has detected one or more cells on the
     * requested frequency.
\assoc PRESENT \ref physCellId
\assoc PRESENT \ref rsrp */

    Boolean                             cellDetected;

    /** Indicates the physical cell id of the strongest cell on this carrier.
     */
    EutraPhysCellId                     physCellId;

    /** Indicates the RSRP measurement of this cell in 1/8dBm units.
     */
    EutraRsrp                           rsrp;

    /** Indicates the RSRQ measurement of this cell in 1/8dB units.
     */
    EutraRsrq                           rsrq;
}
EutraFindCellCnf;

typedef struct EutraRssiScanReqTag
{
    /** Number of frequencies to be scanned for RSSI measurements.
     *  If #numOfFreqs is greater than 0 then #earfcnList is used for the
     *  scan otherwise range of frequencies is used for scan.
     *  \see \ref SigEl1EarfcnToFrequency.
\assoc ARRAY \ref earfcnList */
    Int16                               numOfFreqs;

    /** List of frequencies for which RRSI has to be measured.
     *  ECPHY_MAX_FREQUENCIES_PER_RSSI_SCAN is 20.
     */
    EutraArfcn                          earfcnList [ECPHY_MAX_FREQUENCIES_PER_RSSI_SCAN];

    /** Lower edge of the frequency band. */
    EutraArfcn                          minEarfcnInBand;

    /** Upper edge of the frequency band. */
    EutraArfcn                          maxEarfcnInBand;

    /** Defines the frequency band and is used to convert EARFCN to a carrier frequency.
     *  E-UTRA carrier frequency is calculated as defined in TS 36.101 section 5.7.3. */
    EutraFrequencyBand                  frequencyBand;
}
EutraRssiScanReq;

typedef struct EutraRssiScanCnfTag
{
    /** Total number of frequencies measured during the scan.
\assoc ARRAY \ref eutraRssiReport */
    Int16                               numOfFreqs;

    /** Number of frequencies that have been given a higher ranking by the EL1.
     * This can indicate that results \a eutraRssiReport[0] to
     * \a eutraRssiReport[numOfRankedFreqs - 1] have been identified as being
     * most likely to contain an Eutra carrier.  This is only applicable to band
     * scans where the corresponding \a EcphyRssiScanReq.numOfFreqs is zero,
     * and then only if the EL1 supports some filtering or carrier
     * identification scheme.  It should be zero if not used or not applicable
     * to the scan method.
     */
    Int16                               numOfRankedFreqs;

    /** An array of frequencies and their RSSI results.
     * This gives the measurement results for an RSSI scan.  For band scans,
     * when \a EcphyRssiScanCnfReq.numOfFreqs or \a EcphyDrxRssiScanCnf.numOfFreqs
     * is zero, this should be ordered such that the first \a numOfRankedFreqs
     * are those frequencies identified as being most likely to contain an Eutra
     * carrier, with subsequent results ordered by RSSI.  If should always
     * contain \a numOfFreqs valid results.
     */
    EutraRssiReport                      eutraRssiReport [ECPHY_MAX_FREQUENCIES_IN_RSSI_REPORT];
}
EutraRssiScanCnf;

typedef struct EutraBcchDataIndTag
{
    /* Indicates if the BCCH Data was successfully decoded by EL1 */
    Boolean                             dataValid;

    /** Specifies the downlink frequency on which the BCCH data is decoded. */
    EutraArfcn                          dlFrequency;

    /** Indicates the physical cell id on which the BCCH data is decoded. */
    EutraPhysCellId                     physCellId;

    /** Indicates the number of anntenna ports for PBCH. */
    EutraAntennaPortsCount              pbchAntennaPorts;

    /** Indicates the transport channel the BCCH data was received on.
     *  TRUE = BCH, FALSE = DL_SCH. If BCH then the message is a fixed length
     * of 24 bits stored in #mib. If DL_SCH then the data is in
     * #transportBlockLength and #transportBlock_p. */
    Boolean                             receivedOnBch;

    /** Length of the BCCH data in Bytes.
     * NOTE this excluded the first OCTET which is used for the 'consumed'
     * flag. \ref transportBlock_p. */
    Int16                               transportBlockLength;

    /** Pointer to the transport block containing the BCCH data.
     *  The first OCTET contains a 'consumed' flag which is set TRUE when ERRC
     *  has processed the message. This is to let EL1 know that it can
     *  reuse this transport block for the next BCCH reception.
     *  The transport block data starts at the second OCTET. */
    Int8                                *transportBlock_p;

    /** Data containg the MIB. This is in signal data due to its small size. */
    Int8                                mib [MIB_SIZE_IN_OCTETS];
}
EutraBcchDataInd;

#endif /* EL1_TYP_H */

/* END OF FILE */



