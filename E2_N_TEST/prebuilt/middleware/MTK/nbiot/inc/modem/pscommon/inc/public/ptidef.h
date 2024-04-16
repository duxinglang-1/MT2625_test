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
 * File Description:                                                     */
/** \file
 * Definitions related to PTI (Procedure Transaction Identity).
 * Ref: 3GPP 24.007 11.2.3.1a
 *
 * \note Although the PTI is specified for LTE in 3GPP specifications,
 * we use it as an identifier between the MDL and NAS in both LTE and 2G/3G.
 *
 * \note PTI must always be of type Int8.
 ***************************************************************************/

#ifndef PTIDEF_H
#define PTIDEF_H

/***************************************************************************
*   Manifest Constants
***************************************************************************/

/** No procedure transaction identity assigned */
#define PTI_UNASSIGNED          (0)
/** Reserved PTI value */
#define PTI_RESERVED            (255)
/** Maximum number of PTI values useable (0-254) */
#define MAX_NUM_PTI_VALUES       (255)

/***************************************************************************
*   Types
***************************************************************************/

/** Type of request that caused the PTI allocation */
typedef enum PtiRequestTypeTag
{
    /** No PTI request */
    PTI_REQ_NONE,
    /** PTI was allocated because of a PDN Connectivity Request */
    PTI_REQ_PDN_CONNECTIVITY,
    /** PTI was allocated because of a PDN Disconnect Request */
    PTI_REQ_PDN_DISCONNECT,
    /** PTI was allocated because of a Bearer Resource Allocation Request */
    PTI_REQ_BEARER_RESOURCE_ALLOC,
    /** PTI was allocated because of a Bearer Resource Modification Request */
    PTI_REQ_BEARER_RESOURCE_MODIFY,
    /** PTI was allocated because of a ESM Data Transport */
    PTI_REQ_ESM_DATA_TRANSPORT
}
PtiRequestType;

#endif /* PTIDEF_H */

/* END OF FILE */
