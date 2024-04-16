PRODUCT_VERSION=2625

# Default common setting
MTK_BUILD_SMT_LOAD ?= n
MTK_BOOTLOADER_USE_MBEDTLS                  ?= n

AR      = $(BINPATH)/arm-none-eabi-ar
CC      = $(BINPATH)/arm-none-eabi-gcc
CXX     = $(BINPATH)/arm-none-eabi-g++
OBJCOPY = $(BINPATH)/arm-none-eabi-objcopy
SIZE    = $(BINPATH)/arm-none-eabi-size
OBJDUMP = $(BINPATH)/arm-none-eabi-objdump


ALLFLAGS = -mlittle-endian -mthumb -mcpu=cortex-m4
FPUFLAGS = -fsingle-precision-constant -Wdouble-promotion -mfpu=fpv4-sp-d16 -mfloat-abi=hard

#CFLAGS += $(ALLFLAGS) $(FPUFLAGS) -flto -ffunction-sections -fdata-sections -fno-builtin
CFLAGS += $(ALLFLAGS) $(FPUFLAGS) -ffunction-sections -fdata-sections -fno-builtin
CFLAGS += -gdwarf-2 -Os -fno-strict-aliasing -fno-common
CFLAGS += -Wimplicit-function-declaration -Wall -Werror=uninitialized -Wno-error=maybe-uninitialized -Werror=return-type
CFLAGS += -std=gnu99 -Wno-switch
CFLAGS += -DPCFG_OS=2 -D_REENT_SMALL
CFLAGS += -DPRODUCT_VERSION=$(PRODUCT_VERSION)

##
## MTK_SWLA_ENABLE
## Brief:       This option is to enable and disable the Software Logical Analyzer service, Each event(task/isr activity) is recorded while CPU context switching, also support customization tag
## Usage:     If the value is "y", the MTK_SWLA_ENABLE compile option will be defined. You must also include the gva\kernel\service\module.mk in your Makefile before setting the option to "y".
## Path:       kernel/service
## Dependency:  None
## Notice:      None
## Relative doc:None
##
ifeq ($(MTK_SWLA_ENABLE),y)
CFLAGS += -DMTK_SWLA_ENABLE
endif

##
## MTK_SENSOR_ACCELEROMETER_USE
## Brief:        This option is to enable and select accelerometer driver under driver/board/component/mems folder.
## Usage:        If the value is in the supported list (e.g. BMA255), the MTK_SENSOR_ACCELEROMETER_USE and correspoding selection (e.g. MTK_SENSOR_ACCELEROMETER_USE_BMA255) compile options will be defined,
##               and the corresponding sources and header files under driver/board/component/mems will be included by driver/board/component/mems/module.mk.
## Path:         driver/board/component/mems
## Dependency:   Must enable HAL_SPI_MASTER_MODULE_ENABLED, HAL_I2C_MASTER_MODULE_ENABLED (dependent on the sensor hardware connection), and HAL_EINT_MODULE_ENABLED
##               in the hal_feature_config.h under the project inc folder.
## Notice:       None
## Relative doc: None
##
ifeq ($(MTK_SENSOR_ACCELEROMETER_USE),BMA255)
  CFLAGS += -DMTK_SENSOR_ACCELEROMETER_USE
  CFLAGS += -DMTK_SENSOR_ACCELEROMETER_USE_BMA255
endif

ifeq ($(MTK_SENSOR_ACCELEROMETER_USE),BMI160)
  CFLAGS += -DMTK_SENSOR_ACCELEROMETER_GYROSCOPE_USE
  CFLAGS += -DMTK_SENSOR_ACCELEROMETER_GYROSCOPE_USE_BMI160
ifeq ($(MTK_SENSOR_USE_BMI160_BUS),I2C)
  CFLAGS += -DMTK_SENSOR_USE_BMI160_BUS_I2C
endif
ifeq ($(MTK_SENSOR_USE_BMI160_BUS),SPI)
  CFLAGS += -DMTK_SENSOR_USE_BMI160_BUS_SPI
endif
endif

ifeq ($(MTK_SENSOR_ACCELEROMETER_USE),ICM30630)
  CFLAGS += -DMTK_SENSOR_ACCELEROMETER_GYROSCOPE_USE
  CFLAGS += -DMTK_SENSOR_ACCELEROMETER_GYROSCOPE_USE_ICM30630
endif

##
## MTK_SENSOR_MAGNETIC_USE
## Brief:        This option is to enable and select magnetic field sensor driver under driver/board/component/mems folder.
## Usage:        If the value is in the supported list (e.g. YAS33), the MTK_SENSOR_MAGNETIC_USE and correspoding selection (e.g. MTK_SENSOR_MAGNETIC_USE_YAS533) compile options will be defined,
##               and the corresponding sources and header files under driver/board/component/mems will be included by driver/board/component/mems/module.mk.
## Path:         driver/board/component/mems
## Dependency:   Must enable HAL_SPI_MASTER_MODULE_ENABLED, HAL_I2C_MASTER_MODULE_ENABLED (dependent on the sensor hardware connection), and HAL_EINT_MODULE_ENABLED
##               in the hal_feature_config.h under the project inc folder.
## Notice:       None
## Relative doc: None
##
ifeq ($(MTK_SENSOR_MAGNETIC_USE),YAS533)
  CFLAGS += -DMTK_SENSOR_MAGNETIC_USE
  CFLAGS += -DMTK_SENSOR_MAGNETIC_USE_YAS533
endif

##
## MTK_SENSOR_BAROMETER_USE
## Brief:        This option is to enable and select barometric sensor driver under driver/board/component/mems folder.
## Usage:        If the value is in the supported list (e.g. BMP280), the MTK_SENSOR_BAROMETER_USE and correspoding selection (e.g. MTK_SENSOR_BAROMETER_USE_BMP280) compile options will be defined,
##               and the corresponding sources and header files under driver/board/component/mems will be included by driver/board/component/mems/module.mk.
## Path:         driver/board/component/mems
## Dependency:   Must enable HAL_SPI_MASTER_MODULE_ENABLED, HAL_I2C_MASTER_MODULE_ENABLED (dependent on the sensor hardware connection), and HAL_EINT_MODULE_ENABLED
##               in the hal_feature_config.h under the project inc folder.
## Notice:       None
## Relative doc: None
##
ifeq ($(MTK_SENSOR_BAROMETER_USE),BMP280)
  CFLAGS += -DMTK_SENSOR_BAROMETER_USE
  CFLAGS += -DMTK_SENSOR_BAROMETER_USE_BMP280
endif

##
## FUSION_SENSIO_USE
## Brief:        This option is to enable Mediatek biosensor fusion algorithm library under middleware/MTK/sensor_subsys/lib/fusion_algo folder.
## Usage:        If the value is MT6381, the FUSION_SENSIO_USE compile option will be defined as MT6381,
##               and the corresponding library and header files under middleware/MTK/sensor_subsys/lib/fusion_algo will be included by middleware/MTK/sensor_subsys/src/module.mk.
## Path:         middleware/MTK/sensor_subsys/lib/fusion_algo
## Dependency:   Must enable MTK_BIOSENSOR_USE in the project.
## Notice:       None
## Relative doc: None
##
ifneq ($(FUSION_SENSIO_USE),)
  CFLAGS += -DFUSION_SENSIO_USE=$(FUSION_SENSIO_USE)
endif

##
## MTK_DEBUG_LEVEL
## Brief:       This option is to configure system log debug level.
## Usage:       The valid values are empty, error, warning, info, debug, and none.
##              The setting will determine whether a debug log will be compiled.
##              However, the setting has no effect on the prebuilt library.
##              empty   : All debug logs are compiled.
##              error   : Only error logs are compiled.
##              warning : Only warning and error logs are compiled.
##              info    : Only info, warning, and error logs are compiled.
##              debug   : All debug logs are compiled.
##              none    : All debugs are disabled.
## Path:        kernel/service
## Dependency:  None
## Notice:      None
## Realted doc: Please refer to doc/LinkIt_for_RTOS_System_Log_Developers_Guide.pdf
##
ifeq ($(MTK_DEBUG_LEVEL),)
  CFLAGS += -DMTK_DEBUG_LEVEL_DEBUG
  CFLAGS += -DMTK_DEBUG_LEVEL_INFO
  CFLAGS += -DMTK_DEBUG_LEVEL_WARNING
  CFLAGS += -DMTK_DEBUG_LEVEL_ERROR
endif

ifeq ($(MTK_DEBUG_LEVEL),error)
  CFLAGS += -DMTK_DEBUG_LEVEL_ERROR
endif

ifeq ($(MTK_DEBUG_LEVEL),warning)
  CFLAGS += -DMTK_DEBUG_LEVEL_WARNING
  CFLAGS += -DMTK_DEBUG_LEVEL_ERROR
endif

ifeq ($(MTK_DEBUG_LEVEL),info)
  CFLAGS += -DMTK_DEBUG_LEVEL_INFO
  CFLAGS += -DMTK_DEBUG_LEVEL_WARNING
  CFLAGS += -DMTK_DEBUG_LEVEL_ERROR
endif

ifeq ($(MTK_DEBUG_LEVEL),debug)
  CFLAGS += -DMTK_DEBUG_LEVEL_DEBUG
  CFLAGS += -DMTK_DEBUG_LEVEL_INFO
  CFLAGS += -DMTK_DEBUG_LEVEL_WARNING
  CFLAGS += -DMTK_DEBUG_LEVEL_ERROR
endif

ifeq ($(MTK_DEBUG_LEVEL),none)
  CFLAGS += -DMTK_DEBUG_LEVEL_NONE
endif

ifeq ($(MTK_BT_A2DP_AAC_ENABLE),y)
CFLAGS         += -DMTK_BT_A2DP_AAC_ENABLE
endif

##
## MTK_NVDM_MODEM_ENABLE
## Brief:       This option is to enable nvdm modem.
## Usage:       Enable the feature by configuring it as y.
## Path:        None
## Dependency:  MTK_NVDM_MODEM_ENABLE
## Notice:      None
## Related doc :None
##
ifeq ($(MTK_NVDM_MODEM_ENABLE),y)
CFLAGS         += -DMTK_NVDM_MODEM_ENABLE
ifeq ($(MTK_NB_MODEM_ENABLE),y)
CFLAGS         += -DMTK_NVDM_OTA_SUPPORT
endif
endif

##
## MTK_BUILD_SMT_LOAD
## Brief:       This option is to indicate building SMT load.
## Usage:       Enable the feature by configuring it as y.
## Path:        None
## Dependency:  None
## Notice:      None
## Relative doc:None
##
ifeq ($(MTK_BUILD_SMT_LOAD),y)
CFLAGS         += -DMTK_BUILD_SMT_LOAD
CFLAGS         += -DMTK_USIMSMT_ENABLED
endif

##
## MTK_USB_DEMO_ENABLED
## Brief:       This option is to enable and disable USB module.
## Usage:       If the value is "y", the MTK_USB_DEMO_ENABLED compile option will be defined. You must include following module in your Makefile before setting.
##              the option to "y"
##              include the /middleware/MTK/usb/module.mk
## Path:        middleware/MTK/usb
## Dependency:  HAL_USB_MODULE_ENABLED must also defined in hal_feature_config.h under project inc folder.
##
ifeq ($(MTK_USB_DEMO_ENABLED),y)
CFLAGS         += -DMTK_USB_DEMO_ENABLED
endif

##
## MTK_USB_BOOT_ACM
## Brief:       This option is to enable and disable USB CDC-ACM class in booting.
## Path:        middleware/MTK/usb
## Dependency:  Must enable HAL_USB_MODULE_ENABLED, , and MTK_USB_DEMO_ENABLED
##
ifeq ($(MTK_USB_BOOT_ACM),y)
CFLAGS         += -DMTK_USB_BOOT_ACM
endif

##
## MTK_USB_BOOT_MSC
## Brief:       This option is to enable and disable USB MASS-STORAGE class in booting.
## Path:        middleware/MTK/usb
## Dependency:  Must enable HAL_USB_MODULE_ENABLED, , and MTK_USB_DEMO_ENABLED
##
ifeq ($(MTK_USB_BOOT_MSC),y)
CFLAGS         += -DMTK_USB_BOOT_MSC
endif

##
## MTK_PORT_SERVICE_ENABLE
## Brief:       This option is to enable port service feature.
## Usage:       Enable the feature by configuring it as y.
## Path:        middleware/MTK/port_service
## Dependency:  NVDM module must be enabled.
## Notice:      None
## Relative doc:None
##
ifeq ($(MTK_PORT_SERVICE_ENABLE),y)
  CFLAGS += -DMTK_PORT_SERVICE_ENABLE
endif

##
## MTK_PORT_SERVICE_CONFIGURABLE_BUFFER
## Brief:       This option is to enable port service feature with VDMA settings
## Usage:       Enable the feature by configuring it as y.
## Path:        middleware/MTK/port_service
## Dependency:  Must enable the MTK_PORT_SERVICE_ENABLE for port service feature
## Notice:      None
## Relative doc:None
##
ifeq ($(MTK_PORT_SERVICE_ENABLE),y)
ifeq ($(MTK_PORT_SERVICE_CONFIGURABLE_BUFFER),y)
CFLAGS += -DUSE_CONFIG_BUFFER
endif
endif

##
## MTK_ATCI_VIA_PORT_SERVICE
## Brief:       This option is to enable ATCI through port service feature for data transmission in mt2523.
## Usage:       Enable the feature by configuring it as y.
## Path:        middleware/MTK/atci/src/, middleware/MTK/atci/inc/
## Dependency:  Must enable the MTK_PORT_SERVICE_ENABLE for port service feature
## Notice:      The default implementation of ATCI for data transmission is using HAL UART directly.
## Relative doc:None
##
ifeq ($(MTK_PORT_SERVICE_ENABLE),y)
ifeq ($(MTK_ATCI_VIA_PORT_SERVICE),y)
  CFLAGS += -DMTK_ATCI_VIA_PORT_SERVICE
endif
endif

##
## MTK_MINI_DUMP_ENABLE
## Brief:       This option is to enable mini dump feature that saves crash info in modem nvdm.
## Usage:       Enable the feature by configuring it as y.
## Path:        kernel/service/src
## Dependency:  MTK_NVDM_MODEM_ENABLE
## Notice:
## Related doc :None
##
ifeq ($(MTK_MINI_DUMP_ENABLE),y)
CFLAGS         += -DMTK_MINI_DUMP_ENABLE
endif

##
## MTK_AT_CMD_DISABLE
## Brief:       This option is to disable the default AT command under middleware/MTK/atci/at_command/
## Usage:       Enable the feature by configuring it as y.
## Path:        middleware/MTK/atci/at_command/, middleware/MTK/atci/inc/
## Dependency:  None.
## Notice:      None.
## Relative doc:None
##

ifeq ($(MTK_AT_CMD_DISABLE),y)
CFLAGS         += -DMTK_AT_CMD_DISABLE
endif

##
## MTK_OS_CPU_UTILIZATION_ENABLE
## Brief:       This option is to enable and disable cpu utilization function.
## Usage:     If the value is "y", the MTK_OS_CPU_UTILIZATION_ENABLE compile option will be defined and supports the 'os cpu' MTK CLI commands to show/get statistics of CM4 CPU utilizations of all the tasks running on.You must also include the \kernel\service\module.mk in your Makefile before setting the option to "y".
## Path:       kernel/service
## Dependency:  MTK_MINICLI_ENABLE must be enabled in this file.
## Notice:      None
## Relative doc:None
##
ifeq ($(MTK_OS_CPU_UTILIZATION_ENABLE),y)
CFLAGS += -DMTK_OS_CPU_UTILIZATION_ENABLE
endif

##
## MTK_SYSTEM_AT_COMMAND_ENABLE
## Brief:       This option is to enable and disable at commands for system service, include show task information, heap infomation, and force system crash to get memory dump
## Usage:     If the value is "y", the MTK_SYSTEM_AT_COMMAND_ENABLE compile option will be defined. You must also include the \kernel\service\module.mk in your Makefile before setting the option to "y".
## Path:       kernel/service
## Dependency:  None
## Notice:      None
## Relative doc:None
##
ifeq ($(MTK_SYSTEM_AT_COMMAND_ENABLE),y)
CFLAGS += -DMTK_SYSTEM_AT_COMMAND_ENABLE
endif

##
## MTK_MBEDTLS_CONFIG_FILE
## Brief:       This option is to configure mbedTLS features.
## Usage:       If the value is "*.h", mbedTLS module will use *.h as the configuration file. For example, if its value is "config-mtk-basic.h",
##              config-mtk-basic.h will be used as the configuration file. MTK_MBEDTLS_CONFIG_FILE compile option will be defined. You must also
##              include the gva/middleware/third_party/mbedtls/module.mk in your Makefile before setting the option to "*.h".
## Path:        middleware/third_party/mbedtls
## Dependency:  LWIP module must be enabled.
## Related doc: Please refer to internet and open source software guide under the doc folder for more detail.
##
ifneq ($(MTK_MBEDTLS_CONFIG_FILE),)
CFLAGS         += -DMBEDTLS_CONFIG_FILE=\"$(MTK_MBEDTLS_CONFIG_FILE)\"
endif

##
## MTK_BOOTLOADER_USE_MBEDTLS
## Brief:       This option is to enable using mbedtls in bootloader.
## Usage:       Enable the feature by configuring it as y.
## Path:        middleware/third_party/mbedtls
## Dependency:  libmbedtls
## Notice:      None
## Relative doc:None
##
ifeq ($(MTK_BOOTLOADER_USE_MBEDTLS),y)
  CFLAGS += -DMTK_BOOTLOADER_USE_MBEDTLS
endif


##
## FPGA_ENV
## Brief:       Temporary flag is for FPGA testing purposes with evb loads.
## Usage:       Enable the feature by configuring it as y.
## Path:        None
## Dependency:  None
## Notice:      None
## Relative doc:None
##
ifeq ($(FPGA_ENV), y)
  CFLAGS += -DFPGA_ENV
endif


##
## MT2503_2625_DUAL
## Brief:       Temporary flag is to indicate if it's a dual mode project or not.
## Usage:       Enable the feature by configuring it as y.
## Path:        None
## Dependency:  None
## Notice:      None
## Relative doc:None
##
ifeq ($(MT2503_2625_DUAL), y)
  CFLAGS += -DMT2503_2625_DUAL
endif

ifeq ($(GNSS_HOST_CHIP), MT6261)
  CFLAGS += -D__GNSS_HOST_CHIP_MT6261__
endif
ifeq ($(GNSS_HOST_CHIP), MT2625)
  CFLAGS += -D__GNSS_HOST_CHIP_MT2625__
endif

ifeq ($(NBIOT_SUPPORT), MT2621_P1)
  CFLAGS += -D__NB_IOT_GSM_DMSS__
endif
ifeq ($(NBIOT_SUPPORT), MT2621_P2)
  CFLAGS += -D__NB_IOT_GSM_DMSS_ENH__
endif
ifeq ($(NBIOT_SUPPORT), MT2621_P3)
  CFLAGS += -D__NB_IOT_GSM_DMDS__
  CFLAGS += -D__NB_IOT_GSM_DMDS_POWER_ENH__
endif

#Include Path
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/mbedtls/include
CFLAGS += -I$(SOURCE_DIR)/middleware/third_party/mbedtls/configs

#Middleware Module Path
MID_MBEDTLS_PATH    = $(SOURCE_DIR)/middleware/third_party/mbedtls

## MTK_GNSS_ENABLE
## Brief:       This option is to enable and disable GNSS.
## Usage:       To enable GNSS support in a project, use "include $(SOURCE_DIR)/middleware/MTK/gnss/module.mk" to include the middleware.
##              Additionally, switch on this option to make build script to copy GNSS firmware image.
## Path:        middleware/MTK/gnss
## Dependency:  Add "include $(SOURCE_DIR)/middleware/MTK/gnss/module.mk" in the Makefile to link GNSS middleware.
## Notice:      Keep the copy_firmware.sh statement in Makefile to copy GNSS firmware image.
## Relative doc:Find the GNSS section in API reference.
##
ifeq ($(MTK_GNSS_ENABLE),y)
CFLAGS         += -DMTK_GNSS_ENABLE
export MTK_GNSS_ENABLE=y
endif

## MTK_BT_ENABLED
## Brief:       This option is to enable and disable BT.
## Usage:       To enable BT support in a project, use "include $(SOURCE_DIR)/middleware/MTK/bluetooth/module.mk" to include the middleware.
## Path:        middleware/MTK/bluetooth
## Dependency:  Add "include $(SOURCE_DIR)/middleware/MTK/bluetooth/module.mk" in the Makefile to link Bluetooth middleware.
## Notice:      
## Relative doc:Find the Bluetooth section in API reference.
##
ifeq ($(MTK_BT_ENABLE),y)
CFLAGS         += -DMTK_BT_ENABLED
endif

## MTK_WIFI_ENABLE
## Brief:       This option is to enable and disable WIFI feature.
## Usage:       To enable WIFI support in a project, use "include $(SOURCE_DIR)/middleware/MTK/wifi_host/module.mk" to include the middleware.
##              Additionally, switch on this option to make build script to copy WIFI firmware image.
## Path:        middleware/MTK/wifi_host
## Dependency:  Add "include $(SOURCE_DIR)/middleware/MTK/wifi_host/module.mk" in the Makefile to link WIFI middleware.
## Notice:      Keep the copy_firmware.sh statement in Makefile to copy WIFI firmware image.
## Relative doc:Find document in wifi_host_api.h.
##
ifeq ($(MTK_WIFI_ENABLE),y)
CFLAGS         += -DMTK_WIFI_ENABLE
export MTK_WIFI_ENABLE=y

ifeq ($(MTK_WIFI_STUB_CONF_SPI_ENABLE),y)
CFLAGS         += -DMTK_WIFI_STUB_CONF_SPI_ENABLE
endif

ifeq ($(MTK_WIFI_CHIP_USE_MT5932),y)
CFLAGS         += -DMTK_WIFI_CHIP_USE_MT5932
endif

ifeq ($(MTK_WIFI_STUB_CONF_SPIM_ENABLE),y)
CFLAGS         += -DMTK_WIFI_STUB_CONF_SPIM_ENABLE
endif

ifeq ($(MTK_WIFI_STUB_CONF_ENABLE),y)
CFLAGS         += -DMTK_WIFI_STUB_CONF_ENABLE
endif

endif

## MTK_SYSTEM_CLOCK_SET
## Brief:       This option is used to set system clock
## Usage:       To set cpu clock in a project config file.
## Path:        None
## Dependency:  MTK_SYSTEM_CLOCK_SET
## Notice:      None
## Relative doc:None
##
ifeq ($(MTK_SYSTEM_CLOCK_SET), 78000)
CFLAGS += -DMTK_SYSTEM_CLOCK_SET=78000
endif

## MTK_CREATE_DEFAULT_APN
## Brief:       This option is to enable the default pdn link from AP to MD.
## Usage:       Enable the feature by configuring it as y.
## Path:        middleware/MTK/tel_conn_mgr/apn/, middleware/MTK/tel_conn_mgr/apn/inc/
## Dependency:  None.
## Notice:      None.
## Relative doc:None
##
ifeq ($(MTK_CREATE_DEFAULT_APN),y)
CFLAGS         += -DMTK_CREATE_DEFAULT_APN
endif

##
## MTK_HTTPCLIENT_SSL_ENABLE
## Brief:       This option is to switch SSL/TLS support in SSL client module.
## Usage:       To enable HTTP client support in a project, use "LIBS += $(OUTPATH)/libhttpclient.a" to include the middleware.
##              Additionally, switch this option to configure SSL support in HTTP client.
## Path:        middleware/third_party/httpclient
## Dependency:  LWIP and mbedTLS module must be enabled.
## Related doc: Please refer to internet and open source software guide under the doc folder for more detail.
##
ifeq ($(MTK_HTTPCLIENT_SSL_ENABLE),y)
CFLAGS += -DMTK_HTTPCLIENT_SSL_ENABLE
endif
#Add common config both for Modem & AP
CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/nbiot_custom/gkiprintf

##
## OPTIMIZED_GKI_LOG_ENABLE
## Brief:       This option is to enable optimized GKI logging
## Usage:     Enable the feature by configuring it as y.
## Path:        middleware/nbiot/xxx
## Dependency:  None
## Related doc: None
##
ifeq ($(OPTIMIZED_GKI_LOG_ENABLE),y)
CFLAGS += -DOPTIMIZED_GKI_LOG_ENABLE
endif

CFLAGS += -DRHINO=1
CFLAGS += -DFreeRTOS=2
ifeq ($(MTK_RTOS_TYPE), RHINO)
CFLAGS += -DMTK_RTOS_TYPE=RHINO
else
CFLAGS += -DMTK_RTOS_TYPE=FreeRTOS
endif