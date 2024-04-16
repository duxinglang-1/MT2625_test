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
 *  File Description :
 *
 *      GPRS data structure typedefs for GRR messages.
 *
 **************************************************************************/

#if !defined (GPGRRTYP_H)
#define       GPGRRTYP_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <l13_typ.h>
#include <l3_typ.h>
#include <l3gp_typ.h>
#include <gpie_typ.h>

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

#define NUM_SUPPORTED_REF_FREQ_LISTS  4
#define NUM_SUPPORTED_MA_LISTS        7
#define NUM_SUPPORTED_PCCCH_DESC     16
#define MAX_SIZE_RAW_RFL_CONTENTS    18
#define MAX_PSI3_CHANNELS            24 /* Calc max is 20 in PSI3bis NCELL2 */
#define MAX_PSI4_CHANNELS             8
#define MAX_LSA_IDS                   5

#define MAX_PCCO_REM_CHANNELS        32
#define MAX_NUM_MEASUREMENTS         32

#define PCCO_INVALID_TARGET 0x00
#define PCCO_GSM_TARGET     0x01
#define PCCO_FDD_TARGET     0x02
#define PCCO_TDD128_TARGET  0x04

#define OPT_NCELL_RXLEV_MAX_TXPWR_BIT       0x0001
#define OPT_NCELL_TEMP_OFFSET_PEN_TIME_BIT  0x0002
#define OPT_NCELL_RESELECT_OFFSET_BIT       0x0004
#define OPT_NCELL_HCS_PARAMS_BIT            0x0008

/* These are used in the deocde and storage of the following bitFields: cellBarAccess2, exclusiveAccess, sameRaAsScell CCN */
#define CELL_BAR_ACCESS_2_BIT   0x01
#define EXCLUSIVE_ACCESS_BIT    0x02
#define SAME_RA_AS_SCELL_BIT    0x04

/* Max no of cellgoups which will physically fit in one NCell2 structure: */
#define MAX_PSI3_NCELL2_CELLGROUPS              5

#if defined (UPGRADE_3G_FDD)
#define MAX_NUM_PSI_15_FDD_UARFCN               12
#endif /* UPGRADE_3G_FDD */

#if defined (UPGRADE_3G_TDD128)
#define MAX_NUM_PSI_15_TDD128_UARFCN            12
#endif /* UPGRADE_3G_TDD128 */

/***************************************************************************
 * Typed Constants
 **************************************************************************/

/***************************************************************************
 * Types
 **************************************************************************/

 typedef enum CellChangeFailureCauseTag
{
    FREQ_NOT_IMPLEMENTED                = 0,
    NO_RESPONSE                         = 1,
    IMM_ASSIGN_REJECT_OR_ACCESS_REJECT  = 2,
    ON_GOING_CS_CONNECTION              = 3,
    ANONYMOUS_ACCESS                    = 4,
    MS_IN_GMM_STANDBY_STATE             = 5,
    FORCE_TO_STANDBY_STATE              = 6,
    RESERVED                            = 7
}
/*
 * Values in this enum must match the values of the CAUSE field of
 * the Packet Cell Change Failure msg - see 04.60 11.2.3
 */
CellChangeFailureCause;

typedef struct MeasurementTag
{
    Int8                        freqN;
    Bsic                        bsicN;             /* 0xFF if field absent */
    Int8                        rxlevN;
}
Measurement;

typedef enum NcellPbcchLocationTag
{
    PBCCH_LOCATION_NOT_PRESENT      = 0,
    SI13_LOCATION_PRESENT           = 1,
    PSI1_PRESENT                    = 2
}
NcellPbcchLocation;

typedef enum ExtMeasOrderTag
{
    EM0                             = 0,
    EM1                             = 1,
    EM_RESERVED                     = 2,
    EM_RESET                        = 3
}
ExtMeasOrder;

typedef struct RefFrequencyListTag
{
    Int8                    rflNumber;
    /*
     * The following holds the actual length of the RFL contents received,
     * in the range 3 - 18 octets, as calculated from the
     * 'Length of RFL contents' field (range 0 - 15) in the received msg:
     */
    Int8                    rflLength;
    Int8                    octet[MAX_SIZE_RAW_RFL_CONTENTS];
}
RefFrequencyList;

typedef struct PccchDescriptionTag
{
    Int8                    tsc;
    Boolean                 hopping;
    Int8                    numEntries;
    Int8                    timeslot[NUM_SUPPORTED_PCCCH_DESC];

    /* For the non-hopping case */
    Arfcn                   arfcn[NUM_SUPPORTED_PCCCH_DESC];

    /* For the hopping case */
    Int8                    maNumber;
    Int8                    maio[NUM_SUPPORTED_PCCCH_DESC];
}
PccchDescription;

typedef struct GprsCellAllocationTag
{
    Int8                    cellAllocation;
}
GprsCellAllocation;

typedef struct ScellParametersTag
{
    Boolean                 cellBarAccess2;
    Boolean                 exclusiveAccess;
    Int8                    gprsRxlevAccessMin;
    Int8                    gprsMsTxPwrMax;
    Boolean                 hcsAvailable;
    Int8                    priorityClass;
    Int8                    hcsThreshold;
    Int8                    multibandReporting;
}
ScellParameters;

typedef struct GenCellSelectionParamTag
{
    Int8                    gprsCrh;
    Boolean                 c31Hyst;
    Boolean                 c32Qual;
    Int8                    tResel;         /* If not present set to zero */
    Int8                    raRh;           /* If not present set to > '111' */
}
GenCellSelectionParam;

typedef struct NcellOptionalParamsTag
{
    Int16                   presenceBits;
    Int8                    gprsRxLevAccessMin;
    Int8                    gprsMsTxPwrMax;
    Int8                    gprsTemporaryOffset;
    Int8                    gprsPenaltyTime;
    Int8                    gprsReselectOffset;
    Int8                    priorityClass;
    Int8                    hcsThreshold;
    NcellPbcchLocation      pbcchLocation;
    Boolean                 si13LocationIsExt;    /* i.e. TRUE == BCCH-ext */
    Int8                    pbcchTimeslot;
    Int8                    pbcchRepeatPeriod;
    Int8                    bitField;            /* cellBarAccess2, exclusiveAccess, sameRaAsScell and CCN */
}
NcellOptionalParams;

typedef struct NcellParametersTag
{
    Int8                    numChannels;
    Int8                    bsic[MAX_PSI3_CHANNELS];
    Arfcn                   arfcn[MAX_PSI3_CHANNELS];
    Int32                   cellBarAcc2;                                /* One bit/ncell. Used in NCELL 2 param decode only */
    Int32                   excAcc;                                     /* One bit/ncell. Used in NCELL 2 param decode only */
    Int32                   sameRaAsServingCell;                        /* One bit/ncell. Used in NCELL 2 param decode only */
    Int8                    cellParamsPointer[MAX_PSI3_NCELL2_CELLGROUPS]; /* Used in NCELL 2 param decode only */
    Int8                    maxCellIndex[MAX_PSI3_NCELL2_CELLGROUPS];      /* Used in NCELL 2 param decode only */
    NcellOptionalParams     optParams[MAX_PSI3_CHANNELS];
}
NcellParameters;

typedef struct ScellLsaInfoTag
{
    Int8                    numLsa;
    Boolean                 shortLsaId[MAX_LSA_IDS];  /* If TRUE, shortLsaId is used instead of LsaId*/
    Int32                   lsaId [MAX_LSA_IDS];
}
ScellLsaInfo;

typedef struct IntChannelGroupTag
{
    Boolean                 formatIsArfcn;
    Arfcn                   arfcn;
    Int8                    maNumber;
    Int8                    maio;
    Int8                    timeslotAllocation;
}
IntChannelGroup;

typedef struct ExtMeasurementParamTag
{
    ExtMeasOrder            em;
    Int8                    nccPermitted;
    Int8                    intFrequencyIndex;
    ExtReportType           type;
    Int8                    period;
    Int8                    numExtFreqList;
    Arfcn                   extFreqList[MAX_EXT_FREQ_LIST];
}
ExtMeasurementParam;

/** Packet Cell Change Order structure */

typedef struct PacketCellChangeOrderTag
{
    /** controls the action of the mobile station belonging 
     * to the paging subgroup corresponding to the paging subchannel
     */
    PageMode                pageMode;
    /** Address used */
    Address                 pccoId;
}
PacketCellChangeOrder;

/** The contents of Packet Cell Change Order */
typedef struct PacketCellChangeOrderBodyTag
{
    /** Indicates if immediate abort of operation in the old cell is required or not */
    Boolean                 immediateRel;
    /** Contains the BCCH frequency of the new cell */
    Arfcn                   arfcn;
    /** Contains the BSIC of the new cell */
    Int8                    bsic;
#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
    Int16                   targetCellRat;
#endif
    /** contains the NETWORK_CONTROL_ORDER and the optional parameters 
     * NC_NON_DRX_PERIOD, NC_REPORTING_PERIOD_I, NC_REPORTING_PERIOD_T 
     * and the NC_FREQUENCY LIST. These parameters shall apply in the target cell 
     */
    NcMeasurementParam      ncParam;
    /** Number of cells to be removed from the 3G Neighbour Cell list for measurements */
    Int8                    numRemovedFreq;
    /** Contains a list of cells to be removed from the 3G Neighbour Cell list for measurements */
    Int8                    removedFreqList [MAX_PCCO_REM_CHANNELS];
#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
     /* If in FDD mode, TDD parameters are required for PCCF purposes */
     /* And if in TDD mode, FDD parameters are required for PCCF purposes */
     /* So here we don't add any UPGRADE_3G_FDD or UPGRADE_3G_TDD128 */

    /** FDD parameters */
    /** Any non-supported frequency shall not be considered as an error; 
     * indices of the 3G Neighbour Cell list shall be incremented accordingly.
     */
    UArfcn                  fddUArfcn;
    /** Whether or not diversity is applied for this cell */
    Boolean                 fddDiversity;
    /** This information element will be used for future releases of the protocol. 
     * When missing, this indicates the present FDD bandwidth. When present, this 
     * shall not be considered as an error; indices of the 3G Neighbour Cell list 
     * shall be incremented accordingly.
     */
    Int8                    fddBandwidth;
    /** Scrambling Code as defined in 3GPP TS 25.213 */
    Int16                   scramblingCode;

    /** TDD128 parameters */
    /** Any non supported frequency shall not be considered as an error; 
     * indices of the 3G Neighbour Cell list shall be incremented accordingly
     */
    Int16                   tdd128UArfcn;

    /** Indicates if SCTD is applied for this cell or not:
          If set to FALSE, SCTD is not applied for this cell; if set to TRUE, SCTD is applied for this cell */         
    Boolean                 tdd128SctdIndicator;
    /** 000   3.84Mcps ; 001  1.28Mcps ; All other values shall not be interpreted as an error */

    Int8                    tdd128Bandwidth;
    /** This parameter is defined in 3GPP TS 25.223. */
    Int8                    tdd128CellParameterID;  /* The sctdIndicator and tstdIndicator information are not included */

    /** For 1.28 Mcps TDD, this parameter indicates if TSTD is applied for the cell or not:
          If set to FALSE, TSTD is not applied for this cell; if set to TRUE, TSTD is applied for this cell */ 
    Boolean                 tdd128TstdIndicator;
#endif
    /** GRNTI extention */
    Int8                    gRntiExtension;
}
PacketCellChangeOrderBody;
/*
 * The iterative structures of the PCCO msg are then decoded into 0 or
 * more instances of the following structures:
 * - NcellParameters
 * - NcellLsaInfo
 * - EnhMeasurementParam
 */


/* Packet Cell Measurement structures: */

typedef struct PacketMeasurementOrderTag
{
    PageMode                pageMode;
    Address                 pmoId;
    Int8                    pmoIndex;
    Int8                    pmoCount;
}
PacketMeasurementOrder;


typedef struct PacketMeasurementOrderBodyTag
{
    NcMeasurementParam      ncParam;
    Int8                    numRemovedFreq;
    Int8                    removedFreqList [MAX_PCCO_REM_CHANNELS];
}
PacketMeasurementOrderBody;

/*
 * The iterative structures of the PMO msg are then decoded into 0 or
 * more instances of the following structures:
 * - NcellParameters
 * - ExtMeasurementParam
 * - NcellLsaInfo
 * - EnhMeasurementParam
 */


/* Packet Prach Parameters structure: */

typedef struct PacketPrachParametersTag
{
    PageMode                pageMode;
    PrachControlParam       prachControl;
}
PacketPrachParameters;


/* PSI 1 structures: */

typedef struct PacketSysInfoType1Tag
{
    PageMode                pageMode;
    Int8                    pbcchChangeMark;
    Int8                    psiChangeField;
}
PacketSysInfoType1;

typedef struct PacketSysInfoType1BodyTag
{
    Int8                    psi1RepeatPeriod;
    Int8                    psiCountLr;
    Boolean                 psiCountHrAvailable;
    Int8                    psiCountHr;
    Boolean                 psi5Available;    /* If not available then
                                               * equivalent to NC=0.
                                               * Otherwise, NC=1 */
    GprsCellOptions         gprsCellOptions;
    PrachControlParam       prachControl;
    PccchOrganisation       pccch;
    GlobalPwrCtrlParam      globalPwrCtrl;
    Boolean                 psiStatusSupported;/* 1 = PSI status supported */
    Boolean                 mscR99OrLater;
    Boolean                 sgsnR99OrLater;
    Int16                   bandIndicator;
}
PacketSysInfoType1Body;


/* PSI 2 structures: */
typedef struct PacketSysInfoType2Tag
{
    PageMode                pageMode;
    Int8                    psi2ChangeMark;
    Int8                    psi2Index;
    Int8                    psi2Count;
}
PacketSysInfoType2;

/*
 * The optional & iterative structures of the PSI2 msg are then decoded
 * into 0 or more instances of each of the following structures:
 * - GprsCellIdentification
 * - NonGprsCellOptions
 * - RefFrequencyList;
 * - GprsCellAllocation;
 * - GprsMobileAllocation;
 * - PccchDescription
 */


/* PSI 3 structures: */

typedef struct PacketSysInfoType3Tag
{
    PageMode                pageMode;
    Int8                    psi3ChangeMark;
    Int8                    psi3BisCount;
}
PacketSysInfoType3;

/*
 * (The iterative structures of the PSI3 msg are then decoded into 0 or
 * more instances of the following structure:
 * - ScellParameters
 * - GenCellSelectionParam
 * - NcellParameters
 * - ScellLsaInfo
 * - NcellLsaInfo
 *
 */


/* PSI 3bis structures: */

typedef struct PacketSysInfoType3bisTag
{
    PageMode                pageMode;
    Int8                    psi3ChangeMark;
    Int8                    psi3BisIndex;
    Int8                    psi3BisCount;
}
PacketSysInfoType3bis;

typedef struct PacketSysInfoType3quaHdrTag
{
    PageMode                pageMode;
    Int8                    psi3ChangeMark;
    Int8                    psi3quaIndex;
    Int8                    psi3quaCount;
}
PacketSysInfoType3quaHdr;

typedef struct PacketSysInfoType3terHdrTag
{
    PageMode                pageMode;
    Int8                    psi3ChangeMark;
    Int8                    psi3terIndex;
    Int8                    psi3terCount;
}
PacketSysInfoType3terHdr;

/*
 * (The iterative structures of the PSI3bis msg are then decoded into 0 or
 * more instances of each of the following structure(s):
 * - NcellParameters
 * - NcellLsaInfo
 */


/* PSI 4 structures: */

typedef struct PacketSysInfoType4Tag
{
    PageMode                pageMode;
    Int8                    psi4ChangeMark;
    Int8                    psi4Count;
    Int8                    psi4Index;
}
PacketSysInfoType4;

/*
 * (The iterative structures of the PSI4 msg are then decoded into 1 or
 * more instances of each of the following structure(s):
 * - IntChannelGroup
 */

typedef struct PacketSysInfoType5Tag
{
    PageMode                pageMode;
    Int8                    psi5ChangeMark;
    Int8                    psi5Count;
    Int8                    psi5Index;
}
PacketSysInfoType5;

typedef struct PacketSysInfoType5BodyTag
{
    NcMeasurementParam          ncParam;
    ExtMeasurementParam         extParam;
    Psi5EnhMeasurementParam     enhMeasParam;
}
PacketSysInfoType5Body;

/*
 * (No iterative structures in the PSI5 msg)
 */


/* PSI 13 structures: */

typedef struct PacketSysInfoType13Tag
{
    PageMode                pageMode;
    Int8                    bcchChangeMark;
    Int8                    siChangeField;
}
PacketSysInfoType13;

typedef struct PacketSysInfoType13BodyTag
{
    Boolean                 gprsMaPresent;
    Int8                    si13ChangeMark;
    GprsMobileAllocation    gprsMa;
    Boolean                 pbcchPresent;

    /* If PBCCH is not present */
    Rac                     rac;
    Boolean                 splitPgCycleSupported;
    Int8                    priorityAccessThreshold;
    NetworkControl          nc;
    GprsCellOptions         gprsCellOptions;
    GprsPowerCtrlParam      pwrCtrl;
    /* Note that SI15_IND is not included as it should have been removed */

    /* If PBCCH is present */
    Int8                    psi1RepeatPeriod;
    PbcchDescription        pbcchDesc;
    Boolean                 sgsnR99OrLater;
    Boolean                 siStatusSupported;
}
PacketSysInfoType13Body;

typedef struct PacketSysInfoType14Tag
{
    Int8                    bcchChangeMark;

    Int8                    si13ChangeMark;
    Boolean                 gprsMaPresent;
    GprsMobileAllocation    gprsMa;
    Boolean                 pbcchPresent;

    /* If PBCCH is not present */
    Boolean                 splitPgCycleSupported;
    Int8                    priorityAccessThreshold;
    NetworkControl          nc;
    GprsCellOptions         gprsCellOptions;
    GprsPowerCtrlParam      pwrCtrl;   /* Same as power ctrl params contained in PSI13/SI13 */

    /* If PBCCH is present */
    Int8                    psi1RepeatPeriod;
    Int8                    pb;
    Int8                    timeslot;
    Boolean                 sgsnR99OrLater;
    FrequencyParameters     pbcchFreqParam;
}
PacketSysInfoType14;

typedef struct PacketSysInfoType8Tag
{
    PageMode                pageMode;
    Int8                    psi8ChangeMark;
    Int8                    psi8Count;
    Int8                    psi8Index;
}
PacketSysInfoType8;

typedef struct PacketSysInfoType8BodyTag
{
    Boolean                     cbchChanDescrPresent;
    PsiCbchChanDescription      psiCbchChanDescr;
}
PacketSysInfoType8Body;

#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
typedef struct PacketSysInfoType15Tag
{

#if defined (UPGRADE_3G_FDD)
    Int16       numFddEntries;
    UArfcn      fddUArfcn[MAX_NUM_PSI_15_FDD_UARFCN];
#endif /* UPGRADE_3G_FDD */

#if defined (UPGRADE_3G_TDD128)
    Int16       numTdd128Entries;
    UArfcn      tdd128UArfcn[MAX_NUM_PSI_15_TDD128_UARFCN];
#endif /* UPGRADE_3G_TDD128 */

}
PacketSysInfoType15;
#endif

/*
 * (No iterative structures in the PSI13 msg)
 */

/*
 * ExtMeasReportTag
 * This structure contains the control message used to send extended cell
 * measurements to the network
 */

typedef struct GpExtMeasReportTag
{
    Int32           tlli;
    Int8            psi5ChangeMark;
    ExtReportType   extReportType;
    Int8            timeslotUsedBitMap;  /* ms bit = I_LEVEL_TN0 present etc */
    Int8            InterferenceLevel[ALL_TIMESLOTS];
    Int8            numOfMeas;
    Measurement     meas [MAX_EXT_MEAS];
}
GpExtMeasReport;

/*
 * CellChangeFailureTag
 * This structure is used in encoding the cell change failure message
 */

typedef enum NonGsmInformationTag
{
    NON_GSM_INFO_NOT_PRESENT        = 0,
    NON_GSM_INFO_IN_PSI6            = 1,
    NON_GSM_INFO_IN_PSI7            = 2,
    NON_GSM_INFO_IN_PSI6_AND_PSI7   = 3
}
NonGsmInformation;

typedef struct AdditionalPsiMsgsTag
{
    Boolean                     psi8Available;
    Boolean                     psi3terAvailable;
    Boolean                     psi3quaAvailable;
    NonGsmInformation           nonGsmInfo;
}
AdditionalPsiMsgs;

typedef struct CompactCtrlInfoTag
{
    Boolean largeCellOperation;
    Int8    idleBlockAlloc;
    Int8    idleBlockCcch[4];
    Int8    nCcchNh;
}
CompactCtrlInfo;

typedef struct NcellCompactOptParamsTag
{
    Int8                    gprsRxLevAccessMin;
    Int8                    gprsMsTxPwrMax;
    Int8                    gprsTemporaryOffset;
    Int8                    gprsPenaltyTime;
    Int8                    gprsReselectOffset;
    Int8                    priorityClass;
    Int8                    hcsThreshold;
    Int8                    guarConsantPwrBlks;
    Int8                    bitField;      /* cellBarAccess2, exclusiveAccess, sameRaAsScell and CCN */
}
NcellCompactOptParams;

#if defined (ENABLE_COMPACT)
typedef struct CompactNcellInfoTag
{
    Int8                    numChannels;
    Int8                    bsic[MAX_PSI3_CHANNELS];
    Arfcn                   arfcn[MAX_PSI3_CHANNELS];
    Int8                    timeGroup[MAX_PSI3_CHANNELS];
    Int32                   cellBarAcc2;                                    /* One bit/ncell. Used in NCELL 2 param decode only */
    Int32                   excAcc;                                         /* One bit/ncell. Used in NCELL 2 param decode only */
    Int32                   sameRaAsServingCell;                            /* One bit/ncell. Used in NCELL 2 param decode only */
    Int8                    cellParamsPointer[MAX_PSI3_NCELL2_CELLGROUPS];  /* Used in NCELL 2 param decode only */
    Int8                    maxCellIndex[MAX_PSI3_NCELL2_CELLGROUPS];       /* Used in NCELL 2 param decode only */
    NcellCompactOptParams   optParams[MAX_PSI3_CHANNELS];
}
CompactNcellInfo;
#else
typedef struct CompactNcellInfoTag
{
    Int8                    numChannels;
    Int8                    bsic;
    Arfcn                   arfcn;
    Int8                    timeGroup;
    Int32                   cellBarAcc2;         /* One bit/ncell. Used in NCELL 2 param decode only */
    Int32                   excAcc;              /* One bit/ncell. Used in NCELL 2 param decode only */
    Int32                   sameRaAsServingCell; /* One bit/ncell. Used in NCELL 2 param decode only */
    Int8                    cellParamsPointer[MAX_PSI3_NCELL2_CELLGROUPS];  /* Used in NCELL 2 param decode only */
    Int8                    maxCellIndex[MAX_PSI3_NCELL2_CELLGROUPS];       /* Used in NCELL 2 param decode only */
    NcellCompactOptParams   optParams;
}
CompactNcellInfo;
#endif

typedef struct PccoRel4AdditionsTag
{
    Boolean                   ccnActive;
    Int8                      containerId;
    Boolean                   ccnSupportPresent;
    Int32                     ccn;
}
PccoRel4Additions;

typedef struct PmoRel4AdditionsTag
{
    Boolean                   ccnActive;
    Boolean                   ccnSupportPresent;
    Int32                     ccn;
}
PmoRel4Additions;

#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
typedef struct PccoRel5AdditionsTag
{
#if defined (UPGRADE_3G_FDD)
    Addit3GMeasParam                addit3GMeasParam;
    Addit3GMeasParam2               addit3GMeasParam2;
#else /* UPGRADE_3G_FDD */
    Int8    dummy;
#endif /* UPGRADE_3G_FDD */
}
PccoRel5Additions;

typedef struct PmoRel5AdditionsTag
{
#if defined (UPGRADE_3G_FDD)
    Addit3GMeasParam                addit3GMeasParam;
    Addit3GMeasParam2               addit3GMeasParam2;
#else /* UPGRADE_3G_FDD */
    Int8    dummy;
#endif /* UPGRADE_3G_FDD */
}
PmoRel5Additions;

typedef struct PccoRel6AdditionsTag
{
    Boolean present;
    Boolean ccn3GActive;
}
PccoRel6Additions;

typedef struct PmoRel6AdditionsTag
{
    Boolean present;
    Boolean ccn3GActive;
}
PmoRel6Additions;
#endif /* UPGRADE_3G && UPGRADE_GGE */

typedef struct PktNcellDataContainerTag
{
    Int8                        id;
    Int8                        index;
    Arfcn                       arfcn;
    Bsic                        bsic;
    Int8                        pd;
    Boolean                     msgComplete;
    Int8                        length;
    Int8                        data[SIZE_OF_CS_1];
}
PktNcellDataContainer;

typedef struct PktScellDataContainerTag
{
    Int8                        index;
    Int8                        pd;
    Boolean                     msgComplete;
    Int8                        length;
    Int8                        data[SIZE_OF_CS_1];
}
PktScellDataContainer;

typedef struct PacketSysInfoType3quaBodyTag
{
    ReportPriorityParam             reportPriority;

#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
    Boolean                         ncell3gDescrPresent;
    Boolean                         measParam3gPresent;
    Boolean                         init3gDedModeReportingPresent;

    Boolean                         release6ParamPresent;
    Boolean                         ccn3GActive;

#if defined (UPGRADE_3G_FDD)
    Boolean                         utranFddNcellParamPresent;
    UtranFddNcellParam              utranFddNcellParam;
    Addit3GMeasParam                addit3GMeasParam;
    Addit3GMeasParam2               addit3GMeasParam2;
#endif /* UPGRADE_3G_FDD */
#if defined (UPGRADE_3G_TDD128)
    Boolean                         utranTdd128NcellParamPresent;
    UtranTdd128NcellParam           utranTdd128NcellParam;
#endif /* UPGRADE_3G_TDD128 */

    Psi3qua3gMeasurementParam       measParam3g;
    Initial3gDedModeReportingDescr  init3gDedModeReporting;
#endif
}
PacketSysInfoType3quaBody;

typedef struct PacketSysInfoType3terBodyTag
{
    Boolean                         rtdDescrPresent;
    RtdDescription                  rtdDescr;
    ReportPriorityParam             reportPriority;
}
PacketSysInfoType3terBody;

/***************************************************************************
 *  Signal Definitions
 **************************************************************************/
#endif

/* END OF FILE */
