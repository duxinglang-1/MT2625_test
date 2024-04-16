#include "rs_datatype.h"
#include "rs_system.h"
#include "rs_state.h"
#include "rs_sdk_api.h"
#include "rs_error.h"
#include "rs_thread.h"
#include "rs_mem.h"
//#include "rs_md5.h"
#include "rs_fs.h"
#include "rs_param.h"
#include "rs_debug.h"
#include "rs_flash_operate.h"

//#include "at.h"
//#include "tm.h"
//#include "At_cmd_gprs.h"
//#include "cfw_prv.h"


// 平台的头文件
#include "apb_proxy.h"
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

void * g_msg_handle = RS_NULL;
static const rs_s8 dec2hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
extern tel_conn_mgr_queue_hdl_t g_rsfota_downoad_msg_queue;
extern rs_s32 s_wait_active_msg;

typedef struct {
    char apn[52];
    char user_name[52];
    char password[52];
}rs_fota_apn_account_t;

typedef struct 
{
	RS_MSG_HANDLER_CALLBACK callback;
	rs_u32 uMsgEvent;
	rs_s32 lParam1;
	rs_s32 lParam2;
} RS_MMI_Msg;

typedef struct RS_TIMER_CALLBACK_DATA_tag
{
	RS_TIMER_CALLBACK callback;
	void* data;
}RS_TIMER_CALLBACK_DATA;

TimerHandle_t short_timer_handle = 0;
RS_TIMER_CALLBACK_DATA short_timer_callback = {0};
TimerHandle_t autocheck_timer_handle = 0;

static rs_fota_apn_account_t g_rsfota_current_apn_account = {{0},{0},{0}};
static unsigned int g_rsfota_app_id = 0;


void rs_sdktimer_callback(TimerHandle_t param);
void rs_sys_autocheck_callback(TimerHandle_t param);

void rs_sys_msg_queue_callback(void* msgData)
{
	if(msgData != 0)
	{
		RS_MMI_Msg* data;

		data = (RS_MMI_Msg *)msgData;
		if (data != RS_NULL && g_msg_handle != RS_NULL)
		{
			data->callback(g_msg_handle, data->uMsgEvent, data->lParam1, data->lParam2);
			rs_free(data);
		}
	}
}

/******************************************************************************************************/
/*
函数说明：系统相关功能初始化
参数说明：无
返回值：成功返回RS_ERR_OK
*/
rs_s32 rs_sys_init()
{
	int errCode = RS_ERR_OK;

	rs_create_thread();

	return errCode;
}

/*
函数说明：获取当前系统的时间
参数说明：buf 返回当前的系统的时间
返回值：
*/
void rs_sys_get_time(rs_s8* buf)
{
	//hal_rtc_status_t ret;
    hal_rtc_time_t time;

    //ret = hal_rtc_get_time(&time);
    //if(HAL_RTC_STATUS_OK != ret) {
       // return false;
    //}
    hal_rtc_get_time(&time);
	rs_sprintf(buf, "%d%d%d%d%d%d ",
					   time.rtc_year, time.rtc_mon, time.rtc_day, time.rtc_hour, time.rtc_min, time.rtc_sec);

    //The value of the RTC year is in a range from 0 to 127. The user has to define the base year and the RTC year is defined
    //as an offset. For example, define the base year to 2000 and assign 15 to RTC year to represent the year of 2015.
    return true;

}

/*
函数说明：获取当前系统的时间
参数说明：buf 返回当前的系统的时间
返回值：
*/
void rs_sys_get_time_ex(rs_s8* buf)
{
	//hal_rtc_status_t ret;
	hal_rtc_time_t time;

	//ret = hal_rtc_get_time(&time);
	//if(HAL_RTC_STATUS_OK != ret) {
	   // return false;
	//}
	hal_rtc_get_time(&time);
	rs_sprintf(buf, "%d%d%d%d%d%d 0",
					   time.rtc_year, time.rtc_mon, time.rtc_day, time.rtc_hour, time.rtc_min, time.rtc_sec);

	//The value of the RTC year is in a range from 0 to 127. The user has to define the base year and the RTC year is defined
	//as an offset. For example, define the base year to 2000 and assign 15 to RTC year to represent the year of 2015.
	return true;
}

/*
函数说明：重启进入升级模式
特别说明：在其他的平台需要实现这个接口，windows模拟环境不需要实现
*/
void rs_sys_reboot()
{

	hal_wdt_config_t wdt_config;
    wdt_config.mode = HAL_WDT_MODE_RESET;
    wdt_config.seconds = 1;
    hal_wdt_init(&wdt_config);
    hal_wdt_software_reset();	
}

/**
函数说明：向消息接收模块发送消息
参数说明：nMsgQParam 消息句柄
uMsgEvent 消息事件
lParam1 消息参数1
lParam2 消息参数2
callback 消息接收模块收到消息之后，调用此接口处理消息
返回值：0表示成功，-1表示失败 
*/
rs_bool rs_sys_msg_queue_send(rs_s32 nMsgQParam, rs_u32 uMsgEvent, rs_s32 lParam1, rs_s32 lParam2, RS_MSG_HANDLER_CALLBACK callback)
{
	rs_bool ret = RS_TRUE;
	RS_MMI_Msg * msg;

	msg = (RS_MMI_Msg*)rs_malloc(sizeof(RS_MMI_Msg));
	if (msg == RS_NULL)
	{
		RS_PORITNG_LOG(RS_LOG_DEBUG"rs_sys_msg_queue_send malloc fail\n");
		return RS_FALSE;
	}

	msg->callback = callback;
	msg->uMsgEvent = uMsgEvent;
	msg->lParam1 = lParam1;
	msg->lParam2 = lParam2;

	rs_post_message_to_thread_with_code(UA_TASK_SDK_MSG_SIGNAL_CODE, msg);

	return ret;
}

/**
函数说明：输入日志到文件或者其他
参数说明：需要输出的日志内容
返回值：无
*/
void rs_cb_printLog(const rs_s8* msg)
{
#if 0 
	if(0 == msg) 
		return;

	AT_TC(g_sw_ATE, "%s", msg);
#endif 
	printf(msg);

}
/**
函数说明：创建消息队列 
参数说明：userHandle 使用者的句柄
返回值：RS_NULL表示失败，否则为成功
*/
rs_s32 rs_sys_msg_queue_create(void* userHandle)
{
	g_msg_handle = userHandle;
	return 0;
}

void rs_sdktimer_callback(TimerHandle_t handle)
{
	if (short_timer_callback.callback != RS_NULL)
	{
		short_timer_callback.callback(short_timer_callback.data);

		short_timer_callback.callback = RS_NULL;
		short_timer_callback.data = RS_NULL;
	}
}


/**
函数说明：启动定时器
参数说明：timerPeriod消息时间
返回值：返回定时器句柄，非0值表示成功，否则为失败 
*/
rs_s32 rs_sys_timer_start(rs_s32 timerPeriod, RS_TIMER_CALLBACK callback, void* data)
{
	if (short_timer_handle == NULL)
	{
		short_timer_handle = xTimerCreate( "rs_short_timer",
	                            (timerPeriod/portTICK_PERIOD_MS),
	                            pdFALSE,
	                            NULL,
	                            rs_sdktimer_callback);

		if (short_timer_handle == NULL)
		{
	        printf("rs_sys_timer_start create fail.\r\n");
	        return NULL;
		}
    }
	else
	{
		if (xTimerIsTimerActive(short_timer_handle) != pdFALSE)
		{
	        xTimerStop(short_timer_handle, 0);
	    }

		xTimerChangePeriod( short_timer_handle, timerPeriod/portTICK_PERIOD_MS, 0 );
	}

    xTimerStart(short_timer_handle, 0);

	return short_timer_handle;
}
/**
函数说明：取消定时器
参数说明：nTimerParam定时器句柄 
返回值：rs_true表示成功，rs_false表示失败 
*/
rs_bool rs_sys_timer_cancel(rs_s32 handle)
{

	rs_bool ret = RS_TRUE;
	rs_bool timerRet = TRUE;


	if(short_timer_handle != handle)
	{
		return RS_FALSE;
	}

	if(pdFAIL == xTimerStop(short_timer_handle, 0))
	{
		return RS_FALSE;
	}


	if (short_timer_callback.callback != RS_NULL)
	{
		short_timer_callback.callback = RS_NULL;
		short_timer_callback.data = RS_NULL;
	}

	return ret;
}


/*
函数说明：自动检测周期到达，执行自动检测
参数说明：
返回值：
注意: 定时器的会掉函数最好能够在0.1s内完成，否则可能有问题
*/
void rs_sys_autocheck_callback(TimerHandle_t handle)
{
	RS_FWDL_STATE state = rs_sdk_getDLCurrentState();
	rs_s32 timerPeriod = 0;
	rs_s32 interCycle = 0;
	
	RS_PORITNG_LOG(RS_LOG_DEBUG"rs_sys_autocheck_callback,state = %d",state);

	interCycle = rs_reg_get_internal_cycle();
	timerPeriod = rs_cb_get_auto_check_cycle();
	if (interCycle != 0)
		timerPeriod = interCycle;
	rs_sys_autocheck_timer_start(timerPeriod);


	if (RS_FWDL_STATE_DOWNLOADED == state)
	{
		// 直接安装,无需网络
		rs_sdk_autoCheck();
	}
	else
	{
		if(!rs_sys_simcard_recognize())
		{
			RS_PORITNG_LOG(RS_LOG_DEBUG"rs_sys_autocheck_callback, no sim status is normal\n");
			return;
		}
		
		// auto check
		rs_sdk_autoCheck();
	}
}


/**
函数说明：启动主程序
参数说明：
返回值：rs_true表示成功，rs_false表示失败 
*/
/*根据不同的平台做不同的启动处理*/
rs_s32 rs_sys_autocheck_timer_start(rs_s32 timerPeriod)
{
	if (autocheck_timer_handle == NULL)
	{
		autocheck_timer_handle = xTimerCreate( "rs_autocheck_timer",
	                            (timerPeriod/portTICK_PERIOD_MS),
	                            pdFALSE,
	                            NULL,
	                            rs_sys_autocheck_callback);

		if (autocheck_timer_handle == NULL)
		{
	        printf("rs_sys_autocheck_timer_start create fail.\r\n");
	        return NULL;
		}
    }
	else
	{
		if (xTimerIsTimerActive(autocheck_timer_handle) != pdFALSE)
		{
	        xTimerStop(autocheck_timer_handle, 0);
	    }

		xTimerChangePeriod( autocheck_timer_handle, timerPeriod/portTICK_PERIOD_MS, 0 );
	}

    xTimerStart(autocheck_timer_handle, 0);

	return autocheck_timer_handle;
}

/**
函数说明：取消自动检测定时器
参数说明：handle定时器句柄 
返回值：rs_true表示成功，rs_false表示失败 
*/
rs_bool rs_sys_autocheck_timer_cancel(rs_s32 handle)
{
	rs_bool ret = TRUE;
	
	if(autocheck_timer_handle != handle)
	{
		return RS_FALSE;
	}

	if(pdFAIL == xTimerStop(short_timer_handle, 0))
	{
		RS_PORITNG_LOG(RS_LOG_DEBUG"kill auto check timer fail\n");
		return RS_FALSE;
	}

	return RS_TRUE;
}


/**
函数说明：判断SIM是否已经插入
参数说明：无
返回值：RS_TRUE表示有效，RS_FALSE表示无效 
*/
rs_bool rs_sys_simcard_insert()
{
	return RS_TRUE;
}

/**
函数说明：判断SIM是否可以识别
参数说明：无
返回值：RS_TRUE表示可以识别，RS_FALSE表示不能识别
*/
rs_bool rs_sys_simcard_recognize()
{
	return 1;
}

/**
函数说明：判断imei是否已经写入
参数说明：无
返回值：RS_TRUE表示已经写入，RS_FALSE表示没有写入
*/
rs_bool rs_sys_imei_valid()
{
	return RS_TRUE;
}

/**
函数说明：获取已经下载差分升级包的md5值
参数说明：deltaSize 升级包的长度
md5String 升级包的md5的值
返回值：RS_TRUE表示成功，RS_FALSE表示失败 
*/
rs_bool rs_sys_get_updatefile_md5_string(rs_s32 deltaSize, rs_s8* md5String)
{
	
	mbedtls_md5_context md5_context;
	uint8_t md5[16] = {0};
	
	rs_u8 buf[RS_PAGE_SIZE];
	rs_s32 readSize = 0;
	unsigned char digest[16] = {0};

#ifdef RS_SUPPORT_UPDATE_DATA_FS
	RS_FILE fileHandle = RS_FS_INVALID;
	const rs_s8* filePath = rs_device_get_update_file();
	rs_s32 i = 0;
	mbedtls_md5_init(&md5_context);
	mbedtls_md5_starts(&md5_context);
	//mbedtls_md5_update(&md5_context, (const unsigned char*)src_data, data_size);
	//mbedtls_md5_finish(&md5_context, md5);
	//mbedtls_md5_free(&md5_context);
	
	for (i=0; i<16; i++) 
	{
		md5String[i * 2] = dec2hex[(md5[i] & 0xF0) >> 4];
		md5String[i * 2 + 1] = dec2hex[md5[i] & 0x0F];
	}
	md5String[32] = 0;
	fileHandle = rs_fs_open(filePath, RS_FS_OPEN_READ);
	if (fileHandle != RS_FS_INVALID)
	{
		rs_s32 fileSize = rs_fs_size(fileHandle);

		rs_s32 i = 0;

		if (deltaSize > fileSize)
			return RS_FALSE;

		while(readSize < fileSize)
		{
			rs_s32 len = 0;
			len = rs_fs_read(fileHandle, buf, RS_PAGE_SIZE);
			if (len < 0)
				return RS_FALSE;

			mbedtls_md5_update(&md5_context, (const unsigned char*)buf, len);

			readSize += len;
		}
		rs_fs_close(fileHandle);
		
		mbedtls_md5_finish(&md5_context, md5);
		mbedtls_md5_free(&md5_context);
		

		for (i=0; i<16; i++) 
		{
			md5String[i * 2] = dec2hex[(md5[i] & 0xF0) >> 4];
			md5String[i * 2 + 1] = dec2hex[md5[i] & 0x0F];
		}
		md5String[32] = 0;
	}
	else
	{
		return RS_FALSE;
	}

	return RS_TRUE;
#else
	rs_u32 flashAddr = 0;
	rs_s32 i = 0;
	rs_s32 retry = 10;

	//RS_MD5Init (&md5Ctx);
	mbedtls_md5_init(&md5_context);
	mbedtls_md5_starts(&md5_context);
	flashAddr = rs_device_get_update_addr();
	while(readSize < deltaSize)
	{
		rs_s32 ret = 0;
		ret = rs_flash_readPage(flashAddr + readSize, buf, RS_PAGE_SIZE);
		if (ret != RS_ERR_OK)
			return RS_FALSE;

		if (deltaSize - readSize > RS_PAGE_SIZE)
		{

//			RS_MD5Update (&md5Ctx, buf, RS_PAGE_SIZE);
			mbedtls_md5_update(&md5_context, (const unsigned char*)buf, RS_PAGE_SIZE);
			readSize += RS_PAGE_SIZE;
		}
		else
		{
			//RS_MD5Update (&md5Ctx, buf, deltaSize - readSize);
			mbedtls_md5_update(&md5_context, (const unsigned char*)buf, deltaSize - readSize);
			readSize = deltaSize;
		}
	}

	//RS_MD5Final (&md5Ctx);
	mbedtls_md5_finish(&md5_context, md5);
	mbedtls_md5_free(&md5_context);
	for (i=0; i<16; i++) 
	{
		md5String[i * 2] = dec2hex[(md5[i] & 0xF0) >> 4];
		md5String[i * 2 + 1] = dec2hex[md5[i] & 0x0F];
	}
	md5String[32] = 0;


	return RS_TRUE;
#endif	

}

rs_bool rs_sys_get_data_md5_string(rs_u8* src_data, rs_s32 data_size, rs_s8* md5String)
{
	mbedtls_md5_context md5_context;
	uint8_t md5[16] = {0};
	rs_s32 i = 0;
	mbedtls_md5_init(&md5_context);
	mbedtls_md5_starts(&md5_context);
	mbedtls_md5_update(&md5_context, (const unsigned char*)src_data, data_size);
	mbedtls_md5_finish(&md5_context, md5);
	mbedtls_md5_free(&md5_context);
	
	for (i=0; i<16; i++) 
	{
		md5String[i * 2] = dec2hex[(md5[i] & 0xF0) >> 4];
		md5String[i * 2 + 1] = dec2hex[md5[i] & 0x0F];
	}
	md5String[32] = 0;
	
	return RS_TRUE;
	
}

rs_u32 rs_sys_get_pid()
{
	return 0;
}


/**
函数说明：读取外部配置文件debug.ini中的内容
返回结果作为系统的日志级别
文件地址在rs_param.h中RS_CONFIG_DEBUGINFO_FILE定义
参数说明：
返回值：返回数字1-5
*/
rs_u32 rs_sys_get_log_level()
{
	rs_u8 buf[10];
	RS_FILE rs_file = -1;
	static rs_u32 log_level = 5;
	static int log_level_unknow = 1;

	/*
	if(log_level_unknow == 0)
	{
		return log_level;
	}
	
	log_level_unknow = 0; //目标是为了以下语句当函数rs_sys_get_log_level第一次被调用时才会执行
	if(rs_fs_exists(RS_CONFIG_DEBUGINFO_FILE))
	{
		rs_file = rs_fs_open(RS_CONFIG_DEBUGINFO_FILE,RS_FS_OPEN_READ);
		if(rs_file != RS_FS_INVALID)
		{//打开成功
			//到文件开始位置
			rs_fs_seek(rs_file,0,RS_FS_SEEK_SET);
			if(rs_fs_read(rs_file,buf,1)>0)
			{
				if(buf[0]>='0' && buf[0]<='9')
				{					
					log_level = buf[0] - 0x30;
				}
			}
			rs_fs_close(rs_file);
		}
	}*/

	return log_level;//数字越高可能的打印的log越多
}

rs_bool rs_system_ppp_active()
{
    tel_conn_mgr_pdp_type_enum pdp_type = TEL_CONN_MGR_PDP_TYPE_IPV4V6;
    tel_conn_mgr_pdp_type_enum activated_pdp_type = TEL_CONN_MGR_PDP_TYPE_NONE;
    tel_conn_mgr_ret_enum ret;

    ret = tel_conn_mgr_activate(TEL_CONN_MGR_BEARER_TYPE_NBIOT,
        TEL_CONN_MGR_SIM_ID_1,
        pdp_type,
        "",
        "",
        "",
        g_rsfota_downoad_msg_queue,
        &g_rsfota_app_id,
        &activated_pdp_type);

    if (TEL_CONN_MGR_RET_OK == ret)
	{
		RS_PORITNG_LOG(RS_LOG_DEBUG"rs_system_ppp_active g_rsfota_app_id is %d\n", g_rsfota_app_id);
        return RS_TRUE;
    }
	else
	{
		s_wait_active_msg = 1;
		RS_PORITNG_LOG(RS_LOG_DEBUG"rs_system_ppp_active waiting for bear ready.");
        return RS_FALSE;
    }
}

rs_bool rs_system_ppp_deactive()
{
	tel_conn_mgr_deactivate(g_rsfota_app_id);
	rs_system_ppp_reset_appid();
	return RS_TRUE;
}

void rs_system_ppp_reset_appid()
{
	g_rsfota_app_id = 0;
}

