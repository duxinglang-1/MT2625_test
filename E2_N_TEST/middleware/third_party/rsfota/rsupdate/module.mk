SOURCE_DIR = ../../../../..

RSUPDATE_SRC = middleware/third_party/rsfota/rsupdate

LIBS += $(SOURCE_DIR)/$(RSUPDATE_SRC)/lib/librsua.a
LIBS += $(SOURCE_DIR)/$(RSUPDATE_SRC)/lib/librsuasdk.a


C_FILES += $(RSUPDATE_SRC)/src/rs_ua_porting.c
C_FILES += $(RSUPDATE_SRC)/src/rs_ua_flash.c
C_FILES += $(RSUPDATE_SRC)/src/rs_ua_fs.c


#################################################################################
#include path

CFLAGS 	+= -I$(SOURCE_DIR)/$(RSUPDATE_SRC)/inc

