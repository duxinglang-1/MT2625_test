
MT2625_CHIP = driver/chip/mt2625
COMPONENT = driver/board/component

CFLAGS   += $(FPUFLAGS) -DUSE_HAL_DRIVER -DPRODUCT_VERSION=2625

C_FILES  += $(MT2625_CHIP)/src/hal_adc.c
C_FILES  += $(MT2625_CHIP)/src/hal_cache.c
C_FILES  += $(MT2625_CHIP)/src/hal_cache_internal.c
C_FILES  += $(MT2625_CHIP)/src/hal_mpu.c
C_FILES  += $(MT2625_CHIP)/src/hal_mpu_internal.c
C_FILES  += $(MT2625_CHIP)/src/hal_emi.c
C_FILES  += $(MT2625_CHIP)/src/hal_nvic.c
C_FILES  += $(MT2625_CHIP)/src/hal_nvic_internal.c
C_FILES  += $(MT2625_CHIP)/src/hal_flash_mtd_common.c
C_FILES  += $(MT2625_CHIP)/src/hal_flash_mtd_dal.c
C_FILES  += $(MT2625_CHIP)/src/hal_flash_custom_sfi.c
C_FILES  += $(MT2625_CHIP)/src/hal_flash_combo_init.c
C_FILES  += $(MT2625_CHIP)/src/hal_flash_cfi.c
C_FILES  += $(MT2625_CHIP)/src/hal_flash_custom.c
C_FILES  += $(MT2625_CHIP)/src/hal_flash_combo_nor.c
C_FILES  += $(MT2625_CHIP)/src/hal_flash_disk.c
C_FILES  += $(MT2625_CHIP)/src/hal_flash_mtd.c
C_FILES  += $(MT2625_CHIP)/src/hal_gdma.c
C_FILES  += $(MT2625_CHIP)/src/hal_gdma_internal.c
C_FILES  += $(MT2625_CHIP)/src/hal_pdma_internal.c
C_FILES  += $(MT2625_CHIP)/src/hal_gpt.c
C_FILES  += $(MT2625_CHIP)/src/hal_gpt_internal.c
C_FILES  += $(MT2625_CHIP)/src/hal_keypad.c
C_FILES  += $(MT2625_CHIP)/src/hal_keypad_internal.c
C_FILES  += $(MT2625_CHIP)/src/hal_keypad_powerkey_internal.c
C_FILES  += $(MT2625_CHIP)/src/hal_log.c
C_FILES  += $(MT2625_CHIP)/src/hal_nvic.c
C_FILES  += $(MT2625_CHIP)/src/hal_nvic_internal.c
C_FILES  += $(MT2625_CHIP)/src/hal_uart.c
C_FILES  += $(MT2625_CHIP)/src/hal_uart_internal.c
C_FILES  += $(MT2625_CHIP)/src/hal_i2c_master.c
C_FILES  += $(MT2625_CHIP)/src/hal_i2c_master_internal.c
C_FILES  += $(MT2625_CHIP)/src/hal_weak.c
C_FILES  += $(MT2625_CHIP)/src/hal_eint.c
C_FILES  += $(MT2625_CHIP)/src/hal_eint_internal.c
C_FILES  += $(MT2625_CHIP)/src/hal_clock.c
C_FILES  += $(MT2625_CHIP)/src/hal_dcxo.c
C_FILES  += $(MT2625_CHIP)/src/hal_dvfs.c
C_FILES  += $(MT2625_CHIP)/src/hal_dvfs_internal.c
C_FILES  += $(MT2625_CHIP)/src/hal_dwt.c
C_FILES  += $(MT2625_CHIP)/src/hal_rtc.c
C_FILES  += $(MT2625_CHIP)/src/hal_rtc_internal.c
C_FILES  += $(MT2625_CHIP)/src/hal_wdt.c
C_FILES  += $(MT2625_CHIP)/src/hal_wdt_internal.c
C_FILES  += $(MT2625_CHIP)/src/hal_pwm.c
C_FILES  += $(MT2625_CHIP)/src/hal_trng_internal.c
C_FILES  += $(MT2625_CHIP)/src/hal_trng.c
C_FILES  += $(MT2625_CHIP)/src/hal_msdc.c
C_FILES  += $(MT2625_CHIP)/src/hal_sd.c
C_FILES  += $(MT2625_CHIP)/src/hal_sd_internal.c
C_FILES  += $(MT2625_CHIP)/src/hal_sdio.c
C_FILES  += $(MT2625_CHIP)/src/hal_sdio_internal.c
C_FILES  += $(MT2625_CHIP)/src/hal_sdio_slave.c
C_FILES  += $(MT2625_CHIP)/src/hal_sdio_slave_internal.c
C_FILES  += $(MT2625_CHIP)/src/hal_sensor_ctrl.c
C_FILES  += $(MT2625_CHIP)/src/hal_gpio.c
C_FILES  += $(MT2625_CHIP)/src/hal_gpio_internal.c
C_FILES  += $(MT2625_CHIP)/src/hal_i2s.c
C_FILES  += $(MT2625_CHIP)/src/hal_i2s_internal.c
C_FILES  += $(MT2625_CHIP)/src/hal_pmu.c
C_FILES  += $(MT2625_CHIP)/src/hal_time_check.c
C_FILES  += $(MT2625_CHIP)/src/hal_usb.c
C_FILES  += $(MT2625_CHIP)/src/hal_usb_phy.c
C_FILES  += $(MT2625_CHIP)/src/hal_aes.c
C_FILES  += $(MT2625_CHIP)/src/hal_des.c
C_FILES  += $(MT2625_CHIP)/src/hal_md5.c
C_FILES  += $(MT2625_CHIP)/src/hal_sha.c
C_FILES  += $(MT2625_CHIP)/src/hal_spi_master.c
C_FILES  += $(MT2625_CHIP)/src/hal_spi_master_internal.c
C_FILES  += $(MT2625_CHIP)/src/hal_spi_slave.c
C_FILES  += $(MT2625_CHIP)/src/hal_spi_slave_internal.c
C_FILES  += $(MT2625_CHIP)/src/hal_misc.c
#################################################################################
#include path
CFLAGS  += -I../inc
CFLAGS  += -Isrc/common/include
CFLAGS  += -Isrc/sdio_gen3/include
CFLAGS  += -Iinc
CFLAGS  += -I$(SOURCE_DIR)/driver/chip/inc
CFLAGS  += -I$(SOURCE_DIR)/driver/chip/mt2625/inc
CFLAGS 	+= -I$(SOURCE_DIR)/kernel/service/inc
CFLAGS  += -I$(SOURCE_DIR)/driver/board/component/lcm/ST7789H2
CFLAGS += -I$(SOURCE_DIR)/driver/CMSIS/Device/MTK/mt2625/Include
CFLAGS += -I$(SOURCE_DIR)/driver/CMSIS/Include
CFLAGS += -I$(SOURCE_DIR)/driver/board/component/lcm/ST7789H2
CFLAGS += -I$(SOURCE_DIR)/driver/board/mt25x3_hdk/lcd
CFLAGS += -I$(SOURCE_DIR)/driver/board/component/common
CFLAGS += -I$(SOURCE_DIR)/$(MT2625_CHIP)/src/common/include

ifeq ($(MTK_DCXO_CAPID_LOAD_FROM_EFUSE), y)
    CFLAGS += -DDCXO_CAPID_LOAD_FROM_EFUSE
endif

ifeq ($(MTK_DCXO_CAPID_CAL), y)
    CFLAGS += -DDCXO_CAPID_CAL
    ifeq ($(MTK_DCXO_CAPID_STORE_TO_EFUSE), 0)
        CFLAGS += -DDCXO_CAPID_STORE_TO_EFUSE0
    else ifeq ($(MTK_DCXO_CAPID_STORE_TO_EFUSE), 1)
        CFLAGS += -DDCXO_CAPID_STORE_TO_EFUSE1
    else ifeq ($(MTK_DCXO_CAPID_STORE_TO_EFUSE), 2)
        CFLAGS += -DDCXO_CAPID_STORE_TO_EFUSE2
    endif
endif

#ifeq ($(MTK_EXTERNAL_PMIC), y)
#	CFLAGS += -I$(SOURCE_DIR)/driver/board/component/pmic/
#endif


ifneq ($(wildcard $(strip $(SOURCE_DIR))/driver/chip/mt2625/src_core/),)
include $(SOURCE_DIR)/driver/chip/mt2625/src_core/GCC/module.mk
else
LIBS += $(SOURCE_DIR)/prebuilt/driver/chip/mt2625/lib/libhal_core_CM4_GCC.a
endif

ifneq ($(wildcard $(strip $(SOURCE_DIR))/driver/chip/mt2625/src_protected/),)
include $(SOURCE_DIR)/driver/chip/mt2625/src_protected/GCC/module.mk
else
LIBS += $(SOURCE_DIR)/prebuilt/driver/chip/mt2625/lib/libhal_protected_CM4_GCC.a
endif

#LIBS += $(SOURCE_DIR)/driver/chip/mt2625/lib/libcryptos_CM4_GCC.a
