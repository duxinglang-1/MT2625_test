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
 * File Description
 * ----------------
 *
 * RVCHDATA.H
 * Change control exported data
 **************************************************************************/

#ifndef RVCHDATA_H
#define RVCHDATA_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#if !defined (SYSTEM_H)
#  include <system.h>
#endif
#if !defined (KI_SIGBUF_H)
#  include <ki_sigbuf.h>
#endif
#include <rvsystem.h>

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/***************************************************************************
 * Type Definitions
 **************************************************************************/

typedef enum VgChangeControlElementsTag
{
  CC_SIMAT_BIP                   = PROCEDURE_BL_CC_ID,
  CC_GENERAL_PACKET_RADIO_SYSTEM = PROCEDURE_BL_PD_ID,
  CC_MOBILITY_MANAGEMENT         = PROCEDURE_BL_MM_ID,
  CC_SUBSCRIBER_IDENTITY_MODULE  = PROCEDURE_BL_SIM_ID,
  CC_POWER_MANAGEMENT            = PROCEDURE_BL_PM_ID,
#if defined (FEA_PHONEBOOK)  
  CC_LIST_MANAGEMENT             = PROCEDURE_BL_LM_ID,
#endif /* FEA_PHONEBOOK */
  
  CC_ENGINEERING_MODE            = PROCEDURE_BL_EM_ID,
  CC_SHORT_MESSAGE_SERVICE       = PROCEDURE_BL_SM_ID,
  CC_GENERAL_MODULE              = PROCEDURE_BL_GL_ID,
  CC_DM_MODULE                   = PROCEDURE_BL_DM_ID,  
/* to add more change control for other data services modules, use the following
 * method:
 * CC_NEW_MODULE_A = NUMBER_OF_PROCS;
 * CC_NEW_MODULE_B = NUMBER_OF_PROCS + 1;
 */
  NUMBER_CHANGE_CONTROL_ELEMENTS
} VgChangeControlElements;

typedef enum ControlStateTag
{
  UNREGISTERED,
  CONTROL_GOT,
  CONTROL_NOT,
  PENDING_REGISTRATION,
  PENDING_CONTROL,
  PENDING_RELEASE,
  NOT_READY,
  SUSPENDED
}
ControlState;

typedef struct VgChangeControlTag
{
  ControlState       state;
  VgmuxChannelNumber entity;
  Boolean            isSingleUser;
  Boolean            sendsReadyInd;
  SignalId           pendingSignal;
  Boolean            pendingCompletion;
  Boolean            pendingCommand;
  /* added for job108826 */
  VgmuxChannelNumber previousEntity;
} VgChangeControl;

/***************************************************************************
 *  Macros
 **************************************************************************/

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

#endif  /* RVCHDATA_H */

/* END OF FILE */


