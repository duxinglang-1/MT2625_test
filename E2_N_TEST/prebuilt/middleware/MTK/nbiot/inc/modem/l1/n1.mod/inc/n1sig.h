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
 * Signal Id to structure mapping for N1 signals.
 */

#if !defined (EXCLUDE_N1CD)
    /* N1CD Signals. */
    SIG_DEF( SIG_N1CD_DUMMY = N1CD_SIGNAL_BASE,     EmptySignal                     n1CdDummy)
    SIG_DEF( SIG_N1CD_ENTER_REQ,                    N1CdEnterReq                    n1CdEnterReq)
    SIG_DEF( SIG_N1CD_ENTER_CNF,                    N1CdEnterCnf                    n1CdEnterCnf)
    SIG_DEF( SIG_N1CD_EXIT_REQ,                     N1CdExitReq                     n1CdExitReq)
    SIG_DEF( SIG_N1CD_EXIT_CNF,                     N1CdExitCnf                     n1CdExitCnf) 
#ifdef FEA_N1_TEST_SQ_ENABLE
    SIG_DEF( SIG_N1CD_SEQ_TEST_REQ,                 N1CdSeqTestReq                  n1CdSeqTestReq)
    SIG_DEF( SIG_N1CD_SEQ_TEST_CNF,                 N1CdSeqTestCnf                  n1CdSeqTestCnf)                                                                              
    SIG_DEF( SIG_N1CD_SEQ_TEST_TXM_REQ,             N1CdSeqTestTxmReq               n1CdSeqTestTxmReq)
    SIG_DEF( SIG_N1CD_SEQ_TEST_TXM_CNF,             N1CdSeqTestTxmCnf               n1CdSeqTestTxmCnf)
    SIG_DEF( SIG_N1CD_FSE_TEST_REQ,                 N1CdFseTestReq                  n1CdFseTestReq)
    SIG_DEF( SIG_N1CD_FSE_TEST_CNF,                 N1CdFseTestCnf                  n1CdFseTestCnf)
#endif
    SIG_DEF( SIG_N1CD_NRF_TEST_REQ,                 N1CdRfTestReq                   n1CdRfTestReq)
    SIG_DEF( SIG_N1CD_NRF_TEST_CNF,                 N1CdRfTestCnf                   n1CdRfTestCnf)
    SIG_DEF( SIG_N1CD_IDC_TEST_REQ,                 N1CdIdcTestReq                  n1CdIdcTestReq)
    SIG_DEF( SIG_N1CD_IDC_TEST_CNF,                 N1CdIdcTestCnf                  n1CdIdcTestCnf)
#endif

#if !defined (EXCLUDE_N1TST)
    /* N1TST Signals. */
    SIG_DEF( SIG_N1TST_DUMMY = N1TST_SIGNAL_BASE,   EmptySignal                     n1TstDummy)
    SIG_DEF( SIG_N1TST_STATE_CHANGE_IND,            N1TstStateChangeInd             n1TstStateChangeInd)
    SIG_DEF( SIG_N1TST_ALG_TUNING_SET_REQ,          N1TstAlgTuningSetReq            n1TstAlgTuningSetReq)
#endif /* !EXCLUDE_N1TST */

/* END OF FILE */