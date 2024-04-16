/* Includes ------------------------------------------------------------------*/
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

/* hal includes */
#include "hal.h"
#include "syslog.h"
#include "ril.h"
#include "h10_mmi.h"
#include "tel_conn_mgr_bearer_iprot.h"
#include "apb_proxy_cmd_handler.h"
#include "m2m_timer.h"

TaskHandle_t   sbit_m2m_ct_task = NULL;
QueueHandle_t  sbit_m2m_ct_queue = NULL;
TimerHandle_t  sbit_m2m_ct_timer;

#define  SBIT_M2M_CT_FIRST_TIME     1
#define  SBIT_M2M_CT_RESEND_TIME    30

log_create_module(sbit_m2m_ct,PRINT_LEVEL_INFO);

#define SBIT_CT_ERR(fmt,arg...)   LOG_E(sbit_m2m_ct, "[SBIT]: "fmt,##arg)
#define SBIT_CT_WARN(fmt,arg...)  LOG_W(sbit_m2m_ct, "[SBIT]: "fmt,##arg)
#define SBIT_CT_DBG(fmt,arg...)   LOG_I(sbit_m2m_ct, "[SBIT]: "fmt,##arg)

#define SBIT_PROXY_M2MCLINEW_CMD_ID 0xE1
#define SBIT_PROXY_M2MCLIDEL_CMD_ID 0xE2
#define SBIT_PROXY_M2MCLISEND_CMD_ID 0xE3
TimerHandle_t sbit_ct_cfun_off_timer=NULL;
TimerHandle_t sbit_m2m_ct_delect_delay_timer=NULL;
bool clear_medicine_remind_flag = false;
char curr_remind_time_str[8] = {0};
int min_next_remin_num = 0;
int medicine_remind_delay = 0;
int medicine_lock_sec = 0;
void sbit_m2m_ct_cfun_off(void);

typedef struct {   
	int msgid;   
	int len;   
	char *para;
}m2m_ct_queue_struct;
#if 1

#define SBIT_PROXY_MQTTNEW_CMD_ID 	 0xF1
#define SBIT_PROXY_MQTTCON_CMD_ID 	 0xF2
#define SBIT_PROXY_MQTTPUB_CMD_ID 	 0xF3
#define SBIT_PROXY_MQTTSUB_CMD_ID 	 0xF4
#define SBIT_PROXY_MQTTDISCON_CMD_ID 0xF5


void n1_mqtt_send_fifo();
void n1_mqtt_reconnect();

/**
 * format: AT+EMQNEW=<server>,<port>,<command_timeout_ms>,<buf_size>[,<CID>]
 * server: MQTT server
 * port: MQTT port
 * command_timeout_ms:  MQTT command timeout (ms)
 * buf_size: MQTT send and read buffer size
 * CID:       interger    PDP context id              [option]
 * Response:  +EMQNEW:mqtt_id\r\nOK\r\n     ERROR\r\n
 */
 //AT+EMQNEW="47.107.51.89","1883",12000,100

int g_mqtt_id=-1;
bool g_is_mqtt_publish = false;
char hex_data_buf[1024]={0};
int g_mqtt_disconnect_time_count = 0;
TimerHandle_t mqtt_subscribe_timer = NULL;
int g_mqtt_send_fail_count = 0;

int g_mqtt_is_subscribe = 0;


void n1_mqtt_subscribe();
int ch_fixed_lenth_parse(u8*buf,u8 fix_len)
{
	u8 tmp_buf[10]={0};
	strncpy(tmp_buf,buf,fix_len);
	return atoi(tmp_buf);
}
int fix_count_len(char*buf, char mode)
{

   int len = 0;
   char *ptr = NULL;
   if(mode==0)
   ptr = strstr(buf,",");
   else   
   ptr = strstr(buf,"}");
   len =  ptr - buf;  
   return len;     


}
extern unsigned int Gutc_time;
void m2m_mqttrec_analysis(char *temp)
{
	 /*    
		{1:1:0:0:111100000119572:S15:0,20221202,070659}
	 */
	 hal_rtc_time_t rtc_time;
	
	 unsigned int local_time = 0,sent_bagtime = 0,rec_bagtime = 0,recafter_time = 0,len = 0;
	 char *temp_point = NULL,*anpoint = NULL;

	dbg_print("m2m_mqttrec_analysis: %s",temp);
	 
     hal_rtc_get_time(&rtc_time);
     local_time = applib_dt_mytime_2_utc_sec(&rtc_time);
	 temp_point = strstr(temp,"S15:");
	 char prtbuf[100] = {0};
	 char temp_buffer[20]={0};
	 if((temp_point != NULL))
	 {
	 	  temp_point=strstr(temp_point,",");
		  dbg_print("111temp_point: %s",temp_point);
		 // temp_point: ,20221202,073857}
 	  
		  if(temp_point != NULL)
		  {
		  	temp_point+=1;
			len = fix_count_len(temp_point,0);
			//rec_bagtime = ch_fixed_lenth_parse(temp_point,len);
			memset(temp_buffer,0,sizeof(temp_buffer));
			strncpy(temp_buffer,temp_point,len);			
			dbg_print("%s",temp_buffer);

			memset(prtbuf,0,sizeof(prtbuf));
			strcat(prtbuf,temp_buffer);
			dbg_print("111prtbuf : %s",prtbuf);
			temp_point += (len+1);
			dbg_print("222temp_point %s",temp_point);
			memset(temp_buffer,0,sizeof(temp_buffer));
			strncpy(temp_buffer,temp_point,6);
			strcat(prtbuf,temp_buffer);
			dbg_print("222prtbuf : %s",prtbuf);
			hal_rtc_chuhuiset_utc_time(prtbuf);	
			ctiot_lwm2m_client_send_response(prtbuf);
			sbit_m2m_ct_send_massege(M2M_send_T15_ACK,NULL,0,1);
		  }
	#if 0	
		  temp_point += 4;
		  len = fix_count_len(temp_point);
		  rec_bagtime  = ch_fixed_lenth_parse(temp_point,len);
		  temp_point += (len+1);
		  len = fix_count_len(temp_point);
		 
		  sent_bagtime	= ch_fixed_lenth_parse(temp_point,len);
		  len = fix_count_len(temp_point);
		  anpoint = strstr(temp_point,"-");
		  if(anpoint==NULL)
		  {
			temp_point +=len+1;
			NVRAM_info.offset_time = ch_fixed_lenth_parse(temp_point,len);
			
	      }
		  else
		  {
			temp_point +=len+2;
			NVRAM_info.offset_time = -ch_fixed_lenth_parse(temp_point,len);
		  }
	  
          recafter_time = sent_bagtime + NVRAM_info.offset_time;
		  //(rec_bagtime + sent_bagtime + local_time - Gutc_time)/2 + offset_bagtime;
          hal_rtc_chuhuiset_utc_time(sent_bagtime,NVRAM_info.offset_time);
  		 sprintf(prtbuf,"mqttrec_analysis:%d,%d,%d,%d,%d,%d",rec_bagtime,sent_bagtime,local_time,Gutc_time,recafter_time,NVRAM_info.offset_time);
  		 SBIT_CT_DBG("%s",prtbuf);
#endif         
	 }
	 
}


void m2m_remove_sending_data_bag()
{
	int i=0;
#if defined(MTK_SEND_QUEUE_FIFO)
		i = get_m2m_send_Queue_Fifo();
		if((i >= 0)&&((strstr(temp_info1.m2m_send_temp_Queue[i],temp_info.send_temp_Queue))!=NULL)&&(strstr(response_data,"AABB0000") != NULL))
		{
			del_m2m_send_Queue_Fifo();
			memset(temp_info.send_temp_Queue, 0, sizeof(temp_info.send_temp_Queue));
		}
#else
		i = get_m2m_send_Queue();
		if((i >= 0)&&((strstr(temp_info1.m2m_send_temp_Queue[i],temp_info.send_temp_Queue))!=NULL))
		{
			memset(temp_info1.m2m_send_temp_Queue[i],0,sizeof(temp_info1.m2m_send_temp_Queue[i]));
			memset(temp_info.send_temp_Queue, 0, sizeof(temp_info.send_temp_Queue));
		}
#endif

}

//AT+EMQNEW=<server>, <port>, <command_timeout_ms>, <bufsize>[, <cid>]
apb_proxy_status_t m2m_mqtt_create(char *ip_addr,uint16_t port,uint32_t command_timeout_ms,uint32_t buf_size, uint32_t cid, int *mqtt_id)
{
    char tem[512] = {0};
    int total;
    apb_proxy_parse_cmd_param_t parm;
	apb_proxy_status_t status;
    memset(&parm,0,sizeof(apb_proxy_parse_cmd_param_t));
    total= sprintf(tem,"AT+EMQNEW=\"%d.%d.%d.%d\",\"%d\",%d,%d",ip_addr[0],ip_addr[1],ip_addr[2],ip_addr[3],port,command_timeout_ms,buf_size);

    parm.string_ptr = tem;
    parm.string_len = total;
    parm.name_len   = strlen("AT+EMQNEW");
    parm.parse_pos  = parm.name_len + 1 ;
    parm.mode       = APB_PROXY_CMD_MODE_EXECUTION;
    parm.cmd_id     = SBIT_PROXY_MQTTNEW_CMD_ID;

	g_mqtt_id = -1;
	return apb_proxy_hdlr_mqtt_new_cmd(&parm);
}



/**
 * format: AT+EMQCON=<mqtt_id>,<version>,<will_flag>,<keepalive_internal>,<cleansession>,<client_id>[,<username>,<password>]
 * mqtt_id: AT+EMQNEW response
 * version: MQTT version
 * will_flag:  MQTT connection packet's will flag set
 * keepalive_internal: MQTT connection packet's keepalive internal set
 * cleansession:       MQTT connection packet's clean session set
 * client_id: MQTT connection packet's client_id set
 * username:MQTT connection packet's username set
 * password: MQTT connection packet's password set
 * Response:  OK\r\n     ERROR\r\n
 *                 OK-receive CONACK
 */
//AT+EMQCON=0,3,"3908b315240c4560bf0fcaeb1d6f6eec",1000,1,0
//AT+EMQCON=0,3,"3908b315240c4560bf0fcaeb1d6f6eec",1000,1,"topic=device/n1/111100000047534,Qos=1,retained=0,message_len=4,message=3838"

//*  AT+EMQCON=<mqtt_id>,<version>,<will_flag>,<keepalive_internal>,<cleansession>,<client_id>[,<username>,<password>]

//AT+EMQCON=<mqtt_id>,<version>,<client_id>,<keepalive_internal>,<cleansession>,<will_flag>[,<username>,<password>]
apb_proxy_status_t m2m_mqtt_connect(uint32_t mqtt_id,char version,char *client_id,uint32_t keepalive_internal,char cleansession,char will_flag,char*username,char*password)
{
    char tem[512] = {0};
    int total;
    apb_proxy_parse_cmd_param_t parm;
	apb_proxy_status_t status;
    memset(&parm,0,sizeof(apb_proxy_parse_cmd_param_t));
	if(username == NULL || password == NULL)
	{
		total= sprintf(tem,"AT+EMQCON=%d,%d,\"%s\",%d,%d,%d",mqtt_id,version,client_id,keepalive_internal,cleansession,will_flag);
	}
	else
	{
		total= sprintf(tem,"AT+EMQCON=%d,%d,\"%s\",%d,%d,%d,\"%s\",\"%s\"",mqtt_id,version,client_id,keepalive_internal,cleansession,will_flag,username,password);
	}
    parm.string_ptr = tem;
    parm.string_len = total;
    parm.name_len   = strlen("AT+EMQCON");
    parm.parse_pos  = parm.name_len + 1 ;
    parm.mode       = APB_PROXY_CMD_MODE_EXECUTION;
    parm.cmd_id     = SBIT_PROXY_MQTTCON_CMD_ID;
    SBIT_CT_DBG("%s",tem);
	dbg_print("m2m_mqtt_connect:%s",tem);
	return apb_proxy_hdlr_mqtt_connect_cmd(&parm);
}

void m2m_mqtt_connect_rsp(int error_code)//MQTT_AT_ERRID_OK
{
	int code = error_code;
	sbit_m2m_ct_send_massege(M2M_MQTT_CONNECT,(char*)&code,4,0);
}


/**
 * format: AT+EMQSUB=<mqtt_id>,<QoS>,<topic>,<message_len>,<message>
 * mqtt_id: AT+EMQNEW response
 * QoS: MQTT pubilsh's Qos
 * topic: MQTT pubilsh's topic
 * message_len: MQTT pulish mesage's length
 * message: MQTT pulish mesage
 * Response:  OK\r\n     ERROR\r\n
 */
//AT+EMQPUB=0,"mytopic",1,0,0,4,"31323334"

void Bcd2Char(uint8_t *pNum_Bcd, uint8_t Num_Bcd_Len, uint8_t* pdata)
{
	uint8_t 	i = 0;
	
	for (i=0; i<Num_Bcd_Len; i++)
	{
		if (((*(pNum_Bcd) >>4 ) & 0x0F) == 0x0F)
			break;
		*(pdata++) =  ((*pNum_Bcd >> 4) & 0x0F) + '0';

	
		if (( *(pNum_Bcd) & 0x0F) == 0x0F)
		{
			*pdata = 0;
			break;
		}
		*(pdata++) =  (*pNum_Bcd & 0x0F) + '0';
	
		pNum_Bcd++;
	}
}

//AT+EMQPUB=0,"device/n1/111100000111298",1,0,0,4,"31323334"
/*
uint32_t mqtt_id = atoi(param_list[0]);
char *topic = param_list[1];
uint32_t QoS = atoi(param_list[2]);
uint32_t retained = atoi(param_list[3]);
uint32_t dup = atoi(param_list[4]);
uint32_t message_len = atoi(param_list[5]);
char *message = param_list[6];
*/
apb_proxy_status_t m2m_mqtt_publish(uint32_t mqtt_id,char *topic,uint16_t message_len,char *hex_message)
{
    char tem[800] = {0};
    int total,i;
    apb_proxy_parse_cmd_param_t parm;
	apb_proxy_status_t status;
    memset(&parm,0,sizeof(apb_proxy_parse_cmd_param_t));
    memset(hex_data_buf,0,sizeof(hex_data_buf));
#if 1
	onenet_at_bin_to_hex(hex_data_buf,hex_message,message_len*2);
	total= sprintf(tem,"AT+EMQPUB=%d,\"%s\",%d,%d,%d,%d,\"%s\"",mqtt_id,topic,1,0,0,message_len*2,hex_data_buf);
#else
total= sprintf(tem,"AT+EMQPUB=%d,\"%s\",%d,%d,%d,%d,\"%s\"",mqtt_id,topic,1,0,0,message_len,hex_message);
#endif
    parm.string_ptr = tem;
    parm.string_len = total;
    parm.name_len   = strlen("AT+EMQPUB");
    parm.parse_pos  = parm.name_len + 1 ;
    parm.mode       = APB_PROXY_CMD_MODE_EXECUTION;
    parm.cmd_id     = SBIT_PROXY_MQTTPUB_CMD_ID;

	dbg_print("m2m_mqtt_publish:%s",tem);
	return apb_proxy_hdlr_mqtt_publish_cmd(&parm);
}


/**
 * format: AT+EMQSUB=<mqtt_id>,<QoS>,<topic>
 * mqtt_id: AT+EMQNEW response
 * QoS: MQTT subscribe's Qos
 * topic: MQTT subscribe's topic
 * Response:  OK\r\n     ERROR\r\n
 */
apb_proxy_status_t m2m_mqtt_sublish(uint32_t mqtt_id,char *topic,uint32_t QoS)
{
    char tem[512] = {0};
    int total,i;
    apb_proxy_parse_cmd_param_t parm;
	apb_proxy_status_t status;
    memset(&parm,0,sizeof(apb_proxy_parse_cmd_param_t));
    total= sprintf(tem,"AT+EMQSUB=%d,\"%s\",%d",mqtt_id,topic,QoS);
    parm.string_ptr = tem;
    parm.string_len = total;
    parm.name_len   = strlen("AT+EMQSUB");
    parm.parse_pos  = parm.name_len + 1 ;
    parm.mode       = APB_PROXY_CMD_MODE_EXECUTION;
    parm.cmd_id     = SBIT_PROXY_MQTTSUB_CMD_ID;

    SBIT_CT_DBG("%s",tem);
	return apb_proxy_hdlr_mqtt_subscribe_cmd(&parm);
}

//+EMQPUB: <mqtt_id>, <topic>, <QoS>, <retained>, <dup>, <message_len>, <message>char retained;
char msg_tmp_buf[500]={0};
char msg_tmp_buf2[500]={0};

void m2m_mqtt_message_handler(uint32_t mqtt_id, char *topic,uint32_t qoS,char retained,char dup,uint32_t message_len,char *message)
{
	memset(msg_tmp_buf,0,sizeof(msg_tmp_buf));
	memset(msg_tmp_buf2,0,sizeof(msg_tmp_buf2));
	memcpy(msg_tmp_buf2,message,message_len);
	sprintf(msg_tmp_buf,"%d,%s,%d,%d,%s",mqtt_id,topic,dup,message_len,msg_tmp_buf2);
	dbg_print("message_handler:%s",msg_tmp_buf);
	sbit_m2m_ct_send_massege(M2M_MQTT_DATA_IND,message,message_len,0);
	task_stop_timer(TaskTimer_MQTT_SendTimeout);
}

//device/n1/111100000019301,0,56,{1:1:0:232322:111100000019301:S29:china sichuan chegndu}

void m2m_test_message_handler()
{
	m2m_mqtt_message_handler(g_mqtt_id,"device/n1/111100000019301",0,0,0,59,"{1:1:0:232322:111100000019301:S29:China SiChuan CHENGDU123}");
}


void m2m_test_adress_bag()
{
	char *ptr = "{1:1:0:232322:111150000015254:S29:WIFI          Liverpool     Street        Station, 19a  Liverpool St, London EC2M   7PY, UK }";

	ctiot_lwm2m_command_address(ptr,0);
}

void m2m_test_adress_bag2()
{
	char *ptr = "{1:1:0:232322:111150000015254:S29:WIFI          Columbusstra?e15, 40549     Düsseldorf,   Germany }";

	ctiot_lwm2m_command_address(ptr,0);
}


void m2m_mqtt_disconect_msg_pro(uint32_t mqtt_id)
{
	dbg_print("mqtt_at_disconnect:%d,%d",mqtt_id,g_mqtt_id);
	if(mqtt_id == g_mqtt_id)
	{
		sbit_m2m_ct_send_massege(M2M_MQTT_DISCONNECT,NULL,0,0);
	}
}

apb_proxy_status_t m2m_mqtt_disconect(uint32_t mqtt_id)
{
    char tem[512] = {0};
    int total;
    apb_proxy_parse_cmd_param_t parm;
	apb_proxy_status_t status;
    memset(&parm,0,sizeof(apb_proxy_parse_cmd_param_t));
    total= sprintf(tem,"AT+EMQDISCON=%d",mqtt_id);

    parm.string_ptr = tem;
    parm.string_len = total;
    parm.name_len   = strlen("AT+EMQNEW");
    parm.parse_pos  = parm.name_len + 1 ;
    parm.mode       = APB_PROXY_CMD_MODE_EXECUTION;
    parm.cmd_id     = SBIT_PROXY_MQTTDISCON_CMD_ID;
	if(mqtt_id == g_mqtt_id)
	{
		g_mqtt_id = -1;
	}
	return apb_proxy_hdlr_mqtt_disconnect_cmd(&parm);
	
}

void m2m_mqtt_protocol_receive(char *buf,int len)
{
	temp_info.network_time_cumulative=0;// kyb add 2022-0804
	sbit_m2m_ct_send_massege(M2M_MQTT_TCP_DATA_IND,buf,len,0);
	g_mqtt_is_subscribe = 1;
	g_mqtt_disconnect_time_count = 0;
	task_stop_timer(TaskTimer_MQTT_SendTimeout);
	task_stop_timer(TaskTimer_MQTT_Reconnect);
	task_stop_timer(TaskTimer_MQTT_Subscribe_Timeout);
	if(!task_timer_is_enable(TaskTimer_MQTT_Send_Publish))
	{
		task_start_timer(TaskTimer_MQTT_Send_Publish,2000,n1_mqtt_send_fifo);
	}
	
	task_start_timer(TaskTimer_MQTT_Send_Subscribe,300*1000,n1_mqtt_subscribe);//180 
}


char* flag_ptr = "0123456789abcdefghijklmnopqrstuvwxyz";

char g_will_flag_buf[33]={0};



char* n1_gen_will_flag()
{
	int i=0;
	for(i=0;i<32;i++)
	{
		g_will_flag_buf[i] =*(flag_ptr + (rand() % 36));
	}
	return g_will_flag_buf;
}

//AT+EMQNEW="47.107.51.89","1883",12000,100
apb_proxy_status_t n1_mqtt_create()
{
#if 0//aliyun server
	char server_addr[4]={47,107,51,89};
	short port = 1883;
#else//england server
char server_addr[4]={18,135,53,53};
short port = 1883;
#endif


	apb_proxy_status_t ret = 0;
	dbg_print("mqtt_create:%d",g_mqtt_id);
	if(g_mqtt_id != -1)
	{
		n1_mqtt_disconnect(g_mqtt_id);
		vTaskDelay(100);
	}
	ret = m2m_mqtt_create(server_addr,port,30000,1000,0,NULL);
	sbit_m2m_ct_send_massege(M2M_MQTT_CREATE,NULL,0,0);
	ctiot_lwm2m_client_send_response("n1_mqtt_create");
	return ret;
}

//AT+EMQCON=0,3,"3908b315240c4560bf0fcaeb1d6f6eef",1000,1,"topic=device/n1/111100000111298,Qos=1,retained=0,message_len=4,message=3838","admin123","admin123"
apb_proxy_status_t n1_mqtt_connect()
{
	return m2m_mqtt_connect(g_mqtt_id,4,temp_info.imei_num,1000,1,0,"admin123","admin123");
}

void n1_mqtt_publish_timeout()
{
	dbg_print("publish time out");

	if(++g_mqtt_send_fail_count >= 5)
	{
		m2m_remove_sending_data_bag();
		n1_mqtt_disconnect();
	}
	else
	{
		n1_mqtt_send_fifo();
	}
}

//AT+EMQPUB=0,"device/n1/111100000111298",1,0,0,4,"31323334"
void n1_mqtt_publish(char *data)
{
	if(g_mqtt_id != -1)
	{
		m2m_mqtt_publish(g_mqtt_id,"device/n1/pushdata",strlen(data),data);
		g_is_mqtt_publish = true;
		task_start_timer(TaskTimer_MQTT_Send_Subscribe,10*1000,n1_mqtt_subscribe);
		task_start_timer(TaskTimer_MQTT_SendTimeout,30*1000,n1_mqtt_publish_timeout);
	}
}

//AT+EMQSUB=0,"device/n1/111100000111298",1
void n1_mqtt_subscribe()
{
	char tmp_buf[50]={0};
	sprintf(tmp_buf,"device/n1/%s",temp_info.imei_num);
	//xTimerStop(mqtt_subscribe_timer, 0);
	dbg_print("----n1_mqtt_subscribe:%s",tmp_buf);
	task_start_timer(TaskTimer_MQTT_Subscribe_Timeout,60*1000,n1_mqtt_reconnect);

	m2m_mqtt_sublish(g_mqtt_id,tmp_buf,1);
}

void n1_mqtt_disconnect(uint32_t mqtt_id)
{
	dbg_print("n1_mqtt_disconnect:param:%d, %d",mqtt_id,g_mqtt_id);
	m2m_mqtt_disconect(mqtt_id);
}

void n1_mqtt_reconnect()
{
	n1_mqtt_disconnect(g_mqtt_id);
}

void n1_mqtt_send_fifo()
{
	int i = 0;
	if(g_is_mqtt_publish == false && g_mqtt_is_subscribe == 1)
	{
		#if defined(MTK_SEND_QUEUE_FIFO)
		i = get_m2m_send_Queue_Fifo();
		#else
		i = get_m2m_send_Queue();
		#endif
		//dbg_print("n1_mqtt_send_fifo:mqtt id:%d,count:%d",g_is_mqtt_publish,i);
		if(i >= 0)
		{
			ctiot_lwm2m_client_send_response(temp_info1.m2m_send_temp_Queue[i]);

			temp_info.send_record_flag[i]++;
			memset(temp_info.send_temp_Queue, 0, sizeof(temp_info.send_temp_Queue));
			strcpy(temp_info.send_temp_Queue,temp_info1.m2m_send_temp_Queue[i]);
			str_to_hex(temp_info1.m2m_send_temp_Queue[i],temp_info.data_buffer_hex);
			if((temp_info.registered_network_flag==1)
				&&((temp_info.csq_num>=5)&&(temp_info.csq_num<33)))
			{
				static bool is_frst = 1;
				if(is_frst)
				{
					is_frst = 0;
					//n1_mqtt_publish(H10_synctime_T53());
				}
				else
				{
					dbg_print("n1_mqtt_publish--> %s\r\n",temp_info1.m2m_send_temp_Queue[i]);
					n1_mqtt_publish(temp_info1.m2m_send_temp_Queue[i]);
				}
			}
		}
	}
}
int g_work_time = 0;

void n1_mqtt_timer_pro()
{
	static int i=0;
	static bool is_first = 1;
	g_work_time++;
	if(++g_mqtt_disconnect_time_count >= 8*60)//8分钟没数据,则重连
	{
		n1_mqtt_disconnect(g_mqtt_id);
		//vTaskDelay(100);
		n1_mqtt_create();
		g_mqtt_disconnect_time_count = 0;
	}

	if(temp_info.csq_num>=5 && temp_info.registered_network_flag == 1 && is_first == 1)
	{
		is_first = 0;
		n1_mqtt_create();
	}
	
	if(++i%5 == 0)
	{
		dbg_print("time:%d,count:%d,csq:[%d,net:%d,is publish:%d],dis time:%d,mqtt id:%d,%s",i,get_m2m_Queue_count(),temp_info.csq_num,temp_info.registered_network_flag,g_is_mqtt_publish,g_mqtt_disconnect_time_count,g_mqtt_id,temp_info.imei_num);
	}

	if(i%10 == 0)
	{
		n1_mqtt_send_fifo();
	}
}

#endif

#if 0
apb_proxy_status_t apb_mcgdefcont_cmd_hdlr(apb_proxy_parse_cmd_param_t *parse_cmd)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    apb_proxy_at_cmd_result_t response = {0};
    char *param_buffer;
    int32_t mqtt_error;
    uint32_t mqtt_id;
    char response_data[MQTT_AT_RESPONSE_DATA_LEN];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MQTT_AT_LOGI("%s", parse_cmd->string_ptr);

    switch (parse_cmd->mode) {
        case APB_PROXY_CMD_MODE_EXECUTION:
            param_buffer = (char *)pvPortMalloc(parse_cmd->string_len);
            if (param_buffer == NULL) {
                MQTT_AT_LOGE("memory error");
                mqtt_error = MQTT_AT_ERRID_MEMORY_ERROR;
            } else {
                char *cmd_string = strchr(parse_cmd->string_ptr, '=');
                if (!cmd_string || *(cmd_string + 1) == '\0') {
                    MQTT_AT_LOGE("no parameter");
                    mqtt_error = MQTT_AT_ERRID_PARAMETER_ERROR;
                } else {
                    char *param_list[MQTT_AT_CMD_PARAM_NUM];
                    uint32_t param_num = onenet_at_parse_cmd(++cmd_string, param_buffer, param_list, MQTT_AT_CMD_PARAM_NUM);

                    // AT+EMQNEW=<server>, <port>, <command_timeout_ms>, <bufsize>[, <cid>]
                    if (param_num < 4) {
                        MQTT_AT_LOGE("parameter too short");
                        mqtt_error = MQTT_AT_ERRID_PARAMETER_ERROR;
                    } else {
                        char *server = param_list[0];
                        char *port = param_list[1];
                        uint32_t command_timeout_ms = atoi(param_list[2]);
                        uint32_t bufsize = atoi(param_list[3]);
                        uint32_t cid = -1;
                        if (param_num >= 5) {
                            cid = atoi(param_list[4]);
                        }
                        mqtt_error = mqtt_at_new(server, port, command_timeout_ms, bufsize, cid, &mqtt_id);
                    }
                }
            }

            if (param_buffer != NULL) {
                vPortFree(param_buffer);
            }

            break;

        default:
            MQTT_AT_LOGE("not support");
            mqtt_error = MQTT_AT_ERRID_NOT_SUPPORT;
            response.result_code = APB_PROXY_RESULT_ERROR;
            break;
    }

    if (mqtt_error == MQTT_AT_ERRID_OK) {
        sprintf(response_data, "+EMQNEW: %d", (int)mqtt_id);
        response.pdata = response_data;
        response.length = strlen(response.pdata);
		g_mqtt_id = mqtt_id;
    }

    response.cmd_id = parse_cmd->cmd_id;
    apb_proxy_send_at_cmd_result(&response);

    return APB_PROXY_STATUS_OK;
}


apb_proxy_status_t m2m_set_APN()
{
    char tem[512] = {0};
    int total;
    apb_proxy_parse_cmd_param_t parm;
	apb_proxy_status_t status;
    memset(&parm,0,sizeof(apb_proxy_parse_cmd_param_t));
    total= sprintf(tem,"AT*MCGDEFCONT=\"IP\",\"arkessalp.com\"");

    parm.string_ptr = tem;
    parm.string_len = total;
    parm.name_len   = strlen("AT*MCGDEFCONT");
    parm.parse_pos  = parm.name_len + 1 ;
    parm.mode       = APB_PROXY_CMD_MODE_EXECUTION;
    parm.cmd_id     = 0xf8;

	return apb_mcgdefcont_cmd_hdlr(&parm);
}

#endif

void clear_medicine_remind(void)
{
	memset(&NVRAM_info.Monday, 0, sizeof(NVRAM_info.Monday));
	memset(&NVRAM_info.Tuesday, 0, sizeof(NVRAM_info.Tuesday));
	memset(&NVRAM_info.Wednesday, 0, sizeof(NVRAM_info.Wednesday));
	memset(&NVRAM_info.Thursday, 0, sizeof(NVRAM_info.Thursday));
	memset(&NVRAM_info.Friday, 0, sizeof(NVRAM_info.Friday));
	memset(&NVRAM_info.Saturday, 0, sizeof(NVRAM_info.Saturday));
	memset(&NVRAM_info.Sunday, 0, sizeof(NVRAM_info.Sunday));
    SBIT_CT_DBG("=========clear_medicine_remind");
}

bool check_medicine_remind(void)
{
	bool ret = false;
	hal_rtc_time_t curtime; 
	uint8_t rtc_week = 0;								 
	int i = 0;
	char time_str[8] = {0};

	hal_rtc_get_time(&curtime);//获取rtctime

	

	rtc_week = CaculateWeekDay(curtime.rtc_year+2000,curtime.rtc_mon,curtime.rtc_day);
	sprintf(time_str, "%02d",curtime.rtc_hour);
	sprintf(&time_str[2], "%02d",curtime.rtc_min);
	if(curtime.rtc_year < 18)
	{
		ret = false;
	}
	else
	{
		switch(rtc_week)
		{
			case 0:
				for(i = 0; i < REMIND_MAX; i++)
				{
					if(strlen(NVRAM_info.Sunday.remind_time[i]) > 0)
					{
						if(strstr(time_str,NVRAM_info.Sunday.remind_time[i]) != NULL)
						{
							ret = true;
							break;
						}
					}
				}
				break;
 
			case 1:
				for(i = 0; i < REMIND_MAX; i++)
				{
					if(strlen(NVRAM_info.Monday.remind_time[i]) > 0)
					{
						if(strstr(time_str,NVRAM_info.Monday.remind_time[i]) != NULL)
						{
							ret = true;
							break;
						}
					}
				}
				break;

			case 2:
				for(i = 0; i < REMIND_MAX; i++)
				{
					if(strlen(NVRAM_info.Tuesday.remind_time[i]) > 0)
					{
						if(strstr(time_str,NVRAM_info.Tuesday.remind_time[i]) != NULL)
						{
							ret = true;
							break;
						}
					}
				}
				break;

			case 3:
				for(i = 0; i < REMIND_MAX; i++)
				{
					if(strlen(NVRAM_info.Wednesday.remind_time[i]) > 0)
					{
						if(strstr(time_str,NVRAM_info.Wednesday.remind_time[i]) != NULL)
						{
							ret = true;
							break;
						}
					}
				}
				break;
				
			case 4:
				for(i = 0; i < REMIND_MAX; i++)
				{
					if(strlen(NVRAM_info.Thursday.remind_time[i]) > 0)
					{
						if(strstr(time_str,NVRAM_info.Thursday.remind_time[i]) != NULL)
						{
							ret = true;
							break;
						}
					}
				}
				break;

			case 5:
				for(i = 0; i < REMIND_MAX; i++)
				{
					if(strlen(NVRAM_info.Friday.remind_time[i]) > 0)
					{
						if(strstr(time_str,NVRAM_info.Friday.remind_time[i]) != NULL)
						{
							ret = true;
							break;
						}
					}
				}
				break;

			case 6:
				for(i = 0; i < REMIND_MAX; i++)
				{
					if(strlen(NVRAM_info.Saturday.remind_time[i]) > 0)
					{
						if(strstr(time_str,NVRAM_info.Saturday.remind_time[i]) != NULL)
						{
							ret = true;
							break;
						}
					}
				}
				break;
			default:
				break;
				
		}
	}
	return ret;
}

medicine_time_t *get_curr_date_medicine_remind(void)
{
	hal_rtc_time_t curtime; 
	uint8_t rtc_week = 0;	
	medicine_time_t *ret_p = NULL;
	bool ret = false;
	hal_rtc_get_time(&curtime);//获取rtctime
    

	
	rtc_week = CaculateWeekDay(curtime.rtc_year+2000,curtime.rtc_mon,curtime.rtc_day);
	
	if(check_medicine_remind() == true)
	{
		switch(rtc_week)
		{
			case 0:
				ret_p = &NVRAM_info.Sunday;
				break;
			case 1:
				ret_p = &NVRAM_info.Monday;
				break;
			case 2:
				ret_p = &NVRAM_info.Tuesday;
				break;
			case 3:
				ret_p = &NVRAM_info.Wednesday;
				break;
			case 4:
				ret_p = &NVRAM_info.Thursday;
				break;
			case 5:
				ret_p = &NVRAM_info.Friday;
				break;
			case 6:
				ret_p = &NVRAM_info.Saturday;
				break;
			default:
				break;
		}
	}
	return ret_p;
}

int get_curr_index_medicine_remind(void)
{
	medicine_time_t *curr_date = NULL;
	int i = 0;
	hal_rtc_time_t curtime; 
	uint8_t rtc_week = 0;	
	char time_str[8] = {0};
	int ret = -1;
	curr_date = get_curr_date_medicine_remind();
	if(curr_date != NULL)
	{
		hal_rtc_get_time(&curtime);//获取rtctime

		rtc_week = CaculateWeekDay(curtime.rtc_year+2000,curtime.rtc_mon,curtime.rtc_day);
		sprintf(time_str, "%02d",curtime.rtc_hour);
		sprintf(&time_str[2], "%02d",curtime.rtc_min);
		for(i = 0; i < REMIND_MAX; i++)
		{
			if(strlen(curr_date->remind_time[i]) > 0)
			{
				if(strstr(time_str,curr_date->remind_time[i]) != NULL)
				{
					ret = i;
					break;
				}
			}
		}

	}
	return ret;
}

void get_curr_time_medicine_remind(char *time_buff)
{
	medicine_time_t *curr_date = NULL;
	int i = 0;
	hal_rtc_time_t curtime;
	uint8_t rtc_week = 0;	
	char time_str[8] = {0};
	int ret = -1;

	curr_date = get_curr_date_medicine_remind();
	if(curr_date != NULL)
	{

       
		hal_rtc_get_time(&curtime);//获取rtctime
       
		rtc_week = CaculateWeekDay(curtime.rtc_year+2000,curtime.rtc_mon,curtime.rtc_day);
		sprintf(time_str, "%02d",curtime.rtc_hour);
		sprintf(&time_str[2], "%02d",curtime.rtc_min);
		for(i = 0; i < REMIND_MAX; i++)
		{
			if(strlen(curr_date->remind_time[i]) > 0)
			{
				if(strstr(time_str,curr_date->remind_time[i]) != NULL)
				{
					memcpy(time_buff, time_str, sizeof(time_str));
					break;
				}
			}
		}

	}
	return ret;
}

int get_min_next_medicine_remind(void)
{
	medicine_time_t *curr_date = NULL;
	medicine_time_t *nv_date = NULL;
	int index = -1;
	hal_rtc_time_t curtime;
	uint8_t rtc_week = 0;
	int i = 0;
	
	char curr_remind_time[8] = {0};
	char next_remind_time[8] = {0};
	int ret = 0;
   
	
	hal_rtc_get_time(&curtime);//获取rtctime
	
	
	rtc_week = CaculateWeekDay(curtime.rtc_year+2000,curtime.rtc_mon,curtime.rtc_day);
	curr_date = get_curr_date_medicine_remind();
	if(curr_date != NULL)
	{
		index = get_curr_index_medicine_remind();
		memcpy(curr_remind_time, curr_date->remind_time[index], sizeof(curr_remind_time));
		switch(rtc_week)
		{
			case 0:
				if((rtc_week == 0) && (index != -1))
				{
					i = index;
					i++;
				}
				else
				{
					i = 0;
				}
				nv_date = &NVRAM_info.Sunday;
				for(; i < REMIND_MAX; i++)
				{
					if(strlen(nv_date->remind_time[i]) > 0)
					{
						memcpy(next_remind_time, nv_date->remind_time[i], sizeof(next_remind_time));
						ret = (next_remind_time[0] - curr_remind_time[0])*10*60 + (next_remind_time[1] - curr_remind_time[1])*60 
							+ (next_remind_time[2] - curr_remind_time[2])*10 + (next_remind_time[3] - curr_remind_time[3]) + (0 - rtc_week) *24*60;
						break;
					}
				}
				if(ret != 0)
				{
					break;
				}
			case 1:
				if((rtc_week == 1) && (index != -1))
				{
					i = index;
					i++;
				}
				else
				{
					i = 0;
				}
				nv_date = &NVRAM_info.Monday;
				for(; i < REMIND_MAX; i++)
				{
					if(strlen(nv_date->remind_time[i]) > 0)
					{
						memcpy(next_remind_time, nv_date->remind_time[i], sizeof(next_remind_time));
						ret = (next_remind_time[0] - curr_remind_time[0])*10*60 + (next_remind_time[1] - curr_remind_time[1])*60 
							+ (next_remind_time[2] - curr_remind_time[2])*10 + (next_remind_time[3] - curr_remind_time[3]) + (1 - rtc_week) *24*60;
						break;
					}
				}
				if(ret != 0)
				{
					break;
				}
			case 2:
				if((rtc_week == 2) && (index != -1))
				{
					i = index;
					i++;
				}
				else
				{
					i = 0;
				}
				nv_date = &NVRAM_info.Tuesday;
				for(; i < REMIND_MAX; i++)
				{
					if(strlen(nv_date->remind_time[i]) > 0)
					{
						memcpy(next_remind_time, nv_date->remind_time[i], sizeof(next_remind_time));
						ret = (next_remind_time[0] - curr_remind_time[0])*10*60 + (next_remind_time[1] - curr_remind_time[1])*60 
							+ (next_remind_time[2] - curr_remind_time[2])*10 + (next_remind_time[3] - curr_remind_time[3]) + (2 - rtc_week) *24*60;
						break;
					}
				}
				if(ret != 0)
				{
					break;
				}
			case 3:
				if((rtc_week == 3) && (index != -1))
				{
					i = index;
					i++;
				}
				else
				{
					i = 0;
				}
				nv_date = &NVRAM_info.Wednesday;
				for(; i < REMIND_MAX; i++)
				{
					if(strlen(nv_date->remind_time[i]) > 0)
					{
						memcpy(next_remind_time, nv_date->remind_time[i], sizeof(next_remind_time));
						ret = (next_remind_time[0] - curr_remind_time[0])*10*60 + (next_remind_time[1] - curr_remind_time[1])*60 
							+ (next_remind_time[2] - curr_remind_time[2])*10 + (next_remind_time[3] - curr_remind_time[3]) + (3 - rtc_week) *24*60;
						break;
					}
				}
				if(ret != 0)
				{
					break;
				}
			case 4:
				if((rtc_week == 4) && (index != -1))
				{
					i = index;
					i++;
				}
				else
				{
					i = 0;
				}
				nv_date = &NVRAM_info.Thursday;
				for(; i < REMIND_MAX; i++)
				{
					if(strlen(nv_date->remind_time[i]) > 0)
					{
						memcpy(next_remind_time, nv_date->remind_time[i], sizeof(next_remind_time));
						ret = (next_remind_time[0] - curr_remind_time[0])*10*60 + (next_remind_time[1] - curr_remind_time[1])*60 
							+ (next_remind_time[2] - curr_remind_time[2])*10 + (next_remind_time[3] - curr_remind_time[3]) + (4 - rtc_week) *24*60;
						break;
					}
				}
				if(ret != 0)
				{
					break;
				}
			case 5:
				if((rtc_week == 5) && (index != -1))
				{
					i = index;
					i++;
				}
				else
				{
					i = 0;
				}
				nv_date = &NVRAM_info.Friday;
				for(; i < REMIND_MAX; i++)
				{
					if(strlen(nv_date->remind_time[i]) > 0)
					{
						memcpy(next_remind_time, nv_date->remind_time[i], sizeof(next_remind_time));
						ret = (next_remind_time[0] - curr_remind_time[0])*10*60 + (next_remind_time[1] - curr_remind_time[1])*60 
							+ (next_remind_time[2] - curr_remind_time[2])*10 + (next_remind_time[3] - curr_remind_time[3]) + (5 - rtc_week) *24*60;
						break;
					}
				}
				if(ret != 0)
				{
					break;
				}
			case 6:
				if((rtc_week == 6) && (index != -1))
				{
					i = index;
					i++;
				}
				else
				{
					i = 0;
				}
				nv_date = &NVRAM_info.Saturday;
				for(; i < REMIND_MAX; i++)
				{
					if(strlen(nv_date->remind_time[i]) > 0)
					{
						memcpy(next_remind_time, nv_date->remind_time[i], sizeof(next_remind_time));
						ret = (next_remind_time[0] - curr_remind_time[0])*10*60 + (next_remind_time[1] - curr_remind_time[1])*60 
							+ (next_remind_time[2] - curr_remind_time[2])*10 + (next_remind_time[3] - curr_remind_time[3]) + (6 - rtc_week) *24*60;
						break;
					}
				}
				if(ret != 0)
				{
					break;
				}
			default:
				break;
		}

		if(ret == 0)
		{
			switch(0)
			{
				case 0:
					i = 0;
					nv_date = &NVRAM_info.Sunday;
					for(; i < REMIND_MAX; i++)
					{
						if((rtc_week == 0)&&(i == index))
						{
							if(nv_date->repeat_flag[i] == true)
							{
								ret = 60*24*7;
							}
							else
							{
								ret = 0;
							}
							break;
						}
						if(strlen(nv_date->remind_time[i]) > 0)
						{
							memcpy(next_remind_time, nv_date->remind_time[i], sizeof(next_remind_time));
							ret = 60*24*(7-rtc_week+0)+(next_remind_time[0] - curr_remind_time[0])*10*60 + (next_remind_time[1] - curr_remind_time[1])*60 
								+ (next_remind_time[2] - curr_remind_time[2])*10 + (next_remind_time[3] - curr_remind_time[3]);
							break;
						}
					}
					if(ret != 0)
					{
						break;
					}
					if(rtc_week == 0)
					{
						break;
					}
				case 1:
					i = 0;
					nv_date = &NVRAM_info.Monday;
					for(; i < REMIND_MAX; i++)
					{
						if((rtc_week == 1)&&(i == index))
						{
							if(nv_date->repeat_flag[i] == true)
							{
								ret = 60*24*7;
							}
							else
							{
								ret = 0;
							}
							break;
						}
						if(strlen(nv_date->remind_time[i]) > 0)
						{
							memcpy(next_remind_time, nv_date->remind_time[i], sizeof(next_remind_time));
							ret = 60*24*(7-rtc_week+1)+(next_remind_time[0] - curr_remind_time[0])*10*60 + (next_remind_time[1] - curr_remind_time[1])*60 
								+ (next_remind_time[2] - curr_remind_time[2])*10 + (next_remind_time[3] - curr_remind_time[3]);
							break;
						}
					}
					if(ret != 0)
					{
						break;
					}
					if(rtc_week == 1)
					{
						break;
					}
				case 2:
					i = 0;
					nv_date = &NVRAM_info.Tuesday;
					for(; i < REMIND_MAX; i++)
					{
						if((rtc_week == 2)&&(i == index))
						{
							if(nv_date->repeat_flag[i] == true)
							{
								ret = 60*24*7;
							}
							else
							{
								ret = 0;
							}
							break;
						}
						if(strlen(nv_date->remind_time[i]) > 0)
						{
							memcpy(next_remind_time, nv_date->remind_time[i], sizeof(next_remind_time));
							ret = 60*24*(7-rtc_week+2)+(next_remind_time[0] - curr_remind_time[0])*10*60 + (next_remind_time[1] - curr_remind_time[1])*60 
								+ (next_remind_time[2] - curr_remind_time[2])*10 + (next_remind_time[3] - curr_remind_time[3]);
							break;
						}
					}
					if(ret != 0)
					{
						break;
					}
					if(rtc_week == 2)
					{
						break;
					}
				case 3:
					i = 0;
					nv_date = &NVRAM_info.Wednesday;
					for(; i < REMIND_MAX; i++)
					{
						if((rtc_week == 3)&&(i == index))
						{
							if(nv_date->repeat_flag[i] == true)
							{
								ret = 60*24*7;
							}
							else
							{
								ret = 0;
							}
							break;
						}
						if(strlen(nv_date->remind_time[i]) > 0)
						{
							memcpy(next_remind_time, nv_date->remind_time[i], sizeof(next_remind_time));
							ret = 60*24*(7-rtc_week+3)+(next_remind_time[0] - curr_remind_time[0])*10*60 + (next_remind_time[1] - curr_remind_time[1])*60 
								+ (next_remind_time[2] - curr_remind_time[2])*10 + (next_remind_time[3] - curr_remind_time[3]);
							break;
						}
					}
					if(ret != 0)
					{
						break;
					}
					if(rtc_week == 3)
					{
						break;
					}
				case 4:
					i = 0;
					nv_date = &NVRAM_info.Thursday;
					for(; i < REMIND_MAX; i++)
					{
						if((rtc_week == 4)&&(i == index))
						{
							if(nv_date->repeat_flag[i] == true)
							{
								ret = 60*24*7;
							}
							else
							{
								ret = 0;
							}
							break;
						}
						if(strlen(nv_date->remind_time[i]) > 0)
						{
							memcpy(next_remind_time, nv_date->remind_time[i], sizeof(next_remind_time));
							ret = 60*24*(7-rtc_week+4)+(next_remind_time[0] - curr_remind_time[0])*10*60 + (next_remind_time[1] - curr_remind_time[1])*60 
								+ (next_remind_time[2] - curr_remind_time[2])*10 + (next_remind_time[3] - curr_remind_time[3]);
							break;
						}
					}
					if(ret != 0)
					{
						break;
					}
					if(rtc_week == 4)
					{
						break;
					}
				case 5:
					i = 0;
					nv_date = &NVRAM_info.Friday;
					for(; i < REMIND_MAX; i++)
					{
						if((rtc_week == 5)&&(i == index))
						{
							if(nv_date->repeat_flag[i] == true)
							{
								ret = 60*24*7;
							}
							else
							{
								ret = 0;
							}
							break;
						}
						if(strlen(nv_date->remind_time[i]) > 0)
						{
							memcpy(next_remind_time, nv_date->remind_time[i], sizeof(next_remind_time));
							ret = 60*24*(7-rtc_week+5)+(next_remind_time[0] - curr_remind_time[0])*10*60 + (next_remind_time[1] - curr_remind_time[1])*60 
								+ (next_remind_time[2] - curr_remind_time[2])*10 + (next_remind_time[3] - curr_remind_time[3]);
							break;
						}
					}
					if(ret != 0)
					{
						break;
					}
					if(rtc_week == 5)
					{
						break;
					}
				case 6:
					i = 0;
					nv_date = &NVRAM_info.Saturday;
					for(; i < REMIND_MAX; i++)
					{
						if((rtc_week == 6)&&(i == index))
						{
							if(nv_date->repeat_flag[i] == true)
							{
								ret = 60*24*7;
							}
							else
							{
								ret = 0;
							}
							break;
						}
						if(strlen(nv_date->remind_time[i]) > 0)
						{
							memcpy(next_remind_time, nv_date->remind_time[i], sizeof(next_remind_time));
							ret = 60*24*(7-rtc_week+6)+(next_remind_time[0] - curr_remind_time[0])*10*60 + (next_remind_time[1] - curr_remind_time[1])*60 
								+ (next_remind_time[2] - curr_remind_time[2])*10 + (next_remind_time[3] - curr_remind_time[3]);
							break;
						}
					}
					if(ret != 0)
					{
						break;
					}
					if(rtc_week == 6)
					{
						break;
					}
				default:
					break;
			}
		}
	}
	return ret; 	
}


void clear_curr_medicine_remind(void)
{
	medicine_time_t *curr_date = NULL;
	int i = 0;
	hal_rtc_time_t curtime; 
	uint8_t rtc_week = 0;	
	char time_str[8] = {0};

	curr_date = get_curr_date_medicine_remind();
	if(curr_date != NULL)
	{

       
		hal_rtc_get_time(&curtime);//获取rtctime
       

	
		rtc_week = CaculateWeekDay(curtime.rtc_year+2000,curtime.rtc_mon,curtime.rtc_day);
		sprintf(time_str, "%02d",curtime.rtc_hour);
		sprintf(&time_str[2], "%02d",curtime.rtc_min);
		for(i = 0; i < REMIND_MAX; i++)
		{
			if(strlen(curr_date->remind_time[i]) > 0)
			{
				if(strstr(time_str,curr_date->remind_time[i]) != NULL)
				{
					if(curr_date->repeat_flag[i] == false)
					{
						memset(curr_date->remind_time[i], 0, sizeof(curr_date->remind_time[0]));
					}
					
					break;
				}
			}
		}
	}
}


void sbit_m2m_ct_delect_delay_hanlde(void)
{
	sbit_m2m_ct_delect_hanlde(NULL,0);
}


void sbit_m2m_ct_send_massege(int msg,char *str,int len,bool isr)//sbit_m2m_ct_handle
{
	m2m_ct_queue_struct data={0};
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	
	data.msgid = msg;
	data.para = NULL;
	if((sbit_m2m_ct_queue != NULL)&&(sbit_m2m_ct_task != NULL))
	{
		if(str && len){
			data.para  = pvPortMalloc(len + 2);
			if(data.para){
				data.len = len + 2;
				memset(data.para,0,data.len);
				memcpy(data.para,str,len);
			}else{
			   SBIT_CT_DBG("pvPortMalloc error");
			   dbg_print("pvPortMalloc error");
			   return ;
			}
		}else{
			data.para = NULL;
			data.len  = 0;
		}
		
		if(isr){
			xQueueSendFromISR(sbit_m2m_ct_queue, &data, &xHigherPriorityTaskWoken);
		}else{
			xQueueSend(sbit_m2m_ct_queue, &data, 0);
		}
	}
}

void sbit_m2m_ct_send_massege_isr(int msg,char *str,int len,bool isr)
{
    m2m_ct_queue_struct data={0};
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	data.para = NULL;
	if((sbit_m2m_ct_queue != NULL)&&(sbit_m2m_ct_task != NULL))
	{	
		data.msgid = msg;
		data.len  = 0;
        xQueueSendFromISR(sbit_m2m_ct_queue, &data, &xHigherPriorityTaskWoken);
	}
 
}

void sbit_m2m_ct_new_hanlde(char *data,int len)
{
    char tem[512] = {0};
    int total;
    apb_proxy_parse_cmd_param_t parm;
     
    memset(&parm,0,sizeof(apb_proxy_parse_cmd_param_t));
#if defined(LOGO_SUPPORT_4) 
    /*117.78.42.93：5683 联通正式平台*/
    total= sprintf(tem,"AT+M2MCLINEW=117.78.42.93,5683,\"%s\",15",temp_info.imei_num);
#else
	/*117.60.157.137,5683 电信正式平台*/
	total= sprintf(tem,"AT+M2MCLINEW=117.60.157.137,5683,\"%s\",15",temp_info.imei_num);
#endif
    /*180.101.147.115,5683 测试平台*/
    //total= sprintf(tem,"AT+M2MCLINEW=180.101.147.115,5683,\"%s\",15",temp_info.imei_num);
    
    SBIT_CT_DBG("total %d,%s",total,tem);
    
    parm.string_ptr = tem;
    parm.string_len = total;
    parm.name_len   = strlen("AT+M2MCLINEW");
    parm.parse_pos  = parm.name_len + 1 ;
    parm.mode       = APB_PROXY_CMD_MODE_EXECUTION;
    parm.cmd_id     = SBIT_PROXY_M2MCLINEW_CMD_ID;
    
    apb_proxy_hdlr_lwm2m_client_create_cmd(&parm);
	
}


void sbit_m2m_ct_send_hanlde(char *data,int len)
{
	apb_proxy_status_t	 cmd_ret = APB_PROXY_STATUS_ERROR;
    apb_proxy_parse_cmd_param_t parm;

	if((data != NULL) && (strlen(data) != 0))
	{
		memset(&parm, 0, sizeof(apb_proxy_parse_cmd_param_t));
		parm.string_ptr = data;
		parm.string_len = strlen(data);
		parm.parse_pos = strlen("AT+M2MCLISEND");
		parm.name_len = parm.parse_pos+1;;
		parm.cmd_id = SBIT_PROXY_M2MCLISEND_CMD_ID;
		parm.mode = APB_PROXY_CMD_MODE_EXECUTION;
		SBIT_CT_DBG("ZZZZZZ::send:%s", data);
		ctiot_lwm2m_client_send_response("+M2MCLISEND");
		cmd_ret = apb_proxy_hdlr_lwm2m_client_send_cmd(&parm);
	}
	
}


void sbit_m2m_ct_delect_hanlde(char *data,int len)
{
    char tem[512] = {0};
    int total;
    apb_proxy_parse_cmd_param_t parm;
	
    memset(&parm,0,sizeof(apb_proxy_parse_cmd_param_t));
    total=sprintf(tem,"AT+M2MCLIDEL");
    SBIT_CT_DBG("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<sbit_m2m_ct_delect_hanlde>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> %d,%s",total,tem);
    
    parm.string_ptr = tem;
    parm.string_len = total + 1 ;
    parm.name_len   = strlen("AT+M2MCLIDEL");
    //parm.parse_pos  = parm.name_len + 1 ;
    parm.parse_pos  = parm.name_len;
    parm.mode       = APB_PROXY_CMD_MODE_ACTIVE;
    parm.cmd_id     = SBIT_PROXY_M2MCLIDEL_CMD_ID;
    
    apb_proxy_hdlr_lwm2m_client_delete_cmd(&parm);
	
}

void sbit_m2m_ct_recv_hanlde(char *data,int len)
{
	int i,j;
	char*pch;
	static char test[256]={0};
	static char tmp_buf_1[256]={0};
	static char tmp_buf_2[256]={0};


	i=((strlen(data))/100)+1;

	for(j=0;j<i;j++)
	{
		memset(tmp_buf_1,0,sizeof(tmp_buf_1));
		strncpy(tmp_buf_1,data+(100*j),100);
		SBIT_CT_DBG(" recv   %s\r\n",tmp_buf_1 );
		ctiot_lwm2m_client_send_response(tmp_buf_1);
	}


	memset(tmp_buf_1,0,sizeof(tmp_buf_1));
	strncpy(tmp_buf_1,data,256);

	ctiot_lwm2m_command_address(data,len);

	if((strstr(tmp_buf_1,":S71:"))!=NULL)
	{
	   /* 
	    血压校准指令
		{1:1:0:232322:111100000010342:S71:105,75} 
 	   */
	   if((strpbrk (tmp_buf_1,":S71:"))!=NULL)
	   {
		   pch=strstr(tmp_buf_1,":S71:" );
		   *pch++='\0';
		   *pch++='\0';
		   *pch++='\0';
		   *pch++='\0';
		   *pch++='\0';
		   memset(tmp_buf_2,0,sizeof(tmp_buf_2));
		   strncpy(tmp_buf_2,pch,(strlen(pch)));
	   
		   if((strpbrk (tmp_buf_2,","))!=NULL)
		   {
			   pch=strstr(tmp_buf_2,"," );
			   *pch++='\0';
			   memset(tmp_buf_1,0,sizeof(tmp_buf_1));
			   strncpy(tmp_buf_1,pch,(strlen(pch)));
			   memset(NVRAM_info.blood_hbp,0,sizeof(NVRAM_info.blood_hbp));
			   strncpy(NVRAM_info.blood_hbp,tmp_buf_2,12);
			   ctiot_lwm2m_client_send_response(NVRAM_info.blood_hbp);
			   
			   if((strpbrk (tmp_buf_1,"}"))!=NULL)
			   {
				   pch=strstr(tmp_buf_1,"}" );
				   *pch++='\0';
				   memset(NVRAM_info.blood_lbp,0,sizeof(NVRAM_info.blood_lbp));
				   strncpy(NVRAM_info.blood_lbp,tmp_buf_1,12);
				   ctiot_lwm2m_client_send_response(NVRAM_info.blood_lbp);
			   }
			   
		   }
		   
	   	}

	}	
	else if((strstr(tmp_buf_1,":S95:"))!=NULL)
	{
	   /* 
		{1:1:0:232322:39238239329:S95:10,500}
	   */
	   if((strpbrk (tmp_buf_1,":S95:"))!=NULL)
	   {
		   pch=strstr(tmp_buf_1,":S95:" );
		   *pch++='\0';
		   *pch++='\0';
		   *pch++='\0';
		   *pch++='\0';
		   *pch++='\0';
		   memset(tmp_buf_2,0,sizeof(tmp_buf_2));
		   strncpy(tmp_buf_2,pch,(strlen(pch)));
	   
		   if((strpbrk (tmp_buf_2,","))!=NULL)
		   {
			   pch=strstr(tmp_buf_2,"," );
			   *pch++='\0';
			   memset(tmp_buf_1,0,sizeof(tmp_buf_1));
			   strncpy(tmp_buf_1,pch,(strlen(pch)));
			   memset(test,0,sizeof(test));
			   strncpy(test,tmp_buf_2,12);
			   NVRAM_info.location_interval = atoi(test);
			   ctiot_lwm2m_client_send_response(test);
			   
			   if((strpbrk (tmp_buf_1,"}"))!=NULL)
			   {
				   pch=strstr(tmp_buf_1,"}" );
				   *pch++='\0';
				   memset(test,0,sizeof(test));
				   strncpy(test,tmp_buf_1,12);
				   NVRAM_info.step_interval = atoi(test);
				   if(NVRAM_info.step_interval>0)
				   NVRAM_info.step_intervallogo=1;
				   else
				   NVRAM_info.step_intervallogo=0;
				   ctiot_lwm2m_client_send_response(test);
			   }
			   
		   }
		   
	   	}

	}
	else if(strstr(tmp_buf_1,"S86:")!= NULL)
	{

		/*
		报文类型：S86
		格式：{S86:1|2|3|4|5|6|7&1500_0,1|2|0|4|5|6|0&1800_0,1|2|0|4|5|6|0&1900_0}
		描述：
		命令：S86
		参数：
		*/
		ctiot_lwm2m_client_send_response("======S86:");
		SBIT_CT_DBG("============rec S86:%s\r\n",tmp_buf_1);
		if((strpbrk (tmp_buf_1,":"))!=NULL)
		{
			char *p0 = NULL;
			char *p1 = NULL;
			char time_str[8] = {0};
			bool rp_flag = false;
			int i = 0, week_i = 0;
			p0 = strstr(tmp_buf_1,"S86:");
			p1 = p0;
			NVRAM_info.medicine_set_flag = 0;
			for(i = 0; i < REMIND_MAX; i++)
			{
				if(i == 0)
				{
					p0 = strstr(p0,":" );
				}
				else
				{
					p0 = strstr(p0,"," );
				}
				p1 = strstr(p1,"&" );
				memset(time_str, 0, sizeof(time_str));
				rp_flag = false;
				if((p0 == NULL) || (p1 == NULL))
				{
					if(i == 0)
					{
						NVRAM_info.medicine_set_flag = -1;
					}
					else
					{
						NVRAM_info.medicine_set_flag = 1;
					}
					break;
				}
				if((p0 != NULL) && (p1 != NULL))
				{
					p0++;
					p1++;
					memcpy(time_str,p1,4);
					p1 = strstr(p1,"_" );
					p1++;
					if(*p1 == '0')
					{
						rp_flag = false;
					}
					else
					{
						rp_flag = true;
					}
					if(i == 0)
					{
						clear_medicine_remind();
					}
					for(week_i = 0; week_i < 7; week_i++)
					{
						if((*(p0+week_i*2)) != '0')
						{
							switch(week_i)
							{
								case 0:
									memcpy(NVRAM_info.Monday.remind_time[i], time_str, sizeof(time_str));
									NVRAM_info.Monday.repeat_flag[i] = rp_flag;
									SBIT_CT_DBG("======Monday.remind_time[%d],%s[%d]",i, time_str, rp_flag);
									break;
								case 1:
									memcpy(NVRAM_info.Tuesday.remind_time[i], time_str, sizeof(time_str));
									NVRAM_info.Tuesday.repeat_flag[i] = rp_flag;
									SBIT_CT_DBG("======Tuesday.remind_time[%d],%s[%d]",i, time_str, rp_flag);
									break;
								case 2:
									memcpy(NVRAM_info.Wednesday.remind_time[i], time_str, sizeof(time_str));
									NVRAM_info.Wednesday.repeat_flag[i] = rp_flag;
									SBIT_CT_DBG("======Wednesday.remind_time[%d],%s[%d]",i, time_str, rp_flag);
									break;
								case 3:
									memcpy(NVRAM_info.Thursday.remind_time[i], time_str, sizeof(time_str));
									NVRAM_info.Thursday.repeat_flag[i] = rp_flag;
									SBIT_CT_DBG("======Thursday.remind_time[%d],%s[%d]",i, time_str, rp_flag);
									break;
								case 4:
									memcpy(NVRAM_info.Friday.remind_time[i], time_str, sizeof(time_str));
									NVRAM_info.Friday.repeat_flag[i] = rp_flag;
									SBIT_CT_DBG("======Friday.remind_time[%d],%s[%d]",i, time_str, rp_flag);
									break;
								case 5:
									memcpy(NVRAM_info.Saturday.remind_time[i], time_str, sizeof(time_str));
									NVRAM_info.Saturday.repeat_flag[i] = rp_flag;
									SBIT_CT_DBG("======Saturday.remind_time[%d],%s[%d]",i, time_str, rp_flag);
									break;
								case 6:
									memcpy(NVRAM_info.Sunday.remind_time[i], time_str, sizeof(time_str));
									NVRAM_info.Sunday.repeat_flag[i] = rp_flag;
									SBIT_CT_DBG("======Sunday.remind_time[%d],%s[%d]",i, time_str, rp_flag);
									break;
								default:
									break;
							}
						}
					}
				}
			}

			sbit_m2m_ct_send_massege(M2M_CT_ack_T86,NULL,0,0);
			
		}
	}
	else if(strstr(tmp_buf_1,"S87")!= NULL)
	{
		//?{1:1:0:232322:39238239329:S87} 重置服药提醒指令
		clear_medicine_remind();
		clear_medicine_remind_flag = true;
		medicine_remind_delay = 0;
		sbit_set_vib_time(0,1);
		SBIT_CT_DBG("======S87:");
		ctiot_lwm2m_client_send_response("======S87:");
		NVRAM_info.medicine_set_flag = 0;
		medicine_lock_sec = 0;
	}
	else if((strstr(tmp_buf_1,":S100:"))!=NULL)
	{
		/*
		3.4.2、服务器设置手表工作模式指令 
		报文类型：S100 
		格式：{1:1:0:232322:39238239329:S100:1} 
		描述： 
		命令: S100
		参数： 1 为正常模式、2为省电模式。
		*/
		
		if((strstr(tmp_buf_1,":S100:1}"))!=NULL)
		NVRAM_info.Power_saving_flag = 0;
		else if((strstr(tmp_buf_1,":S100:2}"))!=NULL)
		NVRAM_info.Power_saving_flag = 1;
		
	}

	//{S92:52955257|A0E6F86CFB13_-85,A0E6F86CFB11_-85,A0E6F86CFB12_-85,A0E6F86CFB14_-85,A0E6F86CFB15_-85,A0E6F86CFB16_-85,A0E6F86CFB17_-85,A0E6F86CFB18_-85}
	
  //  sbit_m2m_ct_send_massege(M2M_CT_DEL,0,0,0);
	
}
void sbit_m2m_ct_cfun_off(void)
{
	return;
	ctiot_lwm2m_client_send_response("sbit_m2m_ct_cfun_off");
    SBIT_CT_DBG("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<  sbit_m2m_ct_cfun_off  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	sbit_m2m_ct_send_massege(M2M_CT_DEL,NULL,0,0);
}
#include "hal_platform.h"
extern int g_mqtt_id;
void sbit_m2m_ct_handle(m2m_ct_queue_struct msg)
{
   int i = 0;

    SBIT_CT_DBG("<<<<<<<<<<<<<<<<<<<<<<<<<< temp_info.registered_network_flag >>>>>>>>>>>>>>>>>>%d",temp_info.registered_network_flag);

	switch(msg.msgid)
    {			
		case M2M_DISPLAY_BACKLIGHT:
			SBIT_CT_DBG("M2M_DISPLAY_BACKLIGHT:%d,%d",temp_info.shut_down_flag,g_work_time);
			#if 0
			if(temp_info.shut_down_flag==0  && g_work_time>=5)//防止关机进入时钟界面
			{}
			else
			#endif
			sbit_demo_backlight();
			//ctiot_lwm2m_client_send_response("M2M_DISPLAY_BACKLIGHT");
			break;

		case M2M_SHUTDOWN:
			sbit_shut_down();
			SBIT_CT_DBG("M2M_SHUTDOWN");
			ctiot_lwm2m_client_send_response("M2M_SHUTDOWN");
			break;
			
		case M2M_SOS_KEY:
			SBIT_CT_DBG("M2M_SOS_KEY");
			ctiot_lwm2m_client_send_response("M2M_SOS_KEY");
			sos_PopupBox();
			break;
			
		case M2M_MENU_SET:
			SBIT_CT_DBG("M2M_MENU_SET");
			ctiot_lwm2m_client_send_response("M2M_MENU_SET");
			temp_info.set_mode_select = 0;
			sbit_show_set_idle();
			break;
		case M2M_MENU_KEY:
			SBIT_CT_DBG("M2M_MENU_KEY");
			ctiot_lwm2m_client_send_response("M2M_MENU_KEY");
			break;
		case M2M_CT_SELF_REGISTER:
			ctiot_lwm2m_client_send_response("M2M_CT_SELF_REGISTER");
			if(msg.para)
			{
				int result = *(int *)(msg.para);
				{
					char tmp_buf[100]={0};
					sprintf(tmp_buf,"M2M_CT_SELF_REGISTER:%d",result);
					ctiot_lwm2m_client_send_response(tmp_buf);
				}
				if(result == 0)
				{
					delay_cfun_OffOn();
				}
			}
			break;
		case M2M_CT_NEW:
			SBIT_CT_DBG("M2M_CT_NEW");
			dbg_print("M2M_CT_NEW:%d,%d,%d,%d",temp_info.registered_network_flag,temp_info.csq_num,g_mqtt_disconnect_time_count,g_mqtt_id);
			temp_info.network_sending_flag = 0;
			if((temp_info.registered_network_flag==1)&&((temp_info.csq_num>=5)&&(temp_info.csq_num<33)))
			{
			//	sbit_m2m_ct_new_hanlde(NULL,0);
				if(g_mqtt_disconnect_time_count >=180 || g_mqtt_id == -1)
				{
					n1_mqtt_create();
				}
				if(sbit_ct_cfun_off_timer == NULL)
				sbit_ct_cfun_off_timer = xTimerCreate("cfun_off", 
				30*1000 / portTICK_PERIOD_MS, 
				pdFALSE, 
				(void *) 0, 
				sbit_m2m_ct_cfun_off);
				xTimerStart(sbit_ct_cfun_off_timer,1);
				ctiot_lwm2m_client_send_response("M2M_CT_NEW1");
			}
			else
			{
				if(temp_info.registered_network_flag==1)
				{
					set_phone_functionality(false);
					ctiot_lwm2m_client_send_response("M2M_CT_NEW2");
				}
				else
				{
					set_phone_functionality(true);
					ctiot_lwm2m_client_send_response("M2M_CT_NEW3");
				}
			}
			break;
		case M2M_CT_SEND:
			#if 1
			SBIT_CT_DBG("M2M_CT_SEND"); 
			if(sbit_m2m_ct_delect_delay_timer != NULL)
			xTimerStop(sbit_m2m_ct_delect_delay_timer,0); 
#if defined(MTK_SEND_QUEUE_FIFO)
			i = get_m2m_send_Queue_Fifo();
#else
			i = get_m2m_send_Queue();
#endif
			dbg_print("M2M_CT_SEND  i:%d",i);
			if(i >= 0 && g_mqtt_id>= 0)
			{
				ctiot_lwm2m_client_send_response(temp_info1.m2m_send_temp_Queue[i]);
				if((strstr(temp_info1.m2m_send_temp_Queue[i],"\r"))!=NULL)
				{
					SBIT_CT_DBG("==================================== \r ======================================");
				}
				
				temp_info.send_record_flag[i]++;
				memset(temp_info.send_temp_Queue, 0, sizeof(temp_info.send_temp_Queue));
				strcpy(temp_info.send_temp_Queue,temp_info1.m2m_send_temp_Queue[i]);
				str_to_hex(temp_info1.m2m_send_temp_Queue[i],temp_info.data_buffer_hex);
				if((temp_info.registered_network_flag==1)
					&&((temp_info.csq_num>=5)&&(temp_info.csq_num<33))&&(strlen(temp_info.data_buffer_hex)>0))
				{
					//sbit_m2m_ct_send_hanlde(temp_info.data_buffer_hex,strlen(temp_info.data_buffer_hex));
					//n1_mqtt_publish(temp_info1.m2m_send_temp_Queue[i]);
				}
			}
			#endif
			break;
			
		case M2M_CT_DEL: 
			if(sbit_m2m_ct_delect_delay_timer == NULL)
			sbit_m2m_ct_delect_delay_timer = xTimerCreate("delect_delay", 
			5*1000 / portTICK_PERIOD_MS, 
			pdFALSE, 
			(void *) 0, 
			sbit_m2m_ct_delect_delay_hanlde);
			xTimerStart(sbit_m2m_ct_delect_delay_timer,1);
			SBIT_CT_DBG("M2M_CT_DEL");
			ctiot_lwm2m_client_send_response("M2M_CT_DEL");
			break;
			
		case M2M_CT_REGISTER:
			SBIT_CT_DBG("M2M_CT_REGISTER");
			ctiot_lwm2m_client_send_response("M2M_CT_REGISTER");
			break;
						
		case M2M_CT_NOTIFY:
			if(sbit_ct_cfun_off_timer!=NULL)
			xTimerStop(sbit_ct_cfun_off_timer,0);
			SBIT_CT_DBG("M2M_CT_NOTIFY");
			ctiot_lwm2m_client_send_response("M2M_CT_NOTIFY");
			break;
			
		case M2M_CT_UPDATE:
			if(sbit_ct_cfun_off_timer!=NULL)
			xTimerStop(sbit_ct_cfun_off_timer,0);
			SBIT_CT_DBG("M2M_CT_UPDATE");
			ctiot_lwm2m_client_send_response("M2M_CT_UPDATE");
			break;
			
		case M2M_CT_RECV:
			SBIT_CT_DBG("M2M_CT_RECV");
			ctiot_lwm2m_client_send_response("M2M_CT_RECV");
			sbit_m2m_ct_recv_hanlde(msg.para,msg.len);
			break;
			
		case M2M_CT_REGISTER_FAIL:
			SBIT_CT_DBG("M2M_CT_REGISTER_FAIL");
			ctiot_lwm2m_client_send_response("M2M_CT_REGISTER_FAIL");
			sbit_m2m_ct_delect_hanlde(NULL,0);
			break;
			
		case M2M_CT_DEL_SUCCESS:
			SBIT_CT_DBG("M2M_CT_DEL_SUCCESS");
			ctiot_lwm2m_client_send_response("M2M_CT_DEL_SUCCESS");
			if(sbit_ct_cfun_off_timer!=NULL)
			xTimerStop(sbit_ct_cfun_off_timer,0);
			break;
			
		case M2M_CT_total_data_T50:
			H10_send_total_data_T50();
			SBIT_CT_DBG("M2M_CT_total_data_T50");
			ctiot_lwm2m_client_send_response("M2M_CT_total_data_T50");
			break;
						
		case M2M_CT_sos_data_T0:
			H10_send_sos_data_T0();
			SBIT_CT_DBG("M2M_CT_sos_data_T0");
			ctiot_lwm2m_client_send_response("M2M_CT_sos_data_T0");
			break;
			
		case M2M_CT_gps_data_T29:
			H10_send_gps_data_T29();
			SBIT_CT_DBG("M2M_CT_gps_data_T29");
			ctiot_lwm2m_client_send_response("M2M_CT_gps_data_T29");
			break;
			
		case M2M_CT_hartrate_data_T28:
			H10_send_hartrate_data_T28();
			SBIT_CT_DBG("M2M_CT_hartrate_data_T28");
			ctiot_lwm2m_client_send_response("M2M_CT_hartrate_data_T28");
			break;
			
		case M2M_CT_ack_T86:
			H10_send_ack_T86();
			SBIT_CT_DBG("M2M_CT_ack_T86");
			ctiot_lwm2m_client_send_response("M2M_CT_ack_T86");
			break;

		case M2M_CT_ack_T87:
			H10_send_ack_T87();
			SBIT_CT_DBG("M2M_CT_ack_T87");
			ctiot_lwm2m_client_send_response("M2M_CT_ack_T87");
			break;
			
		case M2M_CT_data_T100:
			H10_send_data_T100();
			SBIT_CT_DBG("M2M_CT_data_T100");
			//ctiot_lwm2m_client_send_response("M2M_CT_data_T100");
			break;
			
		case M2M_CT_data_T101:
			H10_send_data_T101();
			SBIT_CT_DBG("H10_send_data_T101");
			ctiot_lwm2m_client_send_response("H10_send_data_T101");
			break;
	
		case M2M_CT_hartblood_data_T45:
			H10_send_hartblood_data_T45();
			SBIT_CT_DBG("M2M_CT_hartblood_data_T45");
			ctiot_lwm2m_client_send_response("M2M_CT_hartblood_data_T45");
			break;
			
		case M2M_CT_gettime_data:
			H10_send_gettime_data();
			SBIT_CT_DBG("M2M_CT_gettime_data");
			ctiot_lwm2m_client_send_response("M2M_CT_gettime_data");
			break;
			
		case M2M_CT_T71:
			H10_send_T71_data();
			SBIT_CT_DBG("M2M_CT_T71");
			ctiot_lwm2m_client_send_response("M2M_CT_T71");
			break;
		case M2M_send_T15_ACK:
			H10_send_T15_ack();
			SBIT_CT_DBG("H10_send_T15_ack");
			ctiot_lwm2m_client_send_response("H10_send_T15_ack");
			break;		  
		case M2M_CT_temp_send_Queue:
			if(sbit_m2m_ct_delect_delay_timer != NULL)
			xTimerStop(sbit_m2m_ct_delect_delay_timer,0); 
			SBIT_CT_DBG("M2M_CT_temp_send_Queue");
			ctiot_lwm2m_client_send_response("M2M_CT_temp_send_Queue");
			sbit_m2m_ct_send_massege(M2M_CT_CFUN_ON,NULL,0,0);
			break;
			
		case M2M_CT_CFUN_OFF:
			SBIT_CT_DBG("M2M_CT_CFUN_OFF");
			ctiot_lwm2m_client_send_response("M2M_CT_CFUN_OFF");
			break;
			
		case M2M_CT_CFUN_ON:
			if(((get_m2m_send_Queue()>=0)&&( NVRAM_info.network_fail_record == 2))||
			(temp_info.registered_network_flag==0))
			{
				delay_cfun_OffOn();
				ctiot_lwm2m_client_send_response("M2M_CT_CFUN_ON1");
			}
			else if(strlen(temp_info.imei_num)>0)
			{
				ctiot_lwm2m_client_send_response("M2M_CT_CFUN_ON2");
				sbit_m2m_ct_send_massege(M2M_CT_NEW,temp_info.imei_num,strlen(temp_info.imei_num),0);
			}
			else
			{
				SBIT_CT_DBG("M2M_CT_CFUN_ON");
				ctiot_lwm2m_client_send_response("M2M_CT_CFUN_ON3");
			}
			
			break;
		case M2M_MQTT_CREATE:
			if(g_mqtt_id >= 0)
			{
				n1_mqtt_connect();
			}
			break;
			
		case M2M_MQTT_CONNECT:
			{
				if(msg.para)
				{
					int error_code;
					memcpy(&error_code,msg.para,msg.len);
					dbg_print("m2m_mqtt_connect_rsp:%d",error_code);

					if(error_code == 100 || error_code == 0)//MQTT_AT_ERRID_OK
					{
						task_start_timer(TaskTimer_MQTT_Send_Subscribe,1000,n1_mqtt_subscribe);
					}
					else
					{
						task_start_timer(TaskTimer_MQTT_Reconnect,60*1000,n1_mqtt_reconnect);
					}
				}
			}
			break;
		case M2M_MQTT_DATA_IND://MQTT  PUB消息
			{
				dbg_print("---------M2M_MQTT_DATA_IND:%s",msg.para);
				if(msg.para != NULL)
				{
					sbit_m2m_ct_recv_hanlde(msg.para,msg.len);
					m2m_mqttrec_analysis(msg.para);
				}
			}
			break;
		case M2M_MQTT_TCP_DATA_IND:
			{
				
				char tmp_buf[800]={0},tmp_buf2[30]={0};
				int i=0;
				if(msg.para != NULL)
				{
					strcpy(tmp_buf,"mqtt recv:");
					ctiot_lwm2m_client_send_response("mqtt recv");
					for(i=0;i<msg.len;i++)
					{
						memset(tmp_buf2,0,sizeof(tmp_buf2));
						sprintf(tmp_buf2,"%02x,",msg.para[i]);
						strcat(tmp_buf,tmp_buf2);
					}
					dbg_print(tmp_buf);


					for(i=0;i<msg.len;i++)
					{
						if(msg.para[i] == 0x7B && msg.len >= 50)
						{
							sbit_m2m_ct_recv_hanlde(&msg.para[i],msg.len-i);
							m2m_mqttrec_analysis(&msg.para[i]);
							break;
						}
					}

					g_mqtt_disconnect_time_count = 0;
					if(g_is_mqtt_publish)
					{
						g_is_mqtt_publish = false;
						m2m_remove_sending_data_bag();
					}
					g_mqtt_send_fail_count = 0;
				}
			}
			break;
		case M2M_MQTT_DISCONNECT:
			{
				int mqttid;
				if(msg.para)
				{
					int mqttid;
					memcpy(&mqttid,msg.para,msg.len);
					dbg_print("M2M_MQTT_DISCONNECT:%d,%d",mqttid,g_mqtt_id);
					n1_mqtt_disconnect(mqttid);	
					if(mqttid == g_mqtt_id)
					{
						n1_mqtt_create();
					}
				}
			}
			break;
        default :
            break;
     }  
    
}

void sbit_m2m_ct_ril_callback(ril_urc_id_t event_id, void *param, uint32_t param_len)
{
	hal_rtc_time_t curtime;	
    char tem[12] = {0};
	
	hal_rtc_get_time(&curtime);//获取rtctime
    switch(event_id)
    {
        case RIL_URC_ID_CEREG:
            {
                ril_eps_network_registration_status_urc_t *cereg_status = (ril_eps_network_registration_status_urc_t *)param;

				//SBIT_CT_DBG("<<<<<<<<<<<<<<<<<<< cereg_status->stat , >>>>>>>>>>>>>%d",cereg_status->stat);
				//SBIT_CT_DBG("<<<<<<<<<<<<<<<<<<< cereg_status->lac , >>>>>>>>>>>>>%x",cereg_status->lac);
				//SBIT_CT_DBG("<<<<<<<<<<<<<<<<<<< cereg_status->ci , >>>>>>>>>>>>>%x",cereg_status->ci);
				//SBIT_CT_DBG("<<<<<<<<<<<<<<<<<<< cereg_status->act , >>>>>>>>>>>>>%x",cereg_status->act);
				//SBIT_CT_DBG("<<<<<<<<<<<<<<<<<<< cereg_status->rac , >>>>>>>>>>>>>%x",cereg_status->rac);

	            if (TEL_CONN_MGR_NW_REG_STAT_REGED_HMNW == cereg_status->stat ||
	               TEL_CONN_MGR_NW_REG_STAT_REGED_ROAMING == cereg_status->stat ||
	               TEL_CONN_MGR_NW_REG_STAT_SMS_ONLY_HMNW == cereg_status->stat ||
	               TEL_CONN_MGR_NW_REG_STAT_SMS_ONLY_ROAMING == cereg_status->stat) 
                {    //1, 5, 6, 7
	                //register to the network successfully
					if(sbit_ct_cfun_off_timer!=NULL)
					xTimerStop(sbit_ct_cfun_off_timer,0);
                    temp_info.registered_network_flag = 1;
					// 100 --> delay 1 sec
					//if((strlen(temp_info.imei_num)>0)&&(strlen(temp_info.data_buffer_hex)>0)&&(curtime.rtc_year+2000>=2018))
					if((strlen(temp_info.imei_num)>0)&&(curtime.rtc_year+2000>=2018)&&
					(get_m2m_Queue_count()>0)&&(temp_info.network_sending_flag == 1))
					sbit_m2m_ct_send_massege(M2M_CT_NEW,temp_info.imei_num,strlen(temp_info.imei_num),0);
					ctiot_lwm2m_client_send_response("sbit_m2m_ct_ril_callback1");
                }
				else if(TEL_CONN_MGR_NW_REG_STAT_TRYING != cereg_status->stat)
				{
	                temp_info.registered_network_flag = 0;
					ctiot_lwm2m_client_send_response("sbit_m2m_ct_ril_callback2");
				}
				
				sprintf(tem,"%d",cereg_status->stat);
				ctiot_lwm2m_client_send_response(tem);
				
				SBIT_CT_DBG("<<<<<<<<<<<<<<<<<<<<<<<<<< temp_info.registered_network_flag >>>>>>>>>>>>>>>>>>%d",temp_info.registered_network_flag);
            }
            
            break;
    }  
}

void sbit_m2m_ct_main(void *arg)
{
    m2m_ct_queue_struct queue;
    sbit_m2m_ct_queue = xQueueCreate(M2M_CT_MAX*2, sizeof(m2m_ct_queue_struct));
    //sbit_m2m_ct_timer = xTimerCreate("m2m_ct_timer", SBIT_M2M_CT_FIRST_TIME*1000/portTICK_PERIOD_MS,pdFALSE, NULL, sbit_m2m_ct_timer_callback);
    
    while(1)
	{
        if(xQueueReceive(sbit_m2m_ct_queue, &queue, portMAX_DELAY))
		{
            sbit_m2m_ct_handle(queue);
			if(queue.para != NULL)
			{
				vPortFree(queue.para);
			}
        }
    }
	
}
void sbit_m2m_ct_init(void)
{
    ril_status_t ret;
    
    if (pdPASS!=xTaskCreate(sbit_m2m_ct_main,"m2m_ct",4*1024,NULL,TASK_PRIORITY_BELOW_NORMAL,&sbit_m2m_ct_task))
    {
        SBIT_CT_DBG("creat task error");
        return;
    }
    ret = ril_register_event_callback(RIL_GROUP_MASK_ALL, sbit_m2m_ct_ril_callback);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void Iccid_Get_sim (char *myIccID)
{
     
	 uint16_t i=0;
     uint16_t sim_num=2400;
	 
	 static char *sim_list[2400] = 
	  {
	     //1410009066570	8986111822300503706
		 "1410009066570","2300503706",
		 "1410023301156","4600915663",
		 "1410023282944","4600916029",
		 "1410023301087","4600915782",
		 "1410009059864","2300503632",
		 "1410009066650","2300503705",
		 "1410009059964","2300503631",
		 "1410023301476","4600915992",
		 "1410023299914","4600916133",
		 "1410009066690","2300503704",
		 "1410009059884","2300503630",
		 "1410009066630","2300503703",
		 "1410023300836","4600915654",
		 "1410023299934","4600916132",
		 "1410023301596","4600915991",
		 "1410023301107","4600915781",
		 "1410009066590","2300503702",
		 "1410023287570","4600916258",
		 "1410009066610","2300503701",
		 "1410009063378","2300503675",
		 "1410023289955","4600916085",
		 "1410009066670","2300503700",
		 "1410009063398","2300503674",
		 "1410023296091","4600916236",
		 "1410023290862","4600916095",
		 "1410023298776","4600915994",
		 "1410023305392","4600915665",
		 "1410009072149","2300503715",
		 "1410009063278","2300503673",
		 "1410009072189","2300503714",
		 "1410023282964","4600916030",
		 "1410023306492","4600916143",
		 "1410023296031","4600916237",
		 "1410023290015","4600916086",
		 "1410023295971","4600916235",
		 "1410009072269","2300503713",
		 "1410009063298","2300503671",
		 "1410023291801","4600915737",
		 "1410009063418","2300503672",
		 "1410023301536","4600915993",
		 "1410023305452","4600915664",
		 "1410009063318","2300503670",
		 "1410023299217","4600915791",
		 "1410023306172","4600916134",
		 "1410023292221","4600915738",
		 "1410009072089","2300503712",
		 "1410023298783","4600915711",
		 "1410023289219","4600916270",
		 "1410023298856","4600916358",
		 "1410023303454","4600916331",
		 "1410023300077","4600916208",
		 "1410023297458","4600915678",
		 "1410023289259","4600916269",
		 "1410023283384","4600916161",
		 "1410023301460","4600916338",
		 "1410023298956","4600916357",
		 "1410023294875","4600916104",
		 "1410023283084","4600916156",
		 "1410023297431","4600915960",
		 "1410023292863","4600916280",
		 "1410023290403","4600916279",
		 "1410023283304","4600916162",
		 "1410023305287","4600916367",
		 "1410023283064","4600916155",
		 "1410023303514","4600916332",
		 "1410009066710","2300503707",
		 "1410023297478","4600915683",
		 "1410023300433","4600915970",
		 "1410023300137","4600916213",
		 "1410023294915","4600916109",
		 "1410023305447","4600916018",
		 "1410023295936","4600915770",
		 "1410023288699","4600916074",
		 "1410023301640","4600916214",
		 "1410023297251","4600915959",
		 "1410023294835","4600916110",
		 "1410023295286","4600916064",
		 "1410023298896","4600916355",
		 "1410023289099","4600916268",
		 "1410023283424","4600916160",
		 "1410023298703","4600915710",
		 "1410023298936","4600916356",
		 "1410023301440","4600916337",
		 "1410023302227","4600915684",
		 "1410023295391","4600915763",
		 "1410023295311","4600915760",
		 "1410023292946","4600916387",
		 "1410009072229","2300503711",
		 "1410023299287","4600915780",
		 "1410023291115","4600916084",
		 "1410009066330","2300503669",
		 "1410023299074","4600916131",
		 "1410023287690","4600916257",
		 "1410023296051","4600916241",
		 "1410023292241","4600915735",
		 "1410023282984","4600916028",
		 "1410023306152","4600916138",
		 "1410023290885","4600915726",
		 "1410023282644","4600916027",
		 "1410023288585","4600916320",
		 "1410023300816","4600915658",
		 "1410023297048","4600915982",
		 "1410023305407","4600916017",
		 "1410023298663","4600915709",
		 "1410023289239","4600916267",
		 "1410023283444","4600916159",
		 "1410023301480","4600916336",
		 "1410023297108","4600915981",
		 "1410023295951","4600916240",
		 "1410023301647","4600915690",
		 "1410023306607","4600915407",
		 "1410023291935","4600915536",
		 "1410023298023","4600915486",
		 "1410023298743","4600915706",
		 "1410023295331","4600915759",
		 "1410023301036","4600915636",
		 "1410023306567","4600915408",
		 "1410023296758","4600915921",
		 "1410023300956","4600915635",
		 "1410023297743","4600915485",
		 "1410023284444","4600915589",
		 "1410023292095","4600915535",
		 "1410023296698","4600915920",
		 "1410023298868","4600915847",
		 "1410023300180","4600915883",
		 "1410023295831","4600915885",
		 "1410023299008","4600915849",
		 "1410023302056","4600915496",
		 "1410023296578","4600915915",
		 "1410023301056","4600915638",
		 "1410023298916","4600916362",
		 "1410023283364","4600916154",
		 "1410023295811","4600915884",
		 "1410023299048","4600915848",
		 "1410023301776","4600915495",
		 "1410023301016","4600915637",
		 "1410023289825","4600916007",
		 "1410023306547","4600915409",
		 "1410023292035","4600915537",
		 "1410023296598","4600915922",
		 "1410023299977","4600916207",
		 "1410023299957","4600916211",
		 "1410023301013","4600915968",
		 "1410023305487","4600916015",
		 "1410023295126","4600916072",
		 "1410023302207","4600915689",
		 "1410023300057","4600916212",
		 "1410023294975","4600916108",
		 "1410023296996","4600915769",
		 "1410023295206","4600916073",
		 "1410023295075","4600916063",
		 "1410023300353","4600915969",
		 "1410023305387","4600916016",
		 "1410023297638","4600915677",
		 "1410023288525","4600916314",
		 "1410023287610","4600916256",
		 "1410023301856","4600916394",
		 "1410023288565","4600916319",
		 "1410023301796","4600916393",
		 "1410023289885","4600916006",
		 "1410023300413","4600915967",
		 "1410023298603","4600915708",
		 "1410023297016","4600915768",
		 "1410023301680","4600916218",
		 "1410004683040","2300503730",
		 "1410023289785","4600916005",
		 "1410023297291","4600915958",
		 "1410023295671","4600915758",
		 "1410023296098","4600916181",
		 "1410023301240","4600916301",
		 "1410023295558","4600916182",
		 "1410023298623","4600915707",
		 "1410023294995","4600916107",
		 "1410023301900","4600916217",
		 "1410023301180","4600916302",
		 "1410023295976","4600915767",
		 "1410023300920","4600916300",
		 "1410023288159","4600916186",
		 "1410023300196","4600916308",
		 "1410023300556","4600916309",
		 "1410023296018","4600916180",
		 "1410023295538","4600916179",
		 "1410023300296","4600916310",
		 "1410023293286","4600916392",
		 "1410023305694","4600915608",
		 "1410023287670","4600916255",
		 "1410023287999","4600916191",
		 "1410023297397","4600915551",
		 "1410023295371","4600915757",
		 "1410023307072","4600916291",
		 "1410023296058","4600916174",
		 "1410023300236","4600916305",
		 "1410023301367","4600915688",
		 "1410023307132","4600916292",
		 "1410023294398","4600916379",
		 "1410023295618","4600916183",
		 "1410023300536","4600916306",
		 "1410023304574","4600916114",
		 "1410023297391","4600915957",
		 "1410023301220","4600916297",
		 "1410023294121","4600916350",
		 "1410023297783","4600916173",
		 "1410023292906","4600916385",
		 "1410023295226","4600916071",
		 "1410023290385","4600915714",
		 "1410023300176","4600916304",
		 "1410023301200","4600916298",
		 "1410023292986","4600916386",
		 "1410023301660","4600916216",
		 "1410023300316","4600916312",
		 "1410023300216","4600916313",
		 "1410023300960","4600916294",
		 "1410023300680","4600916303",
		 "1410023294158","4600916382",
		 "1410023300256","4600916307",
		 "1410023300900","4600916299",
		 "1410023288079","4600916184",
		 "1410023307172","4600916293",
		 "1410023292998","4600916380",
		 "1410023300276","4600916311",
		 "1410023288139","4600916185",
		 "1410023294378","4600916381",
		 "1410009060404","2300503635",
		 "1410009060424","2300503634",
		 "1410023297823","4600916171",
		 "1410023292926","4600916383",
		 "1410023294081","4600916349",
		 "1410009073967","2300503684",
		 "1410009066230","2300503667",
		 "1410023296978","4600915864",
		 "1410023296269","4600915570",
		 "1410009065530","2300503652",
		 "1410023301260","4600916296",
		 "1410023292966","4600916384",
		 "1410023297943","4600916172",
		 "1410009074047","2300503685",
		 "1410023297566","4600915812",
		 "1410009065570","2300503653",
		 "1410009065590","2300503654",
		 "1410023295341","4600915934",
		 "1410009073927","2300503686",
		 "1410009065870","2300503655",
		 "1410023293018","4600916373",
		 "1410023294001","4600916347",
		 "1410023306972","4600916286",
		 "1410023295598","4600916178",
		 "1410023294738","4600916374",
		 "1410023293981","4600916348",
		 "1410023300940","4600916295",
		 "1410023306632","4600915502",
		 "1410023284842","4600915603",
		 "1410023297377","4600915546",
		 "1410023305194","4600915926",
		 "1410023294515","4600915899",
		 "1410023306667","4600915413",
		 "1410023300237","4600915653",
		 "1410023306992","4600916289",
		 "1410023292075","4600915539",
		 "1410023293038","4600916377",
		 "1410023297763","4600916166",
		 "1410023301420","4600916342",
		 "1410009066150","2300503660",
		 "1410023296289","4600915569",
		 "1410009075672","2300503693",
		 "1410023286939","4600915863",
		 "1410023288810","4600915504",
		 "1410009075812","2300503692",
		 "1410009066170","2300503661",
		 "1410023305174","4600915933",
		 "1410009075832","2300503694",
		 "1410023296038","4600916176",
		 "1410023294041","4600916352",
		 "1410023305267","4600916372",
		 "1410023307032","4600916284",
		 "1410009066190","2300503662",
		 "1410009066290","2300503663",
		 "1410009075692","2300503695",
		 "1410023307152","4600916285",
		 "1410023294021","4600916353",
		 "1410023295578","4600916177",
		 "1410009075732","2300503696",
		 "1410009066210","2300503664",
		 "1410023295041","4600915465",
		 "1410023294435","4600915900",
		 "1410023305433","4600915414",
		 "1410023307052","4600916290",
		 "1410023296078","4600916175",
		 "1410023287019","4600915618",
		 "1410009066250","2300503665",
		 "1410023294718","4600916378",
		 "1410009075712","2300503697",
		 "1410023297686","4600915811",
		 "1410009075772","2300503698",
		 "1410023294061","4600916351",
		 "1410023291083","4600916283",
		 "1410023305127","4600916371",
		 "1410009075792","2300503699",
		 "1410009066310","2300503666",
		 "1410009065360","2300503729",
		 "1410023289179","4600916264",
		 "1410023288039","4600916193",
		 "1410023287079","4600915617",
		 "1410023304787","4600915818",
		 "1410023295001","4600915464",
		 "1410023288545","4600916316",
		 "1410023301960","4600916034",
		 "1410023288839","4600916083",
		 "1410023299347","4600915779",
		 "1410023299177","4600915786",
		 "1410023306472","4600916137",
		 "1410023295931","4600916239",
		 "1410023290905","4600915725",
		 "1410023301096","4600915657",
		 "1410023286459","4600915854",
		 "1410023305154","4600915932",
		 "1410023302136","4600915500",
		 "1410023300397","4600915651",
		 "1410023298968","4600915853",
		 "1410023305214","4600915924",
		 "1410023306552","4600915501",
		 "1410023294495","4600915894",
		 "1410023296369","4600915568",
		 "1410023284802","4600915598",
		 "1410023301996","4600915499",
		 "1410023300337","4600915646",
		 "1410023283882","4600915597",
		 "1410023296638","4600915923",
		 "1410023306687","4600915411",
		 "1410023295791","4600915893",
		 "1410023297437","4600915545",
		 "1410023281084","4600916033",
		 "1410023297128","4600915980",
		 "1410023295015","4600915903",
		 "1410023300277","4600915652",
		 "1410023306587","4600915412",
		 "1410023305114","4600915925",
		 "1410023291975","4600915543",
		 "1410023300700","4600915431",
		 "1410023304887","4600915817",
		 "1410023287059","4600915616",
		 "1410023297357","4600915550",
		 "1410023299028","4600915851",
		 "1410023300357","4600915645",
		 "1410023306672","4600915503",
		 "1410023295161","4600915463",
		 "1410023305034","4600915930",
		 "1410023287002","4600915596",
		 "1410023298988","4600915852",
		 "1410023305333","4600915418",
		 "1410023301756","4600915494",
		 "1410023305313","4600915416",
		 "1410023300317","4600915644",
		 "1410023305094","4600915929",
		 "1410023297457","4600915544",
		 "1410023286242","4600915595",
		 "1410023305353","4600915417",
		 "1410023295871","4600915892",
		 "1410023299157","4600915785",
		 "1410023291005","4600915724",
		 "1410023295306","4600916230",
		 "1410023300856","4600915656",
		 "1410023301076","4600915655",
		 "1410023282924","4600916032",
		 "1410023297028","4600915979",
		 "1410023306512","4600916136",
		 "1410023290982","4600915837",
		 "1410023296331","4600915452",
		 "1410023300936","4600915642",
		 "1410023297963","4600915484",
		 "1410023284624","4600915526",
		 "1410023284744","4600915588",
		 "1410023296738","4600915919",
		 "1410023295911","4600915888",
		 "1410023300876","4600915641",
		 "1410023296311","4600915451",
		 "1410023297663","4600915483",
		 "1410023284544","4600915525",
		 "1410023297331","4600915954",
		 "1410023288502","4600915910",
		 "1410023284704","4600915587",
		 "1410023295851","4600915889",
		 "1410023283004","4600916031",
		 "1410023291095","4600916090",
		 "1410023301256","4600915702",
		 "1410023301576","4600915986",
		 "1410023306132","4600916135",
		 "1410023290485","4600915723",
		 "1410023294866","4600916229",
		 "1410023296271","4600915453",
		 "1410023295891","4600915890",
		 "1410023301616","4600915985",
		 "1410023302874","4600916126",
		 "1410023291142","4600915838",
		 "1410023294846","4600916228",
		 "1410023296371","4600915454",
		 "1410023306707","4600915406",
		 "1410023284644","4600915533",
		 "1410023283902","4600915601",
		 "1410023294475","4600915897",
		 "1410023305453","4600915422",
		 "1410023302076","4600915492",
		 "1410023300257","4600915650",
		 "1410023305074","4600915931",
		 "1410023298888","4600915845",
		 "1410023298111","4600915445",
		 "1410023298763","4600915705",
		 "1410023298908","4600915844",
		 "1410023284664","4600915532",
		 "1410023294555","4600915896",
		 "1410023286282","4600915600",
		 "1410023296618","4600915918",
		 "1410023300377","4600915649",
		 "1410023302016","4600915491",
		 "1410023304727","4600915816",
		 "1410023296329","4600915567",
		 "1410004687002","2300503750",
		 "1410023298091","4600915446",
		 "1410023300996","4600915640",
		 "1410023295771","4600915891",
		 "1410004697513","2300503751",
		 "1410023302096","4600915493",
		 "1410004697553","2300503752",
		 "1410023305134","4600915928",
		 "1410023284484","4600915524",
		 "1410023297703","4600915490",
		 "1410004697573","2300503753",
		 "1410023284822","4600915594",
		 "1410004697613","2300503754",
		 "1410004697593","2300503755",
		 "1410023288262","4600915909",
		 "1410004697673","2300503756",
		 "1410004697493","2300503757",
		 "1410023291082","4600915836",
		 "1410004686982","2300503743",
		 "1410023300896","4600915639",
		 "1410023297983","4600915489",
		 "1410023295351","4600915754",
		 "1410023292055","4600915534",
		 "1410023305413","4600915415",
		 "1410023294795","4600915898",
		 "1410004686942","2300503744",
		 "1410023305054","4600915927",
		 "1410004686922","2300503745",
		 "1410004686682","2300503746",
		 "1410023288602","4600915908",
		 "1410023304287","4600915523",
		 "1410023284764","4600915593",
		 "1410004686902","2300503747",
		 "1410023284122","4600915602",
		 "1410023298928","4600915846",
		 "1410023296131","4600915887",
		 "1410004686642","2300503748",
		 "1410023300916","4600915643",
		 "1410004686662","2300503749",
		 "1410023290962","4600915835",
		 "1410004697633","2300503758",
		 "1410004690914","2300503731",
		 "1410004690934","2300503732",
		 "1410023296878","4600915558",
		 "1410023305393","4600915421",
		 "1410023296391","4600915458",
		 "1410023284564","4600915531",
		 "1410023291002","4600915843",
		 "1410023300297","4600915648",
		 "1410023294535","4600915895",
		 "1410023286262","4600915599",
		 "1410023302116","4600915498",
		 "1410023296718","4600915917",
		 "1410023306527","4600915405",
		 "1410023297477","4600915549",
		 "1410023296658","4600915916",
		 "1410023300660","4600915876",
		 "1410023287790","4600915803",
		 "1410023298643","4600915704",
		 "1410023297971","4600915443",
		 "1410023291022","4600915840",
		 "1410023297683","4600915487",
		 "1410023288119","4600916192",
		 "1410023296678","4600915914",
		 "1410023303352","4600915629",
		 "1410023284464","4600915591",
		 "1410023284524","4600915529",
		 "1410023301280","4600915875",
		 "1410023306647","4600915404",
		 "1410023302036","4600915497",
		 "1410023300217","4600915647",
		 "1410023298131","4600915450",
		 "1410023288562","4600915913",
		 "1410023303056","4600915478",
		 "1410023302872","4600915628",
		 "1410023288445","4600916315",
		 "1410023284604","4600915528",
		 "1410023298048","4600915582",
		 "1410004690334","2300503733",
		 "1410004690814","2300503734",
		 "1410004690394","2300503735",
		 "1410023284584","4600915530",
		 "1410023284384","4600915592",
		 "1410023291102","4600915842",
		 "1410023300160","4600915878",
		 "1410004690854","2300503736",
		 "1410004690354","2300503737",
		 "1410004690834","2300503738",
		 "1410004690894","2300503739",
		 "1410023297618","4600915676",
		 "1410004690874","2300503740",
		 "1410023300600","4600915877",
		 "1410023291122","4600915841",
		 "1410023287822","4600915907",
		 "1410023297991","4600915444",
		 "1410023298043","4600915488",
		 "1410023302832","4600915630",
		 "1410004697653","2300503759",
		 "1410004697533","2300503760",
		 "1410004693790","2300503761",
		 "1410004693750","2300503762",
		 "1410004693730","2300503763",
		 "1410023305654","4600915606",
		 "1410023297606","4600915806",
		 "1410004693630","2300503764",
		 "1410004693770","2300503765",
		 "1410023286419","4600915858",
		 "1410004686622","2300503741",
		 "1410004686962","2300503742",
		 "1410023297417","4600915548",
		 "1410023305493","4600915420",
		 "1410004689641","2300503775",
		 "1410004689761","2300503776",
		 "1410004689701","2300503777",
		 "1410023296558","4600915557",
		 "1410004689721","2300503778",
		 "1410004689681","2300503779",
		 "1410023302852","4600915627",
		 "1410023304867","4600915815",
		 "1410023303116","4600915477",
		 "1410004689661","2300503774",
		 "1410023306627","4600915410",
		 "1410023287139","4600915615",
		 "1410023298011","4600915449",
		 "1410023284504","4600915527",
		 "1410023288582","4600915912",
		 "1410023284684","4600915590",
		 "1410023298948","4600915850",
		 "1410023298008","4600915581",
		 "1410023295751","4600915886",
		 "1410023291062","4600915839",
		 "1410023292115","4600915538",
		 "1410004693710","2300503766",
		 "1410023287710","4600916263",
		 "1410004693690","2300503767",
		 "1410004693610","2300503768",
		 "1410004693670","2300503769",
		 "1410004693650","2300503770",
		 "1410004689741","2300503771",
		 "1410004689621","2300503772",
		 "1410023300620","4600915882",
		 "1410004689601","2300503773",
		 "1410023304167","4600915518",
		 "1410023305427","4600916021",
		 "1410023288295","4600916088",
		 "1410023289995","4600916089",
		 "1410023305367","4600916022",
		 "1410023301196","4600915701",
		 "1410023301496","4600915984",
		 "1410023299054","4600916125",
		 "1410023294306","4600916227",
		 "1410023295381","4600915936",
		 "1410023297337","4600915547",
		 "1410023292866","4600916390",
		 "1410023295421","4600915935",
		 "1410023298051","4600915448",
		 "1410023295856","4600915830",
		 "1410023298128","4600915580",
		 "1410023305714","4600915607",
		 "1410023297726","4600915809",
		 "1410023296798","4600915561",
		 "1410004690739","2300864514",
		 "1410023304227","4600915515",
		 "1410023297176","4600915828",
		 "1410023300720","4600915438",
		 "1410023297723","4600915482",
		 "1410023297518","4600915682",
		 "1410023288542","4600915905",
		 "1410004690799","2300864513",
		 "1410023304307","4600915522",
		 "1410023293266","4600916389",
		 "1410023288425","4600916322",
		 "1410023303332","4600915633",
		 "1410023298003","4600915481",
		 "1410023287842","4600915904",
		 "1410023295876","4600915827",
		 "1410023284404","4600915585",
		 "1410023300976","4600915634",
		 "1410023304187","4600915517",
		 "1410023298148","4600915579",
		 "1410023300640","4600915881",
		 "1410004692796","2300864492",
		 "1410023303136","4600915476",
		 "1410023288282","4600915911",
		 "1410004690759","2300864516",
		 "1410004692556","2300864491",
		 "1410023304327","4600915516",
		 "1410023301320","4600915880",
		 "1410023284724","4600915586",
		 "1410023297216","4600915829",
		 "1410004690819","2300864515",
		 "1410023297538","4600915675",
		 "1410023303156","4600915475",
		 "1410023288522","4600915906",
		 "1410023298031","4600915447",
		 "1410023302787","4600915783",
		 "1410004693250","2300864504",
		 "1410023291065","4600915729",
		 "1410023301276","4600915699",
		 "1410004695600","2300864477",
		 "1410023304694","4600916123",
		 "1410004693290","2300864503",
		 "1410004695520","2300864472",
		 "1410023290925","4600915730",
		 "1410023296071","4600916234",
		 "1410004693310","2300864502",
		 "1410023302834","4600916124",
		 "1410023297348","4600915983",
		 "1410023291035","4600916087",
		 "1410023305307","4600916020",
		 "1410004693230","2300864501",
		 "1410004695580","2300864471",
		 "1410023301296","4600915700",
		 "1410023299257","4600915784",
		 "1410004693096","2300864497",
		 "1410023289199","4600916273",
		 "1410023297586","4600915808",
		 "1410023297498","4600915680",
		 "1410023289642","4600916198",
		 "1410023289119","4600916272",
		 "1410023289282","4600916197",
		 "1410023296411","4600915455",
		 "1410023303394","4600916325",
		 "1410023299196","4600916360",
		 "1410023288779","4600916078",
		 "1410023299494","4600916130",
		 "1410023301216","4600915703",
		 "1410023282904","4600916026",
		 "1410023295326","4600916232",
		 "1410023300373","4600915973",
		 "1410023301560","4600916335",
		 "1410023296988","4600915974",
		 "1410023294326","4600916233",
		 "1410023303494","4600916326",
		 "1410023305347","4600916019",
		 "1410023299236","4600916361",
		 "1410023289262","4600916203",
		 "1410023286027","4600915774",
		 "1410023291045","4600915728",
		 "1410023291063","4600916274",
		 "1410023291042","4600915834",
		 "1410023294201","4600915748",
		 "1410023303854","4600915950",
		 "1410023295175","4600916053",
		 "1410023299676","4600916003",
		 "1410023284424","4600915584",
		 "1410023288505","4600916321",
		 "1410023294621","4600915749",
		 "1410023294141","4600915750",
		 "1410023287730","4600916254",
		 "1410023304267","4600915521",
		 "1410023295836","4600915833",
		 "1410023295215","4600916051",
		 "1410023295315","4600916057",
		 "1410023298168","4600915583",
		 "1410023292681","4600915747",
		 "1410023303036","4600915480",
		 "1410023295235","4600916052",
		 "1410023296626","4600916150",
		 "1410023303312","4600915632",
		 "1410023300860","4600915437",
		 "1410023301300","4600915879",
		 "1410023295415","4600916058",
		 "1410023295295","4600916054",
		 "1410023298876","4600916359",
		 "1410023297140","4600915403",
		 "1410023296838","4600915560",
		 "1410023304547","4600915519",
		 "1410023296029","4600915573",
		 "1410004693076","2300864498",
		 "1410023296498","4600915869",
		 "1410023295141","4600915470",
		 "1410004695620","2300864479",
		 "1410023287119","4600915622",
		 "1410023297598","4600915681",
		 "1410004695560","2300864478",
		 "1410004693170","2300864509",
		 "1410023292886","4600916388",
		 "1410023303016","4600915479",
		 "1410023305252","4600915631",
		 "1410023297038","4600915870",
		 "1410004695540","2300864474",
		 "1410023295395","4600916056",
		 "1410023294661","4600915753",
		 "1410023303894","4600915949",
		 "1410004693056","2300864500",
		 "1410023300780","4600915436",
		 "1410023296646","4600916149",
		 "1410023303954","4600915948",
		 "1410004695480","2300864473",
		 "1410023300800","4600915435",
		 "1410023298068","4600915574",
		 "1410023297196","4600915832",
		 "1410004693116","2300864499",
		 "1410023304207","4600915520",
		 "1410004695500","2300864480",
		 "1410023288719","4600916076",
		 "1410004690779","2300864512",
		 "1410023300393","4600915971",
		 "1410023301607","4600915692",
		 "1410023297096","4600915771",
		 "1410004690699","2300864511",
		 "1410023282664","4600916025",
		 "1410023301127","4600915693",
		 "1410023290405","4600915718",
		 "1410004693150","2300864510",
		 "1410023303434","4600916324",
		 "1410023289139","4600916271",
		 "1410023299954","4600916128",
		 "1410023301620","4600916222",
		 "1410004695640","2300864475",
		 "1410023289622","4600916196",
		 "1410023295366","4600916231",
		 "1410023302854","4600916129",
		 "1410023288819","4600916077",
		 "1410023300273","4600915972",
		 "1410023297076","4600915772",
		 "1410023295956","4600915773",
		 "1410023290985","4600915727",
		 "1410023301376","4600915694",
		 "1410023296858","4600915559",
		 "1410023301916","4600916398",
		 "1410023297558","4600915679",
		 "1410023299176","4600916354",
		 "1410023297068","4600915978",
		 "1410023290425","4600915717",
		 "1410023288405","4600916323",
		 "1410023299974","4600916127",
		 "1410023282684","4600916024",
		 "1410023289302","4600916195",
		 "1410023288799","4600916075",
		 "1410023287590","4600916262",
		 "1410023301360","4600916221",
		 "1410023296389","4600915572",
		 "1410023288830","4600915510",
		 "1410023295061","4600915469",
		 "1410023296958","4600915868",
		 "1410023287039","4600915621",
		 "1410023303814","4600915947",
		 "1410023294981","4600915468",
		 "1410023286999","4600915620",
		 "1410023296918","4600915867",
		 "1410023303794","4600915953",
		 "1410023304847","4600915822",
		 "1410004695460","2300864476",
		 "1410023295596","4600915831",
		 "1410023297951","4600915442",
		 "1410004693330","2300864506",
		 "1410004689581","2300864487",
		 "1410023301587","4600915691",
		 "1410023305467","4600916023",
		 "1410023300840","4600915440",
		 "1410023304654","4600916118",
		 "1410023297988","4600915578",
		 "1410023302767","4600915778",
		 "1410023288750","4600915508",
		 "1410004693210","2300864505",
		 "1410004689541","2300864486",
		 "1410004689521","2300864485",
		 "1410023304827","4600915821",
		 "1410023288710","4600915509",
		 "1410023298071","4600915441",
		 "1410004689561","2300864482",
		 "1410023296349","4600915571",
		 "1410004689781","2300864481",
		 "1410023302140","4600915874",
		 "1410023295081","4600915467",
		 "1410023303974","4600915952",
		 "1410023287099","4600915619",
		 "1410004689501","2300864488",
		 "1410023304554","4600916116",
		 "1410023287630","4600916261",
		 "1410023289845","4600916013",
		 "1410023303354","4600916330",
		 "1410023301816","4600916395",
		 "1410023290365","4600915715",
		 "1410023299327","4600915777",
		 "1410023301356","4600915698",
		 "1410023301340","4600916219",
		 "1410023288759","4600916081",
		 "1410023297368","4600915976",
		 "1410023305327","4600916014",
		 "1410023304594","4600916117",
		 "1410023290465","4600915716",
		 "1410023288739","4600916082",
		 "1410023301380","4600916220",
		 "1410023297088","4600915977",
		 "1410009059924","2300503639",
		 "1410009060444","2300503638",
		 "1410023293246","4600916391",
		 "1410009072249","2300503718",
		 "1410023289342","4600916201",
		 "1410009072129","2300503719",
		 "1410009066336","2300503640",
		 "1410009066396","2300503641",
		 "1410009064175","2300503720",
		 "1410009066116","2300503642",
		 "1410009066056","2300503643",
		 "1410009064235","2300503721",
		 "1410009064255","2300503722",
		 "1410023296818","4600915554",
		 "1410009063438","2300503676",
		 "1410023287750","4600916260",
		 "1410023303414","4600916329",
		 "1410023301936","4600916397",
		 "1410023289322","4600916202",
		 "1410023286007","4600915776",
		 "1410023290345","4600915722",
		 "1410023300760","4600915439",
		 "1410023295321","4600915942",
		 "1410023304707","4600915819",
		 "1410023288770","4600915507",
		 "1410023298108","4600915577",
		 "1410023296458","4600915873",
		 "1410023296478","4600915872",
		 "1410023303372","4600915625",
		 "1410009064335","2300503724",
		 "1410009064195","2300503725",
		 "1410009066016","2300503648",
		 "1410009066376","2300503649",
		 "1410009064275","2300503726",
		 "1410009064355","2300503727",
		 "1410009065810","2300503650",
		 "1410009065510","2300503651",
		 "1410004689481","2300864484",
		 "1410009060384","2300503636",
		 "1410009064295","2300503728",
		 "1410023305272","4600915626",
		 "1410023303096","4600915474",
		 "1410009059944","2300503637",
		 "1410009072169","2300503716",
		 "1410009072209","2300503717",
		 "1410023303834","4600915951",
		 "1410004689801","2300864483",
		 "1410023304767","4600915820",
		 "1410023297136","4600915826",
		 "1410023295301","4600915941",
		 "1410023304247","4600915514",
		 "1410023298088","4600915576",
		 "1410023302996","4600915473",
		 "1410023296998","4600915871",
		 "1410023290845","4600915430",
		 "1410023288099","4600916188",
		 "1410023289602","4600916200",
		 "1410023301976","4600916402",
		 "1410023288465","4600916318",
		 "1410023288019","4600916189",
		 "1410009074027","2300503688",
		 "1410009074007","2300503687",
		 "1410009065830","2300503656",
		 "1410009065850","2300503657",
		 "1410009073907","2300503689",
		 "1410009065890","2300503658",
		 "1410009075752","2300503690",
		 "1410023288690","4600915513",
		 "1410023303076","4600915472",
		 "1410023303292","4600915624",
		 "1410009075852","2300503691",
		 "1410009074067","2300503680",
		 "1410009065550","2300503659",
		 "1410023297646","4600915807",
		 "1410009066356","2300503644",
		 "1410009073947","2300503681",
		 "1410009073987","2300503682",
		 "1410009066076","2300503645",
		 "1410009066036","2300503646",
		 "1410009073887","2300503683",
		 "1410009066096","2300503647",
		 "1410023290865","4600915428",
		 "1410023295916","4600915825",
		 "1410023302976","4600915471",
		 "1410023288730","4600915512",
		 "1410023295401","4600915939",
		 "1410023298028","4600915575",
		 "1410023287179","4600915614",
		 "1410023287159","4600915623",
		 "1410023291085","4600915429",
		 "1410023295481","4600915940",
		 "1410023286439","4600915862",
		 "1410023301956","4600916401",
		 "1410023295461","4600915938",
		 "1410023296518","4600915556",
		 "1410023298317","4600915553",
		 "1410023305674","4600915605",
		 "1410023297626","4600915805",
		 "1410023286499","4600915861",
		 "1410023287199","4600915857",
		 "1410023305373","4600915419",
		 "1410023303474","4600916328",
		 "1410023287650","4600916259",
		 "1410023288179","4600916187",
		 "1410023289582","4600916199",
		 "1410023303934","4600915946",
		 "1410023295121","4600915462",
		 "1410023291165","4600915427",
		 "1410023288790","4600915511",
		 "1410023301896","4600916396",
		 "1410023304807","4600915823",
		 "1410023305794","4600915604",
		 "1410023301600","4600916215",
		 "1410023297666","4600915804",
		 "1410023291105","4600915426",
		 "1410023300880","4600915434",
		 "1410023286979","4600915856",
		 "1410023305734","4600915612",
		 "1410023286479","4600915859",
		 "1410023295021","4600915461",
		 "1410023305774","4600915613",
		 "1410023286519","4600915860",
		 "1410023296309","4600915566",
		 "1410023297156","4600915824",
		 "1410023296049","4600915565",
		 "1410023288650","4600915506",
		 "1410023303914","4600915945",
		 "1410023288485","4600916317",
		 "1410023295441","4600915937",
		 "1410023289362","4600916194",
		 "1410023291145","4600915425",
		 "1410023295651","4600915756",
		 "1410023301876","4600916400",
		 "1410023297171","4600915956",
		 "1410023294895","4600916113",
		 "1410023298683","4600915713",
		 "1410023296538","4600915555",
		 "1410023295375","4600916062",
		 "1410023297497","4600915552",
		 "1410023295255","4600916045",
		 "1410023295355","4600916055",
		 "1410023290322","4600916253",
		 "1410023287450","4600915798",
		 "1410023293121","4600915752",
		 "1410023298796","4600915998",
		 "1410023295095","4600916046",
		 "1410023288010","4600915796",
		 "1410023295275","4600916044",
		 "1410023290302","4600916252",
		 "1410023299656","4600915997",
		 "1410023290882","4600916102",
		 "1410023287550","4600915797",
		 "1410023292901","4600915751",
		 "1410023301836","4600916399",
		 "1410023289159","4600916265",
		 "1410023288059","4600916190",
		 "1410023296586","4600916148",
		 "1410023303334","4600916327",
		 "1410023289079","4600916266",
		 "1410023288670","4600915505",
		 "1410023303874","4600915944",
		 "1410023305312","4600915668",
		 "1410023295195","4600916049",
		 "1410023287470","4600915802",
		 "1410023296666","4600916147",
		 "1410023304747","4600915814",
		 "1410023297018","4600915866",
		 "1410023300740","4600915433",
		 "1410023290782","4600916100",
		 "1410023287770","4600915795",
		 "1410023292361","4600915741",
		 "1410023302080","4600916043",
		 "1410023298816","4600915996",
		 "1410023290342","4600916251",
		 "1410023290742","4600916101",
		 "1410023292341","4600915742",
		 "1410023290722","4600916099",
		 "1410023305332","4600915669",
		 "1410023295115","4600916050",
		 "1410023292301","4600915740",
		 "1410023296431","4600915460",
		 "1410023298836","4600915995",
		 "1410023305412","4600915670",
		 "1410023290422","4600916246",
		 "1410023297191","4600915962",
		 "1410023300097","4600916205",
		 "1410023296778","4600915563",
		 "1410023296351","4600915457",
		 "1410023294935","4600916112",
		 "1410023297746","4600915813",
		 "1410023296291","4600915459",
		 "1410023305634","4600915610",
		 "1410023298723","4600915712",
		 "1410023296898","4600915562",
		 "1410023297706","4600915810",
		 "1410023295435","4600916061",
		 "1410023296409","4600915564",
		 "1410023301627","4600915687",
		 "1410023300037","4600916206",
		 "1410023297411","4600915955",
		 "1410023300820","4600915432",
		 "1410023296938","4600915865",
		 "1410023295361","4600915943",
		 "1410009066550","2300503708",
		 "1410009066730","2300503709",
		 "1410009063358","2300503678",
		 "1410009072109","2300503710",
		 "1410009063338","2300503679",
		 "1410009066270","2300503668",
		 "1410023294955","4600916111",
		 "1410023286959","4600915855",
		 "1410023295335","4600916060",
		 "1410023302020","4600916038",
		 "1410023298356","4600916002",
		 "1410023290382","4600916245",
		 "1410023291055","4600916093",
		 "1410023297578","4600915674",
		 "1410023295135","4600916047",
		 "1410023287510","4600915800",
		 "1410023306232","4600916142",
		 "1410023302040","4600916037",
		 "1410023290442","4600916244",
		 "1410023289975","4600916092",
		 "1410023287490","4600915799",
		 "1410023291781","4600915739",
		 "1410023302812","4600915673",
		 "1410023296526","4600916151",
		 "1410023296546","4600916152",
		 "1410023290802","4600916094",
		 "1410023305432","4600915667",
		 "1410023295155","4600916048",
		 "1410023287430","4600915801",
		 "1410009063258","2300503677",
		 "1410023296706","4600916153",
		 "1410023290842","4600916103",
		 "1410009064215","2300503723",
		 "1410023289725","4600916004",
		 "1410023300017","4600916204",
		 "1410023306252","4600916141",
		 "1410023301940","4600916036",
		 "1410023296011","4600916243",
		 "1410023295101","4600915466",
		 "1410023299197","4600915790",
		 "1410023299696","4600916001",
		 "1410023294641","4600915746",
		 "1410023305372","4600915672",
		 "1410023305754","4600915611",
		 "1410023299277","4600915789",
		 "1410023305292","4600915671",
		 "1410023298036","4600916000",
		 "1410023294181","4600915745",
		 "1410023304714","4600916115",
		 "1410023289705","4600916012",
		 "1410023283404","4600916163",
		 "1410023301540","4600916339",
		 "1410023293078","4600916375",
		 "1410023307012","4600916287",
		 "1410023289039","4600916079",
		 "1410023297187","4600915775",
		 "1410023301236","4600915696",
		 "1410023300117","4600916210",
		 "1410023297863","4600916164",
		 "1410023301580","4600916340",
		 "1410023307112","4600916288",
		 "1410023289865","4600916010",
		 "1410023301336","4600915697",
		 "1410023295711","4600915755",
		 "1410023294346","4600916226",
		 "1410023297008","4600915975",
		 "1410023288679","4600916080",
		 "1410023294815","4600916106",
		 "1410023295455","4600916059",
		 "1410023293058","4600916376",
		 "1410023301400","4600916341",
		 "1410023297803","4600916165",
		 "1410023296451","4600915456",
		 "1410023297231","4600915961",
		 "1410023291125","4600915424",
		 "1410023294906","4600916070",
		 "1410023262573","4600915965",
		 "1410023304674","4600916121",
		 "1410023294886","4600916224",
		 "1410023304634","4600916122",
		 "1410004693190","2300864507",
		 "1410023297056","4600915766",
		 "1410023301316","4600915695",
		 "1410023295346","4600916225",
		 "1410023296273","4600915966",
		 "1410023290505","4600915721",
		 "1410023301456","4600915989",
		 "1410023290822","4600916098",
		 "1410004690679","2300864518",
		 "1410023300796","4600915660",
		 "1410004693036","2300864494",
		 "1410023292321","4600915734",
		 "1410023296686","4600916146",
		 "1410023299317","4600915787",
		 "1410023290402","4600916248",
		 "1410023301980","4600916041",
		 "1410004692536","2300864493",
		 "1410023301636","4600915988",
		 "1410004690659","2300864517",
		 "1410023302100","4600916040",
		 "1410023301176","4600915659",
		 "1410023290965","4600915733",
		 "1410004693270","2300864508",
		 "1410023296566","4600916145",
		 "1410023287530","4600915794",
		 "1410023290362","4600916247",
		 "1410004689441","2300864490",
		 "1410023306192","4600916139",
		 "1410023294161","4600915744",
		 "1410023298136","4600915999",
		 "1410023301116","4600915662",
		 "1410004689461","2300864489",
		 "1410023306212","4600916140",
		 "1410023302060","4600916035",
		 "1410023290462","4600916250",
		 "1410023291075","4600916091",
		 "1410023301516","4600915990",
		 "1410023301136","4600915661",
		 "1410023292281","4600915743",
		 "1410004692816","2300864496",
		 "1410004692576","2300864495",
		 "1410004690639","2300864519",
		 "1410023299337","4600915788",
		 "1410023302000","4600916042",
		 "1410023290282","4600916249",
		 "1410023289745","4600916009",
		 "1410023297843","4600916168",
		 "1410023293141","4600916344",
		 "1410023287883","4600916276",
		 "1410023305207","4600916364",
		 "1410023294101","4600916345",
		 "1410023297923","4600916169",
		 "1410004681420","2300864470",
		 "1410023295246","4600916066",
		 "1410023297903","4600916167",
		 "1410023301520","4600916343",
		 "1410023292883","4600916275",
		 "1410023294891","4600915762",
		 "1410023294855","4600916105",
		 "1410023299216","4600916363",
		 "1410023296606","4600916144",
		 "1410023296111","4600916238",
		 "1410023299237","4600915793",
		 "1410023290902","4600916097",
		 "1410023305247","4600916365",
		 "1410023290423","4600916277",
		 "1410023297883","4600916170",
		 "1410023293761","4600916346",
		 "1410023290383","4600916278",
		 "1410023305167","4600916366",
		 "1410023295166","4600916068",
		 "1410023301433","4600915964",
		 "1410023297116","4600915764",
		 "1410023305814","4600915609",
		 "1410023295691","4600915761",
		 "1410023297036","4600915765",
		 "1410023304614","4600916120",
		 "1410023295146","4600916069",
		 "1410023289805","4600916008",
		 "1410023301147","4600915686",
		 "1410023295266","4600916065",
		 "1410023290525","4600915720",
		 "1410023301500","4600916334",
		 "1410023283324","4600916158",
		 "1410023291043","4600916282",
		 "1410023305227","4600916370",
		 "1410023305473","4600915423",
		 "1410023305147","4600916368",
		 "1410023303374","4600916333",
		 "1410023283344","4600916157",
		 "1410023299997","4600916209",
		 "1410023305187","4600916369",
		 "1410023293103","4600916281",
		 "1410023294455","4600915901",
		 "1410023291955","4600915541",
		 "1410023291995","4600915540",
		 "1410023290945","4600915731",
		 "1410023299297","4600915792",
		 "1410023290762","4600916096",
		 "1410023301556","4600915987",
		 "1410023292015","4600915542",
		 "1410023305472","4600915666",
		 "1410023294775","4600915902",
		 "1410023302120","4600916039",
		 "1410023291025","4600915732",
		 "1410023289765","4600916011",
		 "1410023290445","4600915719",
		 "1410023297271","4600915963",
		 "1410023295186","4600916067",
		 "1410023301920","4600916223",
		 "1410023302247","4600915685",
		 "1410023304334","4600916119",
		 "1410023295991","4600916242",
		 "1410023292261","4600915736",
		 "1410009059904","2300503633",

	 };
	 
	SBIT_CT_DBG("<<<<<<<<<<<<<<<<<<<<<<<<<< myIccID >>>>>>>>>>>>>>>>>> %s ",myIccID);
	
    for (i = 0; i < sim_num; i++)
  	{
	   if((strstr(sim_list[i],myIccID))!=NULL)
       {        
			memset(temp_info.sim_num,0,sizeof(temp_info.sim_num));
			strcat(temp_info.sim_num,sim_list[i-1]);
			SBIT_CT_DBG("################### temp_info.sim_num ################## %s\n",temp_info.sim_num);
			return;
	   }
    }
	  
}


#include   <stdio.h>  
#include   <stdlib.h> 
#include   <time.h>  
#include   <string.h>  

typedef   unsigned   char   *POINTER;  
typedef   unsigned   short   int   UINT2;  
typedef   unsigned   long   int   UINT4;  

typedef   struct    
{  
  UINT4   state[4];  
  UINT4   count[2];  
  unsigned   char   buffer[64];  
}   MD5_CTX;  

void   MD5Init(MD5_CTX   *);  
void   MD5Update(MD5_CTX   *,   unsigned   char   *,   unsigned   int);  
void   MD5Final(unsigned   char   [16],   MD5_CTX   *);  

#define   S11   7  
#define   S12   12  
#define   S13   17  
#define   S14   22  
#define   S21   5  
#define   S22   9  
#define   S23   14  
#define   S24   20  
#define   S31   4  
#define   S32   11  
#define   S33   16  
#define   S34   23  
#define   S41   6  
#define   S42   10  
#define   S43   15  
#define   S44   21  

static   unsigned   char   PADDING[64]   =   {  
  0x80,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0  
};  

#define   F(x,   y,   z)   (((x)   &   (y))   |   ((~x)   &   (z)))  
#define   G(x,   y,   z)   (((x)   &   (z))   |   ((y)   &   (~z)))  
#define   H(x,   y,   z)   ((x)   ^   (y)   ^   (z))  
#define   I(x,   y,   z)   ((y)   ^   ((x)   |   (~z)))  

#define   ROTATE_LEFT(x,   n)   (((x)   <<   (n))   |   ((x)   >>   (32-(n))))  

#define   FF(a,   b,   c,   d,   x,   s,   ac)   {     (a)   +=   F   ((b),   (c),   (d))   +   (x)   +   (UINT4)(ac);     (a)   =   ROTATE_LEFT   ((a),   (s));     (a)   +=   (b);       }  
#define   GG(a,   b,   c,   d,   x,   s,   ac)   {     (a)   +=   G   ((b),   (c),   (d))   +   (x)   +   (UINT4)(ac);     (a)   =   ROTATE_LEFT   ((a),   (s));     (a)   +=   (b);       }  
#define   HH(a,   b,   c,   d,   x,   s,   ac)   {     (a)   +=   H   ((b),   (c),   (d))   +   (x)   +   (UINT4)(ac);     (a)   =   ROTATE_LEFT   ((a),   (s));     (a)   +=   (b);       }  
#define   II(a,   b,   c,   d,   x,   s,   ac)   {     (a)   +=   I   ((b),   (c),   (d))   +   (x)   +   (UINT4)(ac);     (a)   =   ROTATE_LEFT   ((a),   (s));     (a)   +=   (b);   }  


inline   void   Encode(unsigned   char   *output,   UINT4   *input,   unsigned   int   len)  
{  
  unsigned   int   i,   j;  
  
  for   (i   =   0,   j   =   0;   j   <   len;   i++,   j   +=   4)   {  
    output[j]   =   (unsigned   char)(input[i]   &   0xff);  
    output[j+1]   =   (unsigned   char)((input[i]   >>   8)   &   0xff);  
    output[j+2]   =   (unsigned   char)((input[i]   >>   16)   &   0xff);  
    output[j+3]   =   (unsigned   char)((input[i]   >>   24)   &   0xff);  
  }  
}  

inline   void   Decode(UINT4   *output,   unsigned   char   *input,   unsigned   int   len)  
{  
  unsigned   int   i,   j;  
  
  for   (i   =   0,   j   =   0;   j   <   len;   i++,   j   +=   4)  
    output[i]   =   ((UINT4)input[j])   |   (((UINT4)input[j+1])   <<   8)   |  
  (((UINT4)input[j+2])   <<   16)   |   (((UINT4)input[j+3])   <<   24);  
}  

inline   void   MD5Transform   (UINT4   state[4],   unsigned   char   block[64])  
{  
  UINT4   a   =   state[0],   b   =   state[1],   c   =   state[2],   d   =   state[3],   x[16];  
  Decode   (x,   block,   64);  
  FF   (a,   b,   c,   d,   x[   0],   S11,   0xd76aa478);   /*   1   */  
  FF   (d,   a,   b,   c,   x[   1],   S12,   0xe8c7b756);   /*   2   */  
  FF   (c,   d,   a,   b,   x[   2],   S13,   0x242070db);   /*   3   */  
  FF   (b,   c,   d,   a,   x[   3],   S14,   0xc1bdceee);   /*   4   */  
  FF   (a,   b,   c,   d,   x[   4],   S11,   0xf57c0faf);   /*   5   */  
  FF   (d,   a,   b,   c,   x[   5],   S12,   0x4787c62a);   /*   6   */  
  FF   (c,   d,   a,   b,   x[   6],   S13,   0xa8304613);   /*   7   */  
  FF   (b,   c,   d,   a,   x[   7],   S14,   0xfd469501);   /*   8   */  
  FF   (a,   b,   c,   d,   x[   8],   S11,   0x698098d8);   /*   9   */  
  FF   (d,   a,   b,   c,   x[   9],   S12,   0x8b44f7af);   /*   10   */  
  FF   (c,   d,   a,   b,   x[10],   S13,   0xffff5bb1);   /*   11   */  
  FF   (b,   c,   d,   a,   x[11],   S14,   0x895cd7be);   /*   12   */  
  FF   (a,   b,   c,   d,   x[12],   S11,   0x6b901122);   /*   13   */  
  FF   (d,   a,   b,   c,   x[13],   S12,   0xfd987193);   /*   14   */  
  FF   (c,   d,   a,   b,   x[14],   S13,   0xa679438e);   /*   15   */  
  FF   (b,   c,   d,   a,   x[15],   S14,   0x49b40821);   /*   16   */  
  GG   (a,   b,   c,   d,   x[   1],   S21,   0xf61e2562);   /*   17   */  
  GG   (d,   a,   b,   c,   x[   6],   S22,   0xc040b340);   /*   18   */  
  GG   (c,   d,   a,   b,   x[11],   S23,   0x265e5a51);   /*   19   */  
  GG   (b,   c,   d,   a,   x[   0],   S24,   0xe9b6c7aa);   /*   20   */  
  GG   (a,   b,   c,   d,   x[   5],   S21,   0xd62f105d);   /*   21   */  
  GG   (d,   a,   b,   c,   x[10],   S22,     0x2441453);   /*   22   */  
  GG   (c,   d,   a,   b,   x[15],   S23,   0xd8a1e681);   /*   23   */  
  GG   (b,   c,   d,   a,   x[   4],   S24,   0xe7d3fbc8);   /*   24   */  
  GG   (a,   b,   c,   d,   x[   9],   S21,   0x21e1cde6);   /*   25   */  
  GG   (d,   a,   b,   c,   x[14],   S22,   0xc33707d6);   /*   26   */  
  GG   (c,   d,   a,   b,   x[   3],   S23,   0xf4d50d87);   /*   27   */  
  GG   (b,   c,   d,   a,   x[   8],   S24,   0x455a14ed);   /*   28   */  
  GG   (a,   b,   c,   d,   x[13],   S21,   0xa9e3e905);   /*   29   */  
  GG   (d,   a,   b,   c,   x[   2],   S22,   0xfcefa3f8);   /*   30   */  
  GG   (c,   d,   a,   b,   x[   7],   S23,   0x676f02d9);   /*   31   */  
  GG   (b,   c,   d,   a,   x[12],   S24,   0x8d2a4c8a);   /*   32   */  
  HH   (a,   b,   c,   d,   x[   5],   S31,   0xfffa3942);   /*   33   */  
  HH   (d,   a,   b,   c,   x[   8],   S32,   0x8771f681);   /*   34   */  
  HH   (c,   d,   a,   b,   x[11],   S33,   0x6d9d6122);   /*   35   */  
  HH   (b,   c,   d,   a,   x[14],   S34,   0xfde5380c);   /*   36   */  
  HH   (a,   b,   c,   d,   x[   1],   S31,   0xa4beea44);   /*   37   */  
  HH   (d,   a,   b,   c,   x[   4],   S32,   0x4bdecfa9);   /*   38   */  
  HH   (c,   d,   a,   b,   x[   7],   S33,   0xf6bb4b60);   /*   39   */  
  HH   (b,   c,   d,   a,   x[10],   S34,   0xbebfbc70);   /*   40   */  
  HH   (a,   b,   c,   d,   x[13],   S31,   0x289b7ec6);   /*   41   */  
  HH   (d,   a,   b,   c,   x[   0],   S32,   0xeaa127fa);   /*   42   */  
  HH   (c,   d,   a,   b,   x[   3],   S33,   0xd4ef3085);   /*   43   */  
  HH   (b,   c,   d,   a,   x[   6],   S34,     0x4881d05);   /*   44   */  
  HH   (a,   b,   c,   d,   x[   9],   S31,   0xd9d4d039);   /*   45   */  
  HH   (d,   a,   b,   c,   x[12],   S32,   0xe6db99e5);   /*   46   */  
  HH   (c,   d,   a,   b,   x[15],   S33,   0x1fa27cf8);   /*   47   */  
  HH   (b,   c,   d,   a,   x[   2],   S34,   0xc4ac5665);   /*   48   */  
  II   (a,   b,   c,   d,   x[   0],   S41,   0xf4292244);   /*   49   */  
  II   (d,   a,   b,   c,   x[   7],   S42,   0x432aff97);   /*   50   */  
  II   (c,   d,   a,   b,   x[14],   S43,   0xab9423a7);   /*   51   */  
  II   (b,   c,   d,   a,   x[   5],   S44,   0xfc93a039);   /*   52   */  
  II   (a,   b,   c,   d,   x[12],   S41,   0x655b59c3);   /*   53   */  
  II   (d,   a,   b,   c,   x[   3],   S42,   0x8f0ccc92);   /*   54   */  
  II   (c,   d,   a,   b,   x[10],   S43,   0xffeff47d);   /*   55   */  
  II   (b,   c,   d,   a,   x[   1],   S44,   0x85845dd1);   /*   56   */  
  II   (a,   b,   c,   d,   x[   8],   S41,   0x6fa87e4f);   /*   57   */  
  II   (d,   a,   b,   c,   x[15],   S42,   0xfe2ce6e0);   /*   58   */  
  II   (c,   d,   a,   b,   x[   6],   S43,   0xa3014314);   /*   59   */  
  II   (b,   c,   d,   a,   x[13],   S44,   0x4e0811a1);   /*   60   */  
  II   (a,   b,   c,   d,   x[   4],   S41,   0xf7537e82);   /*   61   */  
  II   (d,   a,   b,   c,   x[11],   S42,   0xbd3af235);   /*   62   */  
  II   (c,   d,   a,   b,   x[   2],   S43,   0x2ad7d2bb);   /*   63   */  
  II   (b,   c,   d,   a,   x[   9],   S44,   0xeb86d391);   /*   64   */  
  state[0]   +=   a;  
  state[1]   +=   b;  
  state[2]   +=   c;  
  state[3]   +=   d;  
  memset   ((POINTER)x,   0,   sizeof   (x));  
  }  
   
inline   void   MD5Init(MD5_CTX   *context)  
{  
  context->count[0]   =   context->count[1]   =   0;  
  context->state[0]   =   0x67452301;  
  context->state[1]   =   0xefcdab89;  
  context->state[2]   =   0x98badcfe;  
  context->state[3]   =   0x10325476;  
}  

inline   void   MD5Update(MD5_CTX   *context,   unsigned   char   *input,   unsigned   int   inputLen)  
{  
  unsigned   int   i,   index,   partLen;  
  
  index   =   (unsigned   int)((context->count[0]   >>   3)   &   0x3F);  
  if   ((context->count[0]   +=   ((UINT4)inputLen   <<   3))  
    <   ((UINT4)inputLen   <<   3))  
    context->count[1]++;  
  context->count[1]   +=   ((UINT4)inputLen   >>   29);  
  
  partLen   =   64   -   index;  
  
  if   (inputLen   >=   partLen)   {  
    memcpy((POINTER)&context->buffer[index],   (POINTER)input,   partLen);  
    MD5Transform(context->state,   context->buffer);  
    
    for   (i   =   partLen;   i   +   63   <   inputLen;   i   +=   64)  
      MD5Transform   (context->state,   &input[i]);  
    index   =   0;  
  }  
  else  
    i   =   0;  
  
  memcpy((POINTER)&context->buffer[index],   (POINTER)&input[i],   inputLen-i);  
}  

inline   void   MD5Final(unsigned   char   digest[16],   MD5_CTX   *context)  
{  
  unsigned   char   bits[8];  
  unsigned   int   index,   padLen;  
  
  Encode   (bits,   context->count,   8);  
  index   =   (unsigned   int)((context->count[0]   >>   3)   &   0x3f);  
  padLen   =   (index   <   56)   ?   (56   -   index)   :   (120   -   index);  
  MD5Update   (context,   PADDING,   padLen);  
  MD5Update   (context,   bits,   8);  
  Encode   (digest,   context->state,   16);  
  memset   ((POINTER)context,   0,   sizeof   (*context));  
  }  

void   MD5Digest(char   *pszInput,   unsigned   long   nInputSize,   char   *pszOutPut)  
{  
  MD5_CTX   context;  
  unsigned   int   len   =   strlen   (pszInput);  
  
  MD5Init   (&context);  
  MD5Update   (&context,   (unsigned   char   *)pszInput,   len);  
  MD5Final   ((unsigned   char   *)pszOutPut,   &context);  
}  


