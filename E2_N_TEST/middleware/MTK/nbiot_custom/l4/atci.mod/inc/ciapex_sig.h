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
 * Signal definitions for the APEX signals implemented by the CI Task.
 **************************************************************************/

#ifndef CIAPEX_SIG_H
#define CIAPEX_SIG_H

#if !defined(UPGRADE_GENERIC_MUX)

/** \defgroup SigApexCi APEX-CI Signalling Interface
 * \ingroup Sig
 * \ingroup PrdDataServices
 * Multiplexer selection / query signals that are processed by the CI Task.
 *
 * @{
 */

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <vgmx_typ.h>

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/***************************************************************************
 * Typed Constants
 **************************************************************************/

/***************************************************************************
 * Type Definitions
 **************************************************************************/

/** Multiplexer select request option.
 * This enumeration defines which settings should be changed when the
 * multiplexer request is received by the CI.
 */
typedef enum ApexMuxSelectorTag
{
  APEX_MUX_SELECT_PORT_INDEX,  /**< Change Port Index. */
  APEX_MUX_SELECT_MUX_TYPE,    /**< Change Multiplexer Type. */
  APEX_MUX_SELECT_BOTH         /**< Change both Multiplexer Type and Port Index. */
} ApexMuxSelector;

/** Select multiplexer request.
 * This signal is used to select a specified multiplexer. If the current
 * multiplexer is different to the requested one an attempt is made to select
 * it. If the requested multiplexer is already selected then no action is
 * taken.
 * \see #ApexMuxSelectCnf
 */
typedef struct ApexMuxSelectReqTag
{
  TaskId           sender;       /**< The task that originated the request. */
  VgmuxId          multiplexer;  /**< The multiplexer to be selected. */
  VgmxMuxIndex     muxIndex;     /**< The index of the MUX. */
  VgmxComPortIndex portIndex;    /**< The index of the COM port to be associated
                                  * with the MUX. */
  ApexMuxSelector  selector;     /**< Multiplexer select request option. */
} ApexMuxSelectReq;

/** Select multiplexer confirmation.
 * This signal is used to inform the task that sent the associated
 * #ApexMuxSelectReq signal whether the request was successful or not.
 * \see #ApexMuxSelectReq
 */
typedef struct ApexMuxSelectCnfTag
{
  VgmxMuxIndex muxIndex; /**< The index of the MUX. */
  Boolean      status;   /**< The status of a multiplexer selection request. */
} ApexMuxSelectCnf;

/** Query multiplexer request.
 * This signal is used to query which multiplexer is active and whether any
 * AT command channels are available for AT command operations.
 * \see #ApexMuxQueryCnf
 */
typedef struct ApexMuxQueryReqTag
{
  VgmxMuxIndex muxIndex;  /**< The index of the MUX. */
  TaskId       sender;    /**< The task that originated the request. */
} ApexMuxQueryReq;

/** Query multiplexer confirmation.
 * This signal is used to inform the task that sent the associated
 * #ApexMuxQueryReq signal what multiplexer is currently active and whether
 * it has any command channels are available for AT command operations.
 * \see #ApexMuxQueryReq
 */
typedef struct ApexMuxQueryCnfTag
{
  VgmxMuxIndex     muxIndex;    /**< The index of the MUX. */
  VgmxComPortIndex portIndex;   /**< The index of the COM port to be associated
                                 * with the MUX. */
  VgmuxId          multiplexer; /**< The current active multiplexer. */
  Boolean          commandChannelIsAvailable;
                                /**< TRUE if there is an active AT command channel. */
} ApexMuxQueryCnf;

/***************************************************************************
 *  Macros
 **************************************************************************/

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

/** @} */ /* End of SigCiApex group */
#endif
#endif /* CIAPEX_SIG_H */

