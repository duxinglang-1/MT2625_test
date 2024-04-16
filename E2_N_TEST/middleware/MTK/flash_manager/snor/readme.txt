Serial NAND module usage guide

Brief:          This module is the flash disk management implementation specialized for the serial NOR flash device.
Usage:          GCC: Please add "include $(SOURCE_DIR)/middleware/MTK/flash_manager/snor/module.mk" in your GCC project Makefile.
                KEIL: Please drag the prebuilt/middleware/MTK/flash_manager/snor/lib/lib_serial_nand_xxx_KEIL.lib and middleware/MTK/flash_manager/snor/src folder to your project. Please define the macro DMTK_FATFS_ON_SERIAL_NOR_FLASH in KEIL, and add middleware/MTK/flash_manager/snor/inc to include paths.
                IAR: Please drag the prebuilt/middleware/MTK/flash_manger/snor/lib/lib_serial_nand_xxx_IAR.a and middleware/MTK/flash_manager/snor/src folder to your project. Please define the macro DMTK_FATFS_ON_SERIAL_NOR_FLASH in IAR, and add middleware/MTK/flash_manager/snor/inc to include paths.
Dependency:     Please define HAL_GPT_MODULE_ENABLED and HAL_TRNG_MODULE_ENABLED in hal_feature_config.h under the project inc folder.
                Besides OS need also be enabled to use this feature.
                Please should do SPI and FDM initiation by serial_nand_init and spi_nand_fdm_DAL_init before do File System operation, it's better to do FMD initiation at task level.
Notice:         Support all QVL NOR flash device.
Relative doc:   Please refer to the section of serial_nand in Airoha IoT SDK for xxxx API Reference Manual.html under the doc folder for more detail.
Example project:none.
