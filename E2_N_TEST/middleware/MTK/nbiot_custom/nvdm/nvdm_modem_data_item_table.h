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
/*NVDM_MODEM_ITEM_DEF(NV_NAME, structure, GROUP_NAME, ITEM_NAME, area, default, USE_VER, VERNO)*/
/*NVDM_MODEM_ITEM_DEF_NOGEN_DESCRIPTION(NV_NAME, structure, GROUP_NAME, ITEM_NAME, area, default, USE_VER, VERNO) */

NVDM_MODEM_ITEM_DEF(NAS_ME_PERS,                                                  MePersonalisationData,         "nas",                 "me_pers",                       NVDM_MODEM_AREA_NORMAL,    Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(NAS_APN_STORE,                                                AbpdApn,                       "nas",                 "apn_store",                   NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(NPHY_SIG_NPBCH_SYMBOL_ROTATION_MODE,                          nvdm_uint8,                    "NPHY_SIG",            "NPBCH_SYMBOL_ROTATION_MODE",    NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(NPHY_SIG_NPDCCH_SCRAMBLING_INIT_CR265,                        nvdm_uint8,                    "NPHY_SIG",            "NPDCCH_SCRAMBLING_INIT_CR265",  NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF_NOGEN_DESCRIPTION(NVDM_MODEM_CFG_BAND_SUPPORT,                nvdm_uint32,                   "NVDM_MODEM_CFG",      "BAND_SUPPORT",                NVDM_MODEM_AREA_PROTECTED, Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(NVDM_MODEM_CFG_PSM_SUPPORT,                                   nvdm_bool,                     "NVDM_MODEM_CFG",      "PSM_SUPPORT",                 NVDM_MODEM_AREA_PROTECTED, Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(NVDM_MODEM_CFG_PSM_DEFAULT_T3324_VALUE,                       nvdm_uint8,                    "NVDM_MODEM_CFG",      "PSM_DEFAULT_T3324_VALUE",     NVDM_MODEM_AREA_PROTECTED, Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(NVDM_MODEM_CFG_PSM_DEFAULT_T3412_VALUE,                       nvdm_uint8,                    "NVDM_MODEM_CFG",      "PSM_DEFAULT_T3412_VALUE",     NVDM_MODEM_AREA_PROTECTED, Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(NVDM_MODEM_CFG_EDRX_SUPPORT,                                  nvdm_bool,                     "NVDM_MODEM_CFG",      "EDRX_SUPPORT",                NVDM_MODEM_AREA_PROTECTED, Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(NVDM_MODEM_CFG_EDRX_DEFAULT_PARAMS,                           nvdm_uint8,                    "NVDM_MODEM_CFG",      "EDRX_DEFAULT_PARAMS",         NVDM_MODEM_AREA_PROTECTED, Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(NVDM_MODEM_CFG_COMBINED_ATTACH_SUPPORT,                       nvdm_bool,                     "NVDM_MODEM_CFG",      "COMBINED_ATTACH_SUPPORT",     NVDM_MODEM_AREA_PROTECTED, Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(NAS_NSPS_RECOVER_SEARCH_STEP_CFG_DATA,                        nasNspsSearchCfgData,          "nas",                 "nas_nsps_search_cfg_item",    NVDM_MODEM_AREA_NORMAL,    Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(NVDM_MODEM_CFG_EPCO_SUPPORT,                                  nvdm_bool,                     "NVDM_MODEM_CFG",      "EPCO_SUPPORT",                NVDM_MODEM_AREA_PROTECTED, Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(NAS_AT_IDENT_INFO,                                            AtIdentificationInfo,          "nas",                 "at_ident_info",                 NVDM_MODEM_AREA_NORMAL,  Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(NAS_ABMM_WRIT,                                                AbmmWriteableData,             "nas",                 "abmm_writ",                     NVDM_MODEM_AREA_NORMAL,  Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(NAS_PREFERRED_INFO,                                           NasOperatorPrefInfoNvData,     "nas",                 "operator_pref_info_data_item",  NVDM_MODEM_AREA_NORMAL,  Reserved, USE_VER, 0x03)

NVDM_MODEM_ITEM_DEF(NAS_RPM_PARAMETER,                                            RpmParam,                      "nas",                 "rpm_parameter",                 NVDM_MODEM_AREA_NORMAL,  Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF_NOGEN_DESCRIPTION(TEST_OPERATOR,                              nvdm_uint8,                    "TEST",                "TEST_OPERATOR",                 NVDM_MODEM_AREA_NORMAL,  Reserved, NO_VER, 0xFF)
/* Add for deep sleep enter criterial value   */
NVDM_MODEM_ITEM_DEF(PSC_NVRAM_SLEEP_CRITERIAL_DATA1,                               nvdm_uint32,                   "PSC_SLEEP_GROUP",     "NVRAM_PSC_DATA1",               NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(PSC_NVRAM_SLEEP_CRITERIAL_DATA2,                               nvdm_uint32,                   "PSC_SLEEP_GROUP",     "NVRAM_PSC_DATA2",               NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(NAS_EPS_LOCAT,                                                UsimEpsLocInfo,                "nas",                 "eps_locat",                     NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(NVDM_MODEM_CFG_SEARCH_ALL_SUPPORT_BAND,                       nvdm_bool,                     "NVDM_MODEM_CFG",      "SEARCH_ALL_SUPPORT_BAND",       NVDM_MODEM_AREA_PROTECTED, Reserved, NO_VER, 0xFF)

NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_BAND_1_INFO,                                      NB_RFFE_CONFIG_T,              "N1RF_CUSTOM_DATA",    "BAND_1",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_BAND_2_INFO,                                      NB_RFFE_CONFIG_T,              "N1RF_CUSTOM_DATA",    "BAND_2",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_BAND_3_INFO,                                      NB_RFFE_CONFIG_T,              "N1RF_CUSTOM_DATA",    "BAND_3",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_BAND_4_INFO,                                      NB_RFFE_CONFIG_T,              "N1RF_CUSTOM_DATA",    "BAND_4",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_BAND_5_INFO,                                      NB_RFFE_CONFIG_T,              "N1RF_CUSTOM_DATA",    "BAND_5",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_BAND_8_INFO,                                      NB_RFFE_CONFIG_T,              "N1RF_CUSTOM_DATA",    "BAND_8",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_BAND_11_INFO,                                     NB_RFFE_CONFIG_T,              "N1RF_CUSTOM_DATA",    "BAND_11",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_BAND_12_INFO,                                     NB_RFFE_CONFIG_T,              "N1RF_CUSTOM_DATA",    "BAND_12",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_BAND_13_INFO,                                     NB_RFFE_CONFIG_T,              "N1RF_CUSTOM_DATA",    "BAND_13",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_BAND_14_INFO,                                     NB_RFFE_CONFIG_T,              "N1RF_CUSTOM_DATA",    "BAND_14",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_BAND_17_INFO,                                     NB_RFFE_CONFIG_T,              "N1RF_CUSTOM_DATA",    "BAND_17",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_BAND_18_INFO,                                     NB_RFFE_CONFIG_T,              "N1RF_CUSTOM_DATA",    "BAND_18",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_BAND_19_INFO,                                     NB_RFFE_CONFIG_T,              "N1RF_CUSTOM_DATA",    "BAND_19",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_BAND_20_INFO,                                     NB_RFFE_CONFIG_T,              "N1RF_CUSTOM_DATA",    "BAND_20",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_BAND_21_INFO,                                     NB_RFFE_CONFIG_T,              "N1RF_CUSTOM_DATA",    "BAND_21",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_BAND_25_INFO,                                     NB_RFFE_CONFIG_T,              "N1RF_CUSTOM_DATA",    "BAND_25",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_BAND_26_INFO,                                     NB_RFFE_CONFIG_T,              "N1RF_CUSTOM_DATA",    "BAND_26",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_BAND_28_INFO,                                     NB_RFFE_CONFIG_T,              "N1RF_CUSTOM_DATA",    "BAND_28",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_BAND_31_INFO,                                     NB_RFFE_CONFIG_T,              "N1RF_CUSTOM_DATA",    "BAND_31",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_BAND_66_INFO,                                     NB_RFFE_CONFIG_T,              "N1RF_CUSTOM_DATA",    "BAND_66",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_BAND_70_INFO,                                     NB_RFFE_CONFIG_T,              "N1RF_CUSTOM_DATA",    "BAND_70",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_BAND_71_INFO,                                     NB_RFFE_CONFIG_T,              "N1RF_CUSTOM_DATA",    "BAND_71",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_BAND_72_INFO,                                     NB_RFFE_CONFIG_T,              "N1RF_CUSTOM_DATA",    "BAND_72",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_BAND_73_INFO,                                     NB_RFFE_CONFIG_T,              "N1RF_CUSTOM_DATA",    "BAND_73",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_BAND_74_INFO,                                     NB_RFFE_CONFIG_T,              "N1RF_CUSTOM_DATA",    "BAND_74",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_BAND_85_INFO,                                     NB_RFFE_CONFIG_T,              "N1RF_CUSTOM_DATA",    "BAND_85",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)

NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_RX_EVENT_1,                                       NB_RFFE_RX_EVENT_TABLE,        "N1RF_CUSTOM_DATA",    "RX_E1",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_RX_EVENT_2,                                       NB_RFFE_RX_EVENT_TABLE,        "N1RF_CUSTOM_DATA",    "RX_E2",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_RX_EVENT_3,                                       NB_RFFE_RX_EVENT_TABLE,        "N1RF_CUSTOM_DATA",    "RX_E3",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_RX_EVENT_4,                                       NB_RFFE_RX_EVENT_TABLE,        "N1RF_CUSTOM_DATA",    "RX_E4",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_RX_DATA_1,                                        NB_RFFE_RX_DATA_TABLE,         "N1RF_CUSTOM_DATA",    "RX_D1",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_RX_DATA_2,                                        NB_RFFE_RX_DATA_TABLE,         "N1RF_CUSTOM_DATA",    "RX_D2",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_RX_DATA_3,                                        NB_RFFE_RX_DATA_TABLE,         "N1RF_CUSTOM_DATA",    "RX_D3",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_RX_DATA_4,                                        NB_RFFE_RX_DATA_TABLE,         "N1RF_CUSTOM_DATA",    "RX_D4",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)

NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_TX_EVENT_1,                                       NB_RFFE_TX_EVENT_TABLE,        "N1RF_CUSTOM_DATA",    "TX_E1",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_TX_EVENT_2,                                       NB_RFFE_TX_EVENT_TABLE,        "N1RF_CUSTOM_DATA",    "TX_E2",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_TX_EVENT_3,                                       NB_RFFE_TX_EVENT_TABLE,        "N1RF_CUSTOM_DATA",    "TX_E3",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_TX_EVENT_4,                                       NB_RFFE_TX_EVENT_TABLE,        "N1RF_CUSTOM_DATA",    "TX_E4",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_TX_EVENT_5,                                       NB_RFFE_TX_EVENT_TABLE,        "N1RF_CUSTOM_DATA",    "TX_E5",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)

NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_TX_DATA_1,                                        NB_RFFE_TX_DATA_TABLE,         "N1RF_CUSTOM_DATA",    "TX_D1",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_TX_DATA_2,                                        NB_RFFE_TX_DATA_TABLE,         "N1RF_CUSTOM_DATA",    "TX_D2",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_TX_DATA_3,                                        NB_RFFE_TX_DATA_TABLE,         "N1RF_CUSTOM_DATA",    "TX_D3",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_TX_DATA_4,                                        NB_RFFE_TX_DATA_TABLE,         "N1RF_CUSTOM_DATA",    "TX_D4",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_TX_DATA_5,                                        NB_RFFE_TX_DATA_TABLE,         "N1RF_CUSTOM_DATA",    "TX_D5",                      NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)

NVDM_MODEM_ITEM_DEF(N1RF_CUSTOM_FEM_CONFIG,                                       NB_RFFE_INFO_T,                "N1RF_CUSTOM_DATA",    "CONFIG",                     NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)

/*Provide customer PA Bias item to debug*/
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_PA_DATA_B1,                                     N1RfTxPABiasData,              "N1RF_CAL_DATA",       "PA_DATA_B1",                  NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_PA_DATA_B2,                                     N1RfTxPABiasData,              "N1RF_CAL_DATA",       "PA_DATA_B2",                  NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_PA_DATA_B3,                                     N1RfTxPABiasData,              "N1RF_CAL_DATA",       "PA_DATA_B3",                  NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_PA_DATA_B4,                                     N1RfTxPABiasData,              "N1RF_CAL_DATA",       "PA_DATA_B4",                  NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_PA_DATA_B5,                                     N1RfTxPABiasData,              "N1RF_CAL_DATA",       "PA_DATA_B5",                  NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_PA_DATA_B8,                                     N1RfTxPABiasData,              "N1RF_CAL_DATA",       "PA_DATA_B8",                  NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_PA_DATA_B11,                                    N1RfTxPABiasData,              "N1RF_CAL_DATA",       "PA_DATA_B11",                 NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_PA_DATA_B12,                                    N1RfTxPABiasData,              "N1RF_CAL_DATA",       "PA_DATA_B12",                 NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_PA_DATA_B13,                                    N1RfTxPABiasData,              "N1RF_CAL_DATA",       "PA_DATA_B13",                 NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_PA_DATA_B14,                                    N1RfTxPABiasData,              "N1RF_CAL_DATA",       "PA_DATA_B14",                 NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_PA_DATA_B17,                                    N1RfTxPABiasData,              "N1RF_CAL_DATA",       "PA_DATA_B17",                 NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_PA_DATA_B18,                                    N1RfTxPABiasData,              "N1RF_CAL_DATA",       "PA_DATA_B18",                 NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_PA_DATA_B19,                                    N1RfTxPABiasData,              "N1RF_CAL_DATA",       "PA_DATA_B19",                 NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_PA_DATA_B20,                                    N1RfTxPABiasData,              "N1RF_CAL_DATA",       "PA_DATA_B20",                 NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_PA_DATA_B21,                                    N1RfTxPABiasData,              "N1RF_CAL_DATA",       "PA_DATA_B21",                 NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_PA_DATA_B25,                                    N1RfTxPABiasData,              "N1RF_CAL_DATA",       "PA_DATA_B25",                 NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_PA_DATA_B26,                                    N1RfTxPABiasData,              "N1RF_CAL_DATA",       "PA_DATA_B26",                 NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_PA_DATA_B28,                                    N1RfTxPABiasData,              "N1RF_CAL_DATA",       "PA_DATA_B28",                 NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_PA_DATA_B31,                                    N1RfTxPABiasData,              "N1RF_CAL_DATA",       "PA_DATA_B31",                 NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_PA_DATA_B66,                                    N1RfTxPABiasData,              "N1RF_CAL_DATA",       "PA_DATA_B66",                 NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_PA_DATA_B70,                                    N1RfTxPABiasData,              "N1RF_CAL_DATA",       "PA_DATA_B70",                 NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_PA_DATA_B71,                                    N1RfTxPABiasData,              "N1RF_CAL_DATA",       "PA_DATA_B71",                 NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_PA_DATA_B72,                                    N1RfTxPABiasData,              "N1RF_CAL_DATA",       "PA_DATA_B72",                 NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_PA_DATA_B73,                                    N1RfTxPABiasData,              "N1RF_CAL_DATA",       "PA_DATA_B73",                 NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_PA_DATA_B74,                                    N1RfTxPABiasData,              "N1RF_CAL_DATA",       "PA_DATA_B74",                 NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_PA_DATA_B85,                                    N1RfTxPABiasData,              "N1RF_CAL_DATA",       "PA_DATA_B85",                 NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_MPPA_DATA_B1,                                   N1RfTxMaxPwrPABiasData,        "N1RF_CAL_DATA",       "MPPA_DATA_B1",                NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_MPPA_DATA_B2,                                   N1RfTxMaxPwrPABiasData,        "N1RF_CAL_DATA",       "MPPA_DATA_B2",                NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_MPPA_DATA_B3,                                   N1RfTxMaxPwrPABiasData,        "N1RF_CAL_DATA",       "MPPA_DATA_B3",                NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_MPPA_DATA_B4,                                   N1RfTxMaxPwrPABiasData,        "N1RF_CAL_DATA",       "MPPA_DATA_B4",                NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_MPPA_DATA_B5,                                   N1RfTxMaxPwrPABiasData,        "N1RF_CAL_DATA",       "MPPA_DATA_B5",                NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_MPPA_DATA_B8,                                   N1RfTxMaxPwrPABiasData,        "N1RF_CAL_DATA",       "MPPA_DATA_B8",                NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_MPPA_DATA_B11,                                  N1RfTxMaxPwrPABiasData,        "N1RF_CAL_DATA",       "MPPA_DATA_B11",               NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_MPPA_DATA_B12,                                  N1RfTxMaxPwrPABiasData,        "N1RF_CAL_DATA",       "MPPA_DATA_B12",               NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_MPPA_DATA_B13,                                  N1RfTxMaxPwrPABiasData,        "N1RF_CAL_DATA",       "MPPA_DATA_B13",               NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_MPPA_DATA_B14,                                  N1RfTxMaxPwrPABiasData,        "N1RF_CAL_DATA",       "MPPA_DATA_B14",               NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_MPPA_DATA_B17,                                  N1RfTxMaxPwrPABiasData,        "N1RF_CAL_DATA",       "MPPA_DATA_B17",               NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_MPPA_DATA_B18,                                  N1RfTxMaxPwrPABiasData,        "N1RF_CAL_DATA",       "MPPA_DATA_B18",               NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_MPPA_DATA_B19,                                  N1RfTxMaxPwrPABiasData,        "N1RF_CAL_DATA",       "MPPA_DATA_B19",               NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_MPPA_DATA_B20,                                  N1RfTxMaxPwrPABiasData,        "N1RF_CAL_DATA",       "MPPA_DATA_B20",               NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_MPPA_DATA_B21,                                  N1RfTxMaxPwrPABiasData,        "N1RF_CAL_DATA",       "MPPA_DATA_B21",               NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_MPPA_DATA_B25,                                  N1RfTxMaxPwrPABiasData,        "N1RF_CAL_DATA",       "MPPA_DATA_B25",               NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_MPPA_DATA_B26,                                  N1RfTxMaxPwrPABiasData,        "N1RF_CAL_DATA",       "MPPA_DATA_B26",               NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_MPPA_DATA_B28,                                  N1RfTxMaxPwrPABiasData,        "N1RF_CAL_DATA",       "MPPA_DATA_B28",               NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_MPPA_DATA_B31,                                  N1RfTxMaxPwrPABiasData,        "N1RF_CAL_DATA",       "MPPA_DATA_B31",               NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_MPPA_DATA_B66,                                  N1RfTxMaxPwrPABiasData,        "N1RF_CAL_DATA",       "MPPA_DATA_B66",               NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_MPPA_DATA_B70,                                  N1RfTxMaxPwrPABiasData,        "N1RF_CAL_DATA",       "MPPA_DATA_B70",               NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_MPPA_DATA_B71,                                  N1RfTxMaxPwrPABiasData,        "N1RF_CAL_DATA",       "MPPA_DATA_B71",               NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_MPPA_DATA_B72,                                  N1RfTxMaxPwrPABiasData,        "N1RF_CAL_DATA",       "MPPA_DATA_B72",               NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_MPPA_DATA_B73,                                  N1RfTxMaxPwrPABiasData,        "N1RF_CAL_DATA",       "MPPA_DATA_B73",               NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_MPPA_DATA_B74,                                  N1RfTxMaxPwrPABiasData,        "N1RF_CAL_DATA",       "MPPA_DATA_B74",               NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(N1RF_CAL_DATA_MPPA_DATA_B85,                                  N1RfTxMaxPwrPABiasData,        "N1RF_CAL_DATA",       "MPPA_DATA_B85",               NVDM_MODEM_AREA_NORMAL,   Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(NVDM_MODEM_CFG_RECOVERY_SEARCH_STEP_1,                        nvdm_uint16,                   "NVDM_MODEM_CFG",      "RECOVERY_SEARCH_STEP_1",      NVDM_MODEM_AREA_PROTECTED,Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(NVDM_MODEM_CFG_RECOVERY_SEARCH_STEP_2,                        nvdm_uint16,                   "NVDM_MODEM_CFG",      "RECOVERY_SEARCH_STEP_2",      NVDM_MODEM_AREA_PROTECTED,Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(NVDM_MODEM_CFG_RECOVERY_SEARCH_STEP_3,                        nvdm_uint16,                   "NVDM_MODEM_CFG",      "RECOVERY_SEARCH_STEP_3",      NVDM_MODEM_AREA_PROTECTED,Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(NVDM_MODEM_CFG_RECOVERY_SEARCH_STEP_4,                        nvdm_uint16,                   "NVDM_MODEM_CFG",      "RECOVERY_SEARCH_STEP_4",      NVDM_MODEM_AREA_PROTECTED,Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(NVDM_MODEM_CFG_RECOVERY_SEARCH_STEP_5,                        nvdm_uint16,                   "NVDM_MODEM_CFG",      "RECOVERY_SEARCH_STEP_5",      NVDM_MODEM_AREA_PROTECTED,Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(NVDM_MODEM_CFG_RECOVERY_SEARCH_STEP_6,                        nvdm_uint16,                   "NVDM_MODEM_CFG",      "RECOVERY_SEARCH_STEP_6",      NVDM_MODEM_AREA_PROTECTED,Reserved, NO_VER, 0xFF)
NVDM_MODEM_ITEM_DEF(NAS_MOB_SVN,                                                  Svn,                           "nas",                 "SVN",                           NVDM_MODEM_AREA_PROTECTED, Reserved, NO_VER, 0xFF)

