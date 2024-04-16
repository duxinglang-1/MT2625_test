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
 * File Description: Signal Id to structure mapping for U1 signals
 **************************************************************************/

#if !defined (EXCLUDE_U1CD)
    /* U1CD Signals */
    SIG_DEF( SIG_U1CD_DUMMY = U1CD_SIGNAL_BASE,     EmptySignal                     u1cdDummy)
    SIG_DEF( SIG_U1CD_ENTER_REQ,                    U1cdEnterReq                    u1cdEnterReq)
    SIG_DEF( SIG_U1CD_ENTER_CNF,                    U1cdEnterCnf                    u1cdEnterCnf)
    SIG_DEF( SIG_U1CD_EXIT_REQ,                     U1cdExitReq                     u1cdExitReq)
    SIG_DEF( SIG_U1CD_EXIT_CNF,                     U1cdExitCnf                     u1cdExitCnf)
    SIG_DEF( SIG_U1CD_PHY_TEST_REQ,                 U1cdPhyTestReq                  u1cdPhyTestReq)
    SIG_DEF( SIG_U1CD_PHY_TEST_CNF,                 U1cdPhyTestCnf                  u1cdPhyTestCnf)
    SIG_DEF( SIG_U1CD_PHY_TEST_IND,                 U1cdPhyTestInd                  u1cdPhyTestInd)
    SIG_DEF( SIG_U1CD_HARDWARE_VERSION_REQ,         EmptySignal                     u1cdHardwareVersionReq)
    SIG_DEF( SIG_U1CD_HARDWARE_VERSION_CNF,         U1cdHardwareVersionCnf          u1cdHardwareVersionCnf)
    SIG_DEF( SIG_U1CD_TX_PWR_RMP_CAL_REQ,           U1cdTxPwrRmpCalReq              u1cdTxPwrRmpCalReq)
    SIG_DEF( SIG_U1CD_TX_PWR_RMP_CAL_CNF,           U1cdTxPwrRmpCalCnf              u1cdTxPwrRmpCalCnf)
    SIG_DEF( SIG_U1CD_SLEEP_OFF_REQ,                U1cdSleepOffReq                 u1cdSleepOffReq)
    SIG_DEF( SIG_U1CD_SLEEP_OFF_CNF,                EmptySignal                     u1cdSleepOffCnf)
    SIG_DEF( SIG_U1CD_AGC_CAL_REQ,                  U1cdAgcCalReq                   u1cdAgcCalReq)
    SIG_DEF( SIG_U1CD_AGC_CAL_CNF,                  U1cdAgcCalCnf                   u1cdAgcCalCnf)
    SIG_DEF( SIG_U1CD_SET_AFC_DAC_REQ,              U1cdSetAfcDacReq                u1cdSetAfcDacReq)
    SIG_DEF( SIG_U1CD_SET_AFC_DAC_CNF,              U1cdSetAfcDacCnf                u1cdSetAfcDacCnf)
    SIG_DEF( SIG_U1CD_READ_IQ_BUFFER_REQ,           U1cdReadIqBufferReq             u1cdReadIqBufferReq)
    SIG_DEF( SIG_U1CD_READ_IQ_BUFFER_CNF,           U1cdReadIqBufferCnf             u1cdReadIqBufferCnf)
    SIG_DEF( SIG_U1CD_SET_AFC_DAC_RX_REQ,           U1cdSetAfcDacRxReq              u1cdSetAfcDacRxReq)
    SIG_DEF( SIG_U1CD_SET_AFC_DAC_RX_CNF,           U1cdSetAfcDacRxCnf              u1cdSetAfcDacRxCnf)
    SIG_DEF( SIG_U1CD_ARB_DBG_REQ,                  U1cdArbDbgReq                   u1cdArbDbgReq)
    SIG_DEF( SIG_U1CD_ARB_DBG_CNF,                  U1cdArbDbgCnf                   u1cdArbDbgCnf)
    SIG_DEF( SIG_U1CD_RAD_DBG_REQ,                  U1cdRadDbgReq                   u1cdRadDbgReq)
    SIG_DEF( SIG_U1CD_RAD_DBG_CNF,                  U1cdRadDbgCnf                   u1cdRadDbgCnf)
    SIG_DEF( SIG_U1CD_WRITE_PA_DELTA_POWER_REQ,     U1cdWritePADeltaPowerReq        u1cdWritePADeltaPowerReq)
    SIG_DEF( SIG_U1CD_WRITE_PA_DELTA_POWER_CNF,     U1cdWritePADeltaPowerCnf        u1cdWritePADeltaPowerCnf)
    SIG_DEF( SIG_U1CD_READ_PA_DELTA_POWER_REQ,      U1cdReadPADeltaPowerReq         u1cdReadPADeltaPowerReq)
    SIG_DEF( SIG_U1CD_READ_PA_DELTA_POWER_CNF,      U1cdReadPADeltaPowerCnf         u1cdReadPADeltaPowerCnf)
    SIG_DEF( SIG_U1CD_READ_SAR_ADC_REQ,             U1cdReadSarAdcReq               u1cdReadSarAdcReq)
    SIG_DEF( SIG_U1CD_READ_SAR_ADC_CNF,             U1cdReadSarAdcCnf               u1cdReadSarAdcCnf)    
#if defined (UPGRADE_3G_TDD128)
    SIG_DEF( SIG_U1CD_WRITE_PA_TIME_REQ,            U1cdWritePATimeReq              u1cdWritePATimeReq)
    SIG_DEF( SIG_U1CD_WRITE_PA_TIME_CNF,            U1cdWritePATimeCnf              u1cdWritePATimeCnf)
    SIG_DEF( SIG_U1CD_READ_PA_TIME_REQ,             U1cdReadPATimeReq               u1cdReadPATimeReq)
    SIG_DEF( SIG_U1CD_READ_PA_TIME_CNF,             U1cdReadPATimeCnf               u1cdReadPATimeCnf)
#endif

#if defined (UPGRADE_GGE)
    SIG_DEF( SIG_U1CD_SET_GIT_STATE_REQ,            U1cdSetGitStateReq              u1cdSetGitStateReq)
    SIG_DEF( SIG_U1CD_SET_GIT_STATE_PARAMS_REQ,     U1cdSetGitStateParamsReq        u1cdSetGitStateParamsReq)
    SIG_DEF( SIG_U1CD_GIT_SUSPEND_RESUME_REQ,       U1cdGitSuspendResumeReq         u1cdGitSuspendResumeReq)
#endif

#if defined (T1_ENABLE_RADTEST)
    SIG_DEF( SIG_U1CD_SET_RAD_REG_REQ,              U1cdSetRadRegReq                u1cdSetRadRegReq)
    SIG_DEF( SIG_U1CD_SET_RAD_REG_CNF,              U1cdSetRadRegCnf                u1cdSetRadRegCnf)
    SIG_DEF( SIG_U1CD_SET_RAD_GPO_REQ,              U1cdSetRadGpoReq                u1cdSetRadGpoReq)
    SIG_DEF( SIG_U1CD_SET_RAD_GPO_CNF,              U1cdSetRadGpoCnf                u1cdSetRadGpoCnf)
#endif
#if defined (UPGRADE_UCIPHER_UNIT_TEST)
    SIG_DEF( SIG_U1CD_CIPHER_UNIT_TEST_REQ,         U1cdCipherUnitTestReq           u1cdCipherUnitTestReq)
    SIG_DEF( SIG_U1CD_CIPHER_UNIT_TEST_FAIL_IND,    U1cdCipherUnitTestFailInd       u1cdCipherUnitTestFailInd)
    SIG_DEF( SIG_U1CD_CIPHER_UNIT_TEST_SUCCESS_IND, U1cdCipherUnitTestSuccessInd    u1cdCipherUnitTestSuccessInd)
#endif /* UPGRADE_UCIPHER_UNIT_TEST */
#if defined (RADIO_3G_TDD_IS_TITAN)
    SIG_DEF( SIG_U1CD_READ_TITAN_IQ_NATIVE_REQ,     EmptySignal                     u1cdReadTITANIQNativeReq)
    SIG_DEF( SIG_U1CD_READ_TITAN_IQ_NATIVE_CNF,     U1cdReadTITANIQNativeCnf        u1cdReadTITANIQNativeCnf)
    SIG_DEF( SIG_U1CD_READ_TITAN_TEMPERATURE_REQ,   U1cdReadTITANTemperatureReq     u1cdReadTITANTemperatureReq)
    SIG_DEF( SIG_U1CD_READ_TITAN_TEMPERATURE_CNF,   U1cdReadTITANTemperatureCnf     u1cdReadTITANTemperatureCnf)
    SIG_DEF( SIG_U1CD_WRITE_TITAN_DCOC_REQ,         U1cdWriteTITANDCOCReq           u1cdWriteTITANDCOCReq)
    SIG_DEF( SIG_U1CD_WRITE_TITAN_DCOC_CNF,         EmptySignal                     u1cdWriteTITANDCOCCnf)
#endif

#if defined (ENABLE_FCS_TEST_HARNESS)
    SIG_DEF( SIG_U1CD_FCS_TEST_HARNESS_REQ,         U1cdFcsTestHarnessReq           u1cdFcsTestHarnessReq)
#endif /* ENABLE_FCS_TEST_HARNESS */

    SIG_DEF( SIG_U1CD_FASTCAL_STATE_REQ,            U1cdFastCalStateReq             u1cdFastCalStateReq )
    SIG_DEF( SIG_U1CD_FASTCAL_STATE_CNF,            U1cdFastCalStateCnf             u1cdFastCalStateCnf )
    SIG_DEF( SIG_U1CD_FASTCAL_CHANNELS_REQ,         U1cdFastCalChannelsReq          u1cdFastCalChannelsReq )
    SIG_DEF( SIG_U1CD_FASTCAL_CHANNELS_CNF,         U1cdFastCalChannelsCnf          u1cdFastCalChannelsCnf )
    SIG_DEF( SIG_U1CD_FASTCAL_TX_STEPS_REQ,         U1cdFastCalTxStepsReq           u1cdFastCalTxStepsReq )
    SIG_DEF( SIG_U1CD_FASTCAL_TX_STEPS_CNF,         U1cdFastCalTxStepsCnf           u1cdFastCalTxStepsCnf )
    SIG_DEF( SIG_U1CD_FASTCAL_RX_STEPS_REQ,         U1cdFastCalRxStepsReq           u1cdFastCalRxStepsReq )
    SIG_DEF( SIG_U1CD_FASTCAL_RX_STEPS_CNF,         U1cdFastCalRxStepsCnf           u1cdFastCalRxStepsCnf )
    SIG_DEF( SIG_U1CD_FASTCAL_TX_MEAS_IND,          U1cdFastCalTxMeasInd            u1cdFastCalTxMeasInd  )
    SIG_DEF( SIG_U1CD_FASTCAL_RX_MEAS_IND,          U1cdFastCalRxMeasInd            u1cdFastCalRxMeasInd  )

#endif /* !EXCLUDE_U1CD */

#if !defined (EXCLUDE_U1IT)
    /* U1IT Signals */
    SIG_DEF( SIG_W1IT_DUMMY = U1IT_SIGNAL_BASE,     EmptySignal                     w1itDummy)
    SIG_DEF( SIG_U1IT_READ_SMQU_IND,                EmptySignal                     u1itReadSmquInd)
    SIG_DEF( SIG_U1IT_READ_SMTD_IND,                EmptySignal                     u1itReadSmtdInd)

#if defined (UPGRADE_GGE)
    SIG_DEF( SIG_U1IT_MODE_CHANGE_IND,              U1itModeChangeInd               u1itModeChangeInd)
#endif
#if defined (DEVELOPMENT_VERSION) && defined(ENABLE_DCD_RECORD)
    SIG_DEF( SIG_U1IT_START_DCD_RECORD_REQ,         U1itStartDcdRecordReq           u1itStartDcdRecordReq)
    SIG_DEF( SIG_U1IT_STOP_DCD_RECORD_REQ,          U1itStopDcdRecordReq            u1itStopDcdRecordReq)
    SIG_DEF( SIG_U1IT_INIT_DCD_REQ,                 U1itInitDcdReq                  u1itInitDcdReq)
    SIG_DEF( SIG_U1IT_RESET_VPK_REQ,                U1itResetVpkReq                 u1itResetVpkReq)
#endif

#if defined (UPGRADE_XO_ATC)
    SIG_DEF( SIG_U1IT_XO_ATC_SET_MODEL_REQ,         U1itXoAtcSetModelReq            u1itXoAtcSetModelReq)
    SIG_DEF( SIG_U1IT_XO_ATC_TRAINING_DATA_IND,     U1itXoAtcTrainingDataInd        u1itXoAtcTrainingDataInd)
#endif /* UPGRADE_XO_ATC */

#if defined (DEVELOPMENT_VERSION)
#if defined (UPGRADE_3G_FDD)
    /* Development Signals. */
    SIG_DEF( SIG_W1IT_PSCH_AFC_IND,                 W1itPschAfcInd                  w1itPschAfcInd)
    SIG_DEF( SIG_W1IT_PSCH_DETECT_IND,              W1itPschDetectInd               w1itPschDetectInd)
    SIG_DEF( SIG_W1IT_CODE_GROUP_DETECT_IND,        W1itCodeGroupDetectInd          w1itCodeGroupDetectInd)
    SIG_DEF( SIG_W1IT_SCRAMBLING_CODE_IDENTIFY_IND, W1itScramblingCodeIdentifyInd   w1itScramblingCodeIdentifyInd)
    SIG_DEF( SIG_W1IT_SCRAMBLING_CODE_VERIFY_SUCESS_IND,   W1itScramblingCodeVerifySucessInd     w1itScramblingCodeVerifySucessInd)
    SIG_DEF( SIG_W1IT_SCRAMBLING_CODE_VERIFY_FAIL_IND,W1itScramblingCodeVerifyFailInd w1itScramblingCodeVerifyFailInd)
#if defined(UPGRADE_FDD_CSM)
    SIG_DEF( SIG_W1IT_SCRAMBLING_CODE_VERIFY_IND,   W1itScramblingCodeVerifyInd     w1itScramblingCodeVerifyInd)
#endif
    SIG_DEF( SIG_W1IT_RAKE_PATH_INFO_IND,           W1itRakePathInfoInd             w1itRakePathInfoInd)
    SIG_DEF( SIG_W1IT_TIMEBASE_ALIGN_IND,           W1itTimebaseAlignInd            w1itTimebaseAlignInd)
    SIG_DEF( SIG_W1IT_TIMEBASE_PATH_TRACK_IND,      W1itTimebasePathTrackInd        w1itTimebasePathTrackInd)
    SIG_DEF( SIG_W1IT_TIMEBASE_DL_TRACK_IND,        W1itTimebaseDlTrackInd          w1itTimebaseDlTrackInd)
    SIG_DEF( SIG_W1IT_TIMEBASE_UL_TRACK_IND,        W1itTimebaseUlTrackInd          w1itTimebaseUlTrackInd)
    SIG_DEF( SIG_W1IT_DL_TFCS_IND,                  W1itDlTfcsInd                   w1itDlTfcsInd)
    SIG_DEF( SIG_W1IT_UL_TRCH_IND,                  W1itUlTrchInd                   w1itUlTrchInd)
    SIG_DEF( SIG_W1IT_DL_TRCH_IND,                  W1itDlTrchInd                   w1itDlTrchInd)
    SIG_DEF( SIG_W1IT_INTRA_FREQ_CELL_INFO_IND,     W1itIntraFreqCellInfoInd        w1itIntraFreqCellInfoInd)
    SIG_DEF( SIG_W1IT_INTER_FREQ_CELL_INFO_IND,     W1itInterFreqCellInfoInd        w1itInterFreqCellInfoInd)
    SIG_DEF( SIG_W1IT_CELL_PTR_LIST_IND,            W1itCellPtrListInd              w1itCellPtrListInd)
    SIG_DEF( SIG_W1IT_CELL_VER_LIST_IND,            W1itCellVerListInd              w1itCellVerListInd)
    SIG_DEF( SIG_W1IT_PHCH_RECONFIG_IND,            W1itPhChReconfigInd             w1itPhChReconfigInd)
    SIG_DEF( SIG_W1IT_TRCH_RECONFIG_IND,            W1itTrChReconfigInd             w1itTrChReconfigInd)
    SIG_DEF( SIG_W1IT_MEAS_OCCASION_IND,            W1itMeasOccasionInd             w1itMeasOccasionInd)
    SIG_DEF( SIG_W1IT_CBS_OCCASION_IND,             W1itCbsOccasionInd              w1itCbsOccasionInd)
    SIG_DEF( SIG_W1IT_TGPS_IND,                     W1itTgpsInd                     w1itTgpsInd)
    SIG_DEF( SIG_W1IT_TG_SCHED_IND,                 W1itTgSchedInd                  w1itTgSchedInd)
    SIG_DEF( SIG_W1IT_PCH_SCHED_IND,                W1itPchSchedInd                 w1itPchSchedInd)
    SIG_DEF( SIG_W1IT_RACH_SCHED_IND,               W1itRachSchedInd                w1itRachSchedInd)
    SIG_DEF( SIG_W1IT_DEBUG1_IND,                   W1itDebug1Ind                   w1itDebug1Ind)
    SIG_DEF( SIG_W1IT_DEBUG2_IND,                   W1itDebug2Ind                   w1itDebug2Ind)
    SIG_DEF( SIG_W1IT_DEBUG3_IND,                   W1itDebug3Ind                   w1itDebug3Ind)
    SIG_DEF( SIG_W1IT_DEBUG4_IND,                   W1itDebug4Ind                   w1itDebug4Ind)
    SIG_DEF( SIG_W1IT_DEBUG_GEN_STRUCT_IND,         W1itDebugGenStructInd           w1itDebugGenStructInd)
    SIG_DEF( SIG_W1IT_PICH_IND,                     W1itPichInd                     w1itPichInd)
    SIG_DEF( SIG_W1IT_AICH_IND,                     W1itAichInd                     w1itAichInd)
    SIG_DEF( SIG_W1IT_PDU_LIST_IND,                 W1itPduListInd                  w1itPduListInd)
    SIG_DEF( SIG_W1IT_INT_TRACE_IND,                W1itIntTraceInd                 w1itIntTraceInd)
    SIG_DEF( SIG_W1IT_DEBUG_3_PARAM_IND,            W1itDebug3ParamInd              w1itDebug3ParamInd)
    SIG_DEF( SIG_W1IT_DEBUG_6_PARAM_IND,            W1itDebug6ParamInd              w1itDebug6ParamInd)
    SIG_DEF( SIG_W1IT_MPS_PAM_IND,                  W1itMpsPamInd                   w1itMpsPamInd)
    SIG_DEF( SIG_W1IT_TRCHOUT_IND,                  W1itTrChOutInd                  w1itTrChOutInd)
    SIG_DEF( SIG_W1IT_TPC_IND,                      W1itTpcInd                      w1itTpcInd)
    SIG_DEF( SIG_W1IT_AFC_IND,                      W1itAfcInd                      w1itAfcInd)
    SIG_DEF( SIG_W1IT_MPS_SUCCESS_IND,              W1itMpsSuccessInd               w1itMpsSuccessInd)
    SIG_DEF( SIG_W1IT_MPS_FAIL_IND,                 W1itMpsFailInd                  w1itMpsFailInd)
    SIG_DEF( SIG_W1IT_SFN_DETECT_SUCCESS_IND,       W1itSfnDetectSuccessInd         w1itSfnDetectSuccessInd)
    SIG_DEF( SIG_W1IT_SFN_DETECT_FAIL_IND,          W1itSfnDetectFailInd            w1itSfnDetectFailInd)
    SIG_DEF( SIG_W1IT_BCH_SCHED_IND,                W1itBchSchedInd                 w1itBchSchedInd)
    SIG_DEF( SIG_W1IT_BAD_CRC_IND,                  W1itBadCrcInd                   w1itBadCrcInd)
    SIG_DEF( SIG_W1IT_SIM_SLOW_CLOCK_WAKEUP,        W1itSimSlowClockWakeupInd       w1itSimSlowClockWakeupInd)
    SIG_DEF( SIG_W1IT_TFCI_FAIL_IND,                W1itTfciFailInd                 w1itTfciFailInd)
    SIG_DEF( SIG_W1IT_TXON_IND,                     W1itTxOnInd                     w1itTxOnInd)
    SIG_DEF( SIG_W1IT_TXOFF_IND,                    W1itTxOffInd                    w1itTxOffInd)
    SIG_DEF( SIG_W1IT_ALARM_IND,                    W1itAlarmInd                    w1itAlarmInd)
    SIG_DEF( SIG_W1IT_HSDPA_STAT_IND,               W1itHsdpaStatInd                w1itHsdpaStatInd)
    SIG_DEF( SIG_W1IT_HSUPA_STAT_IND,               W1itHsupaStatInd                w1itHsupaStatInd)
    SIG_DEF( SIG_W1IT_SYNCHRO_GENIE_IND,            W1itSynchroGenieInd             w1itSynchroGenieInd)
    SIG_DEF( SIG_W1IT_DL_TPC_IND,                   W1itDlTpcInd                    w1itDlTpcInd)
#if defined (UPGRADE_GGE)
    SIG_DEF( SIG_W1IT_GSM_CELL_INFO_IND,               W1itGsmCellInfoInd              w1itGsmCellInfoInd)
    SIG_DEF( SIG_W1IT_GSMIN3G_SQ_RSSI_STT_IND,         EmptySignal                     w1itGsmRssiSeqStartInd)
    SIG_DEF( SIG_W1IT_GSMIN3G_SQ_RSSI_END_IND,         EmptySignal                     w1itGsmRssiSeqEndInd)
    SIG_DEF( SIG_W1IT_GSMIN3G_L1_INIT_IND,             W1itL1MmGsmL1InitInd           w1itL1MmGsmL1InitInd)
    SIG_DEF( SIG_W1IT_GSMIN3G_BA_LIST_IND,             W1itL1MmGsmBaListDataInd       w1itL1MmGsmBaListDataInd)
    SIG_DEF( SIG_W1IT_GSMIN3G_BSIC_DECODE_LIST_IND,    W1itL1MmGsmBsicDecodeListInd   w1itL1MmGsmBsicDecodeListInd)
    SIG_DEF( SIG_W1IT_GSMIN3G_DCH_MEAS_START_IND,      W1itL1MmDchGsmMeasStartInd     w1itL1MmDchGsmMeasStartInd)
    SIG_DEF( SIG_W1IT_GSMIN3G_DCH_MEAS_STOP_IND,       W1itL1MmDchGsmMeasStopInd      w1itL1MmDchGsmMeasStopInd)
    SIG_DEF( SIG_W1IT_GSMIN3G_DCH_MEAS_PERIOD_IND,     W1itL1MmDchGsmMeasPeriodSttInd w1itL1MmDchGsmMeasPeriodSttInd)
    SIG_DEF( SIG_W1IT_GSMIN3G_DCH_CM_GAP_INFO_IND,     W1itL1MmCmGapInfoInd           w1itL1MmCmGapInfoInd)
    SIG_DEF( SIG_W1IT_GSMIN3G_L1MM_GSM_RSSI_RES_IND,  W1itL1MmGsmRssiDataInd         w1itL1MmGsmRssiDataInd)
    SIG_DEF( SIG_W1IT_GSMIN3G_L1MM_GSM_BSIC_RES_IND,  W1itL1MmGsmBsicDataInd         w1itL1MmGsmBsicDataInd)
    SIG_DEF( SIG_W1IT_GSMIN3G_SQ_GSM_FB_SB_STT_IND,    W1itGsmFbSbSeqSttInd            w1itGsmFbSbSeqSttInd)
    SIG_DEF( SIG_W1IT_GSMIN3G_SQ_GSM_FB_SB_END_IND,    W1itGsmFbSbSeqEndInd            w1itGsmFbSbSeqEndInd)
    SIG_DEF( SIG_W1IT_GSMIN3G_FACH_MO_GAP_INFO_IND,    W1itU1MmMoGapInfoInd           w1itU1MmMoGapInfoInd)
    SIG_DEF( SIG_W1IT_GSMIN3G_FACH_MEAS_START_IND,     W1itU1MmFachGsmMeasSttInd      w1itU1MmFachGsmMeasSttInd)
    SIG_DEF( SIG_W1IT_GSMIN3G_FACH_MEAS_STOP_IND,      EmptySignal                     w1itU1MmFachGsmMeasStopInd)
    SIG_DEF( SIG_W1IT_GSMIN3G_FACH_MEAS_PERIOD_IND,    W1itU1MmFachGsmMeasPeriodInd   w1itU1MmFachGsmMeasPeriodInd)
    SIG_DEF( SIG_W1IT_GSMIN3G_PAG_MEAS_START_IND,      W1itU1MmPagingGsmMeasStartInd  w1itU1MmPagingGsmMeasStartInd)
    SIG_DEF( SIG_W1IT_GSMIN3G_PAG_MEAS_STOP_IND,       EmptySignal                     w1itU1MmPagingGsmMeasStopInd)
    SIG_DEF( SIG_W1IT_GSMIN3G_OPT_GAP_IND,             W1itU1MmGsmOptionalGapInd      w1itU1MmGsmOptionalGapInd)
    SIG_DEF( SIG_W1IT_GSMIN3G_OPT_GAP_START_IND,       W1itU1MmGsmOptionalGapStartInd w1itU1MmGsmOptionalGapStartInd)
    SIG_DEF( SIG_W1IT_GSMIN3G_STOP_ACTION_IND,         EmptySignal                     w1itU1MmStopActionInd)
    SIG_DEF( SIG_W1IT_GSMIN3G_OPT_GAP_COMP_IND,        W1itL1MmGsmOptGapCompInd       w1itL1MmGsmOptGapCompInd)
    SIG_DEF( SIG_W1IT_GSMIN3G_TST_NCELL_DEBUG_IND,     W1itL1TstGsmNcellDebugInd       w1itL1TstGsmNcellDebugInd)
    SIG_DEF( SIG_W1IT_GSMIN3G_SQ_PLMN_RSSI_STT_IND,    EmptySignal                     w1itGsmPlmnRssiSeqSttInd)
    SIG_DEF( SIG_W1IT_GSMIN3G_SQ_PLMN_RSSI_END_IND,    EmptySignal                     w1itGsmPlmnRssiSeqEndInd)
    SIG_DEF( SIG_W1IT_GSMIN3G_SQ_GSM_BCCH_STT_IND,     EmptySignal                     w1itGsmBcchSeqSttInd)
    SIG_DEF( SIG_W1IT_GSMIN3G_SQ_GSM_BCCH_END_IND,     EmptySignal                     w1itGsmBcchSeqEndInd)
#endif /* UPGRADE_GGE */
#endif /*UPGRADE_3G_FDD*/
#endif
#if defined (DEVELOPMENT_VERSION)||defined(T1_UPHY_STUB_ENABLE)
SIG_DEF( SIG_U1IT_PHY_DL_DATA_STATUS_IND,       U1itPhyDlDataStatusInd          u1itPhyDlDataStatusInd)
SIG_DEF( SIG_U1IT_PHY_UL_DATA_STATUS_IND,       U1itPhyUlDataStatusInd          u1itPhyUlDataStatusInd)
#endif
#if defined (UPGRADE_3G_TDD128)
    SIG_DEF (SIG_T1IT_CRC_STAT_IND,                    T1itCrcStatInd                  t1itCrcStatInd)
    SIG_DEF (SIG_T1IT_MEAS_IND,                        T1itMeasInd                     t1itMeasInd)
    SIG_DEF (SIG_T1IT_DEBUG1_IND,                      T1itDebug1Ind                   t1itDebug1Ind)
    SIG_DEF (SIG_T1IT_DEBUG2_IND,                      T1itDebug2Ind                   t1itDebug2Ind)
    SIG_DEF (SIG_T1IT_DEBUG3_IND,                      T1itDebug3Ind                   t1itDebug3Ind)
    SIG_DEF (SIG_T1IT_DEBUG4_IND,                      T1itDebug4Ind                   t1itDebug4Ind)
    SIG_DEF (SIG_T1IT_SYNC_DL_DETECT_IND,              T1itSyncDlDetectInd             t1itSyncDlDetectInd)
    SIG_DEF (SIG_T1IT_MIDAMBLE_DETECT_IND,             T1itMidambleDetectInd           t1itMidambleDetectInd)
    SIG_DEF (SIG_T1IT_CELLID_DETECT_IND,               T1itCellIdDetectInd             t1itCellIdDetectInd)
    SIG_DEF (SIG_T1IT_DL_TRACKING_IND,                 T1itDlTrackingInd               t1itDlTrackingInd)
    SIG_DEF (SIG_T1IT_TIME_ADVANCE_IND,                T1itTimeAdvanceInd              t1itTimeAdvanceInd)
    SIG_DEF (SIG_T1IT_PHCH_RECONFIG_IND,               T1itPhChReconfigInd             t1itPhChReconfigInd)
    SIG_DEF (SIG_T1IT_FACH_MEAS_OCCASION_IND,          T1itFachMeasOccasionInd         t1itFachMeasOccasionInd)
    SIG_DEF (SIG_T1IT_TPC_IND,                         T1itTpcInd                      t1itTpcInd)
    SIG_DEF (SIG_T1IT_SIR_TARGET_IND,                  T1itSirTargetInd                t1itSirTargetInd)
    SIG_DEF (SIG_T1IT_AFC_IND,                         T1itAfcInd                      t1itAfcInd)
    SIG_DEF (SIG_T1IT_TIMEBASE_ALIGN_IND,              T1itTimebaseAlignInd            t1itTimebaseAlignInd)
    SIG_DEF (SIG_T1IT_SFN_DETECT_SUCCESS_IND,          T1itSfnDetectSuccessInd         t1itSfnDetectSuccessInd)
    SIG_DEF (SIG_T1IT_SFN_DETECT_FAIL_IND,             T1itSfnDetectFailInd            t1itSfnDetectFailInd)
    SIG_DEF (SIG_T1IT_AGC_IND,                         T1itAgcInd                      t1itAgcInd)
#if defined (DEVELOPMENT_VERSION)||defined(T1_UPHY_STUB_ENABLE)
    SIG_DEF( SIG_T1IT_TST_PARAM_REQ,                   T1itTstParamReq                 t1itTstParamReq )
#endif
    SIG_DEF (SIG_T1IT_PICH_IND,                        T1itPichInd                     t1itPichInd)

    SIG_DEF (SIG_T1IT_CQI_IND,                         T1itCqiInd                      t1itCqiInd)
    SIG_DEF (SIG_T1IT_IFIC_CELLS_IND,                  T1itIficCellsInd                t1itIficCellsInd)
    SIG_DEF (SIG_T1IT_PERFORM_TEST_IND,                T1itPerformTestInd              t1itPerformTestInd)
    SIG_DEF (SIG_T1IT_EDCH_DATA_INFO_IND,              T1itEdchDataInfoInd             t1itEdchDataInfoInd)
    SIG_DEF (SIG_T1IT_EDCH_DATA_PRE_ENCODE_IND,        T1itEdchDataPreEncodeInd        t1itEdchDataPreEncodeInd)

#  if defined (UPGRADE_GGE)
    SIG_DEF (SIG_T1IT_GIT_RSSI_SQ_START_IND,           T1itGitRssiSqStartInd          t1itGitRssiSqStartInd)
    SIG_DEF (SIG_T1IT_GIT_RSSI_SQ_MEASURE_IND,         T1itGitRssiSqMeasureInd        t1itGitRssiSqMeasureInd)
    SIG_DEF (SIG_T1IT_GIT_RSSI_SQ_END_IND,             T1itGitRssiSqEndInd            t1itGitRssiSqEndInd)
    SIG_DEF (SIG_T1IT_GIT_FB_SQ_START_IND,             T1itGitFbSqStartInd            t1itGitFbSqStartInd)
    SIG_DEF (SIG_T1IT_GIT_FB_SQ_END_IND,               T1itGitFbSqEndInd              t1itGitFbSqEndInd)
    SIG_DEF (SIG_T1IT_GIT_SB_SQ_START_IND,             T1itGitSbSqStartInd            t1itGitSbSqStartInd)
    SIG_DEF (SIG_T1IT_GIT_SB_SQ_END_IND,               T1itGitSbSqEndInd              t1itGitSbSqEndInd)
    SIG_DEF (SIG_T1IT_GIT_GSM_CELL_INFO_IND,           T1itGitGsmCellInfoInd          t1itGitGsmCellInfoInd)
    SIG_DEF (SIG_T1IT_GIT_RSSI_LIST_IND,               T1itGitRssiListInd             t1itGitRssiListInd)
    SIG_DEF (SIG_T1IT_GIT_BSIC_LIST_IND,               T1itGitBsicListInd             t1itGitBsicListInd)
#	if defined (UPGRADE_T1_GIT_BPS)
    SIG_DEF (SIG_T1IT_GIT_BCCH_SQ_START_IND,           T1itGitBcchSqStartInd          t1itGitBcchSqStartInd)
    SIG_DEF (SIG_T1IT_GIT_BCCH_SQ_END_IND,             T1itGitBcchSqEndInd            t1itGitbcchSqEndInd)
#	endif /* UPGRADE_T1_GIT_BPS */
#  endif /* UPGRADE_GGE */

#if defined (SYSTEM_DEBUG_MODE)
    SIG_DEF (SIG_PARAM_FIX_SET_REQ,                    ParamFixSetReq                 paramFixSetReq)
#endif /* SYSTEM_DEBUG_MODE */

#endif /* UPGRADE_3G_TDD128 */


#endif /* !EXCLUDE_U1IT */


#if defined(OSDRIVERS_TEST_ENABLE)
    /* signals For PLP framework test, which is to inform PLP and feedback from PLP */
    SIG_DEF (SIG_SYS_PLP_FR_TEST,                      SysPlpFrTestStructure          sysPlpFrTestStructure)
    SIG_DEF (SIG_SYS_PLP_FR_TEST_CNF,                  SysPlpFrTestConfirm            sysPlpFrTestConfirm)
#endif /* OSDRIVERS_TEST_ENABLE */

/* END OF FILE */
