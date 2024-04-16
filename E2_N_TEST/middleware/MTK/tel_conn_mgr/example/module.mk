
###################################################
# Sources
TCM_EXAMPLE_DIR = middleware/MTK/tel_conn_mgr/example
TCM_EXAMPLE_FILES = $(TCM_EXAMPLE_DIR)/src/tel_conn_mgr_example.c \
                    $(TCM_EXAMPLE_DIR)/src/tel_conn_mgr_example_ext.c


C_FILES += $(TCM_EXAMPLE_FILES)
###################################################
# include path
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/tel_conn_mgr/example/inc
CFLAGS += -I$(SOURCE_DIR)/kernel/rtos/FreeRTOS/Source/include
