NVDM_MODEM_SRC = middleware/MTK/nvdm_modem

C_FILES += $(NVDM_MODEM_SRC)/src/nvdm_modem_port.c

#################################################################################
#include path
CFLAGS  += -I$(SOURCE_DIR)/$(NVDM_MODEM_SRC)/inc
CFLAGS  += -I$(SOURCE_DIR)/$(NVDM_MODEM_SRC)/inc_core
CFLAGS  += -I$(SOURCE_DIR)/middleware/util/include 
CFLAGS  += -I$(SOURCE_DIR)/kernel/rtos/FreeRTOS/include
CFLAGS  += -I$(SOURCE_DIR)/kernel/rtos/FreeRTOS/Source/include 
CFLAGS  += -I$(SOURCE_DIR)/kernel/rtos/FreeRTOS/Source/include/portable/GCC/ARM_CM4F
CFLAGS  += -I$(SOURCE_DIR)/kernel/rtos/FreeRTOS/Source
CFLAGS  += -I$(SOURCE_DIR)/middleware/mlog/include
CFLAGS  += -I$(SOURCE_DIR)/driver/chip/inc
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/nbiot_custom/rf/inc
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/nbiot_custom/nvdm
ifneq ($(wildcard $(strip $(SOURCE_DIR))/middleware/MTK/nvdm_modem/src_core/),)
include $(SOURCE_DIR)/middleware/MTK/nvdm_modem/src_core/GCC/module.mk
else
LIBS += $(SOURCE_DIR)/middleware/MTK/nvdm_modem/lib/libnvdm_modem_CM4_GCC.a
endif

#################################################################################
