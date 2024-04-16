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
 * File Description                                                      */
/** \file
 * Background Cell Broadcast type definitions & constants
 **************************************************************************/

#ifndef ABCB_TYP_H
#define ABCB_TYP_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/


/***************************************************************************
 * Manifest Constants
 **************************************************************************/


/***************************************************************************
 * Typed Constants
 **************************************************************************/

/** Determines the outcome of the request */
typedef enum CbRequestStatusTag
{
  CB_REQ_OK,                 /**< request successful */
  CB_REQ_CB_NOT_READY,       /**< Cell broadcast procedure is not ready */
  CB_REQ_SIM_ERROR,          /**< Sim has reported error */
  CB_REQ_PS_ERROR,           /**< PS has reported error */
  CB_REQ_INVALID_TYPE,       /**< Type is not configured */
  CB_REQ_NO_NEW_MSGS,        /**< No unread messages */
  CB_REQ_NO_MSG,             /**< no message of this type received */
  CB_REQ_INVALID_INDEX,      /**< The gicen index isn't valid */
  CB_REQ_TOO_MANY_TYPES,     /**< more types are enabled than will fit in an MncbConfigureReq ie CBMI_ENTRIES */
  CB_REQ_TOO_MANY_LANGUAGES, /**< more languages are enabled than will fit in an MncbConfigureReq ie SIM_LANGUAGE_LIST_SIZE */
  CB_REQ_ERROR,              /**< Unspecified error */
}
CbRequestStatus;


/***************************************************************************
 * Type Definitions
 **************************************************************************/


/***************************************************************************
 *  Macros
 **************************************************************************/


/***************************************************************************
 *  Function Prototypes
 **************************************************************************/


#endif

/* END OF FILE */

