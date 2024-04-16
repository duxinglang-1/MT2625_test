
###############################################################################
# feature option dependency
###############################################################################

## MTK_FOTA_ENABLE
## Brief:       This option is to enable and disable FOTA.
## Usage:       If the value is "y", the MTK_FOTA_ENABLE compile option will be defined. You must also include the gva3\middleware\MTK\fota\module.mk in your Makefile before setting the option to "y". 
## Path:        middleware/MTK/fota
## Dependency:  HAL_FLASH_MODULE_ENABLED and HAL_WDT_MODULE_ENABLED must also defined in the hal_feature_config.h  under the project inc folder. If MTK_FOTA_CLI_ENABLE defined in the module.mk is "y", please also include the middleware/MTK/tftp/module.mk. 

ifeq ($(MTK_FOTA_ENABLE),y)
CFLAGS += -DMTK_FOTA_ENABLE
endif

ifeq ($(MTK_GNSS_FOTA_ENABLE),y)
CFLAGS += -DMTK_GNSS_FOTA_ENABLE
endif

ifeq ($(MTK_FOTA_CMD_ENABLE),y)
CFLAGS += -DMTK_FOTA_CMD_ENABLE
endif

ifeq ($(MTK_FOTA_UBIN_ENABLE),y)
CFLAGS += -DMTK_FOTA_UBIN_ENABLE
endif


ifeq ($(MTK_FOTA_CM4_FS_ENABLE), y)
CFLAGS += -DFOTA_FS_ENABLE
endif

ifeq ($(IC_CONFIG), $(filter $(IC_CONFIG),mt7687 mt7697))
CFLAGS += -DMTK_FOTA_ON_7687

ifeq ($(MTK_FOTA_DUAL_IMAGE_ENABLE), y)
CFLAGS += -DMTK_FOTA_DUAL_IMAGE_ENABLE

ifeq ($(MTK_FOTA_DUAL_IMAGE_ONE_PACK_ENABLE), y)
CFLAGS += -DMTK_FOTA_DUAL_IMAGE_ONE_PACK
endif

endif
endif

#################################################################################
# source files
#################################################################################

FOTA_SRC = middleware/MTK/fota
C_FILES  += $(FOTA_SRC)/src/fota.c
C_FILES  += $(FOTA_SRC)/src/internal/fota_http_dl.c
C_FILES  += $(FOTA_SRC)/src/internal/fota_utility.c
C_FILES  += $(FOTA_SRC)/src/internal/fota_timer.c
C_FILES  += $(FOTA_SRC)/src/internal/fota_nw_manager.c
C_FILES  += $(FOTA_SRC)/src/internal/fota_protocol.c
C_FILES  += $(FOTA_SRC)/src/internal/fota_version_controller.c
C_FILES  += $(FOTA_SRC)/src/internal/fota_coap_dl.c
ifeq ($(MTK_GNSS_FOTA_ENABLE),y)
C_FILES  += $(FOTA_SRC)/src/internal/gnss_fota/gnss_fota_bin.c
C_FILES  += $(FOTA_SRC)/src/internal/gnss_fota/gnss_fota_brom.c
C_FILES  += $(FOTA_SRC)/src/internal/gnss_fota/gnss_fota_da.c
C_FILES  += $(FOTA_SRC)/src/internal/gnss_fota/gnss_fota_main.c
C_FILES  += $(FOTA_SRC)/src/internal/gnss_fota/gnss_fota_uart.c
endif

#################################################################################
# include path
#################################################################################

CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/fota/inc
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/fota/inc/internal
ifeq ($(MTK_GNSS_FOTA_ENABLE),y)
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/fota/inc/internal/gnss_fota
endif
CFLAGS  += -I$(SOURCE_DIR)/driver/chip/inc
CFLAGS  += -I$(SOURCE_DIR)/middleware/third_party/httpclient/inc

ifeq ($(MTK_FOTA_DUAL_IMAGE_ENABLE),y)
CFLAGS  += -I$(SOURCE_DIR)/middleware/third_party/mbedtls/include
CFLAGS  += -I$(SOURCE_DIR)/middleware/third_party/httpclient/inc
endif