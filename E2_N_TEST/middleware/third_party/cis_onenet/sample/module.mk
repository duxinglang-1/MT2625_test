
###################################################
# Sources
ONENET_SAMPLE_SRC = middleware/third_party/cis_onenet/sample

C_FILES  += $(ONENET_SAMPLE_SRC)/cis_sample_main.c      \
            $(ONENET_SAMPLE_SRC)/cis_sample_entry.c     \
            $(ONENET_SAMPLE_SRC)/cis_sample_dm_entry.c              

###################################################
# include path
CFLAGS += -DCIS_EMBED -DHAVE_STRUCT_TIMESPEC
CFLAGS += -I$(SOURCE_DIR)/include/rtos/FreeRTOS/Source/include
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/cis_onenet/sample
