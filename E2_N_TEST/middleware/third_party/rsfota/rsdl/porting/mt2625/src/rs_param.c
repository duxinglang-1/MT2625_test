#include "rs_param.h"
#include "rs_system.h"
#include "rs_error.h"
#include "rs_datatype.h"
#include "rs_sdk_api.h"
#include "rs_state.h"
#include "rs_std_fun.h"
#include "rs_debug.h"
#include "rs_sdk_api_ex.h"
#include "rs_thread.h"

// ƽ̨��ͷ�ļ�
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

/***********************************************************************************/
extern rs_s8 g_rsimsi[20];
extern rs_s8 g_rsimei[20];

/***********************************************************************************/
//extern CFW_SIM_ID g_SIMID;

/*
 *	��ȡ�豸Ʒ��
 */
static void get_OEMBrand(char *buf)
{
#if 0
	// TODO:�ͻ�����������ӻ�ȡƷ�ƵĽӿ�
#else // default
	rs_strcpy(buf, "NBIOT");
	return ;
#endif
}

/*
 *	��ȡ�ͺ�
*/
static void get_OEMDevice(char *buf)
{
#if 0
	// TODO:�ͻ�����������ӻ�ȡ�ͺŵĽӿ�
#else //default
	rs_strcpy(buf, "MT2625");
	return ;
#endif
}


/*
 * ��ȡ�豸��Ψһ��ʾ����
*/
static void get_OEMIMEI(char *buf)
{
	rs_u8 nImei[24];
    rs_u8 nImeiLen = 0;

	//CFW_EmodGetIMEI(nImei, &nImeiLen, g_SIMID);
	//nImei[nImeiLen] = 0;
	//if(rs_strcmp(nImei, "000000000000000") == 0)
	//{
	//	rs_strcpy(buf, "IMEI:123456789000003");
	//}
	//else
	//{
		rs_strcpy(buf, "IMEI:");
		rs_strcat(buf, g_rsimei);	
	//}
	
}

/*
 * ��ȡ��ǰ�豸������汾��
*/
static void get_OEMCurrentVersion(char *buf)
{
	rs_strcpy(buf, "v1.1");
}


/*
 * ��ȡ��ǰ�豸����������
*/
static void get_OEMLanguage(char *buf)
{
	rs_strcpy(buf, "zh-CN");
}

/*
 * ��ȡ��Ӫ�̵�ID
*/
static void get_OEMCatoragy(char *buf)
{
	rs_u32 iResult = 0;
	rs_u8 OperatorId[6] = {0};
	rs_u8 nMode = 0;

	rs_strcpy(buf, g_rsimsi);	
}

/*
 * ��ȡOS�İ汾
*/
static void get_OEMOSVersion(char *buf)
{
	rs_strcpy(buf, "C216B");
}

/*
 * ��ȡapp key
*/
static void get_AppKey(char *buf)
{
	rs_strcpy(buf, "111111111111111111111111");
	
}

/*
 * ��ȡ������
*/
static void get_Channel(char *buf)
{
	rs_strcpy(buf, "Channel");
}

/*
* ��ȡ��վ��Ϣ
*/
static void get_CellInfo(char *buf)
{	
	rs_sprintf(buf, "%d-%d-%d", 0, 0, 0);
}


/**
����˵������ȡ�豸����
����˵������key���豸���Ա�ʶ
		  ��value������ֵ
����ֵ���ɹ�����0��ʧ�ܷ���1
ע�⣺value�ĳ���Ϊ64���ֽڣ���Ҫ��Ҫ����63���ֽ�
*/
rs_s32 rs_cb_get_property(const rs_s8 *key, rs_s8 *value)
{
	if (0 == key || 0 == value) 
		return 1;

	if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_DEVID, key)) // ��ȡ�豸ID
	{
		get_OEMIMEI(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_MAN, key)) // ��ȡƷ��
	{
		get_OEMBrand(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_MODEL, key)) // ��ȡ�ͺ�
	{
		get_OEMDevice(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_FWV, key)) // ��ȡ��ǰ������汾
	{
		get_OEMCurrentVersion(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_LANG, key)) // ��ȡ��ǰ������
	{
		get_OEMLanguage(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_OPERATORS, key)) // ��ȡ��Ӫ�̵�ID
	{
		get_OEMCatoragy(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_OSV, key)) // ��ȡϵͳ�İ汾��
	{
		get_OEMOSVersion(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_APP_KEY, key)) // ��ȡapp key
	{
		get_AppKey(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_CHANNEL, key)) // ��ȡ������
	{
		get_Channel(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_DEVMAC, key)) // ��ȡMAC
	{
		value[0] = 0;
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_LOC, key)) // ��ȡLOC
	{
		get_CellInfo(value);
	}
	else
	{
		return 1;
	}
	
	if (key != 0 && value != 0 && (rs_strlen(key) > 0) && (rs_strlen(value) > 0))
	{
		RS_PORITNG_LOG(RS_LOG_INFO"key=%s, value = %s \r\n", key, value);
	}

	return 0;
}


/**
����˵������ѯ�Ƿ����û�����
����˵����
����ֵ������0����ʾû��UI������1����ʾ��UI
����˵������û��UI�����ĳ������ǲ�ͬ�ġ������ϴ��������ع����У�����ĳ���������
�����ֻ���������ô����֮�������UI�����û����������û��UI����������
*/
rs_s32 rs_cb_have_ui()
{
	return 0;
}

/**
����˵������ȡ��һ���Զ������뿪����ʱ������
����˵����
����ֵ������ֵΪ���뿪���ĺ���ʱ��
*/
rs_u32 rs_cb_get_first_check_cycle()
{
	//return (90*1000);
	return (20*1000);
}

/**
����˵������һ���Զ�����������֮��ÿ���Զ����ļ����ʱ��
����˵����
����ֵ������ֵΪÿ���Լ������ڵļ��ʱ��
ע��:���ڶ�ʱ�����֧��37Сʱ�����������Ҫ���ø���ʱ��Ķ�ʱ�������Բ����ۻ��ķ�ʽ���������24Сʱ��
��24СʱΪһ�����ڣ��ۻ�����Ҫ��ʱ��֮���ٴ�������
*/
rs_u32 rs_cb_get_auto_check_cycle()
{
	//return (24*60*60*1000);
	return 60*1000;
}


/**
����˵�����Զ�������ڵ���
����˵������
����ֵ���ɹ�����RS_ERR_OK, ʧ�ܷ�������ֵ
*/
rs_s32 rs_cb_auto_check()
{
	rs_s32 ret = RS_ERR_OK;
	rs_s32 cauType = -1;
	RS_FWDL_STATE state = rs_sdk_getDLCurrentState();

	RS_PORITNG_LOG(RS_LOG_INFO"rs_cb_auto_check, current state is %d \r\n", state);
	if(rs_cb_have_ui() == 0)
	{
		// û��UI
		if (state == RS_FWDL_STATE_DOWNLOAD_PAUSE)
		{
			// ����ϳ���ʲô�쳣��������ֱ������
			ret = rs_sdk_download_ex(1);
			return ret;
		}
        else if(state == RS_FWDL_STATE_DOWNLOADED)
        {
            // ����Ѿ����ص�δ��װ�����ֱ�Ӱ�װ
            ret =  rs_sdk_install_ex();
            return ret;
        }
	}

	//if (rs_sdk_needAutoCheck())
	{
		ret = rs_sdk_check_ex(1); // �����ڲ��ж�״̬���жϣ������ʱ�����ʽ��м�⣬�������˳�
	}

	return ret;
}

/**
����˵����������������Դ���
����˵������
����ֵ���������Դ���
*/
rs_s32 rs_cb_get_check_retry_count()
{
	return 2;
}

/**
����˵������ȡ����������Դ���
����˵����
����ֵ���������Դ���
*/
rs_s32 rs_cb_get_download_retry_count()
{
	return 20;
}

/**
����˵������ȡ��������ϱ�������Դ���
����˵����
����ֵ���������Դ���
�ر�˵���������Դ�����Ҫ���ô�һ�㣬�������ϱ��ɹ���������ں����ж����Ӱ��
*/
rs_s32 rs_cb_get_report_retry_count()
{
	return 5;
}

/**
����˵������ȡ���ع��������ʧ�ܲ��ԣ�rs_cb_get_download_retry_count�������ع��������ʧ���˻��Զ����ԵĴ���
		����ܴ��������˴˺����趨�����ޣ���ô��������������
����˵����
����ֵ���������Դ���
*/
rs_s32 rs_cb_get_download_max_fail_count()
{
	return 10;
}

/**
����˵������ȡ�ϱ����������ʧ�ܲ��ԣ�rs_cb_get_report_retry_count�����ϱ����������ʧ���˻��Զ����ԵĴ�����
		����ܴ���������rs_cb_get_report_max_fail_count�����趨�����ޣ���ô�����ϱ���
����˵����
����ֵ���������Դ���
*/
rs_s32 rs_cb_get_report_max_fail_count()
{
	return 10;
}

/**
����˵����socket�����й����еĸ��ֳ�ʱ����
����˵����type ����ͬ��ʱ����
			0  gethostbyname�ĳ�ʱ����
			1  connect�ĳ�ʱ����
			2  send�ĳ�ʱ����
			3  recv�ĳ�ʱ����
����ֵ�����س�ʱ��ʱ�����ڣ���λΪ����
��ע: RDA��flashд���п�����Ҫ�ȴ��ܳ�ʱ�䣬�����ʱ��һ��Ҫ����rs_flash_operater.c����ȴ�д���flash��ʱ��
*/
rs_s32 rs_cb_get_socket_timeout(rs_s32 type)
{
	rs_s32 timeout = 0;

	switch(type)
	{
	case 0: // gethostbyname ��ʱʱ��
		timeout = 30*1000;
		break;
	case 1: // connect ��ʱʱ��
		timeout = 30*1000;
		break;
	case 2: // send ��ʱʱ��
		timeout = 30*1000;
		break;
	case 3: // recv ��ʱʱ��
		timeout = 30*1000;
		break;
	case 4: // session ���ʱ��
		timeout = 3*1000;
		break;
	case 5: // pdp active ��ʱʱ��
		timeout = 60*1000;
		break;
	}

	return timeout;
}


/**
����˵������ȡ��ǰ���ʣ������Ƿ����
����˵����
����ֵ���������ڵ���33%,�򷵻�RS_TRUE,����RS_FALSE
�ر�˵��:bmt_status.volt��ص�ѹ�������п��ܸ��ݵ�ѹ�жϵ�ǰ���Ӿ�ȷ��
����ʣ�������������Ϊ���Ϊ3593500,���Ϊ4253900���ֳ�100�֣����ǰ�volt
��ӡ������Ϊ�˽�
������ע�⣬��MMITask.c��init�����ϵ������½���ǲ��еģ��п��ܳ���û�г�ʼ��
*/
rs_bool rs_sys_bettery_electricity_enough()
{
	return RS_TRUE;
}

/**
����˵������ȡ��ǰ���õĿռ�
����˵����
����ֵ�����ص�ǰ���õ�ʣ��ռ�,��λΪbyte
*/
rs_u32 rs_sys_available_space()
{
	return ((FOTA_RESERVED_LENGTH - MT2625_FLASH_ERASE_BLOCK_SIZE - 2*RS_LOGIC_WRITE_UINIT_SIZE) / RS_LOGIC_WRITE_UINIT_SIZE) * RS_LOGIC_WRITE_UINIT_SIZE ;
}
rs_s32 rs_sdk_get_auto_cau_type()
{
#ifdef FOTA_AUTO_CHECK_AND_UPDATE //ȫ�Զ�������ģʽATָ����Ч
	return RS_AUTOCHECK_AUTOUPDATE_MODE;
#else							  //�ֶ� 
	#ifdef FOTA_AUTO_UPDATE //�ֶ���⣬�����Զ�
		return RS_MANUALCHECK_AUTOUPDATE_MODE; //����ATָ��������ɼ������غ���������
	#else
		return RS_MANUALCHECK_MANUALUPDATE_MODE; //�ֶ���⡢�ֶ��������������������
	#endif
#endif
}


#ifdef RS_SUPPORT_UPDATE_DATA_FS

/**
����˵������ȡ������������·��
����˵����
����ֵ����������·��
*/
const rs_s8* rs_device_get_update_file()
{
	return FLASH_UPDATE_FILE;
}

#else

rs_u32 rs_device_get_update_package_space()
{
	return 0;
}

/*
������������ݴ洢��flash�У���ô������Ϣ����Ҳ�϶���Ҫ��flash�д洢��
��Ϊbootloader��Ҫ��ȡ
*/

/**
����˵������ȡ�����������ش�ŵ�flash�ĵ�ַ
����˵����
����ֵ�����ش洢��ַ
*/
rs_u32 rs_device_get_update_addr()
{
	return FLASH_UPDATE_ADDR;
}

/**
����˵������ȡ��������Ϣ�Ĵ洢��ַ
����˵����
����ֵ�����ش洢��ַ
*/
rs_u32 rs_device_get_update_info1_addr()
{
	return RS_CONFIG_UPINFO_ADDR1;
}

/**
����˵������ȡ��������Ϣ�Ĵ洢��ַ
����˵����
����ֵ�����ش洢��ַ
*/
rs_u32 rs_device_get_update_info2_addr()
{
	return RS_CONFIG_UPINFO_ADDR2;
}

#endif

#ifdef RS_SUPPORT_UPDATE_INFO_FS

/**
����˵������ȡ��������Ϣ�Ĵ洢·��
����˵����
����ֵ�����ش洢·��
*/
const rs_s8* rs_device_get_update_info1_file()
{
	return RS_CONFIG_UPINFO_FILE1;
}

/**
����˵������ȡ��������Ϣ�Ĵ洢·��
����˵����
����ֵ�����ش洢·��
*/
const rs_s8* rs_device_get_update_info2_file()
{
	return RS_CONFIG_UPINFO_FILE2;
}

#endif



/**
����˵������ȡVIVA����ʼ��ַ
����˵������
����ֵ������VIVA����ʼ��ַ�����û��VIAVA�����򷵻�0 
*/
rs_u32 rs_sys_getVIVAStartAddr()
{
	return 0;
}


rs_u32 rs_cb_get_update_result()
{
	return 1;
} 
