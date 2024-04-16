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
 * External interface for the ci stk module.
 **************************************************************************/

#ifndef RVSTK_H
#define RVSTK_H


/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <rvutil.h>

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/***************************************************************************
 * Type Definitions
 **************************************************************************/

typedef enum StkCommandTypeTag
{
  STK_PROTOCOL_REQ,
  STK_UNSOLICITED_REQ,
  STK_DISPLAY_ALPHA_ID_REQ,
  STK_COMMAND_ERROR
}
StkCommandType;

typedef enum StkCommandIdTag
{
  STK_NOT_PROACTIVE              = 0,

  STK_CMD_SETUP_REFRESH          = 1,
  STK_CMD_SETUP_SS               = 11,
  STK_CMD_SETUP_SM               = 13,
  STK_CMD_DISPLAY_TEXT           = 21,
  STK_CMD_SELECT_ITEM            = 24,
  STK_CMD_SETUP_MENU             = 25,
  STK_CMD_SETUP_DISPLAY_ALPHA_ID = 40,
  STK_CMD_SETUP_EVENT_LIST       = 5,
  STK_CMD_SETUP_CALL             = 10,
  STK_CMD_SETUP_USSD             = 12,
  STK_CMD_SETUP_SEND_DTMF        = 14,
  STK_CMD_PLAY_TONE              = 20,
  STK_CMD_GET_INKEY              = 22,
  STK_CMD_GET_INPUT              = 23,
  STK_CMD_SETUP_IDLE_MODE_TEXT   = 28,
  STK_CMD_SETUP_RUN_AT_COMMAND   = 34,

  STK_CMD_SETUP_LAUNCH_BROWSER   = 15,
  STK_CMD_SETUP_LANGUAGE_NOTIF   = 35,
  STK_CMD_SETUP_OPEN_CHANNEL     = 40,
  STK_CMD_SETUP_CLOSE_CHANNEL    = 41,
  STK_CMD_SETUP_RECEIVE_DATA     = 42,
  STK_CMD_SETUP_SEND_DATA        = 43,
  STK_CMD_GPRS_CALL_CONTROL      = 50,

  STK_END_OF_TRANSACTION         = 81
}
StkCommandId;

/***************************************************************************
 *  Macros
 **************************************************************************/

/***************************************************************************
 *  Variables
 **************************************************************************/

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

Boolean vgStkInterfaceController (const SignalBuffer *signal,
                                   const VgmuxChannelNumber entity);

void vgStkTimerExpiry            (const VgmuxChannelNumber entity);
void vgCiSTKToneTimerExpired     (const VgmuxChannelNumber entity);

#endif

/* END OF FILE */
