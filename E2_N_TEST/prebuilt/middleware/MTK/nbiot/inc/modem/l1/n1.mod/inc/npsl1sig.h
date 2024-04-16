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
*******************************************************************************/
/** \file
 * NB-IoT L1 Signal Id to Signal structure mapping for shared signal
 *                   interfaces between NB-IoT PS and NB-IoT L1
 ******************************************************************************/

#include "nbiot_modem_common_config.h"

#if defined (UPGRADE_NB)

# if !defined (EXCLUDE_NPHY)
SIG_DEF( SIG_NPHY_DUMMY = NPHY_SIGNAL_BASE,     EmptySignal                   nphyDummy )
SIG_DEF( SIG_NPHY_DEACTIVATE_REQ,               NphyDeactivateReq             nphyDeactivateReq )
SIG_DEF( SIG_NPHY_DEACTIVATE_CNF,               NphyDeactivateCnf             nphyDeactivateCnf )
SIG_DEF( SIG_NPHY_CELL_SEARCH_REQ,              NphyCellSearchReq             nphyCellSearchReq )
SIG_DEF( SIG_NPHY_CELL_SEARCH_CNF,              NphyCellSearchCnf             nphyCellSearchCnf )
SIG_DEF( SIG_NPHY_CELL_SEARCH_IND,              NphyCellSearchInd             nphyCellSearchInd )
SIG_DEF( SIG_NPHY_CELL_SEARCH_STOP_REQ,         NphyCellSearchStopReq         nphyCellSearchStopReq )
SIG_DEF( SIG_NPHY_CELL_SEARCH_MEAS_IND,         NphyCellSearchMeasInd         nphyCellSearchMeasInd )
SIG_DEF( SIG_NPHY_NEXT_FREQ_REQ,                NphyNextFreqReq               nphyNextFreqReq )
SIG_DEF( SIG_NPHY_NEXT_CELL_REQ,                NphyNextCellReq               nphyNextCellReq )
SIG_DEF( SIG_NPHY_CELL_SELECT_REQ,              NphyCellSelectReq             nphyCellSelectReq )
SIG_DEF( SIG_NPHY_CELL_SELECT_CNF,              NphyCellSelectCnf             nphyCellSelectCnf )
SIG_DEF( SIG_NPHY_PLMN_SEARCH_REQ,              NphyPlmnSearchReq             nphyPlmnSearchReq )
SIG_DEF( SIG_NPHY_PLMN_SEARCH_IND,              NphyPlmnSearchInd             nphyPlmnSearchInd )
SIG_DEF( SIG_NPHY_PLMN_SEARCH_STOP_REQ,         NphyPlmnSearchStopReq         nphyPlmnSearchStopReq )
SIG_DEF( SIG_NPHY_PLMN_SEARCH_CNF,              NphyPlmnSearchCnf             nphyPlmnSearchCnf )
SIG_DEF( SIG_NPHY_BCCH_REQ,                     NphyBcchReq                   nphyBcchReq )
SIG_DEF( SIG_NPHY_BCCH_CNF,                     NphyBcchCnf                   nphyBcchCnf )
SIG_DEF( SIG_NPHY_BCCH_DATA_IND,                NphyBcchDataInd               nphyBcchDataInd )
SIG_DEF( SIG_NPHY_BCCH_STOP_REQ,                NphyBcchStopReq               nphyBcchStopReq )
SIG_DEF( SIG_NPHY_IDLE_CONFIG_REQ,              NphyIdleConfigReq             nphyIdleConfigReq )
SIG_DEF( SIG_NPHY_IDLE_CONFIG_CNF,              NphyIdleConfigCnf             nphyIdleConfigCnf )
SIG_DEF( SIG_NPHY_IDLE_CONFIG_NEEDED_IND,       NphyIdleConfigNeededInd       nphyIdleConfigNeededInd )
SIG_DEF( SIG_NPHY_CONNECTED_CONFIG_REQ,         NphyConnectedConfigReq        nphyConnectedConfigReq )
SIG_DEF( SIG_NPHY_CONNECTED_CONFIG_CNF,         NphyConnectedConfigCnf        nphyConnectedConfigCnf )
SIG_DEF( SIG_NPHY_MEASUREMENT_CONFIG_REQ,       NphyMeasurementConfigReq      nphyMeasurementConfigReq )
SIG_DEF( SIG_NPHY_MEASUREMENT_IND,              NphyMeasurementInd            nphyMeasurementInd )
SIG_DEF( SIG_NPHY_SIGNAL_LEVEL_IND,             NphySignalLevelInd            nphySignalLevelInd )
SIG_DEF( SIG_NPHY_LPP_ECID_MEAS_REQ,            NphyLppEcidMeasReq            nphyLppEcidMeasReq )
SIG_DEF( SIG_NPHY_LPP_ECID_MEAS_CNF,            NphyLppEcidMeasCnf            nphyLppEcidMeasCnf )
SIG_DEF( SIG_NPHY_LPP_ECID_MEAS_ABORT_REQ,      NphyLppEcidMeasAbortReq       nphyLppEcidMeasAbortReq )
SIG_DEF(SIG_NPHY_LPP_OTDOA_ASSIST_DATA_REQ,     NphyLppOtdoaAssistDataReq     nphyLppOtdoaAssistDataReq )
SIG_DEF(SIG_NPHY_LPP_OTDOA_ASSIST_DATA_CNF,     NphyLppOtdoaAssistDataCnf     nphyLppOtdoaAssistDataCnf )
SIG_DEF(SIG_NPHY_LPP_OTDOA_ADDITIONAL_ASSIST_DATA_IND, NphyLppOtdoaAdditionalAssistDataInd nphyLppOtdoaAdditionalAssistDataInd )
SIG_DEF(SIG_NPHY_LPP_OTDOA_MEAS_REQ,            NphyLppOtdoaMeasReq            nphyLppOtdoaMeasReq )
SIG_DEF(SIG_NPHY_LPP_OTDOA_MEAS_CNF,            NphyLppOtdoaMeasCnf            nphyLppOtdoaMeasCnf )
SIG_DEF(SIG_NPHY_LPP_OTDOA_MEAS_ABORT_REQ,      NphyLppOtdoaMeasAbortReq       nphyLppOtdoaMeasAbortReq )
SIG_DEF( SIG_NPHY_MAC_RESET_REQ,                NphyMacResetReq               nphyMacResetReq )
SIG_DEF( SIG_NPHY_CCCH_RA_READY_IND,            NphyCcchRaReadyInd            nphyCcchRaReadyInd )
SIG_DEF( SIG_NPHY_CCCH_RA_FAIL_IND,             NphyCcchRaFailInd             nphyCcchRaFailInd )
SIG_DEF( SIG_NPHY_PCCH_DATA_IND,                NphyPcchDataInd               nphyPcchDataInd )
SIG_DEF( SIG_NPHY_DATA_IND,                     NphyDataInd                   nphyDataInd )
SIG_DEF( SIG_NPHY_TRANSMIT_IND,                 NphyTransmitInd               nphyTransmitInd )
#ifdef FEA_TEMP_NPHY_TRANSMIT_RSP
SIG_DEF( SIG_NPHY_TRANSMIT_RSP,                 NphyTransmitRsp               nphyTransmitRsp )
SIG_DEF( SIG_NPHY_SCHEDULING_REQUEST_REQ,       NphySchedulingRequestReq      nphySchedulingRequestReq )
#else /* FEA_TEMP_NPHY_TRANSMIT_RSP */
SIG_DEF( SIG_NPHY_RANDOM_ACCESS_REQ,            NphyRandomAccessReq           nphyRandomAccessReq )
#endif /* FEA_TEMP_NPHY_TRANSMIT_RSP */
SIG_DEF( SIG_NPHY_MAC_RESET_IND,                NphyMacResetInd               nphyMacResetInd )
SIG_DEF( SIG_NPHY_TRANSMIT_DONE_INDICATE_REQ,   NphyTransmitDoneIndicateReq   nphyTransmitDoneIndicateReq )
SIG_DEF( SIG_NPHY_TRANSMIT_DONE_IND,            NphyTransmitDoneInd           nphyTransmitDoneInd )
SIG_DEF( SIG_NPHY_L1_TEST_MODE_ACTIVATE_REQ,    NphyL1TestModeActivateReq     nphyL1TestModeActivateReq)
SIG_DEF( SIG_NPHY_RLF_IND,                      NphyRlfInd                    nphyRlfInd )
SIG_DEF( SIG_NPHY_DIRECT_INDICATION_IND,        NphyDirectIndicationInd       nphyDirectIndicationInd )
SIG_DEF( SIG_NPHY_BCCH_MODIFICATION_PERIOD_IND, NphyBcchModificationPeriodInd nphyBcchModificationPeriodInd )
SIG_DEF( SIG_NPHY_EDRX_ACQUISITION_PERIOD_INDICATE_REQ, NphyEdrxAcquisitionPeriodIndicateReq nphyEdrxAcquisitionPeriodIndicateReq )
SIG_DEF( SIG_NPHY_EDRX_ACQUISITION_PERIOD_IND,  NphyEdrxAcquisitionPeriodInd  nphyEdrxAcquisitionPeriodInd )
SIG_DEF( SIG_NPHY_INFO_REQ,                     NphyInfoReq                   nphyInfoReq )
SIG_DEF( SIG_NPHY_INFO_CNF,                     NphyInfoCnf                   nphyInfoCnf )
SIG_DEF( SIG_NPHY_RA_PREAMBLE_START_IND,        NphyRaPreambleStartInd        nphyRaPreambleStartInd )
#ifdef FEA_SIMAT_TA
SIG_DEF( SIG_NPHY_SIMAT_TA_INFO_REQ,            NphySimatTaInfoReq            nphySimatTaInfoReq )
SIG_DEF( SIG_NPHY_SIMAT_TA_INFO_CNF,            NphySimatTaInfoCnf            nphySimatTaInfoCnf )
#endif
#ifdef SR_DELAY_PRACH_CONFILCT_WORKAROUND
SIG_DEF( SIG_NPHY_SR_DELAY_TIME_INFO_IND,       NphySrDelayTimeInfoInd        nphySrDelayTimeInfoInd )
#endif
SIG_DEF( SIG_NPHY_HIGH_SPEED_CONFIG_REQ,        NphyHighSpeedConfigReq        nphyHighSpeedConfigReq )
SIG_DEF( SIG_NPHY_MAC_RESET_CNF,                NphyMacResetCnf               nphyMacResetCnf )
SIG_DEF( SIG_NPHY_ENTER_CONN_STATE_IND,         NphyEnterConnStateInd         nphyEnterConnStateInd )
#endif /* EXCLUDE_NCPHY */

#endif
/* END OF FILE */
