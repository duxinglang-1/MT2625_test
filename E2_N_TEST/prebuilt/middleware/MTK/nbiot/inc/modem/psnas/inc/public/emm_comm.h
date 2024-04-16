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
 * File Description :                                                    */
/** \file
 *
 * Contains definitions of  EMM data common to several other header files
 **************************************************************************/


#ifndef EMM_COMM_H
#define EMM_COMM_H

/** EMM specific failure causes */
typedef enum EmmCauseTag
{
  NO_CAUSE                          = 0x00,
  IMSI_UNKNOWN_IN_HSS               = 0x02,
  ILLEGAL_UE                        = 0x03,
  IMEI_NOT_ACCEPTED                 = 0x05,
  ILLEGAL_ME                        = 0x06,
  EPS_SERVICES_NOT_ALLOWED          = 0x07,
  EPS_AND_NON_SERVICES_NOT_ALLOWED  = 0x08,
  UE_IDENTITY_NOT_DERIVED_IN_NW     = 0x09,
  IMPLICITLY_DETACHED               = 0x0A,
  PLMN_NOT_ALLOWED                  = 0x0B,
  TRACKING_AREA_NOT_ALLOWED         = 0x0C,
  ROAMING_NOT_ALLOWED_IN_THIS_TA    = 0x0D,
  EPS_SERVICES_NOT_ALLOWED_IN_PLMN  = 0x0E,
  NO_SUITABLE_CELLS_IN_TA           = 0x0F,
  MSC_TEMP_NOT_REACHABLE            = 0x10,
  NETWORK_FAILURE                   = 0x11,
  CS_DOMAIN_NOT_AVAILABLE           = 0x12,
  ESM_FAILURE                       = 0x13,
  MAC_FAILURE                       = 0x14,
  SYNCH_FAILURE                     = 0x15,
  CONGESTION                        = 0x16,
  UE_SECURITY_CAP_MISMATCH          = 0x17,
  SECURITY_MODE_REJECT_UNSPECIFIED  = 0x18,
  NOT_AUTHORISED_FOR_THIS_CSG       = 0x19,
  NON_EPS_AUTHENTICATE_UNACCEPTABLE = 0x1A,
  SERVICE_OPTION_NOT_AUTHORIZED     = 0X23,
  CS_DOMAIN_TEMP_UNAVAILABLE        = 0x27,
  NO_EPS_BEARER_ACTIVATED           = 0x28,
  SEVERE_NETWORK_FAILURE            = 0x2A,
  MESSAGE_SEMANTICALY_INCORRECT     = 0x5F,
  INVALID_MANDATORY_INFO            = 0x60,
  MSG_TYPE_DOES_NOT_EXIST           = 0x61,
  MSG_TYP_NOT_COMPAT_WITH_STATE     = 0x62,
  IE_DOES_NOT_EXIST                 = 0x63,
  CONDITIONAL_IE_ERROR              = 0x64,
  MESSAGE_NOT_COMPAT_WITH_STATE     = 0x65,
  PROTOCOL_ERROR_UNSPECIFIED        = 0x6F
}
EmmCause;

/** The SUB-states of EMM used to control the EMM state machine - do not
alter the ordering of states!  */
typedef enum EmmStateTag
{
  EMM_NULL = 0x00,
  EMM_DEREGISTERED_INITIATED = 0x01,
  EMM_DEREGISTERED_NORMAL = 0x02,
  EMM_DEREGISTERED_LIMITED_SERVICE = 0x03,
  EMM_DEREGISTERED_ATTEMPTING_TO_ATTACH = 0x04,
  EMM_DEREGISTERED_PLMN_SEARCH = 0x05,
  EMM_DEREGISTERED_NO_IMSI = 0x06,
  EMM_DEREGISTERED_ATTACH_NEEDED = 0x07,
  EMM_DEREGISTERED_NO_CELL_AVAILABLE = 0x08,
  EMM_REGISTERED_INITIATED = 0x09,
  EMM_REGISTERED_NORMAL = 0x0A,
  EMM_REGISTERED_ATTEMPTING_TO_UPDATE = 0x0B,
  EMM_REGISTERED_LIMITED_SERVICE = 0x0C,
  EMM_REGISTERED_PLMN_SEARCH = 0x0D,
  EMM_REGISTERED_UPDATE_NEEDED = 0x0E,
  EMM_REGISTERED_NO_CELL_AVAILABLE = 0x0F,
  EMM_REGISTERED_ATTEMPTING_TO_UPDATE_MM = 0x10,
  EMM_REGISTERED_IMSI_DETACH_INITIATED = 0x11,
  EMM_SERVICE_REQUEST_INITIATED = 0x12,
  EMM_TRACKING_AREA_UPDATING_INITIATED = 0x13,
  EMM_NUM_STATES = 0x14
}
EmmState;


#endif
