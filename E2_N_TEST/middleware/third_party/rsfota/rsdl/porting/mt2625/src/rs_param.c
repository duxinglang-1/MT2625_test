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

/***********************************************************************************/
extern rs_s8 g_rsimsi[20];
extern rs_s8 g_rsimei[20];

/***********************************************************************************/
//extern CFW_SIM_ID g_SIMID;

/*
 *	获取设备品牌
 */
static void get_OEMBrand(char *buf)
{
#if 0
	// TODO:客户可在这里添加获取品牌的接口
#else // default
	rs_strcpy(buf, "NBIOT");
	return ;
#endif
}

/*
 *	获取型号
*/
static void get_OEMDevice(char *buf)
{
#if 0
	// TODO:客户可在这里添加获取型号的接口
#else //default
	rs_strcpy(buf, "MT2625");
	return ;
#endif
}


/*
 * 获取设备的唯一标示符号
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
 * 获取当前设备的软件版本号
*/
static void get_OEMCurrentVersion(char *buf)
{
	rs_strcpy(buf, "v1.1");
}


/*
 * 获取当前设备的语言类型
*/
static void get_OEMLanguage(char *buf)
{
	rs_strcpy(buf, "zh-CN");
}

/*
 * 获取运营商的ID
*/
static void get_OEMCatoragy(char *buf)
{
	rs_u32 iResult = 0;
	rs_u8 OperatorId[6] = {0};
	rs_u8 nMode = 0;

	rs_strcpy(buf, g_rsimsi);	
}

/*
 * 获取OS的版本
*/
static void get_OEMOSVersion(char *buf)
{
	rs_strcpy(buf, "C216B");
}

/*
 * 获取app key
*/
static void get_AppKey(char *buf)
{
	rs_strcpy(buf, "111111111111111111111111");
	
}

/*
 * 获取渠道号
*/
static void get_Channel(char *buf)
{
	rs_strcpy(buf, "Channel");
}

/*
* 获取基站信息
*/
static void get_CellInfo(char *buf)
{	
	rs_sprintf(buf, "%d-%d-%d", 0, 0, 0);
}


/**
函数说明：获取设备属性
参数说明：【key】设备属性标识
		  【value】属性值
返回值：成功返回0，失败返回1
注意：value的长度为64个字节，主要不要超过63个字节
*/
rs_s32 rs_cb_get_property(const rs_s8 *key, rs_s8 *value)
{
	if (0 == key || 0 == value) 
		return 1;

	if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_DEVID, key)) // 获取设备ID
	{
		get_OEMIMEI(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_MAN, key)) // 获取品牌
	{
		get_OEMBrand(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_MODEL, key)) // 获取型号
	{
		get_OEMDevice(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_FWV, key)) // 获取当前的软件版本
	{
		get_OEMCurrentVersion(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_LANG, key)) // 获取当前的语言
	{
		get_OEMLanguage(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_OPERATORS, key)) // 获取运营商的ID
	{
		get_OEMCatoragy(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_OSV, key)) // 获取系统的版本号
	{
		get_OEMOSVersion(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_APP_KEY, key)) // 获取app key
	{
		get_AppKey(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_CHANNEL, key)) // 获取渠道号
	{
		get_Channel(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_DEVMAC, key)) // 获取MAC
	{
		value[0] = 0;
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_LOC, key)) // 获取LOC
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
函数说明：查询是否有用户界面
参数说明：
返回值：返回0，表示没有UI；返回1，表示有UI
场景说明：有没有UI在许多的场景还是不同的。比如上次正在下载过程中，由于某种特殊情况
导致手机重启，那么开机之后，如果有UI，由用户触发，如果没有UI则主动发起
*/
rs_s32 rs_cb_have_ui()
{
	return 0;
}

/**
函数说明：获取第一次自动检测距离开机的时间周期
参数说明：
返回值：返回值为距离开机的毫秒时间
*/
rs_u32 rs_cb_get_first_check_cycle()
{
	//return (90*1000);
	return (20*1000);
}

/**
函数说明：第一次自动检测周期完成之后，每次自动检测的间隔的时间
参数说明：
返回值：返回值为每次自检测的周期的间隔时间
注意:由于定时器最大支持37小时，这里如果需要设置更长时间的定时器，可以采用累积的方式，如果超过24小时，
以24小时为一个周期，累积到需要的时间之后再触发动作
*/
rs_u32 rs_cb_get_auto_check_cycle()
{
	//return (24*60*60*1000);
	return 60*1000;
}


/**
函数说明：自动检测周期到达
参数说明：无
返回值：成功返回RS_ERR_OK, 失败返回其他值
*/
rs_s32 rs_cb_auto_check()
{
	rs_s32 ret = RS_ERR_OK;
	rs_s32 cauType = -1;
	RS_FWDL_STATE state = rs_sdk_getDLCurrentState();

	RS_PORITNG_LOG(RS_LOG_INFO"rs_cb_auto_check, current state is %d \r\n", state);
	if(rs_cb_have_ui() == 0)
	{
		// 没有UI
		if (state == RS_FWDL_STATE_DOWNLOAD_PAUSE)
		{
			// 如果上出现什么异常情况，这次直接下载
			ret = rs_sdk_download_ex(1);
			return ret;
		}
        else if(state == RS_FWDL_STATE_DOWNLOADED)
        {
            // 如果已经下载但未安装，这次直接安装
            ret =  rs_sdk_install_ex();
            return ret;
        }
	}

	//if (rs_sdk_needAutoCheck())
	{
		ret = rs_sdk_check_ex(1); // 函数内部有对状态的判断，如果此时不合适进行检测，函数会退出
	}

	return ret;
}

/**
函数说明：允许检测最大重试次数
参数说明：无
返回值：返回重试次数
*/
rs_s32 rs_cb_get_check_retry_count()
{
	return 2;
}

/**
函数说明：获取下载最大重试次数
参数说明：
返回值：返回重试次数
*/
rs_s32 rs_cb_get_download_retry_count()
{
	return 20;
}

/**
函数说明：获取升级完成上报最大重试次数
参数说明：
返回值：返回重试次数
特别说明：此重试次数需要设置大一点，尽量让上报成功，否则对于后期判断造成影响
*/
rs_s32 rs_cb_get_report_retry_count()
{
	return 5;
}

/**
函数说明：获取下载过程中最大失败测试，rs_cb_get_download_retry_count是在下载过程中如果失败了会自动重试的次数
		如果总次数超过了此函数设定的上限，那么放弃此升级包。
参数说明：
返回值：返回重试次数
*/
rs_s32 rs_cb_get_download_max_fail_count()
{
	return 10;
}

/**
函数说明：获取上报过程中最大失败测试，rs_cb_get_report_retry_count是在上报过程中如果失败了会自动重试的次数。
		如果总次数超过了rs_cb_get_report_max_fail_count函数设定的上限，那么放弃上报。
参数说明：
返回值：返回重试次数
*/
rs_s32 rs_cb_get_report_max_fail_count()
{
	return 10;
}

/**
函数说明：socket在运行过程中的各种超时设置
参数说明：type 代表不同超时类型
			0  gethostbyname的超时设置
			1  connect的超时设置
			2  send的超时设置
			3  recv的超时设置
返回值：返回超时的时间周期，单位为毫秒
备注: RDA的flash写入有可能需要等待很长时间，这个超时间一定要大于rs_flash_operater.c里面等待写入的flash的时间
*/
rs_s32 rs_cb_get_socket_timeout(rs_s32 type)
{
	rs_s32 timeout = 0;

	switch(type)
	{
	case 0: // gethostbyname 超时时间
		timeout = 30*1000;
		break;
	case 1: // connect 超时时间
		timeout = 30*1000;
		break;
	case 2: // send 超时时间
		timeout = 30*1000;
		break;
	case 3: // recv 超时时间
		timeout = 30*1000;
		break;
	case 4: // session 间隔时间
		timeout = 3*1000;
		break;
	case 5: // pdp active 超时时间
		timeout = 60*1000;
		break;
	}

	return timeout;
}


/**
函数说明：获取当前电池剩余的量是否充足
参数说明：
返回值：电量大于等于33%,则返回RS_TRUE,否则RS_FALSE
特别说明:bmt_status.volt电池电压，厂家有可能根据电压判断当前更加精确的
电量剩余情况，比如认为最低为3593500,最高为4253900，分成100分，我们把volt
打印出来作为了解
另外请注意，在MMITask.c的init中马上调用如下借口是不行的，有可能程序还没有初始化
*/
rs_bool rs_sys_bettery_electricity_enough()
{
	return RS_TRUE;
}

/**
函数说明：获取当前可用的空间
参数说明：
返回值：返回当前可用的剩余空间,单位为byte
*/
rs_u32 rs_sys_available_space()
{
	return ((FOTA_RESERVED_LENGTH - MT2625_FLASH_ERASE_BLOCK_SIZE - 2*RS_LOGIC_WRITE_UINIT_SIZE) / RS_LOGIC_WRITE_UINIT_SIZE) * RS_LOGIC_WRITE_UINIT_SIZE ;
}
rs_s32 rs_sdk_get_auto_cau_type()
{
#ifdef FOTA_AUTO_CHECK_AND_UPDATE //全自动，这种模式AT指令无效
	return RS_AUTOCHECK_AUTOUPDATE_MODE;
#else							  //手动 
	#ifdef FOTA_AUTO_UPDATE //手动检测，升级自动
		return RS_MANUALCHECK_AUTOUPDATE_MODE; //接受AT指令连贯完成检测和下载和升级功能
	#else
		return RS_MANUALCHECK_MANUALUPDATE_MODE; //手动检测、手动升级，分两次命令完成
	#endif
#endif
}


#ifdef RS_SUPPORT_UPDATE_DATA_FS

/**
函数说明：获取升级包的下载路径
参数说明：
返回值：返回下载路径
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
如果升级包数据存储于flash中，那么升级信息数据也肯定是要在flash中存储的
因为bootloader需要读取
*/

/**
函数说明：获取升级包的下载存放的flash的地址
参数说明：
返回值：返回存储地址
*/
rs_u32 rs_device_get_update_addr()
{
	return FLASH_UPDATE_ADDR;
}

/**
函数说明：获取升级包信息的存储地址
参数说明：
返回值：返回存储地址
*/
rs_u32 rs_device_get_update_info1_addr()
{
	return RS_CONFIG_UPINFO_ADDR1;
}

/**
函数说明：获取升级包信息的存储地址
参数说明：
返回值：返回存储地址
*/
rs_u32 rs_device_get_update_info2_addr()
{
	return RS_CONFIG_UPINFO_ADDR2;
}

#endif

#ifdef RS_SUPPORT_UPDATE_INFO_FS

/**
函数说明：获取升级包信息的存储路径
参数说明：
返回值：返回存储路径
*/
const rs_s8* rs_device_get_update_info1_file()
{
	return RS_CONFIG_UPINFO_FILE1;
}

/**
函数说明：获取升级包信息的存储路径
参数说明：
返回值：返回存储路径
*/
const rs_s8* rs_device_get_update_info2_file()
{
	return RS_CONFIG_UPINFO_FILE2;
}

#endif



/**
函数说明：获取VIVA的起始地址
参数说明：无
返回值：返回VIVA的起始地址，如果没有VIAVA分区则返回0 
*/
rs_u32 rs_sys_getVIVAStartAddr()
{
	return 0;
}


rs_u32 rs_cb_get_update_result()
{
	return 1;
} 
