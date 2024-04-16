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
 *      Core Interrupt priorities and definitions
 **************************************************************************/
#ifndef DLIRQDEF_H
#define DLIRQDEF_H


/***************************************************************************
**                         Interrupt definitions
**
**      Label          ISR Entrypoint                   Absolute    Priority
**                                                      IRQ Num
**
****************************************************************************/

#define        FrameInt          L1FrameInterrupt,         4,              5
#if !defined (UPGRADE_L1_SORTER_IN_FRAME_INTS)
#define        SorterInt         PlkSorterInt,             9,              6
#endif /* UPGRADE_L1_SORTER_IN_FRAME_INTS */

#define        DspFatalInt       PlkErrorHandler,         24,             2

# define      GprsPtmOddInt     L1FrPtmOddInt,            5,              5
# define      GprsIdlePrachInt  L1FrIdlePrachInt,         7,              5
# if defined (ABP_VERSION_3)
#if defined (UPGRADE_CBE2000) || defined (UPGRADE_CIPHERK)
#   define    CipherGea3Int     CipherGea3Interrupt,      1,              7
#   define    CipherGeaInt      CipherGeaInterrupt,       38,             7
#  endif
# endif

#if defined (UPGRADE_USB)
#  define      UsbInt            usbDriverInterruptHandler,33,             7
#endif

#if !defined (DISABLE_32K_SLEEP)
#  define      SleepInt          SleepInterrupt,           15,             15
#  define      WakeupInt         WakeupInterrupt,          6,              0
#endif
#define        TcuExceptionInt   PlkTcuExceptionHandler,   28,             1

#define  INT_PRIORITY(eNT,nUM,pRI)  pRI
#define  INT_NUMBER(eNT,nUM,pRI)    nUM
#define  INT_ENTRY(eNT,nUM,pRI)     eNT

#if defined (SIMN_PLKM_IRQ)
Int16     mcuIrqCheckPending(void);
#endif

#define INT_SWI_NUM(INT_INFO)                INT_NUMBER(INT_INFO)
#define TCU_FRAME_INTERRUPT_PRIORITY         (INT_SWI_NUM(FrameInt) << 8)
#if !defined (UPGRADE_L1_SORTER_IN_FRAME_INTS)
#  define TCU_SORTER_INTERRUPT_PRIORITY      (INT_SWI_NUM(SorterInt) << 8)
#endif
#if !defined (DISABLE_32K_SLEEP)
#  define TCU_SLEEP_INTERRUPT_PRIORITY       (INT_SWI_NUM(SleepInt) << 8)
#  define TCU_WAKEUP_INTERRUPT_PRIORITY      (INT_SWI_NUM(WakeupInt) << 8)
#endif
#  define TCU_GPRS_PTM_ODD_INT_PRIORITY      (INT_SWI_NUM(GprsPtmOddInt) << 8)
#  define TCU_GPRS_IDLE_PRACH_INT_PRIORITY   (INT_SWI_NUM(GprsIdlePrachInt) << 8)
#if defined (ABP_VERSION_3)
#if defined (UPGRADE_CBE2000) || defined (UPGRADE_CIPHERK)
#  define TCU_CIPHER_GEA3_INT_PRIORITY       (INT_SWI_NUM(CipherGea3Int) << 8)
#endif
#endif

# endif /* DLIRQDEF_H */
