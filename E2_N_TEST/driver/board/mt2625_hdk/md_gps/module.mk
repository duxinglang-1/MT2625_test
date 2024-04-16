BOARD_SRC = driver/board/mt2625_hdk
COMPONENT_SRC = driver/board/component
#ublox
C_FILES  += $(BOARD_SRC)/md_gps/ublox_gps.c
CFLAGS  += -I$(BOARD_SRC)/md_gps



