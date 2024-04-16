#ifndef _RS_SYSTEM_H_
#define _RS_SYSTEM_H_

#ifdef __cplusplus
  extern "C"
  {
#endif
#include "rs_datatype.h"


//#include "at.h"
typedef enum
{
    OPERATOR_CHINA_MOBILE,
    OPERATOR_CHINA_UNICOM,
    OPERATOR_NOT_SUPPORT
} OperatorType;


/*
����˵����ϵͳ��ع��ܳ�ʼ��
����˵������
����ֵ���ɹ�����RS_ERR_OK
*/
rs_s32 rs_sys_init();

/*
����˵������ȡ��ǰϵͳ��ʱ��
����˵����buf ���ص�ǰ��ϵͳ��ʱ��
����ֵ��
*/
void rs_sys_get_time(rs_s8* buf);

/*
����˵����������������ģʽ
�ر�˵������������ƽ̨��Ҫʵ������ӿڣ�windowsģ�⻷������Ҫʵ��
*/
void rs_sys_reboot();

/**
����˵����������־���ļ���������
����˵������Ҫ�������־����
����ֵ����
*/
void rs_cb_printLog(const rs_s8* msg);


/**
����˵������Ϣ����ģ���յ���Ϣ֮�󣬵��ôλٵ� 
����˵����handle��Ϣ���
          uMsgEvent ��Ϣ�¼�
		 lParam1 ��Ϣ����1
		 lParam2 ��Ϣ����2
����ֵ�� 
*/
typedef  void (*RS_MSG_HANDLER_CALLBACK)(void*handle, rs_u32 uMsgEvent, rs_s32 lParam1, rs_s32 lParam2);

/**
����˵����������Ϣ���� 
����˵����userHandle ʹ���ߵľ��
����ֵ��RS_NULL��ʾʧ�ܣ�����Ϊ�ɹ�
*/
rs_s32 rs_sys_msg_queue_create(void* userHandle);

/**
����˵��������Ϣ����ģ�鷢����Ϣ
����˵����nMsgQParam ��Ϣ���
          uMsgEvent ��Ϣ�¼�
		 lParam1 ��Ϣ����1
		 lParam2 ��Ϣ����2
		 callback ��Ϣ����ģ���յ���Ϣ֮�󣬵��ô˽ӿڴ�����Ϣ
����ֵ��0��ʾ�ɹ���-1��ʾʧ�� 
*/
rs_bool rs_sys_msg_queue_send(rs_s32 nMsgQParam, rs_u32 uMsgEvent, rs_s32 lParam1, rs_s32 lParam2, RS_MSG_HANDLER_CALLBACK callback);


/**
����˵������ʱ������֮����õĻص����� 
����˵����data �û���������
����ֵ�� 
*/
typedef  void (*RS_TIMER_CALLBACK)(void* handle);


/**
����˵����������ʱ��
����˵����timerPeriod��Ϣʱ��
����ֵ�����ض�ʱ���������0ֵ��ʾ�ɹ�������Ϊʧ�� 
*/
rs_s32 rs_sys_timer_start(rs_s32 timerPeriod, RS_TIMER_CALLBACK callback, void* data);

/**
����˵����ȡ����ʱ��
����˵����nTimerParam��ʱ����� 
����ֵ��rs_true��ʾ�ɹ���rs_false��ʾʧ�� 
*/
rs_bool rs_sys_timer_cancel(rs_s32 handle);

/**
����˵��������������
����˵����
����ֵ��rs_true��ʾ�ɹ���rs_false��ʾʧ�� 
*/
/*���ݲ�ͬ��ƽ̨����ͬ����������*/
rs_s32 rs_sys_autocheck_timer_start(rs_s32 timerPeriod);

/**
����˵����ȡ���Զ���ⶨʱ��
����˵����handle��ʱ����� 
����ֵ��rs_true��ʾ�ɹ���rs_false��ʾʧ�� 
*/
rs_bool rs_sys_autocheck_timer_cancel(rs_s32 handle);


/**
����˵������ȡ�Ѿ����ز����������md5ֵ
����˵����deltaSize �������ĳ���
		  md5String ��������md5��ֵ
����ֵ��RS_TRUE��ʾ�ɹ���RS_FALSE��ʾʧ�� 
*/
rs_bool rs_sys_get_updatefile_md5_string(rs_s32 deltaSize, rs_s8* md5String);

rs_u32 rs_sys_get_pid();

rs_bool rs_sys_get_data_md5_string(rs_u8* src_data, rs_s32 data_size, rs_s8* md5String);

void rs_sys_get_time_ex(rs_s8* buf);

/**
����˵������ȡ�ⲿ�����ļ�debug.ini�е�����
		  ���ؽ����Ϊϵͳ����־����
		  �ļ���ַ��rs_param.h��RS_CONFIG_DEBUGINFO_FILE����
����˵����
����ֵ����������0-3
*/
rs_u32 rs_sys_get_log_level();

rs_bool rs_system_ppp_active();

rs_bool rs_system_ppp_deactive();

void rs_system_ppp_reset_appid();

#ifdef __cplusplus
  }
#endif

#endif
