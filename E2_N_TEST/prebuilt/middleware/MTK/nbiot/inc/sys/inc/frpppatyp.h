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
/** \file
 * \ingroup PrdCBEcorePLFservicesFrPppa
 * \brief Defines enums to list the PPPA initial configuration.
 * and the structure of PPPA register configuration ,input/output data
 */

#ifndef FRPPPATYP_H
#define FRPPPATYP_H

/**************************************************************************
 * Nested Include Files
 **************************************************************************/
#include <system.h>

/**************************************************************************
 * Manifest Constants
 **************************************************************************/

/**************************************************************************
 * Type Definitions
 **************************************************************************/
/** Direction of data being processed (downlink or uplink)
 */
typedef enum PppaDataDirTag
{
  PPPA_DIR_DOWNLINK,        /**< Downlink data */
  PPPA_DIR_UPLINK,          /**< Uplink data */
} PppaDataDir;

/** The type of last byte that was processed (for uplink only)
 */
typedef enum PppaLastByteStatusTag
{
  PPPA_LBS_NORMAL_BYTE,     /**< Normal byte */
  PPPA_LBS_FLAG,            /**< Flag sequence (0x7E) */
  PPPA_LBS_ESCAPE           /**< Escape character (0x7D) */
} PppaLastByteStatus;

/** The type of PPPA state
 */
typedef enum PppaStateTag
{
  PPPA_STATE_IDLE,          /**< Idle state */
  PPPA_STATE_PROCESSING,    /**< Processing state */
  PPPA_STATE_SUSPENDED      /**< Suspended state */
} PppaState;

/** PPPA FIFO In enable switch
 */
typedef enum PppaFifoInEnTag
{
  PPPA_FIFO_IN_DIS,          /**< PPPA FIFO In Disable */
  PPPA_FIFO_IN_EN,           /**< PPPA FIFO In Enable */
} PppaFifoInEn;

/** PPPA FIFO Out enable switch
 */
typedef enum PppaFifoOutEnTag
{
  PPPA_FIFO_OUT_DIS,          /**< PPPA FIFO Out Disable */
  PPPA_FIFO_OUT_EN,           /**< PPPA FIFO Out Enable */
} PppaFifoOutEn;


/** PPPA interrupt type
 */
typedef enum PppaIntTypeTag
{
  PPPA_FRAME_END_INT,          /**< PPPA Frame end interrupt */
  PPPA_PROCESS_END_INT,       /**< PPPA Process end interrupt */
  PPPA_INVALID_FRAME_INT,   /**< PPPA Invalid frame interrupt */
  PPPA_BYTE_OVERFLOW_INT,  /**< PPPA Byte overflow interrupt */
} PppaIntType;


/** Data sent to PPPA (input to PPPA) during normal operation
 * (does not include data such as generator polynomial that do not change
 * during normal operation, i.e. data transfer)
 */
typedef struct PppaInputDataTag
{
  Int8                *in_p;                /**< Input pointer (BlockIn) */
  Int8                *out_p;               /**< Output pointer (BlockOut) */
  PppaDataDir         dataDir;              /**< Data direction */
  Int16               inputBlockSize;       /**< Input block size (BlockIn length) */
  Int16               frameLengthCounter;   /**< Current length of frame */
  Int32               pppFrameFcs;          /**< Current FCS */
  Int16               frameLengthOvfVal;    /**< Overflow value to check against */
  PppaLastByteStatus  lastByteStatus;       /**< Last byte status (uplink only) */
  Boolean             discard;              /**< Discard (uplink only) */
  Int32               accm;                 /**< ACCM value */
  Boolean             fcsEnabled;           /**< If FCS is present in PPP frames (disabled only for test purposes) */
} PppaInputData;

/** Data received from PPPA (output from PPPA)
 */
typedef struct PppaOutputDataTag
{
  PppaDataDir         dataDir;                  /**< Data direction */
  PppaState           pppaState;                /**< Current state of PPPA */
  Int16               frameLengthCounter;       /**< Current length of frame */
  Int32               pppFrameFcs;              /**< Current FCS (uplink only) */
  PppaLastByteStatus  lastByteStatus;           /**< Last byte status (uplink only) */
  Boolean             discard;                  /**< If PPPA was last in discard mode (uplink only) */
  Boolean             eoFrameIntStatus;         /**< If End of Frame interrupt has been triggered */
  Boolean             eoProcessingIntStatus;    /**< If End of Processing interrupt has been triggered */
  Boolean             invalidFrameIntStatus;    /**< If Invalid Frame interrupt has been triggered */
  Boolean             ovfIntStatus;             /**< If Overflow interrupt has been triggered */
} PppaOutputData;

/**************************************************************************
 * Macros
 **************************************************************************/

/**************************************************************************
 * Function Prototypes
 **************************************************************************/

#endif /* FRPPPATYP_H */

/* END OF FILE */

