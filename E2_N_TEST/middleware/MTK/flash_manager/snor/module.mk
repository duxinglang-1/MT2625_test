
SERIAL_SNOR_SRC = middleware/MTK/flash_manager

C_FILES  += $(SERIAL_SNOR_SRC)/src/flash_manager.c
C_FILES  += $(SERIAL_SNOR_SRC)/snor/src/snor_port.c

#################################################################################
#include path
CFLAGS 	+= -Iinc
CFLAGS  += -I$(SOURCE_DIR)/kernel/service/inc
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/flash_manager/inc
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/flash_manager/snor/inc

ifneq ($(wildcard $(strip $(SOURCE_DIR))/middleware/MTK/flash_manager_protected/snor_protected/),)
include $(SOURCE_DIR)/middleware/MTK/flash_manager_protected/snor_protected/src_protected/GCC/module.mk
else
include $(SOURCE_DIR)/prebuilt/middleware/MTK/flash_manager/snor/module.mk
endif

#Enable the feature by configuring
CFLAGS += -DMTK_FATFS_ON_SERIAL_NOR_FLASH
