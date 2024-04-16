
###################################################
# Sources
TCM_DIR = middleware/MTK/tel_conn_mgr
TCM_FILES = $(TCM_DIR)/app/src/tel_conn_mgr_app_api.c \
             $(TCM_DIR)/app/src/tel_conn_mgr_app_mgr.c \
             $(TCM_DIR)/bearer/src/tel_conn_mgr_bearer_api.c \
             $(TCM_DIR)/bearer/src/tel_conn_mgr_bearer_at_cmd_util.c \
             $(TCM_DIR)/bearer/src/tel_conn_mgr_bearer_cache.c \
             $(TCM_DIR)/bearer/src/tel_conn_mgr_bearer_list_mgr.c \
             $(TCM_DIR)/bearer/src/tel_conn_mgr_bearer_main.c \
             $(TCM_DIR)/bearer/src/tel_conn_mgr_bearer_msg_hdlr.c \
             $(TCM_DIR)/bearer/src/tel_conn_mgr_bearer_urc.c \
             $(TCM_DIR)/bearer/src/tel_conn_mgr_bearer_util.c \
             $(TCM_DIR)/bearer/src/tel_conn_mgr_bearer_timer.c \
             $(TCM_DIR)/common/src/tel_conn_mgr_bearer_info.c \
             $(TCM_DIR)/common/src/tel_conn_mgr_platform.c \
             $(TCM_DIR)/common/src/tel_conn_mgr_util.c \
             $(TCM_DIR)/common/src/tel_conn_mgr_ut.c

ifeq ($(MTK_CREATE_DEFAULT_APN),y)
TCM_FILES  +=  $(TCM_DIR)/apn/src/tel_conn_mgr_default_pdn_link.c 
endif

C_FILES += $(TCM_FILES)
###################################################
# include path
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/tel_conn_mgr/app/inc
ifeq ($(MTK_CREATE_DEFAULT_APN),y)
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/tel_conn_mgr/apn/inc
endif
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/tel_conn_mgr/bearer/inc
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/tel_conn_mgr/common/inc
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/ril/inc
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/lwip/ports/include
CFLAGS += -I$(SOURCE_DIR)/kernel/rtos/FreeRTOS/Source/include
