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
 * Signal definitions for the CIAPB interface.
 **************************************************************************/

SIG_DEF( SIG_CIAPB_AT_COMMAND_REQ,                  CiApbAtCommandReq               ciApbAtCommandReq         )
SIG_DEF( SIG_CIAPB_AT_RESPONSE_IND,                 CiApbAtResponseInd              ciApbAtResponseInd        )
SIG_DEF( SIG_CIAPB_CLOSE_DATA_CONN_REQ,             CiApbCloseDataConnReq           ciApbCloseDataConnReq     )
SIG_DEF( SIG_CIAPB_OPEN_DATA_CONN_REQ,              CiApbOpenDataConnReq            ciApbOpenDataConnReq      )
SIG_DEF( SIG_CIAPB_REGISTER_AT_COMMAND_IND,         CiApbRegisterAtCommandInd       ciApbRegisterAtCommandInd )
SIG_DEF( SIG_CIAPB_REGISTER_AT_COMMAND_RSP,         CiApbRegisterAtCommandRsp       ciApbRegisterAtCommandRsp )
SIG_DEF( SIG_APB_L4MM_FLOW_CONTROL_IND,             ApbL4mmFlowControlInd           apbL4mmFlowControlInd     )
SIG_DEF( SIG_CIAPB_DATA_MODE_TEMP_DEACTIVATED_IND,  ApbDataModeTempDeactivatedInd   apbDataModeTempDeactivatedInd)
SIG_DEF( SIG_CIAPB_DATA_MODE_RESUMED_IND,           ApbDataModeResumedInd           apbDataModeResumedInd     )
SIG_DEF( SIG_CIAPB_DATA_MODE_RESUME_REQ,            ApbDataModeResumeReq            apbDataModeResumeReq      )

/* END OF FILE */
