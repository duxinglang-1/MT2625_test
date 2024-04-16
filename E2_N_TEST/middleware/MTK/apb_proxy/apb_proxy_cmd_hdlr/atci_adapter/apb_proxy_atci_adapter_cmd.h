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

#ifndef __APB_PROXY_ATCI_ADAPTER_CMD__H__
#define __APB_PROXY_ATCI_ADAPTER_CMD__H__

#include "at_command.h"
#include "hal_feature_config.h"
#include "verno.h"
/*--- Function ---*/
extern atci_status_t atci_cmd_hdlr_test(atci_parse_cmd_param_t *parse_cmd);
#if defined(MTK_QUERY_SDK_VERSION) && defined(MTK_FW_VERSION)
extern atci_status_t atci_cmd_hdlr_sdkinfo(atci_parse_cmd_param_t *parse_cmd);
#endif //  defined(MTK_QUERY_SDK_VERSION) && defined(MTK_FW_VERSION)

#ifndef MTK_AT_CMD_DISABLE
/*--- Function ---*/
#if defined(__GNUC__)
#ifdef TOOL_APP_MODULE
extern atci_status_t atci_cmd_hdlr_testframework(atci_parse_cmd_param_t *parse_cmd);
#endif
#endif

extern atci_status_t atci_cmd_hdlr_syslog(atci_parse_cmd_param_t *parse_cmd);
extern atci_status_t atci_cmd_hdlr_mem(atci_parse_cmd_param_t *parse_cmd);

#if defined(DEVICE_HDK) || defined(DEVICE_BAND)

#ifdef MTK_SENSOR_BIO_USE
extern atci_status_t atci_cmd_hdlr_biogui(atci_parse_cmd_param_t *parse_cmd);
#endif

#else // defined(DEVICE_HDK) || defined(DEVICE_BAND)
extern atci_status_t atci_cmd_hdlr_camera(atci_parse_cmd_param_t *parse_cmd);

#ifdef MTK_GNSS_ENABLE
extern atci_status_t gnss_power_control_at_handler(atci_parse_cmd_param_t *parse_cmd);
extern atci_status_t gnss_send_command_at_handler(atci_parse_cmd_param_t *parse_cmd);
#endif

#ifdef HAL_DISPLAY_LCD_MODULE_ENABLED
extern	atci_status_t atci_cmd_hdlr_lcm(atci_parse_cmd_param_t *parse_cmd);
#endif

#ifdef HAL_CHARGER_MODULE_ENABLED
extern atci_status_t atci_cmd_hdlr_charger(atci_parse_cmd_param_t *parse_cmd);
#endif

#ifdef MTK_EXTERNAL_GAUGE
#ifdef MTK_BUILD_SMT_LOAD
extern atci_status_t atci_cmd_hdlr_external_gauge(atci_parse_cmd_param_t *parse_cmd);
#endif
#endif


#ifndef MTK_EXTERNAL_PMIC
extern atci_status_t atci_cmd_hdlr_pmu(atci_parse_cmd_param_t *parse_cmd);
#endif

#ifdef MTK_EXTERNAL_PMIC
extern atci_status_t atci_cmd_hdlr_external_pmic(atci_parse_cmd_param_t *parse_cmd);
#endif

#ifdef HAL_REG_MODULE_ENABLED
extern atci_status_t atci_cmd_hdlr_reg(atci_parse_cmd_param_t *parse_cmd);
#endif

#ifdef HAL_RTC_MODULE_ENABLED
extern atci_status_t atci_cmd_hdlr_rtc(atci_parse_cmd_param_t *parse_cmd);
#endif

#if defined(HAL_SD_MODULE_ENABLED)
#if defined(MTK_BUILD_SMT_LOAD) || defined(MTK_BUILD_HQA_LOAD)
extern atci_status_t atci_cmd_hdlr_msdc(atci_parse_cmd_param_t *parse_cmd);
#endif
#endif

#if defined(MTK_NVDM_ENABLE)
extern atci_status_t atci_cmd_hdlr_nvdm(atci_parse_cmd_param_t *parse_cmd);
#endif

#if defined(MTK_NVDM_MODEM_ENABLE)
extern atci_status_t atci_cmd_hdlr_nvdm_modem(atci_parse_cmd_param_t *parse_cmd);
#endif

#if defined(MTK_PORT_SERVICE_ENABLE)
extern atci_status_t atci_cmd_hdlr_serial_port(atci_parse_cmd_param_t *parse_cmd);
#endif

#if defined(HAL_GPIO_MODULE_ENABLED)
#ifdef MTK_BUILD_SMT_LOAD
extern atci_status_t atci_cmd_hdlr_led(atci_parse_cmd_param_t *parse_cmd);
#endif
extern atci_status_t atci_cmd_hdlr_gpio(atci_parse_cmd_param_t *parse_cmd);
#endif

#if defined(__GNUC__)
#ifdef HAL_AES_MODULE_ENABLED
extern atci_status_t atci_cmd_hdlr_crypto(atci_parse_cmd_param_t *parse_cmd);
#endif
#endif

#ifdef HAL_DVFS_MODULE_ENABLED
extern atci_status_t atci_cmd_hdlr_dvfs_get(atci_parse_cmd_param_t *parse_cmd);
extern atci_status_t atci_cmd_hdlr_dvfs_set(atci_parse_cmd_param_t *parse_cmd);
extern atci_status_t atci_cmd_hdlr_dvfs_dbg(atci_parse_cmd_param_t *parse_cmd);
#endif

#ifdef HAL_CLOCK_MODULE_ENABLED
extern atci_status_t atci_cmd_hdlr_clock(atci_parse_cmd_param_t *parse_cmd);
#endif

#if defined(HAL_DISPLAY_PWM_MODULE_ENABLED) || defined(HAL_ISINK_MODULE_ENABLED)
extern atci_status_t atci_cmd_hdlr_backlight(atci_parse_cmd_param_t *parse_cmd);
#endif

#if (PRODUCT_VERSION == 2523 || PRODUCT_VERSION == 2533 || PRODUCT_VERSION == 2625)
#ifdef HAL_SLEEP_MANAGER_ENABLED
extern atci_status_t atci_cmd_hdlr_sleep_manager(atci_parse_cmd_param_t *parse_cmd);
#endif
#endif

#if defined(__GNUC__)
#if defined(HAL_AUDIO_MODULE_ENABLED)
extern atci_status_t atci_cmd_hdlr_audio(atci_parse_cmd_param_t *parse_cmd);
#endif
#endif

#if defined(MTK_AUDIO_TUNING_ENABLED)
extern atci_status_t atci_cmd_hdlr_eaps(atci_parse_cmd_param_t *parse_cmd);
extern atci_status_t atci_cmd_hdlr_attdet(atci_parse_cmd_param_t *parse_cmd);
extern atci_status_t atci_cmd_hdlr_atttest(atci_parse_cmd_param_t *parse_cmd);
#endif

#if defined(MTK_BT_AT_COMMAND_ENABLE)
extern atci_status_t atci_cmd_hdlr_bt_ata(atci_parse_cmd_param_t *parse_cmd);
extern atci_status_t atci_cmd_hdlr_bt_power(atci_parse_cmd_param_t *parse_cmd);
extern atci_status_t atci_cmd_hdlr_bt_relay(atci_parse_cmd_param_t *parse_cmd);
extern atci_status_t atci_cmd_hdlr_bt_send_hci_command(atci_parse_cmd_param_t *parse_cmd);
#endif

#ifdef __BT_DEBUG__
extern atci_status_t atci_cmd_hdlr_bt_enable_driver_dump_log(atci_parse_cmd_param_t *parse_cmd);
#endif

#ifndef MTK_EXTERNAL_PMIC
extern atci_status_t atci_cmd_hdlr_vibrator(atci_parse_cmd_param_t *parse_cmd);
#endif


#ifdef MTK_CTP_ENABLE
extern atci_status_t atci_cmd_hdlr_ctp(atci_parse_cmd_param_t *parse_cmd);
extern atci_status_t atci_cmd_hdlr_ctp_simulate(atci_parse_cmd_param_t *parse_cmd);
#endif

#ifdef HAL_KEYPAD_MODULE_ENABLED
extern atci_status_t atci_cmd_hdlr_keypad(atci_parse_cmd_param_t *parse_cmd);
extern atci_status_t atci_cmd_hdlr_keypad_simulate(atci_parse_cmd_param_t *parse_cmd);
#endif

#ifdef HAL_WDT_MODULE_ENABLED
extern atci_status_t atci_cmd_hdlr_wdt(atci_parse_cmd_param_t *parse_cmd);
#endif

#ifdef MTK_BUILD_SMT_LOAD
#ifdef HAL_ADC_MODULE_ENABLED
extern atci_status_t atci_cmd_hdlr_auxadc(atci_parse_cmd_param_t *parse_cmd);
#endif
#endif

#ifdef MTK_SENSOR_AT_COMMAND_ENABLE
extern  atci_status_t atci_cmd_hdlr_sensor(atci_parse_cmd_param_t *parse_cmd);
#endif

#ifdef MTK_SENSOR_ACCELEROMETER_USE_BMA255
extern atci_status_t atci_cmd_hdlr_gsensor(atci_parse_cmd_param_t *parse_cmd);
#endif

#ifdef MTK_SENSOR_BIO_USE
extern atci_status_t atci_cmd_hdlr_biogui(atci_parse_cmd_param_t *parse_cmd);
#endif

#endif //end of DEVICE_HDK || DEVICE_BAND

#ifdef MTK_SYSTEM_AT_COMMAND_ENABLE
extern atci_status_t atci_cmd_hdlr_system(atci_parse_cmd_param_t *parse_cmd);
#endif

#ifdef MTK_OS_CPU_UTILIZATION_ENABLE
extern atci_status_t atci_cmd_hdlr_utilization(atci_parse_cmd_param_t *parse_cmd);
#endif

#if defined(MTK_FOTA_ENABLE) && defined(MTK_FOTA_AT_COMMAND_ENABLE)
extern atci_status_t atci_cmd_hdlr_fota_cmd(atci_parse_cmd_param_t *parse_cmd);
#endif

#if defined(MTK_FW_VERSION) && ((PRODUCT_VERSION == 7687) || (PRODUCT_VERSION == 7697) || (PRODUCT_VERSION == 7686) || (PRODUCT_VERSION == 7682) || (PRODUCT_VERSION == 5932))
extern atci_status_t atci_cmd_hdlr_sys(atci_parse_cmd_param_t *parse_cmd);
#endif // defined(MTK_FW_VERSION) && ((PRODUCT_VERSION == 7687) || (PRODUCT_VERSION == 7697) || (PRODUCT_VERSION == 7686) )	

extern atci_status_t atci_cmd_hdlr_usimsmt(atci_parse_cmd_param_t *parse_cmd);

#endif // MTK_AT_CMD_DISABLE


#endif /* __APB_PROXY_ATCI_ADAPTER_CMD__H__ */

