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
 ***************************************************************************/
/** \file
 * This file contains the HSL configuration interface between test application
 * and target
 */

#ifndef FR_HSL_SIG_H
#define FR_HSL_SIG_H

/**************************************************************************
 * Include Files
 **************************************************************************/

#include <system.h>
#include <frhsl_typ.h>
#if defined(NBIOT_TRACE_FILTER)
#include "nbiot_trace.h"
#endif
/**************************************************************************
 * Manifest Constants
 **************************************************************************/

/** Length of data array used in HSL configure request. This currently limits
    the number of individual HSL log points that can be enabled/disabled with
    a single HSL configure request. */
#define HSL_CONFIGURE_REQ_DATA_SIZE 128

/** Length of data array used in HSL Filter configure request. */
#define HSL_FILTER_CONFIGURE_REQ_DATA_SIZE 256


/***************************************************************************
 * HSL configuration signals
 **************************************************************************/

/** GKI configure request signal
 */
typedef struct HslConfigureReqTag
{
  TaskId           taskId;     /**< Task ID requesting HSL configuration. */
  Int16            commandRef; /**< Could be used by application to associate request and confirmation signals. */
  HslCfgCommand    command;    /**< Command indicating configuration action */
  Int16            dataSize;   /**< Number of elements in data array */
  Int16            data[HSL_CONFIGURE_REQ_DATA_SIZE]; /**< HSL log points to be enabled/disabled. */
} HslConfigureReq;

typedef struct HslFilterConfigureReqTag
{
  TaskId           taskId;       /**< Task ID requesting HSL configuration. */
  Int16            filterAction; /**< 0: Output to T-Flash
                                      1: Output to Hacksaw 
                                      2: Set MCU Mini Log filter
                                      3: Set DSP Mini Log filter  */
  Int8             data[HSL_FILTER_CONFIGURE_REQ_DATA_SIZE]; 
                                 /**< HSL Filter Array. */
  Int16            dataSize;     /**< Number of elements in data array */
} HslFilterConfigureReq;

/** GKI configure confirmation signal
 */
typedef struct HslConfigureCnfTag
{
  Int16         commandRef;      /**< Command reference for this transaction. */
  HslCfgStatus  status;          /**< Returns status back to the test application. */
} HslConfigureCnf;

/** GKI configure confirmation signal
 */
#if defined(NBIOT_TRACE_FILTER)
typedef struct HslFilterConfigureCnfTag
{
  Int16         commandRef;      /**< Command reference for this transaction. */
  Int8          data[(NBIOT_TRACE_MAX_GROUP_VALUE/8)+1]; 
} HslFilterConfigureCnf;
#else
typedef struct HslFilterConfigureCnfTag
{
  Int16         commandRef;      /**< Command reference for this transaction. */
  HslCfgStatus  status;          /**< Returns status back to the test application. */
} HslFilterConfigureCnf;
#endif

#endif /* FR_HSL_SIG_H */
