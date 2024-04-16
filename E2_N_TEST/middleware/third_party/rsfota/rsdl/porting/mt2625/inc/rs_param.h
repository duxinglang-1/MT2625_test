#ifndef _RS_PARAM_H_
#define _RS_PARAM_H_

#include "rs_datatype.h"
#include "rs_porting_cfg.h"
#include "rs_dev.h"


//#define FOTA_AUTO_CHECK_AND_UPDATE

#ifndef FOTA_AUTO_CHECK_AND_UPDATE
#define FOTA_AUTO_UPDATE
#endif



typedef enum {
	RS_AUTOCHECK_AUTOUPDATE_MODE,		
	RS_MANUALCHECK_AUTOUPDATE_MODE,    
	RS_MANUALCHECK_MANUALUPDATE_MODE   
}RS_CHECK_MODE;

//#define WRITE_DELTA_DATA_TO_REMAIN

/**
����˵������ȡ�豸����
����˵������key���豸���Ա�ʶ
		  ��value������ֵ
����ֵ���ɹ�����0��ʧ�ܷ���1
ע�⣺value�ĳ���Ϊ64���ֽڣ���Ҫ��Ҫ����63���ֽ�
*/
rs_s32 rs_cb_get_property(const rs_s8 *key, rs_s8 *value);

/**
����˵������ѯ�Ƿ����û�����
����˵����
����ֵ������0����ʾû��UI������1����ʾ��UI
����˵������û��UI�����ĳ������ǲ�ͬ�ġ������ϴ��������ع����У�����ĳ���������
�����ֻ���������ô����֮�������UI�����û����������û��UI����������
*/
rs_s32 rs_cb_have_ui();

/**
����˵������ȡ��һ���Զ������뿪����ʱ������
����˵����
����ֵ������ֵΪ���뿪���ĺ���ʱ��
*/
rs_u32 rs_cb_get_first_check_cycle();

/**
����˵������һ���Զ�����������֮��ÿ���Զ����ļ����ʱ��
����˵����
����ֵ������ֵΪÿ���Լ������ڵļ��ʱ��
*/
rs_u32 rs_cb_get_auto_check_cycle();


/**
����˵�����Զ�������ڵ���
����˵������
����ֵ���ɹ��Ż�RS_ERR_OK, ʧ�ܷ�������ֵ
*/
rs_s32 rs_cb_auto_check();

/**
����˵����������������Դ���
����˵������
����ֵ���������Դ���
*/
rs_s32 rs_cb_get_check_retry_count();

/**
����˵������ȡ����������Դ���
����˵����
����ֵ���������Դ���
*/
rs_s32 rs_cb_get_download_retry_count();

/**
����˵������ȡ�ϱ����������ʧ�ܲ��ԣ�rs_cb_get_report_retry_count�����ϱ����������ʧ���˻��Զ����ԵĴ�����
		����ܴ���������rs_cb_get_report_max_fail_count�����趨�����ޣ���ô�����ϱ���
����˵����
����ֵ���������Դ���
*/
rs_s32 rs_cb_get_report_max_fail_count();

/**
����˵������ȡ��������ϱ�������Դ���
����˵����
����ֵ���������Դ���
�ر�˵���������Դ�����Ҫ���ô�һ�㣬�������ϱ��ɹ���������ں����ж����Ӱ��
*/
rs_s32 rs_cb_get_report_retry_count();

/**
����˵������ȡ���ع��������ʧ�ܲ��ԣ�rs_cb_get_download_retry_count�������ع��������ʧ���˻��Զ����ԵĴ���
		����ܴ��������˴˺����趨�����ޣ���ô��������������
����˵����
����ֵ���������Դ���
*/
rs_s32 rs_cb_get_download_max_fail_count();

/**
����˵����socket�����й����еĸ��ֳ�ʱ����
����˵����type ����ͬ��ʱ����
			0  gethostbyname�ĳ�ʱ����
			1  connect�ĳ�ʱ����
			2  send�ĳ�ʱ����
			3  recv�ĳ�ʱ����
����ֵ�����س�ʱ��ʱ�����ڣ���λΪ����
*/
rs_s32 rs_cb_get_socket_timeout(rs_s32 type);

/**
����˵������ȡ��ǰ���ʣ������Ƿ����
����˵����
����ֵ���������ڵ���33%,�򷵻�RS_TRUE,����RS_FALSE
*/
rs_bool rs_sys_bettery_electricity_enough();

/**
����˵������ȡ��ǰ���õĿռ�
����˵����
����ֵ�����ص�ǰ���õ�ʣ��ռ�
*/
rs_u32 rs_sys_available_space();

//������־�ȼ������ļ�·��
#define RS_CONFIG_DEBUGINFO_FILE "D://debug.ini"

// ����������Ϣ�洢���ļ�·������flash����ʼ��ַ
#ifdef RS_SUPPORT_UPDATE_INFO_FS

#define RS_CONFIG_UPINFO_FILE1 "D://updateinfo1"
#define RS_CONFIG_UPINFO_FILE2 "D://updateinfo2"

/**
����˵������ȡ��������Ϣ�Ĵ洢·��
����˵����
����ֵ�����ش洢·��
*/
const rs_s8* rs_device_get_update_info1_file();
const rs_s8* rs_device_get_update_info2_file();

#endif

#define MT2625_FLASH_ERASE_BLOCK_SIZE (4*1024)


#define RS_LOGIC_WRITE_UINIT_SIZE (8*1024)

#define RS_PAGE_SIZE 512
#define RS_DI_PAGE_SIZE 64


// ���������ļ��洢���ļ�·������flash�е���ʼ��ַ
#ifdef RS_SUPPORT_UPDATE_DATA_FS

#define FLASH_UPDATE_FILE "D://update.bin"

/**
����˵������ȡ������������·��
����˵����
����ֵ����������·��
*/
const rs_s8* rs_device_get_update_file();

#else

/*
������������ݴ洢��flash�У���ô������Ϣ����Ҳ�϶���Ҫ��flash�д洢��
��Ϊbootloader��Ҫ��ȡ
*/

#define FLASH_UPDATE_ADDR  ((FOTA_RESERVED_BASE - FLASH_BASE) + 2*RS_LOGIC_WRITE_UINIT_SIZE )//((FLASH_BASE&0xffffff) + (DL_DATA_BASE&0xffffff) + 2*RS_LOGIC_WRITE_UINIT_SIZE)

/**
����˵������ȡ�����������ش�ŵ�flash�ĵ�ַ
����˵����
����ֵ�����ش洢��ַ
*/
rs_u32 rs_device_get_update_addr();

#define RS_CONFIG_UPINFO_ADDR1 (FOTA_RESERVED_BASE - FLASH_BASE)//((FLASH_BASE&0xffffff) + (DL_DATA_BASE&0xffffff))
#define RS_CONFIG_UPINFO_ADDR2 (RS_CONFIG_UPINFO_ADDR1 + 1*RS_LOGIC_WRITE_UINIT_SIZE )//((FLASH_BASE&0xffffff) + (DL_DATA_BASE&0xffffff) + RS_LOGIC_WRITE_UINIT_SIZE)

/**
����˵������ȡ��������Ϣ�Ĵ洢��ַ
����˵����
����ֵ�����ش洢��ַ
*/
rs_u32 rs_device_get_update_info1_addr();
rs_u32 rs_device_get_update_info2_addr();

#endif


/**
����˵������ȡVIVA����ʼ��ַ
����˵������
����ֵ������VIVA����ʼ��ַ�����û��VIAVA�����򷵻�0 
*/
rs_u32 rs_sys_getVIVAStartAddr();


rs_u32 rs_cb_get_update_result();

#endif
