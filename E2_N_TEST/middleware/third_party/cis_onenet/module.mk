
###################################################
# Sources
ONENET_SRC = middleware/third_party/cis_onenet

C_FILES  += $(ONENET_SRC)/ciscore/cis_block1.c                      \
            $(ONENET_SRC)/ciscore/cis_bootstrap.c                   \
            $(ONENET_SRC)/ciscore/cis_config.c                      \
            $(ONENET_SRC)/ciscore/cis_core.c                        \
            $(ONENET_SRC)/ciscore/cis_data.c                        \
            $(ONENET_SRC)/ciscore/cis_discover.c                    \
            $(ONENET_SRC)/ciscore/cis_json.c                        \
            $(ONENET_SRC)/ciscore/cis_list.c                        \
            $(ONENET_SRC)/ciscore/cis_log.c                         \
            $(ONENET_SRC)/ciscore/cis_management.c                  \
            $(ONENET_SRC)/ciscore/cis_memtrace.c                    \
            $(ONENET_SRC)/ciscore/cis_objects.c                     \
            $(ONENET_SRC)/ciscore/cis_observe.c                     \
            $(ONENET_SRC)/ciscore/cis_packet.c                      \
            $(ONENET_SRC)/ciscore/cis_registration.c                \
            $(ONENET_SRC)/ciscore/cis_tlv.c                         \
            $(ONENET_SRC)/ciscore/cis_transaction.c                 \
            $(ONENET_SRC)/ciscore/cis_uri.c                         \
            $(ONENET_SRC)/ciscore/cis_utils.c                       \
            $(ONENET_SRC)/ciscore/std_object/std_object.c           \
            $(ONENET_SRC)/ciscore/std_object/std_object_security.c  \
            $(ONENET_SRC)/ciscore/std_object/cis_object_conn_moniter.c \
            $(ONENET_SRC)/ciscore/std_object/cis_object_device.c    \
            $(ONENET_SRC)/ciscore/std_object/cis_object_firmware.c  \
            $(ONENET_SRC)/ciscore/dm_utils/dm_endpoint.c            \
            $(ONENET_SRC)/ciscore/dm_utils/j_base64.c               \
            $(ONENET_SRC)/porting/cis_if_net.c                      \
            $(ONENET_SRC)/porting/cis_if_sys.c                      \
            $(ONENET_SRC)/porting/cis_sock.c


###################################################
# include path
CFLAGS += -DCIS_EMBED -DHAVE_STRUCT_TIMESPEC
CFLAGS += -I$(SOURCE_DIR)/kernel/rtos/FreeRTOS/Source/include
CFLAGS += -I$(SOURCE_DIR)/driver/chip/inc
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/cis_onenet/ciscore
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/er-coap-13
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/cis_onenet/ciscore/std_object
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/cis_onenet/ciscore/dm_utils
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/cis_onenet/porting
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/mbedtls/include/mbedtls

