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
 *      application layer - sim manager interface
 **************************************************************************/

#ifndef ALSI_SIG_H
#define ALSI_SIG_H


/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

#if !defined (SIL3_TYP_H)
#include     <sil3_typ.h>
#endif

#if !defined (SITL_TYP_H)
#include     <sitl_typ.h>
#endif

#if !defined (SITS_TYP_H)
#include     <sits_typ.h>
#endif

#if !defined (ALSI_TYP_H)
#include     <alsi_typ.h>
#endif

#if !defined (CBSI_TYP_H)
#include     <cbsi_typ.h>
#endif

#if !defined (SIR_TYP_H)
#include     <sir_typ.h>
#endif

#if !defined (L1SI_SIG_H)
#include     <l1si_sig.h>
#endif

#if !defined (TS_SIG_H)
#include     <ts_sig.h>
#endif

#if !defined (RRC_TYP_H)
#include     <rrc_typ.h>
#endif

#if defined (UPGRADE_3G)
# if defined (UPGRADE_3G_MBMS)
#  if defined (UPGRADE_3G_TDD128)
#   if !defined (MBMS_TYP_H)
#include     <mbms_typ.h>
#   endif
#   if !defined (MPSI_TYP_H)
#include     <mpsi_typ.h>
#   endif
#  endif /*UPGRADE_3G_TDD128*/
# endif /* UPGRADE_3G_MBMS */
#endif /* UPGRADE_3G */

#if !defined (ABRPM_TYP_H)
#include     <abrpm_typ.h>
#endif

/** \defgroup SigAlsi  ALSI Signalling Interface
 * \ingroup Prd3gDmNas
 * \ingroup Sig
 * The USIM manager interface to the application layer through the ALSI (Application Layer Interface),
#if defined (UPGRADE_LTE)
 * interface with layer 3 UMM and EMM through the SIM
#else
 * interface with layer 3 UMM through the SIM.
#endif
 * The ALSI interface is based on the files held on the SIM and algorithms and procedures executed
 * within the SIM/UICC and at the UICC/ME interface as defined in the Specification TS 102 221 and TS 31.102.
 * The 3G SIM card is referred in the specification as UICC. (ICC stands for Integrated Circuit card).
 * One or several USIM applications can reside on one single UICC.
 * Only one USIM application can be active at a time.
 * Some other applications (like WIM) might be active at the same time as the USIM application,
 * they should use a different logical channel.
 * It is a requirement that the USIM manager should support both 3G UICC cards and
 * Phase 2/Phase 2+ SIMs.
 *
 * <B> The context of the ALSI interface is illustrated in the following diagram.</B>
 *
 * \image html USIM_Manager.png
 * @{
 */


/****************************************************************************
 * Macros
 ****************************************************************************/
/**
** This defines the maximum value that the length field of an address (dial
** string) can have minus the TON/NPI byte (In ADN, FDN, LND, MSISDN & SMS
** files).
*/
#define     SIM_MAX_ADDRESS_VALUE_SIZE     10
/** For a summary the dial string from the ADN file only is used
 * no extension data is returned.
 */
#define     SIM_MAX_SUMMARY_DIALNUM_LENGTH SIM_MAX_ADDRESS_VALUE_SIZE

/** The SIM CHV is an 8 digit numeric */
#define     SIM_CHV_LENGTH                  8
/* first 3 letters of country name (03.40) */
/** Maximum FDD frequencies to be stored in the SIM */
#define     SIM_MAX_STORED_FDD_FREQUENCIES  4

#if defined (UPGRADE_3G_TDD128)
/** Maximum TDD128 frequencies to be stored in the SIM */
#define     SIM_MAX_STORED_TDD_FREQUENCIES  4
#endif

/** Maximum number of the application PINs */
#define     MAX_NUM_PIN_APP                 8
/** The SIM Personal Identification Number (SIM PIN) is an 8 digit numeric */
#define     SIM_PIN_LENGTH                  SIM_CHV_LENGTH
/** Specifies how many PIN entry errors in successive calls subscribers can
 * make before they are locked out. */
#define     MAX_PIN_ATTEMPTS                3
/** Maximum number of unsuccessful unblock attempts. Once the
 * maximum number of unsuccessful unblock attempts is reached the card will be permanently
 * blocked
 */
#define     MAX_UNBLOCK_PIN_ATTEMPTS       10
/** Maximum number of key references for security conditions  */
#define     MAX_NUM_KEY_REF                 2

/** The length of currency code 3bytes */
#define     CURRENCY_CODE_SIZE              3
/** The length of cell broadcast message identifier list in bytes */
#define     SIM_CBMI_LIST_SIZE             10
/** The length of ICC Identification */
#define     SIM_ICCID_LEN                  10
/** Maximum search pattern length */
#define     SIM_MAX_PATT_LENGTH            255  /* in 3G, the search pattern might be much longer*/
/** The size of PLMN list */
#define     SIM_PLMN_LIST_SIZE              8
/** The size of SPDI list */
#define     SIM_SPDI_LIST_SIZE             20
/** The number of PLMNs in a SIM */
#define     SIM_NUM_FPLMNS                  4

/** The size of ALPHA list */
#define     SIM_ALPHA_LIST_SIZE            10
#define     SIM_ALPHA_ID_SIZE              50 /**< max alpha id length for adn,fdn etc.. */
#define     SIM_MAX_GID_SIZE              255 /**< Could be up to 255 */
/** The maximum size of Service Provider Name */
#define     SIM_MAX_SPN_SIZE               16
/** The number of enhanced support features */
#define     SIM_NUM_ENH_SUPPORT_FEATURES    4

/** Maximum number of free SMS records that can be reported by the
** AlsiGetFreeSmReq.
*/
#define     SIM_MAX_FREE_SM_LIST          (255)
/** Bitmap size for status record information */
#define     SIM_SMS_BIT_MAP_SIZE          16


/* the following defines are required for Phase 2+ SIM implementation */
/*  these values need to be checked... */
/** The length of CBMID list in bytes */
#define     SIM_CBMID_LIST_SIZE            10    /* same as for CBMI size */
/** The length of CBMIR list in bytes */
#define     SIM_CBMIR_LIST_SIZE            10    /* same as for CBMI size */
/** The length of CNL list in bytes */
#define     SIM_CNL_LIST_SIZE              10    /* is this enough */
/** Max number of digits in a Depersonalisation Control Key  */
#define     SIM_MAX_NUM_DCK_DIGITS          8
/** Maximum number of Emergency Call Codes on  a 2G SIM */
#define     SIM_MAX_ECC_NUMS                5
#define     SIM_MAX_NUM_ECC_DIGITS          6    /**< max number of digits in an ECC */

#define     SIM_MAX_UICC_ECCS               254  /**< maximum number that could be on USIM app */
/* currently 139 langugages: ISO 639 (1988).  provision for more. Do we allow for all languages */
#define     SIM_MAX_ELP_LIST_SIZE           10    /**< set same as for LP */
#define     SIM_MAX_VOICE_SERVICE_GID_SIZE  50    /**< specified in GSM 11.11 */
/** The maximum number of SMSR records */
#define     SIM_MAX_NUM_SMSR_RECORDS        255

/** The size of SMSR list */
#define     SIM_SMSR_LIST_SIZE              10

#if defined (FEA_TEMP_OBSOLETE_SIM_MANAGER)
/* CPHS specific definitions */
#define    SIM_CPHS_MAX_OP_NAME_SIZE        20  /**< op name - long format */
#define    SIM_CPHS_MAX_OP_NAME_SHORT_SIZE  10  /**< op name - short format */
/** Maximum Customer Service Profile (CSP) entries in CPHS file */
#define    SIM_CPHS_MAX_CSP_ENTRIES         20
/** Maximum CSP information numbers in CPHS file */
#define    SIM_CPHS_MAX_INFONUM_LIST_SIZE   10
/** Maximum CSP information numbers alpha ID */
#define    SIM_CPHS_MAX_INFONUM_ALPHAID     20
/** Maximum CSP information numbers dial string */
#define    SIM_CPHS_MAX_INFONUM_DIALSTR     20
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */

/** Maximum MMS time length on the SIM/UIC */
 #define   MAX_MMSUP_SIM_TIME_LENGTH        10
/** Maximum MMS profile name on the SIM/UIC */
 #define   MAX_MMSUP_PROFILE_NAME           100

/** Position for the status request state in SMS status field*/
#define    SIM_REC_SR_STATE_DEC             3
/** Position for the message reference in a SMS-SUBMIT TPDU*/
#define    SIM_REC_MESSAGE_REF_BYTE         1
/** Bit mask to get the record status for a SMS*/
#define    SIM_REC_STATUS_MASK              0x07

# if defined (SIM_EMULATION_ON)
# define    SIM_MAX_EMU_FILE_WRITE_DATA_LENGTH  255
# endif /* SIM_EMULATION_ON */

/* Maximum DF name size */
#define    SIM_MAX_DF_NAME_SIZE             16
/** The length of Location information */
#define    SIM_LOCI_LEN                     11
/** The length of EPS Location Information */
#define    SIM_EPS_LOCI_LEN                 18

/** moved here as used in data services object related code */
void SimGetAdnAlphaIdLength (   Int8                     *data,
                                Int8                     *length );


/****************************************************************************
 * Types
 ****************************************************************************/
/** This enumerated defines the specific type (SMS TPDU)
 * Six types of short message may be held in an SMS record.
 * WARNING : SOME SMS TPDU TYPE HAVE THE SAME MTI VALUE
 * SO THESE VALUE AREN'T MTI VALUE!
 * The bit 2 and 3 are the mti value and the bit 1
 * is used to differentiate the types
 */
typedef enum SimSmTpduTypeTag
{
    /** DELIVER type SMS TPDU ( MTI = 00)*/
    SIM_SMTPDU_DELIVER          = 0,
    /** DELIVER-REPORT type SMS TPDU ( MTI = 00)*/
    SIM_SMTPDU_DELIVER_REPORT   = 1,
    /** SUBMIT type SMS TPDU ( MTI = 01)*/
    SIM_SMTPDU_SUBMIT           = 2,
    /** SUBMIT-REPORT type SMS TPDU ( MTI = 01)*/
    SIM_SMTPDU_SUBMIT_REPORT    = 3,
    /** conveying a command from the MS to the SC ( MTI = 10) */
    SIM_SMTPDU_COMMAND          = 4,
    /** status report from the SC to the MS ( MTI = 10) */
    SIM_SMTPDU_STATUS_REPORT    = 5    /*job 104228*/
}
SimSmTpduType;

/** Removed cause is included as part of the removed indication signal.*/
typedef enum SimRemovedCauseTag
{
    SIM_REMOVED,                    /**< SIM physically rempved */
    SIM_REMOVED_UNDETECTED,         /**< SIM not detetced */
    SIM_REMOVED_INVALID_STATUS_RSP  /**< Invalid response to STATUS command */
}
SimRemovedCause;

/** The status data is decoded and this enumerated defines the outcome.
 * Whenever a SELECT command is executed on an EF, the associated
 * response data contains generic file status data (e.g. file size, file type etc.).
 */
typedef enum SimEfStatusTag
{
    SIM_EFS_OK,                 /**< The data was decoded successfully */
    SIM_EFS_INVALID,            /**< The data does not contain EF status */
    SIM_EFS_UNKNOWN_STRUCTURE,  /**< The structure is not T, LF or C */
    SIM_EFS_UNEXPECTED_TYPE,    /**< Type EF was expected but not returned */
    SIM_EFS_INVALID_STAT_LENGTH /**< The status data has an invalid length */
}
SimEfStatus;

/** Determines the outcome of the request.
 * For every request (AlsxxReq) there exists an associated confirmation (AlsixxCnf).
 * All confirmations contain a parameter indicating the outcome of the request.
 * The specific errors that may be returned as a result of SIM failure or a
 * communications failure are translated into the following general set.
 */
typedef enum SimRequestStatusTag
{
    SIM_REQ_OK,                     /**< request successful                   */
    SIM_REQ_INVALID_PARAMS,         /**< invalid parameters/data in request signal */
    SIM_REQ_MEMORY_PROBLEM,         /**< SIM internal memory problem.
                                     * Could also be that data we are trying
                                     * to write onto the SIM is too big */
    SIM_REQ_FILE_NOT_FOUND,         /**< file does not exist                  */
    SIM_REQ_SERVICE_NOT_AVAILABLE,  /**< Service not allocated or activated   */
    SIM_REQ_SIM_GENERAL_FAULT,      /**< general sim fault                    */
    SIM_REQ_ACCESS_DENIED,          /**< file access not permitted            */
    SIM_REQ_FILE_INVALIDATED,       /**< file is invalidated                  */
    SIM_REQ_ALLOC_ERROR,            /**< no memory available to USIM Manager   */
    SIM_REQ_SM_FAULT,               /**< Sim manager internal problem         */
    SIM_REQ_POWERING_DOWN,          /* job 100935*/ /**< powering down */
    SIM_REQ_INVALID_SIM_DATA,       /**< Data coming from the SIM could not be decoded/ is invalid */
    SIM_REQ_COMMAND_NOT_ALLOWED,    /**< command is not allowed */
    SIM_REQ_PHONEBOOK_NOT_AVAILABLE,/**< phonebook not initialised, so USIM manager cannot process command*/
    SIM_REQ_NO_FREE_RECORD,         /**<  no free record left*/
    SIM_REQ_NO_FREE_EXT_RECORD,     /**< no extension record left*/
    SIM_REQ_NO_FREE_CCP_RECORD,     /**< no CCP record left*/
    SIM_REQ_NO_FREE_CCP_AND_EXT_RECORDS, /**< No free ext record, no CCP free record */
    SIM_REQ_OK_DATA_INVALID,        /**<  the request is OK, but the data is invalid*/
    SIM_REQ_CODE_BLOCKED,           /**<  CHV code blocked                     */
    SIM_REQ_SIMAT_BUSY,             /**<  SIM busy processing STK data         */
    SIM_REQ_SIM_APP_ERROR,          /**<  SIM Application Error                */
    SIM_REQ_SIM_DDL_ERROR,          /**<  SIM SMS-PP Data Download Error       */
    SIM_REQ_INVALID_MAC,            /** MAC failure */
    SIM_REQ_SYNC_FAILURE,           /** synch failure */
    SIM_REQ_2G_CONTEXT_NOT_SUP,     /** 2G security context not supported */
    SIM_REQ_NOT_UPDATED,            /** not updated */
    SIM_REQ_KEY_EXPIRED,            /** key expired */
    SIM_REQ_UNKNOWN_ERROR           /** unknown error */
}
SimRequestStatus;

/** Forms part of the SIM inserted indication signal. */
typedef enum SimInsertStatusTag
{
    /** The ATR procedure has completed successfully and the SIM
     * relates to the defined application (GSM or DCS1800)
     */
    SIM_INSERTED_OK,
    /** A SIM relating to an application other than GSM (or DCS1800)
     * has been inserted.
     */
    SIM_INSERTED_WRONG_SIM,
    /** The ATR procedure failed or a low level communications failure */
    SIM_INSERTED_ERROR,
    /** Indicates that the inserted SIM can only operate at a voltage
     * not supported by the SIM software/hardware
     */
    SIM_INSERTED_UNSUPPORTED_VOLTAGE,
    /** Error with the SIM */
    SIM_INSERTED_USIM_ERROR
}
SimInsertStatus;

/** Initialise status elements */
typedef enum SimInitialiseStatusTag
{
    /** Initialisation procedure was successful.*/
    SIM_INITIALISE_OK,
    /** CHV1 verification failed or UNBLOCK CHV1 procedure failed
     * (All data is invalid)
     */
    SIM_INITIALISE_CHV_VER_FAILED,
    /** The requested CHV function failed and that code is now blocked.*/
    SIM_INITIALISE_CHV_BLOCKED,
    /** The IMSI on the SIM is invalid.*/
    SIM_INITIALISE_IMSI_FAILED,
    /** A communications failure with the SIM. */
    SIM_INITIALISE_FAILED,
    /** Powring down */
    SIM_INITIALISE_POWERING_DOWN,
    /** Fixed number dialling is active*/
    SIM_INITIALISE_CC_ENABLED,
    /** Initialisation failed because FDN is enabled on the SIM but the
     * Application Layer has indicated in the AlsiSimInitialiseReq that
     * it cannot support FDN operation.
     */
    SIM_INITIALISE_CC_FAILED
}
SimInitialiseStatus;


/** Defines the set of PIN functions that may be executed.*/
typedef enum SimUiccPinFunctionTag
{
    /** CHV verification function */
    SIM_PIN_FUNCT_VERIFY = 0,
    /** CHV changing function */
    SIM_PIN_FUNCT_CHANGE =1,
    /** CHV disabling function */
    SIM_PIN_FUNCT_DISABLE = 2,
    /** CHV enabling function */
    SIM_PIN_FUNCT_ENABLE = 3,
    /** CHV unblocking function */
    SIM_PIN_FUNCT_UNBLOCK = 4
}
SimUiccPinFunction;
/** The set of CHV functions that may be executed.*/
typedef SimUiccPinFunction SimChvFunction;

/** When the SIM is performing the authentication algorithm via the RUN GSM ALGORITHM command,
 * the <I> minimum </I> clock frequency condition must be met and this minimum may either be 13/8 MHz
 * or 13/4 MHz (This is handled transparently by the USIM manager).
 */
typedef enum SimAuthFreqTag
{
    /** 13/8 MHz */
    SIM_MIN_AUTH_FREQ_13OVER8 = 0x00,
    /** 13/4 MHz */
    SIM_MIN_AUTH_FREQ_13OVER4 = 0x02
}
SimAuthFreq;

/** Whenever a SELECT command is executed on an MF or DF, or whenever the STATUS command is
 * executed, the associated response data contains generic directory status data.
 * The status data is decoded, this enumerated defines the outcome.
 */
typedef enum SimDirStatusTag
{
    SIM_DIRS_OK,                 /**< The data was decoded successfully */
    SIM_DIRS_INVALID,            /**< The data does not contain MF or DF status */
    SIM_DIRS_UNEXPECTED_TYPE,    /**< Type MF / DF was expected */
    SIM_DIRS_INVALID_STAT_LENGTH /**< The status data has an invalid length */
}
SimDirStatus;

/** When performing a seek command on a currently selected LF, and a record with the
 * defined pattern is found, the record number pertaining to the found record may
 * (type2 only) or may not (type1 only) be returned from the SIM.
 */
typedef enum SimSeekTypeTag
{
    SIM_SEEK_TYPE1,   /**< phase 1 and phase 2 SIMs */
    SIM_SEEK_TYPE2    /**< phase 2 SIMs only */
}
SimSeekType;
/** Search type element */
typedef enum SimUiccSearchTypeTag
{
    /** Simple search */
    SIM_SEARCHTYPE_SIMPLE_SEARCH      = 0x05,
    /** Enhanced search */
    SIM_SEARCHTYPE_ENHANCED_SEARCH    = 0x06,
    /** Proprietary search */
    SIM_SEARCHTYPE_PROPRIETARY_SEARCH = 0x07
}
SimUiccSearchType;
/** Search mode element */
typedef enum SimUiccSearchModeTag
{
    /** from the beginning forwards*/
    SIM_SEEKMODE_START_FORWARD                = 0x00,
    /** from the end backwards */
    SIM_SEEKMODE_START_BACKWARD               = 0x01,
    /** from the next location forwards */
    SIM_SEEKMODE_START_FORWARD_FROM_NEXT      = 0x02,
    /** from the previous location backwards */
    SIM_SEEKMODE_START_BACK_FROM_PREVIOUS     = 0x03,
    /** Start forward search form record indicated in P1 */
    SIM_SEARCHMODE_START_FORWARD_FROM_P1      = 0x04,
    /** Start backward search form record indicated in P1 */
    SIM_SEARCHMODE_START_BACK_FROM_P1         = 0x05,
    /** search begins with the first record in the case of SEEK from the next location forwards */
    SIM_SEARCHMODE_START_FORWARD_FROM_NEXT    = 0x06,
    /** search begins with the last record in the case of SEEK from the previous location backwards.*/
    SIM_SEARCHMODE_START_BACK_FROM_PREVIOUS   = 0x07
}
SimUiccSearchMode;

#if defined (ENABLE_EHPLMN)
/** EHPLMN Presentation Indication Type  */
typedef enum SimUiccEhplmnDisplayPrefTag
{
    SIM_DISPLAY_NO_PREF                    = 0,
    SIM_DISPLAY_HIGHEST_PRIORTY_AVAIL_ONLY = 1,
    SIM_DISPLAY_ALL_AVAIL                  = 2
}SimUiccEhplmnDisplayPref;
#endif

/** This represents the enhanced search indication byte
 * (to be used to perform a SEARCH RECORD command, enhanced search only).
 */
typedef struct SimUiccEnhancedSearchIndicationTag
{
   /** Indicates if offset contains relevant information */
   Boolean                          startFromOffset;
   Int8                             offset;  /**< This is the second byte of the search indication */
} SimUiccEnhancedSearchIndication;

/** Key reference value */
typedef enum SimUiccKeyRefValueTag
{
    /** No pin */
    USIM_ACCESS_NO_PIN        = 0x00,
    /** PIN Appl 1*/
    USIM_ACCESS_PIN1_APP1     = 0x01,
    /** PIN Appl 2*/
    USIM_ACCESS_PIN1_APP2     = 0x02,
    /** PIN Appl 3*/
    USIM_ACCESS_PIN1_APP3     = 0x03,
    /** PIN Appl 4*/
    USIM_ACCESS_PIN1_APP4     = 0x04,
    /** PIN Appl 5*/
    USIM_ACCESS_PIN1_APP5     = 0x05,
    /** PIN Appl 6*/
    USIM_ACCESS_PIN1_APP6     = 0x06,
    /** PIN Appl 7*/
    USIM_ACCESS_PIN1_APP7     = 0x07,
    /** PIN Appl 8*/
    USIM_ACCESS_PIN1_APP8     = 0x08,
    /** Universal PIN */
    USIM_ACCESS_UNIVERSAL_PIN = 0x11,
    /** PIN2 Appl 1*/
    USIM_ACCESS_PIN2_APP1     = 0x81,
    /** PIN2 Appl 2*/
    USIM_ACCESS_PIN2_APP2     = 0x82,
    /** PIN2 Appl 3*/
    USIM_ACCESS_PIN2_APP3     = 0x83,
    /** PIN2 Appl 4*/
    USIM_ACCESS_PIN2_APP4     = 0x84,
    /** PIN2 Appl 5*/
    USIM_ACCESS_PIN2_APP5     = 0x85,
    /** PIN2 Appl 6*/
    USIM_ACCESS_PIN2_APP6     = 0x86,
    /** PIN2 Appl 7*/
    USIM_ACCESS_PIN2_APP7     = 0x87,
    /** PIN2 Appl 8*/
    USIM_ACCESS_PIN2_APP8     = 0x88,
    /** ADM1 */
    USIM_ACCESS_ADM1          = 0x0a,
    /** ADM2 */
    USIM_ACCESS_ADM2          = 0x0b,
    /** ADM3 */
    USIM_ACCESS_ADM3          = 0x0c,
    /** ADM4 */
    USIM_ACCESS_ADM4          = 0x0d,
    /** ADM5 */
    USIM_ACCESS_ADM5          = 0x0e,
    /** ADM6 */
    USIM_ACCESS_ADM6          = 0x8a,
    /** ADM7 */
    USIM_ACCESS_ADM7          = 0x8b,
    /** ADM8 */
    USIM_ACCESS_ADM8          = 0x8c,
    /** ADM9 */
    USIM_ACCESS_ADM9          = 0x8d,
    /** ADM10 */
    USIM_ACCESS_ADM10         = 0x8e
} SimUiccKeyRefValue;

/** Access condition element */
typedef enum SimUiccAccessConditionTag
{
    /** The action can be performed without any restriction */
    USIM_ACCESS_ALWAYS = 0x00,
    /** The action shall only be possible if one of the following three
     * conditions is fulfilled: <UL>
     * <LI> A correct PIN1 value has already been presented to the SIM during the current session
     * <LI> The PIN1 enabled/disabled indicator is set to disabled
     * <LI> UNBLOCK PIN1 has been successfully performed during the current session
     * </UL>
     */
    USIM_ACCESS_PIN    = 0x01,
    /** The action shall only be possible if one of the following two conditions is fulfilled: <UL>
     * <LI> A correct CHV2 value has already been presented to the SIM during the current session
     * <LI> UNBLOCK CHV2 has been successfully performed during the current session
     * </UL>
     */
    USIM_ACCESS_PIN2   = 0x02,
    /** Access condition RFU */
    USIM_ACCESS_RFU    = 0x03,
    /** Allocation of these levels and the respective requirements for their fulfilment are the
     * responsibility of the appropriate administrative authority
     */
    USIM_ACCESS_ADM    = 0x04,
    /** The action cannot be performed over the SIM/ME interface. The SIM may perform the
     * action internally.
     */
    USIM_ACCESS_NEVER  = 0xff
}
SimUiccAccessCondition;

/** This enumerated type defines all possible access conditions*/
typedef enum SimAccessConditionTag
{
    /** The command may always be applied */
    SIM_ACCESS_ALWAYS,
    /** The command may only be applied if: <UL>
     * <LI> A correct CHV1 has been presented in the current session.
     * <LI> CHV1 is disabled (as defined in the directory status data).
     * <LI> CHV1 has been unblocked in the current session.
     * </UL>
     */
    SIM_ACCESS_CHV1,
    /** The command may only be applied if: <UL>
     * <LI> A correct CHV2 has been presented in the current session.
     * <LI> CHV2 has been unblocked in the current session.
     * </UL>
     */
    SIM_ACCESS_CHV2,
    /** Access condition RFU */
    SIM_ACCESS_RFU,
    /** Access condition to an EF which is under the control of the authority which creates this file. */
    SIM_ACCESS_ADMIN_4,
    /** Access condition to an EF which is under the control of the authority which creates this file. */
    SIM_ACCESS_ADMIN_5,
    /** Access condition to an EF which is under the control of the authority which creates this file. */
    SIM_ACCESS_ADMIN_6,
    /** Access condition to an EF which is under the control of the authority which creates this file. */
    SIM_ACCESS_ADMIN_7,
    /** Access condition to an EF which is under the control of the authority which creates this file. */
    SIM_ACCESS_ADMIN_8,
    /** Access condition to an EF which is under the control of the authority which creates this file. */
    SIM_ACCESS_ADMIN_9,
    /** Access condition to an EF which is under the control of the authority which creates this file. */
    SIM_ACCESS_ADMIN_A,
    /** Access condition to an EF which is under the control of the authority which creates this file. */
    SIM_ACCESS_ADMIN_B,
    /** Access condition to an EF which is under the control of the authority which creates this file. */
    SIM_ACCESS_ADMIN_C,
    /** Access condition to an EF which is under the control of the authority which creates this file. */
    SIM_ACCESS_ADMIN_D,
    /** Access condition to an EF which is under the control of the authority which creates this file. */
    SIM_ACCESS_ADMIN_E,
    /** The MS cannot apply the command. */
    SIM_ACCESS_NEVER
}
SimAccessCondition;



/** For CHV related functions, this enum defines the particular CHV (CHV1 or CHV2).*/
typedef enum SimChvNumTag
{
    /** Card holder verification 1 */
    SIM_CHV_1                   =   1,
    /** Card holder verification 2 */
    SIM_CHV_2                   =   2
}
SimChvNum;

/** Mailbox info group type  */
typedef enum SimMailboxInfoGroupTag
{
    /** Voicemail */
    MB_VOICEMAIL,
    /** Fax */
    MB_FAX,
    /** Electronic Mail */
    MB_EMAIL,
    /** Other */
    MB_OTHER,
    /** Videomail  */
#if defined (UPGRADE_REL6) || defined(UPGRADE_TWGGE_NRW)
    MB_VIDEOMAIL,
#endif
    /** number of mailbox info groups */
    SIM_NUM_MAILBOX_INFO_GROUPS
}
SimMailBoxInfoGroup;

/** Message waiting indication group */
typedef struct SimMBMsgWaitingInfoTag
{
    /** Indicates if message waiting indication is active */
    Boolean      msgWaitingIndActive;
    /** The number of messages waiting */
    Int8         numMsgsWaiting;
}
SimMBMsgWaitingInfo;

/** Mailbox Information  */
typedef struct SimMailboxInfoTag
{
  /** The number of messages waiting */
  Int8         mbdnRecord;
}
SimMailboxInfo;

/** This enumerated defines the ids of the directories held on the SIM. */
typedef enum SimDirIdTag
{
    /** MF */
    SIM_DIR_MF                  =   0x3f00,
    /** Contains the applications for MExE */
    SIM_DIR_DF_MEXE             =   0x5f3c, /* job 101859 */
    /** Contains the applications for GRAPHICS */
    SIM_DIR_DF_GRAPHICS         =   0x5f50,
    /** Contains the applications for GSM */
    SIM_DIR_DF_GSM              =   0x7f20,
    /** Contains the applications for DCS 1800*/
    SIM_DIR_DF_DCS1800          =   0x7f21,
    /** Contains telecom service features */
    SIM_DIR_DF_TELECOM          =   0x7f10,
    /** job 107841 added directories for additional files for enhanced network selection */
    SIM_DIR_DF_ENS              =   0x7f66,
    /** Contains the applications for IRIDIUM */
    SIM_DIR_DF_IRIDIUM          =   0x5f30,
    /** Contains the applications for ARMED AGENT */
    SIM_DIR_DF_ARMED_AGENT      =   0x5f40,
    /** Viag Interkom Directory */
    SIM_DIR_VI                  =   0x7F43,
    /** Contains the applications for phonebook */
    SIM_DIR_DF_PHONEBOOK           =   0x5f3a,
    /** Contains the applications for GM access */
    SIM_DIR_DF_GSM_ACCESS          =   0x5f3b,
    /** Contains the applications for USIM */
    SIM_DIR_ADF_USIM               =   0x7fff,
    /** Used for initialisation and error conditions. */
    SIM_DIR_INVALID             =   0x0000
}
SimDirId;

#if defined (FEA_TEMP_OBSOLETE_SIM_MANAGER)
/** SIM Homezone EF identifier element */
typedef enum SimHomezoneEfIdTag
{
     /** Homezone parameters */
     SIM_EF_HZ_PARAMETERS = 0x6F60,
     /** Homezone cache 1 */
     SIM_EF_HZ_CACHE1     = 0x6F61,
     /** Homezone cache 2 */
     SIM_EF_HZ_CACHE2     = 0x6F62,
     /** Homezone cache 3 */
     SIM_EF_HZ_CACHE3     = 0x6F63,
     /** Homezone cache 4 */
     SIM_EF_HZ_CACHE4     = 0x6F64
} SimHomezoneEfId;
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */

/** This enumerated defines the ids of the elementary files held on
 * the SIM in the MF, DFGSM, DFDCS1800 and DFTelecom applications
 * (Note: A SIM may hold other elementary files for other applications
 * under different directories.
 */

typedef enum SimEfIdTag
{
    /** Access Rule Reference MF */
    SIM_EF_ARR_MF               =   0x2f06,
    /** Application directory */
    SIM_EF_DIR                  =   0x2f00,
    /** ICC identification */
    SIM_EF_ICCID                =   0x2fe2,
    /** Preferred languages  */
    SIM_EF_PL                   =   0x2f05,
    /** UICC Maximum Power Consumption */
    SIM_EF_UMPC                 =   0x2f08,
    /** Access rule reference */
    SIM_EF_ARR_TELECOM          =   0x6f06,
    /** Image data */
    SIM_EF_IMG                  =   0x4f20,
    /*MeXE files*/
    /** MExE ST */
    SIM_EF_MEXE_ST              =   0x4f40,
    /** Operator Root Public Key */
    SIM_EF_ORPK                 =   0x4f41,
    /** Administrator Root Public Key */
    SIM_EF_ARPK                 =   0x4f42,
    /** Third Party Root Public Key */
    SIM_EF_TPRPK                =   0x4f43,
    /** object directory file*/
    SIM_EF_ODF                  =   0x5031,
    /** LP */
    SIM_EF_LP                   =   0x6f05,
    /** IMSI */
    SIM_EF_IMSI                 =   0x6f07,
    /** GSM Ciphering key Kc */
    SIM_EF_KC                   =   0x6f20,
    /** PLMN selection */
    SIM_EF_PLMN_SEL             =   0x6f30,
    /** Higher Priority PLMN */
    SIM_EF_HPLMN                =   0x6f31,
#if defined (FEA_TEMP_OBSOLETE_SIM_MANAGER)
    /** ACM maximum value */
    SIM_EF_ACM_MAX              =   0x6f37,
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    /** USIM service table */
    SIM_EF_SST                  =   0x6f38,
#if defined (FEA_TEMP_OBSOLETE_SIM_MANAGER)
    /** Accumulated call meter */
    SIM_EF_ACM                  =   0x6f39,
    /** Group identifier level 1 */
    SIM_EF_GID1                 =   0x6f3e,
    /** Group identifier level 2 */
    SIM_EF_GID2                 =   0x6f3f,
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    /** Service provider name */
    SIM_EF_SPN                  =   0x6f46,
#if defined (FEA_TEMP_OBSOLETE_SIM_MANAGER)
    /** Price per Unit and Currency Table */
    SIM_EF_PUCT                 =   0x6f41,
    /** CBMI */
    SIM_EF_CBMI                 =   0x6f45,
    /** BCCH */
    SIM_EF_BCCH                 =   0x6f74,
#endif
    /** Access control class */
    SIM_EF_ACC                  =   0x6f78,
    /** Forbidden PLMNs */
    SIM_EF_FPLMN                =   0x6f7b,
    /** Location information */
    SIM_EF_LOCI                 =   0x6f7e,
    /** Administrative data */
    SIM_EF_AD                   =   0x6fad,
    /** Phase */
    SIM_EF_PHASE                =   0x6fae,
#if defined (FEA_TEMP_OBSOLETE_SIM_MANAGER)
    /** Abbreviated dialling numbers */
    SIM_EF_ADN                  =   0x6f3a,
    /** Fixed dialling numbers */
    SIM_EF_FDN                  =   0x6f3b,
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    /** Short messages */
    SIM_EF_SMS                  =   0x6f3c,
    /** Capability Configuration Parameters */
    SIM_EF_CCP                  =   0x6f3d,
    /** MSISDN storage */
    SIM_EF_MSISDN               =   0x6f40,
    /** SMS parameters */
    SIM_EF_SMSP                 =   0x6f42,
    /** SMS status */
    SIM_EF_SMSS                 =   0x6f43,
    /** LND */
    SIM_EF_LND                  =   0x6f44,
    /** Extension 1 */
    SIM_EF_EXT1                 =   0x6f4a,
#if defined (FEA_TEMP_OBSOLETE_SIM_MANAGER)
    /** Extension 2 */
    SIM_EF_EXT2                 =   0x6f4b,
    /** Extended Capability Configuration Parameter */
    SIM_EF_ECCP                 =   0x6f4f, /*same as EF CCP2*/
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    /** SetUp Menu Elements */
    SIM_EF_SUME                 =   0x6f54,
    /** Group identifier level 1 */
    SIM_EF_CPS                  =   0x6f3e,
#if defined (FEA_TEMP_OBSOLETE_SIM_MANAGER)
    /* new files for Phase 2+ */
    /** Voice Group Call Service */
    SIM_EF_VGCS                 =   0x6fb1,
    /** Voice Group Call Service Status */
    SIM_EF_VGCSS                =   0x6fb2,
    /** Voice Broadcast Service */
    SIM_EF_VBS                  =   0x6fb3,
    /** Voice Broadcast Service Status */
    SIM_EF_VBSS                 =   0x6fb4,
    /** Enhanced Multi Level Pre-emption and Priority */
    SIM_EF_EMLPP                =   0x6fb5,
    /** Automatic Answer for eMLPP Service */
    SIM_EF_AAEM                 =   0x6fb6,
    /** CBMID */
    SIM_EF_CBMID                =   0x6f48,
    /** Emergency Call Codes */
    SIM_EF_ECC                  =   0x6fb7,
    /** CBMIR */
    SIM_EF_CBMIR                =   0x6f50,
    /** De-personalization Control Keys */
    SIM_EF_DCK                  =   0x6f2c,
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    /** Co-operative network list */
    SIM_EF_CNL                  =   0x6f32,
#if defined (FEA_TEMP_OBSOLETE_SIM_MANAGER)
    /** Network Indication of Alerting */
    SIM_EF_NIA                  =   0x6f51,
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    /** Service Dialling Numbers */
    SIM_EF_SDN                  =   0x6f49,
    /** Extension 3 */
    SIM_EF_EXT3                 =   0x6f4c,
#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
    /** Barred dialling numbers */
    SIM_EF_BDN                  =   0x6f4d,
    /** Extension 4 */
    SIM_EF_EXT4                 =   0x6f4e,
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    /** Short message status reports */
    SIM_EF_SMSR                 =   0x6f47,
#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
    /* new files for CPHS */
    /** VMWF */
    SIM_EF_CPHS_VMWF            =   0x6f11,
    /** CFF */
    SIM_EF_CPHS_CFF             =   0x6f13,
    /** CPHS is on */
    SIM_EF_CPHS_ON              =   0x6f14,
    /** Customer Service Profile  */
    SIM_EF_CPHS_CSP             =   0x6f15,
    /** The information of CPHS */
    SIM_EF_CPHS_INFO            =   0x6f16,
    /** CPHS Mailbox Numbers*/
    SIM_EF_CPHS_MN              =   0x6f17,
    /** CPHS ONS */
    SIM_EF_CPHS_ONS             =   0x6f18,
    /** CPHS information number  */
    SIM_EF_CPHS_INFO_NUM        =   0x6f19,
    /** CPHS information number  in older SIM cards. */
    SIM_EF_CPHS_INFO_NUM_OLD    =   0xEA01,
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */

    /*These files have the same ID as the homezone file,
     * but they reside under a different directory.
     */
    /** User controlled PLMN selector with Access Technology */
    SIM_EF_PLMNW_ACT            =   0x6F60,
    /** Operator controlled PLMN selector with Access Technology */
    SIM_EF_OPLMNW_ACT           =   0x6F61,
    /** HPLMN selector with Access Technology */
    SIM_EF_HPLMNW_ACT           =   0x6F62,
    /** COMPACT Packet BCCH */
    SIM_EF_CPBCCH               =   0x6F63,
    /** Investigation Scan */
    SIM_EF_INVSCAN              =   0x6f64,

    /* new files for CPHS */
    /** GPRS Ciphering key KcGPRS */
    SIM_EF_KC_GPRS              =   0x6f52,
    /** GPRS location information */
    SIM_EF_LOCI_GPRS            =   0x6f53,
    /** PLMN Network Name */
    SIM_EF_PNN                  =   0x6fc5,
    /** Operator Network List */
    SIM_EF_OPL                  =   0x6fc6,
    /** Service Provider Display Information */
    SIM_EF_SPDI                 =   0x6fcd,
#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
   /* job 127085 add 2G support for Mailbox dialling numbers and
    * message waiting info */
    /** Mailbox Dialling Numbers */
    SIM_EF_MBDN                 =   0x6fc7,
    /** Extension 6 */
    SIM_EF_EXT6                 =   0x6fc8,
    /** Mailbox Identifier */
    SIM_EF_MBI                  =   0x6fc9,
    /** Message Waiting Indication Status */
    SIM_EF_MWIS                 =   0x6fca,
    /** Call Forwarding Indication Status */
    SIM_EF_CFIS                 =   0x6fcb,
    /** Extension 7 */
    SIM_EF_EXT7                 =   0x6fcc,
    /** MMS Notification  */
    SIM_EF_MMSN                 =   0x6fce,
    /** Extension 8 */
    SIM_EF_EXT8                 =   0x6fcf,
    /** MMS Issuer Connectivity Parameters  */
    SIM_EF_MMSICP               =   0x6fd0,
    /** MMS User Connectivity Parameters */
    SIM_EF_MMSUCP               =   0x6fd2,

    /* job 107841 add Terminal Support Table file    */
    /* and Acting Home PLMN - only used for enhanced */
    /* network selection features                    */
    /** MMS User Preferences */
    SIM_EF_MMSUP                =   0x6fd1,
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    /** TST */
    SIM_EF_TST                  =   0x6fd2,
    /** HPLMN selector with Access Technology */
    SIM_EF_ACT_HPLMN            =   0x4F34,
#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
    /** Phone book reference file */
    SIM_EF_PBR                  =   0x4f30,
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    /** used when a field is uninitialised */
    SIM_EF_INVALID              =   0x0000
}
SimEfId;

/** Files present on a UICC containing a USIM Application */
typedef enum UsimEfIdTag
{
    /** Language indication */
    USIM_EF_LI                   =   0x6f05,
    /** Access rule reference  */
    USIM_EF_ARR                  =   0x6f06,
    /** IMSI */
    USIM_EF_IMSI                 =   0x6f07,
    /** Ciphering and integrity keys */
    USIM_EF_KEYS                 =   0x6f08,
    /** Ciphering and integrity keys for packet switched domain */
    USIM_EF_KEYS_PS              =   0x6f09,
    /** Higher Priority PLMN search period */
    USIM_EF_HPLMN                =   0x6f31,
#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
    /** ACM maximum value */
    USIM_EF_ACM_MAX              =   0x6f37,
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    /** USIM service table */
    USIM_EF_UST                  =   0x6f38,
#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
    /** Accumulated call meter */
    USIM_EF_ACM                  =   0x6f39,
    /** Fixed dialling numbers */
    USIM_EF_FDN                  =   0x6f3b,
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    /** Short messages */
    USIM_EF_SMS                  =   0x6f3c,
#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
    /** Group identifier level 1 */
    USIM_EF_GID1                 =   0x6f3e,
    /** Group identifier level 2 */
    USIM_EF_GID2                 =   0x6f3f,
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    /** MSISDN storage */
    USIM_EF_MSISDN               =   0x6f40,
#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
    /** Price per Unit and Currency Table */
    USIM_EF_PUCT                 =   0x6f41,
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    /** SMS parameters */
    USIM_EF_SMSP                 =   0x6f42,
    /** SMS status */
    USIM_EF_SMSS                 =   0x6f43,
#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
    /** CBMI */
    USIM_EF_CBMI                 =   0x6f45,
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    /** Service provider name */
    USIM_EF_SPN                  =   0x6f46,
    /** Short message status reports */
    USIM_EF_SMSR                 =   0x6f47,
#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
    /** CBMID */
    USIM_EF_CBMID                =   0x6f48,
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    /** Service Dialling Numbers */
    USIM_EF_SDN                  =   0x6f49,
#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
    /** Extension 2 */
    USIM_EF_EXT2                 =   0x6f4b,
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    /** Extension 3 */
    USIM_EF_EXT3                 =   0x6f4c,
#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
    /** Barred dialling numbers */
    USIM_EF_BDN                  =   0x6f4d,
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    /** Extension 5 */
    USIM_EF_EXT5                 =   0x6f4e,
#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
    /** CBMIR */
    USIM_EF_CBMIR                =   0x6f50,
    /** Extension 4 */
    USIM_EF_EXT4                 =   0x6f55,
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    /** Enabled services table */
    USIM_EF_EST                  =   0x6f56,
    /** Access point name control list */
    USIM_EF_ACL                  =   0x6f57,
    /** Comparison method information */
    USIM_EF_CMI                  =   0x6f58,
#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
    /** Initialisation value for Hyperframe number */
    USIM_EF_START_HFN            =   0x6f5b,
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    /** Maximum value of START */
    USIM_EF_THRESHOLD            =   0x6f5c,
    /** Packet switched location information */
    USIM_EF_PS_LOCI              =   0x6f73,
    /** Access control class */
    USIM_EF_ACC                  =   0x6f78,
    /** Forbidden PLMNs */
    USIM_EF_FPLMN                =   0x6f7b,
    /** Location information */
    USIM_EF_LOCI                 =   0x6f7e,
#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
    /** Incoming call information */
    USIM_EF_ICI                 =   0x6f80,
    /** Outgoing call information */
    USIM_EF_OCI                 =   0x6f81,
    /** Incoming call timer */
    USIM_EF_ICT                 =   0x6f82,
    /** Outgoing call timer */
    USIM_EF_OCT                 =   0x6f83,
    /** Key for hidden phone book entries */
    USIM_EF_HIDDENKEY           =   0x6fc3,
    /** Network Parameters */
    USIM_EF_NETPAR              =   0x6fc4,
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    /** PLMN Network Name */
    USIM_EF_PNN                 =   0x6fc5,
    /** Operator Network List */
    USIM_EF_OPL                 =   0x6fc6,
#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
    /** Mailbox Dialling Numbers */
    USIM_EF_MBDN                =   0x6fc7,
    /** Extension 6 */
    USIM_EF_EXT6                =   0x6fc8,
    /** Mailbox Identifier */
    USIM_EF_MBI                 =   0x6fc9,
    /** Message Waiting Indication Status */
    USIM_EF_MWIS                =   0x6fca,
    /** Call Forwarding Indication Status */
    USIM_EF_CFIS                =   0x6fcb,
    /** Extension 7 */
    USIM_EF_EXT7                =   0x6fcc,
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    /** Equivalent Home PLMN */
    USIM_EF_EHPLMN              =   0x6fd9,
    /* Equivalent Home PLMN Presentation Indication  */
    USIM_EF_EHPLMNPI            =   0x6fdB,
#if defined (UPGRADE_REL8_USIM)
    /** EPS Location Information */
    USIM_EF_EPS_LOCI            =   0x6fe3,
    /** NAS Securltiy Context */
    USIM_EF_NAS_SECURITY        =   0x6fe4,
#endif
    /** NAS Configuration Context */
    USIM_EF_NASCONFIG           =   0x6fe8,
    /** Last RPLMN Selection Indication */
    USIM_EF_LRPLMNSI            =   0x6fdc,
/*files under DF MEXE*/
    /** Indicates which MExE services are available. */
    USIM_EF_MEXE_ST             =   0x4f40,
    /** Contains the descriptor(s) of certificates containing
     * the Administrator Root Public Key
     */
    USIM_EF_ARPK                =   0x4f42,
    /** Contains descriptor(s) of certificates containing the
     * Third Party root public key
     */
    USIM_EF_TPRK                =   0x4f43,
    /** Contains the descriptor(s) of certificates containing
     * the Operator Root Public Key
     */
    USIM_EF_ORPK                =   0x4f41,
/*files under DF GSM ACCESS */
    /** GSM Ciphering key Kc */
    USIM_EF_KC                  =   0x4f20,
    /** GPRS Ciphring key KcGPRS */
    USIM_EF_KC_GPRS             =   0x4f52,
    /** CPBCCH Information */
    USIM_EF_CPBCCH              =   0x4f63,
    /** Investigation Scan */
    USIM_EF_INVSCAN             =   0x4f64,
#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
/*  phonebook: can be under ADF usim or under DF telecom */
    /** Phone book synchronisation counter */
    USIM_EF_PSC                  =   0x4f22,
    /** Change counter */
    USIM_EF_CC                   =   0x4f23,
    /** Previous unique identifier */
    USIM_EF_PUID                 =   0x4f24,
    /** Phone book reference file */
    USIM_EF_PBR                  =   0x4f30,
    /** Capability configuration parameters 1 */
    USIM_EF_CCP1                 =   0x4f3d,
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    /** Capability configuration parameters 2 */
    USIM_EF_CCP2                 =   0x6f4f,
#if defined (UPGRADE_3G)
# if defined (UPGRADE_3G_MBMS)
# if defined (UPGRADE_3G_TDD128)
    USIM_EF_GBABP                =   0x6fd6,
    USIM_EF_MSK                  =   0x6fd7,
    USIM_EF_MUK                  =   0x6fd8,
    USIM_EF_GBANL                =   0x6fda,
#  endif /* UPGRADE_3G_TDD128 */
# endif /* UPGRADE_3G_MBMS */
#endif /* UPGRADE_3G */
    USIM_EF_NCP_IP               =   0x6fe2,
#if defined (FEA_RPM)
    USIM_EF_RPM_ENABLED          =   0x4f40,
    USIM_EF_RPM_PARAMETERS       =   0x4f41,
    USIM_EF_RPM_LEAK_RATE        =   0x4f42,
    USIM_EF_RPM_COUNTERS         =   0x4f43,
    USIM_EF_RPM_VERSION          =   0x4f44,
#endif /* defined (FEA_RPM) */
/* files that can only be found under ADF USIM dir */
    /** used when a field is uninitialised */
    USIM_EF_INVALID              =   0x0000
}
UsimEfId;


/** Defines the status of a short message record held in the SMS file.*/
typedef enum SimSmRecordStatusTag
{
    /** Free SIM record number */
    SIM_SMREC_FREE                =   0,
    /** Message received by UE from network; message read */
    SIM_SMREC_RECEIVED_READ       =   1,
    /** Message received by UE from network; message to be read */
    SIM_SMREC_RECEIVED_UNREAD     =   3,
    /** UE originating message; message sent to the networkt */
    SIM_SMREC_ORIGINATED_SENT     =   5,
    /** UE originating message; message to be sent */
    SIM_SMREC_ORIGINATED_NOT_SENT =   7,
    /** added for 0211-21790 */
    SIM_SMREC_ORIGINATED          =   0xfe,
    /** Invalid status */
    SIM_SMREC_INVALID_STATUS      =   0xff
}
SimSmRecordStatus;

/** Defines the available state for the status report field*/
typedef enum SimSmRecordSrStatTag
{
    /** Status report not requested*/
    SIM_SMREC_SR_NOT_REQUESTED    =   0,
    /** Status report requested but not (yet) received;*/
    SIM_SMREC_SR_REQUESTED        =   1,
    /** Status report requested, received but not stored in EF-SMSR*/
    SIM_SMREC_SR_NOT_STORED       =   2,
    /** Status report requested, received and stored in EF-SMSR*/
    SIM_SMREC_SR_STORED           =   3
}
SimSmRecordSrStat;

/** When handling abbreviated dial number commands, this command specifies the
 * associated elementary file (ADN, FDN, LND, MSISDN).
 */
typedef enum SimDialNumberFileTag
{
    /** Dial number file for ADN */
    SIM_DIAL_ADN                =   0x6f3a,
    /** Dial number file for FDN */
    SIM_DIAL_FDN                =   0x6f3b,
    /** Dial number file for LND */
    SIM_DIAL_LND                =   0x6f44,
    /** Dial number file for MSISDN */
    SIM_DIAL_MSISDN             =   0x6f40,
    /* new dial number files for Phase 2+ */
    /** Dial number file for SDN */
    SIM_DIAL_SDN                =   0x6f49,
    /** Dial number file for BDN */
    SIM_DIAL_BDN                =   0x6f4d,
    /** Dial number file for CPHS Mailbox Numbers */
    SIM_DIAL_CPHS_MN            =   0x6f17,
    /** Dial number file for OCI */
    SIM_DIAL_OCI                =   0x6f81,
    /** Dial number file for OCI */
    SIM_DIAL_ICI                =   0x6f80,
    /** Dial number file for MBDN */
    SIM_DIAL_MBDN               =   0x6fc7,
    /** Unvalid dial number*/
    SIM_DIAL_INVALID            =   0xFFFF
}
SimDialNumberFile;

/** Defines the valid SIM Phases. */
typedef enum SimPhaseTag
{
    /** Phase 1 */
    SIM_PHASE_1                 =   0,
    /** Phase 2 identifier */
    SIM_PHASE_2                 =   2,
    /** Phase 2+ */
    SIM_PHASE_2P                =   3,
    /** Phase 3G */
    SIM_PHASE_3G                =   4,
    /** SIM phase unvalid */
    SIM_PHASE_UNKNOWN
}
SimPhase;

/** CPHS Phase identifier */
typedef enum SimCphsPhaseTag
{
    /** CPHS Phase 1 */
    SIM_CPHS_PHASE_1            =   1,
    /** CPHS Phase 2 */
    SIM_CPHS_PHASE_2            =   2
}
SimCphsPhase;

/** Specifies file type. File types returned from the SIM could be subject
 * to SIM internal or transmission errors. Unknown or uninitialised values
 * will be reflected by the SIM_FILETYPE_UNKNOWN value.
 */
typedef enum SimFileTypeTag
{
    /** Master file directory */
    SIM_FILETYPE_MF             =   1,
    /** Dedicated file directory/sub-directory */
    SIM_FILETYPE_DF             =   2,
    /** Elementary file */
    SIM_FILETYPE_EF             =   4,
    /** Application dedicated file */
    SIM_FILETYPE_ADF            =   5,
    /** Unknown file */
    SIM_FILETYPE_UNKNOWN        =   0xff
}
SimFileType;

/** When the SIM is temporarily not required, the clock may be turned off as defined
 * in the file characteristics of the current MF or DF.
 * The clock may or may not be turned off. If it is permitted, the level at which it
 * may be stopped is defined as being either high or low. The defined level is either
 * a preferred level or a necessary level. This is handled transparently by the USIM manager.
 */

typedef enum SimClockStopModeTag
{
    /** Clock stop allowed, no preferred level */
    SIM_CLKSTOP_ALLOWED         =   0x01,
    /** Clock stop allowed, high level preferred */
    SIM_CLKSTOP_HIGH_PREFERED   =   0x05,
    /** Clock stop allowed, low level preferred */
    SIM_CLKSTOP_LOW_PREFERED    =   0x09,
    /** Clock stop not allowed */
    SIM_CLKSTOP_NOT_ALLOWED     =   0x00,
    /** clock stop not allowed, unless at high level */
    SIM_CLKSTOP_HIGH_ONLY       =   0x04,
    /** Clock stop not allowed, unless at low level */
    SIM_CLKSTOP_LOW_ONLY        =   0x08
}
SimClockStopMode;

/** This enumeration is used to identify the SIM cards voltage capability
 * as indicated by the GSM Directory data.
 */
typedef enum SimVoltageCapabTag
{
    /** supporting 5V See GSM 11.18, section 4.3 */
    SIM_5_VOLTS_CAPAB           =   0x00,
    /** supporting both 3V and 5V */
    SIM_3_OR_5_VOLTS_CAPAB      =   0x10,
    /** supporting both 1.8V and 3V */
    SIM_1_8_OR_3_VOLTS_CAPAB    =   0x30, /* May also work at 5V */
    /** For future class */
    SIM_FUTURE_CLASS            =   0x70,
    /** Unknown class */
    SIM_UNKNOWN_CLASS           =   0xff
}
SimVoltageCapab;

/** When reading or updating records from/to a linear fixed (LF)
 * or cyclic elementary file, The modes defined in this structure
 * are permitted
 */
typedef enum SimRecordReadWriteModeTag
{
    /** The current record is read/updated (record number = 0) or a
     * defined record is read/updated ( 1 <= record number <= num records in file).
     */
    SIM_REC_RWMODE_CURRENT_ABSOLUTE =   0x04,
    /** The next record is read/updated and the record pointer
     * updated. If the record pointer is undefined, the first
     * record is read/updated. No data is read/updated if the
     * current record is the last record (LF files only).
     * The first record is read/updated if the current record is
     * the last record (C files only).
     */
    SIM_REC_RWMODE_NEXT             =   0x02,
    /** The previous record is read/updated and the record pointer
     * updated. If the record pointer is undefined, the last record
     * is read/updated. No data is read/updated if the current record
     * is the first record (LF files only). The last record is read/updated
     * if the current record is the first record (C files only).
     */
    SIM_REC_RWMODE_PREVIOUS         =   0x03
}
SimRecordReadWriteMode;

/** When registered on the HPLMN or on a PLMN present in the SPDI list,
 * the SPN should always be displayed.
 * The display of the registered PLMN depends upon the condition of this enumerator.
 * See 51.011, section 10.3.11 and 22.101, annex A.4, service provider name indication.
 */

typedef enum SimSpnDisplayCondTag
{
    /** display of the registered PLMN is not required when
     * the registered PLMN is either the HPLMN or a PLMN
     * listed in SPDI list*/
    SIM_SPN_DONT_DISPLAY_PLMN,
    /** display of the registered PLMN is required when
     * the registered PLMN is either the HPLMN or a PLMN
     * listed in the SPDI list
     */
    SIM_SPN_DISPLAY_PLMN
}
SimSpnDisplayCond;

/**Indicates whether the SPN should be displayed
 * See 51.011, section 10.3.11 and 22.101, annex A.4, service provider name indication.
 * When registered on neither the HPLMN nor one of the PLMNs present in the SPDI list,
 * the registered PLMN should always be displayed.
 * The display of the SPN depends upon the condition 2 below
*/
typedef enum SimSpnDisplayCond2Tag
{
    /** display of the SPN is required when the
     * Registered PLMN is neither the HPLMN
     * nor a PLMN listed in the SPDI list*/
    SIM_SPN_DISPLAY_SPN,
    /** display of the SPN is not required when
     * the Registered PLMN is neither the HPLMN
     * nor a PLMN listed in the SPDI list*/
    SIM_SPN_DONT_DISPLAY_SPN
}
SimSpnDisplayCond2;

/** A GSM SIM CHV status element.
 * The SIM has four CHV values: <UL>
 * <LI> CHV1
 * <LI> UNBLOCK CHV1
 * <LI> CHV2
 * <LI> UNBLOCK CHV2
 * </UL> When a CHV validation procedure fails, a counter is decremented within the SIM.
 * Once this counter reaches zero the particular CHV becomes blocked.
 * This structure holds the remaining retries that may be applied.
 * In addition this structure holds the initialisation status of the particular CHV.
 */
typedef struct SimChvStatusTag
{
    /** Remaining retries that may be applied */
    Int8                numRemainingRetrys;
    /** Initialisation status */
    Boolean             secretCodeInitialised;
    /** Enabled or not */
    Boolean             enabled;
    Boolean             verified;                   /**< Has PIN been verified */
    Boolean             used;                       /**< usage qualifier specifies whether the PIN should be used or not */
    SimUiccKeyRefValue  alternativeKeyReference;    /**< alternative key reference */
}
SimChvStatus;

/** Stucture containing status information associated with a UICC PIN */
typedef SimChvStatus    SimUiccPinStatus;

/** This structure contains directory data specific to the GSM application.
 * The data forms part of the SimDirStatusData structure
 */
typedef struct SimGsmSpecificDataTag
{
    /** If the field gsmDataValid is FALSE, then the other fields should not be read.*/
    Boolean             gsmDataValid;
    /** Indicates stooping clock mode */
    SimClockStopMode    clockMode;
    /** Indicates the authentication minimum clock frequency */
    SimAuthFreq         authenticateMinFreq;
    /** Indicates the number of the children of DF */
    Int8                numChildDfs;
    /** Indicates the number of the children of EF */
    Int8                numChildEfs;
    /** Indicates if CHV 1 is enabled */
    Boolean             chv1Enabled;
    Int8                numChvsEtc;
    /** Indicates the status of CHV 1*/
    SimChvStatus        chv1Status;
    /** Indicates the status of UNBLOCK CHV1 */
    SimChvStatus        unblockChv1Status;
    /** Indicates the status of CHV 2*/
    SimChvStatus        chv2Status;
    /** Indicates the status of UNBLOCK CHV1 */
    SimChvStatus        unblockChv2Status;
    /** Indicates SIM cards voltage capability */
    SimVoltageCapab     simVoltageCapab;
    /** Indicates free space in the directory */
    Int16               dirFreeSpace;
}
SimGsmSpecificData;

/** Enumerated type indicating the state of the UICC.*/
typedef enum SimUiccStateTag
{
  /** Info state */
  SIM_UICC_NO_INFO_STATE     = 0x00,
  /** Creation state */
  SIM_UICC_CREATION_STATE    = 0x01,
  /** Initial state */
  SIM_UICC_INIT_STATE        = 0x03,
  /** Deactivated state */
  SIM_UICC_DEACTIVATED_STATE = 0x04,
  /** Activated state */
  SIM_UICC_ACTIVATED_STATE   = 0x05,
  /** Terminated state */
  SIM_UICC_TERMINATED_STATE = 0x0c
} SimUiccState;

/** This defines the space under a directory */
typedef struct SimUiccDirFreeSpaceTag
{
   /** Indicates the length */
   Int8 length;
   Int8 data[MAX_DIR_FREE_SPACE];
}  SimUiccDirFreeSpace;

/** Holds some information that relates to the status of a directory */
typedef struct SimUsimSpecificDataTag
{
    /** The application PIN used */
    SimUiccKeyRefValue              applicationPin;
    SimUiccPinStatus                pinStatus;     /**< status of the application PIN*/
    /** The key reference of the local PIN used in this directory. */
    SimUiccKeyRefValue              localPin;
    SimUiccPinStatus                pin2Status;    /**< status of the PIN2 (local PIN) */
    SimUiccPinStatus                unblockPin2Status;  /**< status of the UNBLOCK PIN2 (local PIN) */
    /** The state of the directory */
    SimUiccState                    fileState;
    /** The free space under a directory*/
    SimUiccDirFreeSpace             dirFreeSpace;
}
SimUsimSpecificData;

/** This union holds some information that relates to the status of a directory.
 * If a GSM SIM is inserted inside the 3G mobile, then gsmSpecificData is used.
 * Otherwise if a UICC is inserted inside the mobile, usimSpecificData is used.
 */
typedef union SimUiccSpecificDataTag
{
   /** GSM specific data about a directory */
   SimGsmSpecificData    gsmSpecificData;
   /** Holds some information that relates to the status of a directory */
   SimUsimSpecificData   usimSpecificData;

}  SimUiccSpecificData;

/** Specifies the type of data (2G/3G)*/
typedef enum SimUiccTypeOfDataTag
{
    /** 2G data */
    SIM_UICC_GSM_DATA,
    /** 3G data */
    SIM_UICC_USIM_DATA
} SimUiccTypeOfData;

/** This structure conveys the MMS Profile Name data.  */
typedef struct SimMmsupProfileNameTag
{
  /** Contains profile name data */
  Int8 data[MAX_MMSUP_PROFILE_NAME];
  /** Indicates the length of data */
  Int8 length;
} SimMmsupProfileName;

/** MMS User Preference priority */
typedef enum SimMmsupPriorityTag
{
    /** Low priority */
    SIM_PRIORITY_LOW = 0,
    /** Normal priority */
    SIM_PRIORITY_NORMAL,
    /** High priority */
    SIM_PRIORITY_HIGH,
    /** Priority unspecified */
    SIM_PRIORITY_UNSPECIFIED
} SimMmsupPriority;

/** Enumerated Type to define the MMS Visibility User Preference */
typedef enum SimMmsupVisibilityTag
{
  /** Hide */
  SIM_VISIBILITY_HIDE = 0,
  /** Show */
  SIM_VISIBILITY_SHOW,
  /** Unknown */
  SIM_VISIBILITY_UNSPECIFIED
} SimMmsupVisibility;

/** Enumerated Type to define the MMS ReadReply User Preference */
typedef enum SimMmsupReadReplyTag
{
  /** Read-Reply is supported */
  SIM_READ_REPLY_YES = 0,
  /** Read-Reply is not supported */
  SIM_READ_REPLY_NO,
  /** Read-Reply is unspecified  */
  SIM_READ_REPLY_UNSPECIFIED
} SimMmsupReadReply;

/** Enumerated Type to define the MMS Delivery report User Preference */
typedef enum SimMmsupDeliveryReportTag
{
  /** Delivery report is supported */
  SIM_DELIVERY_REPORT_YES = 0,
  /** Delivery report is not supported */
  SIM_DELIVERY_REPORT_NO,
  /** Delivery report is unspecified */
  SIM_DELIVERY_REPORT_UNSPECIFIED
} SimMmsupDeliveryReport;

/** Type definition for the MMS TimeUser preference on the SIM/UICC */
typedef struct SimMmsupTimeTag
{
  /** If set to TRUE, value is absolute, otherwise relative*/
  Boolean absolute;
  /** length of data in array below*/
  Int8    length;
  /** If Absolute is set to TRUE, the array will contain the date-value and
   * if it is set to FALSE, it will contain the delta-seconds-value
   * (See MMS encapsulation protocol specification, section 7.2.5, and 7.2.8) */
  Int8    timeValue[MAX_MMSUP_SIM_TIME_LENGTH];
} SimMmsupTime;

/** MMS user preferences element
 * (Currently, only the WAP implementation is supported).
 */
typedef struct SimMmsupTag
{
  /** The MMS Delivery report User Preference */
  SimMmsupDeliveryReport deliveryReport;
  /** The MMS ReadReply User Preference*/
  SimMmsupReadReply      readReply;
  /** MMS User Preference Visibility */
  SimMmsupVisibility     visibility;
  /** MMS User Preference priori */
  SimMmsupPriority       priority;
  /** Indicates if delivery time is present */
  Boolean                deliveryTimePresent;
  /** MMS delivery time */
  SimMmsupTime           deliveryTime;
  /** Indicates if expiry time is present */
  Boolean                expiryTimePresent;
  /** MMS expiry time */
  SimMmsupTime           expiryTime;
}
SimMmsup;

/** Contains an alpha identifier as used in the ADN, FDN, LND, MSISDN  and SMSP files.*/
typedef struct SimAlphaIdentifierTag
{
    /** The length of the alpha Id
\assoc ARRAY \ref data */
    Int8                            length;
    /** The alpha Id */
    Int8                            data[SIM_ALPHA_ID_SIZE];
}
SimAlphaIdentifier;

/** For GSM SIM's contains a CHV value. This data is presented raw to the SIM.
 * It is the responsibility of the application layer to present
 * the CHV in its formatted form.
 */
typedef struct SimChvValueTag
{
    /** Holds CHV value */
    Int8                            value[SIM_CHV_LENGTH];
}
SimChvValue;


/** PIN value. Used for 3G USIM Applications */
typedef SimChvValue  SimUiccPinValue;

#if defined (FEA_TEMP_OBSOLETE_SIM_MANAGER)
/** This structure contains a price per unit currency table.
 * The price per unit (in the chosen currency) is calculated as follows:
 *
 * If (exponentIsPositive)
 *
 * price per unit = elementaryPricePerUnit * 10^(absoluteExponent)
 *
 * Else
 *
 * price per unit = elementaryPricePerUnit * 10^(-absoluteExponent)
 *
 * End If
 */
typedef struct SimPuctDataTag
{
    /** The currency code */
    Int8                            currencyCode[CURRENCY_CODE_SIZE];
    /** Price per Unit */
    Int16                           elementaryPricePerUnit;
    /** Absolute exponent */
    Int8                            absoluteExponent;
    /** Indicates whether or not exponent is positive */
    Boolean                         exponentIsPositive;
}
SimPuctData;

/** Holds the list of preferred cell broadcast message identifiers.
 * It is possible that the SIM does not hold a valid CBMI list
 * (i.e. the service is not allocated and activated, or the data is not initialised).
 */
typedef struct CbmiListTag
{
    /** The number of enties in array <B> cbmi </B> */
    Int8                            numCbmis;
    /** Lists of preferred cell broadcast message identifiers */
    Int16                           cbmi[SIM_CBMI_LIST_SIZE];
}
CbmiList;
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */

/** Specifies the type of service as identified in the SIM/USIM Service Table */
typedef enum SimUiccServiceTag
{
   /** Fixed Dialling Numbers service */
   SIM_FDN_SERVICE,
   /** Abbreviated dialling numbers service */
   SIM_ADN_SERVICE,
   /** LND service */
   SIM_LND_SERVICE,
   /** Service dialling number service */
   SIM_SDN_SERVICE,
   /** Barred Dialling Numbers service */
   SIM_BDN_SERVICE,
   /** Short Message Service Parameters  service */
   SIM_SMSP_SERVICE,
   /** SMS status service */
   SIM_SMSS_SERVICE,
   /** MSISDN service */
   SIM_MSISDN_SERVICE,
   /** Cell Broadcast Message identifier  service */
   SIM_CBMI_SERVICE,
   /** Advice of Charge service */
   SIM_AOC_SERVICE,
   /** Extension1 service */
   SIM_EXT1_SERVICE,
   /** Extension2 service */
   SIM_EXT2_SERVICE,
   /** Extension3 service */
   SIM_EXT3_SERVICE,
   /** Extension4 service */
   SIM_EXT4_SERVICE,
   /** Extension5 service */
   SIM_EXT5_SERVICE,
   /** Group Identifier Level 1 service */
   SIM_GID1_SERVICE,
   /** Group Identifier Level 2 service */
   SIM_GID2_SERVICE,
   /** Service Provider Nameservice */
   SIM_SPN_SERVICE,
   /** Service Provider Display Informationservice */
   SIM_SPDI_SERVICE,
   /** enhanced Multi Level Precedence and Pre-emption service */
   SIM_EMLPP_SERVICE,
   /** Automatic Answer for eMLPP Service */
   SIM_AAEMLPP_SERVICE,
   /** Cell Broadcast Message Identifier Range service */
   SIM_CBMIR_SERVICE,
   /** DCK service */
   SIM_DCK_SERVICE,
   /** Co-operative Network List service */
   SIM_CNL_SERVICE,
   /** Network's Indication of Alerting service */
   SIM_NIA_SERVICE,
   /** Short Message Status Reports  service */
   SIM_SMSR_SERVICE,
   /** PLMN service */
   SIM_PLMN_SEL_SERVICE,
   /** Capability Configuration Parameters  service */
   SIM_CCP_SERVICE,
   /** CPS service */
   SIM_CPS_SERVICE,
   /** Outgoing Call Information service */
   SIM_OCI_SERVICE,
   /** Incoming Call Information service */
   SIM_ICI_SERVICE,
   /** Mailbox Dialling Numbers service */
   SIM_MBDN_SERVICE,
   /** Message Waiting Indication Status service */
   SIM_MWIS_SERVICE,
   /** Call Forwarding Indication Status service */
   SIM_CFIS_SERVICE,
   /** Multimedia Messaging service */
   SIM_MMS_SERVICE,
   /** MMS User Connectivity Parameters service */
   SIM_MMSUCP_SERVICE,
   /** Extension8 service */
   SIM_EXT8_SERVICE,
   /** Extended Capability Configuration Parameter service */
   SIM_ECCP_SERVICE,
   /** GPRS service */
   SIM_GPRS_SERVICE,
   /** HPLMN selector */
   SIM_HPLMN_SEL_SERVICE,
   /** User controlled PLMN selector */
   SIM_UPLMN_SEL_SERVICE,
   /** Operator controlled PLMN selector */
   SIM_OPLMN_SEL_SERVICE,
   /** Local phonebook service */
   SIM_LOCAL_PHONEBOOK_SERVICE,
   /** SMS-PP Data Download */
   SIM_SMS_PP_DD_SERVICE,
   /** SMS-CB Data Download */
   SIM_SMS_CB_DD_SERVICE,
   /** CC service */
   SIM_CC_SERVICE,
   /** MO-SMS service */
   SIM_MO_SM_SERVICE,
   /** Operator PLMN List service */
   SIM_OPL_SERVICE,
   /** PLMN Network Name */
   SIM_PNN_SERVICE,
   /** Call Control on GPRS */
   SIM_CCGPRS_SERVICE,
   /** Call Control on EPS PDN */
   SIM_CC_EPS_PDN_SERVICE,
   /** Connectivity Parameters for USIM IP connections */
   SIM_IP_PARAMETERS_SERVICE
#if defined (ENABLE_EHPLMN)
   ,
   /** Equivalent Home PLMN */
   SIM_EHPLMN_SERVICE,
   /** Equivalent HPLMN Presentation Indication service */
   SIM_EHPLMNPI_SERVICE
#endif
}
SimUiccService;

#if defined (FEA_KOR_LGUP)
typedef enum CardTypeTag
{
    USIM_COMMON_CARD,
    USIM_LGUP_SUBSCRIBED_CARD,
    USIM_LGUP_UNSUBSCRIBED_CARD
} CardType;
#endif

/** Describes whether a SIM service is allocated and activated.*/
typedef struct SimServiceTag
{
    /** Indicates if SIM service is allocated */
    Boolean                         allocated;
    /** Indicates if SIM service is activated */
    Boolean                         activated;
}
SimService;

/** Unique SIM identification number (BCD number)
 * it is read from the card at power on or when a SIM has been inserted.
 */
typedef struct SimIccidTag
{
    /** Raw data returned */
    Int8                            data[SIM_ICCID_LEN];
}
SimIccid;

/** SIM Acess data element */
typedef struct SimAccessDataTag
{
    /** Update function */
    SimAccessCondition    update;
    /** Read seek function */
    SimAccessCondition    readSeek;
    /** Increase function */
    SimAccessCondition    increase;
    /** Invalidate function */
    SimAccessCondition    invalidate;
    /** Rehabilidate function */
    SimAccessCondition    rehabilitate;
}
SimAccessData;

/* -----------------21/03/02 15:08---------------------------------------------------
 * See TS 102 221, section 9.2.4/5/6
 * --------------------------------------------------------------------------------*/
/** This specifies the format used for the security attributes.
 * If a GSM SIM is used, the format will always be USIM_GSM_FORMAT.
 * If a UICC card is used, then it can be one of the following format: <UL>
 * <LI> USIM_EXPANDED_FORMAT,
 * <LI> USIM_REF_TO_EXPANDED_FORMAT or
 * <LI> USIM_COMPACT_FORMAT.
 *</UL>
 * The security attributes are encoded inside the FCP template
 */
typedef enum SimUiccSecAttributesFormatTag
{
   /** GSM format */
   USIM_GSM_FORMAT,
   /** Compact format */
   USIM_COMPACT_FORMAT,
   /** Expanded format */
   USIM_EXPANDED_FORMAT,
   /** Referenced to expanded format */
   USIM_REF_TO_EXPANDED_FORMAT,
   /** Undefined format */
   USIM_UNDEFINED_FORMAT
} SimUiccSecAttributesFormat;

/** Security environement element */
typedef enum SimUiccSecEnvironmentTag
{
    /** Security environement 0 */
    SIM_UICC_SE00 = 0,
    /** Security environement 1 */
    SIM_UICC_SE01 = 1,
    /** Security environement unknown */
    SIM_UICC_SE_UNKNOWN

}SimUiccSecEnvironment;
/* Used to read/write the network parameters on EF NETPAR.*/
typedef struct SimUiccGsmCellInformationTag
{
    /** The number of GSM frequencies
\assoc ARRAY \ref bcchFrequencies */
    Int8                                numberOfGsmFrequencies;
    /** Contains BCCH frequencies */
    Int16                               bcchFrequencies [MAX_STORED_GSM_FREQUENCIES];
} SimUiccGsmCellInformation;

/** Used to read/write the network parameters on EF NETPAR.*/
typedef struct SimUiccFddFrequencyListInfoTag
{
    /** The carrier frequency */
    Int16                               carrierFrequency;
    /** The number of scrambling codes
\assoc ARRAY \ref scramblingCode */
    Int8                                numberOfScramblingCodes;
    /** Lists the scrambling codes */
    UPrimaryScramblingCode              scramblingCode [MAX_STORED_SCRAMBLING_CODES];
}
SimUiccFddFrequencyListInfo;

/** FDD frequencies: the first one is the intra frequency*/
typedef struct SimUiccFddCellInformationTag
{
   /** The numbe of FDD frequencies
\assoc ARRAY \ref frequencyListInfo */
   Int8                                numberOfFddFrequencies;
   /** Determines frequency info for each frequency */
   SimUiccFddFrequencyListInfo         frequencyListInfo [SIM_MAX_STORED_FDD_FREQUENCIES];
} SimUiccFddCellInformation;

#   if defined (UPGRADE_3G_TDD128)
/** Used to read/write the network parameters on EF NETPAR. */
typedef struct SimUiccTddFrequencyListInfoTag
{
    /** The carrier frequency */
    Int16                              carrierFrequency;
    /** Indicates the number of cell parameters ID
\assoc ARRAY \ref cellParametersId
\assoc ARRAY \ref cellParametersId */
    Int8                               numberOfCellParametersIds;
    /** Lists cell parameters ID */
    Int16                              cellParametersId[MAX_STORED_CELLPARAMETERSID];
}
SimUiccTddFrequencyListInfo;

/** TDD128 frequencies (the first one is the intra frequency) */
typedef struct SimUiccTddCellInformationTag
{
    /** The number of TDD128 frequencies
\assoc ARRAY \ref frequencyListInfo */
    Int8                               numberOfTddFrequencies;
    /** Lists Frequency info */
    SimUiccTddFrequencyListInfo        frequencyListInfo [SIM_MAX_STORED_TDD_FREQUENCIES];
} SimUiccTddCellInformation;
#   endif


/** Status data element
 * When an MF or DF is selected or when the STATUS command is executed,
 * the response data contains generic data relating to the particular directory.
 * This structure holds this status data in its decoded form.
 *
 * \note <UL>
 * <LI> If status = SIM_DIRS_INVALID, all other fields are invalid
 * <LI> If status = SIM_DIRS_INVALID_STAT_LENGTH, all other fields are invalid.
 * </UL>
 */
typedef struct SimDirStatusDataTag
{
    /** The status of directory */
    SimDirStatus                    status;
    /** Identifies directory */
    Int16                           dirId;
    /** The file type should always be MF or DF.*/
    SimFileType                     fileType;
    /** Specifies the type of data used (GSM/USIM)
\assoc UNION \ref specificData */
    SimUiccTypeOfData               typeOfData;
    /** Info about the status of the local PIN for instance*/
    SimUiccSpecificData             specificData;
}
SimDirStatusData;

/** This structure is used when accessing data in a linear fixed file.
 * The record to read/write/delete etc. may be specified indirectly by a
 * seek pattern and seek mode, or explicitly via a read/write record mode
 * and if necessary a record number.
 */
typedef struct SimRecordDefinitionTag
{
    /** Perform seek or normal read
\assoc PRESENT \ref searchMode
\assoc PRESENT \ref patternLength
\assoc PRESENT \ref pattern */
    Boolean                         performSeek;

    /*
    only used if performSeek == TRUE
    */
    /** Search mode used */
    SimUiccSearchMode               searchMode;
    /** Search type used */
    SimUiccSearchType               searchType;
    SimUiccEnhancedSearchIndication searchIndication;  /**< only used for enhanced search*/
    /** The length of the pattern data */
    Int8                            patternLength;
    /** The record data to write */
    Int8                            pattern[SIM_MAX_PATT_LENGTH];

    /** Only used if performSeek == FALSE */
    SimRecordReadWriteMode          readMode;
    /** Only used if readMode = SIM_REC_RWMODE_CURRENT_ABSOLUTE
     * (zero implies CURRENT)
     */
    Int8                            recordNumber;
}
SimRecordDefinition;

/** Holds the SIM administration data */
typedef struct SimAdminDataTag
{
    /** Defines the MS operation mode */
    MsOperationMode                 msOperationMode;
    /** MSB is byte 1 2 of AD file.
     * additionalInfo is the special facility number
     * if msOperationMode is MS_OPERN_NORMAL_PLUS_SPECIFIC
     * or MS_OPERN_TA_PLUS_SPECIFIC.  It is ME manufacturer-specific
     * if msOperationMode is MS_OPERN_MAINTENANCE, otherwise it is not used
     */
    Int16                           additionalInfo;
#if defined (UPGRADE_REL6)
    /** if the msOperation mode is decoded to NORMAL_PLUS_SPECIFIC then
       the additional information indicates if ciphering indication is enabled or
       disabled - additionalInfo is decoded in that case - and this parameter
       is set to TRUE.
       \assoc PRESENT \ref cipherIndFeatureEnabled*/
    Boolean                         cipherIndFeaturePresent;
    /** indicates whether the ciphering indication feature in the MMI is enabled or not.
      */
    Boolean                         cipherIndFeatureEnabled;
#endif
    /** Specifies whether the MNC shall be coded on 2 or 3 digits.*/
    Int8                            mncLength;
}
SimAdminData;

/** Holds a list of preferred PLMNs.
 * Note that the SIM may hold more than SIM_PLMN_LIST_SIZE entries.
 * Multiple requests can be made (read, write etc.) if more than SIM_PLMN_LIST_SIZE
 * entries need to be read/written.  If there are more than SIM_PLMN_LIST_SIZE
 * to be read, the nextField field will indicate which field to request next.
 * Only valid Plmn (different from 0xff,0xff,0xff) shall be returned.
 */
typedef struct SimPlmnListTag
{
    /** The number of entries in array <I> simPlmn </I>
\assoc ARRAY \ref simPlmn */
    Int8                            numPlmns;
    /* List of preferred PLMNs */
    SimPlmn                         simPlmn[SIM_PLMN_LIST_SIZE];
}
SimPlmnList;

/** Holds a list of  PLMNs read from the SPDI file.
 * Multiple requests can be made (read, write etc.)
 * if more than SIM_SPDI_LIST_SIZE entries need to be read.
 * Only valid Plmns populate the list.
 */
typedef struct SimSpdiListTag
{
    /** The number of entries in array <I> simPlmn </I>*/
    Int8                            numPlmns;
    /** List of  PLMNs read from the SPDI file */
    SimPlmn                         simPlmn[SIM_SPDI_LIST_SIZE];
}
SimSpdiList;

/** Typedef of SimFplmnList */
typedef SimPlmnList SimFplmnList;

/** Specifies type of service table : 2G or 3G */
typedef enum ServiceTableTypeTag
{
    /** 2G Service table */
    SIM_SERVICE_TABLE = 0,
    /** 3G Service table */
    USIM_SERVICE_TABLE = 1
}  ServiceTableType;

/*-----------------19/11/01 15:19----------------------------------------------
 * This table is defined for UICC cards in TS 31.102 version 4.0.0 release 4
 * with some additional release 5 and release 7 features included
 * --------------------------------------------------------------------------*/
/**  Services Contents.
 * This describes the USIM service table. Only used for 3G SIM.
 * phase2/phase2+ SIMs, the SIM service table is different.
 */
typedef struct UsimServiceTableTag
{
   /** Local Phone Book */
   Boolean   localPhoneBook;
   /** Fixed Dialling Numbers (FDN) */
   Boolean   fdn;
   /** Extension 2*/
   Boolean   extension2;
   /** Service Dialling Numbers (SDN) */
   Boolean   sdn;
   /** Extension3 */
   Boolean   extension3;
   /** Barred Dialling Numbers (BDN) */
   Boolean   bdn;
   /** Extension4 */
   Boolean   extension4;
   /** outgoing call information*/
   Boolean   oci;
   /** incoming call information*/
   Boolean   ici;
   /** Short Message Storage (SMS) */
   Boolean   smsStorage;
   /** Short Message Status Reports (SMSR) */
   Boolean   smsr;
   /** Short Message Service Parameters (SMSP) */
   Boolean   smsParameters;
   /** Advice of Charge (AoC) */
   Boolean   adviceOfCharge;
   /** Capability Configuration Parameters 2 (CCP2) */
   Boolean   capabilityConfig;
   /** Cell Broadcast Message Identifier  */
   Boolean   cbmi;
   /** Cell Broadcast Message Identifier Ranges  */
   Boolean   cbmir;
   /** Group Identifier Level 1 */
   Boolean   gid1;
   /** Group Identifier Level 2 */
   Boolean   gid2;
   /** Service Provider Name */
   Boolean   spn;
   /** User controlled PLMN selector with Access Technology */
   Boolean   userPlmnSelector;
   /** MSISDN */
   Boolean   msisdn;
   /** Image (IMG) */
   Boolean   img;
   /** Support of Localised Service Areas (SoLSA)  */
   Boolean   solsa;
   /** Enhanced Multi Level Precedence and Pre emption Service */
   Boolean   emlpp;
   /** Automatic Answer for eMLPP */
   Boolean   aaEmlpp;
   /** RFU */
   Boolean   rfu;
   /** GSM Access */
   Boolean   gsmAccess;
   /** Data download via SMS-PP */
   Boolean   ddSmsPp;
   /** Data download via SMS CB */
   Boolean   ddSmsCb;
   /** Call Control by USIM */
   Boolean   cc;
   /** MO-SMS Control by USIM */
   Boolean   moSms;
   /** RUN AT COMMAND command */
   Boolean   runAtCommand;

   /** Not used */
   Boolean   notUsed;
   /** Enabled Services Table */
   Boolean   est;
   /** Access point name control list*/
   Boolean   apn;
   /** Depersonalisation Control Keys */
   Boolean   dck;
   /** Co-operative Network List */
   Boolean   cnl;
   /** GSM security context  */
   Boolean   gsmSecurityContext;
   /** CPBCCH Information */
   Boolean   cpbcchInfo;
   /** Investigation Scan */
   Boolean   investigationScan;
   /** MExE */
   Boolean   MExE;
   /** Operator controlled PLMN selector with Access Technology */
   Boolean   operatorPlmnSelector;
   /** HPLMN selector with Access Technology */
   Boolean   hplmnSelector;
   /** Extension 5 */
   Boolean   extension5;
   /** PLMN Network Name */
   Boolean   plmnNetworkName;
   /** Operator PLMN List */
   Boolean   operatorPlmnList;
   /** Mailbox dialling  number*/
   Boolean   mbdn;
   /** Message waiting indication status*/
   Boolean   mwis;
   /** Call forwarding indication status*/
   Boolean   cfis;
   /** Reserved and shall be ignored */
   Boolean   reserved;
   /** Service Provider Display Information */
   Boolean   serviceProviderDisplayInfo;
   /** Multimedia Messaging Service (MMS) */
   Boolean   mms;
   /** Extension 8 */
   Boolean   extension8;
   /** Call control on GPRS by USIM */
   Boolean   ccByGprs;
   /** MMS User Connectivity Parameters */
   Boolean   mmsUserConnectivityParam;
   /** Network indication of alerting in the MS (NIA) */
   Boolean   nia;
   /** VGCS Group Identifier List (EFVGCS and EFVGCSS) */
   Boolean   vgcsIdList;
   /** VBS Group Identifier List (EFVBS and EFVBSS) */
   Boolean   vbsIdList;
#if defined (UPGRADE_REL8_USIM)
   /* releae 7 features */
   /** Pseudonym */
   Boolean   pseudonym;
   /** User Controlled PLMN selector for Wireless LAN access */
   Boolean   uplmnForWlan;
   /** Operator Controlled PLMN selector for Wireless LAN access */
   Boolean   oplmnForWlan;
   /** User controlled WSID list*/
   Boolean   userWsidList;
   /**Operator Controlled WSID list */
   Boolean   operatrWsidList;
   /** VGCS security */
   Boolean   vgcsSecurity;
   /** VBS Security   */
   Boolean   vbsSecurity;
    /*WLAN Reauthentication Identity */
   Boolean   wlanReauthIdent;
   /* MMS storage */
   Boolean   mmsStorage;
   /* Generic Bootstrapping Architecture */
   Boolean   gba;
   /* MBMS Security */
   Boolean   mbmsSecurity;
   /* Data download by USSD or USSD application Mode */
   Boolean   dataDownloadByUssd;
   /** Equivalent Home PLMN */
   Boolean   ehplmn;
   /** Additional Terminal Profile after UICC activation */
   Boolean   additTermProfile;
   /** Equivalent HomePLMN Presentation indication */
   Boolean   ehplmnpi;
   /**  Last RPLMN Selection Indication */
   Boolean   lastRplmnSelectionInd;
   /** not used */
   Boolean   reserved2;
   /** GBA-Based Local Key Establishment Mechanism  */
   Boolean   gbaLocalKey;
   /** Terminal Applications */
   Boolean   terminalApps;
   /** Service Provider Name Icon */
   Boolean   spni;
   /** PLMN Network Name Icon */
   Boolean   plmnnni;
   /** Connectivity Parameters for USIM IP connections */
   Boolean   usimIpParams;
   /** Home IWLAN Specific Identifier List */
   Boolean   homeIwlanList;
   /** IWLAN Equiv HPLMN Presentation Indication */
   Boolean   iwlanEhplmnpi;
   /** IWLAN HPLMN Priority Indication */
   Boolean   iwlanHplmnpi;
   /** IWLAN Last Registered PLMN */
   Boolean   iwlanLRplmn;
   /** EPS Mobility Management Information */
   Boolean   epsMobilityManagementInfo;
   /** Allowed CSG Lists and Indications */
   Boolean   allowedCsgInfo;
   /** Call control on EPS PDN connection by USIM */
   Boolean   ccOnUsimForEpsPdn;
   /**  HPLMN Direct Access */
   Boolean   hplmnDirectAccess;
   /** eCall Data */
   Boolean   eCallData;
   /** Operator CSG Lists and corresponding indications */
   Boolean   operatorCgsLists;
   /** Support for SM-over-IP */
   Boolean   supportSmOverIp;
   /** Support of CSG Display Control */
   Boolean   supportOfCgsDisplayControl;
   /** Communication Control for IMS by USIM */
   Boolean   controlForImsiByUsim;
   /** Extended Terminal Applications */
   Boolean   extTerminalAppl;
   /** Support of UICC access to IMS */
   Boolean   supportUiccAccessImsi;
   /** Non-Access Stratum configuration by USIM */
   Boolean   nasConfigByUsim;
   /** PWS configuration by USIM */
   Boolean   pwsConfigByUsim;
   /** RFU */
   Boolean   rfu1;
   /** URI support by UICC */
   Boolean   uriSupportByUicc;
   /** Extended EARFCN support */
   Boolean   extEarfcnSupport;
   /** ProSe */
   Boolean   proSe;
   /** USAT Application Pairing */
   Boolean   usatApplPairing;
   /** Media Type support */
   Boolean   mediaTypeSupport;
   /** IMS call disconnection cause */
   Boolean   imsiCallDisconnectCause;
   /** URI support for MO SHORT MESSAGE CONTROL */
   Boolean   uriSupportMoSmc;
   /** ePDG configuration Information support */
   Boolean   ePdgConfigInfoSupport;
   /** ePDG configuration Information configured */
   Boolean   ePdgConfigInfoConfigured;
   /** ACDC support */
   Boolean   acdcSupport;
   /** MCPTT */
   Boolean   mcptt;
   /** ePDG configuration Information for Emergency Service support */
   Boolean   ePdgConfigEmergServiceSupport;
   /** ePDG configuration Information for Emergency Service configured */
   Boolean   ePdgConfigEmergServiceConfigured;
   /** eCall Data over IMS */
   Boolean   eCallDataOverIms;
   /** URI support for SMS-PP DOWNLOAD as defined in 3GPP TS 31.111 [12] */
   Boolean   uriSupportSmsPpDownload;
   /** From Preferred */
   Boolean   fromPreferred;
   /** IMS configuration data */
   Boolean   imsConfigData;
   /** TV configuration */
   Boolean   tvConfiguration;
   /** 3GPP PS Data Off */
   Boolean   psDataOff;
   /** 3GPP PS Data Off Service List */
   Boolean   psDataOffServiceList;
   /** V2X */
   Boolean   v2x;
   /** XCAP Configuration Data */
   Boolean   xcapConfigData;

#else
# if defined (ENABLE_EHPLMN) || ( defined (UPGRADE_3G) && defined (UPGRADE_3G_MBMS) )
   /* releae 7 features */
   /** Pseudonym */
   Boolean   pseudonym;
   /** User Controlled PLMN selector for Wireless LAN access */
   Boolean   uplmnForWlan;
   /** Operator Controlled PLMN selector for Wireless LAN access */
   Boolean   oplmnForWlan;
   /** User controlled WSID list*/
   Boolean   userWsidList;
   /**Operator Controlled WSID list */
   Boolean   operatrWsidList;
   /** VGCS security */
   Boolean   vgcsSecurity;
   /** VBS Security   */
   Boolean   vbsSecurity;
    /*WLAN Reauthentication Identity */
   Boolean   wlanReauthIdent;
   /* MMS storage */
   Boolean   mmsStorage;
   /* Generic Bootstrapping Architecture */
   Boolean   gba;
   /* MBMS Security */
   Boolean   mbmsSecurity;
   /* Data download by USSD or USSD application Mode */
   Boolean   dataDownloadByUssd;
   /** Equivalent Home PLMN */
   Boolean   ehplmn;
   /** Additional Terminal Profile after UICC activation */
   Boolean   additTermProfile;
   /** Equivalent HomePLMN Presentation indication */
   Boolean   ehplmnpi;
   /**  Last RPLMN Selection Indication */
   Boolean   lastRplmnSelectionInd;
   /** not used */
   Boolean   reserved2;
   /** GBA-Based Local Key Establishment Mechanism  */
   Boolean   gbaLocalKey;
   /** Terminal Applications */
   Boolean   terminalApps;
# endif /*(ENABLE_EHPLMN) || ( (UPGRADE_3G) && (UPGRADE_3G_MBMS) ) */
#endif /* !UPGRADE_REL8_USIM */
} UsimServiceTable;


/** This describes the SIM service table.
 * This is returned in the AlsiAppInitialiseCnf. Only used for non-3G SIM
 *
 * The order of the services in the following structure must be the
 * same as the order in the SIM.
*/
typedef struct SimServiceTableTag
{
    /** CHV 1 Disable */
    SimService                      chv1Disable;
    /** Abbreviated dialling numbers */
    SimService                      adn;
    /** Fixed Dialling Numbers  */
    SimService                      fdn;
    /** SMS storage */
    SimService                      smsStorage;
    /** Advice of charge */
    SimService                      adviceOfCharge;
    /** Capability configured */
    SimService                      capabilityConfig;
    /** PLMN selctor */
    SimService                      plmnSelector;
    /** Called party sub address */
    SimService                      calledPartySubaddress;
    /** MSISDN */
    SimService                      msisdn;
    /** Extension 1 */
    SimService                      extension1;
    /** Extension 2 */
    SimService                      extension2;
    /** SMS parameters */
    SimService                      smsParameters;
    /** LND */
    SimService                      lnd;
    /** CBMI */
    SimService                      cbmi;
    /** Group Identifier Level 1 */
    SimService                      gid1;
    /** Group Identifier Level  2 */
    SimService                      gid2;
    /** SPN */
    SimService                      spn;
    /* Phase 2+ services */
    /** Service Dialling Numbers  */
    SimService                      sdn;
    /** Extension 3 */
    SimService                      extension3;
    /** Reserved for Future Use */
    SimService                      rfu;
    /** VGCS Id list */
    SimService                      vgcsIdList;
    /** VBS Id list */
    SimService                      vbsIdList;
    /** EMLPP */
    SimService                      emlpp;
    /** Automatic Answer for eMLPP */
    SimService                      aaEmlpp;
    /** Data Download via SMS CB */
    SimService                      ddSmsCb;
    /** Data Download via SMS Pp */
    SimService                      ddSmsPp;
    /** Menu selection */
    SimService                      menuSelection;
    /** Call control */
    SimService                      cc;
    /** Pro-active SIM */
    SimService                      proactiveSim;
    /** Cell Broadcast Message Identifier */
    SimService                      cbmir;
    /** Barred Dialling Numbers  */
    SimService                      bdn;
    /** Extension 4 */
    SimService                      extension4;
    /** Depersonalisation Control Keys) */
    SimService                      dck;
    /** Co-operative Network List */
    SimService                      cnl;
    /** Short Message Status Reports  */
    SimService                      smsr;
    /** Network indication of alerting */
    SimService                      nia;
    /** MO SMS */
    SimService                      moSms;
    /** GPRS */
    SimService                      gprs;
    /** Image */
    SimService                      image;
    /** Support of Localised Service Areas */
    SimService                      solsa;
    SimService                      ussdInCc;
    /** Run AT command */
    SimService                      runAtCommand;
    /** User selects PLMN */
    SimService                      userPlmnSelector;
    /** Operator selects PLMN */
    SimService                      operatorPlmnSelector;
    /** HPLMN selector */
    SimService                      hplmnSelector;
    /** CPBCCH Information */
    SimService                      cpbcchInfo;
    /** Investigation Scan */
    SimService                      investigationScan;
    /** Extended Capability Configuration Parameter */
    SimService                      eccp;
    /** MExE */
    SimService                      MExE;
    /** Last RPLMN used technology */
    SimService                      rplmnLastUsedTechnology;
    /** PLMN network name */
    SimService                      plmnNetworkName;
    /** Opeartor PLMN list */
    SimService                      operatorPlmnList;
    /** Mailbox dialing number */
    SimService                      mailboxDiallingNumber;
    /** Message waiting indication status */
    SimService                      messageWaitingIndStatus;
    /** Call forwarding indication status */
    SimService                      callForwardingIndStatus;
    /** service provider display info */
    SimService                      serviceProviderDisplayInfo;
    /** Multimedia messaging service*/
    SimService                      mms;
    /** Extension 8 */
    SimService                      extension8;
    /** MMS user connectivity param */
    SimService                      mmsUserConnectivityParam;
}
SimServiceTable;

/** Specifies service table to use */
typedef union ServiceTableTag
{
    /** 2G service table */
    SimServiceTable      simService;
    /** 3G service table */
    UsimServiceTable     usimService;
} ServiceTable;

/** The union of short message transfer protocol data units (SM TPDU).*/
typedef union SimSmTpduTag
{
    /** SMS deliver (conveying a short message from the SC to the MS). */
    TsDeliverInd        deliver;
    /** Information as part of a positive or negative acknowledgement.  This is
     * a dummy parameter to make sure the Genie associations work */
    TsStatusReportInd   deliverStatusReport;
    /** SMS submit (conveying a short message from the MS to the SC). */
    TsSubmitReq         submit;
    /** Information as part of a positive or negative acknowledgement.  This is
     * a dummy parameter to make sure the Genie associations work */
    TsStatusReportInd   submitStatusReport;
    /** Command */
    TsCommandReq        command;
    /** Information as part of a positive or negative acknowledgement.*/
    TsStatusReportInd   statusReport;
}
SimSmTpdu;

/** Mobile Subscriber Profile, value can be 1 to 4*/
typedef enum SimMspNumberTag
{
  /** Mobile Subscriber Profile 1 */
  SIM_MSP_1 = 1,
  /** Mobile Subscriber Profile 2 */
  SIM_MSP_2,
  /** Mobile Subscriber Profile 3 */
  SIM_MSP_3,
  /** Mobile Subscriber Profile 4 */
  SIM_MSP_4,
  /** Mobile Subscriber Profile unknown */
  SIM_MSP_INVALID
} SimMspNumber;

/** Call Forwarding Indication Status, see 51.011, section 10.3.46*/
typedef struct SimCfisStatusTag
{
  /** Indicates if call forward voice is active */
  Boolean   callFwdVoiceActive;
  /** Indicates if call forward fax is active */
  Boolean   callFwdFaxActive;
  /** Indicates if call forward data is active - note
    for Rel 6 implementation this is specifically for Data -
    Teleservices - with bearer services CFIS active status
    defined seperately below */
  Boolean   callFwdDataActive;
#if defined (UPGRADE_REL6)
  /** Indicates if call forward sms is active */
  Boolean   callFwdSmsActive;
  /** Indicates if call forward for all data bearer services
         is active */
  Boolean   callFwdDataBearersActive;
#endif
} SimCfisStatus;

/** Decoded contents of a record from the EF CFIS file.*/
typedef struct SimCfisDataTag
{
    /*MSP */
    SimMspNumber                    msp;
    /** Call Forwarding Indication Status */
    SimCfisStatus                   cfisStatus;
    /** Type of Number (TON) */
    BcdNumberType                   typeOfNumber;
    /** Numbering Plan Id (NPI) */
    BcdNumberPlan                   numberPlan;
    /** The length of dialed string */
    Int8                            dialStringLength;
    /** Lists dialed string */
    Bcd                             dialString[SIM_MAX_DIAL_NUMBER_LENGTH];
    /** Indicates if bearer capability is defined */
    Boolean                         bearerCapabilityDefined;
    /** Bearer capability info */
    BearerCapability                bearerCapability;
    /** Indicates if sub address is defined */
    Boolean                         subAddressDefined;
    /** Lists called party sub address */
    CalledPartySubaddress           calledPartySubaddress;
} SimCfisData;

/** Holds all parameters necessary for an abbreviated dial number.*/
typedef struct SimAbbreviatedDialNumberTag
{
    /** Defining the abbreviated dial number */
    SimAlphaIdentifier              alphaId;
    /** set FALSE if SSC string etc */
    Boolean                         isDiallingNumber;
    /** Type of Number (TON) */
    BcdNumberType                   typeOfNumber;
    /** Numbering Plan Id (NPI) */
    BcdNumberPlan                   numberPlan;
    /** The length of dialled number
\assoc ARRAY \ref dialString */
    Int8                            dialStringLength;
    /** Lists dialled number */
    Bcd                             dialString[SIM_MAX_DIAL_NUMBER_LENGTH];
    /** Indicates if bearerCapability is valid
\assoc PRESENT \ref bearerCapability */
    Boolean                         bearerCapabilityDefined;
    /** Beare capability info */
    BearerCapability                bearerCapability;
    /** Indicates if sub address is defined
\assoc PRESENT \ref calledPartySubaddress */
    Boolean                         subAddressDefined;
    /** Lists called party sub address */
    CalledPartySubaddress           calledPartySubaddress;
    /** The ComparisonMethodInfo field is only valid for BDN file.*/
    Int8                            comparisonMethodInfo;
}
SimAbbreviatedDialNumber;

/** SIM dial number element */
typedef struct SimDialNumberTag
{
    /** Defining the abbreviated dial number */
    SimAlphaIdentifier              alphaId;
    /** set FALSE if SSC string etc. */
    Boolean                         isDiallingNumber;
    /** Type of Number (TON) */
    BcdNumberType                   typeOfNumber;
    /** Numbering Plan Id (NPI) */
    BcdNumberPlan                   numberPlan;
    /** The length of dialled number
\assoc ARRAY \ref dialString */
    Int8                            dialStringLength;
    /** Lists dialled number */
    Bcd                             dialString[SIM_MAX_DIAL_NUMBER_LENGTH];
    /** Bearer capability record number */
    Int8                            bearerCapRecordNum;
    /** Indicates if sub address is defined
\assoc PRESENT \ref calledPartySubaddress */
    Boolean                         subAddressDefined;
    /** Sub address record number*/
    Int8                            subAddressRecordNum;
    /** Lists called party sub address */
    CalledPartySubaddress           calledPartySubaddress;
}
SimDialNumber;

/** SIM dial number element without sub address */
typedef struct SimDialNumberWithoutSubAddrTag
{
    /** Defining the abbreviated dial number */
    SimAlphaIdentifier              alphaId;
    /** set FALSE if SSC string etc. */
    Boolean                         isDiallingNumber;
    /** Type of Number (TON) */
    BcdNumberType                   typeOfNumber;
    /** Numbering Plan Id (NPI) */
    BcdNumberPlan                   numberPlan;
    /** The length of dialled number */
    Int8                            dialStringLength;
    /** Lists dialled number */
    Bcd                             dialString[SIM_MAX_DIAL_NUMBER_LENGTH];
    /** Bearer capability record number */
    Int8                            bearerCapRecordNum;
    /** Indicates if sub address is defined */
    Boolean                         subAddressDefined;
    /** Sub address record number*/
    Int8                            subAddressRecordNum;
}
SimDialNumberWithoutSubAddr;

/** used to allocate storage for a block of EXT8 data   */
typedef struct ExtDataBlockTag
{
  /** Points to next list */
  struct ExtDataBlockTag  *nextBlock_p;
  /** Block of EXT data */
  SimDataBlock          simDataBlock;
}
ExtDataBlock;

/** Holds some of the parameters from an abbreviated dial number.
 * This is basically the same as the SimAbbreviatedDialNumber structure except
 * there is no storage space for a sub address or bearer capability
 * (although there are Booleans to indicate whether or not this data is present on the SIM).
 */
typedef struct SimDialNumberSummaryTag
{
    /** Alpha identifier */
    SimAlphaIdentifier              alphaId;
    /** set FALSE if SSC string etc. */
    Boolean                         isDiallingNumber;
    /** Type of Number (TON) */
    BcdNumberType                   typeOfNumber;
    /** Numbering Plan Id (NPI) */
    BcdNumberPlan                   numberPlan;
    /** Indicates the length of dial number
\assoc ARRAY \ref dialString */
    Int8                            dialStringLength;
    /** Lists the dial number */
    Bcd                             dialString[SIM_MAX_SUMMARY_DIALNUM_LENGTH];
    /** Indicates whether or not bearer capability is present */
    Boolean                         bearerCapabilityDefined;
    /** Indicates whether or not sub address is present */
    Boolean                         subAddressDefined;
} SimDialNumberSummary;

/** Contains all the information on an SMS TPDU */
typedef struct SimSmAllTpduTag
{
    /** Identifies which type of TPDU is stored in the shortMessageTpdu field.
\assoc UNION \ref shortMessageTpdu */
    SimSmTpduType                   type;
    /** Short message transfer protocol data units  */
    SimSmTpdu                       shortMessageTpdu;
}
SimSmAllTpdu;

/** Contains a record of the SMSP (SMS parameters) file.*/
typedef struct SimSmsParametersTag
{
    /** Alpha identifier for SMSP */
    SimAlphaIdentifier              alphaId;
    /** Indicates if destination address is present
\assoc PRESENT \ref destAddr */
    Boolean                         destAddrPresent;
    /** Indicates if source address is present
\assoc PRESENT \ref scAddr */
    Boolean                         scAddrPresent;
    /** Indicates if protocol ID is present
\assoc PRESENT \ref smsProtocolId */
    Boolean                         protocolIdPresent;
    /** Indicates if coding scheme is present
\assoc PRESENT \ref smsDataCodingScheme */
    Boolean                         codingSchemePresent;
    /** Indicates if validity period is present
\assoc PRESENT \ref validityPeriodAsValue */
    Boolean                         validityPeriodPresent;
    /** Destination address */
    SmsAddress                      destAddr;
    /** Source address */
    SmsAddress                      scAddr;
    /** SMS protocol ID */
    SmsProtocolId                   smsProtocolId;
    /** Specifies the coding scheme */
    SmsDataCoding                   smsDataCodingScheme;
    /** The valididy period */
    Int8                            validityPeriodAsValue;
}
SimSmsParameters;

/** Holds the security conditions. (Used when the security attributes
 * are encoded in expanded format).
 */
typedef struct SimUiccSecurityConditionTag
{
  /** Condition to access SIM */
  SimUiccAccessCondition    condition;
  /** Indicates the number of key references*/
  Int8                      numKeyRef;
  /** Lists key references value */
  SimUiccKeyRefValue        keyRef[MAX_NUM_KEY_REF];
  /** If PIN is required, specify which PIN is required*/
  Boolean                   orTemplate;
} SimUiccSecurityCondition;


/*-----------------21/03/02 15:08---------------------------------------------------
 * See TS 102 221, section 9.2.4/5/6:
 * The access rules can be indicated in the FCP in one of the following formats:
 * 1) Compact,
 * 2) Expanded,
 * 3) Referenced to expanded format
 * --------------------------------------------------------------------------------*/

/*-----------------12/11/01 10:34----------------------------------------------------*/
/** Holds the security attributes for a file in compact format.
 * This is used to define access conditions for a Ef file, see ISO 7816-9 for details
 */

typedef struct SimUiccEfCompactFormatTag    /*The PIN are implicitely known*/
{
    /** Delete file */
    SimUiccAccessCondition    deleteFile;
    /** Terminate */
    SimUiccAccessCondition    terminate;
    /** Activate */
    SimUiccAccessCondition    activate;
    /** Deactivate */
    SimUiccAccessCondition    deactivate;
    /** Write */
    SimUiccAccessCondition    write;
    /** Update */
    SimUiccAccessCondition    update;
    /** Read and search */
    SimUiccAccessCondition    readAndSearch;
} SimUiccEfCompactFormat;

/** Holds the security attributes for a file in expanded format.
 * This is used to define access conditions for a Ef file
 */
typedef struct SimUiccEfExpandedFormatTag
{
   /** Delete file */
   SimUiccSecurityCondition    deleteFile;
   /** Terminate */
   SimUiccSecurityCondition    terminate;
   /** Activate */
   SimUiccSecurityCondition    activate;
   /** Deactivate */
   SimUiccSecurityCondition    deactivate;
   /** Write */
   SimUiccSecurityCondition    write;
   /** Update */
   SimUiccSecurityCondition    update;
   /** Read and search */
   SimUiccSecurityCondition    readAndSearch;
} SimUiccEfExpandedFormat;

/** Holds the security attributes for a file in referenced to expanded format.*/
typedef struct SimUiccRefToExpandedFormatTag
{
   /** Is the file ID of EF ARR.*/
   Int16        arrEfId;
   Int8         recordNumCurrentSe;  /**< record number to use for current SE*/
   Int8         recordNumSe00;       /**< record number to use for SE00*/
   Int8         recordNumSe01;       /**< record number to use for SE01*/
} SimUiccRefToExpandedFormat;

/** Holds the security attributes */
typedef union SimUiccSecAttributesTag
{
   /** Compact format */
   SimUiccEfCompactFormat           compactFormat;
   /** Expanded format */
   SimUiccEfExpandedFormat          expandedFormat;
   /** Referenced to expanded format */
   SimUiccRefToExpandedFormat       refToExpandedFormat;
   /** GSM format (for GSM SIMs only) */
   SimAccessData                    gsmFormat;
} SimUiccSecAttributes;

/** Status information element of EF */
typedef struct SimEfStatusInfoTag
{
    /** Status of EF */
    SimEfStatus             status;
    /** Identifies EF */
    SimEfId                 fileId;
    /** The size of file */
    Int16                   fileSize;
    /** The length of record */
    Int8                    recordLength;
    /** The number of records */
    Int8                    numberOfRecords;
    /** The state of file */
    SimUiccState            fileState;
}
SimEfStatusInfo;

/** This holds some status information about a specific EF on the SIM/UICC.*/

typedef struct SimEfStatusDataTag
{
    /** Reports whether the information about this file was decoded successfully.*/
    SimEfStatus             status;
    /** Specifies the file ID.*/
    Int16                   fileId;
    /** Defines the state of the file (activated or deactivated)*/
    SimUiccState            fileState;
    /** The size of the file.*/
    Int16                   fileSize;
    /** The length of the records if this file is LF or cyclic.*/
    Int8                    recordLength;
    /** Reports the number of records for this EF.*/
    Int8                    numberOfRecords;
    /** specifies the structure of the file (Linear fixed, transparent, or cyclic) */
    SimEfStructure          efStructure;
    /** Hold the security attributes for this file (what is the access condition
     * to read/update, etc)
     */
    SimUiccSecAttributes    secAttributes;
    /** specifies in which format the security attributes are specified
\assoc UNION \ref secAttributes */
    SimUiccSecAttributesFormat format;
    /** Specifies whether different logical channels may share the access to this file.*/
    Boolean                 fileIsShareable;
    /** The total file size.*/
    Int16                   totalFileSize;
    /** Specifies the Short File Identifier for this file.*/
    SimUiccSfi              sfi;
    /** specifies the type of file (DF/ EF/ ADF USIM, etc */
    SimFileType             fileType;
    /** Indicate whether the INCREASE command can be performed on this EF.*/
    Boolean                 increaseAllowed;
    /** Specifies whether this file can still be accessed when invalidated.*/
    Boolean                 readWriteWhenInvalid;
}
SimEfStatusData;

/** Holds the group identifier */
typedef struct SimGroupIdDataTag
{
    /** The number of IDs
\assoc ARRAY \ref gids */
    Int8            numIds;
    /** Lists GID s*/
    Int8            gids[SIM_MAX_GID_SIZE];
} SimGroupIdData;

/** Displays SPN */
typedef struct SimSpnDataTag
{
    /** indicates whether the registered PLMN should be displayed
     * (along with the SPN)
     * when the registered PLMN is either the HPLMN or a PLMN
     * listed in the SPDI*/
    SimSpnDisplayCond   displayCond;
    /** For SIMs compliant to release 99 or earlier,
     * this second condition will not be specified,
     * so in this case, the display of the SPN when
     * roaming is optional*/
    Boolean             displayCond2Present;
    SimSpnDisplayCond2  displayCond2; /**< indicates whether the SPN should be displayed
                                       * when the registered PLMN is neither the HPLMN nor one
                                       * of the PLMNs listed in the SPDI*/
    /** The size of SPN
\assoc ARRAY \ref spn */
    Int8                spnSize;
    /** Lists SPN */
    Int8                spn[SIM_MAX_SPN_SIZE];
} SimSpnData;


/*EONS structures*/

/** This enumeration holds the type of coding for the PNN data  7 bit packed or USC2.*/
typedef enum SimPnnCodingTag
{
  SIM_PNN_DEFAULT = 0,    /**< 7 bit packed*/
  SIM_PNN_UCS2    = 0x10  /**< USC2 coding*/
} SimPnnCoding;

/** Decoded details of the SimFullName */
typedef struct SimFullNameTag
{
  /** SIM full name */
  Int8             name[ SIM_PLMN_NAME_FULL_LENGTH ];
  /** Whether to use country initials */
  Boolean          addCountryInitials;
  /** Describing the coding scheme */
  SimPnnCoding     coding;

} SimFullName;

/** Decoded details of the SimAbbrName */
typedef struct SimAbbrNameTag
{
  /** SIM abbreviated name */
  Int8             name[ SIM_PLMN_NAME_ABBR_LENGTH ];
  /** Whether to use country initials */
  Boolean          addCountryInitials;
  /** Describing the coding scheme */
  SimPnnCoding     coding;

} SimAbbrName;

/** Decoded details of the SimPlmnName */
typedef struct SimPlmnNameTag
{
  /** Full network name (long)  */
  SimFullName             full;
  /** Abbreviated network name (short)   */
  SimAbbrName             abbr;
}
SimPlmnName;

/** Decoded details of the OPL data read from EF OPL */
typedef struct SimOperatorListTag
{
  /** SIM PLMN codes */
  SimPlmn               simPlmn;
  /** Indicate the entire range of LACs for the given PLMN (first value ) */
#if defined (UPGRADE_LTE)
  /** for LTE Mode this is the entire range of TACs*/
#endif
  Lac                   lac1;
  /** Indicate the entire range of LACs for the given PLMN (first value ) */
#if defined (UPGRADE_LTE)
  /** for LTE Mode this is the entire range of TACs*/
#endif
  Lac                   lac2;
  /** Identifier of operator name to be displayed */
  Int8                  pnnId;
}
SimOperatorList;

/** Used to pass a list of valid Operator List data (OPL) read from the  EF OPL.
 * Contains a prioritised list of Location Area Information (LAI)
 * identities that are used to associate a specific operator name contained
 * in EFPNN with the LAI.
 */
typedef struct SimOplDataTag
{
  /** The number of OPLs */
  Int8                     numOpls;
  /** Lists valid OPLs */
  SimOperatorList          opl[SIM_OPL_LIST_SIZE];
}
SimOplData;

/** Decoded details of the OPL data read from EF OPL.
 * Contains the full and short form versions of the network name for
 * the registered PLMN. */
typedef struct SimNetworkNameListTag
{
  /** The index number */
  Int8                  indexNumber;
  /** Full name and Short name for network */
  SimPlmnName           name;
}
SimNetworkNameList;

/** Used to pass a list of valid PLMN Network Names  (PNN)  read from the  EF PNN */
typedef struct SimPnnDataTag
{
  /** The number of PNNs */
  Int8                     numPnns;
  /** Holds the list of valid PLMN Network Names */
  SimNetworkNameList       pnn[SIM_PNN_LIST_SIZE];
}
SimPnnData;



/*
**  The following definitions are required for Phase 2+ SIM implementation.
**  Note: this does not include SIM Application Toolkit features, as these are
**  defined separately in ALSA_SIG.H.
*/
/** Priority levels allowin/not allowing an automatic answer from the mobile station*/
typedef enum SimAaemPriorityTag
{
  /** Automatic answer with priority levels is allowed for incoming calls */
  SIM_AAEM_NOT_ALLOWED,
  /** Automatic answer with priority levels is not allowed for incoming calls */
  SIM_AAEM_ALLOWED
}
SimAaemPriority;

/** Indicates whether or not the eMLPP service priority levels are
 * subscribed to (i.e available or not). It is also an indication
 * as to which fast call set-up and Aaem priority levels are allowed or not.
 */
typedef enum SimEmlppPriorityTag
{
  /** eMLPP service priority levels not available */
  SIM_EMLPP_NOT_AVAILABLE,
  /** eMLPP service priority levels available */
  SIM_EMLPP_AVAILABLE
}
SimEmlppPriority;

/** Indicates for which of the AAeM priority levels the MS can allow
 * the automatic answer of incoming calls.
 */
typedef struct SimAaemPriorityLevelTag
{
  /** Automatic answer priority for priority level A */
  SimAaemPriority         priorityLevelA;
  /** Automatic answer priority for priority level B */
  SimAaemPriority         priorityLevelB;
  /** Automatic answer priority for priority level 0 */
  SimAaemPriority         priorityLevel0;
  /** Automatic answer priority for priority level 1 */
  SimAaemPriority         priorityLevel1;
  /** Automatic answer priority for priority level 2 */
  SimAaemPriority         priorityLevel2;
  /** Automatic answer priority for priority level 3 */
  SimAaemPriority         priorityLevel3;
  /** Automatic answer priority for priority level 4 */
  SimAaemPriority         priorityLevel4;
}
SimAaemPriorityLevel;

/** Defines the priority levels for the automatic answer of eMLPP service
 * for which the MS should automatically answer incoming calls.
 */
typedef struct SimAaemDataTag
{
  /** Priority level */
  SimAaemPriorityLevel    aaemPriorityLevel;
}
SimAaemData;

/** Part of NIA file manipulation.
 * Defines the category of alerting for terminating traffic.
 *
 * \note There is no definition provided for the NIA information in
 * the latest GSM specifications. Int8 included to avoid empty structure.  TBD..
 */
typedef struct SimAlertingCategoryTag
{
  /** Not used */
  Int8    notUsedByAnything;
}
SimAlertingCategory;

/** Holds the list of preferred CBMID identifiers.*/
typedef struct SimCbmidListTag
{
  /** the number of CBMIDs
\assoc ARRAY \ref cbmid */
  Int8                  numCbmids;
  /** Lists preferred CBMID */
  Int16                 cbmid[SIM_CBMID_LIST_SIZE];
  /** Next field to fetch */
  Int8                  nextField;
}
SimCbmidList;

/** Holds the upper and lower limit range data for each CBMIR identifier.*/
typedef struct SimCbmiRangeTag
{
  /** The lower value of a cell broadcast range */
  Int16                 lowerValue;
  /** The upper value of a cell broadcast range */
  Int16                 upperValue;
}
SimCbmiRange;

/** Holds the list of CBMIR identifiers the subscriber wishes the MS to accept.*/
typedef struct SimCbmirListTag
{
  /** The nuumber of CBMIRs */
  Int8                  numCbmirs;
  /** Lists of CBMIR identifiers */
  SimCbmiRange          cbmir[SIM_CBMIR_LIST_SIZE];
}
SimCbmirList;

/** Contains the Co-operative Network List for the multiple
 * network personalization services.
 * Holds the MCC and MNC (as part of the PLMN structure),
 * network subset, service provider ID and corporate ID of
 * co-operative networks for each element of the co-operative network list.
 */
typedef struct SimCooperativeNetListTag
{
  /** Holds the MCC and MNC of co-operative networks */
  SimPlmn               simPlmn;
  /** PLMN network subset of co-operative networks */
  Int8                  networkSubset;
  /** Service provider ID of co-operative networks */
  Int8                  serviceProviderId;
  /** corporate ID of co-operative networks */
  Int8                  corporateId;
}
SimCooperativeNetList;

/** Defines the CNL data. */
typedef struct SimCnlDataTag
{
  /** The numnber of CNLs */
  Int8                      numCnls;
  /** CNL for the multiple network personalization services  */
  SimCooperativeNetList     cnl[SIM_CNL_LIST_SIZE];
}
SimCnlData;

/** Defines the SimFileStatus data. */
typedef struct SimFileStatusTag
{
  Boolean                       present;
  Boolean                       ContentPresent;
  /** file FCP */
  SimDataBlock                  fileFcp;
  /** file content  */
  SimDataBlock                  fileContent;
}
SimFileStatus;

/** Provides storage for the de-personalization control keys.
 * Holds the DCK data associated with the OTA de-personalization cycle.
 * The Boolean fields determine whether the personalization indicators are
 * set to <B> on </B> or <B> off </B>, and should be initialised to <B> off </B>.
 * The status of these indicators needs to be checked before any
 * personalization/de-personalization takes place */
typedef struct SimDckDataTag
{
  /** Indicates whether the network personalization indicators in on
\assoc PRESENT \ref netDckDigit */
  Boolean               networkDckValid;
  /** Indicates whether the network subset personalization indicators in on
\assoc PRESENT \ref netSubDckDigit */
  Boolean               networkSubsetDckValid;
  /** Indicates whether the service provider personalization indicators in on
\assoc PRESENT \ref spDckDigit */
  Boolean               serviceProviderDckValid;
  /** Indicates whether the corporate personalization indicators in on
\assoc PRESENT \ref corporateDckDigit */
  Boolean               corporateDckValid;
  /** Holds the network control keys required for de-personalisation
   * of the SIM (where SIM_MAX_NUM_DCK_DIGITS denotes the maximum number
   * of digits which may be present in the control key, currently defined
   * to be eight digits)
   */
  Int8                  netDckDigit[SIM_MAX_NUM_DCK_DIGITS];
  /** Holds the network subset  control keys required for de-personalisation
   * of the SIM (where SIM_MAX_NUM_DCK_DIGITS denotes the maximum number
   * of digits which may be present in the control key, currently defined
   * to be eight digits)
   */
  Int8                  netSubDckDigit[SIM_MAX_NUM_DCK_DIGITS];
  /** Holds the service provider  control keys required for de-personalisation
   * of the SIM (where SIM_MAX_NUM_DCK_DIGITS denotes the maximum number
   * of digits which may be present in the control key, currently defined
   * to be eight digits)
   */
  Int8                  spDckDigit[SIM_MAX_NUM_DCK_DIGITS];
  /** Holds the corporate  control keys required for de-personalisation
   * of the SIM (where SIM_MAX_NUM_DCK_DIGITS denotes the maximum number
   * of digits which may be present in the control key, currently defined
   * to be eight digits)
   */
  Int8                  corporateDckDigit[SIM_MAX_NUM_DCK_DIGITS];
}
SimDckData;

/** Array of type Int8 */
typedef Int8 SimEccNumber[SIM_MAX_NUM_ECC_DIGITS];

/** Contains emergency call codes */
typedef struct SimEccRecordTag
{
   /** Emergency call code number */
   SimEccNumber          number;
   /** Is an informative text string that may be displayed to the user*/
   SimAlphaIdentifier    alphaId;
   /** Indicates the type of emergency call code, and this may be sent to the network.*/
   Int8                  serviceCategory;
} SimEccRecord;

/** Defines the list of possible emergency call codes held
 * in the ECC file of the SIM.
 * This does not contain all the ECCs that may be on a 3G
 * USIM Application  it is a list of some or all of the ECCs
 * if there are further ECCs that have not been read the field
 * nextStartField is filled.
 */
typedef struct SimEccListTag
{
  /** The number of Emergency Call Code
\assoc ARRAY \ref ecc */
  Int8                  numEccs;
  /** The total number of ECC in SIM */
  Int8                  totalEccsOnSim;
  /** Lists emergency call codes */
  SimEccRecord          ecc[SIM_MAX_ECC_NUMS];
  /** Next field to fetch */
  Int8                  nextStartField;
}
SimEccList;


/** This indicates which of the eMLPP priority levels are subscribed to */
typedef struct SimEmlppPriorityLevelTag
{
  /** Enhanced Multi Level Precedence and Pre-emption for priority level A */
  SimEmlppPriority        priorityLevelA;
  /** Enhanced Multi Level Precedence and Pre-emption for priority level B */
  SimEmlppPriority        priorityLevelB;
  /** Enhanced Multi Level Precedence and Pre-emption for priority level 0 */
  SimEmlppPriority        priorityLevel0;
  /** Enhanced Multi Level Precedence and Pre-emption for priority level 1 */
  SimEmlppPriority        priorityLevel1;
  /** Enhanced Multi Level Precedence and Pre-emption for priority level 2 */
  SimEmlppPriority        priorityLevel2;
  /** Enhanced Multi Level Precedence and Pre-emption for priority level 3 */
  SimEmlppPriority        priorityLevel3;
  /** Enhanced Multi Level Precedence and Pre-emption for priority level 4 */
  SimEmlppPriority        priorityLevel4;
}
SimEmlppPriorityLevel;

/** Defines the priority levels and fast call set-up conditions for the eMLPP service */
typedef struct SimEmlppDataTag
{
  /** Specifies EMLPP priority level */
  SimEmlppPriorityLevel       emlppPriorityLevel;
  /** Fast call set-up conditions */
  SimEmlppPriorityLevel       fastCallSetUpCondition;
}
SimEmlppData;

/** Holds a list of the preferred extended language preferences in order of priority */
typedef struct SimExtLangPrefListTag
{
  /** The number of entries in the list
\assoc ARRAY \ref extLanguage */
  Int8          numEntries;
  /** First entry in the list */
  Int16         firstEntry;
  /** Lists the preferred extended language */
  Int16         extLanguage[SIM_MAX_ELP_LIST_SIZE];
  /** Total number of entries*/
  Int16          totalNumEntries;
}
SimExtLangPrefList;

/** Defines the type of terminating traffic associated with the related alerting category.
 * Part of NIA file manipulation.
 *
 * There is no formal definition within GSM
 * for this type yet.  Int8 included to avoid empty structure.  TBD... */

typedef struct SimInformativeTextTag
{
  /** Not used */
  Int8    notUsedByAnything;
}
SimInformativeText;

/** Stores the record number of the short message status reports
 * held in the SMSR file and the record number of the corresponding
 * short message held in the SMS file.
 */
typedef struct SimSmsrRecordListTag
{
  /** The record number of the short message */
  Int8          smsRecordNum;
  /** The record number of the short message status reports */
  Int8          smsrRecordNum;
  /** msgRef to identify SM to Service Centre (SC) */
  Int8          msgRef;
  /** Address of the recipient of the previously submitted mobile originated short message */
  SmsAddress    recipientAddr;
}
SimSmsrRecordList;

/** Defines both the number and identifier value of the group
 * identifiers the user has subscribed to
 */
typedef struct SimVoiceServiceDataTag
{
  /** The number of the group identifiers */
  Int8          numGroupIds;
  /** The identifier value of the group identifiers */
  Int32         gids[SIM_MAX_VOICE_SERVICE_GID_SIZE];
}
SimVoiceServiceData;

/** Definition of the decoded data from  Voice Service Status File
 * either EF_VGCSS(6fb2) or  EF_VBSS (6fb4)
 */
typedef struct SimVoiceServiceStatusDataTag
{
  /** The number of VGCS or VBS group identifiers the user has
   * subscribed to, having a maximum value of fifty identifiers
   */
  Int8          numGroupIds;
  /** Contains the group data */
  Boolean       gids[SIM_MAX_VOICE_SERVICE_GID_SIZE];
}
SimVoiceServiceStatusData;



/** Used to pass the configuration data for Enhanced Network Support
 * handling to the USIM Manager.
 * Sets up which enahanced network selection features are supported in the terminal.
 */
typedef struct SimEnhNtwkTermSuppProfileTag
{
   /** Scan control application support */
   Boolean            scanCntrlAppSupp;
   /** Network selection menu control application support */
   Boolean            ntwkSelMenuCntrlAppSupp;
   /** Actual HPLMN rescan feature support */
   Boolean            actHplmnRescanFeatureSupp;
   /** If 3G is supported */
   Boolean            supportFor3g;
}
SimEnhNtwkTermSuppProfile;

#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
/************************
 ** CPHS related types **
 ************************/

/** Service Table information giving the state of CPHS files on the SIM.
 * The order of the services in the following structure must be the
 * same as the order in the SIM.
 */
typedef struct SimCphsServiceTableTag
{
  /** SIM service for CSP */
  SimService    csp;
  /** SIM service for CSP */
  SimService    sst;
  /** Mail box number */
  SimService    mailboxNumbers;
  /** The short form operator name */
  SimService    opNameShortForm;
  /** Information number */
  SimService    infoNumbers;
}
SimCphsServiceTable;

/** CPHS operator name */
typedef struct SimCphsOperatorNameTag
{
  /** The length of the name
\assoc PRESENT \ref name */
  Int8          nameSize;
  /** The operator name */
  Int8          name[SIM_CPHS_MAX_OP_NAME_SIZE];
}
SimCphsOperatorName;

/** Short form of CPHS operator name */
typedef struct SimCphsOperatorNameShortTag
{
  /** The length of the name
\assoc PRESENT \ref name */
  Int8          nameSize;
  /** The short form operator name */
  Int8          name[SIM_CPHS_MAX_OP_NAME_SHORT_SIZE];
}
SimCphsOperatorNameShort;

/** CPHS mail box numbers element */
typedef struct SimCphsMailboxNumbersTag
{
  /** Indicates if Mailbox line1 contains valid data
\assoc PRESENT \ref mnLine1 */
  Boolean                         mnLine1DataValid;
  /** Indicates if Mailbox line2 contains valid data
\assoc PRESENT \ref mnLine2 */
  Boolean                         mnLine2DataValid;
  /** Indicates if Mailbox fax contains valid data
\assoc PRESENT \ref mnFax */
  Boolean                         mnFaxDataValid;
  /** Indicates if Mailbox data contains valid data
\assoc PRESENT \ref mnData */
  Boolean                         mnDataDataValid;
  /** Mailbox Line 1 */
  SimAbbreviatedDialNumber        mnLine1;
  /** Mailbox Line 2 */
  SimAbbreviatedDialNumber        mnLine2;
  /** Mailbox fax */
  SimAbbreviatedDialNumber        mnFax;
  /** Mailbox data*/
  SimAbbreviatedDialNumber        mnData;
}
SimCphsMailboxNumbers;

/** CPHS voice message waiting flags element */
typedef struct SimCphsVoiceMsgWaitingFlagsTag
{
  /** Flag indicating the validity of the Fax and Data flags.*/
  Boolean       faxAndDataVmwfPresent;  /* flags are optional byte on SIM */
  /** Flag indicating whether there is a message waiting for Line 1.*/
  Boolean       msgWaitingLine1;
  /** Flag indicating whether there is a message waiting for Line 2. */
  Boolean       msgWaitingLine2;
  /** Flag indicating whether there is a message waiting for the Fax Line.*/
  Boolean       msgWaitingFax;
  /** Flag indicating whether there is a message waiting for the Data Line.*/
  Boolean       msgWaitingData;
}
SimCphsVoiceMsgWaitingFlags;

/** CPHS call forward flags element */
typedef struct SimCphsCallForwardFlagsTag
{
  /** Flag indicating the validity of the Fax and Data flags.*/
  Boolean       faxAndDataCffPresent;       /* flags are optional byte on the SIM */
  /** Flag indicating whether all calls to Line 1 are unconditionally forwarded.*/
  Boolean       callFwdUncondStatusLine1;
  /** Flag indicating whether all calls to Line 2 are unconditionally forwarded.*/
  Boolean       callFwdUncondStatusLine2;
  /** Flag indicating whether all calls to the Fax Line are unconditionally forwarded.*/
  Boolean       callFwdUncondStatusFax;
  /** Flag indicating whether all calls to Data Line are unconditionally forwarded. */
  Boolean       callFwdUncondStatusData;
}
SimCphsCallForwardFlags;

/** CPHS CSP service group element */
typedef enum SimCphsCspServiceGroupTag
{
    /** Undefined */
    SIM_CPHS_CSP_UNDEFINED            = 0x00,
    /** CSP call offering */
    SIM_CPHS_CSP_CALL_OFFERING        = 0x01,
    /** CSP call restriction */
    SIM_CPHS_CSP_CALL_RESTRICTION     = 0x02,
    /** CSP other supplementary services */
    SIM_CPHS_CSP_OTHER_SUPP_SERV      = 0x03,
    /** CSP call completion */
    SIM_CPHS_CSP_CALL_COMPLETION      = 0x04,
    /** CSP teleservices */
    SIM_CPHS_CSP_TELESERVICES         = 0x05,
    /** CSP CPHS teleservices */
    SIM_CPHS_CSP_CPHS_TELESERVICES    = 0x06,
    /** CSP CPHS features */
    SIM_CPHS_CSP_CPHS_FEATURES        = 0x07,
    /** CSP number identifier */
    SIM_CPHS_CSP_NUMBER_IDENTIF       = 0x08,
    /** CSP phase 2+ services */
    SIM_CPHS_CSP_PHASE_2P_SERVICES    = 0x09,
    /** CSP value added service */
    SIM_CPHS_CSP_VALUE_ADDED_SERV     = 0xC0,
    /** Information numbers*/
    SIM_CPHS_CSP_INFORMATION_NUMS     = 0xD5

} SimCphsCspServiceGroup;

/** CPHS CSP entry */
typedef struct SimCphsCspEntryTag
{
    /** Specifies the service group */
    SimCphsCspServiceGroup       serviceGroup;
    /** The number of enabled services */
    Int8                         enabledServices;

} SimCphsCspEntry;

/** CPHS customer service profile */
typedef struct SimCphsCustServProfileTag
{
    /** The number of CSP in the lst
\assoc ARRAY \ref cspEntries */
    Int8                         numCspEntries;
    /** Lists the CSP */
    SimCphsCspEntry              cspEntries[SIM_CPHS_MAX_CSP_ENTRIES];

} SimCphsCustServProfile;


/** Encode Entry status */
typedef struct SimCphsInfoNumEntryStatusTag
{
    /** Network specific indicator */
    Boolean                      networkSpecificIndicator;
    /** Premium service indicator */
    Boolean                      premiumServiceIndicator;
    /** Index level */
    Int8                         indexLevel;

} SimCphsInfoNumEntryStatus;

/** Indicate which member of the SimCphsInfoNumData union is valid.*/
typedef enum SimCphsInfoNumTypeTag
{
    /** Number Entry */
    SIM_CPHS_INFO_NUM_NUMBER_ENTRY,
    /** Index Entry */
    SIM_CPHS_INFO_NUM_INDEX_ENTRY,
    /** The type of number entry */
    SIM_CPHS_INFO_NUM_NUMBER_TYPES

} SimCphsInfoNumType;

/** CPHS information number of type number entry */
typedef struct SimCphsInfoNumNumberEntryTag
{
    /** The type of number */
    BcdNumberType                typeOfNumber;
    /** Specifies numbering plan */
    BcdNumberPlan                numberPlan;
    /** The number of valid bytes within the dialString array
\assoc ARRAY \ref dialString */
    Int8                         dialStringLength;
    /** Indication that not all of the Dial String stored in
     * the record and possible extension files would fit in the signal
     */
    Boolean                      dialStringTruncated;
    /** The dial string in packed BCD format.
     * Two BCD digits are coded into each byte with the top nibble
     * in byte 1 being digit 1 and the bottom nibble in byte 1 being digit 2.
     * This is the reverse of the coding on the SIM. Unused nibbles are set to all ones.
     */
    Bcd                          dialString[SIM_CPHS_MAX_INFONUM_DIALSTR];

} SimCphsInfoNumNumberEntry;

/** CPHS information number of type index entry */
typedef struct SimCphsInfoNumIndexEntryTag
{
    /** Indication of whether the Index Entry has a record number
     * for the next item at the same index level
     * (This is an optional feature within this Information Number file)
     */
    Boolean                      nextItemValid;
    /** The record number of the next item in the Information
     * Numbers file that has the same index level
     * (Only valid if nextItemValid equals TRUE)
     */
    Int8                         nextItem;

} SimCphsInfoNumIndexEntry;

/** Whethe the number or the index of the entry in Information file is used */
typedef union SimCphsInfoNumDataTag
{
    /** The number of the entry */
    SimCphsInfoNumNumberEntry    numberEntry;
    /** The index of the entry */
    SimCphsInfoNumIndexEntry     indexEntry;

} SimCphsInfoNumData;

/** CPHS information number element */
typedef struct SimCphsInfoNumEntryTag
{
    /** The data decoded from the Entry status byte, including Index level */
    SimCphsInfoNumEntryStatus    status;
    /** The length of the alphaId field.
\assoc ARRAY \ref alphaId */
    Int8                         alphaLength;
    /** Indication that not all of the Alpha Id stored in the record
     * would fit in the signal
     */
    Boolean                      alphaIdTruncated;
    /** the alpha id coded using the SMS alphabet */
    Int8                         alphaId[SIM_CPHS_MAX_INFONUM_ALPHAID];
    /** Indication of whether this Information Number is an Index or Number
\assoc UNION \ref data
\assoc UNION \ref data */
    SimCphsInfoNumType           type;
    /** Contains valid data for either an Index or Number entry depending
     * on the value of the type field
     */
    SimCphsInfoNumData           data;

} SimCphsInfoNumEntry;
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */

/** Specifies the type of record */
typedef enum SimRecordTag
{
    /** Record undefined */
    SIM_REC_UNDEFINED,
    /** Record unknown */
    SIM_REC_UNKNOWN,
    /** Record known */
    SIM_REC_KNOWN
}
SimRecord;

/** Stores data corresponding to each logical channel. */
typedef struct SimUiccAppDataTag
{
  SimUiccAid           aid;                /**< USIM Application Id */
  SimUiccAidLabel      label;              /**< Label */
  /*efDirRecordNum must be kept as this information is stored in EF HIDDEN KEY*/
  Int8                 efDirRecordNum;     /**< Record in EF DIR where this USIM application is referenced*/
  Int16                currentDir;         /**< The currently selected MF or DF    */
  Int16                currentEf;          /**< The currently selected Ef          */
  Int16                subDirectory;       /**< Used if EF is 4fxx when cannot definitively identify Subdir*/
  SimEfStatusData      currentEfStatus;    /**< The status of the current EF       */
  SimDirStatusData     currentDirStatus;   /**< The status of the current DF       */
  Int8                 currentDirStatusSize; /**< the size of status for the current DF       */
  SimRecord            currSimRecord;        /**< The record of the current SIM      */
  SimCommandStatus     cmdStatus;           /**< status of the command */
  SimUiccKeyRefValue   pin1;                /**< Determines the application PIN key reference to use. */
                                            /**< The app can support a universal PIN, a level1 app PIN, and possibly many level 2 APP PIN */

  /** Storage for the status decoded from the SIM status words returned after
   * processing a command
   */
  SimSwStatusCode      simStatus;
  /**
   * Storage for the status decoded from the SIM status words returned after
  ** processing a get response command. The GET RESPONSE command function
  ** is the only command which is called from another command function after
  ** the main command has been performed, hence the need for two
  ** SimSwStatusCode storage areas.
  */
  SimSwStatusCode       respStatus;
  SimVoltageCapab       simVoltageCapab;    /**< Voltage supported by the SIM card.  */
  /*-----------------19/12/01 17:20-------------------
   * Parameters to use for the SIM commands
   * --------------------------------------------------*/
  SimClassByte          classByte;           /**< class byte used in the SIM command (00, A0, 8X) */
  SimUiccSelectMode     mode;                /**<  Mode */
  Boolean               useSfi;              /**<  Whether we should use sfi or not*/
  SimDirId              rootDir;             /**<  Whether file is under USIM directory */
  Int16                 adfApplFileId;       /**<  AID's real application file id */
} SimUiccAppData;

/** Ncp-Ip data structures and defines */
typedef enum SimNcpIpAddressTypeTag
{
    /** IPv4 */
    SIM_NCPIP_ADDRESS_TYPE_IPV4       = 0x21,
    /** IPv6 */
    SIM_NCPIP_ADDRESS_TYPE_IPV6       = 0x57
}
SimNcpIpAddressType;

typedef struct SimUiccDataDestAddrRangeTag
{
    Int8                length;       // byte length
    SimNcpIpAddressType addressType;
    Int8                prefixLength; // prefix bit length
    Int8                prefixLengthBytes; // prefix byte length
    Int8                prefix[16];
} SimUiccDataDestAddrRange;


typedef struct SimUiccAccessPointNameTag
{
    Int8                length;
    Int8                data[100];
}
SimUiccAccessPointName;

typedef struct SimUiccLoginTag
{
    Int8                length;
    Int8                data[50];
}
SimUiccLogin;

typedef struct SimUiccPasswordTag
{
    Int8                length;
    Int8                data[50];
}
SimUiccPassword;

typedef enum SimNcpIpBearerDescTypeTag
{
    /** default bearer */
    SIM_NCPIP_DEFAULT_BEARER       = 0x03,
    /** local bearer */
    SIM_NCPIP_LOCAL_BEARER         = 0x04,
    /** I-WLAN */
    SIM_NCPIP_IWLAN_BEARER         = 0x0A,
    /** E-UTRAN */
    SIM_NCPIP_EUTRAN_BEARER        = 0x0B,
    /** USB */
    SIM_NCPIP_USB_BEARER           = 0x10
}
SimNcpIpBearerDescType;

typedef struct SimUiccBearerDescTag
{
    Int8                   count;
    SimNcpIpBearerDescType bearer[5];
}
SimUiccBearerDesc;

/** \defgroup SigAlsiSignals  ALSI Primitives
 * Primitives exchanged between  USIM Manager and the Application Layer.
 * This section defines primitives exchanged between USIM Manager and the Application Layer.
 *
 * @{
 */
/****************************************************************************
 * Signal types
 ****************************************************************************/

/** This is used to start a USIM application.
 * Request to select a USIM application and open a 3G session.
 * It can only be used when a UICC card is inserted in the ME.
 * The AID of the USIM application can be obtained by reading the
 * record number indicated by recordNum in EF DIR.
 *
 * When receiving this signal the USIM manager terminates the
 * current USIM application, and selects the new one.
 *
 * The application layer is however informed whether the operation
 * was successful, as a AlsiAppStartedInd is sent shortly after.
 */
typedef struct AlsiUsimApplicationStartReqTag
{
    /** Command reference */
    Int16    commandRef;
    /** Record number of EF_DIR (indicates the selected USIM application)*/
    Int8     recordNum;
} AlsiUsimApplicationStartReq;

/** Confirmation to the AlsiUsimApplicationStartReq.*/
typedef struct AlsiUsimApplicationStartCnfTag
{
    /** Command reference copied from AlsiReadDirReq */
    Int16                           commandRef;
    /** Indicates the status of the request */
    SimRequestStatus                requestStatus;
     /** Record number of EF_DIR (indicates the selected USIM application)*/
    Int8                            recordNum;
}
AlsiUsimApplicationStartCnf;

/** This signal will cause the USIM manager to read EF DIR to find out the list of USIM
 * applications present on the SIM card.
 * Request to read EF DIR file (to find out about the applications supported by the UICC).
 * This request can only be used when a UICC card is inserted in the ME. */
typedef struct AlsiReadDirReqTag
{
    /** Command reference */
    Int16                           commandRef;
    /** Defines the first record to read */
    Int8                            recordNum;
    /** The number of desired entries to read */
    Int8                            numEntriesDesired;
} AlsiReadDirReq;

/** Confirmation to the AlsireadDirReq.
 * This signal lists all the applications present on the SIM card.
 * Several AlsiApplicationListReq can be sent if the number of applications exceeds 5*/
typedef struct AlsiReadDirCnfTag
{
    /** Command reference copied from AlsiReadDirReq */
    Int16                           commandRef;
    /** Indicates the status of the request */
    SimRequestStatus                requestStatus;
    /** The total number of entries in EF_DIR */
    Int8                            totalEntries;
    /** The record number */
    Int8                            recordNum;
    /** The list of applications */
    SimUiccApplicationList          applicationList;
}
AlsiReadDirCnf;

/** Indicates that a SIM has been inserted or that a SIM is present at power on  */
typedef struct AlsiAppStartedIndTag
{
    /** card inserted status.
     * If the status field of the AlsiSimInsertedInd signal equals
     * SIM_INSERTED_UNSUPPORTED _VOLTAGE then the simVoltage and
     * simVoltageCapab fields are valid, all other fields are invalid
     * The other fields within the signal are only valid if the returned
     * status is SIM_INSERTED_OK.
     */
    SimInsertStatus                 status;
    /** TRUE if 3G card*/
    Boolean                         cardIsUicc;
    /** phase (SIM PHASE 2, PHASE 2+, or 3G) */
    SimPhase                        phase;
    /** bytes free on the SIM */
    Int16                           simFreeMemory;
    /** list of emergency call codes available */
    SimEccList                      emergencyCallCodes;
    /** extended language preference list */
    SimExtLangPrefList              extLangPrefList;
    /** prefered language list */
    LanguagePrefList                languagePrefList;

    /** PIN1 status data */
    SimUiccPinStatus                pin1Status;
    /** PIN2 status data.
     * If PIN2Status indicates that PIN2 is not initialised, PIN2 commands (e.g. verify PIN2)
     * and related commands should not be requested by the application layer.
     */
    SimUiccPinStatus                pin2Status;
    /** Indicates if the UICC support universal PIN */
    Boolean                         universalPinSupportedByCard;
    /** Universal Pin status data */
    SimUiccPinStatus                universalPinStatus;
    /* UNBLOCK PIN1 status data */
    SimUiccPinStatus                unblockPin1Status;
    /* UNBLOCK PIN2 status data                         */
    /** PIN2 relating to ADF_USIM directory for 3G cards */
    SimUiccPinStatus                unblockPin2Status;
    /** UNBLOCK PIN2 status data */
    SimUiccPinStatus                unblockUniversalPinStatus;
    /** Key reference value for PIN1 */
    SimUiccKeyRefValue              pin1KeyReference;
    /** Key reference value for PIN2 */
    SimUiccKeyRefValue              pin2KeyReference;
    /** Whether the universal PIN should be verified or not
     * (This depends upon whether the application PIN is also enabled,
     * and also depends whether the universal PIN has already been verified
     * in a previous session)
     */
    Boolean                         verifyUniversalPin;
    /** SIM Operating Voltage */
    SimVoltage                      simVoltage;
    /** SIM Voltage Capability */
    SimVoltageCapab                 simVoltageCapab;
    /** Record number of EF_DIR (indicates the started USIM application)*/
    Int8                            aidRecordNum;
    /** Info about the started application*/
    SimUiccAidLabel                 applicationLab;

}
AlsiAppStartedInd;

/** Indicates that a SIM has been inserted or that a SIM is present at power on  */
typedef struct AlsiAppStoppedIndTag
{
    /** The record number of the application that was running*/
    Int8                            recordNum;
    /** Info about the stopped application*/
    SimUiccAidLabel                 applicationInfo;
}
AlsiAppStoppedInd;

/** Indicates that the SIM/UICC has been removed. This signal may
 * also be sent if no response is received to a regular STATUS request.
 * This status request is performed at least every 30 seconds, by the USIM manager.
 */
typedef struct AlsiCardRemovedIndTag
{
    /** Sim removed cause */
    SimRemovedCause                 cause;
}
AlsiCardRemovedInd;

/** A request for SIM/or USIM initialisation.
 * The AlsiAppInitialiseReq requests the USIM manager to initialise the
 * USIM application. (or the GSM application if a GSM SIM was inserted).
 * This signal must be sent after the AlsiAppStartedInd has been received
 * (with status = SIM_INSERTED_OK). It may also be sent each time a new
 * USIM application is starting, resulting in the USIM manager resetting internal
 * variables.
 */

typedef struct AlsiAppInitialiseReqTag
{
    /** Command reference */
    Int16                          commandRef;
    /** Key ref of the PIN to verify */
    SimUiccKeyRefValue             keyReference;  /* Pin to verify */
    /** Flag to indicate whether to verify PIN 1
     * (set if Pin1 enabled and not blocked)
     */
    Boolean                        verifyPin1;
    /** Flag to indicate whether to unblock the PIN
     * (set if Pin1 enabled and blocked)
     */
    Boolean                        unblockPin1;
    /** PIN1 value */
    SimUiccPinValue                pinValue;
    /** Unblock pin value */
    SimUiccPinValue                unblockPinValue;
    /** Indicates to the USIM manager whether the Application
     * Layer is capable of supporting Fixed Dial Number (FDN) operation
     * (TRUE if it can otherwise FALSE)
     */
    Boolean                        msFdnCapable;
    /** The capabilities of ME for Phase 2+ Sim Toolkit functionality */
    SimTerminalProfile             terminalProfile;
    /** Configuration information on Support for enhanced network features */
    SimEnhNtwkTermSuppProfile      enhNtwkTermSuppProfile;
    /** pcsOperationReqd */
    Boolean                        pcsOperationReqd;
}
AlsiAppInitialiseReq;

/** Confirmation to the AlsiReadNasConfigReq; if the request was successful,
 * it returns the NAS Configuration parameters. This includes all data defined in
 * 3GPP TS 31.102 V14.0.0 (2016-10) specification.
*/
typedef struct AlsiReadNasConfigCnfTag
{
  /** Command reference */
  Int16                     commandRef;
  /** Determines the outcome of the request. */
  SimRequestStatus          requestStatus;
  /** Indicates if NAS signalling priority presents */
  Boolean                   nasSignallingPriorityPresent;
  /** NAS signalling priority */
  Int8                      nasSignallingPriority;
  /** Indicates if NMO I Bihaviour presents */
  Boolean                   nmoIBehaviourPresent;
  /** NMO I Bihaviour */
  Int8                      nmoIBehaviour;
  /** Indicates if Attach with IMSI presents */
  Boolean                   attachWithImsiPresent;
  /** Attach with IMSI */
  Int8                      attachWithImsi;
  /** Indicates if Minimum periodic Search Timer presents */
  Boolean                   minimumPeriodicSearchTimerPresent;
  /** Minimum periodic Search Timer */
  Int8                      minimumPeriodicSearchTimer;
  /** Indicates if Extended access barring presents */
  Boolean                   extendedAccessBarringPresent;
  /** Extended access barring */
  Int8                      extendedAccessBarring;
  /** Indicates if Timer T3245 Behaviour presents */
  Boolean                   timerT3245BehaviourPresent;
  /** Timer T3245 Behaviour */
  Int8                      timerT3245Behaviour;
  /** Indicates if Override NAS signalling low priority presents */
  Boolean                   overrideNasSignallingLowPriorityPresent;
  /** Override NAS signalling low priority */
  Int8                      overrideNasSignallingLowPriority;
  /** Indicates if Override Extended access barring presents */
  Boolean                   overrideExtendedAccessBarringPresent;
  /** Override Extended access barring */
  Int8                      overrideExtendedAccessBarring;
  /** Indicates if Fast First Higher Priority PLMN Search presents */
  Boolean                   fastFirstHigherPriorityPlmnPresent;
  /** Fast First Higher Priority PLMN Search */
  Int8                      fastFirstHigherPriorityPlmn;
  /** Indicates if E-UTRA Disabling Allowed for EMM Cause #15 presents */
  Boolean                   eutraDisablingAllowedPresent;
  /** E-UTRA Disabling Allowed for EMM Cause #15 */
  Int8                      eutraDisablingAllowed;
  /** Indicates if SM Retry Waiting Time presents */
  Boolean                   smRetryWaitingTimePresent;
  /** SM Retry Waiting Time */
  Int8                      smRetryWaitingTime;
  /** Indicates if SM Retry at RAT Change presents */
  Boolean                   smRetryAtRatChangePresent;
  /** SM Retry at RAT Change */
  Int8                      smRetryAtRatChange;
  /** Inducates is Default DCN ID value present */
  Boolean                   defaultDcnIdValuePresent;
  /** Default DCN ID value */
  Int16                     defaultDcnIdValue;
  /** Indicates if Exception Data Reporting Allowed presents */
  Boolean                   exceptionDataReportingAllowedPresent;
  /** Exception Data Reporting Allowed */
  Int8                      exceptionDataReportingAllowed;
  /** Indicates if Light connection present */
  Boolean                   lightConnectionPresent;
  /** Light connection */
  Int8                      lightConnection;
  /* UICC not changed after last power off */
  Boolean                   sameUicc;
}AlsiReadNasConfigCnf;

/** Confirmation for open logical channel request,
 * if the request was successful,
 * it returns the channel number parameter.
 */
typedef struct AlsiOpenLogicalChannelCnfTag
{
  /** Command reference */
  Int16                     commandRef;
  /** Determines the outcome of the request. */
  SimRequestStatus          requestStatus;
  /** Channel number identifier */
  Int8                      channelNumber;
} AlsiOpenLogicalChannelCnf;

/** Confirmation for close logical channel request,
 * Closes already opened logical channel on SIM/UICC.
 */
typedef struct AlsiCloseLogicalChannelCnfTag
{
  /** Command reference */
  Int16                     commandRef;
  /** Determines the outcome of the request. */
  SimRequestStatus          requestStatus;
} AlsiCloseLogicalChannelCnf;

/** Confirmation for general access request,
 * if the request was successful,
 * it returns the response data as raw APDU response with data length.
 */
typedef struct AlsiSimLogicalChannelAccessCnfTag
{
    /** Command Reference */
    Int16                   commandRef;
    /** Sending task ID */
    TaskId                  taskId;
    /** Determines the outcome of the request. */
    SimRequestStatus        requestStatus;
    /** The length of response */
    Int16                   length;
    /** Holds the response */
    Int8                    response[SIM_MAX_MSG_SIZE];
}
AlsiSimLogicalChannelAccessCnf;

typedef struct AlsiReadMsisdnCnfTag
{
    /** Command reference */
    Int16                     commandRef;
    /** record number to be read. */
    Int8                      recordNumber;
    /** Request read status */
    SimRequestStatus          requestStatus;
    /** Number read         */
    SimDialNumber             dialNumber;
    /** Whether there are more numbers to read */
    Boolean                   more;
}
AlsiReadMsisdnCnf;

typedef struct AlsiReadNcpIpCnfTag
{
    /** Command reference */
    Int16                     commandRef;
    /** record number to be read. */
    Int8                      recordNumber;
    /** Request read status */
    SimRequestStatus          requestStatus;
    /* Data Destination Address Range */
    SimUiccDataDestAddrRange  addressRange;
    /* Access point Name */
    SimUiccAccessPointName    accessPointName;
    /* Login */
    SimUiccLogin              login;
    /* password */
    SimUiccPassword           password;
    /* Bearer description */
    SimUiccBearerDesc         bearerDesc;
    /** Whether there are more numbers to read */
    Boolean                   more;
}
AlsiReadNcpIpCnf;

#if defined (FEA_RPM)
typedef struct AlsiReadRpmEnabledCnfTag
{
    /** Command reference */
    Int16                  commandRef;
    /** Request read status */
    SimRequestStatus       requestStatus;
    /** Feature enabled flag */
    Boolean                enabled;
}
AlsiReadRpmEnabledCnf;

typedef struct AlsiReadRpmParametersCnfTag
{
    /** Command reference */
    Int16                  commandRef;
    /** Request read status */
    SimRequestStatus       requestStatus;
    /** Max number of SW reset per Hour by RPM following 'permanent' MM/GMM/EMM reject */
    Int8                   countN1;
    /** Average time before RPM reset modem following permanent MM/GMM7EMM reject */
    Int8                   countT1;
    /** Max number of PDP Activations per Hour allowed by
    RPM following a PDP activation ignore Scenario */
    Int8                   countF1;
    /** Max number of PDP Activations per Hour allowed by
    RPM following a 'Permanent' PDP activation ignore Reject */
    Int8                   countF2;
    /** Max number of PDP Activations per Hour allowed by
    RPM following a 'Temporary' PDP activation ignore Reject */
    Int8                   countF3;
    /** Max number of PDP Activation/Deactivation Requests per Hour allowed by RPM */
    Int8                   countF4;
}
AlsiReadRpmParametersCnf;

typedef struct AlsiReadRpmLeakRateCnfTag
{
    /** Command reference */
    Int16                  commandRef;
    /** Request read status */
    SimRequestStatus       requestStatus;
    /** Leak rate for C-BR-1, defined number of
        Hours before C-BR-1 is decremented by 1. */
    Int8                   leakLR1;
    /** Leak rate for C-R-1, defined number of
        Hours before C-R-1 is decremented by 1. */
    Int8                   leakLR2;
    /** Leak rate for C-PDP-1 to C-PDP-4, defined number of
        Hours before C-PDP-1 to C-PDP-4 is decremented by 1. */
    Int8                   leakLR3;
}
AlsiReadRpmLeakRateCnf;

typedef struct AlsiReadRpmCountersCnfTag
{
    /** Command reference */
    Int16                  commandRef;
    /** Request read status */
    SimRequestStatus       requestStatus;
    /** Indicate number of actions triggered by N1 */
    Int8                   counterCBR1;
    /** Indicate number of actions triggered by T1 */
    Int8                   counterCR1;
    /** Indicate number of actions triggered by F1 */
    Int8                   counterCPDP1;
    /** Indicate number of actions triggered by F2 */
    Int8                   counterCPDP2;
    /** Indicate number of actions triggered by F3 */
    Int8                   counterCPDP3;
    /** Indicate number of actions triggered by F4 */
    Int8                   counterCPDP4;
}
AlsiReadRpmCountersCnf;

typedef struct AlsiWriteRpmCountersCnfTag
{
    /** Command reference */
    Int16                     commandRef;
    /** Indicates the status of the request */
    SimRequestStatus          requestStatus;
}
AlsiWriteRpmCountersCnf;

typedef struct AlsiReadRpmVersionCnfTag
{
    /** Command reference */
    Int16                     commandRef;
    /** Indicates the status of the request */
    SimRequestStatus          requestStatus;
    /** Indicates rhe version of RPM implemented in the device */
    Int8                      version;
}
AlsiReadRpmVersionCnf;

typedef struct AlsiWriteRpmVersionCnfTag
{
    /** Command reference */
    Int16                     commandRef;
    /** Indicates the status of the request */
    SimRequestStatus          requestStatus;
}
AlsiWriteRpmVersionCnf;
#endif /* defined (FEA_RPM) */

/** Confirmation of the AlsiAppInitialiseReq.
 * This confirmation signal returns general administration, network and user
 * data required by the application layer.
 */
typedef struct AlsiAppInitialiseCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** State of intialisation proc */
    SimInitialiseStatus             status;
#if defined (FEA_KOR_LGUP)
    CardType                        cardType;
#endif
    /** card ID number */
    SimIccid                        iccid;
    /** phase (SIM PHASE 2, PHASE 2+, or 3G) */
    SimPhase                        phase;
    /** Administation data */
    SimAdminData                    admin;
    /** International mobile subscriber id */
    Imsi                            imsi;
    /** Preferred access technology from SIM_EF_HPLMNW_ACT*/
    AccessTechnologyId              prefHplmnAccessTech;
    /** Hplmn Search Period, units of minutes (0 - 480) */
    Int16                           hplmnSearchPeriod;
    /** Type of service table (2G or 3G)
\assoc UNION \ref serviceTable */
    ServiceTableType                serviceTableType;
    /** SIM Service Table or USIM Service Table */
    ServiceTable                    serviceTable;
    /** The access control class bits */
    AccessClass                     accessClass;
    /** Preferred PLMN list (read from PLMNwACT or PLMN SEL) */
    SimPlmnList                     plmn;
    /** Location Information (read from EF LOCI) */
    LocationInformation             locInfo;
    /** BCCH Information (only valid on GSM SIMs) */
    SimBaList                       baList;
    /** The forbidden PLMN list */
    SimPlmnList                     forbiddenPlmn;
    /** Total number of FPLMNs */
    Int16                           totalNumFplmns;
    /** Is an actingHplmn file present on SIM
\assoc PRESENT \ref actingHplmn */
    Boolean                         actingHplmnFilePresent;  /*Added KJH*/
    /** Acting home plmn for enhanced network selection features */
    SimPlmn                         actingHplmn;
    /** The CBMID list */
    SimCbmidList                    cbmidList;
    /** The DCK data */
    SimDckData                      dckData;
    /** Flag to indicated whether PIN2 is required to access ACM MAX file */
    Boolean                         pin2RequiredAcmMax;
    /** Flag to indicated whether PIN2 is required to access ACM file */
    Boolean                         pin2RequiredAcm;
    /** Flag to indicated whether PIN2 is required to access PUCT file */
    Boolean                         pin2RequiredPuct;
    /** Fixed dialling number capability enabled */
    Boolean                         fdnEnabled;
    /** Barred dialling number capability enabled */
    Boolean                         bdnEnabled;
    /** Access Point Names control list enabled*/
    Boolean                         aclEnabled;
    /** Whether global phonebook is present*/
    Boolean                         globalPhonebookAvailable;
    /** Set if CPHS is supported on the SIM */
    Boolean                         cphsDataPresent;
#if defined (ENABLE_EHPLMN)
    Int16                           totalNumEhplmns;
    /** list of equivalent Home PLMNs */
    SimPlmnList                     ehplmn;
    /** indication of  whether the presentation Indicator is a valid parameter
\assoc PRESENT \ref ehplmnPi */
    Boolean                         ehplmnPiValid;
    /** value of the presentation Indicator - how the ehplmns should be displayed */
    SimUiccEhplmnDisplayPref        ehplmnPi;
#endif
    /** The key reference */
    SimUiccKeyRefValue              keyRef;
    /** PIN status of the PIN we have tried to verify*/
    SimUiccPinStatus                pinStatus;
    /** PIN status of the unblock PIN we have tried to verify */
    SimUiccPinStatus                unblockPinStatus;
    /** Whether the SIM/UICC supports SIM toolkit */
    Boolean                         proactiveSim;
    /** Application Id of the current USIM application */
    SimUiccAid                      aid;
    /** Number of subscriber profiles */
    Int8                            numSubscriberProfiles;
    SimFileStatus                   efIccid;
    SimFileStatus                   efMsisdn;
    SimFileStatus                   efMbi;
    SimFileStatus                   efAd;
    SimFileStatus                   efMwis;
    SimFileStatus                   efCphsVmwf;
    SimFileStatus                   efCfis;
    SimFileStatus                   efCphsCff;
    SimFileStatus                   efSpn;
    SimFileStatus                   efSpdi;
    SimFileStatus                   efPnn;
    SimFileStatus                   efSst;
    SimFileStatus                   efCphsInfo;
    SimFileStatus                   efCphsOn;
    SimFileStatus                   efCphsOns;
#if defined (UPGRADE_LTE)
    /** Indicates if EPS loci is valid */
    Boolean                         epsLocInfoValid;
    /** EPS loci read from USIM or NVRAM */
    UsimEpsLocInfo                  epsLocInfo;
    /** Indicates if EMM info IMSI is valid */
    Boolean                         epsEmmInfoImsiValid;
    /** IMSI associated with the EPS loci (can be different from USIM IMSI
     * if EPS loci was read from NVRAM
     */
    Imsi                            epsEmmInfoImsi;
#endif /* UPGRADE_LTE */
    AlsiReadNasConfigCnf            nasConfig;
    /** Last RPLMN Selection Indication */
    Int8                      LastRPLMNSelectionIndication;
#if defined (FEA_RPM)
    /** If RPM is present in USIM */
    Boolean                         rpmPresent;
    /** RPM data in USIM */
    RpmInfoInSim                    rpmInfo;
#endif
}
AlsiAppInitialiseCnf;

/** Confirmation of the AlsiAppInitialiseReq.
 * This confirmation signal returns general administration, network and user
 * data required by the application layer.
 */
typedef struct AlsiAppInitialiseCnfCrsmTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** State of intialisation proc */
    SimInitialiseStatus             status;
#if defined (FEA_KOR_LGUP)
    CardType                        cardType;
#endif
    /** card ID number */
    SimIccid                        iccid;
    /** phase (SIM PHASE 2, PHASE 2+, or 3G) */
    SimPhase                        phase;
    /** Administation data */
    SimAdminData                    admin;
    /** International mobile subscriber id */
    Imsi                            imsi;
    /** Preferred access technology from SIM_EF_HPLMNW_ACT*/
    AccessTechnologyId              prefHplmnAccessTech;
    /** Hplmn Search Period, units of minutes (0 - 480) */
    Int16                           hplmnSearchPeriod;
    /** Type of service table (2G or 3G)
\assoc UNION \ref serviceTable */
    ServiceTableType                serviceTableType;
    /** SIM Service Table or USIM Service Table */
    ServiceTable                    serviceTable;
    /** The access control class bits */
    AccessClass                     accessClass;
    /** Preferred PLMN list (read from PLMNwACT or PLMN SEL) */
    SimPlmnList                     plmn;
    /** Location Information (read from EF LOCI) */
    LocationInformation             locInfo;
    /** BCCH Information (only valid on GSM SIMs) */
    SimBaList                       baList;
    /** The forbidden PLMN list */
    SimPlmnList                     forbiddenPlmn;
    /** Total number of FPLMNs */
    Int16                           totalNumFplmns;
    /** Is an actingHplmn file present on SIM
\assoc PRESENT \ref actingHplmn */
    Boolean                         actingHplmnFilePresent;  /*Added KJH*/
    /** Acting home plmn for enhanced network selection features */
    SimPlmn                         actingHplmn;
    /** The CBMID list */
    SimCbmidList                    cbmidList;
    /** The DCK data */
    SimDckData                      dckData;
    /** Flag to indicated whether PIN2 is required to access ACM MAX file */
    Boolean                         pin2RequiredAcmMax;
    /** Flag to indicated whether PIN2 is required to access ACM file */
    Boolean                         pin2RequiredAcm;
    /** Flag to indicated whether PIN2 is required to access PUCT file */
    Boolean                         pin2RequiredPuct;
    /** Fixed dialling number capability enabled */
    Boolean                         fdnEnabled;
    /** Barred dialling number capability enabled */
    Boolean                         bdnEnabled;
    /** Access Point Names control list enabled*/
    Boolean                         aclEnabled;
    /** Whether global phonebook is present*/
    Boolean                         globalPhonebookAvailable;
    /** Set if CPHS is supported on the SIM */
    Boolean                         cphsDataPresent;
#if defined (ENABLE_EHPLMN)
    Int16                           totalNumEhplmns;
    /** list of equivalent Home PLMNs */
    SimPlmnList                     ehplmn;
    /** indication of  whether the presentation Indicator is a valid parameter
\assoc PRESENT \ref ehplmnPi */
    Boolean                         ehplmnPiValid;
    /** value of the presentation Indicator - how the ehplmns should be displayed */
    SimUiccEhplmnDisplayPref        ehplmnPi;
#endif
    /** The key reference */
    SimUiccKeyRefValue              keyRef;
    /** PIN status of the PIN we have tried to verify*/
    SimUiccPinStatus                pinStatus;
    /** PIN status of the unblock PIN we have tried to verify */
    SimUiccPinStatus                unblockPinStatus;
    /** Whether the SIM/UICC supports SIM toolkit */
    Boolean                         proactiveSim;
    /** Application Id of the current USIM application */
    SimUiccAid                      aid;
    /** Number of subscriber profiles */
    Int8                            numSubscriberProfiles;
#if defined (UPGRADE_LTE)
    /** Indicates if EPS loci is valid */
    Boolean                         epsLocInfoValid;
    /** EPS loci read from USIM or NVRAM */
    UsimEpsLocInfo                  epsLocInfo;
    /** Indicates if EPS loci is valid */
    Boolean                         epsEmmInfoImsiValid;
    /** IMSI associated with the EPS loci (can be different from USIM IMSI
     * if EPS loci was read from NVRAM
     */
    Imsi                            epsEmmInfoImsi;
#endif /* UPGRADE_LTE */
    AlsiReadNasConfigCnf            nasConfig;
    /** Last RPLMN Selection Indication */
    Int8                      LastRPLMNSelectionIndication;
#if defined (FEA_RPM)
    /** If RPM is present in USIM */
    Boolean                         rpmPresent;
    /** RPM data in USIM */
    RpmInfoInSim                    rpmInfo;
#endif
}
AlsiAppInitialiseCnfCrsm;

/** This signal passes a raw command to the SIM/UICC. The command is indicated
 * in the command field, and its length by the length field.
 */
typedef struct AlsiSimGenAccessReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Sending task ID */
    TaskId                          taskId;
    /** The length of command */
    Int16                           length;
    /** Holds the command */
    Int8                            command[SIM_MAX_MSG_SIZE];
    /** A path (to select some directories and file) can be specified.
     * They will be selected before the raw command is passed to the SIM/UICC
     */
    SimUiccPath                     path;
}
AlsiSimGenAccessReq;

/** Confirmation of the AlsiSimGenAccessReq.This signal returns
 * the raw data returned by the SIM/UICC
 */
typedef struct AlsiSimGenAccessCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Sending task ID */
    TaskId                          taskId;
    /** The length of command */
    Int16                           length;
    /** Conveys the raw data from the SIM/UICC */
    Int8                            command[SIM_MAX_MSG_SIZE];
    /** Returns TRUE if the response data in command array is valid,
     * and files specified in the REQ could be selected successfully
     */
    Boolean                         success;   /* 0205-17637 */
}
AlsiSimGenAccessCnf;

/** This signal passes a raw command to the SIM/UICC. The command is indicated
 * in the command field, and its length by the length field.
 */
typedef struct AlsiSimRawApduAccessReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Sending task ID */
    TaskId                          taskId;
    /** The length of command */
    Int16                           length;
    /** Holds the command */
    Int8                            command[SIM_MAX_MSG_SIZE];
}
AlsiSimRawApduAccessReq;

/** Confirmation of the AlsiSimGenAccessReq.This signal returns
 * the raw data returned by the SIM/UICC
 */
typedef struct AlsiSimRawApduAccessCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Sending task ID */
    TaskId                          taskId;
    /** The length of command */
    Int16                           length;
    /** Conveys the raw data from the SIM/UICC */
    Int8                            command[SIM_MAX_MSG_SIZE];
    /** Returns TRUE if the response data in command array is valid,
     * and files specified in the REQ could be selected successfully
     */
    Boolean                         success;   /* 0205-17637 */
}
AlsiSimRawApduAccessCnf;

/** A request to terminate the GSM session or the USIM application.
 * The AlsiTerminateSessionReq causes the USIM manager to go to the NULL state and
 * if deactivateCard field is set to TRUE, it will also deactivate the card.
 * A request to terminate the GSM session or the USIM application.
 * Before the application layer requests session termination it should
 * perform the necessary requests to store up-to-date values of the following: <UL>
 * <LI> LOCI information
 * <LI> Cipher and Integrity Keys
 * <LI> Advice Of Charge increase
 * <LI> Forbidden PLMN(s)
 * <LI> GSM termination procedures
 * </UL>
 */
typedef struct AlsiTerminateSessionReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Whether or not to start MM */
    Boolean                         restartMM;
    /** Whether or not started by refresh */
    Boolean                         isRefresh;
}
AlsiTerminateSessionReq;

/** Confirmation of session termination. The USIM manager return in NULL state.*/
typedef struct AlsiTerminateSessionCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
}
AlsiTerminateSessionCnf;

/** Request to read the language indication list (EF LI) on the UICC
 * or the Language Preference (EF LP) on the GSM SIM.
 * <B>3G SIM cards:</B> Upon receiving this signal, the USIM manager will read
 * EF LI under the ADF USIM directory. The languages listed in EF LI
 * under ADF USIM should always been used in preference to those listed in EF PL.
 *
 * <B>GSM SIM cards:</B> Upon receiving this signal, the USIM manager will read EF LP.
 * The languages listed in EF ELP should always been used in preference to those
 * listed in EF LP
 *
 * The USIM manager can return up to SIM_LANGUAGE_LIST_SIZE values.
 * The application layer may request more values in the list by making
 * multiple requests, setting the <I> startField </I> field accordingly: <UL>
 * <LI> <I> startField = 0 </I> specifies the first field in the LP file.
 * <LI> <I> StartField = 1 </I> specifies the second language entry.
 *</UL>
 *
 */
typedef struct AlsiReadLpReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Defines the offset */
    Int16                           startField;
    /** Number of desired entries to read */
    Int8                            numEntriesDesired;
}
AlsiReadLpReq;

/** This signal is sent as a response to AlsiReadLpReq */
typedef struct AlsiReadLpCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Indicates the status of the request */
    SimRequestStatus                requestStatus;
    /** The total number of LP file entries */
    Int16                           totalEntries;
    /** start offset to start with */
    Int16                           startField;
    /** The lists of prefered languages */
    LanguagePrefList                languagePrefList;
}
AlsiReadLpCnf;

/** Request to write a list of languages indication to the
 * LI or LP file starting at the defined start field offset.
 * Up to SIM_LANGUAGE_LIST_SIZE entries may be requested.
 * The total number of records in the LI or LP file may be
 * determined by requesting an EF status using the AlsiReadEfStatusReq signal,
 * or from a previous AlsiReadLiReq.
 */
typedef struct AlsiWriteLpReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Start field offset */
    Int16                           startField;
    /** List of languages */
    LanguagePrefList                languagePrefList;
}
AlsiWriteLpReq;

/** Confirmation (or not) that the language indication list has been stored.
 * An indication of the number of entries stored is returned. This will be less than
 * the requested number if the LI/LP file is not large enough.
 */
typedef struct AlsiWriteLpCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Indicates the status of the request */
    SimRequestStatus                requestStatus;
    /** The number of entries stored (zero or more entries stored) */
    Int8                            numEntriesStored;
}
AlsiWriteLpCnf;

/** Request to read a preferred PLMN list.
 * Request to read one of the following files: <UL>
 * <LI> EF_PLMN SEL
 * <LI> EF_PLMNwACT
 * <LI> EF_FPLMN
 * <LI> EF_OPLMNW_ACT
 * <LI> EF_HPLMNW_ACT
 *</UL>
 *
 * Up to SIM_PLMN_LIST_SIZE entries may be requested at once.
 * The application layer may request more values in the list by making
 * multiple requests, setting the startField field accordingly. startField = 0
 * specifies the first field in the PLMNsel file.
 */
typedef struct AlsiReadPlmnSelReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Identifies EF to read */
# if defined (ENABLE_EHPLMN)
    /** need this to be an Int16 so it can handle EHPLMN file
     * which is a USIM file */
    Int16                           efId;
# else
    SimEfId                         efId;
# endif
     /** Defines the valid Plmn we want to start with */
    Int16                           startField;
    /** Number of desired entries to read */
    Int8                            numEntriesDesired;

}
AlsiReadPlmnSelReq;

/** Returns the requested PLMN data.
 * Up to SIM_PLMN_LIST_SIZE entries may be stored with each request.
 * The total number of records in the PLMNsel file may be determined
 * by requesting an EF status using the AlsiReadEfStatusReq signal,
 * or from a previous AlsiReadPlmnSelReq.
 */
typedef struct AlsiReadPlmnSelCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus                requestStatus;
    /** Identifies EF that has been read */
    Int16                           efId;
    /** The total number of PLMNsel entries that the SIM can hold */
    Int16                           totalEntries;
    /** The number of valid PLMN entries*/
    Int16                           numValidEntries;
    /** Defines the valid Plmn to start with */
    Int16                           startField;
    /** The list of valid PLMNs, in order of preference*/
    SimPlmnList                     plmnList;
}
AlsiReadPlmnSelCnf;

/** Request to write a list of preferred PLMNs to the PLMNsel file starting at
 * the defined start field. Up to SIM_PLMN_LIST_SIZE entries may be stored with
 * each request. The total number of records in the PLMNsel file may be determined
 * by requesting an EF status using the AlsiReadEfStatusReq signal, or from a previous
 * AlsiReadPlmnSelReq.
 */
typedef struct AlsiWritePlmnSelReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Identifies EF to write to */
    SimEfId                         efId;
    /** Defines the valid Plmn to start with */
    Int16                           startField;
    /** List of preferred PLMNs */
    SimPlmnList                     plmnList;
}
AlsiWritePlmnSelReq;

/** Confirmation (or not) that the preferred PLMN list has been stored.
 * An indication as to the number of entries stored is returned.
 * This will be less than the requested number if the PLMNsel file is not
 * large enough.
 */
typedef struct AlsiWritePlmnSelCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
    /** Identifies EF */
    SimEfId                         efId;
    /** zero or more entries stored */
    Int8                            numEntriesStored;
}
AlsiWritePlmnSelCnf;

#if defined (FEA_TEMP_OBSOLETE_SIM_MANAGER)
/** Requests to read the accumulated call meter maximum value.*/
typedef struct AlsiReadAcmMaxReqTag
{
    /** Command Reference */
    Int16                           commandRef;
}
AlsiReadAcmMaxReq;

/** Contains the requested accumulated call meter maximum value
 * if the request was successful.
 */
typedef struct AlsiReadAcmMaxCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
    /** Requested accumulated call meter maximum value */
    Int32                           acmMaximumValue;
}
AlsiReadAcmMaxCnf;

/** Request to write the accumulated call meter maximum value.
 * As indicated in the AlsiSimInsertedInd signal, this request may require
 * that PIN2 has already been verified (or unblocked).
 */
typedef struct AlsiWriteAcmMaxReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Accumulated call meter maximum value to be written */
    Int32                           acmMaximumValue;
}
AlsiWriteAcmMaxReq;

/** Confirmation (or not) that the accumulated call meter maximum
 * value has been stored. The field reqAcmMaxVal holds the value
 * that was requested to be written.
 */
typedef struct AlsiWriteAcmMaxCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
    /** Requested accumulated call meter maximum value */
    Int32                           reqAcmMaxVal;
}
AlsiWriteAcmMaxCnf;

/** Request to read the current total of the accumulated call meter.
 * The associated file (ACM) is cyclic. The most recently updated record
 * (record number 1) is read.
 */
typedef struct AlsiReadAcmReqTag
{
    /** Command Reference */
    Int16                           commandRef;
}
AlsiReadAcmReq;

/** Returns the requested accumulated call meter total as held in the most
 * recently updated record.
 */
typedef struct AlsiReadAcmCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
    /** Requested accumulated call meter value */
    Int32                           acmValue;
}
AlsiReadAcmCnf;

/** Requests to write the accumulated call meter initial value.
 * As indicated in the AlsiSimInsertedInd signal, this request may
 * require that PIN2 has already been verified (or unblocked).
 * The cyclic record containing the oldest data is overwritten.
 * This then becomes record number 1.
 */
typedef struct AlsiWriteAcmReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Accumulated call meter initial value to be written */
    Int32                           acmValue;
}
AlsiWriteAcmReq;

/** Confirmation (or not) that the accumulated call meter initial value has been written.
 * The field reqAcmValue holds the value that was requested to be written.
 */
typedef struct AlsiWriteAcmCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
    /** Written accumulated call meter value */
    Int32                           reqAcmValue;
}
AlsiWriteAcmCnf;

/** Request to increase the accumulated call meter */
typedef struct AlsiIncreaseAcmReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** ACM to be incremented */
    Int32                           acmIncrement;
}
AlsiIncreaseAcmReq;

/** Confirmation (or not) that the accumulated call meter has been
 * increased by the requested amount.
 * Both the current total and actual increment are returned. If the
 * record has reached its maximum value (FFFFFF hex), the actual
 * increment may be less than the requested increment.
 */
typedef struct AlsiIncreaseAcmCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus                requestStatus;
    /** ACM value that is received in request signal */
    Int32                           actualAcmIncrement;
    /** Incremented ACM */
    Int32                           newAcmTotal;
}
AlsiIncreaseAcmCnf;

/** Request to read the price per unit currency table (PUCT)*/
typedef struct AlsiReadPuctReqTag
{
    /** Command Reference */
    Int16                           commandRef;
}
AlsiReadPuctReq;

/** Returns the price per unit currency table */
typedef struct AlsiReadPuctCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
    /** Contains a price per unit currency table */
    SimPuctData                     puctData;
}
AlsiReadPuctCnf;

/** Request to write the price per unit currency table.
 * As indicated in the AlsiSimInsertedInd signal, this request may
 * require that PIN2 has already been verified (or unblocked).
 */
typedef struct AlsiWritePuctReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Contains a price per unit currency table requested to be written  */
    SimPuctData                     puctData;
}
AlsiWritePuctReq;

/** Confirmation (or not) that the price per unit currency table has been written.
 * The field reqPuctData holds the data that was requested to be written.
 */
typedef struct AlsiWritePuctCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
    /** Price per unit currency table that has been written */
    SimPuctData                     reqPuctData;
}
AlsiWritePuctCnf;

/** Request to read the cell broadcast message identifier (CBMI) list.
 * The SIM manager can return up to SIM_CBMI_LIST_SIZE values.
 * The application layer may request more values in the list by making multiple
 * requests, setting the startField field accordingly. startField = 0 specifies
 * the first field in the CBMI file.
 */
typedef struct AlsiReadCbmiReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Defines the offset */
    Int8                            startField;
    /** Number of desired entries to read */
    Int8                            numEntriesDesired;
}
AlsiReadCbmiReq;

/** Returns the requested CBMI data. The totalEntries field holds the
 * total number of fields within the CBMI file. This may be used to
 * control subsequent requests. Note that between zero and numEntriesDesired
 * (as specified in the associated request signal) CBMIs may be returned.
 */
typedef struct AlsiReadCbmiCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
    /** Total number of CBMI entries */
    Int8                            totalEntries;
    /** Defines the offset */
    Int8                            startField;
    /** Requested CBMI data */
    CbmiList                        cbmiList;
}
AlsiReadCbmiCnf;

/** Request to write a list of CBMIs to the CBMI file starting at the
 * defined startfield. Up to SIM_CBMI_LIST_SIZE entries may be stored
 * with each request. The total number of records in the CBMI file may
 * be determined by requesting an EF status using the AlsiReadEfStatusReq signal,
 * or from a previous AlsiReadCbmiReq.
 */
typedef struct AlsiWriteCbmiReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Defines the offset */
    Int8                            startField;
    /** List of CBMIs to be written */
    CbmiList                        cbmiList;
}
AlsiWriteCbmiReq;

/** Confirmation (or not) that the CBMI list has been stored.
 * An indication as to the number of entries stored is returned.
 * This will be less than the requested number if the CBMI file is not large enough.
 */
typedef struct AlsiWriteCbmiCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
    /** zero or more entries stored */
    Int8                            numEntriesStored;
    /** Defines the used offset */
    Int8                            startField;
}
AlsiWriteCbmiCnf;

/** Request to read BCCH data from the BCCH file */
typedef struct AlsiReadBcchReqTag
{
    /** Command Reference */
    Int16                           commandRef;
}
AlsiReadBcchReq;

/** Returns requested BCCH data if successful */
typedef struct AlsiReadBcchCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus                requestStatus;
    /** Requested BCCH data */
    SimBaList                       baList;
}
AlsiReadBcchCnf;

/** Request to write BCCH data to the BCCH file.
 * This request is only used with GSM cards.
 */
typedef struct AlsiWriteBcchReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** BCCH data to be written */
    SimBaList                       baList;
}
AlsiWriteBcchReq;

/** Confirmation (or not) that BCCH data has been stored  */
typedef struct AlsiWriteBcchCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
}
AlsiWriteBcchCnf;
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */

/** Request to add a forbidden PLMN to the FPLMN file.
 * The new FPLMN is added to the last field in the file.
 * All other entries are shifted up. The first (least recent) entry is lost.
 * If the FPLMN is already in the file, a new entry will not be added.
 */
typedef struct AlsiAddFplmnReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Holds a forbidden PLMN */
    SimPlmn                         forbiddenPlmn;
    /** Indicates the number of forbidden PLMN to be added */
    Int8                            numFplmns;       /*0202-15390*/
}
AlsiAddFplmnReq;

/** Confirmation (or not) that the specified FPLMN has been added to the FPLMN file */
typedef struct AlsiAddFplmnCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
    /** Indicates the number of forbidden PLMNs added */
    Int8                            numFplmns;
}
AlsiAddFplmnCnf;

/** Deletes a specific FPLMN from the FPLMN file.
 * When receiving this signal, the USIM manager reads the
 * content of EF_FPLMN. The USIM manager rearranges the data
 * so that no invalid entries are stuck at the middle of the list.
 * If any FFFFFF entries are found at the middle of the FPLMN list,
 * they are deleted, and the whole list is shifted.
 */
typedef struct AlsiDeleteFplmnReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** PLMN to delete */
    SimPlmn                         forbiddenPlmn;
    /** If deleteAll is set to TRUE all entries in the list are deleted */
    Boolean                         deleteAll;
}
AlsiDeleteFplmnReq;

/** Confirmation (or not) that the specified FPLMN,
 * or the whole list if deleteAll is TRUE, has been removed from the FPLMN file.
 */
typedef struct AlsiDeleteFplmnCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
    /** Indicates if whole list has been removed*/
    Boolean                         deleteAll;
    /** Indicates if FPLMN is found */
    Boolean                         fplmnFound;
    /** FPLMN  deleted */
    SimPlmn                         forbiddenPlmn;
}
AlsiDeleteFplmnCnf;

/** Request to read the location information in the LOCI file */
typedef struct AlsiReadLociReqTag
{
    /** Command Reference */
    Int16                           commandRef;
}
AlsiReadLociReq;

/** Returns the requested location information if successful */
typedef struct AlsiReadLociCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus                requestStatus;
    /** Location area information */
    LocationInformation             locInfo;
    /** Determines the outcome of the GPRS request.*/
    SimRequestStatus                gprsRequestStatus;
    /* GPRS Location area information */
    RoutingInformation              gprsLocInfo;
}
AlsiReadLociCnf;

#if defined (FEA_TEMP_OBSOLETE_SIM_MANAGER)
/** Performs the reading procedure with EFCCP */
typedef struct AlsiReadCcpReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Record number */
    Int8                            ccpRecordNumber;
    SimDialNumberFile               file;
} AlsiReadCcpReq;

/** This signal is a confirmation of reading CCP */
typedef struct AlsiReadCcpCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus                requestStatus;
    /** Record number */
    Int8                            ccpRecordNumber;
    /** The type of file used */
    SimDialNumberFile               file;
    /** Contains parameters of required bearer capabilities */
    BearerCapability                bearerCapability;
} AlsiReadCcpCnf;


/** This signal is used to read a dial number record from the selected
 * dialling number file.
 * Different mode of access can be used: <UL>
 * <LI> absolute mode (if the record number is known)
 * <LI> previous to access the previous record
 * <LI> next to access the next record).
 * </UL>
 * If Next or previous mode are used, recordNumber should be set to 0,
 * as this field is not relevant in this case.
 *
 * This signal may be used to access one of the following files: <UL>
 * <LI> FDN
 * <LI> BDN
 * <LI> LND
 * <LI> SDN
 * <LI> MSISDN
 * <LI> MBDN </UL>
 *
 * \note For reading ADN file, the AlsuReadPbDialnumReq should be used and
 * for reading OCI or ICI files, the AlsiReadCallInfoReq should be used.
 */
typedef struct AlsiReadDialNumReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** FDN, LND, MSISDN, SDN, BDN, MBDN  */
    SimDialNumberFile               file;
    /** Specifies reading mode (absolute, previous or next) */
    SimRecordReadWriteMode          readMode;
    /** the actual record number */
    Int8                            recordNumber;
} AlsiReadDialNumReq, AlsiReadCallInfoReq;


/** Returns the requested dialling number from OCI or ICI file. This
 * primitive is sent as a response to AlsiReadCallInfoReq*/

typedef struct AlsiReadCallInfoCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
    /** OCI or ICI  */
    SimDialNumberFile               file;
    /** The actual record number */
    Int8                            recordNumber;
    /** Dialled number */
    SimDialNumber                   dialNum;
    /** Reports the timing information associated with the OCI or ICI record. */
    SimUiccTimingInfo               timingInfo;
    /** Reports whether the call was answered or not (Only valid for Incoming calls
     * and should be ignored in case of OCI record)
     */
    Boolean                         callAnswered;
    /** Shows the link in the phonebook if any */
    SimUiccPbLink                   pbLink;
}
AlsiReadCallInfoCnf;


/** Returns the requested abbreviated dial number if successful.
 * The specific record in which the abbreviated dial number is stored is returned.
 * The application layer may use this record number in subsequent requests.
 * A recordNumber of zero is used when the USIM manager does not know the current record.
 */
typedef struct AlsiReadDialNumCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus                requestStatus;
    /** FDN, LND, MSISDN, SDN, BDN, MBDN */
    SimDialNumberFile               file;
    /** The actual record number */
    Int8                            recordNumber;
    /** Requested dialling number */
    SimDialNumber                   dialNum;
    /** The ComparisonMethodInfo field is only valid for BDN file.*/
    Int8                            comparisonMethodInfo;
}
AlsiReadDialNumCnf;


/** This signal is a request to read the accumulated call timer
 * EF associated with either incoming or outgoing calls EF ICT or EF OCT.
 */
typedef struct AlsiReadAccCallTimerReqTag
{
   /** Command Reference */
   Int16                           commandRef;
   /** USIM_EF_OCT or USIM_EF_ICT incoming or outgoing info*/
   UsimEfId                        usimEfId;
}
AlsiReadAccCallTimerReq;

/** This signal is used to report the contents of the accumulated call
 * timer file either for incoming or outgoing calls
 */
typedef struct AlsiReadAccCallTimerCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus                requestStatus;
    /** USIM_EF_OCT or USIM_EF_ICT incoming or outgoing info*/
    UsimEfId                        usimEfId;
    /** Accumulated call timer */
    Int32                           accCallDuration;
}
AlsiReadAccCallTimerCnf;

/** This signal is a request to reset the accumulated call timer
 * EF associated with either incoming or outgoing calls EF ICT or EF OCT.
 */
typedef struct AlsiResetAccCallTimerReqTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** USIM_EF_OCT or USIM_EF_ICT incoming or outgoing info*/
  UsimEfId                        usimEfId;
}
AlsiResetAccCallTimerReq;


/** This signal is used to acknowledge the reset of the accumulated call
 * timer value either for incoming or outgoing calls
 */
typedef struct AlsiResetAccCallTimerCnfTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** Determines the outcome of the request.*/
  SimRequestStatus                requestStatus;
  /** USIM_EF_OCT or USIM_EF_ICT incoming or outgoing info*/
  UsimEfId                        usimEfId;
}
AlsiResetAccCallTimerCnf;



/** This signal is used to inform about a modification in the
 * contents of the accumulated call timer file either for incoming
 * or outgoing calls
 */
typedef struct AlsiAccCallTimerChangedIndTag
{
    /** USIM_EF_OCT or USIM_EF_ICT incoming or outgoing info*/
    UsimEfId                        usimEfId;
}
AlsiAccCallTimerChangedInd;


/** This signal is used to delete an existing dial number record from the
 * selected dialling number file, which can be one of the following: <UL>
 * <LI> FDN
 * <LI> LND
 * <LI> MSISDN
 * <LI> SDN
 * <LI> BDN
 * <LI>  MBDN </UL>
 *
 * For non-cyclic files, different mode of access can be used: absolute mode
 * (if the record number is known, or previous to access the previous record,
 * or next to access the next record).
 *
 * If Next or Previous mode are used, recordNumber should be set to 0,
 * as this field is not relevant in this case.
 *
 * \note For LND, OCI, ICI, only previous mode can be used.
 * This is due to the fact that LND, OCI, ICI are cyclic files.
 * Thus only the oldest record can be overwritten.
 * (See GSM 11.11 for more explanations about cyclic files, and TS 102 221).
 */

typedef struct AlsiDeleteDialNumReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** FDN, LND, MSISDN, SDN, BDN, MBDN  */
    SimDialNumberFile               file;
    /** Specifies delete mode */
    SimRecordReadWriteMode          deleteMode;
    /** The actual record number */
    Int8                            recordNumber;
    /** When set to false, the ccp and ext records associated to the record that
    *   will be deleted will not be delete.
    *   Other difference, when set to false the previous data will not be read
    *   so the dialnum valid in the cnf will not be populated.*/
    Boolean                         deleteCcpAndExt;
}
AlsiDeleteDialNumReq;

/** Confirmation (or not) that the abbreviated dial number has been deleted.
 * The deleted record is returned to the application layer that may be used in subsequent requests.
 * For example an application layer may implement a move command.
 */
typedef struct AlsiDeleteDialNumCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus                requestStatus;
    /** FDN, LND, MSISDN, SDN, BDN, MBDN  */
    SimDialNumberFile               file;
    /** The actual record number */
    Int8                            recordNumber;
    /** Deleted dialling number.
    *   Warning: This field will not be populated when the request deleteCcpAndExt
    *   parameter was FALSE*/
    SimDialNumber                   dialNum;
    /** Indicate if dialNum has been populated*/
    Boolean                         dialNumPresent;
}
AlsiDeleteDialNumCnf;

/** This signal is used to overwrite or to add a new dial number record to the
 * selected dialling number file.  Different mode of access can be used: absolute mode
 * (if the record number is known, or previous to access the previous record, or next
 * to access the next record).
 *
 * If Next or Previous mode are used, recordNumber should be set to 0, as this
 * field is not relevant in this case.  If absolute mode is used, and recordNumber
 * is set to 0, USIM manager will write the dialling number to the first free record.
 *
 * This signal may be used to access one of the following files: FDN, BDN, LND, SDN, MSISDN, MBDN.
 * (For writing to the ADN file, the AlsuWritePbDialnumReq should be used.
 * For writing to OCI or ICI, the AlsiWriteCallInfoReq should be used).
 *
 * \note For LND, only previous mode can be used to write to this file. This is due
 * to the fact that LND is a cyclic file. Thus only the oldest record can be overwritten.
 * (See GSM 11.11 for more explanations about cyclic files).
 */
typedef struct AlsiWriteDialNumReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** FDN, LND, MSISDN, SDN, BDN, MBDN  */
    SimDialNumberFile               file;
    /** Specifies write mode (Absolute, next, previous */
    SimRecordReadWriteMode          writeMode;
    /** The actual record number */
    Int8                            recordNumber;
    /** New dial number */
    SimDialNumber                   dialNum;
    /** Indicates if bearerCapability contains valid information */
    Boolean                         bearerCapabilityDefined;
    /** Determines bearer capability */
    BearerCapability                bearerCapability;
}
AlsiWriteDialNumReq;

/** This signal is used to overwrite or to add a new dial number record to
 * the selected dialling number file, which can be either OCI or ICI file.
 * Those are cyclic file, so only the oldest record in the OCI or ICI file can be updated.
 */
typedef struct AlsiWriteCallInfoReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** OCI/ICI  */
    SimDialNumberFile               file;
    /** New dial number */
    SimDialNumber                   dialNum;
    /** Indicates if bearerCapability contains valid information */
    Boolean                         bearerCapabilityDefined;
    /** Determines bearer capability */
    BearerCapability                bearerCapability;
    /** Link to the phonebook entry */
    SimUiccPbLink                   pbLink;
    /** Timing information associated with a record */
    SimUiccTimingInfo               timingInfo;
    /** Only valid for Incoming calls */
    Boolean                         callAnswered;
}
AlsiWriteCallInfoReq;

/** Confirmation to the AlsiWriteDialNumReq.
 * Confirmation (or not) that the defined abbreviated dial number has been written.
 * If the dial number required extension records (in EXT1 or EXT2) and none were free
 * (or could not be freed) or cannot be re-referenced, the entry will not be stored.
 * Likewise, if the dial number contained associated bearer capability configuration parameters
 * (to be stored in the CCP file) and none were free (or could not be freed) or cannot
 * be re-referenced the entry will not be stored.
 *
 * If the request was successful, the actual record number, in which the abbreviated
 * dial number was stored, is returned and may be used by the application layer in subsequent
 * requests.
 */
typedef struct AlsiWriteDialNumCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus                requestStatus;
    /** FDN, LND, MSISDN, SDN, BDN, MBDN, CFIS  */
    SimDialNumberFile               file;
    /** Record number (if successful) */
    Int8                            recordNumber;
    /** The written dialnum data */
    SimDialNumber                   dialNum;
    /** Indicates if bearer capability is defined */
    Boolean                         bearerCapabilityDefined;
    /** Bearer capability of the newly written dialnum data, this is used to refresh the cached FDN table*/
    BearerCapability                bearerCapability;
    /** The deleted dialnum data (used to refresh the hash-table) */
    SimDialNumber                   prevDialNum;
}
AlsiWriteDialNumCnf;

/** Confirmation to the AlsiWriteCallInfoReq.
 * This signal is sent as a response to a AlsiWriteCallInfoReq to determine the outcome of the request. */
typedef struct AlsiWriteCallInfoCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus                requestStatus;
    /** OCI/ICI  */
    SimDialNumberFile               file;
    /** record number (if successful) */
    Int8                            recordNumber;
    /** the written dialnum data */
    SimDialNumber                   dialNum;
    /** Indicates if bearerCapability contains valid information */
    Boolean                         bearerCapabilityDefined;
    /** Bearer capability of the newly written dialnum data, this is used to refresh the cached FDN table*/
    BearerCapability                bearerCapability;
    /** the deleted dialnum data (used to refresh the hash-table) */
    SimDialNumber                   prevDialNum;
    /** Only valid for ICI*/
    Boolean                         callAnswered;
}
AlsiWriteCallInfoCnf;

/** This signal is used to overwrite or to add a new dial number record to
 * the selected dialling number file, which can be either OCI or ICI file.
 * Those are cyclic file, so only the oldest record in the OCI or ICI file can be updated.
 * Warning: This signal will not write CCP and Ext record! for that please use AlsiWriteCallInfoReq
 */
typedef struct AlsiWriteCallInfoSimpleReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** OCI/ICI  */
    SimDialNumberFile               file;
    /** New dial number */
    SimDialNumber                   dialNum;
    /** Link to the phonebook entry */
    SimUiccPbLink                   pbLink;
    /** Timing information associated with a record */
    SimUiccTimingInfo               timingInfo;
    /** Only valid for Incoming calls */
    Boolean                         callAnswered;
    /** When set to false, the ccp and ext records associated to the record that
    *   will be override will not be delete.*/
    Boolean                         deleteCcpAndExt;
}
AlsiWriteCallInfoSimpleReq;

/** Confirmation to the AlsiWriteCallInfoSimpleReq.
 * This signal is sent as a response to a AlsiWriteCallInfoSimpleReq to determine the outcome of the request. */
typedef struct AlsiWriteCallInfoCnfSimpleTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus                requestStatus;
    /** OCI/ICI  */
    SimDialNumberFile               file;
    /** record number (if successful) */
    Int8                            recordNumber;
    /** the written dialnum data */
    SimDialNumber                   dialNum;
    /** Only valid for ICI*/
    Boolean                         callAnswered;
}
AlsiWriteCallInfoSimpleCnf;

/** Requests a list of alpha ids for the defined abbreviated dial file
 * starting at the defined record number.  Up to SIM_ALPHA_LIST_SIZE identifiers
 *  may be read in any single request. More identifiers may be read by making
 * subsequent requests, setting the start record accordingly. startRecord=1
 * defines the first record. startRecord=0 defines the current record.
 * Only alpha identifiers pertaining to used records are returned.
 * An unused record is defined as all bytes set to FF.
 */
typedef struct AlsiListDialNumReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** The Id of a file */
    SimDialNumberFile               file;
    /** starting record number */
    Int8                            startRecord;
}
AlsiListDialNumReq,
/**This signal requests a list of extended information
 * on dial numbers associated with the specified dial number file.
 */
AlsiListDialNumExtReq;

/*3G signal*/
/** This signal supplies the list of requested dialling numbers.
 * The number of valid entries is held in listSize. (the list only includes the valid entries).
 * Their recordNumbers are stored in the recordNumber array.
 *
 * The signal does not include the dialling numbers themselves, only their alpha ID.
 */
typedef struct AlsiListDialNumCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
    /** The Id of certain files, like EF ADN can vary */
    SimDialNumberFile               file;
    /** Indicates the size of the list
\assoc ARRAY \ref recordNumber
\assoc ARRAY \ref alphaId
\assoc ARRAY \ref callAnswered */
    Int8                            listSize;
    /** This field is only valid for ICI file*/
    Boolean                         callAnswered[SIM_ALPHA_LIST_SIZE];
    /** A list of the associated record numbers */
    Int8                            recordNumber[SIM_ALPHA_LIST_SIZE];
    /** List of alpha ids associated with a defined abbreviated dial file
     * ( \note These values are in order, but not necessarily contiguous as unused
     * records may reside between used records)
     */
    SimAlphaIdentifier              alphaId[SIM_ALPHA_LIST_SIZE];
}
AlsiListDialNumCnf;

/** Returns the list of dial number summaries associated with a defined
 * abbreviated dial file as well as a list of the associated record numbers.
 * These values are in order, but not necessarily contiguous as unused records
 * may reside between used records. The dial number summary provides all the
 * information a normal AlsiReadDialNumReq would except for any sub-address or
 * bearer capability (although it indicates whether or not they are present).
 */
typedef struct AlsiListDialNumExtCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus                requestStatus;
    /** The Id of certain files, like EF ADN can vary */
    SimDialNumberFile               file;
    /** The number of returned alpha ids
\assoc ARRAY \ref recordNumber
\assoc ARRAY \ref dialNums */
    Int8                            listSize;
    /** This field is only valid for ICI file*/
    Boolean                         callAnswered[SIM_ALPHA_LIST_SIZE];
    /** A list of the associated record numbers */
    Int8                            recordNumber[SIM_ALPHA_LIST_SIZE];
    /** A list of the associated dial numbers */
    SimDialNumberSummary            dialNums[SIM_ALPHA_LIST_SIZE];
} AlsiListDialNumExtCnf;
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */

/** This signal is sent to request information relevant to the non-phonebook dial number files.
 * Request for abbreviated dial number status information.
 */
typedef struct AlsiDialNumStatusReqTag
{
    /** Command Reference */
    Int16   commandRef;
}
AlsiDialNumStatusReq;

/** This signal returns the requested information on the status of the
 * dialling number files, except the phonebook ones.
 * Returns the requested abbreviated dial number status information.
 * Information on the status of barred dialling numbers, service dialling
 * numbers and their respective extension files is also returned.
 * This information identifies whether the services are available for use,
 * and denotes the size of the alpha identifier and the number of records
 * within the additional dial number files.
 */
typedef struct AlsiDialNumStatusCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
    /** Indicates if FDN is enabled */
    Boolean                         fdnEnabled;
    /** Barred dialling number capability enabled */
    Boolean                         bdnEnabled;
    /** Availability of the 3G phonebooks */
    Boolean                         globalAdnAvailable;
    /** Indicates if application phonebook is available */
    Boolean                         applAdnAvailable;
    /** Number of records in FDN */
    Int8                            fdnNumRecords;
    /** The size of an FDN alpha id */
    Int8                            fdnAlphaIdSize;
    /** FDN extension available */
    Boolean                         fdnExtAvailable;
    /** Number of records in LND */
    Int8                            lndNumRecords;
    /** The size of an LND alpha id */
    Int8                            lndAlphaIdSize;
    /** LND extension available */
    Boolean                         lndExtAvailable;
    /** Number of records in MSISDN */
    Int8                            msisdnNumRecords;
    /** The size of an MSISDN alpha id */
    Int8                            msisdnAlphaIdSize;
    /** MISIDN extension available */
    Boolean                         msisdnExtAvailable;
    /** Number of records in SDN */
    Int8                            sdnNumRecords;
    /** The size of an SDN alpha id */
    Int8                            sdnAlphaIdSize;
    /** SDN extension available */
    Boolean                         sdnExtAvailable;
    /** Number of records in BDN */
    Int8                            bdnNumRecords;
    /** The size of an BDN alpha id */
    Int8                            bdnAlphaIdSize;
    /** BDN extension available */
    Boolean                         bdnExtAvailable;
    /** Number of records in OCI */
    Int8                            ociNumRecords;
    /** The size of an OCI alpha id */
    Int8                            ociAlphaIdSize;
    /** OCI extension available */
    Boolean                         ociExtAvailable;
    /** Number of records in ICI */
    Int8                            iciNumRecords;
    /** The size of an ICI alpha id */
    Int8                            iciAlphaIdSize;
    /** ICI extension available */
    Boolean                         iciExtAvailable;
    /** Number of records in MBDN */
    Int8                            mbdnNumRecords;
    /** The size of an MBDN alpha id */
    Int8                            mbdnAlphaIdSize;
    /** MBDN extension available */
    Boolean                         mbdnExtAvailable;
    /** Whether MBDN records can be updated when PIN1 is validated*/
    Boolean                         mbdnPin1Updatable;
    /** Number of records in CFIS */
    Int8                            cfisNumRecords;
    /** CFIS extension available */
    Boolean                         cfisExtAvailable;
    /* CPHS specific data */
    /** the size of a CPHS MN alpha id */
    Int8                            cphsMnAlphaIdSize;
    /** Number of records in CPHS MN file */
    Int8                            cphsMnNumRecords;
    /** Whether CPHS MN records can be updated when PIN1 is validated*/
    Boolean                         cphsMnPin1Updatable;
    /** On 2G SIMs, ADN may or may not be readable if invalidated, so we need a flag to indicate this*/
    Boolean                         readWriteInvalidatedAdn;
}
AlsiDialNumStatusCnf;

#if defined (FEA_TEMP_OBSOLETE_SIM_MANAGER)
/** Request to read CFIS file.
 * A signal to request reading of the specified record from the EF CFIS
 * (Call Forwarding Indication Status) file.
 */

typedef struct AlsiReadCfisReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Subscriber profile */
    Int8                            subscriberProfile;
} AlsiReadCfisReq;

/** Returns the result of the request to read the specified record from the
 * EF CFIS (Call Forwarding Indication Status) file.
 */
typedef struct AlsiReadCfisCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
    /** Subscriber profile */
    Int8                            subscriberProfile;
    /** CFIS data that has been read*/
    SimCfisData                     cfisData;
}
AlsiReadCfisCnf;

/** Request to write CFIS file.
 * A signal to request writing of information to the specified
 * record from the EF CFIS (Call Forwarding Indication Status) file.
 */
typedef struct AlsiWriteCfisReqTag
{
    /** Command Reference */
    Int16                           commandRef;
     /** Subscriber profile */
    Int8                            subscriberProfile;
    /** CFIS file to write */
    SimCfisData                     cfisData;
} AlsiWriteCfisReq;

/** Returns the result of the request to write the specified data to
 * the defined record on the EF CFIS (Call Forwarding Indication Status) file.
 */
typedef struct AlsiWriteCfisCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
    /** Subscriber profile */
    Int8                            subscriberProfile;
}
AlsiWriteCfisCnf;

/** Request to delete CFIS file.
 * A signal to request deleting of the specified
 * record from the EF CFIS (Call Forwarding Indication Status) file.
 */
typedef struct AlsiDeleteCfisReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Subscriber profile */
    Int8                            subscriberProfile;
} AlsiDeleteCfisReq;

/** Returns the result of the request to delete the specified record
 * from the EF CFIS (Call Forwarding Indication Status) file.
 */
typedef struct AlsiDeleteCfisCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus                requestStatus;
    /** Subscriber profile */
    Int8                            subscriberProfile;
}
AlsiDeleteCfisCnf;

/** Request to enable or disable FDN */
typedef struct AlsiFixedDialReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Set FALSE to disable FDN */
    Boolean                         enableFdn;
}
AlsiFixedDialReq;

/** Confirmation to the AlsiFixedDialReq */
typedef struct AlsiFixedDialCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus                requestStatus;
    /** Indicates if FDN is enabled */
    Boolean                         fdnEnabled;
    /** Indicates if we can read write the adn when it is invalidated */
    Boolean                         readWriteInvalidatedAdn;
}
AlsiFixedDialCnf;
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */

/** Request to read a short message from the SMS file */
typedef struct AlsiReadSmReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** The definition of record */
    SimRecordDefinition             recordDefinition;
}
AlsiReadSmReq;

/** Returned short message data if successful.  The actual record number in
 * which the short message is stored is returned and may be used in subsequent requests.
 * The smDataValid field defines the validity of the short message data and not the outcome of the request.
 */
typedef struct AlsiReadSmCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
    /** Set FALSE if the short message data could not be decoded successfully
\assoc PRESENT \ref recordNumber
\assoc PRESENT \ref smRecordStatus
\assoc PRESENT \ref scAddr
\assoc PRESENT \ref smTpdu */
    Boolean                         smDataValid;
    /** Record number (if successful) */
    Int8                            recordNumber;
    /** SMS record status */
    SimSmRecordStatus               smRecordStatus;
    /** Source address */
    SmsAddress                      scAddr;
    /** Contains all the information on an SMS TPDU */
    SimSmAllTpdu                    smTpdu;
}
AlsiReadSmCnf;

/** Request to write a short message to the SMS file */
typedef struct AlsiWriteSmReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** The definition of record */
    SimRecordDefinition             recordDefinition;
    /** SMS record status */
    SimSmRecordStatus               smRecordStatus;
    /** Source address */
    SmsAddress                      scAddr;
    /** Contains all the information on an SMS TPDU */
    SimSmAllTpdu                    smTpdu;
}
AlsiWriteSmReq;

/** Confirmation (or not) that the short message has been written.
 * If successful, the actual record number (except when SEEK is used
 * on a phase 1 SIM) in which the short message was stored is returned
 * and may be used in subsequent requests.
 */
typedef struct AlsiWriteSmCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
    /** Indicates if SM is written */
    Boolean                         shortMessageWritten;
    /* record number (if successful) */
    Int8                            recordNumber;
}
AlsiWriteSmCnf, AlsiWriteRawSmCnf;

/** Request to delete a short message from the SMS file.
 * Only the short message record status is set to FREE,
 * the actual short message data is left intact.
 * The deleted short message record data is returned to the application layer.
 * This allows the application layer to perform a move command for example.
 */

typedef struct AlsiDeleteSmReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** The definition of record */
    SimRecordDefinition             recordDefinition;
}
AlsiDeleteSmReq;

/** Confirmation (or not) that the defined short message has been deleted.
 * The actual record number is returned as well as the original short message
 * data if the request was successful.
 */
typedef struct AlsiDeleteSmCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus                requestStatus;
    /** Validity of following data */
    Boolean                         shortMessageDeleted;

    /*
    The following data is only valid if shortMessageDeleted = TRUE
    If requestStatus is OK and shortMessageDeleted = FALSE, this
    implies one of the following:

        A seek has been unsuccessful (pattern not found).
        The defined record was not found.
    */
    /** The actual record deleted */
    Int8                            recordNumber;
    /** Indicates if SM data is valid
\assoc PRESENT \ref scAddr
\assoc PRESENT \ref smTpdu */
    Boolean                         smDataValid;
    /** Source address */
    SmsAddress                      scAddr;
    /** Contains all the information on an SMS TPDU */
    SimSmAllTpdu                    smTpdu;
}
AlsiDeleteSmCnf;

/** Adds a short message to the first free record in the SMS file.
 * A free record is defined by a status word set to zero.
 */
typedef struct AlsiAddSmReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** SMS record status */
    SimSmRecordStatus               smRecordStatus;
    /** SMS addressing */
    SmsAddress                      scAddr;
    /** Contains all the information on an SMS TPDU */
    SimSmAllTpdu                    smTpdu;
}
AlsiAddSmReq;

/** Confirmation (or not) that the defined short message has been added to
 * the first free record in the SMS file. A flag is returned indicating if
 * a free record was found. If the request is successful, the actual record
 * number in which the short message data was stored is returned. This may
 * be used by the application layer for subsequent requests.
 */
typedef struct AlsiAddSmCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus                requestStatus;
    /** Indicates if a free record was found */
    Boolean                         noFreeRecords;
    /** Record number (if successful) */
    Int8                            recordNumber;
}
AlsiAddSmCnf,
/** Confirmation (or not) that the defined short message
 * has been added to the first free record in the SMS file.
 * A flag is returned indicating if a free record was found.
 * If the request is successful, the actual record number in
 * which the short message data was stored is returned. The returned
 * record number may be used by the Application Layer for subsequent
 * requests on this SMS. This request uses exactly the same structure
 * as the AlsiAddSmCnf signal.
 */
 AlsiAddRawSmCnf;

/** This signal is used to write short message data received in
 * a TsRawDeliverInd signal to the SMS file.
 * This signal allows an SMS to be passed from the SMS stack to the
 * SIM without being decoded and then re-encoded. This prevents the
 * possibility of an SMS intended for and Application on the SIM being
 * modified when being transferred from SMS to SIM.
 */
typedef struct AlsiWriteRawSmReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** The definition of record */
    SimRecordDefinition             recordDefinition;
    /** SMS record status */
    SimSmRecordStatus               smRecordStatus;
    /** Raw SMS addressing */
    RawSmsAddress                   scAddr;
    /** contain the raw SMS data as received in the
     * TsRawDeliverInd and as such should be copied directly
     * from the TsRawDeliverInd signal
     */
    RpUserDataElement               tpdu;
}
AlsiWriteRawSmReq;

/** This signal is used to write short message data received in a
 * TsRawDeliverInd signal to the SMS file.
 * This signal allows an SMS to be passed from the SMS stack to the SIM
 * without being decoded and then re-encoded. This prevents the possibility
 * of an SMS intended for and Application on the SIM being modified when being
 * transferred from SMS to SIM. This request differs from the AlsiWriteRawSmReq
 * signal in that it adds a short message to the first free record in the SMS file.
 * A free record is defined by the status words lowest bit being set to zero.
 */
typedef struct AlsiAddRawSmReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** SMS record status */
    SimSmRecordStatus               smRecordStatus;
    /** Raw SMS addressing */
    RawSmsAddress                   scAddr;
    /** contain the raw SMS data as received in the
     * TsRawDeliverInd and as such should be copied directly
     * from the TsRawDeliverInd signal
     */
    RpUserDataElement               tpdu;
}
AlsiAddRawSmReq;

/** Theis signal is used to get an indication of how many free SMS
* records are available. On receiving this signal the USIM Manager (SIM_TASK_ID)
* will search the SMS file for free records and report up to 10
* (defined by SIM_MAX_FREE_SM_LIST) free records in an AlsiGetFreeSmCnf signal.
* This request is currently only supported on Phase 2 (or greater) SIMs as it uses
* the SEEK Type 2 command (see GSM 11.11, sections 8.7 and 9.2.7).
* The SEEK Type 2 command is not available on phase 1 SIMs. The SEEK Type 2
* command is required because it reports the record number of a successful seek,
* whereas SEEK Type 1 does not.
*/
typedef struct AlsiGetFreeSmReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Number of SMS records is restricted in abcfg */
    Int8                            maxNumRecords;
} AlsiGetFreeSmReq;

/** This signal is sent by the USIM Manager (SIM_TASK_ID) to the Application Layer
 * (ALSI_TASK_ID) in response to an AlsiGetFreeSmReq signal.
 * The totalFreeRecords and  totalNumRecords fields in the signal are not valid
 * unless this is set to SIM_REQ_OK. If the SIM is phase 1 or does not support
 * SMS storage the requestStatus will be set to SIM_REQ_SERVICE_NOT_AVAILABLE.
 */
typedef struct AlsiGetFreeSmCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus                requestStatus;
    /** Free SMS records available.*/
    Int8                            totalFreeRecords;
    /** Total SMS records */
    Int8                            totalNumRecords;
    /* Max number of records is 255, we need up to 16 bytes */
    /** Is a bit array which shows which records are currently used
     * \note Free records can be used in subsequent requests to store an
     * SMS using the AlsiWriteSmReq or AlsiWriteRawSmReq signals.
     */
    Int16                           recordsUsedMap [(SIM_MAX_FREE_SM_LIST + 1) / SIM_SMS_BIT_MAP_SIZE];
} AlsiGetFreeSmCnf;

/** Request to set the status of a particular short message record in the SMS file.
 * Only an absolute record number may be specified.
 */
typedef struct AlsiSetSmRecordStatReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Short message record number to be set */
    Int8                            recordNumber;
    /** SMS record status */
    SimSmRecordStatus               smRecordStatus;
}
AlsiSetSmRecordStatReq;

/** Confirmation (or not) that the status of the defined short message record has been set.*/
typedef struct AlsiSetSmRecordStatCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus                requestStatus;
    /** Status has been set successfuly */
    Boolean                         statusSetOk;
    /** Short message record number which has been set */
    Int8                            recordNumber;
    /** SMS record status */
    SimSmRecordStatus               smRecordStatus;
}
AlsiSetSmRecordStatCnf;

/** Request to tell Sim Manager that a given SMS has been sent and we have to
 *  Change its status to SENT and to update its message reference
 */
typedef struct AlsiSetSmRecordSentReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Short message record number to be set */
    Int8                            recordNumber;
    /** New message reference for the record or 0
      * if no modification are needed */
    Int8                            newMessageReference;
    /** Whether a status report has been requested*/
    Boolean                         statusReportRequested;

}
AlsiSetSmRecordSentReq;

/** Confirmation (or not) that the status of the defined short message record has been set.*/
typedef struct AlsiSetSmRecordSentCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus                requestStatus;
    /** The changes has been done successfuly */
    Boolean                         changesOk;
    /** Short message record number which has been set */
    Int8                            recordNumber;
    /** Message reference set*/
    Int8                            messageReference;
    /** Whether a status report has been requested*/
    Boolean                         statusReportRequested;
}
AlsiSetSmRecordSentCnf;

/** Request to set the status report status of a particular short message record in the SMS file.
 * Only an absolute record number may be specified.
 */
typedef struct AlsiSetSmSrStatReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Short message record number to be set */
    Int8                            recordNumber;
     /** SMS status record status */
    SimSmRecordSrStat               smRecordSrStat;

}
AlsiSetSmSrStatReq;

/** Confirmation (or not) that the status report status of the defined short message record has been set.*/
typedef struct AlsiSetSmSrStatCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus                requestStatus;
    /** The changes has been done successfuly */
    Boolean                         changesOk;
    /** Short message record number which has been set */
    Int8                            recordNumber;
    /** SMS status record status */
    SimSmRecordSrStat               smRecordSrStat;
}
AlsiSetSmSrStatCnf;


/** Request to delete to the SMSR file.*/
typedef struct AlsiDeleteSmsrReqTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** The definition of record */
  SimRecordDefinition             recordDefinition;
  /** The number of the record in the SMSR file */
  Int8                            smsrRecordNum;
  /** The number of the SMS record in the SMS file on the SIM */
  Int8                            smsRecordNum;
  /** SMS Status Report */
  TsStatusReportInd               smStatusReport;
}
AlsiDeleteSmsrReq;

/** Confirmation (or not) that the SMSR information has been deleted.*/
typedef struct AlsiDeleteSmsrCnfTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** Determines the outcome of the request */
  SimRequestStatus                requestStatus;
  /** Validity of following data */
  Boolean                         smsrDeleted;
  /** validity of following data */
  Boolean                         smsrDataValid;
  /** The number of the record in the SMSR file */
  Int8                            smsrRecordNum;
  /** The number of the SMS record in the SMS file on the SIM */
  Int8                            smsRecordNum;
  /** SMS Status Report */
  TsStatusReportInd               smStatusReport;
}
AlsiDeleteSmsrCnf;


/** Request to read short message service parameters from the SMSP file.*/
typedef struct AlsiReadSmspReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** The definition of record */
    SimRecordDefinition             recordDefinition;
}
AlsiReadSmspReq;

/** Returns the requested short message service parameters if successful.
 * The specific record in which the short message service parameters are stored is returned.
 * The application layer may use this in subsequent requests.
 */
typedef struct AlsiReadSmspCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus                requestStatus;
    /** validity of following data
\assoc PRESENT \ref recordNumber
\assoc PRESENT \ref smsp */
    Boolean                         smspDataValid;

    /*
    The following data is only valid if smspDataValid = TRUE
    If requestStatus is OK and smspDataValid = FALSE, this implies that
    a seek has been unsuccessful (pattern not found) or the defined record
    was not found.
    */
    /** The actual record number (only valid if smspDataValid = TRUE
     * If requestStatus is OK and smspDataValid = FALSE, this implies that
     * a seek has been unsuccessful (pattern not found) or the defined record
     * was not found)
     */
    Int8                            recordNumber;
    /** Contains a record of the SMSP */
    SimSmsParameters                smsp;
}
AlsiReadSmspCnf;

/** Request to write short message service parameters to the SMSP file.*/
typedef struct AlsiWriteSmspReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** The definition of record */
    SimRecordDefinition             recordDefinition;
    /** Contains a record of the SMSP */
    SimSmsParameters                smsp;
}
AlsiWriteSmspReq;

/** Confirmation (or not) that the defined short message service parameters have been written.
 * If the request was successful, the actual record number, in which the short message service
 * parameters were stored, is returned and may be used by the application layer in subsequent
 * requests.
 */
typedef struct AlsiWriteSmspCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus                requestStatus;
    /** Record number (if successful) */
    Int8                            recordNumber;
}
AlsiWriteSmspCnf;

/** Request to delete SMS parameters.
 * Request to delete short message service parameters from the SMSP file.
 * If the defined record is found, all bytes will be set to FF hex.
 */
typedef struct AlsiDeleteSmspReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** The definition of record */
    SimRecordDefinition             recordDefinition;
}
AlsiDeleteSmspReq;

/** Confirmation (or not) that the short message service parameters have been deleted.
 * The deleted record is returned to the application layer and may be used in subsequent
 * requests. For example an application layer may implement a move command.
 */
typedef struct AlsiDeleteSmspCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
    /** validity of following data
\assoc PRESENT \ref recordNumber
\assoc PRESENT \ref smsp */
    Boolean                         smspDataValid;

    /*
    The following data is only valid if smspDataValid = TRUE
    If requestStatus is OK and smspDataValid = FALSE, this implies that
    a seek has been unsuccessful (pattern not found) or the defined record
    was not found.
    */
    /** The actual record number ( only valid if smspDataValid = TRUE
     * If requestStatus is OK and smspDataValid = FALSE, this implies that
     * a seek has been unsuccessful (pattern not found) or the defined record
     * was not found)
     */
    Int8                            recordNumber;
    /** Contains a record of the SMSP */
    SimSmsParameters                smsp;
}
AlsiDeleteSmspCnf;

/** Adds short message service parameters to the first free record in the SMSP.
 * A free record is defined by all bytes set to FF.
 */
typedef struct AlsiAddSmspReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Contains a record of the SMSP */
    SimSmsParameters                smsp;
}
AlsiAddSmspReq;

/** Confirmation (or not) that the defined short message service
 * parameters has been added to the SMSP file. If no free records
 * were found, this will be indicated.  If the request was successful,
 * the actual record number, in which the short message service parameters
 * were stored, is returned and may be used by the application layer
 * in subsequent requests.
 */
typedef struct AlsiAddSmspCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
    /** All records are used */
    Boolean                         noFreeRecords;
    /** Record number (if successful) */
    Int8                            recordNumber;
}
AlsiAddSmspCnf;

/** Requests a list of alpha ids for the SMSP file starting at the
 * defined record number.
 * Up to SIM_ALPHA_LIST_SIZE identifiers may be read in any single request.
 * More identifiers may be read by making subsequent requests, setting the
 * start record accordingly. startRecord=1 defines the first record.
 * startRecord=0 defines the current record.
 * Only alpha identifiers pertaining to used records are returned.
 * An unused record is defined as all bytes set to FF.
 */
typedef struct AlsiListSmspReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** The starting record for the list */
    Int8                            startRecord;
}
AlsiListSmspReq;

/** Returns the list of alpha ids associated with the SMSP file.
 * A list of the associated record numbers is returned. These values
 * will be in order, but not necessarily contiguous as unused records
 * may reside between used records.
 */
typedef struct AlsiListSmspCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
    /** The number of returned alpha ids  */
    Int8                            listSize;
    /** List of the associated record numbers */
    Int8                            recordNumber[SIM_ALPHA_LIST_SIZE];
    /** Alpha ids associated with the SMSP file */
    SimAlphaIdentifier              alphaId[SIM_ALPHA_LIST_SIZE];
}
AlsiListSmspCnf;

/** Request to read the short message service status (SMSS) file.*/
typedef struct AlsiReadSmssReqTag
{
    /** Command Reference */
    Int16                           commandRef;
}
AlsiReadSmssReq;

/** Returned short message service status parameters.*/
typedef struct AlsiReadSmssCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
    /* msgRef to identify SM to SC */
    Int8                            msgRef;
    /** Indicates if memory capacity is exceeded */
    Boolean                         memoryCapExceeded;
}
AlsiReadSmssCnf;

/** Request to write the short message service status data.
 * this primitive write the short message service status data.
 * Each field may be written independently.
 *
 * It is the responsibility of the application layer to write the message
 * reference each time a short message is sent from the MS.
 *
 * It is also the responsibility of the application layer to set the
 * memory capacity exceeded flag whenever the MS received a short message
 * that it cannot store. This flag is used to request messages that are
 * stored in the network (messages that the network has previously attempted
 * to send) whenever storage (or the right conditions) becomes available in the
 * MS (or specifically the SIM).
 */
typedef struct AlsiWriteSmssReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Write the message reference */
    Boolean                         overwriteMsgRef;
    /** Memory capacity exceeded */
    Boolean                         overwriteMemCapExceeded;
    /** msgRef to identify SM to SC */
    Int8                            msgRef;
    /** Memory capacity exceeded */
    Boolean                         memoryCapExceeded;
}
AlsiWriteSmssReq;

/** Confirmation (or not) that the short message service parameters have been written.*/
typedef struct AlsiWriteSmssCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
}
AlsiWriteSmssCnf;

#if defined (FEA_TEMP_OBSOLETE_SIM_MANAGER)
/** This signal is sent to upper layers when the status of the PIN
 * is modified as the result of an AT+CSIM or AT+CRSM command.
 */
typedef struct AlsiChvStatusIndTag    /*job 104913*/
{
   /** CHV value */
   SimChvNum                       chvNum;
   /** Indicates if CHV is enabled */
   Boolean                         chv1Enabled;
   /** The status of CHV */
   SimChvStatus                    chvStatus;
   /** The status of unblock CHV */
   SimChvStatus                    unblockChvStatus;
} AlsiChvStatusInd;

/** This primitive requests the application layer to perform CHV related functions.
 * This request allows the application layer to perform the following
 * CHV related functions: <UL>
 * <LI> Verify CHV1 or CHV2
 * <LI> Change CHV1 or CHV2
 * <LI> Disable CHV1
 * <LI> Enable CHV1
 * <LI> Unblock CHV1 or CHV2
 *</UL>
 */
typedef struct AlsiChvFunctionReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** CHV related functions */
    SimChvFunction                  chvFunction;
    /** Specifies CHV number (CHV1/ CHV2) */
    SimChvNum                       chvNum;
    /** Containes CHV value */
    SimChvValue                     chvValue;
    /** used for CHANGE & UNBLOCK commands
     * ( When unblocking a CHV set chvValue to the unblock CHV and
     * newChvValue to the new CHV1 or CHV2 value)
     */
    SimChvValue                     newChvValue;
}
AlsiChvFunctionReq;

/** Confirmation (or not) that the requested CHV function was successful.
 * Status information pertaining to the particular CHV and unblock CHV codes is returned.
 * A code is blocked if the numRemainingRetries field within the associated status
 * field is set to zero.
 */
typedef struct AlsiChvFunctionCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
    /** CHV functions */
    SimChvFunction                  chvFunction;
    /** Containes CHV value */
    SimChvNum                       chvNum;
    /** Requested CHV function was successful */
    Boolean                         chvFunctionSuccess;
    /** Status information related to a CHV */
    SimChvStatus                    chvStatus;
    /** Status information related to an unblock CHV */
    SimChvStatus                    unblockChvStatus;
}
AlsiChvFunctionCnf;
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */

/** Should only be used if a UICC is inserted in the mobile.
 * If a GSM SIM is inserted, AlsiChvFunctionReq should be used.
 * This request allows the application layer to perform the following PIN related functions.
 * On 3G cards, each PIN is identified by a key reference. (pinKeyReference).
 * Upon receiving this signal, the USIM manager will perform one of the following commands: <UL>
 * <LI> Verify PIN
 * <LI> Change PIN
 * <LI> Disable PIN
 * <LI> Enable PIN
 * <LI> Unblock PIN
 * </UL>
 *
 * If the PIN is a local PIN, and localPinDirId is a valid directory,
 * the USIM manager will attempt to select this directory before performing the PIN operation.
 */

typedef struct AlsiPinFunctionReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines PIN function to perform */
    SimUiccPinFunction              pinFunction;
    /** Key reference of the PIN */
    SimUiccKeyRefValue              pinKeyReference;
    /** Alternative Key reference of the PIN */
    SimUiccKeyRefValue              altPinKeyReference;
    /** PIN value */
    SimUiccPinValue                 pinValue;
    /** Directory to select */
    SimDirId                        localPinDirId;
    /** New PIn value */
    SimUiccPinValue                 newPinValue;
}
AlsiPinFunctionReq;

/** Confirmation (or not) that the requested PIN function request was successful.*/
typedef struct AlsiPinFunctionCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
    /** Determines PIN function performed */
    SimUiccPinFunction              pinFunction;
    /** Key reference of the PIN */
    SimUiccKeyRefValue              pinKeyReference;
    /** Alternative Key reference of the PIN */
    SimUiccKeyRefValue              altPinKeyReference;
    /** Whether or not the PIN function request was successful */
    Boolean                         pinFunctionSuccess;
    /** The status of the PIN */
    SimUiccPinStatus                pinStatus;
    /** The status of the alternate PIN */
    SimUiccPinStatus                altPinStatus;
    /** The status of the unblock PIN */
    SimUiccPinStatus                unblockPinStatus;
    /** Universal Pin should be verified */
    Boolean                         verifyUniversalPin;
}
AlsiPinFunctionCnf;

/** Reads directory status information for either the MF (master file) DFGSM,
 * DFDCS1800 or DFTelecom directories. The current directory is set to the
 * specified directory.
 */
typedef struct AlsiReadDirStatusReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Identifies the directory held on the SIM */
    SimDirId                        simDirId;
}
AlsiReadDirStatusReq;

/** Returned status information pertaining to the specified directory.*/
typedef struct AlsiReadDirStatusCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
    /** Status data of the directory */
    SimDirStatusData                dirStatusData;
}
AlsiReadDirStatusCnf;

/** Reads elementary file (EF) status information.
 * The appropriate directory is selected and the defined file is set as the current file.
 * If this is successful, the status data is read from the SIM.
 */
typedef struct AlsiReadEfStatusReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** The elementary file id */
    SimEfId                         efId;
}
AlsiReadEfStatusReq;

/** Returned status information pertaining to the specified EF.*/
typedef struct AlsiReadEfStatusCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
    /** Status data of the EF */
    SimEfStatusInfo                 efStatusData;
}
AlsiReadEfStatusCnf;

#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
/** Request to read one of the GID files.*/
typedef struct AlsiReadGidReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Identifies GID file */
    SimEfId                         gidFile;
}
AlsiReadGidReq;

/** This primitive is sent as a response to AlsiReadGidReq.
 * Returns the GID data read.
 */
typedef struct AlsiReadGidCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
    /** GID data read */
    SimGroupIdData                  gidData;
}
AlsiReadGidCnf;
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */

/** Request to read the SPN (Service Provider Name) file from the SIM.*/
typedef struct AlsiReadSpnReqTag
{
    /** Command Reference */
    Int16                           commandRef;
}
AlsiReadSpnReq;

/** This primitive is sent as a response to AlsiReadSpnReq
 * Returns the SPN data read.
 */
typedef struct AlsiReadSpnCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus                requestStatus;
    /** SPN data read */
    SimSpnData                      spnData;
}
AlsiReadSpnCnf;

#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
/** This signal is a request to read and return a list of valid PLMNs
 * from the SPDI (Service Provider Display Information) file starting
 * from the defined startField record.
 */
typedef struct AlsiReadSpdiReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** This indicates the first desired entry, value starting from 0
    *(only valid entries count)
    */
    Int16                           startField;
}
AlsiReadSpdiReq;

/** Returns a  list of valid PLMNs from the SPDI file of maximum
 * length SIM_SPDI_LIST_SIZE starting from the startField record
 */
typedef struct AlsiReadSpdiCnfTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus                requestStatus;
    /** The number of valid PLMN entries*/
    Int16                           numValidEntries;
    /** First valid entry we requested, value starts from 0*/
    Int16                           startField;
    /** The list of valid PLMNs in SPDI */
    SimSpdiList                     spdiList;
}
AlsiReadSpdiCnf;

/** Request to enable/disable BDN.
 * the following signal types are required for Phase 2+ SIM implementation.
 * Note: these do not cater for SIM Toolkit functionality.
 */
typedef struct AlsiBarredDialReqTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** Set FALSE to disable BDN */
  Boolean                         enableBdn;
}
AlsiBarredDialReq;

/** Confirmation to the AlsiBarredDialReq.*/
typedef struct AlsiBarredDialCnfTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** Determines the outcome of the request */
  SimRequestStatus                requestStatus;
  /** BDN capability enabled */
  Boolean                         bdnEnabled;
}
AlsiBarredDialCnf;
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */

/** Requests the extended languages preference from the GSM SIM (EF ELP)
 * or the Preferred Languages from the UICC (EF PL).
 * <B>3G UICC:</B> Upon receiving this signal, the USIM manager will read
 * EF PL under the Master File. However, the languages listed in EF LI
 * under ADF USIM should always been used in preference to those listed in EF PL.
 *
 * <B> GSM SIM cards:</B> Upon receiving this signal, the USIM manager will
 * read EF ELP under the Master File. The languages listed in EF ELP should
 * always been used in preference to those listed in EF LP.
 */
typedef struct AlsiReadElpReqTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** Specifies from where to start in the file  */
  Int16                           startField;
  /** Specifies how many entries are required */
  Int8                            numEntriesDesired;
}
AlsiReadElpReq;

/** Returns the list of preferred languages.*/
typedef struct AlsiReadElpCnfTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** Determines the outcome of the request */
  SimRequestStatus                requestStatus;
  /** Total entries read */
  Int16                           totalEntries;
  /** Specifies from where to start in the file */
  Int16                           startField;
  /** Lists the preferred extended language */
  SimExtLangPrefList              extLangPrefList;
}
AlsiReadElpCnf;

/** This signal is used to write the preferred languages list to the SIM.
 * <B>3G SIMcards:</B> Upon receiving this signal, the USIM manager will update
 * EF PL under the Master File.
 *
 * <B>GSM SIM cards:</B> Upon receiving this signal, the USIM manager will update
 * EF ELP under the Master File.
*/
typedef struct AlsiWriteElpReqTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** Specifies from where to start in the file */
  Int16                           startField;
  /** Lists the preferred extended language to write */
  SimExtLangPrefList              extLangPrefList;
}
AlsiWriteElpReq;

/** This primitive is sent as a response AlsiWriteElpReq
 * Confirmation (or not) that the ELP list has been stored.
 * An indication as to the number of entries stored is returned.
 */
typedef struct AlsiWriteElpCnfTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** Determines the outcome of the request */
  SimRequestStatus                requestStatus;
  /** how many entries could be written */
  Int8                            numEntriesStored;
}
AlsiWriteElpCnf;

#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
/** This signal requests a read of information from a Voice Service File.
 * The only valid file entries are EF_VGCS (6fb1) and  EF_VBS (6fb3)
 */
typedef struct AlsiReadVgcsVbsReqTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** Voice Service File to read */
  SimEfId                         voiceServiceFile;
}
AlsiReadVgcsVbsReq;

/** This signal passes the decoded information read from the Voice Service
 * File requested in AlsiReadVgcsVbsReq.
 */
typedef struct AlsiReadVgcsVbsCnfTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** Determines the outcome of the request */
  SimRequestStatus                requestStatus;
  /** Read Voice Service File */
  SimEfId                         voiceServiceFile;
  /** Identifies voice service */
  SimVoiceServiceData             voiceServiceId;
}
AlsiReadVgcsVbsCnf;

/** This signal requests a read of information from a Voice Service  File.
 * The only valid file entries are EF_VGCSS (6fb2) and  EF_VBSS (6fb4)
 */
typedef struct AlsiReadVgcsVbsStatusReqTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** Voice Service  File */
  SimEfId                         voiceServiceFile;
}
AlsiReadVgcsVbsStatusReq;

/** This signal passes the decoded information read
 * from the Voice Service File requested in AlsiReadVgcsVbsStatusReq.
 */
typedef struct AlsiReadVgcsVbsStatusCnfTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** Determines the outcome of the request */
  SimRequestStatus                requestStatus;
  /** Read voice Service File */
  SimEfId                         voiceServiceFile;
  /** Status data */
  SimVoiceServiceStatusData       voiceServiceStatusData;
}
AlsiReadVgcsVbsStatusCnf;

/** Request to read the enhanced multi-level pre-emption and priority
 * (eMLPP) file from the SIM.
 */
typedef struct AlsiReadEmlppReqTag
{
  /** Command Reference */
  Int16                           commandRef;
}
AlsiReadEmlppReq;

/** Returns the requested eMLPP service data read from the SIM to the ME.
 * The data indicates which of the eMLPP priority levels and fast call set-up
 * conditions are subscribed to.
 */
typedef struct AlsiReadEmlppCnfTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** Determines the outcome of the request */
  SimRequestStatus                requestStatus;
  /** Requested eMLPP service data */
  SimEmlppData                    emlppData;
}
AlsiReadEmlppCnf;

/** Request to read the automatic answer for eMLPP (AAeM) service file.*/
typedef struct AlsiReadAaemReqTag
{
  /** Command Reference */
  Int16                           commandRef;
}
AlsiReadAaemReq;

/** Returns the AAeM data read from the SIM.*/
typedef struct AlsiReadAaemCnfTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** Determines the outcome of the request */
  SimRequestStatus                requestStatus;
  /** AAeM data read */
  SimAaemData                     aaemData;
}
AlsiReadAaemCnf;

/** Request to write priority level data to the AAeM file
 * indicating the levels for which the MS shall answer incoming calls automatically.
 */
typedef struct AlsiWriteAaemReqTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** AAeM data  */
  SimAaemData                     aaemData;
}
AlsiWriteAaemReq;

/** This primitive is sent as a response to AlsiWriteAaemReq.
 * Confirmation (or not) that the automatic answer for eMLPP service priority
 * levels have been written.
 */

typedef struct AlsiWriteAaemCnfTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** Determines the outcome of the request */
  SimRequestStatus                requestStatus;
  /** Holds the data that was requested to be written */
  SimAaemData                     reqAaemData;
}
AlsiWriteAaemCnf;


/** Request to read the de-personalization control keys (DCK) file from the SIM.*/
typedef struct AlsiReadDckReqTag
{
  /** Command Reference */
  Int16                           commandRef;
}
AlsiReadDckReq;

/** This primitive is sent as a response to AlsiReadDckReq.
 * Returns the DCK data read from the SIM.
 */
typedef struct AlsiReadDckCnfTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** Determines the outcome of the request */
  SimRequestStatus                requestStatus;
  /** Requested  DCK data */
  SimDckData                      dckData;
}
AlsiReadDckCnf;

/** Resets the De-personalization Control Keys data.*/
typedef struct AlsiResetDckReqTag
{
  /** Command Reference */
  Int16                           commandRef;
}
AlsiResetDckReq;

/** Returns the status of the request.
 * Confirmation (or not) that the de-personalisation control keys have been reset
 * in the DCK file.
 */
typedef struct AlsiResetDckCnfTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** Determines the outcome of the request */
  SimRequestStatus                requestStatus;
}
AlsiResetDckCnf;
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */

/** Request to read the co-operative network list (CNL) from the SIM.*/
typedef struct AlsiReadCnlReqTag
{
  /** Command Reference */
  Int16                           commandRef;
}
AlsiReadCnlReq;

/** Returns the co-operative network list data read from the CNL file on the SIM.*/
typedef struct AlsiReadCnlCnfTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** Determines the outcome of the request.*/
  SimRequestStatus                requestStatus;
  /** Contains PLMN, network subset, service provider ID and corporate
   * ID data for the co-operative networks
   */
  SimCnlData                      cnlData;
}
AlsiReadCnlCnf;

/** Request a list of valid OPL Records in EF OPL from the offset specified in startField.*/
typedef struct AlsiListOplReqTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** Specifies the first entry desired */
  Int8                            startField;
}
AlsiListOplReq;

/** Confirmation of the AlsiListOplReq. This primitive is sent as a response to
 * AlsiListOplReq. If the request was successful,
 * it returns the list up to a maximum of SIM_OPL_LIST_SIZE of valid OPL records .
 */
typedef struct AlsiListOplCnfTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** Determines the outcome of the request */
  SimRequestStatus                requestStatus;
  /** Next field to fetch */
  Int8                            nextStartField;
  /** Specifies how many entries there are in total */
  Int8                            totalEntries;
  /** Requested OPL Records */
  SimOplData                      oplData;
}
AlsiListOplCnf;

/** Request a list of valid PNN Records in EF PNN from the offset specified in startField. */
typedef struct AlsiListPnnReqTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** Offset to start from */
  Int8                            startField;
}
AlsiListPnnReq;

/** Confirmation of the AlsiListPnnReq; if the request was successful,
 * it returns the list up to a maximum of SIM_PNN_LIST_SIZE of valid PNN records .
 */
typedef struct AlsiListPnnCnfTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** Determines the outcome of the request */
  SimRequestStatus                requestStatus;
  /** Specifies how many entries there are in total */
  Int8                            totalEntries;
  /** Next field to fetch */
  Int8                            nextStartField;
  /** Requested PNN records */
  SimPnnData                      pnnData;
}
AlsiListPnnCnf;

#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
/** Request to read the Network Indication of Alerting (NIA) file on the SIM.
 * Note:  there is no current definition in the GSM Phase 2+ specifications for
 * the format of the NIA information to be returned as a result of this request.*/
typedef struct AlsiReadNiaReqTag
{
  /** Command Reference */
  Int16                           commandRef;
}
AlsiReadNiaReq;

/** Returns the requested NIA data read from the SIM to the ME, indicating the
 * category of alerting for terminating traffic.
 */
typedef struct AlsiReadNiaCnfTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** Determines the outcome of the request */
  SimRequestStatus                requestStatus;
  /** Category of alerting */
  SimAlertingCategory             alertingCategory;
  /** Describes the type of terminating traffic associated with the
   * relevant alerting category
   */
  SimInformativeText              informativeText;
}
AlsiReadNiaCnf;
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */

/** Request to read the Short Message Status Report (SMSR) file on the SIM.*/
typedef struct AlsiReadSmsrReqTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** Identifies the corresponding SMS SR  record in the SMS file
   * (The value 00 signifies an empty record) */
  Int8                            smsrRecordNum;
}
AlsiReadSmsrReq;

/** Returns the requested SMSR data read from the SIM to the ME.*/
typedef struct AlsiReadSmsrCnfTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** Determines the outcome of the request */
  SimRequestStatus                requestStatus;
  /** validity of following data */
  Boolean                         smsrDataValid;
  /** The number of the record in the EF(SMSR) file */
  Int8                            smsrRecordNum;
  /** The number of the associated SMS record in the EF(SMS) on the SIM */
  Int8                            smsRecordNum;
  /** SMS Status Report */
  TsStatusReportInd               smStatusReport;
}
AlsiReadSmsrCnf;

/** Request to write to the SMSR file.*/
typedef struct AlsiWriteSmsrReqTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** The number of the record in the SMSR file */
  Int8                            smsrRecordNum;
  /** The number of the associated SMS record in the EF(SMS) on the SIM */
  Int8                            smsRecordNum;
  /** SMS Status Report */
  TsStatusReportInd               smStatusReport;
}
AlsiWriteSmsrReq;

/** Confirmation (or not) that the SMSR information has been stored.*/
typedef struct AlsiWriteSmsrCnfTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** Determines the outcome of the request */
  SimRequestStatus                requestStatus;
  /** The number of the record in the SMSR file */
  Int8                            smsrRecordNum;
  /** The number of the associated SMS record in the EF(SMS) on the SIM */
  Int8                            smsRecordNum;
  /** SMS Status Report */
  TsStatusReportInd               smStatusReport;
}
AlsiWriteSmsrCnf;

/** Requests a list of the valid records containing Short Message Status Reports,
 * starting at the defined record number.
 */
typedef struct AlsiListSmsrReqTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** Offset to start from */
  Int8                            startField;
}
AlsiListSmsrReq;

/** Confirmation that the list of valid short message status reports
 * has been returned.  The smsrList field contains the list of records
 * in the SMS file which have valid status reports, giving the SMSR record
 * number and the corresponding record number of the short message in the SMS file.
 */
typedef struct AlsiListSmsrCnfTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** Determines the outcome of the request */
  SimRequestStatus                requestStatus;
  /** Total SMR records */
  Int8                            totalNumRecords;
  /** Indicates the number of valid records */
  Int8                            numValidRecords;
  /** Contains the list of records in the SMS file */
  SimSmsrRecordList               smsrList[SIM_SMSR_LIST_SIZE];
}
AlsiListSmsrCnf;

#if defined (FEA_TEMP_OBSOLETE_SIM_MANAGER)
/** This signal is sent by the USIM Manager during SIM initialisation and
 * contains the CPHS data supported by the SIM.
 * Indication sent to AL immediately after AlsiAppInitialiseCnf only if CPHS
 * information is supported on the SIM (given by cphsDataPresent flag).
 * It contains any relevant CPHS data which is supported on the SIM.
 */
typedef struct AlsiCphsDataIndTag
{
  /** Specifies CPHS phase */
  SimCphsPhase                    cphsPhase;
  /** CPHS service */
  SimCphsServiceTable             cphsService;
  /** Indicates if operator name is present
\assoc PRESENT \ref cphsOpName */
  Boolean                         cphsOpNamePresent;
  /** The operator name */
  SimCphsOperatorName             cphsOpName;
  /** Indicates if CPHS optional name in short form is present
\assoc PRESENT \ref cphsOpNameShortForm */
  Boolean                         cphsOpNameShortFormPresent;
  /** CPHS optional name in short form */
  SimCphsOperatorNameShort        cphsOpNameShortForm;
  /** Mailbox numbers available
\assoc PRESENT \ref cphsMn */
  Boolean                         cphsMnPresent;
  /** The CPHS mailbox number */
  SimCphsMailboxNumbers           cphsMn;
  /** Indicates if CPHS voice mail waiting flags are present
\assoc PRESENT \ref cphsVmwFlags */
  Boolean                         cphsVmwFlagsPresent;
  /** CPHS voice message waiting flags */
  SimCphsVoiceMsgWaitingFlags     cphsVmwFlags;
  /** Indicates if CPHS Call Forward Flags are present
\assoc PRESENT \ref cphsCff */
  Boolean                         cphsCffPresent;
  /** CPHS Call Forward Flags */
  SimCphsCallForwardFlags         cphsCff;
  /** Whether or not CPHS customer service profile is present
\assoc PRESENT \ref cphsCsp */
  Boolean                         cphsCspPresent;
  /** CPHS customer service profile */
  SimCphsCustServProfile          cphsCsp;
}
AlsiCphsDataInd;

/** Request to read the CPHS Call Forward flags on the SIM.*/
typedef struct AlsiCphsReadCffReqTag
{
  /** Command Reference */
  Int16                           commandRef;
}
AlsiCphsReadCffReq;

/** Returns the requested CPHS Call Forward flags read from the SIM.*/
typedef struct AlsiCphsReadCffCnfTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** Determines the outcome of the request.*/
  SimRequestStatus                requestStatus;
  /** Requested CPHS Call Forward flags */
  SimCphsCallForwardFlags         cphsCff;
}
AlsiCphsReadCffCnf;

/** Request to write new CPHS Call Forward flag values to the SIM. */
typedef struct AlsiCphsWriteCffReqTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** CPHS Call Forward flag values to be written */
  SimCphsCallForwardFlags         cphsCff;
}
AlsiCphsWriteCffReq;

/** Confirmation (or not) that the CPHS Call Forward flags have been stored.*/
typedef struct AlsiCphsWriteCffCnfTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** Determines the outcome of the request.*/
  SimRequestStatus                requestStatus;
  /** Indicates if CPHS Call Forward flags have been stored successfuly */
  Boolean                         cffDataStoredOk;
  /** Repeat the request value*/
  SimCphsCallForwardFlags         cphsCff;
}
AlsiCphsWriteCffCnf;

/** Request to read the CPHS Voice Message Waiting (VMW) on the SIM.*/
typedef struct AlsiCphsReadVmwfReqTag
{
  /** Command Reference */
  Int16                           commandRef;
}
AlsiCphsReadVmwfReq;

/** Returns the requested CPHS Voice Message Waiting read from the SIM.*/
typedef struct AlsiCphsReadVmwfCnfTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** Determines the outcome of the request */
  SimRequestStatus                requestStatus;
  /** Voice Message Waiting read from SIM */
  SimCphsVoiceMsgWaitingFlags     cphsVmwf;
}
AlsiCphsReadVmwfCnf;

/** Request to write new CPHS Voice Message Waiting values to the SIM. */
typedef struct AlsiCphsWriteVmwfReqTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** Voice Message Waiting to be written */
  SimCphsVoiceMsgWaitingFlags     cphsVmwf;
}
AlsiCphsWriteVmwfReq;

/** Confirmation (or not) that the CPHS Voice Message Waiting values have been stored.*/
typedef struct AlsiCphsWriteVmwfCnfTag
{
  /** Command Reference */
  Int16                           commandRef;
  /** Determines the outcome of the request */
  SimRequestStatus                requestStatus;
  /** Whether or not Voice Message Waiting values have been stored OK */
  Boolean                         vmwfDataStoredOk;
}
AlsiCphsWriteVmwfCnf;

/*
** CPHS CSP requests
*/

/** Request to read the Customer Service Profile file on the SIM.*/
typedef struct AlsiCphsReadCspReqTag
{
    /** Command Reference */
    Int16                     commandRef;

} AlsiCphsReadCspReq;

/** This signal indicates whether or not the corresponding request
 * succeeded and if so it contains the required data. If successful
 * it returns all of the CSP data held in the Customer Service Profile file.
 */
typedef struct AlsiCphsReadCspCnfTag
{
    /** Command Reference */
    Int16                     commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus          requestStatus;
    /** Indicates if the required CSP data is present
\assoc PRESENT \ref cphsCsp */
    Boolean                   cphsCspPresent;
    /** CSP data held in the Customer Service Profile file*/
    SimCphsCustServProfile    cphsCsp;

} AlsiCphsReadCspCnf;

/** Request to write a single CSP entry at a time to the SIM.*/
typedef struct AlsiCphsWriteCspEntryReqTag
{
    /** Command Reference */
    Int16                     commandRef;
    /** Indicates the number of entries */
    Int8                      entryNumber;
    /** CSP entry to be written */
    SimCphsCspEntry           cspEntry;

} AlsiCphsWriteCspEntryReq;

/** Confirmation (or not) that the CSP entry has been written. */
typedef struct AlsiCphsWriteCspEntryCnfTag
{
    /** Command Reference */
    Int16                     commandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus          requestStatus;

} AlsiCphsWriteCspEntryCnf;

/** CPHS Information Number requests.
 * Request to return information about the Information Numbers file on the SIM.
 */
typedef struct AlsiCphsInfoNumStatusReqTag
{
    /** Command Reference which is mirrored in the confirmation signal */
    Int16                     commandRef;

} AlsiCphsInfoNumStatusReq;

/** This signal is sent as a response to AlsiCphsInfoNumStatusReq.
 * This signal indicates whether or not the corresponding request succeeded and
 * if so it contains the required data. If successful it returns some general
 * information about the Information Numbers file.
 */
typedef struct AlsiCphsInfoNumStatusCnfTag
{
    /** Command Reference */
    Int16                     commandRef;
    /** Determines the outcome of the request
     * (The value of SIM_REQ_OK indicates success, all other values indicate failure)
     */
    SimRequestStatus          requestStatus;
    /** The size of the record */
    Int8                      recordSize;
    /** The total number of records in the file */
    Int8                      numRecords;
    /** The file Id and can be SIM_EF_CPHS_INFO_NUM or SIM_EF_CPHS_INFO_NUM_OLD */
    SimEfId                   efId;
    /** Indicates whether or not the next two fields are valid
\assoc PRESENT \ref maxAlphaIdLength
\assoc PRESENT \ref maxDialStringLength */
    Boolean                   lengthsPresent;
    /** The maximum length an Alpha Id can be for this file. */
    Int8                      maxAlphaIdLength;
    /** The maximum length a Dial String can be for this file
     * without overflowing into and extension record
     */
    Int8                      maxDialStringLength;

} AlsiCphsInfoNumStatusCnf;

/** This signal causes the USIM Manager to read and decode
 * the next 10 valid (empty records are skipped over)
 * Information Numbers starting from the given start record.
 * The data is returned in the AlsiCphsListInfoNumCnf.
 */
typedef struct AlsiCphsListInfoNumReqTag
{
    /** Command reference which is mirrored in the confirmation signal */
    Int16                     commandRef;
    /** The record number from which to start looking for valid records
     * (Record numbering starts at 1 for the first record)
     */
    Int8                      startRecord;

} AlsiCphsListInfoNumReq;

/** This signal indicates whether or not the corresponding request
 * succeeded and if so it contains the required data. If successful
 * it returns a list of up to the next 10 valid Information Numbers.
 */
typedef struct AlsiCphsListInfoNumCnfTag
{
    /** Copy of the command reference given in the corresponding request.*/
    Int16                     commandRef;
    /** indication of whether or not the request was successful
     * (The value of SIM_REQ_OK indicates success, all other values
     * indicate failure)
     */
    SimRequestStatus          requestStatus;
    /** The number of valid entries in the list array.
     * If this value is not equal to SIM_CPHS_MAX_INFONUM_LIST_SIZE
     * then the calling entity can assume that there are no more valid
     * entries in the Information Numbers file.

\assoc ARRAY \ref recordNumbers
\assoc ARRAY \ref list */
    Int8                      listSize;
    /** The record numbers of the Information Number entries
     * stored in the list array
     */
    Int8                      recordNumbers[SIM_CPHS_MAX_INFONUM_LIST_SIZE];
    /** Array of up to the next 10 valid Information Number entries */
    SimCphsInfoNumEntry       list[SIM_CPHS_MAX_INFONUM_LIST_SIZE];

} AlsiCphsListInfoNumCnf;

/** This signal reads a single Information Number record.*/
typedef struct AlsiCphsReadInfoNumReqTag
{
    /** Command reference which is mirrored in the confirmation signal.*/
    Int16                     commandRef;
    /** The structure used to specify the required record.
     * This request does not support seeking i.e performSeek equal to FALSE.
     */
    SimRecordDefinition       recordDefinition;

} AlsiCphsReadInfoNumReq;

/** This primitive is sent as a response to AlsiCphsReadInfoNumReq.
 * This signal indicates whether or not the corresponding request
 * succeeded and if so it contains the required data. If successful
 * it returns the Information Number data from the requested record.
 *
 * If requestStatus equals SIM_REQ_OK and dataValid equals FALSE it
 * indicates that the requested record was empty (unused).
 */
typedef struct AlsiCphsReadInfoNumCnfTag
{
    /** Copy of the command reference given in the corresponding request.*/
    Int16                     commandRef;
    /** indication of whether or not the request was successful
     * (The value of SIM_REQ_OK indicates success, all other values indicate failure)

\assoc PRESENT \ref infoNum */
    SimRequestStatus          requestStatus;
    /** The record number of the read Information Number entry */
    Int8                      recordNumber;
    /** Contains the read Information Number data if the request was successful.*/
    SimCphsInfoNumEntry       infoNum;

} AlsiCphsReadInfoNumCnf;

/** This signal is used to write either a Number or Index entry into the
 * Information Number file.
 */
typedef struct AlsiCphsWriteInfoNumReqTag
{
    /** Command reference that is mirrored in the confirmation signal.*/
    Int16                     commandRef;
    /** The structure used to specify the required record.
     * This request does not support seeking i.e performSeek equal to FALSE.
     */
    SimRecordDefinition       recordDefinition;
    /** Indicates whether or not the contents of the infoNum field are valid.
     * If this is set to FALSE this is an indication that the indicated record
     * should be marked as unused.
     */
    Boolean                   dataValid;
    /** The Information Number entry to write */
    SimCphsInfoNumEntry       infoNum;
} AlsiCphsWriteInfoNumReq;

/** This signal is used to confirm a write request to the Information Number file.*/
typedef struct AlsiCphsWriteInfoNumCnfTag
{
    /** Copy of the command reference given in the corresponding request */
    Int16                     commandRef;
    /** Indication of whether or not the request was successful.
     * The value of SIM_REQ_OK indicates success; all other values
     * indicate failure (the requesting entity should assume the data
     * was not written).*/
    SimRequestStatus          requestStatus;
    /** The requestStatus field indicates failure this field
     * indicates if it was because of lack of free extension
     * records in the EXT1 file.
     */
    Boolean                   noFreeExtRecords;
    /** The record number of the written Information Number entry */
    Int8                      recordNumber;
    /** Contains the Information Number data that was requested to be written */
    SimCphsInfoNumEntry       infoNum;

} AlsiCphsWriteInfoNumCnf;

/** This signal requests a reading of the defined subscriber profile (record)
 * from the Message Waiting Information File(EF_MWIS 6FCA).
 */
typedef struct AlsiReadMsgWaitingInfoReqTag
{
   /** Command reference */
   Int16                      commandRef;
   /** Subscriber profile */
   Int8                       subscriberProfile;

} AlsiReadMsgWaitingInfoReq;

/** This signal reports on the success/failure of reading a subscriber
 * profile from the Message Waiting Information File and if successful,
 * passes the decoded information read from the specified profile (record).
 */
typedef struct AlsiReadMsgWaitingInfoCnfTag
{
    /** Command reference copied form AlsiReadMsgWaitingInfoReq */
    Int16                     commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus          requestStatus;
    /** Subscriber profile */
    Int8                      subscriberProfile;
#if defined (UPGRADE_REL6)
    /** indicates if valid videomail info  - optional field - is present on the USIM*/
    Boolean                   videoMailMsgWSupported;
#endif
    /** The Message Waiting Information File */
    SimMBMsgWaitingInfo       msgWaitingInfo[SIM_NUM_MAILBOX_INFO_GROUPS];
} AlsiReadMsgWaitingInfoCnf;

/** This signal requests the update of a defined subscriber
 * profile (record) from the Message Waiting Information File(EF_MWIS 6FCA)
 */
typedef struct AlsiWriteMsgWaitingInfoReqTag
{
    /** Command reference */
    Int16                    commandRef;
    /** Subscriber profile */
    Int8                     subscriberProfile;
    /** Message Waiting Information */
    SimMBMsgWaitingInfo      msgWaitingInfo[SIM_NUM_MAILBOX_INFO_GROUPS];
} AlsiWriteMsgWaitingInfoReq;

/** Determines the outcome of the AlsiWriteMsgWaitingInfoReqrequest.
 * This signal reports on the success/failure of writing a subscriber profile
 * from the Message Waiting Information File.
 */
typedef struct AlsiWriteMsgWaitingInfoCnfTag
{
   /** Command reference */
   Int16                     commandRef;
   /** Subscriber profile */
   Int8                      subscriberProfile;
#if defined (UPGRADE_REL6)
   /** Whether the optional field for videoMail was available and written to */
   Boolean                   videoMailMsgWaitingWritten;
#endif
   /** Determines the outcome of the request */
   SimRequestStatus          requestStatus;
} AlsiWriteMsgWaitingInfoCnf;

/** This signal requests a reading of the defined subscriber profile (record)
 *  from the Mailbox Information File(EF_MBI 6FC9).
 */
typedef struct AlsiReadMailboxInfoReqTag
{
   /** Command reference */
   Int16                      commandRef;
   /** Subscriber profile */
   Int8                       subscriberProfile;

} AlsiReadMailboxInfoReq;

/** This signal reports on the success/failure of reading a subscriber
 *  profile from the Mailbox Information File and if successful,
 *  passes the decoded information read from the specified profile (record).
 */
typedef struct AlsiReadMailboxInfoCnfTag
{
    /** Command reference copied form AlsiReadMailboxInfoReq */
    Int16                     commandRef;
    /** Subscriber profile */
    Int8                      subscriberProfile;

    /** Determines the outcome of the request */
    SimRequestStatus          requestStatus;
#if defined (UPGRADE_REL6)
   /** indicates if valid videomail info  - optional field - is present on the USIM*/
    Boolean                   videoMailInfoSupported;
#endif
    /** selected subscriber profile from The Mailbox Information File */
    SimMailboxInfo            mailboxInfo[SIM_NUM_MAILBOX_INFO_GROUPS];
} AlsiReadMailboxInfoCnf;

/** This signal requests the update of a defined subscriber
 *  profile (record) from the Mailbox Information File(EF_MBI 6FC9)
 */
typedef struct AlsiWriteMailboxInfoReqTag
{
    /** Command reference */
    Int16                    commandRef;
    /** Subscriber profile */
    Int8                     subscriberProfile;
    /** Mailbox Information */
    SimMailboxInfo           mailboxInfo[SIM_NUM_MAILBOX_INFO_GROUPS];
} AlsiWriteMailboxInfoReq;

/** Determines the outcome of the AlsiWriteMailboxInfoRequest.
 * This signal reports on the success/failure of writing a subscriber profile
 * from the Message Waiting Information File.
 */
typedef struct AlsiWriteMailboxInfoCnfTag
{
   /** Command reference */
   Int16                     commandRef;
   /** Subscriber profile */
   Int8                      subscriberProfile;
#if defined (UPGRADE_REL6)
   /** Whether the optional field for videoMail was available and written to */
   Boolean                   videoMailInfoWritten;
#endif
   /** Determines the outcome of the request */
   SimRequestStatus          requestStatus;
} AlsiWriteMailboxInfoCnf;
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */

/** This signal is used to emulate usim when do RF
 *  production test mode
 */
typedef struct AlsiEmulateUsimReqTag
{
    Boolean ok;
} AlsiEmulateUsimReq;

/** This signal is used to emulate usim when do RF
 *  production test mode
 */
typedef struct AlsiEmulateUsimCnfTag
{
    Boolean ok;
} AlsiEmulateUsimCnf;

#if defined (FEA_TEMP_OBSOLETE_SIM_MANAGER)
/*----------------------------------------------------
 * Homezone related signals
 * These signals were added so that the application
 * layer can access the Homezone info on the SIM.
 * --------------------------------------------------*/
/** Request to read homezone parameters.
 * This primitive allows to read the Homezone parameters stored on the SIM.
 *
 * \note The Homezone parameters file is protected by ADM code, so it is not updatable by the user.
 */
typedef struct AlsiReadHZParamsReqTag
{
   /** Command reference */
   Int16                  commandRef;
} AlsiReadHZParamsReq;

/** This signal returns the Homezone parameters. */
typedef struct AlsiReadHZParamsCnfTag
{
   /** Command reference */
   Int16                   commandRef;
   /** Determines the outcome of the request */
   SimRequestStatus        requestStatus;
   /** Requested homezone parameters*/
   SimHomeZoneEFParameters hzParameters;
} AlsiReadHZParamsCnf;

/** Allows to read the Homezone cache stored on the SIM.*/
typedef struct AlsiReadHZCacheReqTag
{
   /** Command reference */
   Int16                  commandRef;
   /** The cache file we want to read*/
   SimHomeZoneCacheId     cacheId;
} AlsiReadHZCacheReq;

/** This signal returns the Homezone cache.*/
typedef struct AlsiReadHZCacheCnfTag
{
   /** Command reference copied from AlsiReadHZCacheReq */
   Int16                   commandRef;
   /** Determines the outcome of the request */
   SimRequestStatus        requestStatus;
   /** The cache file we have read*/
   SimHomeZoneCacheId      cacheId;
   /** Homezone cache that has been read */
   SimHomeZoneCache        hzCache;
} AlsiReadHZCacheCnf;

/** Allows to update the Homezone cache stored on the SIM.   */
typedef struct AlsiWriteHZCacheReqTag
{
   /** Command reference */
   Int16                   commandRef;
   /** The cache file we want to update*/
   SimHomeZoneCacheId      cacheId;
   /** Homezone cache to write */
   SimHomeZoneCache        hzCache;
} AlsiWriteHZCacheReq;

/** This signal confirms whether or not the Homezone cache was updated.*/
typedef struct AlsiWriteHZCacheCnfTag
{
  /** Command reference copied from AlsiWriteHZCacheReq */
  Int16                    commandRef;
  /** Determines the outcome of the request */
  SimRequestStatus         requestStatus;
  /** The cache file we have updated*/
  SimHomeZoneCacheId       cacheId;
} AlsiWriteHZCacheCnf;
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */

/* for 3G there can be more than 5 Emergency numbers stored on the SIM this signal is used to get the extra ones
 not supplied to AB through the alsiSimInsertedInd  */

/** Request to read the Emergency Call Codes from EF ECC.
 * This signal is used if a UICC card is inserted in the ME.
 * The first entry to be read can be specified in startField.
 * The number of entries to be read can be specified in numEntriesToRead.
 */
typedef struct AlsiReadExtEccReqTag
{
    /** Command reference */
    Int16                           commandRef;
    /** Start offset to start from */
    Int8                            startField;
    /** The number of the entries to read */
    Int8                            numEntriesToRead;
}
AlsiReadExtEccReq;

/** Confirmation to the AlsiReadExtEccReq.
 * This is used if a UICC card is inserted in the ME.
 * It returns the valid entries read from EF ECC (field emergencyCallCodes).
 * The nextStartField in the emergencyCallCodes structure indicates what
 * is the next valid ECC to fetch and can be used to set the startField
 * field in the AlsiReadExtEccReq signal.
 */
typedef struct AlsiReadExtEccCnfTag
{
    /** Command reference */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
    /** Read emergency call codes */
    SimEccList                      emergencyCallCodes;
}
AlsiReadExtEccCnf;

#if defined (FEA_TEMP_OBSOLETE_SIM_MANAGER)
/** This signals allows to read EF NETPAR (network Parameters).
 * Reads EF NETPAR on the SIM/UICC. This file describes the Network Parameters.
 */
typedef struct AlsiReadNetParReqTag
{
    /** Command reference */
    Int16                           commandRef;
}
AlsiReadNetParReq;

/** Confirmation to the AlsiReadNetParReq; if the request was successful,
 * it returns the network parameters. This includes the GSM cell information
 * and the FDD/TDD cell information.
 */
typedef struct AlsiReadNetParCnfTag
{
    /** Command reference */
    Int16                           commandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus                requestStatus;
    /** Gsm Cell Info */
    SimUiccGsmCellInformation       gsmCellInfo;
    /** The current camped BCCH frequency  */
    Int16                           gsmCurrentCampedCellFreq;
#if defined (UPGRADE_3G)
    /** FDD Cell information */
    SimUiccFddCellInformation       fddCellInfo;
#  if defined (UPGRADE_3G_TDD128)
    /** TDD128 Cell information */
    SimUiccTddCellInformation       tddCellInfo;
#  endif
#endif
}
AlsiReadNetParCnf;


/** This signal allows writing of EF NETPAR (network Parameters).
 * Updates EF NETPAR on the SIM/UICC. This file describes the Network Parameters.
 */
    typedef struct AlsiWriteNetParReqTag
{
    /** Command reference */
    Int16                           commandRef;
    /** Gsm Cell Info */
    SimUiccGsmCellInformation       gsmCellInfo;
    /** The current camped BCCH frequency */
    Int16                           gsmCurrentCampedCellFreq;
#if defined (UPGRADE_3G)
    /** FDD cell info */
    SimUiccFddCellInformation       fddCellInfo;
#   if defined (UPGRADE_3G_TDD128)
    /** TDD128 cell info */
    SimUiccTddCellInformation       tddCellInfo;
#   endif
#endif
}
AlsiWriteNetParReq;

/** Confirmation to the AlsiWriteNetParReq */
typedef struct AlsiWriteNetParCnfTag
{
    /** Command reference */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
}
AlsiWriteNetParCnf;
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */

/** Interfaces to read/update the Access Point Name Control List  */
typedef struct AlsiListAclReqTag
{
    /** Command reference */
    Int16                           commandRef;
    /** Which field to read first, starts from 0 */
    Int8                            startField;
}
AlsiListAclReq;

/** Confirmation of success or failure of the request to
 * read/update the Access Point Name Control List.
 */
typedef struct AlsiListAclCnfTag
{
    /** Command reference */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
    /** Start offset */
    Int8                            startField;
    /** Lists APN */
    SimApnList                      apnList;
}
AlsiListAclCnf;

/** Request to add an APN to the ACL file on USIM */
typedef struct AlsiWriteAclReqTag
{
    /** Command reference */
    Int16                           commandRef;
    /** Indicates the position in list.
     * EF ACL is a transparent file, so there is no recordNumber as such
     * Indicates where the APN should be added in the list if there
     * is already an APN in this position, it will be overwritten.
     * (Position starts from 0)
     */
    Int8                            position;
    /** the APN to write*/
    TextualAccessPointName          apn;
    /** If this is set to FALSE, the existing APN will be overwritten*/
    Boolean                         insert;
}
AlsiWriteAclReq;

/** Confirmation of success or failure of the request to
 * add an APN to the ACL file on USIM.
 */
typedef struct AlsiWriteAclCnfTag
{
    /** Command reference */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
    /** Indicates the position in list */
    Int8                            position;
    /** Current number of APNs on SIM*/
    Int8                            numApns;
}
AlsiWriteAclCnf;

/** Deletes one or all APNs from EF ACL */
typedef struct AlsiDeleteAclReqTag
{
    /** Command reference */
    Int16                           commandRef;
    /** Indicates the position in list.
     * EF ACL is a transparent file, so there is no recordNumber as such
     * this field below indicates which APN should be deleted in the list
     * When an APN gets deleted, the end of the list gets shifted, there is no <I> empty APN record </I>
     * left at the middle. This is because the APNs are stored on a transparent file
     * Position starts from 0
     */
    Int8                            position;
}
AlsiDeleteAclReq;

/** This signal reports on the success/failure of the request to
 * delete APN file
 */
typedef struct AlsiDeleteAclCnfTag
{
    /** Command reference */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
    /** Indicates the position in list */
    Int8                            position;
    /** Current number of APNs on SIM */
    Int8                            numApns;
}
AlsiDeleteAclCnf;

/** Request to enable/disable the ACL on the USIM */
typedef struct AlsiSetAclReqTag
{
    /** Command reference */
    Int16                           commandRef;
    /** Whether to enable or disable the ACL*/
    Boolean                         enable;
}
AlsiSetAclReq;

/** This signal reports on the success/failure of the request to
 * enable/disable the ACL on the USIM
 */
typedef struct AlsiSetAclCnfTag
{
    /** Command reference */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
    /** Indicates if enabled */
    Boolean                         enabled;
}
AlsiSetAclCnf;



/* MMS related signals. */

/*
 * These signals allow access to both MMS User Connectivity Parameters,
 * and MMS Issuer Connectivity Parameters. (EF MMSICP and EF MMSUCP)
 */

#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
/** This signal requests a reading of a record on MMS Connectivity
 * Parameters File. There might be several Connectivity Parameters
 * objects startField indicates which one is desired.  The type
 * of connectivity parameters can be either Issuer or User Connectivity
 * EF MMSICP (6FD0) or EF MMSUCP (6FD2)
 */
typedef struct AlsiReadMmscpReqTag
{
   /** Command reference */
   Int16                 commandRef;
   /** There might be several Connectivity Parameters objects,
    * startField indicates which one is desired (Starts from value 0)*/
   Int8                  startField;
   /** Type of Connectivity Parameters
    * either Issuer or User Connectivity Parameters */
   SimMmsCpType          type;
} AlsiReadMmscpReq;

/** This signal reports on the success/failure of the read and if successful,
 * passes the decoded connectivity parameters requested.
 */
typedef struct AlsiReadMmscpCnfTag
{
   /** Command reference */
   Int16                   commandRef;
   /** Determines the outcome of the request */
   SimRequestStatus        requestStatus;
   /** Start offset */
   Int8                    startField;
   /** Indicates the implementation of MMS */
   MmsImplementation       implementation;
   /** Type of Connectivity Parameters: either Issuer or User Connectivity Parameters */
   SimMmsCpType            type;
   /** The decoded MMS Relay Server Address present on the SIM/UICC */
   SimRelayServerAddress   relayServerAddress;
   /** First interface to core network and bearer */
   SimIfToCoreNetAndBearer ifToCoreNetAndBearer;
   /** Gateway */
   SimMmsGateway           gateway;

} AlsiReadMmscpCnf;




/** This signal requests a reading of a record on MMS Notifications File.
 * MMS notifications provide information about a MM located at a recipient
 * MMS Proxy-Relay and waiting for retrieval.
 * MMS notifications provide information about a MM located
 * at a recipient MMS Proxy-Relay and waiting for retrieval.
 * This allows the client to directly retrieve the MM at the
 * location indicated in the MMS notification.*/
typedef struct AlsiReadMmsnReqTag
{
   /** Command reference */
   Int16                 commandRef;
   /** Record number to be read */
   Int8                  recordNumber;
} AlsiReadMmsnReq;

/** This signal is a response to AlsiReadMmsnReq.
 * This signal passes the decoded information read from
 * the specified record of the MMS Notifications File.
 * requested in AlsiReadMmsnReq.
 */
typedef struct AlsiReadMmsnCnfTag
{
   /** Command reference */
   Int16                 commandRef;
   /** Determines the outcome of the request */
   SimRequestStatus      requestStatus;
   /** Record number read */
   Int8                  recordNumber;
   /** Indicates the status */
   MmsStatus             status;
   /** Indicates the implementation of MMS */
   MmsImplementation     implementation;
   /** Raw notification data, contains a pointer to the data.
    * Memory should be freed by the application layer.
    */
   MmsNotification       notification;
   /** Total number of MMSN records available on the SIM/USIM */
   Int8                  numMmsnRecords;
} AlsiReadMmsnCnf;

/** This signal requests the update of a record on MMS Notifications
 * File EF MMSN (0x6FCE).
 */
typedef struct AlsiWriteMmsnReqTag
{
   /** Command reference */
   Int16                 commandRef;
   /** Record number to be written */
   Int8                  recordNumber;
   /** Indicates the status */
   MmsStatus             status;
   /** Indicates the implementation of MMS */
   MmsImplementation     implementation;
   /** If set to FALSE, only the status will be updated;
    * the notification will remain unchanged.
    */
   Boolean               updateNotification;
   /** Raw notification data, contains a pointer to the data.
    * Memory allocated by the application layer,
    * and freed by USIM manager after writing it to the SIM.
    * If there is no data, dataLength should be set to 0,
    * and the pointer should be set to PNULL.
    */
   MmsNotification       notification;
} AlsiWriteMmsnReq;

/** This signal reports on the success/failure of the request
 * to update a record on the MMS Notifications File.
 */
typedef struct AlsiWriteMmsnCnfTag
{
   /** Command reference */
   Int16                 commandRef;
   /** Determines the outcome of the request */
   SimRequestStatus      requestStatus;
   /** The record number */
   Int8                  recordNumber;
   /** Total number of MMSN records available on the SIM/USIM  */
   Int8                  numMmsnRecords;
} AlsiWriteMmsnCnf;

/** This signal is sent to request the deletion of a specified
 * record on the MMS Notifications File
 */
typedef struct AlsiDeleteMmsnReqTag
{
   /** Command reference */
   Int16                 commandRef;
   /** Record number */
   Int8                  recordNumber;
} AlsiDeleteMmsnReq;

/** This signal reports on the success/failure of the
* request to delete a record on the MMS Notifications File.
*/
typedef struct AlsiDeleteMmsnCnfTag
{
   /** Command reference */
   Int16                 commandRef;
   /** Determines the outcome of the request */
   SimRequestStatus      requestStatus;
   /** The record number */
   Int8                  recordNumber;
   /** Total number of MMSN records available on the SIM/USIM */
   Int8                  numMmsnRecords;
} AlsiDeleteMmsnCnf;


/** This signal to read MMS user preferences.
 * This signal requests a reading of a record on MMS User
 * Preferences File(EF_MMSUP 6FD1)
 */
typedef struct AlsiReadMmsupReqTag
{
    /** Command reference */
    Int16                           commandRef;
    /** The record number to be read */
    Int8                            recordNumber;
}
AlsiReadMmsupReq;

/** Determines the outcome of the AlsiReadMmsupReq request.
 * This signal reports on the success/failure of reading a record
 * from the MMS User Preference File and if successful, passes the
 * decoded information read from the specified record of the MMS
 * User Preferences File. requested in AlsiReadMmsupReq.
 */
typedef struct AlsiReadMmsupCnfTag
{
    /** Command reference copies from AlsiReadMmsupReq */
    Int16                           commandRef;
    /** Determines the outcome of the request */
    SimRequestStatus                requestStatus;
    /** The record number read */
    Int8                            recordNumber;
    /** Indicates the implementation of MMS */
    MmsImplementation               implementation;
    /** The MMS Profile Name present on the SIM/UICC  */
    SimMmsupProfileName             profileName;
    /** Decoded MMS user preferences */
    SimMmsup                        mmsup;
    /** Number of MMSUP records on the SIM */
    Int8                            numMmsupRecords;
}
AlsiReadMmsupCnf;

/** Signal to update MMS user preferences.
 * This signal requests the update of a record on MMS
 * User Preferences File(EF_MMSUP 6FD1)
 */
typedef struct AlsiWriteMmsupReqTag
{
    /** Command reference */
    Int16                           commandRef;
    /** The record number to be written */
    Int8                            recordNumber;
    /** Indicates the implementation of MMS. */
    MmsImplementation               implementation;
    /** The MMS Profile Name present on the SIM/UICC. */
    SimMmsupProfileName             profileName;
    /** Decoded MMS user preferences */
    SimMmsup                        mmsup;
}
AlsiWriteMmsupReq;

/** This signal reports on the success/failure of the request
 * to update a record on the MMS User Preferences File
 */
typedef struct AlsiWriteMmsupCnfTag
{
    /** Command reference copied form AlsiWriteMmsupReq */
    Int16                           commandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus                requestStatus;
    /** The record number written */
    Int8                            recordNumber;
    /** Number of MMSUP records on the SIM. */
    Int8                            numMmsupRecords;
}
AlsiWriteMmsupCnf;

/** Signal to delete MMS user preferences record.
 * This signal is sent to request the deletion of a
 * specified record on the MMS User Preferences File
 */
typedef struct AlsiDeleteMmsupReqTag
{
    /** Command reference */
    Int16                           commandRef;
    /** The record number to be deleted */
    Int8                            recordNumber;
}
AlsiDeleteMmsupReq;

/** This signal reports on the success/failure of the request to
 * delete a record on the MMS User Preferences File
 */
typedef struct AlsiDeleteMmsupCnfTag
{
    /** Command reference */
    Int16                           commandRef;
    /** Determines the outcome of the request.*/
    SimRequestStatus                requestStatus;
    /** The record number deleted */
    Int8                            recordNumber;
    /** The number of record on the MMS User Preferences File that has been deleted */
    Int8                            numMmsupRecords;
}
AlsiDeleteMmsupCnf;
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */

/** Requests to change target task ID */
typedef struct AlsiChangeTargetTaskIdReqTag
{
  /** Command reference */
  Int16    commandRef;
  /** ID of the target task */
  TaskId   targetTaskId;
  /** Whether or not restore original task ID */
  Boolean  restoreOriginalTask;
} AlsiChangeTargetTaskIdReq;

/** Indicates the success or failuure of the request.
 * Sent as a response to a AlsiChangeTargetTaskIdReq
 */
typedef struct AlsiChangeTargetTaskIdCnfTag
{
  /** Command reference */
  Int16            commandRef;
  /** Determines the outcome of the request.*/
  SimRequestStatus requestStatus;
} AlsiChangeTargetTaskIdCnf;

#if defined (UPGRADE_CMMB)

/** CMMBSecurityTag
 * indicates that it is 2G authentication or UMTS authentication.
 */
typedef enum CmmbSecurityTag
{
  CMMB_2G_SECURITY = 0,                 /** 2G security */
  CMMB_UMTS_SECURITY                    /** UMTS security */
}
CmmbSecurity;

/** KsInputTag
 * the 128bit random number generated by BSF to be used to authenticate.
 * It is only used for 2G authentication.
 */
typedef struct CmmbKsInputTag
{
  Int8        length;                   /** the length of Ks-input data */
  Int8        data[16];      /** Ks_Input data */
}
CmmbKsInput;

typedef struct AlsiCmmbAuthReqTag
{
  /** Command reference */
  Int16                       commandRef;
  /** indicates that it is 2G authentication or UMTS authentication. */
  CmmbSecurity                cmmbSecurity;
  /** rand is the random challenge and is from BSF to be used to authenticate. */
  Rand                        rand;
  /** the key sequence number */
  KeySequence                 cksn;
  /** ks-input is the 128bit random number generated by BSF to be used to authenticate.
   * It is only used for 2G authentication.
   */
  CmmbKsInput                 ksInput;
  /** the authentication token required by the SIM to calculate
   * the user authentication response and is only UMTS authentication.
   */
  Aut                         autn;
}
AlsiCmmbAuthReq;

typedef struct AlsiCmmbAuthCnfTag
{
  /** Command reference */
  Int16                       commandRef;
  /** indicates the status of the request */
  SimRequestStatus            requestStatus;
  /** indicates that it is 2G authentication or UMTS authentication */
  CmmbSecurity                cmmbSecurity;
  /** the user authentication response */
  Res                         res;
  /** the key sequence number */
  KeySequence                 cksn;
  /** the authentication token. If the authentication is failure, it is returned */
  Aut                         auts;
  /** the ciphering key */
  CipheringKey                kc;
  /** the cipher key */
  CipherKey                   ck;
  /** the integrity key */
  IntegrityKey                ik;
}
AlsiCmmbAuthCnf;
#endif


/******************************************************************************
 * Added Interface between (U)SIM Manager and AB for MBMS security            *
 *****************************************************************************/

#if defined (UPGRADE_3G)
# if defined (UPGRADE_3G_MBMS)
#  if defined (UPGRADE_3G_TDD128)

/** AlsiMbmsGbaAuthReqTag
 * This signal is used to transfer necessary parameter to (U)SIM
 */
typedef struct AlsiMbmsGbaAuthReqTag
{
  /** Command reference */
  Int16                       commandRef;
  /** indicates that it is 2G authentication or UMTS authentication. */
  MbmsSecurity                mbmsSecurity;
  /** rand is the random challenge and is from BSF to be used to authenticate. */
  Rand                        rand;
  /** the key sequence number */
  KeySequence                 cksn;
  /** ks-input is the 128bit random number generated by BSF to be used to authenticate.
   * It is only used for 2G authentication.
   */
  KsInput                     ksInput;
  /** the authentication token required by the SIM to calculate
   * the user authentication response and is only UMTS authentication.
   */
  Aut                         autn;
}
AlsiMbmsGbaAuthReq;

/** AlsiMbmsGbaAuthCnfTag
 * This signal is sent by ABMBMS to CI in response to an ApexMbmsGbaAuthReq.
 * This signal returns the authentication parameter: Res/Sres or Auts.
 */
typedef struct AlsiMbmsGbaAuthCnfTag
{
  /** Command reference */
  Int16                       commandRef;
  /** indicates the status of the request */
  SimRequestStatus            requestStatus;
  /** indicates that it is 2G authentication or UMTS authentication */
  MbmsSecurity                mbmsSecurity;
  /** the user authentication response */
  Res                         res;
  /** the value of SRES */
  Sres                        sres;
  /** the key sequence number */
  KeySequence                 cksn;
  /** the authentication token. If the authentication is failure, it is returned */
  Aut                         auts;
  /** the ciphering key */
  CipheringKey                kc;
  /** the cipher key */
  CipherKey                   ck;
  /** the integrity key */
  IntegrityKey                ik;
  /** the shared key that are used to protect the P2P communication between the UE and the BM-SC. */
  Ks                          ks;
}
AlsiMbmsGbaAuthCnf;

/** AlsiMbmsGbaWriteReqTag
 * This signal is sent to SIM from ABMBMS to inform SIM b_tid and ks_Lifetime need be written in (U)SIM card.
 */
typedef struct AlsiMbmsGbaWriteReqTag
{
  /** Command reference */
  Int16                       commandRef;
  /** the bootstrapping transaction identifier */
  BTid                        bTid;
  /** the lifetime of Ks */
  KsLifeTime                  ksLifeTime;
}
AlsiMbmsGbaWriteReq;

/** AlsiMbmsGbaWriteCnfTag
 * This signal is sent by SIM to ABMBMS in response to an AlsiMbmsGbaWriteCnf to
 * confirm that the request is successful.
 */
typedef struct AlsiMbmsGbaWriteCnfTag
{
  /** Command reference */
  Int16                       commandRef;
  /** indicates the status of the request */
  SimRequestStatus            requestStatus;
}
AlsiMbmsGbaWriteCnf;

/** AlsiMbmsMrkReqTag
 * Based on the authentication result by AlsiMbmsGbaAuthReq,
 * this signal sends impi,naf_id,b_tid to SIM manager.
 */
typedef struct AlsiMbmsMrkReqTag
{
  /** Command reference */
  Int16                       commandRef;
  /** whether card supports to calculate MRK and MUK. */
  GbaContext                  gbaContext;
  /** the IP multimedia private identity */
  Impi                        impi;
  /** the FQDN part of NAF_ID */
  NafId                       nafId;
}
AlsiMbmsMrkReq;

/** AlsiMbmsMrkCnfTag
 * This signal is sent by SIM to AB in response to an AlsiMbmsMrkReq to inform BL
 * whether MRK derivate succeed or not. If successful, MRK will be valid.
 */
typedef struct AlsiMbmsMrkCnfTag
{
  /** Command reference */
  Int16                       commandRef;
  /** indicates the status of the request */
  SimRequestStatus            requestStatus;
  /** whether card supports to calculate MRK and MUK. */
  GbaContext                  gbaContext;
  /** the MBMS request key */
  Mrk                         mrk;
  /** The MBMS user key */
  Muk                         muk;
}
AlsiMbmsMrkCnf;

/** AlsiMbmsMikeyReqTag
 * This signal carries MIKEY message to sim manager to derivate MSK or MTK,
 * message_type indicates whether it is MSK MIKEY message or MTK MIKEY message.
 * If the message is MTK, tmgi and sessionId is provided.
 */
typedef struct AlsiMbmsMikeyReqTag
{
  /** Command reference */
  Int16                       commandRef;
  /** the type of Mikey */
  MikeyMessageType            messageType;
  /** the MIKEY message */
  MikeyMessage                mikeyMessage;
  /** used to identify a MBMS broadcast service */
  Tmgi                        tmgi;
  /** MBMS session identity */
  Int16                       sessionId;
}
AlsiMbmsMikeyReq;

/** AlsiMbmsMikeyCnfTag
 * Confirmation of AlsiMbmsMikeyReq returns MSK or MTK to AB.
 * If it is MSK, Verification message may be returned.
 * If it is MTK, Salt may be returned.
 */
typedef struct AlsiMbmsMikeyCnfTag
{
  /** Command reference */
  Int16                       commandRef;
  /** indicates the status of the request */
  SimRequestStatus            requestStatus;
  /** the type of Mikey */
  MikeyMessageType            messageType;
  /** the MBMS service key */
  Msk                         msk;
  /** indicates if MUK need to be updated */
  Boolean                     mukUpdate;
  /** the MBMS user key */
  Muk                         muk;
  /** the MBMS traffic key */
  Mtk                         mtk;
  /** indicates if not MSK time stamp need to be updated */
  Boolean                     mskTsUpdate;
  /** indicates if verification message is present */
  Boolean                     veriPresent;
  /** the MSK verification message that is used to verify MSK MIKEY message update */
  MikeyMessage                verifi;
  /** indicates if salt  is present. */
  Boolean                     saltPresent;
  /** salt is the salt key */
  Salt                        salt;
  /** used to identify a MBMS broadcast service */
  Tmgi                        tmgi;
  /** MBMS session identity */
  Int16                       sessionId;
}
AlsiMbmsMikeyCnf;

/** AlsiMbmsReadFileReqTag
 * This signal used to read files on (U)SIM
 */
typedef struct AlsiMbmsReadFileReqTag
{
  /** Command reference */
  Int16                       commandRef;
  /** indicate the identity of file */
  FileType                    fileType;
  /** defines the number of record */
  Int8                        record;
  /** the offset of bit */
  Int16                       bitOffset;
  /** the length of file content */
  Int16                       length;
}
AlsiMbmsReadFileReq;

/** AlsiMbmsReadFileCnfTag
 * Confirmation of AlsiReadFileReq returns the operation status and the content of the file.
 */
typedef struct AlsiMbmsReadFileCnfTag
{
  /** Command reference */
  Int16                       commandRef;
  /** indicates the status of the request */
  SimRequestStatus            requestStatus;
  /** indicate the identity of file */
  FileType                    fileType;
  /** the length of file content */
  Int16                       length;
  /** the content of file */
  Int8                        fileContent[FILE_SIZE];
}
AlsiMbmsReadFileCnf;

/** AlsiMbmsWriteFileReqTag
 * This signal is sent to SIM from AB to inform SIM to store new MSK, MTK or B_TID on (U)SIM.
 */
typedef struct AlsiMbmsWriteFileReqTag
{
  /** Command reference */
  Int16                       commandRef;
  /** indicate the identity of file */
  FileType                    fileType;
  /** defines the number of record */
  Int8                        record;
  /** the offset of bit */
  Int16                       bitOffset;
  /** the length of file content */
  Int16                       length;
  /** the content of file */
  Int8                        fileContent[FILE_SIZE];
}
AlsiMbmsWriteFileReq;

/** AlsiMbmsWriteFileCnfTag
 * This signal is sent by SIM to AB in response to an AlsiWriteFileReq to
 * indicate whether store succeeds or not.
 */
typedef struct AlsiMbmsWriteFileCnfTag
{
  /** Command reference */
  Int16                       commandRef;
  /** indicates the status of the request */
  SimRequestStatus            requestStatus;
  /** indicate the identity of file */
  FileType                    fileType;
}
AlsiMbmsWriteFileCnf;

/** AlsiMbmsDeleteFileReqTag
 * This signal is used to delete the content of the corresponding file on (U)SIM,
 * such as MSK, MTK or B_TID etc. on (U)SIM.
 */
typedef struct AlsiMbmsDeleteFileReqTag
{
  /** Command reference */
  Int16                       commandRef;
  /** indicate the identity of file */
  FileType                    fileType;
}
AlsiMbmsDeleteFileReq;

/** AlsiMbmsDeleteFileCnfTag
 * This signal is sent by SIM to AB in response to an AlsiDeleteFileReq to
 * indicate whether deletion succeeds or not.
 */
typedef struct AlsiMbmsDeleteFileCnfTag
{
  /** Command reference */
  Int16                       commandRef;
  /** indicates the status of the request */
  SimRequestStatus            requestStatus;
  /** indicate the identity of file */
  FileType                    fileType;
}
AlsiMbmsDeleteFileCnf;
#if defined(SIM_EMULATION_ON)
/** AlsiMbmsSetTestModeReqTag
 * This signal is sent by CoolArt scripts to USIM manager to set the current
 * test mode.
 */
typedef struct AlsiMbmsSetTestModeReqTag
{
  /** test mode */
  Int16                       testMode;
}
AlsiMbmsSetTestModeReq;
#endif

#  endif /*UPGRADE_3G_TDD128*/
# endif /* UPGRADE_3G_MBMS */
#endif /* UPGRADE_3G */

#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
#if defined (ENABLE_DUAL_SIM_SOLUTION)
typedef struct AlsiSimSelectReqTag
{
    /** Command reference */
    Int16            commandRef;
    /** the SIM Holder to be selected */
    SimHolderType    simHolder;
}AlsiSimSelectReq;

typedef struct AlsiSimSelectCnfTag
{
    /** Command reference */
    Int16            commandRef;
    /** the selected SIM Holder*/
    SimHolderType    simHolder;
    /** the number of SIM holders present 1 or 2 */
    Int8             numSimHolders;
}AlsiSimSelectCnf;
#endif
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */

#if defined (UPGRADE_LTE)
/** Response to a request to read EPS security context data from NVRAM.
 * If the USIM being used for EPS operation does not support storage of
 * the EPS security context data then the data is stored in NVRAM - this
 * signal returns the success or otherwise of the request to read EPS
 * security data from NVRAM - alsiReadNvramEpsSecurityInd.
 */
typedef struct AlsiReadNvramEpsSecurityRspTag
{
    /** Whether the read of the EPS security data from NVRAM has been successful
     * or not - if this parameter is set to FALSE then the following
     * epsNasSecurityContext is not valid.
\assoc PRESENT \ref epsNasSecurityContext */
    Boolean                        success;
    /** The epsNasSecurityContext data read from NVRAM */
    UsimEpsNasSecurity             epsNasSecurityContext;

}
AlsiReadNvramEpsSecurityRsp;

/** Request to write the EPS security context to NVRAM.
 * This is only valid when the USIM does not support EF-EPSNSC.
 */
typedef struct AlsiWriteNvramEpsSecurityIndTag
{
    /** The epsNasSecurityContext data to be written to NVRAM */
    UsimEpsNasSecurity             epsNasSecurityContext;
}
AlsiWriteNvramEpsSecurityInd;

/** Signal to indicate whether the write of the EPS security context to NVRAM was successful.
 * This is only valid when the USIM  does not support EF-EPSNSC.
 */
typedef struct AlsiWriteNvramEpsSecurityRspTag
{
    /** Whether the write of the EPS security data to NVRAM has been successful
     * or not.
     */
    Boolean                        success;
}
AlsiWriteNvramEpsSecurityRsp;

/** This signal returns the success or otherwise of a request to
 * read EPS location information from NVRAM.
 * If the USIM being used for EPS operation does not support storage of the EPS location
 * information then the data is stored in NVRAM. If the read was successful the signal
 * contains the EPS Location data read from NVRAM.
 */
typedef struct AlsiReadNvramEpsLocInfoRspTag
{
    /** Whether the read of the EPS Location data from NVRAM has been successful
     * or not - if this parameter is set to FALSE then the following
     * epsLocationInfo is not valid.
\assoc PRESENT \ref epsLocInfo */
    Boolean                        success;
    /** The epsLocationInfo data read from NVRAM. */
    UsimEpsLocInfo                 epsLocInfo;
}
AlsiReadNvramEpsLocInfoRsp;

/** Request to write the EPS Location Information to NVRAM.
 * this is only valid when the USIM does not support EF-EPSLOCI.
 */
typedef struct AlsiWriteNvramEpsLocInfoIndTag
{
    /** The EPS Location data to be written to NVRAM. */
    UsimEpsLocInfo                 epsLocInfo;
}
AlsiWriteNvramEpsLocInfoInd;

/** Signal to indicate whether the write of the EPS Location infromation to NVRAM
 * was successful or not.
 * This signal is only valid when the USIM  does not support EF-EPSLOCI.
 */
typedef struct AlsiWriteNvramEpsLocInfoRspTag
{
    /** Whether the write of the EPS location information to NVRAM has been successful
     * or not.
     */
    Boolean                        success;
}
AlsiWriteNvramEpsLocInfoRsp;

/** This signal returns the success or otherwise of a request to
 * read IMSI for EMM info from NVRAM.
 * If the USIM being used for EPS operation does not support storage of the EMM
 * information then the IMSI is stored in NVRAM. If the read was successful the signal
 * contains the IMSI for EMM info data read from NVRAM.
 */
typedef struct AlsiReadNvramEpsEmmInfoImsiRspTag
{
    /** Whether the read of the IMSI from NVRAM has been successful
     * or not - if this parameter is set to FALSE then the following
     * IMSI is not valid.
\assoc PRESENT \ref epsEmmInfoImsi */
    Boolean                        success;
    /** The IMSI read from NVRAM. */
    Imsi                           epsEmmInfoImsi;
}
AlsiReadNvramEpsEmmInfoImsiRsp;

/** Request to write the IMSI for EMM Information to NVRAM.
 * this is only valid when the USIM does not support service 85.
 */
typedef struct AlsiWriteNvramEpsEmmInfoImsiIndTag
{
    /** The IMSI to be written to NVRAM. */
    Imsi                          epsEmmInfoImsi;
}
AlsiWriteNvramEpsEmmInfoImsiInd;

/** Signal to indicate whether the write of the IMSI to NVRAM
 * was successful or not.
 * This signal is only valid when the USIM does not support service 85.
 */
typedef struct AlsiWriteNvramEpsEmmInfoImsiRspTag
{
    /** Whether the write of the IMSI to NVRAM has been successful
     * or not.
     */
    Boolean                        success;
}
AlsiWriteNvramEpsEmmInfoImsiRsp;
#endif  /* UPGRADE_LTE */

# if defined (SIM_EMULATION_ON)
/** Signal used to write specific USIM Emulator USIM EF files during testing.  This
 * signal is sent directly via a test script or ATCI task and can only be sent
 * when the USIM Manager is in NULL mode.
 */
typedef struct AlsiWriteUsimEmuFileReqTag
{
    /** Originating task ID */
    KiTaskId   taskId;
    /** USIM Field ID */
    Int16      fileId;
    /** Offset within the USIM field into which data should be written */
    Int16      dataOffset;
    /** Length data to be written */
    Int16      dataLength;
    /** Data to be written */
    Int8       data[SIM_MAX_EMU_FILE_WRITE_DATA_LENGTH];
} AlsiWriteUsimEmuFileReq;

/** Confirmation signal sent from USIM manager to originating task to indicate
 * if USIM emulator USIM file has been written successfully.
 */
typedef struct AlsiWriteUsimEmuFileCnfTag
{
    /** If USIM Emulator USIM field write was sucessful */
    Boolean    success;
} AlsiWriteUsimEmuFileCnf;
# endif /* SIM_EMULATION_ON */

#if defined (UPGRADE_REL8_USIM)
typedef struct AlsiCsimLockReqTag
{
  /** Command reference */
  Int16                     commandRef;
  /** whether this csim lock should be applied or removed */
  Boolean                   lock;
}AlsiCsimLockReq;

typedef struct AlsiCsimLockCnfTag
{
  /** Command reference */
  Int16                     commandRef;
  /** whether the lock or unlock was a success */
  Boolean                   success;
}AlsiCsimLockCnf;

typedef struct AlsiCsimLockIndTag
{
  /** whether this csim lock is currently on or off */
  Boolean                   locked;
}AlsiCsimLockInd;
#endif  /* UPGRADE_REL8_USIM */

/** This signal allows to read EF NASCONFIG (Non Access Stratum Configuration)
 * Reads EF NASCONFIG on SIM/UICC.
 * This file describes the Non Access Stratum Configuration parameters.
 */
typedef struct AlsiReadNasConfigReqTag
{
/** Command reference */
  Int16                     commandRef;
} AlsiReadNasConfigReq;

/** This signal allows to open new logical channel
 * Opens logical channnel and select DFname application on SIM/UICC.
 *
 */
typedef struct AlsiOpenLogicalChannelReqTag
{
  /** Command reference */
  Int16                     commandRef;
  /** The length of DFname */
  Int16                     length;
  /** Holds the DFname data */
  Int8                      dfName[SIM_MAX_DF_NAME_SIZE];
} AlsiOpenLogicalChannelReq;

/** This signal allows to close logical channel
 * Closes already opened logical channel on SIM/UICC.
 *
 */
typedef struct AlsiCloseLogicalChannelReqTag
{
  /** Command reference */
  Int16                     commandRef;
  /** channel number to be closed. */
  Int8                      channelNumber;
} AlsiCloseLogicalChannelReq;


/** This signal passes a raw command to the SIM/UICC. The command is indicated
 * in the command field, and its length by the length field.
 */
typedef struct AlsiSimLogicalChannelAccessReqTag
{
    /** Command Reference */
    Int16                           commandRef;
    /** Sending task ID */
    TaskId                          taskId;
    /** channel number to be accessed. */
    Int8                            channelNumber;
    /** The length of command */
    Int16                           length;
    /** Holds the command */
    Int8                            command[SIM_MAX_MSG_SIZE];
    /** A path (to select some directories and file) can be specified.
     * They will be selected before the raw command is passed to the SIM/UICC
     */
    SimUiccPath                     path;
}
AlsiSimLogicalChannelAccessReq;

typedef struct AlsiReadMsisdnReqTag
{
    /** Command reference */
    Int16                     commandRef;
    /** record number to be read. */
    Int8                      recordNumber;
}
AlsiReadMsisdnReq;

typedef struct AlsiReadNcpIpReqTag
{
    /** Command reference */
    Int16                     commandRef;
    /** record number to be read. */
    Int8                      recordNumber;
}
AlsiReadNcpIpReq;

#if defined (FEA_RPM)
typedef struct AlsiReadRpmEnabledReqTag
{
    /** Command reference */
    Int16                     commandRef;
}
AlsiReadRpmEnabledReq;

typedef struct AlsiReadRpmParametersReqTag
{
    /** Command reference */
    Int16                     commandRef;
}
AlsiReadRpmParametersReq;

typedef struct AlsiReadRpmLeakRateReqTag
{
    /** Command reference */
    Int16                     commandRef;
}
AlsiReadRpmLeakRateReq;

typedef struct AlsiReadRpmCountersReqTag
{
    /** Command reference */
    Int16                     commandRef;
}
AlsiReadRpmCountersReq;

typedef struct AlsiWriteRpmCountersReqTag
{
    /** Command reference */
    Int16                     commandRef;
    /** Action count triggered by N1 */
    Int8                      counterCBR1;
    /** Action count triggered by T1 */
    Int8                      counterCR1;
    /** Action count triggered by F1 */
    Int8                      counterCPDP1;
    /** Action count triggered by F2 */
    Int8                      counterCPDP2;
    /** Action count triggered by F3 */
    Int8                      counterCPDP3;
    /** Action count triggered by F4 */
    Int8                      counterCPDP4;
}
AlsiWriteRpmCountersReq;

typedef struct AlsiReadRpmVersionReqTag
{
    /** Command reference */
    Int16                     commandRef;
}
AlsiReadRpmVersionReq;

typedef struct AlsiWriteRpmVersionReqTag
{
    /** Command reference */
    Int16                     commandRef;
    /** Indicates rhe version of RPM implemented in the device */
    Int8                      version;
}
AlsiWriteRpmVersionReq;
#endif /* defined (FEA_RPM) */

/** @} */ /* End of SigAlsiSignals group */
/** @} */ /* End of SigAlsi group */
#endif
/* END OF FILE */
