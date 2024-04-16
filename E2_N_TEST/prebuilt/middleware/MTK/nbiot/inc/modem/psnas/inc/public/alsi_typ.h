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
 *  File Description :                                                   */
/** \file
 *      types common to alsi_sig.h and mmsi_sig.h
 **************************************************************************/

#ifndef ALSI_TYP_H
#define ALSI_TYP_H

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

#if !defined (SIR_TYP_H)
#include     <sir_typ.h>
#endif

#if !defined (UTTIME_H)
#  include <uttime.h>
#endif

#if !defined (PDP_TYP_H)
#include     <pdp_typ.h>
#endif

#if !defined (GPQOS_H)
#include <gpqos.h>
#endif


#if defined (UPGRADE_LTE)
#if !defined (MTK_NBIOT_CUSTOM_BUILD) && !defined(GEN_NVDM_DATABASE_FOR_PC)
#include <emm_typ.h>
#include <eps_asn.h>
#include <eps_typ.h>
#else
#include <emm_public_typ.h>
#endif
#endif

/***************************************************************************
 * Manifest Constants
 **************************************************************************/
#if defined (DATA_IN_SIGNAL)
#define SIM_MAX_MMSN_NOTIFICATION   400
#endif

#define  MAX_SIM_FATAL_HANDLING   2
#define  SIM_LONG_MNC   3
#define  SIM_SHORT_MNC  2
/* common to alsi_sig.h and alsa_sig.h (SIM Toolkit) */
#define  SIM_MAX_DIAL_NUMBER_LENGTH           40

/* job102238: added for SIMs without exdended storage */
#define  SIM_NO_EXT1_MAX_DIAL_NUMBER_LENGTH   20

/* Note: the numbers below do not indicate the maximum number of OPL/PNN entries we can support.
 * These are the maximum number of entries that can be returned inside the AlsiListOplCnf/AlsiListPnnCnf signals
 * if there are more than  SIM_OPL_LIST_SIZE/SIM_PNN_LIST_SIZE entries, another AlsiListOplReq/AlsiListPnnReq will be sent*/
# define    SIM_OPL_LIST_SIZE              20
# define    SIM_PNN_LIST_SIZE              10

/*
 * HomeZone related defines and types
 */

#define SIM_HZ_NUM_HOMEZONES 4
#define SIM_HZ_MAX_CELL_CACHE_LENGTH 21

/*
 * Structure of the HZ Cache file
 */
#define SIM_HZ_CACHE_LAC     0
#define SIM_HZ_CACHE_CELLID  2


/*
 * Structure of the HomeZone parameter data in the SIM
 */
#define SIM_HZ_PARA_SUBS_FLAGS         0
#define SIM_HZ_PARA_SMSC_LEN           1
#define SIM_HZ_PARA_SMSC_TON_AND_NPI   2
#define SIM_HZ_PARA_SMSC_NUMBER        3
#define SIM_HZ_PARA_SCP_LEN            9
#define SIM_HZ_PARA_SCP_TON_AND_NPI   10
#define SIM_HZ_PARA_SCP_NUMBER        11
#define SIM_HZ_PARA_HZ1_PARAMETERS    17
#define SIM_HZ_PARA_LENGTH            26

/*
 * Structure of the HomeZone definition in the SIM
 */
#define SIM_HZ_ZONE_DEFINITION_ID      0
#define SIM_HZ_ZONE_DEFINITION_X       1
#define SIM_HZ_ZONE_DEFINITION_Y       5
#define SIM_HZ_ZONE_DEFINITION_R2      9
#define SIM_HZ_ZONE_DEFINITION_ACTIVE 13
#define SIM_HZ_ZONE_DEFINITION_TAG    14

#define  MAX_RELAY_SERVER_ADDRESS      100
/*The minimum size of the different arrays is defined in the WAP provisioning content spec, section 5. */
#define  MAX_MMS_AUTH_ID                20
#define  MAX_MMS_AUTH_NAME              20
#define  MAX_MMS_AUTH_SECRET            20
#define  MAX_MMS_AUTH_PW                20
#define  MAX_MMS_AP_ADDRESS             64
#define  MAX_GATEWAY_ADDRESS            45
#define  MAX_NUM_KEY_REFERENCE          5
#define  MAX_DIR_FREE_SPACE             4
#define  MAX_NUM_VOLTAGE_CLASSES        3
#define  MAX_NUM_PROTOCOLS              2
#define  MAX_PBR_RECORDS                2
#define  MAX_FILES_PER_PBR_RECORD       15
#define SIM_UICC_MAX_NUM_EMAIL_FILES    4
#define SIM_UICC_MAX_NUM_ANR_FILES      4
#define SIM_UICC_PBID_FIXED_PART_LENGTH 10
#define SIM_UICC_PSC_LENGTH              4
#define UICC_MAX_NUM_RECORDS             40
#define UICC_LABEL_MAX_SIZE              32
#define SIM_MAX_LENGTH_ECC_ALPHA_ID      30  /* max length of the alpha Id of the emergency code */
#define MAX_SIM_APPLICATIONS              5  /* max sim applications we can fetch at a time*/
#define SIM_APN_LIST_SIZE                 5  /* max num APN returned in one go*/
#define UICC_AID_MAX_SIZE                16
#define MAX_SIM_EXT_RECORDS              0xFF
#define SIM_PSM_DATA_SIZE                3600 /* max length of the PSM static efFile data  */
#define SIM_PSM_BLOCK_SIZE               1900
#define SIM_PSM_RTC_DATA_SIZE            122
#define UICC_RESUME_TOKEN_SIZE           8


/****************************************************************************
 * Macros
 ****************************************************************************/

/****************************************************************************
 * Types
 ****************************************************************************/
#define MAX_SELECTION_PATH_LEN                       10

typedef enum MmsStatusInfoTag
{
  MMS_RETRIEVED,
  MMS_NOT_RETRIEVED,
  MMS_REJECTED,
  MMS_FORWARDED
} MmsStatusInfo;

typedef struct MmsStatusTag
{
  Boolean        used;
  Boolean        read;
  MmsStatusInfo  info;
} MmsStatus;

/** MMS implementation.
 * Only WAP is supported currently. */
typedef enum MmsImplementationTag
{
  WAP_IMPLEMENTATION = 0,
  M_IMAP_IMPLEMENTATION,
  SIP_IMPLEMENTATION,
  UNKNOWN_IMPLEMENTATION
} MmsImplementation;

/*MMS WAP Gateway address type (PXADDRTYPE), see 23.140, annex F*/
typedef enum SimGatewayAddrTypeTag
{
  MMS_GW_ADDRTYPE_UNDEFINED = 0,
  MMS_GW_ADDRTYPE_IPV4 = 0x85,
  MMS_GW_ADDRTYPE_IPV6 = 0x86,
  MMS_GW_ADDRTYPE_E164 = 0x87,
  MMS_GW_ADDRTYPE_ALPHA = 0x88
} SimGatewayAddrType;

/*MMS Bearer type (BEARER), see 23.140, annex F*/
typedef enum SimMmsBearerTag
{
  MMS_BEARER_UNDEFINED = 0,
  MMS_GSM_USSD = 0xa2,
  MMS_GSM_SMS = 0xa3,
  MMS_GSM_CSD = 0xaa,
  MMS_GSM_GPRS = 0xab
} SimMmsBearer;

/** Type of connectivity parameters. */
typedef enum SimMmsCpTypeTag
{
  MMS_ISSUER_CP = 0,
  MMS_USER_CP
} SimMmsCpType;

/* Available Service, e.g. connectionless, secured, etc..(SERVICE) */
/* See WAP provisioning content spec. */
typedef enum SimMmsServiceTag
{
  MMS_SERVICE_UNDEF = 0x00,   /*not specified*/
  MMS_CL_WSP = 0xca,          /* WAP connection-less session service */
  MMS_CO_WSP = 0xcb,          /* WAP session service */
  MMS_CL_SEC_WSP = 0xcc,      /* WAP secure connection-less session service */
  MMS_CO_SEC_WSP = 0xcd,      /* WAP secure session service */
  MMS_C0_SEC_WTA = 0xce,      /* WAP WTA secure session service (over WSP) */
  MMS_CL_SEC_WTA = 0xcf,      /* WAP WTA secure connection-less session service (over WSP) */
  MMS_OTA_HTTP_TO = 0xd0,     /* OTA-HTTP service (push), TO-TCP [PushOTA] */
  MMS_OTA_HTTP_TLS_TO = 0xd1, /* OTA-HTTP secure service (push), TO-TCP [PushOTA] */
  MMS_OTA_HTTP_PO = 0xd2,     /* OTA-HTTP service (push), PO-TCP [PushOTA] */
  MMS_OTA_HTTP_TLS_PO = 0xd3  /* OTA-HTTP secure service (push), PO-TCP [PushOTA] */
} SimMmsService;

/*AUTHTYPE/PXAUTH-TYPE: Authentication type*/
typedef enum SimMmsAuthTypeTag
{
  MMS_AUTH_TYPE_UNDEF = 0,
  MMS_AUTH_TYPE_PAP = 0x9a,
  MMS_AUTH_TYPE_CHAP = 0x9b,
  MMS_AUTH_TYPE_HTTP_BASIC = 0x9c,
  MMS_AUTH_TYPE_HTTP_DIGEST = 0x9d,
  MMS_AUTH_TYPE_WTLS_SS = 0x9e,
  MMS_AUTH_TYPE_MD5 = 0x9f
 } SimMmsAuthType;

 /* NAP-ADDRTYPE */
 typedef enum SimMmsApAddrTypeTag
 {
   MMS_AP_ADDRTYPE_UNDEFINED = 0,
   MMS_AP_ADDRTYPE_IPV4 = 0x85,
   MMS_AP_ADDRTYPE_IPV6 = 0x86,
   MMS_AP_ADDRTYPE_E164 =0x87,
   MMS_AP_ADDRTYPE_ALPHA = 0x88,
   MMS_AP_ADDRTYPE_APN= 0x89,
   MMS_AP_ADDRTYPE_SCODE = 0x8a,
   MMS_AP_ADDRTYPE_TETRA = 0x8b,
   MMS_AP_ADDRTYPE_MAN = 0x8c
 } SimMmsApAddrType;

 /*CALLTYPE*/
 typedef enum SimMmsCallTypeTag
 {
   MMS_CALL_TYPE_UNDEFINED = 0x00,
   MMS_ANALOG_MODEM = 0x90,
   MMS_V120 = 0x91,
   MMS_V110 = 0x92,
   MMS_X31 = 0x93,
   MMS_BIT_TRANSPARENT = 0x94,
   MMS_DIRECT_ASYNCHRONOUS = 0x95

 } SimMmsCallType;


/* Address of the WAP gateway. See 23.140, annex F (PXADDR)  */
typedef struct SimGatewayAddressTag
{
  Int8  length;
  Int8  value[MAX_GATEWAY_ADDRESS];
} SimGatewayAddress;

/*MMS Relay/ Server */
typedef struct SimRelayServerAddressTag
{
  Int8  length;
  Int8  value[MAX_RELAY_SERVER_ADDRESS];
} SimRelayServerAddress;

/*NAP-ADDRESS: Address of associated Access point*/
typedef struct SimApAddressTag
{
   Int8 length;
   Int8 value[MAX_MMS_AP_ADDRESS];
} SimApAddress;

typedef struct SimAuthIdTag
{
   Int8 length;
   Int8 value[MAX_MMS_AUTH_ID];
} SimAuthId;

typedef struct SimAuthPwTag
{
   Int8 length;
   Int8 value[MAX_MMS_AUTH_PW];
} SimAuthPw;

/*WAP Gateway for WAP implementation of MMS*/
typedef struct SimMmsGatewayTag
{
  SimGatewayAddress   address;   /*PXADDR*/
  SimGatewayAddrType  addrType;  /*PXADDRTYPE*/
  Int16               port;      /*PORTNBR*/
  SimMmsService       service;   /*SERVICE*/
  SimMmsAuthType      authType;  /*PXAUTH-TYPE*/
  SimAuthId           authId;    /*PXAUTH-ID*/
  SimAuthPw           authPw;    /*PXAUTH-PW*/
  SimMmsCallType      callType;  /*CALLTYPE*/
} SimMmsGateway;

/* AUTHNAME */
typedef struct SimMmsAuthNameTag
{
   Int8 length;
   Int8 value[MAX_MMS_AUTH_NAME];
} SimMmsAuthName;

/* AUTHSECRET */
typedef struct SimMmsAuthSecretTag
{
   Int8 length;
   Int8 value[MAX_MMS_AUTH_SECRET];
} SimMmsAuthSecret;


/*Interface to core network including access point for the core network and required bearer.*/
typedef struct SimIfToCoreNetAndBearerTag
{
   SimMmsBearer      bearer;      /* BEARER */
   SimApAddress      address;     /* NAP-ADDRESS */
   SimMmsApAddrType  addrType;    /* NAP-ADDRTYPE */
   /*If set to TRUE, then the "linkspeed" field will be set to 0*/
   /*If linkspeed wasn' t specified, autobauding will be set to FALSE,
    * and linkspeed will be set to 0*/
   Boolean           autobauding; /* LINKSPEED */
   Int32             linkspeed;   /* LINKSPEED */
   SimMmsCallType    callType;    /* CALLTYPE  */
   SimMmsAuthType    authType;    /* AUTHTYPE*/
   SimMmsAuthName    authName;    /* AUTHNAME*/
   SimMmsAuthSecret  authSecret;  /* AUTHSECRET*/
   Boolean                  errSduDeliveryDefined;
   DeliveryOfErroneousSdu   errSduDelivery;
   Boolean                  residualBerDefined;
   ResidualBER              residualBer;
   Boolean                  sduErrRatioDefined;
   SduErrorRatio            sduErrorRatio;
   Boolean                  trafficClassDefined;
   TrafficClass             trafficClass;
   Int16                    maxBitRateUplink;
   Int16                    maxBitRateDownlink;
} SimIfToCoreNetAndBearer;


typedef struct MmsNotificationTag
{
#if defined (DATA_IN_SIGNAL)
  Int8   data[SIM_MAX_MMSN_NOTIFICATION];
#else
  Int8  *data_p;
#endif
  Int16  dataLength;
} MmsNotification;

 /*-----------------31/10/01 15:17-------------------*/
 /** path used for the SELECT by PATH                */
 /*--------------------------------------------------*/
 typedef struct SimUiccPathTag
 {
    /** The length of data path */
    Int8 length;
    /** Contains data path */
    Int8 data[MAX_SELECTION_PATH_LEN];
 } SimUiccPath;

/* Short File Identifier */
typedef struct SimUiccSfiTag
{
  Boolean supported;
  Int8    data;
}SimUiccSfi;

typedef enum SimEfStructureTag
{
    SIM_EFSTRUCT_T              =   0,
    SIM_EFSTRUCT_LF             =   1,
    SIM_EFSTRUCT_C              =   3,
    SIM_EFSTRUCT_UNKNOWN        =   0xff
}
SimEfStructure;

/**** TYPE SimEfInfo *******************************************************
*
* Holds information on an Elementary File (EF) i.e. number of records, the
* sfi and recordLength
*
****************************************************************************/
typedef struct SimEfInfoTag
{
    Int8            recordLength;
    SimUiccSfi      sfi;                 /* Short File Identifier */
    Int8            numRecords;
} SimEfInfo;

/** Describes the timing information associated with a record from
 * EF OCI or EF ICI file
 */
typedef struct SimUiccTimingInfoTag
{
   /** Date */
   RtcDate            date;
   /** Time */
   RtcTime            time;
   /** Time in a defined zone */
   RtcDisplacement    timezone;
   /** Call duration */
   Int32              callDuration;
} SimUiccTimingInfo;

/** Phonebook Link is used for reading ICI, and OCI file */
typedef struct SimUiccPbLinkTag
{
    /** Indicates if a global phone book */
    Boolean                globalPhonebook;
    /** If adnRecordNum = 0, the link is not valid */
    Int16                  adnRecordNum;
} SimUiccPbLink;


/*information about the current records*/
typedef struct SimUiccCurrRecordInfoTag
{
  Int8 fdnCurrentRecord;
  Int8 bdnCurrentRecord;
  Int8 lndCurrentRecord;
  Int8 msisdnCurrentRecord;
  Int8 ociCurrentRecord;
  Int8 iciCurrentRecord;
  Int8 sdnCurrentRecord;
  Int8 mbdnCurrentRecord;
  Int8 cfisCurrentRecord;
  Int8 smsCurrentRecord;
  Int8 smsrCurrentRecord;
  Int8 smspCurrentRecord;
  /*CPHS file*/
  Int8 cphsInfoNumCurrentRecord;
  Int8 cphsMnCurrentRecord;
} SimUiccCurrRecordInfo;


typedef enum SimUiccFileLinkingTag   /*Used for EF_PBR: type of file linking*/
{
   SIM_UICC_TYPE_1,
   SIM_UICC_TYPE_2,
   SIM_UICC_TYPE_3,
   SIM_UICC_UNDEFINED_TYPE
} SimUiccFileLinking;


typedef struct SimUiccPbrFileInfoTag
{
  Int16                 fileId;
  SimUiccFileLinking    fileLinking;
  SimEfInfo             efInfo;    /*includes info such as the sfi, current record, etc...*/

} SimUiccPbrFileInfo;
/*describes a PBR record */

typedef struct SimUiccPbrRecordTag
{
   SimUiccPbrFileInfo    adnFile;
   SimUiccPbrFileInfo    grpFile;
   SimUiccPbrFileInfo    ext1File;
   SimUiccPbrFileInfo    pbcFile;
   SimUiccPbrFileInfo    sneFile;
   Int8                  sneIapOffset;    /*offset in the IAP file*/
   SimUiccPbrFileInfo    iapFile;
   SimUiccPbrFileInfo    gasFile;
   SimUiccPbrFileInfo    aasFile;
   SimUiccPbrFileInfo    ccp1File;
   SimUiccPbrFileInfo    uidFile;
   Int8                  numEmailFiles;
   SimUiccPbrFileInfo    emailFile[SIM_UICC_MAX_NUM_EMAIL_FILES];
   Int8                  emailIapOffset[SIM_UICC_MAX_NUM_EMAIL_FILES];    /*offset in the IAP file*/
   Int8                  numAnrFiles;
   SimUiccPbrFileInfo    anrFile[SIM_UICC_MAX_NUM_ANR_FILES];
   Int8                  anrIapOffset[SIM_UICC_MAX_NUM_ANR_FILES];    /*offset in the IAP file*/
   Int8                  numInvalidRecords;   /*number invalid ADN records in adnFile*/
   Int8                 *invalidRecords_p;    /*list of invalid record numbers*/
   Int8                  adnRecordUsed[MAX_SIM_EXT_RECORDS+1]; /*adn used indicator in every pbr*/
   Int8                  emailActualUsed[MAX_SIM_EXT_RECORDS]; /*email used indicator in every pbr*/
   Int16                 iapIndicateUsed[MAX_SIM_EXT_RECORDS]; /*IAP used indicator in every pbr*/
} SimUiccPbrRecord;

typedef struct SimUiccPhoneBookTag
{
    Int8                  numPbrRecords;
    SimUiccPbrRecord     *pbrRecord_p;            /*points to the first PBR record*/
    Int16                 currentAdnRecord;       /*last ADN record accessed*/
    Int16                 totalNumAdnRecords;     /*total number of ADN records*/
    /*SEARCH RECORD can be used to determine which ADN records are invalid*/
    Boolean               searchRecordDone;
    Boolean               syncSupported;          /*whether synchronisation is supported*/
    Boolean               pscUpdated;             /* only update PSC once if both CC and UID need reset*/
    Boolean               extCounted;
    Boolean               emailCounted;
     /** Indicates the used email record in the (u)sim SM or AP*/
    Int16                 usedEmailRecords;
    /** Indicates the total email record in the (u)sim  SM or AP*/
    Int16                 maxEmailRecords;
     /** Indicates the used ext record in the (u)sim  SM or AP*/
    Int16                 usedExtRecords;
    /** Indicates the total ext record in the (u)sim  SM or AP*/
    Int16                 maxExtRecords;
    Int8                  adnIndicateUsed[MAX_SIM_EXT_RECORDS];

    Int8                  extActualUsed[MAX_SIM_EXT_RECORDS];
    /*marked because every PBR may contain 250 items, add to SimUiccPbrRecord struct*/
    /*
    Int8                  adnRecordUsed[MAX_SIM_EXT_RECORDS+1];
    Int8                  emailActualUsed[MAX_SIM_EXT_RECORDS];
    Int16                 iapIndicateUsed[MAX_SIM_EXT_RECORDS];
    */
} SimUiccPhoneBook;

/** Specifies the GPRS update status */
typedef enum GprsUpdateStatusTag
{
    /** UPDATED */
    SIM_GUS_UPDATED             =   0,
    /** NOT_UPDATED */
    SIM_GUS_NOT_UPDATED         =   1,
    /** ROAMING NOT ALLOWED */
    SIM_GUS_PLMN_NOT_ALLOWED    =   2,
    /** ROUTING AREA NOT ALLOWED */
    SIM_GUS_RA_NOT_ALLOWED      =   3,
    /** RESERVED */
    SIM_GUS_RESERVED            =   7
}
GprsUpdateStatus;

/** Routing information element */
typedef struct RoutingInformationTag
{
    /** P-TMSI */
    Int32                           pTmsi;
    /** P-TMSI signature identifies a GMM context of an MS */
    Int32                           pTmsiSignature;
    /** provides an unambiguous identification of routing areas
     * within the GPRS coverage area
     */
    Rai                             rai;
    /** GPRS update status stored in a SIM/USIM */
    GprsUpdateStatus                gprsUpdateStatus;
}
RoutingInformation;

/** The update statuse pertains to a specific subscriber embodied by a SIM/USIM.
 * The update status is changed only as a result of a location updating procedure attempt
 * (with the exception of an authentication failure and of some cases of CM service rejection).
 */
typedef enum LocationUpdateStatusTag
/* location update status is stored on SIM */
{
    /** UPDATED */
    SIM_LUS_UPDATED             =   0,
    /** NOT_UPDATED */
    SIM_LUS_NOT_UPDATED         =   1,
    /** ROAMING NOT ALLOWED */
    SIM_LUS_PLMN_NOT_ALLOWED    =   2,
    /* Location Area NOT ALLOWED */
    SIM_LUS_LA_NOT_ALLOWED      =  3,
    /** Reserved */
    SIM_LUS_RESERVED            =  7
}
LocationUpdateStatus;

/** Location information element  */
typedef struct LocationInformationTag
{
    /** TMSI */
    Tmsi                            tmsi;
    /** LAI value */
    LocationAreaIdElement           lai;
    /** Update status defined in TS 04.08 sec 4.1.2.2 */
    LocationUpdateStatus            updateStatus;
}
LocationInformation;

/** SIM BCCH information element */
typedef struct SimBaListTag
{
  /** If the parameter valid is set false, the channel data should not be used
\assoc PRESENT \ref channelData */
    Boolean                         valid;
  /** Is a bit array where each bit represents the presence (or not) of a particular channel */
    Int8                            channelData[BA_LIST_SIZE];
}
SimBaList;



/*
** --------------------------------------------------------------------------
** SimCommandStatus enum:
**
** These enum values are returned by the command procedures used to execute
** sim commands (e.g STATUS, READ RECORD etc.)
** --------------------------------------------------------------------------
*/
typedef enum SimCommandStatusTag
{
    SIM_CS_OK,                     /* the command executed successfully */
    SIM_CS_OK_DATA_INVALID,        /*everything ok, but data not valid*/
    SIM_CS_ALLOC_ERROR,            /* no memory available to allocate a signal */
    SIM_CS_COMMAND_ERROR,          /* SW1, SW2 implies sim error (in swStatus) */
    SIM_CS_INVALID_RESPONSE_LENGTH,/* unexpected rx data length (>2) */
    SIM_CS_NO_SW_DATA,             /* < 2 bytes were rx'ed in a data ind from L1 */
    SIM_CS_SERVICE_NOT_AVAILABLE,  /* Requested service was unavailable */
    SIM_CS_SIM_REMOVED,            /* the SIM has been removed */
    SIM_CS_INVALID_MAC,            /*Only used for AUTHENTICATE : invalid MAC*/
    SIM_CS_DECODE_ERROR,           /*job 100951: could not decode FCP template*/
    SIM_CS_POWER_DOWN,             /*job 100935: mobile is powering down*/
    SIM_CS_COMMAND_INTERRUPTED,    /* A higher priority request has caused */
                                   /* the current command to be stopped.   */
    SIM_CS_SIMAT_BUSY,             /* SIM busy processing STK data */
    SIM_CS_SIM_APP_ERROR,          /* problem with the SIM Application     */
    SIM_CS_SIM_DDL_ERROR,          /* SMS-PP SIM Data Download Error */
    SIM_CS_SIM_DRV_ERROR           /* error generated by SIM Driver */
}
SimCommandStatus;

/** Whenever a command is transmitted to the SIM, the SIM returns two bytes
 * called SW1 and SW2. The contents and interpretation of these is command
 * specfic. The codes below represent a generic set of possible return codes
 * in SW1 and SW2.
 *
 * \note In some cases the value in SW2 should be read in addition to the
 * status below. For example, for code SIM_GS_VALID_RES_LENGTH, SW2 will
 * contain the length of response data held in the SIM that may be read
 * with the GET RESPONSE command. The SW1 and SW2 codes are detailed in
 * section 9.4 of GSM 11.11.
 */
typedef enum SimSwStatusCodeTag
{                                     /* SW1  SW2     (hex values) */
  /** SIM Application Toolkit
   * command processed
   */
    SIM_GS_NORMAL_ENDING,             /* 90   00      */
  /** length XX of the
   * response data
   */
    SIM_GS_VALID_RES_LENGTH,          /* 9F   length  */
  /** update successful but
   * after using an internal
   * retry routine X times
   */
    SIM_GS_UPDATE_RETRIES,            /* 92   retries */
  /** memory problem */
    SIM_GS_MEMORY_PROBLEM,            /* 92   40      */
  /** no EF selected */
    SIM_GS_NO_EF_SELECTED,            /* 94   00      */
  /** out of range
   * (invalid address)
   */
    SIM_GS_INVALID_ADDRESS,           /* 94   02      */
  /** file ID not found
   */
    SIM_GS_FILE_NOT_FOUND,              /* 94   04      */
  /** pattern not found */
    SIM_GS_PATTERN_NOT_FOUND,         /* 94   04      */
  /** file is inconsistent
   * with the command
   */
    SIM_GS_INCONSISTENT_COMMAND,      /* 94   08      */
  /** no CHV initialised */
    SIM_GS_NO_CHV_INITIALISED,        /* 98   02      */
  /** access condition not fulfilled */
    SIM_GS_ACCESS_DENIED,             /* 98   04      */
  /** in contradiction with CHV status */
    SIM_GS_CHV_STAT_CONTRADICT,       /* 98   08      */
  /** in contradiction with invalidation status */
    SIM_GS_INVALID_CONTRADICT,        /* 98   10      */
  /** CHV blocked/
   * UNBLOCK CHV blocked
   */
    SIM_GS_CODE_BLOCKED,              /* 98   40      */
  /** increase cannot be performed,
   * Max value reached
   */
    SIM_GS_MAX_VALUE_REACHED,         /* 98   50      */
  /** incorrect parameter P3 */
    SIM_GS_INCORRECT_P3,              /* 67   correct length */
  /** incorrect parameter P1 or P2 */
    SIM_GS_INCORRECT_P1_OR_P2,        /* 6B   given in GSM 11.11 */
  /** unknown instruction code given in the command */
    SIM_GS_UNKNOWN_INST_CODE,         /* 6D   given in GSM 11.11 */
  /** wrong instruction class given in the command */
    SIM_GS_WRONG_INST_CLASS,          /* 6E   given in GSM 11.11 */
  /** technical problem with no diagnostic given */
    SIM_GS_TECHNICAL_PROBLEM,         /* 6F   given in GSM 11.11 */

    /* status codes for Phase 2+ SIM Toolkit implemenation */
    SIM_GS_NORMAL_END_WITH_SIM_INFO,  /* 91   length  */
    SIM_GS_SIMAT_IS_BUSY,             /* 93   00      */
    SIM_GS_UNSPECIFIED_STATUS,         /* 62 57  job 103155  status not listed in spec returned by faulty SIM  */
    SIM_GS_DDL_ERROR_RES_LENGTH,      /* 9E   length  => Data Download Error */
    SIM_GS_EOF_REACHED_BEFORE_LE,     /* 62   82 job 109582 */
    SIM_GS_INCORRECT_DATA_PARAM,      /* 6A   80 */
    SIM_GS_LC_INCONSISTENT,           /* 6A   87 */
    SIM_GS_REF_DATA_NOT_FOUND,        /* 6A   88 */
    SIM_GS_EXPECTED_LENGTH,           /* 6C XX */
    SIM_GS_FUNC_IN_CLA_NOT_SUP,       /* 68   00 */
    SIM_GS_LOGICAL_CHANNEL_NOT_SUP,   /* 68   81 */
    SIM_GS_SECURE_MSG_NOT_SUP,        /* 68   82 */
    SIM_GS_COM_NOT_ALLOWED,           /* 6900, 6984, 6985 */
    SIM_GS_INVALID_MAC,               /* 98   62 */
    SIM_GS_UNDEFINED_APP_ERROR,       /* 98 XX */
    SIM_GS_GSM_CONTEXT_NOT_SUP,       /* 98 64 */
    SIM_GS_SYNC_FAILURE,              /* 98 65 */
#if defined (UPGRADE_3G)
# if defined (UPGRADE_3G_MBMS)
# if defined (UPGRADE_3G_TDD128)
    SIM_GS_AUTH_FAIL_NO_EFMUK_SPACE,  /* 98 67 */
    SIM_GS_MORE_DATA_EXPECTED,        /* 63 F1 */
    SIM_GS_AUTH_FAIL_NO_MEM_SPACE,    /* 98 66 */
    SIM_GS_MIKEY_EXPIRED,
#  endif /* UPGRADE_3G_TDD128 */
# endif /* UPGRADE_3G_MBMS */
#endif /* UPGRADE_3G */
    SIM_GS_UNKNOWN_STATUS             /* unknown SW1, SW2 combination */

}
SimSwStatusCode;



/*************************************************************************/
/*                  Homezone types                                       */
/*************************************************************************/

typedef Int8  SimHomeZoneID;
typedef Int32 SimHomeZoneXCoord;
typedef Int32 SimHomeZoneYCoord;
typedef Int32 SimHomeZoneRadius;
typedef Int8  SimHomeZoneActive;
typedef Int8  SimHZSubscriptionFlags;

typedef enum SimHomeZoneCacheIdTag
{
  SIM_HZ_CACHE1,
  SIM_HZ_CACHE2,
  SIM_HZ_CACHE3,
  SIM_HZ_CACHE4
} SimHomeZoneCacheId;

#define SIM_HZ_SMSC_LENGTH 6
typedef char SimSMSCNumber[SIM_HZ_SMSC_LENGTH];

#define SIM_HZ_SCP_LENGTH 6
typedef char SimSCPNumber[SIM_HZ_SCP_LENGTH];

#define SIM_HZ_TAG_LENGTH 12
typedef char SimHomeZoneTag[SIM_HZ_TAG_LENGTH];


typedef struct SimHomeZoneDefinitionTag
{
    SimHomeZoneID                 hzId;
    SimHomeZoneXCoord             hzX;
    SimHomeZoneYCoord             hzY;
    SimHomeZoneRadius             hzRSquared;
    SimHomeZoneActive             hzActive;
    SimHomeZoneTag                hzTag;
} SimHomeZoneDefinition;


typedef struct SimHomeZoneEFParametersTag
{
    SimHZSubscriptionFlags        hzSubscriptionFlags;
    Int8                          smscNumberLength;
    Int8                          smscTonAndNPI;
    SimSMSCNumber                 smscNumber;
    Int8                          scpNumberLength;
    Int8                          scpTonAndNPI;
    SimSCPNumber                  scpNumber;
    SimHomeZoneDefinition         homeZoneDefinitions[SIM_HZ_NUM_HOMEZONES];
} SimHomeZoneEFParameters;



typedef Int16 SimHZLAC;
typedef Int16 SimHZCellId;

typedef struct SimHomeZoneCellDataTag
{
    SimHZLAC                      lac;
    SimHZCellId                   cellId;
} SimHomeZoneCellData;

typedef SimHomeZoneCellData SimHomeZoneEFCacheEntry;

typedef struct SimHomeZoneCacheTag
{
   Int8                           entries;  /*this is the maximum number of entries supported by the SIM*/
   SimHomeZoneEFCacheEntry        cache[SIM_HZ_MAX_CELL_CACHE_LENGTH]; /*invalid entries all set to 0xff*/
} SimHomeZoneCache;

typedef struct HomeZoneCacheFilesTag
{
  SimHomeZoneCache                hzCache[SIM_HZ_NUM_HOMEZONES];
} SimHomeZoneCacheFiles;


/* class of instruction */
typedef enum  SimClassByteTag
{
    SIM_3G_CLASS =      0x00,     /*used on 3G SIMs*/
    SIM_80_CLASS =      0x80,
    SIM_90_CLASS =      0x90,
    SIM_GSM_CLASS =     0xa0      /*GSM class*/
} SimClassByte;
/* logical channel */
typedef enum SimUiccLogicalChannelTag
{
   BASIC_CHANNEL = 0,
   LOGICAL_CHANNEL_1  = 1,
   LOGICAL_CHANNEL_2  = 2,
   LOGICAL_CHANNEL_3  = 3,
   MAX_NUM_LOGICAL_CHANNELS = 4
} SimUiccLogicalChannel;

/*mode of SELECT */
typedef enum  SimUiccSelectModeTag
{
   SELECT_BY_FILE_ID                = 0x00,
   SELECT_CHILD_DF_OF_CURRENT_DF    = 0x01,
   SELECT_PARENT_DF_OF_CURRENT_DF   = 0x03,
   SELECT_BY_DF_NAME                = 0x04,
   SELECT_BY_PATH_FROM_MF           = 0x08,
   SELECT_BY_PATH_FROM_DF           = 0x09
} SimUiccSelectMode;



typedef  enum SimUiccModeOfOperationTag    /*job 100951: mode of operation as defined in ISO 7816-3 */
{
   SIM_UICC_NEGOCIABLE_MODE,
   SIM_UICC_SPECIFIC_MODE_CHANGEABLE,
   SIM_UICC_SPECIFIC_MODE_NON_CHANGEABLE
} SimUiccModeOfOperation;

typedef struct SimUiccAtrParamTag
{

   Boolean                     voltageIndicationPresent; /*job 100951:flag indicates whether voltage indication is present in the ATR.
                                                          * If so, it will be stored in simData->simVoltageCapab*/
   Boolean                     uiccTransmissionProtocol[MAX_NUM_PROTOCOLS];  /*Transmission protocols supported by the SIM/UICC*/
   Boolean                     globalInterfaceBytes;
   Boolean                     efDirPresent;                  /*EF DIR present on the SIM/UICC ?*/
   Boolean                     selectByFullDf;                /*select by full DF allowed ? */
   Boolean                     selectByPartialDf;             /*select by partial DF allowed ? */
   Boolean                     selectByPath;                  /*select by PATH allowed ? */
   Boolean                     selectBySfi;                   /*select by SFI allowed ? */
   Boolean                     logicalChannelSupported;       /*more than one logical channel supported ?*/
   Int8                        ta1;                           /*TA1 in the ATR*/
   Boolean                     ta1Present;                    /*TA1 present in the ATR ?*/
   SimUiccModeOfOperation      modeOfOperation;               /*job 100951:Mode of operation: specific or negociable*/
   /*Whether the card capability object is present in the historical bytes. If so, it is a UICC*/
   Boolean                     cardCapabObjPresent;
} SimUiccAtrParam;

typedef enum SimUiccApplicationCodeTag
{
  SIM_UICC_APP_CODE,
  SIM_USIM_APP_CODE,
  SIM_USAT_APP_CODE,
  SIM_ISIM_APP_CODE,
  SIM_USIM_JAVA_APP_CODE,
  SIM_ISIM_JAVA_APP_CODE,
  SIM_MANAGED_JAVA_APP_CODE,
  SIM_USIM_INI_APP_CODE,
  SIM_USIM_RN_APP_CODE,
  SIM_CSIM_APP_CODE,
  SIM_UNKNOWN_APP_CODE
} SimUiccApplicationCode;

/*-----------------19/10/01 10:37------------------------------------*/
/** Application Id that resides on the UICC see TS 31.110 for coding*/
/* -----------------------------------------------------------------*/
typedef struct SimUiccAidTag
{
  Int8       data[UICC_AID_MAX_SIZE];         /**< AID value */
  Int8       length;                          /**< AID length */
} SimUiccAid;

typedef struct SimUiccAidLabelTag
{
  SimUiccApplicationCode          applicationType;
  Int8                            length;                         /* label length */
  Int8                            data[UICC_LABEL_MAX_SIZE];      /* Application label */
} SimUiccAidLabel;

typedef struct SimUiccApplicationListTag
{
  Int8                            numUsimApplications;
  SimUiccAidLabel                 applicationLabel [MAX_SIM_APPLICATIONS];
} SimUiccApplicationList;

typedef struct SimApnListTag
{
  Int8                                         totalNumApns;  /*total number of APNs currently held on the SIM in EF ACL*/
  Int8                                         numApns;  /*num APNs in list below*/
  TextualAccessPointName                       apn[SIM_APN_LIST_SIZE];
} SimApnList;

/** Stores the terminal profile of the ME for passing to the SIM during the initialisation process.
 * The structure identifies the features relevant to the SIMAT that the ME supports.
 */

/** specifies the MMI specific items for terminal profile */
typedef struct SimatMmiSupportProfileTag
{
    /* flag to indicate if there should be stk support if this is false no terminal profile should
     * be downloaded to the SIM/UICC */
    Boolean    stkSupported;
    /** Menu Selection */
    Boolean    menuSelection;
    /** Enter UCS2 characters */
    Boolean    ucs2Entry;
    /** Display UCS2*/
    Boolean    ucs2Display;
    /** Display Text */
    Boolean    displayText;
    /** Get Inkey */
    Boolean    getInkey;
    /** Get Input */
    Boolean    getInput;
    /** Play Tone */
    Boolean    playTone;
    /** Select Item */
    Boolean    selectItem;
    /** Set-up Menu */
    Boolean    setUpMenu;
    /* event - User Activity */
    Boolean    userActivity;
    /** event - Idle  Screen Available */
    Boolean    idleScreenAvail;
    /** event - Browser Termination */
    Boolean    browserTermination;
    /** event - Display Parameters Changed */
    Boolean    displayParamChange;
    /** Set-Up Idle Mode Text */
    Boolean    setUpIdleModeText;
    /** Launch Browser */
    Boolean    launchBrowser;
    /** soft keys to select items */
    Boolean    selectItemSoftKeys;
    /** soft keys for set-up menu */
    Boolean    setUpMenuSoftKeys;
    /** screen sizing supported */
    Boolean    screenSizing;
    /** screen height, No display capability */
    Boolean    noDisplay;
    /** screen height, No keypad available */
    Boolean    noKeypad;
    /** variable size fonts supported */
    Boolean    variableSizeFonts;
    /* display can be resized */
    Boolean    displayResize;
    /** text wrapping is supported */
    Boolean    textWrapping;
    /** text scrolling is supported */
    Boolean    textScrolling;
    /** text attributes are supported */
    Boolean    textAttributes;
    /** the number of softkeys available */
    Int8       numberOfSoftKeys;
    /** the number of chars down on display */
    Int8       numberOfCharsDown;
    /** variable timeout for displaying text */
    Boolean    displayTextVariable;
    /** variable timeout for get inkey command */
    Boolean    getInkeyVariable;
    /** supports help for Get Inkey */
    Boolean    getInkeyHelp;
    /** the number of chars across display */
    Int8       numberOfCharsAcross;
    /* maximum width reduction that is supported */
    Int8       maxWidthReduction;
    /* text attribute - align left */
    Boolean    alignLeft;
    /* text attribute - align centre */
    Boolean    alignCentre;
    /* text attribute - align right */
    Boolean    alignRight;
    /* text attribute - normal font */
    Boolean    fontNormal;
    /* text attribute - large font */
    Boolean    fontLarge;
    /* text attribute - small font */
    Boolean    fontSmall;
    /* text attribute - normal style */
    Boolean    styleNormal;
    /* text attribute - bold style */
    Boolean    styleBold;
    /* text attribute - italic style */
    Boolean    styleItalic;
    /* text attribute - underline style */
    Boolean    styleUnderline;
    /* text attribute - strikethrough style */
    Boolean    styleStrikethrough;
    /* text attribute - foreground colour supported */
    Boolean    foregroundColour;
    /* text attribute - background colour supported */
    Boolean    backgroundColour;
}SimatMmiSupportProfile;

/** specifies the modem-specific items for terminal profile */
typedef struct SimatModemSupportProfileTag
{
  /** SMS-PP Download              */
  Boolean       smsPpDataDownload;
  /** Cell Broadcast data download      */
  Boolean       cBDataDownload;
  /** Call control */
  Boolean       callControl;
  /** MO Short Message Control */
  Boolean       moSmsControl;
  /** Refresh */
  Boolean       refresh;
  /** Proactive UICC: Send SMS */
  Boolean       sendSm;
  /** Proactive UICC: Send SS */
  Boolean       sendSs;
  /** Proactive UICC: send USSD */
  Boolean       sendUssd;
    /** Set up call */
  Boolean       setUpCall;
  /** Provide location info */
  Boolean       provideLocalInfo;
  /** Local Info + Network meas res */
  Boolean       provLocInfoNmr;
  /** Set up event list */
  Boolean       setUpEventList;
  /** Mobile terminating call */
  Boolean       mtCall;
  /** Call connection */
  Boolean       callConn;
  /** Call disconnection */
  Boolean       callDisconn;
  /** Location status */
  Boolean       locStatus;
  /** Timer expiration */
  Boolean       timerExpiration;
  /** USSD string in call control */
  Boolean       ussdStringInCc;
  /** Language selection */
  Boolean       languageSelection;
  /** Data available */
  Boolean       dataAvailable;
  /** Channel status */
  Boolean       channelStatus;
  /** Manually start stop a timer */
  Boolean       timerManStartStop;
  /** Manually get current timer value */
  Boolean       timerManGetCurrVal;
  /** Provide loacal info for the current date, time and time zone */
  Boolean       provLocInfoDttz;
  /** Run AT command */
  Boolean       runAtCommand;
  /** Send DTMF */
  Boolean       sendDtmf;
  /** Provide local info for language setting */
  Boolean       provLocInfoLang;
  /** Provide local info for time advance */
  Boolean       provLocInfoTimAdv;
  /** Language notification */
  Boolean       languageNotification;
  /** Open channel */
  Boolean       openChannel;
  /** Close channel */
  Boolean       closeChannel;
  /** Receive data */
  Boolean       receiveData;
  /** Send data */
  Boolean       sendData;
  /** Get channel status */
  Boolean       getChannelStatus;
  /** CSD supported by ME */
  Boolean       csdSupportedByMe;
  /** GPRS supported by ME */
  Boolean       gprsSupportedByMe;
  /** Number of channels supported by ME ( maximum value of 7 ) */
  Int8          numChannelsSupByMe;
  /** TCP supported */
  Boolean       tcpSupported;
  /** UDP supported */
  Boolean       udpSupported;
  /** Indication of icon support */
  Boolean       mmiIconSupportAvailable;   /* job108644 */
  /** Access technology changed */
  Boolean       accessTechnologyChanged;
  /** Provide local info for the current access technology */
  Boolean       provLocInfoAccessTechnology;
  /** GPRS call control */
  Boolean       gprsCallControl;
  /** nework rejection for EUTRAN */
  Boolean       eUtranNetworkReject;
}SimatModemSupportProfile;

typedef struct SimTerminalProfileTag
{
  SimatMmiSupportProfile     mmiProfile;
  SimatModemSupportProfile   modemProfile;
 }
SimTerminalProfile;

#if defined (UPGRADE_LTE)
/** EPS location information as stored on USIM file EF EPSLOCINFO or
 * NVRAM if this file is not present.
 */
typedef struct UsimEpsLocInfoTag
{
  /** Globally Unique temporary Identifier */
  Int8                        rawGuti[RAW_GUTI_IE_SIZE];
  /** The ID of the last registered Tracking Area */
  Lai                        lastRegTai;
  /** The current EPS update status */
  EpsUpdateStatus            epsUpdateStatus;
}
UsimEpsLocInfo;

/** EPS NAS Security Context is stored on USIM file EF EPSNSC
 * or NVRAM if this file is not present.
 */
typedef struct UsimEpsNasSecurityTag
{
  /** Key created as a result of EPS AKA process and its KSI */
  EutraKAsmeData             asmeKey;
  /** Count for uplink NAS messages */
  EutraNasCount              uplinkNasCount;
  /** Count for downlink NAS messages */
  EutraNasCount              downlinkNasCount;
  /** Bitmap defining which Integrity and Ciphering algorithms to be used.
   * Bits 1 to 3 define the integrity and bits 5 to 7 the encryption
   * algorithms.
   */
  Int8                       nasSecurityAlgorithms;
}
UsimEpsNasSecurity;
#endif /*UPGRADE_LTE */

/** USIM PSM or eDRX or OOS deep sleep data buffer
 * written in first SIM start and updated if SIM is changed.
 */
typedef struct UsimPSMSimDataTag
{
  /** Sim Power Saving Mode static file system data
   */
  Int16                      datalen; /* length of simPSMData */
  Int8                       simPSMData[SIM_PSM_DATA_SIZE];
}
UsimPSMSimData;

/** USIM PSM or eDRX or OOS deep sleep data buffer
 * written in first SIM start and updated if SIM is changed.
 */
typedef struct UsimPSMRtcSimDataTag
{
  /** Sim Power Saving Mode static file system data
   */
  Int16                      datalen; /* length of simPSMRtcData */
  Int8                       simPSMData[SIM_PSM_RTC_DATA_SIZE];
}
UsimPSMRtcSimData;

typedef struct SimUiccResumeTokenTag
{
  Int8       data[UICC_RESUME_TOKEN_SIZE]; /**< resume token value */
} SimUiccResumeToken;

#endif
/* END OF FILE */



