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
 * File Description: Signal Id to Signal structure mapping for shared signal
 *                   interfaces between 3G PS and 3G L1.
 **************************************************************************/

#if defined (UPGRADE_3G)

#if !defined (EXCLUDE_CPHY)
SIG_DEF( SIG_CPHY_DUMMY = CPHY_SIGNAL_BASE,         EmptySignal                     cphy_dummy )
SIG_DEF( SIG_CPHY_UE_CAPABILITY_CONFIG_REQ,         CphyUeCapabilityConfigReq       cphyUeCapabilityConfigReq )
SIG_DEF( SIG_CPHY_FIND_CELL_REQ,                    CphyFindCellReq                 cphyFindCellReq )
SIG_DEF( SIG_CPHY_FIND_CELL_CNF,                    CphyFindCellCnf                 cphyFindCellCnf )
SIG_DEF( SIG_CPHY_NEXT_CELL_REQ,                    CphyNextCellReq                 cphyNextCellReq )
SIG_DEF( SIG_CPHY_NEXT_CELL_CNF,                    CphyNextCellCnf                 cphyNextCellCnf )
SIG_DEF( SIG_CPHY_CELL_SELECT_REQ,                  CphyCellSelectReq               cphyCellSelectReq )
SIG_DEF( SIG_CPHY_CELL_SELECT_CNF,                  CphyCellSelectCnf               cphyCellSelectCnf )
SIG_DEF( SIG_CPHY_BCH_REQ,                          CphyBchReq                      cphyBchReq )
SIG_DEF( SIG_CPHY_NCELL_BCH_REQ,                    CphyNcellBchReq                 cphyNcellBchReq )
SIG_DEF( SIG_CPHY_NCELL_BCH_CNF,                    CphyNcellBchCnf                 cphyNcellBchCnf )

#if defined(UPGRADE_3G_TDD128)
SIG_DEF( SIG_CPHY_ABORT_RSSI_SCAN_REQ,              CphyAbortRssiScanReq            cphyAbortRssiScanReq )
SIG_DEF( SIG_CPHY_ABORT_FIND_CELL_REQ,              CphyAbortFindCellReq            cphyAbortFindCellReq )
#endif /* UPGRADE_3G_TDD128 */

SIG_DEF( SIG_CPHY_PCH_CONFIG_REQ,                   CphyPchConfigReq                cphyPchConfigReq )
SIG_DEF( SIG_CPHY_FACH_CONFIG_REQ,                  CphyFachConfigReq               cphyFachConfigReq )
SIG_DEF( SIG_CPHY_RACH_CONFIG_REQ,                  CphyRachConfigReq               cphyRachConfigReq )
SIG_DEF( SIG_CPHY_CBS_CONFIG_REQ,                   CphyCbsConfigReq                cphyCbsConfigReq )
SIG_DEF( SIG_CPHY_SCCPCH_CONFIG_REQ,                CphySccpchConfigReq             cphySccpchConfigReq )
SIG_DEF( SIG_CPHY_UL_TRCH_CONFIG_REQ,               CphyUlTrchConfigReq             cphyUlTrchConfigReq )
SIG_DEF( SIG_CPHY_DL_TRCH_CONFIG_REQ,               CphyDlTrchConfigReq             cphyDlTrchConfigReq )
SIG_DEF( SIG_CPHY_DL_TFC_CONFIG_REQ,                CphyDlTfcConfigReq              cphyDlTfcConfigReq )
SIG_DEF( SIG_CPHY_RL_COMMON_SETUP_REQ,              CphyRlCommonSetupReq            cphyRlCommonSetupReq )
SIG_DEF( SIG_CPHY_RL_SETUP_REQ,                     CphyRlSetupReq                  cphyRlSetupReq )

#if defined (UPGRADE_3G_MBMS)
#if defined (UPGRADE_3G_TDD128)
SIG_DEF( SIG_CPHY_MCCH_CONFIG_REQ,                  CphyMcchConfigReq               cphyMcchConfigReq )
SIG_DEF( SIG_CPHY_MCCH_PAUSE_REQ,                   CphyMcchPauseReq                cphyMcchPauseReq )
SIG_DEF( SIG_CPHY_MICH_CONFIG_REQ,                  CphyMichConfigReq               cphyMichConfigReq )
SIG_DEF( SIG_CPHY_MSCH_CONFIG_REQ,                  CphyMschConfigReq               cphyMschConfigReq )
SIG_DEF( SIG_CPHY_MSCH_PAUSE_REQ,                   CphyMschPauseReq                cphyMschPauseReq )
SIG_DEF( SIG_CPHY_MTCH_CONFIG_REQ,                  CphyMtchConfigReq               cphyMtchConfigReq )
SIG_DEF( SIG_CPHY_SCCPCH_CONFIG_FOR_MBMS_REQ,       CphySccpchConfigForMbmsReq      cphySccpchConfigForMbmsReq )
#endif /* UPGRADE_3G_TDD128 */
#endif /* UPGRADE_3G_MBMS */

SIG_DEF( SIG_CPHY_CCTRCH_CONFIG_CNF,                CphyCcTrChConfigCnf             cphyCcTrChConfigCnf )
SIG_DEF( SIG_CPHY_CCTRCH_RELEASE_REQ,               CphyCcTrChReleaseReq            cphyCcTrChReleaseReq )
SIG_DEF( SIG_CPHY_SERVING_CELL_MEASUREMENT_REQ,     CphyServingCellMeasurementReq   cphyServingCellMeasurementReq )
SIG_DEF( SIG_CPHY_SERVING_CELL_MEASUREMENT_IND,     CphyServingCellMeasurementInd   cphyServingCellMeasurementInd )
SIG_DEF( SIG_CPHY_STOP_SERVING_CELL_MEAS_REQ,       CphyStopServingCellMeasReq      cphyStopServingCellMeasReq )
SIG_DEF( SIG_CPHY_INTRA_FREQ_CELL_INFO_REQ,         CphyIntraFreqCellInfoReq        cphyIntraFreqCellInfoReq )
SIG_DEF( SIG_CPHY_INTER_FREQ_CELL_INFO_REQ,         CphyInterFreqCellInfoReq        cphyInterFreqCellInfoReq )
SIG_DEF( SIG_CPHY_MONITOR_INTRA_FREQ_CELL_REQ,      CphyMonitorIntraFreqCellReq     cphyMonitorIntraFreqCellReq )
SIG_DEF( SIG_CPHY_MONITOR_INTER_FREQ_CELL_REQ,      CphyMonitorInterFreqCellReq     cphyMonitorInterFreqCellReq )
SIG_DEF( SIG_CPHY_MONITOR_INTRA_FREQ_CELL_IND,      CphyMonitorIntraFreqCellInd     cphyMonitorIntraFreqCellInd )
SIG_DEF( SIG_CPHY_MONITOR_INTER_FREQ_CELL_IND,      CphyMonitorInterFreqCellInd     cphyMonitorInterFreqCellInd )
SIG_DEF( SIG_CPHY_INTRA_FREQ_MEAS_ON_RACH_REQ,      CphyIntraFreqMeasOnRachReq      cphyIntraFreqMeasOnRachReq )
SIG_DEF( SIG_CPHY_INTRA_FREQ_MEAS_ON_RACH_CNF,      CphyIntraFreqMeasOnRachCnf      cphyIntraFreqMeasOnRachCnf )
#ifdef ENABLE_INTER_FREQ_MEAS_ON_RACH
#if defined (UPGRADE_3G_FDD) && defined (UPGRADE_REL6)
SIG_DEF( SIG_CPHY_INTER_FREQ_MEAS_ON_RACH_REQ,      CphyInterFreqMeasOnRachReq      cphyInterFreqMeasOnRachReq )
SIG_DEF( SIG_CPHY_INTER_FREQ_MEAS_ON_RACH_CNF,      CphyInterFreqMeasOnRachCnf      cphyInterFreqMeasOnRachCnf )
#endif
#endif /* ENABLE_INTER_FREQ_MEAS_ON_RACH */
SIG_DEF( SIG_CPHY_STOP_INTRA_FREQ_CELL_MEAS_REQ,    CphyStopIntraFreqCellMeasReq    cphyStopIntraFreqCellMeasReq )
SIG_DEF( SIG_CPHY_STOP_INTER_FREQ_CELL_MEAS_REQ,    CphyStopInterFreqCellMeasReq    cphyStopInterFreqCellMeasReq )
SIG_DEF( SIG_CPHY_STOP_DETECTED_CELL_MEAS_REQ,      CphyStopDetectedCellMeasReq     cphyStopDetectedCellMeasReq )
SIG_DEF( SIG_CPHY_STOP_UE_INTERNAL_MEAS_REQ,        CphyStopUeInternalMeasReq       cphyStopUeInternalMeasReq )
SIG_DEF( SIG_CPHY_DETECTED_CELL_MEASUREMENT_REQ,    CphyDetectedCellMeasurementReq  cphyDetectedCellMeasurementReq )
SIG_DEF( SIG_CPHY_DETECTED_CELL_MEASUREMENT_IND,    CphyDetectedCellMeasurementInd  cphyDetectedCellMeasurementInd )
SIG_DEF( SIG_CPHY_UE_INTERNAL_MEASUREMENT_REQ,      CphyUeInternalMeasurementReq    cphyUeInternalMeasurementReq )
SIG_DEF( SIG_CPHY_UE_INTERNAL_MEASUREMENT_IND,      CphyUeInternalMeasurementInd    cphyUeInternalMeasurementInd )
SIG_DEF( SIG_CPHY_SYNC_IND,                         CphySyncInd                     cphySyncInd )
SIG_DEF( SIG_CPHY_OUT_OF_SYNC_IND,                  CphyOutOfSyncInd                cphyOutOfSyncInd )

SIG_DEF( SIG_CPHY_DEACTIVATE_REQ,                   CphyDeactivateReq               cphyDeactivateReq )
SIG_DEF( SIG_CPHY_DEACTIVATE_CNF,                   CphyDeactivateCnf               cphyDeactivateCnf )

#ifdef UPGRADE_3G_FDD
SIG_DEF( SIG_CPHY_RL_RELEASE_REQ,                   CphyRlReleaseReq                cphyRlReleaseReq )
SIG_DEF( SIG_CPHY_COMPRESSED_MODE_CONFIG_REQ,       CphyCompressedModeConfigReq     cphyCompressedModeConfigReq )
SIG_DEF( SIG_CPHY_COMPRESSED_MODE_ERROR_IND,        CphyCompressedModeErrorInd      cphyCompressedModeErrorInd )
#endif /*UPGRADE_3G_FDD*/

SIG_DEF( SIG_CPHY_RSSI_SCAN_REQ,                    CphyRssiScanReq                 cphyRssiScanReq )
SIG_DEF( SIG_CPHY_RSSI_SCAN_CNF,                    CphyRssiScanCnf                 cphyRssiScanCnf )
SIG_DEF( SIG_CPHY_DL_TRCH_CRC_REQ,                  CphyDlTrchCrcReq                cphyDlTrchCrcReq )
SIG_DEF( SIG_CPHY_DL_TRCH_CRC_CNF,                  CphyDlTrchCrcCnf                cphyDlTrchCrcCnf )
SIG_DEF( SIG_CPHY_GSM_CELL_INFO_REQ,                CphyGsmCellInfoReq              cphyGsmCellInfoReq )
SIG_DEF( SIG_CPHY_MONITOR_GSM_CELL_REQ,             CphyMonitorGsmCellReq           cphyMonitorGsmCellReq )
SIG_DEF( SIG_CPHY_MONITOR_GSM_CELL_IND,             CphyMonitorGsmCellInd           cphyMonitorGsmCellInd )
SIG_DEF( SIG_CPHY_STOP_GSM_CELL_MEAS_REQ,           CphyStopGsmCellMeasReq          cphyStopGsmCellMeasReq )

/* Start of bsic decode signals used by MCR. */
SIG_DEF( SIG_CPHY_GSM_BSIC_DECODE_REQ,              CphyBsicDecodeReq               cphyBsicDecodeReq )
SIG_DEF( SIG_CPHY_GSM_BSIC_DECODE_IND,              CphyBsicDecodeInd               cphyBsicDecodeInd )
/* End of bsic decode signals used by MCR. */

/* Start of signals used for GSM PLMN search when the 3G stack is active. */
SIG_DEF( SIG_CPHY_GSM_LIST_BCCH_REQ,                CphyGsmListBcchReq              cphyGsmListBcchReq )
SIG_DEF( SIG_CPHY_GSM_LIST_MEAS_IND,                CphyGsmListMeasInd              cphyGsmListMeasInd )
SIG_DEF( SIG_CPHY_GSM_LIST_BSIC_IND,                CphyGsmListBsicInd              cphyGsmListBsicInd )
SIG_DEF( SIG_CPHY_GSM_BCCH_DECODE_IND,              CphyGsmBcchDecodeInd            cphyGsmBcchDecodeInd )
SIG_DEF( SIG_CPHY_GSM_LIST_NEXT_REQ,                CphyGsmListNextReq              cphyGsmListNextReq )
SIG_DEF( SIG_CPHY_GSM_LIST_STOP_REQ,                CphyGsmListStopReq              cphyGsmListStopReq )
/* End of signals used for GSM PLMN search when the 3G stack is active. */

SIG_DEF( SIG_CPHY_CELL_SELECT_IND,                  CphyCellSelectInd               cphyCellSelectInd )

SIG_DEF( SIG_CPHY_SUSPEND_IND,                      CphySuspendInd                  cphySuspendInd )

SIG_DEF( SIG_CPHY_DRX_RSSI_SCAN_REQ,                CphyDrxRssiScanReq              cphyDrxRssiScanReq )
SIG_DEF( SIG_CPHY_DRX_RSSI_SCAN_CNF,                CphyDrxRssiScanCnf              cphyDrxRssiScanCnf )
SIG_DEF( SIG_CPHY_DRX_FIND_CELL_REQ,                CphyDrxFindCellReq              cphyDrxFindCellReq )
SIG_DEF( SIG_CPHY_DRX_FIND_CELL_CNF,                CphyDrxFindCellCnf              cphyDrxFindCellCnf )
SIG_DEF( SIG_CPHY_DRX_NEXT_CELL_REQ,                CphyDrxNextCellReq              cphyDrxNextCellReq )
SIG_DEF( SIG_CPHY_DRX_NEXT_CELL_CNF,                CphyDrxNextCellCnf              cphyDrxNextCellCnf )


# if defined(UPGRADE_3G_HSDPA)
SIG_DEF( SIG_CPHY_DL_HS_DSCH_TR_CH_CONFIG_REQ,      CphyDlHsDschTrChConfigReq       cphyDlHsDschTrChConfigReq )
SIG_DEF( SIG_CPHY_HS_SCCH_CONFIG_REQ,               CphyHsScchConfigReq             cphyHsScchConfigReq )
SIG_DEF( SIG_CPHY_HS_DSCH_RELEASE_REQ,              CphyHsDschReleaseReq            cphyHsDschReleaseReq )
# else /* UPGRADE_3G_HSDPA */
SIG_DEF( SIG_CPHY_DL_HS_DSCH_TR_CH_CONFIG_REQ,      EmptySignal                     cphyDlHsDschTrChConfigReq )
SIG_DEF( SIG_CPHY_HS_SCCH_CONFIG_REQ,               EmptySignal                     cphyHsScchConfigReq )
SIG_DEF( SIG_CPHY_HS_DSCH_RELEASE_REQ,              EmptySignal                     cphyHsDschReleaseReq )
# endif /* UPGRADE_3G_HSDPA */
SIG_DEF( SIG_CPHY_UE_CAPABILITY_CONFIG_CNF,         CphyUeCapabilityConfigCnf       cphyUeCapabilityConfigCnf )
# if defined(UPGRADE_3G_HSUPA)
SIG_DEF( SIG_CPHY_EDCH_CONFIG_REQ,                  CphyEdchConfigReq               cphyEdchConfigReq )
SIG_DEF( SIG_CPHY_EDCH_RELEASE_REQ,                 CphyEdchReleaseReq              cphyEdchReleaseReq )
# else /* UPGRADE_3G_HSUPA */
SIG_DEF( SIG_CPHY_EDCH_CONFIG_REQ,                  EmptySignal                     cphyEdchConfigReq )
SIG_DEF( SIG_CPHY_EDCH_RELEASE_REQ,                 EmptySignal                     cphyEdchReleaseReq )
# endif /* UPGRADE_3G_HSUPA */

SIG_DEF( SIG_CPHY_CELL_SEARCH_REQ,                  CphyCellSearchReq               cphyCellSearchReq )
SIG_DEF( SIG_CPHY_QUALITY_MEASUREMENT_REQ,          CphyQualityMeasurementReq       cphyQualityMeasurementReq )
SIG_DEF( SIG_CPHY_QUALITY_MEASUREMENT_IND,          CphyQualityMeasurementInd       cphyQualityMeasurementInd )
SIG_DEF( SIG_CPHY_QUALITY_MEASUREMENT_STOP_REQ,     CphyQualityMeasurementStopReq   cphyQualityMeasurementStopReq )
SIG_DEF( SIG_CPHY_GET_ADDITIONAL_QUALITY_MEASUREMENT_REQ,   CphyGetAdditionalQualityMeasurementReq cphyGetAdditionalQualityMeasurementReq )
SIG_DEF( SIG_CPHY_GET_ADDITIONAL_QUALITY_MEASUREMENT_CNF,   CphyGetAdditionalQualityMeasurementCnf cphyGetAdditionalQualityMeasurementCnf )

SIG_DEF( SIG_CPHY_MODE_SELECT_REQ,                  CphyModeSelectReq               cphyModeSelectReq )
SIG_DEF( SIG_CPHY_MODE_SELECT_CNF,                  CphyModeSelectCnf               cphyModeSelectCnf )
SIG_DEF( SIG_CPHY_TEST_MODE_CONFIG_REQ,             CphyTestModeConfigReq           cphyTestModeConfigReq )

#  if defined (DEVELOPMENT_VERSION)
SIG_DEF( SIG_CPHY_DEBUG_MEASUREMENT_OFFSET_REQ,     CphyDebugMeasurementOffsetReq   cphyDebugMeasurementOffsetReq )
#  endif

#ifdef UPGRADE_3G_FDD
#ifdef ENABLE_UL_TX_CONTROL
SIG_DEF( SIG_CPHY_UL_TX_CONTROL_REQ,                CphyUlTxControlReq              cphyUlTxControlReq )
#endif /* ENABLE_UL_TX_CONTROL */
#endif /* UPGRADE_3G_FDD */
#  if defined (UPGRADE_LTE)
SIG_DEF( SIG_CPHY_EUTRA_MEASUREMENT_REQ,            CphyEutraMeasurementReq         cphyEutraMeasurementReq )
SIG_DEF( SIG_CPHY_EUTRA_MEASUREMENT_IND,            CphyEutraMeasurementInd         cphyEutraMeasurementInd )
SIG_DEF( SIG_CPHY_EUTRA_RSSI_SCAN_REQ,              CphyEutraRssiScanReq            cphyEutraRssiScanReq )
SIG_DEF( SIG_CPHY_EUTRA_RSSI_SCAN_CNF,              CphyEutraRssiScanCnf            cphyEutraRssiScanCnf )
SIG_DEF( SIG_CPHY_EUTRA_FIND_CELL_REQ,              CphyEutraFindCellReq            cphyEutraFindCellReq )
SIG_DEF( SIG_CPHY_EUTRA_FIND_CELL_CNF,              CphyEutraFindCellCnf            cphyEutraFindCellCnf )
SIG_DEF( SIG_CPHY_EUTRA_BCH_DECODE_REQ,             CphyEutraBchDecodeReq           cphyEutraBchDecodeReq )
SIG_DEF( SIG_CPHY_EUTRA_BCH_DECODE_IND,             CphyEutraBchDecodeInd           cphyEutraBchDecodeInd )
#  endif /* UPGRADE_LTE */
#endif /* EXCLUDE_CPHY */

#if !defined (EXCLUDE_UPHY)
SIG_DEF( SIG_UPHY_DUMMY = UPHY_SIGNAL_BASE,         EmptySignal                     uphy_dummy )
SIG_DEF( SIG_PHY_ACCESS_REQ,                        PhyAccessReq                    phyAccessReq )
SIG_DEF( SIG_PHY_ACCESS_CNF,                        PhyAccessCnf                    phyAccessCnf )
SIG_DEF( SIG_PHY_DATA_REQ,                          PhyDataReq                      phyDataReq )
SIG_DEF( SIG_PHY_DATA_IND,                          PhyDataInd                      phyDataInd )
#if defined(UPLANE_INTEGRATION_TEST)
SIG_DEF( SIG_PHY_DATA_DIS_IND,                      PhyDataDisInd                   phyDataDisInd )
#endif /* UPLANE_INTEGRATION_TEST */
SIG_DEF( SIG_PHY_STATUS_IND,                        PhyStatusInd                    phyStatusInd )
SIG_DEF( SIG_PHY_FRAME_IND,                         PhyFrameInd                     phyFrameInd )
SIG_DEF( SIG_PHY_STATE_IND,                         PhyStateInd                     phyStateInd )
# if defined(UPGRADE_3G_HSDPA)
SIG_DEF( SIG_PHY_HS_DATA_IND,                       PhyHsDataInd                    phyHsDataInd )
#if defined(UPLANE_INTEGRATION_TEST)
SIG_DEF( SIG_PHY_HS_DATA_DIS_IND,                   PhyHsDataDisInd                 phyHsDataDisInd )
#endif /* UPLANE_INTEGRATION_TEST */
SIG_DEF( SIG_PHY_HS_ASSIGN_POINTER_REQ,             PhyHsAssignPtrReq               phyHsAssignPointerReq )
SIG_DEF( SIG_PHY_HS_UNASSIGN_POINTER_IND,           EmptySignal                     phyHsUnassignPointerInd )
# else /* UPGRADE_3G_HSDPA */
SIG_DEF( SIG_PHY_HS_DATA_IND,                       EmptySignal                     phyHsDataInd )
SIG_DEF( SIG_PHY_HS_ASSIGN_POINTER_REQ,             EmptySignal                     phyHsAssignPointerReq )
SIG_DEF( SIG_PHY_HS_UNASSIGN_POINTER_IND,           EmptySignal                     phyHsUnassignPointerInd )
# endif /* UPGRADE_3G_HSDPA */
# if defined(UPGRADE_3G_HSUPA)
SIG_DEF( SIG_PHY_EDCH_TTI_IND,                      PhyEdchTtiInd                   phyEdchTtiInd )
SIG_DEF( SIG_PHY_EDCH_DATA_REQ,                     PhyEdchDataReq                  phyEdchDataReq )
# else /* UPGRADE_3G_HSUPA */
SIG_DEF( SIG_PHY_EDCH_TTI_IND,                      EmptySignal                     phyEdchTtiInd )
SIG_DEF( SIG_PHY_EDCH_DATA_REQ,                     EmptySignal                     phyEdchDataReq )
# endif /* UPGRADE_3G_HSUPA */
# if defined(UPGRADE_3G_HSUPA) && defined(UPGRADE_3G_FDD)
SIG_DEF( SIG_PHY_EDCH_ENCODER_READY_IND,            PhyEdchEncoderReadyInd          phyEdchEncoderReadyInd )
# else /* UPGRADE_3G_HSUPA && UPGRADE_3G_FDD*/
SIG_DEF( SIG_PHY_EDCH_ENCODER_READY_IND,            EmptySignal                     phyEdchEncoderReadyInd )
# endif /* UPGRADE_3G_HSUPA && UPGRADE_3G_FDD*/
#if defined (DEVELOPMENT_VERSION)||defined(T1_UPHY_STUB_ENABLE)
SIG_DEF( SIG_PHY_DEBUG_DL_PDU_LIST,                 PhyDebugDlPduList               phyDebugDlPduList )
SIG_DEF( SIG_PHY_DEBUG_UL_PDU_LIST,                 PhyDebugUlPduList               phyDebugUlPduList )
#else /* DEVELOPMENT_VERSION */
SIG_DEF( SIG_PHY_DEBUG_DL_PDU_LIST,                 EmptySignal                     phyDebugDlPduList )
SIG_DEF( SIG_PHY_DEBUG_UL_PDU_LIST,                 EmptySignal                     phyDebugUlPduList )
#endif /* DEVELOPMENT_VERSION */

#if defined(UPGRADE_3G_TDD128)
SIG_DEF( SIG_PHY_SUB_FRAME_IND,                     PhySubFrameInd                  phySubFrameInd )
#else /* UPGRADE_3G_TDD128 */
SIG_DEF( SIG_PHY_SUB_FRAME_IND,                     EmptySignal                     phySubFrameInd )
#endif /* UPGRADE_3G_TDD128 */

#endif /* EXCLUDE_UPHY */

/*for integration testing in PC*/
#ifdef  PC_INTEGRATE_TDD128
SIG_DEF( SIG_PC_INT_START_REQ,       PcIntStartReq          pcIntStartReq)
SIG_DEF( SIG_PC_INT_START_CNF,       PcIntStartCnf          pcIntStartCnf)
SIG_DEF( SIG_PC_INT_START_IND,       PcIntStartInd          pcIntStartInd)
#endif

#if defined(T1_UPHY_STUB_ENABLE)
SIG_DEF( SIG_UPHY_STUB_TEST_REQ,         UphyStubTestReq     uphyStubTestReq )
SIG_DEF( SIG_UPHY_STUB_TEST_CNF,         UphyStubTestCnf     uphyStubTestCnf )
#endif

#endif
/* END OF FILE */

