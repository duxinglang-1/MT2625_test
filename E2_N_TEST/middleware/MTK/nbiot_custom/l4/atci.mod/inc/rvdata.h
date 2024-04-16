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
 * Shared memory resource file
 **************************************************************************/

#ifndef RVDATA_H
#define RVDATA_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>
#include <ki_sigbuf.h>
#include <rvutil.h>
#include <rvsystem.h>
#include <abss_typ.h>
#include <rvnmtyp.h>
#include <alsi_sig.h>
#include <abmm_sig.h>
#include <abem_typ.h>

#if defined (FEA_PHONEBOOK)
#include <ablm_typ.h>
#endif /* FEA_PHONEBOOK */

#include <absi_sig.h>

#include <abpd_sig.h>

#include <mux_cfg.h>
 
#if defined (ENABLE_AT_ENG_MODE)
#include <abem_sig.h>
#endif
#include <cici_sig.h>

#if defined (UPGRADE_SHARE_MEMORY)
#include <r2_hal.h>
#include <t1muxshmdrv.h>
#endif /* UPGRADE_SHARE_MEMORY */
#if defined (UPGRADE_SHMCL_SOLUTION)
#include <muxconn_at.h>      /* For at command data */
#include <t1msyspara_sig.h> 
#endif /* UPGRADE_SHMCL_SOLUTION  */


/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/***************************************************************************
 * Typed Constants
 **************************************************************************/

/* Job 109119: GSM 11.11 - 9.3:  minimum size of a CHV       */
/* N.B. This is also defined in afinc/afsi_lcl.h */
#define MIN_CHV_SIZE                      4

/* MAX_MLTS_STRING_LENGTH */
#define DOUBLE_QUOTATION_LENGTH           (2)
#define COMMA_LENGTH                      (1)
#define COMMA_AND_DOUBLE_QOUTATION_LENGTH (3)
#define LTZ_STRING_LENGTH                 (3)

/* lsaIdentity occurs 3 octets, the output of <LSA identity> is hex string,
 * so the length is 6 */
#define LSAID_STRING_LENGTH               (6)
#define DCS_LENGTH                        (1)

/* if <full name> or <short name> uses UCS2 coding,
 * the output of <full name> or <short name> is hex string,
 * so the length is double */
#define MAX_NETWORK_NAME_AND_DCS_LENGTH   (2*MAX_NETWORK_NAME_LENGTH + COMMA_AND_DOUBLE_QOUTATION_LENGTH + \
                                           DCS_LENGTH + COMMA_LENGTH )

/* if <full name> or <short name> uses CBS coding(7bit coding),
 * the output of <full name> or <short name> is decoded,
 * so the length is ((MAX_NETWORK_NAME_LENGTH*8)/7 + 1 ) */
#define MAX_NETWORK_NAME_LENGTH_IN_CBS    ((MAX_NETWORK_NAME_LENGTH<<3)/7 + 1)

#define MAX_MLTS_STRING_LENGTH            (VG_TIMESTAMP_SIZE + DOUBLE_QUOTATION_LENGTH + \
                                           VG_DST_STRING_LENGTH + COMMA_AND_DOUBLE_QOUTATION_LENGTH + \
                                           MAX_NETWORK_NAME_AND_DCS_LENGTH + \
                                           MAX_NETWORK_NAME_AND_DCS_LENGTH + \
                                           LTZ_STRING_LENGTH + COMMA_AND_DOUBLE_QOUTATION_LENGTH + \
                                           LSAID_STRING_LENGTH + COMMA_AND_DOUBLE_QOUTATION_LENGTH + \
                                           NULL_TERMINATOR_LENGTH)

#define GENERIC_PROFILE_DATA_BIT          (1)
#define SPECIFIC_PROFILE_DATA_BIT         (2)

#ifdef ENABLE_AP_BRIDGE_FEATURE

#ifndef AP_BRIDGE_MAX_AT_CMD_COUNT
#define AP_BRIDGE_MAX_AT_CMD_COUNT 500
#endif/*AP_BRIDGE_MAX_AT_CMD_COUNT*/

#ifndef ENABLE_AP_BRIDGE_FEATURE_DEBUG
//#define ENABLE_AP_BRIDGE_FEATURE_DEBUG
#endif/*ENABLE_AP_BRIDGE_FEATURE_DEBUG*/

#endif/*ENABLE_AP_BRIDGE_FEATURE*/

/***************************************************************************
 * Type Definitions
 **************************************************************************/

/* Note that the order of included files below is important */

#include <rvmxdata.h>
#include <rvccdata.h>
#include <rvmmdata.h>
#include <rvgndata.h>
#if defined (ENABLE_AT_ENG_MODE)
#include <rvemdata.h>
#endif

#include <rvcrdata.h>
#include <rvmsdata.h>
#include <rvchdata.h>
#include <rvssdata.h>
#include <rvtsdata.h>
#include <rvsldata.h>
#include <rvctcfg.h>

#include <rvsldata.h>
#include <rvctcfg.h>
#include <rvomtime.h>
#include <rvnvdata.h>
#include <ki_typ.h>

/***********************************************/
/* Data structures for static memory structure */
/* Which are dynamically used by ATCI          */
/***********************************************/
typedef struct InUseConfBuffDataItemTag
{
  Boolean           inUse;
  Int8              inUseConvBuff[VG_CR_CH_SET_CONV_BUFFER_LEN];
}InUseConvBuffDataItem_t;

typedef struct InUseOutputBuffDataItemTag
{
  Boolean           inUse;
  Int8              inUseOutputBuff[VG_CR_CH_SET_OUTPUT_BUFFER_LEN];
}InUseOutputBuffDataItem_t;

#if defined (FEA_PHONEBOOK)
typedef struct InUseAlphaBuffDataItemTag
{
  Boolean           inUse;
  Int8              inUseAlphaBuff[VG_CR_CH_SET_ALPHA_BUFFER_LEN];
}InUseAlphaBuffDataItem_t;

typedef struct InUseAlphaSearchBuffDataItemTag
{
  Boolean           inUse;
  Int8              inUseAlphaSearchBuff[VG_CR_CH_SET_ALPHA_SEARCH_LEN];
}InUseAlphaSearchBuffDataItem_t;
#endif /* FEA_PHONEBOOK */

typedef struct CrManagerContextTag
{
  ResultCode_t          resultCode;
  CommandId_t           currentAtCommand;
  const Int8            *currentAtCommandName_p;
  ConnectionClass_t     connectionTypeMade;
  Boolean               taskInitiated;
  CrOutputStreamCtrl_t  crOutputStreamCtrl;
  Int8                  *crChSetConversionBuffer_p;
  Int8                  *crChSetOutputBuffer_p;
  
#if defined (FEA_PHONEBOOK)  
  Int8                  *crAlphaStringBuffer_p;
  Int8                  *crAlphaSearchBuffer_p;
#endif /* FEA_PHONEBOOK */  

} CrManagerContext_t;

/** Generic context for CR Manager*/
typedef struct CrManagerGenericContextTag
{
  /** Buffer used by vgPrintf function*/
  Char    vgPrintfBuffer[ VG_PRINTF_CONV_BUFFER + NULL_TERMINATOR_LENGTH];
  /** Used to count how many cirmDataInd are waited for the flow control usage*/
  Int8    cirmDataIndCountFlowControl;

  /* RAM Buffers */
  InUseConvBuffDataItem_t        inUseConvBuffDataItem[ATCI_MAX_NUM_CONV_BUFF];
  InUseOutputBuffDataItem_t      inUseOutputBuffDataItem[ATCI_MAX_NUM_OUTPUT_BUFF];

#if defined (FEA_PHONEBOOK)  
  InUseAlphaBuffDataItem_t       inUseAlphaBuffDataItem[ATCI_MAX_NUM_ALPHA_BUFF];
  InUseAlphaSearchBuffDataItem_t inUseAlphaSearchBuffDataItem[ATCI_MAX_NUM_ALPHA_SEARCH_BUFF];
#endif /* FEA_PHONEBOOK */

#if defined(ATCI_ENABLE_DYN_AT_BUFF)
  atCmdBuffer_t                  atCmdBufferList[ATCI_MAX_NUM_AT_CMD_BUFFERS];
#endif
}CrManagerGenericContext_t;

typedef struct ChannelContextTag
{
    AtChannel_t                 at;
    VgDataEntryMode             dataEntryMode;
    /*
    * This is needed to allow us to delay configuration of ICF and IPR to MUX so that
    * "OK" string can be sent out first using timer TIMER_TYPE_PORT_SETTING_CHANGE
    */
    Boolean                     iprOrIcfSettingsChange;
    /*
    * Stored AT+CMUX parameters for sending to MUX to activate 27.010 MUX.
    * These are used when the TIMER_TYPE_CMUX_ACTIVATION timer has timed out.
    */
    CmuxCmdParams               cmuxCmdParams;
    /**
    *   Define the signal that should be sent when the AT MUX data buffer is empty.
    *   Can be NON_SIGNAL when no signal are waiting.
    */
    SignalId                    delayedSignalId;
    /**
    * Boolean to indicate if the channel is being disabled locally - if so then
    * we don't need to send a CIMUX_CHANNEL_DISABLED_RSP back to the MUX.
    */
    Boolean                     channelDisablingLocally;
    Boolean                     canRunMabortCmd;
    Boolean                     actDataIndNoReply;
} ChannelContext_t;

/* job106314: ScanParseContext_t moved to rvmxdata.h */

typedef struct NvramContextTag
{
  VgNvramState            nramState;
  ProfileContext_t        tmpSpecificProfile;
  ProfileGenericContext_t tmpGenericProfile;
  Int8                    readProfileFlag;
  Int8                    writeProfileFlag;
} NvramContext_t;

typedef struct OpmanContextTag
{
  CiCallControlStates_t   currentCcontrolState;
  Int8                    numberOfCallConnections;
  ConnectionClass_t       vgLastCallConnectionType;
  ConnectionInformation_t callInfo;   /* Can only have one PSD connection in data state
                                       * per channel
                                       */
} OpmanContext_t;




typedef struct SmsContextTag
{
  Int8                     smsMessage[VG_SMS_MAX_TEXT_LENGTH];
  Int16                    smsLength;
  Boolean                  validSms;
  Int16                    tpduLength;
  Int16                    maxSmsMessageLength;

  concatSms_t              concatSms;

  /** Storage used to list, read and delete messages */
  VgCPMSStorageType        cpmsReadDelete;
  /** Storage used to write and send messages */
  VgCPMSStorageType        cpmsWriteSend;

  Int8                     firstRecord;
  SmsCommandInfo           command;
  SmsAddress               da;
  VgSmspState              smspState;
  SmsSimAccessType         readState;
  Int8                     sendRetries;
  LmRecordNumber           cmglRecordNumberToSearchFrom;
  Int8                     cmgdRecordNum;
  Int8                     delFlag;
  SmReadReq                smReadReq;
  VgCmssParameters         atCmssParams;
  SimSmRecordStatus        smStatus;
  SmsTimeStamp             smTimeStamp;

  VgDisplaySmsParam        savedSmDisplayParam;     /* For alphaId lookup */

  const ApexSmReadSmspCnf *ApexSmReadSmspCnf_p;
  const CiapSms           *CiapSms_p;
  const TsCommandReq      *TsCommandReq_p;
  const ApexSmReadCnf     *ApexSmReadCnf_p;

  SignalBuffer             sigApexSmStoreReq;
  SignalBuffer             sigApexSmSendReq;
  SignalBuffer             sigApexSmCommandReq;
  SignalBuffer             sigApexSmDeliveryWithReportRsp;
  Boolean                  enableExtraUnsol;
  Int16                    smsStoreLoc;
  SimSmRecordStatus        smsMmgscStatus;
  Int16                    smsMmgscLoc;

  /** Contains parameters for the signal to send*/
  ApexSmStoreRsp           apexSmStoreRsp;
  ApexSmSmsrStoreRsp       apexSmSmsrStoreRsp;
} SmsContext_t;

#if defined (FEA_PPP)
typedef enum VgLoopbackPppModeTag
{
  LOOPBACK_PPP_MODE_STANDARD,
  LOOPBACK_PPP_MODE_CORE
} VgLoopbackPppMode;
#endif /* FEA_PPP */

typedef struct GprsContextTag
{
  /*
   * This stores a pointer to the context which has a data connection.  We can only
   * have one of these associated with a channnel.  We can have multiple contexts
   * which are not in data mode on one channel (i.e. activated using AT+CGACT).
   */
  VgPsdStatusInfo    *activePsdBearerContextWithDataConn;
  Boolean            pendingOpenDataConnCnf;
  Int8               vgDialCid;         /* Record which CID we are activating. We
                                         * need to do this to make sure we are activating
                                         * up the correct connection.
                                         */
  Int8               vgModifyCid;       /* Record which CID we are modifying. We
                                         * need to do this to make sure we are modifying
                                         * the correct connection.
                                         */
  VgHangupType       vgHangupType;      /* we need to record the hangup type
                                         * i.e. DTR or ATH as it is used within
                                         * ATCI task
                                         */
  Int8               vgHangupCid;       /* Record which CID we are hanging up. We
                                         * need to do this to make sure we are hanging
                                         * up the correct connection.
                                         */
  UserCallId         disconnectionItem; /* Call ident for disconnecting PSD
                                         * connection - PT and PPP connections only
                                         */
  Boolean            attachInProgress;  /* Indicating if we are performing an attach
                                         */
  Boolean            attachAbortInProgress;
                                        /* Indicates if we are performing a detach
                                         * as part of an attach abort.
                                         */
  Boolean            connectionActive;

#if defined (FEA_PPP)
  VgLoopbackState    vgPppLoopbackState;
  Int8               vgPppLoopbackNumDlXmit;
  Int8               vgPppLoopbackMode;
  Int16              vgPppLoopbackDlTimeout;
  Int16              vgPppLoopbackPacketSize;
  Int16              vgPppTotalNumDlXmit;

  VgLoopbackPppMode  vgPppLoopbackPppMode;
  Boolean            vgPppLoopbackFCSCheckingEnabled;
  Boolean            vgPppLoopbackByteStuffingEnabled;
  Boolean            vgPppLoopbackUlCheckingEnabled;
#endif /* FEA_PPP */  
}GprsContext_t;

typedef enum VgLteAttachDefaultBearerStateTag
{
  LTE_ATTACH_DEFAULT_BEARER_NOT_CONNECTED,
  LTE_ATTACH_DEFAULT_BEARER_PENDING_ABPD_DATA,
  LTE_ATTACH_DEFAULT_BEARER_PENDING_CONNECTION,
  LTE_ATTACH_DEFAULT_BEARER_CONNECTED
} VgLteAttachDefaultBearerState;

#define GPRS_GENERIC_CONTEXT_RTC_RAM_DATA \
    VgmuxChannelNumber cidOwner[MAX_NUMBER_OF_CIDS];\
    Int8               lteAttachDefaultBearerCid
  
typedef struct GprsGenericContextTag
{
  GPRS_GENERIC_CONTEXT_RTC_RAM_DATA;

  /*********************************************/
  /* Pre-allocated CID data items              */
  /* Limit number of PSD bearers we can set up */
  /*********************************************/
  InUsePdnMemDataItem_t inUsePsdMemData [ATCI_MAX_NUM_PSD_CONNECTIONS];

  VgPsdStatusInfo    *cidUserData [MAX_NUMBER_OF_CIDS];
  VgPsdBearerInfo     oldPsdBearerInfo;   /* Temporary to backup bearer info for some AT Commands */

#if defined (FEA_MT_PDN_ACT)
  ApexAbpdSetupRsp   vgAbpdSetupRsp;
  /* we save a copy of the signal for mt pdp context activation, so
     that we can set up the context when the user decides to
     accept. */
  ApexAbpdSetupInd   vgAbpdSetupInd;
#endif /* FEA_MT_PDN_ACT */

  VgCGEREPPdpContext vgPdpContext[MAX_NUM_PSD_BEARERS];
  VgMGSINKData       vgMGSINKData[MAX_NUM_PSD_BEARERS];
  /* *MCGDEFCONT data */
  AbpdApn            currentDefaultAPN;
  AbpdApn            definedDefaultAPN;
  SmsRoute           defaultSmsRoute;

#if defined (FEA_MT_PDN_ACT)
  /* incomingPdpContextActivation indicates that have have received a
     request from the network for mtpca, but have not yet responded,
     with ata/ath/at+cgans */
  Boolean            incomingPdpContextActivation;
  VgMMTPDPCIDData    vgMMTPDPCIDData;
#endif /* FEA_MT_PDN_ACT */

  GprsServiceState   gprsServiceState;
  VgCGREGData        vgCGREGData;
  VgCEREGData        vgCEREGData;
#if defined (FEA_ACL)
  VgCGACLData        vgCGACLData;  
#endif
  VgMDPDNPData     vgMDPDNPData;
  VgCSODCPData       vgCSODCPData;
  VgMGCOUNTData      vgMGCOUNTData;
#if defined (FEA_UPDIR)  
  VgMUPDIRData       vgMUPDIRData;
#endif
  /*
   * cidOwner is used to lock a CID to a specific
   * channel.  This is still required because we need to
   * store this before the CID information is allocated.
   */
  /* Allow for transient CID too */

  /* Allow for different channel to the one that owns the cid
   * to hangup a connection
   */
  VgLteAttachDefaultBearerState vgLteAttachDefaultBearerState;
  VgmuxChannelNumber hangupChannel;
  VgReqMNbiotDtData            vgReqNbiotDtData;
  AbpdRelAssistInformation     vgReqNbiotRelAssistData;
  VgPlmnUplinkRateControlInfo  plmnRateControlInfo;
} GprsGenericContext_t;

#if defined (FEA_MT_PDN_ACT)
typedef enum VgCGAUTOModeTag
{
  CGAUTO_MODE_NO_PS_AUTOANSWER = 0,
  CGAUTO_MODE_PS_AUTOANSWER = 1,
  CGAUTO_MODE_MODEM_COMPATIBILITY_PS_ONLY = 2,
  CGAUTO_MODE_MODEM_COMPATIBILITY_PS_AND_CS = 3
}
VgCGAUTOMode;
#endif /* FEA_MT_PDN_ACT */

typedef struct CallContextTag
{
  ApexCauseType       vgApexCallReleaseError;
  CiTimerError_t      vgTimerCallReleaseError;
  CiCallReleaseType_t vgErrorType;
  GsmCause            vgGsmCauseCallReleaseError;
  Int8                athParam;
} CallContext_t;

#if defined (ENABLE_AT_ENG_MODE)
typedef struct EngineeringModeContextTag
{
  VgEmAttributes     emAttributes;
  Boolean            waitForResultCode;
  Boolean            active;
  VgEmEntityconfig   entityConfig[CI_MAX_ENTITIES];
  Int8               TimePeriod;
  Int8               periodicCounter;
  /*  AT*MEMPSET Data */
  ApexEmForceCommand emForceCommand;
  NetworkModes       sysMode;
  EmCellInfo         cellInfo;
  EmLockArfcnInfo    arfcnInfo;
  EmNCellType        nCellType;
  Int8               timerPeriod;
  Boolean            handoverSwitch;
  /* AT*MEMPS Data */
  EmPowerSavingType  psType;
#if defined (ENABLE_AT_TRACE)
  Boolean            emTraceMode;
  Boolean            emTraceEnable;
#endif
} EngineeringModeContext_t;
#endif

typedef enum CesqReasonTag
{
  CESQ_QUERY_COMMAND,
  CESQ_MSQN_UNSOLICITED
} CesqReason_t;

typedef enum MsqnTimerStateTag
{
  MSQN_TIMER_OFF,
  MSQN_TIMER_INTERVAL,
  MSQN_TIMER_AVERAGING
} MsqnTimerState;

/* date for received signal quality unsolicited event filtering */

typedef struct MsqnStateTag
{
  Int8                  lastSentLevel;    /* what was the last level we reported to the host  */
  Int16                 lastSentRsrq;
  Int16                 lastSentRsrp;
} MsqnState;


#define MOBILITY_CONTEXT_RTC_RAM_DATA \
  ReceiveQuality        receiveQuality;\
  ReceiveLevel          receiveLevel;\
  EutraRsrp             eutraRsrp;\
  EutraRsrq             eutraRsrq;\
  uint16_t              vgdlber;\
  uint16_t              vgulber;\
  Boolean               haveReceiveInfo

typedef struct MobilityContextTag
{

  MOBILITY_CONTEXT_RTC_RAM_DATA;
  VgCPOLData            vgCPOLData;
  VgCOPNData            vgCOPNData;
  VgCOPSData            vgCOPSData;
  VgCREGData            vgCREGData;
  VgCPLSData            vgCPLSData;
  VgCSCONData           vgCSCONData;
  VgMFRCLLCKData        vgMfrcllckData;
  VgMBANDSLData         vgMandSLData;
  VgMENGINFOStatisData  vgMengInfoData;

  Boolean               vgNetworkPresent;
  ApexMmNetworkStateInd vgNetworkState;
  Char                  vgMLTSString [MAX_MLTS_STRING_LENGTH];
  Boolean               timeZoneInitialised;
  RtcDisplacement       newTimeZone;
  RtcDisplacement       currentTimeZone;

  ApexMmGAPowerUpOption gaOption;
#if defined (USE_ABAPP)
  Boolean               isImmediate;
#endif
  VgSYSCONFIGData       vgSYSCONFIGData;
#if defined (UPGRADE_SHARE_MEMORY)  || defined(UPGRADE_SHMCL_SOLUTION)
  Boolean       cregUpdated;
  Boolean       cgregUpdated;
  AtCregData    cregData;
  AtCgregData   cgregData;
#endif /* UPGRADE_SHARE_MEMORY || UPGRADE_SHMCL_SOLUTION */
  VgEdrxData        vgEdrxData;
  VgReqEdrxData     vgReqEdrxData;
  VgCciotoptData    vgCciotoptData;
  VgReqCciotoptData vgReqCciotoptData;
  VgCpsmsData       vgCpsmsData;
  VgReqCpsmsData    vgReqCpsmsData;
  VgCipcaOpt        vgReqCipcaOpt;   
  VgCipcaData       vgCipcaData;
  Int8  cnumDatarecordNumber;
#if defined (FEA_NFM)
  VgNfmData         vgNfmData;
  VgReqNfmData      vgReqNfmData;
  VgNfmcData        vgNfmcData;
  VgReqNfmcData     vgReqNfmcData;
#endif
#if defined (FEA_RPM)
  VgMrpmData        vgMrpmData;
#endif
  VgMHPLMNSData     vgMhplmnData;
  
  VgMEHPLMNData     vgMehplmnData;
  VgMOOSAINDData    vgMoosaIndData;
  
} MobilityContext_t;

#define HIDDEN_KEY_INOUT_STRING_LENGTH (2 * SIM_HIDDEN_KEY_LENGTH)

typedef struct GeneralContextTag
{
  Boolean               omitFirstNewLine;

#if defined (FEA_PHONEBOOK)
  VgLmData              vgLmData;
  Char                  hiddenKey[HIDDEN_KEY_INOUT_STRING_LENGTH + NULL_TERMINATOR_LENGTH];
  Char                  newHiddenKey[HIDDEN_KEY_INOUT_STRING_LENGTH + NULL_TERMINATOR_LENGTH];
  Boolean               enableFdn;
  Boolean               enableBdn;
  Boolean               updatingBdn;
  VgAlphaLookup         vgAlphaLookup[VG_MAX_USER_CALL_ID];
  Int8                  pendingAlphaReq;
#endif /* FEA_PHONEBOOK */

  Char                  password [MAX_PASSWORD_LENGTH + NULL_TERMINATOR_LENGTH];
  Int16                 passwordLength;
  Char                  newPassword [MAX_PASSWORD_LENGTH + NULL_TERMINATOR_LENGTH];
  Int16                 newPasswordLength;
  SimUiccPinFunction    pinFunction;
  SimUiccKeyRefValue    keyRef;       /*for UICC cards*/
  SimUiccKeyRefValue    altPinKeyReference;
  SimUiccHiddenKeyFunction hiddenKeyFunction;
  SimChvNum             chvNumber;   /*for GSM SIM cards*/

#if defined (FEA_SIMLOCK)  
  AbsiMepSelector       mepSelector;
  AbsiMepOperation      mepOperation;
#endif /* FEA_SIMLOCK */

  VgCgsnSnt             cgsnSnt;         /* Type of SN requested using AT+CGSN
                                          * command
                                          */

  VgMcgsnData           mcgsnData;        /* Type of SN requested using AT*MCGSN
                                           * command
                                           */
                                          
} GeneralContext_t;

/**
  * Contain general global data shared between all the entities
  */
typedef struct GeneralGenericContextTag
{
    InitDataFromABPDState   initDataFromABPDState;
    /** Some data specific at *MFTRCFG command*/
    VgMFtrCfgData       vgMFtrCfgData;
#if defined (UPGRADE_SHMCL_SOLUTION)
    MtsUpdate           mtsUpdateData;
#endif /* UPGRADE_SHMCL_SOLUTION  */

/*******************************/
/* Context data for NB-IOT     */
/*******************************/
    VgMnvmMcalContext   vgMnvmMcalContext;
    VgNvmMspchscContext vgNvmMspchscContext;
}GeneralGenericContext_t;


#define MAX_DATA_MESSAGE_SIZE  (500)    /*job119784: increased to handle max length Text Strings*/

typedef struct StkBufferTag
{
  Int16   dataLength;
  Char    data[MAX_DATA_MESSAGE_SIZE];
} StkBuffer_t;

/* list of MMI relevant events currently being monitored for */
typedef struct StkMmiEventListTag
{
    /** is user activity currently being monitored for? */
    Boolean                       userActivity;
    /** is Idle Screen availability currently being monitored for? */
    Boolean                       idleScreenAvailable;
    /** is Language selection currently being monitored for?  */
    Boolean                       languageSelection;
    /** is Browser Termination currently being monitored for? */
    Boolean                       browserTermination;
    /** change of Display parameters currently being monitored for? */
    Boolean                       displayParamsChanged;

}
StkMmiEventList;

typedef enum StkRunAtCmdStateTag
{
  STATE_STK_NULL,
  STATE_RESERVING_CHANNEL,
  STATE_ENABLING_CHANNEL,
  STATE_CHANNEL_ENABLED,/* used for when we have a channel but aren't using it */
  STATE_RUNNING_COMMAND,
  STATE_COMMAND_COMPLETED,
  STATE_FREEING_CHANNEL,
  STATE_FREEING_CHANNEL_ENABLE_FAILED
}
StkRunAtCmdState;

typedef struct StkRunAtCmdDataTag
{
 /* used to assign an entity to when doing a RUN AT COMMAND proactive command */
  VgmuxChannelNumber  cmdEntity;
  SimatAtCommandData  *cmdInput_p;
  SimatAtCommandData  *cmdOutput_p;
  Int8                cirmDataIndCount;
}StkRunAtCmdData;


typedef struct StkEntityGenericDataTag
{
  /* vgstktp.c data */
  Boolean             simHasPassedPinCheck;
  Boolean             modemSupportingStk;
  Boolean             proactiveSessionStarted;
  VgmuxChannelNumber  registeredStkEntity;
  StkBuffer_t*        stkCommandBuffer [SIMAT_MAX_NUM_ITEMS + 1];
  Int16               cmdId;
  StkRunAtCmdState    runAtCmdState;
  StkRunAtCmdData     atCommandData;
  SignalId            currentSignal;
  Boolean             dataValid;
  Boolean             menuHasBeenRemoved;
  Int8                firstMenuItemId; /* job102163: for +STMS=? range check */
  Int8                currentMenuItems;
  Int8                minInputRange;
  Int8                maxInputRange;
  Int32               simCommandId;
  Boolean             registeredForStk [CI_MAX_ENTITIES];
  Boolean             vgToneIsCallTone;
  SimatMmiSupportProfile  *currentMmiProfile_p;
  Boolean             responseTimerEnabled;
  StkMmiEventList     currentEventList;
} StkEntityGenericData_t;

typedef struct SupplementaryContextTag
{
  VgSsSignalParams    ssParams;
} SupplementaryContext_t;

typedef struct SsCallRelatedContextTag
{
  VgSsCallerIdData    vgSsCallerIdData[MAX_USER_CALL_ID];
} SsCallRelatedContext_t;

typedef enum SimReadParamTypeTag
{
    SIM_READ_PARAM_SST,
    SIM_READ_PARAM_GID
}SimReadParamType;

typedef struct SimLockGenericContextTag
{
    Boolean             simLocked;
    Boolean             simBlocked;
    SimUiccKeyRefValue  keyRef;                /* for UICC cards */
    SimChvNum           chvNum;                /* for GSM SIM cards */
    Char                pinCode[SIM_CHV_LENGTH + NULL_TERMINATOR_LENGTH];
    Char                pukCode[SIM_CHV_LENGTH + NULL_TERMINATOR_LENGTH];
    Boolean             simEmulate;
    VgSimInsertedState  simInsertedState;
    Boolean             simRejected;
    Boolean             simWrong;
    Boolean             simPoweredUp;          /* is the SIM currently powered up? */
    Boolean             awaitingChvRsp;
    VgSimState          simState;
    VgSimInfo           simInfo;
    Boolean             powerUpProtoStack;
    Boolean             powerUpSim;            /* the SIM should power up */
    /** The EF(DIR) index of the currently running application*/
    Int8                activatedAidIndex;
    SimReadParamType    simReadParamType;

#if defined (SIM_EMULATION_ON)
    /** Stores the data we are going to send to the USIM manager in order to update
     * USIM emulator files.
     */
    AlsiWriteUsimEmuFileReq         alsiWriteUsimEmuFileReq;
#endif /* SIM_EMULATION_ON */
} SimLockGenericContext_t;

typedef struct SimLockContextTag
{
    VgCFUNData                      vgCFUNData;
    VgGenSimAccess_t                simGenAccess;
    VgSimLogivalChannelAccess_t     simLogicalChannelAccess;
#if defined (ENABLE_DUAL_SIM_SOLUTION)
    SimHolderType                   simHolderToSelect;
#endif
    Int8                            commandField;
    Int8                            startField;
    /** MNON specifics data*/
    VgMnonData                      vgMnonData;
    /* MUAPP specifics data*/
    VgMuappData                     vgMuappData;

    /* The start record for a DIR request*/
    Int8                            dirReqStartRecord;
    /* The number of record to read, can be 0 to read the maximum allowed*/
    Int8                            dirReqNumRecord;
    /* USIM Manager locked for external AT+CSIM operation*/
    Boolean             csimLocked;

    VgUiccLogicChanInfo             vgUiccLogicChannelData;
} SimLockContext_t;

typedef struct EntityMobilityContextTag
{
  MsqnState               msqnState;
} EntityMobilityContext_t;

#ifdef ENABLE_AP_BRIDGE_FEATURE

#define ENTITY_APB_CONTEXT_RTC_RAM_DATA \
  Boolean apbChannels[CI_MAX_ENTITIES]
#ifndef APBRIDGE_DATA_MODE_CHANNEL_COUNT
#define APBRIDGE_DATA_MODE_CHANNEL_COUNT 3
#endif

typedef enum
{
  RV_APB_DATA_MODE_DEACTIVATED      = 0,
  RV_APB_DATA_MODE_ACTIVATING       = 1,
  RV_APB_DATA_MODE_ACTIVATED        = 2,
  RV_APB_DATA_MODE_TEMP_DEACTIVATED = 3
} RvApbDataModeState_t;

typedef struct ApBridgeContextTag
{
  ExtendedOperation_t operation; /*At command operation type for specific command.*/
} ApBridgeContext_t;

typedef struct ApBridgeDataModeContextTag
{
  RvApbDataModeState_t dataModeState;
  ResultCode_t dataModeResultCode;
  ExtendedOperation_t dataModeOperationMode;
  Int16 channelIdInDataMode;
  CommandId_t dataModeCmdId; /*The command ID which has been swithched to data mode.*/
  Int8 dataModeCustomResult[40];
  Boolean pendingOpenDataConnCnf; /*waiting for open data connection confirmation siganl.*/
} ApBridgeDataModeContext_t;

typedef struct ApBridgeGenericContextTag
{
  Int16 registeredCmdCount;  /*current registered AT command count.*/
  ApBridgeDataModeContext_t dataModeCtx[APBRIDGE_DATA_MODE_CHANNEL_COUNT];
  Boolean cmdRegistered;
  Int32   pendingCmdCount;
  Int8    pendingCmdChanelIds[CI_MAX_ENTITIES];
  Boolean apbChannels[CI_MAX_ENTITIES];/*Memorize the channels which sent AT command for AP Bridge.
                                         The index of the array is the channel id. The value of the array
                                         is the channel's status for AP Bridge: FALSE: inactive, TRUE: active.*/
  Int32 apAtCommandTableItemSize; /*The table size for saving at command handlers.*/
  void* apAtCommandTable_p; /*points to AT command handler table.*/
} ApBridgeGenericContext_t;
#endif

typedef struct SleepManContextTag
{
  ControlResetCause     powerOnCause;
  Boolean               firstSleep;  /* TRUE for first SLEEP after power-on - FALSE
                                      * otherwise
                                      */
  Boolean               atciInWakeupState;   /* This is FALSE when either not waking up
                                              * or wakeup completed
                                              */
  Boolean               atciWokeUp;         /* This is FALSE when we cold booted,
                                             * otherwise it is always set to TRUE
                                             * indicate this power-on was a wakeup.
                                              */
  Boolean               needSimNok;
  Boolean               needSimOk;
  Boolean               needNetworkStateInd;
  Boolean               needSmReadyInd;
  Boolean               needGlReadyInd;

#if defined (FEA_PHONEBOOK)  
  Boolean               needLmReadyInd;
#endif

  /* TODO: Add more READY_INDs, other signals as required */
  
  Int8                  numPsdBearerStatusIndsNeeded;

}SleepManContext_t;

typedef struct EntityContextDataTag
{
  ScanParseContext_t      scanParseContext;
  ChannelContext_t        channelContext;
  NvramContext_t          nvramContext;
  ProfileContext_t        profileContext;
  CrManagerContext_t      crManagerContext;
  OpmanContext_t          opManContext;
  GeneralContext_t        generalContext;
  CallContext_t           callContext;
  GprsContext_t           gprsContext;
  SmsContext_t            smsContext;
  SupplementaryContext_t  supplementaryContext;
  SimLockContext_t        simLockContext;
  CustomContext_t         customContext;
  EntityMobilityContext_t entityMobilityContext;
  #ifdef ENABLE_AP_BRIDGE_FEATURE
  ApBridgeContext_t       apBridgeContext;
  #endif
} EntityContextData_t;

/***********************************************/
/* Data structures for static memory structure */
/* Which are dynamically used by ATCI          */
/***********************************************/
typedef struct InUseEntityMemDataItemTag
{
  Boolean               inUse;
  EntityContextData_t   entityContextData;
}InUseEntityMemDataItem_t;

#define ENTITY_GENERIC_CONTEXT_RTC_RAM_DATA \
    Boolean            entityActive[CI_MAX_ENTITIES]

typedef struct EntityTag
{
  ENTITY_GENERIC_CONTEXT_RTC_RAM_DATA;
  MuxContext_t                  muxContext;
  MobilityContext_t             mobilityContext;
#if defined (ENABLE_AT_ENG_MODE)
  EngineeringModeContext_t      engineeringModeContext;
#endif
  OpmanGenericContext_t         opmanGenericContext;
  ChManagerContext_t            chManagerContext;
  SimLockGenericContext_t       simLockGenericContext;
  GprsGenericContext_t          gprsGenericContext;
  SmsCommonContext_t            smsCommonContext;
  StkEntityGenericData_t        stkEntityGenericData;
  SsCallRelatedContext_t        ssCallRelatedContext;
  CustomGenericContext_t        customGenericContext;
  GeneralGenericContext_t       generalGenericContext;
  ProfileGenericContext_t       profileGenericContext;
  CrManagerGenericContext_t     crManagerGenericContext;
  #ifdef ENABLE_AP_BRIDGE_FEATURE
  ApBridgeGenericContext_t      apBridgeGenericContext;
  #endif
  SleepManContext_t             sleepManContext;
  EntityContextData_t          *entitySpecificData [CI_MAX_ENTITIES];
         
  /* Fixed memory used for each entity in use */
  InUseEntityMemDataItem_t      inUseEntityMemDataItem[ATCI_MAX_NUM_ENABLED_AT_CHANNELS];
} Entity_t;





/***************************************************************************
 *  Macros
 **************************************************************************/

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

/* mandatory contexts */
Entity_t*                ptrToAtciContextData();

EntityContextData_t*     ptrToEntityContextData     ( const VgmuxChannelNumber entity );
ScanParseContext_t*      ptrToScanParseContext      ( const VgmuxChannelNumber entity );
MuxContext_t*            ptrToMuxContext            ( void );
ChannelContext_t*        ptrToChannelContext        ( const VgmuxChannelNumber entity );
NvramContext_t*          ptrToNvramContext          ( const VgmuxChannelNumber entity );
ProfileGenericContext_t* ptrToProfileGenericContext    ( void );
ProfileContext_t*        ptrToProfileContext        ( const VgmuxChannelNumber entity );
ChManagerContext_t*      ptrToChManagerContext      ( void );
CrManagerContext_t*      ptrToCrManagerContext      ( const VgmuxChannelNumber entity );
CrManagerGenericContext_t* ptrToCrManagerGenericContext ( void );
OpmanContext_t*          ptrToOpManContext          ( const VgmuxChannelNumber entity );
OpmanGenericContext_t*   ptrToOpManGenericContext   ( void );
SimLockGenericContext_t* ptrToSimLockGenericContext ( void );
SimLockContext_t*        ptrToSimLockContext        ( const VgmuxChannelNumber entity );
CallContext_t*           ptrToCallContext           ( const VgmuxChannelNumber entity );
GeneralContext_t*        ptrToGeneralContext        ( const VgmuxChannelNumber entity );
GeneralGenericContext_t* ptrToGeneralGenericContext ( void );
MobilityContext_t*       ptrToMobilityContext       ( void );
EntityMobilityContext_t* ptrToEntityMobilityContext ( const VgmuxChannelNumber entity );

/* optional contexts */
#if defined (ENABLE_AT_ENG_MODE)
EngineeringModeContext_t*  ptrToEngineeringModeContext ( void );
#endif
VgmuxChannelNumber*      ptrToStkRegisteredEntity   ( void );
StkEntityGenericData_t*  ptrToStkGenericContext     ( void );
SmsCommonContext_t*      ptrToSmsCommonContext      ( void );
SmsContext_t*            ptrToSmsContext            ( const VgmuxChannelNumber entity );
GprsContext_t*           ptrToGprsContext           ( const VgmuxChannelNumber entity );
GprsGenericContext_t*    ptrToGprsGenericContext    ( void );
SupplementaryContext_t*  ptrToSupplementaryContext  ( const VgmuxChannelNumber entity );
SsCallRelatedContext_t*  ptrToSsCallRelatedContext  ( void );

/* Custom generic and entity access pointers */
CustomGenericContext_t*  ptrToCustomGenericContext ( void );
CustomContext_t*         ptrToCustomContext        ( const VgmuxChannelNumber entity );

void allocateMemToEntity                ( const VgmuxChannelNumber entity, void  *dPtr );
void initialiseMemToEntity              ( const VgmuxChannelNumber entity );
void allocateMemToGprsContext           ( const Int32 thisCid, void  *dPtr );
#ifdef ENABLE_AP_BRIDGE_FEATURE
ApBridgeContext_t*  ptrToApBridgeContext( VgmuxChannelNumber entity );
ApBridgeGenericContext_t* ptrToApBridgeGenericContext( void );
#endif
SleepManContext_t* ptrToSleepManContext ( void );

void initMcalContextData                ( VgMcalContext            *mcalContext_p);
#endif

