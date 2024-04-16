#include <stdlib.h>
#include <stdio.h>
#include "rs_datatype.h"
#include "rs_flash_operate.h"
#include "rs_dev.h"
#include "rs_error.h"
#include "rs_checksum.h"
#include "rs_param.h"
#include "rs_debug.h"
#include "rs_state.h"

// 平台的头文件
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


/***************************函数声明*****************************************/

/***************************局部函数*****************************************/


/***************************flash函数实现************************************/
/**
函数说明：flash初始化 
参数说明：无
返回值：成功RS_ERR_OK（值为0），其它为失败
*/
rs_s32 rs_flash_init()
{
	
	 hal_flash_status_t ret;
	 ret = hal_flash_init();
	 if (ret < HAL_FLASH_STATUS_OK){
      RS_PORITNG_LOG( RS_LOG_ERROR "rs_flash_init ERR code %d", ret);
      return RS_ERR_FLASH_INIT;
    }
	return RS_ERR_OK;
}

/**
函数说明：获取当前flash的一个page的大小 
参数说明：无
返回值：成功返回page的size，失败返回-1 
*/
rs_s32 rs_flash_getPageSize()
{
	return RS_PAGE_SIZE;
}

/**
函数说明：获取当前flash的一个DI的page的大小 
参数说明：无
返回值：成功返回page的size，失败返回-1 
*/
rs_s32 rs_flash_getDIPageSize()
{
	return RS_DI_PAGE_SIZE;
}


/**
函数说明：flash初始化 
参数说明：无
返回值：成功RS_ERR_OK（值为0），其它为失败
*/
rs_s32 rs_flash_getBlockSize()
{
	/*
	RDA 里面block size不知道是什么概念，erease的大小为4096，可以理解为是一个block，这个里面我们
	设置的逻辑的block size为32K,主要是为了断电续传能够存储的逻辑
	*/
	return RS_LOGIC_WRITE_UINIT_SIZE;
}

#if (!defined(RS_SUPPORT_UPDATE_INFO_FS) || !defined(RS_SUPPORT_UPDATE_DATA_FS))

/**
函数说明：擦除block 
参数说明：addr需要擦除的block的地址
返回值：成功RS_ERR_OK（值为0），其它为失败
*/
rs_s32 rs_flash_eraseBlock(rs_u32 addr)
{
	
    hal_flash_status_t ret;
	rs_u8 i;
	rs_u32 eraseCount = RS_LOGIC_WRITE_UINIT_SIZE/(MT2625_FLASH_ERASE_BLOCK_SIZE);

	if (addr & (RS_LOGIC_WRITE_UINIT_SIZE - 1))
	{
		RS_PORITNG_LOG( RS_LOG_ERROR "rs_flash_eraseBlock not align\n");
		return RS_ERR_FLASH_WRITE;
	}

	for (i = 0; i < eraseCount; i ++)
	{
		
		ret = hal_flash_erase((addr+i*MT2625_FLASH_ERASE_BLOCK_SIZE), HAL_FLASH_BLOCK_4K);
		if (ret < HAL_FLASH_STATUS_OK)
		{
			RS_PORITNG_LOG( RS_LOG_ERROR "rs_flash_eraseBlock ERR code %d\n", ret);
			return RS_ERR_FLASH_WRITE;
		}
	}
	return RS_ERR_OK;
}


/**
函数说明：按照page读取数据 
参数说明：addr 需要读取的地址
		 buffer 存放要读取数据的缓冲区
		 len 需要写入buffer数据的长度，必须为一个page的长度
返回值：成功RS_ERR_OK（值为0），其它为失败
*/
rs_s32 rs_flash_readPage(rs_u32 addr, rs_u8* buffer, rs_s32 len)
{	
    hal_flash_status_t ret;
	ret = hal_flash_read(addr,buffer,len);
	if (ret < HAL_FLASH_STATUS_OK){
		RS_PORITNG_LOG( RS_LOG_ERROR "rs_flash_readPage  ERR code %d\n", ret);
		return RS_ERR_FLASH_READ;
	}
	return RS_ERR_OK;
}

/**
函数说明：按照DI page读取数据 
参数说明：addr 需要读取的地址
		 buffer 存放要读取数据的缓冲区
		 len 需要写入buffer数据的长度，必须为一个page的长度
返回值：成功RS_ERR_OK（值为0），其它为失败
*/

rs_s32 rs_flash_readDIPage(rs_u32 addr, rs_u8* buffer, rs_s32 len)
{
	hal_flash_status_t ret;
	ret = hal_flash_read(addr,buffer,len);
	if (ret < HAL_FLASH_STATUS_OK){
		RS_PORITNG_LOG( RS_LOG_ERROR "rs_flash_readDIPage  ERR code %d\n", ret);
		return RS_ERR_FLASH_READ;
	}
	return RS_ERR_OK;
	
}


/**
函数说明：按照page写入数据 
参数说明：addr 需要写入的地址
		 Buffer 需要写入的buffer数据
		 Len 需要写入的数据的长度，必须为一个page的长度
返回值：成功RS_ERR_OK（值为0），其它为失败
*/

rs_s32 rs_flash_writePage(rs_u32 addr, const rs_u8* buffer, rs_s32 len)
{
	hal_flash_status_t ret;
	ret = hal_flash_write(addr,buffer,len);
	if (ret < HAL_FLASH_STATUS_OK){
		RS_PORITNG_LOG( RS_LOG_ERROR "rs_flash_writePage  ERR code %d\n", ret);
		return RS_ERR_FLASH_WRITE;
	}
	return RS_ERR_OK;
}


/**
函数说明：按照DI page写入数据 
参数说明：addr 需要写入的地址
		 Buffer 需要写入的buffer数据
		 Len 需要写入的数据的长度，必须为一个page的长度
返回值：成功RS_ERR_OK（值为0），其它为失败
*/

rs_s32 rs_flash_writeDIPage(rs_u32 addr, const rs_u8* buffer, rs_s32 len)
{
	hal_flash_status_t ret;
	ret = hal_flash_write(addr,buffer,len);
	if (ret < HAL_FLASH_STATUS_OK){
		RS_PORITNG_LOG( RS_LOG_ERROR "rs_flash_writeDIPage  ERR code %d", ret);
		return RS_ERR_FLASH_WRITE;
	}
	return RS_ERR_OK;
}




/**
函数说明：按照block读取数据 
参数说明：addr 需要读取的地址,block对齐
		 Buffer 需要读取的buffer数据
		 Len 需要读取的数据的长度，必须为小于block的长度
返回值：成功RS_ERR_OK（值为0），其它为失败
*/
rs_s32 rs_flash_readLessBlock(rs_u32 addr, rs_u8* buffer, rs_s32 len)
{

	rs_u32 j, vSizeCount, vSizeRemain;
	rs_u32 vAddress = addr;
	rs_u8 *p = buffer;
	rs_u8 data[RS_PAGE_SIZE];
	rs_u32 blockSize = rs_flash_getBlockSize();
	rs_u32 pageSize = rs_flash_getPageSize();

	if (buffer == RS_NULL)
	{
		return RS_ERR_INVALID_PARAM;
	}

	if ((vAddress & (blockSize - 1)) != 0)
	{
		RS_PORITNG_LOG(RS_LOG_DEBUG"flash read2, address is not an aligned by BLOCK\n");
		return RS_ERR_INVALID_PARAM;
	}
	
	if (len < 0 || len > blockSize)
	{
		RS_PORITNG_LOG(RS_LOG_DEBUG"flash read2 need read len = %d\n", len);
		return RS_ERR_INVALID_PARAM;		
	}
	
	if (pageSize != RS_PAGE_SIZE)
	{
		return RS_ERR_INVALID_PARAM;		
	}
	
	vSizeCount = (len / pageSize);
	vSizeRemain = (len % pageSize);

	for (j = 0 ; j < vSizeCount; j++)
	{
		// read data
		rs_s32 readRet = rs_flash_readPage(vAddress + j*pageSize, data, pageSize);
		if (readRet != RS_ERR_OK)
			return readRet;

		rs_memcpy(p, data, pageSize);
		
		// next page
		p += pageSize;

		rs_memset(data, pageSize, 0);
	}

	if (vSizeRemain != 0)
	{
		// read data	
		rs_s32 readRet = rs_flash_readPage(vAddress + j*pageSize, data, pageSize);
		if (readRet != RS_ERR_OK)
			return readRet;
		
		// copy remain data to destination ptr
		rs_memcpy(p, data, vSizeRemain);
	}
	return RS_ERR_OK;
}

/*************************************扩展接口*************************************/

/**
函数说明：把数据和这段数据对应的checkSum写入到某个block中
参数说明：addr 需要写入的地址,block对齐
		buffer 需要写入的数据的缓冲区
		len buffer数据的缓冲区
		checkSum 这段数据的校验值
返回值：成功RS_ERR_OK（值为0），其它为失败
*/
rs_s32 rs_flash_writeDataWithCSToBlock(rs_u32 addr, const rs_u8* buffer, rs_s32 len, rs_u32 checkSum)
{

	rs_u8 page_buf[RS_PAGE_SIZE];
	rs_s32 writeLen=0;
	rs_u32 w_addr = addr;
	rs_s32 needWriteLen = len;


	if(buffer == RS_NULL || len == 0)
	{
		return RS_ERR_WRITE_DATA_FAILE;
	}

	if (rs_flash_eraseBlock(addr) != RS_ERR_OK)
	{
		return RS_ERR_WRITE_DATA_FAILE;
	}
	
	while(needWriteLen >= RS_PAGE_SIZE)
	{
		if(rs_flash_writePage(w_addr, buffer+writeLen, RS_PAGE_SIZE) != RS_ERR_OK)
		{
			return RS_ERR_WRITE_DATA_FAILE;
		}
		w_addr += RS_PAGE_SIZE;
		writeLen += RS_PAGE_SIZE;
		needWriteLen -= RS_PAGE_SIZE;
	}

	if (needWriteLen + sizeof(rs_u32) > RS_PAGE_SIZE)
	{
		rs_memset(page_buf, 0xFF, RS_PAGE_SIZE);
		rs_memcpy(page_buf, buffer+writeLen, needWriteLen);
		rs_memcpy(page_buf+needWriteLen, &checkSum, RS_PAGE_SIZE-needWriteLen);
		if(rs_flash_writePage(w_addr, page_buf, RS_PAGE_SIZE) != RS_ERR_OK)
		{
			return RS_ERR_WRITE_DATA_FAILE;
		}

		rs_memset(page_buf, 0xFF, RS_PAGE_SIZE);
		rs_memcpy(page_buf, &checkSum + (RS_PAGE_SIZE-needWriteLen), sizeof(rs_u32) - (RS_PAGE_SIZE-needWriteLen));
		if(rs_flash_writePage(w_addr, page_buf, RS_PAGE_SIZE) != RS_ERR_OK)
		{
			return RS_ERR_WRITE_DATA_FAILE;
		}
	}
	else
	{
		rs_memset(page_buf, 0xFF, RS_PAGE_SIZE);
		rs_memcpy(page_buf, buffer+writeLen, needWriteLen);
		rs_memcpy(page_buf+needWriteLen, &checkSum, sizeof(rs_u32));
		if(rs_flash_writePage(w_addr, page_buf, RS_PAGE_SIZE) != RS_ERR_OK)
		{
			return RS_ERR_WRITE_DATA_FAILE;
		}
	}

	return RS_ERR_OK;
}

/**
函数说明：从指定地址读取出来unsigned long型的整型值
参数说明：addr 需要读取的地址
返回值：返回读出的整型值
*/
rs_u32 rs_flash_readU32(rs_u32 addr)
{
	rs_u8 read_buf[RS_PAGE_SIZE];
	rs_s32 page_offset = 0;
	rs_u32 checkSum = 0;
	rs_bool readTwoPage = RS_FALSE;
	rs_u32 blockAddr = (addr / RS_LOGIC_WRITE_UINIT_SIZE) * RS_LOGIC_WRITE_UINIT_SIZE;
	rs_u32 offset = addr - blockAddr;

	if ((offset/RS_PAGE_SIZE) != ((offset + sizeof(rs_u32) - 1)/RS_PAGE_SIZE))
	{
		readTwoPage = RS_TRUE;
	}

	if (readTwoPage == RS_TRUE)
	{
		rs_s32 alreadyRead = 0;
		rs_u8 checkSumBuf[10] = {0};
		rs_u8* p = checkSumBuf;

		rs_memset(read_buf, 0, RS_PAGE_SIZE);
		page_offset = (offset/RS_PAGE_SIZE)*RS_PAGE_SIZE;
		if(rs_flash_readPage(blockAddr + page_offset, read_buf, RS_PAGE_SIZE) != RS_ERR_OK)
		{
			return RS_ERR_READ_DATA_FAILE;
		}

		alreadyRead = RS_PAGE_SIZE - (offset%RS_PAGE_SIZE);
		rs_memcpy(p, read_buf+(offset%RS_PAGE_SIZE), alreadyRead);

		rs_memset(read_buf, 0, RS_PAGE_SIZE);
		page_offset += RS_PAGE_SIZE;
		if(rs_flash_readPage(blockAddr + page_offset, read_buf, RS_PAGE_SIZE) != RS_ERR_OK)
		{
			return RS_ERR_READ_DATA_FAILE;
		}

		rs_memcpy(p + alreadyRead, read_buf, sizeof(rs_u32) - alreadyRead);

		rs_memcpy(&checkSum, checkSumBuf, 4);
	}
	else
	{
		rs_memset(read_buf, 0, RS_PAGE_SIZE);
		page_offset = (offset/RS_PAGE_SIZE)*RS_PAGE_SIZE;
		if(rs_flash_readPage(blockAddr + page_offset, read_buf, RS_PAGE_SIZE) != RS_ERR_OK)
		{
			return RS_ERR_READ_DATA_FAILE;
		}

		rs_memcpy(&checkSum, read_buf+(offset%RS_PAGE_SIZE), sizeof(rs_u32));
	}
	
	return checkSum;
}

/*
函数说明：根据地址及长度，计算该段数据的checksum值
参数说明：addr 起始地址，page对齐
			len 数据长度
返回值：失败为-1，成功为校验值
*/
rs_u32 rs_flash_calculatCSByAddr(rs_u32 addr, rs_s32 len)
{
	rs_u32 checksum = 0;
	rs_u8 buff[RS_PAGE_SIZE];
	rs_s32 pagecount;
	rs_s32 lessPageSize = len & (RS_PAGE_SIZE - 1); //非page对齐长度数据大小
	rs_s32 i;

	if((addr & (RS_PAGE_SIZE - 1)) != 0)
	{
		return -1;
	}

 	if (rs_open_checksum() != 1)
	{
		return -1;
	}
	pagecount = len/RS_PAGE_SIZE ;
	if(lessPageSize > 0)
	{
		pagecount++;
	}

	for(i=0; i<pagecount; i++)
	{
		if(rs_flash_readPage(addr + i*RS_PAGE_SIZE, buff, RS_PAGE_SIZE) != RS_ERR_OK)
		{
			return -1;
		}

		if(i + 1 == pagecount)
		{
			if (lessPageSize != 0)
			{
				checksum = rs_get_checksum(checksum, buff, lessPageSize);
			}
			else
			{
				checksum = rs_get_checksum(checksum, buff, RS_PAGE_SIZE);
			}
		}
		else
		{
			checksum = rs_get_checksum(checksum, buff, RS_PAGE_SIZE);
		}
	}
	rs_close_checksum();

	return checksum;
}


#endif

