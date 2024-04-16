
FEATURE ?= feature.mk
include $(FEATURE)

###################################################
# Sources
SIMAT_PROXY_SRC = middleware/MTK/simat_proxy
SIMAT_PROXY_FILES = $(SIMAT_PROXY_SRC)/src/simat_proxy_network.c

C_FILES += $(SIMAT_PROXY_FILES)

###################################################
# include path
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/simat_proxy/inc
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/mux_ap/inc
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/tel_conn_mgr/app/inc
