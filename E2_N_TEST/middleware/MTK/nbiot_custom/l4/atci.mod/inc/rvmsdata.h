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
 * vgmsdata.h
 * Types and data for the SMS subsystem.
 **************************************************************************/

#ifndef RVMSDATA_H
#define RVMSDATA_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>
#include <rvsystem.h>
#include <absm_typ.h>
#include <absm_sig.h>
#include <abcb_typ.h>

/***************************************************************************
 * Manifest Constants - User alterable
 **************************************************************************/

/* Default SCA to use when have no NVRAM/SIM */
#define VG_SMS_DEFAULT_SCA_NUMBER       ("00000000000")
#define VG_SMS_DEFAULT_SCA_ALPHAID      ("<DEFAULT>")

/*
** Number of times to resend an SMS (or part of a concatenated SMS) when
** the PS says it is busy (probably receiving an SMS!).
**
** Note: this can happen if we send ourselves a concatenated SMS as a test.
*/
#define VG_SMS_MAX_SEND_RETRIES   (4)

/*
** Number of call ids to use for doing phonebook lookups
**
** - see vggnsigi.c:vgSigApexLmGetAlphaCnf
**       vggnsigo.c:vgSigApexLmGetAlphaReq
*/
#define VG_SMS_LMGETALPHA_NUMBER_OF_CALL_IDS  (2)
#define VG_SMS_LMGETALPHA_COMMAND_CALL_ID     (MAX_POSSIBLE_CC_CONNECTIONS + 1)
#define VG_SMS_LMGETALPHA_UNSOLICITED_CALL_ID (MAX_POSSIBLE_CC_CONNECTIONS + 2)

/***************************************************************************
 * Manifest Constants - NOT User alterable
 **************************************************************************/

/* Invalid record number used to init. cmglRecordNumberToSearchFrom */
#define VG_SMS_INVALID_RECORD_NUMBER (0xFF)

/* Maximum sms message size for non-concatenated messages for text and PDU modes */
#define VG_SMS_MAX_MSG_LENGTH_GSM (160)
#define VG_SMS_MAX_MSG_LENGTH_HEX (140)
#define VG_SMS_PDU_TPDU_SIZE      (232)

#  define VG_SMS_CONCAT_BLOCK     (152)
#  define VG_SMS_MAX_TEXT_LENGTH  (1024)


/* We define the maximum allowed SMS address as the product of the allowed
 * length and the largest character symbol size
 */
#define VG_CR_SMS_MAX_ADDR_LEN (SMS_MAX_ADDR_LEN * VG_CR_SYMBOL_LENGTH_UCS2)

#define VG_TIMESTAMP_SIZE        (20)
#define VG_TIMESTAMP_GROUP       (7)
#define VG_CR_SMS_MAX_TIMESTAMP_LEN (VG_TIMESTAMP_SIZE * VG_CR_SYMBOL_LENGTH_UCS2)


#define VG_SMS_PDU_GSM_STANDARD  (0)
#define VG_SMS_PDU_WITH_CNMA_ACK (1)
#define VG_SMS_PDU_TPDU_ONLY     (128)


/*
** Maximum length (total number of TE characters) of SC
** Address data when inputting PDU for +CMGS and +CMGW.
*/
#define VG_SMS_MAX_LEN_SCA_INFO  (24)

#define VG_SMS_INIT_SIMSENDRECORD (0xff)

#define VG_SMS_MAX_ADDRESS_LENGTH          (0x14)

/* Default text mode parameters */

#define VG_SMS_TEXTMODE_DEFAULT_FIRSTOCTET          (17)

#define VG_SMS_TEXTMODE_DEFAULT_STATUSREPORTREQ     (FALSE)
#define VG_SMS_TEXTMODE_DEFAULT_REJECTDUPLICATES    (FALSE)
#define VG_SMS_TEXTMODE_DEFAULT_REPLYPATH           (FALSE)

#define VG_SMS_TEXTMODE_DEFAULT_PROTOCOLMEANING     (PM_SM_AL_PROTOCOL)
#define VG_SMS_TEXTMODE_DEFAULT_PROTOCOLID          (PID_IMPLICIT)
#define VG_SMS_TEXTMODE_DEFAULT_DCS_MSGCODING       (MSG_CODING_DEFAULT)
#if defined (UPGRADE_3G)
/* The default message class value is changed only in 3G code
 * so there is no impact on 2G SMS test cases */
#define VG_SMS_TEXTMODE_DEFAULT_DCS_MSGCLASS        (MSG_CLASS_NOT_GIVEN)
#else
#define VG_SMS_TEXTMODE_DEFAULT_DCS_MSGCLASS        (MSG_CLASS1)
#endif
#define VG_SMS_TEXTMODE_DEFAULT_DCS_COMPRESSEDTEXT  (FALSE)
#define VG_SMS_TEXTMODE_DEFAULT_DCS_MSGWAITINGINDPRESENT (FALSE)
#define VG_SMS_TEXTMODE_DEFAULT_DCS_MARKEDFORAUTOMATICDELETION (FALSE)

#define VG_SMS_DEFAULT_VALIDITYPERIOD       (167)
#define VG_SMS_MAX_VALIDITYPERIOD_VALUE     (0xff)

/* This value is separate and MAX_LP_ENTRIES                                   *
 * It does not define the max lp storage, that is the resposibility of CB code *
 * Instead it defines the storage of lp values waiting to be send to APEX      */
#define VG_LP_NUM_ENTRIES      (CB_LAST_LP + 1) /* it should be possible to disable the whole range */

#define VG_MMGI_NB_PROF_MAX    2

/***************************************************************************
 * Type Definitions
 **************************************************************************/
typedef struct CiapSmsTag
{
  SmsAddress        dest;
  SmsAddress        sca;
  SmsTimeStamp      timeStamp;
  SimSmRecordStatus status;
  Int8              message[VG_SMS_MAX_MSG_LENGTH_GSM];
  Int8              msgLength;
  Boolean           isConcat;
  Int8              seqLength;
  Int8              seqNumber;
  Int8              concatRef;
}
CiapSms;


typedef struct SmReadReqTag
{
  SmsSimAccessType    readType;
  Int8                record;
}
SmReadReq;

typedef struct SmsCommandInfoTag
{
  Int8           firstOctet;
  SmsCommand     commandType;
  Int8           pidByteVal;
  SmsProtocolId  pid;
  Int8           msgNum;
} SmsCommandInfo;


typedef enum VgSmsStatusSIMOverflowTag
{
  VG_SIM_SMS_SPACE_AVAILABLE = 0,
  VG_SIM_SMS_BUFFER_FULL,
  VG_SIM_SMS_BUFFER_FULL_AND_WAIT
} VgSmsStatusSIMOverflow;


typedef struct VgCmssParametersTag
{
  Boolean     useNewDest;
  SmsAddress  dest;
  Int16       recordNumber;
} VgCmssParameters;



typedef enum VgSmspStateTag
{
  VG_SMSP_READ_QUERY_SCA = 0,
  VG_SMSP_READ_WRITE_SCA,
  VG_SMSP_READ_QUERY_VP,
  VG_SMSP_READ_WRITE_VP,
  VG_SMSP_READ_UNSOLICITED,      /* internal read when VSIM ready */
  VG_SMSP_READ_BLOCK_QUERY       /* used for AT&V */
} VgSmspState;

typedef SimAlphaIdentifier VgSmsAlphaId;

typedef enum { VG_SIM_SMTPDU_SUBMIT, VG_SIM_SMTPDU_DELIVER } SimSmtpduSubmitDeliverType;

typedef struct concatSmsTag {
  Int8                  sequenceNumber;
  Int8                  sequenceLength;
  Int8                  concatRef;
} concatSms_t;

typedef enum VgMMGIEventTag
{
  VG_MMGI_SIM_ACCESS_NOK   = 0, /* Access to Sim records unavailable     */
  VG_MMGI_SIM_ACCESS_OK    = 1, /* Access to Sim records available       */
} VgMMGIEventType;

typedef enum VgCPMSStoragetypeTag
{
  VG_CPMS_STORAGE_SM         = 0, /* SMS               Storage is in SIM card record */
  VG_CPMS_STORAGE_SR         = 1, /* SMS Status Report Storage is SIM or/and Phone memory */
  VG_CPMS_MAX_STORAGE_TYPE   = 2  /* Max memory storage type */
} VgCPMSStorageType;

typedef enum VgCPMSParamTag
{
  VG_CPMS_READ_DELETE        = 0, /* Memory used to list, read and delete messages */
  VG_CPMS_WRITE_SEND         = 1, /* Memory used to write and send messages */
  VG_CPMS_PREF_STORE         = 2, /* Memory to which received SMS messages are prefered to be stored */
  VG_CPMS_MAX_MEM            = 3  /* Max CPMS memory parameter number */
} VgCPMSParam_t;

typedef struct SmsCommonContextTag
{
    VgSmsStatusSIMOverflow  actualSmsStatusSIMOverflow;
    ApexSmReadyInd          smSimState;
    Boolean                 smSimStateInitialized;/*True after we received the first ApexSmReadyInd signal*/
    SmsAddress              sca;
    SmsAddress              vgTempSca;
    VgSmsAlphaId            scaAlphaId;
    Int8                    validityPeriodValue;
    SmsTimeStamp            validityPeriodAbsolute;
    SmsDataCoding           dataCodingScheme;
    Boolean                 msgToAck; /* Information that SM needs to be acknowledged */
    Int8                    shortMsgId;
    /** Does we have to ack the SMS or an error occurs */
    SmsStatusOfReport       msgStatus;

    /* job115421: remove compile switch */
    Int8                    firstOctet;

    SmsProtocolId           protocolId;

    ApexSmDeliveryInd       savedUnsolSmDeliveryInd; /* For alphaId lookup */

    Boolean                 requeueSignal;
    Boolean                 unsolicitedChannelBusy; /* Serialization of unsolicited indication handling */

#if defined (FEA_PHONEBOOK)
    Boolean                 enableAlphaId;
#endif /* FEA_PHONEBOOK */

    Boolean                 enableExtraInfo;

    /* job116776 Introducing moreMessagesToSend functionality */
    Int8                    moreMsgsToSendMode;

    /** Storage to use for newly received SMS*/
    VgCPMSStorageType       cpmsStore;

    /** True when the +CMT notification have to be printed only on MME channel */
    Boolean                 printCmtOnlyOnMmi;
} SmsCommonContext_t;

/**
 ** ATCI SMS status value
 **/
typedef enum VgSmsStatusTag
{
    /** Received unread message*/
    VG_SMS_STATUS_UNREAD,
    /** Received read message*/
    VG_SMS_STATUS_READ,
    /** Received unsent message*/
    VG_SMS_STATUS_UNSENT,
    /** Received sent message*/
    VG_SMS_STATUS_SENT,
    /** First invalid SMS status*/
    VG_SMS_MAX_STATUS,
}VgSmsStatus;

/**
 ** Contains the informations required to print a SMS
 **/
typedef struct VgDisplaySmsParamTag
{
    /** The SMS PDU type*/
    SimSmTpduType       tpduType;
    /** SMS PDU*/
    SimSmTpdu           shortMessageTpdu;
    /** SMS status*/
    SimSmRecordStatus   recordStatus;
    /** Record number read */
    LmRecordNumber      recordNumber;
    /** TRUE if record contains a concatenated SMS */
    Boolean             concatSmsPresent;
    /** The record number of the first concatenated SMS (if present).*/
    Int8                concatSmsFirstRec;
    /** The previous concatenated SMS record number (if present). 
    ** Equal to current record if this read contains the first record.
    **/
    Int8                concatSmsPrevRec;
    /** Next concatenated SMS record number (if present). 
    ** Equal to current record if this read contains the last record.
    **/
    Int8                concatSmsNextRec;
    /** Service centre address */
    SmsAddress          scAddr;
}VgDisplaySmsParam;

/**
 ** Contains the informations required to print a SMS-SR
 **/
typedef struct VgDisplaySmsSrParamTag
{
    /** Record number read */
    Int8                recordNumber;
    /** SMS Status Report */
    TsStatusReportInd   smStatusReport;
    /** True if we are processing a ME request*/
    Boolean             processingMe;
}VgDisplaySmsSrParam;

void vgSmsInitialiseData (const VgmuxChannelNumber entity);
void initCommonEntityData (void);

#endif  /* RVSMDATA_H */

/* END OF FILE */



