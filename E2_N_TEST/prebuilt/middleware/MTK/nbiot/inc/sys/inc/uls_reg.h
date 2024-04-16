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
 |   uls_reg.h
 |
 | Project:
 | --------
 |   UMOLY
 |
 | Description:
 | ------------
 |   Complete Unified Logging System (ULS) HW register definitions.
 |   Full Description can be found in the "ULS High-Speed Modem Logger Hardware specification"
 |
 | Author:
 | -------
 |   mtk08910
 |
 |---------------------------------------------------------------------------
 | $Log$
 |
 ---------------------------------------------------------------------------*/

#ifndef _ULS_REGISTERS_H_
#define _ULS_REGISTERS_H_

/* HW depend definitions here */
#include "uls_2625.h"

#ifdef USE_ULS

/*---------------------------------------------------------------------------
 | @file uls_reg.h
 |
 | @brief MediaTek's Unified Logging System (ULS) HW Register Definitions.
 |
 ---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

/*
 | base addresses
 */
#if !defined(ULS_ABP_BASE_ADDR)
#define ULS_ABP_BASE_ADDR 0xA0250000

#define REG_ULS_TIMESTAMP_MODE	                        (ULS_ABP_BASE_ADDR + 0x0)   /* RW */
#define REG_ULS_TIMESTAMP_INSERT	                    (ULS_ABP_BASE_ADDR + 0x4)   /* WO */
#define REG_ULS_ADD_COMPATIBILITY_TIMESTAMP	            (ULS_ABP_BASE_ADDR + 0x8)   /* WO */
#define REG_ULS_PRIORITY_MODE	                        (ULS_ABP_BASE_ADDR + 0xC)   /* RW */
#define REG_ULS_OUTPUT_BUFFER_DEPTH	                    (ULS_ABP_BASE_ADDR + 0x10)  /* RW */
#define REG_ULS_OUTPUT_BUFFER_PREULTRA_LEVEL            (ULS_ABP_BASE_ADDR + 0x14)  /* RW */ 
#define REG_ULS_OUTPUT_BUFFER_ULTRA_LEVEL	            (ULS_ABP_BASE_ADDR + 0x18)  /* RW */
#define REG_ULS_OUTPUT_BUFFER_FLUSH	                    (ULS_ABP_BASE_ADDR + 0x1C)  /* RW */
#define REG_ULS_DDR_START_ADDRESS	                    (ULS_ABP_BASE_ADDR + 0x20)  /* RW */
#define REG_ULS_DDR_END_ADDRESS	                        (ULS_ABP_BASE_ADDR + 0x24)  /* RW */
#define REG_ULS_DDR_FILL_MODE	                        (ULS_ABP_BASE_ADDR + 0x28)  /* RW */
#define REG_ULS_DDR_POSTED_WRITE_POINTER	            (ULS_ABP_BASE_ADDR + 0x2C)  /* RW */
#define REG_ULS_DDR_COMPLETED_WRITE_POINTER             (ULS_ABP_BASE_ADDR + 0x30)  /* RW */
#define REG_ULS_DDR_READ_POINTER	                    (ULS_ABP_BASE_ADDR + 0x34)  /* RW */
#define REG_ULS_DDR_BYTE_COUNT	                        (ULS_ABP_BASE_ADDR + 0x38)  /* RW */
#define REG_ULS_DDR_BLOCK_SIZE	                        (ULS_ABP_BASE_ADDR + 0x3C)  /* RW */
#define REG_ULS_DDR_MAXIMUM_OUTSTANDING_TRANSACTIONS    (ULS_ABP_BASE_ADDR + 0x40)  /* RW */
#define REG_ULS_DDR_BUFFER_STATUS	                    (ULS_ABP_BASE_ADDR + 0x44)  /* RW */
#define REG_ULS_ENABLE_LOGGING	                        (ULS_ABP_BASE_ADDR + 0x48)  /* RW */
#define REG_ULS_SOFT_RESET             	                (ULS_ABP_BASE_ADDR + 0x4C)  /* RW */
#define REG_ULS_HEADER_ID_CODING                        (ULS_ABP_BASE_ADDR + 0x50)  /* RW */ 
#define REG_ULS_FIFO_0_END_WORD_ADDRESS	                (ULS_ABP_BASE_ADDR + 0x54)  /* RW */
#define REG_ULS_FIFO_1_END_WORD_ADDRESS	                (ULS_ABP_BASE_ADDR + 0x58)  /* RW */
#define REG_ULS_FIFO_2_END_WORD_ADDRESS	                (ULS_ABP_BASE_ADDR + 0x5C)  /* RW */
#define REG_ULS_FIFO_3_END_WORD_ADDRESS	                (ULS_ABP_BASE_ADDR + 0x60)  /* RW */
#define REG_ULS_FIFO_0_STALL_ENABLE	                    (ULS_ABP_BASE_ADDR + 0x64)  /* RW */
#define REG_ULS_FIFO_1_STALL_ENABLE	                    (ULS_ABP_BASE_ADDR + 0x68)  /* RW */
#define REG_ULS_FIFO_2_STALL_ENABLE	                    (ULS_ABP_BASE_ADDR + 0x6C)  /* RW */
#define REG_ULS_FIFO_3_STALL_ENABLE	                    (ULS_ABP_BASE_ADDR + 0x70)  /* RW */
#define REG_ULS_FIFO_0_ENABLE	                        (ULS_ABP_BASE_ADDR + 0x74)  /* RW */
#define REG_ULS_FIFO_1_ENABLE	                        (ULS_ABP_BASE_ADDR + 0x78)  /* RW */
#define REG_ULS_FIFO_2_ENABLE	                        (ULS_ABP_BASE_ADDR + 0x7C)  /* RW */
#define REG_ULS_FIFO_3_ENABLE	                        (ULS_ABP_BASE_ADDR + 0x80)  /* RW */
#define REG_ULS_FIFO_0_PRIORITY	                        (ULS_ABP_BASE_ADDR + 0x84)  /* RW */
#define REG_ULS_FIFO_1_PRIORITY	                        (ULS_ABP_BASE_ADDR + 0x88)  /* RW */
#define REG_ULS_FIFO_2_PRIORITY	                        (ULS_ABP_BASE_ADDR + 0x8C)  /* RW */
#define REG_ULS_FIFO_3_PRIORITY	                        (ULS_ABP_BASE_ADDR + 0x90)  /* RW */
#define REG_ULS_INTERRUPT_ENABLE	                    (ULS_ABP_BASE_ADDR + 0x100) /* RW */
#define REG_ULS_INTERRUPT_DISABLE	                    (ULS_ABP_BASE_ADDR + 0x104) /* RW */
#define REG_ULS_ENABLED_INTERRUPTS	                    (ULS_ABP_BASE_ADDR + 0x108) /* RW */
#define REG_ULS_INTERRUPT_STATUS	                    (ULS_ABP_BASE_ADDR + 0x10C) /* RW */
#define REG_ULS_INTERRUPT_CLEAR	                        (ULS_ABP_BASE_ADDR + 0x110) /* RW */
#define REG_ULS_INTERRUPT_TRIGGER                       (ULS_ABP_BASE_ADDR + 0x114) /* RW */

/*
 | ULS GCR registers
 */
#define ULS_GCR_BASE_ADDR (ULS_ABP_BASE_ADDR + 0xC000)

#define REG_ULS_SINGLE                                  (ULS_GCR_BASE_ADDR + 0x0)  /* WO */
#define REG_ULS_START                                   (ULS_GCR_BASE_ADDR + 0x8)  /* WO */
#define REG_ULS_BODY                                    (ULS_GCR_BASE_ADDR + 0x10) /* WO */
#define REG_ULS_END                                     (ULS_GCR_BASE_ADDR + 0x18) /* WO */
#define REG_ULS_SYSTEM_SINGLE                           (ULS_GCR_BASE_ADDR + 0x80) /* WO */

#endif

//ref: SRS Section 2.2.1.1
#define SMP_SOURCE_BASE_ID								0x10

/*!
 | @brief REG_ULS_TIMESTAMP_MODE field description. 
 */
														/* bit 31-2 reserved, read as 0x00 */
#define ULS_LOCAL_TIMESTAMP_MODE            (0)			/* bit 0 = 0 */
#define ULS_GLOBAL_TIMESTAMP_MODE           (1)			/* bit 0 = 1 */
#define ULS_DISABLE_TIMESTAMP_MODE			(1 << 1)	/* bit 1 = 1 */

/*! 
 | @brief REG_ULS_TIMESTAMP_INSERT &  REG_ULS_ADD_COMPATIBILITY_TIMESTAMP field description.
 */
#define ULS_INSERT_TIMESTAMP                (1)			/* bit 0 =1 will insert a timestamp */

/*! 
 | @brief REG_ULS_PRIORITY_MODE field description.
 */
/*
	00 in the MODE bits selects strict priority arbitration (see Section 2.4.1)
	01 in the MODE bits selects priority on full arbitration (see Section 2.4.2)
	10 in the MODE bits selects weighted round robin arbitration (see Section 2.4.3)
	11 in the MODE bits is a reserved code
*/
														/* bit 32-2 reserved, read as 0x00 */
#define ULS_PRIORITY_ARBITRATION			(0)
#define ULS_PRIORITY_ON_FULL                (1)
#define ULS_PRIORITY_ROUND_ROBIN            (2)

/*! 
 | @brief REG_ULS_OUTPUT_BUFFER_DEPTH field description.
 */
/*
	DEPTH
	0	32 words/ 4 bursts / 256 bytes 
	1	64 words / 8 bursts / 512 bytes
	2	128 words / 16 bursts /1K bytes
	3	256 words / 32 bursts / 2K bytes
	4	512 words / 64 bursts / 4K bytes
*/
                                                        /**< bit 32-3 reseved, read as 0x00 */
#define ULS_DEPTH_256_BYTES                 (0)
#define ULS_DEPTH_512_BYTES                 (1)
#define ULS_DEPTH_1K_BYTES                  (2)
#define ULS_DEPTH_2K_BYTES                  (3)
#define ULS_DEPTH_4K_BYTES                  (4)

/*!
 | @brief REG_ULS_OUTPUT_BUFFER_PREULTRA_LEVEL field description.
 */
#define ULS_ENABLE_PREULTRA_LEVEL			(1 << 31)	/* bit 31 = 1 enables pre-ultra on output buffer exceeding level */
                                                        /* bit 30-10 reserved, read as 0x00 */
                                                        /* bit 9-3 upper 6 bits of threshold level */
                                                        /* bit 2-0 always 0 */
/*!
 | @brief REG_ULS_OUTPUT_BUFFER_ULTRA_LEVEL field description.
 */
#define ULS_ENABLE_ULTRA_LEVEL              (1 << 31)	/* bit 31 = 1 enables ultra on output buffer exceeding level */
                                                        /* bit 30-10 reserved, read as 0x00 */
                                                        /* bit 9-3 upper 6 bits of threshold level */
                                                        /* bit 2-0 always 0 */
/*!
 | @brief REG_ULS_OUTPUT_BUFFER_FLUSH field description.
 */
#define ULS_FLUSH_COMPLETE                  (1 << 31)	/* bit 31 signal when the flush is complete */

/*! 
 | @brief REG_ULS_DDR_BUFFER_START_ADDRESS field description. 
 */
                                                        /* bit 31-12 Start address of the ring buffer in EMI */
                                                        /* bit 11-0 always 0 */
/*! 
 | @brief REG_ULS_DDR_BUFFER_START_ADDRESS field description. 
 */
                                                        /* bit 31-12 End address of the ring buffer in EMI */
                                                        /* bit 11-0 always 0 */
/*! 
 | @brief REG_ULS_DDR_FILL_MODE field description. 
 */
                                                        /* bit 31-1 reserved, read as 0x00 */
#define ULS_FILL_MODE_HALT                 (1)			/* bit 0 = 1 HALT when ring buffer becomes full */
#define ULS_FILL_MODE_CONTINUE             (0)          /* bit 0 = 0 Continue writing even when buffer is full */

/*!
 | @brief REG_ULS_DDR_BUFFER_POSTED_WRITE_POINTER field description. 
 */
                                                        /* bit 31-0 ring buffer posted write pointer */
/*!
 | @brief REG_ULS_DDR_BUFFER_COMPLETED_WRITE_POINTER field description. 
 */
                                                        /* bit 31-0 ring buffer completed write pointer */

/*!
 | @brief REG_ULS_DDR_BUFFER_READ_POINTER field description. 
 */
                                                        /* bit 31-0 SW updates this register when data has been read */
/*! 
 | @brief REG_ULS_DDR_BUFFER_BYTE_COUNT field description. 
 */
                                                        /* bit 31-0 number of bytes currently stored in the ring buffer */
/*!
 | @brief REG_ULS_DDR_BLOCK_SIZE field description. 
 */
                                                        /* bit 31-0 threshold for the EMI ring buffer at which interrupt is generated */

/*! 
 | @brief REG_ULS_DDR_MAXIMUM_OUTSTANDING_TRANSACTIONS field description. 
 */
                                                        /* bit 31-3 reserved, read as 0x00 */
                                                        /* bit 2-0 max number of outstanding transactions */
/*!
 | @brief  REG_ULS_DDR_BUFFER_STATUS field Description.
 */
                                                        /* bit 31-1 reserved, read as 0x00 */
#define ULS_RING_BUFFER_FULL_STATUS        (1)			/* bit 0 = 1 indicates ring buffer is FULL */

/*!
 | @brief REG_ULS_ENABLE_LOGGING field description. 
 */
                                                        /* bit 31-2 reserved, read as 0x00 */
#define ULS_GLOBAL_HALT						(1 << 1)	/* bit 1 = 1 prevent new data being written to all input FIFOs */
#define ULS_ENABLE_LOGGING                  (1)			/* bit 0 = 1 enable ULS logging */
#define ULS_DISABLE_LOGGING                 (0)

/*! 
 | @brief REG_ULS_INTERRUPT_ENABLE field description. 
 */
                                                        /* bit 31-6 reserved, read as 0x00 */
#define ULS_INT_ENABLE_GCR_MISALIGNED       (1 << 5)	/* bit 5 = 1 enable GCR misaligned interrupt */
#define ULS_INT_ENABLE_GCR_64_BIT           (1 << 4)	/* bit 4 = 1 enable GCR 64 bit interrupt */
#define ULS_INT_ENABLE_GCR_READ             (1 << 3)	/* bit 3 = 1 enable GCR read interrupt */
#define ULS_INT_ENABLE_OUTPUT_BUFFER_FULL   (1 << 2)	/* bit 2 = 1 enable output buffer full interrupt */
#define ULS_INT_ENABLE_RING_BUFFER_THRESOLD (1 << 1)	/* bit 1 = 1 enable ring buffer threshold exceeded interrupt */
#define ULS_INT_ENABLE_RING_BUFFER_FULL     (1)			/* bit 0 = 1 enable ring buffer full interrupr */

/*! 
 | @brief REG_ULS_INTERRUPT_DISABLE field description. 
 */
                                                        /* bit 31-6 reserved, read as 0x00 */
#define ULS_INT_DISABLE_GCR_MISALIGNED      (1 << 5)	/* bit 5 = 1 disable GCR misaligned interrupt */
#define ULS_INT_DISABLE_GCR_64_BIT          (1 << 4)	/* bit 4 = 1 disable GCR 64 bit interrupt */
#define ULS_INT_DISABLE_GCR_READ            (1 << 3)	/* bit 3 = 1 disable GCR read interrupt */
#define ULS_INT_DISABLE_OUTPUT_BUFFER_FULL  (1 << 2)	/* bit 2 = 1 disable output buffer full interrupt */
#define ULS_INT_DISABLE_RING_BUFFER_THRESHOLD (1 << 1)	/* bit 1 = 1 disable ring buffer threshold exceeded interrupt */
#define ULS_INT_DISABLE_RING_BUFFER_FULL    (1)			/* bit 0 = 1 disable ring buffer full interrupr */

/*! 
 | @brief REG_ULS_ENABLED_INTERRUPTS field description. 
 */
                                                        /* ULS interrupt read only */
/*! 
 | @brief ULS_INTERRUPT_STATUS_REG field description. 
 */
#define ULS_INT_STATUS_GCR_MISALIGNED       (1 << 5)	/* bit 5 = 1 status GCR misaligned interrupt */
#define ULS_INT_STATUS_GCR_64_BIT           (1 << 4)	/* bit 4 = 1 status GCR 64 bit interrupt */
#define ULS_INT_STATUS_GCR_READ             (1 << 3)	/* bit 3 = 1 status GCR read interrupt */
#define ULS_INT_STATUS_OUTPUT_BUFFER_FULL   (1 << 2)	/* bit 2 = 1 status output buffer full interrupt */
#define ULS_INT_STATUS_RING_BUFFER_THRESHOLD (1 << 1)	/* bit 1 = 1 status ring buffer threshold exceeded interrupt */
#define ULS_INT_STATUS_RING_BUFFER_FULL     (1)			/* bit 0 = 1 status ring buffer full interrupr */

/*! 
 | @brief ULS_INTERRUPT_CLEAR_REG field description. 
 */
#define ULS_INT_CLEAR_GCR_MISALIGNED        (1 << 5)	/* bit 5 = 1 clear GCR misaligned interrupt */
#define ULS_INT_CLEAR_GCR_64_BIT            (1 << 4)  /* bit 4 = 1 clear GCR 64 bit interrupt */
#define ULS_INT_CLEAR_GCR_READ              (1 << 3)  /* bit 3 = 1 clear GCR read interrupt */
#define ULS_INT_CLEAR_OUTPUT_BUFFER_FULL    (1 << 2)  /* bit 2 = 1 clear output buffer full interrupt */
#define ULS_INT_CLEAR_RING_BUFFER_THRESOLD  (1 << 1)  /* bit 1 = 1 clear ring buffer threshold exceeded interrupt */
#define ULS_INT_CLEAR_RING_BUFFER_FULL      (1)  /* bit 0 = 1 clear ring buffer full interrupr */

/*! 
 | @brief ULS_INTERRUPT_TRIGGER_REG field description. 
 */
#define ULS_INT_TRIGGER_GCR_MISALIGNED      (1 << 5)  /* bit 5 = 1 trigger GCR misaligned interrupt */
#define ULS_INT_TRIGGER_GCR_64_BIT          (1 << 4)  /* bit 4 = 1 trigger GCR 64 bit interrupt */
#define ULS_INT_TRIGGER_GCR_READ            (1 << 3)  /* bit 3 = 1 trigger GCR read interrupt */
#define ULS_INT_TRIGGER_OUTPUT_BUFFER_FULL  (1 << 2)  /* bit 2 = 1 trigger ULS output buffer full interrupt */
#define ULS_INT_TRIGGER_RING_BUFFER_THRESOLD (1 << 1) /* bit 1 = 1 trigger ring buffer threshold exceeded interrupt */
#define ULS_INT_TRIGGER_RING_BUFFER_FULL    (1)  /* bit 0=1 trigger ring buffer full interrupr */

/*!
 | @brief ULS_SOFT_RESET_REG field description. 
 */
                                            /* bit 31-1 reserved, read as 0x00 */
#define ULS_SOFT_RESET_VALUE                (1)  /* bit 0 = 1 issues a soft reset of the HW */

#define ULS_ENABLE_FIFO                     (1)   /* bit 0 = 1 enabled FIFO */

#ifdef __cplusplus
}
#endif

#endif /* USE_ULS */
#endif /* _ULS_REGISTERS_H_ */
