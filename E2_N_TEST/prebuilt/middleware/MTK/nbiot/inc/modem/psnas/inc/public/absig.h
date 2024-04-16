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
 *      Application Background Layer Signal Definitions
 *      The BL 1/2 of the original alsig.h
 ************************************************************************
**/

/*========================================================================*/
/* APEX:  AL BL Cell Broadcast Procedure                                  */

    SIG_DEF( SIG_APEX_CB_DUMMY = ABCB_SIGNAL_BASE,         EmptySignal             dummyCb)
/* TODO remove if possible, used only in UTs? */
    SIG_DEF( SIG_APEX_CB_READY_IND,                        ApexCbReadyInd          apexCbReadyInd)

/*========================================================================*/
/* APEX:  AL BL Call Control Procedure                                    */

/* TODO remove if possible */
    SIG_DEF( SIG_APEX_CC_DUMMY = ABCC_SIGNAL_BASE,         EmptySignal             dummyCc)
    SIG_DEF( SIG_APEX_CC_CALL_STATUS_REQ,                  ApexCcCallStatusReq     apexCcCallStatusReq)
    SIG_DEF( SIG_APEX_CC_GENERAL_CONFIG_REQ,               ApexCcGeneralConfigReq  apexCcGeneralConfigReq)

/*========================================================================*/
/* APEX:  AL BL General Library Procedure                                 */

    SIG_DEF( SIG_APEXGL_DUMMY = ABGL_SIGNAL_BASE,          EmptySignal                    dummyGl)
    SIG_DEF( SIG_APEXGL_ERROR_IND,                         ApexGlErrorInd                 apexGlErrorInd)
    SIG_DEF( SIG_APEXGL_READY_IND,                         ApexGlReadyInd                 apexGlReadyInd)
    SIG_DEF( SIG_APEXGL_READ_FEATURE_CONFIG_REQ,           ApexGlReadFeatureConfigReq     apexGlReadFeatureConfigReq)
    SIG_DEF( SIG_APEXGL_READ_FEATURE_CONFIG_CNF,           ApexGlReadFeatureConfigCnf     apexGlReadFeatureConfigCnf)
    SIG_DEF( SIG_APEXGL_WRITE_FEATURE_CONFIG_REQ,          ApexGlWriteFeatureConfigReq    apexGlWriteFeatureConfigReq)
    SIG_DEF( SIG_APEXGL_WRITE_FEATURE_CONFIG_CNF,          ApexGlWriteFeatureConfigCnf    apexGlWriteFeatureConfigCnf)
    SIG_DEF( SIG_APEXGL_READ_SUPPORTED_CODECS_REQ,         ApexGlReadSupportedCodecsReq   apexGlReadSupportedCodecsReq)
    SIG_DEF( SIG_APEXGL_READ_SUPPORTED_CODECS_CNF,         ApexGlReadSupportedCodecsCnf   apexGlReadSupportedCodecsCnf)
    SIG_DEF( SIG_APEXGL_WRITE_SUPPORTED_CODECS_REQ,        ApexGlWriteSupportedCodecsReq  apexGlWriteSupportedCodecsReq)
    SIG_DEF( SIG_APEXGL_WRITE_SUPPORTED_CODECS_CNF,        ApexGlWriteSupportedCodecsCnf  apexGlWriteSupportedCodecsCnf)
    SIG_DEF( SIG_APEXGL_SET_SUPPORTED_CODECS_REQ,          ApexGlSetSupportedCodecsReq    apexGlSetSupportedCodecsReq)
    SIG_DEF( SIG_APEXGL_SET_SUPPORTED_CODECS_CNF,          ApexGlSetSupportedCodecsCnf    apexGlSetSupportedCodecsCnf)
/*========================================================================*/
/* APEX:  AL BL Mobility Management Procedure                             */

    SIG_DEF( SIG_APEX_MM_DUMMY = ABMM_SIGNAL_BASE,         EmptySignal                dummyMm)
    SIG_DEF( SIG_APEX_MM_NETWORK_STATE_IND,                ApexMmNetworkStateInd      apexMmNetworkStateInd)
    SIG_DEF( SIG_APEX_MM_RSSI_IND,                         ApexMmRssiInd              apexMmRssiInd)
    SIG_DEF( SIG_APEX_MM_PLMNLIST_REQ,                     ApexMmPlmnListReq          apexMmPlmnListReq)
    SIG_DEF( SIG_APEX_MM_PLMNLIST_CNF,                     ApexMmPlmnListCnf          apexMmPlmnListCnf)
    SIG_DEF( SIG_APEX_MM_PLMN_SELECT_REQ,                  ApexMmPlmnSelectReq        apexMmPlmnSelectReq)
    SIG_DEF( SIG_APEX_MM_PLMN_SELECT_CNF,                  ApexMmPlmnSelectCnf        apexMmPlmnSelectCnf)
    SIG_DEF( SIG_APEX_MM_READ_PLMN_SEL_REQ,                ApexMmReadPlmnSelReq       apexMmReadPlmnSelReq)
    SIG_DEF( SIG_APEX_MM_READ_PLMN_SEL_CNF,                ApexMmReadPlmnSelCnf       apexMmReadPlmnSelCnf)
    SIG_DEF( SIG_APEX_MM_WRITE_PLMN_SEL_REQ,               ApexMmWritePlmnSelReq      apexMmWritePlmnSelReq)
    SIG_DEF( SIG_APEX_MM_WRITE_PLMN_SEL_CNF,               ApexMmWritePlmnSelCnf      apexMmWritePlmnSelCnf)
    SIG_DEF( SIG_APEX_MM_PLMN_LIST_STATS_REQ,              ApexMmPlmnListStatsReq     apexMmPlmnListStatsReq)
    SIG_DEF( SIG_APEX_MM_PLMN_LIST_STATS_CNF,              ApexMmPlmnListStatsCnf     apexMmPlmnListStatsCnf)
    SIG_DEF( SIG_APEX_MM_BAND_IND,                         ApexMmBandInd              apexMmBandInd)
    SIG_DEF( SIG_APEX_MM_READ_BAND_MODE_REQ,               ApexMmReadBandModeReq      apexMmReadBandModeReq)
    SIG_DEF( SIG_APEX_MM_READ_BAND_MODE_CNF,               ApexMmReadBandModeCnf      apexMmReadBandModeCnf)
    SIG_DEF( SIG_APEX_MM_ABORT_PLMNLIST_REQ,               ApexMmAbortPlmnListReq     apexMmAbortPlmnListReq)
    SIG_DEF( SIG_APEX_MM_ABORT_PLMNLIST_CNF,               ApexMmAbortPlmnListCnf     apexMmAbortPlmnListCnf)
    SIG_DEF( SIG_APEX_MM_LOCK_ARFCN_REQ,                   ApexMmLockArfcnReq         apexMmLockArfcnReq)
    SIG_DEF( SIG_APEX_MM_LOCK_ARFCN_CNF,                   ApexMmLockArfcnCnf         apexMmLockArfcnCnf)
    SIG_DEF( SIG_APEX_MM_UE_STATS_REQ,                     ApexMmUeStatsReq           apexMmUeStatsReq)
    SIG_DEF( SIG_APEX_MM_UE_STATS_CNF,                     ApexMmUeStatsCnf           apexMmUeStatsCnf)    
    SIG_DEF( SIG_APEX_MM_LOC_CELL_INFO_REQ,                ApexMmLocCellInfoReq       apexMmLocCellInfoReq)
    SIG_DEF( SIG_APEX_MM_LOC_CELL_INFO_CNF,                ApexMmLocCellInfoCnf       apexMmLocCellInfoCnf)
    SIG_DEF( SIG_APEX_MM_CSCON_REQ,                        ApexMmCsconReq             apexMmCsconReq)
    SIG_DEF( SIG_APEX_MM_CSCON_CNF,                        ApexMmCsconCnf             apexMmCsconCnf)
    SIG_DEF( SIG_APEX_MM_CSCON_IND,                        ApexMmCsconInd             apexMmCsconInd)

    SIG_DEF( SIG_ABMM_POWERED_DOWN_SIG,                    EmptySignal                abmmPoweredDownSig) /* FR9803-2164 */
    SIG_DEF( SIG_APEX_MM_SUSPEND_REQ,                      ApexMmSuspendReq           apexMmSuspendReq)
    SIG_DEF( SIG_APEX_MM_SUSPEND_CNF,                      ApexMmSuspendCnf           apexMmSuspendCnf)
    SIG_DEF( SIG_APEX_MM_RESUME_REQ,                       ApexMmResumeReq            apexMmResumeReq)
    SIG_DEF( SIG_APEX_MM_RESUME_CNF,                       ApexMmResumeCnf            apexMmResumeCnf)

    /* added for 0204-17315  */
    SIG_DEF( SIG_APEX_MM_DEREGISTER_REQ,                   ApexMmDeregisterReq        apexMmDeregisterReq)
    SIG_DEF( SIG_APEX_MM_DEREGISTER_CNF,                   ApexMmDeregisterCnf        apexMmDeregisterCnf)

    SIG_DEF( SIG_ABMM_RESUME_REQ,                          AbmmResumeReq              abmmResumeReq)
    SIG_DEF( SIG_ABMM_RESUME_CNF,                          AbmmResumeCnf              abmmResumeCnf)

    SIG_DEF( SIG_APEX_MM_NETWORK_INFO_IND,                 ApexMmNetworkInfoInd       apexMmNetworkInfoInd)
    SIG_DEF( SIG_APEX_MM_CIPHER_IND,                       ApexMmCipherInd            apexMmCipherInd)
    SIG_DEF( SIG_APEX_MM_PLMN_SELECT_IND,                  ApexMmPlmnSelectInd        apexMmPlmnSelectInd)

    SIG_DEF( SIG_APEX_MM_WRITE_PWON_OPTIONS_REQ,           ApexMmWritePwonOptionsReq  apexMmWritePwonOptionsReq)
    SIG_DEF( SIG_APEX_MM_WRITE_PWON_OPTIONS_CNF,           ApexMmWritePwonOptionsCnf  apexMmWritePwonOptionsCnf)
    SIG_DEF( SIG_APEX_MM_READ_PWON_OPTIONS_REQ,            ApexMmReadPwonOptionsReq   apexMmReadPwonOptionsReq)
    SIG_DEF( SIG_APEX_MM_READ_PWON_OPTIONS_CNF,            ApexMmReadPwonOptionsCnf   apexMmReadPwonOptionsCnf)

    SIG_DEF( SIG_ABMM_DEBUG_INFO_REQ     ,                 AbmmDebugInfoReq           abmmDebugInfoReq)
    SIG_DEF( SIG_ABMM_DEBUG_INFO_CNF     ,                 AbmmDebugInfoCnf           abmmDebugInfoCnf)
#if defined (ENABLE_L23_DEBUG) || defined (ENABLE_ABMM_DEBUG)
    SIG_DEF (SIG_ABMM_REG_STATUS_DEBUG_INFO_IND,           AbmmRegStatusDebugInfoInd abmmRegStatusDebugInfoInd)
#endif /* ENABLE_L23_DEBUG || ENABLE_ABMM_DEBUG*/
#ifdef ABMM_UNIT_TEST
    /* ABMM signals for Unit Testing                                           */
    SIG_DEF( SIG_ABMM_SET_UNIT_TEST_MODE_REQ,              AbmmSetUnitTestModeReq    abmmSetUnitTestModeReq)
    SIG_DEF( SIG_ABMM_CHECK_UNIT_TEST_MODE_IND,            AbmmCheckUnitTestModeInd  abmmCheckUnitTestModeInd)
#endif /* ABMM_UNIT_TEST */

    SIG_DEF( SIG_APEX_MM_WRITE_MOBILE_ID_REQ,              ApexMmWriteMobileIdReq           apexMmWriteMobileIdReq)
    SIG_DEF( SIG_APEX_MM_WRITE_MOBILE_ID_CNF,              ApexMmWriteMobileIdCnf           apexMmWriteMobileIdCnf)
    SIG_DEF( SIG_APEX_MM_READ_MOBILE_ID_REQ,               ApexMmReadMobileIdReq            apexMmReadMobileIdReq)
    SIG_DEF( SIG_APEX_MM_READ_MOBILE_ID_CNF,               ApexMmReadMobileIdCnf            apexMmReadMobileIdCnf)
    SIG_DEF( SIG_APEX_MM_SEARCH_BAND_LIST_REQ,             ApexMmSearchBandListReq          apexMmSearchBandListReq)
    SIG_DEF( SIG_APEX_MM_SEARCH_BAND_LIST_CNF,             ApexMmSearchBandListCnf          apexMmSearchBandListCnf)

    SIG_DEF( SIG_APEX_MM_SET_EHPLMN_REQ,                   ApexMmSetEhplmnReq                  apexMmSetEhplmnReq)
    
    SIG_DEF( SIG_APEX_MM_DISABLE_HPLMN_SEARCH_REQ,         ApexMmDisableHplmnSearchReq         apexMmDisableHplmnSearchReq)
    SIG_DEF( SIG_APEX_MM_DISABLE_HPLMN_SEARCH_CNF,         ApexMmDisableHplmnSearchCnf         apexMmDisableHplmnSearchCnf)
    SIG_DEF( SIG_APEX_MM_SET_EDRX_REQ,                     ApexMmSetEdrxReq          apexMmSetEdrxReq)
    SIG_DEF( SIG_APEX_MM_SET_EDRX_CNF,                     ApexMmSetEdrxCnf          apexMmSetEdrxCnf)
    SIG_DEF( SIG_APEX_MM_READ_EDRX_REQ,                    ApexMmReadEdrxReq         apexMmReadEdrxReq)
    SIG_DEF( SIG_APEX_MM_READ_EDRX_CNF,                    ApexMmReadEdrxCnf         apexMmReadEdrxCnf)
    SIG_DEF( SIG_APEX_WRITE_IOT_OPT_CFG_REQ,               ApexWriteIotOptCfgReq     apexWriteIotOptCfgReq)
    SIG_DEF( SIG_APEX_WRITE_IOT_OPT_CFG_CNF,               ApexWriteIotOptCfgCnf     apexWriteIotOptCfgCnf)
    SIG_DEF( SIG_APEX_READ_IOT_OPT_CFG_REQ,                ApexReadIotOptCfgReq      apexReadIotOptCfgReq)
    SIG_DEF( SIG_APEX_READ_IOT_OPT_CFG_CNF,                ApexReadIotOptCfgCnf      apexReadIotOptCfgCnf)
    SIG_DEF( SIG_APEX_WRITE_PSM_CONF_REQ,                  ApexWritePsmConfReq       apexWritePsmConfReq)
    SIG_DEF( SIG_APEX_WRITE_PSM_CONF_CNF,                  ApexWritePsmConfCnf       apexWritePsmConfCnf)
    SIG_DEF( SIG_APEX_READ_PSM_CONF_REQ,                   ApexReadPsmConfReq        apexReadPsmConfReq)
    SIG_DEF( SIG_APEX_READ_PSM_CONF_CNF,                   ApexReadPsmConfCnf        apexReadPsmConfCnf)
    SIG_DEF( SIG_APEX_MM_PSM_STATUS_IND,                   ApexMmPsmStatusInd        apexMmPsmStatusInd)
    SIG_DEF( SIG_APEX_READ_UE_INFO_REQ,                    ApexReadUeInfoReq         apexReadUeInfoReq)
    SIG_DEF( SIG_APEX_READ_UE_INFO_CNF,                    ApexReadUeInfoCnf         apexReadUeInfoCnf)

#ifdef FEA_NFM
    SIG_DEF( SIG_APEX_READ_NFM_REQ,                        ApexReadNfmReq           apexReadNfmReq)
    SIG_DEF( SIG_APEX_READ_NFM_CNF,                        ApexReadNfmCnf           apexReadNfmCnf)
    SIG_DEF( SIG_APEX_READ_NFM_STATUS_REQ,                 ApexReadNfmStatusReq     apexReadNfmStatusReq)
    SIG_DEF( SIG_APEX_READ_NFM_STATUS_CNF,                 ApexReadNfmStatusCnf     apexReadNfmStatusCnf)
    SIG_DEF( SIG_APEX_WRITE_NFM_REQ,                       ApexWriteNfmReq          apexWriteNfmReq)
    SIG_DEF( SIG_APEX_WRITE_NFM_CNF,                       ApexWriteNfmCnf          apexWriteNfmCnf)
    SIG_DEF( SIG_APEX_WRITE_NFM_CONF_REQ,                  ApexWriteNfmConfReq      apexWriteNfmConfReq)
    SIG_DEF( SIG_APEX_WRITE_NFM_CONF_CNF,                  ApexWriteNfmConfCnf      apexWriteNfmConfCnf)
    SIG_DEF( SIG_APEX_READ_NFM_CONF_REQ,                   ApexReadNfmConfReq       apexReadNfmConfReq)
    SIG_DEF( SIG_APEX_READ_NFM_CONF_CNF,                   ApexReadNfmConfCnf       apexReadNfmConfCnf)
#endif /* FEA_NFM */


    SIG_DEF( SIG_APEX_MM_WRITE_ATTACH_PDN_CONF_REQ,        ApexMmWriteAttachPdnCfgReq   apexMmWriteAttachPdnCfgReq)
    SIG_DEF( SIG_APEX_MM_WRITE_ATTACH_PDN_CONF_CNF,        ApexMmWriteAttachPdnCfgCnf   apexMmWriteAttachPdnCfgCnf)
    SIG_DEF( SIG_APEX_MM_READ_ATTACH_PDN_CONF_REQ,         ApexMmReadAttachPdnCfgReq    apexMmReadAttachPdnCfgReq)
    SIG_DEF( SIG_APEX_MM_READ_ATTACH_PDN_CONF_CNF,         ApexMmReadAttachPdnCfgCnf    apexMmReadAttachPdnCfgCnf)
    /* Internal signal fo ABPD */
    SIG_DEF( SIG_ABMM_CURRENT_PLMN_IND,                    AbmmCurrentPlmnInd        abmmCurrentPlmnInd)
    SIG_DEF( SIG_APEX_MM_OOSA_STATUS_IND,                  ApexMmOosaStatusInd        apexMmOosaStatusInd)

/*========================================================================*/
/* APEX:  AL BL Power Management Procedure                                */
    SIG_DEF( SIG_APEX_PM_DUMMY = ABPM_SIGNAL_BASE,         EmptySignal             dummyPm)
    SIG_DEF( SIG_ABPM_POWER_DOWN_SIG,                      AbpmPowerDownSig        abpmPowerDownSig)
    SIG_DEF( SIG_APEX_PM_MODE_CHANGE_REQ,                  ApexPmModeChangeReq     apexPmModeChangeReq)
    SIG_DEF( SIG_APEX_PM_MODE_CHANGE_CNF,                  ApexPmModeChangeCnf     apexPmModeChangeCnf)

    SIG_DEF( SIG_APEX_PM_POWER_GOING_DOWN_IND,             EmptySignal             apexPmPowerGoingDownInd)
    SIG_DEF( SIG_APEX_PM_POWER_GOING_DOWN_RSP,             ApexPmPowerGoingDownRsp apexPmPowerGoingDownRsp)

/*========================================================================*/
/* APEX:  AL BL Shell                                                     */

    SIG_DEF( SIG_APEX_ALSI_GENERIC_CNF = ABSH_SIGNAL_BASE, ApexAlsiGenericCnf      apexAlsiGenericCnf)
    SIG_DEF( SIG_APEX_SH_REGISTER_TASK_REQ,                ApexShRegisterTaskReq   apexShRegisterTaskReq)
    SIG_DEF( SIG_APEX_SH_REGISTER_TASK_CNF,                ApexShRegisterTaskCnf   apexShRegisterTaskCnf)
    SIG_DEF( SIG_APEX_SH_CHANGE_CONTROL_REQ,               ApexShChangeControlReq  apexShChangeControlReq)
    SIG_DEF( SIG_APEX_SH_CHANGE_CONTROL_IND,               ApexShChangeControlInd  apexShChangeControlInd)
    SIG_DEF( SIG_APEX_SH_CHANGE_CONTROL_RSP,               ApexShChangeControlRsp  apexShChangeControlRsp)
    SIG_DEF( SIG_APEX_SH_CHANGE_CONTROL_CNF,               ApexShChangeControlCnf  apexShChangeControlCnf)
    SIG_DEF( SIG_APEX_SH_GOT_CONTROL_IND,                  ApexShGotControlInd     apexShGotControlInd)
    SIG_DEF( SIG_ABSH_TASK_REGISTERED_SIG,                 AbshTaskRegisteredSig   abshTaskRegisteredSig)
    SIG_DEF( SIG_ABSH_LOCAL_RESET_SIG,                     AbshLocalResetSig       abshLocalResetSig) /* FR9802-2076*/
    SIG_DEF( SIG_ABSH_POWERED_DOWN_SIG,                    EmptySignal             abshPoweredDownSig) /* FR9803-2164 */
#  if defined (NODEBSCRIPT_GKI_API) || defined (ENABLE_AB_UNIT_TEST)
    SIG_DEF( SIG_ABSH_NVRAM_OK_IND,                        EmptySignal             abshNvramOkInd)
    SIG_DEF( SIG_ABSH_NVRAM_OK_RSP,                        EmptySignal             abshNvramOkRSP)
#  endif
# if defined (ON_PC)
    SIG_DEF( SIG_ABSH_START_TIMER_SIG,                     AbshStartTimerSig       abshStartTimerSig)
    SIG_DEF( SIG_ABSH_STOP_TIMER_SIG,                      AbshStopTimerSig        abshStopTimerSig)
# endif


/*========================================================================*/
/* APEX:  AL BL SIM Support Procedure                                     */

    SIG_DEF( SIG_APEX_SIM_DUMMY = ABSI_SIGNAL_BASE,        EmptySignal             dummySim)
    SIG_DEF( SIG_APEX_SIM_GET_CHV_IND,                     ApexSimGetChvInd        apexSimGetChvInd)
    SIG_DEF( SIG_APEX_SIM_GET_CHV_RSP,                     ApexSimGetChvRsp        apexSimGetChvRsp)
    SIG_DEF( SIG_APEX_SIM_OK_IND,                          ApexSimOkInd            apexSimOkInd)
    SIG_DEF( SIG_ABSI_SIM_OK_SIG,                          AbsiSimOkSig            absiSimOkSig)
    SIG_DEF( SIG_APEX_SIM_NOK_IND,                         ApexSimNokInd           apexSimNokInd)
    SIG_DEF( SIG_ABSI_SIM_NOK_SIG,                         AbsiSimNokSig           absiSimNokSig)
    SIG_DEF( SIG_APEX_SIM_FAULT_IND,                       ApexSimFaultInd         apexSimFaultInd)
    SIG_DEF( SIG_APEX_SIM_CHV_FUNCTION_REQ,                ApexSimChvFunctionReq   apexSimChvFunctionReq)
    SIG_DEF( SIG_APEX_SIM_CHV_FUNCTION_CNF,                ApexSimChvFunctionCnf   apexSimChvFunctionCnf)
/*9907-4645 New signal to indicate change in CHV state to all registered FL tasks */
    SIG_DEF( SIG_APEX_SIM_CHV_FUNCTION_IND,                ApexSimChvFunctionInd   apexSimChvFunctionInd)

    SIG_DEF( SIG_ABSI_POWERED_DOWN_SIG,                    EmptySignal             absiPoweredDownSig) /* FR9803-2164 */
    SIG_DEF( APEX_SIM_MEP_STATUS_REQ,                      ApexSimMepStatusReq     apexSimMepStatusReq)
    SIG_DEF( APEX_SIM_MEP_STATUS_CNF,                      ApexSimMepStatusCnf     apexSimMepStatusCnf)
    SIG_DEF( APEX_SIM_MEP_REQ,                             ApexSimMepReq           apexSimMepReq)
    SIG_DEF( APEX_SIM_MEP_CNF,                             ApexSimMepCnf           apexSimMepCnf)

    SIG_DEF( SIG_APEX_SIM_GEN_ACCESS_REQ,                  ApexSimGenAccessReq     apexSimGenAccessReq)
    SIG_DEF( SIG_APEX_SIM_GEN_ACCESS_CNF,                  ApexSimGenAccessCnf     apexSimGenAccessCnf)

    SIG_DEF( APEX_SIM_READ_SIM_PARAM_REQ,                  ApexSimReadSimParamReq  apexSimReadSimParamReq)
    SIG_DEF( APEX_SIM_READ_SIM_PARAM_CNF,                  ApexSimReadSimParamCnf  apexSimReadSimParamCnf)

    SIG_DEF( APEX_SIM_OPEN_LOGICAL_CHANNEL_REQ,            ApexSimOpenLogicalChannelReq apexSimOpenLogicalChannelReq)
    SIG_DEF( APEX_SIM_OPEN_LOGICAL_CHANNEL_CNF,            ApexSimOpenLogicalChannelCnf apexSimOpenLogicalChannelCnf)
    SIG_DEF( APEX_SIM_CLOSE_LOGICAL_CHANNEL_REQ,           ApexSimCloseLogicalChannelReq apexSimCloseLogicalChannelReq)
    SIG_DEF( APEX_SIM_CLOSE_LOGICAL_CHANNEL_CNF,           ApexSimCloseLogicalChannelCnf apexSimCloseLogicalChannelCnf)
    SIG_DEF( APEX_SIM_LOGICAL_CHANNEL_ACCESS_REQ,          ApexSimLogicalChannelAccessReq apexSimLogicalChannelAccessReq)
    SIG_DEF( APEX_SIM_LOGICAL_CHANNEL_ACCESS_CNF,          ApexSimLogicalChannelAccessCnf apexSimLogicalChannelAccessCnf)

    SIG_DEF( SIG_APEX_SIM_RAW_APDU_ACCESS_REQ,             ApexSimRawApduAccessReq  apexSimRawApduAccessReq)
    SIG_DEF( SIG_APEX_SIM_RAW_APDU_ACCESS_CNF,             ApexSimRawApduAccessCnf  apexSimRawApduAccessCnf)

#if defined (FEA_TEMP_OBSOLETE_SIM_MANAGER)
    SIG_DEF( APEX_SIM_READ_CFF_REQ,                        ApexSimReadCffReq       apexSimReadCffReq)
    SIG_DEF( APEX_SIM_READ_CFF_CNF,                        ApexSimReadCffCnf       apexSimReadCffCnf)
    SIG_DEF( APEX_SIM_WRITE_CFF_REQ,                       ApexSimWriteCffReq      apexSimWriteCffReq)
    SIG_DEF( APEX_SIM_WRITE_CFF_CNF,                       ApexSimWriteCffCnf      apexSimWriteCffCnf)

    SIG_DEF( APEX_SIM_READ_VMWF_REQ,                       ApexSimReadVmwfReq      apexSimReadVmwfReq)
    SIG_DEF( APEX_SIM_READ_VMWF_CNF,                       ApexSimReadVmwfCnf      apexSimReadVmwfCnf)
    SIG_DEF( APEX_SIM_WRITE_VMWF_REQ,                      ApexSimWriteVmwfReq     apexSimWriteVmwfReq)
    SIG_DEF( APEX_SIM_WRITE_VMWF_CNF,                      ApexSimWriteVmwfCnf     apexSimWriteVmwfCnf)
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */

#if defined (UPGRADE_CMMB)
    SIG_DEF( SIG_APEX_SIM_CMMB_AUTH_REQ,                   ApexSimCmmbAuthReq           apexSimCmmbAuthReq)
    SIG_DEF( SIG_APEX_SIM_CMMB_AUTH_CNF,                   ApexSimCmmbAuthCnf           apexCmmbAuthCnf)
#endif

#if defined (FEA_TEMP_OBSOLETE_SIM_MANAGER)
    SIG_DEF( APEX_SIM_READ_CSP_REQ,                       ApexSimReadCspReq        apexSimReadCspReq)
    SIG_DEF( APEX_SIM_READ_CSP_CNF,                       ApexSimReadCspCnf        apexSimReadCspCnf)
    SIG_DEF( APEX_SIM_WRITE_CSP_REQ,                      ApexSimWriteCspReq       apexSimWriteCspReq)
    SIG_DEF( APEX_SIM_WRITE_CSP_CNF,                      ApexSimWriteCspCnf       apexSimWriteCspCnf)

    SIG_DEF( APEX_SIM_INFO_NUM_STATUS_REQ,                ApexSimInfoNumStatusReq  apexSimInfoNumStatusReq)
    SIG_DEF( APEX_SIM_INFO_NUM_STATUS_CNF,                ApexSimInfoNumStatusCnf  apexSimInfoNumStatusCnf)
    SIG_DEF( APEX_SIM_LIST_INFO_NUM_REQ,                  ApexSimListInfoNumReq    apexSimListInfoNumReq)
    SIG_DEF( APEX_SIM_LIST_INFO_NUM_CNF,                  ApexSimListInfoNumCnf    apexSimListInfoNumCnf)
    SIG_DEF( APEX_SIM_READ_INFO_NUM_REQ,                  ApexSimReadInfoNumReq    apexSimReadInfoNumReq)
    SIG_DEF( APEX_SIM_READ_INFO_NUM_CNF,                  ApexSimReadInfoNumCnf    apexSimReadInfoNumCnf)
    SIG_DEF( APEX_SIM_WRITE_INFO_NUM_REQ,                 ApexSimWriteInfoNumReq   apexSimWriteInfoNumReq)
    SIG_DEF( APEX_SIM_WRITE_INFO_NUM_CNF,                 ApexSimWriteInfoNumCnf   apexSimWriteInfoNumCnf)

    SIG_DEF( APEX_SIM_READ_CPHS_INFO_REQ,                 ApexSimReadCphsInfoReq   apexSimReadCphsInfoReq)
    SIG_DEF( APEX_SIM_READ_CPHS_INFO_CNF,                 ApexSimReadCphsInfoCnf   apexSimReadCphsInfoCnf)
    SIG_DEF( APEX_SIM_CPHS_READY_IND,                     ApexSimCphsReadyInd      apexSimCphsReadyInd)  /*16037*/
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    SIG_DEF( APEX_SIM_READ_SPN_REQ,                        ApexSimReadSpnReq        apexSimReadSpnReq)
    SIG_DEF( APEX_SIM_READ_SPN_CNF,                        ApexSimReadSpnCnf        apexSimReadSpnCnf)
#if defined (FEA_TEMP_OBSOLETE_SIM_MANAGER)
    SIG_DEF( APEX_SIM_READ_SPDI_REQ,                       ApexSimReadSpdiReq       apexSimReadSpdiReq)
    SIG_DEF( APEX_SIM_READ_SPDI_CNF,                       ApexSimReadSpdiCnf       apexSimReadSpdiCnf)
    SIG_DEF( APEX_SIM_READ_CFIS_REQ,                       ApexSimReadCfisReq       apexSimReadCfisReq)
    SIG_DEF( APEX_SIM_READ_CFIS_CNF,                       ApexSimReadCfisCnf       apexSimReadCfisCnf)
    SIG_DEF( APEX_SIM_DELETE_CFIS_REQ,                     ApexSimDeleteCfisReq     apexSimDeleteCfisReq)
    SIG_DEF( APEX_SIM_DELETE_CFIS_CNF,                     ApexSimDeleteCfisCnf     apexSimDeleteCfisCnf)
    SIG_DEF( APEX_SIM_WRITE_CFIS_REQ,                      ApexSimWriteCfisReq      apexSimWriteCfisReq)
    SIG_DEF( APEX_SIM_WRITE_CFIS_CNF,                      ApexSimWriteCfisCnf      apexSimWriteCfisCnf)
#endif /* FEA_TEMP_OBSOLETE_SIM_MANAGER */
    SIG_DEF( APEX_SIM_LIST_PNN_REQ,                        ApexSimListPnnReq        apexSimListPnnReq)
    SIG_DEF( APEX_SIM_LIST_PNN_CNF,                        ApexSimListPnnCnf        apexSimListPnnCnf)
    SIG_DEF( APEX_SIM_LIST_OPL_REQ,                        ApexSimListOplReq        apexSimListOplReq)
    SIG_DEF( APEX_SIM_LIST_OPL_CNF,                        ApexSimListOplCnf        apexSimListOplCnf)
    SIG_DEF( APEX_SIM_READ_CNL_REQ,                        ApexSimReadCnlReq        apexSimReadCnlReq)
    SIG_DEF( APEX_SIM_READ_CNL_CNF,                        ApexSimReadCnlCnf        apexSimReadCnlCnf)
    SIG_DEF( SIG_APEX_SIM_RESUME_REQ,                      ApexSimResumeReq         apexSimResumeReq)   /*0206-18152*/
    SIG_DEF( SIG_APEX_SIM_APP_STARTED_IND,                 ApexSimAppStartedInd     apexSimAppStartedInd) /*0209-20294*/
    SIG_DEF( SIG_APEX_SIM_APP_STOPPED_IND,                 ApexSimAppStoppedInd     apexSimAppStoppedInd)
    SIG_DEF( SIG_APEX_SIM_CHV_STATUS_IND,                  ApexSimChvStatusInd      apexSimChvStatusInd) /*job 104913*/
    SIG_DEF( SIG_APEX_SIM_USIM_APP_START_REQ,              ApexSimUsimAppStartReq    apexSimUsimAppStartReq)
    SIG_DEF( SIG_APEX_SIM_USIM_APP_START_CNF,              ApexSimUsimAppStartCnf    apexSimUsimAppStartCnf)
    SIG_DEF( SIG_APEX_SIM_READ_DIR_REQ,                    ApexSimReadDirReq         apexSimReadDirReq)
    SIG_DEF( SIG_APEX_SIM_READ_DIR_CNF,                    ApexSimReadDirCnf         apexSimReadDirCnf)
    SIG_DEF( SIG_APEX_SIM_PIN_FUNCTION_REQ,                ApexSimPinFunctionReq     apexSimPinFunctionReq)
    SIG_DEF( SIG_APEX_SIM_PIN_FUNCTION_CNF,                ApexSimPinFunctionCnf     apexSimPinFunctionCnf)
    SIG_DEF( SIG_APEX_SIM_PIN_FUNCTION_IND,                ApexSimPinFunctionInd     apexSimPinFunctionInd)
    SIG_DEF( SIG_APEX_SIM_GET_PIN_IND,                     ApexSimGetPinInd          apexSimGetPinInd)
    SIG_DEF( SIG_APEX_SIM_GET_PIN_RSP,                     ApexSimGetPinRsp          apexSimGetPinRsp)
    SIG_DEF( SIG_APEX_SIM_READ_MMSN_REQ,                   ApexSimReadMmsnReq        apexSimReadMmsnReq)
    SIG_DEF( SIG_APEX_SIM_READ_MMSN_CNF,                   ApexSimReadMmsnCnf        apexSimReadMmsnCnf)
    SIG_DEF( SIG_APEX_SIM_WRITE_MMSN_REQ,                  ApexSimWriteMmsnReq       apexSimWriteMmsnReq)
    SIG_DEF( SIG_APEX_SIM_WRITE_MMSN_CNF,                  ApexSimWriteMmsnCnf       apexSimWriteMmsnCnf)
    SIG_DEF( SIG_APEX_SIM_DELETE_MMSN_REQ,                 ApexSimDeleteMmsnReq      apexSimDeleteMmsnReq)
    SIG_DEF( SIG_APEX_SIM_DELETE_MMSN_CNF,                 ApexSimDeleteMmsnCnf      apexSimDeleteMmsnCnf)
    SIG_DEF( SIG_APEX_SIM_READ_MMSUP_REQ,                  ApexSimReadMmsupReq       apexSimReadMmsupReq)
    SIG_DEF( SIG_APEX_SIM_READ_MMSUP_CNF,                  ApexSimReadMmsupCnf       apexSimReadMmsupCnf)
    SIG_DEF( SIG_APEX_SIM_WRITE_MMSUP_REQ,                 ApexSimWriteMmsupReq      apexSimWriteMmsupReq)
    SIG_DEF( SIG_APEX_SIM_WRITE_MMSUP_CNF,                 ApexSimWriteMmsupCnf      apexSimWriteMmsupCnf)
    SIG_DEF( SIG_APEX_SIM_DELETE_MMSUP_REQ,                ApexSimDeleteMmsupReq     apexSimDeleteMmsupReq)
    SIG_DEF( SIG_APEX_SIM_DELETE_MMSUP_CNF,                ApexSimDeleteMmsupCnf     apexSimDeleteMmsupCnf)
    SIG_DEF( SIG_APEX_SIM_READ_MMSCP_REQ,                  ApexSimReadMmscpReq       apexSimReadMmscpReq)
    SIG_DEF( SIG_APEX_SIM_READ_MMSCP_CNF,                  ApexSimReadMmscpCnf       apexSimReadMmscpCnf)
    SIG_DEF( SIG_APEX_SIM_CHANGE_TARGET_TASK_ID_REQ,       ApexSimChangeTargetTaskIdReq  apexSimChangeTargetTaskIdReq)
    SIG_DEF( SIG_APEX_SIM_CHANGE_TARGET_TASK_ID_CNF,       ApexSimChangeTargetTaskIdCnf  apexSimChangeTargetTaskIdCnf)
    SIG_DEF( SIG_APEX_SIM_WRITE_MEP_NETWORK_ID_REQ,        ApexSimWriteMepNetworkIdReq   apexSimWriteMepNetworkIdReq)
    SIG_DEF( SIG_APEX_SIM_WRITE_MEP_NETWORK_ID_CNF,        ApexSimWriteMepNetworkIdCnf   apexSimWriteMepNetworkIdCnf)
    SIG_DEF( SIG_APEX_SIM_READ_MEP_NETWORK_ID_REQ,         ApexSimReadMepNetworkIdReq    apexSimReadMepNetworkIdReq)
    SIG_DEF( SIG_APEX_SIM_READ_MEP_NETWORK_ID_CNF,         ApexSimReadMepNetworkIdCnf    apexSimReadMepNetworkIdCnf)
    SIG_DEF( SIG_APEX_SIM_READ_HZ_PARAMS_REQ,              ApexSimReadHZParamsReq    apexSimReadHZParamsReq)
    SIG_DEF( SIG_APEX_SIM_READ_HZ_PARAMS_CNF,              ApexSimReadHZParamsCnf    apexSimReadHZParamsCnf)
    SIG_DEF( SIG_APEX_SIM_READ_HZ_CACHE_REQ,               ApexSimReadHZCacheReq     apexSimReadHZCacheReq)
    SIG_DEF( SIG_APEX_SIM_READ_HZ_CACHE_CNF,               ApexSimReadHZCacheCnf     apexSimReadHZCacheCnf)
    SIG_DEF( SIG_APEX_SIM_WRITE_HZ_CACHE_REQ,              ApexSimWriteHZCacheReq    apexSimWriteHZCacheReq)
    SIG_DEF( SIG_APEX_SIM_WRITE_HZ_CACHE_CNF,              ApexSimWriteHZCacheCnf    apexSimWriteHZCacheCnf)
    SIG_DEF( SIG_APEX_SIM_READ_MWIS_REQ,                   ApexSimReadMwisReq        apexSimReadMwisReq)
    SIG_DEF( SIG_APEX_SIM_READ_MWIS_CNF,                   ApexSimReadMwisCnf        apexSimReadMwisCnf)
    SIG_DEF( SIG_APEX_SIM_READ_SYSTEM_MWI_REQ,             ApexSimReadSystemMwiReq   apexSimReadSystemMwiReq)
    SIG_DEF( SIG_APEX_SIM_READ_SYSTEM_MWI_CNF,             ApexSimReadSystemMwiCnf   apexSimReadSystemMwiCnf)
    SIG_DEF( SIG_APEX_SIM_WRITE_MWIS_REQ,                  ApexSimWriteMwisReq       apexSimWriteMwisReq)
    SIG_DEF( SIG_APEX_SIM_WRITE_MWIS_CNF,                  ApexSimWriteMwisCnf       apexSimWriteMwisCnf)
    SIG_DEF( SIG_APEX_SIM_READ_MBI_REQ,                    ApexSimReadMbiReq        apexSimReadMbiReq)
    SIG_DEF( SIG_APEX_SIM_READ_MBI_CNF,                    ApexSimReadMbiCnf        apexSimReadMbiCnf)
    SIG_DEF( SIG_APEX_SIM_WRITE_MBI_REQ,                   ApexSimWriteMbiReq       apexSimWriteMbiReq)
    SIG_DEF( SIG_APEX_SIM_WRITE_MBI_CNF,                   ApexSimWriteMbiCnf       apexSimWriteMbiCnf)
    SIG_DEF( SIG_APEX_SIM_READ_ACC_CALL_TIMER_REQ,         ApexSimReadAccCallTimerReq    apexSimReadAccCallTimerReq)
    SIG_DEF( SIG_APEX_SIM_READ_ACC_CALL_TIMER_CNF,         ApexSimReadAccCallTimerCnf    apexSimReadAccCallTimerCnf)
    SIG_DEF( SIG_APEX_SIM_RESET_ACC_CALL_TIMER_REQ,        ApexSimResetAccCallTimerReq   apexSimResetAccCallTimerReq)
    SIG_DEF( SIG_APEX_SIM_RESET_ACC_CALL_TIMER_CNF,        ApexSimResetAccCallTimerCnf   apexSimResetAccCallTimerCnf)
    SIG_DEF( SIG_APEX_SIM_ACC_CALL_TIMER_CHANGED_IND,      ApexSimAccCallTimerChangedInd apexSimAccCallTimerChangedInd)
#if defined (UPGRADE_REL8_USIM)
    SIG_DEF( SIG_APEX_SIM_CSIM_LOCK_REQ,                   ApexSimCsimLockReq           apexSimCsimLockReq)
    SIG_DEF( SIG_APEX_SIM_CSIM_LOCK_CNF,                   ApexSimCsimLockCnf           apexSimCsimLockCnf)
    SIG_DEF( SIG_APEX_SIM_CSIM_LOCK_IND,                   ApexSimCsimLockInd           apexSimCsimLockInd)
#endif /* UPGRADE_REL8_USIM */

#if defined (ENABLE_DUAL_SIM_SOLUTION)
    SIG_DEF( SIG_APEX_SIM_SELECT_REQ,                      ApexSimSelectReq              apexSimSelectReq)
    SIG_DEF( SIG_APEX_SIM_SELECT_CNF,                      ApexSimSelectCnf              apexSimSelectCnf)
#endif

    SIG_DEF( SIG_APEX_SIM_READ_MSISDN_REQ,                 ApexSimReadMsisdnReq          apexSimReadMsisdnReq)
    SIG_DEF( SIG_APEX_SIM_READ_MSISDN_CNF,                 ApexSimReadMsisdnCnf          apexSimReadMsisdnCnf)
#if defined(FEA_TEMP_INCLUDE_SIM_EMULATION) || defined (FEA_TEMP_ESIM_EMULATION)
    SIG_DEF( SIG_APEX_SIM_EMUSIM_REQ,                      ApexSimEmuSimReq              apexSimEmuSimReq)
    SIG_DEF( SIG_APEX_SIM_EMUSIM_CNF,                      ApexSimEmuSimCnf              apexSimEmuSimCnf)
#endif

#if defined (UPGRADE_CONN_MDT_NB)
    SIG_DEF ( SIG_APEX_SIM_DEBUG_MDT_UINT_DATA,            ApexSimDebugMdtUnitData       apexSimDebugMdtUnitData)
#endif

 /*========================================================================*/
/* APEX:  AL BL Short Messages Procedure                                  */

    SIG_DEF( SIG_APEX_SM_DUMMY = ABSM_SIGNAL_BASE,         EmptySignal             dummySm)
    SIG_DEF( SIG_APEX_SM_READY_IND,                        ApexSmReadyInd          apexSmReadyInd)
    SIG_DEF( SIG_APEX_SM_STATUS_REQ,                       ApexSmStatusReq         apexSmStatusReq)
    SIG_DEF( SIG_APEX_SM_STATUS_CNF,                       ApexSmStatusCnf         apexSmStatusCnf)
    SIG_DEF( SIG_APEX_SM_READ_REQ,                         ApexSmReadReq           apexSmReadReq)
    SIG_DEF( SIG_APEX_SM_READ_CNF,                         ApexSmReadCnf           apexSmReadCnf)
    SIG_DEF( SIG_APEX_SM_READ_SMSR_REQ,                    ApexSmReadSmsrReq       apexSmReadSmsrReq)
    SIG_DEF( SIG_APEX_SM_READ_SMSR_CNF,                    ApexSmReadSmsrCnf       apexSmReadSmsrCnf)
    SIG_DEF( SIG_APEX_SM_DELIVERY_IND,                     ApexSmDeliveryInd       apexSmDeliveryInd)
    SIG_DEF( SIG_APEX_SM_DELIVERY_RSP,                     ApexSmDeliveryRsp       apexSmDeliveryRsp)
    SIG_DEF( SIG_APEX_SM_DELIVERY_WITH_REPORT_RSP,         ApexSmDeliveryWithReportRsp apexSmDeliveryWithReportRsp)
    SIG_DEF( SIG_APEX_SM_MSG_RECEIVED_IND,                 ApexSmMsgReceivedInd    apexSmMsgReceivedInd)
    SIG_DEF( SIG_APEX_SM_SEND_REQ,                         ApexSmSendReq           apexSmSendReq)
    SIG_DEF( SIG_APEX_SM_SEND_CNF,                         ApexSmSendCnf           apexSmSendCnf)
    SIG_DEF( SIG_APEX_SM_READ_ROUTE_REQ,                   ApexSmReadRouteReq      apexSmReadRouteReq)
    SIG_DEF( SIG_APEX_SM_READ_ROUTE_CNF,                   ApexSmReadRouteCnf      apexSmReadRouteCnf)
    SIG_DEF( SIG_APEX_SM_WRITE_ROUTE_REQ,                  ApexSmWriteRouteReq     apexSmWriteRouteReq)
    SIG_DEF( SIG_APEX_SM_WRITE_ROUTE_CNF,                  ApexSmWriteRouteCnf     apexSmWriteRouteCnf)

    SIG_DEF( SIG_APEX_SM_SEND_FROM_SIM_REQ,                ApexSmSendFromSimReq    apexSmSendFromSimReq)
    SIG_DEF( SIG_APEX_SM_SEND_FROM_SIM_CNF,                ApexSmSendFromSimCnf    apexSmSendFromSimCnf)
    SIG_DEF( SIG_APEX_SM_COMMAND_REQ,                      ApexSmCommandReq        apexSmCommandReq)
    SIG_DEF( SIG_APEX_SM_COMMAND_CNF,                      ApexSmCommandCnf        apexSmCommandCnf)
    SIG_DEF( SIG_APEX_SM_STATUS_REPORT_IND,                ApexSmStatusReportInd   apexSmStatusReportInd)
    SIG_DEF( SIG_APEX_SM_SMSR_STORE_IND,                   ApexSmSmsrStoreInd      apexSmSmsrStoreInd)
    SIG_DEF( SIG_APEX_SM_SMSR_STORE_RSP,                   ApexSmSmsrStoreRsp      apexSmSmsrStoreRsp)
    SIG_DEF( SIG_APEX_SM_SR_DELETED_IND,                   ApexSmSrDeletedInd      apexSmSrDeletedInd)
    SIG_DEF( SIG_APEX_SM_STORE_REQ,                        ApexSmStoreReq          apexSmStoreReq)
    SIG_DEF( SIG_APEX_SM_STORE_CNF,                        ApexSmStoreCnf          apexSmStoreCnf)
    SIG_DEF( SIG_APEX_SM_DELETE_REQ,                       ApexSmDeleteReq         apexSmDeleteReq)
    SIG_DEF( SIG_APEX_SM_DELETE_CNF,                       ApexSmDeleteCnf         apexSmDeleteCnf)
    SIG_DEF( SIG_APEX_SM_DELETE_SMSR_REQ,                  ApexSmDeleteSmsrReq     apexSmDeleteSmsrReq)
    SIG_DEF( SIG_APEX_SM_DELETE_SMSR_CNF,                  ApexSmDeleteSmsrCnf     apexSmDeleteSmsrCnf)
    SIG_DEF( SIG_APEX_SM_READ_SMSP_REQ,                    ApexSmReadSmspReq       apexSmReadSmspReq)
    SIG_DEF( SIG_APEX_SM_READ_SMSP_CNF,                    ApexSmReadSmspCnf       apexSmReadSmspCnf)
    SIG_DEF( SIG_APEX_SM_WRITE_SMSP_REQ,                   ApexSmWriteSmspReq      apexSmWriteSmspReq)
    SIG_DEF( SIG_APEX_SM_WRITE_SMSP_CNF,                   ApexSmWriteSmspCnf      apexSmWriteSmspCnf)
    SIG_DEF( SIG_APEX_SM_DELETE_SMSP_REQ,                  ApexSmDeleteSmspReq     apexSmDeleteSmspReq)
    SIG_DEF( SIG_APEX_SM_DELETE_SMSP_CNF,                  ApexSmDeleteSmspCnf     apexSmDeleteSmspCnf)
    SIG_DEF( SIG_APEX_SM_LIST_SMSP_REQ,                    ApexSmListSmspReq       apexSmListSmspReq)
    SIG_DEF( SIG_APEX_SM_LIST_SMSP_CNF,                    ApexSmListSmspCnf       apexSmListSmspCnf)
    SIG_DEF( SIG_APEX_SM_FIND_SMSP_REQ,                    ApexSmFindSmspReq       apexSmFindSmspReq)
    SIG_DEF( SIG_APEX_SM_FIND_SMSP_CNF,                    ApexSmFindSmspCnf       apexSmFindSmspCnf)
    SIG_DEF( SIG_APEX_SM_RECORD_CHANGED_IND,               ApexSmRecordChangedInd  apexSmRecordChangedInd)
    SIG_DEF( SIG_APEX_SM_TRANSACTION_END_IND,              ApexSmTransactionEndInd apexSmTransactionEndInd)
    /*
    ** Signal to indicate that the VG CNMI <mt> and <ds> parameters or CSMS <service> parameter have changed
    ** and the relevant absmContext.profile flags need to be updated for the routing and acknowledgement of MT SMSs.
     */
    SIG_DEF( SIG_APEX_SM_PROFILE_CHANGED_REQ,              ApexSmProfileChangedReq apexSmProfileChangedReq)

    /* Additional signals to allow the foreground to read the SMS
     * status and address from the BL without accessing the SIM
     * or affecting the "unread" status of the message 0008-7362 */
    SIG_DEF( SIG_APEX_SM_READ_NODATA_REQ,                  ApexSmReadReq           apexSmReadNoDataReq)
    SIG_DEF( SIG_APEX_SM_READ_NODATA_CNF,                  ApexSmReadCnf           apexSmReadNoDataCnf)

    /* Additional signals to allow the foreground to read the SMS SR
     * Sms record number and MsgRef from the BL without accessing the SIM */
    SIG_DEF( SIG_APEX_SM_READ_SMSR_NODATA_REQ,             ApexSmReadSmsrReq       apexSmReadSmsrNoDataReq)
    SIG_DEF( SIG_APEX_SM_READ_SMSR_NODATA_CNF,             ApexSmReadSmsrCnf       apexSmReadSmsrNoDataCnf)

    /*job102259 Additional signals for ME SMS storage */
    SIG_DEF( SIG_APEX_SM_STORE_IND,                        ApexSmStoreInd          apexSmStoreInd)
    SIG_DEF( SIG_APEX_SM_STORE_RSP,                        ApexSmStoreRsp          apexSmStoreRsp)
    SIG_DEF( SIG_APEX_SM_RESET_MEMCAP_FULL_REQ,            EmptySignal             dummySm2)
    SIG_DEF( SIG_APEX_SM_ME_CAP_FULL_IND,                  ApexSmMeCapFullInd      apexSmMeCapFullInd)

    /* job 103293 Additional signals for tasks to register as consumer's of sms's from chosen port numbers */
    SIG_DEF( SIG_APEX_SM_REGISTER_PORT_REQ,                ApexSmRegisterPortReq   apexSmRegisterPortReq)
    SIG_DEF( SIG_APEX_SM_REGISTER_PORT_CNF,                ApexSmRegisterPortCnf   apexSmRegisterPortCnf)
    SIG_DEF( SIG_APEX_SM_DEREGISTER_PORT_REQ,              ApexSmDeregisterPortReq apexSmDeregisterPortReq)
    SIG_DEF( SIG_APEX_SM_DEREGISTER_PORT_CNF,              ApexSmDeregisterPortCnf apexSmDeregisterPortCnf)

    /* job116776 Adding Send More Messages functionality as defined in 27.007 */
    SIG_DEF( SIG_APEX_SM_SEND_MORE_REQ,                    ApexSmSendMoreReq       apexSmSendMoreReq)

    SIG_DEF( SIG_APEX_SM_STORE_LOC_REQ,                    ApexSmStoreLocReq       apexSmStoreLocReq)
    SIG_DEF( SIG_APEX_SM_STORE_LOC_CNF,                    ApexSmStoreLocCnf       apexSmStoreLocCnf)
    SIG_DEF( SIG_APEX_SM_SET_LOC_STATUS_REQ,               ApexSmSetLocStatusReq   apexSmSetLocStatusReq)
    SIG_DEF( SIG_APEX_SM_SET_LOC_STATUS_CNF,               ApexSmSetLocStatusCnf   apexSmSetLocStatusCnf)

    /* New signal needed by MSMC commands*/
    SIG_DEF( SIG_APEX_SM_STATUS_CHANGED_IND,               ApexSmStatusChangedInd  apexSmStatusChangedInd)
    SIG_DEF( SIG_APEX_SM_STK_INFO_IND,                     ApexSmStkInfoInd        apexSmStkInfoInd)
#if defined (ABSM_DEBUG)
    SIG_DEF( SIG_APEX_SM_DEBUG_REQ,                        EmptySignal             dummySm3 )
    SIG_DEF( SIG_APEX_SM_DEBUG_CNF,                        ApexSmDebugCnf          apexSmDebugCnf)
    SIG_DEF( SIG_APEX_SM_DEBUG_IND,                        ApexSmDebugInd          apexSmDebugInd)
#endif
#if defined (ENABLE_L23_DEBUG)
    SIG_DEF( SIG_ABSM_DEBUG_IND,                           AbsmDebugInd        absmDebugInd)
#endif
/*========================================================================*/
/* APEX:  AL BL Supplementary Services Procedure                          */

    SIG_DEF( SIG_APEX_SS_DUMMY = ABSS_SIGNAL_BASE,         EmptySignal             dummySs)
    SIG_DEF( SIG_APEX_SS_PROCUSSREQUEST_CNF   ,            ApexSsProcUssRequestCnf apexSsProcUssRequestCnf)
    SIG_DEF( SIG_APEX_SS_PASSWORD_RSP         ,            ApexSsPasswordRsp       apexSsPasswordRsp      )
    SIG_DEF( SIG_APEX_SS_USSRESULT_RSP        ,            ApexSsUssResultRsp      apexSsUssResultRsp     )
    SIG_DEF( SIG_APEX_SS_ERROR_RSP            ,            ApexSsErrorRsp          apexSsErrorRsp         )
    SIG_DEF( SIG_APEX_SS_REJECT_RSP           ,            ApexSsRejectRsp         apexSsRejectRsp        )
    SIG_DEF( SIG_APEX_SS_END_RSP              ,            ApexSsEndRsp            apexSsEndRsp           )
    SIG_DEF( SIG_APEX_SS_GETPASSWORD_IND      ,            ApexSsGetPasswordInd    apexSsGetPasswordInd   )
    SIG_DEF( SIG_APEX_SS_USSMESSAGE_IND       ,            ApexSsUssMessageInd     apexSsUssMessageInd    )
    SIG_DEF( SIG_APEX_SS_FORWARDCHECK_IND     ,            ApexSsForwardCheckInd   apexSsForwardCheckInd  )
    SIG_DEF( SIG_APEX_SS_ERROR_IND            ,            ApexSsErrorInd          apexSsErrorInd         )
    SIG_DEF( SIG_APEX_SS_REJECT_IND           ,            ApexSsRejectInd         apexSsRejectInd        )
    SIG_DEF( SIG_APEX_SS_CANCEL_IND           ,            ApexSsCancelInd         apexSsCancelInd        )
    SIG_DEF( SIG_APEX_SS_RETURN_IND           ,            ApexSsReturnInd         apexSsReturnInd        )
    SIG_DEF( SIG_APEX_SS_RELEASE_IND          ,            ApexSsReleaseInd        apexSsReleaseInd       )
    /* job 111331 */
    SIG_DEF( SIG_APEX_SS_CHANGED_IND          ,            ApexSsChangedInd        apexSsChangedInd       )
    SIG_DEF( SIG_APEX_SS_STK_INFO_IND         ,            ApexSsStkInfoInd        apexSsStkInfoInd       )

/*========================================================================*/
/* APEX:  AL BL Engineering Mode Procedure                          */
    /* FR9802-1945 - new signal base*/
    SIG_DEF( SIG_APEX_EM_DUMMY = ABEM_SIGNAL_BASE,         EmptySignal             dummyEm)
    SIG_DEF( SIG_APEX_EM_ASP_LOGGER_REQ       ,            ApexEmAspLoggerReq      apexEmAspLoggerReq     )
    SIG_DEF( SIG_APEX_EM_ASP_LOGGER_CNF       ,            ApexEmAspLoggerCnf      apexEmAspLoggerCnf     )
    SIG_DEF( SIG_APEX_EM_INFO_REQ             ,            ApexEmInfoReq           apexEmInfoReq          )
    SIG_DEF( SIG_APEX_EM_INFO_CNF             ,            ApexEmInfoCnf           apexEmInfoCnf          )
    SIG_DEF( SIG_APEX_EM_READY_IND            ,            ApexEmReadyInd          apexEmReadyInd         )
    SIG_DEF( SIG_APEX_EM_FORCE_CMD_REQ        ,            ApexEmInfoForceCmdReq   apexEmInfoForceCmdReq  )
    SIG_DEF( SIG_APEX_EM_FORCE_CMD_CNF        ,            ApexEmInfoForceCmdCnf   apexEmInfoForceCmdCnf  )
#if defined (UPGRADE_GGE)
    SIG_DEF( SIG_APEX_EM_GRR_IDLE_INFO_IND    ,            ApexEmGrrIdleInfoInd    apexEmGrrIdleInfoInd   )
    SIG_DEF( SIG_APEX_EM_GRR_DED_INFO_IND     ,            ApexEmGrrDedInfoInd     apexEmGrrDedInfoInd    )
    SIG_DEF( SIG_APEX_EM_GRR_IDLE_SCELL_IND   ,            ApexEmGrrIdleSCellInd   apexEmGrrIdleSCellInd  )
    SIG_DEF( SIG_APEX_EM_GRR_DED_SCELL_IND    ,            ApexEmGrrDedSCellInd    apexEmGrrDedSCellInd   )
#endif
    SIG_DEF( SIG_APEX_EM_MMR_INFO_IND         ,            ApexEmMmrInfoInd        apexEmMmrInfoInd       )
    SIG_DEF( SIG_APEX_EM_PD_SM_INFO_IND       ,            ApexEmPdSmInfoInd       apexEmPdSmInfoInd      )

#if defined (UPGRADE_GGE)
    SIG_DEF( SIG_APEX_EM_GP_RLCMAC_INFO_IND   ,            ApexEmGpRlcmacInfoInd   apexEmGpRlcmacInfoInd  )
    SIG_DEF( SIG_APEX_EM_GP_LLC_INFO_IND      ,            ApexEmGpLlcInfoInd      apexEmGpLlcInfoInd     )
#endif

#if defined (UPGRADE_3G)
    SIG_DEF( SIG_APEX_EM_RRC_INFO_IND        ,             ApexEmRrcInfoInd        apexEmRrcInfoInd      )
#endif /* UPGRADE_3G */

    SIG_DEF( SIG_APEX_EM_PRODUCTION_TEST_IND  ,            ApexEmProductionTestInd apexEmProductionTestInd)
    SIG_DEF( SIG_APEX_EM_RESET_NVRAM_IND      ,            ApexEmResetNvramInd     apexEmResetNvramInd    )
    SIG_DEF( SIG_APEX_EM_RESET_NVRAM_RSP      ,            ApexEmResetNvramRsp     apexEmResetNvramRsp    )
    SIG_DEF( SIG_APEX_EM_PLMN_TEST_REQ        ,            ApexEmPlmnTestReq       apexEmPlmnTestReq      )
    SIG_DEF( SIG_APEX_EM_PLMN_TEST_CNF        ,            ApexEmPlmnTestCnf       apexEmPlmnTestCnf      )

    /*CR:0002-6033 */
    /* NULL State test signals to force handset in and out of NULL state on the production line */
    SIG_DEF( SIG_APEX_EM_ENTER_NULL_STATE_SIG ,            EmptySignal             apexEmEnterNullStateSig)
    SIG_DEF( SIG_APEX_EM_EXIT_NULL_STATE_SIG  ,            EmptySignal             apexEmExitNullStateSig )




/*========================================================================*/
/* Application Layer Non-Volatile RAM Manager Signals                     */

/* ANRM  signals used by original manager (NRAM_TASK)
 * ANRM2 signals used by System 2 manager (ANRM2_TASK)    */

   SIG_DEF( SIG_ANRM_DUMMY = ANRM_SIGNAL_BASE,            EmptySignal              dummyAnrm)
   #include <anrm2sigdef.h>


 /*======================================================================= */
 /* AFSA: SIM Application Toolkit                                                          */

    SIG_DEF ( SIG_AFSA_DUMMY = AFSA_SIGNAL_BASE,            EmptySignal              dummyAfsa)
    SIG_DEF ( SIG_AFSA_DISPLAY_TEXT_IND,                    AfsaDisplayTextInd       afsaDisplayTextInd)
    SIG_DEF ( SIG_AFSA_DISPLAY_TEXT_RSP,                    AfsaDisplayTextRsp       afsaDisplayTextRsp)
    SIG_DEF ( SIG_AFSA_GET_INKEY_IND,                       AfsaGetInkeyInd          afsaGetInkeyInd)
    SIG_DEF ( SIG_AFSA_GET_INKEY_RSP,                       AfsaGetInkeyRsp          afsaGetInkeyRsp)
    SIG_DEF ( SIG_AFSA_GET_INPUT_IND,                       AfsaGetInputInd          afsaGetInputInd)
    SIG_DEF ( SIG_AFSA_GET_INPUT_RSP,                       AfsaGetInputRsp          afsaGetInputRsp)
    SIG_DEF ( SIG_AFSA_PLAY_TONE_IND,                       AfsaPlayToneInd          afsaPlayToneInd)
    SIG_DEF ( SIG_AFSA_PLAY_TONE_RSP,                       AfsaPlayToneRsp          afsaPlayToneRsp)
    SIG_DEF ( SIG_AFSA_SET_UP_MENU_IND,                     AfsaSetUpMenuInd         afsaSetUpMenuInd)
    SIG_DEF ( SIG_AFSA_SET_UP_MENU_RSP,                     AfsaSetUpMenuRsp         afsaSetUpMenuRsp)
    SIG_DEF ( SIG_AFSA_SELECT_ITEM_IND,                     AfsaSelectItemInd        afsaSelectItemInd)
    SIG_DEF ( SIG_AFSA_SELECT_ITEM_RSP,                     AfsaSelectItemRsp        afsaSelectItemRsp)
    SIG_DEF ( SIG_AFSA_MENU_SELECTION_REQ,                  AfsaMenuSelectionReq     afsaMenuSelectionReq)
    SIG_DEF ( SIG_AFSA_MENU_SELECTION_CNF,                  AfsaMenuSelectionCnf     afsaMenuSelectionCnf)
    SIG_DEF ( SIG_AFSA_DISPLAY_STATUS_IND,                  AfsaDisplayStatusInd     afsaDisplayStatusInd)
    SIG_DEF ( SIG_AFSA_USER_ACTIVITY_IND,                   EmptySignal              afsaUserActivityInd)
    SIG_DEF ( SIG_AFSA_ST_REGISTER_TASK_REQ,                AfsaStRegisterTaskReq    afsaStRegisterTaskReq)
    SIG_DEF ( SIG_AFSA_ST_REGISTER_TASK_CNF,                AfsaStRegisterTaskCnf    afsaStRegisterTaskCnf)
    SIG_DEF ( SIG_AFSA_SET_UP_EVENT_LIST_IND,               AfsaSetUpEventListInd    afsaSetUpEventListInd)
    SIG_DEF ( SIG_AFSA_SET_UP_EVENT_LIST_RSP,               AfsaSetUpEventListRsp    afsaSetUpEventListRsp)
    SIG_DEF ( SIG_AFSA_REFRESH_IND,                         AfsaRefreshInd           afsaRefreshInd)
    SIG_DEF ( SIG_AFSA_REFRESH_RSP,                         AfsaRefreshRsp           afsaRefreshRsp)
    SIG_DEF( SIG_AFSA_RUN_AT_COMMAND_IND,                   AfsaRunAtCommandInd        afsaRunAtCommandInd)
    SIG_DEF( SIG_AFSA_RUN_AT_COMMAND_RSP,                   AfsaRunAtCommandRsp        afsaRunAtCommandRsp)
    SIG_DEF( SIG_AFSA_SET_UP_IDLE_MODE_TEXT_IND,            AfsaSetUpIdleModeTextInd   afsaSetUpIdleModeTextInd)
    SIG_DEF( SIG_AFSA_SET_UP_IDLE_MODE_TEXT_RSP,            AfsaSetUpIdleModeTextRsp   afsaSetUpIdleModeTextRsp)
    SIG_DEF( SIG_AFSA_LANGUAGE_NOTIFICATION_IND,            AfsaLanguageNotificationInd afsaLanguageNotificationInd)
    SIG_DEF( SIG_AFSA_LANGUAGE_NOTIFICATION_RSP,            AfsaLanguageNotificationRsp afsaLanguageNotificationRsp)
    SIG_DEF( SIG_AFSA_LANGUAGE_SELECTION_IND,               AfsaLanguageSelectionInd   afsaLanguageSelectionInd)
    SIG_DEF( SIG_AFSA_READ_IMAGE_REC_REQ,                   AfsaReadImageRecReq        afsaReadImageRecReq)
    SIG_DEF( SIG_AFSA_READ_IMAGE_REC_CNF,                   AfsaReadImageRecCnf        afsaReadImageRecCnf)
    SIG_DEF( SIG_AFSA_READ_IMAGE_INST_DATA_REQ,             AfsaReadImageInstDataReq   afsaReadImageInstDataReq)
    SIG_DEF( SIG_AFSA_READ_IMAGE_INST_DATA_CNF,             AfsaReadImageInstDataCnf   afsaReadImageInstDataCnf)
    SIG_DEF( SIG_AFSA_LIST_IMAGE_REC_REQ,                   AfsaListImageRecReq        afsaListImageRecReq)
    SIG_DEF( SIG_AFSA_LIST_IMAGE_REC_CNF,                   AfsaListImageRecCnf        afsaListImageRecCnf)
    SIG_DEF( SIG_AFSA_LAUNCH_BROWSER_IND,                   AfsaLaunchBrowserInd       afsaLaunchBrowserInd)
    SIG_DEF( SIG_AFSA_LAUNCH_BROWSER_RSP,                   AfsaLaunchBrowserRsp       afsaLaunchBrowserRsp)
    SIG_DEF( SIG_AFSA_CLOSE_BROWSER_IND,                    AfsaCloseBrowserInd        afsaCloseBrowserInd)

    SIG_DEF( SIG_AFSA_MMI_PROFILE_UPDATE_REQ,               AfsaMmiProfileUpdateReq    afsaMmiProfileUpdateReq)
    SIG_DEF( SIG_AFSA_MMI_PROFILE_UPDATE_CNF,               AfsaMmiProfileUpdateCnf    afsaMmiProfileUpdateCnf)
    SIG_DEF( SIG_AFSA_READ_MMI_PROFILE_REQ,                 AfsaReadMmiProfileReq      afsaReadMmiProfileReq)
    SIG_DEF( SIG_AFSA_READ_MMI_PROFILE_CNF,                 AfsaReadMmiProfileCnf      afsaReadMmiProfileCnf)
    SIG_DEF( SIG_AFSA_DISPLAY_PARAMS_CHANGED_IND,           AfsaDisplayParamsChangedInd afsaDisplayParamsChangedInd)
 /*======================================================================= */
 /* APEX: SIM SIM Application Toolkit                                                       */

    SIG_DEF ( SIG_ABST_DUMMY = ABST_SIGNAL_BASE,            EmptySignal                  dummyAbst)
    SIG_DEF ( SIG_APEX_ST_SMS_PP_DOWNLOAD_IND,              ApexStSmsPpDownloadInd       apexStSmsPpDownloadInd)
    SIG_DEF ( SIG_APEX_ST_SMS_PP_DOWNLOAD_RSP,              ApexStSmsPpDownloadRsp       apexStSmsPpDownloadRsp)
    SIG_DEF ( SIG_APEX_ST_CONFIGURE_REQ,                    ApexStConfigureReq           apexStConfigureReq)
    SIG_DEF ( SIG_APEX_ST_CONFIGURE_CNF,                    ApexStConfigureCnf           apexStConfigureCnf)
    SIG_DEF ( SIG_APEX_ST_REFRESH_REQ,                      ApexStRefreshReq             apexStRefreshReq)
    SIG_DEF ( SIG_APEX_ST_REFRESH_CNF,                      ApexStRefreshCnf             apexStRefreshCnf)
    SIG_DEF ( SIG_APEX_ST_CC_CALL_SET_UP_REQ,               ApexStCcCallSetUpReq         apexStCcCallSetUpReq)
    SIG_DEF ( SIG_APEX_ST_CC_CALL_SET_UP_CNF,               ApexStCcCallSetUpCnf         apexStCcCallSetUpCnf)
    SIG_DEF ( SIG_APEX_ST_CC_STATUS_IND,                    ApexStCcStatusInd            apexStCcStatusInd)
    SIG_DEF ( SIG_APEX_ST_CALL_SETUP_GET_ACK_RSP,           ApexStCallSetupGetAckRsp     apexStCallSetupGetAckRsp)
    SIG_DEF ( SIG_APEX_ST_CALL_SETUP_GET_ACK_CNF,           ApexStCallSetupGetAckCnf     apexStCallSetupGetAckCnf)
    SIG_DEF ( SIG_APEX_ST_DOWNLOAD_ERROR_IND,               ApexStDownloadErrorInd       apexStDownloadErrorInd)
    SIG_DEF ( SIG_APEX_ST_INFORM_SIM_APP_ERR_REQ,           ApexStInformSimAppErrReq     apexStInformSimAppErrReq)
    SIG_DEF ( SIG_APEX_ST_INFORM_SIM_APP_ERR_IND,           ApexStInformSimAppErrInd     apexStInformSimAppErrInd)
    SIG_DEF ( SIG_APEX_ST_DISPLAY_ALPHA_ID_REQ,             ApexStDisplayAlphaIdReq      apexStDisplayAlphaIdReq)
    SIG_DEF ( SIG_APEX_ST_DISPLAY_ALPHA_ID_IND,             ApexStDisplayAlphaIdInd      apexStDisplayAlphaIdInd)
    SIG_DEF ( SIG_APEX_ST_PROACTIVE_SESSION_END_REQ,        EmptySignal                  apexStProactiveSessionEndReq)
    SIG_DEF ( SIG_APEX_ST_PROACTIVE_SESSION_END_IND,        EmptySignal                  apexStProactiveSessionEndInd)

    SIG_DEF ( SIG_APEX_ST_SM_CONTROL_IND,                   ApexStSmControlInd           apexStSmControlInd)
    SIG_DEF ( SIG_APEX_ST_SM_CONTROL_RSP,                   ApexStSmControlRsp           apexStSmControlRsp)

    SIG_DEF ( SIG_APEX_ST_CANCEL_COMMAND_REQ,               ApexStCancelCommandReq       apexStCancelCommandReq)
    SIG_DEF ( SIG_APEX_ST_CANCEL_COMMAND_IND,               ApexStCancelCommandInd       apexStCancelCommandInd)
    SIG_DEF ( SIG_APEX_ST_CANCEL_COMMAND_RSP,               ApexStCancelCommandRsp       apexStCancelCommandRsp)
    SIG_DEF ( SIG_APEX_ST_CANCEL_COMMAND_CNF,               ApexStCancelCommandCnf       apexStCancelCommandCnf)
    SIG_DEF ( SIG_APEX_ST_CC_PDP_CONTEXT_REQ,               ApexStCcPdpContextReq        apexStCcPdpContextReq)
    SIG_DEF ( SIG_APEX_ST_CC_PDP_CONTEXT_CNF,               ApexStCcPdpContextCnf        apexStCcPdpContextCnf)
    SIG_DEF ( SIG_APEX_ST_CC_PDN_CONTEXT_REQ,               ApexStCcPdnContextReq        apexStCcPdnContextReq)
    SIG_DEF ( SIG_APEX_ST_CC_PDN_CONTEXT_CNF,               ApexStCcPdnContextCnf        apexStCcPdnContextCnf)
/*=========================================================================*/
/* ABPD: BL PSD procedure signals                                          */

    /* ABPD signals related to PSD session management                          */
    SIG_DEF ( SIG_APEX_ABPD_DUMMY = ABPD_SIGNAL_BASE,   EmptySignal                abpdDummy)
    SIG_DEF ( SIG_APEX_ABPD_CHANNEL_ALLOC_REQ,          ApexAbpdChannelAllocReq    apexAbpdChannelAllocReq)
    SIG_DEF ( SIG_APEX_ABPD_CHANNEL_ALLOC_CNF,          ApexAbpdChannelAllocCnf    apexAbpdChannelAllocCnf)
    SIG_DEF ( SIG_APEX_ABPD_CHANNEL_FREE_REQ,           ApexAbpdChannelFreeReq     apexAbpdChannelFreeReq)

    SIG_DEF ( SIG_APEX_ABPD_ATTACH_DEF_BEARER_ACT_IND,  EmptySignal                apexAbpdAttachDefBearerActInd)
    SIG_DEF ( SIG_APEX_ABPD_DIAL_REQ,                   ApexAbpdDialReq            apexAbpdDialReq)
    SIG_DEF ( SIG_APEX_ABPD_ACTIVATE_DATA_CONN_REQ,     ApexAbpdActivateDataConnReq apexAbpdActivateDataConnReq)
    SIG_DEF ( SIG_APEX_ABPD_ACTIVATE_DATA_CONN_CNF,     ApexAbpdActivateDataConnCnf apexAbpdActivateDataConnCnf)
    SIG_DEF ( SIG_APEX_ABPD_CONTEXT_IND,                ApexAbpdContextInd         apexAbpdContextInd)
    SIG_DEF ( SIG_APEX_ABPD_CONNECT_IND,                ApexAbpdConnectInd         apexAbpdConnectInd)
    SIG_DEF ( SIG_APEX_ABPD_CONNECT_RSP,                ApexAbpdConnectRsp         apexAbpdConnectRsp)
    SIG_DEF ( SIG_APEX_ABPD_CONNECT_REJ,                ApexAbpdConnectRej         apexAbpdConnectRej)
    SIG_DEF ( SIG_APEX_ABPD_ERROR_IND,                  ApexAbpdErrorInd           apexAbpdErrorInd)
    SIG_DEF ( SIG_APEX_ABPD_CONNECTING_IND,             ApexAbpdConnectingInd      apexAbpdConnectingInd)
    SIG_DEF ( SIG_APEX_ABPD_CONNECTED_IND,              ApexAbpdConnectedInd       apexAbpdConnectedInd)
    SIG_DEF ( SIG_APEX_ABPD_PSD_BEARER_STATUS_IND,      ApexAbpdPsdBearerStatusInd apexAbpdPsdBearerStatusInd)
    SIG_DEF ( SIG_APEX_ABPD_DISCONNECTED_IND,           ApexAbpdDisconnectedInd    apexAbpdDisconnectedInd)
    SIG_DEF ( SIG_APEX_ABPD_BUSY_IND,                   ApexAbpdBusyInd            apexAbpdBusyInd)
    SIG_DEF ( SIG_APEX_ABPD_HANGUP_REQ,                 ApexAbpdHangupReq          apexAbpdHangupReq)
    SIG_DEF ( SIG_APEX_ABPD_PSD_MODIFY_REQ,             ApexAbpdPsdModifyReq       apexAbpdPsdModifyReq)
    SIG_DEF ( SIG_APEX_ABPD_PSD_MODIFY_CNF,             ApexAbpdPsdModifyCnf       apexAbpdPsdModifyCnf)
    SIG_DEF ( SIG_APEX_ABPD_PSD_MODIFY_REJ,             ApexAbpdPsdModifyRej       apexAbpdPsdModifyRej)
    SIG_DEF ( SIG_APEX_ABPD_PSD_MODIFY_IND,             ApexAbpdPsdModifyInd       apexAbpdPsdModifyInd)

    SIG_DEF ( SIG_APEX_ABPD_SETUP_IND,                  ApexAbpdSetupInd           apexAbpdSetupInd)
    SIG_DEF ( SIG_APEX_ABPD_SETUP_RSP,                  ApexAbpdSetupRsp           apexAbpdSetupRsp)

    SIG_DEF ( SIG_APEX_ABPD_PSD_ATTACH_REQ,             ApexAbpdPsdAttachReq       apexAbpdPsdAttachReq)
    SIG_DEF ( SIG_APEX_ABPD_PSD_ATTACH_CNF,             ApexAbpdPsdAttachCnf       apexAbpdPsdAttachCnf)
    SIG_DEF ( SIG_APEX_ABPD_PSD_DETACH_REQ,             ApexAbpdPsdDetachReq       apexAbpdPsdDetachReq)
    SIG_DEF ( SIG_APEX_ABPD_PSD_DETACH_CNF,             ApexAbpdPsdDetachCnf       apexAbpdPsdDetachCnf)

#if defined (FEA_PPP)
    /* ABPD signals related to PPP                                             */
    SIG_DEF ( SIG_APEX_ABPD_PPP_LOOPBACK_REQ,           ApexAbpdPppLoopbackReq     apexAbpdPppLoopbackReq)
    SIG_DEF ( SIG_APEX_ABPD_PPP_LOOPBACK_CNF,           ApexAbpdPppLoopbackCnf     apexAbpdPppLoopbackCnf)
    SIG_DEF ( SIG_APEX_ABPD_PPP_CONFIG_REQ,             ApexAbpdPppConfigReq       apexAbpdPppConfigReq)
    SIG_DEF ( SIG_APEX_ABPD_PPP_CONFIG_CNF,             ApexAbpdPppConfigCnf       apexAbpdPppConfigCnf)
    SIG_DEF ( SIG_APEX_ABPD_PPP_CONFIG_AUTH_REQ,        ApexAbpdPppConfigAuthReq   apexAbpdPppConfigAuthReq)
    SIG_DEF ( SIG_APEX_ABPD_PPP_CONFIG_AUTH_CNF,        ApexAbpdPppConfigAuthCnf   apexAbpdPppConfigAuthCnf)
#endif /* FEA_PPP */
    /* ABPD signals related to Release Assistance Indication                   */
    SIG_DEF ( SIG_APEX_ABPD_WRITE_REL_ASSIST_REQ,       ApexAbpdWriteRelAssistReq   apexAbpdWriteRelAssistReq)
    SIG_DEF ( SIG_APEX_ABPD_WRITE_REL_ASSIST_CNF,       ApexAbpdWriteRelAssistCnf   apexAbpdWriteRelAssistCnf)
    SIG_DEF ( SIG_APEX_ABPD_READ_REL_ASSIST_REQ,        ApexAbpdReadRelAssistReq    apexAbpdReadRelAssistReq)
    SIG_DEF ( SIG_APEX_ABPD_READ_REL_ASSIST_CNF,        ApexAbpdReadRelAssistCnf    apexAbpdReadRelAssistCnf)

    /* ABPD signals related to APN Data Type                                   */
    SIG_DEF ( SIG_APEX_ABPD_WRITE_APN_DATA_TYPE_REQ,    ApexAbpdWriteApnDataTypeReq apexAbpdWriteApnDataTypeReq)
    SIG_DEF ( SIG_APEX_ABPD_WRITE_APN_DATA_TYPE_CNF,    ApexAbpdWriteApnDataTypeCnf apexAbpdWriteApnDataTypeCnf)
    SIG_DEF ( SIG_APEX_ABPD_READ_APN_DATA_TYPE_REQ,     ApexAbpdReadApnDataTypeReq  apexAbpdReadApnDataTypeReq)
    SIG_DEF ( SIG_APEX_ABPD_READ_APN_DATA_TYPE_CNF,     ApexAbpdReadApnDataTypeCnf  apexAbpdReadApnDataTypeCnf)

    /* ABPD signals related to Mtu                                */
    SIG_DEF ( SIG_APEX_ABPD_MTU_IND,                    ApexAbpdMtuInd              apexAbpdMtuInd)

    /* ABPD signals related to APN Rate Control                                */
    SIG_DEF ( SIG_APEX_ABPD_APN_UL_RATE_CONTROL_IND,    ApexAbpdApnUlRateControlInd  apexAbpdApnUlRateControlInd)

    /* ABPD signals related to PLMN Rate Control                               */
    SIG_DEF ( SIG_APEX_ABPD_PLMN_UL_RATE_CONTROL_IND,   ApexAbpdPlmnUlRateControlInd apexAbpdPlmnUlRateControlInd)

    /* ABPD signals - other general ones (APN, ACL, counter management         */
    SIG_DEF( SIG_APEX_ABPD_REPORT_COUNTER_REQ,          ApexAbpdReportCounterReq  apexAbpdReportCounterReq)
    SIG_DEF( SIG_APEX_ABPD_REPORT_COUNTER_CNF,          ApexAbpdReportCounterCnf  apexAbpdReportCounterCnf)
    SIG_DEF( SIG_APEX_ABPD_RESET_COUNTER_REQ,           ApexAbpdResetCounterReq   apexAbpdResetCounterReq)
    SIG_DEF( SIG_APEX_ABPD_COUNTER_IND,                 ApexAbpdCounterInd        apexAbpdCounterInd)
    SIG_DEF( SIG_APEX_ABPD_APN_READ_REQ,                ApexAbpdApnReadReq        apexAbpdApnReadReq)
    SIG_DEF( SIG_APEX_ABPD_APN_READ_CNF,                ApexAbpdApnReadCnf        apexAbpdApnReadCnf)
    SIG_DEF( SIG_APEX_ABPD_APN_WRITE_REQ,               ApexAbpdApnWriteReq       apexAbpdApnWriteReq)
    SIG_DEF( SIG_APEX_ABPD_APN_WRITE_CNF,               ApexAbpdApnWriteCnf       apexAbpdApnWriteCnf)
    SIG_DEF( SIG_APEX_ABPD_LIST_ACL_REQ,                ApexAbpdListAclReq        apexAbpdListAclReq)
    SIG_DEF( SIG_APEX_ABPD_LIST_ACL_CNF,                ApexAbpdListAclCnf        apexAbpdListAclCnf)
    SIG_DEF( SIG_APEX_ABPD_WRITE_ACL_REQ,               ApexAbpdWriteAclReq       apexAbpdWriteAclReq)
    SIG_DEF( SIG_APEX_ABPD_WRITE_ACL_CNF,               ApexAbpdWriteAclCnf       apexAbpdWriteAclCnf)
    SIG_DEF( SIG_APEX_ABPD_DELETE_ACL_REQ,              ApexAbpdDeleteAclReq      apexAbpdDeleteAclReq)
    SIG_DEF( SIG_APEX_ABPD_DELETE_ACL_CNF,              ApexAbpdDeleteAclCnf      apexAbpdDeleteAclCnf)
    SIG_DEF( SIG_APEX_ABPD_SET_ACL_REQ,                 ApexAbpdSetAclReq         apexAbpdSetAclReq)
    SIG_DEF( SIG_APEX_ABPD_SET_ACL_CNF,                 ApexAbpdSetAclCnf         apexAbpdSetAclCnf)
    SIG_DEF( SIG_APEX_ABPD_ACL_STATUS_REQ,              ApexAbpdAclStatusReq      apexAbpdAclStatusReq)
    SIG_DEF( SIG_APEX_ABPD_ACL_STATUS_CNF,              ApexAbpdAclStatusCnf      apexAbpdAclStatusCnf)
    SIG_DEF( SIG_APEX_ABPD_PACKET_DISCARD_IND,          ApexAbpdPacketDiscardInd  apexAbpdPacketDiscardInd)

    /* ABPD signals related to STK call control of PSD sessions                */
    SIG_DEF( SIG_APEX_ABPD_STK_INFO_IND,                ApexAbpdStkInfoInd        apexAbpdStkInfoInd)
    SIG_DEF( SIG_APEX_ABPD_ATTACH_PDN_CONN_IND,         ApexAbpdAttachPdnConnInd  apexAbpdAttachPdnConnInd)

    /* ABPD signals related to tranmit data over control plane                */
    SIG_DEF( SIG_APEX_ABPD_TRANSMIT_DATA_IND,           ApexAbpdTransmitDataInd       apexAbpdTransmitDataInd)

    #ifdef ENABLE_ABPD_UNIT_TEST
    /* ABPD signals for Unit Testing                                           */
    SIG_DEF( SIG_ABPD_SET_UNIT_TEST_MODE_REQ,           AbpdSetUnitTestModeReq abpdSetUnitTestModeReq)
    #endif /* ENABLE_ABPD_UNIT_TEST */

    /* UPDIR */
    SIG_DEF( SIG_APEX_ABPD_SET_UPDIR_INFO_REQ,          ApexAbpdSetUpdirInfoReq       apexAbpdSetUpdirInfoReq)
    SIG_DEF( SIG_APEX_ABPD_SET_UPDIR_INFO_CNF,          ApexAbpdSetUpdirInfoCnf       apexAbpdSetUpdirInfoCnf)
    SIG_DEF( SIG_APEX_ABPD_RESET_UPDIR_INFO_REQ,        ApexAbpdResetUpdirInfoReq     apexAbpdResetUpdirInfoReq)
    SIG_DEF( SIG_APEX_ABPD_UPDIR_IND,                   ApexAbpdUpdirInd              apexAbpdUpdirInd)

#if defined (FEA_RPM)
    SIG_DEF ( SIG_ABRPM_DUMMY = ABRPM_SIGNAL_BASE,      EmptySignal                     abrpmDummy)
    SIG_DEF ( SIG_APEX_RPM_IND,                         ApexRpmInd                      apexRpmInd)
    SIG_DEF ( SIG_APEX_RPM_RESET_CHIPSET_IND,           ApexRpmResetChipsetInd          apexRpmResetChipsetInd)
    SIG_DEF ( SIG_APEX_RPM_READ_INFO_REQ,               ApexRpmReadInfoReq              apexRpmReadInfoReq)
    SIG_DEF ( SIG_APEX_RPM_READ_INFO_CNF,               ApexRpmReadInfoCnf              apexRpmReadInfoCnf)
#if defined (ENABLE_ABRPM_UNIT_TEST)
    SIG_DEF ( SIG_ABRPM_UT_PREPARE_FOR_INITIALIZE_REQ,  AbrpmUtPrepareForInitializeReq  abrpmUtPrepareForInitializeReq)
    SIG_DEF ( SIG_ABRPM_UT_INITIALIZE_REQ,              AbrpmUtInitializeReq            abrpmUtInitializeReq)
    SIG_DEF ( SIG_ABRPM_UT_CHECK_ACTION_REQ,            AbrpmUtCheckActionReq           abrpmUtCheckActionReq)
    SIG_DEF ( SIG_ABRPM_UT_CHECK_ACTION_CNF,            AbrpmUtCheckActionCnf           abrpmUtCheckActionCnf)
    SIG_DEF ( SIG_ABRPM_UT_HANDLE_NETWORK_RESPONSE_REQ, AbrpmUtHandleNetworkResponseReq abrpmUtHandleNetworkResponseReq)
    SIG_DEF ( SIG_ABRPM_UT_UPDATE_FOR_OTA_REQ,          AbrpmUtUpdateForOtaReq          abrpmUtUpdateForOtaReq)
    SIG_DEF ( SIG_ABRPM_UT_UPDATE_FOR_SIM_CHANGE_REQ,   AbrpmUtUpdateForSimChangeReq    abrpmUtUpdateForSimChangeReq)
    SIG_DEF ( SIG_ABRPM_UT_PREPARE_FOR_SLEEP_REQ,       AbrpmUtPrepareForSleepReq       abrpmUtPrepareForSleepReq)
    SIG_DEF ( SIG_ABRPM_UT_SET_KERNEL_TICK_REQ,         AbrpmUtSetKernelTickReq         abrpmUtSetKernelTickReq)
    SIG_DEF ( SIG_ABRPM_UT_SET_NVDM_RPM_PARAM_REQ,      AbrpmUtSetNvdmRpmParamReq       abrpmUtSetNvdmRpmParamReq)
    SIG_DEF ( SIG_ABRPM_UT_SET_NVDM_CHIP_RESET_INFO_REQ,AbrpmUtSetNvdmChipResetInfoReq  abrpmUtSetNvdmChipResetInfoReq)
    SIG_DEF ( SIG_ABRPM_UT_SET_NVDM_RPM_CONTEXT_REQ,    AbrpmUtSetNvdmRpmContextReq     abrpmUtSetNvdmRpmContextReq)
    SIG_DEF ( SIG_ABRPM_UT_TRANSIT_CONTEXT_STATE_IND,   AbrpmUtTransitContextStateInd   abrpmUtTransitContextStateInd)
    SIG_DEF ( SIG_ABRPM_UT_RESET_CHIPSET_CNF,           AbrpmUtResetChipsetCnf          abrpmUtResetChipsetCnf)
#endif /* ENABLE_ABRPM_UNIT_TEST */
#endif /* FEA_RPM */

#if defined (FEA_PPP)
#if 0 // !defined (EXCLUDE_PPPAB)
/*========================================================================*/
/* AL FL List                                                             */
    SIG_DEF( SIG_ABGP_PPP_DUMMY = GPABGPPPP_SIGNAL_BASE, EmptySignal          abgpPpp_dummy)
    SIG_DEF( SIG_ABGP_PPP_CONFIGURE_IND,         AbgpPppConfigureInd          abgpPppConfigureInd)
    SIG_DEF( SIG_ABGP_PPP_WRITE_CONFIGURE_REQ,   AbgpPppWriteConfigureReq     abgpPppWriteConfigureReq)
    SIG_DEF( SIG_ABGP_PPP_WRITE_CONFIGURE_CNF,   AbgpPppWriteConfigureCnf     abgpPppWriteConfigureCnf)
    SIG_DEF( SIG_ABGP_PPP_READ_CONFIGURE_REQ,    AbgpPppReadConfigureReq      abgpPppReadConfigureReq)
    SIG_DEF( SIG_ABGP_PPP_READ_CONFIGURE_CNF,    AbgpPppReadConfigureCnf      abgpPppReadConfigureCnf)
    SIG_DEF( SIG_ABGP_PPP_WRITE_AUTH_REQ,        AbgpPppWriteAuthReq          abgpPppWriteAuthReq)
    SIG_DEF( SIG_ABGP_PPP_WRITE_AUTH_CNF,        AbgpPppWriteAuthCnf          abgpPppWriteAuthCnf)
    SIG_DEF( SIG_ABGP_PPP_PAP_AUTH_REQ,          AbgpPppPapAuthReq            abgpPppPapAuthReq)
    SIG_DEF( SIG_ABGP_PPP_PAP_AUTH_CNF,          AbgpPppPapAuthCnf            abgpPppPapAuthCnf)
    SIG_DEF( SIG_ABGP_PPP_CHAP_AUTH_REQ,         AbgpPppChapAuthReq           abgpPppChapAuthReq)
    SIG_DEF( SIG_ABGP_PPP_CHAP_AUTH_CNF,         AbgpPppChapAuthCnf           abgpPppChapAuthCnf)
#endif
#endif /* FEA_PPP */


#if defined (UPGRADE_APP_INFO)
    /* Internal signals sent between application background procedures
     * ABAI (application information) and ABPS (positioning systems) */
    SIG_DEF( SIG_ABAI_PS_DATA_IND,                   AbaiPsDataInd                abaiPsDataInd)
    SIG_DEF( SIG_ABAI_PS_DATA_RSP,                   AbaiPsDataRsp                abaiPsDataRsp)
#if defined (UPGRADE_AGPS)
    SIG_DEF( SIG_APEX_AGPS_DATA_IND,                 ApexAgpsDataInd              ApexAgpsDataInd)
    SIG_DEF( SIG_APEX_AGPS_DATA_RSP,                 ApexAgpsDataRsp              ApexAgpsDataRsp)
#if defined  (ENABLE_AGPS_DEBUG)
    SIG_DEF( SIG_APEX_AGPS_STATE_IND,                ApexAgpsStateInd             apexAgpsStateInd)
    SIG_DEF( SIG_APEX_AGPS_POS_INSTRUCT_IND,         ApexAgpsPosInstructInd       apexAgpsPosInstructInd)
    SIG_DEF( SIG_APEX_AGPS_REF_ASSIST_DATA_IND,      ApexAgpsRefAssistDataInd     apexAgpsRefAssistDataInd)
    SIG_DEF( SIG_APEX_AGPS_MORE_ASSIST_DATA_IND,     ApexAgpsMoreAssistDataInd    apexAgpsMoreAssistDataInd)
    SIG_DEF( SIG_APEX_AGPS_REL5_ASSIST_DATA_EXT_IND, ApexAgpsRel5AssistDataExtInd apexAgpsRel5AssistDataExtInd)
    SIG_DEF( SIG_APEX_AGPS_EXT_CONTAINER_IND,        ApexAgpsExtContainerInd      apexAgpsExtContainerInd)
    SIG_DEF( SIG_APEX_AGPS_REF_TIME_IND,             ApexAgpsRefTimeInd           apexAgpsRefTimeInd)
    SIG_DEF( SIG_APEX_AGPS_REF_LOCATION_IND,         ApexAgpsRefLocationInd       apexAgpsRefLocationInd)
    SIG_DEF( SIG_APEX_AGPS_DGPS_CORR_IND,            ApexAgpsDgpsCorrInd          apexAgpsDgpsCorrInd)
    SIG_DEF( SIG_APEX_AGPS_NAV_MODEL_IND,            ApexAgpsNavModelInd          apexAgpsNavModelInd)
    SIG_DEF( SIG_APEX_AGPS_IONO_MODEL_IND,           ApexAgpsIonoModelInd         apexAgpsIonoModelInd)
    SIG_DEF( SIG_APEX_AGPS_UTC_MODEL_IND,            ApexAgpsUtcModelInd          apexAgpsUtcModelInd)
    SIG_DEF( SIG_APEX_AGPS_ALMANAC_IND,              ApexAgpsAlmanacInd           apexAgpsAlmanacInd)
    SIG_DEF( SIG_APEX_AGPS_ACQUIS_ASSIST_IND,        ApexAgpsAcquisAssistInd      apexAgpsAcquisAssistInd)
    SIG_DEF( SIG_APEX_AGPS_BAD_SV_SET_IND,           ApexAgpsBadSvSetInd          apexAgpsBadSvSetInd)
    SIG_DEF( SIG_APEX_AGPS_MULTIPLE_SETS_IND,        ApexAgpsMultipleSetsInd      apexAgpsMultipleSetsInd)
    SIG_DEF( SIG_APEX_AGPS_REFERENCE_IDENTITY_IND,   ApexAgpsReferenceIdentityInd apexAgpsReferenceIdentityInd)
    SIG_DEF( SIG_APEX_AGPS_LOCATION_INFO_IND,        ApexAgpsLocationInfoInd      apexAgpsLocationInfoInd)
    SIG_DEF( SIG_APEX_AGPS_GPS_MEASURE_INFO_IND,     ApexAgpsGpsMeasureInfoInd    apexAgpsGpsMeasureInfoInd)
    SIG_DEF( SIG_APEX_AGPS_GPS_LOCATION_ERROR_IND,   ApexAgpsLocationErrorInd     apexAgpsLocationErrorInd)
    SIG_DEF( SIG_APEX_AGPS_REL5_MSR_RESPONSE_EXT_IND,ApexAgpsRel5MsrResponseExtInd apexAgpsRel5MsrResponseExtInd)
#endif
#endif
#endif

#if defined (UPGRADE_3G)
# if defined (UPGRADE_3G_MBMS)
#  if defined (UPGRADE_3G_TDD128)
    SIG_DEF( SIG_ABEX_MBMS_DUMMY = APEXMBMS_SIGNAL_BASE, EmptySignal              apexMbms_dummy)
    SIG_DEF( SIG_APEX_MBMS_STATE_REQ,                ApexMbmsStateReq             apexMbmsStateReq)
    SIG_DEF( SIG_APEX_MBMS_STATE_CNF,                ApexMbmsStateCnf             apexMbmsStateCnf)
    SIG_DEF( SIG_APEX_MBMS_STATE_IND,                ApexMbmsStateInd             apexMbmsStateInd)
    SIG_DEF( SIG_APEX_MBMS_ACTIVATE_REQ,             ApexMbmsActivateReq          apexMbmsActivateReq)
    SIG_DEF( SIG_APEX_MBMS_ACTIVATE_CNF,             ApexMbmsActivateCnf          apexMbmsActivateCnf)
    SIG_DEF( SIG_APEX_MBMS_DEACTIVATE_REQ,           ApexMbmsDeactivateReq        apexMbmsDeactivateReq)
    SIG_DEF( SIG_APEX_MBMS_DEACTIVATE_IND,           ApexMbmsDeactivateInd        apexMbmsDeactivateInd)
    SIG_DEF( SIG_APEX_MBMS_DEACTIVATE_CNF,           EmptySignal                  apexMbmsDeactivateCnf)
    SIG_DEF( SIG_APEX_MBMS_KEY_TYPE_IND,             ApexMbmsKeyTypeInd           apexMbmsKeyTypeInd)
    SIG_DEF( SIG_APEX_MBMS_GBA_AUTH_REQ,             ApexMbmsGbaAuthReq           apexMbmsGbaAuthReq)
    SIG_DEF( SIG_APEX_MBMS_GBA_AUTH_CNF,             ApexMbmsGbaAuthCnf           apexMbmsGbaAuthCnf)
    SIG_DEF( SIG_APEX_MBMS_GBA_WRITE_REQ,            ApexMbmsGbaWriteReq          apexMbmsGbaWriteReq)
    SIG_DEF( SIG_APEX_MBMS_GBA_WRITE_CNF,            ApexMbmsGbaWriteCnf          apexMbmsGbaWriteCnf)
    SIG_DEF( SIG_APEX_MBMS_MRK_REQ,                  ApexMbmsMrkReq               apexMbmsMrkReq)
    SIG_DEF( SIG_APEX_MBMS_MRK_CNF,                  ApexMbmsMrkCnf               apexMbmsMrkCnf)
    SIG_DEF( SIG_APEX_MBMS_MSK_UPDATE_REQ,           ApexMbmsMskUpdateReq         apexMbmsMskUpdateReq)
    SIG_DEF( SIG_APEX_MBMS_MSK_UPDATE_CNF,           ApexMbmsMskUpdateCnf         apexMbmsMskUpdateCnf)
    SIG_DEF( SIG_APEX_MBMS_MTK_UPDATE_REQ,           ApexMbmsMtkUpdateReq         apexMbmsMtkUpdateReq)
    SIG_DEF( SIG_APEX_MBMS_MTK_UPDATE_CNF,           ApexMbmsMtkUpdateCnf         apexMbmsMtkUpdateCnf)
    SIG_DEF( SIG_APEX_MBMS_SECURE_CONTROL_REQ,       ApexMbmsSecureControlReq     apexMbmsSecureControlReq)
    SIG_DEF( SIG_APEX_MBMS_SECURE_CONTROL_CNF,       ApexMbmsSecureControlCnf     apexMbmsSecureControlCnf)
    SIG_DEF( SIG_APEX_MBMS_USER_SETTING_REQ,         ApexMbmsUserSettingReq       apexMbmsUserSettingReq)
    SIG_DEF( SIG_APEX_MBMS_USER_SETTING_CNF,         ApexMbmsUserSettingCnf       apexMbmsUserSettingCnf)
    SIG_DEF( SIG_APEX_ABMBMS_RELEASE_REQ,            ApexAbMbmsReleaseReq         apexAbMbmsReleaseReq)
    SIG_DEF( SIG_APEX_ABMBMS_SETUP_REQ,              ApexAbMbmsSetupReq           apexAbMbmsSetupReq)
    SIG_DEF( SIG_APEX_ABMBMS_SETUP_CNF,              ApexAbMbmsSetupCnf           apexAbMbmsSetupCnf)
    SIG_DEF( SIG_APEX_ABMBMS_SETUP_REJ,              ApexAbMbmsSetupRej           apexAbMbmsSetupRej)
#  endif /* UPGRADE_3G_TDD128 */
# endif /* UPGRADE_3G_MBMS */
#endif /* UPGRADE_3G */

/* END OF FILE */

