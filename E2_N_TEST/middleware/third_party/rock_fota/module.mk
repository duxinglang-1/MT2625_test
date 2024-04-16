#################################################################################
# source files
#################################################################################
ROCK_SRC = middleware/third_party/rock_fota

#portting code 

C_FILES  += $(ROCK_SRC)/src/rock_ua.c

LIBS += $(SOURCE_DIR)/middleware/third_party/rock_fota/lib/libiotpatch_gmobi_316_armgcc484.a

#################################################################################
# include path
#################################################################################

CFLAGS  += -I$(SOURCE_DIR)/middleware/third_party/rock_fota/inc

#################################################################################
# Define Macros
#################################################################################
ifeq ($(ROCK_FOTA_SUPPORT), y)
CFLAGS += -DROCK_FOTA_SUPPORT
endif