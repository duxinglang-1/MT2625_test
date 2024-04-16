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
 * ----------------                                                      */
/** \file
 *   Include file for Foreground Layer Shell
 **************************************************************************/

#ifndef ABLM_TYP_H
#define ABLM_TYP_H


/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#if !defined (ABCC_TYP_H)
#include <abcc_typ.h>
#endif

#include <alsi_typ.h>
#include <alsu_sig.h>

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/*0005-5897 Increase string length in line withh MAXMMIUSERSTRLEN*/
/* 21841 - increased to 80 from 40 */
# define MAX_SS_OR_DIAL_STRING_LENGTH 80

#define LM_MAX_DIAL_NUMBER_LENGTH     SIM_MAX_DIAL_NUMBER_LENGTH;
#define LM_HIDDEN_KEY_LENGTH          SIM_HIDDEN_KEY_LENGTH;
#define LM_PBID_LENGTH                SIM_UICC_PBID_LENGTH;
#define LM_EMAIL_ADDRESS_SIZE         SIM_UICC_EMAIL_ADDRESS_SIZE;

#define DIAL_NUM_DIGIT_KEY_LEN   (3)  /* NB this is limited to 4 bytes */
                                      /* (8 digits) - limitimg factor  */
                                      /* is a "long" variable          */
#define MAX_NUM_OF_FILES_TO_REFRESH   7
#define LM_GAS_LIST_SIZE              SIM_MAX_GAS_LIST
#define LM_AAS_LIST_SIZE              SIM_MAX_AAS_LIST

#define LM_FIRST_RECORD_NUMBER        (1)

/***************************************************************************
 * Typed Constants
 **************************************************************************/

/* const char alphabet[] = "ABCDEF...."; */

/***************************************************************************
 * Type Definitions
 **************************************************************************/

/*
  The Fixed dialling number matching data structure.
*/
typedef struct FdnNumberCheckTag
{
  const Bcd           *bcd_p;
  Int8                length;
  BcdNumberType       type;
}
FdnNumberCheck;

/** Dial number files are stored on the SIM and in NVRAM. */
typedef enum LmDialNumberFileTag
{
  /** 3G Global phonebook */
  DIAL_LIST_ADN_GLB = 0,
  /**  3G Local Application phonebook */
  DIAL_LIST_ADN_APP,
  /** Fixed dialling numbers */
  DIAL_LIST_FDN,
  /** Barred Dial Number file */
  DIAL_LIST_BDN,
  /** Service Dial Number file */
  DIAL_LIST_SDN,
  /** Mobile subscriber international subscriber identity numbers */
  DIAL_LIST_MSISDN,
  /** CPHS mailbox numbers */
  DIAL_LIST_CPHS_MN,
  /** Last numbers dialled */
  DIAL_LIST_LND,
  /** Last numbers received */
  DIAL_LIST_LNR,
  /** Last numbers missed */
  DIAL_LIST_LNM,
  /** OCI */
  DIAL_LIST_OCI,
  /** ICI */
  DIAL_LIST_ICI,
  /** Abbreviated dialling numbers in NVRAM */
  DIAL_LIST_ADR,
  /** Mailbox Dialled Numbers in USIM */
  DIAL_LIST_MBDN,
  /** Number of dial list files */
  NUMBER_OF_DIAL_LIST_FILES,
  /** NULL */
  DIAL_LIST_NULL
}
LmDialNumberFile;

/** ICI type element.
 * Specifies what type of calls to read which are strored in ICI file on 3G USIMs.
 */
typedef enum LmIciTypeTag
{
  /** All calls */
  LM_ICI_ALL = 0,
  /** Received calls */
  LM_ICI_RECEIVED,
  /** Missed calls */
  LM_ICI_MISSED
}
LmIciType;

/** Request status element used to determine the outcome of the request */
typedef enum LmRequestStatusTag
{
  LM_REQ_OK,                  /**< request successful */
  LM_REQ_OK_DATA_INVALID,     /**< request successful but data is invalid*/
  LM_REQ_PARAM_OUT_OF_RANGE,  /**< parameter out of range*/
  LM_REQ_ALPHA_OK,            /**< BL alpha lists are ready */
  LM_REQ_SIM_NOT_READY,       /**< Sim is not ready */
  LM_REQ_ACCESS_DENIED,       /**< Sim file access not permitted */
  LM_REQ_SIM_ERROR,           /**< Sim has reported error */
  LM_REQ_NRAM_ERROR,          /**< NRAM has reported error */
  LM_REQ_FILE_NOT_SUPPORTED,  /**< requested file is not supported by SIM or NVRAM */
  LM_REQ_RECORD_NOT_FOUND,    /**< requested record not found */
  LM_REQ_INCOMPLETE_WRITE,    /**< not all data could be written; no EXT or CCP records available */
  LM_REQ_EXT_INCOMPLETE_WRITE,    /**< not all data could be written; no EXT or CCP records available */  
  LM_REQ_CCP_INCOMPLETE_WRITE,    /**< not all data could be written; no EXT or CCP records available */    
  LM_REQ_CCP_EXT_INCOMPLETE_WRITE,    /**< not all data could be written; no EXT or CCP records available */    
  LM_REQ_ILLEGAL_OPERATION,   /**< the requested operation is not supported for a file or file type */
  LM_REQ_POWERED_DOWN,        /**< stack powered down so can't do operation */
  LM_REQ_FDN_READY,           /**< The DN list has been loaded and is ready for processing call requests */
  LM_REQ_BUSY,                /**< The stack is busy dealing with another request */
  LM_REQ_PROCESSING           /**< The request is currently processing (this is for internal use and isn't an error)*/
}
LmRequestStatus;

/** Read mode element used to read a dial number from a file. */
typedef enum LmReadModeTag
{
  /** The current record is read */
  LM_READ_ABSOLUTE,
  /** Read the first record */
  LM_READ_FIRST,
  /** Read the next entry */
  LM_READ_NEXT,
  /** Read the previous record */
  LM_READ_PREVIOUS,
  /** Read the last record*/
  LM_READ_LAST
}
LmReadMode;

/** Delete mode element used to delete a dial number from a file. */
typedef enum LmDeleteModeTag
{
  /** The current record is deleted */
  LM_DELETE_ABSOLUTE,
  /** Delete the first record */
  LM_DELETE_FIRST,
  /** Delete the next entry */
  LM_DELETE_NEXT,
  /** Delete the previous record */
  LM_DELETE_PREVIOUS,
  /** Delete the last record*/
  LM_DELETE_LAST
}
LmDeleteMode;

/** Write mode element used by ABLM to update an existing record or add a new record */
typedef enum LmWriteModeTag
{
  /** The current record is updated */
  LM_WRITE_ABSOLUTE,
  /** Write to first free record */
  LM_WRITE_FIRST_FREE,
  /** Write to next free record */
  LM_WRITE_NEXT_FREE
}
LmWriteMode;

/** Find mode element used in ABLM to search for a specified alpha id */
typedef enum LmFindModeTag
{
  LM_FIND_EXACT,              /**<  Find record that matches alphaId exactly */
  LM_FIND_PARTIAL,            /**<  Find record that matches alphaId for its length */
  LM_FIND_FIRST,              /**<  Find first record in list */
  LM_FIND_INDEX,              /**<  Find record by alpha index in list */
  LM_FIND_NEXT,               /**<  Find next record alphabetically */
  LM_FIND_PREVIOUS,           /**<  Find previous record alphabetically */
  LM_FIND_LAST,               /**<  Find last record in list */
  LM_FIND_FIRST_FREE,         /**<  Find first free record in file */
  LM_FIND_NEXT_FREE,          /**<  Find next free record after a given record number */
  LM_FIND_PREVIOUS_FREE,      /**<  Find previous free record before a given record number */
  LM_FIND_LAST_FREE,          /**<  Find last free record in file */
  LM_FIND_FILE_RECORD_NUMBER, /**<  Find using SIM / NVRAM record number */
  LM_FIND_INDEX_TYPE          /**<  Absolute index within type */
}
LmFindMode;

/*9812-3448*/
/** Used to spcify which file types returned dialnums  in the alpha list
 * are being referenced.
 */
typedef enum LmAlphaListEntryTypeTag
{
  /** Any type of dialnum */
  LM_ALPHA_LIST_ALL,        
  /** ADN global phonebook entry type only */
  LM_ALPHA_LIST_ADN_GLB,
  /** ADN application phonebook entry type only */
  LM_ALPHA_LIST_ADN_APP,
  /** ADR entry type only */
  LM_ALPHA_LIST_ADR,          
  /** FDN entry type only */
  LM_ALPHA_LIST_FDN,          
  /** MSISDN entry type only */
  LM_ALPHA_LIST_MSISDN,       
  /** ADR and ADN only       */
  LM_ALPHA_LIST_ADN_ADR,      
  /** CPHS mailbox number entry type only */
  LM_ALPHA_LIST_CPHS_MN,
  /**  SDN entry type only */
  LM_ALPHA_LIST_SDN,          
  /** ADN or ADR or SDN   */
  LM_ALPHA_LIST_ADN_ADR_SDN,  
  /** BDN entry type only */
  LM_ALPHA_LIST_BDN,
  /** Null entry */
  LM_ALPHA_LIST_NULL
}
LmAlphaListEntryType;

/* some of the structures are already defined in SIM related code */

/** Alpha identifier */
typedef SimAlphaIdentifier  LmAlphaId;
/** Contains email address data */
typedef SimUiccEmailData    LmEmailAddress;
/** Holds a list of groups data */
typedef SimUiccGrpData      LmGrpData;

typedef struct LmIciOciRecordTag
{
    /** whether this records is used*/
    Boolean             recordstored;  
    /** Dialled number */
    SimDialNumber       dialNum;
    /** Reports the timing information associated with the OCI or ICI record. */
    SimUiccTimingInfo   timingInfo;
    /** Reports whether the call was answered or not (Only valid for Incoming calls
    * and should be ignored in case of OCI record)
    */
    Boolean             callAnswered;
    /** Shows the link in the phonebook if any */
    SimUiccPbLink       pbLink;
}LmIciOciRecord;

typedef struct LmAdnRecordTag
{
    /** Cache the GRP data to be able to know which records refer to a specifics GAS record*/
    LmGrpData           grpData;
}LmAdnRecord;

typedef union LmRecordSpeDataTag
{
    /** Hold the ICI and OCI records complete data*/
    LmIciOciRecord     *iciOciData_p;
    /** Hold ADN specifics data*/
    LmAdnRecord        *adnData_p;
}LmRecordSpeData;

struct LmDialNumberFileRecordTag;

typedef struct LmDialNumberRecordTag
{
    /** The record number*/
    LmRecordNumber          recordNumber;
    /** Some other information will be added in the future, like alphaId etc, at the moment only record number is relevant */
    Boolean                 recordUsed;
    /** True if we need to get the full record to perform checksum.
     *  Only used and initilised for some selected 2G phonebooks.*/
    Boolean                 needCompletDesc;
    /** Special data for depending of the phonebook, already points on the record data*/
    LmRecordSpeData         recordData;
    /** Back pointer to the phonebook data*/
    struct LmDialNumberFileRecordTag  *pbInfo_p;
}
LmDialNumberRecord;

/** Store the synchronisation info before sent them to AB*/
typedef struct LmPbExtEmailnfoTag
{
    /** Indicates the used email record in the (u)sim */
    Int16                   usedEmailRecords;
    /** Indicates the total email record in the (u)sim */
    Int16                   maxEmailRecords;
     /** Indicates the used ext record in the (u)sim */
    Int16                   usedExtRecords;
    /** Indicates the total ext record in the (u)sim */
    Int16                   maxExtRecords;
  
} LmPbExtEmailnfo;

typedef enum
{
  DIAL_TYPE_LINEAR,
  DIAL_TYPE_CYCLIC,
  DIAL_TYPE_NULL
}
LmDialListType;

typedef enum LmDialNumOperationTag
{
    /** A record has been added*/
    LM_DIAL_NUM_OP_ADD,
    /** An existing record has been modified*/
    LM_DIAL_NUM_OP_MOD,
    /** A record has been deleted*/
    LM_DIAL_NUM_OP_DEL,
}LmDialNumOperation;

typedef struct LmDialNumberFileRecordTag
{
    /** File associated to this structure*/
    LmDialNumberFile        dialNumberFile;
    /** Some informations about each records. 
    *   WARNING: THE INDEX START AT 1, THE RECORD 0 IS NOT USED!!!!*/
    LmDialNumberRecord     *dialNumRecord_p;
    /** Pointer on the specific phonebook data array*/
    LmRecordSpeData         speData;
    LmRecordNumber          numRecords;
    LmRecordNumber          numRecordsUsed;
    TaskId                  storageTaskId;
}
LmDialNumberFileRecord;

/** Dial number element used by ABLM */ 
typedef struct LmDialNumberTag
{
    /** Indicates alpha ID */
    LmAlphaId               alphaId;
    /** Whether it is a dialling number */
    Boolean                 isDiallingNumber;
    /** Specifies the type of the number */
    BcdNumberType           typeOfNumber;
    /** Numbering plan identification */
    BcdNumberPlan           numberPlan;
    /** The length of the dialled string 
\assoc ARRAY \ref dialString */
    Int8                    dialStringLength;
    /** Dialled string */
    Bcd                     dialString[SIM_MAX_DIAL_NUMBER_LENGTH];
    /** Whether or not sub address containes valid information 
\assoc PRESENT \ref calledPartySubaddress */
    Boolean                 subAddressDefined;
    /** Called party sub address */
    CalledPartySubaddress   calledPartySubaddress;
    /** Indicates if Bearer capabilities information is present 
\assoc PRESENT \ref bearerCapability */
    Boolean                 bearerCapabilityDefined;
    /** Contains the Bearer capabilities.*/
    BearerCapability        bearerCapability;
}
LmDialNumber;

/** Determines date and time */
typedef struct LmDateTimeTag
{
  /** Date */
  RtcDate         dnDate;
  /** Time */
  RtcTime         dnTime;
  /** Time in a defined zone */
  RtcDisplacement dnTimeZone;
}
LmDateTime;

/** Required hidden key to view the hidden phonebook entries.*/
typedef SimUiccHiddenKey          LmHiddenKey;

/** Holds identifiers for the  different hidden key functions applied in ABLM interface. */
typedef SimUiccHiddenKeyFunction  LmHiddenKeyFunction;

/** Phonebook ID value */
typedef SimUiccPbid               LmPhoneBookId;

/** Email address data element */
typedef SimUiccEmailData          LmEmailData;

/** ANR dial number element */
typedef SimDialNumber             LmAnrDialNumber;

/** Additional Number element */
typedef SimUiccAnr                LmAnr;

/** Specifies the state of ABLM */
typedef enum AblmStateTag
{
    ABLM_STATE_INITIAL,          /**< Waiting information that SIM is initialised */
    ABLM_STATE_READ_STATUS,      /**< Reading dialling number information - ABLM initialisation */
    ABLM_STATE_WAIT_RESUME,      /**< Only when resume is received lists can be created */
    ABLM_STATE_CREATE_LIST,      /**< AlphaList creation which can start only after resume signal is received */
    ABLM_STATE_ACTIVE,           /**< Initialisation performed, ABLM is ready */
    ABLM_STATE_SIM_ERROR,        /**< SIM ERROR */
  /** If FDN was enabled at start-up for a 2G SIM which does not allow read of 
   * invalidated ADN - when FDN is disabled we need to get the ADN info       
   * If FDN was enabled at startup for a 3G USIM which invalidates ADN when FDN is enabled,
   * then we must read ADN again when FDN gets disabled.
   * In theory, ADN should not be invalidated on 3G USIMs, except on some non-compliant Orange USIMs 
   */
    ABLM_STATE_UPDATE_ADN,
    ABLM_STATE_NVRAM_ERROR,      /**< NVRAM error */
    ABLM_STATE_POWER_DOWN        /**< Power Down request is received */
}
AblmState;

/** Structure to store context for the read synchronisation information signals exhange*/
typedef struct Ablm3gSyncInfoContextTag
{
    /** Differentiate the signal generation reasons*/
    SignalId            type;
    /** Destination task identifiant*/
    TaskId              destTaskId;
    /** Command reference which is mirrored in the confirmation signal */
    Int16               commandRef;
    /** The 3G phonebook accessed */
    LmDialNumberFile    file;
    /** The phonebook change counter */
    Int16               changeCounter;
    /** The last unique ID given to a record */
    Int16               previousUid;
    /** The phonebook ID */
    LmPhoneBookId       phoneBookId;
    /** Indication of whether or not the request was successful */
    LmRequestStatus     requestStatus;
}
Ablm3gSyncInfoContext;

/** The synchronisation information used by 3G ADN phonebooks*/
typedef struct Ablm3gSyncInfoTag
{
    /** The phonebook change counter*/
    Int16          changeCounter;
    /** The last unique ID given to a record*/
    Int16          previousUid;
    /** The phonebook ID*/
    LmPhoneBookId  phoneBookId;
    /** Whether synchronisation information are available*/
    Boolean        syncInfoAvalaible;
    /** Whether the phonebook is available*/
    Boolean available;
}
Ablm3gSyncInfo;

/** The synchronisation information used by 2G phonebooks*/
typedef struct Ablm2gSyncInfoTag
{
    /** Whether the phonebook is available*/
    Boolean available;
}
Ablm2gSyncInfo;

/** The NVRAM stored information for phonebook synchronisation*/
typedef struct AblmSyncInfoAreaTag
{
    /** ADN GLB synchronisation information.
    **  Only significant if card is USIM*/
    Ablm3gSyncInfo  adnGlb;
    /** ADN APP synchronisation information.
    **  Only significant if card is USIM*/
    Ablm3gSyncInfo  adnApp;
                    
    /** The 2G ADN phonebook synchronisation information.
    **  Only significant if card isn't USIM*/
    Ablm2gSyncInfo  adn;
    /** The 2G FDN phonebook synchronisation information.*/
    Ablm2gSyncInfo  fdn;
    /** The 2G LND phonebook synchronisation information.*/
    Ablm2gSyncInfo  lnd;
    /** The 2G MSISDN phonebook synchronisation information.*/
    Ablm2gSyncInfo  msisdn;
    /** The 2G SDN phonebook synchronisation information.*/
    Ablm2gSyncInfo  sdn;
    /** The 2G BDN phonebook synchronisation information.*/
    Ablm2gSyncInfo  bdn;
    /** The 2G CPHS phonebook synchronisation information.*/
    Ablm2gSyncInfo  cphs;
                    
    /** Whether it is SIM card*/
    Boolean         cardIsUicc;
}
AblmSyncInfoArea;

/**Used to store the result for comparaison between synchronision information*/
typedef struct AblmSyncInfoCompareTag
{
    /** The currents values for the synchronisation information (3G and 2G checksums)*/
    AblmSyncInfoArea   syncInfo;

    /** Whether ADN /ADN GLB has been modified*/
    Boolean            adnGlbHasChanged;
    /** Whether ADN APP has been modified*/
    Boolean            adnAppHasChanged;
    /** Whether FDN has been modified*/
    Boolean            fdnHasChanged;
    /** Whether LND has been modified*/
    Boolean            lndHasChanged;
    /** Whether MSISDN has been modified*/
    Boolean            msisdnHasChanged;
    /** Whether SDN has been modified*/
    Boolean            sdnHasChanged;
    /** Whether BDN has been modified*/
    Boolean            bdnHasChanged;
    /** Whether CPHS has been modified*/
    Boolean            cphsHasChanged;
}
AblmSyncInfoCompare;

/** Contains a cached ADN record that will be used to speed up
 ** phonebook synchronisation with upper layer.*/
typedef struct AdnCachedRecordTag
{
    /** Record number */
    LmRecordNumber                  recordNumber;
    /** Index of the record alpha id in the alphastore */
    LmRecordNumber                  alphaIndex;
    /* Phonebook this record came from */
    Boolean                         globalPhonebook;      
    /** set FALSE if SSC string etc. */
    Boolean                         isDiallingNumber;
    /** Type of Number (TON) */
    BcdNumberType                   typeOfNumber;
    /** Numbering Plan Id (NPI) */
    BcdNumberPlan                   numberPlan;
    /** The length of dialled number */
    Int8                            dialStringLength;
    /** Bearer capability record number */
    Int8                            bearerCapRecordNum;
    /** Indicates if sub address is defined */
    Boolean                         subAddressDefined;
    /** Dialled number */
    Bcd                             dialString[SIM_MAX_DIAL_NUMBER_LENGTH];
    /** Lists called party sub address */
    CalledPartySubaddress           calledPartySubaddress;
    /** Whether the entry is hidden*/
    Boolean                         isHidden;
}
AdnCachedRecord;

/*this structure is used to store some info about the number searched*/

typedef struct FurtherSearchInfoTag
{
  Int16               probesDone;
  DialledBcdNum       bcdNumber;    /*number searched*/
  Int32               keyValue;
} FurtherSearchInfo;


typedef struct AmendDialNumContextTag
{
  LmDialNumberFile    dnFile;      /* the adn file */
  LmRecordNumber      recordNumber; /* Record number in the NVRAM file */
  SimDialNumber       dialNum;

} AmendDialNumContext;



/* this structure should be used to store Sim dialNum data
 * while bearer capability information is retrieved */
typedef struct DialNumContextTag
{
    SignalId            type;
    TaskId              destTaskId;
    Int16               commandRef;
    LmDialNumberFile    file;
    LmRecordNumber      recordNumber;
    LmRequestStatus     requestStatus;
    LmDialNumber       *dialNum_p;
    Boolean             bearerCapabilityDefined;
    Boolean             hiddenEntry;
    Int16               callId;
    Int16               magicNumber;
    Boolean             dateTimePresent;  /*job 106697 relevant call info added */
    LmDateTime          dateTime;
    Boolean             callInfoValid;
    Int32               callDuration;
    Boolean             callAnswered;
    FurtherSearchInfo   furtherSearchInfo;
    LmRecordNumber      alphaIndex;
}
DialNumContext;

typedef struct DnDirectoryRecordTag
{
  LmDialNumberFile    dnFile;      /* the dialling number file */
  LmRecordNumber      recordNumber; /* Record number in the SIM/NVRAM fiel */
  Bcd                 partialKey[DIAL_NUM_DIGIT_KEY_LEN];
                         /* Last 'n' DialNumber digits cached in BCD Format*/
} DnDirectoryRecord;

/*
  The BL required to keep a record of the SIM's FDN file.
*/

typedef struct FdnBcdNumTag
{
    Int8                            numberLength;
    BcdNumberType                   type;
    BcdNumberPlan                   numPlan;
    PresentationIndicator           presentInd;
    ScreeningIndicator              screeningInd;
    Bcd                             number[MAX_SS_OR_DIAL_STRING_LENGTH];
}
FdnBcdNum;


typedef struct FdnListEntryTag
{
  Boolean                   bearerCapabilityDefined;
  BearerCapability          bearerCapability;
  Boolean                   subAddressDefined;
  CalledPartySubaddress     calledPartySubaddress;
  FdnBcdNum                 dialledBcdNum;
  Boolean                   isDiallingNumber;
  LmRecordNumber            recordNumber;
}
FdnListEntry;

/* allocation unit for FDN numbers read from the SIM */
typedef struct FdnBlockTag
{
  struct FdnBlockTag    *next_p;      /* ptr to next block */
  Int8                  numFdnsInBlock;
  FdnListEntry          fdnEntries [SIM_ALPHA_LIST_SIZE];
}
FdnBlock;

typedef struct FdnListTag
{
  /* Used when constructing the FDN list */
  Boolean                   isComplete;
  LmRecordNumber            fileSize;

  /* The FDN list */
  LmRecordNumber            totalFdnEntries;
  LmRecordNumber            fdnSimRecordNumber; /* used while FDN records */
                                                /* being read from SIM    */
  FdnBlock                  *firstFdnBlock_p;
}
FdnList;

/* 0006-6836. This defines the contents of a cell within the LNA cache. On
              startup, the LND,LNM and LNR lists are read, and a linear,
              time-sorted array is built for the LNA, with each element
              describing where that record can be found (in LND,LNM or LNR).
*/
typedef struct LnaRecordTag
{
    LmDialNumberFile  containingFile;  /* Which file is the record in? */
    LmRecordNumber    recordNumber;    /* index of record within file  */
} LnaRecord;

/*0209-20012 Include association index to improve lookup speed*/
typedef struct AlphaRecordTag
{
  LmDialNumberFile    file;
  LmRecordNumber      recordNumber;
  Int8                alphaIdLength;
  Int8                alphaIdData[1];
} AlphaRecord;

/***************************************************************************
 *  Macros
 **************************************************************************/


/***************************************************************************
 *  Function Prototypes
 **************************************************************************/
#endif

/* END OF FILE */
