
BOARD_SRC = driver/board/mt2625_hdk
COMPONENT_SRC = driver/board/component

# display driver file
C_FILES  += $(BOARD_SRC)/lcd/mt2625_hdk_lcd.c
C_FILES  += $(BOARD_SRC)/backlight/mt2625_hdk_backlight.c

# LCM driver file
ifeq ($(YQ_WATCH_SPPPORT), y)
C_FILES  += $(COMPONENT_SRC)/lcm/ST7789H2_SPI/lcd_ST7789H2_SPI_YQ.c
else
C_FILES  += $(COMPONENT_SRC)/lcm/ST7789H2_SPI/lcd_ST7789H2_SPI.c
endif

#################################################################################
#include path
CFLAGS 	+= -Iinclude
CFLAGS  += -I$(SOURCE_DIR)/kernel/service/inc
CFLAGS  += -I$(SOURCE_DIR)/driver/chip/inc
CFLAGS  += -I$(SOURCE_DIR)/driver/chip/mt2625/inc


#display driver include path
CFLAGS  += -I$(SOURCE_DIR)/driver/board/component/common
CFLAGS  += -I$(BOARD_SRC)/lcd
CFLAGS  += -I$(BOARD_SRC)/backlight
