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

/*---------------------------------------------------------------------------
 | Filename:
 | ---------
 |   fruls2625.h
 |
 | Project:
 | --------
 |   VALCYRIE
 |
 | Description:
 | ------------
 |   Unified Logging System (ULS) HW definition for chip MT2526
 |
 | Author:
 | -------
 |   mtk13615
 |
 |---------------------------------------------------------------------------
 | $Log$
 |
 ---------------------------------------------------------------------------*/

#ifndef _FR_ULS_2625_H_
#define _FR_ULS_2625_H_

/*
| base addresses
*/
#define ULS_ABP_BASE_ADDR (0xC8012000)

#define REG_ULS_TIMESTAMP_MODE	                        (ULS_ABP_BASE_ADDR + 0x0)   /* WO */
#define REG_ULS_TIMESTAMP_INSERT	                (ULS_ABP_BASE_ADDR + 0x4)   /* RO */
#define REG_ULS_OUTPUT_BUFFER_FLUSH	                (ULS_ABP_BASE_ADDR + 0x8)   /* RW */
#define REG_ULS_EXTMEM_START_ADDRESS	                (ULS_ABP_BASE_ADDR + 0xC)   /* RW */
#define REG_ULS_EXTMEM_END_ADDRESS	                (ULS_ABP_BASE_ADDR + 0x10)  /* RW */
#define REG_ULS_EXTMEM_FILL_MODE	                (ULS_ABP_BASE_ADDR + 0x14)  /* RW */
#define REG_ULS_EXTMEM_POSTED_WRITE_POINTER	        (ULS_ABP_BASE_ADDR + 0x18)  /* RO */
#define REG_ULS_EXTMEM_COMPLETED_WRITE_POINTER          (ULS_ABP_BASE_ADDR + 0x1C)  /* RW */
#define REG_ULS_EXTMEM_READ_POINTER	                (ULS_ABP_BASE_ADDR + 0x20)  /* RW */
#define REG_ULS_EXTMEM_BYTE_COUNT	                (ULS_ABP_BASE_ADDR + 0x24)  /* RO */
#define REG_ULS_EXTMEM_BLOCK_SIZE	                (ULS_ABP_BASE_ADDR + 0x28)  /* RW */
#define REG_ULS_EXTMEM_BUFFER_STATUS	                (ULS_ABP_BASE_ADDR + 0x2C)  /* RW */
#define REG_ULS_EXTMEM_SAMPLE_TRESHOLD	                (ULS_ABP_BASE_ADDR + 0x30)  /* RW */
#define REG_ULS_EXTMEM_BURST_LENGTH  	                (ULS_ABP_BASE_ADDR + 0x34)  /* RW */
#define REG_ULS_ENABLE_LOGGING	                        (ULS_ABP_BASE_ADDR + 0x38)  /* RW */
#define REG_ULS_SOFT_RESET             	                (ULS_ABP_BASE_ADDR + 0x3C)  /* WO */
#define REG_ULS_FIFO_MCU_SETUP	                        (ULS_ABP_BASE_ADDR + 0x40)  /* RW */
#define REG_ULS_FIFO_DSP_SETUP	                        (ULS_ABP_BASE_ADDR + 0x44)  /* RW */
#define REG_ULS_FIFO_DFE_SETUP	                        (ULS_ABP_BASE_ADDR + 0x48)  /* RW */
#define REG_ULS_FIFO_FFT_SETUP	                        (ULS_ABP_BASE_ADDR + 0x4C)  /* RW */
#define REG_ULS_INTERRUPT_ENABLE	                (ULS_ABP_BASE_ADDR + 0x50)  /* WO */
#define REG_ULS_INTERRUPT_DISABLE	                (ULS_ABP_BASE_ADDR + 0x54)  /* WO */
#define REG_ULS_ENABLED_INTERRUPTS	                (ULS_ABP_BASE_ADDR + 0x58)  /* RO */
#define REG_ULS_ACTIVE_INTERRUPTS	                (ULS_ABP_BASE_ADDR + 0x5C)  /* RO */
#define REG_ULS_INTERRUPT_CLEAR	                        (ULS_ABP_BASE_ADDR + 0x60)  /* WO */
#define REG_ULS_INTERRUPT_TRIGGER                       (ULS_ABP_BASE_ADDR + 0x64)  /* WO */

/*
| ULS GCR registers
*/
#define ULS_GCR_BASE_ADDR (0xC9000000)

#define REG_ULS_SINGLE                                  (ULS_GCR_BASE_ADDR + 0x0)  /* WO */
#define REG_ULS_START                                   (ULS_GCR_BASE_ADDR + 0x4)  /* WO */
#define REG_ULS_BODY                                    (ULS_GCR_BASE_ADDR + 0x8)  /* WO */
#define REG_ULS_END                                     (ULS_GCR_BASE_ADDR + 0xC)  /* WO */


#endif
