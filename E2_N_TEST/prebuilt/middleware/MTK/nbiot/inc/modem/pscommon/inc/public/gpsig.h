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
 * GPRS signal definitions
 **************************************************************************/


#if !defined (EXCLUDE_LLC)
SIG_DEF( SIG_LLC_DUMMY = GPLLC_SIGNAL_BASE, EmptySignal   gpllc_dummy)

SIG_DEF( SIG_LLGMM_ASSIGN_REQ,    LlgmmAssignReq     llgmmAssignReq)
SIG_DEF( SIG_LLGMM_RESET_REQ,     LlgmmResetReq      llgmmResetReq)  /* SGSN */
SIG_DEF( SIG_LLGMM_RESET_CNF,     LlgmmResetCnf      llgmmResetCnf)  /* SGSN */
SIG_DEF( SIG_LLGMM_TRIGGER_REQ,   LlgmmTriggerReq    llgmmTriggerReq)
SIG_DEF( SIG_LLGMM_SUSPEND_REQ,   LlgmmSuspendReq    llgmmSuspendReq)
SIG_DEF( SIG_LLGMM_RESUME_REQ,    LlgmmResumeReq     llgmmResumeReq)
SIG_DEF( SIG_LLGMM_PAGE_IND,      LlgmmPageInd       llgmmPageInd)   /* SGSN */
SIG_DEF( SIG_LLGMM_IOV_REQ,       LlgmmIovReq        llgmmIovReq)    /* SGSN */
SIG_DEF( SIG_LLGMM_IOV_CNF,       LlgmmIovCnf        llgmmIovCnf)    /* SGSN */
SIG_DEF( SIG_LLGMM_STATUS_IND,    LlgmmStatusInd     llgmmStatusInd)

SIG_DEF( SIG_LLC_RESET_IND,       LlcResetInd        llcResetInd)
SIG_DEF( SIG_LLC_ESTABLISH_REQ,   LlcEstablishReq    llcEstablishReq)
SIG_DEF( SIG_LLC_ESTABLISH_CNF,   LlcEstablishCnf    llcEstablishCnf)
SIG_DEF( SIG_LLC_ESTABLISH_IND,   LlcEstablishInd    llcEstablishInd)
SIG_DEF( SIG_LLC_ESTABLISH_RSP,   LlcEstablishRsp    llcEstablishRsp)
SIG_DEF( SIG_LLC_RELEASE_REQ,     LlcReleaseReq      llcReleaseReq)
SIG_DEF( SIG_LLC_RELEASE_CNF,     LlcReleaseCnf      llcReleaseCnf)
SIG_DEF( SIG_LLC_RELEASE_IND,     LlcReleaseInd      llcReleaseInd)
SIG_DEF( SIG_LLC_DATA_REQ,        LlcDataReq         llcDataReq)
SIG_DEF( SIG_LLC_DATA_CNF,        LlcDataCnf         llcDataCnf)
SIG_DEF( SIG_LLC_DATA_IND,        LlcDataInd         llcDataInd)
SIG_DEF( SIG_LLC_UNIT_DATA_REQ,   LlcUnitDataReq     llcUnitDataReq)
SIG_DEF( SIG_LLC_UNIT_DATA_CNF,   LlcUnitDataCnf     llcUnitDataCnf)
SIG_DEF( SIG_LLC_UNIT_DATA_IND,   LlcUnitDataInd     llcUnitDataInd)
SIG_DEF( SIG_LLC_XID_REQ,         LlcXidReq          llcXidReq)
SIG_DEF( SIG_LLC_XID_CNF,         LlcXidCnf          llcXidCnf)
SIG_DEF( SIG_LLC_XID_IND,         LlcXidInd          llcXidInd)
SIG_DEF( SIG_LLC_XID_RSP,         LlcXidRsp          llcXidRsp)
SIG_DEF( SIG_LLC_STATUS_IND,      LlcStatusInd       llcStatusInd)

SIG_DEF( SIG_LLC_RX_OFFSET_REQ,   LlcRxOffsetReq     llcRxOffsetReq)
SIG_DEF( SIG_LLC_CFG_REQ,         LlcCfgReq          llcCfgReq)
SIG_DEF( SIG_LLC_PDU_TRACE_REQ,   LlcPduTraceReq     llcPduTraceReq)
SIG_DEF( SIG_LLC_PDU_TRACE_IND,   LlcPduTraceInd     llcPduTraceInd)

#endif

#if defined (UPGRADE_NASMDL2)
#if !defined (EXCLUDE_DBM)
SIG_DEF( SIG_DBM_DUMMY = DBM_SIGNAL_BASE,  EmptySignal                 dbm_dummy)
SIG_DEF( SIG_DBM_CLOSE_DATA_CHANNEL_REQ,   DbmCloseDataChannelReq      dbmCloseDataChannelReq)
SIG_DEF( SIG_DBM_COUNTER_IND,              DbmCounterInd               dbmCounterInd)
SIG_DEF( SIG_DBM_EXCEPTION_REPORTS_REQ,    DbmExceptionReportsReq      dbmExceptionReportsReq)
SIG_DEF( SIG_DBM_EXCEPTION_REPORTS_CNF,    DbmExceptionReportsCnf      dbmExceptionReportsCnf)
SIG_DEF( SIG_DBM_OPEN_DATA_CHANNEL_CNF,    DbmOpenDataChannelCnf       dbmOpenDataChannelCnf)
SIG_DEF( SIG_DBM_OPEN_DATA_CHANNEL_REQ,    DbmOpenDataChannelReq       dbmOpenDataChannelReq)
SIG_DEF( SIG_DBM_REPORT_COUNTER_REQ,       DbmReportCounterReq         dbmReportCounterReq)
SIG_DEF( SIG_DBM_REPORT_COUNTER_CNF,       DbmReportCounterCnf         dbmReportCounterCnf)
SIG_DEF( SIG_DBM_RESET_COUNTER_REQ,        DbmResetCounterReq          dbmResetCounterReq)
SIG_DEF( SIG_DBM_RESTRICTION_REQ,          DbmRestrictionReq           dbmRestrictionReq)
SIG_DEF( SIG_DBM_PACKET_DISCARD_IND,       DbmPacketDiscardInd         dbmPacketDiscardInd)
SIG_DEF( SIG_DBM_EXCEPTION_REPORTS_ALLOWED_REQ,     DbmExceptionReportsAllowedReq      dbmExceptionReportsAllowedReq)
SIG_DEF( SIG_DBM_USER_DATA_IND,            DbmUserdataInd              dbmUserdataInd)
SIG_DEF( SIG_DBM_POWER_DOWN_REQ,           DbmPowerDownReq             dbmPowerDownReq)
SIG_DEF( SIG_DBM_SET_UPDIR_INFO_REQ,       DbmSetUpdirInfoReq          dbmSetUpdirInfoReq)
SIG_DEF( SIG_DBM_RESET_UPDIR_INFO_REQ,     DbmResetUpdirInfoReq        dbmResetUpdirInfoReq)
SIG_DEF( SIG_DBM_SET_UPDIR_INFO_CNF,       DbmSetUpdirInfoCnf          dbmSetUpdirInfoCnf)
SIG_DEF( SIG_DBM_UPDIR_IND,                DbmUpdirInd                 dbmUpdirInd)

#if defined (DBM_UNIT_TEST)
/* Used during unit testing only */
SIG_DEF( SIG_DBM_UT_CONFIG_REQ,            DbmUtConfigReq              dbmUtConfigReq)
SIG_DEF( SIG_DBM_UT_CONFIG_CNF,            DbmUtConfigCnf              dbmUtConfigCnf)
#endif /* DBM_UNIT_TEST */
#endif /* !defined (EXCLUDE_DBM) */
#endif /* UPGRADE_NASMDL2 */

#if !defined (EXCLUDE_DBMINT)
SIG_DEF( SIG_DBMINT_DUMMY = DBMINT_SIGNAL_BASE,     EmptySignal                  dbmInt_dummy             )
SIG_DEF( SIG_DBMINT_USER_DATA_TRIGGER_REQ,          DbmIntUserDataTriggerReq     dbmIntUserDataTriggerReq )
SIG_DEF( SIG_DBMINT_CP_DATA_DISCARD_IND,            DbmIntCpDataDiscardInd       dbmIntCpDataDiscardInd   )
SIG_DEF( SIG_DBMINT_UPDIR_PACKET_DISCARD_IND,       DbmIntUpdirPacketDiscardInd  dbmIntUpdirPacketDiscardInd   )
#endif

#if !defined (EXCLUDE_SN)
SIG_DEF( SIG_SN_DUMMY = GPSN_SIGNAL_BASE, EmptySignal   gpsn_dummy)

SIG_DEF( SIG_SN_DATA_REQ,       SnDataReq           snDataReq)
SIG_DEF( SIG_SN_UNIT_DATA_REQ,  SnUnitDataReq       snUnitDataReq)
SIG_DEF( SIG_SN_DATA_IND,       SnDataInd           snDataInd)
SIG_DEF( SIG_SN_UNIT_DATA_IND,  SnUnitDataInd       snUnitDataInd)
SIG_DEF( SIG_SN_XID_REQ,        SnXidReq            snXidReq)
SIG_DEF( SIG_SN_XID_CNF,        SnXidCnf            snXidCnf)
SIG_DEF( SIG_SN_XID_IND,        SnXidInd            snXidInd)
SIG_DEF( SIG_SN_XID_RSP,        SnXidRsp            snXidRsp)
SIG_DEF( SIG_SN_DATA_RSP,       EmptySignal         snDataRsp)
#if defined (ENABLE_TRAFFIC_SHAPING)
SIG_DEF( SIG_SN_TRAFFIC_SHAPING_CONFIG_REQ, SnTrafficShapingConfigReq snTrafficShapingConfigReq)
#endif
#if !defined (DATA_IN_SIGNAL)
SIG_DEF( SIG_SN_NPDU_DATA_DL_DEBUG,     NpduDataDebug           npduDataDlDebug)
SIG_DEF( SIG_SN_NPDU_DATA_UL_DEBUG,     NpduDataDebug           npduDataUlDebug)
#endif /* !defined (DATA_IN_SIGNAL)  */
#endif /* !defined (EXCLUDE_SN) */

#if !defined (EXCLUDE_SM)
SIG_DEF( SIG_SNSM_DUMMY = GPSNSM_SIGNAL_BASE, EmptySignal   gpsnsm_dummy)

SIG_DEF( SIG_SNSM_ACTIVATE_IND,   SnSmActivateInd    snsmActivateInd)
SIG_DEF( SIG_SNSM_ACTIVATE_RSP,   SnSmActivateRsp    snsmActivateRsp)
SIG_DEF( SIG_SNSM_DEACTIVATE_IND, SnSmDeactivateInd  snsmDeactivateInd)
SIG_DEF( SIG_SNSM_DEACTIVATE_RSP, SnSmDeactivateRsp  snsmDeactivateRsp)
SIG_DEF( SIG_SNSM_MODIFY_IND,     SnSmModifyInd      snsmModifyInd)
SIG_DEF( SIG_SNSM_MODIFY_RSP,     SnSmModifyRsp      snsmModifyRsp)
SIG_DEF( SIG_SNSM_STATUS_REQ,     SnSmStatusReq      snsmStatusReq)
SIG_DEF( SIG_SNSM_SEQUENCE_IND,   SnSmSequenceInd    snsmSequenceInd)
SIG_DEF( SIG_SNSM_SEQUENCE_RSP,   SnSmSequenceRsp    snsmSequenceRsp)
SIG_DEF( SIG_SNSM_REPORT_COUNTER_REQ,   SnSmReportCounterReq    snsmReportCounterReq)
SIG_DEF( SIG_SNSM_RESET_COUNTER_REQ,    SnSmResetCounterReq     snsmResetCounterReq)
SIG_DEF( SIG_SNSM_COUNTER_IND,   SnSmCounterInd      snsmCounterInd)

#if defined (UPGRADE_3G)
SIG_DEF( SIG_SNSM_DEACTIVATE_REQ, SnSmDeactivateReq  snsmDeactivateReq)
SIG_DEF( SIG_SNSM_RESUME_IND,     SnSmResumeInd      snsmResumeInd)
SIG_DEF( SIG_SNSM_SUSPEND_IND,    SnSmSuspendInd     snsmSuspendInd)
SIG_DEF( SIG_SNSM_INFO_IND,       SnSmInfoInd        snsmInfoInd)
#endif
#endif

#if defined(ENABLE_L23_DEBUG)
SIG_DEF( SIG_RASN_DEBUG_IND,      RasnDebugInd       rasnDebugInd)
#else
SIG_DEF( SIG_RASN_DEBUG_IND,      EmptySignal       rasnDebugInd)
#endif

#if defined (UPGRADE_NASMDL2)
#if !defined (EXCLUDE_DBMSM)
SIG_DEF( SIG_DBMSM_DUMMY = DBMSM_SIGNAL_BASE, EmptySignal       dbmsm_dummy)
SIG_DEF( SIG_DBMSM_ACTIVATE_IND,        DbmSmActivateInd        dbmSmActivateInd)
SIG_DEF( SIG_DBMSM_ACTIVATE_RSP,        DbmSmActivateRsp        dbmSmActivateRsp)
SIG_DEF( SIG_DBMSM_DEACTIVATE_IND,      DbmSmDeactivateInd      dbmSmDeactivateInd)
SIG_DEF( SIG_DBMSM_DEACTIVATE_RSP,      DbmSmDeactivateRsp      dbmSmDeactivateRsp)
SIG_DEF( SIG_DBMSM_MODIFY_IND,          DbmSmModifyInd          dbmSmModifyInd)
SIG_DEF( SIG_DBMSM_MODIFY_RSP,          DbmSmModifyRsp          dbmSmModifyRsp)
SIG_DEF( SIG_DBMSM_STATUS_REQ,          DbmSmStatusReq          dbmSmStatusReq)
SIG_DEF( SIG_DBMSM_SEQUENCE_IND,        DbmSmSequenceInd        dbmSmSequenceInd)
SIG_DEF( SIG_DBMSM_SEQUENCE_RSP,        DbmSmSequenceRsp        dbmSmSequenceRsp)
SIG_DEF( SIG_DBMSM_DEACTIVATE_REQ,      DbmSmDeactivateReq      dbmSmDeactivateReq)
SIG_DEF( SIG_DBMSM_RESUME_IND,          DbmSmResumeInd          dbmSmResumeInd)
SIG_DEF( SIG_DBMSM_SUSPEND_IND,         DbmSmSuspendInd         dbmSmSuspendInd)
SIG_DEF( SIG_DBMSM_STATUS_IND,          DbmSmStatusInd          dbmSmStatusInd)
SIG_DEF( SIG_DBMSM_UNIT_DATA_IND,       DbmSmUnitDataInd        dbmSmUnitDataInd)
SIG_DEF( SIG_DBMSM_UNIT_DATA_REQ,       DbmSmUnitDataReq        dbmSmUnitDataReq)
SIG_DEF( SIG_DBMSM_INIT_IND,            DbmSmInitInd            dbmSmInitInd)

#endif
#endif /* UPGRADE_NASMDL2 */

#if !defined (EXCLUDE_SMREG)
SIG_DEF( SIG_SMREG_DUMMY = GPSMREG_SIGNAL_BASE, EmptySignal       gpsmReg_dummy)

SIG_DEF( SIG_SMREG_PDP_ACTIVATE_REQ,     SmRegPdpActivateReq      smRegPdpActivateReq)
SIG_DEF( SIG_SMREG_PDP_ACTIVATE_CNF,     SmRegPdpActivateCnf      smRegPdpActivateCnf)
SIG_DEF( SIG_SMREG_PDP_ACTIVATE_REJ,     SmRegPdpActivateRej      smRegPdpActivateRej)
SIG_DEF( SIG_SMREG_PDP_REDUNDANT,        EmptySignal              smRegPdpRedundant)
SIG_DEF( SIG_SMREG_PDP_DEACTIVATE_REQ,   SmRegPdpDeactivateReq    smRegPdpDeactivateReq)
SIG_DEF( SIG_SMREG_PDP_DEACTIVATE_CNF,   SmRegPdpDeactivateCnf    smRegPdpDeactivateCnf)
SIG_DEF( SIG_SMREG_PDP_DEACTIVATE_IND,   SmRegPdpDeactivateInd    smRegPdpDeactivateInd)
SIG_DEF( SIG_SMREG_PDP_MODIFY_IND,       SmRegPdpModifyInd        smRegPdpModifyInd)

SIG_DEF( SIG_SMREG_PDP_MODIFY_REQ,       SmRegPdpModifyReq        smRegPdpModifyReq)
SIG_DEF( SIG_SMREG_PDP_MODIFY_CNF,       SmRegPdpModifyCnf        smRegPdpModifyCnf)
SIG_DEF( SIG_SMREG_PDP_MODIFY_REJ,       SmRegPdpModifyRej        smRegPdpModifyRej)
SIG_DEF( SIG_SMREG_PDP_ACTIVATE_SEC_REQ, SmRegPdpActivateSecReq   smRegPdpActivateSecReq)
SIG_DEF( SIG_SMREG_PDP_ACTIVATE_SEC_CNF, SmRegPdpActivateSecCnf   smRegPdpActivateSecCnf)
SIG_DEF( SIG_SMREG_PDP_ACTIVATE_SEC_REJ, SmRegPdpActivateSecRej   smRegPdpActivateSecRej)
SIG_DEF( SIG_SMREG_PDP_ACTIVATE_REJ_RSP, SmRegPdpActivateRejRsp   smRegPdpActivateRejRsp)

#endif

#if defined (UPGRADE_NASMDL2)
#if !defined (EXCLUDE_SMR)
SIG_DEF (SIG_SMR_DUMMY = SMR_SIGNAL_BASE,           EmptySignal                      smr_dummy)
SIG_DEF (SIG_SMR_ACTIVATE_DEDICATED_BEARER_IND,     SmrActivateDedicatedBearerInd    smrActivateDedicatedBearerInd)
SIG_DEF (SIG_SMR_ACTIVATE_DEDICATED_BEARER_REJ,     SmrActivateDedicatedBearerRej    smrActivateDedicatedBearerRej)
SIG_DEF (SIG_SMR_ACTIVATE_DEDICATED_BEARER_RSP,     SmrActivateDedicatedBearerRsp    smrActivateDedicatedBearerRsp)
SIG_DEF (SIG_SMR_ACTIVATE_DEFAULT_BEARER_IND,       SmrActivateDefaultBearerInd      smrActivateDefaultBearerInd)
SIG_DEF (SIG_SMR_ACTIVATE_DEFAULT_BEARER_REJ,       SmrActivateDefaultBearerRej      smrActivateDefaultBearerRej)
SIG_DEF (SIG_SMR_ACTIVATE_DEFAULT_BEARER_RSP,       SmrActivateDefaultBearerRsp      smrActivateDefaultBearerRsp)
SIG_DEF (SIG_SMR_BEARER_RESOURCE_ALLOC_CNF,         SmrBearerResourceAllocCnf        smrBearerResourceAllocCnf)
SIG_DEF (SIG_SMR_BEARER_RESOURCE_ALLOC_REJ,         SmrBearerResourceAllocRej        smrBearerResourceAllocRej)
SIG_DEF (SIG_SMR_BEARER_RESOURCE_ALLOC_REQ,         SmrBearerResourceAllocReq        smrBearerResourceAllocReq)
SIG_DEF (SIG_SMR_BEARER_RESOURCE_MODIFY_CNF,        SmrBearerResourceModifyCnf       smrBearerResourceModifyCnf)
SIG_DEF (SIG_SMR_BEARER_RESOURCE_MODIFY_REJ,        SmrBearerResourceModifyRej       smrBearerResourceModifyRej)
SIG_DEF (SIG_SMR_BEARER_RESOURCE_MODIFY_REQ,        SmrBearerResourceModifyReq       smrBearerResourceModifyReq)
SIG_DEF (SIG_SMR_CONFIG_IND,                        SmrConfigInd                     smrConfigInd)
SIG_DEF (SIG_SMR_DEACTIVATE_BEARER_IND,             SmrDeactivateBearerInd           smrDeactivateBearerInd)
SIG_DEF (SIG_SMR_DEACTIVATE_BEARER_RSP,             SmrDeactivateBearerRsp           smrDeactivateBearerRsp)
SIG_DEF (SIG_SMR_MODIFY_BEARER_IND,                 SmrModifyBearerInd               smrModifyBearerInd)
SIG_DEF (SIG_SMR_MODIFY_BEARER_REJ,                 SmrModifyBearerRej               smrModifyBearerRej)
SIG_DEF (SIG_SMR_MODIFY_BEARER_RSP,                 SmrModifyBearerRsp               smrModifyBearerRsp)
SIG_DEF (SIG_SMR_PDN_CONNECT_CNF,                   SmrPdnConnectCnf                 smrPdnConnectCnf)
SIG_DEF (SIG_SMR_PDN_CONNECT_IND,                   SmrPdnConnectInd                 smrPdnConnectInd)
SIG_DEF (SIG_SMR_PDN_CONNECT_REJ,                   SmrPdnConnectRej                 smrPdnConnectRej)
SIG_DEF (SIG_SMR_PDN_CONNECT_REQ,                   SmrPdnConnectReq                 smrPdnConnectReq)
SIG_DEF (SIG_SMR_PDN_DISCONNECT_CNF,                SmrPdnDisconnectCnf              smrPdnDisconnectCnf)
SIG_DEF (SIG_SMR_PDN_DISCONNECT_REQ,                SmrPdnDisconnectReq              smrPdnDisconnectReq)
SIG_DEF (SIG_SMR_PDN_INFO_IND,                      SmrPdnInfoInd                    smrPdnInfoInd)
SIG_DEF (SIG_SMR_PDN_INFO_REJ,                      SmrPdnInfoRej                    smrPdnInfoRej)
SIG_DEF (SIG_SMR_STATUS_IND,                        SmrStatusInd                     smrStatusInd)
SIG_DEF (SIG_SMR_TEST_INFO_IND,                     SmrTestInfoInd                   smrTestInfoInd)
SIG_DEF (SIG_SMR_BEARER_CONFIGURE_REQ,              SmrBearerConfigureReq            smrBearerConfigureReq)
SIG_DEF (SIG_SMR_BEARER_CONFIGURE_CNF,              SmrBearerConfigureCnf            smrBearerConfigureCnf)
SIG_DEF (SIG_SMR_INIT_TFT_IND,                      SmrInitialiseTftInd              smrInitialiseTftInd)
SIG_DEF (SIG_SMR_INIT_QOS_IND,                      SmrInitialiseQosInd              smrInitialiseQosInd)
SIG_DEF (SIG_SMR_INIT_ESM_REQ,                      SmrInitEsmReq                    smrInitEsmReq)

#endif /* EXCLUDE_SMR */
#endif /* UPGRADE_NASMDL2 */

#if !defined (EXCLUDE_GMM)
SIG_DEF( SIG_GMM_DUMMY = GPGMM_SIGNAL_BASE, EmptySignal       gpgmm_dummy)
SIG_DEF( SIG_GMM_UNIT_DATA_REQ,          GmmUnitDataReq       gmmUnitDataReq)
SIG_DEF( SIG_GMM_UNIT_DATA_IND,          GmmUnitDataInd       gmmUnitDataInd)
SIG_DEF( SIG_GMM_PDU_UNIT_DATA_IND,      GmmPduUnitDataInd    gmmPduUnitDataInd)
SIG_DEF( SIG_GPRS_NAS_MESSAGE_DEBUG_IND,      GprsNasMessageDebugInd    gprsNasMessageDebugInd)
# if defined (UPGRADE_3G)
SIG_DEF( SIG_UMM_UNIT_DATA_REQ,          UmmUnitDataReq       ummUnitDataReq)
SIG_DEF( SIG_UMM_UNIT_DATA_IND,          UmmUnitDataInd       ummUnitDataInd)
#  if defined (UPGRADE_LTE)
SIG_DEF( SIG_EMM_UNIT_DATA_REQ,          EmmUnitDataReq       emmUnitDataReq)
SIG_DEF( SIG_EMM_UNIT_DATA_IND,          EmmUnitDataInd       emmUnitDataInd)
#   if defined (EMM_TEST)
SIG_DEF( SIG_EMM_TIMER_INFO_IND,         EmmTimerInfoInd      emmTimerInfoInd)
SIG_DEF( SIG_EMM_DEEP_SLEEP_ENTRY_IND,   EmptySignal          emmDeepSleepEntryInd)
SIG_DEF( SIG_EMM_DEEP_SLEEP_WAKEUP_IND,  EmmDeepSleepWakeupInd   emmDeepSleepWakeupInd)
#   endif
SIG_DEF( SIG_NAS_DEBUG_MESSAGE_IND,      NasDebugMessageInd      nasDebugMessageInd)
#  endif
# endif
#endif
#if defined (EMM_UNIT_TESTING)
/* Used during unit testing only */
SIG_DEF( SIG_EMM_UT_CONFIG_REQ,          EmmUtConfigReq       emmUtConfigReq)
/* EMM_TO_BE_DONE SIG_DEF( SIG_EMM_UT_CONFIG_CNF,          EmmUtConfigCnf       emmUtConfigCnf) */
#endif /* EMM_UNIT_TESTING */

#if !defined (EXCLUDE_GMMSM)
SIG_DEF( SIG_GMMSM_DUMMY = GPGMMSM_SIGNAL_BASE, EmptySignal   gpgmmSm_dummy)
SIG_DEF( SIG_GMMSM_ESTABLISH_REQ, EmptySignal                 gmmSmEstablishReq)
SIG_DEF( SIG_GMMSM_ESTABLISH_CNF, EmptySignal                 gmmSmEstablishCnf)
SIG_DEF( SIG_GMMSM_ESTABLISH_REJ, GmmSmEstablishRej           gmmSmEstablishRej)
SIG_DEF( SIG_GMMSM_RELEASE_IND,   EmptySignal                 gmmSmReleaseInd)
SIG_DEF( SIG_GMMSM_UNIT_DATA_REQ, GmmSmUnitDataReq            gmmSmUnitDataReq)
SIG_DEF( SIG_GMMSM_UNIT_DATA_IND, GmmSmUnitDataInd            gmmSmUnitDataInd)
SIG_DEF( SIG_GMMSM_SEQUENCE_IND,  GmmSmSequenceInd            gmmSmSequenceInd)
SIG_DEF( SIG_GMMSM_SEQUENCE_RSP,  GmmSmSequenceRsp            gmmSmSequenceRsp)
SIG_DEF( SIG_GMMSM_RESUME_IND,    GmmSmResumeInd              gmmSmResumeInd)
SIG_DEF( SIG_GMMSM_SUSPEND_IND,   GmmSmSuspendInd             gmmSmSuspendInd)
SIG_DEF( SIG_GMMSM_PDP_CONTEXT_IND,GmmSmPdpContextInd         gmmSmPdpContextInd)
SIG_DEF( SIG_GMMSM_PDP_CONTEXT_RSP,GmmSmPdpContextRsp         gmmSmPdpContextRsp)
SIG_DEF( SIG_GMMSM_TEST_MODE_IND, GmmSmTestModeInd            gmmSmTestModeInd)
#if defined (UPGRADE_3G)
SIG_DEF( SIG_GMMSM_INFO_IND,      GmmSmInfoInd                gmmSmInfoInd)
SIG_DEF( SIG_GMMSM_ABORT_IND,     GmmSmAbortInd               gmmSmAbortInd)
#endif
SIG_DEF( SIG_GMMSM_ABORT_REQ,     GmmSmAbortReq               gmmSmAbortReq)
#endif

#if defined (UPGRADE_NASMDL2)
#if !defined (EXCLUDE_MMSM)
SIG_DEF( SIG_MMSM_DUMMY = MMSM_SIGNAL_BASE, EmptySignal             mmSm_dummy)
SIG_DEF( SIG_MMSM_ABORT_IND,                MmSmAbortInd            mmSmAbortInd)
SIG_DEF( SIG_MMSM_ABORT_REQ,                MmSmAbortReq            mmSmAbortReq)
SIG_DEF( SIG_MMSM_ESTABLISH_CNF,            MmSmEstablishCnf        mmSmEstablishCnf)
SIG_DEF( SIG_MMSM_ESTABLISH_REJ,            MmSmEstablishRej        mmSmEstablishRej)
SIG_DEF( SIG_MMSM_PSM_STATUS_IND,           MmSmPsmStatusInd        mmSmPsmStatusInd)
SIG_DEF( SIG_MMSM_ESTABLISH_REQ,            MmSmEstablishReq        mmSmEstablishReq)
SIG_DEF( SIG_MMSM_PDN_CONN_INFO_IND,        MmSmPdnConnInfoInd      mmSmPdnConnInfoInd)
SIG_DEF( SIG_MMSM_PDN_CONN_INFO_REJ,        MmSmPdnConnInfoRej      mmSmPdnConnInfoRej)
SIG_DEF( SIG_MMSM_PSD_BEARER_CONTEXT_IND,   MmSmPsdBearerContextInd mmSmPsdBearerContextInd)
SIG_DEF( SIG_MMSM_PSD_BEARER_CONTEXT_RSP,   MmSmPsdBearerContextRsp mmSmPsdBearerContextRsp)
SIG_DEF( SIG_MMSM_RELEASE_IND,              MmSmReleaseInd          mmSmReleaseInd)
SIG_DEF( SIG_MMSM_SEQUENCE_IND,             MmSmSequenceInd         mmSmSequenceInd)
SIG_DEF( SIG_MMSM_SEQUENCE_RSP,             MmSmSequenceRsp         mmSmSequenceRsp)
SIG_DEF( SIG_MMSM_STATUS_IND,               MmSmStatusInd           mmSmStatusInd)
SIG_DEF( SIG_MMSM_STATUS_REQ,               MmSmStatusReq           mmSmStatusReq)
SIG_DEF( SIG_MMSM_RESUME_IND,               MmSmResumeInd           mmSmResumeInd)
SIG_DEF( SIG_MMSM_SUSPEND_IND,              MmSmSuspendInd          mmSmSuspendInd)
SIG_DEF( SIG_MMSM_TEST_MODE_IND,            MmSmTestModeInd         mmSmTestModeInd)
SIG_DEF( SIG_MMSM_UNIT_DATA_IND,            MmSmUnitDataInd         mmSmUnitDataInd)
SIG_DEF( SIG_MMSM_UNIT_DATA_REQ,            MmSmUnitDataReq         mmSmUnitDataReq)
#endif /* EXCLUDE_MMSM */
#endif /* UPGRADE_NASMDL2 */

#if 0 // !defined (EXCLUDE_ABGPSM)
SIG_DEF( SIG_ABGP_SM_DUMMY = GPABGPSM_SIGNAL_BASE, EmptySignal     abgpSm_dummy)
SIG_DEF( SIG_ABGP_SM_QOS_IND,              AbgpSmQosInd            abgpSmQosInd)
SIG_DEF( SIG_ABGP_SM_REG_PDP_ACTIVATE_IND, AbgpSmRegPdpActivateInd abgpSmRegPdpActivateInd)
SIG_DEF( SIG_ABGP_SM_REG_PDP_ACTIVATE_RSP, AbgpSmRegPdpActivateRsp abgpSmRegPdpActivateRsp)
SIG_DEF( SIG_ABGP_SM_REPORT_COUNTER_REQ,   AbgpSmReportCounterReq  abgpSmReportCounterReq)
SIG_DEF( SIG_ABGP_SM_RESET_COUNTER_REQ,    AbgpSmResetCounterReq   abgpSmResetCounterReq)
SIG_DEF( SIG_ABGP_SM_COUNTER_IND,          AbgpSmCounterInd        abgpSmCounterInd)
SIG_DEF( SIG_ABGP_SM_TEST_INFO_IND,        AbgpSmTestInfoInd       abgpSmTestInfoInd)
SIG_DEF( SIG_ABGP_SM_CONFIG_IND,           AbgpSmConfigInd         abgpSmConfigInd)
#endif

#if defined (FEA_PPP)
#if !defined (EXCLUDE_DSPPP)
SIG_DEF( SIG_DS_PPP_DUMMY = GPDSPPP_SIGNAL_BASE, EmptySignal   dsppp_dummy)
SIG_DEF( SIG_DS_PPP_ERROR_IND,        DsPppErrorInd    dsPppErrorInd)
SIG_DEF( SIG_DS_PPP_DATA_IND,         DsPppDataInd     dsPppDataInd)
SIG_DEF( SIG_DS_PPP_CONFIG_REQ,       DsPppConfigReq   dsPppConfigReq)
SIG_DEF( SIG_DS_PPP_DATA_REQ,         DsPppDataReq     dsPppDataReq)
#endif

#if defined (UPGRADE_NASMDL2)
#if !defined (EXCLUDE_GPPP)
SIG_DEF( SIG_GPPP_DUMMY = GPPP_SIGNAL_BASE,    EmptySignal              gppp_dummy)
SIG_DEF( SIG_GPPP_NEG_START_REQ,               GpppNegStartReq          gpppNegStartReq)
SIG_DEF( SIG_GPPP_NEG_START_CNF,               GpppNegStartCnf          gpppNegStartCnf)
SIG_DEF( SIG_GPPP_PSD_ACTIVATE_IND,            GpppPsdActivateInd       gpppPsdActivateInd)
SIG_DEF( SIG_GPPP_PSD_ACTIVATE_RSP,            GpppPsdActivateRsp       gpppPsdActivateRsp)
SIG_DEF( SIG_GPPP_PSD_ACTIVATE_REJ,            GpppPsdActivateRej       gpppPsdActivateRej)
SIG_DEF( SIG_GPPP_PSD_DEACTIVATE_REQ,          GpppPsdDeactivateReq     gpppPsdDeactivateReq)
SIG_DEF( SIG_GPPP_PSD_DEACTIVATE_CNF,          GpppPsdDeactivateCnf     gpppPsdDeactivateCnf)
SIG_DEF( SIG_GPPP_PSD_DEACTIVATE_IND,          GpppPsdDeactivateInd     gpppPsdDeactivateInd)
SIG_DEF( SIG_GPPP_LOOPBACK_REQ,                GpppLoopbackReq          gpppLoopbackReq)
SIG_DEF( SIG_GPPP_LOOPBACK_CNF,                GpppLoopbackCnf          gpppLoopbackCnf)
SIG_DEF( SIG_GPPP_CONFIG_REQ,                  GpppConfigReq            gpppConfigReq)
SIG_DEF( SIG_GPPP_CONFIG_CNF,                  GpppConfigCnf            gpppConfigCnf)
SIG_DEF( SIG_GPPP_CONFIG_AUTH_REQ,             GpppConfigAuthReq        gpppConfigAuthReq)
SIG_DEF( SIG_GPPP_CONFIG_AUTH_CNF,             GpppConfigAuthCnf        gpppConfigAuthCnf)
#endif
/* TODO: Remove CIPPP interface under #else part of UPGRADE_NASMDL2 */
#endif

#if !defined (UPGRADE_NASMDL2)
#if !defined (EXCLUDE_CIPPP)
SIG_DEF( SIG_CI_PPP_DUMMY = GPCIPPP_SIGNAL_BASE, EmptySignal ciPpp_dummy)
SIG_DEF( SIG_CI_PPP_DIAL_IND,         CiPppDialInd        ciPppDialInd)
SIG_DEF( SIG_CI_PPP_HANGUP_IND,       CiPppHangupInd      ciPppHangupInd)
SIG_DEF( SIG_CI_PPP_CONNECT_REQ,      CiPppConnectReq     ciPppConnectReq)
SIG_DEF( SIG_CI_PPP_NO_CARRIER_REQ,   CiPppNoCarrierReq   ciPppNoCarrierReq)
SIG_DEF( SIG_CI_PPP_NO_CARRIER_CNF,   Int8                ciPppNoCarrierCnf)
SIG_DEF( SIG_CI_PPP_BUSY_REQ,         Int8                ciPppBusyReq)
/* 0206-18266: ERROR_REQ signal now has cause field */
SIG_DEF( SIG_CI_PPP_ERROR_REQ,        CiPppErrorReq       ciPppErrorReq)
SIG_DEF( SIG_CI_PPP_FLUSH_REQ,        EmptySignal         ciPppFlushReq)

SIG_DEF( SIG_CI_PPP_PDP_MODIFY_REQ,          CiPppPdpModifyReq          ciPppPdpModifyReq)
SIG_DEF( SIG_CI_PPP_PDP_MODIFY_CNF,          CiPppPdpModifyCnf          ciPppPdpModifyCnf)
SIG_DEF( SIG_CI_PPP_PDP_CONTEXT_IND,         CiPppPdpContextInd         ciPppPdpContextInd)
SIG_DEF( SIG_CI_PPP_PDP_ACTIVE_IND,          CiPppPdpActiveInd          ciPppPdpActiveInd)
SIG_DEF( SIG_CI_PPP_LOOPBACK_REQ,            CiPppLoopbackReq           ciPppLoopbackReq)
SIG_DEF( SIG_CI_PPP_LOOPBACK_CNF,            CiPppLoopbackCnf           ciPppLoopbackCnf)
SIG_DEF( SIG_CI_PPP_PDP_MODIFY_IND,          CiPppPdpModifyInd          ciPppPdpModifyInd)

SIG_DEF( SIG_CI_PPP_CONFIG_REQ,              CiPppConfigReq             ciPppConfigReq)
SIG_DEF( SIG_CI_PPP_CONFIG_CNF,              CiPppConfigCnf             ciPppConfigCnf)
SIG_DEF( SIG_CI_PPP_ANSWER_IND,              CiPppAnswerInd             ciPppAnswerInd)
SIG_DEF( SIG_CI_PPP_CONFIG_AUTH_REQ,         CiPppConfigAuthReq         ciPppConfigAuthReq)
SIG_DEF( SIG_CI_PPP_CONFIG_AUTH_CNF,         CiPppConfigAuthCnf         ciPppConfigAuthCnf)
#endif
#endif /* !UPGRADE_NASMDL2 */
#endif /* FEA_PPP */

#if !defined (EXCLUDE_RLCMAC)
SIG_DEF( SIG_RLCMAC_DUMMY = GPRLCMAC_SIGNAL_BASE, EmptySignal rlcmac_dummy)
SIG_DEF( SIG_RLCMAC_DATA_REQ,         RlcmacDataReq           rlcmacDataReq )
SIG_DEF( SIG_RLCMAC_DATA_IND,         RlcmacDataInd           rlcmacDataInd )
SIG_DEF( SIG_RLCMAC_UNIT_DATA_REQ,    RlcmacUnitDataReq       rlcmacUnitDataReq )
SIG_DEF( SIG_RLCMAC_UNIT_DATA_IND,    RlcmacUnitDataInd       rlcmacUnitDataInd )
SIG_DEF( SIG_RLCMAC_DATA_CFG_REQ,     RlcmacDataCfgReq        rlcmacDataCfgReq )
SIG_DEF( SIG_RLCMAC_FLUSH_REQ,        RlcmacFlushReq          rlcmacFlushReq )
#endif

#if defined (GPDEC_TEST)
/* Used during unit testing only */
SIG_DEF( SIG_GPDEC_DUMMY = GPDEC_SIGNAL_BASE, EmptySignal          gpdec_dummy)
SIG_DEF( SIG_GPDEC_DECODE_REQ,        GpdecDecodeReq               gpdecDecodeReq)
SIG_DEF( SIG_GPDEC_DECODE_IND,        EmptySignal                  gpdecDecodeInd)
#endif

#if defined (GPENC_TEST)
/* Used during unit testing only */
SIG_DEF( SIG_GPENC_DUMMY = GPENC_SIGNAL_BASE, EmptySignal               gpenc_dummy)
SIG_DEF( SIG_GPENC_PACKET_CONTROL_ACK_REQ,            GpencPacketControlAckReq            gpencPacketControlAckReq )
SIG_DEF( SIG_GPENC_PACKET_CELL_CHANGE_FAILURE_REQ,    GpencPacketCellChangeFailureReq     gpencPacketCellChangeFailureReq )
SIG_DEF( SIG_GPENC_PACKET_DOWNLINK_ACK_NACK_REQ,      GpencPacketDownlinkAckNackReq       gpencPacketDownlinkAckNackReq )
SIG_DEF( SIG_GPENC_PACKET_UPLINK_DUMMY_CTRL_BLK_REQ,  GpencPacketUplinkDummyCtrlBlkReq    gpencPacketUplinkDummyCtrlBlkReq )
SIG_DEF( SIG_GPENC_PACKET_MEASUREMENT_REPORT_REQ_EXT, GpencPacketMeasurementReportReqExt  gpencPacketMeasurementReportReqExt )
SIG_DEF( SIG_GPENC_PACKET_MOBILE_TBF_STATUS_REQ,      GpencPacketMobileTbfStatusReq       gpencPacketMobileTbfStatusReq )
SIG_DEF( SIG_GPENC_PACKET_RESOURCE_REQUEST_REQ,       GpencPacketResourceRequestReq       gpencPacketResourceRequestReq )
SIG_DEF( SIG_GPENC_PACKET_PSI_STATUS_REQ,             GpencPacketPsiStatusReq             gpencPacketPsiStatusReq )
SIG_DEF( SIG_GPENC_UT_BIT_REQ,                        GpencUtBitReq                       gpencUtBitReq )
SIG_DEF( SIG_GPENC_UT_BIT_FIELD_8_REQ,                GpencUtBitField8Req                 gpencUtBitField8Req )
SIG_DEF( SIG_GPENC_UT_BIT_FIELD_16_REQ,               GpencUtBitField16Req                gpencUtBitField16Req )
SIG_DEF( SIG_GPENC_UT_BIT_FIELD_32_REQ,               GpencUtBitField32Req                gpencUtBitField32Req )
SIG_DEF( SIG_GPENC_UT_OPT_FIELD_7_REQ,                GpencUtOptField7Req                 gpencUtOptField7Req )
SIG_DEF( SIG_GPENC_ENCODED_MESSAGE_IND,               GpencEncodedMessageInd              gpencEncodedMessageInd )
SIG_DEF( SIG_GPENC_ENCODED_MESSAGE_4_IND,             GpencEncodedMessage4Ind             gpencEncodedMessage4Ind )
SIG_DEF( SIG_GPENC_ENCODED_MESSAGE_11_IND,            GpencEncodedMessage11Ind            gpencEncodedMessage11Ind )
#endif

#if !defined (EXCLUDE_GMMRD)
SIG_DEF( SIG_RD_GMM_DUMMY = GPGMMRD_SIGNAL_BASE,      EmptySignal                          rdGmm_dummy     )
SIG_DEF( SIG_GMM_RD_TRIGGER_IND,                      GmmRdTriggerInd                      gmmRdTriggerInd )
SIG_DEF( SIG_GMM_RD_TEST_MODE_REQ,                    GmmRdTestModeReq                     gmmRdTestModeReq )
SIG_DEF( SIG_GMM_RD_RESUME_REQ,                       EmptySignal                          gmmRdResumeReq )
SIG_DEF( SIG_GMM_RD_SUSPEND_REQ,                      EmptySignal                          gmmRdSuspendReq )
SIG_DEF( SIG_GMM_RD_FLUSH_REQ,                        GmmRdFlushReq                        gmmRdFlushReq   )
#endif

#if !defined (EXCLUDE_GRRMAC)
SIG_DEF( SIG_GRR_MAC_DUMMY = GPGRRMAC_SIGNAL_BASE,    EmptySignal               grrMac_dummy            )
SIG_DEF( SIG_GRR_MAC_DL_TBF_ACK_NACK_IND,             GrrMacDlTbfAckNackInd     grrMacDlTbfAckNackInd   )
SIG_DEF( SIG_GRR_MAC_PKT_CONTROL_IND,                 GrrMacPktControlInd       grrMacPktControlInd     )
SIG_DEF( SIG_GRR_MAC_PKT_CONTROL_REQ,                 GrrMacPktControlReq       grrMacPktControlReq     )
SIG_DEF( SIG_GRR_MAC_PKT_SENT_IND,                    GrrMacPktSentInd          grrMacPktSentInd        )
SIG_DEF( SIG_GRR_MAC_UL_TBF_ACK_NACK_REQ,             GrrMacUlTbfAckNackReq     grrMacUlTbfAckNackReq   )
SIG_DEF( SIG_GRR_MAC_PKT_RELEASE_IND,                 GrrMacPktReleaseInd       grrMacPktReleaseInd     )
SIG_DEF( SIG_GRR_MAC_PKT_DECODE_ERROR_IND,            GrrMacPktDecodeErrorInd   grrMacPktDecodeErrorInd )
SIG_DEF( SIG_GRR_MAC_ENG_MODE_CTRL_REQ,               GrrMacEngModeCtrlReq      grrMacEngModeCtrlReq    )
SIG_DEF( SIG_GRR_MAC_ENG_INFO_IND,                    GrrMacEngInfoInd          grrMacEngInfoInd        )
#endif

#if !defined (EXCLUDE_RDMAC)
SIG_DEF( SIG_RD_MAC_DUMMY = GPRDMAC_SIGNAL_BASE,      EmptySignal               rdMac_dummy         )
SIG_DEF( SIG_RD_MAC_DATA_REQ,                         RdMacDataReq              rdMacDataReq        )
# if defined (DUMP_BSN_UPLINK) || defined (DUMP_BSN_DOWNLINK)
/* UP and DOWN */
SIG_DEF_CUSTOM_REF(SIG_RD_MAC_DATA_IND, RdMacDataInd, rdMacDataInd, DumpRlcmacBlock, DumpRlcmacData)
# else /* !(DUMP_BSN_UPLINK) && !(DUMP_BSN_DOWNLINK) */
/* nore Up nore Down */
SIG_DEF( SIG_RD_MAC_DATA_IND,                         RdMacDataInd              rdMacDataInd        )
# endif /* !(DUMP_BSN_UPLINK) && !(DUMP_BSN_DOWNLINK) */
# if defined (DUMP_BSN_UPLINK)
SIG_DEF_CUSTOM_REF(SIG_RD_MAC_READY_TO_SEND_IND, RdMacReadyToSendInd, rdMacReadyToSendInd, DumpRlcmacUpBlock, DumpRlcmacUpData)
# else /* !(DUMP_BSN_UPLINK) */
SIG_DEF( SIG_RD_MAC_READY_TO_SEND_IND,                RdMacReadyToSendInd       rdMacReadyToSendInd )
# endif /* !(DUMP_BSN_UPLINK) */
SIG_DEF( SIG_RD_MAC_LLC_PDU_ACK_IND,                  RdMacLlcPduAckInd         rdMacLlcPduAckInd   )
#endif

#if !defined (EXCLUDE_RDGRR)
SIG_DEF( SIG_RD_GRR_DUMMY = GPRDGRR_SIGNAL_BASE,      EmptySignal                 rdGrr_dummy                )
SIG_DEF( SIG_RD_GRR_CELL_CHANGE_IND,                  RdGrrCellChangeInd          rdGrrCellChangeInd         )
SIG_DEF( SIG_RD_GRR_DL_TBF_ASSIGN_IND,                EmptySignal                 rdGrrDlTbfAssignInd        )
SIG_DEF( SIG_RD_GRR_DL_TBF_FAILURE_IND,               RdGrrDlTbfFailureInd        rdGrrDlTbfFailureInd       )
SIG_DEF( SIG_RD_GRR_DL_TBF_RELEASE_IND,               EmptySignal                 rdGrrDlTbfReleaseInd       )
SIG_DEF( SIG_RD_GRR_UL_TBF_ASSIGN_IND,                RdGrrUlTbfAssignInd         rdGrrUlTbfAssignInd        )
SIG_DEF( SIG_RD_GRR_UL_TBF_ESTABLISH_REQ,             RdGrrUlTbfEstablishReq      rdGrrUlTbfEstablishReq     )
SIG_DEF( SIG_RD_GRR_UL_TBF_ESTABLISH_CNF,             RdGrrUlTbfEstablishCnf      rdGrrUlTbfEstablishCnf     )
SIG_DEF( SIG_RD_GRR_UL_TBF_ESTABLISH_FAIL_IND,        RdGrrUlTbfEstablishFailInd  rdGrrUlTbfEstablishFailInd )
SIG_DEF( SIG_RD_GRR_UL_TBF_RELEASE_IND,               EmptySignal                 rdGrrUlTbfReleaseInd       )
SIG_DEF( SIG_RD_GRR_UL_TBF_FAILURE_IND,               RdGrrUlTbfFailureInd        rdGrrUlTbfFailureInd       )
SIG_DEF( SIG_RD_GRR_UL_TBF_RESOURCE_REQ,              RdGrrUlTbfResourceReq       rdGrrUlTbfResourceReq      )
SIG_DEF( SIG_RD_GRR_SUSPEND_IND,                      EmptySignal                 rdGrrSuspendInd            )
SIG_DEF( SIG_RD_GRR_RESUME_IND,                       EmptySignal                 rdGrrResumeInd             )
SIG_DEF( SIG_RD_GRR_RECONNECT_IND,                    EmptySignal                 rdGrrReconnectInd          )
SIG_DEF( SIG_RD_GRR_PKT_TBF_REL_IND,                  RdGrrPktTbfRelInd           rdGrrPktTbfRelInd          )
SIG_DEF( SIG_RD_GRR_RESET_IND,                        EmptySignal                 rdGrrResetInd              )
SIG_DEF( SIG_RD_GRR_PKT_TBF_REL_RSP,                  EmptySignal                 rdGrrPktTbfRelRsp          )
#endif

#if !defined (EXCLUDE_MACINT)
SIG_DEF( SIG_MAC_INT_DUMMY = GPMACINT_SIGNAL_BASE,    EmptySignal                 macInt_dummy         )
SIG_DEF( SIG_MAC_INT_UL_TBF_STATUS_IND,               MacIntUlTbfStatusInd        macIntUlTbfStatusInd )
SIG_DEF( SIG_MAC_INT_DL_FLUSH_IND,                    EmptySignal                 macIntDlFlushInd     )
SIG_DEF( SIG_MAC_INT_UL_FLUSH_IND,                    EmptySignal                 macIntUlFlushInd     )
#if defined (DEVELOPMENT_VERSION)
SIG_DEF( SIG_MAC_INT_ERROR_IND,                       MacIntErrorInd              macIntErrorInd           )
SIG_DEF( SIG_MAC_INT_TX_DATA_DEBUG_IND,               MacIntTxDataDebugInd        macIntTxDataDebugInd     )
SIG_DEF( SIG_MAC_INT_RX_DATA_DEBUG_IND,               MacIntRxDataDebugInd        macIntRxDataDebugInd     )
SIG_DEF( SIG_MAC_INT_TX_STATE_DEBUG_IND,              MacIntTxStateDebugInd       macIntTxStateDebugInd    )
SIG_DEF( SIG_MAC_INT_RX_STATE_DEBUG_IND,              MacIntRxStateDebugInd       macIntRxStateDebugInd    )
SIG_DEF( SIG_MAC_INT_TX_RX_PTR_INFO_IND,              MacIntTxRxPtrInfoInd        macIntTxRxPtrInfoInd     )
SIG_DEF( SIG_MAC_INT_DATA_CONTENT_IND,                MacIntDataContentInd        macIntDataContentInd     )
SIG_DEF( SIG_MAC_INT_VS_VA_RDLB_DEBUG_IND,            MacIntTxVsVaRdLbDebugInd    macIntTxVsVaRdLbDebugInd )
SIG_DEF( SIG_MAC_INT_CTRL_MSG_BUFF_IND,               MacIntCtrlMsgBuffInd        macIntCtrlMsgBuffInd     )
SIG_DEF( SIG_MAC_INT_TX_INFO_REQ_DEBUG_IND,           MacIntTxInfoReqDebugInd     macIntTxInfoReqDebugInd  )
SIG_DEF( SIG_MAC_INT_TX_BLKS_PURG_DEBUG_IND,          MacIntTxBlksPurgDebugInd    macIntTxBlksPurgDebugInd )
#if defined (UPGRADE_EDGE)
SIG_DEF( SIG_MAC_INT_EGPRS_RX_QUAL_DEBUG_IND,         MacIntEgprsRxQualDebugInd   macIntEgprsRxQualDebugInd)
SIG_DEF( SIG_MAC_INT_EGPRS_RX_DATA_DEBUG_IND,         MacIntEgprsRxDataDebugInd   macIntEgprsRxDataDebugInd)
#endif /* UPGRADE_EDGE */
#endif /* DEVELOPMENT_VERSION */
#endif

#if defined (UPGRADE_EDGE)

#if defined (SDS_MEM_TRACK_VIA_SIGNALS)
SIG_DEF( SIG_MAC_INT_SDS_MEM_ALLOC_IND,               MacIntSdsMemAllocInd        macIntSdsMemAllocInd )
SIG_DEF( SIG_MAC_INT_SDS_MEM_FREE_IND,                MacIntSdsMemFreeInd         macIntSdsMemFreeInd  )
#endif

#if defined(SDS_MEM_STATISTICS)
SIG_DEF( SIG_MAC_INT_SDS_MEM_STATS_IND,               MacIntSdsMemStatisticsInd   macIntSdsMemStatsInd )
#endif

#endif

#if !defined (EXCLUDE_RDINT)
#if defined (DEVELOPMENT_VERSION)
SIG_DEF( SIG_RD_INT_DUMMY = GPRDINT_SIGNAL_BASE,     EmptySignal                  rdInt_dummy              )
SIG_DEF( SIG_RD_INT_STATUS_IND,                      RdIntStatusInd               rdIntStatusInd           )
SIG_DEF( SIG_RD_INT_TX_BLK_DATA_CONTENT_IND,         RdIntTxBlkDataContentInd     rdIntTxBlkDataContentInd )
SIG_DEF( SIG_RD_INT_RX_BLK_DATA_CONTENT_IND,         RdIntRxBlkDataContentInd     rdIntRxBlkDataContentInd )
SIG_DEF( SIG_RD_INT_PDU_DATA_CONTENT_IND,            RdIntPduDataContentInd       rdIntPduDataContentInd   )
SIG_DEF( SIG_RD_INT_OCTET_COUNT_DEBUG_IND,           RdIntOctetCountDebugInd      rdIntOctetCountDebugInd  )
SIG_DEF( SIG_RD_INT_SEG_CV_DEBUG_IND,                RdIntSegCvDebugInd           rdIntSegCvDebugInd       )
SIG_DEF( SIG_RD_INT_TX_PDU_QUEUE_DEBUG_IND,          RdIntTxPduQueueDebugInd      rdIntTxPduQueueDebugInd  )
#endif /* DEVELOPMENT_VERSION */
#endif

/* END OF FILE */
