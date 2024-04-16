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

#ifndef __APB_PROXY_ATCI_ADAPTER_CMD_DEF__H__
#define __APB_PROXY_ATCI_ADAPTER_CMD_DEF__H__

#ifndef MTK_AT_CMD_DISABLE

#if defined(DEVICE_HDK) || defined(DEVICE_BAND)
#ifdef MTK_SENSOR_BIO_USE
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EBIO", atci_cmd_hdlr_biogui)
#endif

#else //else defined(DEVICE_HDK) || defined(DEVICE_BAND)

#ifdef MTK_ATCI_CAMERA_ENABLE
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ECMP", atci_cmd_hdlr_camera)
#endif

#ifdef HAL_DISPLAY_LCD_MODULE_ENABLED
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ELCM", atci_cmd_hdlr_lcm)
#endif
#ifdef MTK_GNSS_ENABLE
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EGPSC", gnss_power_control_at_handler)
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EGPSS", gnss_send_command_at_handler)
#endif

#ifdef HAL_CHARGER_MODULE_ENABLED
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ECHAR", atci_cmd_hdlr_charger)
#endif

#ifdef MTK_EXTERNAL_GAUGE
#ifdef MTK_BUILD_SMT_LOAD
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EEXTGAUGE", atci_cmd_hdlr_external_gauge)
#endif
#endif

#if 0
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EPMUREG",	atci_cmd_hdlr_pmu)
#endif

#ifdef MTK_EXTERNAL_PMIC
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EPMICREG", atci_cmd_hdlr_external_pmic)
#endif

#ifdef HAL_REG_MODULE_ENABLED
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EREG", atci_cmd_hdlr_reg)
#endif

#ifdef HAL_RTC_MODULE_ENABLED
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ERTCREG", atci_cmd_hdlr_rtc)
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ERTC32K", atci_cmd_hdlr_rtc)
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ERTC", atci_cmd_hdlr_rtc)
#endif

#if defined(HAL_SD_MODULE_ENABLED)
#if defined(MTK_BUILD_SMT_LOAD) || defined(MTK_BUILD_HQA_LOAD)
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EMSDC", atci_cmd_hdlr_msdc)
#endif
#endif

#if defined(MTK_NVDM_ENABLE)
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ENVDM", atci_cmd_hdlr_nvdm)
#endif

#if defined(MTK_NVDM_MODEM_ENABLE)
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ENVDMMD", atci_cmd_hdlr_nvdm_modem)
#endif

#if defined(MTK_PORT_SERVICE_ENABLE)
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EPORT", atci_cmd_hdlr_serial_port)
#endif

#if defined(HAL_GPIO_MODULE_ENABLED)
#ifdef MTK_BUILD_SMT_LOAD
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ELED", atci_cmd_hdlr_led)
#endif
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EGPIO", atci_cmd_hdlr_gpio)
#endif

#if defined(HAL_DISPLAY_PWM_MODULE_ENABLED) || defined(HAL_ISINK_MODULE_ENABLED)
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EBLT", atci_cmd_hdlr_backlight)
#endif
#if (PRODUCT_VERSION == 2523 || PRODUCT_VERSION == 2533 || PRODUCT_VERSION == 2625)
#ifdef HAL_SLEEP_MANAGER_ENABLED
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+SM", atci_cmd_hdlr_sleep_manager)
#endif
#endif

#if defined(__GNUC__)
#if defined(HAL_AUDIO_MODULE_ENABLED)
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EAUDIO", atci_cmd_hdlr_audio)
#endif
#endif

#if 0
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EVIB", atci_cmd_hdlr_vibrator)
#endif

#if defined(MTK_AUDIO_TUNING_ENABLED)
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EAPS", atci_cmd_hdlr_eaps)
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ATTDET", atci_cmd_hdlr_attdet)
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ATTTEST", atci_cmd_hdlr_atttest)
#endif

#ifdef MTK_CTP_ENABLE
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EPENURC", atci_cmd_hdlr_ctp)
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ECTP", atci_cmd_hdlr_ctp_simulate)
#endif

#ifdef HAL_KEYPAD_MODULE_ENABLED
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EKEYURC", atci_cmd_hdlr_keypad)
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EKP", atci_cmd_hdlr_keypad_simulate)
#endif

#ifdef MTK_SENSOR_AT_COMMAND_ENABLE
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ESENSOR", atci_cmd_hdlr_sensor)
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ESENSORTEST", atci_cmd_hdlr_sensor)
#endif

#ifdef MTK_BT_AT_COMMAND_ENABLE
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EBTAT", atci_cmd_hdlr_bt_ata)
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EBTPW", atci_cmd_hdlr_bt_power)
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EBTER", atci_cmd_hdlr_bt_relay)
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EBTSHC", atci_cmd_hdlr_bt_send_hci_command)
#endif

#ifdef __BT_DEBUG__
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EBTEDL", atci_cmd_hdlr_bt_enable_driver_dump_log)
#endif

#ifdef HAL_WDT_MODULE_ENABLED
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EWDT", atci_cmd_hdlr_wdt)
#endif

#ifdef MTK_BUILD_SMT_LOAD
#ifdef HAL_ADC_MODULE_ENABLED
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EAUXADC", atci_cmd_hdlr_auxadc)
#endif
#endif

#if defined(__GNUC__)
#ifdef HAL_AES_MODULE_ENABLED
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+ECRYPTO", atci_cmd_hdlr_crypto)
#endif
#endif

#ifdef HAL_DVFS_MODULE_ENABLED
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+CPUFGET", atci_cmd_hdlr_dvfs_get)
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+CPUFSET", atci_cmd_hdlr_dvfs_set)
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+DVFSDBG", atci_cmd_hdlr_dvfs_dbg)
#endif

#ifdef HAL_CLOCK_MODULE_ENABLED
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+CLOCK", atci_cmd_hdlr_clock)
#endif //HAL_CLOCK_MODULE_ENABLED

#if defined(__GNUC__)
#ifdef TOOL_APP_MODULE
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+TF", atci_cmd_hdlr_testframework)
#endif
#endif

#if !defined(MTK_DEBUG_LEVEL_NONE)
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+SYSLOG", atci_cmd_hdlr_syslog)
#endif

#ifdef MTK_SENSOR_ACCELEROMETER_USE_BMA255
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EGSENSOR", atci_cmd_hdlr_gsensor)
#endif

#ifdef MTK_SENSOR_BIO_USE
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EBIO", atci_cmd_hdlr_biogui)
#endif

#endif //end of defined(DEVICE_HDK) || defined(DEVICE_BAND)

#if defined(MTK_FOTA_ENABLE) && defined(MTK_FOTA_AT_COMMAND_ENABLE)
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+FOTA", atci_cmd_hdlr_fota_cmd)
#endif

#ifdef MTK_OS_CPU_UTILIZATION_ENABLE
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+UTILIZATION", atci_cmd_hdlr_utilization)
#endif
#ifdef MTK_SYSTEM_AT_COMMAND_ENABLE
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+SYSTEM", atci_cmd_hdlr_system)
#endif

    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+MEM", atci_cmd_hdlr_mem)

#if defined(MTK_FW_VERSION) && ((PRODUCT_VERSION == 7687) || (PRODUCT_VERSION == 7697) || (PRODUCT_VERSION == 7686) || (PRODUCT_VERSION == 7682) || (PRODUCT_VERSION == 5932))	
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+SYS", atci_cmd_hdlr_sys)
#endif // defined(MTK_FW_VERSION) && ((PRODUCT_VERSION == 7687) || (PRODUCT_VERSION == 7697) || (PRODUCT_VERSION == 7686) )
#endif // MTK_AT_CMD_DISABLE

#if defined(MTK_QUERY_SDK_VERSION) && defined(MTK_FW_VERSION)
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EVERINFO", atci_cmd_hdlr_sdkinfo)
#endif //  defined(MTK_QUERY_SDK_VERSION) && defined(MTK_FW_VERSION)

#if defined(MTK_USIMSMT_ENABLED) && defined(MTK_BUILD_SMT_LOAD)
    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+EUSIMSMT", atci_cmd_hdlr_usimsmt)
#endif

    AT_CMD_HDLR_ITEM_ENTRY_DEF("AT+TEST", atci_cmd_hdlr_test)

#endif/* __APB_PROXY_ATCI_ADAPTER_CMD_DEF__H__ */

