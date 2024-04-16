TOOLAUTH_ROOT = middleware/MTK/tool_authentication

###############################################################################
# feature option dependency
###############################################################################
CFLAGS += -DMTK_TOOL_AUTHENTICATION_ENABLE
CFLAGS += -UMBEDTLS_CONFIG_FILE -DMBEDTLS_CONFIG_FILE='<config-mtk-rsassa_pss.h>'

#################################################################################
# library files
#################################################################################
LIBS += $(SOURCE_DIR)/prebuilt/$(TOOLAUTH_ROOT)/lib/libtool_authentication_CM4_GCC.a

#################################################################################
# include path
#################################################################################
CFLAGS += -I$(SOURCE_DIR)/prebuilt/$(TOOLAUTH_ROOT)/inc/
