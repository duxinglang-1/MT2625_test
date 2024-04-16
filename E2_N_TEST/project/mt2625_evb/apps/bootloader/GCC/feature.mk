IC_CONFIG                   = mt2625
BOARD_CONFIG                = mt2625_evb
MTK_BL_LOAD_ENABLE          = y

#can modify
MTK_BL_FOTA_CUST_ENABLE     = n
MTK_BL_DEBUG_ENABLE         = n
MTK_FOTA_ENABLE             = y
MTK_FOTA_FS_ENABLE          = n


#internal use
MTK_BL_FPGA_LOAD_ENABLE     = n

#factory
MTK_CAL_DCXO_CAPID          = n
# DCXO calibration value is in SW
MTK_BL_DCXO_KVALUE_SW       = n

MTK_SECURE_BOOT_ENABLE      = n
MTK_BOOTLOADER_USE_MBEDTLS  = n
MTK_ANTI_CLONE_ENABLE       = n
#redstone FOTA support
RS_FOTA_SUPPORT = n

#For Gmobi DIFF FOTA
ROCK_FOTA_SUPPORT = n
