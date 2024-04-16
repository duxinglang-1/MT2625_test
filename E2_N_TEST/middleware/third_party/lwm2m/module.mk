
###################################################
# Sources
WAKAAMA_SOURCES_DIR = middleware/third_party/lwm2m/core
APPOBJECT_SOURCES_DIR = middleware/third_party/lwm2m/objects

C_FILES  += ${WAKAAMA_SOURCES_DIR}/liblwm2m.c        \
            ${WAKAAMA_SOURCES_DIR}/uri.c             \
            ${WAKAAMA_SOURCES_DIR}/utils.c           \
            ${WAKAAMA_SOURCES_DIR}/objects.c         \
            ${WAKAAMA_SOURCES_DIR}/tlv.c             \
            ${WAKAAMA_SOURCES_DIR}/data.c            \
            ${WAKAAMA_SOURCES_DIR}/list.c            \
            ${WAKAAMA_SOURCES_DIR}/packet.c          \
            ${WAKAAMA_SOURCES_DIR}/transaction.c     \
            ${WAKAAMA_SOURCES_DIR}/registration.c    \
            ${WAKAAMA_SOURCES_DIR}/bootstrap.c       \
            ${WAKAAMA_SOURCES_DIR}/management.c      \
            ${WAKAAMA_SOURCES_DIR}/observe.c         \
            ${WAKAAMA_SOURCES_DIR}/json.c            \
            ${WAKAAMA_SOURCES_DIR}/discover.c        \
            ${WAKAAMA_SOURCES_DIR}/block1.c          \
            ${WAKAAMA_SOURCES_DIR}/platform.c        \
            ${WAKAAMA_SOURCES_DIR}/dtlsconnection.c  \
            ${WAKAAMA_SOURCES_DIR}/mbedtlsconnection.c  \
            ${WAKAAMA_SOURCES_DIR}/connection.c

C_FILES  += ${APPOBJECT_SOURCES_DIR}/object_accelerometer.c     \
            ${APPOBJECT_SOURCES_DIR}/object_access_control.c    \
            ${APPOBJECT_SOURCES_DIR}/object_barometer.c         \
            ${APPOBJECT_SOURCES_DIR}/object_connectivity_moni.c \
            ${APPOBJECT_SOURCES_DIR}/object_connectivity_stat.c \
            ${APPOBJECT_SOURCES_DIR}/object_device.c            \
            ${APPOBJECT_SOURCES_DIR}/object_firmware.c          \
            ${APPOBJECT_SOURCES_DIR}/object_location.c          \
            ${APPOBJECT_SOURCES_DIR}/object_security.c          \
            ${APPOBJECT_SOURCES_DIR}/object_server.c            \
            ${APPOBJECT_SOURCES_DIR}/object_cellular_conn.c     \
            ${APPOBJECT_SOURCES_DIR}/object_watermeter.c


###################################################
# include path
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/lwm2m/core
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/er-coap-13


