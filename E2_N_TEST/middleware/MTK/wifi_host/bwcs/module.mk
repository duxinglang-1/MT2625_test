
WIFI_BWCS_SRC = middleware/MTK/wifi_host/bwcs

C_FILES  += $(WIFI_BWCS_SRC)/src/bwcs_wifi_host.c



#################################################################################
#include path
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/wifi_host/bwcs/inc