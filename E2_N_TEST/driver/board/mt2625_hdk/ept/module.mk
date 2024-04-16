
EPT_2625_SRC = driver/board/mt2625_hdk

C_FILES  += driver/board/mt2625_hdk/ept/src/bsp_gpio_ept_config.c

#################################################################################
#include path
CFLAGS 	+= -Iinclude
CFLAGS  += -I$(SOURCE_DIR)/driver/board/mt2625_hdk/ept/inc
CFLAGS  += -I$(SOURCE_DIR)/driver/CMSIS/Device/MTK/mt2625/Include


