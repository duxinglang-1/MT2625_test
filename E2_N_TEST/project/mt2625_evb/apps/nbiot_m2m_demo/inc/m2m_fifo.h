#ifndef __H10_FIFO_H__
#define __H10_FIFO_H__
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

#include "ril.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "task_def.h"

#include "hal_gpio.h"
#include "hal_rtc_external.h"
#include "hal_rtc_internal.h"
#include "hal_sleep_driver.h"

#include "memory_attribute.h"

#include "tel_conn_mgr_common.h"
#include "tel_conn_mgr_app_api.h"
#include "tel_conn_mgr_bearer_iprot.h"

#include "hal_eint.h"
#include "hal_platform.h"
#include "timers.h"
#include "apb_proxy.h"
#include "nvdm_modem.h"
#include "nvdm.h"
#include "ril.h"
#include "Hal_keypad.h"
#include "hal_rtc.h"
#include "hal_wdt.h"
#include "hal_gpt.h"
#include "gdi.h"


#define Queue_num  128   /* 队列*/

#define MAX_BAG_INFO_SIZE 		(256)
#define MAX_BAG_FIFO_SUM 		(Queue_num)

typedef struct
{
	uint8_t      		Info[MAX_BAG_INFO_SIZE];
	uint16_t   		   	Len;
}T_BAG_INFO;

typedef struct
{
	uint16_t 			head;     					// 当前的序号
	uint16_t 			tail;     
	uint16_t 			counts; 					// 已有的信息条数
	T_BAG_INFO 			array[MAX_BAG_FIFO_SUM];
}FIFO_INFO;

extern int send_fail_count;

extern T_BAG_INFO * M2M_FIFO_BAG_POP();
extern T_BAG_INFO * M2M_FIFO_BAG_Get();
extern uint16_t M2M_FIFO_BAG_Flash();
extern uint16_t M2M_FIFO_BAG_Counts();


extern void M2M_FIFO_Update_To_Nvram();
extern void M2M_FIFO_Bag_Init();

extern void M2M_Send_Timer_Pro();
#endif

