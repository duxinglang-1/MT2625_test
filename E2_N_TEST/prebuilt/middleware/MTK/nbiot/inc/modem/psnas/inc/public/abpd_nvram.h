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
 ***************************************************************************
 *
 * File Description:
 *
 * ABPD type definitions for storing data to NVRAM ()
 *
 **************************************************************************/

#ifndef ABPD_NVRAM_H
#define ABPD_NVRAM_H

/***************************************************************************

***************************************************************************/
/* Temporary definition until real RTC macro for modem is available */
#if !defined ATTR_MD_RWDATA_IN_RETSRAM
#define ATTR_MD_RWDATA_IN_RETSRAM
#endif

#define NV_RAM_DATA_BLOCK_SIZE         2000
#define NV_RAM_APN_INFO_BLOCK_SIZE     NV_RAM_DATA_BLOCK_SIZE
#define NV_RAM_PCO_BLOCK_SIZE          sizeof(ProtocolConfigOptions)
#define NAS_GROUP_NAME                 "nas"
#define NVRAM_ABPD_NAME_LENGTH         15
#define NVRAM_ABPD_PCO1                "pco_store1"
#define NVRAM_ABPD_PCO2                "pco_store2"
#define NVRAM_ABPD_PCO3                "pco_store3"
#define NVRAM_ABPD_PCO4                "pco_store4"
#define NVRAM_ABPD_APN_INFO_STORE      "apn_info_s"

#ifdef FEA_TEMP_PDN_ACCESS_CONTROL
#define NVRAM_ABPD_BLOCKER_INFO_STORE  "blocker_info_s"
#endif

/* Indicator for unsuccessful writing of TFT data to NVRAM */
#define PCO_NVRAM_INDEX_NOT_SET 0xFF

typedef enum AbpdNvramDataNameTag
{
   NRAM2_PCO_STORE1,
   NRAM2_PCO_STORE2,
   NRAM2_PCO_STORE3,
   NRAM2_PCO_STORE4,
   NRAM2_APN_INFO_STORE
}
AbpdNvramDataName;

typedef struct AbpdPcoStoreTag{
    Int8  epsBearerId;
    Int16 pcoDataLength;
    Int8  pcoData[NV_RAM_PCO_BLOCK_SIZE];
}
AbpdPcoStore;

typedef struct AbpdApnInfoStoreTag{
    Int16 apnInfoLength;
    Int8  apnInfoData[NV_RAM_APN_INFO_BLOCK_SIZE];
}
AbpdApnInfoStore;

#endif /* ABPD_NVRAM_H */

/* END OF FILE */
