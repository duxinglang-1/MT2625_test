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
 * AS and NAS Common Data Types
 *************************************************************************/

#ifndef AS_NAS_COMMON_TYP_H
#define AS_NAS_COMMON_TYP_H

/***************************************************************************
* Nested Include Files
***************************************************************************/

#include "eps_typ.h"

/* TODO: Remove these when structures in this file have been cleaned. */
#include "ut_mcc_mnc.h"
#include "grrmrtyp.h"

/***************************************************************************
 * Constants
 ***************************************************************************/

/** Maximum number of PLMNs in PlmnList */
#define MAX_PLMN_LIST_NUM              NUM_PLMNS /* Replace with 20 after removing grrmrtyp.h */

/** Maximum number of EHPLMNs */
#define MAX_EHPLMN_NUM                 MAX_EHPLMNS /* Replace with 16 after removing sir_typ.h */

/** Maximum number of EPLMNs */
#define MAX_EQUIVALENT_PLMN_NUM        MAX_EQUIVALENT_PLMNS /* Replace with 16 after removing sir_typ.h */

/** Maximum number of forbidden TA's for roaming ,
  *  should be 40 from the spec, but it was reduced to 20 to save retention data, and for NBIOT, it should not need that large value*/
#define MAX_FORBIDDEN_TA_NUM           20

/** Fixed offset from L4 buffer allocation address to actual SDU.
 *  Used on downlink user data path by EL2, ESM and DBM */
#define DBM_DL_DATA_ALLOCATION_OFFSET  48

/** Fixed tail size to be reserved from L4 buffer after each SDU.
 *  Used on downlink user data path by EL2 and ESM */
#define DBM_DL_DATA_TAIL_SIZE          8

/** Total extra allocation size for each downlink data packet */
#define DBM_DL_DATA_EXTRA_ALLOC_SIZE (DBM_DL_DATA_ALLOCATION_OFFSET + DBM_DL_DATA_TAIL_SIZE)

/* Radio info of monitored neighbour cells maximum number */
#define NCELL_RADIO_INFO_MAX_NUM       4

/* Cell info of neighbour cells maximum number */
#define NCELL_INFO_MAX_NUM             3

/***************************************************************************
 * Macro Functions
 ***************************************************************************/


/***************************************************************************
 * Types
 ***************************************************************************/

/** Defines the search type. */
typedef enum EutraSearchTypeTag
{
    /** Shallow search type */
    EUTRA_SEARCH_TYPE_SHALLOW,

    /** Deep search type */
    EUTRA_SEARCH_TYPE_DEEP
} EutraSearchType;

/** camping status
 */
typedef enum CampingStatusTag
{
    /** Camped on suitable cell */
    CAMPING_STATUS_CAMPED_ON_SUITABLE_CELL,
    /** Cell reselection or SI Refresh is ongoing */
    CAMPING_STATUS_TEMP_NOT_CAMPED,
    /** Not camped */
    CAMPING_STATUS_NOT_CAMPED
} CampingStatus;

/** PLMN search result
 */
typedef enum PlmnSearchResultTag
{
    /** PLMN search was completed */
    PLMN_SEARCH_RESULT_COMPLETED,
    /** PLMN search was aborted */
    PLMN_SEARCH_RESULT_ABORTED
} PlmnSearchResult;

/* NB IoT band numbers */
typedef enum NbiotBandTag
{
    NBIOT_BAND_INVALID  = 0,
    NBIOT_BAND_1        = 1,
    NBIOT_BAND_2        = 2,
    NBIOT_BAND_3        = 3,
    NBIOT_BAND_4        = 4,
    NBIOT_BAND_5        = 5,
    NBIOT_BAND_8        = 8,
    NBIOT_BAND_11       = 11,
    NBIOT_BAND_12       = 12,
    NBIOT_BAND_13       = 13,
    NBIOT_BAND_14       = 14,
    NBIOT_BAND_17       = 17,
    NBIOT_BAND_18       = 18,
    NBIOT_BAND_19       = 19,
    NBIOT_BAND_20       = 20,
    NBIOT_BAND_21       = 21,
    NBIOT_BAND_25       = 25,
    NBIOT_BAND_26       = 26,
    NBIOT_BAND_28       = 28,
    NBIOT_BAND_31       = 31,
    NBIOT_BAND_66       = 66,
    NBIOT_BAND_70       = 70,
    NBIOT_BAND_71       = 71,
    NBIOT_BAND_72       = 72,
    NBIOT_BAND_73       = 73,
    NBIOT_BAND_74       = 74,
    NBIOT_BAND_85       = 85
}
NbiotBand;

typedef struct PlmnListElemTag
{
    /** PLMN identity */
    Plmn                            plmn;
    /** Frequency band */
    EutraBand                       band;
    /** Is PLMN high quality */
    Boolean                         is_high_quality;
} PlmnListElem;

/** This structure contains a list of PLMNs.
 */
typedef struct PlmnListTag
{
    /** Defines the number of PLMNs included in the list.
\assoc ARRAY \ref plmn_list */
    Int16                           numPlmn;
    /** Identifies PLMNs in the list. */
    PlmnListElem                    plmn_list[MAX_PLMN_LIST_NUM];
} PlmnList;

typedef enum EarfcnOffsetTag
{
    EARFCN_OFFSET_MINUS_2,
    EARFCN_OFFSET_MINUS_1,
    EARFCN_OFFSET_MINUS_0_DOT_5,
    EARFCN_OFFSET_0,
    EARFCN_OFFSET_1,
} EarfcnOffset;

typedef enum RequestedModemInfoTag
{
    REQUESTED_MODEM_INFO_RADIO_INFO,
    REQUESTED_MODEM_INFO_DATA_TRANSFER_INFO,
} RequestedModemInfo;

typedef struct ServingCellMeasResultsTag
{
    int16_t                         rsrp;
    int8_t                          rsrq;
    int16_t                         rssi;
    int8_t                          snr;
} ServingCellMeasResults;

typedef struct ServingCellRadioInfoTag
{
    EutraArfcn                      earfcn;
    EarfcnOffset                    earfcnOffset;
    EutraBand                       band;
    EutraPhysCellId                 pci;
    EutraCellIdentity               cellIdentity;
    EutraTrackingAreaCode           tac;
    bool                            servingCellMeasResultsPresent;
    ServingCellMeasResults          servingCellMeasResults;
    bool                            eclPresent;
    uint8_t                         ecl;
    bool                            txPowerPresent;
    int16_t                         txPower;
} ServingCellRadioInfo;

typedef struct NeighbourCellRadioInfoTag
{
    EutraArfcn                      earfcn;
    EarfcnOffset                    earfcnOffset;
    EutraPhysCellId                 pci;
    int16_t                         rsrp;
} NeighbourCellRadioInfo;

typedef struct LocationCellInfoTag
{
    EutraArfcn                      earfcn;
    EarfcnOffset                    earfcnOffset;
    EutraPhysCellId                 pci;
    int16_t                         rsrp;
    int8_t                          rsrq;
    int8_t                          rssi;
    int8_t                          snr;
    Plmn                            plmn;
    EutraTrackingAreaCode           tac;
    EutraCellIdentity               cellIdentity;
} LocationCellInfo;

typedef struct DataTransferInfoTag
{
    uint8_t                         rlcUlBler;
    uint8_t                         rlcDlBler;
    uint8_t                         macUlBler;
    uint8_t                         macDlBler;
    uint32_t                        rlcUlThroughput;
    uint32_t                        rlcDlThroughput;
    uint32_t                        macUlThroughput;
    uint32_t                        macDlThroughput;

    uint32_t                        macUlTotalBytes;
    uint32_t                        macUlTotalHarqTrans;
    uint32_t                        macUlHarqRetrans;

    uint32_t                        macDlTotalBytes;
    uint32_t                        macDlTotalHarqTrans;
    uint32_t                        macDlHarqRetrans;
} DataTransferInfo;

/** Defines the ERRC state info */
typedef enum ErrcStateEngInfoTag
{
    ERRC_STATE_NULL_ENG_INFO,
    ERRC_STATE_SEARCH_ENG_INFO,
    ERRC_STATE_IDLE_ENG_INFO,
    ERRC_STATE_DEACTIVATING_ENG_INFO,
    ERRC_STATE_CONNECTION_ENG_INFO,
    ERRC_STATE_CELL_RESEL_ENG_INFO,
    ERRC_STATE_L1_TEST_MODE_ENG_INFO,
    ERRC_STATE_REESTABLISHMENT_ENG_INFO,
    ERRC_STATE_PSM_ENG_INFO,
    ERRC_STATE_BG_SEARCH_ENG_INFO,
    ERRC_STATE_INVALID_REQ_ENG_INFO
} ErrcStateEngInfo;

typedef struct CellLockInfoTag
{
    EutraArfcn                      earfcn;
    EarfcnOffset                    earfcnOffset;
    /** \assoc PRESENT \ref pci */
    bool                            pciPresent;
    EutraPhysCellId                 pci;

    bool                            byPassSCriteria;
} CellLockInfo;

#endif /* AS_NAS_COMMON_TYP_H */

/* END OF FILE */
