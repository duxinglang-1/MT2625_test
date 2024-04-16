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
/***************************************************************************
 *  File Description :
 *
 *      Voyager DS/DTEIF - PPP Task Signal Type Definitions
 **************************************************************************/

#ifndef DSPPPTYP_H
#define DSPPPTYP_H

#include <nbiot_modem_common_config.h>

#if defined (FEA_PPP)

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/***************************************************************************
 * Typed Constants
 **************************************************************************/

/***************************************************************************
 * Types
 **************************************************************************/
/** DS-PPP error causes */
typedef enum DsPppErrorCauseTag
{
  /** No error */
  CAUSE_DS_NO_ERROR,
  /** Parity error */
  CAUSE_DS_PARITY_ERROR,
  /** Framing error */
  CAUSE_DS_FRAMING_ERROR,
  /** overrun error */
  CAUSE_DS_OVERRUN_ERROR,
  /** buffer overflow */
  CAUSE_DS_BUFFER_OVERFLOW,
  /** Flag character search timeout */
  CAUSE_DS_FLAG_SEARCH_TIMEOUT,
  /** buffer underflow */
  CAUSE_DS_BUFFER_UNDERFLOW,
  /** Packet corrupted */
  CAUSE_DS_CORRUPT_PACKET,
  /** Packet at end of session */
  CAUSE_DS_PACKET_AT_END_OF_SESSION,
  /** Outgoing packet is discarded */
  CAUSE_DS_OUTGOING_PACKET_DISCARDED,
  /** Frame with zero length */
  CAUSE_DS_ZERO_LENGTH_FRAME,
  /** Number of error causes */
  NUM_OF_DS_ERROR_CAUSES
} DsPppErrorCause;

/***************************************************************************
 *  Signal Definitions
 **************************************************************************/

#endif /* FEA_PPP */
#endif

/* END OF FILE */
