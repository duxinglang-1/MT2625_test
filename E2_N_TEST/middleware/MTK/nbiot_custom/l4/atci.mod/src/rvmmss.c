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
 ***************************************************************************
 * File Description:
 * Handler for Mobility Management AT Commands and related signals
 **************************************************************************/

#define MODULE_NAME     "RVMMSS"

/***************************************************************************
 * Include Files
 ***************************************************************************/

#include <system.h>
#include <cici_sig.h>
#include <rvutil.h>
#include <rvdata.h>
#include <rvmmss.h>
#include <rvmmut.h>
#include <rvchman.h>
#include <rvmmsigo.h>
#include <rvmmsigi.h>
#include <dmrtc_sig.h>
#if defined (UPGRADE_SHARE_MEMORY)
#include <r2_hal.h>
#include <t1muxshmdrv.h>
#endif
/***************************************************************************
 * Signal definitions
 ***************************************************************************/

union Signal
{
    CiRunAtCommandInd ciRunAtCommandInd;
};

/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/* General command information table */

static const AtCmdControl mmAtCommandTable[] =
{
    {ATCI_CONST_CHAR_STR "+COPS",    vgMmCOPS,   VG_AT_MM_COPS,   AT_CMD_ACCESS_NONE},
    {ATCI_CONST_CHAR_STR "+CPOL",    vgMmCPOL,   VG_AT_MM_CPOL,   AT_CMD_ACCESS_NONE},
    {ATCI_CONST_CHAR_STR "+CREG",    vgMmCREG,   VG_AT_MM_CREG,   AT_CMD_ACCESS_NONE},
    {ATCI_CONST_CHAR_STR "*MGATTCFG", vgMmGATTCFG, VG_AT_MM_GATTCFG, AT_CMD_ACCESS_CFUN_1},
    {ATCI_CONST_CHAR_STR "+CPLS",    vgMmCPLS,   VG_AT_MM_CPLS,   AT_CMD_ACCESS_SIM_READY_AND_PRESENT},
    {ATCI_CONST_CHAR_STR "^SYSCONFIG", vgMmSYSCONFIG, VG_AT_MM_SYSCONFIG, AT_CMD_ACCESS_CFUN_1},
    {ATCI_CONST_CHAR_STR "*MNON",    vgMmMNON,       VG_AT_MM_MNON,      AT_CMD_ACCESS_SIM_READY_AND_PRESENT},
    {ATCI_CONST_CHAR_STR "*MOPL",    vgMmMOPL,       VG_AT_MM_MOPL,      AT_CMD_ACCESS_SIM_READY_AND_PRESENT},
    {ATCI_CONST_CHAR_STR "*MBAND",   vgMmMBAND,      VG_AT_MM_MBAND,     AT_CMD_ACCESS_NONE},
    {ATCI_CONST_CHAR_STR "*MBANDSL",   vgMmMBANDSL,      VG_AT_MM_MBANDSL,     AT_CMD_ACCESS_NONE},
#if defined(UPGRADE_MTNET)
    {ATCI_CONST_CHAR_STR "+OFF",     vgMmOFF,        VG_AT_MM_OFF,       AT_CMD_ACCESS_NONE},
    {ATCI_CONST_CHAR_STR "+RESET",   vgMmRESET,      VG_AT_MM_RESET,     AT_CMD_ACCESS_NONE},
#endif
    {ATCI_CONST_CHAR_STR "+CSCON",   vgMmCSCON,           VG_AT_MM_CSCON,     AT_CMD_ACCESS_NONE},
    {ATCI_CONST_CHAR_STR "*MFRCLLCK",vgMmMFRCLLCK,         VG_AT_MM_MFRCLLCK,  AT_CMD_ACCESS_NONE},
    {ATCI_CONST_CHAR_STR "+CESQ",         vgMmCESQ,       VG_AT_MM_CESQ,      AT_CMD_ACCESS_NONE},
    {ATCI_CONST_CHAR_STR "+CSQ",         vgMmCSQ,       VG_AT_MM_CSQ,      AT_CMD_ACCESS_NONE},    
    {ATCI_CONST_CHAR_STR "*MENGINFO",     vgMmMENGINFO,   VG_AT_MM_MENGINFO,  AT_CMD_ACCESS_CFUN_1},

    {ATCI_CONST_CHAR_STR "*MEMMREEST",vgMmMEMMREEST, VG_AT_MM_MEMMREEST, AT_CMD_ACCESS_CFUN_1},
    {ATCI_CONST_CHAR_STR "*MCELLINFO",  vgMmMCELLINFO,    VG_AT_MM_MCELLINFO,  AT_CMD_ACCESS_CFUN_1},
    {ATCI_CONST_CHAR_STR "*MEHPLMN",    vgMmMEHPLMN,      VG_AT_MM_MEHPLMN,    AT_CMD_ACCESS_NONE},
    {ATCI_CONST_CHAR_STR "*MHPLMNS",    vgMmMHPLMNS,      VG_AT_MM_MHPLMNS,    AT_CMD_ACCESS_NONE},

    /* AT commands for NB-IOT */
    {ATCI_CONST_CHAR_STR "+CEDRXS",         vgMmCEDRXS,       VG_AT_MM_CEDRXS,      AT_CMD_ACCESS_NONE},
    {ATCI_CONST_CHAR_STR "+CEDRXRDP",       vgMmCEDRXRDP,     VG_AT_MM_CEDRXRDP,    AT_CMD_ACCESS_NONE},
    {ATCI_CONST_CHAR_STR "+CCIOTOPT",       vgMmCCIOTOPT,     VG_AT_MM_CCIOTOPT,    AT_CMD_ACCESS_NONE},
    {ATCI_CONST_CHAR_STR "+CPSMS",          vgMmCPSMS,        VG_AT_MM_CPSMS,       AT_CMD_ACCESS_NONE},
    {ATCI_CONST_CHAR_STR "+CIPCA",          vgMmCIPCA,        VG_AT_MM_CIPCA,       AT_CMD_ACCESS_NONE},
    {ATCI_CONST_CHAR_STR "*MEDRXCFG",   vgMmMEDRXCFG,     VG_AT_MM_MEDRXCFG,   AT_CMD_ACCESS_NONE},
    {ATCI_CONST_CHAR_STR "*MNBIOTEVENT",vgMmMNBIOTEVENT,  VG_AT_MM_MNBIOTEVENT,AT_CMD_ACCESS_NONE},
    {ATCI_CONST_CHAR_STR "*MOOSAIND",   vgMmMOOSAIND,     VG_AT_MM_MOOSAIND,   AT_CMD_ACCESS_NONE},
#if defined (FEA_NFM)
    {ATCI_CONST_CHAR_STR "+NFM",            vgMmNFM,          VG_AT_MM_NFM,         AT_CMD_ACCESS_NONE},
    {ATCI_CONST_CHAR_STR "+NFMC",           vgMmNFMC,         VG_AT_MM_NFMC,        AT_CMD_ACCESS_NONE},
    {ATCI_CONST_CHAR_STR "*MNFM",           vgMmMNFM,         VG_AT_MM_MNFM,        AT_CMD_ACCESS_NONE},
    {ATCI_CONST_CHAR_STR "*MTC",            vgMmMTC,        VG_AT_MM_NFMTC,       AT_CMD_ACCESS_NONE},
#endif
#if defined (FEA_RPM)
    {ATCI_CONST_CHAR_STR "*MRPM",           vgMmMRPM,         VG_AT_MM_MRPM,        AT_CMD_ACCESS_NONE},
    {ATCI_CONST_CHAR_STR "*MRPMR",          vgMmMRPMR,        VG_AT_MM_MRPMR,       AT_CMD_ACCESS_NONE},
#endif

    {PNULL,     PNULL,      VG_AT_LAST_CODE, AT_CMD_ACCESS_NONE}
};

/***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

static void initialiseMmssGeneric (void);


/***************************************************************************
 * Type Definitions
 ***************************************************************************/

/***************************************************************************
 * Local Functions
 ***************************************************************************/

/****************************************************************************
*
* Function:    initialiseMmss
*
* Parameters:  none
*
* Returns:     nothing
*
* Description: initiates mobility sub-system generic data to default
*              state, initiates timer utilities and registers interest in
*              background layer procedures to enable change control requests.
*
****************************************************************************/

static void initialiseMmssGeneric (void)
{
    /* Job137602: Add a loop flag to set the BAND */

    MobilityContext_t * mobilityContext_p = ptrToMobilityContext ();
    VgSYSCONFIGData   * vgSYSCONFIGData   = &mobilityContext_p->vgSYSCONFIGData;
#if defined (FEA_NFM)
    Int8  index;
#endif
    /* Radio information */

    mobilityContext_p->receiveQuality  = RXQUAL_0;
    mobilityContext_p->receiveLevel    = 0;
    mobilityContext_p->haveReceiveInfo = FALSE;

    mobilityContext_p->eutraRsrp       = 0;
    mobilityContext_p->eutraRsrq       = 0;

    mobilityContext_p->vgdlber          = 0;
    mobilityContext_p->vgulber          = 0;

    /* network information */

    mobilityContext_p->vgNetworkPresent = FALSE;

    mobilityContext_p->vgNetworkState.taskId                 = (TaskId)0;
    mobilityContext_p->vgNetworkState.serviceStatus          = PLMN_NO_SERVICE;
    mobilityContext_p->vgNetworkState.isSelecting            = FALSE;
    mobilityContext_p->vgNetworkState.currentlyRoaming       = FALSE;
    mobilityContext_p->vgNetworkState.validNetworkName       = FALSE;
    mobilityContext_p->vgNetworkState.plmn.mcc               = 0;
    mobilityContext_p->vgNetworkState.plmn.mnc               = 0;
    mobilityContext_p->vgNetworkState.name.plmnCoding        = PLMN_CODING_DEFAULT;
    mobilityContext_p->vgNetworkState.inHomeCountry          = TRUE;
    mobilityContext_p->vgNetworkState.manuallySelected       = FALSE;
    mobilityContext_p->vgNetworkState.abmmDetachTrig         = GPRS_NO_DETACH_TRIG;
    mobilityContext_p->vgNetworkState.serviceType            = GPRS_SERVICE;
    mobilityContext_p->vgNetworkState.lai.mcc                = 0;
    mobilityContext_p->vgNetworkState.lai.mnc                = 0;
    mobilityContext_p->vgNetworkState.lai.lac                = 0;
    mobilityContext_p->vgNetworkState.cellId                 = 0;
    mobilityContext_p->vgNetworkState.isInManualMode         = FALSE;

    mobilityContext_p->vgNetworkState.additionalUpdateResult = AUR_NO_ADDITIONAL_INFO;

    /* AT command information */
    mobilityContext_p->vgMLTSString[0] = NULL_CHAR;

    mobilityContext_p->vgCOPSData.mode   = VG_OP_AUTOMATIC_MODE;
    mobilityContext_p->vgCOPSData.format = VG_OP_NUM;
    mobilityContext_p->vgCOPSData.state  = VG_COPS_LIST;


#if defined(UPGRADE_3G)
    mobilityContext_p->vgCOPSData.returnToRplmn = FALSE;
#endif
    mobilityContext_p->vgCOPSData.selectedPlmn.present              = FALSE;
    mobilityContext_p->vgCOPSData.selectedPlmn.plmn.mcc             = 0;
    mobilityContext_p->vgCOPSData.selectedPlmn.plmn.mnc             = 0;
    mobilityContext_p->vgCOPSData.selectedPlmn.plmn.accessTechnology= 0;
    mobilityContext_p->vgCOPSData.selectedPlmn.plmnName.plmnCoding  = PLMN_CODING_DEFAULT;
    mobilityContext_p->vgCOPSData.selectedPlmn.plmnName.full[0]     = NULL_CHAR;
    mobilityContext_p->vgCOPSData.selectedPlmn.plmnName.abbr[0]     = NULL_CHAR;
    mobilityContext_p->vgCOPSData.selectedPlmn.plmnName.initials[0] = NULL_CHAR;
    mobilityContext_p->vgCOPSData.selectedPlmn.plmnName.format[0]   = NULL_CHAR;
    mobilityContext_p->vgCOPSData.fplmnOnlyAvailable                = FALSE;
    mobilityContext_p->vgCOPSData.autoSelectInProgress              = FALSE;

    mobilityContext_p->vgCOPNData.currentPlmn.mnc = 0;
    mobilityContext_p->vgCOPNData.currentPlmn.mcc = 0;
    mobilityContext_p->vgCOPNData.initialPlmn.mnc = 0;
    mobilityContext_p->vgCOPNData.initialPlmn.mcc = 0;
    mobilityContext_p->vgCOPNData.listed          = 0;
    mobilityContext_p->vgCOPNData.order           = EM_PLMN_GET_FIRST;

    mobilityContext_p->vgCSCONData.rptCfgOp   = VG_CSCON_DISABLED;
    mobilityContext_p->vgCSCONData.SigConMode = VG_CSCON_IDLE_MODE;
    mobilityContext_p->vgMfrcllckData.isPciValid          = FALSE;
    mobilityContext_p->vgMfrcllckData.lockStatus          = VG_MFRCLLCK_LOCK_REMOVE_LOCK;
    mobilityContext_p->vgMfrcllckData.byPassSCriteria = FALSE;
    vgMmCPOLInitialiseData (TRUE);
    mobilityContext_p->vgMandSLData.enable = FALSE;
    mobilityContext_p->vgMandSLData.numSearchBand = 0;
    mobilityContext_p->vgMengInfoData.curSrvCell.cellInfoValid              = FALSE;
    mobilityContext_p->vgMengInfoData.neighbourCell.neighbourCellinfoNumber = 0;
    mobilityContext_p->vgMengInfoData.dataTransfer.dataTransferInfoValid    = FALSE;

    
    mobilityContext_p->vgCREGData.state  = VGMNL_NOT_REGISTRATED;
    mobilityContext_p->vgCREGData.lac    = 0;
    mobilityContext_p->vgCREGData.cellId = 0;
    mobilityContext_p->vgMhplmnData.enable = FALSE;

    mobilityContext_p->vgCPLSData.plmnSelector = ABMM_USER_SELECTOR;
    mobilityContext_p->vgMehplmnData.numEhplmn = 0;
    mobilityContext_p->vgMoosaIndData.oosaStatus = 0;

    /*SYSCONFIG*/
    vgSYSCONFIGData->read = TRUE;

    vgSYSCONFIGData->currentParam.mode     = SYSCONFIG_MODE_AUTO;
    vgSYSCONFIGData->currentParam.roamSupport = ROAMING_SUPPORT;
    vgSYSCONFIGData->currentParam.srvDomain   = DOMAIN_ANY;

    /* time zone info */
    mobilityContext_p->timeZoneInitialised    = FALSE;
    mobilityContext_p->currentTimeZone.format = RTC_DISP_FORMAT_INVALID;

    mobilityContext_p->gaOption = ABMM_GAPWON_DEFAULT;

#if defined(UPGRADE_3G)
# if defined(USE_ABAPP)
    mobilityContext_p->isImmediate = FALSE;
# endif
#endif

#if defined (UPGRADE_SHARE_MEMORY)
    mobilityContext_p->cregUpdated                = TRUE;
    mobilityContext_p->cregData.accessTechnology  = 0;
    mobilityContext_p->cregData.cellId            = 0;
    mobilityContext_p->cregData.lac               = 0;
    mobilityContext_p->cregData.state             = 0;
    mobilityContext_p->cregData.serviceStatus     = 0;

    mobilityContext_p->cgregUpdated               = TRUE;
    mobilityContext_p->cgregData.accessTechnology = 0;
    mobilityContext_p->cgregData.cellId           = 0;
    mobilityContext_p->cgregData.lac              = 0;
    mobilityContext_p->cgregData.state            = 0;
#endif /* UPGRADE_SHARE_MEMORY */

    /* Initialise the NB-IOT generic data.    */
    mobilityContext_p->vgEdrxData.userDataValid      = FALSE;
    mobilityContext_p->vgEdrxData.mode               = VG_EDRX_MODE_DISABLE_EDRX;
    mobilityContext_p->vgEdrxData.nwDataValid        = FALSE;
    mobilityContext_p->vgEdrxData.nwEdrxSupport      = FALSE;
    mobilityContext_p->vgEdrxData.nwEdrxValue        = 0;
    mobilityContext_p->vgEdrxData.nwPagingTimeWindow = 0;
    mobilityContext_p->vgEdrxData.userEdrxSupport    = FALSE;
    mobilityContext_p->vgEdrxData.userEdrxValue      = 0;
    mobilityContext_p->vgEdrxData.userPtwPresence   = FALSE;
    mobilityContext_p->vgEdrxData.userPagingTimeWindow = 0;

    mobilityContext_p->vgReqEdrxData.mode            = VG_EDRX_MODE_DISABLE_EDRX;
    mobilityContext_p->vgReqEdrxData.userEdrxSupport = FALSE;
    mobilityContext_p->vgReqEdrxData.userEdrxValue   = 0;
    mobilityContext_p->vgReqEdrxData.userPtwPresence = FALSE;
    mobilityContext_p->vgReqEdrxData.userPtwValue   = 0;

    mobilityContext_p->vgCciotoptData.uEdataValid = FALSE;
    mobilityContext_p->vgCciotoptData.nwDataValid = FALSE;
    mobilityContext_p->vgCciotoptData.reportOpt = VG_CCIOTOPT_N_DISABLE_UNSOL;

    mobilityContext_p->vgCpsmsData.dataValid    = FALSE;
    mobilityContext_p->vgReqCpsmsData.mode      = VG_CPSMS_DISABLE;
    mobilityContext_p->vgReqCpsmsData.reqActiveTimePresent = FALSE;
    mobilityContext_p->vgReqCpsmsData.reqTauPresent = FALSE;
    mobilityContext_p->vgCipcaData.dataValid    = FALSE;
#if defined (FEA_RPM)
    mobilityContext_p->vgMrpmData.enable = 1;
#endif

#if defined (FEA_NFM)
    mobilityContext_p->vgNfmData.dataValid          = FALSE;
    mobilityContext_p->vgNfmData.nfmActive          = FALSE;
    mobilityContext_p->vgNfmData.startTimerActive   = FALSE;
    mobilityContext_p->vgNfmData.remainingNfmStartTimerValue   = 0xFFFF;
    mobilityContext_p->vgNfmData.remainingNfmBackOffTimerValue = 0xFFFF;
    mobilityContext_p->vgReqNfmData.reqNfmActive         = FALSE;
    mobilityContext_p->vgReqNfmData.reqStartTimerActivePresent = FALSE;
    mobilityContext_p->vgReqNfmData.reqStartTimerActive  = FALSE;
    mobilityContext_p->vgReqNfmData.reqStParPresent = FALSE;
    mobilityContext_p->vgReqNfmData.reqStPar        = 0;
    mobilityContext_p->vgReqNfmData.reqStTmPresent  = FALSE;
    mobilityContext_p->vgReqNfmData.reqStTm         = 0;  
    mobilityContext_p->vgNfmcData.dataValid         = FALSE;
    for (index = 0; index < MAX_NFM_PAR_VALUE; index++)
    {
      mobilityContext_p->vgNfmcData.nfmPar[index] = 1;
    }
    mobilityContext_p->vgNfmcData.stPar = 1;
    for (index = 0; index < MAX_NFM_PAR_VALUE; index++)
    {
      mobilityContext_p->vgReqNfmcData.reqNfmPar[index] = 1;
      mobilityContext_p->vgReqNfmcData.reqNfmParPresent[index] = FALSE;
    }
    mobilityContext_p->vgReqNfmcData.reqStPar = 1;
    mobilityContext_p->vgReqNfmcData.reqStParPresent = FALSE;
#endif

} /* initialiseMmssGeneric */

/****************************************************************************
*
* Function:    initialiseMmss
*
* Parameters:  none
*
* Returns:     nothing
*
* Description: initiates mobility sub-system entity specific data to default
*              state, initiates timer utilities and registers interest in
*              background layer procedures to enable change control requests.
*
****************************************************************************/
void initialiseMmss (const VgmuxChannelNumber entity)
{
    PARAMETER_NOT_USED (entity);
} /* initialiseMmss */

/***************************************************************************
 * Global Functions
 ***************************************************************************/

/****************************************************************************
*
* Function:    vgMmssInterfaceController
*
* Parameters:  SignalBuffer       - structure containing incoming signal
*              VgmuxChannelNumber - mux channel number
*
* Returns:     Boolean - indicates whether the sub-system has recognised and
*                        procssed the signal given.
*
* Description: determines action for received signals
*
****************************************************************************/

Boolean vgMmssInterfaceController (const SignalBuffer       * signal_p,
                                   const VgmuxChannelNumber entity)
{
    Boolean accepted = FALSE;
    Int8    aloop;

    /* Signal type to handling procedure table */

    const SignalCnfFunc signalCnfFunc[] =
    {
        { SIG_APEX_MM_READ_PLMN_SEL_CNF,  vgSigApexMmReadPlmnSelCnf   },
        { SIG_APEX_MM_WRITE_PLMN_SEL_CNF, vgSigApexMmWritePlmnSelCnf  },
        { SIG_APEX_MM_PLMNLIST_CNF,       vgSigApexMmPlmnListCnf      },
        { SIG_APEX_MM_ABORT_PLMNLIST_CNF, vgSigApexMmAbortPlmnListCnf },
        { SIG_APEX_MM_PLMN_SELECT_CNF,    vgSigApexMmPlmnSelectCnf    },
        { SIG_APEX_MM_DEREGISTER_CNF,     vgSigApexMmDeregisterCnf    },
        { SIG_APEX_EM_PLMN_TEST_CNF,      vgSigApexAbemPlmnTestCnf    },
        { SIG_DM_RTC_READ_TIME_ZONE_CNF,  vgSigAclkReadTimeZoneCnf    },
        { SIG_DM_RTC_SET_TIME_ZONE_CNF,   vgSigAclkSetTimeZoneCnf     },
        { SIG_APEX_MM_READ_BAND_MODE_CNF, vgApexMmReadBandModeCnf     },
        { SIG_APEX_MM_WRITE_PWON_OPTIONS_CNF, vgApexMmWritePwonOptionsCnf  },
        { SIG_APEX_MM_READ_PWON_OPTIONS_CNF, vgApexMmReadPwonOptionsCnf   },
        { SIG_APEX_MM_LOCK_ARFCN_CNF,        vgApexMmLockArfcnCnf     },
        { SIG_APEX_MM_SEARCH_BAND_LIST_CNF,  vgApexMmSearchBandListCnf},
        { SIG_APEX_MM_UE_STATS_CNF,        vgApexMmUeStatsCnf     },
        { SIG_APEX_MM_LOC_CELL_INFO_CNF,   vgApexMmLocCellInfoCnf      },
#if defined(UPGRADE_MTNET)
        {  SIG_APEX_MM_SUSPEND_CNF,        vgApexMmSuspendCnf         },
        {  SIG_APEX_MM_RESUME_CNF,         vgApexMmResumeCnf          },
#endif
        {  SIG_APEX_MM_SET_EDRX_CNF,       vgApexMmSetEdrxCnf         },
        {  SIG_APEX_MM_READ_EDRX_CNF,      vgApexMmReadEdrxCnf        },

        {  SIG_APEX_WRITE_IOT_OPT_CFG_CNF,           vgApexWriteIotOptCfgCnf     },
        {  SIG_APEX_READ_IOT_OPT_CFG_CNF,            vgApexReadIotOptCfgCnf      },
        {  SIG_APEX_WRITE_PSM_CONF_CNF,              vgApexWritePsmConfCnf       },
        {  SIG_APEX_READ_PSM_CONF_CNF,               vgApexReadPsmConfCnf        },
        {  SIG_APEX_MM_WRITE_ATTACH_PDN_CONF_CNF,    vgApexWriteAttachPdnConfCnf },
        {  SIG_APEX_MM_READ_ATTACH_PDN_CONF_CNF,     vgApexReadAttachPdnConfCnf  },
#if defined (FEA_NFM)
        { SIG_APEX_READ_NFM_CNF,           vgApexReadNfmCnf          },
        { SIG_APEX_WRITE_NFM_CNF,          vgApexWriteNfmCnf         },
        { SIG_APEX_READ_NFM_CONF_CNF,      vgApexReadNfmConfCnf      },
        { SIG_APEX_WRITE_NFM_CONF_CNF,     vgApexWriteNfmConfCnf     },
        { SIG_APEX_READ_NFM_STATUS_CNF,    vgApexReadNfmStatusCnf    },
#endif
#if defined (FEA_RPM)
        { SIG_APEX_RPM_READ_INFO_CNF,    vgApexReadRpmCnf},
#endif
        { SIG_APEX_MM_DISABLE_HPLMN_SEARCH_CNF,     vgApexMmDisableHplmnSearchCnf}, 

    };

#define NUM_CN_SIG_CNF_FUNC     (sizeof(signalCnfFunc) / sizeof(SignalCnfFunc))

    /* Signal Handler */

    switch (*signal_p->type)
    {
        case SIG_CI_RUN_AT_COMMAND_IND:
        {
            accepted = parseCommandBuffer (mmAtCommandTable, entity);
            break;
        }

        case SIG_INITIALISE:
        {
            initialiseMmssGeneric ();
            break;
        }

        case SIG_CIMUX_CHANNEL_ENABLE_IND:
            {
                initialiseMmss (entity);
                break;
            }

        case SIG_APEX_MM_NETWORK_STATE_IND:
        {
            vgSigApexMmNetworkStateInd (signal_p);

            /* not accepted since another sub-system also processes the signal */
            break;
        }

        case SIG_APEX_MM_NETWORK_INFO_IND:
        {
            vgSigApexMmNetworkInfoInd (signal_p, entity);
            accepted = TRUE;
            break;
        }

        case SIG_APEX_MM_RSSI_IND:
        {
            vgSigApexMmRssiInd (signal_p);
            accepted = TRUE;
            break;
        }

        case SIG_APEX_MM_BAND_IND:
        {
            vgSigApexMmBandInd (signal_p);
            accepted = TRUE;
            break;
        }

#if defined(ENABLE_AT_ENG_MODE)
        case SIG_APEX_MM_CIPHER_IND:
        {
            vgSigApexMmCipherInd (signal_p);
            accepted = TRUE;
            break;
        }
#endif
        case SIG_APEX_MM_CSCON_IND:
        {
            vgSigApexMmCsconInd (signal_p);
            accepted = TRUE;
            break;
        }

        case SIG_APEX_MM_PSM_STATUS_IND:
        {
            vgSigApexMmPsmStatusInd(signal_p);
            accepted = TRUE;
            break;
        }
    #if defined (FEA_RPM)
        case SIG_APEX_RPM_IND:
        {
            vgSigApexMmRpmInfoInd (signal_p);
            accepted = TRUE;
            break;
        }
#endif

        case SIG_APEX_MM_OOSA_STATUS_IND:
        {
            vgSigApexMmOosaStatusInd (signal_p);
            accepted = TRUE;
            break;
        }

        default:
        {
            for (aloop = 0; (aloop < NUM_CN_SIG_CNF_FUNC) && (accepted == FALSE); aloop++)
            {
                if (signalCnfFunc[aloop].signalId == (*signal_p->type))
                {
                    (signalCnfFunc[aloop].procFunc)(signal_p, entity);
                    accepted = TRUE;
                }
            }
            break;
        }
    } /* switch */
    return (accepted);
} /* vgMmssInterfaceController */

/***************************************************************************
 * Processes
 ***************************************************************************/

/* END OF FILE */

