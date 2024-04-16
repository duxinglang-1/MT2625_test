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
SDK_PATH = $(abspath $(PWD)/$(SOURCE_DIR))/
BINPATH = $(SOURCE_DIR)/tools/gcc/gcc-arm-none-eabi/bin

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

# LD Flags
LDFLAGS = $(ALLFLAGS) $(FPUFLAGS) --specs=nano.specs -lnosys -nostartfiles
LDFLAGS += -Wl,-wrap=malloc -Wl,-wrap=calloc -Wl,-wrap=realloc -Wl,-wrap=free
#LDFLAGS += -Wl,-Tram.ld -Wl,--gc-sections
LDFLAGS += -Wl,-Tmt2625_flash.ld -Wl,--gc-sections
ifeq ($(DEBUG_IO), 1)
LDFLAGS += --specs=rdimon.specs -lrdimon
CFLAGS += -DDEBUG_IO
endif
#######################################################
# Include Module

include $(FEATURE)
# Gloabl Config
-include $(SOURCE_DIR)/.config
# IC Config
-include $(SOURCE_DIR)/config/chip/$(IC_CONFIG)/chip.mk
# Board Config
#-include $(SOURCE_DIR)/config/board/$(BOARD_CONFIG)/board.mk

# EPT Config
-include $(SOURCE_DIR)/driver/board/mt2625_hdk/ept/module.mk
ifeq ($(MTK_PORT_SERVICE_ENABLE), y)
# add PORT SERVICE files
include $(SOURCE_DIR)/middleware/MTK/port_service/module.mk
endif

ifeq ($(MTK_USB_DEMO_ENABLED), y)
# add USB CLASS files
include $(SOURCE_DIR)/middleware/MTK/usb/module.mk
endif

# Sources or Lib
# RTOS source files
include $(SOURCE_DIR)/kernel/rtos/FreeRTOS/module.mk


ifndef FLAG_STORE_FOLDER
FLAG_STORE_FOLDER = $(OUTPATH)
endif

# kernel service files
include $(SOURCE_DIR)/kernel/service/module.mk

# HAL driver files
include $(SOURCE_DIR)/driver/chip/mt2625/module.mk

# Keypad Config
-include $(SOURCE_DIR)/driver/board/mt2625_hdk/keypad/module.mk

ifeq ($(MTK_ATCI_ENABLE), y)
# add ATCI files
include $(SOURCE_DIR)/middleware/MTK/atci/module.mk
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

ifneq ($(MTK_SYSTEM_CLOCK_SET), )
CFLAGS += -DMTK_SYSTEM_CLOCK_SET=$(MTK_SYSTEM_CLOCK_SET)
endif

#######################################################
# Main APP files

# Main APP files
APP_PATH        = $(patsubst $(SDK_PATH)/%,%,$(abspath $(dir $(PWD))))

$(info $(SDK_PATH))
$(info $(PROJ_PATH))
$(info $(APP_PATH))

APP_PATH_SRC = $(APP_PATH)/src

APP_FILES += $(APP_PATH_SRC)/main.c


APP_FILES += $(APP_PATH_SRC)/ept_gpio_var.c
APP_FILES += $(APP_PATH_SRC)/ept_eint_var.c
APP_FILES += $(APP_PATH_SRC)/sys_init.c
APP_FILES += $(APP_PATH_SRC)/system_mt2625.c
APP_FILES += $(APP_PATH_SRC)/md_init.c
APP_FILES += $(APP_PATH)/GCC/syscalls.c

C_FILES += $(APP_FILES)

# add startup file to build
S_FILES += $(APP_PATH)/GCC/startup_mt2625.s
S_FILES += $(APP_PATH)/GCC/startup_md.s

C_OBJS = $(C_FILES:%.c=$(BUILD_DIR)/%.o)
CXX_OBJS        = $(CXX_FILES:%.cpp=$(BUILD_DIR)/%.o)
S_OBJS = $(S_FILES:%.s=$(BUILD_DIR)/%.o)

#######################################################
# Include path

CFLAGS += -I$(SOURCE_DIR)/$(APP_PATH)/inc
CFLAGS += -I$(SOURCE_DIR)/$(APP_PATH)/psram/inc
CFLAGS += -I$(SOURCE_DIR)/driver/CMSIS/Include
CFLAGS += -I$(SOURCE_DIR)/driver/CMSIS/Device/MTK/mt2625/Include
CFLAGS += -I$(SOURCE_DIR)/driver/chip/mt2625/inc
CFLAGS += -I$(SOURCE_DIR)/driver/chip/inc
CFLAGS += -I$(SOURCE_DIR)/kernel/service/inc

# Rules

.PHONY: proj clean $(MODULE_PATH)

all: cleanlog proj
	@mkdir -p $(BUILD_DIR)
	@$(SIZE) $(OUTPATH)/$(PROJ_NAME).elf
	@$(SOURCE_DIR)/tools/scripts/build/copy_firmware.sh $(SOURCE_DIR) $(OUTPATH) $(IC_CONFIG) $(BOARD_CONFIG) $(PROJ_NAME).bin $(PWD)

MOD_EXTRA = BUILD_DIR=$(BUILD_DIR) OUTPATH=$(OUTPATH) PROJ_PATH=$(PROJ_PATH)

$(LIBS): $(MODULE_PATH)

$(MODULE_PATH):
	@+make -C $@ $(MOD_EXTRA) $($@_EXTRA)

proj: $(OUTPATH)/$(PROJ_NAME).elf

$(OUTPATH)/$(PROJ_NAME).elf: $(C_OBJS) $(CXX_OBJS) $(S_OBJS) $(LIBS)
	@echo "export LDFLAGS"
	echo $(LDFLAGS) > $(FLAG_STORE_FOLDER)/LDFLAGS.config
	@echo "export CFLAGS"
	echo $(CFLAGS) > $(FLAG_STORE_FOLDER)/CFLAGS.config
	@echo "export end"
	@echo Linking...
	@if [ -e "$@" ]; then rm -f "$@"; fi
	@if [ -e "$(OUTPATH)/$(PROJ_NAME).map" ]; then rm -f "$(OUTPATH)/$(PROJ_NAME).map"; fi
	@if [ -e "$(OUTPATH)/$(PROJ_NAME).dis" ]; then rm -f "$(OUTPATH)/$(PROJ_NAME).dis"; fi
	@if [ -e "$(OUTPATH)/$(PROJ_NAME).hex" ]; then rm -f "$(OUTPATH)/$(PROJ_NAME).hex"; fi
	@if [ -e "$(OUTPATH)/$(PROJ_NAME).bin" ]; then rm -f "$(OUTPATH)/$(PROJ_NAME).bin"; fi
	@echo $(patsubst $(SDK_PATH)/%,$(SOURCE_DIR)/%, $(LDFLAGS) -Wl,--start-group $^ -Wl,--end-group -Wl,-Map=$(patsubst $(SDK_PATH)/%,$(SOURCE_DIR)/%,$(OUTPATH)/$(PROJ_NAME).map) -lm -o $@) > $(OUTPATH)/link_option.tmp
	@$(CC) @$(OUTPATH)/link_option.tmp 2>>$(ERR_LOG)
	@rm -f $(OUTPATH)/link_option.tmp
	@$(OBJDUMP) -D $(OUTPATH)/$(PROJ_NAME).elf > $(OUTPATH)/$(PROJ_NAME).dis
	@$(OBJCOPY) -O ihex $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).hex
	@$(OBJCOPY) -O binary $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).bin
	@echo Done


include $(SOURCE_DIR)/.rule.mk

clean:
	rm -rf $(OUTPATH)


