
BOARD_SRC = driver/board/mt2625_hdk
COMPONENT_SRC = driver/board/component
#font
C_FILES  += $(BOARD_SRC)/public/public_mmi.c
CFLAGS  += -I$(BOARD_SRC)/public




