NVDM_MODEM_SRC = middleware/MTK/nvdm_modem

C_FILES += $(NVDM_MODEM_SRC)/src_core/nvdm_modem_main.c
C_FILES += $(NVDM_MODEM_SRC)/src_core/nvdm_modem_data.c
C_FILES += $(NVDM_MODEM_SRC)/src_core/nvdm_modem_io.c
C_FILES += $(NVDM_MODEM_SRC)/src_core/nvdm_modem_interface.c
C_FILES += $(NVDM_MODEM_SRC)/src_core/nvdm_modem_manager.c
C_FILES += $(NVDM_MODEM_SRC)/src_core/nvdm_modem_otp_data.c

ifeq ($(MTK_MINI_DUMP_ENABLE), y)
C_FILES += $(NVDM_MODEM_SRC)/src_core/nvdm_modem_minidump.c
endif
#################################################################################
#include path
CFLAGS 	+= -I$(SOURCE_DIR)/$(NVDM_MODEM_SRC)/inc
CFLAGS 	+= -I$(SOURCE_DIR)/$(NVDM_MODEM_SRC)/inc_core
CFLAGS  += -I$(SOURCE_DIR)/middleware/util/include
CFLAGS  += -I$(SOURCE_DIR)/kernel/rtos/FreeRTOS/include
CFLAGS  += -I$(SOURCE_DIR)/kernel/rtos/FreeRTOS/Source/include
CFLAGS  += -I$(SOURCE_DIR)/kernel/rtos/FreeRTOS/Source/include/portable/GCC/ARM_CM4F
CFLAGS  += -I$(SOURCE_DIR)/kernel/rtos/FreeRTOS/Source
CFLAGS  += -I$(SOURCE_DIR)/middleware/mlog/include
CFLAGS  += -I$(SOURCE_DIR)/driver/chip/inc

#################################################################################
