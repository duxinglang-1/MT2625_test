
WIFI_STUB_CONF_WFCM_SRC = middleware/MTK/wifi_host/wfcm


ifeq ($(MTK_WIFI_STUB_CONF_SPI_ENABLE),y)
    C_FILES  += $(WIFI_STUB_CONF_WFCM_SRC)/src/wfcm_spi.c
    C_FILES  += $(WIFI_STUB_CONF_WFCM_SRC)/src/wfcm_stub.c
else
  ifeq ($(MTK_WIFI_STUB_CONF_SDIO_MSDC_ENABLE),y)
    C_FILES  += $(WIFI_STUB_CONF_WFCM_SRC)/src/wfcm_sdio.c
    C_FILES  += $(WIFI_STUB_CONF_WFCM_SRC)/src/wfcm_stub.c
  endif
endif


#################################################################################
#include path
CFLAGS 	+= -Iinc
CFLAGS  += -I$(SOURCE_DIR)/kernel/service/inc
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/wifi_host/wfcm/inc
