ifeq ($(MTK_SENSOR_BIO_USE),MT6381)
###################################################

VSM_DRIVER_SRC = driver/board/component/biosensor/mt6381
VSM_DRIVER_FILES = $(VSM_DRIVER_SRC)/src/vsm_driver.c \
                   $(VSM_DRIVER_SRC)/src/vsm_platform_function.c \
                   $(VSM_DRIVER_SRC)/src/ppg_control_lib.c

ifeq ($(MTK_SENSOR_BIO_SUBSYS),y)
C_FILES += $(VSM_DRIVER_SRC)/src/vsm_sensor_subsys_adaptor.c
endif

C_FILES += $(VSM_DRIVER_FILES)

CFLAGS	+= -DMTK_SENSOR_BIO_USE_MT6381

ifeq ($(MTK_SENSOR_BIO_INTERFACE),SPI)
VSM_DRIVER_FILES += $(VSM_DRIVER_SRC)/src/vsm_spi_operation.c
CFLAGS	+= -DMT6381_USE_SPI
else
VSM_DRIVER_FILES += $(VSM_DRIVER_SRC)/src/vsm_i2c_operation.c
CFLAGS	+= -DMT6381_USE_I2C
endif

###################################################
# include path
CFLAGS += -I$(SOURCE_DIR)/$(VSM_DRIVER_SRC)/inc
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/sensor_subsys/inc
CFLAGS += -I$(SOURCE_DIR)/$(VSM_DRIVER_SRC)/module/default

ifeq ($(MTK_SENSOR_BIO_USE_IRQ),y)
CFLAGS	+= -DMT6381_USE_IRQ
endif

endif
