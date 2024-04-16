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
 ****************************************************************************
 * File Description : Layer 1 Signal Definitions
 ****************************************************************************/

#if defined(UPGRADE_SYS_TOOLS)
#if defined (ENABLE_L1_PROF)
    SIG_DEF ( SIG_L1PROF_DUMMY = L1PROF_SIGNAL_BASE, EmptySignal                 l1prof_dummy)
    SIG_DEF ( SIG_L1PROF_MEASURE_REQ,                L1ProfMeasureReq            l1profMeasureReq)
    SIG_DEF ( SIG_L1PROF_MEASURE_CNF,                L1ProfMeasureCnf            l1profMeasureCnf)
    SIG_DEF ( SIG_L1PROF_MEASURE_ABORT_REQ,          EmptySignal                 l1profMeasureAbortReq)
    SIG_DEF ( SIG_L1PROF_MEASURE_ABORT_CNF,          EmptySignal                 l1profMeasureAbortCnf)
    SIG_DEF ( SIG_L1PROF_RESULTS_REQ,                EmptySignal                 l1profResultsReq)
    SIG_DEF ( SIG_L1PROF_RESULTS_IND,                L1ProfResultsInd            l1profResultsInd)
    SIG_DEF ( SIG_L1PROF_RESULTS_CNF,                L1ProfResultsCnf            l1profResultsCnf)
    SIG_DEF ( SIG_L1PROF_TMR_RESULTS_REQ,            EmptySignal                 l1profTmrResultsReq)
    SIG_DEF ( SIG_L1PROF_TMR_RESULTS_IND,            L1ProfTmrResultsInd         l1profTmrResultsInd)
    SIG_DEF ( SIG_L1PROF_TMR_RESULTS_CNF,            L1ProfTmrResultsCnf         l1profTmrResultsCnf)
#endif
#endif

#if !defined (EXCLUDE_MPH)
    SIG_DEF( SIG_MPH_DUMMY = MPH_SIGNAL_BASE,      EmptySignal                   mph_dummy)
    SIG_DEF( SIG_MPH_BCH_CONFIG_CNF,               MphBchConfigCnf               mphBchConfigCnf)
    SIG_DEF( SIG_MPH_IMM_ASSIGNMENT_CNF,           MphImmAssignmentCnf           mphImmAssignmentCnf)
    SIG_DEF( SIG_MPH_CIPHER_MODE_CNF,              MphCipherModeCnf              mphCipherModeCnf)
    SIG_DEF( SIG_MPH_HANDOVER_CNF,                 MphHandoverCnf                mphHandoverCnf)
    SIG_DEF( SIG_MPH_HANDOVER_FAIL_CNF,            MphHandoverFailCnf            mphHandoverFailCnf)
    SIG_DEF( SIG_MPH_HANDOVER_START_IND,           MphHandoverStartInd           mphHandoverStartInd)
    SIG_DEF( SIG_MPH_IDLE_NCELL_MEAS_IND,          MphIdleNcellMeasInd           mphIdleNcellMeasInd)
    SIG_DEF( SIG_MPH_IDLE_SCELL_MEAS_IND,          MphIdleScellMeasInd           mphIdleScellMeasInd)
    SIG_DEF( SIG_MPH_ERROR_IND,                    MphErrorInd                   mphErrorInd)
    SIG_DEF( SIG_MPH_BCCH_MEAS_IND,                MphBcchMeasInd                mphBcchMeasInd)
    SIG_DEF( SIG_MPH_FIND_BCCH_REQ,                MphFindBcchReq                mphFindBcchReq)
    SIG_DEF( SIG_MPH_BCH_CONFIG_REQ,               MphBchConfigReq               mphBchConfigReq)
    SIG_DEF( SIG_MPH_SERVING_CELL_BCCH_REQ,        MphServingCellBcchReq         mphServingCellBcchReq)
    SIG_DEF( SIG_MPH_IMM_ASSIGNMENT_REQ,           MphImmAssignmentReq           mphImmAssignmentReq)
    SIG_DEF( SIG_MPH_DEACTIVATE_REQ,               MphDeactivateReq              mphDeactivateReq)
    SIG_DEF( SIG_MPH_BCCH_DECODE_REQ,              MphBcchDecodeReq              mphBcchDecodeReq)
    SIG_DEF( SIG_MPH_BSIC_DECODE_REQ,              MphBsicDecodeReq              mphBsicDecodeReq)
    SIG_DEF( SIG_MPH_BSIC_DECODE_CNF,              MphBsicDecodeCnf              mphBsicDecodeCnf)
    SIG_DEF( SIG_MPH_BSIC_DECODE_IND,              MphBsicDecodeInd              mphBsicDecodeInd)
    SIG_DEF( SIG_MPH_PAGE_MODE_REQ,                MphPageModeReq                mphPageModeReq)
    SIG_DEF( SIG_MPH_HANDOVER_REQ,                 MphHandoverReq                mphHandoverReq)
    SIG_DEF( SIG_MPH_HANDOVER_FAIL_REQ,            MphHandoverFailReq            mphHandoverFailReq)
    SIG_DEF( SIG_MPH_TIMING_ADV_REQ,               MphTimingAdvReq               mphTimingAdvReq)
    SIG_DEF( SIG_MPH_RADIO_LINK_TIMEOUT_REQ,       MphRadioLinkTimeoutReq        mphRadioLinkTimeoutReq)
    SIG_DEF( SIG_MPH_CIPHER_MODE_REQ,              MphCipherModeReq              mphCipherModeReq)
    SIG_DEF( SIG_MPH_NCELL_MEAS_REQ,               MphNcellMeasReq               mphNcellMeasReq)
    SIG_DEF( SIG_MPH_CBCH_CONTROL_REQ,             MphCbchControlReq             mphCbchControlReq)
    SIG_DEF( SIG_MPH_NEXT_BCCH_REQ,                EmptySignal                   mphNextBcchReq)
    SIG_DEF( SIG_MPH_CHAN_ASSIGNMENT_FAIL_REQ,     MphChanAssignmentFailReq      mphChanAssignmentFailReq)
    SIG_DEF( SIG_MPH_CHAN_ASSIGNMENT_FAIL_CNF,     MphChanAssignmentFailCnf      mphChanAssignmentFailCnf)
    SIG_DEF( SIG_MPH_FREQUENCY_CHANGE_REQ,         MphFrequencyChangeReq         mphFrequencyChangeReq)
    SIG_DEF( SIG_MPH_CHANNEL_MODE_REQ,             MphChannelModeReq             mphChannelModeReq)
    SIG_DEF( SIG_MPH_CHANNEL_MODE_CNF,             MphChannelModeCnf             mphChannelModeCnf)
    SIG_DEF( SIG_MPH_CLASSMARK_REQ,                MphClassmarkReq               mphClassmarkReq)
    SIG_DEF( SIG_MPH_CLASSMARK_CNF,                MphClassmarkCnf               mphClassmarkCnf)
    SIG_DEF( SIG_MPH_STOP_RACH_REQ,                MphStopRachReq                mphStopRachReq)
    SIG_DEF( SIG_MPH_DEACTIVATE_CNF,               MphDeactivateCnf              mphDeactivateCnf)
    SIG_DEF( SIG_MPH_FREQUENCY_CHANGE_CNF,         MphFrequencyChangeCnf         mphFrequencyChangeCnf)
    SIG_DEF( SIG_MPH_UNIT_DATA_IND,                MphUnitDataInd                mphUnitDataInd)
    SIG_DEF( SIG_MPH_RADIO_LINK_TIMEOUT_IND,       MphRadioLinkTimeoutInd        mphRadioLinkTimeoutInd)
    SIG_DEF( SIG_MPH_DOWNLINK_SIGNAL_FAIL_IND,     MphDownlinkSignalFailInd      mphDownlinkSignalFailInd)
    SIG_DEF( SIG_MPH_MEASURE_ALL_REQ,              MphMeasureAllReq              mphMeasureAllReq)
    SIG_DEF( SIG_MPH_RANDOM_ACCESS_REQ,            MphRandomAccessReq            mphRandomAccessReq)
    SIG_DEF( SIG_MPH_RANDOM_ACCESS_CNF,            MphRandomAccessCnf            mphRandomAccessCnf)
    SIG_DEF( SIG_MPH_CHAN_ASSIGNMENT_REQ,          MphChanAssignmentReq          mphChanAssignmentReq)
    SIG_DEF( SIG_MPH_CHAN_ASSIGNMENT_CNF,          MphChanAssignmentCnf          mphChanAssignmentCnf)
    SIG_DEF( SIG_MPH_ABORT_NCELL_OP_REQ,           EmptySignal                   mphAbortNcellOpReq)
    SIG_DEF( SIG_MPH_EXT_MEASUREMENT_REQ,          MphExtMeasurementReq          mphExtMeasurementReq)
    SIG_DEF( SIG_MPH_EXT_MEASUREMENT_CNF,          MphExtMeasurementCnf          mphExtMeasurementCnf)
    SIG_DEF( SIG_MPH_DEDICATED_MEAS_IND,           MphDedicatedMeasInd           mphDedicatedMeasInd)
    SIG_DEF( SIG_MPH_MONITOR_PLMN_REQ,             MphMonitorPlmnReq             mphMonitorPlmnReq)
    SIG_DEF( SIG_MPH_MONITOR_PLMN_CNF,             MphMonitorPlmnCnf             mphMonitorPlmnCnf)
    SIG_DEF( SIG_MPH_MONITOR_PLMN_IND,             MphMonitorPlmnInd             mphMonitorPlmnInd)
    SIG_DEF( SIG_MPH_ENGINEERING_COMMAND_REQ,             MphEngineeringCommandReq             mphEngineeringCommandReq)
    SIG_DEF( SIG_L1_MEASURE_COMPLETE_IND,          L1MeasureCompleteInd          l1MeasureCompleteInd)
    SIG_DEF( SIG_L1_SYNC_COMPLETE_IND,             L1SyncCompleteInd             l1SyncCompleteInd)
    SIG_DEF( SIG_L1_SB_COMPLETE_IND,               L1SbCompleteInd               l1SbCompleteInd)
    SIG_DEF( SIG_L1_FB_COMPLETE_IND,               L1FbCompleteInd               l1FbCompleteInd)
    SIG_DEF( SIG_L1_CONFIG_COMPLETE_IND,           L1ConfigCompleteInd           l1ConfigCompleteInd)
    SIG_DEF( SIG_L1_AGE_SYNC_INFO_IND,             L1AgeSyncInfoInd              l1AgeSyncInfoInd)
    SIG_DEF( SIG_L1_DEACTIVATE_CNF,                EmptySignal                   l1DeactivateCnf)
    SIG_DEF( SIG_L1_CSR_SB_COMPLETE_IND,           L1CsrSbCompleteInd            l1CsrSbCompleteInd)
    SIG_DEF( SIG_MPH_CSR_SCELL_MEAS_IND,           MphCsrScellMeasInd            mphCsrScellMeasInd)
    SIG_DEF( SIG_L1_RATSCCH_DECODE_IND,            L1RatscchDecodeInd            l1RatscchDecodeInd)
    SIG_DEF( SIG_L1_TST_CARR_INT_MEAS_IND,        L1TstCarrIntMeasInd            l1TstCarrIntMeasInd)
#if !defined (EXCLUDE_DBG)
    SIG_DEF( SIG_MPH_DIAG_IND,                     MphDiagInd                    mphDiagInd)
    SIG_DEF( SIG_L1_TST_FB_GROUP_IND,              L1TstFbGroupInd               l1TstFbGroupInd)
    SIG_DEF( SIG_L1_TST_SB_DECODE_IND,             L1TstSbDecodeInd              l1TstSbDecodeInd)
    SIG_DEF( SIG_L1_TST_NCELL_DEBUG_IND,           L1TstNcellDebugInd            l1TstNcellDebugInd)
    SIG_DEF( SIG_L1_TST_TIMING_CHANGE_IND,         L1TstTimingChangeInd          l1TstTimingChangeInd)
    SIG_DEF( SIG_L1_TST_AFC_IND,                   L1TstAfcInd                   l1TstAfcInd)
    SIG_DEF( SIG_L1_TST_SC_TRUNCATE_SLEEP_IND,     L1TstScTruncateSleep          l1TstScTruncateSleep)
    SIG_DEF( SIG_L1_TST_SC_ABANDON_SLEEP_IND,      L1TstScAbandonSleep           l1TstScAbandonSleep)
    SIG_DEF( SIG_L1_TST_SC_TIMING_ADJ_IND,         L1TstScTimingAdjust           l1TstScTimingAdjust)
    SIG_DEF( SIG_L1_TST_SC_LARGE_TIMING_ERROR_IND, L1TstScLargeTimingError       l1TstScLargeTimingError)
    SIG_DEF( SIG_L1_TST_SC_EARLY_WAKEUP_IND,       L1TstScEarlyWakeup            l1TstScEarlyWakeup)
    SIG_DEF( SIG_L1_TST_SC_32K_NUDGE_IND,          L1TstSc32kNudge               l1TstSc32kNudge)
    SIG_DEF( SIG_L1_TST_SC_CAL_INFO_IND,           L1TstScCalInfo                l1TstScCalInfo)
    SIG_DEF( SIG_L1_TST_SC_RECAL_DATA_IND,         L1TstScRecalData              l1TstScRecalData)
    SIG_DEF( SIG_L1_TST_SC_FORCE_RECAL_IND,        L1TstScForceRecal             l1TstScForceRecal)
    SIG_DEF( SIG_L1_TST_SC_RECAL_MEAS_DATA_IND,    L1TstScRecalMeasData          l1TstScRecalMeasData)
    SIG_DEF( SIG_L1_TST_SC_RECAL_ACCUM_DATA_IND,   L1TstScRecalAccumData         l1TstScRecalAccumData)
    SIG_DEF( SIG_L1_TST_SC_NOT_SC_REASON,          L1TstScNotScReasonInd         l1TstScNotScReasonInd)
    SIG_DEF( SIG_L1_TST_FRAME_INT_IND,             L1TstFrameIntInd              l1TstFrameIntInd)
    SIG_DEF( SIG_L1_TST_TCB_ACTION_IND,            L1TstTcbActionInd             l1TstTcbActionInd)
    SIG_DEF( SIG_L1_TST_TCB_EVENTS_IND,            L1TstTcbEventsInd             l1TstTcbEventsInd)
    SIG_DEF( SIG_L1_TST_TCB_ACTIVE_IND,            L1TstTcbActiveInd             l1TstTcbActiveInd)
    SIG_DEF( SIG_L1_TST_SEQ_ACTIVE_IND,            L1TstSeqActiveInd             l1TstSeqActiveInd)
    SIG_DEF( SIG_L1_DSP_SOFT_RESET_IND,            EmptySignal                   l1DspSoftResetInd)
    SIG_DEF( SIG_L1_TST_INT_TRACE_IND,             L1TstIntTraceInd              l1TstIntTraceInd)
    SIG_DEF( SIG_L1_TST_SLEEP_TRACE_IND,           L1TstSleepTraceInd            l1TstSleepTraceInd)
    SIG_DEF( SIG_L1_TST_LOAD_TRACE_IND,            L1TstLoadTraceInd             l1TstLoadTraceInd)
#endif
#if defined (UPGRADE_VCXO)
    SIG_DEF( SIG_L1_VCXO_PROCESS_DATA_REQ,         L1VcxoProcessDataReq          l1VcxoProcessDataReq)
    SIG_DEF( SIG_L1_VCXO_PROCESS_DATA_CNF,         L1VcxoProcessDataCnf          l1VcxoProcessDataCnf)
    SIG_DEF( SIG_L1_VCXO_PROCESS_CLEAR_REQ,        L1VcxoProcessClearReq         l1VcxoProcessClearReq)
    SIG_DEF( SIG_L1_VCXO_PROCESS_CLEAR_CNF,        L1VcxoProcessClearCnf         l1VcxoProcessClearCnf)
#if defined (DEVELOPMENT_VERSION)
    SIG_DEF( SIG_L1_VCXO_FREQ_MEAS_IND,            L1VcxoFreqMeasInd             l1VcxoFreqMeasInd)
#endif
#endif
#endif   /* EXCLUDE_MPH */

#if !defined (EXCLUDE_PH)
    /*
    ** L2-L1 signals
    */
    SIG_DEF( SIG_PH_DUMMY = PH_SIGNAL_BASE,        EmptySignal                   ph_dummy)
    SIG_DEF( SIG_PH_DATA_REQ,                      PhDataReq                     phDataReq)
    SIG_DEF( SIG_PH_REDUNDANT_1,                   EmptySignal                   phRedundant1)
    SIG_DEF( SIG_PH_EMPTY_FRAME_IND,               PhEmptyFrameInd               phEmptyFrameInd)
    SIG_DEF( SIG_PH_DATA_IND,                      PhDataInd                     phDataInd)
    SIG_DEF( SIG_PH_REDUNDANT_2,                   EmptySignal                   phRedundant2)
    SIG_DEF( SIG_PH_CONNECT_IND,                   PhConnectInd                  phConnectInd)
    SIG_DEF( SIG_PH_REDUNDANT_3,                   EmptySignal                   phRedundant3)
    SIG_DEF( SIG_PH_READY_TO_SEND_IND,             PhReadyToSendInd              phReadyToSendInd)
    /* Data on Board <-> Layer 1 signals */
    SIG_DEF( SIG_RTL1_DOWNLINK_DATA_IND,           Rtl1DownlinkDataInd           rtl1DownlinkDataInd)
    SIG_DEF( SIG_RTL1_UPLINK_DATA_REQ,             Rtl1UplinkDataReq             rtl1UplinkDataReq)
#endif   /* EXCLUDE_PH */

# if !defined (EXCLUDE_GMPH)
    SIG_DEF( SIG_GMPH_DUMMY = GPGMPH_SIGNAL_BASE,  EmptySignal                   gmph_dummy )
    SIG_DEF( SIG_GMPH_PCCCH_CONFIG_REQ,            GmphPccchConfigReq            gmphPccchConfigReq)
    SIG_DEF( SIG_GMPH_PCCCH_CONFIG_CNF,            GmphPccchConfigCnf            gmphPccchConfigCnf)
    SIG_DEF( SIG_GMPH_PBCCH_DECODE_REQ,            GmphPbcchDecodeReq            gmphPbcchDecodeReq)
    SIG_DEF( SIG_GMPH_PRACH_REQ,                   GmphPrachReq                  gmphPrachReq)
    SIG_DEF( SIG_GMPH_PRACH_CNF,                   GmphPrachCnf                  gmphPrachCnf)
    SIG_DEF( SIG_GMPH_STOP_PRACH_REQ,              GmphStopPrachReq              gmphStopPrachReq)
    SIG_DEF( SIG_GMPH_UNIT_DATA_IND,               GmphUnitDataInd               gmphUnitDataInd)
    SIG_DEF( SIG_GMPH_PRACH_FORMAT_REQ,            GmphPrachFormatReq            gmphPrachFormatReq)
    SIG_DEF( SIG_GMPH_PWR_CTRL_CONFIG_REQ,         GmphPwrCtrlConfigReq          gmphPwrCtrlConfigReq)

    SIG_DEF( SIG_GMPH_UL_SB_CONFIG_REQ,            GmphUlSbConfigReq             gmphUlSbConfigReq)
    SIG_DEF( SIG_GMPH_UL_SB_CONFIG_CNF,            GmphUlSbConfigCnf             gmphUlSbConfigCnf)
    SIG_DEF( SIG_GMPH_UL_SB_ABORT_REQ,             GmphUlSbAbortReq              gmphUlSbAbortReq)
    SIG_DEF( SIG_GMPH_DL_SB_CONFIG_REQ,            GmphDlSbConfigReq             gmphDlSbConfigReq)
    SIG_DEF( SIG_GMPH_DL_SB_CONFIG_CNF,            GmphDlSbConfigCnf             gmphDlSbConfigCnf)

    SIG_DEF( SIG_GMPH_DL_TBF_CONFIG_REQ,           GmphDlTbfConfigReq            gmphDlTbfConfigReq)
    SIG_DEF( SIG_GMPH_DL_TBF_CONFIG_CNF,           GmphDlTbfConfigCnf            gmphDlTbfConfigCnf)
    SIG_DEF( SIG_GMPH_UL_DYN_TBF_CONFIG_REQ,       GmphUlDynTbfConfigReq         gmphUlDynTbfConfigReq)
    SIG_DEF( SIG_GMPH_UL_DYN_TBF_CONFIG_CNF,       GmphUlDynTbfConfigCnf         gmphUlDynTbfConfigCnf)
    SIG_DEF( SIG_GMPH_UL_FXD_TBF_CONFIG_REQ,       GmphUlFxdTbfConfigReq         gmphUlFxdTbfConfigReq)
    SIG_DEF( SIG_GMPH_UL_FXD_TBF_CONFIG_CNF,       GmphUlFxdTbfConfigCnf         gmphUlFxdTbfConfigCnf)
    SIG_DEF( SIG_GMPH_TIMING_POWER_REQ,            GmphTimingPowerReq            gmphTimingPowerReq)
    SIG_DEF( SIG_GMPH_TIMING_POWER_CNF,            GmphTimingPowerCnf            gmphTimingPowerCnf)
    SIG_DEF( SIG_GMPH_PDCH_RELEASE_REQ,            GmphPdchReleaseReq            gmphPdchReleaseReq)
    SIG_DEF( SIG_GMPH_PDCH_RELEASE_CNF,            GmphPdchReleaseCnf            gmphPdchReleaseCnf)
    SIG_DEF( SIG_GMPH_DL_TBF_SHUTDOWN_REQ,         GmphDlTbfShutdownReq          gmphDlTbfShutdownReq)
    SIG_DEF( SIG_GMPH_UL_TBF_SHUTDOWN_REQ,         GmphUlTbfShutdownReq          gmphUlTbfShutdownReq)

    SIG_DEF( SIG_GMPH_NET_CTRL_MEAS_REQ,           GmphNetCtrlMeasReq            gmphNetCtrlMeasReq)
    SIG_DEF( SIG_GMPH_EXT_MEAS_REQ,                GmphExtMeasReq                gmphExtMeasReq)
    SIG_DEF( SIG_GMPH_EXT_MEAS_CNF,                GmphExtMeasCnf                gmphExtMeasCnf)
    SIG_DEF( SIG_GMPH_INT_MEAS_REQ,                GmphIntMeasReq                gmphIntMeasReq)
    SIG_DEF( SIG_GMPH_INT_MEAS_IND,                GmphIntMeasInd                gmphIntMeasInd)
    SIG_DEF( SIG_GMPH_PTM_MEAS_IND,                GmphPtmMeasInd                gmphPtmMeasInd)

    SIG_DEF( SIG_GMPH_UL_FXD_ALLOC_END_IND,        GmphUlFxdAllocEndInd          gmphUlFxdAllocEndInd)

    SIG_DEF( SIG_GMPH_PTM_ENTER_IND,               GmphPtmEnterInd               gmphPtmEnterInd)
    SIG_DEF( SIG_GMPH_PTM_EXIT_IND,                GmphPtmExitInd                gmphPtmExitInd)

    SIG_DEF( SIG_GL1_CIPHER_DATA_REQ,              Gl1CipherDataReq              gl1CipherDataReq)
    SIG_DEF( SIG_GL1_CIPHER_DATA_CNF,              Gl1CipherDataCnf              gl1CipherDataCnf)
    SIG_DEF( SIG_GL1_CIPHER_FLUSH_REQ,             EmptySignal                 gl1CipherFlushReq)

    SIG_DEF( SIG_GMPH_DYN_TS_RECONFIG_REQ,         GmphDynTsReconfigReq          gmphDynTsReconfigReq)
    SIG_DEF( SIG_GMPH_DYN_TS_RECONFIG_CNF,         GmphDynTsReconfigCnf          gmphDynTsReconfigCnf)
    SIG_DEF( SIG_GMPH_FXD_TS_RECONFIG_REQ,         GmphFxdTsReconfigReq          gmphFxdTsReconfigReq)
    SIG_DEF( SIG_GMPH_FXD_TS_RECONFIG_CNF,         GmphFxdTsReconfigCnf          gmphFxdTsReconfigCnf)

    SIG_DEF( SIG_GMPH_PKT_OP_CTRL_IND,             GmphPktOpCtrlInd              gmphPktOpCtrlInd)
    SIG_DEF( SIG_GMPH_PKT_OP_CTRL_RESP,            GmphPktOpCtrlResp             gmphPktOpCtrlResp)

#  if defined (UPGRADE_EDGE)
    SIG_DEF( SIG_GMPH_UL_MB_CONFIG_REQ,            GmphUlMbConfigReq             gmphUlMbConfigReq)
    SIG_DEF( SIG_GMPH_REDUNDANT_1,                 EmptySignal                   gmphRedundant1)
    SIG_DEF( SIG_GMPH_REDUNDANT_2,                 EmptySignal                   gmphRedundant2)

    SIG_DEF( SIG_GMPH_EPRACH_REQ,                  GmphEprachReq                 gmphEprachReq)
    SIG_DEF( SIG_GMPH_EPRACH_CNF,                  GmphEprachCnf                 gmphEprachCnf)
#  endif
    SIG_DEF( SIG_L1_IDLE_DEBUG_IND,                L1IdleDebugInd                l1IdleDebugInd)
    SIG_DEF( SIG_L1_IDLE_TCB_LST_DEBUG_IND,        L1IdleTcbLstDebugInd          l1IdleTcbListDebugInd)
    SIG_DEF( SIG_L1_IDLE_PRACH_DEBUG_IND,          L1IdlePrachDebugInd           l1IdlePrachDebugInd)

# endif /* EXCLUDE_GMPH */

#if !defined (EXCLUDE_CDMG)
    SIG_DEF( SIG_CDMG_DUMMY = CDMG_SIGNAL_BASE,    EmptySignal                   cdmgDummy)
    SIG_DEF( SIG_CDMG_REQ,                         CalDevGsmReq                  calDevGsmReq)
    SIG_DEF( SIG_CDMG_CNF,                         CalDevGsmCnf                  calDevGsmCnf)
    SIG_DEF( SIG_CDMG_FINISH_REQ,                  CalDevGsmFinishReq            calDevGsmFinishReq)
    SIG_DEF( SIG_CDMG_FINISH_CNF,                  CalDevGsmFinishCnf            calDevGsmFinishCnf)
    SIG_DEF( SIG_CDMG_RSSI_REQ,                    CalDevGsmRssiReq              calDevGsmRssiReq)
    SIG_DEF( SIG_CDMG_RSSI_CNF,                    CalDevGsmRssiCnf              calDevGsmRssiCnf)
    SIG_DEF( SIG_CDMG_FREQ_OFFSET_MEAS_REQ,        CalDevGsmFreqOffsetMeasReq    calDevGsmFreqOffsetMeasReq)
    SIG_DEF( SIG_CDMG_FREQ_OFFSET_MEAS_CNF,        CalDevGsmFreqOffsetMeasCnf    calDevGsmFreqOffsetMeasCnf)
    SIG_DEF( SIG_CDMG_SET_POWER_RAMP_REQ,          CalDevGsmSetPowerRampReq      calDevGsmSetPowerRampReq)
    SIG_DEF( SIG_CDMG_SET_POWER_RAMP_CNF,          CalDevGsmSetPowerRampCnf      calDevGsmSetPowerRampCnf)
    SIG_DEF( SIG_CDMG_DC_OFFSET_REQ,               CalDevGsmDcOffsetReq          calDevGsmDcOffsetReq)
    SIG_DEF( SIG_CDMG_DC_OFFSET_CNF,               CalDevGsmDcOffsetCnf          calDevGsmDcOffsetCnf)
    SIG_DEF( SIG_CDMG_GAIN_PROGRAM_REQ,            CalDevGsmGainProgramReq       calDevGsmGainProgramReq)
    SIG_DEF( SIG_CDMG_GAIN_PROGRAM_CNF,            CalDevGsmGainProgramCnf       calDevGsmGainProgramCnf)
    /* signals below are for radio development and integration */
    SIG_DEF( SIG_CDMG_BURST_REQ,                   CalDevGsmBurstReq             calDevGsmBurstReq)
    SIG_DEF( SIG_CDMG_BURST_CNF,                   CalDevGsmBurstCnf             calDevGsmBurstCnf)
    SIG_DEF( SIG_CDMG_SET_AFC_DAC_REQ,             CalDevGsmSetAfcDacReq         calDevGsmSetAfcDacReq)
    SIG_DEF( SIG_CDMG_SET_AFC_DAC_CNF,             CalDevGsmSetAfcDacCnf         calDevGsmSetAfcDacCnf)
    SIG_DEF( SIG_CDMG_SET_BURST_DATA_REQ,          CalDevGsmSetBurstDataReq      calDevGsmSetBurstDataReq)
    SIG_DEF( SIG_CDMG_SET_BURST_DATA_CNF,          CalDevGsmSetBurstDataCnf      calDevGsmSetBurstDataCnf)
    SIG_DEF( SIG_CDMG_RX_CONTROL_REQ,              CalDevGsmRxControlReq         calDevGsmRxControlReq)
    SIG_DEF( SIG_CDMG_RX_CONTROL_CNF,              CalDevGsmRxControlCnf         calDevGsmRxControlCnf)
    SIG_DEF( SIG_CDMG_RAMP_SCALE_REQ,              CalDevGsmRampScaleReq         calDevGsmRampScaleReq)
    SIG_DEF( SIG_CDMG_RAMP_SCALE_CNF,              CalDevGsmRampScaleCnf         calDevGsmRampScaleCnf)
    SIG_DEF( SIG_CDMG_SET_BAND_MODE_REQ,           CalDevGsmSetBandModeReq       calDevGsmSetBandModeReq)
    SIG_DEF( SIG_CDMG_SET_BAND_MODE_CNF,           CalDevGsmSetBandModeCnf       calDevGsmSetBandModeCnf)
    SIG_DEF( SIG_CDMG_LOOPBACK_REQ,                CalDevGsmLoopBackReq          calDevGsmLoopBackReq)
    SIG_DEF( SIG_CDMG_LOOPBACK_CNF,                CalDevGsmLoopBackCnf          calDevGsmLoopBackCnf)
    SIG_DEF( SIG_CDMG_LOOPBACK_DATA_REQ,           CalDevGsmLoopBackDataReq      calDevGsmLoopBackDataReq)
    SIG_DEF( SIG_CDMG_LOOPBACK_DATA_CNF,           CalDevGsmLoopBackDataCnf      calDevGsmLoopBackDataCnf)
# if !defined (DISABLE_MULTISLOT_TEST)
    SIG_DEF( SIG_CDMG_SLOT_DEFINE_REQ,             CalDevGsmSlotDefineReq        calDevGsmSlotDefineReq)
    SIG_DEF( SIG_CDMG_SLOT_DEFINE_CNF,             CalDevGsmSlotDefineCnf        calDevGsmSlotDefineCnf)
    SIG_DEF( SIG_CDMG_FRAME_DEFINE_REQ,            CalDevGsmFrameDefineReq       calDevGsmFrameDefineReq)
    SIG_DEF( SIG_CDMG_FRAME_DEFINE_CNF,            CalDevGsmFrameDefineCnf       calDevGsmFrameDefineCnf)
    SIG_DEF( SIG_CDMG_FRAME_TIMING_REQ,            CalDevGsmFrameTimingReq       calDevGsmFrameTimingReq)
    SIG_DEF( SIG_CDMG_FRAME_TIMING_CNF,            CalDevGsmFrameTimingCnf       calDevGsmFrameTimingCnf)
    SIG_DEF( SIG_CDMG_FRAME_USE_REQ,               CalDevGsmFrameUseReq          calDevGsmFrameUseReq)
    SIG_DEF( SIG_CDMG_FRAME_USE_CNF,               CalDevGsmFrameUseCnf          calDevGsmFrameUseCnf)
    SIG_DEF( SIG_CDMG_CONST_TX_REQ,                CalDevGsmConstTxReq           calDevGsmConstTxReq)
    SIG_DEF( SIG_CDMG_CONST_TX_CNF,                CalDevGsmConstTxCnf           calDevGsmConstTxCnf)
# endif /* defined (DISABLE_MULTISLOT_TEST) */
#if defined (UPGRADE_VCXO)
    SIG_DEF( SIG_CDMG_VCXO_CALIBRATE_REQ,          CalDevGsmVcxoCalibrateReq     calDevGsmVcxoCalibrateReq)
    SIG_DEF( SIG_CDMG_VCXO_CALIBRATE_CNF,          CalDevGsmVcxoCalibrateCnf     calDevGsmVcxoCalibrateCnf)
#endif
#if defined (ENABLE_RATT3G)
    SIG_DEF( SIG_CDMG_TX_GPO_TIMING_REQ,           CalDevGsmTxGpoTimingReq       calDevGsmTxGpoTimingReq)
    SIG_DEF( SIG_CDMG_TX_GPO_TIMING_CNF,           CalDevGsmTxGpoTimingCnf       calDevGsmTxGpoTimingCnf)
    SIG_DEF( SIG_CDMG_SET_RAMP_THRESHOLD_REQ,      CalDevGsmSetRampThresholdReq  calDevGsmSetRampThresholdReq)
    SIG_DEF( SIG_CDMG_SET_RAMP_THRESHOLD_CNF,      CalDevGsmSetRampThresholdCnf  calDevGsmSetRampThresholdCnf)    
#endif
#if defined (DSP_TASK_TEST_HARNESS)
    SIG_DEF( SIG_DSPTT_DSP_GET_BUF_REQ,            DspTaskTestDspGetBufReq       dspTaskTestDspGetBufReq)
    SIG_DEF( SIG_DSPTT_DSP_GET_BUF_CNF,            DspTaskTestDspGetBufCnf       dspTaskTestDspGetBufCnf)
    SIG_DEF( SIG_DSPTT_DSP_FREE_BUF_REQ,           DspTaskTestDspFreeBufReq      dspTaskTestDspFreeBufReq)
    SIG_DEF( SIG_DSPTT_DSP_FREE_BUF_CNF,           DspTaskTestDspFreeBufCnf      dspTaskTestDspFreeBufCnf)
    SIG_DEF( SIG_DSPTT_DSP_COPY_BUF_REQ,           DspTaskTestDspCopyBufReq      dspTaskTestDspCopyBufReq)
    SIG_DEF( SIG_DSPTT_DSP_COPY_BUF_CNF,           DspTaskTestDspCopyBufCnf      dspTaskTestDspCopyBufCnf)
    SIG_DEF( SIG_DSPTT_TDS_ALLOC_BUF_REQ,          DspTaskTestTdsAllocBufReq     dspTaskTestTdsAllocBufReq)
    SIG_DEF( SIG_DSPTT_TDS_ALLOC_BUF_CNF,          DspTaskTestTdsAllocBufCnf     dspTaskTestTdsAllocBufCnf)
    SIG_DEF( SIG_DSPTT_TDS_FREE_BUF_REQ,           DspTaskTestTdsFreeBufReq      dspTaskTestTdsFreeBufReq)
    SIG_DEF( SIG_DSPTT_TDS_FREE_BUF_CNF,           DspTaskTestTdsFreeBufCnf      dspTaskTestTdsFreeBufCnf)
    SIG_DEF( SIG_DSPTT_TDS_COPY_BUF_REQ,           DspTaskTestTdsCopyBufReq      dspTaskTestTdsCopyBufReq)
    SIG_DEF( SIG_DSPTT_TDS_COPY_BUF_CNF,           DspTaskTestTdsCopyBufCnf      dspTaskTestTdsCopyBufCnf)
    SIG_DEF( SIG_DSPTT_MCU_ALLOC_BUF_REQ,          DspTaskTestMcuAllocBufReq     dspTaskTestMcuAllocBufReq)
    SIG_DEF( SIG_DSPTT_MCU_ALLOC_BUF_CNF,          DspTaskTestMcuAllocBufCnf     dspTaskTestMcuAllocBufCnf)
    SIG_DEF( SIG_DSPTT_MCU_FREE_BUF_REQ,           DspTaskTestMcuFreeBufReq      dspTaskTestMcuFreeBufReq)
    SIG_DEF( SIG_DSPTT_MCU_FREE_BUF_CNF,           DspTaskTestMcuFreeBufCnf      dspTaskTestMcuFreeBufCnf)
    SIG_DEF( SIG_DSPTT_MCU_COPY_BUF_REQ,           DspTaskTestMcuCopyBufReq      dspTaskTestMcuCopyBufReq)
    SIG_DEF( SIG_DSPTT_MCU_COPY_BUF_CNF,           DspTaskTestMcuCopyBufCnf      dspTaskTestMcuCopyBufCnf)
    SIG_DEF( SIG_DSPTT_CFG_COPY_BUF_REQ,           DspTaskTestCfgCopyBufReq      dspTaskTestCfgCopyBufReq)
    SIG_DEF( SIG_DSPTT_CFG_COPY_BUF_CNF,           DspTaskTestCfgCopyBufCnf      dspTaskTestCfgCopyBufCnf)
    SIG_DEF( SIG_DSPTT_SIM_COPY_BUF_REQ,           DspTaskTestSimCopyBufReq      dspTaskTestSimCopyBufReq)
    SIG_DEF( SIG_DSPTT_SIM_COPY_BUF_CNF,           DspTaskTestSimCopyBufCnf      dspTaskTestSimCopyBufCnf)
    SIG_DEF( SIG_DSPTT_DSP_RUN_TASK_REQ,           DspTaskTestDspRunTaskReq      dspTaskTestDspRunTaskReq)
    SIG_DEF( SIG_DSPTT_DSP_RUN_TASK_CNF,           DspTaskTestDspRunTaskCnf      dspTaskTestDspRunTaskCnf)
    SIG_DEF( SIG_DSPTT_DSP_RESET_REQ,              DspTaskTestDspResetReq        dspTaskTestDspResetReq)
    SIG_DEF( SIG_DSPTT_DSP_RESET_CNF,              DspTaskTestDspResetCnf        dspTaskTestDspResetCnf)
    SIG_DEF( SIG_DSPTT_WR_MCU_PTR_REQ,             DspTaskTestWrMcuPtrReq        dspTaskTestWrMcuPtrReq)
    SIG_DEF( SIG_DSPTT_WR_MCU_PTR_CNF,             DspTaskTestWrMcuPtrCnf        dspTaskTestWrMcuPtrCnf)
    SIG_DEF( SIG_DSPTT_INI_TST_HARNESS_REQ,        DspTaskTestIniTstHarnessReq   dspTaskTestIniTstHarnessReq)
    SIG_DEF( SIG_DSPTT_INI_TST_HARNESS_CNF,        DspTaskTestIniTstHarnessCnf   dspTaskTestIniTstHarnessCnf)
    SIG_DEF( SIG_DSPTT_EXIT_TST_HARNESS_REQ,       DspTaskTestExitTstHarnessReq  dspTaskTestExitTstHarnessReq)
    SIG_DEF( SIG_DSPTT_EXIT_TST_HARNESS_CNF,       DspTaskTestExitTstHarnessCnf  dspTaskTestExitTstHarnessCnf)
    SIG_DEF( SIG_DSPTT_REF_CHKSUM_REQ,             DspTaskTestRefChksumReq       dspTaskTestRefChksumReq)
    SIG_DEF( SIG_DSPTTX_PLFX_GET_BUF_REQ,          DspTaskTestXPlfxGetBufReq     dspTaskTestXPlfxGetBufReq) 
    SIG_DEF( SIG_DSPTTX_PLFX_GET_BUF_CNF,          DspTaskTestXPlfxGetBufCnf     dspTaskTestXPlfxGetBufCnf) 
    SIG_DEF( SIG_DSPTTX_PLFX_FREE_BUF_REQ,         DspTaskTestXPlfxFreeBufReq    dspTaskTestXPlfxFreeBufReq)
    SIG_DEF( SIG_DSPTTX_PLFX_FREE_BUF_CNF,         DspTaskTestXPlfxFreeBufCnf    dspTaskTestXPlfxFreeBufCnf)
    SIG_DEF( SIG_DSPTTX_PLFX_COPY_BUF_REQ,         DspTaskTestXPlfxCopyBufReq    dspTaskTestXPlfxCopyBufReq)
    SIG_DEF( SIG_DSPTTX_PLFX_COPY_BUF_CNF,         DspTaskTestXPlfxCopyBufCnf    dspTaskTestXPlfxCopyBufCnf)
    SIG_DEF( SIG_DSPTTX_MCU_BUF_COPY_REQ,          DspTaskTestXMcuBufCopyReq     dspTaskTestXMcuBufCopyReq)
    SIG_DEF( SIG_DSPTTX_MCU_BUF_COPY_CNF,          DspTaskTestXMcuBufCopyCnf     dspTaskTestXMcuBufCopyCnf)
#endif /* (DSP_TASK_TEST_HARNESS) */                                             

#if defined (ENABLE_TVPREP)
    SIG_DEF( SIG_DSPTT_RENAME_LOB_FILE_REQ,        DspTaskTestRenameLobFileReq dspTaskTestRenameLobFileReq)
    SIG_DEF( SIG_DSPTT_RENAME_LOB_FILE_CNF,        DspTaskTestRenameLobFileCnf dspTaskTestRenameLobFileCnf)
    SIG_DEF( SIG_DSPTT_RENAME_PLKSIM_FILE_REQ,     DspTaskTestRenamePlkSimFileReq dspTaskTestRenamePlkSimFileReq)
    SIG_DEF( SIG_DSPTT_RENAME_PLKSIM_FILE_CNF,     DspTaskTestRenamePlkSimFileCnf dspTaskTestRenamePlkSimFileCnf)
#endif
#endif  /* EXCLUDE_DM */

#if !defined (EXCLUDE_TI)
    SIG_DEF( SIG_TI_DUMMY = TI_SIGNAL_BASE,        EmptySignal                 ti_dummy)
    SIG_DEF( SIG_TI_DAI_REQ,                       TiDaiReq                    tiDaiReq)
    SIG_DEF( SIG_TI_TCH_REQ,                       TiTchReq                    tiTchReq)
    SIG_DEF( SIG_TI_DAI_CNF,                       EmptySignal                 tiDaiCnf)
    SIG_DEF( SIG_TI_TCH_CNF,                       EmptySignal                 tiTchCnf)
#endif  /* EXCLUDE_TI */

#if defined (UPGRADE_MIPS_MANAGER)
    SIG_DEF( SIG_MIPSMAN_DUMMY = MIPSMAN_SIGNAL_BASE, EmptySignal    MipsMan_dummy )
    SIG_DEF( SIG_MIPS_MANAGER_RESOURCE_REQ,        MipsManResourceReq          mipsManResourceReq)
    SIG_DEF( SIG_MIPS_MANAGER_RESOURCE_CNF,        MipsManResourceCnf          mipsManResourceCnf)
    SIG_DEF( SIG_MIPS_MANAGER_RESOURCE_CHANGE_IND, MipsManResourceChangeInd    mipsManResourceChangeInd)
    SIG_DEF( SIG_MIPS_MANAGER_DEBUG_IND,           MipsManDebugInd             mipsManDebugInd)
#endif

#if !defined (EXCLUDE_L1TEST)
    SIG_DEF( SIG_L1TEST_DUMMY = L1TEST_SIGNAL_BASE, EmptySignal                l1testDummy )

# if defined (UPGRADE_L1_TEST_APP)
    SIG_DEF( SIG_L1TT_FAST_SCROLL_IND,             L1ttFastScrollInd           l1ttFastScrollInd)
    SIG_DEF( SIG_L1TT_FB_IND,                      L1ttFbInd                   l1ttFbInd)
    SIG_DEF( SIG_L1TT_SB_IND,                      L1ttSbInd                   l1ttSbInd)
# else
    SIG_DEF( SIG_L1TT_FAST_SCROLL_IND,             EmptySignal                 l1ttFastScrollInd)
    SIG_DEF( SIG_L1TT_FB_IND,                      EmptySignal                 l1ttFbInd)
    SIG_DEF( SIG_L1TT_SB_IND,                      EmptySignal                 l1ttSbInd)
# endif

    SIG_DEF( SIG_MAC_TST_RX_PAYLOAD_IND,           MacTstRxPayloadInd          macTstRxPayloadInd)

# if defined (UPGRADE_EDGE)
    SIG_DEF( SIG_MAC_TST_EGPRS_RX_HEADER_IND,      MacTstEgprsRxHeaderInd      macTstEgprsRxHeaderInd)
    SIG_DEF( SIG_MAC_TST_EGPRS_RX_PAYLOAD_IND,     MacTstEgprsRxPayloadInd     macTstEgprsRxPayloadInd)
    SIG_DEF( SIG_MAC_TST_EGPRS_DECODE_FAILED_IND,  MacTstEgprsDecodeFailedInd  macTstEgprsDecodeFailedInd)
# else
    SIG_DEF( SIG_MAC_TST_EGPRS_RX_HEADER_IND,      EmptySignal                 macTstEgprsRxHeaderInd)
    SIG_DEF( SIG_MAC_TST_EGPRS_RX_PAYLOAD_IND,     EmptySignal                 macTstEgprsRxPayloadInd)
    SIG_DEF( SIG_MAC_TST_EGPRS_DECODE_FAILED_IND,  EmptySignal                 macTstEgprsDecodeFailedInd)
# endif /* UPGRADE_EDGE */

#endif /* EXCLUDE_L1TEST */

/* END OF FILE */
