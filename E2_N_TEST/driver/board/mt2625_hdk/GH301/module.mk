
BOARD_SRC = driver/board/mt2625_hdk
COMPONENT_SRC = driver/board/component
#hrs3300
C_FILES  += $(BOARD_SRC)/hrs3300/hrs3300.c
CFLAGS  += -I$(BOARD_SRC)/hrs3300
LIBS += $(SOURCE_DIR)/$(BOARD_SRC)/hrs3300/gcc_mt2523_20171226_v21_05d_lbp.lib



