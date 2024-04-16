#include "rs_sdk_api_ex.h"
#include "rs_param.h"
#include "rs_sdk_api.h"
#include "rs_error.h"
#include "rs_debug.h"
#include "rs_flash_operate.h"

//#include "at.h"


//#define EARLY_INVALD_VALUE_CHECK

static rs_s32 s_erase_delta_space = 0; 

/**
����˵��������sdk���ṩ�ļ��ӿڵ�һ����չ
����˵����type 0 ��ʾ�ֶ�����  1 ��ʾ�Զ�����
����ֵ���ɹ�RS_ERR_OK��ֵΪ0��������Ϊʧ��
*/
rs_s32 rs_sdk_check_ex(rs_s32 type)
{
	rs_s32 ret = 0;

#ifdef EARLY_INVALD_VALUE_CHECK
	if(rs_sys_simcard_insert() == RS_FALSE)
	{
		return RS_CARD_NOT_INSERT;
	}

	if (rs_sys_simcard_recognize() == RS_FALSE)
	{
		return RS_CARD_NOT_RECOGNIZE;
	}

	if (rs_sys_imei_valid() == RS_FALSE)
	{
		return RS_IMEI_NOT_WRITTEN;
	}
#endif

	// ��0ֵ��ʾϵͳ��æ�����ڽ��������� ���� ��������
	return rs_sdk_check(type);
}

/**
����˵��������sdk���ṩ�����ؽӿڵ�һ����չ�������˿ռ���ж�
����˵������
����ֵ���ɹ�RS_ERR_OK��ֵΪ0��������Ϊʧ��
*/
rs_s32 rs_sdk_download_ex(rs_s32 type)
{
	rs_u32 availableSpace = rs_sys_available_space();
	rs_s32 alreadyDown = rs_sdk_getAlreadyDownloadSize();
	const RS_FWDL_DD*dd = rs_sdk_getPkgDD();
	rs_u32 needSpace = 0;

#ifdef EARLY_INVALD_VALUE_CHECK
	if(rs_sys_simcard_insert() == RS_FALSE)
	{
		return RS_CARD_NOT_INSERT;
	}

	if (rs_sys_simcard_recognize() == RS_FALSE)
	{
		return RS_CARD_NOT_RECOGNIZE;
	}

	if (rs_sys_imei_valid() == RS_FALSE)
	{
		return RS_IMEI_NOT_WRITTEN;
	}
#endif

	needSpace = dd->objectSize - alreadyDown;
	//hal_HstSendEvent(0x50002000);
	//hal_HstSendEvent(dd->objectSize);
	//hal_HstSendEvent(alreadyDown);

	RS_PORITNG_LOG(RS_LOG_INFO"rs_sdk_download_ex, current size=%d, total size=%d\n\r", alreadyDown, dd->objectSize);

	if (availableSpace < needSpace)
	{
		return RS_ERR_SPACE_NOT_ENGHOU;
	}
	else
	{
#if  0//def WRITE_DELTA_DATA_TO_REMAIN
		// s_erase_delta_space ��ʾ����û��erase����������һ��
		// alreadyDown��ʾ��ǰ����Ϊ0������һ���������֮�󣬷������������⣬���еڶ������أ�alreadyDown���ж�����������
		if (s_erase_delta_space == 0 || alreadyDown == 0)
		{
			if(rs_flash_eraseDeltaSpace(alreadyDown) != RS_ERR_OK)
				return RS_ERR_FLASH_WRITE;
		}
#endif
		return rs_sdk_download(type);
	}
}


/**
����˵������װ�Ѿ����ص�������
����˵������
����ֵ���ɹ�RS_ERR_OK��ֵΪ0��������Ϊʧ��
�ر�˵������
��¶���û�����
*/
rs_s32 rs_sdk_install_ex()
{
	if (rs_sys_bettery_electricity_enough() == RS_FALSE)
	{
		return RS_ERR_BATTERY_LEVEL_LOW;
	}

	return rs_sdk_install();
}

/*
��ȡ��ǰ�汾�ĳ���
*/
rs_u32 rs_sdk_new_version_size(void)
{
	const RS_FWDL_DD* dd = RS_NULL;

	dd = rs_sdk_getPkgDD();
	if (dd == RS_NULL)
		return -1;

	return dd->objectSize;
}

/*
* ��ȡ�°汾�İ汾��
*/
rs_s8* rs_sdk_new_version_name(void)
{
	const RS_FWDL_DD* dd = RS_NULL;

	dd = rs_sdk_getPkgDD();
	if (dd == RS_NULL)
		return RS_NULL;

	return (rs_s8*)dd->toVersion;
}

/*
* ��ȡ�°汾�İ汾����
*/
rs_s8* rs_sdk_new_version_description(void)
{
	const RS_FWDL_DD* dd = RS_NULL;

	dd = rs_sdk_getPkgDD();
	if (dd == RS_NULL)
		return RS_NULL;

	return (rs_s8*)dd->brief;
}


/*
*	AT+RSFOTA=1  //�������+����
*	AT+RSFOTA=2  //�������
*	AT+RSFOTA=3  //����
*   ֻ֧��������ָ��
*/
#if 0 
VOID AT_RSFOTA(AT_CMD_PARA *pParam)
{

	UINT8 		nSim = AT_SIM_ID(pParam->nDLCI);
	INT32		iResult;
	UINT8 		uParamCount = 0;
	UINT8 		iIndex = 0;
	UINT16 		uSize = 0;
	UINT8		Vendor;

	rs_s32 cauType = rs_sdk_get_auto_cau_type();
	RS_PORITNG_LOG(RS_LOG_DEBUG"AT_RSFOTA cauType = %d", cauType);
	  
	if(NULL == pParam)	    
	{	       
		AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
		return;
	}
	
	if (pParam->iType == AT_CMD_SET)
	{
		iResult = AT_Util_GetParaCount(pParam->pPara, &uParamCount);
		if ( iResult != ERR_SUCCESS || uParamCount != 1 )
		{
	        AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
	        return;
		}


		iIndex = 0;
		uSize	= SIZEOF(Vendor);
		iResult = AT_Util_GetParaWithRule(pParam->pPara, iIndex++, AT_UTIL_PARA_TYPE_UINT8, &Vendor, &uSize);
		if (iResult != ERR_SUCCESS)
		{
	        AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
	        return;
		}

		if (uParamCount == 1 )
		{
			
			RS_PORITNG_LOG(RS_LOG_DEBUG"AT_RSFOTA Vendor = %d " , Vendor);
			if (Vendor == 1)//������ֱ������
			{
				if (cauType != RS_MANUALCHECK_AUTOUPDATE_MODE)
				{
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
					return;
				}
//#ifndef FOTA_AUTO_CHECK_AND_UPDATE
				rs_fota_autocheck();
//#endif
				RS_PORITNG_LOG(RS_LOG_DEBUG"AT_RSFOTA Vendor =1");
			}
			else if (Vendor == 2)//ֻ�������ز��Զ�����
			{		
				if (cauType != RS_MANUALCHECK_MANUALUPDATE_MODE)
				{
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
					return;
				}
//#ifndef FOTA_AUTO_CHECK_AND_UPDATE
				rs_fota_autocheck();
//#endif
				RS_PORITNG_LOG(RS_LOG_DEBUG"AT_RSFOTA Vendor =2");
			}
			else if (Vendor == 3)//ֻ����
			{
				if (cauType != RS_MANUALCHECK_MANUALUPDATE_MODE)
				{
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
					return;
				}
//#ifndef FOTA_AUTO_CHECK_AND_UPDATE
				rs_fota_enter_update_mode();
//#endif
				RS_PORITNG_LOG(RS_LOG_DEBUG"AT_RSFOTA Vendor =3");
			}else{//����ֻ�ܽ���1��2��3����������Ч
				AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
				return;
			}
			AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, NULL, 0, pParam->nDLCI, nSim);
		}	
	}
	else
	{
		AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
		return;
	}	
}



void ReaStone_start_get_new_version(void)
{
	return rs_fota_autocheck();
}

INT32 ReaStone_isNewVesion(void)
{
	RS_FWDL_STATE state = RS_FWDL_STATE_IDEL;

	state = rs_sdk_getDLCurrentState();
	if (state == RS_FWDL_STATE_CHECKING)
		return 3;

	if (state >= RS_FWDL_STATE_DOWNLOADING)
		return 2;

	return rs_notify_get_check_ret();
}

INT32 ReaStone_get_file_size(void)
{
	return (INT32)rs_sdk_new_version_size();
}

INT32 ReaStone_recvd_size(void)
{
	return (INT32)rs_sdk_getAlreadyDownloadSize();
	
}

INT32 ReaStone_fotaStatus(void)
{
	RS_FWDL_STATE state = RS_FWDL_STATE_IDEL;

	state = rs_sdk_getDLCurrentState();
	if (state >= RS_FWDL_STATE_DOWNLOADING)
		return 3;

	// ��ȡflash����ı�־λ ###########

	if(rs_notify_get_update_ret() == 2)
		return 2;
	else
		return 1;
}

#endif

