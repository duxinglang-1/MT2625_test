/* 
* 	Date:2020-12-24
* 	Author:huzhangli
* 	Version : 1.0
* 	Modification:
*/
#include "m2m_fifo.h"
#include "H10_mmi.h"

typedef	unsigned char u8;/**< used for unsigned 8bit */


#if 1  //huzhangli  修改数据发送缓存队列

static bool FIFO_BAG_Push(FIFO_INFO *fifo,uint8_t* data, uint16_t len)
{
	uint8_t i = 0;
	if ( NULL==data || 0==len || len > MAX_BAG_INFO_SIZE) 
		return;

	if (fifo->counts == 0)
	{
		fifo->tail = 0;
		fifo->head = 0;
	}
	else
	{
		if (fifo->tail == (MAX_BAG_FIFO_SUM-1))
			fifo->tail = 0;
		else
			fifo->tail ++;

		if (fifo->tail == fifo->head)
		{
			if (fifo->head == (MAX_BAG_FIFO_SUM-1))
				fifo->head = 0;
			else
				fifo->head++;
		}
	}
	memset(&fifo->array[fifo->tail], 0, sizeof(T_BAG_INFO)); 
	memcpy(fifo->array[fifo->tail].Info, data, len);
	fifo->array[fifo->tail].Len = len;
	fifo->counts++;
	if (fifo->counts > MAX_BAG_FIFO_SUM)
		fifo->counts = MAX_BAG_FIFO_SUM;

	//dbg_print("FIFO_BAG_Push:[len:%d],[%d,%d],counts:%d",len,fifo->head,fifo->tail,fifo->counts);
	return true;
}

static  T_BAG_INFO * FIFO_BAG_POP(FIFO_INFO *fifo)
{
	uint16_t temp = 0; 
	if (fifo->counts == 0)
		return (NULL);
	if (fifo->counts == 1)
	{
		fifo->counts--;
		temp = fifo->head;

		fifo->tail = 0;
		fifo->head = 0;
	}
	else
	{
		fifo->counts--;
		temp = fifo->head;

		if (fifo->head == (MAX_BAG_FIFO_SUM-1))
			fifo->head = 0;
		else
			fifo->head ++;
	}
	//dbg_print("FIFO_BAG_POP:[%s]",fifo->array[temp].Info);
	return (&fifo->array[temp]);
}

static  T_BAG_INFO * FIFO_BAG_Get(FIFO_INFO *fifo)
{
	uint16_t temp = 0; 

	if (fifo->counts == 0)
		return (NULL);
	//dbg_print("FIFO_BAG_Get:%s",fifo->array[fifo->head].Info);
	return (&fifo->array[fifo->head]);
}

static  uint16_t FIFO_BAG_Flash(FIFO_INFO *fifo)
{
	fifo->counts = 0;
	fifo->tail = 0;
	fifo->head = 0;
	return 1;
}

static  uint16_t FIFO_BAG_Counts(FIFO_INFO *fifo)
{
	return (fifo->counts);
}

#if 1
//这个队列负责存储需要服务器返回的数据
static FIFO_INFO		g_Bag_FIFO={0};
static int m2m_fifo_get_count = 0;

bool M2M_FIFO_BAG_Push(uint8_t* data, uint16_t len)
{
	if((strstr(temp_info.imei_num,"000000000000000"))!=NULL || strlen(temp_info.imei_num) == 0)
	{
		return false;
	}
	return FIFO_BAG_Push(&g_Bag_FIFO,data,len);
}

T_BAG_INFO * M2M_FIFO_BAG_POP()
{
	m2m_fifo_get_count = 0;
	return FIFO_BAG_POP(&g_Bag_FIFO);
}

T_BAG_INFO * M2M_FIFO_BAG_Get()
{
	if(temp_info.shut_down_flag)
	{
		return NULL;
	}
	if(++m2m_fifo_get_count >= 5)
	{
		FIFO_BAG_POP(&g_Bag_FIFO);
		m2m_fifo_get_count = 0;
	}
	return FIFO_BAG_Get(&g_Bag_FIFO);
}

uint16_t M2M_FIFO_BAG_Flash()
{
	return FIFO_BAG_Flash(&g_Bag_FIFO);
}

uint16_t M2M_FIFO_BAG_Counts()
{
	return FIFO_BAG_Counts(&g_Bag_FIFO);
}

char ptr[72][MAX_BAG_INFO_SIZE] = {0};//pvPortMalloc(72*MAX_BAG_INFO_SIZE);

void M2M_FIFO_Update_To_Nvram()
{
	int i=0,j=0,count = 0;
	memset(ptr,0,sizeof(ptr));
	for(i=0;i<72;i++)
	{
		if(M2M_FIFO_BAG_Counts())
		{
			count ++;
			T_BAG_INFO *info_ptr = M2M_FIFO_BAG_POP();
			memcpy(ptr[i],info_ptr->Info,MAX_BAG_INFO_SIZE);
		}
		else
		{
			break;
		}
	}
	dbg_print("M2M_FIFO_Update_To_Nvram:%d",count);
	#if 0
	for(i = 0; i < 8; i++)
	{
		memcpy(NVRAM_info1.m2m_send_Queue1[i],ptr[i]   ,MAX_BAG_INFO_SIZE);
		memcpy(NVRAM_info2.m2m_send_Queue2[i],ptr[8+i] ,MAX_BAG_INFO_SIZE);
		memcpy(NVRAM_info3.m2m_send_Queue3[i],ptr[16+i],MAX_BAG_INFO_SIZE);
		memcpy(NVRAM_info4.m2m_send_Queue4[i],ptr[24+i],MAX_BAG_INFO_SIZE);
		memcpy(NVRAM_info5.m2m_send_Queue5[i],ptr[32+i],MAX_BAG_INFO_SIZE);
		memcpy(NVRAM_info6.m2m_send_Queue6[i],ptr[40+i],MAX_BAG_INFO_SIZE);
		memcpy(NVRAM_info7.m2m_send_Queue7[i],ptr[48+i],MAX_BAG_INFO_SIZE);
		memcpy(NVRAM_info8.m2m_send_Queue8[i],ptr[56+i],MAX_BAG_INFO_SIZE);
		memcpy(NVRAM_info9.m2m_send_Queue9[i],ptr[64+i],MAX_BAG_INFO_SIZE);
	}
	#endif
}

void M2M_FIFO_Bag_Init()
{
	int i=0,count=0;
	bool is_update = false;
	#if 0
	//char (*ptr)[72][MAX_BAG_INFO_SIZE] = pvPortMalloc(72*MAX_BAG_INFO_SIZE);
	//if(ptr == NULL)
	{
	//	return;
	}
	memset(ptr,0,72*MAX_BAG_INFO_SIZE);
	for(i = 0; i < 8; i++)
	{
		memcpy(ptr[i],NVRAM_info1.m2m_send_Queue1[i],MAX_BAG_INFO_SIZE);
		memcpy(ptr[8+i],NVRAM_info2.m2m_send_Queue2[i],MAX_BAG_INFO_SIZE);
		memcpy(ptr[16+i],NVRAM_info3.m2m_send_Queue3[i],MAX_BAG_INFO_SIZE);
		memcpy(ptr[24+i],NVRAM_info4.m2m_send_Queue4[i],MAX_BAG_INFO_SIZE);
		memcpy(ptr[32+i],NVRAM_info5.m2m_send_Queue5[i],MAX_BAG_INFO_SIZE);
		memcpy(ptr[40+i],NVRAM_info6.m2m_send_Queue6[i],MAX_BAG_INFO_SIZE);
		memcpy(ptr[48+i],NVRAM_info7.m2m_send_Queue7[i],MAX_BAG_INFO_SIZE);
		memcpy(ptr[56+i],NVRAM_info8.m2m_send_Queue8[i],MAX_BAG_INFO_SIZE);
		memcpy(ptr[64+i],NVRAM_info9.m2m_send_Queue9[i],MAX_BAG_INFO_SIZE);
	}

	memset(&NVRAM_info1,0,sizeof(NVRAM_info1));
	memset(&NVRAM_info2,0,sizeof(NVRAM_info2));
	memset(&NVRAM_info3,0,sizeof(NVRAM_info3));
	memset(&NVRAM_info4,0,sizeof(NVRAM_info4));
	memset(&NVRAM_info5,0,sizeof(NVRAM_info5));
	memset(&NVRAM_info6,0,sizeof(NVRAM_info6));
	memset(&NVRAM_info7,0,sizeof(NVRAM_info7));
	memset(&NVRAM_info8,0,sizeof(NVRAM_info8));
	memset(&NVRAM_info9,0,sizeof(NVRAM_info9));

	for(i=0;i<72;i++)
	{
		if(strlen(ptr[i])>0)
		{
			is_update = true;
			M2M_FIFO_BAG_Push(ptr[i],strlen(ptr[i]));
		}
	}

	dbg_print("M2M_FIFO_BAG_Push");
	if(is_update)
	{
		dbg_print("M2M_FIFO_Bag_Init:%d",M2M_FIFO_BAG_Counts());
		m2m_fifo_bag_nvram_write();
	}
	//vPortFree(ptr);
	#endif
}

void M2M_Fifo_Power_On_Init()
{
	M2M_FIFO_BAG_Flash();
	m2m_fifo_bag_nvram_read();
	M2M_FIFO_Bag_Init();
}

static int open_time_count=0;

int Get_Power_On_Time()
{
	return open_time_count;
}

TimerHandle_t m2m_send_timer = NULL;
bool g_is_need_rsp = false;
int send_fail_count = 0;
void M2M_Send_Timeout()
{
	g_is_need_rsp = false;
	if(m2m_send_timer != NULL)
	{
		xTimerStop(m2m_send_timer,0);
	}
	
	if(++send_fail_count >= 5)
	{
		delay_cfun_OffOn(5);
		send_fail_count = 0;
	}
}

void M2M_Auto_Reset()
{
	if(temp_info.usb_connect_flag)
	{
		hal_rtc_enter_forced_reset_mode();
	}
}
extern char g_wear_status;

void M2M_Timer_Print()
{
	hal_rtc_time_t curtime,utc_time; 
	hal_rtc_get_time(&curtime);
	hal_rtc_get_utc_time(&utc_time);

	if(open_time_count % 30 ==0)
	{
		//dbg_print("\r\n\r\nrtc:%d-%d-%d  %d:%d",curtime.rtc_year,curtime.rtc_mon,curtime.rtc_day,curtime.rtc_hour,curtime.rtc_min);
		//dbg_print("utc:%d-%d-%d  %d:%d",utc_time.rtc_year,utc_time.rtc_mon,utc_time.rtc_day,utc_time.rtc_hour,utc_time.rtc_min);
	}

	open_time_count++;

}

void M2M_Send_Start()
{
	if(m2m_send_timer == NULL)
	{
		m2m_send_timer = xTimerCreate("delect_delay", 75*1000 / portTICK_PERIOD_MS, pdFALSE, (void *) 0, M2M_Send_Timeout);
	}
	xTimerStart(m2m_send_timer,1);
	g_is_need_rsp = true;
}

void  M2M_Send_Timer_Pro()
{
	static int need_rsp_count=0;
	if(open_time_count < 12)
	{
		return;
	}
	if(g_is_need_rsp)
	{
		if(++need_rsp_count >= 60)
		{
			need_rsp_count = 0;
			g_is_need_rsp = false;
		}
	}
	else
	{
		need_rsp_count = 0;
	}
	//if(g_is_need_rsp == false && M2M_FIFO_BAG_Counts()>0&&(NVRAM_info.net_reg_iot_sta==1)&&((g_m2m_info.csq_num>=5)&&(g_m2m_info.csq_num<32)))
	{
		T_BAG_INFO *info_ptr = NULL;
		info_ptr = M2M_FIFO_BAG_Get();
		if(info_ptr != NULL)
		{
			dbg_print(info_ptr->Info);
			//str_to_hex(info_ptr->Info,NVRAM_info.data_buffer_hex,sizeof(g_m2m_info.data_buffer_hex));
			//sbit_m2m_ct_send_hanlde(g_m2m_info.data_buffer_hex,strlen(g_m2m_info.data_buffer_hex));
			M2M_Send_Start();
		}
	}
}


#endif

#endif

#if 1

#define __TRACK_RELEASE__
#ifdef __TRACK_RELEASE__
u8 g_dbg_output= 0;
#else
u8 g_dbg_output= 0;
#endif

#define MAXFRACT    	10000
#define MAXCHARS     2049
#define NumFract     4
SemaphoreHandle_t g_dbgprint_sem = NULL;
char print_buf[MAXCHARS];
/*
	%08.2f 解释:
	%开始符
	0是 "填空字元" 表示,如果长度不足时就用0来填满。
	8格式化后总长度
	2f小数位长度，即2位
*/
//12345.6789  %08.2f  格式化总长度8,小数点后2位  
//这个函数只负责转换小数点后的部分
//char buffer[500]={0};
void track_itof(char **buf, int i)
{
	char *s;
#define LEN	20
	int rem, j;
	static char rev[LEN+1];

	rev[LEN] = 0;
	s = &rev[LEN];
	for (j= 0 ; j < NumFract ; j++)
		{
		rem = i % 10;
			*--s = rem + '0';
		i /= 10;
		}
	while (*s)
		{
		(*buf)[0] = *s++;
		++(*buf);
		}
}

void track_itoa(char **buf, int i, int base)
{
	char *s;
#define LEN	20
	int rem;
	static char rev[LEN+1];

	rev[LEN] = 0;
	if (i == 0)
	{
		(*buf)[0] = '0';
		++(*buf);
		return;
	}
	s = &rev[LEN];
	while (i)
	{
		rem = i % base;
		if (rem < 10)
			*--s = rem + '0';
		else if (base == 16)
			*--s = "abcdef"[rem - 10];
		i /= base;
	}
	while (*s)
	{
		(*buf)[0] = *s++;
		++(*buf);
	}
}

void dbg_print(char *fmt,...)
{
	if(g_dbg_output)
	{
		if(g_dbgprint_sem == NULL)
		{
			g_dbgprint_sem = xSemaphoreCreateBinary();
			xSemaphoreGive(g_dbgprint_sem);
		}
	
	   xSemaphoreTake(g_dbgprint_sem, portMAX_DELAY);
	   va_list ap;
	   double dval;
	   int ival;
	   char *p, *sval;
	   char *bp, cval;
	   int fract;
	   unsigned short len;
		bp= print_buf;
		memset(print_buf,0,sizeof(print_buf));
		*bp= 0;

		va_start(ap, fmt);
		for (p= fmt; *p; p++)
		{
			if (*p != '%')
			{
				*bp++= *p;
				continue;
			}
			switch (*++p) {
			case 'd':
				ival= va_arg(ap, unsigned int);
				if (ival < 0){
					*bp++= '-';
					 ival= -ival;
				}
				track_itoa (&bp, ival, 10);
				break;
				
				case 'o':
				ival= va_arg(ap, int);
				if (ival < 0){
					*bp++= '-';
					 ival= -ival;
				}
				*bp++= '0';
				track_itoa (&bp, ival, 8);
				break;
				
			case 'x':
			case 'X':
				ival= va_arg(ap, int);
				if (ival < 0){
					 *bp++= '-';
					 ival= -ival;
				}
				*bp++= '0';
				*bp++= 'x';
				track_itoa (&bp, ival, 16);
				break;
				
			case 'c':
				cval= va_arg(ap, int);
				*bp++= cval;
				break;
				
			case 'f':
				dval= va_arg(ap, double);
				if (dval < 0){
					*bp++= '-';
					dval= -dval;
				}
				if (dval >= 1.0)
					track_itoa (&bp, (int)dval, 10);
					else
					*bp++= '0';
				*bp++= '.';
				fract= (int)((dval- (double)(int)dval)*(double)(MAXFRACT));
						track_itof(&bp, fract);
				break;
				
			case 's':
				for (sval = va_arg(ap, char *) ; *sval ; sval++ )
					*bp++= *sval;
				break;
			}
		}

		if(*(bp-2) == '\n' || *(bp-1) == '\n')
		{}
		else
		{
			if(strlen(print_buf) >= 2)
			{
				*bp++ = '\r';
				*bp++ = '\n';
			}
		}
		*bp= 0;
		len = (unsigned short)(bp - print_buf);
		va_end (ap);
		print_buf[MAXCHARS-1]='\0';
		//printf("%s",print_buf);
		ctiot_lwm2m_client_send_response(print_buf);
		xSemaphoreGive(g_dbgprint_sem);
   }
}

#endif



#define NRF_OTA_FILE_END	0x83891B8

uint32_t ext_flash_start_addr = (NRF_OTA_FILE_END - FLASH_BASE);



void test_ext_flash_write()
{
#if 0
	hal_flash_status_t ret;

	flash_mdl_erase(ext_flash_start_addr,HAL_FLASH_BLOCK_32K);
	
	ret = flash_mdl_write(ext_flash_start_addr,g_Bag_FIFO.array,sizeof(g_Bag_FIFO.array));
	SBIT_DBG("test_ext_flash_write:%d",ret);
#endif	
}


void test_ext_flash_read()
{
#if 0

	hal_flash_status_t ret;
	ret = flash_mdl_read(ext_flash_start_addr,g_Bag_FIFO.array,sizeof(g_Bag_FIFO.array));
	SBIT_DBG("test_ext_flash_read:%d",ret);
	SBIT_DBG("test_ext_flash_read1:%s",g_Bag_FIFO.array[0].Info);

	SBIT_DBG("test_ext_flash_read2:%s",g_Bag_FIFO.array[MAX_BAG_FIFO_SUM-1].Info);
#endif	
}

void test_ext_flash_init()
{
#if 0

	int i=0;
	return;
	memset(&g_Bag_FIFO,0,sizeof(g_Bag_FIFO));
	for(i=0;i<MAX_BAG_FIFO_SUM;i++)
	{
		strcpy(g_Bag_FIFO.array[i].Info,"hello world!!!");
	}
	test_ext_flash_write();

	test_ext_flash_read();
#endif	
}

