#ifndef __M2M_TIMER_H__
#define __M2M_TIMER_H__

#ifndef u8
typedef	unsigned char u8;/**< used for unsigned 8bit */
typedef	unsigned short u16;/**< used for unsigned 16bit */
typedef	unsigned int u32;/**< used for unsigned 32bit */
typedef	unsigned long u64;/**< used for unsigned 64bit */
#endif

#ifndef bool
#define bool	_Bool
#define true	1
#define false	0
#endif

typedef bool (*pFunc)();

typedef struct 
{
	u8   enable;       /*flg*/
	u32  interval;     /* Set time*/
    u32  cur_value;    /* current value */
	pFunc  		pHandlers;    /* call back function*/
}T_Apptimer;

typedef enum
{
    TaskTimer_100ms = 0x00,                         //GPS读取定时器
    TaskTimer_sec,                                  //秒定时器
	TaskTimer_MQTT_Send_Subscribe,
	TaskTimer_MQTT_Subscribe_Timeout,
	TaskTimer_MQTT_Send_Publish,
	TaskTimer_MQTT_SendTimeout,
	TaskTimer_MQTT_Reconnect,	
	TaskTimer_shutdown_ponit,	
	TaskTimer_mada_off,
	
	TIMER_ID_MAX
}E_Timer_ID;

extern 	void m2m_timer_pro_task_create();
extern	void task_start_timer(u16 timerid,u32 wait_ms,void(*func)(void));
extern	void task_stop_timer(u16 timerid);
extern	u8 task_timer_is_enable(u16 timerid);
#endif
