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
 *  File Description :
 *      Protocol Stack Signal Definitions
 **************************************************************************/

#if !defined (EXCLUDE_MNCC)
    /*
    ** CC to AL signals
    */
    SIG_DEF( SIG_MNCC_DUMMY = MNCC_SIGNAL_BASE, EmptySignal              mncc_dummy)
    SIG_DEF( SIG_MNCC_SETUP_REQ,                MnccSetupReq             mnccSetupReq)
    SIG_DEF( SIG_MNCC_SETUP_RSP,                MnccSetupRsp             mnccSetupRsp)
    SIG_DEF( SIG_MNCC_REJ_REQ,                  MnccRejReq               mnccRejReq)
    SIG_DEF( SIG_MNCC_CALL_CONF_REQ,            MnccCallConfReq          mnccCallConfReq)
    SIG_DEF( SIG_MNCC_ALERT_REQ,                MnccAlertReq             mnccAlertReq)
    SIG_DEF( SIG_MNCC_NOTIFY_REQ,               MnccNotifyReq            mnccNotifyReq)
    SIG_DEF( SIG_MNCC_DISC_REQ,                 MnccDiscReq              mnccDiscReq)
    SIG_DEF( SIG_MNCC_REL_REQ,                  MnccRelReq               mnccRelReq)
    SIG_DEF( SIG_MNCC_FACILITY_REQ,             MnccFacilityReq          mnccFacilityReq)
    SIG_DEF( SIG_MNCC_START_DTMF_REQ,           MnccStartDtmfReq         mnccStartDtmfReq)
    SIG_DEF( SIG_MNCC_STOP_DTMF_REQ,            MnccStopDtmfReq          mnccStopDtmfReq)
    SIG_DEF( SIG_MNCC_MODIFY_REQ,               MnccModifyReq            mnccModifyReq)
    SIG_DEF( SIG_MNCC_MODIFY_RSP,               MnccModifyRsp            mnccModifyRsp)
    SIG_DEF( SIG_MNCC_HOLD_REQ,                 MnccHoldReq              mnccHoldReq)
    SIG_DEF( SIG_MNCC_RETRIEVE_REQ,             MnccRetrieveReq          mnccRetrieveReq)
    SIG_DEF( SIG_MNCC_SETUP_IND,                MnccSetupInd             mnccSetupInd)
    SIG_DEF( SIG_MNCC_SETUP_CNF,                MnccSetupCnf             mnccSetupCnf)
    SIG_DEF( SIG_MNCC_SETUP_COMPLETE_IND,       MnccSetupCompleteInd     mnccSetupCompleteInd)
    SIG_DEF( SIG_MNCC_CALL_PROCEEDING_IND,      MnccCallProceedingInd    mnccCallProceedingInd)
    SIG_DEF( SIG_MNCC_CALL_PROGRESS_IND,        MnccCallProgressInd      mnccCallProgressInd)
    SIG_DEF( SIG_MNCC_ALERT_IND,                MnccAlertInd             mnccAlertInd)
    SIG_DEF( SIG_MNCC_MODIFY_IND,               MnccModifyInd            mnccModifyInd)
    SIG_DEF( SIG_MNCC_NOTIFY_IND,               MnccNotifyInd            mnccNotifyInd)
    SIG_DEF( SIG_MNCC_DISC_IND,                 MnccDiscInd              mnccDiscInd)
    SIG_DEF( SIG_MNCC_REL_IND,                  MnccRelInd               mnccRelInd)
    SIG_DEF( SIG_MNCC_REL_CNF,                  MnccRelCnf               mnccRelCnf)
    SIG_DEF( SIG_MNCC_FACILITY_IND,             MnccFacilityInd          mnccFacilityInd)
    SIG_DEF( SIG_MNCC_START_DTMF_CNF,           MnccStartDtmfCnf         mnccStartDtmfCnf)
    SIG_DEF( SIG_MNCC_STOP_DTMF_CNF,            MnccStopDtmfCnf          mnccStopDtmfCnf)
    SIG_DEF( SIG_MNCC_MODIFY_CNF,               MnccModifyCnf            mnccModifyCnf)
    SIG_DEF( SIG_MNCC_SYNC_IND,                 MnccSyncInd              mnccSyncInd)
    SIG_DEF( SIG_MNCC_HOLD_CNF,                 MnccHoldCnf              mnccHoldCnf)
    SIG_DEF( SIG_MNCC_RETRIEVE_CNF,             MnccRetrieveCnf          mnccRetrieveCnf)
    SIG_DEF( SIG_MNCC_REJ_IND,                  MnccRejInd               mnccRejInd)
    SIG_DEF( SIG_MNCC_STATUS_IND,               MnccStatusInd            mnccStatusInd)
    SIG_DEF( SIG_MNCC_STATUS_RSP,               MnccStatusRsp            mnccStatusRsp)
#endif

#if !defined (EXCLUDE_MNSS)
    /*
    ** SS to AL signals
    */
    SIG_DEF( SIG_MNSS_DUMMY = MNSS_SIGNAL_BASE, EmptySignal              mnss_dummy)
    SIG_DEF( SIG_MNSS_BEGIN_REQ,                MnssBeginReq             mnssBeginReq)
    SIG_DEF( SIG_MNSS_FACILITY_REQ,             MnssFacilityReq          mnssFacilityReq)
    SIG_DEF( SIG_MNSS_END_REQ,                  MnssEndReq               mnssEndReq)
    SIG_DEF( SIG_MNSS_BEGIN_IND,                MnssBeginInd             mnssBeginInd)
    SIG_DEF( SIG_MNSS_FACILITY_IND,             MnssFacilityInd          mnssFacilityInd)
    SIG_DEF( SIG_MNSS_END_IND,                  MnssEndInd               mnssEndInd)
    SIG_DEF( SIG_MNSS_REJ_IND,                  MnssRejInd               mnssRejInd)
#endif

#if !defined (EXCLUDE_MNSM)
    /*
    ** sm-rl to sm-cm signals
    */
    SIG_DEF( SIG_MNSM_DUMMY = MNSM_SIGNAL_BASE, EmptySignal              mnsm_dummy)
    SIG_DEF( SIG_MNSM_ABORT_REQ,                MnsmAbortReq             mnsmAbortReq)
    SIG_DEF( SIG_MNSM_DATA_REQ,                 MnsmDataReq              mnsmDataReq)
    SIG_DEF( SIG_MNSM_DATA_IND,                 MnsmDataInd              mnsmDataInd)
    SIG_DEF( SIG_MNSM_EST_REQ,                  MnsmEstReq               mnsmEstReq)
    SIG_DEF( SIG_MNSM_EST_IND,                  MnsmEstInd               mnsmEstInd)
    SIG_DEF( SIG_MNSM_ERR_IND,                  MnsmErrInd               mnsmErrInd)
    SIG_DEF( SIG_MNSM_REL_REQ,                  MnsmRelReq               mnsmRelReq)
    SIG_DEF( SIG_MNSM_CONFIG_REQ,               MnsmConfigReq            mnsmConfigReq)
#endif

#if !defined (EXCLUDE_MMXX)
    /*
    ** MM-CM signals
    */
    SIG_DEF( SIG_MMXX_DUMMY = MMXX_SIGNAL_BASE, EmptySignal              mmxx_dummy)
    SIG_DEF( SIG_MMXX_DATA_REQ,                 MmxxDataReq              mmxxDataReq)
    SIG_DEF( SIG_MMXX_EST_REQ,                  MmxxEstReq               mmxxEstReq)
    SIG_DEF( SIG_MMXX_REEST_REQ,                MmxxReestReq             mmxxReestReq)
    SIG_DEF( SIG_MMXX_REL_REQ,                  MmxxRelReq               mmxxRelReq)
    SIG_DEF( SIG_MMXX_REDUNDANT_1,              EmptySignal              mmxxRedundant1 )
    SIG_DEF( SIG_MMXX_EST_CNF,                  MmxxEstCnf               mmxxEstCnf)
    SIG_DEF( SIG_MMXX_REEST_CNF,                MmxxReestCnf             mmxxReestCnf)
    SIG_DEF( SIG_MMXX_DATA_IND,                 MmxxDataInd              mmxxDataInd)
    SIG_DEF( SIG_MMXX_ERR_IND,                  MmxxErrInd               mmxxErrInd)
    SIG_DEF( SIG_MMXX_EST_IND,                  MmxxEstInd               mmxxEstInd)
    SIG_DEF( SIG_MMXX_REL_IND,                  MmxxRelInd               mmxxRelInd)
    SIG_DEF( SIG_MMXX_GSMS_ROUTE_REQ,           MmxxGsmsRouteReq         mmxxGsmsRouteReq)
    SIG_DEF( SIG_MMXX_GSMS_ROUTE_CNF,           MmxxGsmsRouteCnf         mmxxGsmsRouteCnf)

    SIG_DEF( SIG_MMCC_SYNC_IND,                 MmccSyncInd              mmccSyncInd)
    SIG_DEF( SIG_MMSM_SYNC_IND,                 MmsmSyncInd              mmsmSyncInd)
#endif
//zhaoyong bbb
#if !defined (EXCLUDE_MMR)
    /*
    ** MM-AL (registration) signals
    */
    SIG_DEF( SIG_MMR_DUMMY = MMR_SIGNAL_BASE,   EmptySignal              mmr_dummy)
    SIG_DEF( SIG_MMR_NREG_REQ,                  MmrNregReq               mmrNregReq)
    SIG_DEF( SIG_MMR_PLMN_LIST_REQ,             MmrPlmnListReq           mmrPlmnListReq)
    SIG_DEF( SIG_MMR_REG_REQ,                   MmrRegReq                mmrRegReq)
    SIG_DEF( SIG_MMR_PLMN_LIST_CNF,             MmrPlmnListCnf           mmrPlmnListCnf)
    SIG_DEF( SIG_MMR_REG_CNF,                   MmrRegCnf                mmrRegCnf)
    SIG_DEF( SIG_MMR_NREG_IND,                  MmrNregInd               mmrNregInd)
    SIG_DEF( SIG_MMR_ERROR_IND,                 MmrErrorInd              mmrErrorInd)
    SIG_DEF( SIG_MMR_REG_IND,                   MmrRegInd                mmrRegInd)
    SIG_DEF( SIG_MMR_ME_DATA_REQ,               MmrMeDataReq             mmrMeDataReq)
    SIG_DEF( SIG_MMR_NREG_CNF,                  MmrNregCnf               mmrNregCnf)
    SIG_DEF( SIG_MMR_INFO_IND,                  MmrInfoInd               mmrInfoInd) /* 9802-1981 */
    SIG_DEF( SIG_MMR_PLMN_LIST_IND,             MmrPlmnListInd           mmrPlmnListInd)
    SIG_DEF( SIG_MMR_ENG_INFO_REQ,              MmrEngInfoReq            mmrEngInfoReq)    /* FR9805-2463 */
    SIG_DEF( SIG_MMR_NETWORK_INFO_IND,          MmrNetworkInfoInd        mmrNetworkInfoInd)
    SIG_DEF( SIG_MMR_EQUIV_PLMN_LIST_REQ,       MmrEquivPlmnListReq      mmrEquivPlmnListReq)
    SIG_DEF( SIG_MMR_ABMM_TIMER_EXPIRY,         MmrAbmmTimerExpiry       mmrAbmmTimerExpiry)
#if defined(ENABLE_EHPLMN)
    SIG_DEF( SIG_MMR_EHPLMN_LIST_REQ,           MmrEhplmnListReq         mmrEhplmnListReq)
#endif /* (ENABLE_EHPLMN) */
#if defined (UPGRADE_GGE)
    SIG_DEF( SIG_MMR_LLC_INFO_IND,              MmrLlcInfoInd            mmrLlcInfoInd) /* EMM TODO: remove this, dependencies to ABEM */
#endif
    SIG_DEF( SIG_MMR_ABORT_PLMN_LIST_REQ,       EmptySignal              mmrAbortPlmnListReq)
    SIG_DEF( SIG_MMR_SET_EDRX_REQ,              MmrSetEdrxReq            mmrSetEdrxReq)
    SIG_DEF( SIG_MMR_SET_EDRX_CNF,              MmrSetEdrxCnf            mmrSetEdrxCnf)
    SIG_DEF( SIG_MMR_READ_EDRX_REQ,             MmrReadEdrxReq           mmrReadEdrxReq)
    SIG_DEF( SIG_MMR_READ_EDRX_CNF,             MmrReadEdrxCnf           mmrReadEdrxCnf)
    SIG_DEF( SIG_MMR_WRITE_IOT_OPT_CFG_REQ,     MmrWriteIotOptCfgReq     mmrWriteIotOptCfgReq)
    SIG_DEF( SIG_MMR_WRITE_IOT_OPT_CFG_CNF,     MmrWriteIotOptCfgCnf     mmrWriteIotOptCfgCnf)
    SIG_DEF( SIG_MMR_READ_IOT_OPT_CFG_REQ,      MmrReadIotOptCfgReq      mmrReadIotOptCfgReq)
    SIG_DEF( SIG_MMR_READ_IOT_OPT_CFG_CNF,      MmrReadIotOptCfgCnf      mmrReadIotOptCfgCnf)
    SIG_DEF( SIG_MMR_WRITE_PSM_CONF_REQ,        MmrWritePsmConfReq       mmrWritePsmConfReq)
    SIG_DEF( SIG_MMR_WRITE_PSM_CONF_CNF,        MmrWritePsmConfCnf       mmrWritePsmConfCnf)
    SIG_DEF( SIG_MMR_READ_PSM_CONF_REQ,         MmrReadPsmConfReq        mmrReadPsmConfReq)
    SIG_DEF( SIG_MMR_READ_PSM_CONF_CNF,         MmrReadPsmConfCnf        mmrReadPsmConfCnf)
    SIG_DEF( SIG_MMR_PSM_STATUS_IND,            MmrPsmStatusInd          mmrPsmStatusInd)
    SIG_DEF( SIG_MMR_SEARCH_IND,                MmrSearchInd             mmrSearchInd)
    SIG_DEF( SIG_MMR_CSCON_IND,                 MmrCsconInd              mmrCsconInd)
    SIG_DEF( SIG_MMR_NAS_AS_INFO_REQ,           MmrAsNasInfoReq          mmrAsNasInfoReq)
    SIG_DEF( SIG_MMR_NAS_AS_INFO_CNF,           MmrAsNasInfoCnf          mmrAsNasInfoCnf)
    SIG_DEF( SIG_MMR_EMM_TIMER_EXPIRY_IND,      MmrEmmTimerExpiryInd     mmrEmmTimerExpiryInd)
    SIG_DEF( SIG_MMR_READ_UE_INFO_REQ,          MmrReadUeInfoReq         mmrReadUeInfoReq)
    SIG_DEF( SIG_MMR_READ_UE_INFO_CNF,          MmrReadUeInfoCnf         mmrReadUeInfoCnf) 
    SIG_DEF( SIG_MMR_LOC_CELL_INFO_REQ,         MmrLocCellInfoReq        mmrLocCellInfoReq)
    SIG_DEF( SIG_MMR_LOC_CELL_INFO_CNF,         MmrLocCellInfoCnf        mmrLocCellInfoCnf)
#ifdef FEA_NFM
    SIG_DEF( SIG_MMR_WRITE_NFM_REQ,             MmrWriteNfmReq           mmrWriteNfmReq)
    SIG_DEF( SIG_MMR_WRITE_NFM_CNF,             MmrWriteNfmCnf           mmrWriteNfmCnf)
    SIG_DEF( SIG_MMR_READ_NFM_REQ,              MmrReadNfmReq            mmrReadNfmReq)
    SIG_DEF( SIG_MMR_READ_NFM_CNF,              MmrReadNfmCnf            mmrReadNfmCnf)
    SIG_DEF( SIG_MMR_WRITE_NFM_CONF_REQ,        MmrWriteNfmConfReq       mmrWriteNfmConfReq)
    SIG_DEF( SIG_MMR_WRITE_NFM_CONF_CNF,        MmrWriteNfmConfCnf       mmrWriteNfmConfCnf)
    SIG_DEF( SIG_MMR_READ_NFM_CONF_REQ,         MmrReadNfmConfReq        mmrReadNfmConfReq)
    SIG_DEF( SIG_MMR_READ_NFM_CONF_CNF,         MmrReadNfmConfCnf        mmrReadNfmConfCnf)
    SIG_DEF( SIG_MMR_READ_NFM_STATUS_REQ,       MmrReadNfmStatusReq      mmrReadNfmStatusReq)
    SIG_DEF( SIG_MMR_READ_NFM_STATUS_CNF,       MmrReadNfmStatusCnf      mmrReadNfmStatusCnf)
#endif /* FEA_NFM */
    SIG_DEF( SIG_MMR_SIM_INFO_NOTIFY_REQ,       MmrSimInfoNotifyReq      mmrSimInfoNotifyReq)
#if defined (ENABLE_L23_DEBUG)
# if defined (UPGRADE_LTE)
    SIG_DEF( SIG_EMM_STATE_DEBUG_IND,           EmmStateDebugInd         emmStateDebugInd)
# endif /* UPGRADE_LTE */
#endif /* ENABLE_L23_DEBUG */

# if defined (UPGRADE_LTE)
#  if defined (DEVELOPMENT_VERSION)
    SIG_DEF( SIG_EMM_DISABLE_INT_CHECK_REQ,     EmptySignal              emmDisableIntCheckReq)
    SIG_DEF( SIG_EMM_ENABLE_INT_CHECK_REQ,      EmptySignal              emmEnableIntCheckReq)
#  endif /* DEVELOPMENT_VERSION */
# endif /* UPGRADE_LTE */
#endif /* EXCLUDE_MMR */

#if !defined (EXCLUDE_SMS)
    /*
    ** SMS signals
    */
    SIG_DEF( SIG_TS_DUMMY = SMS_SIGNAL_BASE,    EmptySignal              ts_dummy)
    SIG_DEF( SIG_TS_SUBMIT_REQ,                 TsSubmitReq              tsSubmitReq)
    SIG_DEF( SIG_TS_COMMAND_REQ,                TsCommandReq             tsCommandReq)
    SIG_DEF( SIG_TS_DELIVER_REPORT_REQ,         TsDeliverReportReq       tsDeliverReportReq)
    SIG_DEF( SIG_TS_MEM_AVAIL_REQ,              TsMemAvailReq            tsMemAvailReq)
    SIG_DEF( SIG_TS_DELIVER_IND,                TsDeliverInd             tsDeliverInd)
    SIG_DEF( SIG_TS_STATUS_REPORT_IND,          TsStatusReportInd        tsStatusReportInd)
    SIG_DEF( SIG_TS_REPORT_IND,                 TsReportInd              tsReportInd)
    SIG_DEF( SIG_TS_CONFIG_REQ,                 TsConfigReq              tsConfigReq)
    SIG_DEF( SIG_SMRL_DATA_REQ,                 SmrlDataReq              smrlDataReq)
    SIG_DEF( SIG_SMRL_DATA_IND,                 SmrlDataInd              smrlDataInd)
    SIG_DEF( SIG_SMRL_MEM_AVAIL_REQ,            SmrlMemAvailReq          smrlMemAvailReq)
    SIG_DEF( SIG_SMRL_REPORT_REQ,               SmrlReportReq            smrlReportReq)
    SIG_DEF( SIG_SMRL_REPORT_IND,               SmrlReportInd            smrlReportInd)
    SIG_DEF( SIG_TS_RAW_DELIVER_IND,            TsRawDeliverInd          tsRawDeliverInd)
    SIG_DEF( SIG_SMCM_UNIT_DATA_IND,            SmcmUnitDataInd          smcmUnitDataInd)
    SIG_DEF( SIG_SMRL_CONFIG_REQ,               SmrlConfigReq            smrlConfigReq)
#endif

#if !defined (EXCLUDE_MMSI)
    /*
    ** MM-SIM signals
    */
    SIG_DEF( SIG_MM_SIM_DUMMY = SIM_SIGNAL_BASE, EmptySignal             sim_dummy)
#if defined (UPGRADE_LTE)
    SIG_DEF( SIG_MM_SIM_READ_DATA_REQ,           MmSimReadDataReq        mmSimReadDataReq)
#else
    SIG_DEF( SIG_MM_SIM_READ_DATA_REQ,           EmptySignal             mmSimReadDataReq)
#endif
    SIG_DEF( SIG_MM_SIM_READ_DATA_CNF,           MmSimReadDataCnf        mmSimReadDataCnf)
    SIG_DEF( SIG_MM_SIM_WRITE_DATA_REQ,          MmSimWriteDataReq       mmSimWriteDataReq)
    SIG_DEF( SIG_MM_SIM_WRITE_DATA_CNF,          MmSimWriteDataCnf       mmSimWriteDataCnf)
    SIG_DEF( SIG_MM_SIM_CONNECTION_IND,          MmSimConnectionInd      mmSimConnectionInd)
    SIG_DEF( SIG_MM_SIM_REMOVED_IND,             MmSimRemovedInd         mmSimRemovedInd)
    SIG_DEF( SIG_MM_SIM_APP_STARTED_IND,         MmSimAppStartedInd      mmSimAppStartedInd)
#if defined (UPGRADE_LTE)
    SIG_DEF( SIG_MM_SIM_EPS_AUTH_REQ,            MmSimEpsAuthenticateReq mmSimEpsAuthenticateReq)
    SIG_DEF( SIG_MM_SIM_EPS_AUTH_CNF,            MmSimEpsAuthenticateCnf mmSimEpsAuthenticateCnf)
#endif
    SIG_DEF( SIG_MM_SIM_LOW_POWER_IND,           MmSimLowPowerInd        mmSimLowPowerInd)

#endif    /* EXCLUDE_MMSI */

#if !defined (EXCLUDE_MNCB)
    /*
    ** CB signals to/from Application layer
    */
    SIG_DEF( SIG_MNCB_DUMMY = MNCB_SIGNAL_BASE, EmptySignal              mncb_dummy)
    SIG_DEF( SIG_MNCB_CONFIGURE_REQ,            MncbConfigureReq         mncbConfigureReq)
    SIG_DEF( SIG_MNCB_CONFIGURE_CBMI_REQ,       MncbConfigureCbmiReq     mncbConfigureCbmiReq)
    SIG_DEF( SIG_MNCB_MESSAGE_IND,              MncbMessageInd           mncbMessageInd)
    SIG_DEF( SIG_MNCB_MACRO_MESSAGE_IND,        MncbMacroMessageInd      mncbMacroMessageInd)
    SIG_DEF( SIG_MNCB_CELL_CHANGE_IND,          MncbCellChangeInd        mncbCellChangeInd)
#endif

#if !defined (EXCLUDE_ALSI)
    /*
    ** SIM signals to/from Application layer
    */
    /* !!!NB this signal base is full in certain configurations - to be sure that no problems arise
    *  if new ALSI signals are added these need to go into the ALSI_EX signal base below
    */
    SIG_DEF( SIG_ALSI_DUMMY = ALSI_SIGNAL_BASE, EmptySignal              alsi_dummy)
    SIG_DEF( SIG_ALSI_APP_STARTED_IND,          AlsiAppStartedInd        alsiAppStartedInd)
    SIG_DEF( SIG_ALSI_CARD_REMOVED_IND,         AlsiCardRemovedInd       alsiCardRemovedInd)
    SIG_DEF( SIG_ALSI_APP_INITIALISE_REQ,       AlsiAppInitialiseReq     alsiAppInitialiseReq)
    SIG_DEF( SIG_ALSI_APP_INITIALISE_CNF,       AlsiAppInitialiseCnf     alsiAppInitialiseCnf)
    SIG_DEF( SIG_ALSI_SIM_GEN_ACCESS_REQ,       AlsiSimGenAccessReq      alsiSimGenAccessReq)
    SIG_DEF( SIG_ALSI_SIM_GEN_ACCESS_CNF,       AlsiSimGenAccessCnf      alsiSimGenAccessCnf)
    SIG_DEF( SIG_ALSI_TERMINATE_SESSION_REQ,    AlsiTerminateSessionReq  alsiTerminateSessionReq)
    SIG_DEF( SIG_ALSI_TERMINATE_SESSION_CNF,    AlsiTerminateSessionCnf  alsiTerminateSessionCnf)
    SIG_DEF( SIG_ALSI_APP_STOPPED_IND,          AlsiAppStoppedInd        alsiAppStoppedInd)
    SIG_DEF( SIG_ALSI_READ_LP_REQ,              AlsiReadLpReq            alsiReadLpReq)
    SIG_DEF( SIG_ALSI_READ_LP_CNF,              AlsiReadLpCnf            alsiReadLpCnf)
    SIG_DEF( SIG_ALSI_WRITE_LP_REQ,             AlsiWriteLpReq           alsiWriteLpReq)
    SIG_DEF( SIG_ALSI_WRITE_LP_CNF,             AlsiWriteLpCnf           alsiWriteLpCnf)
    SIG_DEF( SIG_ALSI_READ_PLMNSEL_REQ,         AlsiReadPlmnSelReq       alsiReadPlmnSelReq)
    SIG_DEF( SIG_ALSI_READ_PLMNSEL_CNF,         AlsiReadPlmnSelCnf       alsiReadPlmnSelCnf)
    SIG_DEF( SIG_ALSI_WRITE_PLMNSEL_REQ,        AlsiWritePlmnSelReq      alsiWritePlmnSelReq)
    SIG_DEF( SIG_ALSI_WRITE_PLMNSEL_CNF,        AlsiWritePlmnSelCnf      alsiWritePlmnSelCnf)
#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
    SIG_DEF( SIG_ALSI_READ_ACMMAX_REQ,          AlsiReadAcmMaxReq        alsiReadAcmMaxReq)
    SIG_DEF( SIG_ALSI_READ_ACMMAX_CNF,          AlsiReadAcmMaxCnf        alsiReadAcmMaxCnf)
    SIG_DEF( SIG_ALSI_WRITE_ACMMAX_REQ,         AlsiWriteAcmMaxReq       alsiWriteAcmMaxReq)
    SIG_DEF( SIG_ALSI_WRITE_ACMMAX_CNF,         AlsiWriteAcmMaxCnf       alsiWriteAcmMaxCnf)
    SIG_DEF( SIG_ALSI_READ_ACM_REQ,             AlsiReadAcmReq           alsiReadAcmReq)
    SIG_DEF( SIG_ALSI_READ_ACM_CNF,             AlsiReadAcmCnf           alsiReadAcmCnf)
    SIG_DEF( SIG_ALSI_WRITE_ACM_REQ,            AlsiWriteAcmReq          alsiWriteAcmReq)
    SIG_DEF( SIG_ALSI_WRITE_ACM_CNF,            AlsiWriteAcmCnf          alsiWriteAcmCnf)
    SIG_DEF( SIG_ALSI_INCREASE_ACM_REQ,         AlsiIncreaseAcmReq       alsiIncreaseAcmReq)
    SIG_DEF( SIG_ALSI_INCREASE_ACM_CNF,         AlsiIncreaseAcmCnf       alsiIncreaseAcmCnf)
    SIG_DEF( SIG_ALSI_READ_PUCT_REQ,            AlsiReadPuctReq          alsiReadPuctReq)
    SIG_DEF( SIG_ALSI_READ_PUCT_CNF,            AlsiReadPuctCnf          alsiReadPuctCnf)
    SIG_DEF( SIG_ALSI_WRITE_PUCT_REQ,           AlsiWritePuctReq         alsiWritePuctReq)
    SIG_DEF( SIG_ALSI_WRITE_PUCT_CNF,           AlsiWritePuctCnf         alsiWritePuctCnf)
    SIG_DEF( SIG_ALSI_READ_CBMI_REQ,            AlsiReadCbmiReq          alsiReadCbmiReq)
    SIG_DEF( SIG_ALSI_READ_CBMI_CNF,            AlsiReadCbmiCnf          alsiReadCbmiCnf)
    SIG_DEF( SIG_ALSI_WRITE_CBMI_REQ,           AlsiWriteCbmiReq         alsiWriteCbmiReq)
    SIG_DEF( SIG_ALSI_WRITE_CBMI_CNF,           AlsiWriteCbmiCnf         alsiWriteCbmiCnf)
    SIG_DEF( SIG_ALSI_READ_BCCH_REQ,            AlsiReadBcchReq          alsiReadBcchReq)
    SIG_DEF( SIG_ALSI_READ_BCCH_CNF,            AlsiReadBcchCnf          alsiReadBcchCnf)
    SIG_DEF( SIG_ALSI_WRITE_BCCH_REQ,           AlsiWriteBcchReq         alsiWriteBcchReq)
    SIG_DEF( SIG_ALSI_WRITE_BCCH_CNF,           AlsiWriteBcchCnf         alsiWriteBcchCnf)
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    SIG_DEF( SIG_ALSI_ADD_FPLMN_REQ,            AlsiAddFplmnReq          alsiAddFplmnReq)
    SIG_DEF( SIG_ALSI_ADD_FPLMN_CNF,            AlsiAddFplmnCnf          alsiAddFplmnCnf)
    SIG_DEF( SIG_ALSI_DELETE_FPLMN_REQ,         AlsiDeleteFplmnReq       alsiDeleteFplmnReq)
    SIG_DEF( SIG_ALSI_DELETE_FPLMN_CNF,         AlsiDeleteFplmnCnf       alsiDeleteFplmnCnf)
#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
    SIG_DEF( SIG_ALSI_READ_DIALNUM_REQ,         AlsiReadDialNumReq       alsiReadDialNumReq)
    SIG_DEF( SIG_ALSI_READ_DIALNUM_CNF,         AlsiReadDialNumCnf       alsiReadDialNumCnf)
    SIG_DEF( SIG_ALSI_READ_CCP_REQ,               AlsiReadCcpReq             alsiReadCcpReq)
    SIG_DEF( SIG_ALSI_READ_CCP_CNF,               AlsiReadCcpCnf             alsiReadCcpCnf)
    SIG_DEF( SIG_ALSI_READ_CALL_INFO_REQ,         AlsiReadCallInfoReq        alsiReadCallInfoReq)
    SIG_DEF( SIG_ALSI_READ_CALL_INFO_CNF,         AlsiReadCallInfoCnf        alsiReadCallInfoCnf)
    SIG_DEF( SIG_ALSI_READ_ACC_CALL_TIMER_REQ,    AlsiReadAccCallTimerReq    alsiReadAccCallTimerReq)
    SIG_DEF( SIG_ALSI_READ_ACC_CALL_TIMER_CNF,    AlsiReadAccCallTimerCnf    alsiReadAccCallTimerCnf)
    SIG_DEF( SIG_ALSI_RESET_ACC_CALL_TIMER_REQ,   AlsiResetAccCallTimerReq   alsiResetAccCallTimerReq)
    SIG_DEF( SIG_ALSI_RESET_ACC_CALL_TIMER_CNF,   AlsiResetAccCallTimerCnf   alsiResetAccCallTimerCnf)
    SIG_DEF( SIG_ALSI_ACC_CALL_TIMER_CHANGED_IND, AlsiAccCallTimerChangedInd alsiAccCallTimerChangedInd)
    SIG_DEF( SIG_ALSI_DELETE_DIALNUM_REQ,       AlsiDeleteDialNumReq     alsiDeleteDialNumReq)
    SIG_DEF( SIG_ALSI_DELETE_DIALNUM_CNF,       AlsiDeleteDialNumCnf     alsiDeleteDialNumCnf)
    SIG_DEF( SIG_ALSI_WRITE_DIALNUM_REQ,        AlsiWriteDialNumReq      alsiWriteDialNumReq)
    SIG_DEF( SIG_ALSI_WRITE_DIALNUM_CNF,        AlsiWriteDialNumCnf      alsiWriteDialNumCnf)
    SIG_DEF( SIG_ALSI_WRITE_CALL_INFO_REQ,      AlsiWriteCallInfoReq     alsiWriteCallInfoReq)
    SIG_DEF( SIG_ALSI_WRITE_CALL_INFO_CNF,      AlsiWriteCallInfoCnf     alsiWriteCallInfoCnf)
    SIG_DEF( SIG_ALSI_WRITE_CALL_INFO_SIMPLE_REQ, AlsiWriteCallInfoSimpleReq alsiWriteCallInfoSimpleReq)
    SIG_DEF( SIG_ALSI_WRITE_CALL_INFO_SIMPLE_CNF, AlsiWriteCallInfoSimpleCnf alsiWriteCallInfoSimpleCnf)
    SIG_DEF( SIG_ALSI_LIST_DIALNUM_REQ,         AlsiListDialNumReq       alsiListDialNumReq)
    SIG_DEF( SIG_ALSI_LIST_DIALNUM_CNF,         AlsiListDialNumCnf       alsiListDialNumCnf)
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    SIG_DEF( SIG_ALSI_DIALNUM_STATUS_REQ,       AlsiDialNumStatusReq     alsiDialNumStatusReq)
    SIG_DEF( SIG_ALSI_DIALNUM_STATUS_CNF,       AlsiDialNumStatusCnf     alsiDialNumStatusCnf)
    SIG_DEF( SIG_ALSI_READ_MSISDN_REQ,          AlsiReadMsisdnReq        alsiReadMsisdnReq)
    SIG_DEF( SIG_ALSI_READ_MSISDN_CNF,          AlsiReadMsisdnCnf        alsiReadMsisdnCnf)
#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
    SIG_DEF( SIG_ALSI_FIXED_DIAL_REQ,           AlsiFixedDialReq         alsiFixedDialReq)
    SIG_DEF( SIG_ALSI_FIXED_DIAL_CNF,           AlsiFixedDialCnf         alsiFixedDialCnf)
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    SIG_DEF( SIG_ALSI_READ_SM_REQ,              AlsiReadSmReq            alsiReadSmReq)
    SIG_DEF( SIG_ALSI_READ_SM_CNF,              AlsiReadSmCnf            alsiReadSmCnf)
    SIG_DEF( SIG_ALSI_WRITE_SM_REQ,             AlsiWriteSmReq           alsiWriteSmReq)
    SIG_DEF( SIG_ALSI_WRITE_SM_CNF,             AlsiWriteSmCnf           alsiWriteSmCnf)
    SIG_DEF( SIG_ALSI_DELETE_SM_REQ,            AlsiDeleteSmReq          alsiDeleteSmReq)
    SIG_DEF( SIG_ALSI_DELETE_SM_CNF,            AlsiDeleteSmCnf          alsiDeleteSmCnf)
    SIG_DEF( SIG_ALSI_ADD_SM_REQ,               AlsiAddSmReq             alsiAddSmReq)
    SIG_DEF( SIG_ALSI_ADD_SM_CNF,               AlsiAddSmCnf             alsiAddSmCnf)
    SIG_DEF( SIG_ALSI_SET_SM_RECORDSTAT_REQ,    AlsiSetSmRecordStatReq   alsiSetSmRecordStatReq)
    SIG_DEF( SIG_ALSI_SET_SM_RECORDSTAT_CNF,    AlsiSetSmRecordStatCnf   alsiSetSmRecordStatCnf)
    SIG_DEF( SIG_ALSI_SET_SM_RECORD_SENT_REQ,   AlsiSetSmRecordSentReq   alsiSetSmRecordSentReq)
    SIG_DEF( SIG_ALSI_SET_SM_RECORD_SENT_CNF,   AlsiSetSmRecordSentCnf   alsiSetSmRecordSentCnf)
    SIG_DEF( SIG_ALSI_SET_SM_SR_STAT_REQ,       AlsiSetSmSrStatReq       alsiSetSmSrStatReq)
    SIG_DEF( SIG_ALSI_SET_SM_SR_STAT_CNF,       AlsiSetSmSrStatCnf       alsiSetSmSrStatCnf)
    SIG_DEF( SIG_ALSI_READ_SMSP_REQ,            AlsiReadSmspReq          alsiReadSmspReq)
    SIG_DEF( SIG_ALSI_READ_SMSP_CNF,            AlsiReadSmspCnf          alsiReadSmspCnf)
    SIG_DEF( SIG_ALSI_WRITE_SMSP_REQ,           AlsiWriteSmspReq         alsiWriteSmspReq)
    SIG_DEF( SIG_ALSI_WRITE_SMSP_CNF,           AlsiWriteSmspCnf         alsiWriteSmspCnf)
    SIG_DEF( SIG_ALSI_DELETE_SMSP_REQ,          AlsiDeleteSmspReq        alsiDeleteSmspReq)
    SIG_DEF( SIG_ALSI_DELETE_SMSP_CNF,          AlsiDeleteSmspCnf        alsiDeleteSmspCnf)
    SIG_DEF( SIG_ALSI_ADD_SMSP_REQ,             AlsiAddSmspReq           alsiAddSmspReq)
    SIG_DEF( SIG_ALSI_ADD_SMSP_CNF,             AlsiAddSmspCnf           alsiAddSmspCnf)
    SIG_DEF( SIG_ALSI_LIST_SMSP_REQ,            AlsiListSmspReq          alsiListSmspReq)
    SIG_DEF( SIG_ALSI_LIST_SMSP_CNF,            AlsiListSmspCnf          alsiListSmspCnf)
    SIG_DEF( SIG_ALSI_READ_SMSS_REQ,            AlsiReadSmssReq          alsiReadSmssReq)
    SIG_DEF( SIG_ALSI_READ_SMSS_CNF,            AlsiReadSmssCnf          alsiReadSmssCnf)
    SIG_DEF( SIG_ALSI_WRITE_SMSS_REQ,           AlsiWriteSmssReq         alsiWriteSmssReq)
    SIG_DEF( SIG_ALSI_WRITE_SMSS_CNF,           AlsiWriteSmssCnf         alsiWriteSmssCnf)
#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
    SIG_DEF( SIG_ALSI_CHV_FUNCTION_REQ,         AlsiChvFunctionReq       alsiChvFunctionReq)
    SIG_DEF( SIG_ALSI_CHV_FUNCTION_CNF,         AlsiChvFunctionCnf       alsiChvFunctionCnf)
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    SIG_DEF( SIG_ALSI_READ_DIR_STATUS_REQ,      AlsiReadDirStatusReq     alsiReadDirStatusReq)
    SIG_DEF( SIG_ALSI_READ_DIR_STATUS_CNF,      AlsiReadDirStatusCnf     alsiReadDirStatusCnf)
    SIG_DEF( SIG_ALSI_READ_EF_STATUS_REQ,       AlsiReadEfStatusReq      alsiReadEfStatusReq)
    SIG_DEF( SIG_ALSI_READ_EF_STATUS_CNF,       AlsiReadEfStatusCnf      alsiReadEfStatusCnf)
#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
    SIG_DEF( SIG_ALSI_READ_GID_REQ,             AlsiReadGidReq           alsiReadGidReq)
    SIG_DEF( SIG_ALSI_READ_GID_CNF,             AlsiReadGidCnf           alsiReadGidCnf)
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    SIG_DEF( SIG_ALSI_READ_SPN_REQ,             AlsiReadSpnReq           alsiReadSpnReq)
    SIG_DEF( SIG_ALSI_READ_SPN_CNF,             AlsiReadSpnCnf           alsiReadSpnCnf)
#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
    SIG_DEF( SIG_ALSI_READ_SPDI_REQ,            AlsiReadSpdiReq          alsiReadSpdiReq)
    SIG_DEF( SIG_ALSI_READ_SPDI_CNF,            AlsiReadSpdiCnf          alsiReadSpdiCnf)
    SIG_DEF( SIG_ALSI_READ_CFIS_REQ,            AlsiReadCfisReq          alsiReadCfisReq)
    SIG_DEF( SIG_ALSI_READ_CFIS_CNF,            AlsiReadCfisCnf          alsiReadCfisCnf)
    SIG_DEF( SIG_ALSI_WRITE_CFIS_REQ,           AlsiWriteCfisReq         alsiWriteCfisReq)
    SIG_DEF( SIG_ALSI_WRITE_CFIS_CNF,           AlsiWriteCfisCnf         alsiWriteCfisCnf)
    SIG_DEF( SIG_ALSI_DELETE_CFIS_REQ,          AlsiDeleteCfisReq        alsiDeleteCfisReq)
    SIG_DEF( SIG_ALSI_DELETE_CFIS_CNF,          AlsiDeleteCfisCnf        alsiDeleteCfisCnf)
    SIG_DEF( SIG_ALSI_LIST_DIALNUM_EXT_REQ,     AlsiListDialNumExtReq    alsiListDialNumExtReq)
    SIG_DEF( SIG_ALSI_LIST_DIALNUM_EXT_CNF,     AlsiListDialNumExtCnf    alsiListDialNumExtCnf)
    SIG_DEF( SIG_ALSI_CHV_STATUS_IND,           AlsiChvStatusInd         alsiChvStatusInd)  /*job 104913*/
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    SIG_DEF( SIG_ALSI_READ_ELP_REQ,             AlsiReadElpReq            alsiReadElpReq)
    SIG_DEF( SIG_ALSI_READ_ELP_CNF,             AlsiReadElpCnf            alsiReadElpCnf)
    SIG_DEF( SIG_ALSI_WRITE_ELP_REQ,            AlsiWriteElpReq           alsiWriteElpReq)
    SIG_DEF( SIG_ALSI_WRITE_ELP_CNF,            AlsiWriteElpCnf           alsiWriteElpCnf)
#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
    SIG_DEF( SIG_ALSI_BARRED_DIAL_REQ,          AlsiBarredDialReq         alsiBarredDialReq)
    SIG_DEF( SIG_ALSI_BARRED_DIAL_CNF,          AlsiBarredDialCnf         alsiBarredDialCnf)
    SIG_DEF( SIG_ALSI_READ_VGCS_VBS_REQ,        AlsiReadVgcsVbsReq        alsiReadVgcsVbsReq)
    SIG_DEF( SIG_ALSI_READ_VGCS_VBS_CNF,        AlsiReadVgcsVbsCnf        alsiReadVgcsVbsCnf)
    SIG_DEF( SIG_ALSI_READ_VGCS_VBS_STATUS_REQ, AlsiReadVgcsVbsStatusReq  alsiReadVgcsVbsStatusReq)
    SIG_DEF( SIG_ALSI_READ_VGCS_VBS_STATUS_CNF, AlsiReadVgcsVbsStatusCnf  alsiReadVgcsVbsStatusCnf)
    SIG_DEF( SIG_ALSI_READ_EMLPP_REQ,           AlsiReadEmlppReq          alsiReadEmlppReq)
    SIG_DEF( SIG_ALSI_READ_EMLPP_CNF,           AlsiReadEmlppCnf          alsiReadEmlppCnf)
    SIG_DEF( SIG_ALSI_READ_AAEM_REQ,            AlsiReadAaemReq           alsiReadAaemReq)
    SIG_DEF( SIG_ALSI_READ_AAEM_CNF,            AlsiReadAaemCnf           alsiReadAaemCnf)
    SIG_DEF( SIG_ALSI_WRITE_AAEM_REQ,           AlsiWriteAaemReq          alsiWriteAaemReq)
    SIG_DEF( SIG_ALSI_WRITE_AAEM_CNF,           AlsiWriteAaemCnf          alsiWriteAaemCnf)
    SIG_DEF( SIG_ALSI_READ_DCK_REQ,             AlsiReadDckReq            alsiReadDckReq)
    SIG_DEF( SIG_ALSI_READ_DCK_CNF,             AlsiReadDckCnf            alsiReadDckCnf)
    SIG_DEF( SIG_ALSI_RESET_DCK_REQ,            AlsiResetDckReq           alsiResetDckReq)
    SIG_DEF( SIG_ALSI_RESET_DCK_CNF,            AlsiResetDckCnf           alsiResetDckCnf)
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    SIG_DEF( SIG_ALSI_READ_CNL_REQ,             AlsiReadCnlReq            alsiReadCnlReq)
    SIG_DEF( SIG_ALSI_READ_CNL_CNF,             AlsiReadCnlCnf            alsiReadCnlCnf)
#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
    SIG_DEF( SIG_ALSI_READ_NIA_REQ,             AlsiReadNiaReq            alsiReadNiaReq)
    SIG_DEF( SIG_ALSI_READ_NIA_CNF,             AlsiReadNiaCnf            alsiReadNiaCnf)
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    SIG_DEF( SIG_ALSI_READ_SMSR_REQ,            AlsiReadSmsrReq           alsiReadSmsrReq)
    SIG_DEF( SIG_ALSI_READ_SMSR_CNF,            AlsiReadSmsrCnf           alsiReadSmsrCnf)
    SIG_DEF( SIG_ALSI_WRITE_SMSR_REQ,           AlsiWriteSmsrReq          alsiWriteSmsrReq)
    SIG_DEF( SIG_ALSI_WRITE_SMSR_CNF,           AlsiWriteSmsrCnf          alsiWriteSmsrCnf)
    SIG_DEF( SIG_ALSI_DELETE_SMSR_REQ,          AlsiDeleteSmsrReq         alsiDeleteSmsrReq)
    SIG_DEF( SIG_ALSI_DELETE_SMSR_CNF,          AlsiDeleteSmsrCnf         alsiDeleteSmsrCnf)
    SIG_DEF( SIG_ALSI_LIST_SMSR_REQ,            AlsiListSmsrReq           alsiListSmsrReq)
    SIG_DEF( SIG_ALSI_LIST_SMSR_CNF,            AlsiListSmsrCnf           alsiListSmsrCnf)
    SIG_DEF( SIG_ALSI_LIST_OPL_REQ,             AlsiListOplReq            alsiListOplReq)
    SIG_DEF( SIG_ALSI_LIST_OPL_CNF,             AlsiListOplCnf            alsiListOplCnf)
    SIG_DEF( SIG_ALSI_LIST_PNN_REQ,             AlsiListPnnReq            alsiListPnnReq)
    SIG_DEF( SIG_ALSI_LIST_PNN_CNF,             AlsiListPnnCnf            alsiListPnnCnf)
#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
    /* CPHS specific signals */
    SIG_DEF( SIG_ALSI_READ_CPHS_DATA_REQ,       EmptySignal               alsiReadCphsDataReq)
    SIG_DEF( SIG_ALSI_CPHS_DATA_IND,            AlsiCphsDataInd           alsiCphsDataInd)
    SIG_DEF( SIG_ALSI_CPHS_READ_CFF_REQ,        AlsiCphsReadCffReq        alsiCphsReadCffReq)
    SIG_DEF( SIG_ALSI_CPHS_READ_CFF_CNF,        AlsiCphsReadCffCnf        alsiCphsReadCffCnf)
    SIG_DEF( SIG_ALSI_CPHS_WRITE_CFF_REQ,       AlsiCphsWriteCffReq       alsiCphsWriteCffReq)
    SIG_DEF( SIG_ALSI_CPHS_WRITE_CFF_CNF,       AlsiCphsWriteCffCnf       alsiCphsWriteCffCnf)
    SIG_DEF( SIG_ALSI_CPHS_READ_VMWF_REQ,       AlsiCphsReadVmwfReq       alsiCphsReadVmwfReq)
    SIG_DEF( SIG_ALSI_CPHS_READ_VMWF_CNF,       AlsiCphsReadVmwfCnf       alsiCphsReadVmwfCnf)
    SIG_DEF( SIG_ALSI_CPHS_WRITE_VMWF_REQ,      AlsiCphsWriteVmwfReq      alsiCphsWriteVmwfReq)
    SIG_DEF( SIG_ALSI_CPHS_WRITE_VMWF_CNF,      AlsiCphsWriteVmwfCnf      alsiCphsWriteVmwfCnf)

    SIG_DEF( SIG_ALSI_CPHS_READ_CSP_REQ,        AlsiCphsReadCspReq        alsiCphsReadCspReq)
    SIG_DEF( SIG_ALSI_CPHS_READ_CSP_CNF,        AlsiCphsReadCspCnf        alsiCphsReadCspCnf)
    SIG_DEF( SIG_ALSI_CPHS_WRITE_CSP_ENTRY_REQ, AlsiCphsWriteCspEntryReq  alsiCphsWriteCspEntryReq)
    SIG_DEF( SIG_ALSI_CPHS_WRITE_CSP_ENTRY_CNF, AlsiCphsWriteCspEntryCnf  alsiCphsWriteCspEntryCnf)
    SIG_DEF( SIG_ALSI_CPHS_INFO_NUM_STATUS_REQ, AlsiCphsInfoNumStatusReq  alsiCphsInfoNumStatusReq)
    SIG_DEF( SIG_ALSI_CPHS_INFO_NUM_STATUS_CNF, AlsiCphsInfoNumStatusCnf  alsiCphsInfoNumStatusCnf)
    SIG_DEF( SIG_ALSI_CPHS_LIST_INFO_NUM_REQ,   AlsiCphsListInfoNumReq    alsiCphsListInfoNumReq)
    SIG_DEF( SIG_ALSI_CPHS_LIST_INFO_NUM_CNF,   AlsiCphsListInfoNumCnf    alsiCphsListInfoNumCnf)
    SIG_DEF( SIG_ALSI_CPHS_READ_INFO_NUM_REQ,   AlsiCphsReadInfoNumReq    alsiCphsReadInfoNumReq)
    SIG_DEF( SIG_ALSI_CPHS_READ_INFO_NUM_CNF,   AlsiCphsReadInfoNumCnf    alsiCphsReadInfoNumCnf)
    SIG_DEF( SIG_ALSI_CPHS_WRITE_INFO_NUM_REQ,  AlsiCphsWriteInfoNumReq   alsiCphsWriteInfoNumReq)
    SIG_DEF( SIG_ALSI_CPHS_WRITE_INFO_NUM_CNF,  AlsiCphsWriteInfoNumCnf   alsiCphsWriteInfoNumCnf)
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    SIG_DEF( SIG_ALSI_WRITE_RAW_SM_REQ,         AlsiWriteRawSmReq         alsiWriteRawSmReq)
    SIG_DEF( SIG_ALSI_WRITE_RAW_SM_CNF,         AlsiWriteRawSmCnf         alsiWriteRawSmCnf)
    SIG_DEF( SIG_ALSI_ADD_RAW_SM_REQ,           AlsiAddRawSmReq           alsiAddRawSmReq)
    SIG_DEF( SIG_ALSI_ADD_RAW_SM_CNF,           AlsiAddRawSmCnf           alsiAddRawSmCnf)

    SIG_DEF( SIG_ALSI_GET_FREE_SM_REQ,          AlsiGetFreeSmReq          alsiGetFreeSmReq)
    SIG_DEF( SIG_ALSI_GET_FREE_SM_CNF,          AlsiGetFreeSmCnf          alsiGetFreeSmCnf)
#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
    SIG_DEF( SIG_ALSI_READ_MMSN_REQ,            AlsiReadMmsnReq              alsiReadMmsnReq)
    SIG_DEF( SIG_ALSI_READ_MMSN_CNF,            AlsiReadMmsnCnf              alsiReadMmsnCnf)
    SIG_DEF( SIG_ALSI_WRITE_MMSN_REQ,           AlsiWriteMmsnReq             alsiWriteMmsnReq)
    SIG_DEF( SIG_ALSI_WRITE_MMSN_CNF,           AlsiWriteMmsnCnf             alsiWriteMmsnCnf)
    SIG_DEF( SIG_ALSI_DELETE_MMSN_REQ,          AlsiDeleteMmsnReq            alsiDeleteMmsnReq)
    SIG_DEF( SIG_ALSI_DELETE_MMSN_CNF,          AlsiDeleteMmsnCnf            alsiDeleteMmsnCnf)
    SIG_DEF( SIG_ALSI_READ_MMSCP_REQ,           AlsiReadMmscpReq             alsiReadMmscpReq)
    SIG_DEF( SIG_ALSI_READ_MMSCP_CNF,           AlsiReadMmscpCnf             alsiReadMmscpCnf)
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    SIG_DEF( SIG_ALSI_USIM_APP_START_REQ,       AlsiUsimApplicationStartReq  alsiUsimApplicationStartReq)
    SIG_DEF( SIG_ALSI_USIM_APP_START_CNF,       AlsiUsimApplicationStartCnf  alsiUsimApplicationStartCnf)
    SIG_DEF( SIG_ALSI_READ_DIR_REQ,             AlsiReadDirReq               alsiReadDirReq)    /*reads EF_DIR on the SIM*/
    SIG_DEF( SIG_ALSI_READ_DIR_CNF,             AlsiReadDirCnf               alsiReadDirCnf)
    SIG_DEF( SIG_ALSI_PIN_FUNCTION_REQ,         AlsiPinFunctionReq           alsiPinFunctionReq)
    SIG_DEF( SIG_ALSI_PIN_FUNCTION_CNF,         AlsiPinFunctionCnf           alsiPinFunctionCnf)
    SIG_DEF( SIG_ALSI_READ_EXT_ECC_REQ,         AlsiReadExtEccReq            alsiReadExtEccReq)
    SIG_DEF( SIG_ALSI_READ_EXT_ECC_CNF,         AlsiReadExtEccCnf            alsiReadExtEccCnf)
#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
    SIG_DEF( SIG_ALSI_READ_NETPAR_REQ,          AlsiReadNetParReq            alsiReadNetParReq)
    SIG_DEF( SIG_ALSI_READ_NETPAR_CNF,          AlsiReadNetParCnf            alsiReadNetParCnf)
    SIG_DEF( SIG_ALSI_WRITE_NETPAR_REQ,         AlsiWriteNetParReq           alsiWriteNetParReq)
    SIG_DEF( SIG_ALSI_WRITE_NETPAR_CNF,         AlsiWriteNetParCnf           alsiWriteNetParCnf)
    SIG_DEF( SIG_ALSI_READ_MMSUP_REQ,           AlsiReadMmsupReq             alsiReadMmsupReq)
    SIG_DEF( SIG_ALSI_READ_MMSUP_CNF,           AlsiReadMmsupCnf             alsiReadMmsupCnf)
    SIG_DEF( SIG_ALSI_WRITE_MMSUP_REQ,          AlsiWriteMmsupReq            alsiWriteMmsupReq)
    SIG_DEF( SIG_ALSI_WRITE_MMSUP_CNF,          AlsiWriteMmsupCnf            alsiWriteMmsupCnf)
    SIG_DEF( SIG_ALSI_DELETE_MMSUP_REQ,         AlsiDeleteMmsupReq           alsiDeleteMmsupReq)
    SIG_DEF( SIG_ALSI_DELETE_MMSUP_CNF,         AlsiDeleteMmsupCnf           alsiDeleteMmsupCnf)
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    SIG_DEF( SIG_ALSI_LIST_ACL_REQ,             AlsiListAclReq               alsiListAclReq)
    SIG_DEF( SIG_ALSI_LIST_ACL_CNF,             AlsiListAclCnf               alsiListAclCnf)
    SIG_DEF( SIG_ALSI_WRITE_ACL_REQ,            AlsiWriteAclReq              alsiWriteAclReq)
    SIG_DEF( SIG_ALSI_WRITE_ACL_CNF,            AlsiWriteAclCnf              alsiWriteAclCnf)
    SIG_DEF( SIG_ALSI_DELETE_ACL_REQ,           AlsiDeleteAclReq             alsiDeleteAclReq)
    SIG_DEF( SIG_ALSI_DELETE_ACL_CNF,           AlsiDeleteAclCnf             alsiDeleteAclCnf)
    SIG_DEF( SIG_ALSI_SET_ACL_REQ,              AlsiSetAclReq                alsiSetAclReq)
    SIG_DEF( SIG_ALSI_SET_ACL_CNF,              AlsiSetAclCnf                alsiSetAclCnf)
    SIG_DEF( SIG_ALSI_CHANGE_TARGET_TASK_ID_REQ,  AlsiChangeTargetTaskIdReq   alsiChangeTargetTaskIdReq)
    SIG_DEF( SIG_ALSI_CHANGE_TARGET_TASK_ID_CNF,  AlsiChangeTargetTaskIdCnf   alsiChangeTargetTaskIdCnf)
#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
    SIG_DEF( SIG_ALSI_READ_HZ_PARAMS_REQ,         AlsiReadHZParamsReq         alsiReadHZParamsReq)
    SIG_DEF( SIG_ALSI_READ_HZ_PARAMS_CNF,         AlsiReadHZParamsCnf         alsiReadHZParamsCnf)
    SIG_DEF( SIG_ALSI_READ_HZ_CACHE_REQ,          AlsiReadHZCacheReq          alsiReadHZCacheReq)
    SIG_DEF( SIG_ALSI_READ_HZ_CACHE_CNF,          AlsiReadHZCacheCnf          alsiReadHZCacheCnf)
    SIG_DEF( SIG_ALSI_WRITE_HZ_CACHE_REQ,         AlsiWriteHZCacheReq         alsiWriteHZCacheReq)
    SIG_DEF( SIG_ALSI_WRITE_HZ_CACHE_CNF,         AlsiWriteHZCacheCnf         alsiWriteHZCacheCnf)

    SIG_DEF( SIG_ALSI_READ_MSG_WAITING_INFO_REQ,  AlsiReadMsgWaitingInfoReq   alsiReadMsgWaitingInfoReq)
    SIG_DEF( SIG_ALSI_READ_MSG_WAITING_INFO_CNF,  AlsiReadMsgWaitingInfoCnf   alsiReadMsgWaitingInfoCnf)
    SIG_DEF( SIG_ALSI_WRITE_MSG_WAITING_INFO_REQ, AlsiWriteMsgWaitingInfoReq  alsiWriteMsgWaitingInfoReq)
    SIG_DEF( SIG_ALSI_WRITE_MSG_WAITING_INFO_CNF, AlsiWriteMsgWaitingInfoCnf  alsiWriteMsgWaitingInfoCnf)
    SIG_DEF( SIG_ALSI_READ_MAILBOX_INFO_REQ,      AlsiReadMailboxInfoReq      alsiReadMailboxInfoReq)
    SIG_DEF( SIG_ALSI_READ_MAILBOX_INFO_CNF,      AlsiReadMailboxInfoCnf      alsiReadMailboxInfoCnf)
    SIG_DEF( SIG_ALSI_WRITE_MAILBOX_INFO_REQ,     AlsiWriteMailboxInfoReq     alsiWriteMailboxInfoReq)
    SIG_DEF( SIG_ALSI_WRITE_MAILBOX_INFO_CNF,     AlsiWriteMailboxInfoCnf     alsiWriteMailboxInfoCnf)
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    SIG_DEF( SIG_ALSI_EMULATE_USIM_REQ,           AlsiEmulateUsimReq          alsiEmulateUsimReq)
    SIG_DEF( SIG_ALSI_EMULATE_USIM_CNF,           AlsiEmulateUsimCnf          alsiEmulateUsimCnf)
    SIG_DEF( SIG_ALSI_READ_NAS_CONFIG_REQ,        AlsiReadNasConfigReq        alsiReadNasConfigReq)
    SIG_DEF( SIG_ALSI_READ_NAS_CONFIG_CNF,        AlsiReadNasConfigCnf        alsiReadNasConfigCnf)
    SIG_DEF( SIG_ALSI_OPEN_LOGICAL_CHANNEL_REQ,   AlsiOpenLogicalChannelReq   alsiOpenLogicalChannelReq)
    SIG_DEF( SIG_ALSI_OPEN_LOGICAL_CHANNEL_CNF,   AlsiOpenLogicalChannelCnf   alsiOpenLogicalChannelCnf)
    SIG_DEF( SIG_ALSI_CLOSE_LOGICAL_CHANNEL_REQ,  AlsiCloseLogicalChannelReq  alsiCloseLogicalChannelReq)
    SIG_DEF( SIG_ALSI_CLOSE_LOGICAL_CHANNEL_CNF,  AlsiCloseLogicalChannelCnf  alsiCloseLogicalChannelCnf)
    SIG_DEF( SIG_ALSI_SIM_LOGICAL_CHANNEL_ACCESS_REQ, AlsiSimLogicalChannelAccessReq alsiSimLogicalChannelAccessReq)
    SIG_DEF( SIG_ALSI_SIM_LOGICAL_CHANNEL_ACCESS_CNF, AlsiSimLogicalChannelAccessCnf alsiSimLogicalChannelAccessCnf)
    SIG_DEF( SIG_ALSI_SIM_RAW_APDU_ACCESS_REQ,    AlsiSimRawApduAccessReq     alsiSimRawApduAccessReq)
    SIG_DEF( SIG_ALSI_SIM_RAW_APDU_ACCESS_CNF,    AlsiSimRawApduAccessCnf     alsiSimRawApduAccessCnf)
    SIG_DEF( SIG_ALSI_READ_NCPIP_REQ,             AlsiReadNcpIpReq            alsiReadNcpIpReq)
    SIG_DEF( SIG_ALSI_READ_NCPIP_CNF,             AlsiReadNcpIpCnf            alsiReadNcpIpCnf)
#endif
#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
#if defined(UPGRADE_CMMB)
    SIG_DEF( SIG_ALSI_CMMB_AUTH_REQ,              AlsiCmmbAuthReq             alsiCmmbAuthReq)
    SIG_DEF( SIG_ALSI_CMMB_AUTH_CNF,              AlsiCmmbAuthCnf             alsiCmmbAuthCnf)
#endif
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
#if defined (UPGRADE_LTE)
    SIG_DEF( SIG_ALSI_READ_NVRAM_EPS_SECURITY_IND,  EmptySignal                  alsiReadNvramEpsSecurityInd)
    SIG_DEF( SIG_ALSI_READ_NVRAM_EPS_SECURITY_RSP,  AlsiReadNvramEpsSecurityRsp  alsiReadNvramEpsSecurityRsp)
    SIG_DEF( SIG_ALSI_WRITE_NVRAM_EPS_SECURITY_IND, AlsiWriteNvramEpsSecurityInd alsiWriteNvramEpsSecurityInd)
    SIG_DEF( SIG_ALSI_WRITE_NVRAM_EPS_SECURITY_RSP, AlsiWriteNvramEpsSecurityRsp alsiWriteNvramEpsSecurityRsp)
    SIG_DEF( SIG_ALSI_READ_NVRAM_EPS_LOC_INFO_IND,  EmptySignal                  alsiReadNvramEpsLocInfoInd)
    SIG_DEF( SIG_ALSI_READ_NVRAM_EPS_LOC_INFO_RSP,  AlsiReadNvramEpsLocInfoRsp   alsiReadNvramEpsLocInfoRsp)
    SIG_DEF( SIG_ALSI_WRITE_NVRAM_EPS_LOC_INFO_IND, AlsiWriteNvramEpsLocInfoInd alsiWriteNvramEpsLocInfoInd)
    SIG_DEF( SIG_ALSI_WRITE_NVRAM_EPS_LOC_INFO_RSP, AlsiWriteNvramEpsLocInfoRsp alsiWriteNvramEpsLocInfoRsp)
    SIG_DEF( SIG_ALSI_READ_NVRAM_EPS_EMM_INFO_IMSI_IND,  EmptySignal                        alsiReadNvramEpsEmmInfoImsiInd)
    SIG_DEF( SIG_ALSI_READ_NVRAM_EPS_EMM_INFO_IMSI_RSP,  AlsiReadNvramEpsEmmInfoImsiRsp     alsiReadNvramEpsEmmInfoImsiRsp)
    SIG_DEF( SIG_ALSI_WRITE_NVRAM_EPS_EMM_INFO_IMSI_IND, AlsiWriteNvramEpsEmmInfoImsiInd    alsiWriteNvramEpsEmmInfoImsiInd)
    SIG_DEF( SIG_ALSI_WRITE_NVRAM_EPS_EMM_INFO_IMSI_RSP, AlsiWriteNvramEpsEmmInfoImsiRsp    alsiWriteNvramEpsEmmInfoImsiRsp)
#endif
#if defined (FEA_RPM)
    SIG_DEF( SIG_ALSI_READ_RPM_ENABLED_REQ,       AlsiReadRpmEnabledReq       alsiReadRpmEnabledReq)
    SIG_DEF( SIG_ALSI_READ_RPM_ENABLED_CNF,       AlsiReadRpmEnabledCnf       alsiReadRpmEnabledCnf)
    SIG_DEF( SIG_ALSI_READ_RPM_PARAMETERS_REQ,    AlsiReadRpmParametersReq    alsiReadRpmParametersReq)
    SIG_DEF( SIG_ALSI_READ_RPM_PARAMETERS_CNF,    AlsiReadRpmParametersCnf    alsiReadRpmParametersCnf)
    SIG_DEF( SIG_ALSI_READ_RPM_LEAK_RATE_REQ,     AlsiReadRpmLeakRateReq      alsiReadRpmLeakRateReq)
    SIG_DEF( SIG_ALSI_READ_RPM_LEAK_RATE_CNF,     AlsiReadRpmLeakRateCnf      alsiReadRpmLeakRateCnf)
    SIG_DEF( SIG_ALSI_READ_RPM_COUNTERS_REQ,      AlsiReadRpmCountersReq      alsiReadRpmCountersReq)
    SIG_DEF( SIG_ALSI_READ_RPM_COUNTERS_CNF,      AlsiReadRpmCountersCnf      alsiReadRpmCountersCnf)
    SIG_DEF( SIG_ALSI_WRITE_RPM_COUNTERS_REQ,     AlsiWriteRpmCountersReq     alsiWriteRpmCountersReq)
    SIG_DEF( SIG_ALSI_WRITE_RPM_COUNTERS_CNF,     AlsiWriteRpmCountersCnf     alsiWriteRpmCountersCnf)
    SIG_DEF( SIG_ALSI_READ_RPM_VERSION_REQ,       AlsiReadRpmVersionReq       alsiReadRpmVersionReq)
    SIG_DEF( SIG_ALSI_READ_RPM_VERSION_CNF,       AlsiReadRpmVersionCnf       alsiReadRpmVersionCnf)
    SIG_DEF( SIG_ALSI_WRITE_RPM_VERSION_REQ,      AlsiWriteRpmVersionReq      alsiWriteRpmVersionReq)
    SIG_DEF( SIG_ALSI_WRITE_RPM_VERSION_CNF,      AlsiWriteRpmVersionCnf      alsiWriteRpmVersionCnf)
#endif /* defined (FEA_RPM) */
    SIG_DEF( SIG_ALSI_FAKE_SIM_INIT_IND,          EmptySignal                 alsiFakeSimInitInd)
    /* moved outside of MBMS define so that any extra signals - not just MBMS - are handled in this signal base */
    /* any new ALSI signals need to be added to this additional base */
    SIG_DEF( SIG_ALSI_EX_DUMMY = ALSI_EX_SIGNAL_BASE,  EmptySignal            alsi_ex_dummy )
#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
#if defined (UPGRADE_3G)
# if defined (UPGRADE_3G_MBMS)
#  if defined (UPGRADE_3G_TDD128)
    SIG_DEF( SIG_ALSI_MBMS_GBA_AUTH_REQ,          AlsiMbmsGbaAuthReq          alsiMbmsGbaAuthReq)
    SIG_DEF( SIG_ALSI_MBMS_GBA_AUTH_CNF,          AlsiMbmsGbaAuthCnf          alsiMbmsGbaAuthCnf)
    SIG_DEF( SIG_ALSI_MBMS_GBA_WRITE_REQ,         AlsiMbmsGbaWriteReq         alsiMbmsGbaWriteReq)
    SIG_DEF( SIG_ALSI_MBMS_GBA_WRITE_CNF,         AlsiMbmsGbaWriteCnf         alsiMbmsGbaWriteCnf)
    SIG_DEF( SIG_ALSI_MBMS_MRK_REQ,               AlsiMbmsMrkReq              alsiMbmsMrkReq)
    SIG_DEF( SIG_ALSI_MBMS_MRK_CNF,               AlsiMbmsMrkCnf              alsiMbmsMrkCnf)
    SIG_DEF( SIG_ALSI_MBMS_MIKEY_REQ,             AlsiMbmsMikeyReq            alsiMbmsMikeyReq)
    SIG_DEF( SIG_ALSI_MBMS_MIKEY_CNF,             AlsiMbmsMikeyCnf            alsiMbmsMikeyCnf)
    SIG_DEF( SIG_ALSI_MBMS_READ_FILE_REQ,         AlsiMbmsReadFileReq         alsiMbmsReadFileReq)
    SIG_DEF( SIG_ALSI_MBMS_READ_FILE_CNF,         AlsiMbmsReadFileCnf         alsiMbmsReadFileCnf)
    SIG_DEF( SIG_ALSI_MBMS_WRITE_FILE_REQ,        AlsiMbmsWriteFileReq        alsiMbmsWriteFileReq)
    SIG_DEF( SIG_ALSI_MBMS_WRITE_FILE_CNF,        AlsiMbmsWriteFileCnf        alsiMbmsWriteFileCnf)
    SIG_DEF( SIG_ALSI_MBMS_DELETE_FILE_REQ,       AlsiMbmsDeleteFileReq       alsiMbmsDeleteFileReq)
    SIG_DEF( SIG_ALSI_MBMS_DELETE_FILE_CNF,       AlsiMbmsDeleteFileCnf       alsiMbmsDeleteFileCnf)
#if defined(SIM_EMULATION_ON)
    SIG_DEF( SIG_ALSI_MBMS_SET_TEST_MODE_REQ,     AlsiMbmsSetTestModeReq      alsiMbmsSetTestModeReq)
#endif
#  endif /* UPGRADE_3G_TDD128 */
# endif /* UPGRADE_3G_MBMS */
#endif /* UPGRADE_3G */
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
#if defined (UPGRADE_REL8_USIM)
    SIG_DEF( SIG_ALSI_CSIM_LOCK_REQ,              AlsiCsimLockReq             alsiCsimLockReq)
    SIG_DEF( SIG_ALSI_CSIM_LOCK_CNF,              AlsiCsimLockCnf             alsiCsimLockCnf)
    SIG_DEF( SIG_ALSI_CSIM_LOCK_IND,              AlsiCsimLockInd             alsiCsimLockInd)
#endif /* UPGRADE_REL8_USIM */

#if defined (ENABLE_DUAL_SIM_SOLUTION)
    SIG_DEF( SIG_ALSI_SIM_SELECT_REQ,             AlsiSimSelectReq            alsiSimSelectReq)
    SIG_DEF( SIG_ALSI_SIM_SELECT_CNF,             AlsiSimSelectCnf            alsiSimSelectCnf)
#endif
#if defined (SIM_EMULATION_ON)
    SIG_DEF( SIG_ALSI_WRITE_USIM_EMU_FILE_REQ,   AlsiWriteUsimEmuFileReq    alsiWriteUsimEmuFileReq)
    SIG_DEF( SIG_ALSI_WRITE_USIM_EMU_FILE_CNF,   AlsiWriteUsimEmuFileCnf    alsiWriteUsimEmuFileCnf)
#endif /* SIM_EMULATION_ON */
    SIG_DEF( SIG_ALSI_SIM_STARTED_IND,           EmptySignal                alsiSimStartedInd)
    SIG_DEF( SIG_SIM_SLEEP_PWROFF_DRV_IND,       EmptySignal                simSleepPwroffDrvInd)
    SIG_DEF( SIG_SIM_STATE_CHANGE_IND,           EmptySignal                simStateChangeInd)

#if defined(FEA_TEMP_OBSOLETE_SIM_MANAGER)
    /*moved ALSU signal base here as USIM manager will soon be part of 2G only builds*/
    SIG_DEF( SIG_ALSU_DUMMY = ALSU_SIGNAL_BASE,             EmptySignal                     alsu_dummy )
    SIG_DEF( SIG_ALSU_READ_PBR_REC_REQ,                     AlsuReadPbrRecReq               alsuReadPbrRecReq )
    SIG_DEF( SIG_ALSU_READ_PBR_REC_CNF,                     AlsuReadPbrRecCnf               alsuReadPbrRecCnf )
    SIG_DEF( SIG_ALSU_READ_PB_DIALNUM_REQ,                  AlsuReadPbDialNumReq            alsuReadPbDialNumReq )
    SIG_DEF( SIG_ALSU_READ_PB_DIALNUM_CNF,                  AlsuReadPbDialNumCnf            alsuReadPbDialNumCnf )
    SIG_DEF( SIG_ALSU_WRITE_PB_DIALNUM_REQ,                 AlsuWritePbDialNumReq           alsuWritePbDialNumReq )
    SIG_DEF( SIG_ALSU_WRITE_PB_DIALNUM_CNF,                 AlsuWritePbDialNumCnf           alsuWritePbDialNumCnf )
    SIG_DEF( SIG_ALSU_DELETE_PB_DIALNUM_REQ,                AlsuDeletePbDialNumReq          alsuDeletePbDialNumReq )
    SIG_DEF( SIG_ALSU_DELETE_PB_DIALNUM_CNF,                AlsuDeletePbDialNumCnf          alsuDeletePbDialNumCnf )
    SIG_DEF( SIG_ALSU_READ_PB_CCP_REQ,                      AlsuReadPbCcpReq                alsuReadPbCcpReq )
    SIG_DEF( SIG_ALSU_READ_PB_CCP_CNF,                      AlsuReadPbCcpCnf                alsuReadPbCcpCnf )
    SIG_DEF( SIG_ALSU_READ_PB_EMAIL_REQ,                    AlsuReadPbEmailReq              alsuReadPbEmailReq )
    SIG_DEF( SIG_ALSU_READ_PB_EMAIL_CNF,                    AlsuReadPbEmailCnf              alsuReadPbEmailCnf )
    SIG_DEF( SIG_ALSU_WRITE_PB_EMAIL_REQ,                   AlsuWritePbEmailReq             alsuWritePbEmailReq )
    SIG_DEF( SIG_ALSU_WRITE_PB_EMAIL_CNF,                   AlsuWritePbEmailCnf             alsuWritePbEmailCnf )
    SIG_DEF( SIG_ALSU_DELETE_PB_EMAIL_REQ,                  AlsuDeletePbEmailReq            alsuDeletePbEmailReq )
    SIG_DEF( SIG_ALSU_DELETE_PB_EMAIL_CNF,                  AlsuDeletePbEmailCnf            alsuDeletePbEmailCnf )
    SIG_DEF( SIG_ALSU_READ_PB_ANR_REQ,                      AlsuReadPbAnrReq                alsuReadPbAnrReq )
    SIG_DEF( SIG_ALSU_READ_PB_ANR_CNF,                      AlsuReadPbAnrCnf                alsuReadPbAnrCnf )
    SIG_DEF( SIG_ALSU_WRITE_PB_ANR_REQ,                     AlsuWritePbAnrReq               alsuWritePbAnrReq )
    SIG_DEF( SIG_ALSU_WRITE_PB_ANR_CNF,                     AlsuWritePbAnrCnf               alsuWritePbAnrCnf )
    SIG_DEF( SIG_ALSU_DELETE_PB_ANR_REQ,                    AlsuDeletePbAnrReq              alsuDeletePbAnrReq )
    SIG_DEF( SIG_ALSU_DELETE_PB_ANR_CNF,                    AlsuDeletePbAnrCnf              alsuDeletePbAnrCnf )
    SIG_DEF( SIG_ALSU_READ_PB_AAS_REQ,                      AlsuReadPbAasReq                alsuReadPbAasReq )
    SIG_DEF( SIG_ALSU_READ_PB_AAS_CNF,                      AlsuReadPbAasCnf                alsuReadPbAasCnf )
    SIG_DEF( SIG_ALSU_WRITE_PB_AAS_REQ,                     AlsuWritePbAasReq               alsuWritePbAasReq )
    SIG_DEF( SIG_ALSU_WRITE_PB_AAS_CNF,                     AlsuWritePbAasCnf               alsuWritePbAasCnf )
    SIG_DEF( SIG_ALSU_DELETE_PB_AAS_REQ,                    AlsuDeletePbAasReq              alsuDeletePbAasReq )
    SIG_DEF( SIG_ALSU_DELETE_PB_AAS_CNF,                    AlsuDeletePbAasCnf              alsuDeletePbAasCnf )
    SIG_DEF( SIG_ALSU_LIST_PB_PBC_REQ,                      AlsuListPbPbcReq                alsuListPbPbcReq )
    SIG_DEF( SIG_ALSU_LIST_PB_PBC_CNF,                      AlsuListPbPbcCnf                alsuListPbPbcCnf )
    SIG_DEF( SIG_ALSU_READ_PB_SNE_REQ,                      AlsuReadPbSneReq                alsuReadPbSneReq )
    SIG_DEF( SIG_ALSU_READ_PB_SNE_CNF,                      AlsuReadPbSneCnf                alsuReadPbSneCnf )
    SIG_DEF( SIG_ALSU_WRITE_PB_SNE_REQ,                     AlsuWritePbSneReq               alsuWritePbSneReq )
    SIG_DEF( SIG_ALSU_WRITE_PB_SNE_CNF,                     AlsuWritePbSneCnf               alsuWritePbSneCnf )
    SIG_DEF( SIG_ALSU_DELETE_PB_SNE_REQ,                    AlsuDeletePbSneReq              alsuDeletePbSneReq )
    SIG_DEF( SIG_ALSU_DELETE_PB_SNE_CNF,                    AlsuDeletePbSneCnf              alsuDeletePbSneCnf )
    SIG_DEF( SIG_ALSU_READ_PB_GAS_REQ,                      AlsuReadPbGasReq                alsuReadPbGasReq )
    SIG_DEF( SIG_ALSU_READ_PB_GAS_CNF,                      AlsuReadPbGasCnf                alsuReadPbGasCnf )
    SIG_DEF( SIG_ALSU_WRITE_PB_GAS_REQ,                     AlsuWritePbGasReq               alsuWritePbGasReq )
    SIG_DEF( SIG_ALSU_WRITE_PB_GAS_CNF,                     AlsuWritePbGasCnf               alsuWritePbGasCnf )
    SIG_DEF( SIG_ALSU_DELETE_PB_GAS_REQ,                    AlsuDeletePbGasReq              alsuDeletePbGasReq )
    SIG_DEF( SIG_ALSU_DELETE_PB_GAS_CNF,                    AlsuDeletePbGasCnf              alsuDeletePbGasCnf )
    SIG_DEF( SIG_ALSU_CLEAR_PB_GAS_REQ,                     AlsuClearPbGasReq               alsuClearPbGasReq )
    SIG_DEF( SIG_ALSU_CLEAR_PB_GAS_CNF,                     AlsuClearPbGasCnf               alsuClearPbGasCnf )
    SIG_DEF( SIG_ALSU_LIST_PB_GAS_REQ,                      AlsuListPbGasReq                alsuListPbGasReq )
    SIG_DEF( SIG_ALSU_LIST_PB_GAS_CNF,                      AlsuListPbGasCnf                alsuListPbGasCnf )
    SIG_DEF( SIG_ALSU_LIST_PB_AAS_REQ,                      AlsuListPbAasReq                alsuListPbAasReq )
    SIG_DEF( SIG_ALSU_LIST_PB_AAS_CNF,                      AlsuListPbAasCnf                alsuListPbAasCnf )
    SIG_DEF( SIG_ALSU_CLEAR_PB_AAS_REQ,                     AlsuClearPbAasReq               alsuClearPbAasReq )
    SIG_DEF( SIG_ALSU_CLEAR_PB_AAS_CNF,                     AlsuClearPbAasCnf               alsuClearPbAasCnf )
    SIG_DEF( SIG_ALSU_READ_PB_GRP_REQ,                      AlsuReadPbGrpReq                alsuReadPbGrpReq )
    SIG_DEF( SIG_ALSU_READ_PB_GRP_CNF,                      AlsuReadPbGrpCnf                alsuReadPbGrpCnf )
    SIG_DEF( SIG_ALSU_WRITE_PB_GRP_REQ,                     AlsuWritePbGrpReq               alsuWritePbGrpReq )
    SIG_DEF( SIG_ALSU_WRITE_PB_GRP_CNF,                     AlsuWritePbGrpCnf               alsuWritePbGrpCnf )
    SIG_DEF( SIG_ALSU_DELETE_PB_GRP_REQ,                    AlsuDeletePbGrpReq              alsuDeletePbGrpReq )
    SIG_DEF( SIG_ALSU_DELETE_PB_GRP_CNF,                    AlsuDeletePbGrpCnf              alsuDeletePbGrpCnf )
    SIG_DEF( SIG_ALSU_READ_PB_HIDDENKEY_REQ,                AlsuReadPbHiddenKeyReq          alsuReadPbHiddenKeyReq )
    SIG_DEF( SIG_ALSU_READ_PB_HIDDENKEY_CNF,                AlsuReadPbHiddenKeyCnf          alsuReadPbHiddenKeyCnf )
    SIG_DEF( SIG_ALSU_PHONEBOOK_STATUS_REQ,                 AlsuPhoneBookStatusReq          alsuPhoneBookStatusReq )
    SIG_DEF( SIG_ALSU_PHONEBOOK_STATUS_CNF,                 AlsuPhoneBookStatusCnf          alsuPhoneBookStatusCnf )
    SIG_DEF( SIG_ALSU_HIDDENKEY_FUNCTION_REQ,               AlsuHiddenKeyFunctionReq        alsuHiddenKeyFunctionReq )
    SIG_DEF( SIG_ALSU_HIDDENKEY_FUNCTION_CNF,               AlsuHiddenKeyFunctionCnf        alsuHiddenKeyFunctionCnf )
    SIG_DEF( SIG_ALSU_LIST_PB_DIALNUM_EXT_REQ,              AlsuListPbDialNumExtReq         alsuListPbDialNumExtReq )
    SIG_DEF( SIG_ALSU_LIST_PB_DIALNUM_EXT_CNF,              AlsuListPbDialNumExtCnf         alsuListPbDialNumExtCnf )
    SIG_DEF( SIG_ALSU_READ_PB_UID_REQ,                      AlsuReadPbUidReq                alsuReadPbUidReq )
    SIG_DEF( SIG_ALSU_READ_PB_UID_CNF,                      AlsuReadPbUidCnf                alsuReadPbUidCnf )
    SIG_DEF( SIG_ALSU_READ_PB_SYNC_INFO_REQ,                AlsuReadPbSyncInfoReq           alsuReadPbSyncInfoReq )
    SIG_DEF( SIG_ALSU_READ_PB_SYNC_INFO_CNF,                AlsuReadPbSyncInfoCnf           alsuReadPbSyncInfoCnf )
    SIG_DEF( SIG_ALSU_PB_SYNC_INFO_CHANGE_IND,              AlsuPbSyncInfoChangeInd         alsuPbSyncInfoChangeInd )
    SIG_DEF( SIG_ALSU_PB_ANR_CHANGED_IND,                   AlsuPbAnrChangedInd             alsuPbAnrChangedInd )
    SIG_DEF( SIG_ALSU_PB_EXT_EMAIL_REQ,                     AlsuPbExtEmailReq               alsuPbExtEmailReq )
    SIG_DEF( SIG_ALSU_PB_EXT_EMAIL_CNF,                     AlsuPbExtEmailCnf               alsuPbExtEmailCnf )
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
# if !defined (EXCLUDE_ALSA)
    /*
    ** SIM Application Toolkit signals to/from Application layer
    */
    SIG_DEF( SIG_ALSA_DUMMY = ALSA_SIGNAL_BASE, EmptySignal              alsa_dummy)
    SIG_DEF( SIG_ALSA_DISPLAY_TEXT_IND,         AlsaDisplayTextInd       alsaDisplayTextInd)
    SIG_DEF( SIG_ALSA_DISPLAY_TEXT_RSP,         AlsaDisplayTextRsp       alsaDisplayTextRsp)
    SIG_DEF( SIG_ALSA_GET_INKEY_IND,            AlsaGetInkeyInd          alsaGetInkeyInd)
    SIG_DEF( SIG_ALSA_GET_INKEY_RSP,            AlsaGetInkeyRsp          alsaGetInkeyRsp)
    SIG_DEF( SIG_ALSA_GET_INPUT_IND,            AlsaGetInputInd          alsaGetInputInd)
    SIG_DEF( SIG_ALSA_GET_INPUT_RSP,            AlsaGetInputRsp          alsaGetInputRsp)
    SIG_DEF( SIG_ALSA_PLAY_TONE_IND,            AlsaPlayToneInd          alsaPlayToneInd)
    SIG_DEF( SIG_ALSA_PLAY_TONE_RSP,            AlsaPlayToneRsp          alsaPlayToneRsp)
    SIG_DEF( SIG_ALSA_SET_UP_MENU_IND,          AlsaSetUpMenuInd         alsaSetUpMenuInd)
    SIG_DEF( SIG_ALSA_SET_UP_MENU_RSP,          AlsaSetUpMenuRsp         alsaSetUpMenuRsp)
    SIG_DEF( SIG_ALSA_SELECT_ITEM_IND,          AlsaSelectItemInd        alsaSelectItemInd)
    SIG_DEF( SIG_ALSA_SELECT_ITEM_RSP,          AlsaSelectItemRsp        alsaSelectItemRsp)
    SIG_DEF( SIG_ALSA_SEND_SM_IND,              AlsaSendSmInd            alsaSendSmInd)
    SIG_DEF( SIG_ALSA_SEND_SM_RSP,              AlsaSendSmRsp            alsaSendSmRsp)
    SIG_DEF( SIG_ALSA_SET_UP_CALL_IND,          AlsaSetUpCallInd         alsaSetUpCallInd)
    SIG_DEF( SIG_ALSA_SET_UP_CALL_RSP,          AlsaSetUpCallRsp         alsaSetUpCallRsp)
    SIG_DEF( SIG_ALSA_REFRESH_IND,              AlsaRefreshInd           alsaRefreshInd)
    SIG_DEF( SIG_ALSA_REFRESH_RSP,              AlsaRefreshRsp           alsaRefreshRsp)
    SIG_DEF( SIG_ALSA_PROVIDE_LOCAL_INFO_IND,   AlsaProvideLocalInfoInd  alsaProvideLocalInfoInd)
    SIG_DEF( SIG_ALSA_PROVIDE_LOCAL_INFO_RSP,   AlsaProvideLocalInfoRsp  alsaProvideLocalInfoRsp)
    SIG_DEF( SIG_ALSA_SMS_PP_DOWNLOAD_REQ,      AlsaSmsPpDownloadReq     alsaSmsPpDownloadReq)
    SIG_DEF( SIG_ALSA_SMS_PP_DOWNLOAD_CNF,      AlsaSmsPpDownloadCnf     alsaSmsPpDownloadCnf)
    SIG_DEF( SIG_ALSA_MENU_SELECTION_REQ,       AlsaMenuSelectionReq     alsaMenuSelectionReq)
    SIG_DEF( SIG_ALSA_MENU_SELECTION_CNF,       AlsaMenuSelectionCnf     alsaMenuSelectionCnf)
    /* signals for call control implementation */
    SIG_DEF( SIG_ALSA_CC_CALL_SET_UP_REQ,       AlsaCcCallSetUpReq       alsaCcCallSetUpReq)
    SIG_DEF( SIG_ALSA_CC_CALL_SET_UP_CNF,       AlsaCcCallSetUpCnf       alsaCcCallSetUpCnf)
    SIG_DEF( SIG_ALSA_CC_PDP_CONTEXT_REQ,       AlsaCcPdpContextReq      alsaCcPdpContextReq)
    SIG_DEF( SIG_ALSA_CC_PDP_CONTEXT_CNF,       AlsaCcPdpContextCnf      alsaCcPdpContextCnf)
    SIG_DEF( SIG_ALSA_CC_PDN_CONTEXT_REQ,       AlsaCcPdnContextReq      alsaCcPdnContextReq)
    SIG_DEF( SIG_ALSA_CC_PDN_CONTEXT_CNF,       AlsaCcPdnContextCnf      alsaCcPdnContextCnf)
    /*
    ** This signal is only used internally by the SIM Manager. No other task
    ** should send this request/response to the SIM Manager.
    */
    SIG_DEF( SIG_ALSA_INTERNAL_FETCH_REQ,       EmptySignal              alsaInternalFetchReq)
    SIG_DEF( SIG_ALSA_INTERNAL_STOP_TIMERS_IND, AlsaInternalStopTimersInd alsaInternalStopTimersInd)
    /*
    ** This signal is sent to the Application Layer when a FETCH command
    ** fails it indicates the reasons for the failure.
    */
    SIG_DEF( SIG_ALSA_FETCH_FAIL_IND,           AlsaFetchFailInd         alsaFetchFailInd)

    SIG_DEF( SIG_ALSA_PROACTIVE_SESSION_END_IND,EmptySignal              alsaProactiveSessionEndInd)
    /* New signals for SIM Application Toolkit Release 97 implementation */
    SIG_DEF( SIG_ALSA_SET_UP_EVENT_LIST_IND,    AlsaSetUpEventListInd    alsaSetUpEventListInd)
    SIG_DEF( SIG_ALSA_SET_UP_EVENT_LIST_RSP,    AlsaSetUpEventListRsp    alsaSetUpEventListRsp)
    SIG_DEF( SIG_ALSA_EVENT_DOWNLOAD_REQ,       AlsaEventDownloadReq     alsaEventDownloadReq)
    SIG_DEF( SIG_ALSA_EVENT_DOWNLOAD_CNF,       AlsaEventDownloadCnf     alsaEventDownloadCnf)
    SIG_DEF( SIG_ALSA_MOSM_CONTROL_REQ,         AlsaMoSmControlReq       alsaMoSmControlReq)
    SIG_DEF( SIG_ALSA_MOSM_CONTROL_CNF,         AlsaMoSmControlCnf       alsaMoSmControlCnf)
    /* job 103179 : indicate MM-IDLE mode */
    SIG_DEF( SIG_ALSA_MM_MODE_IND,              AlsaMmModeInd            alsaMmModeInd)
    SIG_DEF( SIG_ALSA_SEND_TERM_PROFILE_REQ,    AlsaSendTermProfileReq   alsaSendTermProfileReq)
    SIG_DEF( SIG_ALSA_SEND_TERM_PROFILE_CNF,    AlsaSendTermProfileCnf   alsaSendTermProfileCnf)
   /* New signals for SIM Application Toolkit Release 98 implementation */
    SIG_DEF( SIG_ALSA_TIMER_MANAGEMENT_IND,     AlsaTimerManagementInd     alsaTimerManagementInd)
    SIG_DEF( SIG_ALSA_TIMER_MANAGEMENT_RSP,     AlsaTimerManagementRsp     alsaTimerManagementRsp)
    SIG_DEF( SIG_ALSA_SET_UP_IDLE_MODE_TEXT_IND,AlsaSetUpIdleModeTextInd   alsaSetUpIdleModeTextInd)
    SIG_DEF( SIG_ALSA_SET_UP_IDLE_MODE_TEXT_RSP,AlsaSetUpIdleModeTextRsp   alsaSetUpIdleModeTextRsp)
    SIG_DEF( SIG_ALSA_RUN_AT_COMMAND_IND,       AlsaRunAtCommandInd        alsaRunAtCommandInd)
    SIG_DEF( SIG_ALSA_RUN_AT_COMMAND_RSP,       AlsaRunAtCommandRsp        alsaRunAtCommandRsp)
    SIG_DEF( SIG_ALSA_LANGUAGE_NOTIFICATION_IND,AlsaLanguageNotificationInd alsaLanguageNotificationInd)
    SIG_DEF( SIG_ALSA_LANGUAGE_NOTIFICATION_RSP,AlsaLanguageNotificationRsp alsaLanguageNotificationRsp)
    SIG_DEF( SIG_ALSA_TIMER_EXPIRATION_REQ,     AlsaTimerExpirationReq     alsaTimerExpirationReq)
    SIG_DEF( SIG_ALSA_TIMER_EXPIRATION_CNF,     AlsaTimerExpirationCnf     alsaTimerExpirationCnf)
    SIG_DEF( SIG_ALSA_READ_IMAGE_REC_REQ,       AlsaReadImageRecReq        alsaReadImageRecReq)       /* Icon related signal */
    SIG_DEF( SIG_ALSA_READ_IMAGE_REC_CNF,       AlsaReadImageRecCnf        alsaReadImageRecCnf)       /* Icon related signal */
    SIG_DEF( SIG_ALSA_READ_IMAGE_INST_DATA_REQ, AlsaReadImageInstDataReq   alsaReadImageInstDataReq)  /* Icon related signal */
    SIG_DEF( SIG_ALSA_READ_IMAGE_INST_DATA_CNF, AlsaReadImageInstDataCnf   alsaReadImageInstDataCnf)  /* Icon related signal */
    SIG_DEF( SIG_ALSA_LIST_IMAGE_REC_REQ,       AlsaListImageRecReq        alsaListImageRecReq)       /* Icon related signal */
    SIG_DEF( SIG_ALSA_LIST_IMAGE_REC_CNF,       AlsaListImageRecCnf        alsaListImageRecCnf)       /* Icon related signal */

   /* New signals for SIM Application Toolkit Release 99 implementation */
    SIG_DEF( SIG_ALSA_LAUNCH_BROWSER_IND,       AlsaLaunchBrowserInd       alsaLaunchBrowserInd)
    SIG_DEF( SIG_ALSA_LAUNCH_BROWSER_RSP,       AlsaLaunchBrowserRsp       alsaLaunchBrowserRsp)
    SIG_DEF( SIG_ALSA_OPEN_CHANNEL_IND,         AlsaOpenChannelInd         alsaOpenChannelInd)
    SIG_DEF( SIG_ALSA_OPEN_CHANNEL_RSP,         AlsaOpenChannelRsp         alsaOpenChannelRsp)
    SIG_DEF( SIG_ALSA_CLOSE_CHANNEL_IND,        AlsaCloseChannelInd        alsaCloseChannelInd)
    SIG_DEF( SIG_ALSA_CLOSE_CHANNEL_RSP,        AlsaCloseChannelRsp        alsaCloseChannelRsp)
    SIG_DEF( SIG_ALSA_RECEIVE_DATA_IND,         AlsaReceiveDataInd         alsaReceiveDataInd)
    SIG_DEF( SIG_ALSA_RECEIVE_DATA_RSP,         AlsaReceiveDataRsp         alsaReceiveDataRsp)
    SIG_DEF( SIG_ALSA_SEND_DATA_IND,            AlsaSendDataInd            alsaSendDataInd)
    SIG_DEF( SIG_ALSA_SEND_DATA_RSP,            AlsaSendDataRsp            alsaSendDataRsp)
    SIG_DEF( SIG_ALSA_GET_CHANNEL_STATUS_IND,   AlsaGetChannelStatusInd    alsaGetChannelStatusInd)
    SIG_DEF( SIG_ALSA_GET_CHANNEL_STATUS_RSP,   AlsaGetChannelStatusRsp    alsaGetChannelStatusRsp)
    SIG_DEF( SIG_ALSA_FAKE_SETUP_MENU_IND,      EmptySignal                alsaFakeSetupMenuInd)

# endif

#ifdef FEA_LPP_SUPPORT
# if !defined (EXCLUDE_LPP)
    SIG_DEF( SIG_LPP_DUMMY = LPP_SIGNAL_BASE                    ,    EmptySignal                                 lpp_dummy)
	SIG_DEF( SIG_LPP_LBS_SESSION_END_REQ                        ,    EmptySignal                                 lppLbsSessionEndReq)
	SIG_DEF( SIG_LPP_LBS_CAPABILITY_UPDATE_REQ                  ,    LppLbsCapabilityUpdateReq                   lppLbsCapUpdateReq)

#ifdef __AGNSS_SUPPORT__
	//SIG_DEF( SIG_LPP_LBS_RESET_UE_STORED_POS_INFO_REQ         ,    LppLbsResetUeStoredPosInfoReq               lppLbsResetUeStoredPosInfoReq)
	SIG_DEF( SIG_LCSP_GNSS_COMMON_ASSIST_DATA_CNF               ,    LcspGnssCommonAssistDataCnf                 lcspGnssCommonAssistDataCnf)
	SIG_DEF( SIG_LCSP_GNSS_GENERIC_ASSIST_DATA_CNF              ,    LcspGnssGenericAssistDataCnf                lcspGnssGenericAssistDataCnf)
	SIG_DEF( SIG_LCSP_GNSS_POS_CNF                              ,    LcspGnssPosCnf                              lcspGnssPosCnf)
	SIG_DEF( SIG_LCSP_GNSS_MEAS_CNF                             ,    LcspGnssMeasCnf                             lcspGnssMeasCnf)
	SIG_DEF( SIG_LCSP_GNSS_ADDITIONAL_ASSIST_DATA_REQ           ,    LcspGnssAdditionalAssistDataReq             lcspGnssAdditionalAssistDataReq)
#endif
	//SIG_DEF( SIG_LPP_PEER_OTDOA_MEAS_REQ                      ,    LppPeerOtdoaMeasReq                         lppPeerOtdoaMeasReq)
	SIG_DEF( SIG_LPP_PEER_OTDOA_MEAS_REQ                        ,    LppPeerOtdoaMeasReq                         lpp_peer_otdoa_meas_req)
	//SIG_DEF( SIG_LPP_PEER_OTDOA_ASSIST_DATA_REQ               ,    LppPeerOtdoaAssistDataReq                   lppPeerOtdoaAssistDataReq)
	SIG_DEF( SIG_LPP_PEER_OTDOA_ASSIST_DATA_REQ                 ,    LppPeerOtdoaAssistDataReq                   lpp_peer_otdoa_assist_data_req)
	//SIG_DEF( SIG_LPP_PEER_OTDOA_MEAS_ABORT_REQ                ,    LppPeerOtdoaMeasAbortReq                    lppPeerOtdoaMeasAbortReq)
	SIG_DEF( SIG_LPP_PEER_OTDOA_MEAS_ABORT_REQ                  ,    LppPeerOtdoaMeasAbortReq                    lpp_peer_otdoa_meas_abort_req)
	SIG_DEF( SIG_LPP_PEER_EUTRAN_BAND_UPDATE_REQ                ,    EmptySignal                                 lppPeerEutranBandUpdateReq)
	SIG_DEF( SIG_LPP_PEER_OTDOA_RESET_UE_STORED_POS_REQ         ,    EmptySignal                                 lppPeerOtdoaResetUeStoredPosReq)
	//SIG_DEF( SIG_LPP_PEER_OTDOA_MEAS_CNF                      ,    LppPeerOtdoaMeasCnf                         lppPeerOtdoaMeasCnf)
	SIG_DEF( SIG_LPP_PEER_OTDOA_MEAS_CNF                        ,    LppPeerOtdoaMeasCnf                         lpp_peer_otdoa_meas_cnf)
	SIG_DEF( SIG_LPP_PEER_OTDOA_ASSIST_DATA_CNF                 ,    LppPeerOtdoaAssistDataCnf                   lppPeerOtdoaAssitDataCnf)
	//SIG_DEF( SIG_LPP_PEER_OTDOA_ADDITIONAL_ASSIST_DATA_REQ    ,    LppPeerOtdoaAdditionalAssistDataReq         lppPeerOtdoaAddAssistDataReq)
	SIG_DEF( SIG_LPP_PEER_OTDOA_ADDITIONAL_ASSIST_DATA_REQ      ,    LppPeerOtdoaAdditionalAssistDataReq         lpp_peer_otdoa_additional_assist_data_req)
	//SIG_DEF( SIG_LPP_PEER_EUTRAN_BAND_UPDATE_CNF              ,    LppPeerEutranBandUpdateCnf                  lppPeerEutranBandUpdateCnf)
	SIG_DEF( SIG_LPP_PEER_EUTRAN_BAND_UPDATE_CNF                ,    LppPeerEutranBandUpdateCnf                  lpp_peer_eutran_band_update_cnf)
	//SIG_DEF( SIG_LPP_PEER_ECID_MEAS_REQ                       ,    LppPeerEcidMeasReq                          lppPeerEcidMeasReq)
	SIG_DEF( SIG_LPP_PEER_ECID_MEAS_REQ                         ,    LppPeerEcidMeasReq                          lpp_peer_ecid_meas_req)
	//SIG_DEF( SIG_LPP_PEER_ECID_MEAS_CNF                       ,    LppPeerEcidMeasCnf                          lppPeerEcidMeasCnf)
	SIG_DEF( SIG_LPP_PEER_ECID_MEAS_CNF                         ,    LppPeerEcidMeasCnf                          lpp_peer_ecid_meas_cnf)
	//SIG_DEF( SIG_LPP_PEER_ECID_MEAS_ABORT_REQ                 ,    LppPeerEcidMeasAbortReq                     lppPeerEcidMeasAbortReq)
	SIG_DEF( SIG_LPP_PEER_ECID_MEAS_ABORT_REQ                   ,    LppPeerEcidMeasAbortReq                     lpp_peer_ecid_meas_abort_req)
	SIG_DEF( SIG_LPP_SUPL_PDU_DATA_IND                          ,    EmptySignal                                 lppSuplPduDataInd)
	SIG_DEF( SIG_LPP_SUPL_PDU_DATA_CNF                          ,    EmptySignal                                 lppSuplPduDataCnf)
	SIG_DEF( SIG_LCSP_START_REQ                                 ,    EmptySignal                                 lppLcspStartReq)
	SIG_DEF( SIG_LCSP_END_REQ                                   ,    EmptySignal                                 lppLcspEndReq)
    SIG_DEF( SIG_ERRC_NBR_CELL_INFO_REG_CNF                     ,    L4cNbrCellInfoRegCnf                        l4c_nbr_cell_info_reg_cnf)
    SIG_DEF( SIG_EMM_LPP_DATA_IND                               ,    LppEvalDataInd                              lpp_eval_data_ind)
    SIG_DEF( SIG_EMM_LPP_DATA_CNF                               ,    LppEvalDataCnf                              lpp_eval_data_cnf)
    SIG_DEF( SIG_EVAL_LPP_NW_RESET_UE_STORED_POS_INFO_IND       ,    EvalLppNwResetUeStoredPosInfoInd            eval_lpp_nw_reset_ue_stored_pos_info_ind)
    SIG_DEF( SIG_LPP_LBS_RESET_UE_STORED_POS_INFO_REQ           ,    LppLbsResetUeStoredPosInfoReq               lpp_lbs_reset_ue_stored_pos_info_req)
    //SIG_DEF( SIG_ERRC_NBR_CELL_INFO_DEREG_CNF                 ,    )
    SIG_DEF( SIG_ERRC_NBR_CELL_INFO_IND                         ,    L4cNbrCellInfoInd                           l4c_nbr_cell_info_ind)
    SIG_DEF( SIG_LPP_EL1_OTDOA_MEAS_CNF                         ,    LppEl1OtdoaMeasCnf                          lpp_el1_otdoa_meas_cnf)
    SIG_DEF( SIG_LPP_EL1_OTDOA_ASSIST_DATA_CNF                  ,    LppEl1OtdoaAssistDataCnf                    lpp_el1_otdoa_assist_data_cnf)
    SIG_DEF( SIG_LPP_EL1_OTDOA_ADDITIONAL_ASSIST_DATA_IND       ,    LppEl1OtdoaAdditionalAssistDataInd          lpp_el1_otdoa_additional_assist_data_ind)
    SIG_DEF( SIG_ERRC_NBR_CELL_INFO_DEREG_CNF                   ,    EmptySignal                                 lppErrcNbrCellInfoDeregCnf)
    SIG_DEF( SIG_LPP_LCS_DATA_REQ                               ,    SsLcsBeginReq                               lppLcsDataReq)
    SIG_DEF( SIG_LPP_LCS_DATA_IND                               ,    LppLcsDataInd                               lppLcsDataInd)
    SIG_DEF( SIG_LPP_SUPL_DATA_IND                              ,    LppSuplDataInd                              lppSuplDataInd)
    SIG_DEF( SIG_LPP_SUPL_DATA_REQ                              ,    LppSuplDataReq                              lppSuplDataReq)
#endif

# if !defined (EXCLUDE_LBS)
    SIG_DEF( SIG_LBS_DUMMY = LBS_SIGNAL_BASE                      ,    EmptySignal                                 lbs_dummy)
    SIG_DEF( SIG_LPP_LBS_SESSION_START_IND                        ,    EmptySignal                                 lppLbsSessionStartInd)
    SIG_DEF( SIG_LPP_LBS_SESSION_END_IND                          ,    EmptySignal                                 lppLbsSessionEndInd)
    SIG_DEF( SIG_LPP_LBS_POS_METHOD_IND                           ,    LppLbsPosMethodInd                          lppLbsPosMethodInd)
	SIG_DEF( SIG_LPP_LBS_PEER_OTDOA_MEAS_CNF                      ,    LppPeerOtdoaMeasCnf                         lppLbsPeerOtdoaMeasCnf)
	SIG_DEF( SIG_LPP_LBS_PEER_OTDOA_ASSIST_DATA_CNF               ,    LppPeerOtdoaAssistDataCnf                   lppLbsPeerOtdoaAssistDataCnf)
	SIG_DEF( SIG_LPP_LBS_PEER_OTDOA_ADDITIONAL_ASSIST_DATA_REQ    ,    LppPeerOtdoaAdditionalAssistDataReq         lppLbsPeerOtdoaAddAssistDataReq)
	SIG_DEF( SIG_LPP_LBS_PEER_ECID_MEAS_CNF                       ,    LppPeerEcidMeasCnf                          lppLbsPeerEcidMeasCnf)
	SIG_DEF( SIG_LPP_LBS_PEER_EUTRAN_BAND_UPDATE_CNF              ,    LppPeerEutranBandUpdateCnf                  lppLbsPeerEutranBandUpdate)
    SIG_DEF( SIG_MUX_LBS_DATA_REQ                                 ,    MuxLbsDataReq                               muxLbsDataReq)
#endif
#endif

#if !defined (EXCLUDE_TEST)
    /*
    ** Test signals
    */
    SIG_DEF( SIG_TEST_DUMMY = TEST_SIGNAL_BASE,    EmptySignal                 test_dummy)
    SIG_DEF( SIG_SHUTDOWN,                         EmptySignal                 shutdown)
    SIG_DEF( SIG_DUMMY,                            Int8                        dummy[LARGEST_SIGNAL_SIZE])

# if !defined (EXCLUDE_GSM_PS)
    SIG_DEF( SIG_TEST_CC,                          CcTestSignal                testCc)
    SIG_DEF( SIG_TEST_SM,                          TestSmcm                    testSmcm)
    SIG_DEF( SIG_TEST_MM,                          MmTestReq                   mmTestReq)
    SIG_DEF( SIG_TEST_RR,                          RrTestSignal                testRr)
    SIG_DEF( SIG_TEST_SMRL,                        TestSmrl                    testSmrl)
    SIG_DEF( SIG_TEST_SMTL,                        TestSmtl                    testSmtl)
    SIG_DEF( SIG_TEST_CB,                          CbTestSignal                testCb)
# endif
#endif /* !defined (EXCLUDE_TEST) */


/* END OF FILE */
