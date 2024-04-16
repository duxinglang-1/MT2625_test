BOARD_SRC = driver/board/mt2625_hdk
COMPONENT_SRC = driver/board/component
#ublox
C_FILES  += $(BOARD_SRC)/md_ble/ch_ble.c
CFLAGS  += -I$(BOARD_SRC)/md_ble
#LIBS += $(SOURCE_DIR)/$(BOARD_SRC)/hrs3300/gcc_mt2523_20171226_v21_05d_lbp.lib


