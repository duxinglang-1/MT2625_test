
NBIOT_NVDM_CUSTOM_SRC = middleware/MTK/nbiot_custom/nvdm

# Source files
#   COMMAND INTERPRETER files
C_FILES += $(NBIOT_NVDM_CUSTOM_SRC)/src/nvdm_modem_default_config.c


##############################################################################
# Include paths for this module
##############################################################################

CFLAGS += -I$(SOURCE_DIR)/$(NBIOT_NVDM_CUSTOM_SRC)
CFLAGS += -I$(SOURCE_DIR)/$(NBIOT_NVDM_CUSTOM_SRC)/atci.mod/inc
CFLAGS += -I$(SOURCE_DIR)/$(NBIOT_NVDM_CUSTOM_SRC)/mux_gpio/inc
CFLAGS += -I$(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/sys/inc
CFLAGS += -I$(OUTPATH)/autogen/conf_xml

