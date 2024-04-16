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
 * Signal definitions for the MDT interface.
 **************************************************************************/

SIG_DEF( SIG_MDT_UNITDATA_IND,              MdtUnitdataInd              mdtUnitdataInd     )
SIG_DEF( SIG_MDT_UNITDATA_REQ,              MdtUnitdataReq              mdtUnitdataReq     )
SIG_DEF( SIG_MDT_CHANNEL_NUMBER_IND,        MdtChannelNumberInd         mdtChannelNumberInd )
SIG_DEF( SIG_MDT_STREAM_DATA_IND,           MdtStreamDataInd            mdtStreamDataInd   )
SIG_DEF( SIG_MDT_STREAM_DATA_REQ,           MdtStreamDataReq            mdtStreamDataReq   )
SIG_DEF( SIG_MDT_STREAM_XON_REQ,            MdtStreamXonReq             mdtStreamXonReq    )
SIG_DEF( SIG_MDT_STREAM_XOFF_REQ,           MdtStreamXoffReq            mdtStreamXoffReq   )
SIG_DEF( SIG_MDT_CAPABILITIES_REQ,          MdtCapabilitiesReq          mdtCapabilitiesReq )
SIG_DEF( SIG_MDT_SETUP_IND,                 MdtSetupInd                 mdtSetupInd        )
SIG_DEF( SIG_MDT_CAPABILITIES2_REQ,         MdtCapabilities2Req         mdtCapabilities2Req )
SIG_DEF( SIG_MDT_SETUP2_IND,                MdtSetup2Ind                mdtSetup2Ind       )
SIG_DEF( SIG_MDT_CLOSE_DOWN_REQ,            MdtCloseDownReq             mdtCloseDownReq    )
SIG_DEF( SIG_MDT_CLOSE_DOWN_IND,            MdtCloseDownInd             mdtCloseDownInd    )
SIG_DEF( SIG_MDT_UL_FLOW_CTRL_IND,          MdtUlFlowCtrlInd            mdtUlFlowCtrlInd   )
SIG_DEF( SIG_MUX_APB_FLOW_CTRL_IND,         MuxApbFlowCtrlInd           muxApbFlowCtrlInd   )

/* END OF FILE */

