
STK8321_SRC = driver/board/mt2625_hdk/stk8321

C_FILES  += $(STK8321_SRC)/stk8321_driver.c

#################################################################################
#include module
#include $(SOURCE_DIR)/middleware/third_party/libstkMotion/module.mk

CFLAGS  += -I$(SOURCE_DIR)/driver/board/mt2625_hdk/stk8321
LIBS += $(SOURCE_DIR)/$(BOARD_SRC)/stk8321/libstkMotion_v0.3.6_201808290047.a



