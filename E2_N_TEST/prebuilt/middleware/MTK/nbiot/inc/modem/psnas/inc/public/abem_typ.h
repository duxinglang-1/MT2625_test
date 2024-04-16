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
 * File Description                                                      */
/** \file
 *    Application Layer Background Layer Engineering Mode
 *    Type Definitions
 **************************************************************************/

#ifndef ABEM_TYP_H
#define ABEM_TYP_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#if !defined(APPLAYER_H)
#   include <applayer.h>
#endif

#if !defined(TI_SIG_H)
#   include <ti_sig.h>
#endif

# if !defined(MMR_SIG_H)
#    include <mmr_sig.h>
#endif

#include <l1ut_typ.h>

#if !defined (L3GP_TYP_H)
#include     <l3gp_typ.h>
#endif

#if !defined(SIR_TYP_H)
#include <sir_typ.h>
#endif

#if !defined(GRR_SIG_H)
#include <grr_sig.h>
#endif

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/* These define the meanings of the bits within the 17 x Int16 config block */
#define ACTIVATE_BYTE 0          /* Switches logging on */
#define ACTIVATE_BITS 0x3000

#if defined(UPGRADE_TWGGE_NRW)
#define  SERIAL_NUMBER_LEN  17
#else
#if !defined (REMOVE_PROD_SERIAL_NUM)
/* ss3 - increase space for production SN for some INTOUCH products */
/* job106189: increased number of digits to 16 (17 for INTOUCH) */
#if defined (INTOUCH_PRODTEST)
#define  SERIAL_NUMBER_LEN  17
#else
#define  SERIAL_NUMBER_LEN  16
#endif
#endif
#endif

#define EM_NUM_NCELLS                  6
#define EM_NUM_FDD_NCELL_INFO          8
#define EM_MAX_CELL_MEAS              32

/***************************************************************************
 * Typed Constants
 **************************************************************************/

/***************************************************************************
 * Type Definitions
 **************************************************************************/

typedef struct AspLoggerConfigDataTag
{
  Boolean  present;
  Int8     configLength; /* In Int16s, not bytes !! */
  Int16    configOffset;
  Int16    configData [MAX_DSP_LOGGER_CONFIG_LENGTH];
}
AspLoggerConfigData;

/*CR:9909-5171*/
typedef struct AbemProductionDataTag
{
  Boolean testKeypadOnStart;  /* Run keypad test on production line */
  Boolean testDisplayOnStart; /* Run display test on production line */
  Boolean testLayer1OnStart;  /* Enter L1 null state on production line */
  Boolean testLedOnStart;     /* Run LED test on production line */

#if !defined (REMOVE_PROD_SERIAL_NUM) || defined(UPGRADE_TWGGE_NRW)
  Char    serialNumber[SERIAL_NUMBER_LEN];
#endif

  Boolean imeiLockout;        /* This boolean may be used to lockout an IMEI change*/
}
AbemProductionData;

typedef struct AbemConfigDataTag
{
  EngineeringData      engineeringData;
  AspLoggerConfigData  aspLoggerConfigData;
  AbemProductionData   productionData;  /*CR:9909-5171*/
}
AbemConfigData;

typedef enum EmUmtsModeTag
{
#if defined (UPGRADE_3G_FDD) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Compliance with 3GPP FDD mode. */
    EM_UMTS_FDD,
#endif
#if defined (UPGRADE_3G_TDD128) || defined (UPGRADE_3G_DOUBLE_DUAL_MODE)
    /** Compliance with TDD128 mode. */
    EM_UMTS_TDD128,
#endif
    EM_UMTS_INVALID
}
EmUmtsMode;


/** Determines the outcome of the ApexEmAspLoggerReq request */
typedef enum EmRequestStatusTag
{
  /** Request successful */
  EM_REQ_OK,
  /** Not yet read NVRAM etc */
  EM_REQ_NOT_INITIALISED,
  /** Error reading or writing NVRAM */
  EM_REQ_NVRAM_ERROR,
  /** Non-controlling task sent in a Req */
  EM_REQ_NOT_CONTROLLING_TASK,
  /** Must have development version defined for ASP logging */
  EM_REQ_ASP_LOG_NEEDS_DEV_VER,
  /** Cannot ASP log and use real NVRAM */
  EM_REQ_NO_ASP_LOG_WITH_REAL_NVRAM,
  /** Cannot log, for whatever reason */
  EM_REQ_ASP_LOG_NOT_AVAILABLE
}
EmRequestStatus;

typedef enum ForceCommandTypeTag
{   
    TYPE_NULL,
    FULL_SCAN,
    SPECIFIED_ARFCN_SCAN
}
ForceCommandType;

typedef struct AbemContextTag
{
  EngineeringData      engineeringData;
  AspLoggerConfigData  aspLoggerConfigData;
  AbemProductionData   productionData;  /*CR:9909-5171*/
  Boolean              waitingForCnf;
  EmRequestStatus      requestStatus;
  }
AbemContext;

/** Specifies wich PLMN to get from the list */
typedef enum EmPlmnTestTypeTag
{
  /** Get the first in the list */
  EM_PLMN_GET_FIRST,
  /** Get the next of the indexed one */
  EM_PLMN_GET_NEXT,
  /** Get the previous of the indexed one */
  EM_PLMN_GET_PREVIOUS,
  /** Get the last in the list */
  EM_PLMN_GET_LAST
} EmPlmnTestType;

/** Engineering mode mandatory command result type */
typedef enum ApexEmForceCommandResultTypeTag
{
    EM_CMD_RESULT_FULL_SCAN,
    EM_CMD_RESULT_SPECIFIED_ARFCN_SCAN,
    EM_CMD_NO_COMMAND
}
ApexEmForceCommandResultType;

/** Defines the Network Mode . */
typedef enum EmNetworkModeTag
{
#if defined (UPGRADE_3G_FDD) 
    /** Compliance with 3GPP FDD mode. */
    EM_NETWORK_FDD,
#endif
#if defined (UPGRADE_3G_TDD128)
    /** Compliance with TDD128 mode. */
    EM_NETWORK_TDD128,
#endif
#if defined (UPGRADE_GGE)
    /** Compliance with TDD128 mode. */
    EM_NETWORK_GSM,
#endif
    EM_NETWORK_INVALID
}
EmNetworkMode;

typedef enum EmGsmUmtsTypeTag
{
    /** 2G MODE  */
    EM_GSM_TYPE = 0,

    /** 3G MODE */
    EM_UMTS_TYPE,
  
    EM_INVALID_TYPE   
}
EmGsmUmtsType;

/** This structure is used in engineering mode to give information
 * on the visible neighbour cells
 */
typedef struct EmGrrTestIdleNcellInfoTag
{
    /** The neighbour cell BCCH channel number */
    Arfcn             arfcn;
    /** The current level of the neighbour cell */
    Int8              level;
    /** The BSIC of the neighbour cell */
    Int8              bsic;
    /** Whether or not BCCH is valid */
    Boolean           bcchValid;
    /** Cell reselection C1 for the neighbour cell;
     * C1 is a cell selection algorithm employed in GSM and GPRS;
     * The algorithm uses the power received from cells plus additional
     * parameters in order to assess the cell that will provide the best
     * radio connection for the MS
     */
    SignedInt16       c1;
    /** GSM cell reselection C2 for the neighbour cell. Once the MS (Mobile Station)
     * has camped onto a cell it will continue to assess the surrounding cells to
     * ensure it is monitoring the cell that will offer the best radio connection.
     * As the user moves the camped on cell may become unsuitable. This situation
     * will generate a cell reselection.
     */
    SignedInt16       c2;
    /** Cell reselection C31 for the neighbour cell.
     * C31 is a criteria based algorithm used as part of the GPRS cell reselection process.
     * It is used to assess if prospective cells qualify as reselection candidates prior
     * to applying the C32 GPRS cell reselection algorithm.
     */
    SignedInt16       c31;
    /** Cell reselection C32 for the neighbour cell.
     * C32 is the GPRS cell reselection algorithm. Once the MS (Mobile Station) has camped
     * onto a cell it will continue to assess the surrounding cells to ensure it is
     * monitoring the cell that will offer the best radio connection. As the user moves
     * the camped on cell may become unsuitable. This situation will generate a cell reselection.
      */
    SignedInt16       c32;
#if defined (UPGRADE_ADDIT_ENG_INFO)
    /** The Mobile Country Code is a three digit number uniquely identifying a given country */
    Mcc               mcc;
    /** The Mobile Network Code is either a two or three digit number used to uniquely identify
     * a given network from within a specified country (MCC)
     */
    Mnc               mnc;
    /** Cell ID for the neighbour cell */
    CellId            cellId;
#endif
    /** The Location Area Code for the neighbour cell; The Location Area Code uniquely identifies
     * a Location Area (LA) within a PLMN
     */
    Lac               lac;
}
EmGrrTestIdleNcellInfo;


#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
/** FDD neighbour cell information element */
typedef struct EmGrrTstIdleFddNcllInfoTag
{
    /** Frequency for neighbour cell */
    Int16           uArfcn;
    /** Primary scrambling code for neighbour cell */
    Int16           psc;
    /** Given only signal power is received, the average power of the received signal
     * from the neighbour cell after despreading and combining*/
    SignedInt16     rscp;
    /** Ec/No for neighbour cell, in units of 1/8 dB*/
    SignedInt16     ecno;
}
EmGrrTstIdleFddNcllInfo;
#endif


/** GRR engineering data */
typedef struct EmGrrTestIdleInfoTag
{   /* GRR idle ncell data */
    /** Indicates whether the software is in packet idle mode or CS idle mode */
    Boolean isInPacketIdle;

    /** Is the channel number of the serving cell BCCH carrier */
    Arfcn   sCellArfcn;

    /** Is the level of the serving cell BCCH carrier */
    Int8    sCellLevel;

    /** Cell reselection C1 for the serving cell;
     * C1 is a cell selection algorithm employed in GSM and GPRS;
     * The algorithm uses the power received from cells plus additional
     * parameters in order to assess the cell that will provide the best
     * radio connection for the MS
     */
    SignedInt16 sCellC1;

    /**Cell reselection C2 for the serving cell . Once the MS (Mobile Station)
     * has camped onto a cell it will continue to assess the surrounding cells to
     * ensure it is monitoring the cell that will offer the best radio connection.
     * As the user moves the camped on cell may become unsuitable. This situation
     * will generate a cell reselection.
     */
    SignedInt16 sCellC2;

    /** The cell reselection C31 for the serving cell
     * (only relevant if isInPacketIdle is TRUE)
     */
    SignedInt16 sCellC31;

    /** The cell reselection C32 for the serving cell
     * (only relevant if isInPacketIdle is TRUE)
     */
    SignedInt16          sCellC32;

    /** Is the number of items contained in the nCellInfo array */
    Int8                 top6Size;

    /** Bit map of 32-bit indicating which cell is blocked */
    Int32                gsmBlockingActive;

    /** Information relevant to neighbour cell */
    GrrTestIdleNcellInfo ncellInfo[NUM_NCELLS];

    /** Network control (NC) parameters */
    NcMeasurementParam   ncParam;
#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
    /** Qsearch_I defines a threshold and also indicates whether these
     * measurements shall be performed when RLA_C of the serving cell
     * is below or above the threshold
     */
    Int8 qSearchI;

    /** Qsearch_C defines a threshold and also indicates whether these
     * tasks shall be performed when RXLEV of the serving BCCH carrier
     * is below or above the threshold.
     * Search for 3G cells if signal level below threshold (0-7)
     * or above threshold (8-15).
     */
    Int8 qSearchC;

    /** Qsearch_P defines a threshold and also indicates whether these measurements
     * shall be performed when RLA_P of the serving cell is below or above the threshold
     */
    Int8  qSearchP;

    /** The number of frequencies in reselection list */
    Int8  numUarfcnInReselList;                   /* Shared by FDD and TDD128 */
    /** List of reselected frequencies */
    Int16 uArfcn[NUM_UMTS_NCELL_INFO];                   /* Shared by FDD and TDD128 */

#if defined (UPGRADE_3G_FDD)
    /** Specifies the offset between the two cells.
     * Applies an offset to RLA_C for cell re selection to access technology/mode FDD, where: <UL>
     * <LI> 0 = (-infinity) always select a cell if acceptable, 1 = -28 dB, 2 = -24 dB,..., 15 = 28 dB.
     * </UL>
     */
    Int8 fddQOffset;

    /** Minimum required quality level in the cell.
     * A minimum threshold for Ec/No for UTRAN FDD cell re-selection, where: <UL>
     * <LI> 0= -20dB , 1= -6dB, 2= -18dB, 3= -8dB, 4= -16dB, 5= -10dB, 6= -14dB, 7= -12dB.
     *</UL>
     */
    Int8 fddQMin;

    /* Release 5 additions .... */

    /** Offset to the signalled fddQMin (0 = 0 dB, 1 = 2 dB, 2 = 4 dB, 3 = 6 dB,
     * 4 = 8 dB, 5 = 10 dB, 6 = 12 dB, 7 = 14 dB)
     */
    Int8 fddQMinOffset;

    /** A minimum threshold of RSCP for UTRAN FDD cell re-selection
     * (0 = -114 dBm, 1 = -112 dBm, 2 = -110 dBm, 3 = -108 dBm, 4 = -106 dBm,
     * 5 = -104 dBm, 6 = -102 dBm, 7 = -100 dBm, 8 = -98 dBm, 9 = -96 dBm,
     * 10 = -94 dBm, 11 = -92 dBm, 12 = -90 dBm, 13 = -88 dBm, 14 = -86 dBm, 15 = -84 dBm,
     * Default value = -102 dBm)
     */
    Int8 fddRscpMin;

    /* Measurement Reporting parameters */

    /** Apply an offset to the reported value when prioritising the cells for reporting for
     * GSM frequency band or access technology/mode FDD
     */
    Int8 fddReportOffset;

    /** Apply priority reporting if the reported value is above threshold for GSM frequency
    * band or access technology/mode FDD
    */
    Int8                    fddReportThreshold;
    Int8                    fddReportThreshold2;

    /** FDD EcN0 threshold */
    SignedInt16             fddEcnoThreshold;

    /** FDD RSCP threshold */
    SignedInt16             fddRscpThreshold;

    /** The FDD band indication for FDD POWER CLASS4 */
    Int16                   class4Indicator;

    /** TRUE means inclusion RSCP in the report is requested */
    Boolean                 reportRscp;

    /** List of blocking FDD cells; where each entry is a
     * bit map of 32-bit indicating which cell is blocked */
    Int32                   fddBlockingActive[3];

    /** The number of cells in the list where the quality measure for cell
     * selection and re-selection is set to CPICH RSCP */
    Int8                    numRscpCells;

    /** The number of cells in the list where the quality measure for cell
     * selection and re-selection is set to CPICH Ec/No */
    Int8                    numEcnoCells;

    /** Holds top six cells selected using CPICH RSCP quality measurement */
    GrrTestIdleFddNcellInfo rscpTopSix[NUM_NCELLS];

    /** Holds top six cells selected using CPICH Ec/No quality measurement */
    GrrTestIdleFddNcellInfo ecnoTopSix[NUM_NCELLS];
#endif  /* UPGRADE_3G_FDD */
#if defined (UPGRADE_3G_TDD128)
    /** An absolute threshold of RSCP for UTRAN TDD cell re-selection, where: <UL>
     * <LI> 0 = -105dBm, 1 = -102dBm, 2=-99dBm, 3 = -96dBm, ..., 15 = -60dBm.
     * Default value = -90dBm.
     * </UL>
     */
    Int8 tdd128QOffset;

    /* Measurement Reporting parameters */

    /** Apply an offset to the reported value when prioritising the cells for reporting for
     * GSM frequency band or access technology/mode TDD
     */
    Int8  tdd128ReportOffset;

    /** TDD RSCP threshold */
    Int8  tdd128ReportThreshold;

    /** List of blocking TDD128 cells; where each entry is a
     * bit map of 32-bit indicating which cell is blocked */
    Int32 tdd128BlockingActive[3];

    /** The number of cells in the Top Six list */
    Int8  numTdd128RscpCells;

    /** Holds top six cells */
    GrrTestIdleTdd128NcellInfo tdd128RscpTopSix[NUM_NCELLS];
#endif /* UPGRADE_3G_TDD128 */
#endif /* UPGRADE_3G && UPGRADE_GGE */
}
EmGrrTestIdleInfo;

/** Engineering mode serving cell measurement.
 * Contains measurement information about the current serving cell.
 * \note Only the levelFull field is valid when the MS is in idle mode.
 */
typedef struct EmServingCellMeasTag
{
    /** Set to TRUE if uplink DTX was used in the last reporting period */
    Boolean                         dtxUsed;
    /** The received signal level averages over all bursts in the last reporting period (0-63)*/
    Int8                            levelFull;
    /** The received signal level average over the subset of bursts which must be present if
     * the network is using downlink DTX in the last reporting period (0-63)
     */
    Int8                            levelSubset;
    /** The received signal quality averaged over all bursts (0-7) */
    Int8                            qualityFull;
    /** the received signal quality averaged over the subset of bursts
     * which must be present if the network is using downlink DTX (0-7)
     */
    Int8                            qualitySubset;
    /** The timing advance value used during last reporting period */
    Int8                            timingAdv;
    /** The currently commanded TX power level */
    Int8                            txPowerLevel;
#if defined (DEVELOPMENT_VERSION)
    /** Bit errors averaged over all bursts (0-7) */
    Int32                           bitErrQualityFull;
    /** The bit errors averaged over the subset of bursts that must be
     * present if the network is using downlink DTX (0-7)
     */
    Int32                           bitErrQualitySubset;
#endif
}
EmServingCellMeas;

/** GRR engineering data during dedicated mode */
typedef struct EmGrrTestDedInfoTag
{
    /** The measurement information for the dedicated channel */
    EmServingCellMeas               scMeas;
    /** The number of items in the following arrays */
    Int8                            numMeas;
    /** The current setting of the BA ind flag */
    Int8                            baUsed;
    /** The channel numbers of the neighbour cells */
    Arfcn                           nCellArfcn[EM_NUM_NCELLS];
    /** The level of the neighbour cells */
    Int8                            nCellLevel[EM_NUM_NCELLS];
    /** The BSIC of the neighbour cells (if set to 0xFF there is no
     * synchronisation information for this neighbour cell).
     */
    Int8                            nCellBsic[EM_NUM_NCELLS];
    /** MCC of the serving cell
     * (The Mobile Country Code (MCC)is a three digit number uniquely identifying a given country)
     */
    Mcc                             mcc;
    /** MNC of the serving cell
     *  (The Mobile Network Code is either a two or three digit number used to uniquely identify
     * a given network from within a specified country (MCC))
     */
    Mnc                             mnc;
     /** The Location Area Code for the serving cell; The Location Area Code uniquely identifies
     * a Location Area (LA) within a PLMN
     */
    Lac                             lac;
    /** Cell ID of the serving cell */
    CellId                          cellId;
    /** Whether or not NCC is permitted */
    Int8                            nccPermitted;
    /** Whether to include sCell in measurements.
     * If TRUE C0 not included in serving-cell measurements.
     */
    Boolean                         pwrc;
    /** Whether or not DTX is permitted */
    Boolean                         dtxAllowed;
    /** Radio link timeout value 0, 4, 8 upto 64 */
    Int8                            radioLinkTimeout;
    /** Handover conuter */
    Int16                           hoCount;
    /** Handover success conuter */
    Int16                           hoSuccessCount;
    /** Channel assignment counter */
    Int16                           chanAssCount;
    /** Channel assignment success counter */
    Int16                           chanAssSuccessCount;
}
EmGrrTestDedInfo;

/** Provides information on the serving cell configuration for engineering purposes.
 * RR idle Serving cell data
 */
typedef struct EmGrrIdleSCellInfoTag
{
    /** The channel number of the serving cell BCCH carrier */
    Arfcn                       c0Arfcn;
    /** The timeslot of the CCCH */
    Int8                        timeslot;
    /** Base transceiver Station Identity Code of the serving cell; this is a code in GSM which is
     * broadcast on the SCH (Synchronization Channel) in order to identify the NCC
     *(Network Colour Code) and the BCC (Base Station Colour Code)
     */
    Bsic                        bsic;
    /** The Location Area ID of the serving cell */
    Lai                         lai;
    /** The Cell ID of the serving cell */
    CellId                      cellId;
    /** The maximum number of permitted retransmissions of RACH bursts
     * during an access derived from the system information broadcast
     * by the network
     */
    Int16                       maxRetrans;  /* translated by decode */
    /** The Cell Bar access cell reselection parameter derived from the
     * system information broadcast by the network
     */
    Boolean                     cellBarAccess;
    /** Indicates whether the broadcast system information permits
     * re-establishment for circuit switched calls
     */
    Boolean                     reestabNotAllowed;
    /** The access class of the serving cell from system information */
    AccessClass                 accessClass;
    /** The number of block on the BCCH carrier reserved for access grant
     * (derived from system information)
     */
    Int8                        numAccessGrantBlocks;
    /** The configuration of the CCCH channel */
    CcchConfiguration           ccchConf;
    /** Paging repeat rate in multiframes ( The
     * number of multiframes between pages and it is derived from system information)
     */
    Int8                        bsPaMfrms;          /* raw value + 2 */
    /** Whether to include sCell in measurements.
     * If TRUE C0 not included in serving-cell measurements.
     */
    Boolean                     pwrc;
    /** Derived from system information and indicates whether the mobile
     * should use DTX on the uplink
     */
    Boolean                     applyDtx;
    /** The downlink signalling failure initial value derived from system information */
    Int8                        dsc;
    /** Indicates the Additional Hysteresis which applies in Ready state for cells in same RA;
     * it ranges from 0 to 14 dB in steps of 2 dB.
     * A Cell Reselect Hysteresis is necessary when the mobile is transported along the border
     * of two cells.*/
    Int16                       cellReselectHysteresis;
    /** The maximum allowed power for transmission on CCH in the service cell */
    Int8                        msTxpwrMaxCch;
    /** Minimum access level for serving cell */
    Int16                       rxlevAccessMin;
    /** Indicates if C2 parameter is present */
    Boolean                     c2ParamPresent;
    /** If enabled, then cell selection priority will be low, but cell reselection status
     * (barred/normal)will be normal
     */
    Boolean                     cellBarQualify;
    /** The cell reselection offset.
     * The cell reselection offset is relatively static, mandatory parameter, which can be positive or
     * negative or zero. It is used to adjust the measurement result of the cell for network
     * planning purposes.
     */
    SignedInt16                 reselectOffset;
    /** Used to obtain adjusted signal quality measurements of the candidate cell */
    Int16                       temporaryOffset;
    /** When the MS places the cell on the list of the strongest carriers, it starts a timer,
     * which expires after the Penalty_Time. This timer will be reset when the cell is taken
     * off the list
     */
    Int16                       penaltyTime;
    /* Channel config and control info for CBCH */
    /** Indicates whether the CBCH channel is supported on the serving cell */
    Boolean                     cbchSupported;
    /** Contains the channel configuration for the CBCH channel */
    ChannelDescrElement         cbch;
    /** The paging ID being used by (G)RR */
    IdType                      pagingId;
    /** The paging group being used by (G)RR */
    Int8                        pagingGroup;

    /** Indicates whether system information 2 ter is being broadcast */
    Boolean                     si2TerAvailable;
    /** Indicates whether extended BCCH is being used */
    Boolean                     extBcchRequired;
    /** Indicates whether early classmark sending is required by the network */
    Boolean                     ecsControl;
    /** Indicates the band of the current serving cell */
    Band                        band;
    /** Derived from system information */
    Int8                        multiBandNcellReports;
    /** Maximum value of DSF */
    Int8                        dsfInitValue;
    /** TRUE if MSC is R99+, FALSE if R98- */
    Boolean                     mscR99OrLater;
    /** TRUE if GPRS available on cell */
    Boolean                     gprsSupported;
    /** TRUE if SGSN is R99+, FALSE if R98- */
    Boolean                     sgsnR99OrLater;
    /** TRUE if EDGE available on cell */
    Boolean                     egprsSupported;
    /** TRUE if default packet control ack format is RLC/MAC control
     * block, FALSE if 4 access bursts
     */
    Boolean                     ackIsRlcFormat;
    /** TRUE if 8-bit access burst format used, FALSE if 11-bit format used */
    Boolean                     accessBurstIsEightBit;
    /** TRUE if MS is in GPRS READY state */
    Boolean                     inReadyState;
    /** TRUE if PBCCH present */
    Boolean                     pbcchActive;
    /** Network Mode of Operation (1, 2 or 3) */
    Int8                        gprsNetworkMode;
    /** Network Control Order (0, 1 or 2) */
    Int8                        nc;
#if defined (UPGRADE_ADDIT_ENG_INFO)
    /** Whether the PBCCH is allocated as a hopping or non-hopping PBCCH*/
    Boolean                     pbcchHopping;
    /** PBCCH ARFCN */
    Arfcn                       pbcchArfcn;
#endif
    /** PBCCH timeslot */
    Int8                        pbcchTimeslot;
    /** PCCCH timeslot */
    Int8                        pccchTimeslot;
    /** PSI1 repeat period (0-15, see 04.18 for format).
     * The field is coded according to the following table:
     * <TABLE>
     *  <TR><TD>Bits Band</TD><TD> Definition           </TD></TR>
     *  <TR><TD> 0000 </TD><TD> PSI1 repeat period = 1 multiframe </TD></TR>
     *  <TR><TD> 0001 </TD><TD> PSI1 repeat period = 2 multiframe </TD></TR>
     *  <TR><TD> 0002 </TD><TD> PSI1 repeat period = 3 multiframe </TD></TR>
     *  <TR><TD> 0003 </TD><TD> PSI1 repeat period = 4 multiframe </TD></TR>
     *  <TR><TD> 0004 </TD><TD> PSI1 repeat period = 5 multiframe </TD></TR>
     *  <TR><TD> 0005 </TD><TD> PSI1 repeat period = 6 multiframe </TD></TR>
     *  <TR><TD> 0006 </TD><TD> PSI1 repeat period = 7 multiframe </TD></TR>
     *  <TR><TD> 0007 </TD><TD> PSI1 repeat period = 8 multiframe </TD></TR>
     *  <TR><TD> 0008 </TD><TD> PSI1 repeat period = 9 multiframe </TD></TR>
     *  <TR><TD> 0009 </TD><TD> PSI1 repeat period = 10 multiframe </TD></TR>
     *  <TR><TD> 0010 </TD><TD> PSI1 repeat period = 11 multiframe </TD></TR>
     *  <TR><TD> 0011 </TD><TD> PSI1 repeat period = 12 multiframe </TD></TR>
     *  <TR><TD> 0012 </TD><TD> PSI1 repeat period = 13 multiframe </TD></TR>
     *  <TR><TD> 0013 </TD><TD> PSI1 repeat period = 14 multiframe </TD></TR>
     *  <TR><TD> 0014 </TD><TD> PSI1 repeat period = 15 multiframe </TD></TR>
     *  <TR><TD> 0015 </TD><TD> PSI1 repeat period = 16 multiframe </TD></TR>
     * </Table>
     */
    Int8                        psi1RepeatPeriod;
    /** power reduction value on PBCCH blocks (0-15, see 04.60 for format)*/
    Int8                        pb;
    /**  Priority access threshold (0-7, see 04.18 for format) */
    Int8                        priorityAccThr;
    /** Timeout value of timer T3192 in TDMA frames */
    Int32                       t3192InFrames;
}
EmGrrIdleSCellInfo;

/** Dedicated channel engineering information.
 * It provides information on the configuration of the traffic
 * channel for engineering purposes
 */
typedef struct EmGrrDedSCellInfoTag
{
    /** The channel number of the BCCH carrier of the serving cell */
    Arfcn                           c0Arfcn;
    /** Base transceiver Station Identity Code of the serving cell; this is a code in GSM which is
     * broadcast on the SCH (Synchronization Channel) in order to identify the NCC
     *(Network Colour Code) and the BCC (Base Station Colour Code)
     */
    Bsic                            bsic;
    /** Contains the channel description of the dedicate channel */
    ChannelDescrElement             chanDesc;
    /** The mode of the assigned channel */
    ChannelMode                     mode;

}
EmGrrDedSCellInfo;

/** EmTestPtmNcellInfo element */
typedef struct EmTestPtmNcellInfoTag
{
    /** Neighbour cell ARFCN */
    Arfcn             arfcn;
    /** The strength of the received BCCH signal (0-63) */
    Int8              level;
    /** Base transceiver Station Identity Code of the serving cell; this is a code in GSM which is
     * broadcast on the SCH (Synchronization Channel) in order to identify the NCC
     *(Network Colour Code) and the BCC (Base Station Colour Code)
     */
    Bsic              bsic;
    /** Indicates if BCCH is valid */
    Boolean           bcchValid;
    /** Path loss parameter used for cell selection and reselection.
     * This is calculated by the MS and used for deciding which cell to camp to
     */
    SignedInt16       c1;
    /** Signal level threshold parameter */
    SignedInt16       c31;
    /** Cell ranking parameter */
    SignedInt16       c32;
#if defined (UPGRADE_ADDIT_ENG_INFO)
    /** The Mobile Country Code is a three digit number uniquely identifying a given country */
    Mcc               mcc;
    /** The Mobile Network Code is either a two or three digit number used to uniquely identify
     * a given network from within a specified country (MCC)
     */
    Mnc               mnc;
    /** The Location Area Code for the neighbour cell; The Location Area Code uniquely identifies
     * a Location Area (LA) within a PLMN
     */
    Lac               lac;
    /** Cell ID */
    CellId            cellId;
#endif
}
EmTestPtmNcellInfo;

/** GPRS Engineering Information element */
typedef struct EmGrrTestGprsEngInfoTag
{
    /** Bit map of UL slot assignment */
    Int8                    ulSlotAssignment;
    /** Bit map of DL slot assignment */
    Int8                    dlSlotAssignment;
    /** Uplink TFI (0 to 31) */
    Int8                    ulTfi;
    /** Downlink TFI (0 to 31) */
    Int8                    dlTfi;
    /** Normalised received signal level (see 3GPP TS  0508-770) */
    Int8                    cValue;
    /** Received signal quality is derived from the BER (see 3GPP TS  0508-770) */
    Int8                    rxQual;
    /** The variance of the received signal leve */
    Int8                    signVariance;
    /** Total number of GPRS uplink TBFs.
     * A TBF is temporary and is maintained only for the duration of the data transfer,
     * i.e. until there are no more RLC (Radio Link Control) / MAC (Medium Access Control)
     * blocks to be transmitted.
     */
    Int16                   numGprsUlTbfs;
    /** Total number of GPRS downlink TBFs  */
    Int16                   numGprsDlTbfs;
#if defined (UPGRADE_EDGE)
    /** Whether or not TBF is supporting EDGE */
    Boolean                 isEdgeTbf;
    /** Total number of EDGE uplink TBFs     */
    Int16                   numEgprsUlTbfs;
    /** Total number of EDGE downlink TBFs    */
    Int16                   numEgprsDlTbfs;
#endif
    /** Total number of RLCMAC blocks transmitted including Tx, ReTx, Ctrl + Dummy      */
    Int32                   totNumRlcBlocksTx;
    /** Total number of RLCMAC blocks received including Rx, Ctrl, discarded + Dummy */
    Int32                   totNumRlcBlocksRx;
    /** Number of RLCMAC data blocks transmitted   */
    Int32                   totNumRlcDataBlocksTx;
    /** Number of RLCMAC data blocks received and transmitted  */
    Int32                   totNumRlcDataBlocksReTx;
    /** Number of RLCMAC control blocks transmitted  */
    Int32                   totNumRlcCtrlBlocksTx;
    /** Number of RLCMAC dummy blocks transmitted  */
    Int32                   totNumRlcDummyBlocksTx;
    /** Number of RLCMAC data blocks received      */
    Int32                   totNumRlcDataBlocksRx;
    Int32                   totNumRlcCtrlBlocksRx;          /**< Number of RLCMAC control blocks received    */
    Int32                   totNumRlcDummyBlocksRx;         /**< Number of RLCMAC dummy blocks received                        */
    Int32                   totNumRlcDataBlocksRxDiscWin;   /**< Number of RLCMAC blocks outside window                                 */
    Int32                   totNumRlcDataBlocksRxDiscTfi;   /**< Number of RLCMAC blocks discarded due to incorrect TFI                 */
    Int32                   numGoodDecodeBlksRx;            /**< Number blocks received that have been decoded correctly                     */
    Int32                   numBadDecodeBlksRx;             /**< Number blocksreceived that have been decoded incorrectly                   */
    Int8                    ulCommandedCodingScheme;        /**< The coding scheme commanded by the network                           */
    Int8                    dlReceivedCodingScheme;         /**< The coding scheme of the last received payload block                 */

    /** Serving cell ARFCN */
    Arfcn                   sCellArfcn;
    /** The strength of the received BCCH signal (0-63) */
    Int8                    sCellLevel;
#if defined (UPGRADE_ADDIT_ENG_INFO)
    /** Base Station Identity Code of the serving cell*/
    Int8                    sCellBsic;
#endif
    /** Path loss parameter */
    SignedInt16             sCellC1;
    /** Signal level threshold parameter */
    SignedInt16             sCellC31;
    /** Cell ranking parameter */
    SignedInt16             sCellC32;
    /** Number of entries in ncellInfo[] */
    Int8                    top6Size;
    /** Neighbour cell information */
    EmTestPtmNcellInfo      ncellInfo[EM_NUM_NCELLS];
}
EmGrrTestGprsEngInfo;

#if defined (UPGRADE_3G)

/** Engineering mode RNTI */
typedef struct EmUEngRntiTag
{
    /** SRNC identity (The SRNC is responsible for the users mobility within
     * the UTRAN and is also the point of connection towards the CN)
     */
    Int16   srnc_Identity;
     /** Is the UE identifier which is allocated by the serving RN and is unique whithin that SRNC;
     * it is allocated to all UEs having RRC connection; S-RNTI is reallocated when the serving RNC
     * for the RRC connection is changed and de-allocated when the RRC connection is released
     */
    Int32   s_RNTI;
}
EmUEngRnti;

/** UE engineering mode ciphering information */
typedef struct EmUEngCipherInfoTag
{
    /** Specifies ciphering algorithm (UEA0 = no ciphering , UEA1 = ciphering) */
    Int8                    algorithm;
    /** Whether or not the algorithm is present */
    Boolean                 algorithmPresent;
    /** Indicates if ciphering is started */
    Boolean                 started;
}
EmUCipherInfo;

#if defined (UPGRADE_3G_HSDPA)
/** UE engineering mode HSDPA information */
typedef struct EmUEngHsdpaInfoTag
{
    /** Indicates if HSDPA is supported */
    Boolean             hsdpaIsSupported;
    /** Whether or not there is a reception on HSDSCH */
    Boolean             hsDschReception;
}
EmUHsdpaInfo;
#endif /* UPGRADE_3G_HSDPA */

/** UE operation engineering mode information
 * (excluding RRC mode/state)
 */
typedef struct EmUEngUeInfoTag
{
    /** The UE having an RRC connection will be allocated a S-RNTI; the SRNC identifier together
     * with S-RNTI is a unique identifier of the RRC connection within a network; the combination
     * of SRNC identifier and S-RNTI is reffered to as U-RNTI, which is used on the radio interface
     */
    EmUEngRnti          uRnti;
    /** Indicates the number of radio links for which it reports the Rx to Tx
     * time difference for the cells identified by the associated PSC.
     */
    Int8                numberOfRadioLinks;
    /** Maximum allowed UL TX power in units of 1/8 dBm */
    SignedInt16         txPower;
    /** The measured power of a received signal */
    SignedInt16         rssi;

    /** CS ciphering information */
    EmUCipherInfo       csCipheringInfo;
    /** PS ciphering information */
    EmUCipherInfo       psCipheringInfo;

#if defined (UPGRADE_3G_HSDPA)
    /** HSDPA information */
    EmUHsdpaInfo        hsdpaInfo;
#endif /* UPGRADE_3G_HSDPA */
}
EmUEngUeInfo;

/** FDD Serving cell parameter information  */
typedef struct EmUServingCellCommonParamTag
{
    /** ARFCN downlink of the serving cell */
    Int16                   arfcn_dl;
    /** URA identity; the UTRAN Registration Area is an area covered by a number of cells */
    Int16                   uraId;
    /** Whether or not Hierarchical Cell Structure (HCS) is used.
     * This is a term typically used in GSM to describe the priority of cells within a mixed
     * environment. That is when Macro, Micro, and Pico cells may be viewed as candidates for
     * cell reselection the priority described by the HCS will be used in the associated
     * calculations.
     */
    Boolean                 hcsUsed;

    /** The CS domain DRX cycle length */
    Int16                   csDrxCycleLength;
    /** The PS domain DRX cycle length */
    Int16                   psDrxCycleLength;
    /** UTRAN DRX cycle length */
    Int16                   utranDrxCycleLength;

    /* PS domain items  - also carried by
     * MM engineering mode signal: MmrInfoInd */
    /** Routing Area Code is a fixed length of 1 octet and identifies a routing area within a
     * location area; the RAC is part of the RAI (Routing Area Identity)
     */
    Int8                     rac;
    /** The mode of GPRS network */
    GprsNetworkMode          gprsNetworkMode;
}
EmUServingCellCommonParam;

#if defined (UPGRADE_3G_FDD)
/**
 * FDD Serving Cell measurement information.
 */
typedef struct EmUServingFddCellParamsAndMeasTag
{
    /** Primary scrambling code of the serving cell*/
    Int16           primaryScramblingCode;
    /** CPICH (RSCP)used as a measurement quantity in the measurement report for the serving cell;
     * in units of 1/8 dBm */
    SignedInt16     cpich_RSCP;
    /** CPICH Ec/N0 used as the quality measure for determining which cell to choose when
     * performing a cell selection or mobile-initiated cell reselection; in units of 1/8 dB
     */
    SignedInt16     cpich_Ec_N0;
    /** Minimum Required Quality Level is the minimum required quality level in the cell (dB)used
     * to determine if it may select/reselect to the cell; in units of 1/8 dB i.e, (-24 to 0 )
     */
    SignedInt16     q_QualMin;
    /** Minimum Required Rx Level is the minimum required RX level in the cell (dBm)used
     * to determine if it may select/reselect to the cell; in units of 1/8 dBm
     */
    SignedInt16     q_RxlevMin;
    /** Maximum allowed UL TX power in units of 1/8 dBm */
    SignedInt16     txPowerMax;

    /** serving cell S criteria */
    SignedInt16     servingSresult;

    /** serving cell R criteria */
    SignedInt16     servingRresult;
}
EmUServingFddCellParamsAndMeas;
#endif
#if defined (UPGRADE_3G_TDD128)
/**
 * TDD128 Serving Cell measurement information.
 */
typedef struct EmUServingTdd128CellParamsAndMeasTag
{
    /** Identifies the serving cell */
    Int8             cellParametersId;
    /** Received Signal Code Power, the received power on one code measured on the PCCPCH from
     * a TDD cell (The reference point for the RSCP shall be the antenna connector of the UE)
     */
    SignedInt16      p_ccpch_RSCP;
    /** Minimum Required Rx Level is the minimum required RX level in the cell (dBm)used
     * to determine if it may select/reselect to the cell
     */
    SignedInt16      q_RxlevMin;
    /** Maximum allowed UL TX power */
    SignedInt16      txPowerMax;

    /** serving cell S criteria */
    SignedInt16      servingSresult;

    /** serving cell R criteria */
    SignedInt16      servingRresult;
}
EmUServingTdd128CellParamsAndMeas;

#endif /* UPGRADE_3G_TDD128 */

/** Specifies if the cell is barred or not */
typedef enum T_EmUCellBarredTag
{
    /** The UE is not permitted to select/re-select this cell */
    T_EmUCellBarred_barred = 0,
    /** Not Barred; UE shall treat this cell as candidate during the cell selection
     * and cell re-selection procedures in Idle mode and in Connected mode
     */
    T_EmUCellBarred_notBarred = 1
}
T_EmUCellBarred;

/** Cell barred information element */
typedef struct EmUCellBarred_barredTag
{
    /** Indicates whether or not intra-frequency cell reselection is allowed */
    Boolean            intraFreqCellReselectionInd;
    /** The time interval Tbarred is sent via system information in a barred
     * cell together with Cell status information in the Cell Access Restriction IE
     */
    Int16              t_Barred;
}
EmUCellBarred_barred;

/** Engineering mode cell barred information */
typedef struct EmUCellBarredTag
{
    /** Indicates if the cell is barred
\assoc UNION \ref choice */
    T_EmUCellBarred tag;
    /** Cell barred information */
    union
    {
        /** Cell barred information */
        EmUCellBarred_barred                                 barred;
        /** Dummy */
        Int8                                               _dummy2_;
    }
    choice;
}
EmUCellBarred;


/** Engineering mode cell access restrictions information */
typedef struct EmUEngCellAccessRstrctnsTag
{
    /** Whether or not the cell is barred */
    EmUCellBarred                  cellBarred;
    /** Indicates if cell reserved for operator use */
    Boolean                        resForOpUse;
    /** Whether or not the cell is reserved for future extension */
    Boolean                        resExtension;
    /** Access control class */
    Int16                          accessControlClass;
    /** Intra-frequency barring type */
    Int8                           freqBarring;
}
EmUCellAccessRstrctns;

/** Engineering mode serving cell parameters and measurement */
typedef union EmUServingCellParamsAndMeasUnionTag
{
#if defined (UPGRADE_3G_FDD)
    /** Engineering mode FDD cell parameters and measurement */
    EmUServingFddCellParamsAndMeas    fdd;
#endif
#if defined (UPGRADE_3G_TDD128)
    /** Engineering mode TDD128 cell parameters and measurement */
    EmUServingTdd128CellParamsAndMeas tdd128;
#endif
}
EmUServingCellParamsAndMeasUnion;

/**
 * Cell camp information.
 */
typedef struct EmUCellCampReSelectTag
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
EmUCellCampReSelect;


/** Engineering mode serving cell information element */
typedef struct EmUEngServingCellInfoTag
{
    /** Cell ID of the serving cell */
    UCellIdentity                    cellId;
    /** LAI; Identifies uniquely a location area for a GSM-MAP type of PLMN */
    Lai                               lai;
    /** Common parameters for cells */
    EmUServingCellCommonParam         common;
    /** Specifies if the mode is FDD or TDD128
\assoc UNION \ref mode */
    UmtsMode                          umtsMode;
    /** Mode specific parameters */
    EmUServingCellParamsAndMeasUnion  mode;
    /** Access restriction parameters used for cell selection/reselection purpose */
    EmUCellAccessRstrctns             accessRestrictions;
    /** Cell barred status; This parameter indicates whether it is allowed to camp to a cell */
    Int8                              barredStatus;
    /** Indicates the cell SIB3 inf */
    EmUCellCampReSelect               cellCampInf;
}
EmUServingCellInfo;

/** Engineering mode intra-frequency common cell */
typedef struct EmUIntraFreqCommonCellParamsTag
{
    /** Indicates if cell is active */
    Boolean     isActive;
    /** Indicates the timing difference between the primary
     * CCPCH of the current cell and the primary CCPCH of a
     * neighbouring cell
     */
    Int16       referenceTimeDifferenceToCell;
}
EmUIntraFreqCommonCellParams;

#if defined (UPGRADE_3G_FDD)
/**
 * Intra-Frequency FDD cell measurement information.
 */
typedef struct EmUIntraFreqFddCellParamsAndMeasTag
{
    /** Primary scrambling code */
    Int16           primaryScramblingCode;
    /** CPICH (RSCP)used as a measurement quantity in the measurement report for the serving cell;
     * the unit is dBm */
    SignedInt16     cpich_RSCP;
    /** CPICH Ec/N0 used as the quality measure for determining which cell to choose when
     * performing a cell selection or mobile-initiated cell reselection; the unit is dB
     */
    SignedInt16     cpich_Ec_N0;
    /** Path loss */
    SignedInt16     pathloss;
    /** Minimum Required Quality Level is the minimum required quality level in the cell (dB)used
     * to determine if it may select/reselect to the cell
     */
    SignedInt16     q_QualMin;
    /** Minimum Required Rx Level is the minimum required RX level in the cell (dBm)used
     * to determine if it may select/reselect to the cell
     */
    SignedInt16     q_RxlevMin;
    /** Maximum allowed UL TX power */
    SignedInt16     txPowerMax;

    /** This cell reselect R value */
    SignedInt16     rValue;

    /** This cell Qoffset */
    SignedInt16     qOffset;
}
EmUIntraFreqFddCellParamsAndMeas;
#endif

#if defined (UPGRADE_3G_TDD128)
/**
 * Intra-Frequency TDD128 cell measurement information.
 */
typedef struct EmUIntraFreqTdd128CellParamsAndMeasTag
{
    /** Identifies the cell */
    Int8        cellParametersId;
    /** Received Signal Code Power, the received power on one code measured on the PCCPCH from
     * a TDD cell (The reference point for the RSCP shall be the antenna connector of the UE)
     */
    SignedInt16 p_ccpch_RSCP;
    /** Path loss */
    SignedInt16 pathloss;
    /** Minimum Required Rx Level is the minimum required RX level in the cell (dBm)used
     * to determine if it may select/reselect to the cell
     */
    SignedInt16 q_RxlevMin;
    /** Maximum allowed UL TX power */
    SignedInt16 txPowerMax;

    /** This cell reselect R value */
    SignedInt16 rValue;

    /** This cell Qoffset */
    SignedInt16 qOffset;
}
EmUIntraFreqTdd128CellParamsAndMeas;

#endif /* UPGRADE_3G_TDD128 */

/** Engineering mode intra frequency cell parameters and measurement */
typedef union EmUIntraFreqCellParamsAndMeasUnionTag
{
#if defined (UPGRADE_3G_FDD)
    /** Engineering mode intra frequency FDD cell parameters and measurement */
    EmUIntraFreqFddCellParamsAndMeas      fdd;
#endif
#if defined (UPGRADE_3G_TDD128)
    /** Engineering mode intra frequency TDD128 cell parameters and measurement */
    EmUIntraFreqTdd128CellParamsAndMeas   tdd128;
#endif
}
EmUIntraFreqCellParamsAndMeasUnion;

/** Engineering mode information about
 * an Intra-Frequency cell information
 */
typedef struct EmUIntraFreqCellInfoTag
{
    /** Identifies intra frequency cell */
    Int8                                cellId;
    /** LAI of the intra frequency cell */
    Lai                                 lai;
    /** Intra frequency cell common parameters */
    EmUIntraFreqCommonCellParams        common;
    /** Specifies if the mode is FDD or TDD128
\assoc UNION \ref mode */
    UmtsMode                            umtsMode;
    /** Intra frequency cell parameters */
    EmUIntraFreqCellParamsAndMeasUnion  mode;
    /** Indicates if access restrictions are present */
    Boolean                             accessRestrictionsPresent;
    /** Access restriction parameters used for cell selection/reselection purpose */
    EmUCellAccessRstrctns               accessRestrictions;
}
EmUIntraFreqCellInfo;

/** Engineering mode intra frequency cell list element */
typedef struct EmUEngIntraFreqCellListTag
{
    /** The number of the cells in the list */
    Int8                        numberOfCells;
    /** Cell information */
    EmUIntraFreqCellInfo        cellInfo[EM_MAX_CELL_MEAS];
}
EmUIntraFreqCellList;

#if defined (UPGRADE_3G_FDD)
/** Engineering mode inter frequency FDD cell parameters */
typedef struct EmUInterFreqFddCellParamsAndMeasTag
{
    /** Primary scrambling code */
    Int16           primaryScramblingCode;
    /** CPICH (RSCP) */
    SignedInt16     cpich_RSCP;
    /** CPICH Ec/N0 */
    SignedInt16     cpich_Ec_N0;
    /** Utra carrier RSSI */
    SignedInt16     utra_carrierRSSI;
    /** Path loss */
    SignedInt16     pathloss;
    /** Ec/N0, [dB] */
    SignedInt16     q_QualMin;
    /** RSCP, [dB] */
    SignedInt16     q_RxlevMin;
    /** CCPCH TX power maximum */
    SignedInt16     txPowerMax;
    /** This cell reselect R value */
    SignedInt16     rValue;
    /** This cell Qoffset */
    SignedInt16     qOffset;
}
EmUInterFreqFddCellParamsAndMeas;

/** Indicates the FDD Cell Identifier and Results.*/
typedef struct EmFddCellInfoTag
{
    /** Primary scrambling code */
    Int16           primaryScramblingCode;
    /** CPICH (RSCP) */
    SignedInt16     cpich_RSCP;
    /** CPICH Ec/N0 */
    SignedInt16     cpich_Ec_N0;
}
EmFddCellScanInfo;
#endif /* UPGRADE_3G_FDD */

#if defined (UPGRADE_3G_TDD128)
/**
 * Inter-Frequency TDD128 cell measurement information.
 */
typedef struct EmUInterFreqTdd128CellParamsAndMeasTag
{
    /** Identifies the cell */
    Int16       cellParametersId;
    /** CPICH (RSCP) */
    SignedInt16 p_ccpch_RSCP;
    /** Utra carrier RSSI */
    SignedInt16 utra_carrierRSSI;
    /** Path loss */
    SignedInt16 pathloss;
    /** Ec/N0, [dB] */
    SignedInt16 q_RxlevMin;
    /** CCPCH TX power maximum */
    SignedInt16 txPowerMax;

    /** This cell reselect R value */
    SignedInt16 rValue;

    /** This cell Qoffset */
    SignedInt16 qOffset;
}
EmUInterFreqTdd128CellParamsAndMeas;

typedef struct EmTdd128CellInfoTag
{
    /** Identifies the cell */
    Int8       cellParametersId;
    /** CPICH (RSCP) */
    SignedInt16 p_ccpch_RSCP;
}
EmTdd128CellScanInfo;
#endif /* UPGRADE_3G_TDD128 */

/** Engineering mode inter frequency common parameters */
typedef struct EmUInterFreqCommonCellParamsTag
{
    /** Downlink ARFCN */
    Int16       arfcn_dl;
    /** indicates the timing difference between the primary
     * CCPCH of the current cell and the primary CCPCH of a
     * neighbouring cell
     */
    Int16       referenceTimeDifferenceToCell;
}
EmUInterFreqCommonCellParams;

/** Engineering mode inter frequency cell parameters and measurement */
typedef union EmUInterFreqCellParamsAndMeasUnionTag
{
#if defined (UPGRADE_3G_FDD)
    /** Engineering mode inter frequency FDD cell parameters and measurement */
    EmUInterFreqFddCellParamsAndMeas             fdd;
#endif
#if defined (UPGRADE_3G_TDD128)
    /** Engineering mode inter frequency TDD128 cell parameters and measurement */
    EmUInterFreqTdd128CellParamsAndMeas          tdd128;
#endif
}
EmUInterFreqCellParamsAndMeasUnion;

/** Inter frequency cell information */
typedef struct EmUInterFreqCellInfoTag
{
    /** Identifies the inter freq cell */
    Int8                                cellId;
    /** LAI of the inter freq cell */
    Lai                                 lai;
    /** Common parameters of the inter frequency cell */
    EmUInterFreqCommonCellParams        common;
    /** Specifies if the mode is FDD or TDD128
\assoc UNION \ref mode */
    UmtsMode                            umtsMode;
    /** Inter frequency parameters */
    EmUInterFreqCellParamsAndMeasUnion  mode;
    /** Indicates if access restriction information is present */
    Boolean                             accessRestrictionsPresent;
    /** Access restriction parameters used for cell selection/reselection purpose */
    EmUCellAccessRstrctns               accessRestrictions;
}
EmUInterFreqCellInfo;

/**
 * Engineering mode information about the
 * Inter-Frequency FDD cells.
 */
typedef struct EmUInterFreqCellListTag
{
    /** The number of inter frequency cells in the list */
    Int8                    numberOfCells;
    /** The list of inter frequency cells */
    EmUInterFreqCellInfo    cellInfo[EM_MAX_CELL_MEAS];
}
EmUInterFreqCellList;


#if defined (UPGRADE_GGE)
/**
 * GSM cell measurement information.
 */
typedef struct EmUGsmCellMeasTag
{
    /** Received Signal Strength Indicator, the wide-band received power within the
     * relevant channel bandwidth. Measurement shall be performed on a GSM BCCH carrier;
     * the reference point for the RSSI shall be the antenna connector of the UE.
     * \note This measurement is for handover between UTRAN and GSM.
     */
    SignedInt16     gsm_carrierRSSI;
    /** Minimum Required Rx Level is the minimum required RX level in the cell (dBm)used
     * to determine if it may select/reselect to the cell
     */
    SignedInt16     q_RxlevMin;
}
EmUGsmCellMeas;

/**
 * GSM cell parameter information.
 */
typedef struct EmUBsicTag
{
    /** Network Colour Code (NCC);
     * The first/leftmost bit of the bit string contains the
     * most significant bit of the NCC
     */
    Int8    ncc;
    /** Base Station Colour Code (BCC);
     * The first/leftmost bit of the bit string contains the most
     * significant bit of the BCC
     */
    Int8    bcc;
}
EmUBsic;

/**
 * GSM cell parameter information.
 */
typedef struct EmUGsmCellEngParamsTag
{
    /** BCCH ARFCN */
    Int16             bcch_arfcn;
    /** GSM band indicator (Indicates how to interpret the BCCH ARFCN) */
    UFrequency_Band   gsmBandIndicator;
    /** Base transceiver Station Identity Code; this is a code in GSM which is
     * broadcast on the SCH (Synchronization Channel) in order to identify the NCC
     *(Network Colour Code) and the BCC (Base Station Colour Code)
     */
    EmUBsic           bsic;
}
EmUGsmCellParams;

/**
 * Engineering mode information about a  GSM cell.
 */
typedef struct EmUGsmCellEngInfoTag
{
    /** GSM cell ID */
    Int8                cellId;  /*UInterRATCellID Int8*/
    /** The Location Area Identity uniquely identifies a LA (Location Area)
     * within any PLMN (Public Land Mobile Network); it is comprised of the
     * MCC (Mobile Country Code), MNC (Mobile Network Code) and the LAC
     * (Location Area Code)
     */
    Lai                 lai;
    /** Parameters of GSM cell */
    EmUGsmCellParams    params;
    /** Measurement of GSM cell */
    EmUGsmCellMeas      meas;
}
EmUGsmCellInfo;

/**
 * Engineering mode information about the GSM cells.
 */
typedef struct EmUGsmCellListTag
{
    /** The number of the supplied GSM cells in the list */
    Int8                numberOfCells;
    /** Holds information for each GSM cell in the list */
    EmUGsmCellInfo     cellInfo[EM_MAX_CELL_MEAS];
}
EmUGsmCellList;

typedef struct EmGsmCellScanInfoTag
{
    Int16           arfcn;
    SignedInt16     rxLev;
}
EmGsmCellScanInfo;
#endif /* UPGRADE_GGE */

/** Engineering mode RRC information */
typedef struct EmRrcEngInfoTag
{
    /** Defines RRC mode and connected states */
    Int8                            rrcModeState;
    /** Defines UE Operation */
    EmUEngUeInfo                    ueOperation;
    /** Indicates if UE operation data is required*/
    Boolean                         ueOperationPresent;
    /** Carries the contents of the supplied serving cell */
    EmUServingCellInfo              servingCell;
    /** Indicates if current Serving Cell data is required
     * and supported by the current mode/state
     */
    Boolean                         servingCellPresent;
    /** Carries the contents of the supplied Intra-Frequency
     * FDD cell list structure
     */
    EmUIntraFreqCellList            intraFreqCells;
    /** Set to TRUE if Intra-Frequency FDD Cell data is required */
    Boolean                         intraFreqCellsPresent;
    /** Carries the contents of the supplied Inter-Frequency cell  */
    EmUInterFreqCellList            interFreqCells;
    /** Set to TRUE if Inter-Frequency FDD Cell data is required */
    Boolean                         interFreqCellsPresent;
#if defined (UPGRADE_GGE)
    /** Contains information of the supplied Inter-RAT GSM cell  */
    EmUGsmCellList                  interRatGsmCells;
    /** Set to TRUE if Inter-RAT GSM Cell data is required */
    Boolean                         interRatGsmCellsPresent;
#endif /* UPGRADE_GGE */
#if defined (ENABLE_AT_TRACE)
    /** Set to TRUE if AT trace info included, when other elements in this structure
      * are ignored.
      */
    Boolean                         atTraceIncluded;
    /** Air interface message category ,like UL_DCCH.*/
    UPduIdentifier                  message;
    /** Air interface message type in specific category, like cellUpdataConfirm.*/
    Int32                           messageType;
    /** PDU length.*/
    Int32                           dataLen;
    /** PDU data.*/
    Int8                          * data_p;
#endif/* ENABLE_AT_TRACE*/

}
EmRrcEngInfoInd;
#endif

/**
 * Engineering mode information about
 * an arfcn rssi inf.
 */
typedef struct EmArfcnRssiInfoTag
{    
    /** The DL ARFCN of the cell */
    Int16       arfcnDl;

    /** Received RSSI  */
    SignedInt16 rssi;
}
EmArfcnRssiInfo;

/**
 * Engineering mode information about
 * All frequency scan result.
 */

#define EM_MAX_FREQUENCE_NUMBER                   20
#define EM_MAX_REPORT_CELL_NUMBER                 8


typedef struct EmScanResultListTag
{    
    /** The number of scan frequency */
    Int16                 arfcnNumber;
    
    /** All frequency scan rssi result */
    EmArfcnRssiInfo       arfcnRssiInfoList[EM_MAX_FREQUENCE_NUMBER];   
}
EmScanResultList;

typedef union EmSpecifiedScanCellInfoTag
{
#if defined (UPGRADE_3G_FDD)
    /** Indicates the FDD Cell Identifier and Results.*/
    EmFddCellScanInfo           fddCellScanInfo[EM_MAX_REPORT_CELL_NUMBER];
#endif
#if defined (UPGRADE_3G_TDD128)
    /** Indicates the TDD128 Cell Identifier and Results.*/
    EmTdd128CellScanInfo        tdd128CellScanInfo[EM_MAX_REPORT_CELL_NUMBER];
#endif
#if defined (UPGRADE_GGE)
    EmGsmCellScanInfo           gsmCellScanInfo;                                
#endif
}EmSpecifiedScanCellInfo;

/** generic Cell Identifier and Results. */
typedef  struct EmSpecifiedScanResultListTag
{

    Int8                        numOfCell;
        /** NetworkMode: FDD or TDD128 or GSM.
\assoc UNION \ref specifiedScanCellInfo */
    EmNetworkMode               emNetworkMode;

    EmSpecifiedScanCellInfo     specifiedScanCellInfo;
}
EmSpecifiedScanResultList;

#if defined(UPGRADE_3G)
typedef struct EmUmtsCellInfoTag
{

    /** The DL ARFCN of the cell */
    UUARFCN              arfcnDl;

    /** UmtsMode: FDD or TDD128.
\assoc UNION \ref pscOrCpi */
    EmNetworkMode        umtsMode;

    UrrCellIdentityUnion pscOrCpi; 

}EmUmtsCellInfo;
#endif

#if defined(UPGRADE_GGE)
typedef struct EmGsmCellInfoTag
{

    /** The ARFCN of GSM cell.*/
    Arfcn                   arfcn;

    /** The BSIC of GSM cell.*/
    Int8                    bsic;
}EmGsmCellInfo;
#endif

/**
 * Engineering mode cell information.
 */
typedef union EmCellTypeTag
{
    /** GSM cell information.*/
#if defined(UPGRADE_GGE)    
        EmGsmCellInfo   gsmCellInfo;
#endif
    /** UMTS cell information.*/
#if defined(UPGRADE_3G)
        EmUmtsCellInfo  umtsCellInfo; 
#endif
}
EmCellType;

typedef enum EmNCellTypeTag
{
    /** Invalid value  */
    EM_NCELL_INVALID_TYPE = 0,

    /** Inter-freq neighbour cell */
    EM_NCELL_INTER_FREQ,

    /** Inter-rat neighbour cell */
    EM_NCELL_INTER_RAT,

    /** Intra-freq neighbour cell */
    EM_NCELL_INTRA_FREQ
}
EmNCellType;


/**
 * Engineering mode command 'LOCK_CELL' information.
 */
typedef struct EmCellInfoTag
{    
    /** GsmUmts: GSM or UTMS.
\assoc UNION \ref cellType */
   EmGsmUmtsType  gsmUtmsMode;
   
   EmCellType     cellType;
}
EmCellInfo;

typedef EmCellInfo EmLockCellInfo;

typedef union EmArfcnInfoTag
{

#if defined(UPGRADE_GGE)    
        Arfcn        gsmArfcn;
#endif
#if defined(UPGRADE_3G)
        UUARFCN      umtsArfcn;       
#endif        
}EmArfcnInfo;

/**
 * Engineering mode command 'LOCK_ARFCN' information.
 */
typedef struct EmLockArfcnInfoTag
{    
    /** GsmUmtsType: GSM or UTMS.
\assoc UNION \ref arfcnInfo */
   EmGsmUmtsType  gsmUtmsMode;
    
   EmArfcnInfo arfcnInfo;
}
EmLockArfcnInfo;
/**
 * Engineering mode command 'SPECIFIED_ARFCN_SCAN' information.
 */
typedef EmLockArfcnInfo EmSpecifiedArfcnScanInfo;

/**
 * Engineering mode command 'TRIGGER_HANDOVER' information.
 */
typedef struct EmTriggerHandoverInfoTag
{
    /** Tell RRC destination neighbor cell type which is intra-freq, inter-freq or inter-rat cell.
      */
    EmNCellType             nCellType;    
    EmCellInfo              cellInfo;    
      
    /** Possible range is from 3 to 10 seconds, Default value is 5 seconds.
      */
    Int8                    timerPeriod;
}
EmTriggerHandoverInfo;

/**
 * Engineering mode command 'HANDOVER_PERMITTED' information.
 */
typedef struct EmHandoverPermittedInfoTag
{
    /** False indicate switching off handover function, 
      * true means executing handover normally.
      */
    Boolean                 handoverSwitch;
}
EmHandoverPermittedInfo;

/**
 * Engineering mode command 'TRIGGER_CELL_RESELECTION' information.
 */
typedef struct EmTriggerCellReselectionInfoTag
{
    /** Tell RRC destination neighbor cell type which is intra-freq, inter-freq or inter-rat cell.
      */
    EmNCellType             nCellType;
    EmCellInfo              cellInfo;
}
EmTriggerCellReselectionInfo;

typedef struct EmSelectACellOneTimeInfoTag
{
    EmCellInfo              cellInfo;
}
EmSelectACellOneTimeInfo;

/** Engineering mode power saving type, Only used when command is  * ' POWER_SAVING '. */
typedef enum EmPowerSavingTypeTag
{ 
    /** Invalid value  */ 
    EM_POWER_SAVING_OFF = 0,
 
    /** NO CELL - all RxLev is lower than -105dbm */ 
    EM_POWER_SAVING_NO_CELL = 1,

    /** NO SYNC - FB sync always fail for all Arfcns */ 
    EM_POWER_SAVING_NO_SYNC_0 = 2,

    /** NO SYNC - FB sync always fail if already has 5 Arfcns which FB sync successful. */
    EM_POWER_SAVING_NO_SYNC_5 = 3,

    /** NO SYNC - FB sync always fail if already has 6 Arfcns which FB sync successful. */
    EM_POWER_SAVING_NO_SYNC_6 = 4,

    /** SYNC - FB sync always successful. */
    EM_POWER_SAVING_SYNC_OK_ALL = 5,

    /** NO BA - The BA list is empty */
    EM_POWER_SAVING_NO_BA = 6
}
EmPowerSavingType;


/** * Engineering mode command 'POWER_SAVING' information. */
typedef struct EmCmdPowerSavingInfoTag
{ 
    EmPowerSavingType    powerSavingType; 
}
EmCmdPowerSavingInfo;


/***************************************************************************
 *  Macros
 **************************************************************************/


/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

#endif

/* END OF FILE */
