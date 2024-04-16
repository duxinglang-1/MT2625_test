BOARD_SRC = driver/board/mt2625_hdk
COMPONENT_SRC = driver/board/component

C_FILES  += $(BOARD_SRC)/max30102/MAX30102.c
C_FILES  += $(BOARD_SRC)/max30102/algorithm.c
C_FILES  += $(BOARD_SRC)/max30102/yc_spo2_alg.c
C_FILES  += $(BOARD_SRC)/max30102/hrm_alg_v01.c
C_FILES  += $(BOARD_SRC)/max30102/DataProcess.c


#################################################################################
#include path
CFLAGS  += -I$(BOARD_SRC)/max30102
#LIBS += $(SOURCE_DIR)/$(BOARD_SRC)/max30102/spo2.a



