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
 * All the globals for the general sub-system.  Only used by vgdata.h
 **************************************************************************/

#ifndef RVGNDATA_H
#define RVGNDATA_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>
#include <rvsystem.h>
#include <rvprof.h>
#include <rvmxdata.h>
#include <abgl_typ.h>
#include <absi_typ.h>
#include <rvmsdata.h>

#if defined (MTK_NVDM_MODEM_ENABLE)
#include <nvdm_tool.h>
#include <nvdm_modem_minidump.h>
#endif

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

#define VG_MUPBSYNC_ICCID_LENGTH        10 /* Length in byte of the ICCID fixed part in the PBID*/
#define VG_MFTRCFG_MODE_READ            1
#define VG_MFTRCFG_MODE_WRITE           2
#define VG_MFTRCFG_MAX_BOOLEAN_VALUE    1

#define VG_MROUTEMMI_MODE_READ          1
#define VG_MROUTEMMI_MODE_WRITE         2
#define VG_MROUTEMMI_MAX_OPTION_VALUE   1

/* Max number of additionals groups with an extended CPBW/R request */
#define VG_MUPBCFG_MAX_AD_GROUPS        9
/* Max number of additionals dial string with an extended CPBW/R request */
#define VG_MUPBCFG_MAX_AD_NUMBER        2
/* Max number of alpha number string with an extended CPBW/R request */
#define VG_MUPBCFG_MAX_AD_AAS           (VG_MUPBCFG_MAX_AD_NUMBER + 1)
/* Max number of additionals email with an extended CPBW/R request */
#define VG_MUPBCFG_MAX_AD_EMAIL         3

/* Number of additional number to read in SCPBW command*/
#define VG_SCPBW_NUM_AD_NUMBER          3

/* Index for the ME additionals numbers*/
#define VG_ME_WORK_INDEX                0
#define VG_ME_FAX_INDEX                 1

#define VG_DEFAULT_SUBSCRIBER_PROFILE 1

/************************************************/
/* TODO: For NB-IOT - should be in include file */
/************************************************/
#define MAX_NVRAM_DATA_ITEM_SIZE_BYTES      (2048)
#define MAX_NVRAM_CERTIFICATE_SIZE_BYTES    (MAX_NVRAM_DATA_ITEM_SIZE_BYTES)
#define MAX_NVRAM_TOOL_VERSION_LEN          (16)
#define MAX_NVRAM_GROUP_ID_LEN              (16)
#define MAX_NVRAM_DATA_ITEM_ID_LEN          (32)
#define MAX_NVRAM_OTP_DATA_ITEM_ID_LEN      (8)
#define MAX_NVRAM_UID_STR_LEN          (16)

#define MAX_NVRAM_DATA_ITEM_STR_LEN         (MAX_NVRAM_DATA_ITEM_SIZE_BYTES * 2)
#define MAX_NVRAM_CERTIFICATE_STR_LEN       (MAX_NVRAM_CERTIFICATE_SIZE_BYTES * 2)

#define MAX_NVRAM_MINI_DUMP_INDEX           (255)   /* Temporary - may change */
#define MAX_NVRAM_MINI_DUMP_OFFSET          (65535) /* Temporary - may change */
#define MAX_NVRAM_MINI_DUMP_READ_LENGTH     (2048)

#define MCAL_ENDIAN_STR_LEN                 (1)
#define MAX_MCAL_DATA_SIZE_BYTES            (2048)
#define MAX_MCAL_DATA_STR_LEN               (MAX_MCAL_DATA_SIZE_BYTES * 2 + MCAL_ENDIAN_STR_LEN)

#define MCAL_BIG_ENDIAN_CHAR                ('B')
#define MCAL_LITTLE_ENDIAN_CHAR             ('L')

#define MCAL_MIN_TOKEN                      (0)
#define MCAL_MAX_TOKEN                      (65535)

#define MCAL_MIN_COMMAND                    (0)
#define MCAL_MAX_COMMAND                    (255)

#define MCAL_MIN_LENGTH                     (0)
#define MCAL_MIN_STR_LEN_WITH_ENDIAN        (3)

#define MCAL_NRF_STR                         ("NRF")
#define MCAL_MAX_NRF_STR_SIZE                (3)

#define IDC_RF_MAX_NUM_FREQ_RANGES           (3)
#define IDC_RF_MAX_FREQ                      (65535)
#define IDC_RF_FREQ_STOP_URC                 (0)
#define IDC_RF_MAX_ATTENUATION_POWER         (23)

#define IDC_RF_MAX_TX_PWR                    (23)
#define IDC_RF_MIN_PERIOD                    (1)
#define IDC_RF_MAX_PERIOD                    (600)

/***************************************************************************
 * Type Definitions
 **************************************************************************/

#if defined (FEA_PHONEBOOK)

/** Define the available values for the coding parameter
*   of the SCPBR/W command
*/
typedef enum VgScpbwCodingTag
{
    VG_SCPBW_CODING_GSM = 0,
    VG_SCPBW_CODING_RAW = 1,
    VG_SCPBW_CODING_END
}VgScpbwCoding;

typedef enum VgMupbcfgModeValueTag
{
    VG_MUPBCFG_EXT_PARAM_DISABLE    = 0,
    VG_MUPBCFG_EXT_PARAM_ENABLE     = 1,
    VG_MUPBCFG_EXT_PARAM_INVALID
}VgMupbcfgModeValue;

typedef enum VgMupbgasModeValueTag
{
    VG_MUPBGAS_MODE_READ        = 1,
    VG_MUPBGAS_MODE_WRITE       = 2,
    VG_MUPBGAS_MODE_DELETE_ALL  = 3,
    VG_MUPBGAS_MODE_INVALID,
    VG_MUPBGAS_MODE_RANGE
}VgMupbgasModeValue;

typedef enum VgMupbaasModeValueTag
{
    VG_MUPBAAS_MODE_READ        = 1,
    VG_MUPBAAS_MODE_WRITE       = 2,
    VG_MUPBAAS_MODE_DELETE_ALL  = 3,
    VG_MUPBAAS_MODE_INVALID,
    VG_MUPBAAS_MODE_RANGE,
    VG_MUPBAAS_MODE_LIST
}VgMupbaasModeValue;

/* List management definitions */

typedef enum VgPhoneBookOperationTag
{
  VG_PB_INVALID,
  VG_PB_RANGE,
  VG_PB_READ,
  VG_PB_WRITE,
  VG_PB_DELETE,
  VG_PB_FIND
}
VgPhoneBookOperation;

typedef enum VgAssocDataTag
{
  VG_PB_GROUP = 0,
  VG_PB_ANR,
  VG_PB_SNE,
  VG_PB_EMAIL,
  VG_PB_AD_GRP,
  VG_PB_AD_ANR,
  VG_PB_AD_EMAIL,
  VG_PB_AD_AAS,
  VG_PB_COMPLETE,
  VG_PB_NUM_ASSOC_DATA
}
VgAssocData;

typedef struct PhoneBookInfoTag
{
  Boolean           available;
  LmRecordNumber    records;
  LmRecordNumber    used;
  Int8              alpha;
  Int8              dial;
  Boolean           enabled;
  LmDialNumberFile  phoneBook;
/*this info below is only relevant to the global phonebook,
 * or local phonebook*/
  Int8              gLength;  /*group length*/
  Int8              eLength;  /*email length*/
  Int8              sLength;  /*second name length*/
} PhoneBookInfo;

/** Store context information for AT*MUPBSYNC command*/
typedef struct VgMUPBSYNCContextTag
{
    /** Current AT*MUPBSYNC execution operation.
     ** Only used on phonebook operation.*/
    ExtendedOperation_t operation;
    /** Selected phonebook record capacity.
    *   Used to know the maximum value for index*/
    LmRecordNumber      pbCapacity;
    /** The current read mode for UID*/
    LmReadMode          uidReadMode;
    /** Index used to know the record to read*/
    LmRecordNumber      uidIndex;
} VgMUPBSYNCContext;

/** Context data for CPBS command*/
typedef struct VgCpbsDataTag
{
    /** Current CPBS operation to execute.*/
    ExtendedOperation_t  operation;
    /** The phonebook choosen with the CPBS command
      * Only avalaible when operation is EXTENDED_ASSIGN*/
    LmDialNumberFile     phoneBook;
    /** The selected phonebook index in the VgLmInfo phonebook list
      * Only avalaible when operation is EXTENDED_ASSIGN*/
    Int8                 phoneBookIndex;
    /** Used for Icoming Call Info : specifies whether the user requested the missed calls or received calls
      * Only avalaible when operation is EXTENDED_ASSIGN and phonebook is ICI*/
    LmIciType            iciType;
    /** Whether the password is present
      * Only avalaible when operation is EXTENDED_ASSIGN*/
    Boolean              passwordPresent;
    /** The phonebook password to use
      * Only avalaible when operation is EXTENDED_ASSIGN*/
    Char                 password [MAX_PASSWORD_LENGTH + NULL_TERMINATOR_LENGTH];
    /** The password length
      * Only avalaible when operation is EXTENDED_ASSIGN*/
    Int16                passwordLength;
}VgCpbsData;

/** Hold group information for a record
*/
typedef struct VgLmDataGroupTag
{
    /** Temporary index used when sending group related signals*/
    SimUiccGrpData      grpData;
    /** input/output group text*/
    LmAlphaId           grpAlphaId;
    /** whether the currently selected phonebook supports grouping info*/
    Boolean             groupSupported;
    /** Maximal number of group per record*/
    Int8                groupPerRec;
    /** First free record in EF(GAS)*/
    Int8                gasFreeRec;
    /** Indicates the maximum number of groups that the UICC is able to store.*/
    LmRecordNumber      gasNumRecords;
    /** Length of the alpha for a GAS record*/
    Int8                gasSize;
    /** The additional gas records index from an extended CPBW request*/
    Int8                adGasIndex[ VG_MUPBCFG_MAX_AD_GROUPS];
    /** Number of additional GAS index*/
    Int8                nbAdGas;
    /** Whether the SIM manager have to delete the reference to a GAS record
    *   from the GRP records when we are doing a delete GAS request*/
    Boolean             deleteGrpRef;
}VgLmDataGroup;


/** Hold email information for a record
*/
typedef struct VgLmDataEmailTag
{
    /** whether the currently selected phonebook supports additional number*/
    Boolean             emailSupported;
    /** Maximal number of email per record*/
    Int8                emailPerRec;
    /** The main email for a record*/
    LmEmailAddress      email;
    /** Additionals emails for an extended CPBW/R request*/
    LmEmailAddress      adEmails[ VG_MUPBCFG_MAX_AD_EMAIL];
    /** Number of additional email*/
    Int8                nbAdEmails;
    /* The position for the next record to write/read*/
    Int8                emailIndex;
}VgLmDataEmail;

/** Hold a SIM dial string
*/
typedef struct VgSimDialNumTag
{
    /** Dial number*/
    Char                dialNum[MAX_CALLED_BCD_NO_LENGTH + NULL_TERMINATOR_LENGTH];
    /** Dial number's length*/
    Int16               dialNumLength;
    /** Dial number's type*/
    BcdNumberType       dialNumType;
}VgSimDialNum;


/** Hold additional numbers for a record
*/
typedef struct VgLmDataAdNumbersTag
{
    /** whether the currently selected phonebook supports additional number*/
    Boolean             anrSupported;
    /** Maximal number of additional numbers per record*/
    Int8                adNumPerRec;
    /** The main additional number for a record*/
    VgSimDialNum        adNum;
    /** Additionals add numbers for an extended CPBW/R request*/
    VgSimDialNum        adAdNums[ VG_MUPBCFG_MAX_AD_NUMBER];
    /** Number of additional ANR*/
    Int8                nbAdAnr;
    /* The position for the next record to write/read*/
    Int8                anrIndex;
}VgLmDataAdNumbers;

/** Hold additional number alpha information
*/
typedef struct VgLmDataAasTag
{
    /** whether the currently selected phonebook supports additional number*/
    Boolean             aasSupported;
    /** Number of AAS records*/
    LmRecordNumber      aasNumRecord;
    /** Maximum length of the alpha for a AAS record*/
    Int8                aasSize;
    /** Input/output AAS text*/
    LmAlphaId           aasAlphaId;
    /** Additionals dial number alpha for an extended CPBW/R request*/
    Int8                adAasIndex[ VG_MUPBCFG_MAX_AD_AAS];
    /** Number of additional AAS*/
    Int8                nbAdAas;
    /* Current AAS index to write for the next ANR*/
    Int8                aasIndex;
}VgLmDataAas;

/** Context for the MUPBGAS command
*/
typedef struct VgLmMupbgasContextTag
{
    /** The operation asked by the user*/
    VgMupbgasModeValue    mupbgasMode;
}VgLmMupbgasContext;

/** Context for the MUPBAAS command
*/
typedef struct VgLmMupbaasContextTag
{
    /** The operation asked by the user*/
    VgMupbaasModeValue    mupbaasMode;
}VgLmMupbaasContext;

/** Data used for USIM call info (OCI/ICI) access
*/
typedef struct VgLmUsimCallInfoDataTag
{
    /** True if the dateTime field is revelent*/
    Boolean                   dataTimePresent;
    /** Provides date and time information */
    LmDateTime                dateTime;
    /** Call duration*/
    Int32                     callDuration;
    /** Whether the call has been answered.*/
    Boolean                   callAnswered;
}VgLmUsimCallInfoData;

typedef struct VgLmDataTag
{
  VgPhoneBookOperation  phoneBookOperation;
  LmRecordNumber        phoneIndex1;
  LmRecordNumber        phoneIndex2;
  LmRecordNumber        NumRecords;
  LmRecordNumber        currentLnaRecord;
  BcdNumberType         phoneBookNumType;
  Boolean               phoneBookNumTypePresent;
  Char                  alpha    [SIM_ALPHA_ID_SIZE + NULL_TERMINATOR_LENGTH];
  Int8                  alphaLength;
  Char                  writeNum [MAX_CALLED_BCD_NO_LENGTH + NULL_TERMINATOR_LENGTH];
  Int16                 writeNumLength;
  Boolean               hiddenEntry;
  LmAlphaId             secondName;
  Int8                  startRecord;
  Int8                  missingParamCount;
  /** Store the group related data*/
  VgLmDataGroup         grpInfo;
  /** Store the email related data*/
  VgLmDataEmail         emailInfo;
  /** Store the additional number related data*/
  VgLmDataAdNumbers     adNumInfo;
  /** Store the AAS related data*/
  VgLmDataAas           aasInfo;
  /** Command result to return after reciving a delete confirmation.
  *   This is because a delete dialnum operation will be require to remove
  *   the main dialnum if an write operation failed on an associated
  *   record like group, email, ...*/
  ResultCode_t          deleteRes;
  LmReadMode            readMode;
  LmWriteMode           writeMode;
  LmDialNumberFile      phoneBook;
  VgAssocData           assocData;       /*type of association data we' re currently dealing with...*/
  Boolean               sneSupported;    /*whether the currently selected phonebook supports second name*/

  /** Whether the operation is a replace one*/
  Boolean               forReplace;

  /*Used for Icoming Call Info : specifies whether the user requested the missed calls or received calls*/
  LmIciType             iciType;
  LmDialNumberFile      oldPhoneBook;
  Int8                  phoneBookIndex;
  Boolean               needOldPhoneBook;
  Boolean               firstDialNumRecordFromList;
  Char                  fixedDialNum [MAX_CALLED_BCD_NO_LENGTH + NULL_TERMINATOR_LENGTH];
  Int16                 fixedDialNumLength;
  BcdNumberType         fixedDialNumType;
  Boolean               moreRecordsToRead;

  /** USIM call info data, only revelant when selected phonebook is ICI or OCI*/
  VgLmUsimCallInfoData  usimCallInfo;

  /** Context data for CPBS command*/
  VgCpbsData            vgCpbsData;
  /** Context data for MUPBSYNC command*/
  VgMUPBSYNCContext     vgMupbsyncContext;
  /** Context data for MUPBGAS command*/
  VgLmMupbgasContext    vgMupbgasContext;
  /** Context data for MUPBAAS command*/
  VgLmMupbaasContext    vgMupbaasContext;
}
VgLmData;

typedef struct VgAlphaLookupTag
{
  Boolean             active;
  DialledBcdNum       dialledBcdNum;
} VgAlphaLookup;

typedef enum VgMUPBSYNCDataTypeTag
{
    /* PSC/CC/PUID data type*/
    VG_MUPBSYNC_DATATYPE_PHONEBOOK    = 1,
    /* UID data type*/
    VG_MUPBSYNC_DATATYPE_RECORD       = 2
}VgMUPBSYNCDataType;

#endif /* FEA_PHONEBOOK */

/** Define the different variables configurable with the MFTRCFG command
 */
typedef enum VgMFtrCfgVariableTag
{
    /** Modem Mode                                      */
    VG_MFTRCFG_VAR_MODEM_MODE,
    /** ME memory location                              */
    VG_MFTRCFG_VAR_ME_LOCATION,
    /** Whether SM Handle Concat SMS                    */
    VG_MFTRCFG_VAR_SM_HANDLE_CONCAT_SMS,
    /** Chosen storage for ME phonebooks*/
    VG_MFTRCFG_VAR_LM_CALL_INFO_STORAGE,
    /** The total number of configurable variable       */
    VG_MFTRCFG_NB_VARIABLE
}VgMFtrCfgVariable;

typedef struct VgMFtrCfgRebootInfoTag
{
    /** Whether a reboot is needed to apply the modem mode modification                     */
    Boolean     modemModeNR;
    /** Whether a reboot is needed to apply the ME location modification                    */
    Boolean     meLocationNR;
    /** Whether a reboot is needed to apply the SM Handle Concat SMS modification           */
    Boolean     smHandleConcatSmsNR;
    /** Whether a reboot is needed to apply the call info storage configuration change      */
    Boolean     callInfoStorageModeNR;
}VgMFtrCfgRebootInfo;

typedef struct VgMFtrCfgDataTag
{
    /** Values for configuration variable currently stored in NVRAM*/
    AbglFeatureConfigDataArea   nvramCfgVar;
    /** Values for configuration variable in use in T1*/
    AbglFeatureConfigDataArea   currentCfgVar;
    /** Whether a reboot is needed to apply the configuration modifications*/
    VgMFtrCfgRebootInfo         rebootInfo;
    /** Whether nvramCfgVar has been initialised */
    Boolean                     initialised;
}VgMFtrCfgData;

#if defined (FEA_PHONEBOOK)
typedef struct VgMeSimDialNumTag
{
    /** Dial number*/
    Char                dialNum[MAX_CALLED_BCD_NO_LENGTH + NULL_TERMINATOR_LENGTH];
    /** Dial number's length*/
    Int16               dialNumLength;
    /** Dial number's type*/
    BcdNumberType       dialNumType;
}VgMeSimDialNum;

typedef struct VgMeSimAlphaTag
{
    /** Alpha*/
    Char                alpha[SIM_ALPHA_ID_SIZE + NULL_TERMINATOR_LENGTH];
    /** Alpha's length*/
    Int16               alphaLength;
}VgMeSimAlpha;

typedef struct VgMeSimEmailTag
{
    /** Email*/
    Char                email[SIM_UICC_EMAIL_ADDRESS_SIZE + NULL_TERMINATOR_LENGTH];
    /** Email's length*/
    Int16               emailLength;
}VgMeSimEmail;

/**
 ** The content for a ME phonebook record
 **/
typedef struct VgMePhonebookRecordTag
{
    /** Contact alpha string*/
    VgMeSimAlpha        alpha;
    /** Record's first dial number*/
    VgMeSimDialNum      dialNumMain;
    /** Record's second dial number*/
    VgMeSimDialNum      dialNumSecond;
    /** Record's groups name*/
    VgMeSimAlpha        grpAlpha;
    /** Record's second alpha*/
    VgMeSimAlpha        secondName;
    /** Record's email*/
    VgMeSimEmail        email;
    /** True if the record should be hidden*/
    Boolean             hiddenEntry;
    /* The group number*/
    Int8                groupNumber;

    /* Additional ME specifics entries*/

    /** Additional work dial number*/
    VgMeSimDialNum      dialNumWork;
    /** Additional fax dial number*/
    VgMeSimDialNum      dialNumFax;
    /** Additional email*/
    VgMeSimEmail        adEmail;
    /** Additional group*/
    VgMeSimAlpha        adGrpAlpha;

}VgMePhonebookRecord;
#endif /* FEA_PHONEBOOK */

/************************************/
/* TODO: Remove this later          */
/************************************/
#if 0
#if defined (UPGRADE_NVRAM_ENHANCED)
#define MAX_IMEI_LENGTH            15
#define MAX_IMEISVN_LENGTH         2
#endif

#if defined (UPGRADE_NVRAM_ENHANCED)
typedef enum VgGnMcalFacCalTableTypeTag
{
    FAC_DEFAULT_IMEI_TABLE = 0,
    FAC_IMEI_INDEX1_TABLE,
    FAC_IMEI_INDEX2_TABLE,
    FAC_IMEI_INDEX3_TABLE,
    FAC_IMEI_INDEX4_TABLE,
    FAC_IMEISVN_TABLE,
    FAC_TABLE_INVALID
} VgGnMcalFacCalTableType;

typedef enum VgGnMcalAccessImeiTypeTag
{
    VG_GN_MCAL_FAC_IMEI_READ,
    VG_GN_MCAL_FAC_IMEI_WRITE,
    VG_GN_MCAL_FAC_IMEISVN_READ,
    VG_GN_MCAL_FAC_IMEISVN_WRITE,
    VG_GN_MCAL_FAC_IMEI_ACCESS_INVALID
} VgGnMcalAccessImeiType;

typedef enum VgGnMcalFacImeiFunctionIndexTag
{
    VG_GN_MCAL_FAC_READ_IMEI = 0,
    VG_GN_MCAL_FAC_WRITE_IMEI,
    VG_GN_MCAL_FAC_READ_IMEISVN,
    VG_GN_MCAL_FAC_WRITE_IMEISVN,
    VG_GN_MCAL_FAC_ACCESS_INVALID
} VgGnMcalFacImeiFunctionIndex;
#endif
typedef enum VgGnMcalFacItemIndexTag
{
    VG_GN_MCAL_FAC_BACKUP = 10,
    VG_GN_MCAL_FAC_QUERY_DEFAULT_IMEI = 11,
    VG_GN_MCAL_FAC_ACCESS_IMEI = 16,
    VG_GN_MCAL_FAC_INVALID
} VgGnMcalFacItemIndex;
#endif /* 0 */

#if defined (FEA_PHONEBOOK)
/** Hidden Key Mode */
typedef enum VgHiddenKeyModeTag
{
  VG_VERIFY_HIDDEN_KEY,
  VG_CHANGE_HIDDEN_KEY,
  NUM_OF_VG_HIDDEN_KEY_MODE
} VgHiddenKeyMode;
#endif /* FEA_PHONEBOOK */


/** State of reading initialization data from ABPD */
typedef enum InitDataFromABPDStateTag
{
  WAITING_FOR_DEFAULT_APN = 0,
  READY
}InitDataFromABPDState;

/*******************************/
/* Context data for NB-IOT     */
/*******************************/

typedef enum VgCgsnSntTag
{
  VG_CGSN_SNT_SN = 0,
  VG_CGSN_SNT_IMEI = 1,
  VG_CGSN_SNT_IMEISV = 2,
  VG_CGSN_SNT_SVN = 3,
  VG_CGSN_INVALID_SNT
} VgCgsnSnt;

typedef enum VgMcgsnSntTag
{
  VG_MCGSN_SNT_SN = 0,
  VG_MCGSN_SNT_IMEI = 1,
  VG_MCGSN_SNT_IMEISV = 2,
  VG_MCGSN_SNT_SVN = 3,
  VG_MCGSN_INVALID_SNT
} VgMcgsnSnt;

typedef struct VgMcgsnDataTag
{
    VgMcgsnSnt     mcgsnSnt;         
    
    Int16          digitImeiArraySize;
    Int16          digitSNArraySize;
    
    Int8           digitMobileId[MAX_UE_ID_LENGTH];
}VgMcgsnData;

typedef struct  VgMnvmDataContextTag
{
  Char                     tool_version[MAX_NVRAM_TOOL_VERSION_LEN + NULL_TERMINATOR_LENGTH];
  Char                     group_id[MAX_NVRAM_GROUP_ID_LEN + NULL_TERMINATOR_LENGTH];
  Char                     data_item_id[MAX_NVRAM_DATA_ITEM_ID_LEN + NULL_TERMINATOR_LENGTH];
  /* Need to reserve length for \" as well as \0 */
  Char                     data_str[MAX_NVRAM_DATA_ITEM_STR_LEN + SINGLE_CHAR_LENGTH + NULL_TERMINATOR_LENGTH];
  Int8                     data[MAX_NVRAM_DATA_ITEM_SIZE_BYTES];  
  
} VgMnvmDataContext;

typedef enum VgNvmAreaInfoTag
{
  VG_NVM_NORMAL_AREA = 0,
  VG_NVM_PROTECTED_AREA = 1,
  VG_NVM_NORMAL_AREA_WITH_BACKUP = 2,
  VG_NVM_PROTECTED_AREA_WITH_BACKUP = 3,
  VG_NVM_NUM_AREAS
}VgNvmAreaInfo;

typedef enum VgNvmDataTypeTag
{
  VG_NVM_DATA_TYPE_RAW = 0,
  VG_NVM_DATA_TYPE_STRING = 1,
  VG_NVM_NUM_DATA_TYPES
}VgNvmDataType;


typedef enum VgNvmStatusTag
{
  VG_NVM_STATUS_OK = 0,
  VG_NVM_STATUS_NVRAM_CORRUPT = 1,
  VG_NVM_STATUS_OTHER_FAILURE = 2
}VgNvmStatus;

typedef enum VgCaldevStatusTag
{
  VG_CALDEV_DISABLED = 0,
  VG_CALDEV_ENABLED  = 1,
  VG_NUM_CALDEV_STATUS
}VgCaldevStatus;

/**
 ** Global context for MCAL variables
 **/
typedef struct VgMcalContextTag
{
    Int32                  token;
    Int32                  command;
    Int32                  length;
    /* Need to reserve length for \" as well as \0 */
    Char                   data_str[MAX_MCAL_DATA_STR_LEN + SINGLE_CHAR_LENGTH + NULL_TERMINATOR_LENGTH];
    Int8                   data[MAX_MCAL_DATA_SIZE_BYTES];

}VgMcalContext;

typedef union VgNvmMcalContextUnionTag
{
  VgMnvmDataContext  vgMnvmDataContext;
  VgMcalContext      vgMcalContext;
}
VgMnvmMcalContextUnion;

typedef struct VgNvmMcalContextTag
{
  VgCaldevStatus          vgCaldevStatus;
  Boolean                 vgMnvmAccessAuthorised;

  /* Data needed to transmit all NVRAM data items IDs to MUX.
   * This is in order to prevent too many signals building up on
   * ATCI or MUX queue
   */
#if defined (MTK_NVDM_MODEM_ENABLE)
  nvdm_modem_data_item_info_t  *info_list_p_normal;
  nvdm_modem_data_item_info_t  *info_list_p_protected;
#endif
  Int32                        nextDataItemNumberToSend;
  Int32                        normalDataItems;
  Int32                        protectedDataItems;
  Int32                        totalDataItemsToSend;
  VgmuxChannelNumber           currentNvmChannel;
  VgMnvmMcalContextUnion       vgMnvmMcalContextUnion;
}
VgMnvmMcalContext;

typedef struct VgNvmMspchscContextTag
{
    Boolean NpbchSymbolRotationModeInitDone; 

    /*current symbol rotation mode: 0 - New scrambling code, 1 - old scrambling code*/
    Boolean curNpbchSymbolRotationMode;  
    
}
VgNvmMspchscContext;

typedef struct VgIdcRfFreqRangeTag
{
  Int16   freq_start;
  Int16   freq_stop;
}
VgIdcRfFreqRange;

typedef VgIdcRfFreqRange VgIdcRfFreqRangeList[IDC_RF_MAX_NUM_FREQ_RANGES];

typedef enum VgIdcTx2GpsModeTag
{
  IDC_TX2GPS_MODE_STOP_TX         = 0,
  IDC_TX2GPS_MODE_SINGLE_TONE     = 1,
  IDC_TX2GPS_MODE_MULTI_TONE      = 2,
  IDC_TX2GPS_NUM_MODES
}VgIdcTx2GpsMode;


/***************************************************************************
 *  Macros
 **************************************************************************/

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

#endif  /* RVGNDATA_H */

/* END OF FILE */

