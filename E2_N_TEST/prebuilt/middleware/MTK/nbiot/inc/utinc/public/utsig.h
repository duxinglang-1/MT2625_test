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
 * File Description: GKI signal definitions for utilities.
 **************************************************************************/

    SIG_DEF( SIG_UT_LOG_DUMMY = UT_SIGNAL_BASE,    EmptySignal                 ut_dummy)
#if !defined (EXCLUDE_UT_LOGGER)
    SIG_DEF( SIG_UT_LOG_ERROR_REQ,                 UtLogErrorReq               utLogErrorReq)
    SIG_DEF( SIG_UT_LOG_PDU_IND,                   UtLogPduInd                 utLogPduInd)
    /* Logging signal from the Extended Block Memory Manager (EBMM) to the application layer. */
    SIG_DEF( SIG_EBMM_LOG_ACTION_IND,              EbmmLogActionInd            ebmmLogActionInd)
#else
    SIG_DEF( SIG_UT_LOG_ERROR_REQ,                 EmptySignal                 utLogErrorReqNotPresent)
    SIG_DEF( SIG_UT_LOG_PDU_IND,                   EmptySignal                 utLogPduIndNotPresent)
    SIG_DEF( SIG_EBMM_LOG_ACTION_IND,              EmptySignal                 ebmmLogActionIndNotPresent)
#endif /* !defined (EXCLUDE_UT_LOGGER) */
    SIG_DEF( SIG_UT_FLEXIBLE_TRACE_CONTROL_REQ,    UtFlexibleTraceControlReq   utFlexibleTraceControlReq)
    SIG_DEF( SIG_UT_FLEXIBLE_TRACE_OUT,            UtFlexibleTraceOut          utFlexibleTraceOut)
    SIG_DEF( SIG_UT_FLEXIBLE_TRACE_TA_CONTROL_REQ, UtFlexibleTraceTaControlReq utFlexibleTraceTaControlReq)
    SIG_DEF( SIG_UT_FLEXIBLE_TRACE_TA_OUT,         UtFlexibleTraceTaOut        utFlexibleTraceTaOut)
    SIG_DEF( SIG_UT_FLEXIBLE_TRACE_TASK_INFO,      UtFlexibleTraceTaskInfo     utFlexibleTraceTaskInfo)

#if defined (UPGRADE_SYS_TOOLS)
# if defined (UT_ML_USE_MINILOGGER)
    SIG_DEF( SIG_ML_EVENT_RETRIEVAL_IND,           MiniLoggerEvent             mlEventRetrievalInd)
# else /* UT_ML_USE_MINILOGGER */
    SIG_DEF( SIG_ML_EVENT_RETRIEVAL_IND,           EmptySignal                 mlEventRetrievalIndNotPresent)
# endif /* UT_ML_USE_MINILOGGER */
#else /* UPGRADE_SYS_TOOLS */
    SIG_DEF( SIG_ML_EVENT_RETRIEVAL_IND,           EmptySignal                 mlEventRetrievalIndNotPresent)
#endif /* UPGRADE_SYS_TOOLS */

    SIG_DEF( SIG_UT_TMM_DUMMY = UTTMM_SIGNAL_BASE, EmptySignal                 dummyUt)
#if !defined (EXCLUDE_TMM)
    SIG_DEF( SIG_UT_MEM_ABOVE_HWM_IND,             UtMemAboveHwmInd            utMemAboveHwmInd)
    SIG_DEF( SIG_UT_MEM_BELOW_LWM_IND,             UtMemBelowLwmInd            utMemBelowLwmInd)
    SIG_DEF( SIG_UT_MEM_OP_DEBUG_IND,              UtMemOpDebugInd             utMemOpDebugInd)
#else /* EXCLUDE_TMM */
    SIG_DEF( SIG_UT_MEM_ABOVE_HWM_IND,             EmptySignal                 utMemAboveHwmInd)
    SIG_DEF( SIG_UT_MEM_BELOW_LWM_IND,             EmptySignal                 utMemBelowLwmInd)
    SIG_DEF( SIG_UT_MEM_OP_DEBUG_IND,              EmptySignal                 utMemOpDebugInd)
#endif /* EXCLUDE_TMM */

#if defined (ENABLE_UCIPHER_DEV_INTEGRATION)
    SIG_DEF( SIG_UT_UCIPHER_DEVINT_F8_SUCCESS_IND, UtUcipherDevIntF8SuccessInd utUcipherDevIntF8SuccessInd)
    SIG_DEF( SIG_UT_UCIPHER_DEVINT_F8_FAIL_IND,    UtUcipherDevIntF8FailInd    utUcipherDevIntF8FailInd)
    SIG_DEF( SIG_UT_UCIPHER_DEVINT_F9_SUCCESS_IND, UtUcipherDevIntF9SuccessInd utUcipherDevIntF9SuccessInd)
    SIG_DEF( SIG_UT_UCIPHER_DEVINT_F9_FAIL_IND,    UtUcipherDevIntF9FailInd    utUcipherDevIntF9FailInd)
#endif /* ENABLE_UCIPHER_DEV_INTEGRATION */
