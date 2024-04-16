
WIFI_SPI_SRC = middleware/MTK/wifi_host/platform/freertos/hif/spi

C_FILES  += $(WIFI_SPI_SRC)/src/hif_spi.c



#################################################################################
#include path
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/wifi_host/platform/freertos/hif/spi/inc