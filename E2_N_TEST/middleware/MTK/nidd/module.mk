
##############################################################################
#
#   Dummy Foreground App for Background Layer
#   
#
##############################################################################
NIDD_SRC = middleware/MTK/nidd

##############################################################################
# Source files for this module
##############################################################################
C_FILES += $(NIDD_SRC)/src/nidd_main.c
C_FILES += $(NIDD_SRC)/src/nidd_util.c
C_FILES += $(NIDD_SRC)/src/nidd_ut.c

##############################################################################
# Include paths for this module
##############################################################################
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/mux_ap/inc
CFLAGS += -I$(SOURCE_DIR)/$(NIDD_SRC)/inc



