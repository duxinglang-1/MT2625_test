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
 *    Application Layer Background Layer SIM Support Type Definitions
 **************************************************************************/

#ifndef ABSI_TYP_H
#define ABSI_TYP_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#if !defined(APPLAYER_H)
#   include <applayer.h>
#endif

#if !defined(ALSI_SIG_H)
#   include <alsi_sig.h>
#endif

# if !defined(MMSI_SIG_H)
#   include <mmsi_sig.h>
# endif

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

#define MAX_PASSWORD_LENGTH  12

/* invalid item codes */
#define INVALID_PLMN_ELEMENT_VALUE  0xffff
#define SIM_INVALID_DATA            0xff
#define MEP_GID_DATA_INVALID_VALUE  (SIM_INVALID_DATA)

/*IMSI digits*/
#define MAX_MSIN_SIZE               5
/* MEP types */
/*0003-6232 Number of MEP networks now defined in abcfg.c so that it is client configurable*/

/* 9906-4268 From GSM 02.22 Page 9, the MEP network subset test shall be
 * performed on digits 6 and 7 of the IMSI, which translates to a single
 * msin array element since they are stored as 2 BCD's per byte. So change
 * number of subset digits to 1
 */
#define NUM_MEP_SUBSET_DIGITS       1
/* bit fields for MEP types */
#define MEP_SIM_MASK                0x01
#define MEP_NETWORK_MASK            0x02
#define MEP_NET_SUBSET_MASK         0x04
#define MEP_SP_MASK                 0x08
#define MEP_CP_MASK                 0x10


/* Default MEP password  and no of reties allowed before blocked */
#define MEP_DEFAULT_PASSWORD        "15987539"
#define MEP_NO_RETRIES              10

#define ABSI_DELETED_TAI            0xFFFEU

/***************************************************************************
 * Typed Constants
 **************************************************************************/
/*
    Enum to identify a personalisation
*/
typedef enum AbsiMepSelectorTag
{
    SIM_PERSONALISATION = 0,
    NETWORK_PERSONALISATION,
    NETWORK_SUBSET_PERSONALISATION,
    SERVICE_PROVIDER_PERSONALISATION,
    CORPORATE_PERSONALISATION,

    NUM_MEP_PERSONALISATIONS
}
AbsiMepSelector;

/*
    Enum to select operation on a personalisation
*/
typedef enum AbsiMepOperationTag
{
    ACTIVATE_PERSONALISATION = 0,
    DEACTIVATE_PERSONALISATION,
    DISABLE_PERSONALISATION,
    STATUS_PERSONALISATION,
    CHANGE_PASSWORD,
    NUM_OF_MEP_OPERATIONS,
}
AbsiMepOperation;

/*
    Result codes for personalisation requests
*/
typedef enum AbsiMepResultTag
{
    /* absiRAM local values */
    PERSONALISATION_OK,
    PERSONALISATION_FAILURE,
    PERSONALISATION_UNKNOWN,
    /* Additional info supplied on request to AF */
    PERSONALISATION_INVALID_LOCK_CODE,
    PERSONALISATION_INACTIVE,
    PERSONALISATION_DISABLED,
    PERSONALISATION_BLOCKED
}
AbsiMepResult;

/* ApexSimNokInd reasons for error message */
typedef enum SimNokReasonTag
{
  SIM_NOK_POWER_OFF,

  /* AlsiSimInsertedInd error codes */
  SIM_NOK_INSERTED_ERROR,           /* SIM_INSERTED_ERROR/_INSERTED_WRONG_SIM */

  /* AlsiSimRemovedInd codes */
  SIM_NOK_REMOVED,                  /* SIM_REMOVED/_REMOVED_UNDETECTED        */
  SIM_NOK_INVALID_STATUS_RSP,       /* SIM_REMOVED_INVALID_STATUS_RSP         */

  /* AlsiSimInitialiseCnf error codes */
  SIM_NOK_INITIALISE_FAILED,        /* SIM_INITIALISE_FAILED                  */

  /* Used when the running application session is stopped                     */
  SIM_NO_APPLICATION_RUNNING,

  /* Alsi...Cnf error SimRequestStatus' */
  SIM_NOK_INVALID_PARAMS,           /* SIM_REQ_INVALID_PARAMS                 */
  SIM_NOK_MEMORY_PROBLEM,           /* SIM_REQ_MEMORY_PROBLEM                 */
  SIM_NOK_FILE_NOT_FOUND,           /* SIM_REQ_FILE_NOT_FOUND                 */
  SIM_NOK_SERVICE_NOT_AVAILABLE,    /* SIM_REQ_SERVICE_NOT_AVAILABLE          */
  SIM_NOK_GENERAL_FAULT,            /* SIM_REQ_SIM_GENERAL_FAULT              */
  SIM_NOK_FILE_INVALIDATED,         /* SIM_REQ_FILE_INVALIDATED               */
  SIM_NOK_ALLOC_ERROR,              /* SIM_REQ_ALLOC_ERROR                    */
  SIM_NOK_SM_FAULT,                 /* SIM_REQ_SM_FAULT                       */

  /* codes resulting from info in AlsiChvFunctionCnf (chvStatus etc.) */
  /* OR the Initialise failed
   */
  /*0211-21410 Use specific codes for indicating blocked CHV values
   *The uninitialised values were never used.*/
  SIM_NOK_CHV1_BLOCKED,
  SIM_NOK_CHV2_BLOCKED,
  SIM_NOK_CHV1_UNBLOCK_BLOCKED,
  SIM_NOK_CHV2_UNBLOCK_BLOCKED,

  SIM_NOK_MEP_CHECK_FAILED,
  SIM_NOK_REFRESH,

  /* SIM invalidated due to network reject */
  SIM_NOK_NETWORK_REJECT,

  SIM_NOK_IMSI_FAILED,
  SIM_NOK_UNKNOWN,

#if !defined (UPGRADE_3G)
  SIM_NOK_GSM_ACCESS_UNAVAIL,  /*The USIM does not support GSM ACCESS*/
#endif

#if defined (UPGRADE_LTE)
  SIM_NOK_NOT_VALID_FOR_LTE,  /*The SIM is not valid for LTE */
#endif

  NUM_OF_SIM_NOK,
  SIM_NOK_NULL
}
SimNokReason;


/* Apex Sim Fault Ind type of file operation in progress when error occurred */
typedef enum OperationTypeTag
{
  SIMOP_READ,
  SIMOP_WRITE,
  SIMOP_DELETE,
  SIMOP_OTHER,

  NUM_OF_SIMOPS,
  SIMOP_NULL
}
OperationType;

typedef enum MepStateElementTag
{
    MEP_LOCK_INACTIVE,
    MEP_LOCK_ACTIVE,
    MEP_LOCK_DISABLED,
    MEP_LOCK_BLOCKED,
    MEP_LOCK_UNKNOWN
} MepStateElement;

typedef enum SimPlmnModeBitTag
{
  PLMN_MODE_BIT_0 = 0,
  PLMN_MODE_BIT_1,
  PLMN_MODE_BIT_UNDEFINED
} SimPlmnModeBit;

/***************************************************************************
 * Type Definitions
 **************************************************************************/


/* Moved here from anrm_typ.h  */

typedef struct PasswordTag
{
/** \assoc ARRAY \ref number */
  Int8    length;
  Int8    number[MAX_PASSWORD_LENGTH];
}
Password;


/*0003-6232 New structure to use in the ABSI context, network IDs now stored in abcfg.c */
typedef struct MePersonalisationContextTag
{
  Int8      personalisationsActivated; /* Bit field - masks required to select personalisations */
  Int8      personalisationsEnabled;   /* Bit field - masks required to select personalisations */
  Password  personalisationKeys [NUM_MEP_PERSONALISATIONS];
  Int8      keyRetries[NUM_MEP_PERSONALISATIONS];  /* no of retries remaining */
  SimPlmn  *networkIds_p;
  Bcd       networkSubsetId[NUM_MEP_SUBSET_DIGITS];
  Int8      serviceproviderId;
  Int8      corporateId;
  Imsi      simId;
} MePersonalisationContext;

typedef struct AlImsiTag
{
   Plmn                             hplmn;
   Bcd                              msin[MAX_MSIN_SIZE];
   Int8                             numMsinDigits;
   Boolean                          mncThreeDigitsDecoding;
}
AlImsi;



/* if there are more than 5 ECCs on the USIM */
typedef struct ExtendedEccContextTag
{
  Boolean        extendedListValid;
  Int8           numberOfEccs;         /* total number of Eccs on the UICC                    */
  Int8           currentEccCount;      /* number of ECCs that have been written to the array  */
  SimEccRecord   *eccRecord_p;         /* pointer to the array holding the Eccs               */
}
ExtendedEccContext;

/* ME Personalisation */
typedef struct MepLockStateTag
{
    MepStateElement           network ;
    MepStateElement           networkSubset ;
    MepStateElement           serviceProvider ;
    MepStateElement           corporate ;
    MepStateElement           sim;
}
MepLockState;

/* used to allocate storage for a block of PNNs read from AlsiListPnnCnf    */
/* Each block can be chained together to accommodate an arbitrary number of */
/* PNNs. Smae for OPL blocks                                                */
typedef struct PnnBlockTag
{
  struct      PnnBlockTag *nextBlock_p;
  SimPnnData  simPnnData;
}
PnnBlock;

typedef struct OplBlockTag
{
  struct      OplBlockTag *nextBlock_p;
  SimOplData  simOplData;
}
OplBlock;

#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
typedef struct CphsInitData
{
  /** Specifies CPHS phase */
  SimCphsPhase                    cphsPhase;
  /** CPHS service */
  SimCphsServiceTable             cphsService;
  /** Indicates if operator name is present */
  Boolean                         cphsOpNamePresent;
  /** The operator name */
  SimCphsOperatorName             cphsOpName;
  /** Indicates if CPHS optional name in short form is present */
  Boolean                         cphsOpNameShortFormPresent;
  /** CPHS optional name in short form */
  SimCphsOperatorNameShort        cphsOpNameShortForm;
  /** Mailbox numbers available */
  Boolean                         cphsMnPresent;
  /** The CPHS mailbox number */
  SimCphsMailboxNumbers           cphsMn;
  /** Indicates if CPHS voice mail waiting flags are present */
  Boolean                         cphsVmwFlagsPresent;
  /** CPHS voice message waiting flags */
  SimCphsVoiceMsgWaitingFlags     cphsVmwFlags;
  /** Indicates if CPHS Call Forward Flags are present */
  Boolean                         cphsCffPresent;
  /** CPHS Call Forward Flags */
  SimCphsCallForwardFlags         cphsCff;
  /** Whether or not CPHS customer service profile is present */
  Boolean                         cphsCspPresent;
  /** CPHS customer service profile */
  SimCphsCustServProfile          cphsCsp;
} CphsInitData;
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */

/***************************************************************************
 *  Macros
 **************************************************************************/


/***************************************************************************
 *  Function Prototypes
 **************************************************************************/


#endif

/* END OF FILE */
