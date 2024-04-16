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
 *      GRR data types exported
 **************************************************************************/

#if !defined (GRR_TYP_H)
#define       GRR_TYP_H

#include <gprlctyp.h>
#include <gpdecgrr.h>
#include <gpdecrlc.h>

#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
#if !defined (UMPH_TYP_H)
#include <umph_typ.h>
#endif
#endif

/**** MANIFEST CONSTANTS ***************************************************/

#define     NUM_NCELLS                     6

#define     MAX_PCCO_NCELL                 20
#define     MAX_PMO_NCELL                  20

/* Approx limits for PSI2 IEs: */
#define     MAX_PSI2_RFL                   6
#define     MAX_PSI2_GCA                   40
#define     MAX_PSI2_GMA                   20
#define     MAX_PSI2_PCCCH                 50

/* Approx limits for PSI3 IEs: */
#define     MAX_PSI3_NCELL                 10
#define     MAX_PSI3BIS_NCELL              10

#define MAX_NUM_NCELL_SYS_INFO             8
#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
#define MAX_NUM_UTRAN_UMTS_RESEL_NCELL     6
#endif /* UPGRADE_3G && UPGRADE_GGE */
#define MAX_NUM_SYNC_NCELL                 (MAX_NUM_NCELL_SYS_INFO)

/** System information operation element */
typedef enum SiBufferOperationTag
{
    /** System information buffer is allocated */
    SI_BUFFER_ALLOCATED = 0,

    /** System information buffer is re-linked */
    SI_BUFFER_RELINKED = 1,

    /** System information buffer is deallocated */
    SI_BUFFER_DEALLOCATED = 2,

    /** ERROR in SI buffer; i.e buffer not available or buffer mis-aligned with the BA list */
    SI_BUFFER_ERROR = 3,

    /** Buffer used to dump anuy new SI for debugging */
    SI_BUFFER_DUMP = 4
}
SiBufferOperation;

typedef enum SiBufferStatusTag
{
    SI_BUFFER_INFO_VALID   = 0,
    SI_BUFFER_INFO_INVALID = 1
}
SiBufferStatus;

typedef enum NcellDedBsicOpLocationTag
{
    BSIC_OP_INIT                = 0,
    BSIC_OP_MEAS_PROCESSING     = 1,
    BSIC_OP_BSIC_CNF_PROCESSING = 2,
    BSIC_OP_RESTART_BA_CHANGE   = 3,
    BSIC_OP_RESTART_NCELL_MEAS  = 4,
    BSIC_OP_RESTART_BSIC_CNF    = 5,
    BSIC_OP_BEST_NCELL_PROC_0   = 6,
    BSIC_OP_BEST_NCELL_PROC_1   = 7,
    BSIC_OP_RESUME              = 8
}
NcellDedBsicOpLocation;

/** Neighbour cell BSIC operation elements */
typedef enum GrrNcellBsicMonOpTag
{
    NCELL_BSIC_OP_NO_OP        = 0,       /**< Nothing to do */
    NCELL_BSIC_OP_VERIFICATION = 1,       /**< Performing verification */
    NCELL_BSIC_OP_NEW_NCELL    = 2,       /**< New NCELL found */
    NCELL_BSIC_OP_SUSPENDED    = 3        /**< Suspended due to handover or channel assignment */
}
GrrNcellBsicMonOp;

typedef enum GrrMaTypeTag
{
    MA_IN_BITMAP_FORMAT      = 0,
    MA_IN_ARFCN_INDEX_FORMAT = 1
}
GrrMaType;

typedef enum SystemInfoSourceTag
{
    SI_FROM_BCCH  = 0,
    SI_FROM_PBCCH = 1
}
SystemInfoSource;

typedef enum PccchChannelTypeTag
{
    PCCCH_IS_NON_HOPPING = 0,
    PCCCH_IS_HOPPING     = 1
}
PccchChannelType;

typedef struct GrrNcellPsiLocationTag
{
    Int8 pbcchTimeslot;
    Int8 pbcchRepeatPeriod;
}
GrrNcellPsiLocation;

typedef struct PccchMaTag
{
    Int8 maNumber;
    Int8 maio;
}
PccchMa;

typedef struct GrrPccchInfoTag
{
    /* First the PCCCH description received in PSI2 */
    Int8             bsPccChans;
    PccchChannelType chanType[NUM_SUPPORTED_PCCCH_DESC];
    Int8             tsc[NUM_SUPPORTED_PCCCH_DESC];
    Int8             timeslot[NUM_SUPPORTED_PCCCH_DESC];
#if defined (GRR_TST_DUMP_SYSTEM_INFO)
    Arfcn            arfcn[NUM_SUPPORTED_PCCCH_DESC];
    PccchMa          pccchMa[NUM_SUPPORTED_PCCCH_DESC];
#else
    union
    {
        Arfcn   arfcn[NUM_SUPPORTED_PCCCH_DESC];
        PccchMa pccchMa[NUM_SUPPORTED_PCCCH_DESC];
    }
    chan;
#endif

    /* Now the PCCCH organisation copied from PccchOrganisation */
    Boolean organisationValid;
    Int8    bsPccRel;
    Int8    bsPbcchBlks;
    Int8    bsPagBlksRes;
    Int8    bsPrachBlks;
}
GrrPccchInfo;

typedef struct GrrGprsMobileAllocationTag
{
    Int8          hsn;
    Int8          maNumber;
    GprsRflNumber rfl;
/** \assoc UNION \ref ma */
    GrrMaType     maType;
    union
    {
        GprsRawMa          rawMa;
        GprsArfcnIndexList arfcnIndex;
    }
    ma;
}
GrrGprsMobileAllocation;

typedef struct BcchBufferInfoTag
{
    Arfcn          arfcn;
    Bsic           bsic;
    Boolean        bcchDataValid;

    Lai            lai;
    CellId         cellId;
    Int8           numEntriesInCa;
    Arfcn          caArfcn[MAX_BA_CA_CHANNELS];
    BcchAllocation si2AndBisBa;             /* neighbour cell list contained in SI2 + SI2bis  */
    BcchAllocation si2bisBa;                /* neighbour cell list contained in SI 2bis   */
    BcchAllocation si2TerBa;                /* neighbour cell list contained in SI 2/5ter */
    Int8           nccPermitted;

    /* Start of ReselectionParameters */
    /* General band and level data for serving cell */
    Band           band;

    /* C1 criteria */
    /** The maximum allowed power for transmission on CCH in the service cell */
    Int8           msTxpwrMaxCch;

    /** Minimum access level for serving cell */
    Int8           rxlevAccessMin;

    /** Power Offset which is only used by DCS 1800 Class 3 MS */
    Int8           dcsPowerOffset;

    /* Now the C2 parameters */

    /** The cell reselection offset.
     * The cell reselection offset is relatively static, mandatory parameter, which can be positive or
     * negative or zero. It is used to adjust the measurement result of the cell for network
     * planning purposes.
     */
    SignedInt16 reselectOffset;            /* Both GPRS and CS cell reselection */
    /** Used to obtain adjusted signal quality measurements of the candidate cell */
    Int16       temporaryOffset;           /* Both GPRS and CS cell reselection */

    /** When the MS places the cell on the list of the strongest carriers, it starts a timer,
     * which expires after the Penalty_Time. This timer will be reset when the cell is taken
     * off the list
     */
    Int16   penaltyTime;                   /* Both GPRS and CS cell reselection */
    Int32   penaltyTimeTicks;              /* Both GPRS and CS cell reselection */
    Boolean tPenaltyRunning;
    Int32   tPenaltyTimeStamp;

    /** Indicates the Additional Hysteresis which applies in Ready state for cells in same RA;
     * it ranges from 0 to 14 dB in steps of 2 dB.
     * A Cell Reselect Hysteresis is necessary when the mobile is transported along the border
     * of two cells
     */
    Int16 cellReselectHysteresis;

    /** Reselection flags.
     * The following flags are maintained in the following bit field flags: <UL>
     * <LI> C2_PARAM_PRESENT
     * <LI> CELL_BAR_ACCESS
     * <LI> CELL_BAR_QUALIFY
     * <LI> HCS_AVAILABLE
     * <LI> RA_SAME_AS_SERVING_CELL
     * <LI> C31_HYST
     * <LI> C32_QUAL
     * </UL>
     */
    Int16 reselFlags;

    /* Now the GPRS specific stuff */
    Int8  hcsThreshold;
    Int8  priorityClass;
    Int16 gprsCellReselectHysteresis;
    Int16 raReselectHysteresis;
    Int8  gpRxlevAccessMin;
    Int8  gpMsTxpwrMaxCch;
    Int8  nonGpMsTxpwrMaxCch;

    /* End ReselectionParameters */

    /* from RachControlParamElement */

    /** The maximum number of permitted retransmissions of RACH bursts
     * during an access derived from the system information broadcast
     * by the network
     */
    Int16 maxRetrans;                        /* translated by decode */
    Int16 txInteger;

    /** Indicates whether the broadcast system information permits
     * re-establishment for circuit switched calls
     */
    Boolean     reestabNotAllowed;

    /** The access class of the serving cell from system information */
    AccessClass accessClass;

    /* from ControlChannelDescrElement  */
    /** TRUE if MSC is R99+, FALSE if R98- */
    Boolean     mscR99OrLater;

    /** IMSI attach detach allowed flag coded as a 1 bit number
     * contained in the LSB of the second byte.
     */
    Boolean attachDetachAllowed;

    /** The number of block on the BCCH carrier reserved for access grant
     * (derived from system information)
     */
    Int8              numAccessGrantBlocks;

    /** The configuration of the CCCH channel */
    CcchConfiguration ccchConf;

    /** Page period in 51-frame multiframes.
     * The Mobile Station MS should receive 1 paging block during this period.
     * Range 2 - 9.
     */
    Int8 bsPaMfrms;                                 /* raw value + 2 */
    /** TMSI Time (T3212) */
    Int8 t3212;

    /* from CellOptionsElement in GrrCellOptions */

    /** Whether to include sCell in measurements.
     * If TRUE C0 not included in serving-cell measurements.
     */
    Boolean                 pwrc;

    /** Whether Discontinuous Transmission should be used on new channel */
    DtxIndicator            dtx;

    /** Radio link timeout value 0, 4, 8...64 */
    Int8                    radioLinkTimeout;

    /* from CellSelectionParamElement */
    Boolean                 additionalCellParams;
    Boolean                 neci;

    /* re-selection parameters - from SI rest octets */
    /** Indicates whether system information 2 ter is being broadcast */
    Boolean                 si2TerAvailable;

    /** Indicates whether early classmark sending is required by the network */
    Boolean                 ecsControl;

    /** Indicates the band of the current serving cell */
    Int8                    baInd;

    /** Additional GSM network parameters; from 2G SIM */
    Si2BaList               si2BaList;

    /** Derived from system information */
    Int8                    multiBandNcellReports;

    Boolean                 cbchSupported;
    ChannelDescrElement     cbch;                     /* L1 - CBCH */
    /*MobileAllocationElement     cbchMa; Removed this is is brings the signal size under the top mem. pool size */

    /** Indicates if SI 9 is available */
    Boolean                 si9Available;
    Boolean                 gprsIndicatorPresent;

    /** Indicates if SI 13 is available */
    Boolean                 si13Available;
    Int8                    si9Location;
    Int8                    raColour;
    Boolean                 si13IsInBcchExt;

    /* System Information 13 fields */
    Int8                    bcchChangeMark;
    Int8                    siChangeField;

    /** This is contained in SI13. If SI13 is not received then it is set to the LAC */
    Rac                     rac;

    SystemInfoSource        siSource;
    Boolean                 maFourteenPresent;
    Int8                    si13ChangeMark;
    GrrGprsMobileAllocation maFourteen;
    Boolean                 pbcchPresent;

    /* If PBCCH is not present */
    Boolean                 splitPgCycleSupported;
    Int8                    priorityAccessThreshold;

    /* And now the GPRS cell options */
    GprsNetworkMode         gprsNetworkMode;
    Int32                   t3168InTicks;
    Int32                   t3192InTicks;

    /** DRX max is calculated by:- 2^(drxTimerMax - 1) seconds */
    Int8                    drxTimerMax;
    AccessBurstType         accessBurstType;
    Boolean                 ackIsRlcMacFormat;
    GlobalPwrCtrlParam      globalPwrCtrlParam;

    Int8                    bsCvMax;
    Int8                    panDec;
    Int8                    panInc;
    Int8                    panMaxN3102;      /* in units if 4 i.e. 0 = 4, 1 = 8,.... 7 = 32 */
    /** TRUE if EGPRS available on cell */
    Boolean                 egprsSupported;
#if defined (UPGRADE_EDGE)
    Boolean                 eprachSupported;
    Int8                    bepPeriod;
#endif
    Boolean                 pfcFeatureModeSupported;
    Boolean                 dtmSupported;
    Boolean                 bssPagingCoordination;
    Boolean                 extUlTbfSupported;
    Boolean                 ccn2GActive;
    Int32                   ccn;
#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
    Boolean                 ccn3GActive;
#endif

    /* End of GPRS cell options */

    /* Note that SI15_IND is not included as it should have been removed */

    /* If PBCCH is present */
    Int8              psi1RepeatPeriod;
    PbcchDescription  pbcchDesc;

    /** TRUE if SGSN is R99+, FALSE if R98- */
    Boolean           sgsnR99OrLater;
    Boolean           siStatusSupported;

    /* Common Elements */
    /* PacketSysInfoType1 */
    Int8              pbcchChangeMark;
    Int8              psiChangeField;

    Int8              psiCountLr;
    Int8              psiCountHr;

    /** If not available then equivalent to NC=0. Otherwise, NC=1 */
    Boolean           psi5Available;
    PrachControlParam prachControl;
    Boolean           psiStatusSupported;          /* 1 = PSI status supported */

    /* PacketSysInfoType2 */
    Int8              psi2ChangeMark;

    /*
    * The optional & iterative structures of the PSI2 msg are then decoded
    * into 0 or more instances of each of the following structures: */
    Boolean      ecBit;
    GrrPccchInfo pccch;

    /* PacketSysInfoType3 & PacketSysInfoType3bis */
    Int8         psi3ChangeMark;

    /* (The iterative structures of the PSI3 msg are then decoded into 0 or
    ** more instances of the following structure: */
    Boolean      exclusiveAccess;

#if defined (ENABLE_LSA)
    /* For the time being, LSA is not supported */
    ScellLsaInfo scellLsaInfo;
    NcellLsaInfo ncellLsaInfo;
#endif

    /* PacketSysInfoType3bis */

    /* (The iterative structures of the PSI3bis msg are then decoded into 0 or
    ** more instances of each of the following structure(s):
    ** - NcellParameters
    ** - NcellLsaInfo */

    /* PacketSysInfoType4 */
    Int8                psi4ChangeMark;

    /* PacketSysInfoType5 */
    Int8                psi5ChangeMark;
    NcMeasurementParam  ncParam;
#if defined (ENABLE_GPRS_EXT_MEASUREMENTS)
    ExtMeasurementParam extParam;
#endif

    /** T_RESEL in PSI3 determines the length of time for which this ncell cannot
     * be re-selected if another suitable cell exists
     */
    Int32 tReselInTicks;

    /*
    * (No iterative structures in the PSI5 msg)
    */

    /* Note that SI15_IND is not included as it should have been removed from the spec. */
    Int8                         psi8ChangeMark;
#if defined (ENABLE_COMPACT)
    CompactCtrlInfo              compactCtrlInfo;
#endif
    Boolean                      additionalPsiDecoded;
    AdditionalPsiMsgs            additionalPsi;

#if !defined (REMOVE_CB)
    FrequencyParameters          cbchFreqParam;
    FrequencyParameters          psi14PbcchFreqParam;
#endif  /* REMOVE_CB */

    Boolean                      si2QuaterAvailable;
    Boolean                      si2QuaterInBcchExt;
    Int8                         si2QuaCount;
    Int16                        si2QuaIndexReceived;

    /* Taken from SI2Qua. 3G ncell descr, BSIC descr, RTD ... etc are stored external to BCCH data */
    Int8                         baInd2Qua;
    Int8                         mpChangeMark;
    MeasReportType               reportType;
    MeasReportType               gprsReportType;
    MeasReportRate               reportRate;
    Boolean                      invalidBsicReporting;
    Int8                         servingBandReporting;
    Int8                         gprsServingBandReporting;
    Int8                         multibandReporting;
    Int8                         scaleOrd;
    ReportPriorityParam          reportPriority;

    MeasReportingOffsetThreshold report900Param;
    MeasReportingOffsetThreshold report1800Param;
    MeasReportingOffsetThreshold report1900Param;
    MeasReportingOffsetThreshold report850Param;
#if defined (UPGRADE_GSM400)
    MeasReportingOffsetThreshold report400Param;
#endif
#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
    Boolean                      si2TerRestOctetsPresent;
    Boolean                      si2TerRestOctetsReceived;
    Int8                         si2TerCount;
    Int16                        si2TerIndexReceived;
    Int8                         qSearchI;
    Int8                         qSearchP;
    Boolean                      search3gPrio;
#if defined (UPGRADE_3G_FDD)
    FddReportingQuantity         fddRepQuant;
    Int8                         fddMultiRatReporting;
    FddReportingQuantity         fddRepQuantGprs;
    Int8                         fddReportOffset;
    Int8                         fddReportThreshold;
    Int8                         fddQOffset;
    Int8                         fddQMin;
    Int8                         initialDedModeFddQOffset;
#endif /* UPGRADE_3G_FDD */
    Int8                         mpChangeMark2ter;     /* SI2ter rest octets */
    Int8                         changeMark3g2ter;     /* SI2ter rest octets */
    Int16                        numEntries;
    UArfcn                       uArfcn[8]; /* received in SI2ter */     /* Shared by FDD and TDD128 */
#if defined (UPGRADE_3G_TDD128)
    Int8                         tdd128MultiRatReporting;
    Int8                         tdd128ReportOffset;
    Int8                         tdd128ReportThreshold;
    Int8                         tdd128QOffset;
    Int8                         initialDedModeTdd128QOffset;
#endif /* UPGRADE_3G_TDD128 */
#endif
}
BcchBufferInfo;

/** Neighbour cell BCCH buffer information */
typedef struct NcellBcchBufferInfoTag
{
    /* Masks which are used to control system information message reception  */
    /** Neighbour cell ARFCN */
    Arfcn arfcn;

    /** BA Index */
    Int8  baIndex;

    /** Base transceiver Station Identity Code of the serving cell; this is a code in GSM which is
     * broadcast on the SCH (Synchronization Channel) in order to identify the NCC
     *(Network Colour Code) and the BCC (Base Station Colour Code)
     */
    Bsic bsic;

    /** Path loss parameter used for cell selection and reselection.
     * This is calculated by the MS and used for deciding which cell to camp to
     */
    SignedInt16 c1;
    SignedInt16 c2;

    /** LAI; Identifies uniquely a location area for a GSM-MAP type of PLMN */
    Lai         lai;

    /* Start of ReselectionParameters */
    /** General band and level data for serving cell */
    Band        band;

    /* C1 criteria */
    Int8        msTxpwrMaxCch;

    /** Sets the minimum received signal level at the mobile
     * station required for the mobile station to access
     * the cell
     */
    Int8        rxlevAccessMin;
    Int8        dcsPowerOffset;

    /* Now the C2 parameters */
    /** Applies an offset to the Cell 2 (C2) reselection criterion */
    SignedInt16 reselectOffset;            /* Both GPRS and CS cell reselection */

    /** Sets the offset applied to C2 for the duration of C2
     * penalty time
     */
    Int16 temporaryOffset;                 /* Both GPRS and CS cell reselection */

    /** Sets the duration that the C2 temporary offset
     * is applied
     */
    Int16   penaltyTime;                   /* Both GPRS and CS cell reselection */
    Int32   penaltyTimeTicks;              /* Both GPRS and CS cell reselection */
    Boolean tPenaltyRunning;
    Int32   tPenaltyTimeStamp;

    /** Indicates the Additional Hysteresis which applies in Ready state for cells in same RA;
     * it ranges from 0 to 14 dB in steps of 2 dB.
     * A Cell Reselect Hysteresis is necessary when the mobile is transported along the border
     * of two cells.
     */
    Int16 cellReselectHysteresis;

    /** Reselection flags.
     * The following flags are maintained in the following bit field flags: <UL>
     * <LI> C2_PARAM_PRESENT
     * <LI> CELL_BAR_ACCESS
     * <LI> CELL_BAR_QUALIFY
     * <LI> HCS_AVAILABLE
     * <LI> RA_SAME_AS_SERVING_CELL
     * <LI> C31_HYST
     * <LI> C32_QUAL
     * </UL>
     */
    Int16            reselFlags;

    /* Now the GPRS specific stuff */
    Int8             hcsThreshold;
    Int8             priorityClass;
    Int16            gprsCellReselectHysteresis;
    Int16            raReselectHysteresis;
    Int8             gpRxlevAccessMin;
    Int8             gpMsTxpwrMaxCch;
    Int8             nonGpMsTxpwrMaxCch;

    /* End ReselectionParameters */

    SystemInfoSource siSource;
    SignedInt16      c31;
    SignedInt16      c32;
    SignedInt16      c4;

    /** Indicates if BCCH data is valid */
    Boolean          bcchDataValid;

    /** Flags used in ncell BCCH processing. As there are many ncell BCCH
     * buffers these are stored as the following bits: <UL>
     * <LI> C2_BETTER_THAN_SC,
     * <LI> C2_BETTER_THAN_SC_FOR_5S
     * <LI> CELL_BAR_ACCESS
     * <LI> ADDITIONAL_CELL_PARAMS
     * <LI> SI9_AVAILABLE
     * <LI> GPRS_INDICATOR
     * <LI> SI13_AVAILABLE
     * <LI> SI13_IN_BCCH_EXT
     * </UL>
     */
    Int16               ncFlags;

    /** Flags indicated phase 2+ features in SI3/SI4 rest octets */
    Int8                si9Location;
    Int8                raColour;

    AccessClass         accessClass;

    /* System Information 13 fields */
    Int8                bcchChangeMark;
    Int8                siChangeField;

    /** Guard (delay) timer to prevent channel request after imm. assignment reject */
    Int32               t3122InTicks;

    /** Indicates if T3122 is running */
    Boolean             t3122Running;

    /** Guard timer used to prevent cell being reselected within 5 seconds of being "deselected" */
    Boolean             tExclusionRunning;

    /** Timer used to detect if a ncell C2 is greater than scell for more than 5 seconds */
    Boolean             tC2GreaterThanScellRunning;

    NcellPbcchLocation  pbcchLocation;
    GrrNcellPsiLocation psi1Location;

    /** Guard (delay) timer to prevent channel request after imm. assignment reject */
    Int32               t3142InTicks;

    /** Indicates if T3142 is running */
    Boolean             t3142Running;

    /** Guard (delay) timer to prevent channel request after packet access reject */
    Int32               t3172InTicks;

    /** Indicates if T3172 is running */
    Boolean             t3172Running;

    /** T_RESEL in PSI3 determines the length of time for which this ncell cannot be
     * re-selected if another suitable cell exists
     */
    Int32   tReselInTicks;
    Boolean tReselRunning;
#if defined (ENABLE_ENHANCED_CELL_RESEL)
    /** The raw system information 13 */
    Int8    rawSi13[23];
#endif
}
NcellBcchBufferInfo;

typedef struct GrrNcellBsicReqTag
{
    Int16 baIndex[MAX_NUM_SYNC_NCELL];
    Arfcn arfcn[MAX_NUM_SYNC_NCELL];

    /** The number of requests
\assoc ARRAY \ref baIndex
\assoc ARRAY \ref arfcn */
    Int16 numRequests;
    Int16 numVerify;
}
GrrNcellBsicReq;

typedef struct GrrNcellTopSixTag
{
    /** Number of valid entries in the index field
\assoc ARRAY \ref index */
    Int8 numEntries;

    /** Index to neighbor cells */
    Int8 index[MAX_NUM_SYNC_NCELL];
}
GrrNcellTopSix;

typedef struct GrrNcellOldTopSixTag
{
/** \assoc ARRAY \ref index
\assoc ARRAY \ref arfcn */
    Int8  numEntries;
    Int8  index[MAX_NUM_NCELL_SYS_INFO];
    Arfcn arfcn[MAX_NUM_NCELL_SYS_INFO];
}
GrrNcellOldTopSix;

/** Tpedef of L3PktAccessReject */
typedef PacketAccessReject L3PktAccessReject;

/** This message is sent on the PCCCH or PACCH by the network to the mobile
 * station to command the mobile station to leave the current cell and change
 * to a new cell. For a (3G) multi-RAT mobile station the new cell may be a 3G Cell.
 */
typedef struct L3PktCellChangeOrderTag
{
    /** The header of the Packet Cell Change Order */
    PacketCellChangeOrder     pccoHeader;

    /** The body of the Packet Cell Change Order */
    PacketCellChangeOrderBody pccoBody;

    /** LSA parameters */
    NcellLsaInfo              pccoNcLsa;
    PccoOptIeType             pccoOptIeType;
    NcellParameters           pccoNcell; /*[MAX_PCCO_NCELL];*/
    EnhMeasurementParam       enhMeasParam;
    PccoRel4Additions         rel4Additions;
#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
    PccoRel5Additions         rel5Additions;
    PccoRel6Additions         rel6Additions;
#endif
}
L3PktCellChangeOrder;

/** Packet downlink assignmen elements */
typedef struct L3PktDownlinkAssignmentTag
{
    /** The header of RLC/MAC message */
    RlcMacMsgHeader        rmmh;

    PacketDnlinkAssignment pdaBody;
}
L3PktDownlinkAssignment;

typedef struct L3PktMeasurementOrderTag
{
    PacketMeasurementOrder     pmoHeader;
    PacketMeasurementOrderBody pmoBody;
    ExtMeasurementParam        pmoExt;
    NcellLsaInfo               pmoNcLsa;
    PmoOptIeType               pmoOptIeType;
    NcellParameters            pmoNcell;
    EnhMeasurementParam        enhMeasParam;
    PmoRel4Additions           rel4Additions;
#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
    PmoRel5Additions           rel5Additions;
    PmoRel6Additions           rel6Additions;
#endif
}
L3PktPacketMeasurementOrder;

typedef PacketPagingRequest L3PktPagingRequest;
typedef PacketPdchRelease L3PktPdchRelease;
typedef PacketPollingRequest L3PktPollingRequest;

typedef struct L3PktPwrCtrlTimingAdvTag
{
    RlcMacMsgHeader rmmh;
    PacketPwrCtrlTa pctaBody;
}
L3PktPwrCtrlTimingAdv;

typedef PacketPrachParameters L3PktPrachParameters;
typedef PacketQueueingNotification L3PktQueuingNotification;

typedef struct L3PktTimeslotReconfigTag
{
    RlcMacMsgHeader     rmmh;
    PacketTimeslotRecfg ptrBody;
}
L3PktTimeslotReconfig;

typedef PacketTbfRelease L3PktTbfRelease;

typedef struct L3PktUplinkAckNackTag
{
    RlcMacMsgHeader     rmmh;
    PacketUplinkAckNack puanBody;
}
L3PktUplinkAckNack;

typedef struct L3PktUplinkAssignmentTag
{
    RlcMacMsgHeader        rmmh;
    PacketUplinkAssignment puaBody;
}
L3PktUplinkAssignment;

typedef PacketDnlinkDummyCtrlBlock L3PktDnlinkDummyCtrlBlock;

typedef struct L3PktSysInfo1Tag
{
    PacketSysInfoType1     psi1Header;
    PacketSysInfoType1Body psi1Body;
}
L3PktSysInfo1;

typedef struct L3PktSysInfo2Tag
{
    PacketSysInfoType2     psi2Header;
    GprsCellIdentification psi2Gci;
    NonGprsCellOptions     psi2Ngco;
    Psi2OptIeType          psi2OptIeType;
    RefFrequencyList       psi2Rfl;                 /*[MAX_PSI2_RFL];*/
    GprsCellAllocation     psi2Gca;                 /* [MAX_PSI2_GCA];*/
    GprsMobileAllocation   psi2Gma;                 /* [MAX_PSI2_GMA]; */
    PccchDescription       psi2Pccch;               /* [MAX_PSI2_PCCCH];*/
    CompactCtrlInfo        psi2CompactCtrlInfo;
    AdditionalPsiMsgs      psi2AdditionalPsiMsgs;
}
L3PktSysInfo2;

typedef struct L3PktSysInfo3Tag
{
    PacketSysInfoType3     psi3Header;
    ScellParameters        psi3Scell;
    GenCellSelectionParam  psi3Gcell;
    ScellLsaInfo           psi3ScLsa;
    NcellLsaInfo           psi3NcLsa;
    Psi3OptIeType          psi3Type;
    NcellParameters        psi3Ncell;     /*[MAX_PSI3_NCELL];*/
    GprsCellIdentification psi3CmpctCellId;
    CompactNcellInfo       psi3CompactNcellInfo;
    Int32                  psi3Ccn;
}
L3PktSysInfo3;

typedef struct L3PktSysInfo3bisTag
{
    PacketSysInfoType3bis psi3bisHeader;
    NcellLsaInfo          psi3bisNcLsa;
    Psi3bisOptIeType      psi3bisType;
    NcellParameters       psi3bisNcell;     /*[MAX_PSI3BIS_NCELL];*/
    CompactNcellInfo      psi3bisCompactNcellInfo;
    Int32                 psi3bisCcn;
}
L3PktSysInfo3bis;

typedef struct L3PktSysInfo4Tag
{
    PacketSysInfoType4 psi4Header;
    IntChannelGroup    chanGroup;       /*[MAX_PSI4_CHANNELS];*/
}
L3PktSysInfo4;

typedef struct L3PktSysInfo5Tag
{
    PacketSysInfoType5     psi5Header;
    PacketSysInfoType5Body psi5Body;
}
L3PktSysInfo5;

typedef struct L3PktSysInfo13Tag
{
    PacketSysInfoType13     psi13Header;
    PacketSysInfoType13Body psi13Body;
}
L3PktSysInfo13;

typedef struct L3PktSysInfo14Tag
{
    PageMode            pageMode;
    PacketSysInfoType14 psi14Body;
}
L3PktSysInfo14;

#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
typedef struct L3PktSysInfo15Tag
{
    PageMode            pageMode;
    PacketSysInfoType15 psi15Body;
}
L3PktSysInfo15;
#endif

typedef struct L3PktSysInfo8Tag
{
    PacketSysInfoType8     psi8Header;
    PacketSysInfoType8Body psi8Body;
}
L3PktSysInfo8;

typedef struct L3PktCellChangeContinueTag
{
    PageMode pageMode;
    Address  address;
    Arfcn    arfcn;
    Bsic     bsic;
    Int8     containerId;
}
L3PktCellChangeContinue;

typedef struct L3PktNcellDataTag
{
    PageMode              pageMode;
    Address               address;
    PktNcellDataContainer container;
}
L3PktNcellData;

typedef struct L3PktScellDataTag
{
    PageMode              pageMode;
    Address               address;
    PktScellDataContainer container;
}
L3PktScellData;

typedef struct L3PktSysInfo3quaTag
{
    PacketSysInfoType3quaHdr  psi3quaHeader;
    PacketSysInfoType3quaBody psi3quaBody;
}
L3PktSysInfo3qua;

typedef struct L3PktSysInfo3terTag
{
    PacketSysInfoType3terHdr  psi3terHeader;
    PacketSysInfoType3terBody psi3terBody;
}
L3PktSysInfo3ter;

/** Layer 3 packet downlink messages */
typedef union L3PktDnlinkMsgTag
{
    /** Packet Access Reject */
    L3PktAccessReject           pktAccessReject;

    /** Packet Cell Change Order */
    L3PktCellChangeOrder        pktCellChangeOrder;

    /** Packet downlink assignment */
    L3PktDownlinkAssignment     pktDownlinkAssignment;

    /** Packet Measurement Order */
    L3PktPacketMeasurementOrder pktPacketMeasurementOrder;

    /** Packet Paging Request */
    L3PktPagingRequest          pktPagingRequest;

    /** Packet PDCH Release */
    L3PktPdchRelease            pktPdchRelease;

    /** Packet Polling Request */
    L3PktPollingRequest         pktPollingRequest;

    /** Packet Power Control/Timing Advance */
    L3PktPwrCtrlTimingAdv       pktPwrCtrlTimingAdv;

    /** Packet PRACH Parameters */
    L3PktPrachParameters        pktPrachParameters;

    /** Packet Queueing Notification */
    L3PktQueuingNotification    pktQueuingNotification;

    /** Packet Timeslot Reconfigure */
    L3PktTimeslotReconfig       pktTimeslotReconfig;

    /** Packet TBF Release */
    L3PktTbfRelease             pktTbfRelease;

    /** Packet Uplink Ack/Nack */
    L3PktUplinkAckNack          pktUplinkAckNack;

    /** Packet Assignment */
    L3PktUplinkAssignment       pktUplinkAssignment;

    /** Packet Cell Change Continue */
    L3PktCellChangeContinue     pktCellChangeContinue;

    /** Packet Neighbour Cell Data */
    L3PktNcellData              pktNcellData;

    /** Packet Serving Cell Data */
    L3PktScellData              pktScellData;

    /** Packet DBPSCH Assignment */
    Int8                        pktDbpschAssignment;

    /** Multiple TBF Downlink Assignment */
    Int8                        multipleTbfDnlinkAssignment;

    /** Multiple TBF Uplink Assignment */
    Int8                        multipleTbfUplinkAssignment;

    /** Multiple Timeslot Reconfigure */
    Int8                        multipleTimeslotReconfigure;

    /** MBMS MS_ID Assignment */
    Int8                        mbmsMsIDAssignement;

    /** MBMS Assignment (Non-distuibution)*/
    Int8                        mbmsAssignmentNonDistribution;

    /** PS Handover */
    Int8                        pSHandoverCommand;

    /** Packet Physical Information */
    Int8                        pktPhysicalInformation;

    /** DTM Handover Command */
    Int8                        dTmHandoverCommand;

    /** Packet Downlink Dummy Control Block */
    L3PktDnlinkDummyCtrlBlock   pktDnlinkDummyCtrlBlock;

    /** PSI1 */
    L3PktSysInfo1               pktSysInfo1;

    /** PSI2 */
    L3PktSysInfo2               pktSysInfo2;

    /** PSI3 */
    L3PktSysInfo3               pktSysInfo3;

    /** PSI3bis */
    L3PktSysInfo3bis            pktSysInfo3bis;

    /** PSI4 */
    L3PktSysInfo4               pktSysInfo4;

    /** PSI5 */
    L3PktSysInfo5               pktSysInfo5;

    /** PSI6 */
    Int8                        pktSysInfo6;

    /** PSI7 */
    Int8                        pktSysInfo7;

    /** PSI8 */
    L3PktSysInfo8               pktSysInfo8;

    /** PSI13 */
    L3PktSysInfo13              pktSysInfo13;

    /** PSI14 */
    L3PktSysInfo14              pktSysInfo14;

    /** PSI3ter */
    L3PktSysInfo3ter            pktSysInfo3ter;

    /** PSI3qua */
    L3PktSysInfo3qua            pktSysInfo3qua;

    /** PSI15 */
#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
    L3PktSysInfo15              pktSysInfo15;
#else
    Int8                        pktSysInfo15;
#endif
    /** PSI16 */
    Int8                        pktSysInfo16;

    /** Packet Serving Cell SI */
    Int8                        pktScellSI;

    /** Packet CS Command */
    Int8                        pktCSCommand;

    /** Packet CS Release Indication */
    Int8                        pktCSReleaseIndication;

    /** MBMS Assignment Distribution */
    Int8                        mbmsAssignmentDistribution;

    /** MBMS Neighbouring Cell Informatino */
    Int8                        mbmsNcellSI;

    /** Packet MBMS Announcement */
    Int8                        pktMbmsAnnouncement;

    /** Packet Application Information */
    Int8                        pktApplicationInformation;

    /** Unknown Message */
    Int8                        unknownMessage;
}
L3PktDnlinkMsg;

typedef struct GrrPbcchNcellReselParamTag
{
    Int16              presenceBits;
    Int8               bsic;
    Int8               gprsRxLevAccessMin;
    Int8               gprsMsTxPwrMax;
    Int8               gprsTemporaryOffset;
    Int8               gprsPenaltyTime;
    Int8               gprsReselectOffset;
    Int8               priorityClass;
    Int8               hcsThreshold;
    Int8               bitField;           /* cellBarAccess2, exclusiveAccess, sameRaAsScell and CCN */
/** \assoc UNION \ref ncellPbcch */
    NcellPbcchLocation pbcchLocation;
    union
    {
        Int8                pbcchNotPresent;
        Boolean             si13LocationIsExt;
        GrrNcellPsiLocation psi1Location;
    }
    ncellPbcch;
}
GrrPbcchNcellReselParam;

typedef struct GrrNcellReselStoreInfoTag
{
    Int16                   numEntries;
    Arfcn                   arfcn[MAX_BA_CHANNELS];
    Int16                   status[MAX_BA_CHANNELS];
    GrrPbcchNcellReselParam param[MAX_BA_CHANNELS];
}
GrrNcellReselStoreInfo;

typedef struct GrrFreqParamStoreInfoTag
{
    Arfcn                   maFifteenArfcn;
    Arfcn                   intFrequency;
    Int8                    numStoredRfl;
    Int8                    numStoredCellAlloc;
    Int8                    numStoredMa;
    GrrGprsMobileAllocation storedMa[9];
    RefFrequencyList        storedRfl[4];
    Int8                    cellAllocation[4];
    GrrGprsMobileAllocation directEncodedMa;
#if defined (GRR_TST_DUMP_INT_MEAS_LIST)
    /* Storage of the channels required for interference measurements. There are two formats, ARFCN and MA. */
    GrrIntMeasList          intMeasList;
#endif
}
GrrFreqParamStoreInfo;

typedef struct GrrRflStoreInfoTag
{
    Int8                 numStoredRfl;
    Int8                 numStoredCellAlloc;
    RefFrequencyList     storedRfl[4];
    Int8                 cellAllocation[4];
    FrequencyListElement freqList;
}
GrrRflStoreInfo;

#if defined (UPGRADE_3G_FDD)
typedef struct GrrFddNcellElementInfoTag
{
    Int16 uArfcn;
    Int16 numScramblingCodes;
    Int8  rptIndex[32];
    Int16 scramblingCode[32];
}
GrrFddNcellElementInfo;

typedef struct FddNcellDesriptionInfoTag
{
    Int16                  numEntries;
    Int8                   absStartEmr;
    Int8                   sequenceId;
    GrrFddNcellElementInfo ncellElement[3];
}
FddNcellDescriptionInfo;
#endif /* UPGRADE_3G_FDD */

#if defined (UPGRADE_3G_TDD128)
typedef struct GrrTdd128NcellElementInfoTag
{
    Int16 uArfcn;
    Int16 numCellParametersID;
    Int8  rptIndex[32];
    Int16 cellParametersID[32];      /* Contains the sctdIndicator and tstdIndicator information */
}
GrrTdd128NcellElementInfo;

typedef struct Tdd128NcellDesriptionInfoTag
{
    Int16                     numEntries;
    Int8                      absStartEmr;
    Int8                      sequenceId;
    GrrTdd128NcellElementInfo ncellElement[3];
}
Tdd128NcellDescriptionInfo;
#endif /* UPGRADE_3G_TDD128 */

typedef struct BsicListElementInfoTag
{
    Int8 bsic[3];
}
BsicListElementInfo;

typedef struct BsicDescriptionInfoTag
{
    Boolean             valid;
    BsicListElementInfo bsicListInfo[32];
}
BsicDescriptionInfo;

#endif /* GRR_TYP_H */
