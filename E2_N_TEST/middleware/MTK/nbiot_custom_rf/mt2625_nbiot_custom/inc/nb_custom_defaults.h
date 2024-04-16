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
*******************************************************************************/
#ifndef NB_CUSTOM_DEFAULTS_H
#define NB_CUSTOM_DEFAULTS_H


/* ========================================================================= */
/*   FORMAT CONVERSION MULTIPLIERS                                           */
/* ========================================================================= */
#define NB_CUSTOM_Q8_FIXEDPOINT_MULT        (256)

#include "nb_custom_defaults_hw_var0.h"
#include "nb_custom_defaults_hw_var1.h"
#include "nb_custom_defaults_hw_var2.h"
#include "nb_custom_defaults_hw_var3.h"
#include "nb_custom_defaults_hw_var4.h"

/*************************************************************************/
/* DRDI mode default value, constomer can change it                         */
/*brief: the relation of DRDI mode and Chip option
*    RF_HW_DYNAMIC_USID           -->  E3 chip +ES2(SKY68018)/Qorvo PA or other FEM...
*    RF_HW_DYNAMIC                ---->  E3 chip + ES1/ES2(dynamic select by adc pinmux) FEM; 
*    RF_HW_VAR_SINGLE_PA          ---->  E2/E3 chip + single PA; 
*    RF_HW_VAR_1                  ---->  E2/E3 chip + ES1 FEM;
*    RF_HW_VAR_2                  ---->  E2/E3 chip + ES2 FEM;
*    RF_HW_VAR_3                  ---->  configure 2621_chip of BPI parameter
*    RF_HW_VAR_4                  ---->  MT2625 + QM55001_E2
*************************************************************************/
#define NB_CUSTOM_DATA_DRDI_MODE_SET                (uint32_t)RF_HW_VAR_2

/*default DRDI data values*/
#define Low_VOLTAGE_HW_VAR1                         (uint16_t)0
#define High_VOLTAGE_HW_VAR1                        (uint16_t)200
#define Low_VOLTAGE_HW_VAR2                         (uint16_t)800
#define High_VOLTAGE_HW_VAR2                        (uint16_t)1000
#define Low_VOLTAGE_HW_VAR3                         (uint16_t)1000
#define High_VOLTAGE_HW_VAR3                        (uint16_t)1200

#define Low_VOLTAGE_HW_VAR4                         (uint16_t)600
#define High_VOLTAGE_HW_VAR4                        (uint16_t)800

/* single PA memory size manager*/
//#define NB_RF_CUSTOM_SINGLE_PA

/* internal band list control,
  **for custom, it should be disabled.
  ** for internal use, it can be enabled */
#define INTERNAL_BAND_LIST_CTRL            RF_INTERNAL_BAND_LIST_DISABLED

#endif /* NB_CUSTOM_DEFAULTS_H */
/* end of file */
