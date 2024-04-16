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
 * GKI header for the comunication mechanism from emmi-low to emmi-high
 * tasks.
 **************************************************************************/

#ifndef EMMIEV_H
#define EMMIEV_H

/***************************************************************************
 * Nested Include Files
***************************************************************************/

#include <system.h>
#include <emmi_sig.h>
#include <ki_typ.h>

/***************************************************************************
 * Manifest Constants
***************************************************************************/
#if defined(UPGRADE_GENIE_PROTOCOL_FOR_FIFO_NO_FLOWCON)
#define UPGRAGE_GENIE_EMMI_L2_CRC
#endif
/*******************************************************************************
* Define: EMMI_L3_BLOCK_MAX_MESSAGE_SIZE
* Description: Max amount of user data within the EMMI Layer 3 block (255 minus
*                  1 for the MI). The actual amount of user data may be further
*                  reduced by any preamble (sequence number and length).
*******************************************************************************/
#if defined(UPGRADE_GENIE_PROTOCOL_FOR_FIFO_NO_FLOWCON)
#define EMMI_L3_BLOCK_MAX_MESSAGE_SIZE  (255)
#else
#define EMMI_L3_BLOCK_MAX_MESSAGE_SIZE  (254)
#endif
/*******************************************************************************
* Define: EMMI_L3_BLOCK_MAX_PREAMBLE_SIZE
* Description: The maximum size of any preamble data within the EMMI Layer 3
*                  block body. This comprises of the sequence number plus,
*                  in some blocks, a 2 byte length field, other fields if
*                  EX MI's are in use (bits and seq).
*******************************************************************************/
#define EMMI_L3_BLOCK_MAX_PREAMBLE_SIZE   (5)

/*******************************************************************************
* Define: EMMI_ACK_TIMEOUT_USER_VALUE
* Description: User value for the EMMI ACK timeout GKI timer.
*******************************************************************************/
#define EMMI_ACK_TIMEOUT_USER_VALUE       (1)

/*******************************************************************************
* Define: EMMI_RETRANS_USER_VALUE
* Description: User value for the EMMI Retransmission GKI timer.
*******************************************************************************/
#define EMMI_RETRANS_USER_VALUE           (2)

#if defined (EMMI_UART_DMA_MODE)
/*******************************************************************************
* Define: EMMI_RX_POLL_TIMER_USER_VALUE
* Description: User value for the EMMI DMA poll GKI timer.
*******************************************************************************/
# define EMMI_RX_POLL_TIMER_USER_VALUE    (3)
#endif

/*******************************************************************************
* Define: EMMI_MAX_NUM_RX_BLOCKS
* Description: Number fo EMMI Rx blocks that can be buffered at one time. If
*                  the target receives more than this without being able to
*                  process them in time it will start NAKing new blocks until
*                  there is storage room.
*******************************************************************************/
#if !defined (EMMI_MAX_NUM_RX_BLOCKS)
#define EMMI_MAX_NUM_RX_BLOCKS           (2)
#endif

/***************************************************************************
 * Typed Constants
***************************************************************************/

typedef enum EmmiHighEvent
/*
** use as 16-bit flag
*/
{
    /* events from interrupt */
#if !defined (EMMI_UART_DMA_MODE)
    FAILED_RECEIVE_EVENT       = 0x0001,
    RECEIVED_BLOCK_EVENT       = 0x0002,
    TRANSMIT_BLOCK_COMPLETE    = 0x0004,
    RECEIVED_ACK_EVENT         = 0x0008,
    RECEIVED_NAK_EVENT         = 0x0010,
# if defined (EMMI_UART_FIFO_MODE)
    EMMI_FIFO_TX_EVENT         = 0x0020,
# endif
#else
    EMMI_DMA_RX_EVENT          = 0x0001,
    EMMI_DMA_TX_EVENT          = 0x0002,
    EMMI_DMA_POLL_RX_EVENT     = 0x0004,
#endif

    /* events from low pri task, the value of these
    ** must not change based on conditional compiles. */
    PUT_TX_BLOCK_EVENT         = 0x0040,
    ACK_TIMEOUT_EVENT          = 0x0080, /* no response from host so re-try */
    RETRANS_TIMEOUT_EVENT      = 0x0100, /* give host time to bin last block */

#if defined (EMMI_UART_FIFO_MODE) && defined (UPGRADE_GENIE_PROTOCOL_FOR_FIFO_NO_FLOWCON)
    
    EMMI_TX_INTERRUPT_EVENT  = 0x0200, /*Event indicate that uart tx complete*/
    EMMI_HIGH_ALL_EVENTS       = 0x03FF, /* Ensure this is updated when new events are added */

    END_OF_EVENTS_TO_EMMI_HIGH = 0x0400  /* next in binary sequence */
#else

    EMMI_HIGH_ALL_EVENTS       = 0x01FF, /* Ensure this is updated when new events are added */

    END_OF_EVENTS_TO_EMMI_HIGH = 0x0200  /* next in binary sequence */
#endif
}
EmmiHighEvent;

/***************************************************************************
 * Type Definitions
***************************************************************************/

typedef struct TxInterruptBlockTag
{
    Int8            blockLength;      /* including preamble and MI */
    Int8            mi;
    Int8            preambleLength;   /* 1 for ttp MIs (3 for first) */
    Int8            preamble[EMMI_L3_BLOCK_MAX_PREAMBLE_SIZE]; /* seq, len*2 */
    Int8           *data;
}
TxInterruptBlock;

typedef struct RxInterruptBlockTag
{
    Boolean         empty;
    Int8            blockLength;      /* including preamble and MI */
    Int8            mi;
#if defined(EMMI_L3_BLOCK_MAX_MESSAGE_SIZE)
    Int8            data[EMMI_L3_BLOCK_MAX_MESSAGE_SIZE + 2];
#else
    Int8            data[EMMI_L3_BLOCK_MAX_MESSAGE_SIZE];
#endif
}
RxInterruptBlock;

/***************************************************************************
 * Global Variables
***************************************************************************/

extern RxInterruptBlock         rxInterruptBlocks[EMMI_MAX_NUM_RX_BLOCKS];
extern TxInterruptBlock         txInterruptBlock;
extern KiTimer                  emmiAckTimer;
extern KiTimer                  emmiRetransTimer;
#if defined (EMMI_UART_DMA_MODE)
extern KiTimer                  emmiDmaRxPollTimer;
#endif

/***************************************************************************
 * Function Prototypes
***************************************************************************/

void EmmiHighSendEvent         (EmmiHighEvent theEvent);

#endif
/* END OF FILE */
