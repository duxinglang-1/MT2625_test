
###############################################################################
# feature option dependency
###############################################################################

ifneq ($(APB_PROXY_UNIT_TEST), Y)
include $(SOURCE_DIR)/middleware/MTK/mux_ap/module.mk
endif

#################################################################################
# source files
#################################################################################

APB_SRC = middleware/MTK/apb_proxy

C_FILES  += $(APB_SRC)/src/apb_proxy_context_manager.c
C_FILES  += $(APB_SRC)/src/apb_proxy_event_handler.c
C_FILES  += $(APB_SRC)/src/apb_proxy_queue.c
C_FILES  += $(APB_SRC)/src/apb_proxy_msg_queue_def.c
C_FILES  += $(APB_SRC)/src/apb_proxy_packet_decoder.c
C_FILES  += $(APB_SRC)/src/apb_proxy_packet_encoder.c
C_FILES  += $(APB_SRC)/src/apb_proxy.c
C_FILES  += $(APB_SRC)/src/apb_proxy_task.c
C_FILES  += $(APB_SRC)/src/apb_proxy_utility.c
C_FILES  += $(APB_SRC)/src/apb_proxy_at_cmd_tbl.c
ifeq ($(APB_PROXY_UNIT_TEST), Y)
C_FILES  += $(APB_SRC)/unit_test/mux_ap_stub.c
C_FILES  += $(APB_SRC)/unit_test/apb_proxy_utility_unit_test.c
C_FILES  += $(APB_SRC)/unit_test/apb_proxy_queue_unit_test.c
C_FILES  += $(APB_SRC)/unit_test/apb_proxy_unit_test_framework.c
C_FILES  += $(APB_SRC)/unit_test/apb_proxy_packet_encoder_unit_test.c
C_FILES  += $(APB_SRC)/unit_test/apb_proxy_packet_decoder_unit_test.c
endif
C_FILES  += $(APB_SRC)/apb_proxy_cmd_hdlr/apb_test/apb_proxy_test.c
C_FILES  += $(APB_SRC)/apb_proxy_cmd_hdlr/ril/apb_proxy_ril_ut_cmd.c
ifeq ($(MTK_ATCI_APB_PROXY_NETWORK_ENABLE), y)
C_FILES  += $(APB_SRC)/apb_proxy_cmd_hdlr/network/apb_proxy_nw_cmd.c
C_FILES  += $(APB_SRC)/apb_proxy_cmd_hdlr/network/apb_proxy_nw_cmd_util.c
C_FILES  += $(APB_SRC)/apb_proxy_cmd_hdlr/network/apb_proxy_nw_socket_cmd.c
C_FILES  += $(APB_SRC)/apb_proxy_cmd_hdlr/network/lwm2m/lwm2m_app_context_manager.c
C_FILES  += $(APB_SRC)/apb_proxy_cmd_hdlr/network/apb_proxy_nw_lwm2m_cmd.c
C_FILES  += $(APB_SRC)/apb_proxy_cmd_hdlr/network/apb_proxy_nw_mqtt_cmd.c
C_FILES  += $(APB_SRC)/apb_proxy_cmd_hdlr/network/apb_proxy_nw_sntp_cmd.c
C_FILES  += $(APB_SRC)/apb_proxy_cmd_hdlr/network/apb_proxy_nw_httpclient_cmd.c
C_FILES  += $(APB_SRC)/apb_proxy_cmd_hdlr/network/apb_proxy_nw_onenet_cmd.c
C_FILES  += $(APB_SRC)/apb_proxy_cmd_hdlr/network/apb_proxy_nw_dm_cmd.c
C_FILES  += $(APB_SRC)/apb_proxy_cmd_hdlr/network/apb_proxy_nw_tmo_cmd.c
C_FILES  += $(APB_SRC)/apb_proxy_cmd_hdlr/network/apb_proxy_nw_ctm2m_cmd.c
C_FILES  += $(APB_SRC)/apb_proxy_cmd_hdlr/network/apb_proxy_nw_ctiot_cmd.c
ifeq ($(MTK_COAP_SUPPORT), y)
C_FILES  += $(APB_SRC)/apb_proxy_cmd_hdlr/network/coap/apb_proxy_nw_coap_cmd.c
C_FILES  += $(APB_SRC)/apb_proxy_cmd_hdlr/network/coap/apb_proxy_nw_coap.c
endif
C_FILES  += $(APB_SRC)/apb_proxy_cmd_hdlr/network/apb_proxy_nw_tls_cmd.c
endif
ifeq ($(MTK_FOTA_ENABLE), y)
C_FILES  += $(APB_SRC)/apb_proxy_cmd_hdlr/fota/apb_proxy_fota_cmd.c
endif

#################################################################################
# include path
#################################################################################

CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/apb_proxy/inc
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/apb_proxy/inc_private
ifeq ($(APB_PROXY_UNIT_TEST), Y)
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/apb_proxy/unit_test
endif
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/apb_proxy/apb_proxy_cmd_hdlr/ril
ifeq ($(MTK_ATCI_APB_PROXY_NETWORK_ENABLE), y)
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/apb_proxy/apb_proxy_cmd_hdlr/network
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/apb_proxy/apb_proxy_cmd_hdlr/network/lwm2m
ifeq ($(MTK_COAP_SUPPORT), y)
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/apb_proxy/apb_proxy_cmd_hdlr/network/coap
endif
endif
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/apb_proxy/apb_proxy_cmd_hdlr/apb_test
ifeq ($(MTK_FOTA_ENABLE), y)
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/apb_proxy/apb_proxy_cmd_hdlr/fota
endif
ifeq ($(MTK_ATCI_APB_PROXY_ADAPTER_ENABLE), y)
CFLAGS  += -I$(SOURCE_DIR)/middleware/MTK/apb_proxy/apb_proxy_cmd_hdlr/atci_adapter
endif

#################################################################################
# Define Macros
#################################################################################

ifeq ($(APB_PROXY_UNIT_TEST), Y)
CFLAGS += -DENABLE_APB_PROXY_UNIT_TEST
endif

ifeq ($(MTK_APB_PROXY_PROJECT_COMMAND_ENABLE), y)
CFLAGS += -DAPB_PROXY_PROJECT_COMMAND_ENABLE
endif

ifeq ($(MTK_FOTA_ENABLE), y)
CFLAGS += -DAPB_PROXY_FOTA_CMD_ENABLE
endif

ifeq ($(MTK_ATCI_APB_PROXY_NETWORK_ENABLE), y)
CFLAGS += -DMTK_ATCI_APB_PROXY_NETWORK_ENABLE
endif

ifeq ($(MTK_COAP_SUPPORT), y)
CFLAGS += -DMTK_COAP_SUPPORT
endif

ifeq ($(MTK_LWM2M_AT_CMD_SUPPORT), y)
CFLAGS += -DMTK_LWM2M_AT_CMD_SUPPORT
endif
