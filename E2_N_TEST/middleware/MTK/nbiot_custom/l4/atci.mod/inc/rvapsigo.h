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
 * Header file for vgapsigo.c
 **************************************************************************/

#ifndef RVAPSIGO_H
#define RVAPSIGO_H

/***************************************************************************
 * Include Files
 **************************************************************************/

 #include <system.h>
 #include <rvutil.h>
 #include <ciapb_sig.h>

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/
/*--------------------------------------------------------------------------
*
* Function:    vgSigCiapbRegisterAtCommandRsp
*
* Parameters:  request_p - points to register AT command response data structure.
*
* Returns:     Nothing
*
* Description: Function which sends a SIG_CIAPB_REGISTER_AT_COMMAND_RSP
*              to the AP Bridge to inform the register results to AP modain.
*-------------------------------------------------------------------------*/
void vgSigCiapbRegisterAtCommandRsp(CiApbRegisterAtCommandRsp* request_p);

/*--------------------------------------------------------------------------
*
* Function:    vgSigCiapbAtCommandReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*              pCmdString - the whole data for the specific AT Command.
*              length - the length of the command's data.
* Returns: Identify whether the signal is sent out or not.
*
* Description: Sending a SIG_CIAPB_AT_COMMAND_REQ to the AP Bridge to
*              deliver the AT Command request to AP modain to be executed.
*-------------------------------------------------------------------------*/
Boolean vgSigCiapbAtCommandReq(VgmuxChannelNumber entity,
                               const Char* pCmdString,
                               Int32 length);
/*--------------------------------------------------------------------------
*
* Function:    vgSigCiapbOpenDataConnReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Function which sends a SIG_CIAPB_OPEN_DATA_CONN_REQ
*              to the AP Bridge to configure the data channel between MUX and AP Bridge.
*-------------------------------------------------------------------------*/
void vgSigCiapbOpenDataConnReq(VgmuxChannelNumber entity);
/*--------------------------------------------------------------------------
*
* Function:    vgSigCiapbCloseDataConnReq
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Function which sends a SIG_CIAPB_CLOSE_DATA_CONN_REQ
*              to the AP Bridge to close the data channel between MUX and AP Bridge.
*-------------------------------------------------------------------------*/
void vgSigCiapbCloseDataConnReq(VgmuxChannelNumber entity);
/*--------------------------------------------------------------------------
*
* Function:    vgSigCiapbDataModeTempDeactived
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Function which sends a SIG_CIAPB_DATA_MODE_TEMP_DEACTIVATED_IND
*              to the AP Bridge.
*-------------------------------------------------------------------------*/
void vgSigCiapbDataModeTempDeactived(VgmuxChannelNumber entity);
/*--------------------------------------------------------------------------
*
* Function:    vgSigCiapbDataModeResumed
*
* Parameters:  VgmuxChannelNumber - entity which sent request
*
* Returns:     Nothing
*
* Description: Function which sends a SIG_CIAPB_DATA_MODE_RESUMED_IND
*              to the AP Bridge.
*-------------------------------------------------------------------------*/
void vgSigCiapbDataModeResumed(VgmuxChannelNumber entity);

#endif  /* RVAPSIGO_H */
/* END OF FILE */

