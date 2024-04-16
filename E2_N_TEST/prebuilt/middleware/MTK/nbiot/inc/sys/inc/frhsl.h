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
 * \ingroup ApiCBEcorePLFdriversFrHsl
 * \ingroup PrdCBEcorePLFdriversFrHsl
 * \brief This is the low-level drivers header file for the FrHsl module.
 * This file contains register definitions and access macros which define
 * an API to the HSL device in the ABP.  The logging routines and functions used
 * by software on the same processor as the HSL, and by software on another
 * processor (via the client/server parts of the module) are built on these
 * low-level routines.
 *
 * The ABP HSL module is documented for ABPv3 in
 * csiabp_3131.doc/.pdf.
 * \see frhslcfg.h for logging routines on the same processor.
 * \see frhslc.h for the client interface.
 * \see frhsls.h for the server interface.
 */

#ifndef FRHSL_H
#define FRHSL_H

/*******************************************************************************
** Includes
*******************************************************************************/

#include <system.h>
#include <frhslcfg.h>

#ifdef  __cplusplus
extern "C" {
#endif

/*******************************************************************************
** Constants
*******************************************************************************/

/** \addtogroup PrdCBEcorePLFdriversFrHsl
 * @{ */

/** \def FR_HSL_FIFO_SIZE
 *  The number of entries in the HSL non interleaved FIFO.
 */
#define FR_HSL_FIFO_SIZE                (16)
/** \def FR_HSL_OFFSET_XXXX
 *  Register offsets for the HSL Controller from the HSL base address.
 */
#if !defined (FR_ABP_2)
/** Offset of HSL version register from HSL base address.
 *  Offset of HSL version register from HSL base address.  Read this to
 *  identify the version of the HSL ABP module available.
 */
# define FR_HSL_OFFSET_VERSION          (0x000E << FR_HSL_ADDR_OFFSET_SHIFT)
#endif

/** Offset of HSL config register from HSL base address.
 *  Offset of HSL config register from HSL base address.  Write to this
 *  register to configure the HSL before use.
 */
#define FR_HSL_OFFSET_CONFIG            (0x0008 << FR_HSL_ADDR_OFFSET_SHIFT)

/** Offset of HSL message end register from HSL base address.
 *  Offset of HSL message end register from HSL base address.  Write the last
 *  word of a multi-word message to this register.
 */
#define FR_HSL_OFFSET_SWMSG_LAST_WORD   (0x0000 << FR_HSL_ADDR_OFFSET_SHIFT)

/** Offset of HSL message body register from HSL base address.
 *  Offset of HSL message body register from HSL base address.  Write the
 *  middle words (but not the first or last) of a multi-word message to
 *  this register.
 */
#define FR_HSL_OFFSET_SWMSG_BODY        (0x0002 << FR_HSL_ADDR_OFFSET_SHIFT)

/** Offset of HSL message start from HSL base address.
 *  Offset of HSL message start from HSL base address.  Write the first
 *  word of a multi-word message to this register
 */
#define FR_HSL_OFFSET_SWMSG_HEADER      (0x0004 << FR_HSL_ADDR_OFFSET_SHIFT)

/** Offset of HSL single-word message register from HSL base address.
 *  Offset of HSL single-word message register from HSL base address.
 *  Write a single-word message to this register.
 */
#define FR_HSL_OFFSET_SWMSG_SINGLE      (0x0006 << FR_HSL_ADDR_OFFSET_SHIFT)

/** Offset of HSL FIFO nearly full register from HSL base address.
 *  Offset of HSL FIFO mearly full register from HSL base address.  Sets the
 *  high-water mark in the HSL FIFO to generate a flow-control HWI.
 */
#define FR_HSL_OFFSET_NEARLY_FULL       (0x000A << FR_HSL_ADDR_OFFSET_SHIFT)

/** Offset of HSL clear IRQ register from HSL base address.
 *  Offset of HSL clear IRQ register from HSL base address. Write to this
 *  register to clear the flow-control HWI.
 */
#define FR_HSL_OFFSET_CLEAR_IRQ         (0x000C << FR_HSL_ADDR_OFFSET_SHIFT)

/** Offset of HSL SW Enable 0 register from HSL base address.
 *  Offset of HSL SW Enable 0 register from HSL base address.  Write to this
 *  register to configure the enabled type 0 log-points.
 */
#define FR_HSL_OFFSET_SW_ENABLE_0       (0x0010 << FR_HSL_ADDR_OFFSET_SHIFT)

/** Offset of HSL SW Enable 1 register from HSL base address.
 *  Offset of HSL SW Enable 1 register from HSL base address.  Write to this
 *  register to configure the enabled type 1 log-points.
 */
#define FR_HSL_OFFSET_SW_ENABLE_1       (0x0012 << FR_HSL_ADDR_OFFSET_SHIFT)

/** Offset of HSL SW Enable 2 register from HSL base address.
 *  Offset of HSL SW Enable 2 register from HSL base address.  Write to this
 *  register to configure the enabled type 2 log-points.
 */
#define FR_HSL_OFFSET_SW_ENABLE_2       (0x0014 << FR_HSL_ADDR_OFFSET_SHIFT)

/** Offset of HSL SW Enable 3 register from HSL base address.
 *  Offset of HSL SW Enable 3 register from HSL base address.  Write to this
 *  register to configure the enabled type 3 log-points.
 */
#define FR_HSL_OFFSET_SW_ENABLE_3       (0x0016 << FR_HSL_ADDR_OFFSET_SHIFT)

/** Offset of HSL TCU Enable register from HSL base address.
 *  Offset of HSL TCU Enable register from HSL base address.  Write to this
 *  register to configure the enabled TCU Event log-points.
 */
#define FR_HSL_OFFSET_TCU_ENABLE        (0x0018 << FR_HSL_ADDR_OFFSET_SHIFT)

/** Offset of HSL ??? register from HSL base address.
 *  Offset of HSL ??? register from HSL base address.  Write to this
 *  register to ???.
 */
#define FR_HSL_OFFSET_NESTING_STACK     (0x001A << FR_HSL_ADDR_OFFSET_SHIFT)

#if !defined (FR_ABP_2)
/** Offset of HSL MCU reserve register from HSL base address.
 *  Offset of HSL MCU reserve register from HSL base address.  Write to this
 *  register to request MCU control of the HSL.
 */
# define FR_HSL_OFFSET_MCU_RESERVE      (0x001C << FR_HSL_ADDR_OFFSET_SHIFT)

/** Offset of HSL DSP reserve register from HSL base address.
 *  Offset of HSL DSP reserve register from HSL base address.  Write to this
 *  register to request DSP control of the HSL.
 */
# define FR_HSL_OFFSET_DSP_RESERVE      (0x001E << FR_HSL_ADDR_OFFSET_SHIFT)
#endif

#if defined (UPGRADE_FR_HSL_VERSION2)

/** \ingroup CfgCBEcorePLFdriversFrHsl
 * \def FR_HSL_V2_FILTERED_LOG_POINT_COMMON_MSB
 * Common MSB 2 bits for all filtered log points.
 * Common MSB 2 bits for all filtered log points.
 * All filtered log points will have the 2 most significant bits
 * set.
 */
#  define FR_HSL_V2_FILTERED_LOG_POINT_COMMON_MSB  ((Int16)0xC000)


/** \ingroup CfgCBEcorePLFdriversFrHsl
 * \def FR_HSL_V2_MAX_NUMBER_FILTER_LOG_POINTS
 * Max number of log points for which run-time filtering can be applied
 * Max number of log points for which run-time filtering can be applied. No more
 * than this number of log points can have run-time filtering applied. The
 * reason for this limit is linked to the size of the Int16 array which is used to
 * maintain the enable/disable state for each such log point.
 */
#  define FR_HSL_V2_MAX_NUMBER_FILTERED_LOG_POINTS  ((Int16)1024)

/** \ingroup CfgCBEcorePLFdriversFrHsl
 * \def FR_HSL_DYNAMIC_LOGGING_ARRAY_SIZE
 * Max size of FrHslDynamicLogPointArray[].
 * Max size of FrHslDynamicLogPointArray[].
 */
#  define FR_HSL_DYNAMIC_LOGGING_ARRAY_SIZE ((Int16)(FR_HSL_V2_MAX_NUMBER_FILTERED_LOG_POINTS/(sizeof(Int16)*8)))  /* Factor of 8 for number of bits per byte */


/** \ingroup CfgCBEcorePLFdriversFrHsl
 * \def FR_HSL_V2_FILTERED_LOG_POINT_ID_RANGE
 * Log point ID Mask for allowed range of filtered log point IDs
 * Log point ID Mask for allowed range of filtered log point IDs
 * This represents the allowed bits that can be set barred the common MSB
 */
#  define FR_HSL_V2_FILTERED_LOG_POINT_ID_RANGE   ((Int16)(FR_HSL_V2_MAX_NUMBER_FILTERED_LOG_POINTS-1)) /* -1 to give 0x03FF */

/** \ingroup CfgCBEcorePLFdriversFrHsl
 * \def FR_HSL_V2_FILTERED_HEADER_LOG_POINT_ID_MASK
 * Mask giving the allowed bits that can be set in a log point ID for which run-time filtering is applied.
 * Mask giving the allowed bits that can be set in a log point ID for which run-time filtering is applied.
 */
#  define FR_HSL_V2_FILTERED_HEADER_LOG_POINT_ID_MASK (FR_HSL_V2_FILTERED_LOG_POINT_COMMON_MSB | FR_HSL_V2_FILTERED_LOG_POINT_ID_RANGE)
#endif


/** \def FR_HSL_ADDR_XXXX
 *  Register addresses for the HSL Controller.
 */
#if !defined (FR_ABP_2)
/** Address of HSL version register.
 *  Address of HSL version register.  Read this to
 *  identify the version of the HSL ABP module available.
 */
# define FR_HSL_ADDR_VERSION        (FR_HSL_BASE_ADDR + FR_HSL_OFFSET_VERSION)

/** Address of HSL DSP reserve register from HSL base address.
 *  Address of HSL DSP reserve register from HSL base address.  Write to this
 *  register to request DSP control of the HSL.
 */
# define FR_HSL_ADDR_DSP_RESERVE    (FR_HSL_BASE_ADDR+ FR_HSL_OFFSET_DSP_RESERVE)

/** Address of HSL MCU reserve register from HSL base address.
 *  Address of HSL MCU reserve register from HSL base address.  Write to this
 *  register to request MCU control of the HSL.
 */
# define FR_HSL_ADDR_MCU_RESERVE    (FR_HSL_BASE_ADDR+ FR_HSL_OFFSET_MCU_RESERVE)

#endif

/** Address of HSL config register.
 *  Address of HSL config register.  Write to this
 *  register to configure the HSL before use.
 */
#define FR_HSL_ADDR_CONFIG          (FR_HSL_BASE_ADDR + FR_HSL_OFFSET_CONFIG)

/** Address of HSL message end register.
 *  Address of HSL message end register.  Write the last
 *  word of a multi-word message to this register.
 */
#define FR_HSL_ADDR_SWMSG_LAST_WORD (FR_HSL_BASE_ADDR + FR_HSL_OFFSET_SWMSG_LAST_WORD)

/** Address of HSL message body register.
 *  Address of HSL message body register.  Write the
 *  middle words (but not the first or last) of a multi-word message to
 *  this register.
 */
#define FR_HSL_ADDR_SWMSG_BODY      (FR_HSL_BASE_ADDR + FR_HSL_OFFSET_SWMSG_BODY)

/** Address of HSL message start.
 *  Address of HSL message start.  Write the first
 *  word of a multi-word message to this register.
 */
#define FR_HSL_ADDR_SWMSG_HEADER    (FR_HSL_BASE_ADDR + FR_HSL_OFFSET_SWMSG_HEADER)

/** Address of HSL single-word message register.
 *  Address of HSL single-word message register.
 *  Write a single-word message to this register.
 */
#define FR_HSL_ADDR_SWMSG_SINGLE    (FR_HSL_BASE_ADDR + FR_HSL_OFFSET_SWMSG_SINGLE)

/** Address of HSL FIFO nearly full register.
 *  Address of HSL FIFO mearly full register.  Sets the
 *  high-water mark in the HSL FIFO to generate a flow-control HWI.
 */
#define FR_HSL_ADDR_NEARLY_FULL     (FR_HSL_BASE_ADDR + FR_HSL_OFFSET_NEARLY_FULL)

/** Address of HSL clear IRQ register.
 *  Address of HSL clear IRQ register. Write to this
 *  register to clear the flow-control HWI.
 */
#define FR_HSL_ADDR_CLEAR_IRQ       (FR_HSL_BASE_ADDR + FR_HSL_OFFSET_CLEAR_IRQ)

/** Address of HSL SW Enable 0 register.
 *  Address of HSL SW Enable 0 register.  Write to this
 *  register to configure the enabled type 0 log-points.
 */
#define FR_HSL_ADDR_SW_ENABLE_0     (FR_HSL_BASE_ADDR + FR_HSL_OFFSET_SW_ENABLE_0)

/** Address of HSL SW Enable 1 register.
 *  Address of HSL SW Enable 1 register.  Write to this
 *  register to configure the enabled type 1 log-points.
 */
#define FR_HSL_ADDR_SW_ENABLE_1     (FR_HSL_BASE_ADDR + FR_HSL_OFFSET_SW_ENABLE_1)

/** Address of HSL SW Enable 2 register.
 *  Address of HSL SW Enable 2 register.  Write to this
 *  register to configure the enabled type 2 log-points.
 */
#define FR_HSL_ADDR_SW_ENABLE_2     (FR_HSL_BASE_ADDR + FR_HSL_OFFSET_SW_ENABLE_2)

/** Address of HSL SW Enable 3 register.
 *  Address of HSL SW Enable 3 register.  Write to this
 *  register to configure the enabled type 3 log-points.
 */
#define FR_HSL_ADDR_SW_ENABLE_3     (FR_HSL_BASE_ADDR + FR_HSL_OFFSET_SW_ENABLE_3)

/** Address of HSL TCU Enable register.
 *  Address of HSL TCU Enable register.  Write to this
 *  register to configure the enabled TCU Event log-points.
 */
#define FR_HSL_ADDR_TCU_ENABLE      (FR_HSL_BASE_ADDR + FR_HSL_OFFSET_TCU_ENABLE)

/** Address of HSL stack register.
 *  Address of HSL stack register.  Read this to
 *  see the nesting state for long messages.
 */
#define FR_HSL_ADDR_NESTING_STACK   (FR_HSL_BASE_ADDR + FR_HSL_OFFSET_NESTING_STACK)


/** Size of the Int8 array that is used in the PC simulation of the
 *  HSL Controller. Size of the Int8 array that is used in the PC
 *  simulation of the HSL Controller. Must be big enough for all the
 *  registers (doesn't matter if it is too big).
 */
#define FR_HSL_PC_REGISTER_ARRAY_SIZE   (0x20 << FR_HSL_ADDR_OFFSET_SHIFT)

/** The number of SW Enable registers within the HSL module.
 *  The number of SW Enable registers within the HSL module.  Used in the
 *  ADS for the HSL configuration action.
 */
#define FR_HSL_NUM_SW_ENABLE_REGS       (4)

/** @} */ /* End of addtogroup PrdCBEcorePLFdriversFrHsl */


/** \addtogroup ApiCBEcorePLFdriversFrHsl
 * @{ */

/** Bit in the HSL configuration register to enable the HSL port.
 *  Bit in the HSL configuration register to enable the HSL port.  This should be
 *  set at initialisation.
 */
#define FR_HSL_CONFIG_PORT_ENABLE_BIT   (0x01)

/** Bit in the HSL configuration register to enable TCU logging.
 *  Bit in the HSL configuration register to enable TCU logging.  This should be
 *  set at initialisation.
 */
#define FR_HSL_CONFIG_TCU_LOG_BIT       (0x08)

/** Bit in the HSL configuration register to enable software logpoints.
 *  Bit in the HSL configuration register to enable software logpoints.  This
 *  should be set at initialisation.
 */
#define FR_HSL_CONFIG_SW_LOG_BIT        (0x10)

/** Bit in the HSL configuration register to disable nested logpoints.
 *  Bit in the HSL configuration register to disable nested logpoints.  This
 *  should not be set.
 */
#define FR_HSL_CONFIG_NEST_DISABLE_BIT  (0x20)

/** In the HSL configuration register, set the output clock frequency to the DSP
 *  clock divided by 1.
 *  Value to write to the bitfield 2:1 in the HSL configuration register to set
 *  the HSL output clock frequency to the DSP clock divided by 1.  This maximises
 *  the HSL throughput (and therefore minimises delays to the DSP), but requires
 *  the highest driving of the HSL output pins.
 *  \see FR_HSL_CONFIG_CLK_DIVIDE_8_BITS
 */
#define FR_HSL_CONFIG_CLK_DIVIDE_1_BITS (0x00)

/** In the HSL configuration register, set the output clock frequency to the DSP
 *  clock divided by 2.
 *  Value to write to the bitfield 2:1 in the HSL configuration register to set
 *  the HSL output clock frequency to the DSP clock divided by 2.
 * \see FR_HSL_CONFIG_CLK_DIVIDE_1_BITS, FR_HSL_CONFIG_CLK_DIVIDE_8_BITS.
 */
#define FR_HSL_CONFIG_CLK_DIVIDE_2_BITS (0x02)

/** In the HSL configuration register, set the output clock frequency to the DSP
 *  clock divided by 4.
 *  Value to write to the bitfield 2:1 in the HSL configuration register to set
 *  the HSL output clock frequency to the DSP clock divided by 4.
 * \see FR_HSL_CONFIG_CLK_DIVIDE_1_BITS, FR_HSL_CONFIG_CLK_DIVIDE_8_BITS.
 */
#define FR_HSL_CONFIG_CLK_DIVIDE_4_BITS (0x04)

/** In the HSL configuration register, set the output clock frequency to the DSP
 *  clock divided by 8.
 *  Value to write to the bitfield 2:1 in the HSL configuration register to set
 *  the HSL output clock frequency to the DSP clock divided by 8.  This minimises
 *  the driving required by the HSL output pins, but also minimises HSL throughput,
 *  and may increase delays to the DSP.
 * \see FR_HSL_CONFIG_CLK_DIVIDE_1_BITS.
 */
#define FR_HSL_CONFIG_CLK_DIVIDE_8_BITS (0x06)

/** Defines the mask required to extract only the clock divide bits from the
 *  HSL configuration register.
 *  Defines the mask required to extract only the clock divide bits from the
 *  HSL configuration register.
 */
#define FR_HSL_CONFIG_CLK_DIVIDE_MASK   (0x06)

/*******************************************************************************
** Macros
*******************************************************************************/

#if !defined (FR_ABP_2)
/** Reads the HSL version register.
 *  Reads the HSL version register.
 */
#define M_FrHslReadVersion()            M_FrHslReadInt16Reg(FR_HSL_ADDR_VERSION)

/** Reads the MCU Reserve register.
 *  Reads the MCU Reserve register, a non zero value indicates the MCU has
 *  control of the HSL, otherwise the DSP (PLP) has control.
 */
#define M_FrHslReadMcuReserve()         M_FrHslReadInt16Reg(FR_HSL_ADDR_MCU_RESERVE)

/** Writes 1 to the MCU reserve register to request control of the HSL.
 *  Writes 1 to the MCU reserve register to request control of the HSL.
 *  Writing a non-zero value requests control of the HSL for the MCU,
 *  when control is given the MCU reserve count will be increased by 1 (this
 *  can be read using M_FrHslReadMcuReserve(), any non zero value indicates
 *  that control is given).
 */
#define M_FrHslIncMcuReserve()          M_FrHslWriteInt16Reg(FR_HSL_ADDR_MCU_RESERVE, 1)

/** Writes 0 to the MCU reserve register.
 *  Writes 0 to the MCU reserve register. Writing a zero value
 *  decreases the MCU reserve count by 1 (this can be read using
 *  M_FrHslReadMcuReserve()). When MCU reserve count reaches zero
 *  the MCU no longer has control of the HSL.
 */
#define M_FrHslDecMcuReserve()          M_FrHslWriteInt16Reg(FR_HSL_ADDR_MCU_RESERVE, 0)

/** Reads the DSP Reserve register.
 *  Reads the DSP Reserve register, a non zero value indicates the DSP (PLP)
 *  has control of the HSL, otherwise the MCU has control.
 */
#define M_FrHslReadDspReserve()         M_FrHslReadInt16Reg(FR_HSL_ADDR_DSP_RESERVE)

/** Writes 1 to the DSP reserve register to request control of the HSL.
 *  Writes 1 to the DSP reserve register to request control of the HSL.
 *  Writing a non-zero value requests control of the HSL for the DSP
 *  (PLP), when control is given the DSP reserve count will be
 *  increased by 1 (this can be read using M_FrHslReadDspReserve(),
 *  any non zero value indicates that control is given).
 */
#define M_FrHslIncDspReserve()          M_FrHslWriteInt16Reg(FR_HSL_ADDR_DSP_RESERVE, 1)

/** Writes 0 to the DSP reserve register.
 *  Writes 0 to the DSP reserve register. Writing a zero value
 *  decreases the DSP reserve count by 1 (this can be read using
 *  M_FrHslReadDspReserve()). When DSP reserve count reaches zero
 *  the DSP (PLP) no longer has control of the HSL.
 */
#define M_FrHslDecDspReserve()          M_FrHslWriteInt16Reg(FR_HSL_ADDR_DSP_RESERVE, 0)


#endif /* FR_ABP_2 */

/** Writes to the HSL config register.
 *  Writes to the HSL config register - use at system initialisation.
 */
#define M_FrHslWriteConfig(cONFIG)      M_FrHslWriteInt16Reg(FR_HSL_ADDR_CONFIG, cONFIG, 0 /* not used*/)

/** Reads from the HSL config register.
 *  Reads from the HSL config register.
 */
#define M_FrHslReadConfig()             M_FrHslReadInt16Reg(FR_HSL_ADDR_CONFIG)

/** Writes to the HSL Nearly Full register.
 *  Writes to the HSL Nearly Full register.
 */
#define M_FrHslWriteNearlyFull(vALUE)   M_FrHslWriteInt16Reg(FR_HSL_ADDR_NEARLY_FULL, vALUE, 0 /* not used */)

/** Reads from the HSL Nearly Full register.
 *  Reads from the HSL Nearly Full register.
 */
#define M_FrHslReadNearlyFull()         M_FrHslReadInt16Reg(FR_HSL_ADDR_NEARLY_FULL)

/** Writes to the HSL TCU Enable register.
 *  Writes to the HSL TCU Enable register, each bit enables logging
 *  of the corresponding TCU instruction.
 */
#define M_FrHslWriteTcuEnable(vALUE)    M_FrHslWriteInt16Reg(FR_HSL_ADDR_TCU_ENABLE, vALUE, 0 /* not used */)

/** Reads from the HSL TCU Enable register.
 *  Reads from the HSL TCU Enable register. \see M_FrHslWriteTcuEnable.
 */
#define M_FrHslReadTcuEnable()          M_FrHslReadInt16Reg(FR_HSL_ADDR_TCU_ENABLE)

/** Writes to one of the HSL SW Enable registers.
 *  Writes to the given HSL SW Enable registers.  These registers allow
 *  hardware to filter out HSL messages based on their headers.
 *  \param[in] Int8 rEG = SwEnable register to write to: 0 - FR_HSL_NUM_SW_ENABLE_REGS
 *  \param[in] Int16 vALUE = value to write to register: 0 - 0x3FFF
 */
#define M_FrHslWriteSwEnable(rEG, vALUE)    M_FrHslWriteInt16Reg(FR_HSL_ADDR_SW_ENABLE_0 +\
                                              ((FR_HSL_OFFSET_SW_ENABLE_1 - FR_HSL_OFFSET_SW_ENABLE_0) * (rEG)), vALUE, 0 /* not used */)

/** Enable HSL logging of a specific TCU command.
 *  Enable HSL logging of a specific TCU command.  This must be one of the
 *  FR_TCU_CMD_XXXX defines from FRTCU.H.
 *  \param[in] Int8 tCUcMD = TCU command to enable logging for.
 */
#define M_FrHslEnableLogTcuCmd(tCUcMD)  M_FrHslWriteInt16Reg(FR_HSL_ADDR_TCU_ENABLE, (M_FrHslReadTcuEnable() | (1 << M_FrTcuCmdToDec(tCUcMD))))

/** Disable HSL logging of a specific TCU command.
 *  Disable HSL logging of a specific TCU command.  This must be one of the
 *  FR_TCU_CMD_XXXX defines from FRTCU.H.
 *  \param[in] Int8 tCUcMD = TCU command to disable logging for.
 */
#define M_FrHslDisableLogTcuCmd(tCUcMD) M_FrHslWriteInt16Reg(FR_HSL_ADDR_TCU_ENABLE, (M_FrHslReadTcuEnable() & (~(1 << M_FrTcuCmdToDec(tCUcMD)))))

/** Clear the HSL IRQ.
 *  Clear the HSL (flow-control) IRQ.  This is done by writing any value to the
 *  HSL Clear IRQ register.
 */
#define M_FrHslClearIrq()               M_FrHslWriteInt16Reg(FR_HSL_ADDR_CLEAR_IRQ, 0, 0)


/** Writes to the HSL SWMsg Last Word register.
 *  Writes the last word of a multi-word message to this register.
 */
#if !defined(M_FrHslWriteSwMsgLastWord)
# define M_FrHslWriteSwMsgLastWord(vALUE, vALUE2)   M_FrHslWriteInt16Reg(FR_HSL_ADDR_SWMSG_LAST_WORD, vALUE, vALUE2)
#endif
/** Writes to the HSL SWMsg Body register.
 *  Writes the middle words (but not the first or last) of a multi-word message to
 *  this register.
 */
#if !defined(M_FrHslWriteSwMsgBody)
#  define M_FrHslWriteSwMsgBody(vALUE, vALUE2)      M_FrHslWriteInt16Reg(FR_HSL_ADDR_SWMSG_BODY, vALUE, vALUE2)
#endif

/** Writes to the HSL SWMsg Header register.
 *  Writes the first word of a multi-word message to this register.
 */
#if !defined(M_FrHslWriteSwMsgHeader)
# define M_FrHslWriteSwMsgHeader(vALUE, len)       M_FrHslWriteInt16Reg(FR_HSL_ADDR_SWMSG_HEADER, len, vALUE)
#endif

/** Writes to the HSL SWMsg Single register.
 *  Writes single-word logpoints to this register.  Note that the LG_SINGLE_WORD
 *  bit (0x2000) must be set in the header value written.  This is essential
 *  to allow the message nesting scheme to operate correctly.
 */
#if !defined(M_FrHslWriteSwMsgSingle)
# define M_FrHslWriteSwMsgSingle(vALUE)     M_FrHslWriteInt16Reg(FR_HSL_ADDR_SWMSG_SINGLE, 0, vALUE) /* one word is currently unused is currently unused in ULS */
#endif

/** Macro to extract the least significant 16 bits of a 32 bit value.
 *
 *  \param[in] iNT32vALUE - Int32 value to extract from.
 *
 *  \retval    Least significant 16 bits from the input value.
 */
#define M_FrHslPrintInt32Lo(iNT32vALUE)     (Int16)(((Int32)(iNT32vALUE)) & 0x0000ffff)

/** Macro to extract the most significant 16 bits of a 32 bit value.
 *
 *  \param[in] iNT32vALUE - Int32 value to extract from.
 *
 *  \retval    Most significant 16 bits from the input value.
 */
#define M_FrHslPrintInt32Hi(iNT32vALUE)     (Int16)((((Int32)(iNT32vALUE)) & 0xffff0000) >> 16)


/*******************************************************************************
** Global Function Prototypes
*******************************************************************************/

/** \ingroup ApiCBEcorePLFservicesFrHsl
 *  This function is called to initialise the HSL controller.
 *  This function is called to initialise the HSL controller.
 */
void FrHslInitialise(void);
void FrHslString(Int16 hDR, const char *aRRAY);


void FrHslDynamicLogPointEnable(Int16);
void FrHslDynamicLogPointDisable(Int16);
void FrHslDynamicLogPointEnableAll(void);
void FrHslDynamicLogPointDisableAll(void);
Boolean FrHslDynamicLogPointTest(Int16, Int16);
void FrHslV2ApplySingleLogPointFiltering(Int16 lgHdr, Boolean enableState);
void FrHslV2ApplyMultipleLogPointsFilter(Int16* lgHdr_p, Boolean* enableState_p, Int16 sizeArray);
void FrHslV2InitialiseLogPointFiltering(void);


void FrHslGetOverflowOccurredNumber(void);

#if defined HSL_OVERFLOW_INT_DISABLED
#define SHOW_FLOW_CONTROL_ISR_STATUS FrHslGetOverFlowStatus()
#else
#define SHOW_FLOW_CONTROL_ISR_STATUS FrHslGetOverflowOccurredNumber()
#endif

/* The status bit Fifo Empty did not exist on previous version of HSL
 * Make the macro exist and return Empty to avoid build issue with (ENABLE_PLTSIM) */
#define M_FrHslFifoEmpty()                  TRUE



/** @} */ /* End of addtogroup ApiCBEcorePLFdriversFrHsl */

/*******************************************************************************
** Include PLK Backwards compatibility header here as it may need some of the
** above definitions
*******************************************************************************/
#include <frhslbc.h>

#ifdef  __cplusplus
}
#endif

#endif
/* END OF FILE */
