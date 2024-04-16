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
 *  File Description : GSM Protocol Stack Signal Definitions
 **************************************************************************/

#if !defined (EXCLUDE_CB)
    /*
    ** SMS/CB-RR signal
    */
    SIG_DEF( SIG_RRCB_DUMMY = CB_SIGNAL_BASE,   EmptySignal              rrcb_dummy)
    SIG_DEF( SIG_GRRCB_CONTROL_REQ,             GrrCbControlReq          grrCbControlReq)
    SIG_DEF( SIG_GRRCB_CELL_CHANGE_IND,         GrrCbCellChangeInd       grrCbCellChangeInd)
    /*
    ** SMS/CB-L2 signals
    */
    SIG_DEF( SIG_L2CB_HEADER_IND,               L2cbHeaderInd            l2cbHeaderInd)
    SIG_DEF( SIG_L2CB_DATA_IND,                 L2cbDataInd              l2cbDataInd)
    SIG_DEF( SIG_L2CB_SCHED_HEADER_IND,         L2cbSchedHeaderInd       l2cbSchedHeaderInd)
#endif

#if !defined (EXCLUDE_DL)
    /*
    ** RR-L2 signals
    */
    SIG_DEF( SIG_DL_DUMMY = DL_MDL_SIGNAL_BASE, EmptySignal              dl_dummy)
    SIG_DEF( SIG_DL_DATA_IND,                   DlDataInd                dlDataInd)
    SIG_DEF( SIG_DL_UNIT_DATA_IND,              DlUnitDataInd            dlUnitDataInd)
    SIG_DEF( SIG_DL_RELEASE_IND,                DlReleaseInd             dlReleaseInd)
    SIG_DEF( SIG_DL_ESTABLISH_IND,              DlEstablishInd           dlEstablishInd)
    SIG_DEF( SIG_DL_BCCH_BSIC_IND,              DlBcchBsicInd            dlBcchBsicInd)
    SIG_DEF( SIG_DL_ESTABLISH_CNF,              DlEstablishCnf           dlEstablishCnf)
    SIG_DEF( SIG_DL_RELEASE_CNF,                DlReleaseCnf             dlReleaseCnf)
    SIG_DEF( SIG_DL_SUSPEND_CNF,                DlSuspendCnf             dlSuspendCnf)
    SIG_DEF( SIG_DL_REDUNDANT_1,                EmptySignal              dlRedundant1)
    SIG_DEF( SIG_DL_REDUNDANT_2,                EmptySignal              dlRedundant2)
    SIG_DEF( SIG_DL_DATA_REQ,                   DlDataReq                dlDataReq)
    SIG_DEF( SIG_DL_UNIT_DATA_REQ,              DlUnitDataReq            dlUnitDataReq)
    SIG_DEF( SIG_DL_ESTABLISH_REQ,              DlEstablishReq           dlEstablishReq)
    SIG_DEF( SIG_DL_RELEASE_REQ,                DlReleaseReq             dlReleaseReq)
    SIG_DEF( SIG_DL_SUSPEND_REQ,                EmptySignal              dlSuspendReq)
    SIG_DEF( SIG_DL_RESUME_REQ,                 DlResumeReq              dlResumeReq)
    SIG_DEF( SIG_DL_RECONNECT_REQ,              DlReconnectReq           dlReconnectReq)
    SIG_DEF( SIG_DL_REDUNDANT_3,                EmptySignal              dlRedundant3)
    SIG_DEF( SIG_MDL_ERROR_IND,                 MdlErrorInd              mdlErrorInd)
    SIG_DEF( SIG_MDL_RELEASE_REQ,               EmptySignal              mdlReleaseReq)
#endif


#if !defined (EXCLUDE_GRR)
    /*
    ** GMM-GRR signals
    */
    SIG_DEF( SIG_GRR_DUMMY = GRR_SIGNAL_BASE,    EmptySignal              grr_dummy)
    SIG_DEF( SIG_GRR_ABORT_REQ,                  GrrAbortReq              grrAbortReq)
    SIG_DEF( SIG_GRR_ACT_REQ,                    GrrActReq                grrActReq)
    SIG_DEF( SIG_GRR_DATA_REQ,                   GrrDataReq               grrDataReq)
    SIG_DEF( SIG_GRR_DEACT_REQ,                  GrrDeactReq              grrDeactReq)
    SIG_DEF( SIG_GRR_EST_REQ,                    GrrEstReq                grrEstReq)
    SIG_DEF( SIG_GRR_PLMN_LIST_REQ,              GrrPlmnListReq           grrPlmnListReq)
    SIG_DEF( SIG_GRR_CLASSMARK_REQ,              GrrClassmarkReq          grrClassmarkReq)
    SIG_DEF( SIG_GRR_EST_CNF,                    EmptySignal              grrEstCnf)
    SIG_DEF( SIG_GRR_PLMN_LIST_CNF,              GrrPlmnListCnf           grrPlmnListCnf)
    SIG_DEF( SIG_GRR_ABORT_IND,                  EmptySignal              grrAbortInd)
    SIG_DEF( SIG_GRR_ACT_IND,                    GrrActInd                grrActInd)
    SIG_DEF( SIG_GRR_DATA_IND,                   GrrDataInd               grrDataInd)
    SIG_DEF( SIG_GRR_EST_IND,                    EmptySignal              grrEstInd)
    SIG_DEF( SIG_GRR_REL_IND,                    GrrRelInd                grrRelInd)
    SIG_DEF( SIG_GRR_SYNC_IND,                   GrrSyncInd               grrSyncInd)
    SIG_DEF( SIG_GRR_UNIT_DATA_IND,              GrrUnitDataInd           grrUnitDataInd)
    SIG_DEF( SIG_GRR_PLMN_LIST_IND,              GrrPlmnListInd           grrPlmnListInd)
    SIG_DEF( SIG_GRR_TEST_IDLE_INFO_IND,         GrrTestIdleInfoInd       grrTestIdleInfoInd)
    SIG_DEF( SIG_GRR_TEST_DED_INFO_IND,          GrrTestDedInfoInd        grrTestDedInfoInd)
    SIG_DEF( SIG_GRR_TEST_IDLE_SCELL_IND,        GrrTestIdleSCellInd      grrTestIdleSCellInd)
    SIG_DEF( SIG_GRR_TEST_DED_SCELL_IND,         GrrTestDedSCellInd       grrTestDedSCellInd)
    SIG_DEF( SIG_GRR_DEACT_CNF,                  EmptySignal              grrDeactCnf)
    SIG_DEF( SIG_GRR_ACT_CNF,                    GrrActCnf                grrActCnf)
    SIG_DEF( SIG_GRR_REDUNDANT_1,                EmptySignal              grrRedundant1)
    SIG_DEF( SIG_GRR_PAGE_IND,                   GrrPageInd               grrPageInd)
    SIG_DEF( SIG_GRR_SYS_INFO_IND,               GrrSysInfoInd            grrSysInfoInd)
    SIG_DEF( SIG_GRR_ME_DATA_REQ,                GrrMeDataReq             grrMeDataReq)
    SIG_DEF( SIG_GRR_MS_DATA_REQ,                GrrMsDataReq             grrMsDataReq)
    SIG_DEF( SIG_GRR_UPDATE_REQ,                 GrrUpdateReq             grrUpdateReq)
    SIG_DEF( SIG_GRR_CELL_UPDATE_IND,            GrrCellUpdateInd         grrCellUpdateInd)
    SIG_DEF( SIG_GRR_TEST_MODE_REQ,              GrrTestModeReq           grrTestModeReq)
    SIG_DEF( SIG_GRR_TST_IDLE_SI_BUFFER_IND,     GrrTstIdleSiBufferInd    grrTstIdleSiBufferInd)
    SIG_DEF( SIG_GRR_TST_IDLE_RESEL_IND,         GrrTstIdleReselInd       grrTstIdleReselInd)
    SIG_DEF( SIG_GRR_TST_DED_BSIC_MON_IND,       GrrTstDedBsicMonInd      grrTstDedBsicMonInd)
    SIG_DEF( SIG_GRR_GPRS_MSLOT_CHANGE_REQ,      GrrGprsMslotChangeReq    grrGprsMslotChangeReq)
    SIG_DEF( SIG_GRR_GPRS_READY_STATE_REQ,       GrrGprsReadyStateReq     grrGprsReadyStateReq)
    SIG_DEF( SIG_GRR_PKT_MSG_DECODE_IND,         GrrPktMsgDecodeInd       grrPktMsgDecodeInd)
    SIG_DEF( SIG_GRR_TEST_RESEL_INFO_IND,        GrrTestReselInfoInd      grrTestReselInfoInd)
    SIG_DEF( SIG_GRR_TEST_GPRS_ENG_INFO_IND,     GrrTestGprsEngInfoInd    grrTestGprsEngInfoInd)

# if defined (UPGRADE_APP_INFO)
    SIG_DEF( SIG_GRR_AI_DATA_IND,                GrrAiDataInd             grrAiDataInd)
    SIG_DEF( SIG_GRR_AI_DATA_RSP,                GrrAiDataRsp             grrAiDataRsp)
    SIG_DEF( SIG_GRR_AI_RESET_IND,               GrrAiResetInd            grrAiResetInd)
# endif


    SIG_DEF( SIG_GRR_SHORT_PD_DECODE_IND,        GrrShortPdDecodeInd        grrShortPdDecodeInd)
    SIG_DEF( SIG_GRR_TST_ENH_REPORT_IND,         GrrTstEnhReportInd         grrTstEnhReportInd)

    SIG_DEF( SIG_GRR_LOCAL_INFO_REQ,             EmptySignal                grrLocalInfoReq)
    SIG_DEF( SIG_GRR_LOCAL_INFO_CNF,             GrrLocalInfoCnf            grrLocalInfoCnf)

# if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
    SIG_DEF( SIG_GRR_NAS_TX_QUEUE_INFO_IND,              GrrNasTxQueueInfoInd       grrNasTxQueueInfoInd)
    SIG_DEF( SIG_GRR_MODE_SELECT_REQ,                    GrrModeSelectReq           grrModeSelectReq)
    SIG_DEF( SIG_GRR_MODE_SELECT_CNF,                    GrrModeSelectCnf           grrModeSelectCnf)
    SIG_DEF( SIG_GRR_DEBUG_INTER_RAT_HO_INFO_TX_IND,     GrrDebugInterRatHoTxInd    grrDebugInterRatHoTxInd)
# endif   /* UPGRADE_3G && UPGRADE_GGE */

    SIG_DEF( SIG_GRR_ACC_STR_CAPAB_INFO_IND,     GrrAccStratumCapabInfoInd  grrAccStratumCapabInfoInd)
    SIG_DEF( SIG_GRR_ENGINEERING_COMMAND_REQ,    GrrEngineeringCommandReq   grrEngineeringCommandReq)
    SIG_DEF( SIG_GRR_ENGINEERING_COMMAND_CNF,    GrrEngineeringCommandCnf   grrEngineeringCommandCnf)

#endif   /* EXCLUDE_GRR */

#if defined (UPGRADE_3G) && defined (UPGRADE_GGE)
    SIG_DEF( SIG_GRRRC_DUMMY = GRRRC_SIGNAL_BASE,    EmptySignal               grrRc_dummy)
    SIG_DEF( SIG_GRRRC_RESELECT_TO_GSM_REQ,          GrrRcReselectToGsmReq     grrRcReselectToGsmReq)
    SIG_DEF( SIG_GRRRC_RESELECT_TO_GSM_CNF,          GrrRcReselectToGsmCnf     grrRcReselectToGsmCnf)
    SIG_DEF( SIG_GRRRC_RESELECT_TO_UMTS_REQ,         GrrRcReselectToUmtsReq    grrRcReselectToUmtsReq)
    SIG_DEF( SIG_GRRRC_RESELECT_TO_UMTS_CNF,         GrrRcReselectToUmtsCnf    grrRcReselectToUmtsCnf)
    SIG_DEF( SIG_GRRRC_HANDOVER_TO_GSM_REQ,          GrrRcHandoverToGsmReq     grrRcHandoverToGsmReq)
    SIG_DEF( SIG_GRRRC_HANDOVER_TO_GSM_CNF,          GrrRcHandoverToGsmCnf     grrRcHandoverToGsmCnf)
    SIG_DEF( SIG_GRRRC_HANDOVER_TO_UMTS_REQ,         GrrRcHandoverToUmtsReq    grrRcHandoverToUmtsReq)
    SIG_DEF( SIG_GRRRC_HANDOVER_TO_UMTS_CNF,         GrrRcHandoverToUmtsCnf    grrRcHandoverToUmtsCnf)
    SIG_DEF( SIG_GRRRC_GSM_INFO_IND,                 GrrRcGsmInfoInd           grrRcGsmInfoInd)
    SIG_DEF( SIG_GRRRC_UMTS_INFO_IND,                GrrRcUmtsInfoInd          grrRcUmtsInfoInd)
# if defined (UPGRADE_LTE)
    SIG_DEF( SIG_GRRRC_RESELECT_TO_EUTRA_REQ,        GrrRcReselectToEutraReq   grrRcReselectToEutraReq)
    SIG_DEF( SIG_GRRRC_RESELECT_TO_EUTRA_CNF,        GrrRcReselectToEutraCnf   grrRcReselectToEutraCnf)
    SIG_DEF( SIG_GRRRC_REDIRECT_TO_EUTRA_REQ,        GrrRcRedirectToEutraReq   grrRcRedirectToEutraReq)
    SIG_DEF( SIG_GRRRC_REDIRECT_TO_EUTRA_CNF,        GrrRcRedirectToEutraCnf   grrRcRedirectToEutraCnf)
    SIG_DEF( SIG_GRRRC_HANDOVER_TO_EUTRA_REQ,        GrrRcHandoverToEutraReq   grrRcHandoverToEutraReq)
    SIG_DEF( SIG_GRRRC_HANDOVER_TO_EUTRA_CNF,        GrrRcHandoverToEutraCnf   grrRcHandoverToEutraCnf)
# endif /* UPGRADE_LTE */
#endif /* UPGRADE_3G && UPGRADE_GGE */

/* END OF FILE */
