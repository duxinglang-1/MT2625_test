
NBIOT_CUSTOM_SRC = middleware/MTK/nbiot_custom

##############################################################################
# Sources for this module, by default build create library from nbiot level
##############################################################################
ifneq ($(MTK_NBIOT_LIB),n)
MID_NBIOT_MD_CUSTOM_PATH += $(SOURCE_DIR)/middleware/MTK/nbiot_custom
MODULE_PATH += $(MID_NBIOT_MD_CUSTOM_PATH)
LIBS += $(OUTPATH)/custom_lib_md.a
else
ifneq ($(wildcard $(strip $(SOURCE_DIR))/middleware/MTK/nbiot/),)
include $(SOURCE_DIR)/middleware/MTK/nbiot/md_flags.mk
else
include $(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/md_flags.mk
endif
include $(SOURCE_DIR)/middleware/MTK/nbiot_custom_rf/$(MTK_NB_CUSTOM_DIR)/module.mk
include $(SOURCE_DIR)/middleware/MTK/nbiot_custom/l4/module.mk
include $(SOURCE_DIR)/middleware/MTK/nbiot_custom/nvdm/module.mk
endif

##############################################################################
# Include paths for this module
##############################################################################
ifneq ($(wildcard $(strip $(SOURCE_DIR))/middleware/MTK/nbiot/),)
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/nbiot/sys/inc
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/nbiot/modem/config
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/nbiot/modem/trace
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/nbiot/platforms/mtk_mt2625
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/nbiot/platforms/inc/public
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/nbiot/modem/l1/inc/public
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/nbiot/devices/inc/public
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/nbiot/modem/pscommon/inc
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/nbiot/modem/pscommon/inc/public
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/nbiot/utinc/public
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/nbiot/modem/psnas/inc/public
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/nbiot/modem/psmdl/inc/public
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/nbiot/modem/psas/inc/public
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/nbiot/modem/l1/n1.mod/inc/public
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/nbiot/modem/l1/n1.mod/state_handler
else
include $(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/module.mk
endif
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/nbiot_custom_rf/$(MTK_NB_CUSTOM_DIR)/inc
