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
 * File Description:
 *
 * 3G PS Signal definitions
 **************************************************************************/

#if defined (UPGRADE_3G)

#if !defined (EXCLUDE_CBMC)
SIG_DEF (SIG_CBMC_DUMMY = CBMC_SIGNAL_BASE,             EmptySignal cbmc_dummy)
SIG_DEF (SIG_CBMC_RX_IND,                               CbmcRxInd cbmcRxInd)
SIG_DEF (SIG_CBMC_CONFIG_IND,                           CbmcConfigInd cbmcConfigInd)
SIG_DEF (SIG_CBMC_DRX_LEVEL_1_IND,                      CbmcDrxLevel1Ind cbmcDrxLevel1Ind)
SIG_DEF (SIG_CBMC_CELL_CHANGE_IND,                      CbmcCellChangeInd cbmcCellChangeInd)
#endif /* EXCLUDE_CBMC */

#if !defined (EXCLUDE_CPDC)
SIG_DEF (SIG_CPDC_DUMMY = CPDC_SIGNAL_BASE,             EmptySignal cpdc_dummy)
SIG_DEF (SIG_CPDCP_CONFIG_REQ,                          CpdcpConfigReq cpdcpConfigReq)
SIG_DEF (SIG_CPDCP_RELEASE_REQ,                         CpdcpReleaseReq cpdcpReleaseReq)
SIG_DEF (SIG_CPDCP_RELOC_REQ,                           CpdcpRelocReq cpdcpRelocReq)
SIG_DEF (SIG_CPDCP_RELOC_CNF,                           CpdcpRelocCnf cpdcpRelocCnf)
SIG_DEF (SIG_CPDCP_STATUS_IND,                          CpdcpStatusInd cpdcpStatusInd)
SIG_DEF (SIG_CPDCP_CLOSE_TEST_LOOP_MODE_REQ,            CpdcpCloseTestLoopReq cpdcpCloseTestLoopReq)
SIG_DEF (SIG_CPDCP_OPEN_TEST_LOOP_MODE_REQ,             EmptySignal cpdcpOpenTestLoopReq)
SIG_DEF (SIG_CPDCP_SN_IND,                              CpdcpSnInd cpdcpSnInd)
#if defined (PDCP_TEST_SN)
SIG_DEF (SIG_CPDCP_SN_REQ,                              CpdcpSnReq cpdcpSnReq)
#  endif /* PDCP_TEST_SN */
#endif /* EXCLUDE_CPDC */

#if !defined (EXCLUDE_CCSD)
SIG_DEF (SIG_CCSD_DUMMY = CCSD_SIGNAL_BASE,             EmptySignal ccsd_dummy)
SIG_DEF (SIG_CCSD_CONFIG_REQ,                           CcsdConfigReq ccsdConfigReq)
SIG_DEF (SIG_CCSD_RELEASE_REQ,                          CcsdReleaseReq ccsdReleaseReq)
SIG_DEF (SIG_CCSD_SUSPEND_REQ,                          CcsdSuspendReq ccsdSuspendReq)
SIG_DEF (SIG_CCSD_RESUME_REQ,                           CcsdResumeReq ccsdResumeReq)
SIG_DEF (SIG_CCSD_UL_TTI_IND,                           CcsdUlTtiInd ccsdUlTtiInd)
SIG_DEF (SIG_CCSD_DL_TTI_IND,                           CcsdDlTtiInd ccsdDlTtiInd)
#endif /* EXCLUDE_CCSD */

#if !defined (EXCLUDE_CSDI)
SIG_DEF (SIG_CSDI_DUMMY = CSDI_SIGNAL_BASE,             EmptySignal csdi_dummy)
SIG_DEF (SIG_CSDI_CONFIG_IND,                           CsdiConfigInd csdiConfigInd)
SIG_DEF (SIG_CSDI_RELEASE_IND,                          CsdiReleaseInd csdiReleaseInd)
SIG_DEF (SIG_CSDI_SUSPEND_IND,                          CsdiSuspendInd csdiSuspendInd)
SIG_DEF (SIG_CSDI_RESUME_IND,                           CsdiResumeInd csdiResumeInd)
SIG_DEF (SIG_CSDI_CONNECT_REQ,                          CsdiConnectReq csdiConnectReq)
SIG_DEF (SIG_CSDI_DISCONNECT_REQ,                       CsdiDisconnectReq csdiDisconnectReq)
SIG_DEF (SIG_CSDI_CONNECT_CNF,                          CsdiConnectCnf csdiConnectCnf)
SIG_DEF (SIG_CSDI_DISCONNECT_CNF,                       CsdiDisconnectCnf csdiDisconnectCnf)
SIG_DEF (SIG_CSDI_TTI_IND,                              CsdiTtiInd csdiTtiInd)
SIG_DEF (SIG_CSDI_AMR_DATA_REQ,                         CsdiAmrDataReq csdiAmrDataReq)
SIG_DEF (SIG_CSDI_CSD_DATA_REQ,                         CsdiCsdDataReq csdiCsdDataReq)
SIG_DEF (SIG_CSDI_AMR_DATA_IND,                         CsdiAmrDataInd csdiAmrDataInd)
SIG_DEF (SIG_CSDI_CSD_DATA_IND,                         CsdiCsdDataInd csdiCsdDataInd)
#endif /* EXCLUDE_CSDI */

#if !defined (EXCLUDE_RRC)
SIG_DEF (SIG_RRC_DUMMY = RRC_SIGNAL_BASE,               EmptySignal rrc_dummy)
SIG_DEF (SIG_RRC_ACT_REQ,                               RrcActReq rrcActReq)
SIG_DEF (SIG_RRC_ACT_CNF,                               RrcActCnf rrcActCnf)
SIG_DEF (SIG_RRC_ACT_IND,                               RrcActInd rrcActInd)
SIG_DEF (SIG_RRC_DEACT_REQ,                             RrcDeactReq rrcDeactReq)
SIG_DEF (SIG_RRC_DEACT_CNF,                             EmptySignal rrcDeactCnf)
SIG_DEF (SIG_RRC_PAGE_IND,                              RrcPageInd rrcPageInd)
SIG_DEF (SIG_RRC_ESTABLISH_REQ,                         RrcEstablishReq rrcEstablishReq)
SIG_DEF (SIG_RRC_ESTABLISH_CNF,                         RrcEstablishCnf rrcEstablishCnf)
SIG_DEF (SIG_RRC_CONN_REL_REQ,                          RrcConnRelReq rrcConnRelReq)
SIG_DEF (SIG_RRC_CONN_REL_CNF,                          RrcConnRelCnf rrcConnRelCnf)
SIG_DEF (SIG_RRC_CONN_REL_IND,                          RrcConnRelInd rrcConnRelInd)
SIG_DEF (SIG_RRC_DATA_REQ,                              RrcDataReq rrcDataReq)
SIG_DEF (SIG_RRC_DATA_IND,                              RrcDataInd rrcDataInd)
SIG_DEF (SIG_RRC_SYS_INFO_IND,                          RrcSysInfoInd rrcSysInfoInd)
#if defined (UPGRADE_REL6) && defined (UPGRADE_3G_FDD)
SIG_DEF (SIG_RRC_DSAC_INFO_IND,                         RrcDsacInfoInd rrcDsacInfoInd)
#endif
SIG_DEF (SIG_RRC_ME_DATA_REQ,                           RrcMeDataReq rrcMeDataReq)
SIG_DEF (SIG_RRC_MS_DATA_REQ,                           RrcMsDataReq rrcMsDataReq)
SIG_DEF (SIG_RRC_SECURITY_KEY_SET_CONFIG_REQ,           RrcSecurityKeySetConfigReq rrcSecurityKeySetConfigReq)
SIG_DEF (SIG_RRC_UPDATE_REQ,                            RrcUpdateReq rrcUpdateReq)
SIG_DEF (SIG_RRC_CELL_UPDATE_IND,                       RrcCellUpdateInd rrcCellUpdateInd)
SIG_DEF (SIG_RRC_POWER_CLASS_REQ,                       RrcPowerClassReq rrcPowerClassReq)
SIG_DEF (SIG_RRC_STATUS_IND,                            RrcStatusInd rrcStatusInd)
SIG_DEF (SIG_RRC_PLMN_LIST_REQ,                         RrcPlmnListReq rrcPlmnListReq)
SIG_DEF (SIG_RRC_PLMN_LIST_CNF,                         RrcPlmnListCnf rrcPlmnListCnf)
SIG_DEF (SIG_RRC_TEST_MODE_REQ,                         RrcTestModeReq rrcTestModeReq)
SIG_DEF (SIG_RRC_DEBUG_SYSTEM_INFORMATION_IND,          RrcDebugSystemInformationInd rrcDebugSystemInformationInd)
SIG_DEF (SIG_RRC_DEBUG_AIR_INTERFACE_RX_IND,            RrcDebugAirInterfaceRxInd rrcDebugAirInterfaceRxInd)
SIG_DEF (SIG_RRC_DEBUG_AIR_INTERFACE_TX_IND,            RrcDebugAirInterfaceTxInd rrcDebugAirInterfaceTxInd)
SIG_DEF (SIG_RRC_DEBUG_GSM_AIR_INTERFACE_RX_IND,        RrcDebugGsmAirInterfaceRxInd rrcDebugGsmAirInterfaceRxInd)
SIG_DEF (SIG_RRC_DEBUG_INTEGRITY_PROTECT_FAIL_IND,      RrcDebugIntegrityProtectFailInd rrcDebugIntegrityProtectFailInd)
SIG_DEF (SIG_RRC_DEBUG_TRANSACTION_LIST_IND,            RrcDebugTransListInd rrcDebugTransListInd)
SIG_DEF (SIG_RRC_DEBUG_STATE_CHANGE_IND,                RrcDebugStateChangeInd rrcDebugStateChangeInd)
SIG_DEF (SIG_RRC_SYNC_IND,                              RrcSyncInd rrcSyncInd)
SIG_DEF (SIG_RRC_NAS_TX_QUEUE_INFO_IND,                 RrcNasTxQueueInfoInd rrcNasTxQueueInfoInd)
SIG_DEF (SIG_RRC_ENG_INFO_REQ,                          RrcEngInfoReq rrcEngInfoReq)
SIG_DEF (SIG_RRC_ENG_INFO_IND,                          RrcEngInfoInd rrcEngInfoInd)
SIG_DEF (SIG_RRC_ENG_INFO_FORCE_COMMAND_REQ,            RrcForceCommandReq rrcForceCommandReq)
SIG_DEF (SIG_RRC_ENG_INFO_FORCE_COMMAND_CNF,            RrcForceCommandCnf rrcForceCommandCnf)
SIG_DEF (SIG_RRC_NETWORK_AUTH_FAIL_REQ,                 EmptySignal rrcNetworkAuthFailReq)
SIG_DEF (SIG_RRC_DEBUG_INFORMATION_IND,                 RrcDebugInformationInd rrcDebugInformationInd)
#if defined (UPGRADE_RRC_UNIT_TEST)
SIG_DEF (SIG_RRC_UNIT_TEST_REQ,                         RrcUnitTestReq rrcUnitTestReq)
#else
SIG_DEF (SIG_RRC_UNIT_TEST_REQ,                         EmptySignal rrcUnitTestReq)
#endif
SIG_DEF (SIG_RRC_DEBUG_SYS_INFO_WITH_ERROR_IND,         RrcDebugSysInfoWithErrorInd rrcDebugSysInfoWithErrorInd)
SIG_DEF (SIG_RRC_DEBUG_MSC_IND,                         RrcDebugMscInd rrcDebugMscInd)
SIG_DEF (SIG_RRC_PLMN_LIST_IND,                         RrcPlmnListInd rrcPlmnListInd)
SIG_DEF (SIG_RRC_DEBUG_AIR_INTERFACE_STATUS_IND,        RrcDebugAirInterfaceStatusInd rrcDebugAirInterfaceStatusInd)
SIG_DEF (SIG_RRC_PLMN_LIST_ABORT_REQ,                   EmptySignal rrcPlmnListAbortReq)
SIG_DEF (SIG_RRC_MODE_SELECT_REQ,                       RrcModeSelectReq rrcModeSelectReq)
SIG_DEF (SIG_RRC_MODE_SELECT_CNF,                       RrcModeSelectCnf rrcModeSelectCnf)
#if defined(UPGRADE_3G_FDD)
SIG_DEF (SIG_RRC_DEBUG_ACTIVE_SET_IND,                  RrcDebugActiveSetInd rrcDebugActiveSetInd)
#else
SIG_DEF (SIG_RRC_DEBUG_ACTIVE_SET_IND,                  EmptySignal rrcDebugActiveSetInd)
#endif

#if defined (UPGRADE_3G_FDD) && defined (ENABLE_ENHANCED_RL_MAINTENANCE)
SIG_DEF (SIG_RRC_DEBUG_ENHANCED_RL_MAINTENANCE_IND,     RrcDebugEnhancedRlMaintenanceInd rrcDebugEnhancedRlMaintenanceInd)
#else
SIG_DEF (SIG_RRC_DEBUG_ENHANCED_RL_MAINTENANCE_IND,     EmptySignal rrcDebugEnhancedRlMaintenanceInd)
#endif /* UPGRADE_3G_FDD && ENABLE_ENHANCED_RL_MAINTENANCE */

#if defined (UPGRADE_3G_TDD128)
SIG_DEF (SIG_RRC_DEBUG_CPHY_MONITOR_INTRA_FREQ_CELL_IND,RrcDebugIntraFreqMeasInd rrcDebugIntraFreqMeasInd)
SIG_DEF (SIG_RRC_DEBUG_CPHY_MONITOR_INTER_FREQ_CELL_IND,RrcDebugInterFreqMeasInd rrcDebugInterFreqMeasInd)
#endif /* UPGRADE_3G_TDD128 */
SIG_DEF (SIG_RRC_DEBUG_CONFIG_INFO_IND,                 RrcDebugConfigInfoInd         rrcDebugConfigInfoInd)
SIG_DEF (SIG_RRC_DEBUG_GLOBAL_VARIABLE_IND,             RrcDebugGlobalVariableInd     rrcDebugGlobalVariableInd)
#endif /* EXCLUDE_RRC */

#if !defined (EXCLUDE_CRLC)
SIG_DEF (SIG_CRLC_DUMMY = CRLC_SIGNAL_BASE,             EmptySignal crlc_dummy)
SIG_DEF (SIG_CRLC_CONFIG_REQ,                           CrlcConfigReq crlcConfigReq)
SIG_DEF (SIG_CRLC_STOP_REQ,                             CrlcStopReq crlcStopReq)
SIG_DEF (SIG_CRLC_HALT_REQ,                             CrlcHaltReq crlcHaltReq)
SIG_DEF (SIG_CRLC_CONTINUE_REQ,                         CrlcContinueReq crlcContinueReq)
SIG_DEF (SIG_CRLC_RELEASE_REQ,                          CrlcReleaseReq crlcReleaseReq)
SIG_DEF (SIG_CRLC_STATUS_IND,                           CrlcStatusInd crlcStatusInd)
SIG_DEF (SIG_CRLC_DATA_PENDING_IND,                     CrlcDataPendingInd crlcDataPendingInd)
SIG_DEF (SIG_CRLC_CLOSE_TEST_LOOP_MODE_REQ,             CrlcCloseTestLoopReq crlcCloseTestLoopReq)
SIG_DEF (SIG_CRLC_OPEN_TEST_LOOP_MODE_REQ,              EmptySignal crlcOpenTestLoopReq)
SIG_DEF (SIG_CRLC_SUSPEND_REQ,                          CrlcSuspendReq crlcSuspendReq)
SIG_DEF (SIG_CRLC_SUSPEND_CNF,                          CrlcSuspendCnf crlcSuspendCnf)
SIG_DEF (SIG_CRLC_RESUME_REQ,                           CrlcResumeReq crlcResumeReq)
SIG_DEF (SIG_CRLC_COUNTC_REQ,                           CrlcCountcReq crlcCountcReq)
SIG_DEF (SIG_CRLC_COUNTC_CNF,                           CrlcCountcCnf crlcCountcCnf)
SIG_DEF (SIG_CRLC_COUNTC_IND,                           CrlcCountcInd crlcCountcInd)
SIG_DEF (SIG_CRLC_PREPARE_CIPHER_CFG_CHANGE_REQ,        CrlcPrepareCipherCfgChangeReq crlcPrepareCipherCfgChangeReq)
SIG_DEF (SIG_CRLC_PREPARE_CIPHER_CFG_CHANGE_CNF,        CrlcPrepareCipherCfgChangeCnf crlcPrepareCipherCfgChangeCnf)
SIG_DEF (SIG_CRLC_ABORT_CIPHER_CONFIG_REQ,              CrlcAbortCipherConfigReq crlcAbortCipherConfigReq)
SIG_DEF (SIG_CRLC_TEST_MODE_REQ,                        CrlcTestModeReq crlcTestModeReq)
SIG_DEF (SIG_CRLC_TEST_MODE_DIAGNOSTICS_RSP,            CrlcTestModeDiagnosticsRsp crlcTestModeDiagnosticsRsp)
SIG_DEF (SIG_CRLC_DEACTIVATE_REQ,                       EmptySignal crlcDeactivateReq)
# if defined (ENABLE_URLC_UNIT_TEST)
SIG_DEF (SIG_CRLC_TEST_MODE_UL_SDU_SEGMENTED,           CrlcUnitTestUlAmSduSegmented crlcUnitTestUlAmSduSegmented)
SIG_DEF (SIG_CRLC_TEST_MODE_UL_SDU_EXPIRED,             CrlcUnitTestUlAmSduExpired crlcUnitTestUlAmSduExpired)
SIG_DEF (SIG_CRLC_TEST_MODE_UL_SDU_DELETED,             CrlcUnitTestUlAmSduDeleted crlcUnitTestUlAmSduDeleted)
# else
SIG_DEF (SIG_CRLC_TEST_MODE_UL_SDU_SEGMENTED,           EmptySignal crlcUnitTestUlAmSduSegmented)
SIG_DEF (SIG_CRLC_TEST_MODE_UL_SDU_EXPIRED,             EmptySignal crlcUnitTestUlAmSduExpired)
SIG_DEF (SIG_CRLC_TEST_MODE_UL_SDU_DELETED,             EmptySignal crlcUnitTestUlAmSduDeleted)
# endif /* ENABLE_URLC_UNIT_TEST */
SIG_DEF (SIG_CRLC_DL_AM_PDU_SIZE_IND,                   CrlcDlAmPduSizeInd crlcDlAmPduSizeInd)

#if defined (UPGRADE_3G_MBMS)
#if defined (UPGRADE_3G_TDD128)
SIG_DEF (SIG_CRLC_TEST_LOOP_COUNTER_REQ,                CrlcTestLoopCounterReq crlcTestLoopCounterReq)
SIG_DEF (SIG_CRLC_TEST_LOOP_COUNTER_CNF,                CrlcTestLoopCounterCnf crlcTestLoopCounterCnf)
#endif /* UPGRADE_3G_TDD128 */
#endif /* UPGRADE_3G_MBMS */
#endif /* EXCLUDE_CRLC */

#if !defined (EXCLUDE_URLC)
SIG_DEF (SIG_URLC_DUMMY = URLC_SIGNAL_BASE,             EmptySignal urlc_dummy)
SIG_DEF (SIG_URLC_TM_DATA_REQ,                          UrlcTmDataReq urlcTmDataReq)
SIG_DEF (SIG_URLC_TM_DATA_IND,                          UrlcTmDataInd urlcTmDataInd)
SIG_DEF (SIG_URLC_UM_DATA_REQ,                          UrlcUmDataReq urlcUmDataReq)
SIG_DEF (SIG_URLC_UM_DATA_TRANSMITTED_IND,              UrlcUmDataTransmittedInd urlcUmDataTransmittedInd)
SIG_DEF (SIG_URLC_UM_DATA_IND,                          UrlcUmDataInd urlcUmDataInd)
SIG_DEF (SIG_URLC_AM_DATA_REQ,                          UrlcAmDataReq urlcAmDataReq)
SIG_DEF (SIG_URLC_AM_DATA_CNF,                          UrlcAmDataCnf urlcAmDataCnf)
SIG_DEF (SIG_URLC_AM_DATA_IND,                          UrlcAmDataInd urlcAmDataInd)
SIG_DEF (SIG_URLC_AM_DL_SDU_DISCARD_IND,                UrlcAmDlSduDiscardInd urlcAmDlSduDiscardInd)
SIG_DEF (SIG_URLC_UL_SDU_DISCARD_IND,                   UrlcUlSduDiscardInd urlcUlSduDiscardInd)
SIG_DEF (SIG_URLC_XOFF_IND,                             UrlcXoffInd urlcXoffInd)
SIG_DEF (SIG_URLC_XON_IND,                              UrlcXonInd urlcXonInd)
SIG_DEF (SIG_URLC_UL_CONFIG_CHANGE_IND,                 UrlcUlConfigChangeInd urlcUlConfigChangeInd)
SIG_DEF (SIG_URLC_DEBUG_MAC_DATA_REQ,                   UrlcDebugMacDataReq urlcDebugMacDataReq)
SIG_DEF (SIG_URLC_DEBUG_MAC_DATA_IND,                   UrlcDebugMacDataInd urlcDebugMacDataInd)
SIG_DEF (SIG_URLC_AMR_TM_DATA_REQ,                      UrlcAmrTmDataReq urlcAmrTmDataReq)
SIG_DEF (SIG_URLC_CSD_TM_DATA_REQ,                      UrlcCsdTmDataReq urlcCsdTmDataReq)
SIG_DEF (SIG_URLC_DL_CONFIG_CHANGE_IND,                 UrlcDlConfigChangeInd urlcDlConfigChangeInd)
SIG_DEF (SIG_URLC_AM_DATA_RSP,                          EmptySignal urlc_dummy1)
SIG_DEF (SIG_URLC_DEBUG_PARAM,                          UrlcDebugParam urlcDebugParam)
SIG_DEF (SIG_URLC_DEBUG_AM_TXRX_ENTITY_IND,             UrlcDebugAmTxRxEntityInd urlcDebugAmTxRxEntityInd)
SIG_DEF (SIG_URLC_DEBUG_UM_TX_ENTITY_IND,               UrlcDebugUmTxEntityInd urlcDebugUmTxEntityInd)
SIG_DEF (SIG_URLC_DEBUG_UM_RX_ENTITY_IND,               UrlcDebugUmRxEntityInd urlcDebugUmRxEntityInd)
SIG_DEF (SIG_URLC_DEBUG_TM_TX_ENTITY_IND,               UrlcDebugTmTxEntityInd urlcDebugTmTxEntityInd)
SIG_DEF (SIG_URLC_DEBUG_TM_RX_ENTITY_IND,               UrlcDebugTmRxEntityInd urlcDebugTmRxEntityInd)
SIG_DEF (SIG_URLC_BUSY_IND,                             EmptySignal urlcBusyInd)
#if defined (ENABLE_TRAFFIC_SHAPING)
SIG_DEF (SIG_URLC_TS_BUFFER_STATUS_REQ,                 UrlcTsBufferStatusReq urlcTsBufferStatusReq)
SIG_DEF (SIG_URLC_TS_BUFFER_STATUS_CNF,                 UrlcTsBufferStatusCnf urlcTsBufferStatusCnf)
#endif
#endif /* EXCLUDE_URLC */

#if !defined (EXCLUDE_CMAC)
SIG_DEF (SIG_CMAC_DUMMY = CMAC_SIGNAL_BASE,             EmptySignal cmac_dummy)
SIG_DEF (SIG_CMAC_RNTI_CONFIG_REQ,                      CmacRNTIConfigReq cmacRNTIConfigReq)
SIG_DEF (SIG_CMAC_CIPHERING_CONFIG_REQ,                 CmacCipheringConfigReq cmacCipheringConfigReq)
SIG_DEF (SIG_CMAC_RACH_CONFIG_REQ,                      CmacRachConfigReq cmacRachConfigReq)
SIG_DEF (SIG_CMAC_RACH_TX_STATUS_IND,                   CmacRachTxStatusInd cmacRachTxStatusInd)
SIG_DEF (SIG_CMAC_UL_DEDICATED_TFCS_CONFIG_REQ,         CmacUlDedicatedTfcsConfigReq cmacUlDedicatedTfcsConfigReq)
SIG_DEF (SIG_CMAC_UL_TFC_SUBSET_CONFIG_REQ,             CmacUlTfcSubsetConfigReq cmacUlTfcSubsetConfigReq)
SIG_DEF (SIG_CMAC_CS_RAB_CONFIG_REQ,                    CmacCsRabConfigReq cmacCsRabConfigReq)
SIG_DEF (SIG_CMAC_HANDOVER_REQ,                         CmacHandoverReq cmacHandoverReq)
SIG_DEF (SIG_CMAC_DL_RB_MAPPING_CONFIG_REQ,             CmacDlRbMappingConfigReq cmacDlRbMappingConfigReq)
SIG_DEF (SIG_CMAC_UL_RB_MAPPING_CONFIG_REQ,             CmacUlRbMappingConfigReq cmacUlRbMappingConfigReq)
SIG_DEF (SIG_CMAC_UL_TRCH_CONFIG_REQ,                   CmacUlTrChConfigReq cmacUlTrChConfigReq)
SIG_DEF (SIG_CMAC_UL_MIN_TFC_SET_CONFIG_REQ,            CmacUlMinTfcSetConfigReq cmacUlMinTfcSetConfigReq)
SIG_DEF (SIG_CMAC_TRAFFIC_MEASUREMENT_REQ,              CmacTrafficMeasurementReq cmacTrafficMeasurementReq)
SIG_DEF (SIG_CMAC_TRAFFIC_MEASUREMENT_IND,              CmacTrafficMeasurementInd cmacTrafficMeasurementInd)
SIG_DEF (SIG_CMAC_TRAFFIC_MEASUREMENT_STOP_REQ,            CmacTrafficMeasurementStopReq cmacTrafficMeasurementStopReq)
SIG_DEF (SIG_CMAC_GET_ADDITIONAL_TRAFFIC_MEASUREMENT_REQ,
         CmacGetAdditionalTrafficMeasurementReq cmacGetAdditionalTrafficMeasurementReq)
SIG_DEF (SIG_CMAC_GET_ADDITIONAL_TRAFFIC_MEASUREMENT_CNF,
         CmacGetAdditionalTrafficMeasurementCnf cmacGetAdditionalTrafficMeasurementCnf)
SIG_DEF (SIG_CMAC_ACTIVATION_TIME_RESET_REQ,            CmacActivationTimeResetReq cmacActivationTimeResetReq)
#if defined(UPGRADE_3G_FDD)
SIG_DEF (SIG_CMAC_COMPRESSED_MODE_CONFIG_REQ,           CmacCompressedModeConfigReq cmacCompressedModeConfigReq)
#endif
SIG_DEF (SIG_CMAC_HFN_CONFIG_REQ,                       CmacHfnConfigReq cmacHfnConfigReq)
SIG_DEF (SIG_CMAC_HFN_CONFIG_CNF,                       CmacHfnConfigCnf cmacHfnConfigCnf)
SIG_DEF (SIG_CMAC_HFN_MEASUREMENT_IND,                  CmacHfnMeasurementInd cmacHfnMeasurementInd)
SIG_DEF (SIG_CMAC_MODE_SELECT_REQ,                      CmacModeSelectReq cmacModeSelectReq)
SIG_DEF (SIG_CMAC_DEACTIVATE_REQ,                       EmptySignal cmacDeactivateReq)
/* DEVELOPMENT_VERSION */
SIG_DEF (SIG_CMAC_TEST_MODE_REQ,                        CmacTestModeReq cmacTestModeReq)
/* DEVELOPMENT_VERSION */

/* Internal UMAC control signals, not externally visible */
SIG_DEF (SIG_CMAC_INTERNAL_RESOLVE_RB_TRCH_CONFIG,      CmacInternalResolveRbTrChConfig cmacInternalResolveRbTrChConfig)
SIG_DEF (SIG_CMAC_INTERNAL_TIMER_EXPIRY,                CmacInternalTimerExpiry cmacInternalTimerExpiry)
/* DEVELOPMENT_VERSION */
SIG_DEF (SIG_CMAC_INTERNAL_RECONFIG,                    CmacInternalReconfig cmacInternalReconfig)
/* DEVELOPMENT_VERSION */
SIG_DEF (SIG_CMAC_INTERNAL_UL_TM_BEARERS,               CmacInternalUlTmBearers cmacInternalUlTmBearers)
# if defined(UPGRADE_3G_HSDPA)
SIG_DEF (SIG_CMAC_HS_QUEUE_CONFIG_REQ,                  CmacHsQueueConfigReq cmacHsQueueConfigReq)
SIG_DEF (SIG_CMAC_HS_QUEUE_RELEASE_REQ,                 CmacHsQueueReleaseReq cmacHsQueueReleaseReq)
# else /* UPGRADE_3G_HSDPA */
SIG_DEF (SIG_CMAC_HS_QUEUE_CONFIG_REQ,                  EmptySignal cmacHsQueueConfigReq)
SIG_DEF (SIG_CMAC_HS_QUEUE_RELEASE_REQ,                 EmptySignal cmacHsQueueReleaseReq)
# endif /* UPGRADE_3G_HSDPA */
#if (defined (ON_PC) || defined (ON_TARGET_UNIT_TEST))/* DEVELOPMENT_VERSION */

/* Internal test signals used for unit testing */
SIG_DEF (SIG_CMAC_INTERNAL_TEST_CM,                     CmacInternalTestCm cmacInternalTestCm)
#else /* (ON_PC ||  ON_TARGET_UNIT_TEST) */
SIG_DEF (SIG_CMAC_INTERNAL_TEST_CM,                     EmptySignal cmacInternalTestCm)
#endif /* (ON_PC ||  ON_TARGET_UNIT_TEST) */
/* DEVELOPMENT_VERSION */
SIG_DEF (SIG_CMAC_DEBUG_TFC,                            CmacDebugTfc cmacDebugTfc)
/* DEVELOPMENT_VERSION */
SIG_DEF (SIG_CMAC_HFN_ABORT_REQ,                        CmacHfnAbortReq cmacHfnAbortReq)
#if defined (UPGRADE_3G_HSUPA)
SIG_DEF (SIG_CMAC_EDCH_MACD_FLOW_LIST_CONFIG_REQ,       CmacEdchMacDflowListConfigReq cmacEdchMacDflowListConfigReq)
SIG_DEF (SIG_CMAC_EDCH_CONFIG_REQ,                      CmacEdchConfigReq cmacEdchConfigReq)
SIG_DEF (SIG_CMAC_EDCH_RELEASE_REQ,                     CmacEdchReleaseReq cmacEdchReleaseReq)
SIG_DEF (SIG_CMAC_UL_DEDICATED_RELEASE_REQ,             CmacUlDedicatedReleaseReq cmacUlDedicatedReleaseReq)
#else  /* UPGRADE_3G_HSUPA */
SIG_DEF (SIG_CMAC_EDCH_MACD_FLOW_LIST_CONFIG_REQ,       EmptySignal cmacEdchMacDflowListConfigReq)
SIG_DEF (SIG_CMAC_EDCH_CONFIG_REQ,                      EmptySignal cmacEdchTfcsConfigReq)
SIG_DEF (SIG_CMAC_EDCH_RELEASE_REQ,                     EmptySignal cmacEdchReleaseReq)
SIG_DEF (SIG_CMAC_UL_DEDICATED_RELEASE_REQ,             EmptySignal cmacUlDedicatedReleaseReq)
#endif /* UPGRADE_3G_HSUPA */
#if defined (UPGRADE_3G_MBMS) && defined (UPGRADE_3G_TDD128)
SIG_DEF (SIG_CMAC_DL_MBMS_RB_MAPPING_CONFIG_REQ,        CmacDlMbmsRbMappingConfigReq cmacDlMbmsRbMappingConfigReq)
SIG_DEF (SIG_CMAC_DL_MBMS_RB_RELEASE_REQ,               EmptySignal cmacDlMbmsRbReleaseReq)
#else  /* UPGRADE_3G_MBMS && UPGRADE_3G_TDD128 */
SIG_DEF (SIG_CMAC_DL_MBMS_RB_MAPPING_CONFIG_REQ,        EmptySignal cmacDlMbmsRbMappingConfigReq)
SIG_DEF (SIG_CMAC_DL_MBMS_RB_RELEASE_REQ,               EmptySignal cmacDlMbmsRbReleaseReq)
#endif /* UPGRADE_3G_MBMS && UPGRADE_3G_TDD128 */
SIG_DEF (SIG_CMAC_INTERNAL_EDCH_SURVIVAL_RESET,         EmptySignal cmacInternalEdchSurvivalReset)
#endif /* EXCLUDE_CMAC */

#if !defined (EXCLUDE_UMAC)
SIG_DEF (SIG_UMAC_DUMMY = UMAC_SIGNAL_BASE,             EmptySignal umac_dummy)
SIG_DEF (SIG_UMAC_TRAFFIC_REQ,                          UmacTrafficReq umacTrafficReq)
SIG_DEF (SIG_UMAC_NO_TRAFFIC_REQ,                       UmacNoTrafficReq umacNoTrafficReq)
SIG_DEF (SIG_UMAC_UPDATE_TRAFFIC_IND,                   UmacUpdateTrafficInd umacUpdateTrafficInd)

SIG_DEF (SIG_UMAC_TRAFFIC_IND,                          UmacTrafficInd umacTrafficInd)
SIG_DEF (SIG_UMAC_NO_TRAFFIC_IND,                       EmptySignal umacNoTrafficInd)
SIG_DEF_CUSTOM_REF (SIG_UMAC_DATA_REQ,                  UmacDataReq,                    umacDataReq,
                                                        UmacDataReqRefDataPayload,      RefLogUmacDataReq)
SIG_DEF_CUSTOM_REF (SIG_UMAC_DATA_IND,                  UmacDataInd,                    umacDataInd,
                                                        UmacDataIndRefDataPayload,      RefLogUmacDataInd)
SIG_DEF (SIG_UMAC_TX_STATUS_IND,                        UmacTxStatusInd umacTxStatusInd)
SIG_DEF (SIG_UMAC_UL_CONFIG_CHANGE_IND,                 UmacUlConfigChangeInd umacUlConfigChangeInd)
SIG_DEF (SIG_UMAC_DL_CONFIG_CHANGE_IND,                 UmacDlConfigChangeInd umacDlConfigChangeInd)
/* DEVELOPMENT_VERSION */

/* Used for checking activation times */
SIG_DEF (SIG_UMAC_DEBUG_PHY_FRAME_IND,                  PhyFrameInd umacDebugPhyFrameInd)
SIG_DEF (SIG_UMAC_DEBUG_PHY_DATA_IND,                   PhyDataInd umacDebugPhyDataInd)

/* Used for checking signal contents */
SIG_DEF (SIG_UMAC_DEBUG_PHY_DATA_INFO_IND,              PhyDebugDataInfoInd umacDebugPhyDataInfoInd)
SIG_DEF (SIG_UMAC_DEBUG_PHY_DATA_INFO_REQ,              PhyDebugDataInfoReq umacDebugPhyDataInfoReq)
/* DEVELOPMENT_VERSION */
SIG_DEF (SIG_UMAC_PDU_LIST_INFO_IND,                    UmacDlPduListInfoInd umacDlPduListInfoInd)
SIG_DEF (SIG_UMAC_PDU_LIST_INFO_RSP,                    UmacDlPduListInfoRsp umacDlPduListInfoRsp)
# if defined(UPGRADE_3G_HSDPA)
SIG_DEF_CUSTOM_REF (SIG_UMAC_HS_DATA_IND,               UmacHsDataInd,                 umacHsDataInd,
                                                        UmacHsDataIndRefDataPayload,     RefLogUmacHsDataInd)
SIG_DEF (SIG_UMAC_HS_DATA_RSP,                          EmptySignal umacHsDataRsp)
SIG_DEF (SIG_UMAC_HS_PDU_LIST_INFO_IND,                 UmacHsPduListInfoInd umacHsPduListInfoInd)
SIG_DEF (SIG_UMAC_HS_PDU_LIST_INFO_RSP,                 UmacHsPduListInfoInd umacHsPduListInfoRsp)
SIG_DEF (SIG_UMAC_HS_REQUEST_STATUS_IND,                UmacHsRequestStatusInd umacHsRequestStatusInd)
SIG_DEF (SIG_UMAC_HS_DEBUG_PHY_HS_DATA_IND,             UmacHsDebugPhyHsDataInd umacHsDebugPhyHsDataInd)
# else /* UPGRADE_3G_HSDPA */
SIG_DEF (SIG_UMAC_HS_DATA_IND,                          EmptySignal umacHsDataInd)
SIG_DEF (SIG_UMAC_HS_DATA_RSP,                          EmptySignal umacHsDataRsp)
SIG_DEF (SIG_UMAC_HS_PDU_LIST_INFO_IND,                 EmptySignal umacHsPduListInfoInd)
SIG_DEF (SIG_UMAC_HS_PDU_LIST_INFO_RSP,                 EmptySignal umacHsPduListInfoRsp)
SIG_DEF (SIG_UMAC_HS_REQUEST_STATUS_IND,                EmptySignal umacHsRequestStatusInd)
SIG_DEF (SIG_UMAC_HS_DEBUG_PHY_HS_DATA_IND,             EmptySignal umacHsDebugPhyHsDataInd)
# endif /* UPGRADE_3G_HSDPA */
/* DEVELOPMENT_VERSION */
SIG_DEF (SIG_UMAC_DEBUG_TFC_SELECTION,                  UmacDebugTfcSelection umacDebugTfcSelection)
SIG_DEF (SIG_UMAC_DEBUG_PARAM,                          UmacDebugParam umacDebugParam)
# if defined (UPGRADE_3G_HSUPA)
SIG_DEF (SIG_UMAC_DEBUG_EDCH_MPX,                       UmacDebugEdchMpx umacDebugEdchMpx)
#else /* UPGRADE_3G_HSUPA */
SIG_DEF (SIG_UMAC_DEBUG_EDCH_MPX,                       EmptySignal umacDebugEdchMpx)
# endif /* UPGRADE_3G_HSUPA */
/* DEVELOPMENT_VERSION */
# if defined (UPGRADE_3G_HSUPA)
SIG_DEF (SIG_UMAC_EDCH_TRAFFIC_IND,                     UmacEdchTrafficInd umacEdchTrafficInd)
SIG_DEF (SIG_UMAC_EDCH_NO_TRAFFIC_IND,                  EmptySignal umacEdchNoTrafficInd)
SIG_DEF_CUSTOM_REF (SIG_UMAC_EDCH_DATA_REQ,             UmacEdchDataReq,                umacEdchDataReq,
                                                        UmacEdchDataReqRefDataPayload,  RefLogUmacEdchDataReq)
# else  /* UPGRADE_3G_HSUPA */
SIG_DEF (SIG_UMAC_EDCH_TRAFFIC_IND,                     EmptySignal umacEdchTrafficInd)
SIG_DEF (SIG_UMAC_EDCH_NO_TRAFFIC_IND,                  EmptySignal umacEdchNoTrafficInd)
SIG_DEF (SIG_UMAC_EDCH_DATA_REQ,                        EmptySignal umacEdchDataReq)
# endif /* UPGRADE_3G_HSUPA */
#endif /* EXCLUDE_UMAC */

#if !defined (EXCLUDE_RABMRRC)
SIG_DEF (SIG_RABMRRC_DUMMY = RABMRRC_SIGNAL_BASE,       EmptySignal rabmrrc_dummy)
SIG_DEF (SIG_RABMRRC_ESTABLISH_IND,                     RabmRrcEstablishInd rabmRrcEstablishInd)
SIG_DEF (SIG_RABMRRC_ESTABLISH_RES,                     RabmRrcEstablishRes rabmRrcEstablishRes)
SIG_DEF (SIG_RABMRRC_ESTABLISH_REJ,                     EmptySignal rabmRrcEstablishRej)
SIG_DEF (SIG_RABMRRC_RELEASE_IND,                       RabmRrcReleaseInd rabmRrcReleaseInd)
SIG_DEF (SIG_RABMRRC_RELEASE_RES,                       RabmRrcReleaseRes rabmRrcReleaseRes)
SIG_DEF (SIG_RABMRRC_STATUS_IND,                        RabmRrcStatusInd rabmRrcStatusInd)
#if defined (UPGRADE_3G_FDD)
SIG_DEF (SIG_RABMRRC_FAST_DORMANCY_IND,                 RabmRrcFastDormancyInd rabmRrcFastDormancyInd)
SIG_DEF (SIG_RABMRRC_FAST_DORMANCY_REQ,                 RabmRrcFastDormancyReq rabmRrcFastDormancyReq)
SIG_DEF (SIG_RABMRRC_FAST_DORMANCY_CNF,                 RabmRrcFastDormancyCnf rabmRrcFastDormancyCnf)
#endif
# endif /* EXCLUDE_RABMRRC */

#if !defined (EXCLUDE_GMMRABM)
SIG_DEF (SIG_GMMRABM_DUMMY = GMMRABM_SIGNAL_BASE,       EmptySignal gmmrabm_dummy)
SIG_DEF (SIG_GMMRABM_REESTABLISH_REQ,                   GmmRabmReestablishReq gmmRabmReestablishReq)
SIG_DEF (SIG_GMMRABM_REESTABLISH_RSP,                   EmptySignal gmmRabmReestablishRsp)
SIG_DEF (SIG_GMMRABM_REESTABLISH_REJ,                   GmmRabmReestablishRej gmmRabmReestablishRej)
SIG_DEF (SIG_GMMRABM_REESTABLISH_IND,                   EmptySignal gmmRabmReestablishInd)
SIG_DEF (SIG_GMMRABM_RELEASE_REQ,                       EmptySignal gmmRabmReleaseReq)
SIG_DEF (SIG_GMMRABM_STATUS_IND,                        GmmRabmStatusInd gmmRabmStatusInd)
# endif /* EXCLUDE_GMMRABM */

#if defined (UPGRADE_NASMDL2)
#if !defined (EXCLUDE_MMDBM)
SIG_DEF (SIG_MMDBM_DUMMY = MMDBM_SIGNAL_BASE,           EmptySignal         mmdbm_dummy)
SIG_DEF (SIG_MMDBM_REESTABLISH_IND,                     MmDbmReestablishInd mmDbmReestablishInd)
SIG_DEF (SIG_MMDBM_REESTABLISH_CNF,                     MmDbmReestablishCnf mmDbmReestablishCnf)
SIG_DEF (SIG_MMDBM_REESTABLISH_REJ,                     MmDbmReestablishRej mmDbmReestablishRej)
SIG_DEF (SIG_MMDBM_REESTABLISH_REQ,                     MmDbmReestablishReq mmDbmReestablishReq)
SIG_DEF (SIG_MMDBM_REESTABLISH_RSP,                     MmDbmReestablishRsp mmDbmReestablishRsp)
SIG_DEF (SIG_MMDBM_RELEASE_REQ,                         MmDbmReleaseReq     mmDbmReleaseReq)
SIG_DEF (SIG_MMDBM_SUSPEND_IND,                         MmDbmSuspendInd     mmDbmSuspendInd)
SIG_DEF (SIG_MMDBM_RESUME_IND,                          MmDbmResumeInd      mmDbmResumeInd)
SIG_DEF (SIG_MMDBM_ACTIVE_SERVICE_COMPLETE_IND,         MmDbmActiveServiceCompleteInd mmDbmActiveServiceCompleteInd)
SIG_DEF (SIG_MMDBM_BEARER_STATUS_IND,                   MmDbmBearerStatusInd mmDbmBearerStatusInd)
SIG_DEF (SIG_MMDBM_CIOT_CONFIG_IND,                     MmDbmCiotConfigInd  mmDbmCiotConfigInd)
SIG_DEF (SIG_MMDBM_CONN_FAILED_IND,                     MmDbmConnFailedInd  mmDbmConnFailedInd)
SIG_DEF (SIG_MMDBM_CLOSE_UE_TEST_LOOP_IND,              MmDbmCloseUeTestLoopInd         mmDbmCloseUeTestLoopInd)
SIG_DEF (SIG_MMDBM_OPEN_UE_TEST_LOOP_IND,               MmDbmOpenUeTestLoopInd          mmDbmOpenUeTestLoopInd)
SIG_DEF (SIG_MMDBM_ACTIVATE_TEST_MODE_IND,              MmDbmActivateTestModeInd        mmDbmActivateTestModeInd)
SIG_DEF (SIG_MMDBM_DEACTIVATE_TEST_MODE_IND,            MmDbmDeactivateTestModeInd      mmDbmDeactivateTestModeInd)
SIG_DEF (SIG_MMDBM_EXCEPTION_REPORTS_REQ,               MmDbmExceptionReportsReq        mmDbmExceptionReportsReq)
SIG_DEF (SIG_MMDBM_CP_REESTABLISH_IND,                  MmDbmCpReestablishInd           mmDbmCpReestablishInd)

# endif /* EXCLUDE_MMDBM */
#endif /* UPGRADE_NASMDL2 */

#if !defined (EXCLUDE_PMMSMS)
SIG_DEF (SIG_PMMSMS_DUMMY = PMMSMS_SIGNAL_BASE,         EmptySignal pmmsms_dummy)
SIG_DEF (SIG_PMMSMS_EST_REQ,                            PmmSmsEstReq pmmSmsEstReq)
SIG_DEF (SIG_PMMSMS_EST_CNF,                            PmmSmsEstCnf pmmSmsEstCnf)
SIG_DEF (SIG_PMMSMS_ERROR_IND,                          PmmSmsErrorInd pmmSmsErrorInd)
SIG_DEF (SIG_PMMSMS_REL_REQ,                            PmmSmsRelReq pmmSmsRelReq)
SIG_DEF (SIG_PMMSMS_UNITDATA_REQ,                       PmmSmsUnitDataReq pmmSmsUnitDataReq)
SIG_DEF (SIG_PMMSMS_UNITDATA_IND,                       PmmSmsUnitDataInd pmmSmsUnitDataInd)
SIG_DEF (SIG_PMMSMS_ACTIVATE_TEST_MODE_IND,             PmmSmsActivateTestModeInd   pmmSmsActivateTestModeInd)
SIG_DEF (SIG_PMMSMS_CLOSE_UE_TEST_LOOP_IND,             PmmSmsCloseUeTestLoopInd    pmmSmsCloseUeTestLoopInd)
SIG_DEF (SIG_PMMSMS_OPEN_UE_TEST_LOOP_IND,              PmmSmsOpenUeTestLoopInd     pmmSmsOpenUeTestLoopInd)
SIG_DEF (SIG_PMMSMS_DEACTIVATE_TEST_MODE_IND,           PmmSmsDeactivateTestModeInd pmmSmsDeactivateTestModeInd)
SIG_DEF (SIG_PMMSMS_LOOP_DATA_TRIGGER_REQ,              PmmSmsLoopDataTriggerReq pmmSmsLoopDataTriggerReq)
SIG_DEF (SIG_PMMSMS_FAIL_IND,                           PmmSmsFailInd pmmSmsFailInd)
# endif /* EXCLUDE_PMMSMS */

#ifdef FEA_LPP_SUPPORT
SIG_DEF (SIG_MMLPP_DUMMY = MMLPP_SIGNAL_BASE,           EmptySignal mmlpp_dummy)
SIG_DEF (SIG_MMLPP_CELL_INFO_IND,                       MmLppCellInfoInd mmLppCellInfoInd)
SIG_DEF (SIG_MMLPP_DATA_REQ,                            MmLppDataReq mmLppDataReq)
SIG_DEF (SIG_MMLPP_DATA_CNF,                            MmLppDataCnf mmLppDataCnf)
SIG_DEF (SIG_MMLPP_DATA_IND,                            MmLppDataInd mmLppDataInd)
SIG_DEF (SIG_MMLPP_ACTIVE_START_REQ,                    MmLppActiveStartReq mmLppActiveStartReq)
SIG_DEF (SIG_MMLPP_ACTIVE_STOP_REQ,                     MmLppActiveStopReq mmLppActiveStopReq)
#endif /* FEA_LPP_SUPPORT */

#if defined (UPGRADE_3G_MBMS)
# if defined (UPGRADE_3G_TDD128)
#  if !defined (EXCLUDE_RRCMBMS)
SIG_DEF (SIG_RRCMBMS_DUMMY = RRCMBMS_SIGNAL_BASE,       EmptySignal rrcmbms_dummy)
SIG_DEF (SIG_RRCMBMS_ACTIVATE_REQ,                      RrcMbmsActivateReq rrcMbmsActivateReq)
SIG_DEF (SIG_RRCMBMS_ACTIVATE_CNF,                      RrcMbmsActivateCnf rrcMbmsActivateCnf)
SIG_DEF (SIG_RRCMBMS_ACCEPT_REQ,                        RrcMbmsAcceptReq rrcMbmsAcceptReq)
SIG_DEF (SIG_RRCMBMS_DEACTIVATE_REQ,                    RrcMbmsDeactivateReq rrcMbmsDeactivateReq)
SIG_DEF (SIG_RRCMBMS_DEACTIVATE_CNF,                    RrcMbmsDeactivateCnf rrcMbmsDeactivateCnf)
SIG_DEF (SIG_RRCMBMS_STATE_IND,                         RrcMbmsStateInd rrcMbmsStateInd)
SIG_DEF (SIG_RRCMBMS_STATE_REQ,                         RrcMbmsStateReq rrcMbmsStateReq)
SIG_DEF (SIG_RRCMBMS_STATE_CNF,                         RrcMbmsStateCnf rrcMbmsStateCnf)
#  endif /* EXCLUDE_RRCMBMS */
# endif /* UPGRADE_3G_TDD128 */
#endif /* UPGRADE_3G_MBMS */

#if !defined (EXCLUDE_TIRRC)
SIG_DEF (SIG_TI_RRC_DUMMY = TIRRC_SIGNAL_BASE,          EmptySignal tirrc_dummy)
SIG_DEF (SIG_TI_RRC_ACTIVATE_RB_TEST_LOOP_MODE_REQ,     EmptySignal tiRrcActivateRbTestLoopModeReq)
SIG_DEF (SIG_TI_RRC_CLOSE_TEST_LOOP_MODE_REQ,           TiRrcCloseTestLoopModeReq tiRrcCloseTestLoopModeReq)
SIG_DEF (SIG_TI_RRC_CLOSE_TEST_LOOP_MODE_CNF,           TiRrcCloseTestLoopModeCnf tiRrcCloseTestLoopModeCnf)
SIG_DEF (SIG_TI_RRC_OPEN_TEST_LOOP_MODE_REQ,            EmptySignal tiRrcOpenTestLoopModeReq)
SIG_DEF (SIG_TI_RRC_OPEN_TEST_LOOP_MODE_CNF,            TiRrcOpenTestLoopModeCnf tiRrcOpenTestLoopModeCnf)
SIG_DEF (SIG_TI_RRC_DEACTIVATE_RB_TEST_LOOP_MODE_REQ,   EmptySignal tiRrcDeactivateRbTestLoopModeReq)
#if defined (UPGRADE_3G_MBMS)
#if defined (UPGRADE_3G_TDD128)
SIG_DEF (SIG_TI_RRC_TEST_LOOP_MODE_3_RLC_SDU_COUNTER_REQ, EmptySignal tiRrcTestLoopMode3RlcSduCounterReq)
SIG_DEF (SIG_TI_RRC_TEST_LOOP_MODE_3_RLC_SDU_COUNTER_CNF,
         TiRrcTestLoopMode3RlcSduCounterCnf tiRrcTestLoopMode3RlcSduCounterCnf)
#endif /* UPGRADE_3G_TDD128 */
#endif /* UPGRADE_3G_MBMS */
#endif /* EXCLUDE_TIRRC */

#if defined (NODEBSCRIPT_GKI_API)
# if !defined (EXCLUDE_NODEBSCRIPT)

/* Scripting API used for NodeB2 and NetSim*/
SIG_DEF (SIG_NODEBSCRIPT_DUMMY = NODEBSCRIPT_SIGNAL_BASE,           EmptySignal nodebscript_dummy)
SIG_DEF (SIG_NODEBSCRIPT_RX_AIR_MSG_IND,                            NodebScriptRxAirMsgInd nodebScriptRxAirMsgInd)
SIG_DEF (SIG_NODEBSCRIPT_TX_AIR_MSG_IND,                            NodebScriptTxAirMsgInd nodebScriptTxAirMsgInd)
SIG_DEF (SIG_NODEBSCRIPT_CREATE_CELL_REQ,                           NodebScriptCreateCellReq nodebScriptCreateCellReq)
SIG_DEF (SIG_NODEBSCRIPT_CREATE_CELL_CNF,                           NodebScriptCreateCellCnf nodebScriptCreateCellCnf)
SIG_DEF (SIG_NODEBSCRIPT_CREATE_GSM_CELL_REQ,
         NodebScriptCreateGsmCellReq nodebScriptCreateGsmCellReq)
SIG_DEF (SIG_NODEBSCRIPT_CREATE_GSM_CELL_CNF,
         NodebScriptCreateGsmCellCnf nodebScriptCreateGsmCellCnf)
SIG_DEF (SIG_NODEBSCRIPT_SET_GSM_CELL_LEVELS_REQ,
         NodebScriptSetGsmCellLevelsReq nodebScriptSetGsmCellLevelsReq)
SIG_DEF (SIG_NODEBSCRIPT_UPLOAD_SIB_REQ,                            NodebScriptUploadSibReq nodebScriptUploadSibReq)
SIG_DEF (SIG_NODEBSCRIPT_ACTIVATE_NEW_SIBS_REQ,
         NodebScriptActivateNewSibsReq nodebScriptActivateNewSibsReq)
SIG_DEF (SIG_NODEBSCRIPT_SET_CELL_LEVELS_REQ,
         NodebScriptSetCellLevelsReq nodebScriptSetCellLevelsReq)
SIG_DEF (SIG_NODEBSCRIPT_RESET_REQ,                                 EmptySignal nodebScriptResetReq)
SIG_DEF (SIG_NODEBSCRIPT_RESET_CNF,                                 EmptySignal nodebScriptResetCnf)
#  if defined (UPGRADE_3G_FDD)
SIG_DEF (SIG_NODEBSCRIPT_FDD_TM_DATA_IND,                           NodebScriptFddTmDataInd nodebScriptFddTmDataInd)
SIG_DEF (SIG_NODEBSCRIPT_FDD_UM_DATA_IND,                           NodebScriptFddUmDataInd nodebScriptFddUmDataInd)
SIG_DEF (SIG_NODEBSCRIPT_FDD_AM_DATA_IND,                           NodebScriptFddAmDataInd nodebScriptFddAmDataInd)
#  endif /* UPGRADE_3G_FDD */
#  if defined (UPGRADE_3G_TDD128)
SIG_DEF (SIG_NODEBSCRIPT_TDD_TM_DATA_IND,                           NodebScriptTddTmDataInd nodebScriptTddTmDataInd)
SIG_DEF (SIG_NODEBSCRIPT_TDD_UM_DATA_IND,                           NodebScriptTddUmDataInd nodebScriptTddUmDataInd)
SIG_DEF (SIG_NODEBSCRIPT_TDD_AM_DATA_IND,                           NodebScriptTddAmDataInd nodebScriptTddAmDataInd)
#  endif /* UPGRADE_3G_TDD128 */
SIG_DEF (SIG_NODEBSCRIPT_TRAFFIC_VOLUME_MEASUREMENT_REQ,
         NodebScriptTrafficVolumeMeasurementReq nodebScriptTrafficVolumeMeasurementReq)
SIG_DEF (SIG_NODEBSCRIPT_CONFIG_CPHY_STUB_REQ,
         NodebScriptConfigCphyStubReq nodebScriptConfigCphyStubReq)
SIG_DEF (SIG_NODEBSCRIPT_FORCE_OUT_OF_SYNC_REQ,
         NodebScriptForceOutOfSyncReq nodebScriptForceOutOfSyncReq)
SIG_DEF (SIG_NODEBSCRIPT_FORCE_SIB_SCHEDULE_ERROR_REQ,
         NodebScriptForceSibScheduleErrorReq nodebScriptForceSibScheduleErrorReq)
SIG_DEF (SIG_NODEBSCRIPT_ENABLE_L2_REQ,                             EmptySignal nodebScriptEnableL2Req)
SIG_DEF (SIG_NODEBSCRIPT_RLC_CONFIG_REQ,                            NodebScriptRlcConfigReq nodebScriptRlcConfigReq)
SIG_DEF (SIG_NODEBSCRIPT_RB_RELEASE_REQ,                            NodebScriptRbReleaseReq nodebScriptRbReleaseReq)
SIG_DEF (SIG_NODEBSCRIPT_TRCH_CONFIG_REQ,                           NodebScriptTrchConfigReq nodebScriptTrchConfigReq)
SIG_DEF (SIG_NODEBSCRIPT_TRCH_RELEASE_REQ,                          NodebScriptTrchReleaseReq nodebScriptTrchReleaseReq)
SIG_DEF (SIG_NODEBSCRIPT_L2_INTERFACE_CONFIG_REQ,
         NodebScriptL2InterfaceConfigReq nodebScriptL2InterfaceConfigReq)
SIG_DEF (SIG_NODEBSCRIPT_RLC_PDU_REQ,                               NodebScriptRlcPduReq nodebScriptRlcPduReq)
SIG_DEF (SIG_NODEBSCRIPT_RLC_PDU_IND,                               NodebScriptRlcPduInd nodebScriptRlcPduInd)
SIG_DEF (SIG_NODEBSCRIPT_RLC_SDU_REQ,                               NodebScriptRlcSduReq nodebScriptRlcSduReq)
SIG_DEF (SIG_NODEBSCRIPT_RLC_SDU_IND,                               NodebScriptRlcSduInd nodebScriptRlcSduInd)
SIG_DEF (SIG_NODEBSCRIPT_TRANSPORT_BLOCK_REQ,
         NodebScriptTransportBlockReq nodebScriptTransportBlockReq)
SIG_DEF (SIG_NODEBSCRIPT_TRANSPORT_BLOCK_IND,
         NodebScriptTransportBlockInd nodebScriptTransportBlockInd)
SIG_DEF (SIG_NODEBSCRIPT_TYPICAL_SERVICE_SETUP_REQ,
         NodebScriptTypicalServiceSetupReq nodebScriptTypicalServiceSetupReq)
SIG_DEF (SIG_NODEBSCRIPT_START_DATA_GEN_REQ,
         NodebScriptStartDataGenReq nodebScriptStartDataGenReq)
SIG_DEF (SIG_NODEBSCRIPT_STOP_DATA_GEN_REQ,                         NodebScriptStopDataGenReq nodebScriptStopDataGenReq)
SIG_DEF (SIG_NODEBSCRIPT_LOOPBACK_ACTIVATE_REQ,
         NodebScriptLoopbackActivateReq nodebScriptLoopbackActivateReq)
SIG_DEF (SIG_NODEBSCRIPT_LOOPBACK_DEACTIVATE_REQ,
         NodebScriptLoopbackDeactivateReq nodebScriptLoopbackDeactivateReq)
SIG_DEF (SIG_NODEBSCRIPT_STATUS_PDU_IND,                            NodebScriptStatusPdu nodebScriptStatusPduInd)
SIG_DEF (SIG_NODEBSCRIPT_STATUS_PDU_REQ,                            NodebScriptStatusPdu nodebScriptStatusPduReq)
SIG_DEF (SIG_NODEBSCRIPT_PHY_DATA_REQ,                              NodebScriptPhyDataReq nodebScriptPhyDataReq)
SIG_DEF (SIG_NODEBSCRIPT_PHY_ACCESS_REQ,                            NodebScriptPhyAccessReq nodebScriptPhyAccessReq)
SIG_DEF (SIG_NODEBSCRIPT_PHY_DATA_IND,                              NodebScriptPhyDataInd nodebScriptPhyDataInd)
SIG_DEF (SIG_NODEBSCRIPT_FRAME_TIMER_TICK,                          NodebScriptFrameTimerTick nodebScriptFrameTimerTick)
#  if defined (UPGRADE_3G_HSDPA)
SIG_DEF (SIG_NODEBSCRIPT_HSDSCH_TRCH_CONFIG_REQ,
         NodebScriptHsDschTrchConfigReq nodebScriptHsDschTrchConfigReq)
SIG_DEF (SIG_NODEBSCRIPT_HSQUEUE_RELEASE_REQ,
         NodebScriptHsQueueReleaseReq nodebScriptHsQueueReleaseReq)
SIG_DEF (SIG_NODEBSCRIPT_HSQUEUE_CONFIG_REQ,
         NodebScriptHsQueueConfigReq nodebScriptHsQueueConfigReq)
SIG_DEF (SIG_NODEBSCRIPT_HSDSCH_TRCH_RELEASE_REQ,
         NodebScriptHsDschTrchReleaseReq nodebScriptHsDschTrchReleaseReq)
SIG_DEF (SIG_NODEBSCRIPT_PHY_HSDATA_IND,                            NodebScriptPhyHsDataInd nodebScriptPhyHsDataInd)
#  endif
#if defined (UPGRADE_3G_TDD128)
SIG_DEF (SIG_NODEBSCRIPT_SET_UE_TRANSMITTED_POWER_REQ,
         NodebScriptSetUeTransmittedPowerReq nodebScriptSetUeTransmittedPowerReq)
SIG_DEF (SIG_NODEBSCRIPT_SET_TIMING_ADVANCE_INFO_REQ,
         NodebScriptSetTimingAdvanceInfoReq nodebScriptSetTimingAdvanceInfoReq)
#endif
#if defined (UPGRADE_3G_HSUPA)
SIG_DEF (SIG_NODEBSCRIPT_EDCH_CONFIG_REQ,                           NodebScriptEDchConfigReq nodebScriptEDchConfigReq)
SIG_DEF (SIG_NODEBSCRIPT_EMACD_FLOW_RELEASE_REQ,
         NodebScriptEMacdFlowReleaseReq nodebScriptEMacdFlowReleaseReq)
SIG_DEF (SIG_NODEBSCRIPT_EDCH_RELEASE_REQ,                          NodebScriptEDchReleaseReq nodebScriptEDchReleaseReq)
SIG_DEF (SIG_NODEBSCRIPT_PHY_EDCH_DATA_REQ,                         NodebScriptPhyEDchDataReq nodebScriptPhyEDchDataReq)
SIG_DEF (SIG_NODEBSCRIPT_EDCH_GRANT_CONFIG_REQ,
         NodebScriptEDchGrantConfigReq nodebScriptEDchGrantConfigReq)
SIG_DEF (SIG_NODEBSCRIPT_EDCH_ACK_NACK_RESULT_REQ,
         NodebScriptEDchAckNackResultReq nodebScriptEDchAckNackResultReq)
#endif

#if defined (UPGRADE_3G_MBMS)
#if defined (UPGRADE_3G_TDD128)
SIG_DEF (SIG_NODEBSCRIPT_UPLOAD_MBMS_REQ,                           NodebScriptUploadMbmsReq nodebScriptUploadMbmsReq)
SIG_DEF (SIG_NODEBSCRIPT_MCCH_SEND_OUTOFSEQUENCE_REQ,
         NodebScriptMcchSendOutOfSequenceReq nodebScriptMcchSendOutOfSequenceReq)
#endif
#endif

SIG_DEF (SIG_NODEBSCRIPT_CFN_QUERY_REQ,                             NodebScriptCfnQueryReq nodebScriptCfnQueryReq)
SIG_DEF (SIG_NODEBSCRIPT_CFN_QUERY_CNF,                             NodebScriptCfnQueryCnf nodebScriptCfnQueryCnf)
SIG_DEF (SIG_NODEBSCRIPT_DISABLE_ASN_EXTRACTION_REQ,
         NodebScriptDisableAsnExtractionReq nodebScriptDisableAsnExtractionReq)
SIG_DEF (SIG_NODEBSCRIPT_ENABLE_ASN_EXTRACTION_REQ,                 EmptySignal nodebScriptEnableAsnExtractionReq)
SIG_DEF (SIG_NODEBSCRIPT_SET_CRCERROR_FOR_TMDATAIND_REQ,
         NodebScriptSetCrcErrorForTmDataIndReq nodebScriptSetCrcErrorForTmDataIndReq)
SIG_DEF (SIG_NODEBSCRIPT_CONFIG_GRR_STUB_REQ,
         NodebScriptConfigGrrStubReq nodebScriptConfigGrrStubReq)
SIG_DEF (SIG_NODEBSCRIPT_CONFIG_L1BG_STUB_REQ,
         NodebScriptConfigL1BgStubReq nodebScriptConfigL1BgStubReq)
SIG_DEF (SIG_NODEBSCRIPT_TIME_JUMP_REQ,                             NodebScriptTimeJumpReq nodebScriptTimeJumpReq)
SIG_DEF (SIG_NODEBSCRIPT_TIME_JUMP_CNF,                             EmptySignal nodebScriptTimeJumpCnf)
SIG_DEF (SIG_NODEBSCRIPT_NEXT_CNF_FAIL_IND,                         EmptySignal nodebScriptNextCnfFailInd)
SIG_DEF (SIG_NODEBSCRIPT_NEXT_CNF_FAIL_CNF,                         EmptySignal nodebScriptNextCnfFailCnf)
SIG_DEF (SIG_NODEBSCRIPT_PPPMUX_DATA_REQ,                           NodebScriptPppMuxDataReq nodebScriptPppMuxDataReq)
SIG_DEF (SIG_NODEBSCRIPT_PPPMUX_DATA_IND,                           NodebScriptPppMuxDataInd nodebScriptPppMuxDataInd)
SIG_DEF (SIG_NODEBSCRIPT_LLC_UNIT_DATA_IND,                         NodebScriptLlcUnitDataInd nodebScriptLlcUnitDataInd)
SIG_DEF (SIG_NODEBSCRIPT_STOP_BCH_REQ,                              NodebScriptStopBchReq nodebScriptStopBchReq)
SIG_DEF (SIG_NODEBSCRIPT_BCCH_OVER_FACH_REQ,                        NodebScriptBcchOverFachReq nodebScriptBcchOverFachReq)
SIG_DEF (SIG_NODEBSCRIPT_UPLOAD_GSM_SI_REQ,                         NodebScriptUploadGsmSiReq nodebScriptUploadGsmSiReq)
SIG_DEF (SIG_NODEBSCRIPT_GSM_FN_QUERY_REQ,                          NodebScriptGsmFNQueryReq nodebScriptGsmFNQueryReq)
SIG_DEF (SIG_NODEBSCRIPT_GSM_FN_QUERY_CNF,                          NodebScriptGsmFNQueryCnf nodebScriptGsmFNQueryCnf)
SIG_DEF (SIG_NODEBSCRIPT_STOP_SIB_REQ,                              NodebScriptStopSibReq nodebScriptStopSibReq)
# endif /* EXCLUDE_NODEBSCRIPT */
#endif /* NODEBSCRIPT_GKI_API */

#if defined (NODEB_GKI_API)
#  if !defined (EXCLUDE_NODEBSIG)
SIG_DEF (SIG_NODEB_DUMMY = NODEB_SIGNAL_BASE,           EmptySignal nodeb_dummy)
SIG_DEF (SIG_NODEB_INIT_TO_DEFAULT_STATE_REQ,           EmptySignal nodebInitToDefaultStateReq)
SIG_DEF (SIG_NODEB_REPORT_RRC_DCCH_MESSAGE_REQ,         NodebRrcReportDcchMessageReq nodebRrcReportDcchMessageReq)
SIG_DEF (SIG_NODEB_REPORTED_RRC_MESSAGE_IND,            NodebRrcMessageInd nodebRrcReportedMessageInd)
SIG_DEF (SIG_NODEB_UNREPORTED_RRC_MESSAGE_IND,          NodebRrcMessageInd nodebRrcUnreportedMessageInd)
SIG_DEF (SIG_NODEB_MENU_SPECIAL_CELL_DCH_REQ,           NodebMenuSpecialCellDchReq nodebMenuSpecialCellDchReq)
SIG_DEF (SIG_NODEB_PARAM_CELL_STRENGTH_REQ,             NodebParamCellStrengthReq nodebParamCellStrengthReq)
SIG_DEF (SIG_NODEB_PARAM_ACTIVE_SET_REQ,                NodebParamActiveSetReq nodebParamActiveSetReq)
SIG_DEF (SIG_NODEB_PARAM_GSM_CELL_STRENGTH_REQ,         NodebParamGsmCellStrengthReq nodebParamGsmCellStrengthReq)
SIG_DEF (SIG_NODEB_MENU_SPECIAL_CELL_FACH_REQ,          NodebMenuSpecialCellFachReq nodebMenuSpecialCellFachReq)
SIG_DEF (SIG_NODEB_PARAM_CELL_CONFIG_REQ,               NodebParamCellConfigReq nodebParamCellConfigReq)
SIG_DEF (SIG_NODEB_PARAM_CELL_SELECT_DATA_REQ,          NodebParamCellSelectDataReq nodebParamCellSelectDataReq)
SIG_DEF (SIG_NODEB_PARAM_CSR_QUALITY_HCS_REQ,           NodebParamCsrQualityHcsReq nodebParamCsrQualityHcsReq)
SIG_DEF (SIG_NODEB_PARAM_CELL_ACCESS_INFO_REQ,          NodebParamCellAccessInfoReq nodebParamCellAccessInfoReq)
SIG_DEF (SIG_NODEB_PARAM_HCS_DATA_REQ,                  NodebParamHcsDataReq nodebParamHcsDataReq)
SIG_DEF (SIG_NODEB_DEBUG_IND,                           NodebDebugInd nodebDebugInd)
SIG_DEF (SIG_NODEB_ENVIRONMENT_FILE_REQ,                NodebEnvironmentFileReq nodebEnvironmentFileReq)
SIG_DEF (SIG_NODEB_CHECK_RACH_REQ,                      NodebCheckRachReq nodebCheckRachReq)
SIG_DEF (SIG_NODEB_CHECK_RACH_SUCCESS_IND,              NodebCheckRachSuccessInd nodebCheckRachSuccessInd)
SIG_DEF (SIG_NODEB_CHECK_RACH_FAIL_IND,                 NodebCheckRachFailInd nodebCheckRachFailInd)
SIG_DEF (SIG_NODEB_SIB12_INTRA_MEAS_ON_RACH_REQ,        NodebSib12IntraMeasOnRachReq nodebSib12IntraMeasOnRachReq)
SIG_DEF (SIG_NODEB_MENU_SPECIAL_CELL_PCH_REQ,           NodebMenuSpecialCellPchReq nodebMenuSpecialCellPchReq)
SIG_DEF (SIG_NODEB_TEST_LOG_IND,                        NodebTestLogInd nodebTestLogInd)
SIG_DEF (SIG_NODEB_IE_CHECK_INIT_REQ,                   NodebIECheckInitReq nodebIECheckInitReq)
SIG_DEF (SIG_NODEB_IE_CHECK_REQ,                        NodebIECheckReq nodebIECheckReq)
SIG_DEF (SIG_NODEB_IE_CHECK_CNF,                        EmptySignal nodebIECheckCnf)
SIG_DEF (SIG_NODEB_REPORT_RRC_CCCH_MESSAGE_REQ,         NodebRrcReportCcchMessageReq nodebRrcReportCcchMessageReq)
SIG_DEF (SIG_NODEB_SETUP_RB25_AM_DCH,                   EmptySignal nodebSetupRb25AmDch)
SIG_DEF (SIG_NODEB_MEASUREMENT_CONTROL_CONFIGURE_TVM,               EmptySignal nodebMeasurementControlConfigureTvm)
SIG_DEF (SIG_NODEB_MEASUREMENT_CONTROL_UE_INTERNAL_PERIODIC,
         EmptySignal nodebMeasurementControlUeInternalPeriodic)
SIG_DEF (SIG_NODEB_MEASUREMENT_CONTROL_PERIODIC_RELEASE,            EmptySignal nodebMeasurementControlPeriodicRelease)
SIG_DEF (SIG_NODEB_MEASUREMENT_CONTROL_INTER_RAT_MODIFY_PERIODIC,
         EmptySignal nodebMeasurementControlInterRatModifyPeriodic)
SIG_DEF (SIG_NODEB_MEASUREMENT_CONTROL_INTER_RAT_MODIFY_PERIODIC_2,
         EmptySignal nodebMeasurementControlInterRatModifyPeriodic2)
SIG_DEF (SIG_NODEB_MEASUREMENT_CONTROL_INTER_RAT_PERIODIC,          EmptySignal nodebMeasurementControlInterRatPeriodic)
SIG_DEF (SIG_NODEB_MEASUREMENT_CONTROL_INTER_RAT_3A,                EmptySignal nodebMeasurementControlInterRat3a)
SIG_DEF (SIG_NODEB_MEASUREMENT_CONTROL_INTER_RAT_3B,                EmptySignal nodebMeasurementControlInterRat3b)
SIG_DEF (SIG_NODEB_MEASUREMENT_CONTROL_INTER_RAT_3C,                EmptySignal nodebMeasurementControlInterRat3c)
SIG_DEF (SIG_NODEB_MEASUREMENT_CONTROL_INTER_RAT_3D,                EmptySignal nodebMeasurementControlInterRat3d)
SIG_DEF (SIG_NODEB_MEASUREMENT_CONTROL_INTER_PERIODIC,              EmptySignal nodebMeasurementControlInterPeriodic)
SIG_DEF (SIG_NODEB_MEASUREMENT_CONTROL_INTER_EVENT_2F,              EmptySignal nodebMeasurementControlInterEvent2f)
SIG_DEF (SIG_NODEB_MEASUREMENT_CONTROL_INTER_EVENT_2E,              EmptySignal nodebMeasurementControlInterEvent2e)
SIG_DEF (SIG_NODEB_MEASUREMENT_CONTROL_INTER_EVENT_2D,              EmptySignal nodebMeasurementControlInterEvent2d)
SIG_DEF (SIG_NODEB_MEASUREMENT_CONTROL_INTER_EVENT_2C,              EmptySignal nodebMeasurementControlInterEvent2c)
SIG_DEF (SIG_NODEB_MEASUREMENT_CONTROL_INTER_EVENT_2B,              EmptySignal nodebMeasurementControlInterEvent2b)
SIG_DEF (SIG_NODEB_MEASUREMENT_CONTROL_EVENT_RELEASE,               EmptySignal nodebMeasurementControlEventRelease)
SIG_DEF (SIG_NODEB_MEASUREMENT_CONTROL_INTER_EVENT_2A,              EmptySignal nodebMeasurementControlInterEvent2a)

/* NodeB release 6 signals */
SIG_DEF (SIG_NODEB_SETUP_RB25_HSDPA_HSUPA,              EmptySignal nodebRbsRb25HsdpaHsupa)
SIG_DEF (SIG_NODEB_RELEASE_RB25_HSDPA_HSUPA,            EmptySignal nodebReleaseRb25HsdpaHsupa)
SIG_DEF (SIG_NODEB_QOS_DOWNGRADE_EDCH_TFCI_POWER_OFFSET, EmptySignal nodebPhyChanReconfigDowngradeEDCHTfciPower)
SIG_DEF (SIG_NODEB_QOS_DOWNGRADE_EDCH_MAC_PDU_SIZE,     EmptySignal nodebRbReconfigDowngradeEDCHPduSize)
SIG_DEF (SIG_NODEB_SETUP_RB25_PS_HSDPA_HSUPA,           EmptySignal nodebRbsRb25PsHsdpaHsupa)
SIG_DEF (SIG_NODEB_DOWNGRADE_EDCH_HSDCH_TO_DCH,         EmptySignal nodebDowngradeEdchHsdchToDch)
SIG_DEF (SIG_NODEB_UPGRADE_DCH_TO_EDCH_HSDCH,           EmptySignal nodebUpgradeDchToEdchHsdch)
SIG_DEF (SIG_NODEB_FACH_TO_DCH,                         EmptySignal nodebFachToDch)
SIG_DEF (SIG_NODEB_DCH_TO_FACH,                         EmptySignal nodebDchToFach)
SIG_DEF (SIG_NODEB_RECONFIG_RB25_DCH_TO_HSDPA_HSUPA_HHO, EmptySignal nodebRbsRb25PsHsdpaHsupaHho)
SIG_DEF (SIG_NODEB_HSUPA_HSDPA_UM_RBS,                  EmptySignal nodebHsupaHsdpaUmRbs)
SIG_DEF (SIG_NODEB_PHY_CHAN_RECFG_EDCH_RL,              EmptySignal nodebPhyChanRecfgEdchRl)
SIG_DEF (SIG_NODEB_ASU_DELETE_PSC_100,                  EmptySignal nodebAsuDeletePsc100)
SIG_DEF (SIG_NODEB_ASU_ADD_PSC_100,                     EmptySignal nodebAsuAddPsc100)

/* Signals to control Raven */
SIG_DEF (SIG_RAVEN_ENABLE_CH1_REQ,                      EmptySignal ravenEnableChannel1)
SIG_DEF (SIG_RAVEN_AT_COMMAND_REQ,                      RavenAtCommandReq ravenAtCommandReq)

/* Signal to control cell band */
SIG_DEF (SIG_NODEB_CELL_BAND_REQ,                       NodebCellBandReq nodebCellBandReq)

/* NodeB Sim Scripted signals */
SIG_DEF (SIG_NODEB_MOVE_TO_SCRIPTED_MODE_REQ,           NbMoveToScriptedModeReq nbMoveToScriptedModeReq)
SIG_DEF (SIG_NODEB_REPORT_NAS_MESSAGE_REQ,              NbReportNasMessageReq nbReportNasMessageReq)
SIG_DEF (SIG_NODEB_REPORT_NAS_MESSAGE_CNF,              EmptySignal nbReportNasMessageCnf)
SIG_DEF (SIG_NODEB_WRONG_NAS_MESSAGE_IND,               EmptySignal nbWrongNasMessageInd)
SIG_DEF (SIG_NODEB_SET_DATA_GEN_REQ,                    NbSetDataGenReq nbSetDataGenReq)
SIG_DEF (SIG_NODEB_CLR_DATA_GEN_REQ,                    NbClrDataGenReq nbClrDataGenReq)
SIG_DEF (SIG_NODEB_SET_RLC_CONFIG_REQ,                  NbSetRlcConfigReq nbSetRlcConfigReq)
SIG_DEF (SIG_NODEB_SET_MAC_CONFIG_REQ,                  NbSetMacConfigReq nbSetMacConfigReq)

/* NodeB signals to encode loopback control signals */
SIG_DEF (SIG_NODEB_LOOPBACK_ACTIVATE_REQ,               EmptySignal nodebLoopbackActivateReq)
SIG_DEF (SIG_NODEB_LOOPBACK_CLOSE_REQ,                  NbCloseTestLoopReq nodebLoopbackCloseReq)
SIG_DEF (SIG_NODEB_LOOPBACK_GENERATE_REQ,               NbGenerateLoopbackSduReq nbGenerateLoopbackSduReq)
SIG_DEF (SIG_NODEB_LOOPBACK_OPEN_REQ,                   EmptySignal nodebLoopbackOpenReq)
SIG_DEF (SIG_NODEB_LOOPBACK_DEACTIVATE_REQ,             EmptySignal nodebLoopbackDeactivateReq)

/* NodeB signal to force a RrcConnectionRelease */
SIG_DEF (SIG_NODEB_CONNECTION_RELEASE_REQ,              EmptySignal nodebConnectionReleaseReq)
#  endif
#endif

#if !defined (EXCLUDE_UPSUT)
SIG_DEF (SIG_UPSUT_DUMMY = UPSUT_SIGNAL_BASE,           EmptySignal upsut_dummy)
# if defined (ENABLE_UPLANE_STATISTICS)
SIG_DEF (SIG_UPSUT_UPLANE_STATISTICS,                   UplaneStatisticsReport uplaneStatisticsReport)
# else /* !ENABLE_UPLANE_STATISTICS */
SIG_DEF (SIG_UPSUT_UPLANE_STATISTICS,                   EmptySignal uplaneStatisticsReport)
# endif /* ENABLE_UPLANE_STATISTICS */
#endif /* EXCLUDE_UPSUT */

#endif /* UPGRADE_3G */
/* END OF FILE */
