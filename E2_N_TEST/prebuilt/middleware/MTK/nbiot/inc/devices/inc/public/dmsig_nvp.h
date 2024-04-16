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
 *  File Description :
 *      DM Layer Non-Volatile RAM Manager Signals
 **************************************************************************/

    /* NVRAM Processor task signals. */
#if defined (EXCLUDE_DM)
#if defined (MTK_NBIOT_TARGET_BUILD) || defined (UT_TARGET_CONFIG)
    SIG_DEF( SIG_DM_NVP_READ_REQ,                  EmptySignal                dmNvPReadReq              )
    SIG_DEF( SIG_DM_NVP_READ_CNF,                  EmptySignal                dmNvPReadCnf              )
    SIG_DEF( SIG_DM_NVP_WRITE_REQ,                 EmptySignal                dmNvPWriteReq             )
    SIG_DEF( SIG_DM_NVP_WRITE_CNF,                 EmptySignal                dmNvPWriteCnf             )
#if 0
    SIG_DEF( SIG_DM_NVP_READ_CAL_REQ,              EmptySignal                dmNvPReadCalReq           )
    SIG_DEF( SIG_DM_NVP_READ_CAL_CNF,              EmptySignal                dmNvPReadCalCnf           )
#endif
    SIG_DEF( SIG_DM_NVP_DELETE_REQ,                EmptySignal                dmNvPDeleteReq            )
    SIG_DEF( SIG_DM_NVP_DELETE_CNF,                EmptySignal                dmNvPDeleteCnf            )
#endif
#else
    SIG_DEF( SIG_DM_NVP_READ_REQ,                  DmNvPReadReq               dmNvPReadReq              )
    SIG_DEF( SIG_DM_NVP_READ_CNF,                  DmNvPReadCnf               dmNvPReadCnf              )
    SIG_DEF( SIG_DM_NVP_WRITE_REQ,                 DmNvPWriteReq              dmNvPWriteReq             )
    SIG_DEF( SIG_DM_NVP_WRITE_CNF,                 DmNvPWriteCnf              dmNvPWriteCnf             )
#if 0
    SIG_DEF( SIG_DM_NVP_WRITE_CAL_REQ,             DmNvPWriteCalReq           dmNvPWriteCalReq          )
    SIG_DEF( SIG_DM_NVP_WRITE_CAL_CNF,             DmNvPWriteCalCnf           dmNvPWriteCalCnf          )
    SIG_DEF( SIG_DM_NVP_ERASE_CAL_REQ,             DmNvPEraseCalReq           dmNvPEraseCalReq          )
    SIG_DEF( SIG_DM_NVP_ERASE_CAL_CNF,             DmNvPEraseCalCnf           dmNvPEraseCalCnf          )
    SIG_DEF( SIG_DM_NVP_READ_CAL_REQ,              DmNvPReadCalReq            dmNvPReadCalReq           )
    SIG_DEF( SIG_DM_NVP_READ_CAL_CNF,              DmNvPReadCalCnf            dmNvPReadCalCnf           )
#endif
    SIG_DEF( SIG_DM_NVP_DELETE_REQ,                DmNvPDeleteReq             dmNvPDeleteReq            )
    SIG_DEF( SIG_DM_NVP_DELETE_CNF,                DmNvPDeleteCnf             dmNvPDeleteCnf            )
#endif
