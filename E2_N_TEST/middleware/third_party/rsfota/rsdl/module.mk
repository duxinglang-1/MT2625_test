SOURCE_DIR = ../../../../..
RSDL_SRC = middleware/third_party/rsfota/rsdl

#portting code 

C_FILES  += $(RSDL_SRC)/porting/mt2625/src/rs_socket.c
C_FILES  += $(RSDL_SRC)/porting/mt2625/src/rs_fs.c
C_FILES  += $(RSDL_SRC)/porting/mt2625/src/rs_thread.c
C_FILES  += $(RSDL_SRC)/porting/mt2625/src/rs_sdk_api_ex.c
C_FILES  += $(RSDL_SRC)/porting/mt2625/src/rs_notify_user.c
C_FILES  += $(RSDL_SRC)/porting/mt2625/src/rs_system.c
C_FILES  += $(RSDL_SRC)/porting/mt2625/src/rs_std_fun.c
C_FILES  += $(RSDL_SRC)/porting/mt2625/src/rs_param.c
C_FILES  += $(RSDL_SRC)/porting/mt2625/src/rs_mem.c
C_FILES  += $(RSDL_SRC)/porting/mt2625/src/rs_dev.c
C_FILES  += $(RSDL_SRC)/porting/mt2625/src/rs_flash_operate.c

LIBS += $(SOURCE_DIR)/$(RSDL_SRC)/lib/librsdl.a


#################################################################################
#include path

CFLAGS 	+= -I$(SOURCE_DIR)/$(RSDL_SRC)/porting/inc
CFLAGS 	+= -I$(SOURCE_DIR)/$(RSDL_SRC)/porting/mt2625/inc



