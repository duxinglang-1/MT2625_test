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
 *  File Description :
 *      SIM signal definitions
 *
 ****************************************************************************
 * Please, please, please do not make any additions to this file wrapped in
 * #if defined() #endif.  New signals should still be added next to the most
 * appropriate group as before.
 **************************************************************************/

#if !defined (EXCLUDE_L1SI)
    /*
    ** SIM signals to/from layer 1
    */
    SIG_DEF( SIG_L1SI_DUMMY = L1SI_SIGNAL_BASE,    EmptySignal                 l1si_dummy)
    SIG_DEF( SIG_L1SI_DATA_REQ,                    L1siDataReq                 l1siDataReq)
    SIG_DEF( SIG_L1SI_CONFIG_REQ,                  L1siConfigReq               l1siConfigReq)
    SIG_DEF( SIG_L1SI_RESET_REQ,                   L1siResetReq                l1siResetReq)
    SIG_DEF( SIG_L1SI_REJECT_REQ,                  L1siRejectReq               l1siRejectReq)
    SIG_DEF( SIG_L1SI_IDLE_REQ,                    L1siIdleReq                 l1siIdleReq)
    SIG_DEF( SIG_L1SI_DATA_IND,                    L1siDataInd                 l1siDataInd)
    SIG_DEF( SIG_L1SI_CARD_PRESENT_IND,            L1siCardPresentInd          l1siCardPresentInd)
    SIG_DEF( SIG_L1SI_CARD_REMOVED_IND,            L1siCardRemovedInd          l1siCardRemovedInd)
    SIG_DEF( SIG_L1SI_ERROR_IND,                   L1siErrorInd                l1siErrorInd)
    SIG_DEF( SIG_L1SI_INTERRUPT_REQ,               L1siInterruptReq            l1siInterruptReq)
#if defined (ENABLE_DUAL_SIM_SOLUTION)
    SIG_DEF( SIG_L1SI_SIM_SELECT_REQ,              L1siSimSelectReq            l1siSimSelectReq)
    SIG_DEF( SIG_L1SI_SIM_SELECT_CNF,              L1siSimSelectCnf            l1siSimSelectCnf)
    SIG_DEF( SIG_L1SI_SHUTDOWN_REQ,                L1siShutdownReq             l1siShutdownReq)
#else
    SIG_DEF( SIG_L1SI_CONFIG_IND,                  L1siConfigInd               l1siConfigInd)
#endif
    SIG_DEF( SIG_L1SI_SMALL_DATA_REQ,              L1siSmallDataReq            l1siSmallDataReq)
    SIG_DEF( SIG_L1SI_MEDIUM_DATA_REQ,             L1siMediumDataReq           l1siMediumDataReq)
    SIG_DEF( SIG_L1SI_LARGE_DATA_REQ,              L1siLargeDataReq            l1siLargeDataReq)
    SIG_DEF( SIG_L1SI_SMALL_DATA_IND,              L1siSmallDataInd            l1siSmallDataInd)
    SIG_DEF( SIG_L1SI_MEDIUM_DATA_IND,             L1siMediumDataInd           l1siMediumDataInd)
    SIG_DEF( SIG_L1SI_LARGE_DATA_IND,              L1siLargeDataInd            l1siLargeDataInd)
    SIG_DEF( SIG_L1SI_STOP_TIMER_REQ,              L1siStopTimerReq            l1siStopTimerReq)
    SIG_DEF( SIG_L1SI_INTERFACE_CHECK_REQ,         L1siInterfaceCheckReq       l1siInterfaceCheckReq)
    SIG_DEF( SIG_L1SI_INTERFACE_CHECK_CNF,         L1siInterfaceCheckCnf       l1siInterfaceCheckCnf)

#if defined ( ENABLE_USIM_EMU_COOLART_SCRIPTS)
    SIG_DEF( SIG_L1SI_SIM_IMAG_PTR_IND,            L1siSimImagPtrInd           l1siSimImagPtrInd)
    SIG_DEF( SIG_L1SI_SIM_IMAG_PTR_RSP,            L1siSimImagPtrRsp           l1siSimImagPtrRsp)
    SIG_DEF( SIG_L1SI_SCRIPT_READY_IND,            L1siScriptReadyInd          l1siScriptReadyInd)
#endif/*ENABLE_USIM_EMU_COOLART_SCRIPTS*/


#if defined (ENABLE_SIM_SOCKET_SELECT)
    SIG_DEF( SIG_L1SI_SOCKET_SELECT,            L1siSimSocketSelect          l1siSimSocketSelect)
#endif  /*  ENABLE_SIM_SOCKET_SELECT */
    SIG_DEF( SIG_L1SI_REJECTED_IND,                EmptySignal                 l1siRejectedInd)


#endif
/* END OF FILE */
