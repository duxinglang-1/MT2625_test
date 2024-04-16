
WIFI_SDIO_SRC = middleware/MTK/wifi_host/platform/freertos/hif/sdio

C_FILES  += $(WIFI_SDIO_SRC)/src/hif_sdio.c



#################################################################################
#include path
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/wifi_host/platform/freertos/hif/inc