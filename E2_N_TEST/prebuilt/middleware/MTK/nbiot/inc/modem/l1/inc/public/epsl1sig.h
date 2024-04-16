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
 *                   interfaces between LTE PS and LTE L1.
 **************************************************************************/

#if defined (UPGRADE_LTE)

# if !defined (EXCLUDE_ECPHY)
SIG_DEF( SIG_ECPHY_DUMMY = ECPHY_SIGNAL_BASE,         EmptySignal                        ecphyDummy )
SIG_DEF( SIG_ECPHY_MODE_SELECT_REQ,                   EcphyModeSelectReq                 ecphyModeSelectReq )
SIG_DEF( SIG_ECPHY_MODE_SELECT_CNF,                   EcphyModeSelectCnf                 ecphyModeSelectCnf )
SIG_DEF( SIG_ECPHY_DEACTIVATE_REQ,                    EcphyDeactivateReq                 ecphyDeactivateReq )
SIG_DEF( SIG_ECPHY_DEACTIVATE_CNF,                    EcphyDeactivateCnf                 ecphyDeactivateCnf )
SIG_DEF( SIG_ECPHY_RSSI_SCAN_REQ,                     EcphyRssiScanReq                   ecphyRssiScanReq )
SIG_DEF( SIG_ECPHY_RSSI_SCAN_CNF,                     EcphyRssiScanCnf                   ecphyRssiScanCnf )
SIG_DEF( SIG_ECPHY_FIND_CELL_REQ,                     EcphyFindCellReq                   ecphyFindCellReq )
SIG_DEF( SIG_ECPHY_FIND_CELL_CNF,                     EcphyFindCellCnf                   ecphyFindCellCnf )
SIG_DEF( SIG_ECPHY_CELL_SELECT_REQ,                   EcphyCellSelectReq                 ecphyCellSelectReq )
SIG_DEF( SIG_ECPHY_CELL_SELECT_CNF,                   EcphyCellSelectCnf                 ecphyCellSelectCnf )
SIG_DEF( SIG_ECPHY_DRX_RSSI_SCAN_REQ,                 EcphyDrxRssiScanReq                ecphyDrxRssiScanReq )
SIG_DEF( SIG_ECPHY_DRX_RSSI_SCAN_CNF,                 EcphyDrxRssiScanCnf                ecphyDrxRssiScanCnf )
SIG_DEF( SIG_ECPHY_DRX_FIND_CELL_REQ,                 EcphyDrxFindCellReq                ecphyDrxFindCellReq )
SIG_DEF( SIG_ECPHY_DRX_FIND_CELL_CNF,                 EcphyDrxFindCellCnf                ecphyDrxFindCellCnf )
SIG_DEF( SIG_ECPHY_NEXT_CELL_REQ,                     EcphyNextCellReq                   ecphyNextCellReq )
SIG_DEF( SIG_ECPHY_NEXT_CELL_CNF,                     EcphyNextCellCnf                   ecphyNextCellCnf )
SIG_DEF( SIG_ECPHY_BCH_REQ,                           EcphyBchReq                        ecphyBchReq )
SIG_DEF( SIG_ECPHY_NCELL_BCH_REQ,                     EcphyNcellBchReq                   ecphyNcellBchReq )
SIG_DEF( SIG_ECPHY_IDLE_MODE_CONFIG_REQ,              EcphyIdleModeConfigReq             ecphyIdleModeConfigReq )
SIG_DEF( SIG_ECPHY_IDLE_MODE_CONFIG_CNF,              EcphyIdleModeConfigCnf             ecphyIdleModeConfigCnf )
SIG_DEF( SIG_ECPHY_CONNECTED_MODE_CONFIG_REQ,         EcphyConnectedModeConfigReq        ecphyConnectedModeConfigReq )
SIG_DEF( SIG_ECPHY_CONNECTED_MODE_CONFIG_CNF,         EcphyConnectedModeConfigCnf        ecphyConnectedModeConfigCnf )
SIG_DEF( SIG_ECPHY_RL_FAILURE_IND,                    EcphyRlFailureInd                  ecphyRlFailureInd )
SIG_DEF( SIG_ECPHY_RACH_FAILURE_IND,                  EcphyRachFailureInd                ecphyRachFailureInd )
SIG_DEF( SIG_ECPHY_RACH_SUCCESS_IND,                  EcphyRachSuccessInd                ecphyRachSuccessInd )
SIG_DEF( SIG_ECPHY_RELEASE_PUCCH_SRS_IND,             EcphyReleasePucchSrsInd            ecphyReleasePucchSrsInd )
SIG_DEF( SIG_ECPHY_RACH_INFO_REQ,                     EcphyRachInfoReq                   ecphyRachInfoReq )
SIG_DEF( SIG_ECPHY_RACH_INFO_CNF,                     EcphyRachInfoCnf                   ecphyRachInfoCnf )
SIG_DEF( SIG_ECPHY_INTRA_FREQUENCY_MEASUREMENT_REQ,   EcphyIntraFrequencyMeasurementReq  ecphyIntraFrequencyMeasurementReq )
SIG_DEF( SIG_ECPHY_INTRA_FREQUENCY_MEASUREMENT_IND,   EcphyIntraFrequencyMeasurementInd  ecphyIntraFrequencyMeasurementInd )
SIG_DEF( SIG_ECPHY_INTER_FREQUENCY_MEASUREMENT_REQ,   EcphyInterFrequencyMeasurementReq  ecphyInterFrequencyMeasurementReq )
SIG_DEF( SIG_ECPHY_INTER_FREQUENCY_MEASUREMENT_IND,   EcphyInterFrequencyMeasurementInd  ecphyInterFrequencyMeasurementInd )
SIG_DEF( SIG_ECPHY_SEARCH_MODE_REQ,                   EcphySearchModeReq                 ecphySearchModeReq )

#  if defined (UPGRADE_GGE)
SIG_DEF( SIG_ECPHY_GSM_MEASUREMENT_REQ,               EcphyGsmMeasurementReq             ecphyGsmMeasurementReq )
SIG_DEF( SIG_ECPHY_GSM_MEASUREMENT_IND,               EcphyGsmMeasurementInd             ecphyGsmMeasurementInd )
SIG_DEF( SIG_ECPHY_GSM_BCCH_DECODE_REQ,               EcphyGsmBcchDecodeReq              ecphyGsmBcchDecodeReq )
SIG_DEF( SIG_ECPHY_GSM_LIST_BCCH_REQ,                 EcphyGsmListBcchReq                ecphyGsmListBcchReq )
SIG_DEF( SIG_ECPHY_GSM_LIST_MEAS_IND,                 EcphyGsmListMeasInd                ecphyGsmListMeasInd )
SIG_DEF( SIG_ECPHY_GSM_LIST_BSIC_IND,                 EcphyGsmListBsicInd                ecphyGsmListBsicInd )
SIG_DEF( SIG_ECPHY_GSM_LIST_NEXT_REQ,                 EcphyGsmListNextReq                ecphyGsmListNextReq )
SIG_DEF( SIG_ECPHY_GSM_LIST_STOP_REQ,                 EcphyGsmListStopReq                ecphyGsmListStopReq )
SIG_DEF( SIG_ECPHY_CELL_SELECT_IND,                   EcphyCellSelectInd                 ecphyCellSelectInd )
#  endif /* UPGRADE_GGE */

#  if defined (UPGRADE_3G)
SIG_DEF( SIG_ECPHY_UTRA_MEASUREMENT_REQ,              EcphyUtraMeasurementReq            ecphyUtraMeasurementReq )
SIG_DEF( SIG_ECPHY_UTRA_MEASUREMENT_IND,              EcphyUtraMeasurementInd            ecphyUtraMeasurementInd )
SIG_DEF( SIG_ECPHY_UTRA_BCCH_DECODE_REQ,              EcphyUtraBcchDecodeReq             ecphyUtraBcchDecodeReq )
SIG_DEF( SIG_ECPHY_UTRA_RSSI_SCAN_REQ,                EcphyUtraRssiScanReq               ecphyUtraRssiScanReq )
SIG_DEF( SIG_ECPHY_UTRA_RSSI_SCAN_CNF,                EcphyUtraRssiScanCnf               ecphyUtraRssiScanCnf )
SIG_DEF( SIG_ECPHY_UTRA_FIND_CELL_REQ,                EcphyUtraFindCellReq               ecphyUtraFindCellReq )
SIG_DEF( SIG_ECPHY_UTRA_FIND_CELL_CNF,                EcphyUtraFindCellCnf               ecphyUtraFindCellCnf )
#  endif /* UPGRADE_3G */

#  if defined (UPGRADE_3G) || defined (UPGRADE_GGE)
SIG_DEF( SIG_ECPHY_RESELECT_TO_EUTRA_FAIL_REQ,        EcphyReselectToEutraFailReq        ecphyReselectToEutraFailReq )
SIG_DEF( SIG_ECPHY_RESELECT_TO_EUTRA_FAIL_CNF,        EcphyReselectToEutraFailCnf        ecphyReselectToEutraFailCnf )
SIG_DEF( SIG_ECPHY_HANDOVER_TO_EUTRA_FAIL_REQ,        EcphyHandoverToEutraFailReq        ecphyHandoverToEutraFailReq )
SIG_DEF( SIG_ECPHY_HANDOVER_TO_EUTRA_FAIL_CNF,        EcphyHandoverToEutraFailCnf        ecphyHandoverToEutraFailCnf )
#  endif /* UPGRADE_3G || UPGRADE_GGE */

# endif /* EXCLUDE_ECPHY */

# if !defined (EXCLUDE_ESPHY)
SIG_DEF( SIG_ESPHY_DUMMY = ESPHY_SIGNAL_BASE,         EmptySignal                        esphyDummy )
SIG_DEF( SIG_ESPHY_BCCH_DATA_IND,                     EsphyBcchDataInd                   esphyBcchDataInd )
SIG_DEF( SIG_ESPHY_PCCH_DATA_IND,                     EsphyPcchDataInd                   esphyPcchDataInd )
#  if defined (UPGRADE_GGE)
SIG_DEF( SIG_ESPHY_GSM_BCCH_DECODE_IND,               EsphyGsmBcchDecodeInd              esphyGsmBcchDecodeInd )
#  endif /* UPGRADE_GGE */
#  if defined (UPGRADE_3G)
SIG_DEF( SIG_ESPHY_UTRA_BCCH_DECODE_IND,              EsphyUtraBcchDecodeInd             esphyUtraBcchDecodeInd )
#  endif /* UPGRADE_3G */
# endif /* EXCLUDE_ESPHY */

# if !defined (EXCLUDE_EUPHY)
SIG_DEF( SIG_EUPHY_DUMMY = EUPHY_SIGNAL_BASE,         EmptySignal                        euphyDummy )
SIG_DEF( SIG_EUPHY_DATA_IND,                          EuphyDataInd                       euphyDataInd )
SIG_DEF( SIG_EUPHY_TRANSMIT_IND,                      EuphyTransmitInd                   euphyTransmitInd )
SIG_DEF( SIG_EUPHY_FLUSH_IND,                         EuphyFlushInd                      euphyFlushInd )
SIG_DEF( SIG_EUPHY_MAC_RESET_IND,                     EuphyMacResetInd                   euphyMacResetInd )
SIG_DEF( SIG_EUPHY_RACH_ORDER_IND,                    EuphyRachOrderInd                  euphyRachOrderInd )
SIG_DEF( SIG_EUPHY_RACH_ORDER_RSP,                    EuphyRachOrderRsp                  euphyRachOrderRsp )
SIG_DEF( SIG_EUPHY_SCHEDULING_REQ,                    EuphySchedulingReq                 euphySchedulingReq )
SIG_DEF( SIG_EUPHY_ZEROMACSDU_REQ,                    EuphyZeroMacSduReq                 euphyZeroMacSduReq )
# endif /* EXCLUDE_EUPHY*/

# if defined (ENABLE_R_S_VIRTUAL_TESTER)
SIG_DEF( SIG_VT_DUMMY = VT_SIGNAL_BASE,               EmptySignal                        vtDummy )
SIG_DEF( SIG_VT_SYS_START_REQ,                        VtSysStartReq                      vtSysStartReq)
SIG_DEF( SIG_VT_SYS_START_CNF,                        VtSysStartCnf                      vtSysStartCnf)
SIG_DEF( SIG_VT_SYS_TRIGGER_IND,                      VtSysTriggerInd                    vtSysTriggerInd)
SIG_DEF( SIG_VT_SYS_READY_IND,                        VtSysReadyInd                      vtSysReadyInd)
SIG_DEF( SIG_VT_SYS_READY_RSP,                        VtSysReadyRsp                      vtSysReadyRsp)
SIG_DEF( SIG_VT_SYS_STOP_REQ,                         VtSysStopReq                       vtSysStopReq)
SIG_DEF( SIG_VT_SYS_STOP_CNF,                         VtSysStopCnf                       vtSysStopCnf)
SIG_DEF( SIG_VT_SYS_STOP_IND,                         VtSysStopInd                       vtSysStopInd)
SIG_DEF_CUSTOM_REF( SIG_VT_PHY_DATA_REQ,              VtUePhyDataReq,                    vtUePhyDataReq,
                                                      VtUePhyDataReqRefDataPayLoad,      RefLogVtUePhyDataReq)
SIG_DEF_CUSTOM_REF( SIG_VT_PHY_DATA_IND,              VtUePhyDataInd,                    vtUePhyDataInd,
                                                      VtUePhyDataIndRefDataPayLoad,      RefLogVtUePhyDataInd)
SIG_DEF( SIG_VT_SYS_INIT_REQ,                         VtSysInitReq                       vtSysInitReq)
SIG_DEF( SIG_VT_SYS_INIT_CNF,                         VtSysInitCnf                       vtSysInitCnf)
SIG_DEF( SIG_VT_MESSAGE_IND,                          VtMessage                          vtMessageInd)
SIG_DEF( SIG_VT_DATA_UNIT_IND,                        VtDataUnitRef                      vtDataUnitInd)
SIG_DEF( SIG_VT_MESSAGE_REQ,                          VtMessage                          vtMessageReq)
SIG_DEF( SIG_VT_DATA_UNIT_REQ,                        VtDataUnitRef                      vtDataUnitReq)

SIG_DEF( SIG_VT_MESSAGE_SYS_READY_IND,                VtMessageSysReadyInd               vtMessageSysReadyInd)
SIG_DEF( SIG_VT_MESSAGE_SYS_TRIGGER_IND,              VtMessageSysTriggerInd             vtMessageSysTriggerInd)
SIG_DEF( SIG_VT_MESSAGE_PHY_DATA_REQ,                 VtMessagePhyDataReq                vtMessagePhyDataReq)
# endif /* ENABLE_R_S_VIRTUAL_TESTER*/

#if defined (SIG_EUPHY_STUB)
SIG_DEF (SIG_EUPHY_STUB_TEST_REQ,                     EuphyStubTestReq                   euphyStubTestReq )
#endif /* EL1_TEST_PLATFORM */

#if defined (EL1_TEST_PLATFORM)
SIG_DEF( SIG_E1TEST_PACKET_REQ,                       E1testPacketReq                    e1testPacketReq)
#endif /* EL1_TEST_PLATFORM */

#endif
/* END OF FILE */
