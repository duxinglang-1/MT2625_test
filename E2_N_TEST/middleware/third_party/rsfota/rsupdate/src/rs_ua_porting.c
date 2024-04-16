#include "rsplatform.h"
#include "rs_ua_fs.h"
#include "rs_ua_flash.h"
#include "rs_ua_porting.h"

//#include "memory_map.h"


#include "bl_common.h"
#include "bl_fota.h"
#include "hal_uart.h"
#include "hal_flash.h"
#include "core_cm4.h"
#include "hal_emi.h"
#include "hal_clock_internal.h"
//#include "hal_dcxo.h"
#include "hal_rtc.h"
#include "hal_wdt.h"


#include <stdarg.h>
#include <stdint.h>



// TODO: 配置差分升级所需内存
rs_u8 rs_ua_ram_buffer[RS_FOTA_UA_RAM_BUFFER_SIZE];
// TODO: 配置一个BLOCK大小的内存
rs_u8 rs_ua_block_data[RS_FOTA_UA_FLASH_BLCOK_SIZE];


static rs_u32 g_deltaPartitionAddr = 0;
static rs_u32 s_lastProgress = -1;

#ifndef RS_DIFF_PACKAGE_ON_FLASH

rs_bool rs_ua_writeDataToFile(rs_u8* buffer, rs_s32 bufferSize, rs_u32 destFile);

static rs_s8* g_updateInfo1File = RS_UA_UPDATEINFO1_FILE;
static rs_s8* g_updateInfo2File = RS_UA_UPDATEINFO2_FILE;
static rs_s8* g_deltaFile = RS_UA_DELTA_FILE;
static rs_s8* g_extendFile = RS_UA_EXTEND_FILE;

void rs_ua_setUpdateInfo1File(const rs_s8* file)
{
	g_updateInfo1File = file;
}

void rs_ua_setUpdateInfo2File(const rs_s8* file)
{
	g_updateInfo2File = file;
}

void rs_ua_setDeltaFile(const rs_s8* file)
{
	g_deltaFile = file;
}

void rs_ua_setExtendFile(const rs_s8* file)
{
	g_extendFile = file;
}

#endif


/********************************************************************************/
/**
 * rs_ua_sdk_heap_size
 *
 *
 */
rs_u32 rs_ua_sdk_heap_size()
{
  return RS_FOTA_UA_RAM_BUFFER_SIZE;
}

/**
 * rs_ua_postUpdateProgress
 *
 * 注意：由于HAL_WDT_FEED_MAGIC执行时间比较长，所以放到此函数
 */
void rs_ua_postUpdateProgress(rs_u32 current , rs_u32 total)
{
	if (s_lastProgress != current)
	{
		s_lastProgress = current;
		hal_wdt_feed(HAL_WDT_FEED_MAGIC);
	}

	rs_trace("fota update progress %d/100\n\r", current);
}

/**
 * rs_ua_kick_watchdog
 * 
 */
void rs_ua_kick_watchdog(void)
{
	
	//hal_wdt_feed(HAL_WDT_FEED_MAGIC);
}

rs_u32 rs_ua_sdk_heap_addr()
{
	return (rs_u32 )rs_ua_ram_buffer;
}

rs_u32 rs_ua_ram_block()
{
	return (rs_u32 )rs_ua_block_data;
}

rs_u32 rs_ua_ram_block_size(void)
{
	return RS_FOTA_UA_FLASH_BLCOK_SIZE;
}

void rs_ua_set_deltaPartitionAddr(rs_u32 addr)
{
#ifdef RS_DIFF_PACKAGE_ON_FLASH
	if (addr == 0)
		g_deltaPartitionAddr = RS_UA_DELTA_ADDR;
#endif
}

/*
	主要用于update block size小的情况，也就是升级单元和update info 的单元不一致的情况
	注意：虽然每一个updateinfo 占用了2个RS_FOTA_UA_FLASH_BLCOK_SIZE，但是有效数据小于
	一个RS_FOTA_UA_FLASH_BLCOK_SIZE，所以这里设置为一个RS_FOTA_UA_FLASH_BLCOK_SIZE
*/
rs_u32 rs_ua_getUpdateBlockSize()
{
	return RS_FOTA_UA_FLASH_BLCOK_SIZE;
}

/**
 * rs_ua_getPackageAddr
 * return address of fota diff package on flash
 */
rs_u32 rs_ua_getPackageAddr()
{
#ifdef RS_DIFF_PACKAGE_ON_FLASH
	return g_deltaPartitionAddr + 2*RS_FOTA_UA_UPDATEINFO_BLCOK_SIZE;
#else
	return 0;
#endif
}

/**
 * rs_ua_getUpdateInfo1Addr
 * return address of storing fota update status info, main block
 */
rs_u32 rs_ua_getUpdateInfo1Addr()
{
#ifdef RS_DIFF_PACKAGE_ON_FLASH
	return g_deltaPartitionAddr;
#else
	return 0;
#endif
}

/**
 * rs_ua_getUpdateInfo2Addr
 * return address of storing fota update status info, second block
 */
rs_u32 rs_ua_getUpdateInfo2Addr()
{
#ifdef RS_DIFF_PACKAGE_ON_FLASH
	return g_deltaPartitionAddr + RS_FOTA_UA_UPDATEINFO_BLCOK_SIZE;
#else
	return 0;
#endif
}


rs_u8 rs_ua_getUpdateInfo1File()
{
#ifdef RS_DIFF_PACKAGE_ON_FLASH
	return 0;
#else
	return RS_UA_UPDATEINFO1_FILE_INDEX;
#endif
}

rs_u8 rs_ua_getUpdateInfo2File()
{
#ifdef RS_DIFF_PACKAGE_ON_FLASH
	return 0;
#else
	return RS_UA_UPDATEINFO2_FILE_INDEX;
#endif
}

rs_u8 rs_ua_getOTAPackageFile()
{
#ifdef RS_DIFF_PACKAGE_ON_FLASH
	return 0;
#else
	return RS_UA_DELTA_FILE_INDEX;
#endif
}

rs_s32 rs_ua_openFileRD(rs_u32 fileIndex)
{
#ifndef RS_DIFF_PACKAGE_ON_FLASH
	if (fileIndex == RS_UA_UPDATEINFO1_FILE_INDEX)
		return (rs_s32)rs_ua_fs_open((rs_s8 *)g_updateInfo1File, RS_FS_OPEN_READ);
	else if (fileIndex == RS_UA_UPDATEINFO2_FILE_INDEX)
		return (rs_s32)rs_ua_fs_open((rs_s8 *)g_updateInfo2File, RS_FS_OPEN_READ);
	else if (fileIndex == RS_UA_DELTA_FILE_INDEX)
		return (rs_s32)rs_ua_fs_open((rs_s8 *)g_deltaFile, RS_FS_OPEN_READ);
	else if (fileIndex == RS_UA_EXTEND_FILE_INDEX)
		return (rs_s32)rs_ua_fs_open((rs_s8 *)g_extendFile, RS_FS_OPEN_READ);
#endif
	return RS_FS_INVALID;
}


/*
函数说明：扩展数据从rsua输出完成之后，开始处理这些扩展数据，如果不需要处理则不做任何动作
*/
rs_u32 rs_ua_processExtendFile()
{
	// 这里和之前的区别是处理完成之后，需要自行删除扩展数据，sdk不会删除数据
	//return rs_ua_processGPS();
	return 0;
}

/*
函数说明：rsua分阶段输出扩展数据，适配层把输出的分段数据写入文件 flash 或者内存中
*/
rs_u32 rs_ua_saveExtendDataToFile(rs_u8* buff, rs_u32 size, rs_u32 offset, rs_bool isFinal)
{
	return 0;
}

rs_s32 rs_ua_openFileCreate(rs_u32 fileIndex)
{
#ifndef RS_DIFF_PACKAGE_ON_FLASH
	if (fileIndex == RS_UA_UPDATEINFO1_FILE_INDEX)
		return (rs_s32)rs_ua_fs_open((rs_s8 *)g_updateInfo1File, RS_FS_OPEN_CREATE);
	else if (fileIndex == RS_UA_UPDATEINFO2_FILE_INDEX)
		return (rs_s32)rs_ua_fs_open((rs_s8 *)g_updateInfo2File, RS_FS_OPEN_CREATE);
	else if (fileIndex == RS_UA_DELTA_FILE_INDEX)
		return (rs_s32)rs_ua_fs_open((rs_s8 *)g_deltaFile, RS_FS_OPEN_CREATE);
	else if (fileIndex == RS_UA_EXTEND_FILE_INDEX)
		return (rs_s32)rs_ua_fs_open((rs_s8 *)g_extendFile, RS_FS_OPEN_CREATE);
#endif
	return RS_FS_INVALID;
}

rs_s32 rs_ua_openFileRW(rs_u32 fileIndex)
{
#ifndef RS_DIFF_PACKAGE_ON_FLASH
	if (fileIndex == RS_UA_UPDATEINFO1_FILE_INDEX)
		return (rs_s32)rs_ua_fs_open((rs_s8 *)g_updateInfo1File, RS_FS_OPEN_WRITE);
	else if (fileIndex == RS_UA_UPDATEINFO2_FILE_INDEX)
		return (rs_s32)rs_ua_fs_open((rs_s8 *)g_updateInfo2File, RS_FS_OPEN_WRITE);
	else if (fileIndex == RS_UA_DELTA_FILE_INDEX)
		return (rs_s32)rs_ua_fs_open((rs_s8 *)g_deltaFile, RS_FS_OPEN_WRITE);
	else if (fileIndex == RS_UA_EXTEND_FILE_INDEX)
		return (rs_s32)rs_ua_fs_open((rs_s8 *)g_extendFile, RS_FS_OPEN_WRITE);
#endif
	return RS_FS_INVALID;
}

rs_bool rs_ua_removeFile(rs_u32 fileIndex)
{
#ifndef RS_DIFF_PACKAGE_ON_FLASH
	if (fileIndex == RS_UA_UPDATEINFO1_FILE_INDEX)
		return rs_ua_fs_remove((const rs_s8*)g_updateInfo1File);
	else if (fileIndex == RS_UA_UPDATEINFO2_FILE_INDEX)
		return rs_ua_fs_remove((const rs_s8*)g_updateInfo2File);
	else if (fileIndex == RS_UA_DELTA_FILE_INDEX)
		return rs_ua_fs_remove((const rs_s8*)g_deltaFile);
	else if (fileIndex == RS_UA_EXTEND_FILE_INDEX)
		return rs_ua_fs_remove((const rs_s8*)g_extendFile);
#endif
	return rs_false;
}

rs_u32 rs_ua_getFileSize(rs_u32 fileIndex)
{
#ifndef RS_DIFF_PACKAGE_ON_FLASH
	RS_FILE fileHandle = RS_FS_INVALID;
	rs_u32 filesize = 0;
	
	fileHandle = rs_ua_openFileRD(fileIndex);
	if (fileHandle == RS_FS_INVALID)
		return 0;
	
	filesize = rs_ua_fs_size(fileHandle);
	rs_ua_fs_close(fileHandle);
	
	return filesize;
#else
	return 0;
#endif
}

/**
 * rs_ua_packageStore
 * return value for SDK using
 */
rs_u8 rs_ua_packageStore()
{
#if defined(RS_DIFF_PACKAGE_ON_FLASH)
	return 1;
#else
	return 0;
#endif
}

/**
* 
*/
rs_bool rs_trace(const rs_s8 *format,...)
{
   va_list ap;
   va_start (ap, format);

   bl_print_internal(LOG_INFO, format, ap);

   va_end (ap);

   bl_print(LOG_INFO, "\r"); 

   
   
   return rs_true;
}

int rs_ua_prepareEnv()
{
	rs_s32 ret = 0;

	if (!rs_ua_flash_init())
	{
		//rs_trace("%s, Flash init failed\n\r", __func__);
		return -1;
	}

	return 0;
}

int rs_ua_execute()
{
	e_updateState update_state = UPSTATE_IDLE;
	rs_u32 fotaRet = 1;

	if(rs_ua_prepareEnv() != 0)
		return 1;

	// deltaAddr是从外部输入的差分包存储空间的位置
	// 如果不能从外部获取则输入0
	rs_ua_set_deltaPartitionAddr(0);

	update_state = rs_ua_getUpdateState();
	if (update_state == UPSTATE_INVALID)
		return 1;

	switch(update_state)
	{
		case UPSTATE_CHECK:
			fotaRet = rs_ua_validation(rs_false, 0);
			if(fotaRet == 0)
			{
				if(!rs_ua_updatePhase())
					return 1;
				fotaRet = rs_ua_upgrade(rs_false, 0);
				if (fotaRet == 0)
				{
					if(!rs_ua_updatePhaseComplete())
						return 1;
				}
			}
			break;
			
		case UPSTATE_UPDATE:
			rs_ua_setRecoveryPhase();
			fotaRet = rs_ua_upgrade(rs_false, 0);
			if (fotaRet == 0)
			{
				if(!rs_ua_updatePhaseComplete())
					return 1;
			}
			break;
			
		default:
			break;
	}

	rs_trace("fotaRet = %d\n\r", fotaRet);

	return fotaRet;
}
