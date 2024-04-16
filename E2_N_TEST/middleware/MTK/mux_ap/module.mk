
###############################################################################
# feature option dependency
###############################################################################

#CFLAGS += -DMUX_AP_UT

#################################################################################
# source files
#################################################################################

ifeq ($(MTK_NB_MODEM_ENABLE), n)
C_FILES  += middleware/MTK/nbiot/devices/transports/csci.mod/pub/csci.c
endif

C_FILES  += middleware/MTK/mux_ap/src/mux_ap.c
C_FILES  += middleware/MTK/mux_ap/src/mux_ap_csci.c
C_FILES  += middleware/MTK/mux_ap/src/mux_ap_test.c

#################################################################################
# include path
#################################################################################

CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/mux_ap/inc
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/nbiot/devices/transports/csci.mod/api

