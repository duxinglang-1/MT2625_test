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
 *    SYS trace definitions
 *
 **************************************************************************/

#if !defined (SYS_TRACE_H)
#define       SYS_TRACE_H

/***************************************************************************
* Include Files
***************************************************************************/
#include "nbiot_trace.h"

/***************************************************************************
 * Trace definitions
 ***************************************************************************/

BEGIN_TRACE_IDS(SYS_TRACE)
/*These are from emmioshi.c & emmioslo.c */
TRACE_DEF(SYS_TRACE_EMMI_TASK_END, 					HSL_SYS_LOG_DEBUG,    "EmmiHighPriTask(End)")
TRACE_DEF(SYS_TRACE_EMMI_TASK_START, 				HSL_SYS_LOG_DEBUG,    "EmmiHighPriTask(Start)")
TRACE_DEF(SYS_TRACE_EMMI_TASK_SEND_EVENT, 			HSL_SYS_LOG_EXTENDED, "EMMI High Event =%(EmmiHighEvent)16.0e")
TRACE_DEF(SYS_TRACE_EMMI_TASK_DISCARD_SIGNAL, 		HSL_SYS_LOG_DEBUG,    "EMMIHsl DiscardedSignal: SignalId=%#08lX")
TRACE_DEF(SYS_TRACE_EMMI_PORT_WRITE,				HSL_SYS_LOG_DEBUG,    "EMMIHi EmmiDmaCheckForTxData::Port Service Write  status=%d")
TRACE_DEF(SYS_TRACE_EMMI_PORT_READ, 				HSL_SYS_LOG_DEBUG,    "EMMIHi EmmiDmaCheckForRxData::Port Service Read	 status=%d")
TRACE_DEF(SYS_TRACE_EMMI_PORT_INIT, 				HSL_SYS_LOG_DEBUG,    "EMMIHi EmmiHighPriTask::Port Service Init Done  status=%d")
TRACE_DEF(SYS_TRACE_EMMI_PORT_CALLB,				HSL_SYS_LOG_DEBUG,    "EMMIHi Port Service callback function  event=%d")
TRACE_DEF(SYS_TRACE_EMMI_PORT_SET_BEGIN,            HSL_SYS_LOG_WARNING,  "Emmi port change called")
TRACE_DEF(SYS_TRACE_EMMI_PORT_SET,                  HSL_SYS_LOG_WARNING,  "Emmi port changed to %(PortIdTag)16.0e")
TRACE_DEF(SYS_TRACE_EMMIL_LOGGED_SIG_SEND,			HSL_SYS_LOG_DEBUG,    "EMMILo::GKI logged signal length=%d")
TRACE_DEF(SYS_TRACE_EMMIH_LOGGED_SIG_SEND,			HSL_SYS_LOG_DEBUG,    "EMMIHi::GKI send data length=%d")
TRACE_DEF(SYS_TRACE_EMMI_RECV_BLOCK, 				HSL_SYS_LOG_DEBUG,    "EMMIHi EmmiReceivedBlock")
TRACE_DEF(SYS_TRACE_EMMI_RX_PROCESS, 				HSL_SYS_LOG_DEBUG,    "EMMIHi EmmiRxProcess c= %d")
TRACE_DEF(SYS_TRACE_EMMI_TX_PROCESS, 				HSL_SYS_LOG_DEBUG,    "EMMIHi EmmiTxProcess emmiTxBlockState= %{1}8.8b c= %{1}8.0b")
TRACE_DEF(SYS_TRACE_EMMI_LP_DEBUG,                  HSL_SYS_LOG_BASIC,    "[LOG][EMMI][LP] debug Event: %u")

/*These are from kifrt.c*/
TRACE_DEF(SYS_TRACE_SUSPEND_TASK, 					HSL_SYS_LOG_EXTENDED, "TaskSuspend: %(TaskIdTag)16.0e")
TRACE_DEF(SYS_TRACE_RESUME_TASK, 					HSL_SYS_LOG_EXTENDED, "TaskResume: %(TaskIdTag)16.0e")
TRACE_DEF(SYS_TRACE_ALIVE_TASK_TRACE, 				HSL_SYS_LOG_EXTENDED, "I am Alive! %d %d %d")
TRACE_DEF(SYS_TRACE_ULS_CONFIG, 					HSL_SYS_LOG_DEBUG,    "ULS block config -> REG_ULS_EXTMEM_END_ADDRESS=%#08lx, REG_ULS_EXTMEM_START_ADDRESS=%#08lx, REG_ULS_EXTMEM_BLOCK_SIZE=%#04x")
TRACE_DEF(SYS_TRACE_TASK_SWITCH, 					HSL_SYS_LOG_BASIC,    "TASK SWITCH -> %(TaskIdTag)16.0e")
TRACE_DEF(SYS_TRACE_TASK_IDLE_SWITCH, 					HSL_SYS_LOG_BASIC,    "TASK SWITCH -> IDLE_TASK_ID")
TRACE_DEF(SYS_TRACE_BLOCK_CREATE_POOL, 				HSL_SYS_LOG_BASIC,    "Pool %d created: Blocksize=%d,count=%d")
TRACE_DEF(SYS_TRACE_BLOCK_IS_EMPTY, 				HSL_SYS_LOG_ERROR,    "Blockset %d Empty")
TRACE_DEF(SYS_TRACE_BLOCK_ALLOC, 					HSL_SYS_LOG_BASIC,    "Block alloc: count=%d pool=%d length=%d address=%#08lX Caller=%#08lX task/[irq]=%(TaskIdTag)16.0e")  
TRACE_DEF(SYS_TRACE_BLOCK_DEALLOC, 					HSL_SYS_LOG_BASIC,    "Block dealloc: count=%d pool=%d address=%#08lX")
TRACE_DEF(SYS_TRACE_ULS_MEMDUMP_FAIL,				HSL_SYS_LOG_ERROR,    "uls_handle_extmem FAILED!!: UlsEmiBufferPtr=%#08lX")
TRACE_DEF(SYS_TRACE_HSL_FILTER_SET, 				HSL_SYS_LOG_WARNING,  "HSL trace filter setting [%{1}8.8b] = %{1}8.0B")
TRACE_DEF(SYS_TRACE_HSL_FILTER_SET2, 				HSL_SYS_LOG_WARNING,  "HSL trace filter set/max   %{1}8.8b/%{1}8.0b")
TRACE_DEF(SYS_TRACE_HSL_FILTER_SET_FAIL,			HSL_SYS_LOG_WARNING,  "HSL trace filter set NVDM fail!  nvdm_status = %d")
TRACE_DEF(SYS_TRACE_HSL_FILTER_NOTE,                HSL_SYS_LOG_WARNING,  "NO BANDWIDTH TO TRACE ALL GROUPS!!  DEFAULT SET SELECTED !!")
TRACE_DEF(SYS_TRACE_ULS_PORT_SET_BEGIN,             HSL_SYS_LOG_WARNING,  "ULS port change called")
TRACE_DEF(SYS_TRACE_ULS_PORT_SET,                   HSL_SYS_LOG_WARNING,  "ULS port changed to %(PortIdTag)16.0e")

/*These are from kiosmem.c */
TRACE_DEF(SYS_TRACE_REQUEST_MEMORY, 				HSL_SYS_LOG_DEBUG,    "GKIMem_KiOsRequestMemoryEx_DynMem: address=%#08lX, (*buffer)=%#08lX, numberOfBytesRequired=%ld")
TRACE_DEF(SYS_TRACE_ALLOC_MEMORY, 					HSL_SYS_LOG_DEBUG,    "GKIMem_KiOsAllocMemoryEx_DynMem: address=%#08lX, (*buffer)=%#08lX, numberOfBytesRequired=%ld")
TRACE_DEF(SYS_TRACE_ALLOC_ZERO_MEMORY, 				HSL_SYS_LOG_DEBUG,    "GKIMem_KiOsAllocZeroMemoryEx_ZeroDynMem: address=%#08lX, (*buffer)=%#08lX, numberOfBytesRequired=%ld")
TRACE_DEF(SYS_TRACE_ALLOC_POLL_MEMORY, 				HSL_SYS_LOG_DEBUG,    "GKIMem_KiOsAllocPollMemory_DynMem: address=%#08lX, (*buffer)=%#08lX, numberOfBytesRequired=%ld")
TRACE_DEF(SYS_TRACE_REALLOC_MEMORY, 				HSL_SYS_LOG_DEBUG,    "GKIMem_KiOsReallocMemoryEx_DynMem: address=%#08lX, (*buffer)=%#08lX, numberOfBytesRequired=%ld")
TRACE_DEF(SYS_TRACE_RE_REQUEST_MEMORY, 				HSL_SYS_LOG_DEBUG,    "GKIMem_KiOsRerequestMemoryEx_DynMem: address=%#08lX, (*buffer)=%#08lX, numberOfBytesRequired=%ld")
TRACE_DEF(SYS_TRACE_RESIZE_MEMORY, 					HSL_SYS_LOG_DEBUG,    "GKIMem_KiOsResizeMemory_DynMem: address=%#08lX, buffer=%#08lX, numberOfBytesRequired=%ld")
TRACE_DEF(SYS_TRACE_RESIZE_MEMORY_FAILED, 			HSL_SYS_LOG_DEBUG,    "GKIMem_KiOsResizeMemory_DynMemFailed: address=%#08lX, buffer=%#08lX, numberOfBytesRequired=%ld")
TRACE_DEF(SYS_TRACE_FREE_MEMORY, 					HSL_SYS_LOG_DEBUG,    "GKIMem_KiOsFreeMemory_DynMem: address=%#08lX, (*buffer)=%#08lX, numberOfBytesRequired=%ld")
TRACE_DEF(SYS_TRACE_ALLOC_POLL_NON_CACHEABLE_MEMORY, HSL_SYS_LOG_DEBUG,   "GKIMem_KiOsAllocPollMemoryNonCacheable_DynMemFromPool: address=%#08lX, (*buffer)=%#08lX, numberOfBytesRequired=%ld")
TRACE_DEF(SYS_TRACE_RESIZE_NON_CACHEABLE_MEMORY, 	HSL_SYS_LOG_DEBUG,    "GKIMem_KiOsResizeMemoryNonCacheable_DynMem: address=%#08lX, buffer=%#08lX, numberOfBytesRequired=%ld")
TRACE_DEF(SYS_TRACE_RESIZE_NON_CACHEABLE_MEMORY_FAILED, HSL_SYS_LOG_DEBUG,"GKIMem_KiOsResizeMemoryNonCacheable_DynMemFailed: address=%#08lX, buffer=%#08lX, numberOfBytesRequired=%ld")
TRACE_DEF(SYS_TRACE_FREE_NON_CACHEABLE_MEMORY, 		HSL_SYS_LOG_DEBUG,    "GKIMem_KiOsFreeMemoryNonCacheable_DynMem: address=%#08lX, (*buffer)=%#08lX, numberOfBytesRequired=%ld")
/*These are from kiosq.c */
TRACE_DEF(SYS_TRACE_ENQUEUE_ACCESS1, 				HSL_SYS_LOG_EXTENDED, "GKI_Enqueue: queue=%#08lX, queue->maxCount=%#08lX, queue->count=%#08lX")
TRACE_DEF(SYS_TRACE_ENQUEUE_ACCESS2, 				HSL_SYS_LOG_EXTENDED, "GKI_Enqueue: queue=%#08lX, queue->numberInBlock=%#04X, queue->length=%#04X")
TRACE_DEF(SYS_TRACE_ENQUEUE_SIGNAL,					HSL_SYS_LOG_EXTENDED, "GKI_Enqueue: (*(buffer->type))=%(SignalIdTag)le, buffer->sig=%#08lX")
TRACE_DEF(SYS_TRACE_DEQUEUE_ACCESS1, 				HSL_SYS_LOG_EXTENDED, "GKI_Dequeue: queue=%#08lX, queue->maxCount=%#08lX, queue->count=%#08lX")
TRACE_DEF(SYS_TRACE_DEQUEUE_ACCESS2, 				HSL_SYS_LOG_EXTENDED, "GKI_Dequeue: queue=%#08lX, queue->numberInBlock=%#04X, queue->length=%#04X")
TRACE_DEF(SYS_TRACE_DEQUEUE_SIGNAL, 				HSL_SYS_LOG_EXTENDED, "GKI_Dequeue: (*(buffer->type))=%(SignalIdTag)le, buffer->sig=%#08lX")
/*These are from kiossig.c */
TRACE_DEF(SYS_TRACE_RECEIVE_SIGNAL_END, 			HSL_SYS_LOG_DEBUG,    "GKISignal_KiOsReceiveSignal(End): TaskId=%(TaskIdTag)16.0e")
TRACE_DEF(SYS_TRACE_RECEIVE_SIGNAL_START, 			HSL_SYS_LOG_DEBUG,    "GKISignal_KiOsReceiveSignal(Start): TaskId=%(TaskIdTag)16.0e")
TRACE_DEF(SYS_TRACE_RECEIVE_SIGNAL, 				HSL_SYS_LOG_BASIC,    "GKISignal_KiOsReceiveSignal: block->record.id=%(SignalIdTag)le, block->record.length=%#04X, signal->sig=%#08lX")
TRACE_DEF(SYS_TRACE_RECEIVE_SIGNAL_POLL, 			HSL_SYS_LOG_BASIC,    "GKISignal_KiOsReceiveSignalPoll: block->record.id=%(SignalIdTag)le, block->record.length=%#04X, signal->sig=%#08lX")
TRACE_DEF(SYS_TRACE_RECEIVE_INT_SIGNAL_POLL, 		HSL_SYS_LOG_BASIC,    "GKISignal_KiOsReceiveIntSignalPoll: block->record.id=%(SignalIdTag)le, block->record.length=%#04X, signal->sig=%#08lX")
TRACE_DEF(SYS_TRACE_SEND_SIGNAL, 					HSL_SYS_LOG_BASIC,    "GKISignal_KiOsSendSignal: block->record.id=%(SignalIdTag)le, block->record.length=%#04X, signal->sig=%#08lX")
TRACE_DEF(SYS_TRACE_SEND_INT_SIGNAL, 				HSL_SYS_LOG_BASIC,    "GKISignal_KiOsSendIntSignal: block->record.id=%(SignalIdTag)le, block->record.length=%#04X, signal->sig=%#08lX")
TRACE_DEF(SYS_TRACE_CREATE_SIGNAL, 					HSL_SYS_LOG_EXTENDED, "GKISignal_KiOsCreateSignal: block->record.id=%(SignalIdTag)le, block->record.length=%#04X, signal->sig=%#08lX")
TRACE_DEF(SYS_TRACE_CREATE_INT_SIGNAL,				HSL_SYS_LOG_EXTENDED, "GKISignal_KiOsCreateIntSignal: block->record.id=%(SignalIdTag)le, block->record.length=%#04X, signal->sig=%#08lX")
TRACE_DEF(SYS_TRACE_REQUEST_SIGNAL_SUCCESS, 		HSL_SYS_LOG_EXTENDED, "GKISignal_KiOsRequestSignal_Success: signalType=%#08lX, signalSize=%#04X, signal->sig=%#08lX")
TRACE_DEF(SYS_TRACE_REQUEST_SIGNAL_FAILURE, 		HSL_SYS_LOG_ERROR,    "GKISignal_KiOsRequestSignal_Failure: signalType=%#08lX, signalSize=%#8lX")

/*These are from kiosstat.c */
TRACE_DEF(SYS_TRACE_CHECK_STACK_OVERFLOW_MEMDUMP_1, HSL_SYS_LOG_ERROR,    "GKIStatistics_KiCheckStackOverFlow_MemDump: taskCounter=%#04X, kiTaskInitTable.stack=%#08lX, kiTaskInitTable.stackSize=%#04X")
TRACE_DEF(SYS_TRACE_CHECK_STACK_OVERFLOW_MEMDUMP_2, HSL_SYS_LOG_ERROR,    "GKIStatistics_KiCheckStackOverFlow_MemDump: kiIntStackPtr=%#08lX, INTERRUPT_STACK_SIZE=%#04X")

/*These are from kiostim.c */
TRACE_DEF(SYS_TRACE_TIM_START_TIMER_HANDLE_STEP_0, 	HSL_SYS_LOG_DEBUG,    "GKITimer_KiTimTaskStartTimerHandleStep0: listIndex=%d,kiTimLastCheckTime=%ld,insertionPoint=%ld")
TRACE_DEF(SYS_TRACE_TIM_START_TIMER, 				HSL_SYS_LOG_BASIC,    "KiTimStartTimer: listIndex=%d,timerData_earliestExpiry=%ld,Array_earliestExpiry=%ld")
TRACE_DEF(SYS_TRACE_TIM_START_TIMER_HANDLE_STEP_1, 	HSL_SYS_LOG_DEBUG,    "GKITimer_KiTimTaskStartTimerHandleStep1")
TRACE_DEF(SYS_TRACE_TIM_START_TIMER_HANDLE_STEP_2, 	HSL_SYS_LOG_DEBUG,    "GKITimer_KiTimTaskStartTimerHandleStep2")
TRACE_DEF(SYS_TRACE_TIM_CHECK_SIGNAL_QUEUE, 		HSL_SYS_LOG_EXTENDED, "GKITimer_KiTimTaskCheckSignalQueue_RxSignal: signalBlock->record.id=%(SignalIdTag)le, signalBlock->record.length=%#04X, (&signalBlock->record.body)=%#08lX")
TRACE_DEF(SYS_TRACE_TIM_HANDLE_RX_SIGNAL_END, 		HSL_SYS_LOG_EXTENDED, "GKITimer_KiTimTaskHandleRxSignalEnd")
TRACE_DEF(SYS_TRACE_TIM_CHECK_EXPIRY_LIST, 			HSL_SYS_LOG_EXTENDED, "KiTimTaskCheckExpiryList: now=%lx,kiTimLastCheckTime=%lx,earliestExpiry=%lx")
TRACE_DEF(SYS_TRACE_TIM_START_TIMER_TX_SIGNAL,		HSL_SYS_LOG_BASIC,    "GKITimer_KiOsStartTimer_TxSignal: SIG_START_TIMER, TASK:%(TaskIdTag)16.0e USER: %d TID: %#04X")
TRACE_DEF(SYS_TRACE_TIM_STOP_TIMER_TX_SIGNAL, 		HSL_SYS_LOG_BASIC,    "GKITimer_KiOsStopTimer_TxSignal: SIG_STOP_TIMER, TASK:%(TaskIdTag)16.0e USER: %d TID: %#04X")
TRACE_DEF(SYS_TRACE_KERNEL_TICK, 					HSL_SYS_LOG_DEBUG,    "KiOsTick: Tick=%lu")
TRACE_DEF(SYS_TRACE_TIMER_TASK_END, 				HSL_SYS_LOG_DEBUG,    "KiTimerTask(End)")
TRACE_DEF(SYS_TRACE_TIMER_TASK_START, 				HSL_SYS_LOG_DEBUG,    "KiTimerTask(Start)")
TRACE_DEF(SYS_TRACE_DUMP_ACTIVE_TIMERS, 			HSL_SYS_LOG_EXTENDED, "Active Timers: Task: %(TaskIdTag)16.0e  USER: %d TID: 0x%x Early Exp: %ld Late Exp: %ld CUR: %ld")
TRACE_DEF(SYS_TRACE_DUMP_NO_ACTIVE_TIMERS, 			HSL_SYS_LOG_EXTENDED, "No active timers")  
TRACE_DEF(SYS_TRACE_TIM_RESTORE_ACTION,             HSL_SYS_LOG_EXTENDED, "GKITimer restore result = %d, var1 = 0x%lx")
TRACE_DEF(SYS_TRACE_TIM_NO_ACTIVITY_CALLBACK,       HSL_SYS_LOG_EXTENDED, "NO Timer Activity Notify callback function registered!!!")
TRACE_DEF(SYS_TRACE_TIM_ACTIVITY_NOTIFY_DONE,       HSL_SYS_LOG_EXTENDED, "Timer Activity notify done, timer expiry case = %d")
TRACE_DEF(SYS_TRACE_TIM_SUBSCRIBE_TIMER_ACTIVITY,   HSL_SYS_LOG_EXTENDED, "Ki_subscribeEventTimer %d")
TRACE_DEF(SYS_TRACE_TIM_TICK_ADJUST,                HSL_SYS_LOG_EXTENDED, "Adjust GKI Tick, sec = 0x%lx, tick = 0x%lx, old = 0x%lx, new = 0x%lx")
TRACE_DEF(SYS_TRACE_TIM_CHECK_MODEM_RUNNING,        HSL_SYS_LOG_BASIC,    "KiTimCheckModemRunningStatus, LastReadyRunningNum = %lx, KiTick = %lx")

/* For GPT 32K Clk Check */
TRACE_DEF(SYS_TRACE_GPT_CNT_REC,                    HSL_SYS_LOG_BASIC,    "GPT32KCnt: Record32kCnt = %lu")
TRACE_DEF(SYS_TRACE_GPT_32K_CHK,                    HSL_SYS_LOG_WARNING,  "GPT 32K clock check: Diff=%lu, Gpt1mCnt=%lu, Gpt32kCnt=%lu")
TRACE_DEF(SYS_TRACE_DUMP_32K_REG,                   HSL_SYS_LOG_WARNING,  "RTC 32K registers: rtc_cali=%u, rtc_osc32con0=%u, rtc_osc32con1=%u")


/*These are from kiostti.c */
TRACE_DEF(SYS_TRACE_TTI_LOG_SYNC_MARKER,			HSL_SYS_LOG_WARNING,  "GKI_SYNC: sig:%(SignalIdTag)le, FrameNbr: %lu")
TRACE_DEF(SYS_TRACE_TTI_TEST_TASK_START_FROM_RUNNING, HSL_SYS_LOG_DEBUG,  "KiTestTask(Start)   state= running")
TRACE_DEF(SYS_TRACE_TTI_TEST_TASK_END_FROM_RUNNING, HSL_SYS_LOG_DEBUG,    "KiTestTask(End)     state= running")
TRACE_DEF(SYS_TRACE_TTI_TEST_TASK_START_FROM_WAIT, 	HSL_SYS_LOG_DEBUG,    "KiTestTask(Start)   state= wait")
TRACE_DEF(SYS_TRACE_TTI_TEST_TASK_END_FROM_WAIT, 	HSL_SYS_LOG_DEBUG,    "KiTestTask(End)     state= wait")
TRACE_DEF(SYS_TRACE_TTI_TEST_TASK_START_FROM_STOP, 	HSL_SYS_LOG_DEBUG,    "KiTestTask(Start)   state= stop")
TRACE_DEF(SYS_TRACE_TTI_TEST_TASK_END_FROM_STOP, 	HSL_SYS_LOG_DEBUG,    "KiTestTask(End)     state= stop")
TRACE_DEF(SYS_TRACE_TTI_MIPS_START, 				HSL_SYS_LOG_DEBUG,    "MIPS Monitor(Start)")
TRACE_DEF(SYS_TRACE_TTI_MIPS_END, 					HSL_SYS_LOG_DEBUG,    "MIPS Monitor(End)")
TRACE_DEF(SYS_TRACE_TTI_LOG_SIGNAL_STRUCT, 			HSL_SYS_LOG_EXTENDED, "TestTask_KiOsLogSignalStruct: signalId=%08lX, signalSize=%#04X, signal=%8lX")
TRACE_DEF(SYS_TRACE_TTI_FILTER_REQ,					HSL_SYS_LOG_DEBUG,    "KiTestTask::HSL filter request signal received: Action=%X")
TRACE_DEF(SYS_TRACE_TTI_LOGGED_SIG_SEND,			HSL_SYS_LOG_DEBUG,    "KiTestTask::GKI logged signal length=%d")
TRACE_DEF(HSL_SYS_LOG_TTI_PCOMM,			        HSL_SYS_LOG_DEBUG,    "KiTtiProcessCommsRx  format=%d")
TRACE_DEF(HSL_SYS_LOG_TTI_PCOMM_SIG,		        HSL_SYS_LOG_DEBUG,    "KiTtiProcessCommsRx  signal=%(SignalIdTag)le")
TRACE_DEF(HSL_SYS_LOG_TTI_PCOMM_SIG_F,		        HSL_SYS_LOG_DEBUG,    "KiTtiProcessCommsRx  forward=%d")
TRACE_DEF(HSL_SYS_LOG_TTI_PCOMM_SIG_R,		        HSL_SYS_LOG_DEBUG,    "KiTtiProcessCommsRx  redirect=%d")
TRACE_DEF(HSL_SYS_LOG_TTI_PCOMM_SIG_D,		        HSL_SYS_LOG_DEBUG,    "KiTtiProcessCommsRx  destination=%(TaskIdTag)16.0e")
TRACE_DEF(SYS_TRACE_GKI_FILTER_SET_FAIL,            HSL_SYS_LOG_WARNING,  "GKI trace filter mismatches default filter, please update default filter!!!")

/*These are from kiosdynmem.c */

TRACE_DEF(SYS_TRACE_MEMORY_ADD_BLOCK_TO_FREE_LIST, 	HSL_SYS_LOG_EXTENDED, "KMDYN: KmMemoryFree: mergeHeaderAddr=%#08lX,  nextGlobalAddr=%#08lX,prevGlobalAddr=%#08lX")
TRACE_DEF(SYS_TRACE_MEMORY_GET_1, 					HSL_SYS_LOG_EXTENDED, "KMDYN: KmMemoryGet: freeHeaderAddr=%#08lX, nextGlobalAddr=%#08lX, prevGlobalAddr=%#08lX")
TRACE_DEF(SYS_TRACE_MEMORY_GET_2, 					HSL_SYS_LOG_EXTENDED, "KMDYN: KmMemoryGet: newFreeHeaderAddr=%#08lX")
TRACE_DEF(SYS_TRACE_MEMORY_FREE_BLOCK, 				HSL_SYS_LOG_EXTENDED, "KMDYN: KmMemoryFree: originalHeaderAddr=%#08lX,  nextGlobalAddr=%#08lX,prevGlobalAddr=%#08lX")

/*These are from kiostask.c */
TRACE_DEF(SYS_TRACE_TASK_CREATE_1,   				HSL_SYS_LOG_BASIC,    "%(TaskIdTag)16.0e started! ID: %{1}d")
TRACE_DEF(SYS_TRACE_TASK_CREATE_2,   				HSL_SYS_LOG_BASIC,    "All Modem tasks started. Total tasks: %d")
TRACE_DEF(SYS_TRACE_TASK_BOOTUP_NOTIFY,             HSL_SYS_LOG_EXTENDED, "Notify all task bootup reason: %d")

/*These are from kisys.c */
TRACE_DEF(SYS_TRACE_BUFFER_SIGNAL,                  HSL_SYS_LOG_EXTENDED, "sig:%(SignalIdTag)le is buffered")
TRACE_DEF(SYS_TRACE_BUFFER_LOGGED_SIGNAL,           HSL_SYS_LOG_EXTENDED, "logged sig:%(SignalIdTag)le is buffered")
TRACE_DEF(SYS_TRACE_BUFFER_COMMAND,                 HSL_SYS_LOG_EXTENDED, "cmd:%ld is buffered")
TRACE_DEF(SYS_TRACE_BUFFER_UNKNOWN,                 HSL_SYS_LOG_EXTENDED, "unknown format %d is buffered")
TRACE_DEF(SYS_TRACE_FLUSH_BUFFERED_SIGNAL,          HSL_SYS_LOG_EXTENDED, "sig:%(SignalIdTag)le is flush out")
TRACE_DEF(SYS_TRACE_ALWAYS_ACTIVE_SIGNAL,           HSL_SYS_LOG_EXTENDED, "sig:%(SignalIdTag)le is always actived, pass via case %d")
TRACE_DEF(SYS_TRACE_SUBSCRIBE_SIGNAL_EVENT,         HSL_SYS_LOG_EXTENDED, "subscribe signal event %d")
TRACE_DEF(SYS_TRACE_SIGNAL_ACTIVITY_NOTIFY,         HSL_SYS_LOG_EXTENDED, "signal activity notify done")

/*These are from md_init.c */
TRACE_DEF(SYS_TRACE_EMMI_DMA_CALLBACK, 				HSL_SYS_LOG_DEBUG,    "EMMI DMA callback - status:0x%08x ")
TRACE_DEF(SYS_TRACE_ULS_DMA_CALLBACK1, 				HSL_SYS_LOG_DEBUG,    "ULS DMA callback ")
TRACE_DEF(SYS_TRACE_ULS_DMA_CALLBACK2, 				HSL_SYS_LOG_DEBUG,    "ULS DMA callback - Some Try to Send Data to Target With ULS DMA !!")
TRACE_DEF(SYS_TRACE_ULS_IRQ_HANDLER, 				HSL_SYS_LOG_BASIC,    "ULS IRQ  - active: 0x%08X ")
TRACE_DEF(SYS_TRACE_ULS_IRQ_RING_BUFFER_FULL, 		HSL_SYS_LOG_WARNING,  "ULS IRQ  - Ring Buffer FULL ")
TRACE_DEF(SYS_TRACE_ULS_IRQ_OUTPUT_BUFFER_FULL,	    HSL_SYS_LOG_WARNING,  "ULS IRQ  - Output Buffer FULL ")
TRACE_DEF(SYS_TRACE_ULS_SPEED_INFO,		            HSL_SYS_LOG_WARNING,  "UART feeding SPEED: %d kB/s")
TRACE_DEF(SYS_TRACE_ULS_SPEED_WARNING,		        HSL_SYS_LOG_WARNING,  "WARNING UART close to MAXIMUM SPEED 95kB/s. Traces lost!!")
TRACE_DEF(SYS_TRACE_ULS_PORT_INIT,					HSL_SYS_LOG_WARNING,  "open ULS serial port error: %d")
TRACE_DEF(SYS_TRACE_TIME_LOST,						HSL_SYS_LOG_WARNING,  "time lost duration:%lu")
TRACE_DEF(SYS_TRACE_ULS_ST_CHANGE,					HSL_SYS_LOG_EXTENDED, "uls low power change %d %d %#lx")
TRACE_DEF(SYS_TRACE_ULS_ST_32ALIGN,					HSL_SYS_LOG_EXTENDED, "ULS Send data was not 32 align remain=%d, try to send=%d")
TRACE_DEF(SYS_TRACE_ULS_MDSYS_OFF_CTRL,             HSL_SYS_LOG_BASIC,    "MDSYS OFF PD CTRL :%u")
TRACE_DEF(SYS_TRACE_ULS_CB_READ,					HSL_SYS_LOG_DEBUG,    "ULS port service callback for read called")
TRACE_DEF(SYS_TRACE_ULS_CB_WRITE,					HSL_SYS_LOG_DEBUG,    "ULS port service callback for write called")
TRACE_DEF(SYS_TRACE_ULS_USB_DISCONNECT,				HSL_SYS_LOG_DEBUG,    "ULS port service callback for USB disconnect called")
TRACE_DEF(SYS_TRACE_ULS_USB_CONNECT,				HSL_SYS_LOG_DEBUG,    "ULS port service callback for USB connect called")
TRACE_DEF(SYS_TRACE_ULS_LP_DBG,                     HSL_SYS_LOG_BASIC,    "[LOG][ULS][LP] debug event: %u arg0: %u arg1: %u")
TRACE_DEF(SYS_TRACE_SLP_LOCK_INFO,                  HSL_SYS_LOG_BASIC,    "[LOG][SLP]lock info: hi->%#lx, lo->%#lx")

/* hal_nvic.c temp */
TRACE_DEF(SYS_TRACE_IRQ_IN,                         HSL_SYS_LOG_DEBUG,    "IRQ in  %d")
TRACE_DEF(SYS_TRACE_IRQ_OUT,                        HSL_SYS_LOG_DEBUG,    "IRQ out  %d")
TRACE_DEF(SYS_TRACE_IRQ_SPEED_WARNING,              HSL_SYS_LOG_DEBUG,    "Warning: Stayed too long in IRQ context: TIME:%d IRQ:%d")

/* csci */
TRACE_DEF(SYS_TRACE_CSCI_DBG,                       HSL_SYS_LOG_DEBUG,    "[csci] event->%4d arg0:%4d arg1:%4d")

/* KI log */
TRACE_DEF(SYS_TRACE_KI_LOG_DEBUG,                   HSL_SYS_LOG_BASIC,    "%(KiLogDbgEvent_t)e %#08x %#08x %#08x %#08x")
TRACE_DEF(SYS_TRACE_LOG_SPEED_INFO,                 HSL_SYS_LOG_WARNING,  "KiLog/ULS feeding speed(KB/s): %3d %3d")

/* For 32K Clk Check */
TRACE_DEF(SYS_TRACE_MD_32K_CHK_1,                   HSL_SYS_LOG_WARNING,  "MD 32K clock check: Diff=%d, Stime=%lu, 32kCnt=%lu")
TRACE_DEF(SYS_TRACE_MD_32K_CHK_2,                   HSL_SYS_LOG_WARNING,  "MD 32K clock check: K_res=%lX, 32kCnt=%lu")

END_TRACE_IDS(SYS_TRACE)

#endif /* SYS_TRACE_H */

/* END OF FILE */

