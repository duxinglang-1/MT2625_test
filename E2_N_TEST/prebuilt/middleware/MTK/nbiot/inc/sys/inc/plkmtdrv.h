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
 *  File Description: PLK/M
 *    ABP TCU/TDQ Driver functions/Macros
 **************************************************************************/

#ifndef PLKMTDRV_H
#define PLKMTDRV_H

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/
#if defined (UPGRADE_CORE_FR)
# include <frtcu.h>
# include <frpki.h>
#else /* UPGRADE_CORE_FR */

#if !defined (ENABLE_PLKSIM)
# include <plkmirq.h>
#endif

#include <dlirqdef.h>

#if defined (ON_PC)
/* will be neater to add these to a seperate include file later on */
extern Int16 tcuReadReg (Int16 address);
extern void  tcuWriteReg(Int16 address, Int16 value);
#endif

/****************************************************************************
 * Manifest constants
 ****************************************************************************/
 /*******************************************************************************
 *
 * @Define:         GSM_TCU_PRESCALE_VALUE @
 *
 * @Interface:      PLK/M TCU@
 *
 * @Description:    Tcu prescale value. This value is configurable depending on the
  *                 ABP clock. For example for 26Mhz clock (RP) the ratio
  *                 (26/GSM_TCU_PRESCALE_VALUE+1) = 13MHz/12 @
 *
 ******************************************************************************/
#if !defined (ON_PC)
# if defined (ABP_VERSION_3)
#  define GSM_TCU_PRESCALE_VALUE     (11)
# else
#  define GSM_TCU_PRESCALE_VALUE     (23)  /* default case is for reference platform */
# endif
#else
# define GSM_TCU_PRESCALE_VALUE       (11)
#endif

/****************************************************************************
 * Macros and constants to access TCU hardware.
 ****************************************************************************/
/*
** Offset of registers into the TCU block
*/
#define TCU_OFFSET       0x000


#if defined (ABP_VERSION_3)
/*
** TCU Control Registers
** Address offsets.                          RW   Def Size
*/
#define TCU_VERSION             0x00      /* R    --  16 */
#define TCU_CPU_SEL             0x02      /* RW   00   1 */
#define TCU_STATUS              0x04      /* RW   --   8 */
#define TCU_CONTROL             0x06      /* RW   00   4 */
#define TCU_MCLK_RATE           0x08      /*  W   --   4 */
#define TCU_DSP_RATE            0x0A      /* RW   7F   4 */
#define TCU_MONITOR_SEL         0x0C      /* RW   00   3 */
#define TCU_MEM_CTRL            0X0E      /* RW   00   4 */
#define TCU_MEM_PTR             0X10      /* RW   00   8 */
#define TCU_MEM_IO              0X12      /* RW   11  16 */
#define TCU_NUM_INSTR           0X14      /* R    --  16 */
#define TCU_EXCEPTION_ENABLE    0X16      /* RW   00   7 */
#define TCU_NEARLY_FULL_LEVEL   0X18      /* RW   7F   8 */
#define TCU_NEARLY_EMPTY_LEVEL  0X1A      /* RW   00   8 */
#define TCU_READ_IDX            0X1C      /* RW   00   8 */
#define TCU_PRESCALE            0X20      /* RW   11   8 */
#define TCU_DELTA_TIMER         0X22      /* R    --  16 */

#define TCU_EVENT_LOG           0X24
#define TCU_CLEAR_GPOA          0x26
#define TCU_SET_GPOA            0x28
#define TCU_CLEAR_GPOB          0x2A
#define TCU_SET_GPOB            0x2C

/****************************************************************************
 * Bit mask defines
 ****************************************************************************/

#define TCU_REV_IMP_MASK        0x3F
#define TCU_REV_MINOR_MASK      (0x3F) << 6
#define TCU_REV_MAJOR_MASK      (0x0F) << 12

#define TCU_CLK_RATE_MASK       0x0F

#define TCU_MON_SEL_OUTPUT_LOW  0
#define TCU_MON_SEL_INTERNAL    1
#define TCU_MON_SEL_INSTR       2
#define TCU_MON_SEL_DELTA       3
#define TCU_MON_SEL_TCU_STROBES 4
#define TCU_MON_SEL_SLOW_CLOCK  5
#define TCU_MON_SEL_HSL         6

#define TCU_MEM_RW_BIT          (1<<8)
#define TCU_MEM_BUSY_BIT        (1<<9)
#define TCU_MEM_MODE_VIOLATION  (1<<10)
#define TCU_MEM_BUSY_VIOLATION  (1<<11)
#define TCU_MEM_VIOLATION_MASK  (TCU_MEM_MODE_VIOLATION | TCU_MEM_BUSY_VIOLATION)
#define TCU_WRITE_IDX           TCU_MEM_PTR

#else
/*
** TCU Control Registers
** Address offsets.                          RW   Def Size
*/
#define TCU_FIFO_IN             0x00      /*  W   --  16 */
#define TCU_NUM_INSTR           0x02      /* R    00   8 */
#define TCU_STATUS              0x04      /* RW   --   8 */
#define TCU_EXCEPTION_ENABLE    0x06      /* RW   00   7 */
#define TCU_CONTROL             0x08      /*  W   --   3 */
#define TCU_NEARLY_FULL_LEVEL   0x0A      /* RW   7F   8 */
#define TCU_NEARLY_EMPTY_LEVEL  0x0C      /* RW   00   8 */
#define TCU_WRITE_IDX           0x0E      /* RW   00   8 */
#define TCU_READ_IDX            0x10      /* RW   00   8 */
#define TCU_PRESCALE            0x12      /* RW   11   6 */
#define TCU_DELTA_TIMER         0x14      /* R    --  16 */

#define TCU_EVENT_LOG           0x18
#define TCU_CLEAR_GPOA          0x20
#define TCU_SET_GPOA            0x22
#define TCU_CLEAR_GPOB          0x24
#define TCU_SET_GPOB            0x26

#endif
/* CORESIM defines only */
#define TCU_FIFO_RAM_RAMSIZE       (0x1Fe)
#define TCU_FIFO_RAM_START         0x200
#define TCU_FIFO_RAM_END           (TCU_FIFO_RAM_START + TCU_FIFO_RAM_RAMSIZE)

/****************************************************************************
 * Bit mask defines
 ****************************************************************************/

/*Instruction field bit masks*/
#define TCU_TIME_MASK       0x00FF
#define TCU_COMMAND_MASK    0xF000
#define TCU_DATA_MASK       0x0F00

/*
** Status Register bit masks.
*/
#define TCU_NEARLY_FULL_MASK       (~0x01)
#define TCU_NEARLY_EMPTY_MASK      (~0x02)
#define TCU_OVERFLOW_ERR_MASK      (~0x04)
#define TCU_UNDERFLOW_WARN_MASK    (~0x08)
#define TCU_UNDERFLOW_ERR_MASK     (~0x10)
#define TCU_CONCURRENCY_LIMIT_MASK (~0x20)
#define TCU_SHORTWAIT_FAIL_MASK    (~0x40)

#define TCU_ALL_EXCEPTIONS         (0x7F)
/* any TCU exceptions to be disabled should be ANDed in here */
#define TCU_DISABLE_EXCEPTIONS     (TCU_CONCURRENCY_LIMIT_MASK)

/*
** Command Set.
** Upper 4 bits of the TCU Fifo Instruction Contents.
*/
#define TCU_GPIOA_SET       0x1000
#define TCU_GPIOA_CLR       0x0000
#define TCU_GPIOB_SET       0x3000
#define TCU_GPIOB_CLR       0x2000
#define TCU_GPIOC_SET       0x5000
#define TCU_GPIOC_CLR       0x4000

#define TCU_SIG_GPIOA       0x6000
#define TCU_SIG_GPIOB       0x7000
#define TCU_MCU_INT         0x8000
#define TCU_DSP_INT         0x9000
#define TCU_DMA_INT         0xA000 /* This isn't core, is it used ????? */
#define TCU_RESERVED2       0xB000
#define TCU_SHORT_WAIT      0xC000
#define TCU_LONG_WAIT       0xD000
#define TCU_CLOCK_CTRL      0xE000
#define TCU_NO_OPERATION    0xF000

#if !defined (DISABLE_32K_SLEEP)
#define TCU_SNAP_CODE           0x000
#define TCU_SLEEP_CODE          0x100
#endif

/*
** These are equivalent definitions for the GPIO for use with
** the TCU_GPIO_A, TCU_SET_BIT alternate style.
** Or TCU_GPIOA | TCU_SET_BIT
*/
#define TCU_GPIOA           0x0000
#define TCU_GPIOB           0x2000
#define TCU_GPIOC           0x4000

#define TCU_SET_BIT         0x1000
#define TCU_CLR_BIT         0x0000

/* 4-bit data field definitions for  MCU/DSP/DMA_INT commands */
#define TCU_DSP_PRIORITY_0      0x0000
#define TCU_DSP_PRIORITY_1      0x0200
#define TCU_DSP_PRIORITY_2      0x0400
/* priority 3 is reserved for direct DSP interrupts generated from the L1_BG_TASK
** and should not be used in Macro-SM sequences
** #define TCU_DSP_PRIORITY_3      0x0600
*/
#define TCU_DSP_PRIORITY_4      0x0800
#define TCU_DSP_PRIORITY_5      0x0A00
#define TCU_DSP_PRIORITY_6      0x0C00
#define TCU_DSP_PRIORITY_7      0x0E00
#define TCU_DSP_PRIORITY_MASK   0x0E00
#define TCU_DSP_PRIORITY_POSN   8

#define TCU_MCU_PRIORITY_0      0x0000
#define TCU_MCU_PRIORITY_1      0x0100
#define TCU_MCU_PRIORITY_2      0x0200
#define TCU_MCU_PRIORITY_3      0x0300
#define TCU_MCU_PRIORITY_4      0x0400
#define TCU_MCU_PRIORITY_5      0x0500
#define TCU_MCU_PRIORITY_6      0x0600
#define TCU_MCU_PRIORITY_7      0x0700
#define TCU_MCU_PRIORITY_8      0x0800
#define TCU_MCU_PRIORITY_9      0x0900
#if defined (UPGRADE_THAMIS)
/* Thamis may still need this fix - Core used it with TCIA/GPIA,
 * and abandoned in when these were replaced by IrqCtrl
 * #define TCU_MCU_PRIORITY_9      0x0100
 */
#endif
#define TCU_MCU_PRIORITY_10     0x0A00
#define TCU_MCU_PRIORITY_11     0x0B00
#define TCU_MCU_PRIORITY_12     0x0C00
#define TCU_MCU_PRIORITY_13     0x0D00
#define TCU_MCU_PRIORITY_14     0x0E00
#define TCU_MCU_PRIORITY_15     0x0F00
#define TCU_MCU_PRIORITY_MASK   0x0F00
#define TCU_MCU_PRIORITY_POSN   8

/*
** Gpio lines definitions for use with GPIO_SET/CLR/SIG.
** Lower 4 bits of the Fifo Instruction Contents.
*/
#define TCU_GPIO_BIT_0      0x0000
#define TCU_GPIO_BIT_1      0x0100
#define TCU_GPIO_BIT_2      0x0200
#define TCU_GPIO_BIT_3      0x0300
#define TCU_GPIO_BIT_4      0x0400
#define TCU_GPIO_BIT_5      0x0500
#define TCU_GPIO_BIT_6      0x0600
#define TCU_GPIO_BIT_7      0x0700
#define TCU_GPIO_BIT_8      0x0800
#define TCU_GPIO_BIT_9      0x0900
#define TCU_GPIO_BIT_A      0x0A00
#define TCU_GPIO_BIT_B      0x0B00
#define TCU_GPIO_BIT_C      0x0C00
#define TCU_GPIO_BIT_D      0x0D00
#define TCU_GPIO_BIT_E      0x0E00
#define TCU_GPIO_BIT_F      0x0F00


/*
 * TCU ports  do not necessarily match
 * 1:1 with the direct-writeable GPIO's defined in DLGPIO.H - some do, but some
 * do not. These macros hide the mapping of TCU numbers to GPIO or GPO
 * numbers. Also, there are one or two lines that do not exist in the real
 * hardware.
 */

/* this table is ordered in terms of TCU output ports  */
#define TCU_GPO_0        (TCU_GPIOA + TCU_GPIO_BIT_0)  /* TCU-0 */
#define TCU_GPO_1        (TCU_GPIOA + TCU_GPIO_BIT_1)  /* TCU-1 */
#define TCU_GPO_2        (TCU_GPIOA + TCU_GPIO_BIT_2)  /* TCU-2 */
#define TCU_GPO_3        (TCU_GPIOA + TCU_GPIO_BIT_3)  /* TCU-3 */
#define TCU_GPO_4        (TCU_GPIOA + TCU_GPIO_BIT_4)  /* TCU-4 */
#define TCU_GPO_5        (TCU_GPIOA + TCU_GPIO_BIT_5)  /* TCU-5 */
#define TCU_GPO_6        (TCU_GPIOA + TCU_GPIO_BIT_6)  /* TCU-6 */
#define TCU_GPO_7        (TCU_GPIOA + TCU_GPIO_BIT_7)  /* TCU-7 */
#define TCU_GPO_8        (TCU_GPIOA + TCU_GPIO_BIT_8)  /* TCU-8 */
#define TCU_GPO_9        (TCU_GPIOA + TCU_GPIO_BIT_9)  /* TCU-9 */
#define TCU_GPO_10       (TCU_GPIOA + TCU_GPIO_BIT_A)  /* TCU-10 */
#define TCU_GPO_11       (TCU_GPIOA + TCU_GPIO_BIT_B)  /* TCU-11 */
#define TCU_GPO_12       (TCU_GPIOA + TCU_GPIO_BIT_C)  /* TCU-12 alt 1 */
#define TCU_GPO_13       (TCU_GPIOA + TCU_GPIO_BIT_D)  /* TCU-13 alt 1  */
#define TCU_GPO_14       (TCU_GPIOA + TCU_GPIO_BIT_E)  /* TCU-14 alt 1 */
#define TCU_GPO_15       (TCU_GPIOA + TCU_GPIO_BIT_F)  /* TCU-15 alt 1  */

#define TCU_GPO_16       (TCU_GPIOB + TCU_GPIO_BIT_0)  /* TCU-16 */
#define TCU_GPO_17       (TCU_GPIOB + TCU_GPIO_BIT_1)  /* TCU-17 */
#define TCU_GPO_18       (TCU_GPIOB + TCU_GPIO_BIT_2)  /* TCU-18 */
                         /* TCU 19 does not exist in the TCU */
#define TCU_GPIO_32      (TCU_GPIOB + TCU_GPIO_BIT_4)  /* TCU-20 */
#define TCU_GPO_27       (TCU_GPIOB + TCU_GPIO_BIT_5)  /* TCU-21 alt 1  */
#define TCU_GPIO_22      (TCU_GPIOB + TCU_GPIO_BIT_6)  /* TCU-22 */
#define TCU_GPIO_23      (TCU_GPIOB + TCU_GPIO_BIT_7)  /* TCU-23 */
#define TCU_GPIO_24      (TCU_GPIOB + TCU_GPIO_BIT_8)  /* TCU-24 */
#define TCU_GPIO_25      (TCU_GPIOB + TCU_GPIO_BIT_9)  /* TCU-25 */
#define TCU_GPIO_26      (TCU_GPIOB + TCU_GPIO_BIT_A)  /* TCU-26 */
#define TCU_GPIO_27      (TCU_GPIOB + TCU_GPIO_BIT_B)  /* TCU-27 */
#define TCU_GPIO_28      (TCU_GPIOB + TCU_GPIO_BIT_C)  /* TCU-28 */
#define TCU_GPIO_29      (TCU_GPIOB + TCU_GPIO_BIT_D)  /* TCU-29 */
#define TCU_GPIO_30      (TCU_GPIOB + TCU_GPIO_BIT_E)  /* TCU-30 */
#define TCU_GPIO_31      (TCU_GPIOB + TCU_GPIO_BIT_F)  /* TCU-31 */

#define TCU_GPIO_0       (TCU_GPIOC + TCU_GPIO_BIT_0)  /* TCU-32 */
#define TCU_GPIO_1       (TCU_GPIOC + TCU_GPIO_BIT_1)  /* TCU-33 */
#define TCU_GPIO_2       (TCU_GPIOC + TCU_GPIO_BIT_2)  /* TCU-34 */
#define TCU_GPIO_3       (TCU_GPIOC + TCU_GPIO_BIT_3)  /* TCU-35 */
#define TCU_GPIO_4       (TCU_GPIOC + TCU_GPIO_BIT_4)  /* TCU-36 */
#define TCU_GPIO_5       (TCU_GPIOC + TCU_GPIO_BIT_5)  /* TCU-37 */
#define TCU_GPIO_6       (TCU_GPIOC + TCU_GPIO_BIT_6)  /* TCU-38 */
#define TCU_GPIO_7       (TCU_GPIOC + TCU_GPIO_BIT_7)  /* TCU-39 */
#define TCU_GPIO_8       (TCU_GPIOC + TCU_GPIO_BIT_8)  /* TCU-40 */
#define TCU_GPIO_9       (TCU_GPIOC + TCU_GPIO_BIT_9)  /* TCU-41 */
#define TCU_GPIO_10      (TCU_GPIOC + TCU_GPIO_BIT_A)  /* TCU-42 */
#define TCU_GPIO_11      (TCU_GPIOC + TCU_GPIO_BIT_B)  /* TCU-43 */
#define TCU_GPIO_12      (TCU_GPIOC + TCU_GPIO_BIT_C)  /* TCU-44 */
#define TCU_GPIO_14      (TCU_GPIOC + TCU_GPIO_BIT_D)  /* TCU-45 */
#define TCU_GPIO_15      (TCU_GPIOC + TCU_GPIO_BIT_E)  /* TCU-46 */
#define TCU_GPIO_16      (TCU_GPIOC + TCU_GPIO_BIT_F)  /* TCU-47 */


/* M_CONTROL register Bit definitions*/
#define TCU_RESET           0x01
#define TCU_FLUSH           0x02
#define TCU_FAST_STEP       0x04

#define TCU_LOG_ENABLE      0x08

#define TCU_SYNC_TDQ_0   0
#define TCU_SYNC_TDQ_1   1
#define TCU_SYNC_TDQ_2   2
#define TCU_ASYNC_TDQ    3     /* This is the one ASYNC queue for bg tasks to use*/
#define TCU_SYNC_TDQ_4   4
#define TCU_SYNC_TDQ_5   5
#define TCU_SYNC_TDQ_6   6
#define TCU_SYNC_TDQ_7   7

#define TCU_INVALID_FRAME_INT  0xFFFF


/* Translate from the TcuIndex enumeration to the bit + bank value used as an
 * index in the other TCU access macros
 */
#define M_DLTcuXlateIndex(tCU) ((((tCU) & 0x30)<<9) | (((tCU) & 0x0f)<<8))

#if defined (UPGRADE_ENHANCED_PLK)
#define DSP_ASYNC_RI_ID_PRI_1     1
#define DSP_ASYNC_RI_ID_PRI_3     3
#define DSP_ASYNC_RI_ID_PRI_5     5
#define DSP_ASYNC_RI_ID_PRI_7     7
#define DSP_ASYNC_RI_ID_PRI_9     9
#define DSP_ASYNC_RI_ID_PRI_11    11
#define DSP_ASYNC_RI_ID_PRI_13    13
#define DSP_ASYNC_RI_ID_PRI_15    15
#endif

/****************************************************************************
 * Types defines used in Prototypes and Globals
 ****************************************************************************/

/* enumerate TCU GPIO/GPO indices */
typedef enum TcuIndexTag
{
    tCU_0,  tCU_1,  tCU_2,  tCU_3,  tCU_4,  tCU_5,  tCU_6,  tCU_7,
    tCU_8,  tCU_9,  tCU_10, tCU_11, tCU_12, tCU_13, tCU_14, tCU_15,
    tCU_16, tCU_17, tCU_18, tCU_19, tCU_20, tCU_21, tCU_22, tCU_23,
    tCU_24, tCU_25, tCU_26, tCU_27, tCU_28, tCU_29, tCU_30, tCU_31,
    tCU_32, tCU_33, tCU_34, tCU_35, tCU_36, tCU_37, tCU_38, tCU_39,
    tCU_40, tCU_41, tCU_42, tCU_43, tCU_44, tCU_45, tCU_46, tCU_47
 } TcuIndex;


/****************************************************************************
 * Global Variables
 ****************************************************************************/

/* dlTcuFrameIntPri
**
** This variable sets the frame interrupt priority (actually the selection of
** SWI). It is ORed with the TCU_MCU_INT value before getting written to
** TCU.
*/
extern Int16  dlTcuFrameIntPri;

#if defined (UPGRADE_ENHANCED_PLK)
/*
 * TDS index for DSP_TID_ASYNC_TASK_INV_TASK.
 */
extern Int16  asyncDspTaskInvTdsIndex;
#endif

/****************************************************************************
 * Global Macros
 ****************************************************************************/

/* M_DLTcuSetFrameInterrupt()
**
** This macro allows the SWI that will be used for the next frame interrupt
** to be set.
*/
#define M_DLTcuSetFrameInterrupt(iNTpRI) dlTcuFrameIntPri = iNTpRI;



/* The TDQ are always defined in the order:
 *                  (pTdq+0)    ShmInt16 tdq_N_writeIndex
 *                  (pTdq+1)    ShmInt16 tdq_N_writeDummy  (for allowing 32 bit writes)
 *                  (pTdq+2)    ShmInt16 tdq_N_length
 *                              (this contains recSize (in top Byte), length (in bottom Byte))
 *                  (pTdq+3)    ShmInt16 tdq_N_readIndex
 *                  (pTdq+4)    TcuTaskDef tdq_N_entry[ ] */


/*******************************************************************************
 *
 * @Define:         M_WriteAsyncTdq @
 *
 * @Interface:      PLK/M TDQ@
 *
 * @Description:    This service loads the the TDQ with a DSP task that is
 *                  required to be scheduled immediately and informs the DSP
 *                  (PLK/D) that the Asynchronous TDQ is loaded with a new
 *                  event by cross asserting a software interrupt. TDQ's are
 *                  consecutive entries within L1DspShm.
 *                  Parameters:     task    TASK ID
 *                                  data    TDS data for TASK @
 *
 ******************************************************************************/
#define M_WriteAsyncTdq( tASK, dATA)                                   \
{                                                                      \
    ShmInt16 temp;                                                     \
    ShmInt16 writeIndex;                                               \
    TcuTaskDef *pEntry;                                                \
    /* Index into TDQ lookup table to get ptr to write index queue */  \
    ShmInt16 *pTdq   = tdqLookupTable[TCU_ASYNC_TDQ];                  \
    KiDisableInterrupts();                                                    \
    pEntry = (TcuTaskDef*) (pTdq + 4 + *pTdq);                         \
    pEntry->taskId  = (tASK);                                          \
    pEntry->tdsData = (dATA);                                          \
    /* update the new TcuTaskDef via the mirror if there is one */     \
    M_WriteToDsp (pEntry, sizeof(TcuTaskDef));                         \
    /* read the recSize & length info from mirror if there is one */   \
    M_ReadFromDsp (pTdq+2, sizeof(ShmInt16));                          \
    temp = *(pTdq+2);                                                  \
    writeIndex = (*pTdq + (temp >> 8));                                \
    /* increment the writeIndex by recSize, and test wrap length */    \
    if ( writeIndex >= (temp & 0x00FF) )                               \
    {                                                                  \
      writeIndex = 0;                                                  \
    }                                                                  \
    /* check to see if the queue is FULL, (new wrIndex == rdIndex) */  \
    /* Read the readIndex from the DSP mirror if there is one */       \
    M_ReadFromDsp (pTdq+3, sizeof(ShmInt16));                          \
    DevCheck ((writeIndex != *(pTdq+3)), pEntry->taskId, TCU_ASYNC_TDQ, writeIndex); \
    /* Update the queue with the new writeIndex */                     \
    *pTdq = writeIndex;                                                \
    /* Update the new writeIndex in the mirror if there is one */      \
    M_WriteToDsp (pTdq, sizeof(ShmInt16));                             \
    /* Assert the relevant SWI */                                      \
    dlCrossAssertSwi(TCU_ASYNC_TDQ);                                   \
    KiEnableInterrupts();                                                     \
}

#if defined (UPGRADE_ENHANCED_PLK)
/*******************************************************************************
 *
 * @Define:         M_FrxExeRemAction @
 *
 * @Interface:      PLK/M TDQ@
 *
 * @Parameter: dSPrEGiNTpRIORITY - Priority must take one of the following values
 *                                 DSP_ASYNC_RI_ID_PRI_1  DSP_ASYNC_RI_ID_PRI_9
 *                                 DSP_ASYNC_RI_ID_PRI_3  DSP_ASYNC_RI_ID_PRI_11
 *                                 DSP_ASYNC_RI_ID_PRI_5  DSP_ASYNC_RI_ID_PRI_13
 *                                 DSP_ASYNC_RI_ID_PRI_7  DSP_ASYNC_RI_ID_PRI_15
 * @Parameter: tASK              - DSP task Id
 * @Parameter: dATA              - TDS pointer. Offset to base of shared memory
 *                                 Must be filled in using DSP_PTR macro
 * @Parameter: cONVERTpTRfLAG    - Set it always to FALSE. This parameter is not
 *                                 use on the PLK implemenation of this macro.
 *
 * @Description:    This service loads the the TDQ with a DSP task that is
 *                  required to be scheduled immediately and informs the DSP
 *                  (PLK/D) that the Asynchronous TDQ is loaded with a new
 *                  event by cross asserting a software interrupt. TDQ's are
 *                  consecutive entries within L1DspShm.
 ******************************************************************************/
#define M_FrxExeRemAction(dSPrEGiNTpRIORITY, tASK, dATA, cONVERTpTRfLAG)          \
{                                                                                 \
  Int16  i;                                                                       \
  KiTgtInterruptContext interruptContext;                                                                                \
  M_KiTgtIntContextDisable(interruptContext);                                                                 \
  i = asyncDspTaskInvTdsIndex;                                                    \
                                                                                  \
  M_ReadFromDsp (&L1_SHM_SI.asyncDspTaskInvTds[i].status, (sizeof (ShmInt16))); \
  if (L1_SHM_SI.asyncDspTaskInvTds[i].status == TASK_COMPLETED_OK)              \
  {                                                                               \
      L1_SHM_SI.asyncDspTaskInvTds[i].status = TASK_NOT_STARTED;                \
      asyncDspTaskInvTdsIndex++;                                                  \
      if (asyncDspTaskInvTdsIndex == NUM_ASYNC_DSP_TASK_INV_TDS)                  \
      {                                                                           \
        asyncDspTaskInvTdsIndex = 0;                                              \
      }                                                                           \
      M_KiTgtIntContextRestore(interruptContext);                                                              \
                                                                                  \
      L1_SHM_SI.asyncDspTaskInvTds[i].dspTaskDef.taskId  = tASK;                \
      L1_SHM_SI.asyncDspTaskInvTds[i].dspTaskDef.tdsData = dATA;                \
      L1_SHM_SI.asyncDspTaskInvTds[i].dspRIPriority      = dSPrEGiNTpRIORITY;   \
                                                                                  \
      M_WriteToDsp (&L1_SHM_SI.asyncDspTaskInvTds[i],                           \
          (sizeof (AsyncDspTaskInvTds)));                                         \
      M_WriteAsyncTdq (DSP_TID_ASYNC_TASK_INV_TASK,                               \
          DSP_PTR(&L1_SHM_SI.asyncDspTaskInvTds[i]));                           \
  }                                                                               \
  else                                                                            \
  {                                                                               \
    M_KiTgtIntContextRestore(interruptContext);                                                                \
    DevFail ("No free AsyncDspTaskInvTds");                                       \
  }                                                                               \
}
#endif

/*******************************************************************************
 *
 * @Define:         M_DLTcuBaseAddress @
 *
 * @Interface:      PLK/M TCU@
 *
 * @Description:    TCU Base address configuration @
 *
 ******************************************************************************/
#if !defined (ON_PC)
# define M_DLTcuBaseAddress      (0xC0000000)  /* default case is for reference platform */
#else
    /* CUSTOMER CONFIGURATION API - customer provides base address of TCU */
# define CUSTOMER_TCU_BASE_ADDRESS  0
# define TCU_BLOCK                  0
# define M_DLTcuBaseAddress         (CUSTOMER_TCU_BASE_ADDRESS + TCU_BLOCK + TCU_OFFSET)
#endif

#define TcuBa                M_DLTcuBaseAddress

/****************************************************************************
 * Global TCU DRIVER Macros Functions for PLK/M
 ****************************************************************************/

#if !defined (ON_PC)

extern void cfTcuResetDelay (void);


#if defined (ABP_VERSION_3)

#define M_TcuDrvReadVer()                                                               \
                (Int16)(* (volatile Int16*) (TcuBa + TCU_VERSION))

#define M_TcuDrvSelectCpuMcu()                                                          \
                (* (volatile Int16*) (TcuBa + TCU_CPU_SEL) = 0)

#define M_TcuDrvSelectCpuDsp()                                                          \
                (* (volatile Int16*) (TcuBa + TCU_CPU_SEL) = 1)

#define M_TcuDrvIsCpuSelectedDsp()                                                      \
                ((* (volatile Int16*) (TcuBa + TCU_CPU_SEL)) & 0x0001)

#define M_TcuDrvSelectMcuClkRate(mUL)                                                   \
                (* (volatile Int16*) (TcuBa + TCU_MCLK_RATE) = (mUL & TCU_CLK_RATE_MASK))

#define M_TcuDrvGetMcuClkRate()                                                         \
                ((Int16)(* (volatile Int16*) (TcuBa + TCU_MCLK_RATE)))

#define M_TcuDrvSelectDspClkRate(mUL)                                                   \
                (* (volatile Int16*) (TcuBa + TCU_DSP_RATE) = (mUL & TCU_CLK_RATE_MASK))

#define M_TcuDrvGetDspClkRate()                                                         \
                ((Int16)(* (volatile Int16*) (TcuBa + TCU_DSP_RATE)))

#define M_TcuDrvSelectMonitorModule(mODULE)                                             \
                (* (volatile Int16*) (TcuBa + TCU_MONITOR_SEL) = mODULE)

#define M_TcuDrvSetWriteMode()                                                          \
                (* (volatile Int16*) (TcuBa + TCU_MEM_CTRL) &= (~TCU_MEM_RW_BIT))

#define M_TcuDrvSetReadMode()                                                           \
                (* (volatile Int16*) (TcuBa + TCU_MEM_CTRL) |= TCU_MEM_RW_BIT)

#define M_TcuDrvIsTcuBusy()                                                             \
                ((* (volatile Int16*) (TcuBa + TCU_MEM_CTRL)) & TCU_MEM_BUSY_BIT)

#define M_TcuDrvViolationOccurred()                                                     \
                (((* (volatile Int16*) (TcuBa + TCU_MEM_CTRL)) & TCU_MEM_VIOLATION_MASK) >> 10)

#define M_TcuDrvGetNumInstr()                                                           \
              (*(volatile Int16*) (TcuBa + TCU_NUM_INSTR))
#define M_TcuDrvLoadFifo(Code, DeltaTime)                                               \
                  { while(M_TcuDrvIsTcuBusy());                                         \
                    * (volatile Int16*) (TcuBa + TCU_MEM_IO)   =                        \
                                            (Code | DeltaTime);                         \
                  }
#else

/*******************************************************************************
 *
 * Define:         M_TcuDrvLoadFifo
 *
 * Interface:      PLK/M
 *
 * Description:    Writes a new event to the TCU FIFO.
                    Parameters:
                    Code        Code of TCU event
 *                  DeltaTime   Time relative to last event
 *
 ******************************************************************************/
#define M_TcuDrvLoadFifo(Code, DeltaTime)                                       \
                (* (volatile Int16*) (TcuBa + TCU_FIFO_IN)   =                          \
                                            (Code | DeltaTime));

#endif

#define M_TcuDrvClrGpoA(gPO)                                                           \
                (* (volatile Int16*) (TcuBa + TCU_CLEAR_GPOA) = gPO)
#define M_TcuDrvSetGpoA(gPO)                                                           \
                (* (volatile Int16*) (TcuBa + TCU_SET_GPOA) = gPO)
#define M_TcuDrvClrGpoB(gPO)                                                           \
                (* (volatile Int16*) (TcuBa + TCU_CLEAR_GPOB) = gPO)
#define M_TcuDrvSetGpoB(gPO)                                                           \
                (* (volatile Int16*) (TcuBa + TCU_SET_GPOB) = gPO)


/*******************************************************************************
 *
 * Define:         M_TcuDrvSetNearlyFullLevel
 *
 * Interface:      PLK/M
 *
 * Description:    Sets the level for the Nearly Full exception
 *                  Parameter  level: Nearly full level
 *
 ******************************************************************************/
#define M_TcuDrvSetNearlyFullLevel(level)                                      \
                (* (volatile Int16*) (TcuBa + TCU_NEARLYFULLREG)  = level)

/*******************************************************************************
 *
 * Define:         M_TcuDrvSetNearlyEmptyLevel
 *
 * Interface:      PLK/M
 *
 * Description:    Sets the level for the NearlyEmpty exception.
 *                  Parameter  level: Nearly empty level
 *
 ******************************************************************************/
#define M_TcuDrvSetNearlyEmptyLevel(level)                                     \
                (* (volatile Int16*) (TcuBa + TCU_NEARLYEMPTYREG) = level)

/*******************************************************************************
 *
 * Define:         M_TcuDrvControl
 *
 * Interface:      PLK/M
 *
 * Description:    Sets the TCU control register to control reset, flush, fast
 *                  step and logEnable.
 *                  Parameter  controlRegisterAction: Action mask for control register
 *
 ******************************************************************************/
#define M_TcuDrvControl(controlRegisterAction)                                 \
                (* (volatile Int16*) (TcuBa + TCU_CONTROL)  = controlRegisterAction)

/*******************************************************************************
 *
 * Define:         M_TcuDrvStatus
 *
 * Interface:      PLK/M
 *
 * Description:    Returns the TCU status
 *
 ******************************************************************************/
#define M_TcuDrvStatus()                                                       \
                (* (volatile Int16*) (TcuBa + TCU_STATUS))

/*******************************************************************************
 *
 * Define:         M_TcuDrvSetException
 *
 * Interface:      PLK/M
 *
 * Description:    Enables the exceptions reported in the TCU status register
 *                  Parameter  TcuStatus: defines which exceptions to enable
 *
 ******************************************************************************/
#define M_TcuDrvSetException(TcuStatus)                                    \
                (* (volatile Int16*) (TcuBa + TCU_EXCEPTION_ENABLE)= TcuStatus)

 /*******************************************************************************
 *
 * Define:         M_TcuDrvSetWriteIndex
 *
 * Interface:      PLK/M
 *
 * Description:    Sets the location where the next event will be written
 *                  Parameter  newIindexValue: New index value
 *
 ******************************************************************************/
#define M_TcuDrvSetWriteIndex(newIindexValue)                                  \
                (* (volatile Int16*) (TcuBa + TCU_WRITEIDX) = newIindexValue)

/*******************************************************************************
 *
 * Define:         M_TcuDrvSetReadIndex
 *
 * Interface:      PLK/M
 *
 * Description:    Sets the location where the next event will be read.
 *                  Parameter  newIindexValue: New index value
 *
 ******************************************************************************/
#define M_TcuDrvSetReadIndex(newIindexValue)                                  \
                (* (volatile Int16*) (TcuBa + TCU_READ_IDX)  = newIindexValue)

/*******************************************************************************
 *
 * Define:         M_TcuDrvWriteIndex
 *
 * Interface:      PLK/M
 *
 * Description:    Reads the current TCU write index
 *
 ******************************************************************************/
#define M_TcuDrvWriteIndex()                                                   \
                (* (volatile Int16*) (TcuBa + TCU_WRITEIDX))

/*******************************************************************************
 *
 * Define:         M_TcuDrvReadIndex
 *
 * Interface:      PLK/M
 *
 * Description:    Reads the current read index
 *
 ******************************************************************************/
#define M_TcuDrvReadIndex()                                                   \
                (* (volatile Int16*) (TcuBa + TCU_READ_IDX))

/*******************************************************************************
 *
 * Define:         M_TcuDrvNumInstructions
 *
 * Interface:      PLK/M
 *
 * Description:    Reads the number of instructions in the FIFO
 *
 ******************************************************************************/
#define M_TcuDrvNumInstructions()                                              \
                (* (volatile Int16*) (TcuBa + TCU_NUM_INSTR))

/*******************************************************************************
 *
 * Define:         M_TcuDrvSetPrescale
 *
 * Interface:      PLK/M
 *
 * Description:    Sets the prescaler count, set to N-1 for divide by N.
 *                        Parameter  sCALE Prescale value
 *
 ******************************************************************************/
#define M_TcuDrvSetPrescale(sCALE)                                              \
                (* (volatile Int16*) (TcuBa + TCU_PRESCALE) = sCALE);

/*******************************************************************************
 *
 * Define:         M_TcuDrvPrescale
 *
 * Interface:      PLK/M
 *
 * Description:    Reads the current prescale value
 *
 ******************************************************************************/
#define M_TcuDrvPrescale()                                                     \
                (* (volatile Int16*) (TcuBa + TCU_PRESCALE))

/*******************************************************************************
 *
 * Define:         M_TcuDrvDeltaTimer
 *
 * Interface:      PLK/M
 *
 * Description:    Access the delta timer for debug purposes
 *
 ******************************************************************************/
#define M_TcuDrvDeltaTimer()                                                   \
                (* (volatile Int16*) (TcuBa + TCU_DELTA_TIMER))

/*******************************************************************************
 *
 * Define:         M_TcuDrvReset
 *
 * Interface:      PLK/M
 *
 * Description:    Reset the TCU
 *
 ******************************************************************************/
#define M_TcuDrvReset()                                                                           \
                (* (volatile Int16*) (TcuBa + TCU_CONTROL) |= (TCU_RESET | TCU_LOG_ENABLE));      \
                cfTcuResetDelay();

/*******************************************************************************
 *
 * Define:         M_TcuFirstLoadPreActions
 *
 * Interface:      PLK/M
 *
 * Description:    Perform any actions which may be required before calling the
 *                 sorter for the first time
 *
 ******************************************************************************/
#define M_TcuFirstLoadPreActions()

/*******************************************************************************
 *
 * Define:         M_TcuFirstLoadPostActions
 *
 * Interface:      PLK/M
 *
 * Description:    Perform any actions which may be required after calling the
 *                 sorter for the first time
 *
 ******************************************************************************/
#define M_TcuFirstLoadPostActions()  dlEnableTcuUnderflowWarning()

#else

/* CORE SIM TCU DRIVER MACROS */

#define M_TcuDrvPrescale()                               \
    tcuReadReg(TCU_PRESCALE)
#define M_TcuDrvSetPrescale(sCALE)                       \
    tcuWriteReg(TCU_PRESCALE, sCALE)
#define M_TcuDrvReset()                                  \
    tcuWriteReg(TCU_CONTROL, TCU_RESET)
#define M_TcuDrvLoadFifo(Code, DeltaTime)                \
    tcuWriteReg(TCU_FIFO_IN, Code | DeltaTime)

#define M_TcuDrvSetException(TcuStatus)
#define M_TcuDrvStatus() 0
#define M_TcuFirstLoadPreActions()
#define M_TcuFirstLoadPostActions()

#endif /* ON_PC */

#endif /* UPGRADE_CORE_FR */

#endif
/******************************************************************************
 * End Of File
 *****************************************************************************/
