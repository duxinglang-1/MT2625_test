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
 * Signal definitions for the CONN interface.
 **************************************************************************/

SIG_DEF( SIG_CONN_INIT2_IND,                ConnInit2Ind                connInit2Ind            )
SIG_DEF( SIG_CONN_INIT_RSP,                 ConnInitRsp                 connInitRsp             )
SIG_DEF( SIG_CONN_CLOSED_IND,               ConnClosedInd               connClosedInd           )
SIG_DEF( SIG_CONN_DATA_SETUP_REQ,           ConnDataSetupReq            connDataSetupReq        )
SIG_DEF( SIG_CONN_DATA_SETUP_CNF,           ConnDataSetupCnf            connDataSetupCnf        )
SIG_DEF( SIG_CONN_DATA_CLOSE_REQ,           ConnDataCloseReq            connDataCloseReq        )
SIG_DEF( SIG_CONN_DATA_CLOSED_IND,          ConnDataClosedInd           connDataClosedInd       )
SIG_DEF( SIG_CONN_CONFIGURE_CHANNEL_REQ,    ConnConfigureChannelReq     connConfigureChannelReq )
SIG_DEF( SIG_CONN_DL_LINE_STATUS_REQ,       ConnDlLineStatusReq         connDlLineStatusReq     )
SIG_DEF( SIG_CONN_UL_LINE_STATUS_IND,       ConnUlLineStatusInd         connUlLineStatusInd     )
SIG_DEF( SIG_CONN_CTRL_MSG_REQ,             ConnCtrlMsgReq              connCtrlMsgReq          )
SIG_DEF( SIG_CONN_CTRL_MSG_CNF,             ConnCtrlMsgCnf              connCtrlMsgCnf          )
SIG_DEF( SIG_CONN_CTRL_MSG_IND,             ConnCtrlMsgInd              connCtrlMsgInd          )
SIG_DEF( SIG_CONN_CTRL_MSG_RSP,             ConnCtrlMsgRsp              connCtrlMsgRsp          )
SIG_DEF( SIG_CONN_ATCI_READY_RSP,           EmptySignal                 connAtciReadyRsp        )

/* END OF FILE */

