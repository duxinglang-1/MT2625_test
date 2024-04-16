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
 *  File Description : Common types between GSM and GPRS for R99 and 3G functionality
 **************************************************************************/

#ifndef L33G_TYP_H
#define L33G_TYP_H

#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
#include <uas_asn.h>
#endif

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

/****************************************************************************
 * L3 Message elements exported to Dual mode (UTRAN FDD)/GSM, GPRS and GSM
 ***************************************************************************/

#define MAX_PREDEF_CONFIG_ID              16

#if defined (UPGRADE_3G_FDD)
#define MAX_NUM_FDD_SCRAMBLING_CODES      32
#define MAX_FDD_NUM_W                     32
#define MAX_NUM_UMTS_ARFCN_FDD            3
#endif /* UPGRADE_3G_FDD */

#if defined (UPGRADE_3G_TDD128)
#define MAX_NUM_TDD128_CELL_PARAMETER_ID     32
#define MAX_TDD128_NUM_W                     32       /* Maximum number of W for TDD128 Neighbour Cell Information */
#define MAX_NUM_UMTS_ARFCN_TDD128            9
#endif /* UPGRADE_3G_TDD128 */

#define MAX_REMOVED_3G_CELL_FIELD          8  /* Max poss per message assuming 22 bits / per N2 (on a per message basis */
#define MAX_NUM_CDMA2000_CELL_PARAM       31
#define MAX_NUM_REP_PRIORITY              16

typedef struct MeasReportingOffsetThresholdTag
{
    Boolean                 present;
    Int8                    offset;
    Int8                    threshold;
}
MeasReportingOffsetThreshold;

typedef struct ReportPriorityParamTag
{
    Int8                    numCells;
    Int8                    repPriority[MAX_NUM_REP_PRIORITY];
}
ReportPriorityParam;

#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)

#if defined (UPGRADE_3G_FDD)
typedef enum FddReportingQuantityTag
{
    REPORT_CPICH_RSCP = 0,
    REPORT_CPICH_ECNO = 1
}
FddReportingQuantity;
#endif  /* UPGRADE_3G_FDD */

/* 04.60 Table 59, CDMA2000 Description Struct */
typedef enum Cdma2000CellInfoTypeTag
{
    CDMA2000_CELL_INFO_NONE,   /* 0   */
    CDMA2000_CELL_INFO_TYPE_1, /* 1   */
    CDMA2000_CELL_INFO_TYPE_2, /* 001 */
    CDMA2000_CELL_INFO_TYPE_3, /* 010 */
    CDMA2000_CELL_INFO_TYPE_4, /* 011 */
    CDMA2000_CELL_INFO_TYPE_5, /* 110 */
    CDMA2000_CELL_INFO_INVALID
}
Cdma2000CellInfoType;

typedef struct Cdma2000CellInfoType1Tag
{
    Int8                    tdMode;
    Int8                    tdPowerLevel;
}
Cdma2000CellInfoType1;

typedef struct Cdma2000CellInfoType2Tag
{
    Int8                    qof;
    Int8                    walshLenA;
    Int16                   auxPilotWalsh;
}
Cdma2000CellInfoType2;

typedef struct Cdma2000CellInfoType3Tag
{
    Int8                    qof;
    Int8                    walshLenB;
    Int16                   auxTdWalsh;
    Int8                    auxTdPowerLevel;
    Int8                    tdMode;
}
Cdma2000CellInfoType3;

typedef struct Cdma2000CellInfoType4Tag
{
    Int8                    sr3PrimPilot;
    Int8                    sr3PilotPower1;
    Int8                    sr3PilotPower2;
}
Cdma2000CellInfoType4;

typedef struct Cdma2k3xAuxPilotParamTag
{
    Int8                    qof;
    Int8                    walshLength;
    Int16                   auxPilotWalsh;
}
Cdma2k3xAuxPilotParam;

typedef struct Cdma2000CellInfoType5Tag
{
    Int8                    sr3PrimPilot;
    Int8                    sr3PilotPower1;
    Int8                    sr3PilotPower2;
    Int8                    qof;
    Int8                    walshLenC;
    Int16                   auxWalshLen;

    Boolean                 auxPilotParam1Present;
    Cdma2k3xAuxPilotParam   auxPilotParam1;

    Boolean                 auxPilotParam2Present;
    Cdma2k3xAuxPilotParam   auxPilotParam2;
}
Cdma2000CellInfoType5;

typedef union Cdma2000CellInfoTag
{
    Cdma2000CellInfoType1   cellInfoType1;
    Cdma2000CellInfoType2   cellInfoType2;
    Cdma2000CellInfoType3   cellInfoType3;
    Cdma2000CellInfoType4   cellInfoType4;
    Cdma2000CellInfoType5   cellInfoType5;
}
Cdma2000CellInfo;

typedef struct Cdma2000CellParamTag
{
/** \assoc UNION \ref cellInfo */
    Cdma2000CellInfoType    cellInfoType;
    Cdma2000CellInfo        cellInfo;
}
Cdma2000CellParam;

typedef struct Cdma2000DescriptionTag
{
    Int8                    frequencyBand;
    Int16                   frequency;
    Int8                    numCells;

    Int16                   pilotPnOffset;
    Cdma2000CellParam       cellParam [MAX_NUM_CDMA2000_CELL_PARAM];
}
Cdma2000Description;

#if defined (UPGRADE_3G_FDD)
typedef struct UtranFddNcellElementTag
{
    Int16   uArfcn;
    Int16   numScramblingCodes;
    Int16   scramblingCode[MAX_NUM_FDD_SCRAMBLING_CODES];
}
UtranFddNcellElement;

typedef struct UtranFddNcellParamTag
{
    Int16                   numNcell;
    Int8                    indexStart3g;
    Int8                    absIndexStartEmr;

    UtranFddNcellElement    ncell[MAX_NUM_UMTS_ARFCN_FDD];
    Int8                    bandwidth;

    /* These fields are used in the decode of the scrambling codes to relieve stack usage and for debug */
    Int8                    numFddCells;
    Int16                   w[MAX_FDD_NUM_W];
}
UtranFddNcellParam;
#endif  /* UPGRADE_3G_FDD */


#if defined (UPGRADE_3G_TDD128)
typedef struct UtranTdd128NcellElementTag
{
    Int16   uArfcn;
    Int16   numCellParametersID;
    Int16   cellParametersID[MAX_NUM_TDD128_CELL_PARAMETER_ID];/* Need to include the sctdIndicator and tstdIndicator information */
}
UtranTdd128NcellElement;


typedef struct UtranTdd128NcellParamTag
{
    Int16                   numNcell;
    Int8                    indexStart3g;
    Int8                    absIndexStartEmr;

    UtranTdd128NcellElement    ncell[MAX_NUM_UMTS_ARFCN_TDD128];
    Int8                       bandwidth;

    /* These fields are used in the decode of the cell parameter ids to relieve stack usage and for debug */
    Int8                    numTdd128Cells;
    Int16                   w[MAX_TDD128_NUM_W];
}
UtranTdd128NcellParam;
#endif /* UPGRADE_3G_TDD128 */


typedef struct Removed3gNcellDescriptionTag
{
    Int8                    n1;
    Int8                    n2;

    Int8                    removed3gNcellIndex[MAX_REMOVED_3G_CELL_FIELD];
    Int8                    ncellDiff          [MAX_REMOVED_3G_CELL_FIELD];
}
Removed3gNcellDescription;

typedef struct Gprs3gMeasurementParamTag
{
    Int8                            qSearchP;
    Boolean                         search3gPrio;
#if defined (UPGRADE_3G_FDD)
    Boolean                         fddRepParamPresent;
    FddReportingQuantity            fddRepQuant;
    Int8                            fddMultiRatReporting; /* 0xFF = not present */
    MeasReportingOffsetThreshold    reportFddParam;
#endif /* UPGRADE_3G_FDD */
#if defined (UPGRADE_3G_TDD128)
    Int8                            tdd128MultiRatReporting; /* 0xFF = not present */
    MeasReportingOffsetThreshold    reportTdd128Param;
#endif /* UPGRADE_3G_TDD128 */
#if defined (INCLUDE_CDMA2000)
    /* The following only present in packet measurement order */
    Boolean                         cdma2000ReportPresent;
    Int8                            cdma2000MultiRatReporting; /* 0xFF = not present */
    MeasReportingOffsetThreshold    reportCdma2000Param;
#endif
}
Gprs3gMeasurementParam;

#if defined (UPGRADE_3G_FDD)
typedef struct Addit3GMeasParamTag
{
    Boolean     present;
    Int8        fddQMinOffset;
    Int8        fddRscpMin;
}
Addit3GMeasParam;

typedef struct Addit3GMeasParam2Tag
{
    Boolean     present;
    Int8        fddReportThreshold2;
}
Addit3GMeasParam2;
#endif /* UPGRADE_3G_FDD */

typedef struct Psi3qua3gMeasurementParamTag
{
    Int8                    qSearchP;
    Boolean                 search3gPrio;
#if defined (UPGRADE_3G_FDD)
    Int8                    fddGprsQOffset;
    Int8                    fddQmin;
#endif /* UPGRADE_3G_FDD */
#if defined (UPGRADE_3G_TDD128)
    Int8                    tdd128GprsQOffset;
#endif /* UPGRADE_3G_TDD128 */
}
Psi3qua3gMeasurementParam;

typedef struct MeasurementParameter3gDescrTag
{
    Int8                    qSearchI;
    Boolean                 qSearchCInitial;
#if defined (UPGRADE_3G_FDD)
    Int8                    fddQOffset;
    FddReportingQuantity    fddRepQuant;
    Int8                    fddMultiRatReporting;
    Int8                    fddQMin;
#endif /* UPGRADE_3G_FDD */
#if defined (UPGRADE_3G_TDD128)
    Int8                    tdd128QOffset;
    Int8                    tdd128MultiRatReporting;
#endif /* UPGRADE_3G_TDD128 */
}
MeasurementParameter3gDescr;

typedef struct Initial3gDedModeReportingDescrTag
{
    Boolean                 baInd3g;
    Int8                    qSearchI;
    Boolean                 qSearchCInitial;
#if defined (UPGRADE_3G_FDD)
    Int8                    fddQOffset;
    FddReportingQuantity    fddRepQuant;
    Int8                    fddMultiRatReporting;
#endif /* UPGRADE_3G_FDD */
#if defined (UPGRADE_3G_TDD128)
    Int8                    tdd128QOffset;
    Int8                    tdd128MultiRatReporting;
#endif /* UPGRADE_3G_TDD128 */

}
Initial3gDedModeReportingDescr;

typedef struct MiMeasurementParameter3gTag
{
    Int8                            qSearchC;
    Boolean                         searchPriority3g;
#if defined (UPGRADE_3G_FDD)
    FddReportingQuantity            fddRepQuant;

    Int8                            fddMultiRatReporting;
    MeasReportingOffsetThreshold    fddReport;
#endif /* UPGRADE_3G_FDD */
#if defined (UPGRADE_3G_TDD128)
    Int8                            tdd128MultiRatReporting;
    MeasReportingOffsetThreshold    tdd128Report;
#endif /* UPGRADE_3G_TDD128 */
}
MiMeasurementParameter3g;

typedef struct PredefConfigStatusInfoTag
{
    Boolean                         allConfigIdsReceived;
/** \assoc ARRAY \ref configId */
    Int8                            numConfigIds;
    UPredefinedConfigIdentity       configId [MAX_PREDEF_CONFIG_ID];
}
PredefConfigStatusInfo;

#if defined (UPGRADE_3G_FDD)
typedef struct FddCellDescrTag
{
    Int16                   numCells;
    UtranFddNcellElement    cell[MAX_NUM_UMTS_ARFCN_FDD];
    /* These fields are used in the decode of the scrambling codes to relieve stack usage and for debug */
    Int8                    numFddCells;
    Int16                   w[MAX_FDD_NUM_W];
}
FddCellDescr;
#endif /* UPGRADE_3G_FDD */

#if defined (UPGRADE_3G_TDD128)
typedef struct Tdd128CellDescrTag
{
    Int16                    numCells;
    UtranTdd128NcellElement  cell[MAX_NUM_UMTS_ARFCN_TDD128];
    /* These fields are used in the decode of the scrambling codes to relieve stack usage and for debug */
    Int8                     numTdd128Cells;
    Int16                    w[MAX_TDD128_NUM_W];
}
Tdd128CellDescr;
#endif /* UPGRADE_3G_TDD128 */


#endif /* UPGRADE_3G && UPGRADE_GGE */
#endif

/* END OF FILE */

