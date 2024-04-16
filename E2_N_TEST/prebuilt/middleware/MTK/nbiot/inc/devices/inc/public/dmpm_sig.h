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
 * DM Power Control Module signals
 **************************************************************************/
#ifndef DMPM_SIG_H
#define DMPM_SIG_H

/***************************************************************************
 * Include Files
 **************************************************************************/
#include <dmpm_typ.h>
#if defined (USE_TTP_AL)
#include <applayer.h>
#endif

#include <kitqid.h>    /* for TaskId type */

/** \defgroup Abpm  ABPM Signalling Interface
 * \ingroup NasApex
 * \ingroup Sig
 * ABPM Signalling Interface. 
 * The Power Management procedure interfaces with the Controlling Tasks 
 * (such as Foreground Layer or ATCI). These are the <I>users</I> of the 
 * Power Management procedure and access facilities via the APEX interface. 
 * All APEX messages related to the Power Management procedure have the prefix <B>ApexPm</B>.
 *
 * The Figure below shows the ABPM interface.
 *
 * \image html ABPM_interface.png
 * @{
 */

/***************************************************************************
 * Defines
 **************************************************************************/
/* none */

 /****************************************************************************
 * Type Definitions
 ****************************************************************************/

/** This signal is sent by all FL tasks registered BL, in response to ApexPmPowerGoingDownInd broadcast 
 * This signal must be sent by all Registered Tasks (for example the Foreground Layer task or external 
 * interface task) to the Background Layer Power Management, on receipt of an ApexPmPowerGoingDownInd 
 * signal, to indicate that they are ready for power to be removed from the handset. Registered Tasks 
 * can thus delay power-off slightly, in order to finish an operation such as saving data to non-volatile 
 * storage. \note There is a configurable timeout after which ABPM assumes it will not receive these 
 * response signals, and goes ahead to remove power anyway. This should never happen in a final product.
 */
typedef struct ApexPmPowerGoingDownRspTag
{
  /** Caller Task identity (thus, ID of task accepting the power-down request) */
  TaskId taskId;
}
ApexPmPowerGoingDownRsp;

/** Power-Down request.
 * If the protocol stack power-mode setting is commanded to take effect immediately, 
 * ABPM sends an AbpmPowerDownSig to the Background Layer which calls appropriate functions 
 * in ABCC, ABMM, ABSI to close all calls and detach from the network. 
 *
 * Similarly, if the SIM power-mode setting is commanded to take effect immediately, 
 * ABPM sends an AbpmPowerDownSig to the Background Layer ensuring the <I> closeSim </I> 
 * field is set to TRUE. 
 */
typedef struct AbpmPowerDownSigTag
{
  /** Caller Task identity */
  TaskId          taskId;
  /** TRUE => terminate STACK session */
  Boolean         closeSTACK;
  /** TRUE => terminate SIM session */
  Boolean         closeSIM;
  /** TRUE => unmount all file systems */
  Boolean         closeFsystem;
}
AbpmPowerDownSig;


/** Mode Change request.
 * This signal is a request to Background Layer Power Management to perform all 
 * shut down actions as for ApexPmPowerDownReq, but without removing power. 
 */
typedef struct ApexPmModeChangeReqTag
{
  /** Caller Task identity (The ID of Controlling Task)*/
  TaskId                 taskId;
  /** New mode */
  PmRequiredPowerState   newState;
}
ApexPmModeChangeReq;

/** This signal is sent as a response to #ApexPmModeChangeReq.
 * This signal is confirmation from Background Layer Power Management that the 
 * shut down actions requested by an #ApexPmPowerDownReq have been completed.
 */
typedef struct ApexPmModeChangeCnfTag
{
  /** Caller Task identity */
  TaskId                 taskId;
  /** Indicates if the request was successful */
  Boolean                result;
  /** SIM is powered on/off */
  Boolean                simState;
  /** Protocol Stack is powered on/off */
  Boolean                protoStackState;
  /** SIM needs to be powered up */
  Boolean                postPowerUpSim;
  /** Protocol Stack needs to be powered up */
  Boolean                postPowerUpStack;
}
ApexPmModeChangeCnf;

/** This signal is sent from BL to DM to request that the power [hardware] be latched on/off */
typedef struct DmPowerControlReqTag
{
  /** TRUE,  Turns power On; FALSE, Turns power Off */
  Boolean         powerOn;
}
DmPowerControlReq;

/** @} */ /* End of group. */

#endif
/* END Of FILE */
