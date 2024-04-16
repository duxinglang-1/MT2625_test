#include "rs_datatype.h"
#include "rs_system.h"
#include "rs_state.h"
#include "rs_sdk_api.h"
#include "rs_thread.h"
#include "rs_mem.h"
#include "rs_debug.h"
#include "rs_sdk_api_ex.h"
#include "rs_param.h"
#include "rs_notify_user.h"
#include "rs_socket.h"
#include "rs_error.h"


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
#include "task_def.h"
#include "ril.h"



typedef struct {
    uint32_t message_id;
    void *param;
} rs_fota_message_t;

    
tel_conn_mgr_queue_hdl_t g_rsfota_downoad_msg_queue = NULL;
rs_s32 s_wait_active_msg = 0;

extern rs_s32 rs_reg_get_internal_cycle();

rs_s8 g_rsimsi[20] = {0};
rs_s8 g_rsimei[20] = {0};

void rs_async_get_devid(rs_s32 timerPeriod);


// 检查下载的执行入口
void  Uadl_Task_Entry(void *argv)
{
	
	RS_PORITNG_LOG(RS_LOG_DEBUG "start Uadl_Task_Entry \r\n");
	
	rs_async_get_devid(1000);
	
	while(1)
	{			
		void* msgData = 0;
		rs_u32 msgID = 0;
		rs_fota_message_t message = { 0 };
		
		if (xQueueReceive(g_rsfota_downoad_msg_queue, &message, portMAX_DELAY))
		{
		
			msgID = (rs_u32)message.message_id;
			msgData = message.param;

			RS_PORITNG_LOG(RS_LOG_DEBUG"thread recv msg, msgID = 0X%X\n", msgID);


			if(UA_TASK_SDK_MSG_SIGNAL_CODE == msgID)
			{
				rs_sys_msg_queue_callback(msgData);
			}

#ifdef SUPPORT_USER_MSG
			else if (UA_TASK_USER_MSG_SIGNAL_CODE == msgID)
			{
				rs_user_msg_queue_callback(msgData);
			}
#endif
			else
			{
				rs_fota_message_t* pmsg = (rs_fota_message_t*)message.message_id;
				if (MSG_ID_TEL_CONN_MGR_ACTIVATION_RSP == pmsg->message_id)
				{	
					RS_PORITNG_LOG(RS_LOG_DEBUG"tMSG_ID_TEL_CONN_MGR_ACTIVATION_RSP\n");
					if(s_wait_active_msg == 1)
					{
						tel_conn_mgr_activation_rsp_struct *act_msg = (tel_conn_mgr_activation_rsp_struct *)pmsg;
						s_wait_active_msg = 0;
							
						if (act_msg->result == TEL_CONN_MGR_TRUE)
						{
							fota_pdp_event_handle(0);
						}
						else// if(CFW_GPRS_DEACTIVED == nType)
						{
							// 1激活失败，什么也不做，等待下一个周期
							// 2有可能是去激活的消息
							fota_pdp_event_handle(-1);
						}				
					}
					else
					{
						RS_PORITNG_LOG(RS_LOG_DEBUG"MSG_ID_TEL_CONN_MGR_ACTIVATION_RSP, but no wait\n");
					}
				}
				else if (MSG_ID_TEL_CONN_MGR_DEACTIVATION_RSP == pmsg->message_id)
				{
					RS_PORITNG_LOG(RS_LOG_DEBUG"MSG_ID_TEL_CONN_MGR_DEACTIVATION_RSP\n");
					if(s_wait_active_msg == 1)
					{
						tel_conn_mgr_activation_rsp_struct *act_msg = (tel_conn_mgr_activation_rsp_struct *)pmsg;
						s_wait_active_msg = 0;
							
						fota_pdp_event_handle(-1);				
					}
					else
					{
						RS_PORITNG_LOG(RS_LOG_DEBUG"MSG_ID_TEL_CONN_MGR_DEACTIVATION_RSP, but no wait\n");
					}
				}
				else if (MSG_ID_TEL_CONN_MGR_DEACTIVATION_IND == pmsg->message_id)
				{
					RS_PORITNG_LOG(RS_LOG_DEBUG"MSG_ID_TEL_CONN_MGR_DEACTIVATION_IND\n");
					if(s_wait_active_msg == 1)
					{
						tel_conn_mgr_activation_rsp_struct *act_msg = (tel_conn_mgr_activation_rsp_struct *)pmsg;
						s_wait_active_msg = 0;
							
						fota_pdp_event_handle(-1);				
					}
					else
					{
						RS_PORITNG_LOG(RS_LOG_DEBUG"MSG_ID_TEL_CONN_MGR_DEACTIVATION_IND, but no wait\n");
					}
				}
				else
				{
					RS_PORITNG_LOG(RS_LOG_DEBUG"thread recv msg,  but not support \n");
				}
				
			}
		}
	}
}


rs_s32  rs_create_thread()
{
	BaseType_t xReturned;
	RS_PORITNG_LOG(RS_LOG_DEBUG  "start rs_create_thread \r\n"); 
    g_rsfota_downoad_msg_queue = xQueueCreate(RS_FOTA_QUEUE_LENGTH, sizeof(rs_fota_message_t));
    if( g_rsfota_downoad_msg_queue == NULL )
	{
        RS_PORITNG_LOG(RS_LOG_DEBUG"create rsfota mq failed\n");
        return RS_ERR_FAILED;
    }
	
    xReturned = xTaskCreate(Uadl_Task_Entry,
                RS_FOTA_TASK_NAME,
                RS_FOTA_TASK_STACKSIZE / ((uint32_t)sizeof(StackType_t)),
                NULL, 
                RS_FOTA_TASK_PRIORITY, 
                NULL);
	if( xReturned == pdPASS )
    {
        /* The task was created.  Use the task's handle to delete the task. */
        //vTaskDelete( xHandle );
        
		RS_PORITNG_LOG(RS_LOG_DEBUG  "xTaskCreate success  \r\n");
    }
	else
    {
		
		RS_PORITNG_LOG(RS_LOG_DEBUG  "xTaskCreate fail %d   \r\n" ,xReturned );
	}

	//rs_async_get_devid(1000);				
	
	RS_PORITNG_LOG(RS_LOG_DEBUG  "end rs_create_thread \r\n");
	return 0;
}

void rs_post_message_to_thread_with_code(unsigned int msgID,  void* msgData)
{
	if(NULL == g_rsfota_downoad_msg_queue)
	{
		RS_PORITNG_LOG(RS_LOG_DEBUG"message thread not exit\n");
		return;
	}

	rs_fota_message_t message;
	message.message_id = msgID;
    message.param  = msgData;

    xQueueSend(g_rsfota_downoad_msg_queue, &message, 0);
}

int32_t rs_serial_number_callback(ril_cmd_response_t *response)	
{
	ril_serial_number_rsp_t *param = NULL;

	RS_PORITNG_LOG(RS_LOG_DEBUG  "start rs_serial_number_callback\r\n");
	
	if (!response || RIL_RESULT_CODE_OK != response->res_code)
	{
		return 0;
	}

	param = (ril_serial_number_rsp_t *)response->cmd_param;

    if (param && param->value.imei)
    {
        rs_memcpy(g_rsimei, param->value.imei, rs_strlen(param->value.imei));
		RS_PORITNG_LOG(RS_LOG_DEBUG  "rs_imsi_callback g_rsimei = %s\r\n", g_rsimei);
    }

    return 0;
}


int32_t rs_imsi_callback(ril_cmd_response_t *response)
{
	ril_imsi_rsp_t *param = NULL;

	RS_PORITNG_LOG(RS_LOG_DEBUG  "start rs_imsi_callback \r\n");

	if (!response || RIL_RESULT_CODE_OK != response->res_code)
	{
		return 0;
	}

	param = (ril_imsi_rsp_t *)response->cmd_param;

    if (param && param->imsi)
    {
        rs_memcpy(g_rsimsi, param->imsi, rs_strlen(param->imsi));
		RS_PORITNG_LOG(RS_LOG_DEBUG  "rs_imsi_callback g_rsimsi = %s\r\n", g_rsimsi);
    }

    return 0;    
}

void rs_async_get_devid_callback(TimerHandle_t handle)
{
	RS_PORITNG_LOG(RS_LOG_DEBUG "start rs_async_get_devid_callback \r\n");
    ril_request_imsi(RIL_READ_MODE,
                     rs_imsi_callback,
                     NULL);

    ril_request_serial_number(RIL_READ_MODE,
                              RIL_OMITTED_INTEGER_PARAM,
                              rs_serial_number_callback,
                              NULL);
}

void rs_async_get_devid(rs_s32 timerPeriod)
{
	
	RS_PORITNG_LOG(RS_LOG_DEBUG  "start rs_async_get_devid \r\n");
	TimerHandle_t getdevid_timer_handle = xTimerCreate( "rs_getdevid_timer",
                            (timerPeriod/portTICK_PERIOD_MS),
                            pdFALSE,
                            NULL,
                            rs_async_get_devid_callback);

	if (getdevid_timer_handle == NULL)
	{
        printf("rs_async_get_devid create timer fail.\r\n");
        return;
	}

    xTimerStart(getdevid_timer_handle, 0);
	
	RS_PORITNG_LOG(RS_LOG_DEBUG  "end rs_async_get_devid \r\n");
	return;
}


