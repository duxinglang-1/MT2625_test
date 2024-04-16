/* Copyright Statement:
 *
 * (C) 2005-2017 MediaTek Inc. All rights reserved.
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
 * the License Agreement ("Permitted User"). If you are not a Permitted User,
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

#ifndef __TEL_CONN_MGR_BEARER_URC_H__
#define __TEL_CONN_MGR_BEARER_URC_H__

#include "tel_conn_mgr_platform.h"
#include "tel_conn_mgr_common.h"


typedef enum
{
    TEL_CONN_MGR_BEARER_URC_TYPE_CGEV,
    TEL_CONN_MGR_BEARER_URC_TYPE_CGREG,

    TEL_CONN_MGR_BEARER_URC_TYPE_MAX
}tel_conn_mgr_bearer_urc_type_enum;


typedef enum
{   
    TEL_CONN_MGR_BEARER_CGEV_URC_TYPE_NW_DETACH,
    TEL_CONN_MGR_BEARER_CGEV_URC_TYPE_ME_DETACH,
    TEL_CONN_MGR_BEARER_CGEV_URC_TYPE_NW_PDN_DEACT,
    TEL_CONN_MGR_BEARER_CGEV_URC_TYPE_ME_PDN_DEACT,
    TEL_CONN_MGR_BEARER_CGEV_URC_TYPE_ME_PDN_ACT,
    
    TEL_CONN_MGR_BEARER_CGEV_URC_TYPE_MAX,
}tel_conn_mgr_bearer_cgev_urc_type_enum;



typedef struct
{
    unsigned int msg_id;  /* MSG_ID_TEL_CONN_MGR_BEARER_URC_IND */
    tel_conn_mgr_bearer_urc_type_enum urc_type;
    void *urc_detail;
}tel_conn_mgr_bearer_urc_ind_struct;



typedef struct
{
    tel_conn_mgr_bearer_cgev_urc_type_enum cgev_urc_type;
    int cid;
    int reason;
}tel_conn_mgr_cgev_urc_struct;

tel_conn_mgr_ret_enum tel_conn_mgr_urc_process(tel_conn_mgr_bearer_urc_ind_struct *urc_ind);

tel_conn_mgr_ret_enum tel_conn_mgr_bearer_urc_init(void);

tel_conn_mgr_ret_enum tel_conn_mgr_bearer_urc_deinit(void);

void tel_conn_mgr_bearer_free_urc_ind(tel_conn_mgr_bearer_urc_ind_struct *urc_ind);


#endif /* __TEL_CONN_MGR_BEARER_URC_H__ */

