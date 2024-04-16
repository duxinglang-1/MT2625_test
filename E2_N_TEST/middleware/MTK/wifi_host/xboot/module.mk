WIFI_HOST_SRC = middleware/MTK/wifi_host

ifeq ($(MTK_WIFI_STUB_CONF_SPI_ENABLE),y)

C_FILES  += $(WIFI_HOST_SRC)/xboot/src/xboot_over_spi.c
#include path
CFLAGS 	+= -Iinc
else

C_FILES  += $(WIFI_HOST_SRC)/xboot/src/xboot_over_sdio.c
#include path
CFLAGS 	+= -Iinc
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/wifi_host/platform/freertos/hif/sdio/inc
endif