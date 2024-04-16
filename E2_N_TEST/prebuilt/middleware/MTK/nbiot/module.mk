ifeq ($(MTK_AGNSS_SUPL2_ENABLE),y)
	LIBS   += $(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/lib/supl2_lib_md.a
else
	LIBS   += $(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/lib/lib_md.a
endif

include  $(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/md_flags.mk

CFLAGS += -I$(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/inc/modem/l1/n1.mod/hw_driver
CFLAGS += -I$(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/inc/modem/l1/n1.mod/api
CFLAGS += -I$(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/inc/modem/trace
CFLAGS += -I$(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/inc/utinc/public
CFLAGS += -I$(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/inc/platforms/mtk_mt2625
CFLAGS += -I$(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/inc/platforms/inc/public
CFLAGS += -I$(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/inc/modem/config
CFLAGS += -I$(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/inc/modem/l1/inc/public
CFLAGS += -I$(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/inc/modem/l1/n1.mod/inc
CFLAGS += -I$(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/inc/devices/inc/public
CFLAGS += -I$(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/inc/modem/pscommon/inc/public
CFLAGS += -I$(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/inc/modem/psnas/inc/public
CFLAGS += -I$(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/inc/modem/psmdl/inc/public
CFLAGS += -I$(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/inc/modem/psas/inc/public
CFLAGS += -I$(SOURCE_DIR)/prebuilt/middleware/MTK/nbiot/inc/sys/inc
