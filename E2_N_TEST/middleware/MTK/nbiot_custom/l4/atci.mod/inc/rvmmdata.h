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
 * File Description
 * ----------------
 *
 * RVMMDATA.H
 * Common data header file, used only by vgdata.h
 **************************************************************************/

#ifndef RVMMDATA_H
#define RVMMDATA_H
  
/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>
#include <kitqid.h>
#include <rvsldata.h>
#include <abmm_sig.h>


/***************************************************************************
 * Manifest Constants
 **************************************************************************/
#define VG_CSIM_3G_CLASS              (0x00)
#define VG_CSIM_80_CLASS              (0x80)
#define VG_CSIM_SELECT_P2_NO_DATA     (0x0C)
#define VG_CSIM_SELECT_P2_FCP         (0x04)
#define VG_3G_SW1_WRONG_PARAM         (0x6a)
#define VG_3G_SW2_FILE_NOT_FOUND      (0x82)
#define VG_SW1_REFERENCE_ERROR        (0x94)
#define VG_SW2_FILE_ID_NOT_FOUND      (0x04)
#define VG_GSM_SEL_FILE_ID1             (05)
#define VG_GSM_SEL_FILE_ID2             (06)
#define VG_GSM_ACT_OFFSET               (07)
#define VG_GSM_COMPACT_ACT_OFFSET       (06)
#define VG_EUTRAN_ACT_OFFSET            (14)
#define VG_UTRAN_ACT_OFFSET             (15)

/** Number of minutes for a timezone unit*/
#define VG_TIMEZONE_NB_MINUTE_PER_UNIT  (15)
/** Number of minutes for a timezone unit*/
#define VG_TIMEZONE_HOUR_DIVISOR        (04) /* = 60 / VG_TIMEZONE_NB_MINUTE_PER_UNIT*/
/* Number of characters used by the string when DST is activated */
#define VG_DST_STRING_LENGTH            (15)

#define VG_CEDRX_ACT_NB_IOT            5
#define VG_CEDRX_MAX_BIN_STR_LEN       4
#define VG_CPSMS_MAX_STR_LEN           8
#define VG_CPSMS_MAX_TAU_UNIT_VAL      6  /* top three bits of periodic TAU string conversions - max is 110 */
#define VG_CPSMS_VALUE_MASK            0x1F

#define VG_CESQ_INVALID_RXLEV           99
#define VG_CESQ_INVALID_RSRQ           255
#define VG_CESQ_INVALID_RSRP           255

#define VG_MNBIOTEVENT_PSM_STATE    (0x00000001 << 0)

/***************************************************************************
 * Type Definitions
 **************************************************************************/

/* COPS definitions */

typedef enum VgOpFormatTag
{
  VG_OP_LONG = 0,
  VG_OP_SHORT,
  VG_OP_NUM,
  NUM_OF_VG_OPS
}
VgOpFormat;

typedef enum VgOpModeTag
{
  VG_OP_AUTOMATIC_MODE = 0,
  VG_OP_MANUAL_OPERATOR_SELECTION,
  VG_OP_MANUAL_DEREGISTER,
  VG_OP_SET_FORMAT,
  VG_OP_MANUAL_THEN_AUTOMATIC,
  NUM_OF_VG_OP_MODES
}
VgOpMode;

typedef enum VgCopsStateTag
{
  VG_COPS_MANUAL = 0,
  VG_COPS_LIST
}
VgCopsState;
typedef enum VgOpActTag
{
  VG_OP_ACT_DISABLED = 0,
  VG_OP_ACT_ENABLED,
  VG_OP_ACT_INVALID
}
VgOpAct;


typedef enum VgSystemModesTag
{
  SYSCONFIG_MODE_AUTO = 2,
  SYSCONFIG_MODE_NO_MODIFICATION = 16
}VgSystemModes;

typedef enum VgNetworkAccessOrderTag
{
  ACCESS_AUTO, 
  ACCESS_GSM_FIRST,
  ACCESS_3G_FIRST,
  ACCESS_NO_MODIFICATION
}VgNetworkAccessOrder;

typedef enum VgRoamingSupportTag
{
  ROAMING_NOT_SUPPORT,
  ROAMING_SUPPORT,
  ROAMING_NO_MODIFICATION  
}VgRoamingSupport;

typedef enum VgDomainSettingTag
{
  DOMAIN_CS_ONLY,   /* Not used for NB-IOT */
  DOMAIN_PS_ONLY,
  DOMAIN_CS_PS,   /* Not used for NB-IOT */
  DOMAIN_ANY,
  DOMAIN_NO_MODIFICATION
}VgDomainSetting;

typedef struct VgCOPSDataTag
{
  VgOpMode       mode;
  VgOpFormat     format;
  VgCopsState    state;
  PlmnId         selectedPlmn;
  Boolean        fplmnOnlyAvailable;
  Boolean        autoSelectInProgress;
  Boolean        returnToRplmn;
  Boolean        threeDigitMnc;
} VgCOPSData;


typedef enum VgCPOLStateTag
{
  CPOL_IDLE,
  CPOL_INIT,
  CPOL_QUERY,
  CPOL_READ,
  CPOL_WRITE
} VgCPOLState;


typedef struct VgCPOLDataTag
{
  VgOpFormat      readFormat;
  PlmnMenuAction  action;
  Int16           index;
  Boolean         plmnNumberPresent;
  Plmn            plmnNumber;
  Boolean         threeDigitMnc;
  Boolean         plmnNamePresent;
  PlmnName        plmnName;
  Boolean         firstPlmn;
} VgCPOLData;

typedef struct VgCOPNDataTag
{
  Plmn           currentPlmn;
  Plmn           initialPlmn;
  Int16          listed;
  EmPlmnTestType order;
}
VgCOPNData;

typedef struct VgCPLSDataTag
{
  AbmmPlmnSelector   plmnSelector;
} VgCPLSData;

typedef enum VgModeTag
{
  VG_MODE_DISABLED,
  VG_MODE_ENABLED,
  VG_MODE_NUMBER_OF_SETTINGS
} VgMode;

typedef enum VgCregTag
{
  VG_CREG_DISABLED,
  VG_CREG_ENABLED,
  VG_CREG_ENABLED_WITH_LOCATION_INFO,
  VG_CREG_NUMBER_OF_SETTINGS
} VgCreg;

typedef struct VgCREGDataTag
{
  Int8    state;
  Lac     lac;
  Int32   cellId;
} VgCREGData;

typedef enum VgCsconRptCfgTag
{
  VG_CSCON_DISABLED,
  VG_CSCON_ENABLED,
  VG_CSCON_QUERY,
  VG_CSCON_NUMBER_OF_SETTINGS
} VgCsconRptCfg;

/*signal connection status*/
typedef enum VgCsconSigConnectModeTag
{
    VG_CSCON_IDLE_MODE,     /*Idle, No signal connection*/
    VG_CSCON_CONN_MODE,     /*Connected, have signal connection*/
    VG_CSCON_NUMBER_OF_CONN_STATE
}VgCsconSigConnectMode;


typedef struct VgCsconDataTag
{
    VgCsconRptCfg          rptCfgOp;
    VgCsconSigConnectMode  SigConMode;

}VgCSCONData;

/* Following struct is used for *MFRCLLCK*/
#define VG_MFRCLLCK_LOCK_REMOVE_LOCK                (0)
#define VG_MFRCLLCK_LOCK_ACTIVATE_LOCK              (1)

#define VG_MFRCLLCK_EARFCN_REMOV_ANY_LOCK_INFO      (0)
#define VG_MFRCLLCK_EARFCN_MAX_NUMBER               (262143)

#define VG_MFRCLLCK_PCI_MAX_NUMBER                  (503)
#define MAX_NUM_OF_BAND_LIST (4)
typedef enum VgMfrcllckEarfcnOffsetTag
{
    VG_MFRCLLCK_EARFCN_OFFSET_MINUS_2,
    VG_MFRCLLCK_EARFCN_OFFSET_MINUS_1,
    VG_MFRCLLCK_EARFCN_OFFSET_MINUS_0_DOT_5,
    VG_MFRCLLCK_EARFCN_OFFSET_0,
    VG_MFRCLLCK_EARFCN_OFFSET_1,
    VG_MFRCLLCK_NUMBER_OF_EARFCN_OFFSET,
} VgMfrcllckEarfcnOffset;

typedef struct VgMfrcllckDataTag
{
    /* Lock status field indicates if frequency lock is set or released */
    Boolean                     lockStatus;
    /** Frequency lock info * */
    Int32                       earfcn;
    VgMfrcllckEarfcnOffset      earfcnOffset;
    Boolean                     isPciValid;
    Int16                       pci;
    MmLockArfcnCmdMode  lockArfcnCmdMode; 
    Boolean             byPassSCriteria;
}VgMFRCLLCKData;
typedef struct VgMbandslDataTag
{
    /** Band info * */
    Boolean                     enable;
    Int8                        numSearchBand;
    NbiotBand                   searchBandList[MAX_NUM_OF_BAND_LIST];
}VgMBANDSLData;

typedef struct VgMEHPLMNDataTag
{
    /* EHPLMN list */
    Int8        numEhplmn;
    Plmn        ehplmnList[MAX_NUM_USER_CFG_EHPLMN_LIST];
}VgMEHPLMNData;

typedef struct VgMOOSAINDDataTag
{
    uint8_t                oosaStatus;
}VgMOOSAINDData;

typedef enum VgMengInfoQuerModeTag
{
  VG_MENGINFO_QUERY_MODEM_INFO_RADIO_INFO,
  VG_MENGINFO_QUERY_MODEM_INFO_DATA_TRANSFER_INFO,
  VG_MENGINFO_NUMBER_OF_QUERING
} VgMengInfoQuerMode;


/*service cell info struct*/
typedef struct VgMengInfoSrvCellTag
{
    /* presend serving cellInfo */
    Boolean                 cellInfoValid;
    /** Serving Cell info */
    ServingCellRadioInfo    cellInfo;

}VgMENGINFOSCInfo;

#define VG_MENGINFO_NB_CELL_INFO_INVALID        (0)
#define VG_MENGINFO_NB_CELL_NUMBER              (4)
/*neighbour cell info struct*/
typedef struct VgMengInfoNbCellTag
{
    /*Presend neighbourCellRadioInfo */
    Int8                     neighbourCellinfoNumber;
    /* Neighbour Cell info */
    NeighbourCellRadioInfo  neighbourCellRadioInfoList[VG_MENGINFO_NB_CELL_NUMBER];
}VgMENGINFONCInfo;

/*data transfer info struct*/
typedef struct VgMengInfoDataTransferTag
{
    /**PRESENT dataTransferInfo */
    Boolean                 dataTransferInfoValid;
    /** Error rate and throughput parameters */
    DataTransferInfo        dataTransferInfo;

}VgMENGINFODTInfo;

/* engine info statistics struct*/
typedef struct VgMengInfoStatisDataTag
{
    VgMengInfoQuerMode      queryMode;
    VgMENGINFOSCInfo        curSrvCell;
    VgMENGINFONCInfo        neighbourCell;
    VgMENGINFODTInfo        dataTransfer;

}VgMENGINFOStatisData;

/* Network location state for AT+CREG */

typedef enum VgMnLocationTag
{
  VGMNL_NOT_REGISTRATED,
  VGMNL_REGISTRATED_HOME,
  VGMNL_SEARCHING,
  VGMNL_REGISTRATION_DENIED,
  VGMNL_UNKNOWN,
  VGMNL_REGISTRATED_ROAMING
  /* For LTE we may also have registration states indicating
   * whether we are registered for SMS only - or for
   * emergency bearer services
   */
  ,
  VGMNL_REG_HOME_SMS_ONLY,
  VGMNL_REG_ROAMING_SMS_ONLY,
  VGMNL_REG_EMERGENCY_BEARER_SERVICES
} VgMnLocation;

typedef enum VgMnbiotEventCfgTag
{
  VG_MNBIOTEVENT_DISABLED,
  VG_MNBIOTEVENT_ENABLED,
  VG_MNBIOTEVENT_NUMBER_OF_SETTINGS
} VgMnbiotEventCfg;

typedef enum VgMoosaIndCfgTag
{
  VG_MOOSAIND_DISABLED,
  VG_MOOSAIND_ENABLED,
  VG_MOOSAIND_NUMBER_OF_SETTINGS
} VgMoosaIndCfg;

typedef struct VgSYSCONFIGParamTag
{
  VgSystemModes         mode;
  VgRoamingSupport      roamSupport;
  VgDomainSetting       srvDomain;
}VgSYSCONFIGParam;

typedef struct VgSYSCONFIGDataTag
{
  Boolean               read; 
  VgSYSCONFIGParam      newParam;
  VgSYSCONFIGParam      currentParam; 
} VgSYSCONFIGData; 

#if defined(UPGRADE_MTNET)

/* AT+RESET definitions */
typedef enum VgStateTag
{
  VG_STATE_WITH_DETACH = 0,
  VG_STATE_WITHOUT_DETACH,
  NUM_OF_VG_STATE
}
VgState;

#endif

typedef enum VgEdrxModeTag
{
  VG_EDRX_MODE_DISABLE_EDRX                  = 0,
  VG_EDRX_MODE_ENABLE_EDRX                   = 1,
  VG_EDRX_MODE_ENABLE_EDRX_ENABLE_UNSOL      = 2,  /* Cannot actually set this mode globally.
                                                    * Setting via AT will cause PROF_CEDRXS
                                                    * profile to be on or off - see below
                                                    */
  VG_EDRX_MODE_DISABLE_EDRX_AND_RESET   = 3,
  NUM_VG_EDRX_MODE
}
VgEdrxMode;

typedef enum VgEdrxCedrxsUnsolTag
{
  VG_CEDRXP_UNSOL_DISABLE = 0,
  VG_CEDRXP_UNSOL_ENABLE  = 1
}
VgEdrxCedrxsUnsol;

typedef struct VgEdrxDataTag
{
  Boolean        userDataValid;
  VgEdrxMode     mode;
  Boolean        userEdrxSupport;
  Int8           userEdrxValue;
  Boolean        userPtwPresence;
  Int8           userPagingTimeWindow;
  Boolean        nwDataValid;
  Boolean        nwEdrxSupport;
  Int8           nwEdrxValue;
  Int8           nwPagingTimeWindow;
}
VgEdrxData;

typedef struct VgReqEdrxDataTag
{
  VgEdrxMode     mode;
  Boolean        userEdrxSupport;
  Int8           userEdrxValue;
  Boolean         userPtwPresence;
  Int8              userPtwValue;
}
VgReqEdrxData;

typedef enum VgCiotOptNTag
{
  VG_CCIOTOPT_N_DISABLE_UNSOL                        = 0,
  VG_CCIOTOPT_N_ENABLE_UNSOL                         = 1,
  VG_CCIOTOPT_N_DISABLE_UNSOL_RESET_PARAMS_TO_DEF    = 2,
  NUM_VG_CCIOTOPT_N
}
VgCiotOptN;

typedef enum VgCpsmsOptTag
{
  VG_CPSMS_DISABLE  = 0,
  VG_CPSMS_ENABLE = 1,
  VG_CPSMS_DISABLE_OR_RESET_TO_DEFAULTS = 2,
  NUM_VG_CPSMS_OPTS
}
VgCpsmsOpt;

typedef struct VgCpsmsDataTag
{

  Boolean       dataValid;
  VgCpsmsOpt    mode;
  Int8          requestedTau;
  Int8          requestedActiveTime; 

}
VgCpsmsData;

typedef struct VgReqCpsmsDataTag
{
  VgCpsmsOpt    mode;
  Boolean       reqTauPresent;
  Int8          reqTau;
  Boolean       reqActiveTimePresent;
  Int8          reqActiveTime;  

}
VgReqCpsmsData;


typedef struct VgCciotoptDataTag
{

  VgCiotOptN                reportOpt;
  Boolean                   uEdataValid;
  SupportedIotOptions       supportedUEOpt;
  CiotPreference            prefUEOpt;
  Boolean                   nwDataValid;
  SupportedIotOptions       supportedNwOpt;
}
VgCciotoptData;

typedef struct VgReqCciotoptDataTag
{
  Boolean                   resetCiotOptParams;
  Boolean                   supportedUEOptPresent;
  SupportedIotOptions       supportedUEOpt;
  Boolean                   prefUEOptPresent;
  CiotPreference            prefUEOpt;
}
VgReqCciotoptData;

typedef enum VgCipcaOptTag
{
   VG_CIPCA_ATTACH_WITH_PDN      = 0,
   VG_CIPCA_ATTACH_WITHOUT_PDN   = 1,
   NUM_CIPCA_OPTIONS
}VgCipcaOpt;

typedef struct VgCipcaDataTag
{
  Boolean                dataValid;
  VgCipcaOpt             vgCipcaOpt;
}
VgCipcaData;
typedef enum VgMbandslOptTag
{
   VG_BAND_SEATCH_LIST_DISABLE  = 0,
   VG_BAND_SEATCH_LIST_ENABLE   = 1,
   NUM_MBANDSL_OPTIONS
}VgMbandSlOpt;
#if defined (FEA_RPM)
typedef struct VgMrpmDataTag
{
  Int8         read_area;
  Boolean      enable;
}
VgMrpmData;
#endif

#if defined (FEA_NFM)
#define VG_NFM_DISABLED             (0)
#define VG_NFM_ENABLED              (1)

#define VG_NFM_UNSOL_DISABLE        (0)
#define VG_NFM_UNSOL_ENABLE         (1)

typedef struct VgNfmDataTag
{
  Int16         remainingNfmStartTimerValue;
  Int16         remainingNfmBackOffTimerValue;
  Boolean       dataValid;
  Boolean       nfmActive;
  Boolean       startTimerActive;
}
VgNfmData;

typedef struct VgReqNfmDataTag
{
  Boolean       reqNfmActive;
  Boolean       reqStartTimerActivePresent;
  Boolean       reqStartTimerActive;
  Boolean       reqStParPresent;
  Int16         reqStPar;
  Boolean       reqStTmPresent;
  uint8_t       reqStTm;
}
VgReqNfmData;

#define VG_NFMC_MAX_VALUE           (15360)

typedef struct VgNfmcDataTag
{
  Boolean       dataValid;
  Int16         nfmPar[MAX_NFM_PAR_VALUE];
  Int16         stPar;
}
VgNfmcData;

typedef struct VgReqNfmcDataTag
{
  Boolean       reqNfmParPresent[MAX_NFM_PAR_VALUE];
  Int16         reqNfmPar[MAX_NFM_PAR_VALUE];
  Boolean       reqStParPresent;
  Int16         reqStPar;
}
VgReqNfmcData;

#endif /* FEA_NFM */

typedef struct VgMHPLMNSDataTag
{
    Boolean                     enable;

}VgMHPLMNSData;


/***************************************************************************
 *  Macros
 **************************************************************************/

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

#endif  /* vgMmDATA_H */

/* END OF FILE */

