
###################################################
# Sources
WAKAAMA_EXAMPLE_DIR = middleware/third_party/lwm2m/examples

C_FILES  += ${WAKAAMA_EXAMPLE_DIR}/bootstrap_server/bootstrap_server.c     \
      			${WAKAAMA_EXAMPLE_DIR}/bootstrap_server/bootstrap_info.c       \
    			  ${WAKAAMA_EXAMPLE_DIR}/client/lwm2mclient.c                    \
  				  ${WAKAAMA_EXAMPLE_DIR}/client/system_api.c                     \
    			  ${WAKAAMA_EXAMPLE_DIR}/client/object_security.c                \
  			    ${WAKAAMA_EXAMPLE_DIR}/client/object_server.c                  \
    			  ${WAKAAMA_EXAMPLE_DIR}/client/object_device.c                  \
  			    ${WAKAAMA_EXAMPLE_DIR}/client/object_firmware.c                \
    			  ${WAKAAMA_EXAMPLE_DIR}/client/object_location.c                \
    			  ${WAKAAMA_EXAMPLE_DIR}/client/object_connectivity_moni.c       \
    			  ${WAKAAMA_EXAMPLE_DIR}/client/object_connectivity_stat.c       \
    			  ${WAKAAMA_EXAMPLE_DIR}/client/object_access_control.c          \
				    ${WAKAAMA_EXAMPLE_DIR}/client/object_barometer.c				\
				    ${WAKAAMA_EXAMPLE_DIR}/client/object_watermeter.c				\
    			  ${WAKAAMA_EXAMPLE_DIR}/client/test_object.c

#C_FILES  += ${WAKAAMA_EXAMPLE_DIR}/bootstrap_server/bootstrap_server.c     \
#     			  ${WAKAAMA_EXAMPLE_DIR}/bootstrap_server/bootstrap_info.c       \
#    			    ${WAKAAMA_EXAMPLE_DIR}/lightclient/lightclient.c               \
#  				    ${WAKAAMA_EXAMPLE_DIR}/lightclient/object_device.c             \
#     			  ${WAKAAMA_EXAMPLE_DIR}/lightclient/object_security.c           \
#     			  ${WAKAAMA_EXAMPLE_DIR}/lightclient/object_server.c             \
#     			  ${WAKAAMA_EXAMPLE_DIR}/lightclient/test_object.c

C_FILES  += ${WAKAAMA_EXAMPLE_DIR}/shared/commandline.c                    \
            ${WAKAAMA_EXAMPLE_DIR}/lwm2m_cli.c

C_FILES  += ${WAKAAMA_EXAMPLE_DIR}/server/lwm2mserver.c

###################################################
# include path
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/lwm2m/core
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/lwm2m/core/er-coap-13
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/lwm2m/examples
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/lwm2m/examples/bootstrap_server
#CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/lwm2m/examples/client
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/lwm2m/examples/server
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/lwm2m/examples/shared