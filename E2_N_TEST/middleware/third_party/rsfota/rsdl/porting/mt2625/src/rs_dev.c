#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "stdlib.h"
#include "rs_datatype.h"
#include "rs_dev.h"
#include "rs_mem.h"
#include "rs_error.h"
#include "rs_flash_operate.h"


#include "FreeRTOS.h"
#include "memory_map.h"
#include "hal_flash.h"
#include "string.h"
#include "hal_wdt.h"
#include "timers.h"
#include "tel_conn_mgr_common.h"
#include "tel_conn_mgr_app_api.h"
#include "stdlib.h"
#include "stdio.h"
#include "mbedtls/md5.h"

// 6531相关头文件
//#include "at.h"


rs_s32 rs_device_init()
{
	rs_s32 errCode = RS_ERR_OK;

	errCode = rs_flash_init();

	return errCode;
}




