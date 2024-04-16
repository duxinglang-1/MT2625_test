
BOARD_SRC = driver/board/mt2625_hdk
COMPONENT_SRC = driver/board/component
#font
C_FILES  += $(BOARD_SRC)/font/gdi_font.c
C_FILES  += $(BOARD_SRC)/font/ascii_map.c
C_FILES  += $(BOARD_SRC)/font/ascii_24.c



CFLAGS  += -I$(BOARD_SRC)/font




