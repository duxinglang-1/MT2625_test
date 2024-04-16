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
 * definition of SAPI
 **************************************************************************/

#ifndef GPSAPI_H
#define GPSAPI_H

/**** NESTED INCLUDE FILES *************************************************/

/** Identifies the service access point that is used for the GPRS data 
 * transfer at LLC layer.
 *(ref. GSM 04.64/6.2.3)
 * 
 * SAPIs are used at the SNDCP-LLC interface. A SAPI is associated with a
 * quality of service. For acknowledged data transfer, the SAPI is negotiated
 * with the network. For unacknowledged data transfer, the SAPI is chosen by SM
 * locally. GSM 04.64 states that "ABM operation for SAPIs 1 and 7 is not
 * permitted". This has no impact on SNDCP, which uses only SAPI values 3, 5, 9
 * and 11.
 */
typedef enum LlcSapiTag
{
   /** LLC SAPI not assigned
    * (Vaue to be used in UMTS single mode)
  */
    LLC_SAPI_NOT_ASSIGNED = 0,
    /**  SAPI GMM */
    GPRS_SAPI_LLGMM    =  1, 
    /** SAPI 3 */
    GPRS_SAPI_LL3      =  3,  
    /** SAPI 5 */
    GPRS_SAPI_LL5      =  5, 
    /** Not included in NUM_OF_SAPIS yet! */
    GPRS_SAPI_LLSMS    =  7, 
    /** SAPI 9 */
    GPRS_SAPI_LL9      =  9, 
    /** SAPI 11 */
    GPRS_SAPI_LL11     = 11,   
    /** Out of range (0 to 15) SAPI value */
    GPRS_SAPI_RESERVED = 16    
}
LlcSapi;

#define GPRS_NUM_OF_SAPIS 6  /* SAPIs supported by LLC */
#define GPRS_NUM_OF_TLLIS 1  /* TLLIs supported by LLC */
#define GPRS_NUM_OF_DLCIS (GPRS_NUM_OF_SAPIS * GPRS_NUM_OF_TLLIS) /* DLCIs */


#endif

/* END OF FILE */
