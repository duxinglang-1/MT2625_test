# Copyright Statement:
#
# (C) 2005-2017  MediaTek Inc. All rights reserved.
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

SOURCE_DIR = ../../..
BINPATH = ~/gcc-arm-none-eabi/bin
ifeq ($(PROJ_NAME),nbiot_m2m_demo)
PROJ_PATH = ../../../project/mt2625_evb/apps/nbiot_m2m_demo/GCC
else ifeq ($(PROJ_NAME),nbiot_meter_demo)
PROJ_PATH = ../../../project/mt2625_evb/apps/nbiot_meter_demo/GCC
else ifeq ($(PROJ_NAME),nbiot_sdk)
PROJ_PATH = ../../../project/mt2625_bc26/apps/nbiot_sdk/GCC
else
PROJ_PATH = ../../../project/mt2625_evb/apps/nbiot_m2m_demo/GCC
endif

CONFIG_PATH ?= .

CFLAGS += -I$(PROJ_PATH)/../inc
CFLAGS += -I$(SOURCE_DIR)/$(CONFIG_PATH)

FEATURE    ?= feature.mk

include $(PROJ_PATH)/$(FEATURE)

# Gloabl Config
-include $(SOURCE_DIR)/.config
# IC Config
-include $(SOURCE_DIR)/config/chip/$(IC_CONFIG)/chip.mk
# Board Config
#-include $(SOURCE_DIR)/config/board/$(BOARD_CONFIG)/board.mk

CFLAGS      += -I$(SOURCE_DIR)/kernel/rtos/FreeRTOS/Source/include
CFLAGS      += -I$(SOURCE_DIR)/kernel/service/inc
CFLAGS      += -I$(SOURCE_DIR)/kernel/rtos/FreeRTOS/Source/portable/GCC/$(IC_CONFIG)/ARM_CM4F
CFLAGS      += -I$(SOURCE_DIR)/driver/chip/$(IC_CONFIG)/inc
CFLAGS      += -I$(SOURCE_DIR)/driver/chip/inc
CFLAGS      += -I$(SOURCE_DIR)/driver/CMSIS/Device/MTK/$(IC_CONFIG)/Include
CFLAGS      += -I$(SOURCE_DIR)/driver/CMSIS/Include

ifeq ($(MTK_PORT_SERVICE_ENABLE), y)
CFLAGS      += -I$(SOURCE_DIR)/middleware/MTK/port_service/inc
endif
ifeq ($(MTK_NVDM_MODEM_ENABLE), y)
CFLAGS      += -I$(SOURCE_DIR)/middleware/MTK/nvdm_modem/inc
endif
# Project name
TARGET_LIB=custom_lib_md

BUILD_DIR = Build
OUTPATH = Build

#######################################################
#SDK version flags
ifdef MTK_FW_VERSION
CFLAGS += -DMTK_FW_VERSION=\"$(MTK_FW_VERSION)\"
endif

#######################################################

CFLAGS += -DMTK_NBIOT_CUSTOM_BUILD

ifeq ($(MTK_TOOL_AUTHENTICATION_ENABLE), y)
CFLAGS += -DMTK_TOOL_AUTHENTICATION_ENABLE
endif

include module.mk

ifneq ($(wildcard $(strip $(SOURCE_DIR))/middleware/MTK/nbiot/),)
include $(SOURCE_DIR)/middleware/MTK/nbiot/md_flags.mk
else
include $(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/md_flags.mk
endif

include $(SOURCE_DIR)/middleware/MTK/nbiot_custom_rf/$(MTK_NB_CUSTOM_DIR)/module.mk
include $(SOURCE_DIR)/middleware/MTK/nbiot_custom/l4/module.mk
include $(SOURCE_DIR)/middleware/MTK/nbiot_custom/nvdm/module.mk

C_OBJS = $(C_FILES:%.c=$(BUILD_DIR)/%.o)


.PHONY: all $(TARGET_LIB).a

all: $(TARGET_LIB).a
	@echo $(CFLAGS) > $(OUTPATH)/MD_CUSTOM_CFLAGS.config
	@echo Build $< Done

include $(SOURCE_DIR)/.rule.mk

clean:
	rm -rf $(OUTPATH)/$(TARGET_LIB).a
	rm -rf $(BUILD_DIR)

prebuilt_install:
	mkdir -p $(SOURCE_DIR)/prebuilt/middleware/MTK/
	mkdir -p $(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot_custom_rf/$(MTK_NB_CUSTOM_DIR)/inc
	mkdir -p $(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot_custom/l4/inc
	cp $(SOURCE_DIR)/middleware/MTK/nbiot_custom_rf/$(MTK_NB_CUSTOM_DIR)/inc/*.h $(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot_custom_rf/$(MTK_NB_CUSTOM_DIR)/inc
	cp $(SOURCE_DIR)/middleware/MTK/nbiot_custom/l4/atci.mod/inc/*.h $(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot_custom/l4/atci.mod/inc
