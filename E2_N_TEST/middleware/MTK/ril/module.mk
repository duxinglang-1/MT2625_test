
###############################################################################
# feature option dependency
###############################################################################



#################################################################################
# source files
#################################################################################

RIL_SRC = middleware/MTK/ril

C_FILES  += $(RIL_SRC)/src/ril_task.c 
C_FILES  += $(RIL_SRC)/src/ril_utils.c
C_FILES  += $(RIL_SRC)/src/ril_cmds_def.c
C_FILES  += $(RIL_SRC)/src/ril_cmds_27007.c
C_FILES  += $(RIL_SRC)/src/ril_cmds_27005.c
C_FILES  += $(RIL_SRC)/src/ril_cmds_v250.c
C_FILES  += $(RIL_SRC)/src/ril_cmds_proprietary.c
C_FILES  += $(RIL_SRC)/src/ril_cmds_common.c
C_FILES  += $(RIL_SRC)/src/ril_event_callback.c
C_FILES  += $(RIL_SRC)/src/ril_log.c
C_FILES  += $(RIL_SRC)/src/ril_general_setting.c
C_FILES  += $(RIL_SRC)/src/ril_ut.c

ifeq ($(MTK_RIL_UT_TEST_CASE_ENABLE),y)
CFLAGS  += -D__RIL_UT_TEST_CASES__
endif

ifeq ($(MTK_RIL_SMS_COMMAND_SUPPORT),y)
CFLAGS  += -D__RIL_SMS_COMMAND_SUPPORT__
endif

ifeq ($(MTK_RIL_CMD_SET_SLIM_ENABLE),y)
CFLAGS  += -D__RIL_CMD_SET_SLIM_ENABLE__
endif

#################################################################################
# include path
#################################################################################

CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/ril/inc
