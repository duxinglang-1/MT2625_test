# Copyright Statement:
#
# (C) 2005-2016  MediaTek Inc. All rights reserved.
#
# This software/firmware and related documentation ("MediaTek Software") are
# protected under relevant copyright laws. The information contained herein
# is confidential and proprietary to MediaTek Inc. ("MediaTek") and/or its licensors.
# Without the prior written permission of MediaTek and/or its licensors,
# any reproduction, modification, use or disclosure of MediaTek Software,
# and information contained herein, in whole or in part, shall be strictly prohibited.
# You may only use, reproduce, modify, or distribute (as applicable) MediaTek Software
# if you have agreed to and been bound by the applicable license agreement with
# MediaTek ("License Agreement") and been granted explicit permission to do so within
# the License Agreement ("Permitted User").  If you are not a Permitted User,
# please cease any access or use of MediaTek Software immediately.
# BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
# THAT MEDIATEK SOFTWARE RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES
# ARE PROVIDED TO RECEIVER ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
# WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
# NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
# SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
# SUPPLIED WITH MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
# THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
# THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
# CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
# SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
# STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
# CUMULATIVE LIABILITY WITH RESPECT TO MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
# AT MEDIATEK'S OPTION, TO REVISE OR REPLACE MEDIATEK SOFTWARE AT ISSUE,
# OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
# MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
#
 
#######################################################
# Project vairables assignment

SOURCE_DIR = ../../../../..

PWD= $(shell pwd)
SDK_PATH    = $(abspath $(PWD)/$(SOURCE_DIR))
FEATURE ?= feature.mk

DEBUG = 0
FLOAT_TYPE = hard
BUILD_DIR = $(PWD)/Build

# Project name
PROJ_NAME = $(shell basename $(dir $(PWD)))
PROJ_PATH = $(abspath $(dir $(PWD)))/GCC
OUTPATH = $(PWD)/Build

ifndef FLAG_STORE_FOLDER
FLAG_STORE_FOLDER = $(OUTPATH)
endif

include $(FEATURE)

# LD Flags
LDFLAGS = $(ALLFLAGS) $(FPUFLAGS) --specs=nano.specs -lnosys -nostartfiles -u _printf_float -u _scanf_float
LDFLAGS += -Wl,-wrap=malloc -Wl,-wrap=calloc -Wl,-wrap=realloc -Wl,-wrap=free
LDFLAGS += -Wl,-wrap=memcpy -Wl,-wrap=memset -Wl,-wrap=memmove -Wl,-wrap=memcmp -Wl,-wrap=strcpy -Wl,-wrap=strcmp -Wl,-wrap=strlen -Wl,-wrap=fprintf
LDFLAGS += -Wl,-wrap=_printf_i

ifeq ($(ULS_FUNCTION_PROFILER),y)
LDFLAGS += -Wl,-Tmt2625_flash_profiler.ld -Wl,--gc-sections
else
LDFLAGS += -Wl,-Tmt2625_flash_bring_up.ld -Wl,--gc-sections
endif

ifeq ($(DEBUG_IO), 1)
LDFLAGS += --specs=rdimon.specs -lrdimon
CFLAGS += -DDEBUG_IO
endif

ifeq ($(ULS_FUNCTION_PROFILER), y)
CFLAGS += -DULS_FUNCTION_PROFILER
CFLAGS += -DULS_FUNCTION_PROFILER_EXIT
endif

#chunhui driver begin
#hrs3300
ifeq ($(MTK_HRS3300_SUPPORT), y)
CFLAGS += -DMTK_HRS3300_SUPPORT
endif

#fm78100
ifeq ($(MTK_fm78100_SUPPORT), y)
CFLAGS += -DMTK_fm78100_SUPPORT
endif
#VC32S
ifeq ($(MTK_VC32S_SUPPORT), y)
CFLAGS += -DMTK_VC32S_SUPPORT
endif

#stk8321 driver
ifeq ($(MTK_STK8321_SUPPORT), y)
CFLAGS += -DMTK_STK8321_SUPPORT
endif

#QRcode driver
ifeq ($(MTK_QRcode_SUPPORT), y)
CFLAGS += -DMTK_QRcode_SUPPORT
endif

#gps
ifeq ($(MTK_GPS_SUPPORT), y)
CFLAGS += -DMTK_GPS_SUPPORT
endif

#wifi
ifeq ($(MTK_WIFI_SUPPORT), y)
CFLAGS += -DMTK_WIFI_SUPPORT
endif

#ble
ifeq ($(MTK_BLE_SUPPORT), y)
CFLAGS += -DMTK_BLE_SUPPORT
endif

ifeq ($(MTK_H10_CFUN), y)
CFLAGS += -DMTK_H10_CFUN
endif

ifeq ($(MTK_H10_DEBUG), y)
CFLAGS += -DMTK_H10_DEBUG
endif

#FONT
ifeq ($(MTK_FONT_SUPPORT), y)
CFLAGS += -DMTK_FONT_SUPPORT
endif

#LANUAGE
ifeq ($(English_SUPPORT), y)
CFLAGS += -DEnglish_SUPPORT
endif 

ifeq ($(German_SUPPORT), y)
CFLAGS += -DGerman_SUPPORT
endif 

#MTK_DATA_SYN_SETTING_SUPPORT
ifeq ($(MTK_DATA_SYN_SETTING_SUPPORT), y)
CFLAGS += -DMTK_DATA_SYN_SETTING_SUPPORT
endif

#H10S
ifeq ($(MTK_H10S_SUPPORT), y)
CFLAGS += -DMTK_H10S_SUPPORT
endif


#logo
ifeq ($(LOGO_SUPPORT), y)
CFLAGS += -DLOGO_SUPPORT
endif 

#logo 1
ifeq ($(LOGO_SUPPORT_1), y)
CFLAGS += -DLOGO_SUPPORT_1
endif 

#logo 2
ifeq ($(LOGO_SUPPORT_2), y)
CFLAGS += -DLOGO_SUPPORT_2
endif 

#logo 3
ifeq ($(LOGO_SUPPORT_3), y)
CFLAGS += -DLOGO_SUPPORT_3
endif 

#logo 4
ifeq ($(LOGO_SUPPORT_4), y)
CFLAGS += -DLOGO_SUPPORT_4
endif 

#logo 5
ifeq ($(LOGO_SUPPORT_5), y)
CFLAGS += -DLOGO_SUPPORT_5
endif 

#logo 6
ifeq ($(LOGO_SUPPORT_6), y)
CFLAGS += -DLOGO_SUPPORT_6
endif 

#logo 7
ifeq ($(LOGO_SUPPORT_7), y)
CFLAGS += -DLOGO_SUPPORT_7
endif 



#logo 8
ifeq ($(LOGO_SUPPORT_8), y)
CFLAGS += -DLOGO_SUPPORT_8
endif



ifeq ($(MTK_MAX30102_SUPPORT), y)
CFLAGS += -DMTK_MAX30102_SUPPORT
endif

ifeq ($(MONITOR_SLEEP_SUPPORT), y)
CFLAGS += -DMONITOR_SLEEP_SUPPORT
endif

ifeq ($(MTK_SEND_QUEUE_FIFO), y)
CFLAGS += -DMTK_SEND_QUEUE_FIFO
endif
#chunhui driver end
#######################################################
# Include Module

ifneq ($(MTK_SYSTEM_CLOCK_SET), )
CFLAGS += -DMTK_SYSTEM_CLOCK_SET=$(MTK_SYSTEM_CLOCK_SET)
endif

#include $(FEATURE)
# Gloabl Config
-include $(SOURCE_DIR)/.config
# IC Config
-include $(SOURCE_DIR)/config/chip/$(IC_CONFIG)/chip.mk
# Board Config
#-include $(SOURCE_DIR)/config/board/$(BOARD_CONFIG)/board.mk

# EPT Config
-include $(SOURCE_DIR)/driver/board/mt2625_hdk/ept/module.mk

ifeq ($(MTK_USB_DEMO_ENABLED), y)
# add USB CLASS files
include $(SOURCE_DIR)/middleware/MTK/usb/module.mk
endif

#chunhui driver begin
# mems driver files
include $(SOURCE_DIR)/driver/board/component/mems/module.mk
include $(SOURCE_DIR)/driver/board/mt2625_hdk/mems_init/module.mk

#hrs3300
ifeq ($(MTK_HRS3300_SUPPORT), y)
include $(SOURCE_DIR)/driver/board/mt2625_hdk/hrs3300/module.mk
endif

#fm78100
ifeq ($(MTK_fm78100_SUPPORT), n)
include $(SOURCE_DIR)/driver/board/mt2625_hdk/fm78100/module.mk
endif
#VC32S
ifeq ($(MTK_VC32S_SUPPORT), n)
include $(SOURCE_DIR)/driver/board/mt2625_hdk/VC32S/module.mk
endif

#stk8321 driver
ifeq ($(MTK_STK8321_SUPPORT), y)
include $(SOURCE_DIR)/driver/board/mt2625_hdk/stk8321/module.mk
endif

#QRcode driver
ifeq ($(MTK_QRcode_SUPPORT), y)
include $(SOURCE_DIR)/driver/board/mt2625_hdk/QRcode/module.mk
endif

#gps
ifeq ($(MTK_GPS_SUPPORT), y)
include $(SOURCE_DIR)/driver/board/mt2625_hdk/gps/module.mk
endif

#MAX30102
ifeq ($(MTK_MAX30102_SUPPORT), y)
include $(SOURCE_DIR)/driver/board/mt2625_hdk/max30102/module.mk
endif

#gps
ifeq ($(MTK_WIFI_SUPPORT), y)
include $(SOURCE_DIR)/driver/board/mt2625_hdk/wifi/module.mk
endif

ifeq ($(MTK_H10S_SUPPORT), y)
include $(SOURCE_DIR)/driver/board/mt2625_hdk/ble/module.mk
endif

ifeq ($(MTK_FONT_SUPPORT), y)
include $(SOURCE_DIR)/driver/board/mt2625_hdk/font/module.mk
endif

#chunhui driver end

# Sources or Lib
# RTOS source files
include $(SOURCE_DIR)/kernel/rtos/FreeRTOS/module.mk

# kernel service files
include $(SOURCE_DIR)/kernel/service/module.mk

# HAL driver files
include $(SOURCE_DIR)/driver/chip/mt2625/module.mk


# Httpclient files
include $(SOURCE_DIR)/middleware/third_party/httpclient/module.mk

# lwip files
include $(SOURCE_DIR)/middleware/third_party/lwip/module.mk

# ping files
include $(SOURCE_DIR)/middleware/third_party/ping/module.mk

# iperf files
include $(SOURCE_DIR)/middleware/MTK/iperf/module.mk

#include mbedtls
include $(SOURCE_DIR)/middleware/third_party/mbedtls/module.mk

#include er-coap-13
include $(SOURCE_DIR)/middleware/third_party/er-coap-13/module.mk

# onenet files
ifeq ($(MTK_ONENET_SUPPORT), y)
#include cis_onenet
CFLAGS += -DMTK_ONENET_SUPPORT 
include $(SOURCE_DIR)/middleware/third_party/cis_onenet/module.mk

#include cis_onenet_example
include $(SOURCE_DIR)/middleware/third_party/cis_onenet/sample/module.mk
endif

# lwm2m files
ifeq ($(MTK_LWM2M_SUPPORT), y)
CFLAGS += -DMTK_LWM2M_SUPPORT
include $(SOURCE_DIR)/middleware/third_party/lwm2m/module.mk

# CT IOT platform (project level)
ifeq ($(MTK_LWM2M_CT_SUPPORT), y)
CFLAGS += -DMTK_LWM2M_CT_SUPPORT
endif

# CT IOT platform (middleware level)
ifeq ($(MTK_LWM2M_CT_SUPPORT), ctiot)
CFLAGS += -DMTK_CTIOT_SUPPORT
endif

# AEP platform
ifeq ($(MTK_CTM2M_SUPPORT), y)
CFLAGS += -DMTK_CTM2M_SUPPORT
endif

# TMO platform
ifeq ($(MTK_TMO_CERT_SUPPORT), y)
CFLAGS += -DMTK_TMO_CERT_SUPPORT
endif

# tiny dtls
ifeq ($(MTK_LWM2M_WITH_DTLS_SUPPORT), y)
CFLAGS += -DMTK_LWM2M_WITH_DTLS_SUPPORT
endif

# mbed dtls
ifeq ($(MTK_LWM2M_WITH_DTLS_SUPPORT), mbed)
CFLAGS += -DMTK_LWM2M_WITH_MBEDTLS_SUPPORT
endif

#include tinydtls
include $(SOURCE_DIR)/middleware/third_party/tinydtls/module.mk
endif #MTK_LWM2M_SUPPORT

# mqtt files
include $(SOURCE_DIR)/middleware/third_party/mqtt/module.mk

# sntp files
include $(SOURCE_DIR)/middleware/third_party/sntp/module.mk

# MUX AP files
include $(SOURCE_DIR)/middleware/MTK/mux_ap/module.mk

# KeyPad customer files
include $(SOURCE_DIR)/driver/board/mt2625_hdk/keypad/module.mk

# add ATCI files
include $(SOURCE_DIR)/middleware/MTK/atci/module.mk

# add FOTA files
include $(SOURCE_DIR)/middleware/MTK/fota/module.mk

#include lwip
include $(SOURCE_DIR)/middleware/third_party/lwip/module.mk

#include sntp
include $(SOURCE_DIR)/middleware/third_party/sntp/module.mk

# SIMAT Proxy
include $(SOURCE_DIR)/middleware/MTK/simat_proxy/module.mk

#include nidd
include $(SOURCE_DIR)/middleware/MTK/nidd/module.mk

#include ril
include $(SOURCE_DIR)/middleware/MTK/ril/module.mk

#include AP Bridge Proxy
include $(SOURCE_DIR)/middleware/MTK/apb_proxy/module.mk

#include tel_conn_mgr
include $(SOURCE_DIR)/middleware/MTK/tel_conn_mgr/module.mk

include $(SOURCE_DIR)/middleware/third_party/cjson/module.mk

ifeq ($(RS_FOTA_SUPPORT), y)
include $(SOURCE_DIR)/middleware/third_party/rsfota/rsdl/module.mk
CFLAGS += -D__RS_FOTA_SUPPORT__
endif

#include libcoap
ifeq ($(MTK_COAP_SUPPORT), y)
include $(SOURCE_DIR)/middleware/third_party/libcoap/module.mk
endif

# NVDM files
ifeq ($(MTK_NVDM_ENABLE), y)
CFLAGS += -DMTK_NVDM_ENABLE
include $(SOURCE_DIR)/middleware/MTK/nvdm/module.mk
endif

# NVDM MODEM files
ifeq ($(MTK_NVDM_MODEM_ENABLE), y)
CFLAGS += -DMTK_NVDM_MODEM_ENABLE
include $(SOURCE_DIR)/middleware/MTK/nvdm_modem/module.mk
endif

# MD files
ifeq ($(MTK_NB_MODEM_ENABLE), y)
CFLAGS += -DMTK_NB_MODEM_ENABLE
include $(SOURCE_DIR)/middleware/MTK/nbiot_custom/module.mk
ifneq ($(wildcard $(strip $(SOURCE_DIR))/middleware/MTK/nbiot/),)
include $(SOURCE_DIR)/middleware/MTK/nbiot/module.mk
else
include $(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/module.mk
endif
endif

ifeq ($(MTK_PORT_SERVICE_ENABLE), y)
include $(SOURCE_DIR)/middleware/MTK/port_service/module.mk
endif

ifeq ($(MTK_TOOL_AUTHENTICATION_ENABLE), y)
ifneq ($(wildcard $(strip $(SOURCE_DIR))/middleware/MTK/tool_authentication/src_protected/),)
include $(SOURCE_DIR)/middleware/MTK/tool_authentication/src_protected/GCC/module.mk
else
include $(SOURCE_DIR)/prebuilt/middleware/MTK/tool_authentication/module.mk
endif
endif

#display
include $(SOURCE_DIR)/driver/board/mt2625_hdk/module.mk
#gdi
include $(SOURCE_DIR)/middleware/MTK/gdi/module.mk
#public
include $(SOURCE_DIR)/driver/board/mt2625_hdk/public/module.mk
ifeq ($(MTK_SWLA_ENABLE), y)
CFLAGS += -DMTK_FW_VERSION=\"$(MTK_FW_VERSION)\"
CFLAGS += -DPRODUCT_VERSION_STR=\"$(PRODUCT_VERSION)\"
endif

ifeq ($(MTK_AUTO_BAUD_RATE_ENABLE), y)
CFLAGS += -DMTK_AUTO_BAUD_RATE_ENABLE
endif

ifeq ($(MTK_SAVE_AUTO_BAUD_RATE_ENABLE), y)
CFLAGS += -DMTK_SAVE_AUTO_BAUD_RATE_ENABLE
endif

CFLAGS += -DMTK_QUERY_SDK_VERSION

ifeq ($(USE_HAIWEI_RAWDATA_PLUGIN), y)
CFLAGS += -DUSE_HAIWEI_RAWDATA_PLUGIN
endif

ifeq ($(MD_WATCH_SPPPORT), y)
CFLAGS += -DMD_WATCH_SPPPORT
endif
OS_VERSION := $(shell uname)

#######################################################
# Main APP files

APP_PATH        = $(patsubst $(SDK_PATH)/%,%,$(abspath $(dir $(PWD))))

$(info $(SDK_PATH))
$(info $(PROJ_PATH))
$(info $(APP_PATH))

APP_PATH_SRC = $(APP_PATH)/src
APP_LWM2M_EXAMPLE_SRC = $(APP_PATH_SRC)/examples
APP_LWM2M_EXAMPLE_INC = $(SOURCE_DIR)/$(APP_LWM2M_EXAMPLE_SRC)

APP_FILES += $(APP_PATH_SRC)/main.c
APP_FILES += $(APP_PATH_SRC)/ept_gpio_var.c
APP_FILES += $(APP_PATH_SRC)/ept_eint_var.c
APP_FILES += $(APP_PATH_SRC)/sys_init.c
APP_FILES += $(APP_PATH_SRC)/system_mt2625.c
APP_FILES += $(APP_PATH_SRC)/apb_proxy_onenet_client_cmd.c
APP_FILES += $(APP_PATH_SRC)/apb_proxy_lwm2m_client_cmd.c

APP_FILES += $(APP_PATH_SRC)/h10_mmi.c
APP_FILES += $(APP_PATH_SRC)/h10_net.c
APP_FILES += $(APP_PATH_SRC)/h10_init.c
APP_FILES += $(APP_PATH_SRC)/md_init.c
APP_FILES += $(APP_PATH_SRC)/m2m_fifo.c
APP_FILES += $(APP_PATH_SRC)/m2m_timer.c
APP_FILES += $(APP_PATH_SRC)/auto_register.c
APP_FILES += $(APP_PATH_SRC)/VCHR02_SRC/module_heart_vc_int.c
APP_FILES += $(APP_PATH_SRC)/VCHR02_SRC/vcHr02DEBUG.c
APP_FILES += $(APP_PATH_SRC)/VCHR02_SRC/vcHr02Hci.c

APP_FILES += $(APP_PATH)/GCC/syscalls.c

ifeq ($(English_SUPPORT), y)
APP_FILES += $(APP_PATH_SRC)/image_data/image_info.c
APP_FILES += $(APP_PATH_SRC)/image_data/main_AudioPlayer.c
APP_FILES += $(APP_PATH_SRC)/image_data/coco.c
APP_FILES += $(APP_PATH_SRC)/image_data/0.c
APP_FILES += $(APP_PATH_SRC)/image_data/1.c
APP_FILES += $(APP_PATH_SRC)/image_data/2.c
APP_FILES += $(APP_PATH_SRC)/image_data/3.c
APP_FILES += $(APP_PATH_SRC)/image_data/4.c
APP_FILES += $(APP_PATH_SRC)/image_data/5.c
APP_FILES += $(APP_PATH_SRC)/image_data/6.c
APP_FILES += $(APP_PATH_SRC)/image_data/7.c
APP_FILES += $(APP_PATH_SRC)/image_data/8.c
APP_FILES += $(APP_PATH_SRC)/image_data/9.c
APP_FILES += $(APP_PATH_SRC)/image_data/00.c
APP_FILES += $(APP_PATH_SRC)/image_data/11.c
APP_FILES += $(APP_PATH_SRC)/image_data/22.c
APP_FILES += $(APP_PATH_SRC)/image_data/33.c
APP_FILES += $(APP_PATH_SRC)/image_data/44.c
APP_FILES += $(APP_PATH_SRC)/image_data/55.c
APP_FILES += $(APP_PATH_SRC)/image_data/66.c
APP_FILES += $(APP_PATH_SRC)/image_data/77.c
APP_FILES += $(APP_PATH_SRC)/image_data/88.c
APP_FILES += $(APP_PATH_SRC)/image_data/99.c
endif

ifeq ($(German_SUPPORT), y)
#APP_FILES += $(APP_PATH_SRC)/N1_german/blood/mmHg.c
#APP_FILES += $(APP_PATH_SRC)/N1_german/calorie/KCAL.c
#APP_FILES += $(APP_PATH_SRC)/N1_german/calorie/KM.c
#APP_FILES += $(APP_PATH_SRC)/N1_german/calorie/M.c
APP_FILES += $(APP_PATH_SRC)/N1_german/charge/Akku_geladen.c
APP_FILES += $(APP_PATH_SRC)/N1_german/charge/Bitte_Laden.c
APP_FILES += $(APP_PATH_SRC)/N1_german/charge/Wird_geladen.c
#APP_FILES += $(APP_PATH_SRC)/N1_german/hr/BPM.c
APP_FILES += $(APP_PATH_SRC)/N1_german/idle/Januar01.c
APP_FILES += $(APP_PATH_SRC)/N1_german/idle/feb02.c
APP_FILES += $(APP_PATH_SRC)/N1_german/idle/mrs03.c
APP_FILES += $(APP_PATH_SRC)/N1_german/idle/apr04.c
APP_FILES += $(APP_PATH_SRC)/N1_german/idle/mai05.c
APP_FILES += $(APP_PATH_SRC)/N1_german/idle/jun06.c
APP_FILES += $(APP_PATH_SRC)/N1_german/idle/jul07.c
APP_FILES += $(APP_PATH_SRC)/N1_german/idle/aug08.c
APP_FILES += $(APP_PATH_SRC)/N1_german/idle/sept09.c
APP_FILES += $(APP_PATH_SRC)/N1_german/idle/okt10.c
APP_FILES += $(APP_PATH_SRC)/N1_german/idle/nov11.c
APP_FILES += $(APP_PATH_SRC)/N1_german/idle/dez12.c
APP_FILES += $(APP_PATH_SRC)/N1_german/idle/mo01.c
APP_FILES += $(APP_PATH_SRC)/N1_german/idle/di02.c
APP_FILES += $(APP_PATH_SRC)/N1_german/idle/mi03.c
APP_FILES += $(APP_PATH_SRC)/N1_german/idle/do04.c
APP_FILES += $(APP_PATH_SRC)/N1_german/idle/fr05.c
APP_FILES += $(APP_PATH_SRC)/N1_german/idle/sa06.c
APP_FILES += $(APP_PATH_SRC)/N1_german/idle/so07.c
APP_FILES += $(APP_PATH_SRC)/N1_german/location/location_nofound.c
APP_FILES += $(APP_PATH_SRC)/N1_german/location/locationge.c

APP_FILES += $(APP_PATH_SRC)/N1_german/poweron/GE_Poweroff_logo.c
APP_FILES += $(APP_PATH_SRC)/N1_german/poweron/gepower_on_logo.c
#APP_FILES += $(APP_PATH_SRC)/N1_german/sedentary/Sedentaryreminder.c
APP_FILES += $(APP_PATH_SRC)/N1_german/set/Auto_Bildschirm_aufwachen.c
APP_FILES += $(APP_PATH_SRC)/N1_german/set/Languagegr.c
APP_FILES += $(APP_PATH_SRC)/N1_german/set/Languagewr.c
APP_FILES += $(APP_PATH_SRC)/N1_german/set/Sprachegr.c
APP_FILES += $(APP_PATH_SRC)/N1_german/set/Sprachewr.c
APP_FILES += $(APP_PATH_SRC)/N1_german/set/DEGR.c
APP_FILES += $(APP_PATH_SRC)/N1_german/set/DEWR.c
APP_FILES += $(APP_PATH_SRC)/N1_german/set/ENGE.c
APP_FILES += $(APP_PATH_SRC)/N1_german/set/ENWR.c

APP_FILES += $(APP_PATH_SRC)/N1_german/set/SyncEinstellungeng.c
APP_FILES += $(APP_PATH_SRC)/N1_german/set/SyncEinstellungnw.c


#APP_FILES += $(APP_PATH_SRC)/N1_german/set/Bewegungs_erinnerung.c
#APP_FILES += $(APP_PATH_SRC)/N1_german/set/Bewegungs_erinnerungblue.c
APP_FILES += $(APP_PATH_SRC)/N1_german/set/s_Auto_Bildschim_aufwachen.c
#APP_FILES += $(APP_PATH_SRC)/N1_german/set/Stromspar_modusg.c
#APP_FILES += $(APP_PATH_SRC)/N1_german/set/Sync_Einstellungen.c
#APP_FILES += $(APP_PATH_SRC)/N1_german/set/SyncEinstellungen.c
APP_FILES += $(APP_PATH_SRC)/N1_german/set/Zurucksetze.c
APP_FILES += $(APP_PATH_SRC)/N1_german/set/Zurucksetzenblue.c
APP_FILES += $(APP_PATH_SRC)/N1_german/sleep/GESLEEP_MIN.c
APP_FILES += $(APP_PATH_SRC)/N1_german/sleep/GESLEEP_Std.c
APP_FILES += $(APP_PATH_SRC)/N1_german/step/STEPS.c
APP_FILES += $(APP_PATH_SRC)/N1_german/ver/N1_FW_Version.c
APP_FILES += $(APP_PATH_SRC)/N1_german/ver/IMEI_No.c



endif

#APP_FILES += $(APP_PATH_SRC)/image_data/w1.c
#APP_FILES += $(APP_PATH_SRC)/image_data/w2.c
#APP_FILES += $(APP_PATH_SRC)/image_data/w3.c
#APP_FILES += $(APP_PATH_SRC)/image_data/w4.c
#APP_FILES += $(APP_PATH_SRC)/image_data/w5.c
#APP_FILES += $(APP_PATH_SRC)/image_data/w6.c
#APP_FILES += $(APP_PATH_SRC)/image_data/w7.c
APP_FILES += $(APP_PATH_SRC)/image_data/sos.c
APP_FILES += $(APP_PATH_SRC)/image_data/s0.c
APP_FILES += $(APP_PATH_SRC)/image_data/s1.c
APP_FILES += $(APP_PATH_SRC)/image_data/s2.c
APP_FILES += $(APP_PATH_SRC)/image_data/s3.c
APP_FILES += $(APP_PATH_SRC)/image_data/s4.c
APP_FILES += $(APP_PATH_SRC)/image_data/s5.c
APP_FILES += $(APP_PATH_SRC)/image_data/s6.c
APP_FILES += $(APP_PATH_SRC)/image_data/s7.c
APP_FILES += $(APP_PATH_SRC)/image_data/s8.c
APP_FILES += $(APP_PATH_SRC)/image_data/s9.c
APP_FILES += $(APP_PATH_SRC)/image_data/x0.c
APP_FILES += $(APP_PATH_SRC)/image_data/x1.c
APP_FILES += $(APP_PATH_SRC)/image_data/x2.c
APP_FILES += $(APP_PATH_SRC)/image_data/x3.c
APP_FILES += $(APP_PATH_SRC)/image_data/x4.c
APP_FILES += $(APP_PATH_SRC)/image_data/x5.c
APP_FILES += $(APP_PATH_SRC)/image_data/x6.c
APP_FILES += $(APP_PATH_SRC)/image_data/x7.c
APP_FILES += $(APP_PATH_SRC)/image_data/x8.c
APP_FILES += $(APP_PATH_SRC)/image_data/x9.c
APP_FILES += $(APP_PATH_SRC)/image_data/cdian.c
APP_FILES += $(APP_PATH_SRC)/image_data/hartdw.c
APP_FILES += $(APP_PATH_SRC)/image_data/hartls.c
APP_FILES += $(APP_PATH_SRC)/image_data/c0.c
APP_FILES += $(APP_PATH_SRC)/image_data/c1.c
APP_FILES += $(APP_PATH_SRC)/image_data/c2.c
APP_FILES += $(APP_PATH_SRC)/image_data/c3.c
APP_FILES += $(APP_PATH_SRC)/image_data/c4.c
APP_FILES += $(APP_PATH_SRC)/image_data/c5.c
APP_FILES += $(APP_PATH_SRC)/image_data/c6.c
APP_FILES += $(APP_PATH_SRC)/image_data/c7.c
APP_FILES += $(APP_PATH_SRC)/image_data/c8.c
APP_FILES += $(APP_PATH_SRC)/image_data/c9.c
APP_FILES += $(APP_PATH_SRC)/image_data/stepdw.c
APP_FILES += $(APP_PATH_SRC)/image_data/stepls.c
APP_FILES += $(APP_PATH_SRC)/image_data/blh.c
APP_FILES += $(APP_PATH_SRC)/image_data/bll.c
APP_FILES += $(APP_PATH_SRC)/image_data/poweroff.c
APP_FILES += $(APP_PATH_SRC)/image_data/bldw.c
APP_FILES += $(APP_PATH_SRC)/image_data/bldwt.c
APP_FILES += $(APP_PATH_SRC)/image_data/gpsopen.c
APP_FILES += $(APP_PATH_SRC)/image_data/gpssuccess.c
APP_FILES += $(APP_PATH_SRC)/image_data/imeivs.c
APP_FILES += $(APP_PATH_SRC)/image_data/title.c
APP_FILES += $(APP_PATH_SRC)/image_data/tips.c
APP_FILES += $(APP_PATH_SRC)/image_data/week.c
APP_FILES += $(APP_PATH_SRC)/image_data/Power_off_bg.c

APP_FILES += $(APP_PATH_SRC)/image_data/hart.c
ifneq ($(MTK_FONT_SUPPORT), y)
APP_FILES += $(APP_PATH_SRC)/image_data/message.c
APP_FILES += $(APP_PATH_SRC)/image_data/weather.c
endif

APP_FILES += $(APP_PATH_SRC)/image_data/dians.c
APP_FILES += $(APP_PATH_SRC)/image_data/dian.c
#APP_FILES += $(APP_PATH_SRC)/image_data/Sedentary_remind.c
#APP_FILES += $(APP_PATH_SRC)/image_data/Sedentary_reminder_gg.c
APP_FILES += $(APP_PATH_SRC)/image_data/Low_battery.c

APP_FILES += $(APP_PATH_SRC)/image_data/calorie.c
APP_FILES += $(APP_PATH_SRC)/image_data/distance.c
APP_FILES += $(APP_PATH_SRC)/image_data/km_cn.c
APP_FILES += $(APP_PATH_SRC)/image_data/m_cn.c
APP_FILES += $(APP_PATH_SRC)/image_data/kcalorie_cn.c
APP_FILES += $(APP_PATH_SRC)/image_data/hour_cn.c
APP_FILES += $(APP_PATH_SRC)/image_data/min_cn.c

APP_FILES += $(APP_PATH_SRC)/image_data/num_0.c
APP_FILES += $(APP_PATH_SRC)/image_data/num_1.c
APP_FILES += $(APP_PATH_SRC)/image_data/num_2.c
APP_FILES += $(APP_PATH_SRC)/image_data/num_3.c
APP_FILES += $(APP_PATH_SRC)/image_data/num_4.c
APP_FILES += $(APP_PATH_SRC)/image_data/num_5.c
APP_FILES += $(APP_PATH_SRC)/image_data/num_6.c
APP_FILES += $(APP_PATH_SRC)/image_data/num_7.c
APP_FILES += $(APP_PATH_SRC)/image_data/num_8.c
APP_FILES += $(APP_PATH_SRC)/image_data/num_9.c
APP_FILES += $(APP_PATH_SRC)/image_data/step0.c
APP_FILES += $(APP_PATH_SRC)/image_data/step1.c
APP_FILES += $(APP_PATH_SRC)/image_data/step2.c
APP_FILES += $(APP_PATH_SRC)/image_data/step3.c
APP_FILES += $(APP_PATH_SRC)/image_data/step4.c
APP_FILES += $(APP_PATH_SRC)/image_data/step5.c
APP_FILES += $(APP_PATH_SRC)/image_data/step6.c
APP_FILES += $(APP_PATH_SRC)/image_data/step7.c
APP_FILES += $(APP_PATH_SRC)/image_data/step8.c
APP_FILES += $(APP_PATH_SRC)/image_data/step9.c
APP_FILES += $(APP_PATH_SRC)/image_data/dian_1.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/gps_bg_fall.c

APP_FILES += $(APP_PATH_SRC)/XiTuo_data/point.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/time0.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/time1.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/time2.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/time3.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/time4.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/time5.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/time6.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/time7.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/time8.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/time9.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/Low_charge.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/jan_01.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/feb_02.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/mar_03.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/apr_04.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/may_05.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/jun_06.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/jul_07.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/aug_08.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/sep_09.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/oct_10.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/nov_11.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/dec_12.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/signal_0.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/signal_1.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/signal_2.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/signal_3.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/signal_4.c

APP_FILES += $(APP_PATH_SRC)/XiTuo_data/hr_x_0.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/hr_x_1.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/hr_x_2.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/hr_x_3.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/hr_x_4.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/hr_x_5.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/hr_x_6.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/hr_x_7.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/hr_x_8.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/hr_x_9.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/hr_x_xie.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/hr_x_dian.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/model_line.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/bt.c
   


#APP_FILES += $(APP_PATH_SRC)/XiTuo_data/weathericon0.c
#APP_FILES += $(APP_PATH_SRC)/XiTuo_data/weathericon1.c
#APP_FILES += $(APP_PATH_SRC)/XiTuo_data/weathericon2.c
#APP_FILES += $(APP_PATH_SRC)/XiTuo_data/weathericon3.c
#APP_FILES += $(APP_PATH_SRC)/XiTuo_data/weathericon4.c
#APP_FILES += $(APP_PATH_SRC)/XiTuo_data/weathericon6.c
#APP_FILES += $(APP_PATH_SRC)/XiTuo_data/weathericon6.c
#APP_FILES += $(APP_PATH_SRC)/XiTuo_data/weathericon7.c
#APP_FILES += $(APP_PATH_SRC)/XiTuo_data/weathericon8.c
#APP_FILES += $(APP_PATH_SRC)/XiTuo_data/weathericon9.c
#APP_FILES += $(APP_PATH_SRC)/XiTuo_data/weathericon10.c
#APP_FILES += $(APP_PATH_SRC)/XiTuo_data/weathericon11.c
#APP_FILES += $(APP_PATH_SRC)/XiTuo_data/weathericon12.c
#APP_FILES += $(APP_PATH_SRC)/XiTuo_data/weathericon13.c
#APP_FILES += $(APP_PATH_SRC)/XiTuo_data/weathericon14.c
#APP_FILES += $(APP_PATH_SRC)/XiTuo_data/weathericon15.c
#APP_FILES += $(APP_PATH_SRC)/XiTuo_data/weathericon16.c
#APP_FILES += $(APP_PATH_SRC)/XiTuo_data/weathericon17.c
#APP_FILES += $(APP_PATH_SRC)/XiTuo_data/weathericon18.c
#APP_FILES += $(APP_PATH_SRC)/XiTuo_data/weathericon20.c
#APP_FILES += $(APP_PATH_SRC)/XiTuo_data/weathericon21.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/network_strong.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/network_weak.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/slash.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/temperature.c

APP_FILES += $(APP_PATH_SRC)/XiTuo_data/week0.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/week00.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/week1.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/week2.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/week3.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/week4.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/week5.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/week6.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/week7.c

APP_FILES += $(APP_PATH_SRC)/XiTuo_data/battery1.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/battery2.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/battery3.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/battery4.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/battery5.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/battery6.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/battery7.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/battery8.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/battery9.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/battery10.c


APP_FILES += $(APP_PATH_SRC)/XiTuo_data/Step.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/StepW.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/Step_bg.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/complete.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/charging.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/hr_bg.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/bp_bg.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/bp_font.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/hr.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/bp.c

APP_FILES += $(APP_PATH_SRC)/XiTuo_data/red_point.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/blue_point.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/yellow_point.c

APP_FILES += $(APP_PATH_SRC)/XiTuo_data/Bright_screen.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/Bright_screen_g.c
#APP_FILES += $(APP_PATH_SRC)/XiTuo_data/Power_saving.c
#APP_FILES += $(APP_PATH_SRC)/XiTuo_data/Power_saving_g.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/off.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/on.c
#APP_FILES += $(APP_PATH_SRC)/XiTuo_data/sedentary.c
#APP_FILES += $(APP_PATH_SRC)/XiTuo_data/sedentary_g.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/gps_bg.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/gps_flag1.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/gps_flag2.c
ifeq ($(MTK_DATA_SYN_SETTING_SUPPORT), y)
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/data_syn.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/data_syn_g.c
endif

APP_FILES += $(APP_PATH_SRC)/XiTuo_data/gps_font.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/gps_font2.c
#APP_FILES += $(APP_PATH_SRC)/XiTuo_data/theme.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/step_flag.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/location_flag.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/Flight.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/activity_time.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/medicine.c
#APP_FILES += $(APP_PATH_SRC)/XiTuo_data/curr_medicine.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/medicine_hour.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/medicine_hour_only.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/medicine_min.c
#APP_FILES += $(APP_PATH_SRC)/XiTuo_data/medicine_next.c
#APP_FILES += $(APP_PATH_SRC)/XiTuo_data/sedentary_time.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/sleep_time.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/movement.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/movement_g.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/median.c

APP_FILES += $(APP_PATH_SRC)/XiTuo_data/Factory_Reset.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/Factory_Reset_g.c

#同步界面
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/syn_fail.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/syn_ing.c
APP_FILES += $(APP_PATH_SRC)/XiTuo_data/syn_succe.c



C_FILES += $(APP_FILES)
C_FILES += $(APP_PATH_SRC)/app_state_machine.c
C_FILES += $(APP_PATH_SRC)/util.c

#中性logo
#ifeq ($(LOGO_SUPPORT), y)
#APP_FILES += $(APP_PATH_SRC)/image_data/animation.c
#endif

#天翼乐寻logo
ifeq ($(LOGO_SUPPORT_1), y)
APP_FILES += $(APP_PATH_SRC)/image_data/animation1.c
endif

#统捷logo
ifeq ($(LOGO_SUPPORT_2), y)
APP_FILES += $(APP_PATH_SRC)/image_data/animation2.c
endif

#中金育能logo
ifeq ($(LOGO_SUPPORT_3), y)
APP_FILES += $(APP_PATH_SRC)/image_data/animation3.c
endif

#中国联通logo
ifeq ($(LOGO_SUPPORT_4), y)
APP_FILES += $(APP_PATH_SRC)/image_data/animation4.c
endif

#中国电信logo
ifeq ($(LOGO_SUPPORT_5), y)
APP_FILES += $(APP_PATH_SRC)/image_data/animation5.c
endif

#奥科斯logo
ifeq ($(LOGO_SUPPORT_6), y)
APP_FILES += $(APP_PATH_SRC)/image_data/animation6.c
endif

#??????logo
ifeq ($(LOGO_SUPPORT_7), y)
APP_FILES += $(APP_PATH_SRC)/image_data/animation.c
endif

#?? logo
ifeq ($(LOGO_SUPPORT_8), y)
APP_FILES += $(APP_PATH_SRC)/image_data/animation8.c
endif

###########################################################


###########################################################
#lwm2m client source code files

CFLAGS += -I$(APP_LWM2M_EXAMPLE_INC)
CFLAGS += -I$(APP_LWM2M_EXAMPLE_INC)/bootstrap_server
CFLAGS += -I$(APP_LWM2M_EXAMPLE_INC)/client
CFLAGS += -I$(APP_LWM2M_EXAMPLE_INC)/server
CFLAGS += -I$(APP_LWM2M_EXAMPLE_INC)/shared

C_FILES  += ${APP_LWM2M_EXAMPLE_SRC}/bootstrap_server/bootstrap_server.c     \
                  ${APP_LWM2M_EXAMPLE_SRC}/bootstrap_server/bootstrap_info.c       \
                  ${APP_LWM2M_EXAMPLE_SRC}/client/lwm2mclient.c                    \
                  ${APP_LWM2M_EXAMPLE_SRC}/client/system_api.c

C_FILES  += ${APP_LWM2M_EXAMPLE_SRC}/shared/commandline.c                    \
            ${APP_LWM2M_EXAMPLE_SRC}/lwm2m_cli.c

C_FILES  += ${APP_LWM2M_EXAMPLE_SRC}/server/lwm2mserver.c

#######################################################
# add startup file to build
S_FILES += $(APP_PATH)/GCC/startup_mt2625.s
S_FILES += $(APP_PATH)/GCC/startup_md.s

C_OBJS = $(C_FILES:%.c=$(BUILD_DIR)/%.o)
CXX_OBJS        = $(CXX_FILES:%.cpp=$(BUILD_DIR)/%.o)
S_OBJS = $(S_FILES:%.s=$(BUILD_DIR)/%.o)

#######################################################
# Include path
CFLAGS += -I$(SOURCE_DIR)/$(APP_PATH)/src/image_data
CFLAGS += -I$(SOURCE_DIR)/$(APP_PATH)/inc
CFLAGS += -I$(SOURCE_DIR)/$(APP_PATH)/psram/inc
CFLAGS += -I$(SOURCE_DIR)/driver/CMSIS/Include
CFLAGS += -I$(SOURCE_DIR)/driver/CMSIS/Device/MTK/mt2625/Include
CFLAGS += -I$(SOURCE_DIR)/driver/chip/mt2625/inc
CFLAGS += -I$(SOURCE_DIR)/driver/chip/inc
CFLAGS += -I$(SOURCE_DIR)/kernel/service/inc
CFLAGS += -I$(SOURCE_DIR)/driver/board/mt2625_hdk/lcd
CFLAGS += -I$(SOURCE_DIR)/driver/board/mt2625_hdk/backlight
#CFLAGS += -I$(SOURCE_DIR)/driver/board/mt25x3_hdk/ept/inc
CFLAGS += -I$(SOURCE_DIR)/$(APP_PATH)/src/XiTuo_data


#######################################################
# Rules

.PHONY: proj clean $(MODULE_PATH)

all: cleanlog proj nvdm_database
	@mkdir -p $(BUILD_DIR)
	@$(SIZE) $(OUTPATH)/$(PROJ_NAME).elf
	@$(SOURCE_DIR)/tools/scripts/build/copy_firmware.sh $(SOURCE_DIR) $(OUTPATH) $(IC_CONFIG) $(BOARD_CONFIG) $(PROJ_NAME).bin $(PWD) $(MTK_SECURE_BOOT_ENABLE) $(MTK_ANTI_CLONE_ENABLE)

ifeq ($(MTK_SECURE_BOOT_ENABLE),y)
	@$(SOURCE_DIR)/tools/scripts/build/build_sboot_header.sh $(SOURCE_DIR) $(OUTPATH) $(BOARD_CONFIG) $(PROJ_NAME).bin
endif

MOD_EXTRA = BUILD_DIR=$(BUILD_DIR) OUTPATH=$(OUTPATH) PROJ_PATH=$(PROJ_PATH)

$(LIBS): $(MODULE_PATH)

$(MODULE_PATH):
	@+make -C $@ $(MOD_EXTRA) $($@_EXTRA)

ifneq ($(wildcard $(strip $(SOURCE_DIR))/middleware/MTK/nbiot/),)
proj: $(OUTPATH)/$(PROJ_NAME).elf
else
proj: $(OUTPATH)/$(PROJ_NAME).elf prebuilt_dec_db
endif
silox_ext_db_gen:
	@echo "[customized silo db] # Copy Customized Silo database from $(SOURCE_DIR)/tools/scripts/silox/silo_db_ext.sox to $(OUTPATH)"
	@cp -f $(SOURCE_DIR)/tools/scripts/silox/silo_db_ext.sox $(OUTPATH) ;\
	if [ "$$?" != "0" ]; then \
		echo "[customized silo db] # Error: Failed to copy Customized Silo database file at $@" | tee -a $(ERR_LOG); exit 1; \
	fi
	@echo "[customized silo db] # Silo Customized database copied."	
	@echo "[customized silo db] # Update Customized Silo database with L4/ATCI src..."
ifneq ($(filter MINGW%,$(OS_VERSION)),)
	$(SOURCE_DIR)/tools/scripts/silox/Silox.exe -v -sox=$(OUTPATH)/silo_db_ext.sox -l "$(SOURCE_DIR)/tools/scripts/silox/Customized_SiloDB_srclist.txt" -lm -ig -modeasmodem -sm "$(OUTPATH)/siloMessage_ext.log" -r >>$(ERR_LOG) ; \
	if [ "$$?" != "0" ]; then \
		echo "[customized silo db] # Error: Failed to Update Customized Silo database with L4/ATCI src at $@" | tee -a $(ERR_LOG); exit 1; \
	fi
else
	$(WINE) $(SOURCE_DIR)/tools/scripts/silox/Silox.exe -v -sox=$(OUTPATH)/silo_db_ext.sox -l "$(SOURCE_DIR)/tools/scripts/silox/Customized_SiloDB_srclist.txt" -lm -ig -modeasmodem -sm "$(OUTPATH)/siloMessage_ext.log" -r -pwd "$(PWD)" >>$(ERR_LOG) ; \
	if [ "$$?" != "0" ]; then \
		echo "[customized silo db] # Error: Failed to Update Customized Silo database with L4/ATCI src at $@" | tee -a $(ERR_LOG); exit 1; \
	fi
endif
	@echo "[customized silo db] # Customized Silo database is updated."
	@echo "[customized silo db] # Remove the backup file of Customized Silo database..."
	@rm -f $(OUTPATH)/silo_db_ext.sox.backup ;\
	if [ "$$?" != "0" ]; then \
		echo "[customized silo db] # Error: Failed to removed the backup file of Customized Silo database at $@" | tee -a $(ERR_LOG); exit 1; \
	fi
	@echo "[customized silo db] # The backup file of Customized Silo database is removed."	

proj: nvdm_database

proj: cust_silo_db

$(OUTPATH)/$(PROJ_NAME).elf: $(C_OBJS) $(CXX_OBJS) $(S_OBJS) $(LIBS)
	@echo "export LDFLAGS"
	@echo $(LDFLAGS) > $(FLAG_STORE_FOLDER)/LDFLAGS.config
	@echo "export CFLAGS"
	@echo $(CFLAGS) > $(FLAG_STORE_FOLDER)/CFLAGS.config
	@echo "export end"
	@echo Linking...
	@if [ -e "$@" ]; then rm -f "$@"; fi
	@if [ -e "$(OUTPATH)/$(PROJ_NAME).map" ]; then rm -f "$(OUTPATH)/$(PROJ_NAME).map"; fi
	@if [ -e "$(OUTPATH)/$(PROJ_NAME).dis" ]; then rm -f "$(OUTPATH)/$(PROJ_NAME).dis"; fi
	@if [ -e "$(OUTPATH)/$(PROJ_NAME).sym" ]; then rm -f "$(OUTPATH)/$(PROJ_NAME).sym"; fi
	@if [ -e "$(OUTPATH)/$(PROJ_NAME).hex" ]; then rm -f "$(OUTPATH)/$(PROJ_NAME).hex"; fi
	@if [ -e "$(OUTPATH)/$(PROJ_NAME).bin" ]; then rm -f "$(OUTPATH)/$(PROJ_NAME).bin"; fi
	@echo $(patsubst $(SDK_PATH)/%,$(SOURCE_DIR)/%, $(LDFLAGS) -Wl,--whole-archive -Wl,--start-group $^ -Wl,--end-group -Wl,--no-whole-archive -Wl,-Map=$(patsubst $(SDK_PATH)/%,$(SOURCE_DIR)/%,$(OUTPATH)/$(PROJ_NAME).map) -lm -o $@) > $(OUTPATH)/link_option.tmp
	@$(CC) @$(OUTPATH)/link_option.tmp 2>>$(ERR_LOG)
	@rm -f $(OUTPATH)/link_option.tmp
ifeq ($(WITH_DIS), y)
	@$(OBJDUMP) -D $(OUTPATH)/$(PROJ_NAME).elf > $(OUTPATH)/$(PROJ_NAME).dis
endif
	@$(OBJCOPY) -O ihex $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).hex
	@$(OBJDUMP) -x $(OUTPATH)/$(PROJ_NAME).elf > $(OUTPATH)/$(PROJ_NAME).sym
	@$(OBJCOPY) -O binary $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).bin
	@echo Done

prebuilt_dec_db:
	@echo "restore prebuilt database..."
ifeq ($(MTK_AGNSS_SUPL2_ENABLE), y)
	@cp -af $(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/db/feature_supl2/*.dec ${OUTPATH}/
else
	@cp -af $(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/db/default/*.dec ${OUTPATH}/
endif
	@echo "restore done."

include $(SOURCE_DIR)/.rule.mk

clean:
	rm -rf $(OUTPATH)

NB_VERSION := "NB_VERSION"
#use command argv if exist
CUSTOMER_ID?=MTK_INTERNAL

nvdm_database: $(OUTPATH)/$(PROJ_NAME).elf
ifeq ($(MTK_NVDM_AUTOGEN_DATABASE), y)
	@$(SOURCE_DIR)/tools/scripts/nvdm_database_gen/gen_nvdm_db.sh $(CC) $(FLAG_STORE_FOLDER)/CFLAGS.config $(OUTPATH) $(SOURCE_DIR)/tools/scripts/nvdm_database_gen $(SOURCE_DIR)/middleware/MTK/nbiot/nvdm_internal "nvdm_database.EDB" $(NB_VERSION) $(CUSTOMER_ID)
endif

cust_silo_db: $(OUTPATH)/$(PROJ_NAME).elf
	@echo "[customized flox db] # check if the customized flox database file exists..."
# Delete the Existed Customized flox database
	@echo "## Delete the Existed Customized flox database ##"
	@if [ -f $(OUTPATH)/floggerBaseTransform_ext.flox ]; then \
		echo "[customized flox db] # Delete the Existed Customized flox database from $(OUTPATH)/floggerBaseTransform_ext.flox "; \
		rm -f $(OUTPATH)/floggerBaseTransform_ext.flox; \
		if [ "$$?" != "0" ]; then \
			echo "[customized flox db] # Error: Failed to delete the existed Customized Flox database file at $@" | tee -a $(ERR_LOG); exit 1; \
		fi; \
		echo "[customized flox db] # The Existed Customized Flox database deleted."; \
	fi
# Copy Customized flox database
	@echo "## Copy Customized flox database ##"
	@if [ -f $(OUTPATH)/floggerBaseTransform.flox ]; then \
		echo "[customized flox db] # Copy Customized flox database from $(OUTPATH)/floggerBaseTransform.flox to $(OUTPATH)"; \
		cp -f $(OUTPATH)/floggerBaseTransform.flox $(OUTPATH)/floggerBaseTransform_ext.flox; \
		if [ "$$?" != "0" ]; then \
			echo "[customized flox db] # Error: Failed to copy Customized Flox database file at $@" | tee -a $(ERR_LOG); exit 1; \
		fi; \
		echo "[customized flox db] # Customized Flox database copied."; \
	elif [ -f $(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/db/floggerBaseTransform.flox ]; then \
		echo "[customized flox db] # Copy Customized flox database from $(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/db/floggerBaseTransform.flox to $(OUTPATH)"; \
		cp -f $(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/db/floggerBaseTransform.flox $(OUTPATH)/floggerBaseTransform_ext.flox; \
		if [ "$$?" != "0" ]; then \
			echo "[customized flox db] # Error: Failed to copy Customized Flox database file at $@" | tee -a $(ERR_LOG); exit 1; \
		fi; \
		echo "[customized flox db] # Customized Flox database copied."; \
	else \
		echo "Customized Flox database does not exist in $(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/db/."; \
	fi
# [cust_silo_db] completed
	@echo "## cust_dilo_db has completed! ##"
