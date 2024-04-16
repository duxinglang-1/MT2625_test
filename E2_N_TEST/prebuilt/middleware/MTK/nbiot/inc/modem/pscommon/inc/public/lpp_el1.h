/******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2013
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*******************************************************************************/

/******************************************************************************
 *   LTE Positioning Protocol - Message Structure Definition
 *****************************************************************************/

#ifndef _LPP_EL1_H
#define _LPP_EL1_H

#include "nbiot_modem_common_config.h"

#ifdef FEA_LPP_SUPPORT
//#include "kal_public_api.h"
//#include "kal_public_defs.h"
//#include "kal_general_types.h"

/*Always trun on LPP CP options for L1 stand alone build*/
//#if defined(__LPP_CP_SUPPORT__) || defined(__LPP_UP_SUPPORT__) || defined(__L1EDPS_ENABLE__)

//#include "ps_public_struct.h"  /* for L4C SS struct */
//#include "as2l4c_struct.h"     /* for L4CPS NBR cell info */
//#include "mcd_l3_inc_struct.h" /* for PLMN id */
//#include "l3_inc_enums.h"      /* for LCS/EMM enum */

/*****************************************************************************
 * Macros Without Parameters
 *****************************************************************************/


/*****************************************************************************
 * Macros With Parameters
 *****************************************************************************/


/*****************************************************************************
 * Enums without tags
 *****************************************************************************/

#define LPP_OTDOA_MAX_NBR_CELL_FREQ_LAYER_NUM   3
#define LPP_OTDOA_MAX_NBR_CELL_LIST_NUM        24
#define LPP_OTDOA_MAX_NBR_CELL_LIST_NUM_TOTAL  3*24


#define LPP_ECID_MAX_MEASURED_CELL_INFO_NUM    32

#define LPP_MAX_BANDS                          64 // should align LPP_maxBands

/* LPP procedure enum mapped to bitmask (BIT position equals to enum value) */
typedef enum
{
    LPP_PROC_TYPE_NULL,
    LPP_PROC_TYPE_CAPABILITY_TRANSFER_INDICATION,
    LPP_PROC_TYPE_ASSIST_DATA_TRANSFER_DELIVERY,
    LPP_PROC_TYPE_LOC_INFO_TRANSFER_DELIVERY,
    LPP_PROC_TYPE_ERROR,
    LPP_PROC_TYPE_ABORT
} lpp_procedure_type_enum;


typedef enum
{
    LPP_LOC_OTDOA_RESULT_NULL,
    LPP_LOC_OTDOA_RESULT_NO_ERROR,
    LPP_LOC_OTDOA_RESULT_UNDEFINED,
    LPP_LOC_OTDOA_RESULT_ASSIST_DATA_MISSING,          /* TBD: used for timeout without assist data case */
    LPP_LOC_OTDOA_RESULT_UNABLE_MEASURE_REF_CELL,      /* LPP_OTDOA_TargetDeviceErrorCauses_cause_unableToMeasureReferenceCell */
    LPP_LOC_OTDOA_RESULT_UNABLE_MEASURE_ANY_NBR_CELL,  /* LPP_OTDOA_TargetDeviceErrorCauses_cause_unableToMeasureAnyNeighbourCell */
    LPP_LOC_OTDOA_RESULT_UNABLE_MEASURE_SOME_NBR_CELL  /* LPP_OTDOA_TargetDeviceErrorCauses_cause_attemptedButUnableToMeasureSomeNeighbourCells */
} lpp_loc_otdoa_result_enum;


typedef enum
{
    LPP_LOC_ECID_RESULT_NULL,
    LPP_LOC_ECID_RESULT_NO_ERROR,
    LPP_LOC_ECID_RESULT_UNDEFINED,
    LPP_LOC_ECID_RESULT_REQUESTED_MEASUREMENT_NOT_AVAILABLE,      /* LPP_ECID_TargetDeviceErrorCauses_cause_requestedMeasurementNotAvailable, e.g. RRC connection reestablishment */
    LPP_LOC_ECID_RESULT_NOT_ALL_REQUESTED_MEASUREMENTS_POSSIBLE   /* LPP_ECID_TargetDeviceErrorCauses_cause_notAllrequestedMeasurementsPossible */
} lpp_loc_ecid_result_enum;


typedef enum
{
    LPP_OTDOA_PRS_INFO_BANDWIDTH_N6,
    LPP_OTDOA_PRS_INFO_BANDWIDTH_N15,
    LPP_OTDOA_PRS_INFO_BANDWIDTH_N25,
    LPP_OTDOA_PRS_INFO_BANDWIDTH_N50,
    LPP_OTDOA_PRS_INFO_BANDWIDTH_N75,
    LPP_OTDOA_PRS_INFO_BANDWIDTH_N100
} lpp_otdoa_prs_info_bandwidth_enum;


typedef enum
{
    LPP_OTDOA_PRS_INFO_NUM_DL_FRAME_SF_1,
    LPP_OTDOA_PRS_INFO_NUM_DL_FRAME_SF_2,
    LPP_OTDOA_PRS_INFO_NUM_DL_FRAME_SF_4,
    LPP_OTDOA_PRS_INFO_NUM_DL_FRAME_SF_6
} lpp_otdoa_prs_info_num_dl_frame_enum;


typedef enum
{
    LPP_OTDOA_PRS_INFO_MUTING_PO_2,
    LPP_OTDOA_PRS_INFO_MUTING_PO_4,
    LPP_OTDOA_PRS_INFO_MUTING_PO_8,
    LPP_OTDOA_PRS_INFO_MUTING_PO_16,
} lpp_otdoa_prs_info_muting_po_enum;

typedef enum
{
    LPP_OTDOA_PRS_OCC_GROUP_LEN_G_2,
    LPP_OTDOA_PRS_OCC_GROUP_LEN_G_4,
    LPP_OTDOA_PRS_OCC_GROUP_LEN_G_8,
    LPP_OTDOA_PRS_OCC_GROUP_LEN_G_16,
    LPP_OTDOA_PRS_OCC_GROUP_LEN_G_32,
    LPP_OTDOA_PRS_OCC_GROUP_LEN_G_64,
    LPP_OTDOA_PRS_OCC_GROUP_LEN_G_128,
} lpp_otdoa_prs_occ_group_len_enum;

typedef enum
{
   LPP_OTDOA_ReferenceCellInfo_dlBandwidth_n6,
   LPP_OTDOA_ReferenceCellInfo_dlBandwidth_n15,
   LPP_OTDOA_ReferenceCellInfo_dlBandwidth_n25,
   LPP_OTDOA_ReferenceCellInfo_dlBandwidth_n50,
   LPP_OTDOA_ReferenceCellInfo_dlBandwidth_n75,
   LPP_OTDOA_ReferenceCellInfo_dlBandwidth_n100
} LPP_OTDOA_ReferenceCellInfo_dlBandwidth_enum;

/*****************************************************************************
 * Type Definitions
 *****************************************************************************/

/* LPP -> ERRC */
/* This structure for MSG_ID_LPP_EL1_OTDOA_MEAS_REQ */
typedef struct
{
    Int16  transactionID;
    Int8   responseTime;     /* [1..128], unit in seconds, for EL1 knowing about how long it will take for OTDOA measurement, but LPP still controls the response timer */
} LppEl1OtdoaMeasReq;


typedef struct
{
    Int8 numBand;
    Int8 Bands[3];
}PrsHoppingInfo;

typedef struct
{
   lpp_otdoa_prs_info_bandwidth_enum        prsBandwidth;
   Int16                                    prsConfigIndex;     /* [0..4095] */
   lpp_otdoa_prs_info_num_dl_frame_enum     numDLFrames;
   Boolean                                  prsMutingInfoValid;
   lpp_otdoa_prs_info_muting_po_enum        prsMutingInfoType;
   Int16                                    prsMutingInfoBits;  /* Position Occasion: PO-2: 2 bits, PO-4: 4 bits, PO-8: 8 bits, PO-16: 16 bits */
   Boolean                                  prsIdValid;
   Int16                                    prsId;
   Boolean                                  prsOccGroupLenValid;
   lpp_otdoa_prs_occ_group_len_enum         prsOccGroupLen;
   Boolean                                  prsHoppingInfoValid;
  PrsHoppingInfo                     prsHoppingInfo;

} lpp_otdoa_prs_info;

#define maxAddPRSconfigRef 2
#if 0
typedef struct
{
    Int16                                            phyCellId;                 /* [0..503] */
    Boolean                                          earfcnValid;               /* FALSE: indicate this is a intra-freq cell, EL1 should use current serving cell's earfcn; TRUE: indicate this is a inter-freq cell */
    Int32                                            earfcn;                    /* [0..262143], 65535 is invalid to indicate ARFCN-ValueEUTRA-v9a0 present; LPP assign reference cell if received message IE does not present */
    Boolean                                          isFourAntPortConfigValid;
    Boolean                                          isFourAntPortConfig;       /* TRUE: 4 antenna ports for CRS are used in the assistance data reference cell, FALSE: 1 (or 2) antenna port(s) */
    Boolean                                          isCPLengthExtended;        /* TRUE: cyclic prefix length of PRS or CRS is extended, FALSE: cyclic prefix length is normal */
    Boolean                                          prsInfoValid;
    lpp_otdoa_prs_info                               prsInfo;
    Boolean                                          tpIdValid;
    Int16                                            tpId;
    Boolean                                          cpLengthCRSExtended;
    Boolean                                          sameMBSFNconfigRef;
    Boolean                                          dlBandwidthValid;
    LPP_OTDOA_ReferenceCellInfo_dlBandwidth_enum     dlBandwidth;
    Boolean                                          addPRSconfigRefValid;
    Int8                                             numAddPRSconfigRef;
    lpp_otdoa_prs_info                               addPRSconfigRef[maxAddPRSconfigRef];
} LppOtdoaRefCellInfo;
#endif
typedef enum
{
 CARRIER_FREQ_OFFSET_VALUE_MINUS_10,
 CARRIER_FREQ_OFFSET_VALUE_MINUS_9,
 CARRIER_FREQ_OFFSET_VALUE_MINUS_8,
 CARRIER_FREQ_OFFSET_VALUE_MINUS_7,
 CARRIER_FREQ_OFFSET_VALUE_MINUS_6,
 CARRIER_FREQ_OFFSET_VALUE_MINUS_5,
 CARRIER_FREQ_OFFSET_VALUE_MINUS_4,
 CARRIER_FREQ_OFFSET_VALUE_MINUS_3,
 CARRIER_FREQ_OFFSET_VALUE_MINUS_2,
 CARRIER_FREQ_OFFSET_VALUE_MINUS_1,
 CARRIER_FREQ_OFFSET_VALUE_MINUS_0_dot_5,
 CARRIER_FREQ_OFFSET_VALUE_0,
 CARRIER_FREQ_OFFSET_VALUE_1,
 CARRIER_FREQ_OFFSET_VALUE_2,
 CARRIER_FREQ_OFFSET_VALUE_3,
 CARRIER_FREQ_OFFSET_VALUE_4,
 CARRIER_FREQ_OFFSET_VALUE_5,
 CARRIER_FREQ_OFFSET_VALUE_6,
 CARRIER_FREQ_OFFSET_VALUE_7,
 CARRIER_FREQ_OFFSET_VALUE_8,
 CARRIER_FREQ_OFFSET_VALUE_9,
}carrierFreqOffset_enum;
typedef struct
{
    Int32  earfcn;
    Boolean  carrierFreqOffsetValid;
    carrierFreqOffset_enum  carrierFreqOffset;
}CarrierFreqRef;

typedef struct
{
    Boolean                                          phyCellIdNBValid;
    Int16                                            phyCellIdNB;                 /* [0..503] */
    Boolean                                          cellGlobalIdNBValid;               /* FALSE: indicate this is a intra-freq cell, EL1 should use current serving cell's earfcn; TRUE: indicate this is a inter-freq cell */
    CarrierFreqRef                            carrierFreqRef;                    /* [0..262143], 65535 is invalid to indicate ARFCN-ValueEUTRA-v9a0 present; LPP assign reference cell if received message IE does not present */

    Boolean                                          isEutraNumCRSFourPortValid;
    Boolean                                          isEutraNumCRSFourPort;       /* TRUE: 4 antenna ports for CRS are used in the assistance data reference cell, FALSE: 1 (or 2) antenna port(s) */
    Boolean                                          isCPLengthExtended;        /* TRUE: cyclic prefix length of PRS or CRS is extended, FALSE: cyclic prefix length is normal */
    Boolean                                          prsInfoValid;
    lpp_otdoa_prs_info                               prsInfo;
    Boolean                                          tpIdValid;
    Int16                                            tpId;
    Boolean                                          cpLengthCRSExtended;
    Boolean                                          sameMBSFNconfigRef;
    Boolean                                          dlBandwidthValid;
    LPP_OTDOA_ReferenceCellInfo_dlBandwidth_enum     dlBandwidth;
    Boolean                                          addPRSconfigRefValid;
    Int8                                             numAddPRSconfigRef;
    lpp_otdoa_prs_info                               addPRSconfigRef[maxAddPRSconfigRef];
} LppOtdoaRefCellInfoNb;


typedef struct
{
    Int16                                            phyCellId;                 /* [0..503] */
    Boolean                                          earfcnValid;               /* FALSE: indicate this is a intra-freq cell, EL1 should use current serving cell's earfcn; TRUE: indicate this is a inter-freq cell */
    Int32                                            earfcn;                    /* [0..262143], 65535 is invalid to indicate ARFCN-ValueEUTRA-v9a0 present; LPP assign reference cell if received message IE does not present */
    Boolean                                          isCPLengthExtendedValid;   /* only present when cyclic prefix length is not the same as reference cell */
    Boolean                                          isCPLengthExtended;        /* TRUE: cyclic prefix length of PRS or CRS is extended, FALSE: cyclic prefix length is normal */
    Boolean                                          isFourAntPortConfigValid;  /* only present when antenna port config is not the same as reference cell */
    Boolean                                          isFourAntPortConfig;       /* TRUE: 4 antenna ports for CRS are used in the assistance data reference cell, FALSE: 1 (or 2) antenna port(s) */

    Boolean                                          prsInfoValid;              /* only present when prs info is not the same as reference cell */
    lpp_otdoa_prs_info                               prsInfo;

    Boolean                                          slotNumberOffsetValid;     /* only present when slot timing is not the same as reference cell */
    Int8                                             slotNumberOffset;          /* [0..19] */
    Boolean                                          prsSubFrameOffsetValid;    /* only present when earfcn is not the same as reference cell */
    Int16                                            prsSubFrameOffset;         /* [0..1279] */
    Int16                                            expectedRSTD;              /* [0..16383] */
    Int16                                            expectedRSTDUnc;           /* [0..1023] */
    Boolean                                          tpIdValid;
    Int16                                            tpId;
    Boolean                                          prsOnlyTp;
    Boolean                                          cpLengthCRSExtended;
    Boolean                                          sameMBSFNconfigRef;
    Boolean                                          dlBandwidthValid;
    LPP_OTDOA_ReferenceCellInfo_dlBandwidth_enum     dlBandwidth;
    Int8                                             numAddPRSconfigNeighbour;
    lpp_otdoa_prs_info                               addPRSconfigNeighbour[maxAddPRSconfigRef];
} LppOtdoaNbrCellInfo;



typedef struct
{
    Int8                               numCellInfo;
    LppOtdoaNbrCellInfo     cellInfo[LPP_OTDOA_MAX_NBR_CELL_LIST_NUM];

} LppOtdoaNbrCellInfoList;



typedef struct
{
    Int8                                    numFreqInfo;
    LppOtdoaNbrCellInfoList     freqInfo[LPP_OTDOA_MAX_NBR_CELL_FREQ_LAYER_NUM];

}  LppOtdoaNbrFreqInfoList;



/* This structure for MSG_ID_LPP_EL1_OTDOA_ASSIST_DATA_REQ */
typedef struct
{
    Int16                           transactionID;
    Boolean                             refCellInfoValid;
    //LppOtdoaRefCellInfo       refCellInfo;

    Boolean                             nbrCellInfoValid;
    LppOtdoaNbrFreqInfoList  nbrCellInfo;

    Boolean                             refCellInfoNBValid;
    LppOtdoaRefCellInfoNb    refCellInfoNB;

} LppEl1OtdoaAssistDataReq;



/* This structure for MSG_ID_LPP_EL1_OTDOA_MEAS_ABORT_REQ */
typedef struct
{
    Int16  transactionID;
} LppEl1OtdoaMeasAbortReq;


#if 0
/* This structure for MSG_ID_LPP_EL1_OTDOA_ASSIST_DATA_CLEAR_NTF  */
typedef struct
{
    /* empty */
} LppEl1OtdoaAssistDataClearNtf;
#endif

/* This structure for MSG_ID_LPP_ERRC_ECID_MEAS_ABORT_REQ */
/* the conditions LPP will send abort request to ERRC:
 * (1) network abort ECID session or LPP protocol error
 * (2) ECID response timer expiry
 */
typedef struct
{
    Int16  transactionID;
} LppErrcEcidMeasAbortReq;


/* This structure for MSG_ID_LPP_ERRC_SRB_STATUS_REQ */
typedef struct
{
    Boolean active; /*to request ERRC to send SRB2 status, TRUE: send, FALSE: stop*/
} LppErrcSrbStatusReq;



/* ERRC -> LPP */
/**
 *   error-Resolution 
 *   This field specifies the resolution R used in error-Value field. The encoding on two bits is as follows: 
 *        '00'            5 meters 
 *        '01'           10 meters 
 *        '10'           20 meters 
 *        '11'           30 meters. 
 *   
 *   error-Value 
 *   This field specifies the target device"s best estimate of the uncertainty of the OTDOA (or TOA) measurement. 
 *   The encoding on five bits is as follows: 
 *        '00000'     0    to   (R*1-1) meters 
 *        '00001'     R*1  to   (R*2-1) meters 
 *        '00010'     R*2  to   (R*3-1) meters 
 *        ¡K 
 *        '11111'     R*31  meters or more; 
 *   where R is the resolution defined by error-Resolution field.  
 *   e.g. , R=20 m corresponds to 0-19 m, 20-39 m,¡K,620+ m.  
 *
 *   error-NumSamples 
 *   If the error-Value field provides the sample uncertainty of the OTDOA (or TOA) measurement, this field specifies how 
 *   many measurements have been used by the target device to determine this (i.e., sample size). Following 3 bit 
 *   encoding is used: 
 *        "000"         Not the baseline metric 
 *        '001'         5-9 
 *        '010'         10-14 
 *        '011'         15-24 
 *        '100'         25-34 
 *        '101'         35-44 
 *        '110'         45-54 
 *        '111'         55 or more. 
 *   In case of the value "000", the error-Value field contains the target device"s best estimate of the uncertainty of the 
 *   OTDOA (or TOA) measurement not based on the baseline metric. E.g., other measurements such as signal-to-noise-
 *   ratio or signal strength can be utilized to estimate the error-Value. 
 *   If this field is absent, the value of this field is "000". 
 */
typedef struct
{
    Int8  errResolution;     /* 2 bits, refer description as above */
    Int8  errValue;          /* 5 bits, refer description as above */
    Boolean   errSamplesValid;
    Int8  errSamples;        /* 3 bits, refer description as above */
} LppOtdoaMeasuredQuality;



typedef struct
{
    Int16                         phyCellId;
    Int32                         earfcn;        /* [0..262143], 65535 is invalid to indicate ARFCN-ValueEUTRA-v9a0 present */
    Int16                         rstd;          /* [0..12711], mapping refer to TS 36.133 table 9.1.10.3-1 */
    LppOtdoaMeasuredQuality  rstdQuality;

} LppOtdoaMeasuredNbrCell;



typedef struct
{
    Int16                                  sysFrameNumber;      /* 10 bits */
    Int16                                  refPhyCellId;        /* [0..503] */
    Int32                                  refEarfcn;           /* [0..262143], 65535 is invalid to indicate ARFCN-ValueEUTRA-v9a0 present */
    Boolean                                refQualityValid;
    LppOtdoaMeasuredQuality      refQuality;

    Int8                                   numMeasuredNbrCell;
    LppOtdoaMeasuredNbrCell     measuredNbrCell[LPP_OTDOA_MAX_NBR_CELL_LIST_NUM_TOTAL];

}  LppOtdoaMeasuredInfo;



/* This structure for MSG_ID_LPP_EL1_OTDOA_MEAS_CNF */
typedef struct
{
    Int16                      transactionID;
    LppOtdoaMeasuredInfo  measurementResult;

} LppEl1OtdoaMeasCnf;



/* This structure for MSG_ID_LPP_EL1_OTDOA_ASSIST_DATA_CNF */
typedef struct
{
    Int16  transactionID;
} LppEl1OtdoaAssistDataCnf;



/* This structure for MSG_ID_LPP_EL1_OTDOA_ADDITIONAL_ASSIST_DATA_IND & MSG_ID_LPP_EL1_OTDOA_ADDITIONAL_ASSIST_DATA_REQ */
typedef struct
{
    Int16  transactionID;   /* assoicated measurement transaction ID */
    Int16  physCellId;      /* [0..503] */
} LppEl1OtdoaAdditionalAssistDataInd;


typedef struct
{
    Int16              phyCellId;            /* [0..503] */
    Boolean            cellGlobalIdValid;
    Int32              cellGlobalId;               /* eutra 28 bits */
    Int32              earfcn;               /* [0..262143], 65535 is invalid to indicate ARFCN-ValueEUTRA-v9a0 present */
    Boolean            sysFrameNumberValid;
    Int16              sysFrameNumber;       /* 10 bits */
    /*                 measured result */
    Boolean            nrsrpResultValid;
    Int8               nrsrpResult;           /* [0..97]  */
    Boolean            nrsrqResultValid;
    Int8               nrsrqResult;           /* [0..34] */
    Int8               carrierFreOffsetValid;
    Int16              carrierFreqOffset;
    Int8               hyperSFNValid;
    Int16              hyperSFN;
} LppEcidCellMeasuredInfo;



typedef struct
{
    /**
     * Please note that the interface here is more straight-forward than ECID-SignalMeasurementInformation IE of LPP Message:
     * when ERRC has measured primary cell, the result will place in primaryCellMeasuredResult;
     * (if serving (primary) cell is not present, the whole measured info should be ignored since result will be error)
     *
     * in addition to primary cell, if ERRC also measured neighbor cells, they will be put into the array of measuredCellResult[],
     * and the number of neighbor will specify in numMeasuredCellResult
     */
    LppEcidCellMeasuredInfo     primaryCellMeasuredResult;

    Int8                                   numMeasuredCellResult;
    LppEcidCellMeasuredInfo     measuredCellResult[LPP_ECID_MAX_MEASURED_CELL_INFO_NUM];

} LppEcidMeasuredInfo;



/* This structure for MSG_ID_LPP_ERRC_ECID_MEAS_CNF */
#if 0
typedef struct
{
    lpp_loc_ecid_result_enum          result;
    LppEcidMeasuredInfo     measurementResult;

} NphyLppEcidMeasCnf;
#endif


/* This structure for MSG_ID_LPP_ERRC_SRB_STATUS_IND */
typedef struct
{
    Boolean isConnected;  /*TRUE: SRB2 active, FALSE: not sent*/
} LppErrcSrbStatusInd;




//#endif  /* #if defined(__LPP_CP_SUPPORT__) || defined(__LPP_UP_SUPPORT__) || defined(__L1EDPS_ENABLE__) */
#endif
#endif /* _LPP_MSG_STRUCT_H */
