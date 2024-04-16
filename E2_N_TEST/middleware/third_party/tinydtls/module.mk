
###################################################
# Sources
TINYDTLS_SOURCES_DIR = middleware/third_party/tinydtls
    			  
C_FILES  +=	${TINYDTLS_SOURCES_DIR}/dtls.c           \
   				  ${TINYDTLS_SOURCES_DIR}/crypto.c         \
   				  ${TINYDTLS_SOURCES_DIR}/ccm.c            \
   				  ${TINYDTLS_SOURCES_DIR}/hmac.c           \
   				  ${TINYDTLS_SOURCES_DIR}/dtls_debug.c     \
   				  ${TINYDTLS_SOURCES_DIR}/netq.c           \
   				  ${TINYDTLS_SOURCES_DIR}/peer.c           \
   				  ${TINYDTLS_SOURCES_DIR}/dtls_time.c      \
   				  ${TINYDTLS_SOURCES_DIR}/session.c        \
   				  ${TINYDTLS_SOURCES_DIR}/sha2/sha2.c      \
   				  ${TINYDTLS_SOURCES_DIR}/aes/rijndael.c   \
   				  ${TINYDTLS_SOURCES_DIR}/ecc/ecc.c

###################################################
# include path
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/tinydtls
