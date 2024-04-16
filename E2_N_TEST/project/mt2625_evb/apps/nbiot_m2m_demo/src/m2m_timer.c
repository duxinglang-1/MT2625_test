/* 
* 	Date:2021-01-12
* 	Author:huzhangli
* 	Version : 1.0
* 	Modification:
*/
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
	
	/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
	/* device.h includes */
#include "mt2625.h"

#include "m2m_timer.h"
static T_Apptimer AppTimer[TIMER_ID_MAX]={0};


//开始定时器
void task_start_timer(u16 timerid,u32 wait_ms,void(*func)(void))
{
    if (timerid >=  TIMER_ID_MAX)
		return (-1);
	AppTimer[timerid].enable 		= true;
	AppTimer[timerid].interval 	=   wait_ms;
	AppTimer[timerid].cur_value 	= 0;
	AppTimer[timerid].pHandlers 	= func;
	return 0;
}

//停止定时器
void task_stop_timer(u16 timerid)
{
	if (timerid >=  TIMER_ID_MAX)
		return (-1);
	
	AppTimer[timerid].enable = false;
	AppTimer[timerid].interval = 0;
	AppTimer[timerid].cur_value = 0;
	return 0;
}

//获取定时器状态
u8 task_timer_is_enable(u16 timerid)
{
    return AppTimer[timerid].enable;
}


static void task_timer_handler(void)
{
	u8 timers = 0;
	for (timers = 0; timers < TIMER_ID_MAX; timers++)
	{
		if (AppTimer[timers].enable)
		{
			AppTimer[timers].cur_value += 1000;

			if (AppTimer[timers].cur_value >= AppTimer[timers].interval)
			{
				task_stop_timer(timers);
				AppTimer[timers].pHandlers();
			}
		}
	}
}

#if 1

static TaskHandle_t timer_pro_task = NULL;

void m2m_timer_pro_task()
{
	while(1)
    {
        vTaskDelay(1000/ portTICK_PERIOD_MS);
        task_timer_handler();
		n1_mqtt_timer_pro();
		M2M_Timer_Print();
	}
}


void m2m_timer_pro_task_create()
{
	if(pdPASS != xTaskCreate(m2m_timer_pro_task, "m2m_timer_pro_task", 4000, NULL, TASK_PRIORITY_LOW, &timer_pro_task)){
			return -1;
		}
		return 0;

}

#endif
