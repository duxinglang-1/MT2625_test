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
 *      Core Manager Layer Interrupt Controller Interface
 *
 *      SEE CORRESPONDING C FILE FOR MORE INFORMATION
 *      ALSO SEE DLIRQUTL.C FOR NON-CORE INTERRUPT CONTROLLER INTERFACE
 **************************************************************************/
 #if !defined (PLKMIRQ_H)
 #define PLKMIRQ_H

#if defined (UPGRADE_CORE_FR)
# include <frirq.h>
#else

/***************************************************************************
 * Macros
 ***************************************************************************/

/***************************************************************************
 * Variables
 ***************************************************************************/

/* irqDisableCount
 *  Interrupt disable count.  This could be declared static, but it is
 *  useful when debugging to be able to access it.
 */
extern Int16 irqDisableCount;

#if defined (PLK_VERIFY_MAX_IRQ_DISABLED_TIME) && defined (DEVELOPMENT_VERSION)
/* These variables find code that disables interrupts for long periods of time. */
extern Int32 irqDisableMaxTime;
extern Int32 irqDisableMaxTimeDisableCallerAddress;
extern Int32 irqDisableMaxTimeEnableCallerAddress;
#endif

/***************************************************************************
 * Function Prototypes
 ***************************************************************************/
extern void DLMicInitialise (void);


extern void dlCrossAssertSwi(Int16 swi);

extern void dlIgnoreMaxIrqDisabledPeriod(void);
extern void dlResetMaxIrqDisabledPeriodMeasurement(void);

extern void dlAckFrameInt(void);
extern void dlAckSorterInt(void);
#if defined (UPGRADE_ENHANCED_PLK)
extern void dlAckDspXswi(void);
#endif
extern void dlAckDspFatalInt(void);
#if defined(UPGRADE_SYS_TOOLS)
#if defined(ENABLE_L1_PROF)
extern void dlAckProfileInt(void);
#endif
#endif
extern void dlAckGprsPtmOddInt(void);
extern void dlAckGprsIdlePrachInt(void);

extern void dlEnableFrameInt(void);
extern void dlEnableSorterInt(void);
#if defined (UPGRADE_ENHANCED_PLK)
extern void dlEnableDspXswi(void);
#endif
extern void dlEnableDspFatalInt(void);
extern void dlEnableTcuExceptionInt(void);
extern void dlEnableTcuUnderflowWarning(void);
extern void dlDisableTcuUnderflowWarning(void);

#if defined(UPGRADE_SYS_TOOLS)
#if defined(ENABLE_L1_PROF)
extern void dlEnableProfileInt(void);
extern void dlDisableProfileInt(void);
#endif
#endif

extern void dlEnableGprsPtmOddInt(void);
extern void dlEnableGprsIdlePrachInt(void);
extern void dlDisableGprsIdlePrachInt(void);

#if !defined (DISABLE_32K_SLEEP)
void dlDisableFrameInt(void);
extern void dlAckSleepInt(void);
extern void dlAckWakeupInt(void);
#endif

#if defined (ABP_VERSION_3)
#if defined (UPGRADE_CBE2000) || defined (UPGRADE_CIPHERK)
void dlAckCipherGea3Int(void);
void dlEnableCipherGea3Int(void);
void dlDisableCipherGea3Int(void);
#endif
#endif


#endif

#endif                                  /* PLKMIRQ_H */
