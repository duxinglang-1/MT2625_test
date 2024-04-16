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
 **************************************************************************/
/** \file
 * Signal definitions for interfaces with the MUX task.
 **************************************************************************/

/* CIMUX interface */
SIG_DEF( SIG_CIMUX_AT_DATA_CNF,                     CiMuxAtDataCnf                  ciMuxAtDataCnf             )
SIG_DEF( SIG_CIMUX_AT_DATA_IND,                     CiMuxAtDataInd                  ciMuxAtDataInd             )
SIG_DEF( SIG_CIMUX_AT_DATA_REQ,                     CiMuxAtDataReq                  ciMuxAtDataReq             )
SIG_DEF( SIG_CIMUX_AT_DATA_RSP,                     CiMuxAtDataRsp                  ciMuxAtDataRsp             )
SIG_DEF( SIG_CIMUX_ATO_CMD_REQ,                     CiMuxAtoCmdReq                  ciMuxAtoCmdReq             )
SIG_DEF( SIG_CIMUX_CHANNEL_DISABLED_IND,            CiMuxChannelDisabledInd         ciMuxChannelDisabledInd    )
#if defined (UPGRADE_NASMDL2)
SIG_DEF( SIG_CIMUX_CHANNEL_DISABLED_RSP,            CiMuxChannelDisabledRsp         ciMuxChannelDisabledRsp    )
#endif
SIG_DEF( SIG_CIMUX_CHANNEL_ENABLE_IND,              CiMuxChannelEnableInd           ciMuxChannelEnableInd      )
SIG_DEF( SIG_CIMUX_CHANNEL_ENABLE_RSP,              CiMuxChannelEnableRsp           ciMuxChannelEnableRsp      )
SIG_DEF( SIG_CIMUX_CHECK_CMUX_CMD_PARAMS_CNF,       CiMuxCheckCmuxCmdParamsCnf      ciMuxCheckCmuxCmdParamsCnf )
SIG_DEF( SIG_CIMUX_CHECK_CMUX_CMD_PARAMS_REQ,       CiMuxCheckCmuxCmdParamsReq      ciMuxCheckCmuxCmdParamsReq )
SIG_DEF( SIG_CIMUX_CLOSE_DATA_CONN_REQ,             CiMuxCloseDataConnReq           ciMuxCloseDataConnReq      )
#if defined (UPGRADE_NASMDL2)
SIG_DEF( SIG_CIMUX_CLOSED_DATA_CONN_IND,            CiMuxClosedDataConnInd          ciMuxClosedDataConnInd     )
#endif
SIG_DEF( SIG_CIMUX_CONFIGURE_CHANNEL_REQ,           CiMuxConfigureChannelReq        ciMuxConfigureChannelReq   )
SIG_DEF( SIG_CIMUX_CONFIGURE_MUX_CNF,               CiMuxConfigureMuxCnf            ciMuxConfigureMuxCnf       )
SIG_DEF( SIG_CIMUX_CONFIGURE_MUX_REQ,               CiMuxConfigureMuxReq            ciMuxConfigureMuxReq       )
SIG_DEF( SIG_CIMUX_OPEN_DATA_CONN_CNF,              CiMuxOpenDataConnCnf            ciMuxOpenDataConnCnf       )
SIG_DEF( SIG_CIMUX_OPEN_DATA_CONN_REQ,              CiMuxOpenDataConnReq            ciMuxOpenDataConnReq       )
SIG_DEF( SIG_CIMUX_READ_CMUX_CMD_PARAMS_CNF,        CiMuxReadCmuxCmdParamsCnf       ciMuxReadCmuxCmdParamsCnf  )
SIG_DEF( SIG_CIMUX_READ_CMUX_CMD_PARAMS_REQ,        CiMuxReadCmuxCmdParamsReq       ciMuxReadCmuxCmdParamsReq  )
SIG_DEF( SIG_CIMUX_RING_INDICATOR_OFF_REQ,          CiMuxRingIndicatorOffReq        ciMuxRingIndicatorOffReq   )
SIG_DEF( SIG_CIMUX_RING_INDICATOR_ON_REQ,           CiMuxRingIndicatorOnReq         ciMuxRingIndicatorOnReq    )
SIG_DEF( SIG_CIMUX_START_27010MUX_REQ,              CiMuxStart27010MuxReq           ciMuxStart27010MuxReq      )
SIG_DEF( SIG_CIMUX_SWITCHED_TO_CMD_MODE_IND,        CiMuxSwitchedToCmdModeInd       ciMuxSwitchedToCmdModeInd  )
SIG_DEF( SIG_CIMUX_VERSION_CNF,                     CiMuxVersionCnf                 ciMuxVersionCnf            )
SIG_DEF( SIG_CIMUX_VERSION_REQ,                     CiMuxVersionReq                 ciMuxVersionReq            )
#if !defined (UPGRADE_NASMDL2)
SIG_DEF( SIG_CIMUX_RESERVE_CHANNEL_REQ,             EmptySignal                     ciMuxReserveChannelReq     )
SIG_DEF( SIG_CIMUX_RESERVE_CHANNEL_CNF,             CiMuxReserveChannelCnf          ciMuxReserveChannelCnf     )
SIG_DEF( SIG_CIMUX_FREE_RESERVED_CHANNEL_REQ,       CiMuxFreeReservedChannelReq     ciMuxFreeReservedChannelReq)
SIG_DEF( SIG_CIMUX_FREE_RESERVED_CHANNEL_CNF,       CiMuxFreeReservedChannelCnf     ciMuxFreeReservedChannelCnf)
#endif

/* CSDMUX interface */
SIG_DEF( SIG_CSDMUX_DL_64K_DATA_REQ,                CsdMuxDl64KDataReq              csdMuxDl64KDataReq )
SIG_DEF( SIG_CSDMUX_UL_64K_DATA_IND,                CsdMuxUl64KDataInd              csdMuxUl64KDataInd )
SIG_DEF( SIG_RTDS_RX_XON_REQ,                       EmptySignal                     rtdsRxXonReq       )
SIG_DEF( SIG_RTDS_TX_DATA_IND,                      EmptySignal                     rtdsTxDataInd      )
SIG_DEF( SIG_UFDS_RX_XON_REQ,                       EmptySignal                     ufdsRxXonReq       )
SIG_DEF( SIG_UFDS_TX_DATA_IND,                      EmptySignal                     ufdsTxDataInd      )

#if defined (FEA_PPP)
/* PPPMUX interface */
SIG_DEF( SIG_PPPMUX_CONFIG_REQ,                     PppMuxConfigReq                 pppMuxConfigReq )
#endif /* FEA_PPP */

/* MUXMUX interface */
SIG_DEF( SIG_MUXMUX_TX_DATA_READY_REQ,              MuxMuxTxDataReadyReq            muxMuxTxDataReadyReq )

/* MUX debug signals */
SIG_DEF( SIG_MUXDEBUG_ERROR_IND,                    MuxDebugErrorInd                muxDebugErrorInd        )
SIG_DEF( SIG_MUXDEBUG_DISCARD_IND,                  MuxDebugDiscardInd              muxDebugDiscardInd      )
SIG_DEF( SIG_MUXDEBUG_SEGMENTATION_IND,             MuxDebugSegmentationInd         muxDebugSegmentationInd )
SIG_DEF( SIG_MUXDEBUG_REASSEMBLY_IND,               MuxDebugReassemblyInd           muxDebugReassemblyInd   )

#if defined (EMMI_OVER_GENERIC_MUX)
/* EMMIMUX interface */
SIG_DEF( SIG_EMMIMUX_START_IND,                     EmptySignal                     emmiMuxStartInd  )
SIG_DEF( SIG_EMMIMUX_RX_DATA_IND,                   EmmiMuxRxDataInd                emmiMuxRxDataInd )
SIG_DEF( SIG_EMMIMUX_TX_DATA_CNF,                   EmptySignal                     emmiMuxTxDataCnf )
SIG_DEF( SIG_EMMIMUX_TX_DATA_REQ,                   EmmiMuxTxDataReq                emmiMuxTxDataReq )
#endif

#if defined (FEA_PPP)
#if !defined (UPGRADE_NASMDL2)
/* PPPAD statistics for Genie pass-thru task */
SIG_DEF( SIG_PPPASTATS_IND,                         PppaStatsInd                    pppaStatsInd )
#endif /* !UPGRADE_NASMDL2 */

/* MUXPPPAD signals for non-blocking PPPAD */
SIG_DEF( SIG_MUXPPPAD_DL_DATA_REQ,                  MuxPppadDlDataReq               muxPppadDlDataReq    )
SIG_DEF( SIG_MUXPPPAD_UL_DATA_REQ,                  MuxPppadUlDataReq               muxPppadUlDataReq    )
SIG_DEF( SIG_MUXPPPAD_UL_POOL_LWM_REQ,              MuxPppadUlPoolLwmReq            muxPppadUlPoolLwmReq )
#endif /* FEA_PPP */
SIG_DEF( SIG_MUXDEBUG_27010_TX_MODEM_STATUS_CMD,    MuxDebug27010TxModemStatusCmd   muxDebug27010TxModemStatusCmd   )
SIG_DEF( SIG_MUXDEBUG_27010_RX_MODEM_STATUS_CMD,    MuxDebug27010RxModemStatusCmd   muxDebug27010RxModemStatusCmd   )

#if defined (UPGRADE_MUX_RETENTION_UT)
SIG_DEF( SIG_MUX_PREPARE_FOR_DEEP_SLEEP_IND,        EmptySignal                     muxPrepareForDeepSleepInd  )
#endif /* UPGRADE_MUX_RETENTION_UT */
#if defined (UPGRADE_CSCI_MUX)
#if defined (UPGRADE_CSCI_MUX_UT)
/* MUX CSCI emulation signals*/
SIG_DEF( SIG_MUXCSCI_TEST_SIGNAL_REQ,               MuxCsciTestSignalReq            muxCsciTestSignalReq )
SIG_DEF( SIG_MUXCSCI_TEST_SIGNAL_IND,               MuxCsciTestSignalInd            muxCsciTestSignalInd )
#endif /* UPGRADE_CSCI_MUX_UT */
SIG_DEF( SIG_MUXCSCI_READY_TO_READ_IND,             EmptySignal                     muxCsciReadyToReadInd )
SIG_DEF( SIG_MUXCSCI_READY_TO_WRITE_IND,            EmptySignal                     muxCsciReadyToWriteInd )
#endif /* UPGRADE_CSCI_MUX */
SIG_DEF( SIG_MUX_L4MM_FLOW_CONTROL_IND,             MuxL4mmFlowControlInd           muxL4mmFlowControlInd )
SIG_DEF( SIG_CIMUX_AT_READY_IND,                    EmptySignal                     ciMuxAtReadyInd )

/* END OF FILE */

