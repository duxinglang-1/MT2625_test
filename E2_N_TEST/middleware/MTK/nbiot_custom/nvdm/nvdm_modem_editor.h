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
/****************************************************************************
 * This file is used for generate NVDM database
 ****************************************************************************/
#ifndef __NVDM_MODEM_EDITOR_H__
#define __NVDM_MODEM_EDITOR_H__


/****************************************************************************
 * Include Files
 ****************************************************************************/
#include <system.h>
#include <dmnvmcfg.h>
#include <afnv_typ.h>
#if defined (MTK_NBIOT_TARGET_BUILD)
#include "n1_rf_cal_data.h"
#include "nb_custom_data.h"
#endif

typedef struct{
    char CHAR_DATA;
} nvdm_char;

typedef struct{
    uint8_t UINT8_DATA;
} nvdm_uint8;

typedef struct{
    int8_t INT8_DATA;
}nvdm_int8;

typedef struct{
    uint16_t UINT16_DATA;
} nvdm_uint16;

typedef struct{
    int16_t INT16_DATA;
} nvdm_int16;

typedef struct{
    uint32_t UINT32_DATA;
} nvdm_uint32;

typedef struct{
    int32_t INT32_DATA;
} nvdm_int32;

typedef struct{
    bool BOOL_DATA;
} nvdm_bool;

#undef NVDM_MODEM_ITEM_DEF
#undef NVDM_MODEM_ITEM_DEF_NOGEN_DESCRIPTION
#define NVDM_MODEM_ITEM_DEF(NV_NAME, STRUCTURE, GROUP_NAME, ITEM_NAME, AREA, DEFAULT, USE_VER, VERNO) NV_NAME,
#define NVDM_MODEM_ITEM_DEF_NOGEN_DESCRIPTION(NV_NAME, STRUCTURE, GROUP_NAME, ITEM_NAME, AREA, DEFAULT, USE_VER, VERNO) NV_NAME,
typedef enum
{
    NVDM_MODEM_START_LID = 0x0,
    #include "nvdm_modem_data_item_table.h"
    NVDM_MODEM_ITEM_MAX
}NVDM_MODEM_LID;



/****************************************************************************
 * Structure description define
 ****************************************************************************/
#ifdef GEN_NVDM_DATABASE_FOR_PC
#define VER_LID(x) "000" x

BEGIN_NVRAM_DATA
#undef NVDM_MODEM_ITEM_DEF
#undef NVDM_MODEM_ITEM_DEF_NOGEN_DESCRIPTION
#define NVDM_MODEM_ITEM_DEF_NOGEN_DESCRIPTION(NV_NAME, STRUCTURE, GROUP_NAME, ITEM_NAME, AREA, DEFAULT, USE_VER, VERNO)
#define NVDM_MODEM_ITEM_DEF(NV_NAME, STRUCTURE, GROUP_NAME, ITEM_NAME, AREA, DEFAULT, USE_VER, VERNO) LID_BIT "000" NV_NAME \
                                                     STRUCTURE * 1 \
                                                     {             \
                                                     };
#include "nvdm_modem_data_item_table.h"

//User can add structure description below if necessary for ITEM with "NVDM_MODEM_ITEM_DEF_NOGEN_DESCRIPTION"
LID_BIT "000" NVDM_MODEM_CFG_BAND_SUPPORT
nvdm_uint32 * 1 {
    UINT32_DATA:"BAND_SUPPORT_INFO"
    {
        BAND_73:1 "BAND 73"{
            0: "Not support";
            1: "Support";
        };
        BAND_74:1 "BAND 74"{
            0: "Not support";
            1: "Support";
        };
        Dummy:6 "Dummy"{
        };

        BAND_66:1 "BAND 66"{
            0: "Not support";
            1: "Support";
        };
        BAND_70:1 "BAND 70"{
            0: "Not support";
            1: "Support";
        };
        BAND_21:1 "BAND 21"{
            0: "Not support";
            1: "Support";
        };
        BAND_4:1 "BAND 4"{
            0: "Not support";
            1: "Support";
        };
        BAND_71:1 "BAND 71"{
            0: "Not support";
            1: "Support";
        };
        BAND_85:1 "BAND 85"{
            0: "Not support";
            1: "Support";
        };
        BAND_14:1 "BAND 14"{
            0: "Not support";
            1: "Support";
        };
        BAND_72:1 "BAND 72"{
            0: "Not support";
            1: "Support";
        };

        BAND_17:1 "BAND 17"{
            0: "Not support";
            1: "Support";
        };
        BAND_18:1 "BAND 18"{
            0: "Not support";
            1: "Support";
        };
        BAND_19:1 "BAND 19"{
            0: "Not support";
            1: "Support";
        };
        BAND_20:1 "BAND 20"{
            0: "Not support";
            1: "Support";
        };
        BAND_25:1 "BAND 25"{
            0: "Not support";
            1: "Support";
        };
        BAND_26:1 "BAND 26"{
            0: "Not support";
            1: "Support";
        };
        BAND_28:1 "BAND 28"{
            0: "Not support";
            1: "Support";
        };
        BAND_31:1 "BAND 31"{
            0: "Not support";
            1: "Support";
        };


        BAND_1:1 "BAND 1"{
            0: "Not support";
            1: "Support";
        };
        BAND_2:1 "BAND 2"{
            0: "Not support";
            1: "Support";
        };
        BAND_3:1 "BAND 3"{
            0: "Not support";
            1: "Support";
        };
        BAND_5:1 "BAND 5"{
            0: "Not support";
            1: "Support";
        };
        BAND_8:1 "BAND 8"{
            0: "Not support";
            1: "Support";
        };
        BAND_11:1 "BAND 11"{
            0: "Not support";
            1: "Support";
        };
        BAND_12:1 "BAND 12"{
            0: "Not support";
            1: "Support";
        };
        BAND_13:1 "BAND 13"{
            0: "Not support";
            1: "Support";
        };
    };
};

LID_BIT "000" TEST_OPERATOR
nvdm_uint8 * 1 {
    UINT8_DATA:"" {
        OPERATOR:8 "OPERATOR_ENUM" {
            0x00: "OPERATOR_UNKNOWN";
            0x01: "OPERATOR_CMCC";
            0x02: "OPERATOR_CT";
            0x03: "OPERATOR_CU";
            0x04: "OPERATOR_SOFTBANK";
            0x05: "OPERATOR_TMO";
            0x06: "OPERATOR_DEUTSCHE_TELEKOM";
            0x07: "OPERATOR_LGUP";
            0x08: "OPERATOR_RJIO";
        };
    };
};

END_NVRAM_DATA
#endif /* GEN_NVDM_DATABASE_FOR_PC */

#endif
