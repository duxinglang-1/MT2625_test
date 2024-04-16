/*
 *
 * ��Ȩ(c) 2015 ��ʯ���⣨�������Ƽ����޹�˾
 * ��Ȩ(c) 2011-2015 ��ʯ���⣨�������Ƽ����޹�˾��Ȩ����
 * 
 * ���ĵ�������Ϣ��˽�ܵ���Ϊ��ʯ����˽�У������񷨺��̷��ж���Ϊ��ҵ������Ϣ��
 * �κ���ʿ�������Ը�ӡ��ɨ����������κη�ʽ���д����������ʯ������Ȩ׷���������Ρ�
 * �Ķ���ʹ�ñ����ϱ�������Ӧ��������Ȩ���е��������κͽ�����Ӧ�ķ���Լ����
 *
 */

#include <stdio.h>
#include "rs_fs.h"
#include "rs_error.h"
#include "rs_debug.h"

//#include "Fs.h"

//#define SUPPORT_FILESYSTEM

/*
����˵�������ļ�
����˵����fileName  �ļ���·��
		  openMode  �򿪵�ģʽ���μ�rs_fs_openMode
����ֵ���ɹ������ļ������ʧ�ܷ���RS_FS_INVALID
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
����˵�����ر��ļ�
����˵����fh  �ļ��򿪺�ľ��
����ֵ���ɹ�����0��ʧ��Ϊ-1
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
����˵���������ݶ�ȡ����������
����˵����fh  �ļ��򿪺�ľ��
		  buffer  ��Ҫ��������ݵĻ�����
		  size  ��Ҫд������ݵĳ���
����ֵ���ɹ���ȡ�ļ����ݵĳ��ȣ�ʧ�ܷ���-1
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
����˵����������д�뵽�ļ���
����˵����fh  �ļ��򿪺�ľ��
		  buffer  ��Ҫд�������
		  size  ��Ҫд������ݵĳ���
����ֵ���ɹ�Ϊд���ļ��ĳ��ȣ�ʧ�ܷ���-1
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
����˵���������ļ�ָ���λ��
����˵����fh  �ļ��򿪺�ľ��
		  offset  �����origin��ƫ����
		  origin  �ļ�ͷ β ��ǰλ��
����ֵ������0��ʧ�ܷ���-1
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
����˵������ȡ�ļ��Ĵ�С
����˵����fh  �ļ��򿪺�ľ��
����ֵ���ļ��Ĵ�С
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
����˵�����ж��ļ�ϵͳ�е��ļ��Ƿ����
����˵����fileName  �ļ���·��
����ֵ������Ϊ1������Ϊ0
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
����˵����ɾ���ļ�ϵͳ�е��ļ�
����˵����fileName  �ļ���·��
����ֵ���ɹ�Ϊ0������Ϊʧ��
*/
rs_s32 rs_fs_remove(const rs_s8* fileName)
{
#ifdef SUPPORT_FILESYSTEM
	return FS_Delete(fileName) ? -1 : 0;
#else
	return -1;
#endif
}
