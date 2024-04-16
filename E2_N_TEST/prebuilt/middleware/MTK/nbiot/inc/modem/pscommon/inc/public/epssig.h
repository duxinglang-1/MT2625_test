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
 * LTE PS Signal definitions
 **************************************************************************/

#if !defined (EXCLUDE_ERRC)
SIG_DEF (SIG_ERRC_DUMMY = ERRC_SIGNAL_BASE,         EmptySignal                      errc_dummy)
SIG_DEF (SIG_ERRC_DEBUG_ASN1_MESSAGE_IND,           ErrcDebugAsn1MessageInd          errcDebugAsn1MessageInd)
SIG_DEF (SIG_ERRC_DL_INFORMATION_TRANSFER_NB,       EmptySignal                      errcDLInformationTransferNb)
SIG_DEF (SIG_ERRC_MASTER_INFORMATION_BLOCK_NB,      EmptySignal                      errcMasterInformationBlockNb)
SIG_DEF (SIG_ERRC_PAGING_NB,                        EmptySignal                      errcPagingNb)
SIG_DEF (SIG_ERRC_RRC_CONNECTION_RECONFIGURATION_NB, EmptySignal                     errcRRCConnectionReconfigurationNb)
SIG_DEF (SIG_ERRC_RRC_CONNECTION_RECONFIGURATION_COMPLETE_NB,  EmptySignal           errcRRCConnectionReconfigurationCompleteNb)
SIG_DEF (SIG_ERRC_RRC_CONNECTION_REESTABLISHMENT_NB, EmptySignal                     errcRRCConnectionReestablishmentNb)
SIG_DEF (SIG_ERRC_RRC_CONNECTION_REESTABLISHMENT_COMPLETE_NB, EmptySignal            errcRRCConnectionReestablishmentCompleteNb)
SIG_DEF (SIG_ERRC_RRC_CONNECTION_REESTABLISHMENT_REQUEST_NB, EmptySignal             errcRRCConnectionReestablishmentRequestNb)
SIG_DEF (SIG_ERRC_RRC_CONNECTION_REJECT_NB,         EmptySignal                      errcRRCConnectionRejectNb)
SIG_DEF (SIG_ERRC_RRC_CONNECTION_RELEASE_NB,        EmptySignal                      errcRRCConnectionReleaseNb)
SIG_DEF (SIG_ERRC_RRC_CONNECTION_REQUEST_NB,        EmptySignal                      errcRRCConnectionRequestNb)
SIG_DEF (SIG_ERRC_RRC_CONNECTION_RESUME_NB,         EmptySignal                      errcRRCConnectionResumeNb)
SIG_DEF (SIG_ERRC_RRC_CONNECTION_RESUME_COMPLETE_NB, EmptySignal                     errcRRCConnectionResumeCompleteNb)
SIG_DEF (SIG_ERRC_RRC_CONNECTION_RESUME_REQUEST_NB, EmptySignal                      errcRRCConnectionResumeRequestNb)
SIG_DEF (SIG_ERRC_RRC_CONNECTION_SETUP_NB,          EmptySignal                      errcRRCConnectionSetupNb)
SIG_DEF (SIG_ERRC_RRC_CONNECTION_SETUP_COMPLETE_NB, EmptySignal                      errcRRCConnectionSetupCompleteNb)
SIG_DEF (SIG_ERRC_SYSTEM_INFORMATION_NB,            EmptySignal                      errcSystemInformationNb)
SIG_DEF (SIG_ERRC_SYSTEM_INFORMATION_BLOCK_TYPE_1_NB, EmptySignal                    errcSystemInformationBlockType1Nb)
SIG_DEF (SIG_ERRC_UE_CAPABILITY_ENQUIRY_NB,         EmptySignal                      errcUECapabilityEnquiryNb)
SIG_DEF (SIG_ERRC_UE_CAPABILITY_INFORMATION_NB,     EmptySignal                      errcUECapabilityInformationNb)
SIG_DEF (SIG_ERRC_UL_INFORMATION_TRANSFER_NB,       EmptySignal                      errcULInformationTransferNb)
SIG_DEF (SIG_ERRC_RRC_CONNECTION_REESTABLISHMENT_REJECT, EmptySignal                 errcRRCConnectionReestablishmentReject)
SIG_DEF (SIG_ERRC_SECURITY_MODE_COMMAND,            EmptySignal                      errcSecurityModeCommand)
SIG_DEF (SIG_ERRC_SECURITY_MODE_COMPLETE,           EmptySignal                      errcSecurityModeComplete)
SIG_DEF (SIG_ERRC_SECURITY_MODE_FAILURE,            EmptySignal                      errcSecurityModeFailure)
SIG_DEF (SIG_ERRC_UPDATE_REQ,                       ErrcUpdateReq                    errcUpdateReq)
SIG_DEF (SIG_ERRC_CELL_SELECT_REQ,                  ErrcCellSelectReq                errcCellSelectReq)
SIG_DEF (SIG_ERRC_CELL_SELECT_CNF,                  ErrcCellSelectCnf                errcCellSelectCnf)
SIG_DEF (SIG_ERRC_CELL_SELECT_IND,                  ErrcCellSelectInd                errcCellSelectInd)
SIG_DEF (SIG_ERRC_PLMN_LIST_REQ,                    ErrcPlmnListReq                  errcPlmnListReq)
SIG_DEF (SIG_ERRC_PLMN_LIST_IND,                    ErrcPlmnListInd                  errcPlmnListInd)
SIG_DEF (SIG_ERRC_PLMN_LIST_CNF,                    ErrcPlmnListCnf                  errcPlmnListCnf)
SIG_DEF (SIG_ERRC_PLMN_LIST_ABORT_REQ,              ErrcPlmnListAbortReq             errcPlmnListAbortReq)
SIG_DEF (SIG_ERRC_ESTABLISH_REQ,                    ErrcEstablishReq                 errcEstablishReq)
SIG_DEF (SIG_ERRC_ESTABLISH_CNF,                    ErrcEstablishCnf                 errcEstablishCnf)
SIG_DEF (SIG_ERRC_PAGE_IND,                         ErrcPageInd                      errcPageInd)
SIG_DEF (SIG_ERRC_DATA_REQ,                         ErrcDataReq                      errcDataReq)
SIG_DEF (SIG_ERRC_DATA_IND,                         ErrcDataInd                      errcDataInd)
SIG_DEF (SIG_ERRC_IMPORT_TRANS_IND,                 ErrcImportTransInd               errcImportTransInd)
SIG_DEF (SIG_ERRC_DRB_IND,                          ErrcDrbInd                       errcDrbInd)
SIG_DEF (SIG_ERRC_RELEASE_REQ,                      ErrcReleaseReq                   errcReleaseReq)
SIG_DEF (SIG_ERRC_RELEASE_IND,                      ErrcReleaseInd                   errcReleaseInd)
SIG_DEF (SIG_ERRC_RESUME_REQ,                       ErrcResumeReq                    errcResumeReq)
SIG_DEF (SIG_ERRC_RESUME_CNF,                       ErrcResumeCnf                    errcResumeCnf)
SIG_DEF (SIG_ERRC_SUSPEND_IND,                      ErrcSuspendInd                   errcSuspendInd)
SIG_DEF (SIG_ERRC_DISCARD_IND,                      ErrcDiscardInd                   errcDiscardInd)
SIG_DEF (SIG_ERRC_DEACTIVATE_REQ,                   ErrcDeactivateReq                errcDeactivateReq)
SIG_DEF (SIG_ERRC_DEACTIVATE_CNF,                   ErrcDeactivateCnf                errcDeactivateCnf)
SIG_DEF (SIG_ERRC_DEBUG_STATE_CHANGE_IND,           ErrcDebugStateChangeInd          errcDebugStateChangeInd)
SIG_DEF (SIG_ERRC_DEBUG_CSR_STATUS_IND,             ErrcDebugCsrStatusInd            errcDebugCsrStatusInd)
SIG_DEF (SIG_ERRC_DEBUG_RBC_MESSAGE_IND,            ErrcRbcDebugMessage              errcRbcDebugMessage)
SIG_DEF (SIG_ERRC_TEST_AIR_INTERFACE_SIGNAL_IND,    ErrcTestAirInterfaceSignalInd    errcTestAirInterfaceSignalInd)
SIG_DEF (SIG_ERRC_DEBUG_CELL_LIST_IND,              ErrcDebugCellListInd             errcDebugCellListInd)
SIG_DEF (SIG_ERRC_L1_TEST_MODE_ACTIVATE_REQ,        ErrcL1TestModeActivateReq        errcL1TestModeActivateReq)
SIG_DEF (SIG_ERRC_DEEP_SLEEP_REQ,                   ErrcDeepSleepReq                 errcDeepSleepReq)
SIG_DEF (SIG_ERRC_NAS_MAC_NEEDED_IND,               ErrcNasMacNeededInd              errcNasMacNeededInd)
SIG_DEF (SIG_ERRC_NAS_MAC_NEEDED_RSP,               ErrcNasMacNeededRsp              errcNasMacNeededRsp)
SIG_DEF (SIG_ERRC_INFO_REQ,                         ErrcInfoReq                      errcInfoReq)
SIG_DEF (SIG_ERRC_INFO_CNF,                         ErrcInfoCnf                      errcInfoCnf)
SIG_DEF (SIG_ERRC_SIM_INFO_NOTIFY_REQ,              ErrcSimInfoNotifyReq             errcSimInfoNotifyReq)
SIG_DEF (SIG_ERRC_LOCATION_CELL_INFO_REQ,           ErrcLocationCellInfoReq          errcLocationCellInfoReq)
SIG_DEF (SIG_ERRC_LOCATION_CELL_INFO_CNF,           ErrcLocationCellInfoCnf          errcLocationCellInfoCnf)
SIG_DEF (SIG_ERRC_ATTACH_STATUS_NOTIFY_REQ,         ErrcAttachStatusNotifyReq        errcAttachStatusNotifyReq)
#endif /* EXCLUDE_ERRC */

#if !defined (EXCLUDE_EL2)
SIG_DEF (SIG_EL2_DUMMY = EL2_SIGNAL_BASE,           EmptySignal                      el2Dummy)
SIG_DEF (SIG_EL2_DATA_REQ,                          El2DataReq                       el2DataReq)
SIG_DEF (SIG_EL2_DATA_CNF,                          El2DataCnf                       el2DataCnf)
SIG_DEF (SIG_EL2_DATA_IND,                          El2DataInd                       el2DataInd)
SIG_DEF (SIG_EL2_DATA_RSP,                          El2DataRsp                       el2DataRsp)
SIG_DEF (SIG_EL2_USER_DATA_REQ,                     El2UserDataReq                   el2UserDataReq)
SIG_DEF (SIG_EL2_USER_DATA_IND,                     El2UserDataInd                   el2UserDataInd)
SIG_DEF (SIG_EL2_RELEASE_IND,                       El2ReleaseInd                    el2ReleaseInd)
SIG_DEF (SIG_EL2_BUFFER_FULL_IND,                   El2BufferFullInd                 el2BufferFullInd)
SIG_DEF (SIG_EL2_DISCARD_IND,                       El2DiscardInd                    el2DiscardInd)
SIG_DEF (SIG_EL2_L1_TEST_MODE_ACTIVATE_REQ,         El2L1TestModeActivateReq         el2L1TestModeActivateReq)
SIG_DEF (SIG_EL2_CONFIG_REQ,                        El2ConfigReq                     el2ConfigReq)
SIG_DEF (SIG_EL2_CONFIG_CNF,                        El2ConfigCnf                     el2ConfigCnf)
SIG_DEF (SIG_EL2_DATA_ACK_REQ,                      El2DataAckReq                    el2DataAckReq)
SIG_DEF (SIG_EL2_DATA_ACK_CNF,                      El2DataAckCnf                    el2DataAckCnf)
SIG_DEF (SIG_EL2_SECURITY_ACTIVATE_REQ,             El2SecurityActivateReq           el2SecurityActivateReq)
SIG_DEF (SIG_EL2_SECURITY_ACTIVATE_CNF,             El2SecurityActivateCnf           el2SecurityActivateCnf)
SIG_DEF (SIG_EL2_INTEGRITY_CHECK_FAIL_IND,          El2IntegrityCheckFailInd         el2IntegrityCheckFailInd)
SIG_DEF (SIG_EL2_RLC_MAX_RETRIES_IND,               El2RlcMaxRetriesInd              el2RlcMaxRetriesInd)
SIG_DEF (SIG_EL2_SUSPEND_REQ,                       El2SuspendReq                    el2SuspendReq)
SIG_DEF (SIG_EL2_RESUME_REQ,                        El2ResumeReq                     el2ResumeReq)
SIG_DEF (SIG_EL2_UL_GRANT_IND,                      El2UlGrantInd                    el2UlGrantInd)
SIG_DEF (SIG_EL2_INFO_REQ,                          El2InfoReq                       el2InfoReq)
SIG_DEF (SIG_EL2_INFO_CNF,                          El2InfoCnf                       el2InfoCnf)
SIG_DEF (SIG_EL2_DATA_INACT_TIMER_EXPIRY_IND,       El2DataInactTimerExpiryInd       el2DataInactTimerExpiryInd)
SIG_DEF (SIG_USER_DATA_TPUT_INFO_IND,               UserDataTputInfoInd              userDataTputInfoInd)
SIG_DEF (SIG_EL2_CP_DATA_DELIVERY_IND,              El2CpDataDeliveryInd             el2CpDataDeliveryInd)
SIG_DEF (SIG_EL2_UPDIR_DELIVERY_COMPLETE_IND,       El2UpdirDeliveryCompleteInd      el2UpdirDeliveryCompleteInd)
#endif /* EXCLUDE_EL2 */

#if !defined (EXCLUDE_TIMDBM)
SIG_DEF (SIG_TIMDBM_DUMMY = TIMDBM_SIGNAL_BASE,     EmptySignal                      timDbm_dummy)
SIG_DEF (SIG_TIMDBM_CLOSE_UE_TEST_LOOP_REQ,         TimDbmCloseUeTestLoopReq         timDbmCloseUeTestLoopReq)
SIG_DEF (SIG_TIMDBM_CLOSE_UE_TEST_LOOP_CNF,         TimDbmCloseUeTestLoopCnf         timDbmCloseUeTestLoopCnf)
SIG_DEF (SIG_TIMDBM_OPEN_UE_TEST_LOOP_REQ,          TimDbmOpenUeTestLoopReq          timDbmOpenUeTestLoopReq)
SIG_DEF (SIG_TIMDBM_OPEN_UE_TEST_LOOP_CNF,          TimDbmOpenUeTestLoopCnf          timDbmOpenUeTestLoopCnf)
SIG_DEF (SIG_TIMDBM_ACTIVATE_TEST_MODE_REQ,         TimDbmActivateTestModeReq        timDbmActivateTestModeReq)
SIG_DEF (SIG_TIMDBM_DEACTIVATE_TEST_MODE_REQ,       TimDbmDeactivateTestModeReq      timDbmDeactivateTestModeReq)
#endif /* EXCLUDE_TIMDBM */

/* END OF FILE */
