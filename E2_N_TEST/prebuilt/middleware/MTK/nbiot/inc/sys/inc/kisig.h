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
 * GKI header file that defines the signals in the SYS signal base.
 **************************************************************************/

#if !defined (EXCLUDE_SYS)
/*
** system signals

 IMPORTANT: The coolart engine has dependencies on the signals below up to a repeat of this notice. 
 If any are changed/added/removed, then need to rebuild MCTask.exe and SWTask.exe 
 before running the unit tests. 
*/

SIG_DEF (SIG_SYS_DUMMY = SYS_SIGNAL_BASE,      EmptySignal sysDummy)

SIG_DEF (SIG_TIMER_EXPIRY,                     KiTimerExpiry timerExpiry)
SIG_DEF (SIG_START_TIMER,                      KiTimerStart kiTimerStart)
SIG_DEF (SIG_STOP_TIMER,                       KiTimerStop kiTimerStop)

SIG_DEF (SIG_INITIALISE,                       KiInitialiseTask initialise)
SIG_DEF (SIG_INITIALISE_CNF,                   KiInitialiseTask kiSigInitialiseCnf)
/* The new out of logging memory signal has the same
** id as the old SIG_OUT_OF_MEMORY. */
SIG_DEF (SIG_KI_LOGGING_MEM_FULL_IND,          KiLoggingMemFullInd kiLoggingMemFullInd)
SIG_DEF (SIG_TEST_FILE_OUT,                    TestFileOut testFileOut)
SIG_DEF (SIG_SYSLOG_TEST_FILE_OUT,             TestFileOut sysLogTestFileOut)

SIG_DEF (SIG_END_OF_QUEUE,                     EmptySignal endOfQueue)

SIG_DEF (SIG_KI_SETS_FILTER_MATRIX,            KiSetsFilterMatrix kiSetsFilterMatrix)

SIG_DEF (SIG_KI_DEV_FAIL_IND,                  KiDevFailInd kiDevFailInd)
SIG_DEF (SIG_KI_DEV_ASSERT_IND,                KiDevAssertInd kiDevAssertInd)
SIG_DEF (SIG_KI_DEV_CHECK_IND,                 KiDevCheckInd kiDevCheckInd)

SIG_DEF (SIG_KI_WARN_FAIL_IND,                  KiWarnFailInd kiWarnFailInd)
SIG_DEF (SIG_KI_WARN_ASSERT_IND,                KiWarnAssertInd kiWarnAssertInd)
SIG_DEF (SIG_KI_WARN_CHECK_IND,                 KiWarnCheckInd kiWarnCheckInd)
SIG_DEF (SIG_KI_WARN_PARAM_IND,                 KiWarnParamInd kiWarnParamInd)
SIG_DEF (SIG_KI_WARN_CONFIG_REQ,                KiWarnConfigReq kiWarnConfigReq)

/** IMPORTANT: The coolart engine has dependencies on the above signals. 
 If any are changed/added/removed, then need to rebuild MCTask.exe and SWTask.exe 
 before running the unit tests. */
 
SIG_DEF (SIG_KI_FIX_STATISTICS_IND,            KiFixStatisticsInd kiFiStatisticsInd)
SIG_DEF (SIG_KI_STATISTICS_IND,                KiStatisticsInd kiStatisticsInd)

SIG_DEF (SIG_EMMI_MESSAGE_IND,                 EmmiMessageInd emmiMessageInd)
SIG_DEF (SIG_EMMI_MESSAGE_REQ,                 EmmiMessageReq emmiMessageReq)

/* Internal EMMI signalling */
SIG_DEF (SIG_KI_REDUNDANT_1,                   EmptySignal kiRedundant1)
SIG_DEF (SIG_KI_REDUNDANT_2,                   EmptySignal kiRedundant2)
SIG_DEF (SIG_EMMI_FREE_TX_BLOCK,               EmptySignal emmiFreeTxBlock)
SIG_DEF (SIG_EMMI_RX_BLOCK_IND,                EmmiRxBlockInd emmiRxBlockInd)

SIG_DEF (SIG_WAIT_END,                         EmptySignal waitEnd)

/* The following 3 ids will become redundant ids in the next release */
SIG_DEF (SIG_NORMAL_FILTER_MATRIX,             EmptySignal kiRedundant4)
SIG_DEF (SIG_COMPACT_FILTER_MATRIX,            EmptySignal kiRedundant5)
SIG_DEF (SIG_TINY_FILTER_MATRIX,               EmptySignal kiRedundant6)

#if defined (KI_ENABLE_LOGGING_SIGNALS_OVER_HSL)
SIG_DEF (SIG_KI_SETS_FILTER_MATRIX_HSL,        KiSetsFilterMatrix kiSetsFilterMatrixHsl)
#endif
SIG_DEF (SIG_KI_GPS_POSITION_IND,              KiGpsPositionInd kiGpsPositionInd)

SIG_DEF (SIG_TST_STATE_CHANGE_IND,             KiStateChangeInd kiStateChangeInd)
SIG_DEF (SIG_KI_DUMP_MEM_REQ,                  KiDumpMemReq kiDumpMemReq)
SIG_DEF (SIG_KI_DUMP_MEM_CNF,                  KiDumpMemCnf kiDumpMemCnf)

SIG_DEF (SIG_KI_FIX_STATISTICS_REQ,            KiFixStatisticsReq kiFixStatisticsReq)
SIG_DEF (SIG_KI_STATISTICS_REQ,                KiStatisticsReq kiStatisticsReq)

SIG_DEF (SIG_KI_MEM_POOL_STATISTICS_REQ,       KiMemPoolStatisticsReq kiMemPoolStatisticsReq)
SIG_DEF (SIG_KI_MEM_POOL_STATISTICS_CNF,       KiMemPoolStatisticsCnf kiMemPoolStatisticsCnf)

SIG_DEF (SIG_KI_DYNAMIC_MEM_USAGE_REQ,         KiDynamicMemUsageReq kiDynamicMemUsageReq)
SIG_DEF (SIG_KI_DYNAMIC_MEM_USAGE_CNF,         KiDynamicMemUsageCnf kiDynamicMemUsageCnf)

SIG_DEF (SIG_KI_MEM_READ_REQ,                  KiMemReadReq kiMemReadReq)
SIG_DEF (SIG_KI_MEM_READ_CNF,                  KiMemReadCnf kiMemReadCnf)
SIG_DEF (SIG_KI_MEM_WRITE_REQ,                 KiMemWriteReq kiMemWriteReq)
SIG_DEF (SIG_KI_MEM_WRITE_CNF,                 KiMemWriteCnf kiMemWriteCnf)
SIG_DEF (SIG_KI_MEM_READ_MULTI_REQ,            KiMemReadMultiReq kiMemReadMultiReq)
SIG_DEF (SIG_KI_MEM_READ_MULTI_CNF,            KiMemReadMultiCnf kiMemReadMultiCnf)
SIG_DEF (SIG_KI_MEM_WRITE_MULTI_REQ,           KiMemWriteMultiReq kiMemWriteMultiReq)
SIG_DEF (SIG_KI_MEM_WRITE_MULTI_CNF,           KiMemWriteMultiCnf kiMemWriteMultiCnf)
#if defined (KI_ENABLE_LOGGING_SIGNALS_OVER_HSL)
SIG_DEF (SIG_KI_LOGGING_DISABLE_HSL_REQ,           EmptySignal kiLoggingDisableHslReq)
SIG_DEF (SIG_KI_LOGGING_DISABLED_HSL_IND,          EmptySignal kiLoggingDisabledHslInd)
SIG_DEF (SIG_KI_LOGGING_ENABLE_HSL_REQ,            EmptySignal kiLoggingEnableHslReq)
SIG_DEF (SIG_KI_LOGGING_ENABLED_HSL_IND,           EmptySignal kiLoggingEnabledHslInd)
#endif
SIG_DEF (SIG_KI_LOGGING_DISABLE_REQ,           EmptySignal kiLoggingDisableReq)
SIG_DEF (SIG_KI_LOGGING_DISABLED_IND,          EmptySignal kiLoggingDisabledInd)
SIG_DEF (SIG_KI_LOGGING_ENABLE_REQ,            EmptySignal kiLoggingEnableReq)
SIG_DEF (SIG_KI_LOGGING_ENABLED_IND,           EmptySignal kiLoggingEnabledInd)
SIG_DEF (SIG_KI_MIPS_LOGGING_ENABLE_REQ,       KiMipsLoggingEnableReq kiMipsLoggingEnableReq)
SIG_DEF (SIG_KI_MIPS_LOGGING_DISABLE_REQ,      KiMipsLoggingDisableReq kiMipsLoggingDisableReq)
SIG_DEF (SIG_KI_MIPS_LOGGING_DISABLE_IND,      KiMipsLoggingInd kiMipsLoggingInd)
#if defined (KI_ENABLE_OWN_QUEUEING)
SIG_DEF (SIG_KI_QUEUE_STATS_REQ,               KiQueueStatsReq kiQueueStatsReq)
SIG_DEF (SIG_KI_QUEUE_STATS_IND,               KiQueueStatsInd kiQueueStatsInd)
#else
SIG_DEF (SIG_KI_RESERVED_1,                    EmptySignal kiReserved1)
SIG_DEF (SIG_KI_RESERVED_2,                    EmptySignal kiReserved2)
#endif

SIG_DEF (SIG_KI_LOGGING_MEM_FREE_IND,          KiLoggingMemFreeInd kiLoggingMemFreeInd)

SIG_DEF (SIG_HSL_CONFIGURE_REQ,                 HslConfigureReq hslConfigureReq)
SIG_DEF (SIG_HSL_CONFIGURE_CNF,                 HslConfigureCnf hslConfigureCnf)
SIG_DEF (SIG_HSL_FILTER_CONFIGURE_REQ,          HslFilterConfigureReq hslFilterConfigureReq)
SIG_DEF (SIG_HSL_FILTER_CONFIGURE_CNF,          HslFilterConfigureCnf hslFilterConfigureCnf)

#if defined (KI_ENABLE_REF_DATA_LOGGING)
SIG_DEF (SIG_REF_DATA_IND,                      ReferencedDataInd referencedDataInd)
#endif /* KI_ENABLE_REF_DATA_LOGGING */

SIG_DEF (SIG_KI_CONFIGURE_LOGGING_TASK_REQ,      KiConfigureLoggingTaskReq kiConfigureLoggingTaskReq)
#if defined (KI_ASSERT_TESTING)
SIG_DEF (SIG_ASSERT_TEST_REQ,                   KiAssertTestReq kiAssertTestReq)
#endif /* KI_ASSERT_TESTING */

#if defined (UPGRADE_NVRAM_ENHANCED_TEST_ONLY)
SIG_DEF (SIG_KI_NVRAM_IT_TEST_REQ,        KiNvramItTestReq  kiNvramItTestReq)
SIG_DEF (SIG_KI_NVRAM_IT_TEST_CNF,        KiNvramItTestCnf  kiNvramItTestCnf)
#endif

#if defined (UPGRADE_GENIE_OVER_SHMCONN)
SIG_DEF (SIG_EMMI_CLEAN_LOGGING_MEMORY,        EmptySignal emmiCleanLoggingMemory)
#endif

#if defined(UPGRADE_GENIE_PROTOCOL_FOR_FIFO_NO_FLOWCON)
SIG_DEF (SIG_EMMI_INVALID_TX_BLOCK,          EmptySignal emmiInvalidTxBlock)
#endif

#if defined (FR_PRIMELOG_PRESENT)
SIG_DEF (SIG_PRIMELOG_CONFIGURE_REQ,          PrimeLogConfigureReq primeLogConfigureReq)
SIG_DEF (SIG_PRIMELOG_CONFIGURE_CNF,          EmptySignal primeLogConfigureCnf)
#endif

#if defined (FR_HSL_2_GKI)
SIG_DEF (SIG_KI_HSL_PRINTF_LEVEL4_IND,    KiHslPrintfLevel4Ind kiHslPrintf_LEVEL4_Ind)
SIG_DEF (SIG_KI_HSL_PRINTF_ATCI_IND,      KiHslPrintfAtciInd   kiHslPrintf_ATCI_Ind)
#endif

#if defined (MTK_NBIOT_ICD_ENABLE)
SIG_DEF (SIG_KI_ICD_PRINTF_IND,                 KiIcdPrintfWrapperInd kiIcdPrintfInd)
#endif

#if defined (OPTIMIZED_GKI_LOG_ENABLE)
/*Ki Log flush trigger flush signal*/
SIG_DEF (SIG_KI_LOG_TRIGGER_FLUSH_SIG,         KiLogSigEventT   kiLogSigEvent)
/*Ki Log buffer full signal*/
SIG_DEF (SIG_KI_LOG_BUFFER_FULL_SIG,           KiLogBuffFullSigT kiLogBuffFullSig)
/*Ki Log task Rx parser signal*/
SIG_DEF (SIG_KI_LOG_TASK_RX_DATA_SIG,          KiLogSigEventT   kiLogSigEvent1)
SIG_DEF (SIG_KI_SYMBOL_TAG,                    KiHslDebugSymbol kiHslDebugSymbol)
#endif /* OPTIMIZED_GKI_LOG_ENABLE */
SIG_DEF (SIG_KI_SYS_BOOT_INFO,                 KiSysBootInfo    kiSysBootInfo)

#endif

/* END OF FILE */
