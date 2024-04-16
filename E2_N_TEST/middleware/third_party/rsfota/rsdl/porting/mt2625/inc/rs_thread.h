#ifndef __RSTHREAD_H__
#define __RSTHREAD_H__

#define RS_FOTA_EVENT_BASE	(0)


//sdk需要处理的消息
#define UA_TASK_SDK_MSG_SIGNAL_CODE  (RS_FOTA_EVENT_BASE)
// user message
#define UA_TASK_USER_MSG_SIGNAL_CODE (RS_FOTA_EVENT_BASE + 2)
// 附着消息
#define UA_TASK_ATT_MSG_CODE (RS_FOTA_EVENT_BASE + 3)
// pdp激活消息
#define UA_TASK_ACT_MSG_CODE (RS_FOTA_EVENT_BASE + 4)
// socket 消息
#define UA_TASK_SOCKET_MSG_CODE (RS_FOTA_EVENT_BASE + 5)

rs_s32  rs_create_thread();
void rs_post_message_to_thread( rs_u32 uMsgEvent, rs_s32 lParam1, rs_s32 lParam2);
#endif