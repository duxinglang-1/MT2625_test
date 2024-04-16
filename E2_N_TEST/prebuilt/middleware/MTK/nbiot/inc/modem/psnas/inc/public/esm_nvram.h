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
 * ESM type definitions for storing data to NV RAM
 *
 **************************************************************************/

#ifndef ESM_NVRAM_H
#define ESM_NVRAM_H

/***************************************************************************

***************************************************************************/
#define NV_RAM_DATA_BLOCK_SIZE 2000
#define NV_RAM_QOS_BLOCK_SIZE  NV_RAM_DATA_BLOCK_SIZE
#define NV_RAM_TFT_BLOCK1_SIZE NV_RAM_DATA_BLOCK_SIZE
#define NV_RAM_TFT_BLOCK2_SIZE (sizeof(TrafficFlowTemplate) - NV_RAM_TFT_BLOCK1_SIZE)

#define NAS_GROUP_NAME       "nas"
#define NVRAM_TFT_NAME_LENGTH 15
#define NVRAM_BEARER1_TFT_D1 "esm_b1_tft_d1"
#define NVRAM_BEARER1_TFT_D2 "esm_b1_tft_d2"
#define NVRAM_BEARER2_TFT_D1 "esm_b2_tft_d1"
#define NVRAM_BEARER2_TFT_D2 "esm_b2_tft_d2"
#define NVRAM_BEARER3_TFT_D1 "esm_b3_tft_d1"
#define NVRAM_BEARER3_TFT_D2 "esm_b3_tft_d2"
#define NVRAM_BEARER4_TFT_D1 "esm_b4_tft_d1"
#define NVRAM_BEARER4_TFT_D2 "esm_b4_tft_d2"
#define NVRAM_QOS_DATA       "esm_qos_data"

typedef struct EsmQosDataTag{
    Int16 esmQosDataLength;
    Int8  esmQosData[NV_RAM_QOS_BLOCK_SIZE];
}
EsmQosData;

typedef struct EsmBearerTftNvBlock1Tag{
    Int8  epsBearerId;
    Int16 esmTftDataLength;
    Int8  esmTftData[NV_RAM_TFT_BLOCK1_SIZE];
}
EsmBearerTftNvBlock1;

typedef struct EsmBearerTftNvBlock2Tag{
    Int8  epsBearerId;
    Int16 esmTftDataLength;
    Int8  esmTftData[NV_RAM_TFT_BLOCK2_SIZE];
}
EsmBearerTftNvBlock2;

#endif /* ESM_NVRAM_H */

/* END OF FILE */
