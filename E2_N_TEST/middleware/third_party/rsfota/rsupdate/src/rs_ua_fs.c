/*
 *
 * ��Ȩ(c) 2017 ��ʯ���⣨�������Ƽ����޹�˾
 * ��Ȩ(c) 2011-2017 ��ʯ���⣨�������Ƽ����޹�˾��Ȩ����
 * 
 * ���ĵ�������Ϣ��˽�ܵ���Ϊ��ʯ����˽�У������񷨺��̷��ж���Ϊ��ҵ������Ϣ��
 * �κ���ʿ�������Ը�ӡ��ɨ����������κη�ʽ���д����������ʯ������Ȩ׷���������Ρ�
 * �Ķ���ʹ�ñ����ϱ�������Ӧ��������Ȩ���е��������κͽ�����Ӧ�ķ���Լ����
 *
 */


#include "rs_ua_fs.h"
#include "rs_ua_porting.h"

#ifndef RS_DIFF_PACKAGE_ON_FLASH

/*
����˵�������ļ�
����˵����[IN]fileName  �ļ���·��
		      [IN]openMode  �򿪵�ģʽ���μ�rs_fs_openMode
����ֵ���ɹ������ļ������ʧ�ܷ���RS_FS_INVALID
*/
RS_FILE rs_ua_fs_open(const rs_s8* fileName, rs_fs_openMode openMode)
{
	return RS_FS_INVALID;
}

/*
����˵�����ر��ļ�
����˵����[IN]fh  �ļ��򿪺�ľ��
����ֵ���ɹ�����rs_true��ʧ��Ϊrs_false
*/
rs_bool rs_ua_fs_close(RS_FILE fh)
{
	return rs_false;
}

/*
����˵���������ݶ�ȡ����������
����˵����[IN]fh  �ļ��򿪺�ľ��
		      [IN OUT]buffer  ��Ҫ��������ݵĻ�����
		      [IN]size  ��Ҫд������ݵĳ���
����ֵ���ɹ���ȡ�ļ����ݵĳ��ȣ�ʧ�ܷ���-1
*/
rs_s32 rs_ua_fs_read(RS_FILE fh, void* buffer, rs_u32 size)
{
	return -1;
}

/*
����˵����������д�뵽�ļ���
����˵����[IN]fh  �ļ��򿪺�ľ��
		      [IN]buffer  ��Ҫд�������
		      [IN]size  ��Ҫд������ݵĳ���
����ֵ���ɹ�Ϊд���ļ��ĳ��ȣ�ʧ�ܷ���-1
*/
rs_s32 rs_ua_fs_write(RS_FILE fh, const void* buffer, rs_u32 size)
{
	return -1;
}

/*
����˵���������ļ�ָ���λ��
����˵����[IN]fh  �ļ��򿪺�ľ��
		      [IN]offset  �����origin��ƫ����
		      [in]origin  �ļ�ͷ β ��ǰλ��,�μ�rs_ua_fs_seekMode
����ֵ������rs_true��ʧ�ܷ���rs_false
*/
rs_bool rs_ua_fs_seek(RS_FILE fh, rs_s32 offset, rs_s32 origin)
{
	return rs_false;
}

/*
����˵������ȡ�ļ��Ĵ�С
����˵����[IN]fh  �ļ��򿪺�ľ��
����ֵ���ļ��Ĵ�С
*/
rs_u32 rs_ua_fs_size(RS_FILE fh)
{ 
	return 0;
}

/*
����˵�����ж��ļ�ϵͳ�е��ļ��Ƿ����
����˵����[IN]fileName  �ļ���·��
����ֵ������Ϊrs_true��������Ϊrs_false
*/
rs_bool rs_ua_fs_exists(const rs_s8* fileName)
{
	return rs_false;
}


/*
����˵����ɾ���ļ�ϵͳ�е��ļ�
����˵����[IN]fileName  �ļ���·��
����ֵ���ɹ�Ϊrs_true��ʧ��Ϊrs_false��
*/
rs_bool rs_ua_fs_remove(const rs_s8* fileName)
{
	return rs_false;
}

#endif

