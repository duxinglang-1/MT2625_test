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
*************************************************************************/
/** \file
 * \ingroup PrdLTECommonCalDev
 * Signal Id to structure mapping for E1 signals.
 */

#if !defined (EXCLUDE_E1CD)
    /* E1CD Signals. */
    SIG_DEF( SIG_E1CD_DUMMY = E1CD_SIGNAL_BASE,     EmptySignal                     e1cdDummy)
    SIG_DEF( SIG_E1CD_ENTER_REQ,                    E1cdEnterReq                    e1cdEnterReq)
    SIG_DEF( SIG_E1CD_ENTER_CNF,                    E1cdEnterCnf                    e1cdEnterCnf)
    SIG_DEF( SIG_E1CD_EXIT_REQ,                     E1cdExitReq                     e1cdExitReq)
    SIG_DEF( SIG_E1CD_EXIT_CNF,                     E1cdExitCnf                     e1cdExitCnf)

    SIG_DEF( SIG_E1CD_PHY_TEST_REQ,                 E1cdPhyTestReq                  e1cdPhyTestReq)
    SIG_DEF( SIG_E1CD_PHY_TEST_CNF,                 E1cdPhyTestCnf                  e1cdPhyTestCnf)
    SIG_DEF( SIG_E1CD_PHY_TEST_IND,                 E1cdPhyTestInd                  e1cdPhyTestInd)


    SIG_DEF(SIG_E1CD_UPLANE_TEST_ON_REQ,            E1cdUplaneTestOnReq             e1cdUplaneTestOnReq)
    SIG_DEF(SIG_E1CD_UPLANE_TEST_ON_CNF,            E1cdUplaneTestOnCnf             e1cdUplaneTestOnCnf)
    SIG_DEF(SIG_E1CD_UPLANE_TEST_OFF_REQ,           E1cdUplaneTestOffReq            e1cdUplaneTestOffReq)
    SIG_DEF(SIG_E1CD_UPLANE_TEST_OFF_CNF,           E1cdUplaneTestOffCnf            e1cdUplaneTestOffCnf)
                                                                                    

#if defined (TARGET_SIM_HW_MODEL)
    SIG_DEF( SIG_E1CD_LTESIM_INIT_REQ,              E1cdLteSimInitReq               e1cdLteSimInitReq)
    SIG_DEF( SIG_E1CD_LTESIM_INIT_CNF,              E1cdLteSimInitCnf               e1cdLteSimInitCnf)
    SIG_DEF( SIG_E1CD_LTESIM_COMPLETE_IND,          E1cdLteSimCompleteInd           e1cdLteSimCompleteInd)
    SIG_DEF( SIG_E1CD_LTESIM_STOP_REQ,              E1cdLteSimStopReq               e1cdLteSimStopReq)
    SIG_DEF( SIG_E1CD_LTESIM_STOP_CNF,              E1cdLteSimStopCnf               e1cdLteSimStopCnf)
#endif
#endif /* !EXCLUDE_E1CD */

#if !defined (EXCLUDE_E1IT)
    /* E1IT Signals. */
    SIG_DEF( SIG_E1IT_DUMMY = E1IT_SIGNAL_BASE,     EmptySignal                     e1itDummy)
    SIG_DEF( SIG_E1IT_READ_SMQU_IND,                EmptySignal                     e1itReadSmquInd)

#if defined (DEVELOPMENT_VERSION)
    SIG_DEF( SIG_E1IT_STATE_CHANGE_IND,             E1itStateChangeInd              e1itStateChangeInd)
#endif

#endif /* !EXCLUDE_E1IT */

#if !defined (EXCLUDE_E1TEST) && defined (DEVELOPMENT_VERSION)
    /* E1TEST Signals. */
    SIG_DEF( SIG_E1TEST_DUMMY = E1TEST_SIGNAL_BASE, EmptySignal                     e1testDummy)
    SIG_DEF( SIG_E1TEST_TIME_ALIGN_IND,             E1testTimeAlignInd              e1testTimeAlignInd)
    SIG_DEF( SIG_E1TEST_PBCH_DECODE_IND,            E1testPbchDecodeInd             e1testPbchDecodeInd)

#endif /* !EXCLUDE_E1TEST && DEVELOPMENT_VERSION */

#if defined(EL1_TEST_PLATFORM)
    SIG_DEF(SIG_E1TEST_RACH_DEBUG_REQ,              E1testRachDebugReq              e1testRachDebugReq)
    SIG_DEF(SIG_E1TEST_RACH_DEBUG_IND,              E1testRachDebugInd              e1testRachDebugInd)
    SIG_DEF(SIG_E1TEST_PWC_DEBUG_INFO_IND,          E1testPwcDebugInfoInd           e1testPwcDebugInfoInd)
    SIG_DEF(SIG_E1TEST_AGC_DEBUG_INFO_IND,          E1testAgcDebugInfoInd           e1testAgcDebugInfoInd)
    SIG_DEF(SIG_E1TEST_AFC_DEBUG_INFO_IND,          E1testAfcDebugInfoInd           e1testAfcDebugInfoInd)
    SIG_DEF(SIG_E1TEST_RX_PATH_DEBUG_INFO_IND,      E1TestRxpathDebugInfoInd        e1testRxpathDebugInfoInd)
    SIG_DEF(SIG_E1TEST_PHICH_DEBUG_INFO_IND,        E1TestPhichDebugInfoInd         e1PhichDebugInfoInd)
    SIG_DEF( SIG_E1TEST_CFG_REQ,                    E1testCfgReq                    e1testCfgReq)
    SIG_DEF( SIG_E1TEST_DRX_DEBUG_IND,              E1testDrxDebugInd               e1testDrxDebugInd)
    SIG_DEF (SIG_E1TEST_TA_IND,                     E1testTaInd                     e1testTaInd)
    SIG_DEF(SIG_E1TEST_CQI_IND,                     E1testCqiInd                    e1testCqiInd)    
    SIG_DEF(SIG_E1TEST_SRS_IND,                     E1testSrsInd                    e1testSrsInd) 
    SIG_DEF(SIG_E1TEST_AFC_DAC_ADJUST_REQ,          E1testAfcDacAdjustReq           e1testAfcDacAdjustReq)
#endif

/* END OF FILE */
