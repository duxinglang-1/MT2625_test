/*
 *
 * 版权(c) 2015 红石阳光（北京）科技有限公司
 * 版权(c) 2011-2015 红石阳光（北京）科技有限公司版权所有
 * 
 * 本文档包含信息是私密的且为红石阳光私有，且在民法和刑法中定义为商业秘密信息。
 * 任何人士不得擅自复印，扫描或以其他任何方式进行传播，否则红石阳光有权追究法律责任。
 * 阅读和使用本资料必须获得相应的书面授权，承担保密责任和接受相应的法律约束。
 *
 */

#include <stdio.h>
#include "rs_fs.h"
#include "rs_error.h"
#include "rs_debug.h"

//#include "Fs.h"

//#define SUPPORT_FILESYSTEM

/*
函数说明：打开文件
参数说明：fileName  文件的路径
		  openMode  打开的模式，参见rs_fs_openMode
返回值：成功返回文件句柄，失败返回RS_FS_INVALID
*/
RS_FILE rs_fs_open(const rs_s8* fileName, rs_fs_openMode openMode)
{
	RS_FILE fh = RS_FS_INVALID;
#ifdef SUPPORT_FILESYSTEM
	
	UINT32 iFlag = 0;

	if(openMode == RS_FS_OPEN_CREATE)
	{
		iFlag = FS_O_CREAT;
	}
	else if(openMode == RS_FS_OPEN_READ)
	{
		iFlag = FS_O_RDONLY;
	}
	else if(openMode == RS_FS_OPEN_WRITE)
	{
		iFlag = FS_O_RDWR;
	}

	fh = (RS_FILE)FS_Open(fileName, iFlag, 0);
	if ( fh < 0)
	{
		RS_PORITNG_LOG(RS_LOG_DEBUG"open file fail \n");
		fh = RS_FS_INVALID;
	}
	else
	{
		RS_PORITNG_LOG(RS_LOG_DEBUG"open file successfully \n");
	}
#else
	fh = RS_FS_INVALID;
#endif
	return fh;
}

/*
函数说明：关闭文件
参数说明：fh  文件打开后的句柄
返回值：成功返回0，失败为-1
*/
rs_s32 rs_fs_close(RS_FILE fh)
{
#ifdef SUPPORT_FILESYSTEM
	FS_Close((INT32)fh);
	return  0;
#else
	return -1;
#endif
}

/*
函数说明：把数据读取到缓冲区内
参数说明：fh  文件打开后的句柄
		  buffer  需要读入的数据的缓冲区
		  size  需要写入的数据的长度
返回值：成功读取文件数据的长度，失败返回-1
*/
rs_s32 rs_fs_read(RS_FILE fh, void* buffer, rs_s32 size)
{ 
#ifdef SUPPORT_FILESYSTEM
	return FS_Read((INT32)fh, (UINT8 *)buffer, (UINT32)size);
#else
	return -1;
#endif
}

/*
函数说明：把数据写入到文件中
参数说明：fh  文件打开后的句柄
		  buffer  需要写入的数据
		  size  需要写入的数据的长度
返回值：成功为写入文件的长度，失败返回-1
*/
rs_s32 rs_fs_write(RS_FILE fh, const void* buffer, rs_s32 size)
{
#ifdef SUPPORT_FILESYSTEM
	return FS_Write((INT32)fh, (UINT8 *)buffer, (UINT32)size);
#else
	return -1;
#endif
}

/*
函数说明：设置文件指针的位置
参数说明：fh  文件打开后的句柄
		  offset  相对于origin的偏移量
		  origin  文件头 尾 当前位置
返回值：返回0，失败返回-1
*/
rs_s32 rs_fs_seek(RS_FILE fh, rs_s32 offset, rs_s32 origin)
{
#ifdef SUPPORT_FILESYSTEM
	return FS_Seek((INT32)fh, offset, (UINT8)origin);
#else
	return -1;
#endif
}

/*
函数说明：获取文件的大小
参数说明：fh  文件打开后的句柄
返回值：文件的大小
*/
rs_s32 rs_fs_size(RS_FILE fh)
{ 
#ifdef SUPPORT_FILESYSTEM
	return (rs_s32)FS_GetFileSize((INT32)fh);
#else
	return -1;
#endif

}

/*
函数说明：判断文件系统中的文件是否存在
参数说明：fileName  文件的路径
返回值：存在为1，其他为0
*/
rs_s32 rs_fs_exists(const rs_s8* fileName)
{
#ifdef SUPPORT_FILESYSTEM
	RS_FILE f;
	f = rs_fs_open(fileName, RS_FS_OPEN_READ);
	if (f != RS_FS_INVALID)
	{
		// it existes
		rs_fs_close(f);
		return 1;
	}
	else
	{
		// not exist
		return 0;
	} 
#else
	return 0;
#endif
}


/*
函数说明：删除文件系统中的文件
参数说明：fileName  文件的路径
返回值：成功为0，其他为失败
*/
rs_s32 rs_fs_remove(const rs_s8* fileName)
{
#ifdef SUPPORT_FILESYSTEM
	return FS_Delete(fileName) ? -1 : 0;
#else
	return -1;
#endif
}
