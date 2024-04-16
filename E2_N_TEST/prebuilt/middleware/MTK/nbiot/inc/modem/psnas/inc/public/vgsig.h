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
 * File Description
 * ----------------
 *
 * Signal definitions for data services
 **************************************************************************/

SIG_DEF( SIG_NOCI_DUMMY = NOCI_SIGNAL_BASE,   EmptySignal                noCi_dummy)

SIG_DEF( SIG_DSDS_START_RX_POLL_REQ,          EmptySignal                dsdsStartRxPollReq)
SIG_DEF( SIG_DSDS_START_ESC_POLL_REQ,         EmptySignal                dsdsStartEscPollReq)

SIG_DEF( SIG_CIEX_DUMMY = CIEX_SIGNAL_BASE,   EmptySignal                ciexdummy)

SIG_DEF( SIG_CI_RUN_AT_COMMAND_IND,           CiRunAtCommandInd          ciRunAtCommandInd)

#if !defined (UPGRADE_GENERIC_MUX)
SIG_DEF( SIG_CI_EMPTY_TX_CACHE_IND,           CiEmptyTxCacheInd          ciEmptyTxCacheInd)
#endif
SIG_DEF( SIG_CI_DATA_ENTRY_MODE_IND,          CiDataEntryModeInd         ciDataEntryModeInd)
SIG_DEF( SIG_CI_USER_PROF_LOADED_IND,         CiUserProfLoadedInd        ciUserProfLoadedInd)
SIG_DEF( SIG_VGCI_SS_REGISTRATION_IND,        VgCiSsRegistrationInd      vgCiSsRegistrationInd)
SIG_DEF( SIG_CIRM_DATA_IND,                   CirmDataInd                cirmDataInd)

#if !defined (UPGRADE_GENERIC_MUX)
SIG_DEF( SIG_VGMX_PORT_RATE_IND,              VgmuxPortRateInd           vgmuxPortRateInd)
SIG_DEF( SIG_VGMX_DUMMY_22,                   EmptySignal                vgmuxDummy22)
SIG_DEF( SIG_CIDS_ENABLE_REQ,                 EmptySignal                cidsEnableReq)
SIG_DEF( SIG_CIDS_ENABLE_CNF,                 EmptySignal                cidsEnableCnf)
SIG_DEF( SIG_CIDS_SWITCH_REQ,                 CidsSwitchReq              cidsSwitchReq)
SIG_DEF( SIG_CIDS_TX_DATA_IND,                EmptySignal                cidsTxDataInd)
SIG_DEF( SIG_CIDS_RX_DATA_IND,                EmptySignal                cidsRxDataInd)
SIG_DEF( SIG_CIDS_RX_DATA_CNF,                EmptySignal                cidsRxDataCnf)
SIG_DEF( SIG_CIDS_TX_STATUS_IND,              VgStatus                   cidsTxStatusInd)
SIG_DEF( SIG_CIDS_RX_STATUS_IND,              VgStatus                   cidsRxStatusInd)
SIG_DEF( SIG_CIDS_SWITCH_IND,                 EmptySignal                cidsSwitchInd)
SIG_DEF( SIG_CIDS_SWITCH_STANDARD_IND,        CidsSwitchStandardInd      cidsSwitchStandardInd)
SIG_DEF( SIG_CIDS_SELECT_MUX_REQ,             EmptySignal                cidsSelectMuxReq)
SIG_DEF( SIG_CIDS_SELECT_MUX_CNF,             EmptySignal                cidsSelectMuxCnf)

SIG_DEF( SIG_CIDS_AT_COMMAND_DONE_REQ,        EmptySignal                cidsAtCommandDoneReq)
SIG_DEF( SIG_CIDS_UNSOLICITED_RESULT_REQ,     EmptySignal                cidsUnsolicitedResultReq)
SIG_DEF( SIG_CIDS_AT_O_COMMAND_REQ,           EmptySignal                cidsAtOCommandReq)
SIG_DEF( SIG_CIDS_AT_D_COMMAND_REQ,           EmptySignal                cidsAtDCommandReq)
SIG_DEF( SIG_CIDS_ADDITIONAL_INPUT_REQ,       EmptySignal                cidsAdditionalInputReq)

SIG_DEF( SIG_CIDS_START_SESSION_IND,          EmptySignal                cidsStartSessionInd)
SIG_DEF( SIG_CIDS_END_SESSION_IND,            EmptySignal                cidsEndSessionInd)
SIG_DEF( SIG_CIDS_START_GPRS_SESSION_IND,     EmptySignal                cidsStartGprsSessionInd)
SIG_DEF( SIG_CIDS_END_GPRS_SESSION_IND,       EmptySignal                cidsEndGprsSessionInd)
#endif

SIG_DEF( SIG_CIUF_HANGUP_FAX_CALL_IND,        EmptySignal                ciufHangupFaxCallInd)
#if defined (UPGRADE_3G) && !defined (UPGRADE_GENERIC_MUX)
SIG_DEF( SIG_EXCSD_DATA_REQ,                  ExcsdDataReq               excsdDataReq)
SIG_DEF( SIG_EXCSD_DATA_IND,                  ExcsdDataInd               excsdDataInd)
#endif

#if !defined (UPGRADE_GENERIC_MUX)
SIG_DEF( SIG_VGMX_CHANNEL_ENABLE_IND,         VgmuxEnableChannelInd      vgmuxEnableChannelInd)
SIG_DEF( SIG_VGMX_CHANNEL_ENABLE_RSP,         VgmuxEnableChannelRsp      vgmuxEnableChannelRsp)
SIG_DEF( SIG_VGMX_CHANNEL_DISABLE_IND,        VgmuxDisableChannelInd     vgmuxDisableChannelInd)
SIG_DEF( SIG_VGMX_CHANNEL_DISABLE_RSP,        VgmuxDisableChannelRsp     vgmuxDisableChannelRsp)

SIG_DEF( SIG_VGMX_AT_DATA_REQ,                VgmuxAtDataReq             vgmuxAtDataReq)
SIG_DEF( SIG_VGMX_AT_DATA_CNF,                VgmuxAtDataCnf             vgmuxAtDataCnf)
SIG_DEF( SIG_VGMX_AT_DATA_IND,                VgmuxAtDataInd             vgmuxAtDataInd)
SIG_DEF( SIG_VGMX_AT_DATA_RSP,                VgmuxAtDataRsp             vgmuxAtDataRsp)

SIG_DEF( SIG_VGMX_CHANNEL_XOFF_IND,           VgmuxXoffInd               vgmuxXoffInd)
SIG_DEF( SIG_VGMX_CHANNEL_XON_IND,            VgmuxXonInd                vgmuxXonInd)
SIG_DEF( SIG_VGMX_CHANNEL_XOFF_REQ,           VgmuxXoffReq               vgmuxXoffReq)
SIG_DEF( SIG_VGMX_CHANNEL_XON_REQ,            VgmuxXonReq                vgmuxXonReq)

SIG_DEF( SIG_VGMX_OPEN_DATA_CONN_REQ,         VgmuxOpenDataConnReq       vgmuxOpenDataConnReq)
SIG_DEF( SIG_VGMX_OPEN_DATA_CONN_CNF,         VgmuxOpenDataConnCnf       vgmuxOpenDataConnCnf)
SIG_DEF( SIG_VGMX_CLOSE_DATA_CONN_REQ,        VgmuxCloseDataConnReq      vgmuxCloseDataConnReq)
SIG_DEF( SIG_VGMX_ATO_COMMAND_REQ,            VgmuxAtoCommandReq         vgmuxAtoCommandReq)
SIG_DEF( SIG_VGMX_PPP_CONFIG_REQ,             VgmuxPppConfigReq          vgmuxPppConfigReq)
SIG_DEF( SIG_VGMX_VGMX_START_TIMER_REQ,       VgmuxStartTimerReq         vgmuxStartTimerReq)

SIG_DEF( SIG_VGMX_CONFIGURE_COM_PORT_REQ,     VgmuxConfigureComPortReq   vgmuxConfigureComPortReq)
SIG_DEF( SIG_VGMX_CONFIGURE_COM_PORT_IND,     VgmuxConfigureComPortInd   vgmuxConfigureComPortInd)

SIG_DEF( SIG_VGMX_INITIALISE_REQ,             VgmuxInitialiseReq         vgmuxInitialiseReq)
SIG_DEF( SIG_VGMX_INITIALISE_CNF,             VgmuxInitialiseCnf         vgmuxInitialiseCnf)
SIG_DEF( SIG_VGMX_CLOSE_REQ,                  VgmuxCloseReq              vgmuxCloseReq)
SIG_DEF( SIG_VGMX_CLOSE_CNF,                  VgmuxCloseCnf              vgmuxCloseCnf)
SIG_DEF( SIG_VGMX_CLOSE_IND,                  VgmuxCloseInd              vgmuxCloseInd)

SIG_DEF( SIG_VGMX_RECEIVED_LINE_STATUS_IND,   VgmuxReceivedLineStatusInd vgmuxReceivedLineStatusInd)
SIG_DEF( SIG_VGMX_TRANSMIT_LINE_STATUS_REQ,   VgmuxTransmitLineStatusReq vgmuxTransmitLineStatusReq)
SIG_DEF( SIG_VGMX_TX_DATA_ERROR_IND,          EmptySignal                vgmuxTxDataErrorInd)
SIG_DEF( SIG_VGMX_RX_DATA_ERROR_IND,          EmptySignal                vgmuxRxDataErrorInd)
SIG_DEF( SIG_VGMX_START_PASSTHROUGH_IND,      EmptySignal                vgmuxStartPassThroughInd)
SIG_DEF( SIG_VGMX_END_PASSTHROUGH_IND,        EmptySignal                vgmuxEndPassThroughInd)

SIG_DEF( SIG_CI_BOOT_REGISTER_TASK_REQ,       SigCiBootRegisterTaskReq   sigCiBootRegisterTaskReq)
SIG_DEF( SIG_CI_BOOT_REGISTER_TASK_IND,       SigCiBootRegisterTaskInd   sigCiBootRegisterTaskInd)

SIG_DEF( SIG_VGMX_SWITCH_COM_PORT_REQ,        VgmuxSwitchComPortReq      vgmuxSwitchComPortReq)
SIG_DEF( SIG_VGMX_SWITCH_COM_PORT_CNF,        VgmuxSwitchComPortCnf      vgmuxSwitchComPortCnf)

SIG_DEF( SIG_APEX_MUX_SELECT_REQ,             ApexMuxSelectReq           apexMuxSelectReq)
SIG_DEF( SIG_APEX_MUX_SELECT_CNF,             ApexMuxSelectCnf           apexMuxSelectCnf)
SIG_DEF( SIG_APEX_MUX_QUERY_REQ,              ApexMuxQueryReq            apexMuxQueryReq)
SIG_DEF( SIG_APEX_MUX_QUERY_CNF,              ApexMuxQueryCnf            apexMuxQueryCnf)
SIG_DEF( SIG_APEX_MUX_IRDA_TIMEOUT_IND,       EmptySignal                apexMuxIrDATimeoutInd)
# if defined (ON_PC) && defined (VG_MUX_DTE_PC_ONLY_VERSION)
SIG_DEF( SIG_VGMX_DTE_DEBUG_EVENT_IND,        VgmuxDteDebugEventInd      vgmuxDteDebugEventInd)
SIG_DEF( SIG_VGMX_DTE_DEBUG_RX_DATA_REQ,      VgmuxDteRxDataReq          vgmuxDteRxDataReq)
#endif
#endif

SIG_DEF( SIG_VGMX_RESEND_LOW_POWER_STATE_REQ, EmptySignal                vgmuxResendLowPowerStateReq)

SIG_DEF( SIG_CI_CFUN_SET_IND,                 CiCfunSetInd               ciCfunSetInd)
#if defined(UPGRADE_MTNET)
SIG_DEF( SIG_CI_RESET_IND,                    CiResetInd                 ciResetInd)
#endif
#if defined (ENABLE_L23_DEBUG)
SIG_DEF( SIG_CI_DEBUG_IND,                    CiDebugInd                 ciDebugInd)
#endif

#if defined (ENABLE_ATCI_UNIT_TEST)
SIG_DEF( SIG_CI_UNIT_TEST_SLEEP_MODE_IND,  CiUnitTestSleepModeInd  ciUnitTestSleepModeInd)
SIG_DEF( SIG_CI_UNIT_TEST_SLEEP_MODE_RSP,  CiUnitTestSleepModeRsp  ciUnitTestSleepModeRsp)
#endif
/* END OF FILE */

