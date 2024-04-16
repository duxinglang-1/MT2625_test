#ifndef __H10_MMI_H__
#define __H10_MMI_H__
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
// Jerry add
#define Queue_num  32   /* 队列*/ 

#define STEP_MAX	8000

#define KEY_EINT_PIN  HAL_GPIO_35
#define KEY_EINT_MODE HAL_GPIO_12_EINT12
#define KEY_EINT_NUM  HAL_EINT_NUMBER_12

#define WIFI_EN         HAL_GPIO_10
#define WIFI_RX         HAL_GPIO_12
#define WIFI_TX         HAL_GPIO_13
#define WIFI_WORK_EN    HAL_GPIO_11
#define WIFI_CH         HAL_GPIO_9




#if defined(MTK_H10S_SUPPORT) 

#define BLE_NUM_MAX 50
#define MAC_LEN 18
#define SIGNAL_MIN (-1000)

typedef struct
{
	char mac_str[MAC_LEN];
	int signal_db_value;
	int index;
	int state; /* 是否可以被搜索状态 */
}BLE_DEVICE;
extern BLE_DEVICE BLE_DEVICE_ARRAY[BLE_NUM_MAX];

typedef struct
{
	char mac_str[MAC_LEN];
	int signal_db_value;
}BLE_DEVICE_FILTER;
#endif

typedef enum
{
	M2M_CT_NONE,
    M2M_CT_NEW,
    M2M_CT_SELF_REGISTER,
    M2M_CT_REGISTER,
    M2M_CT_NOTIFY,
    M2M_CT_UPDATE,
    M2M_CT_SEND,  //5
    M2M_CT_DEL,
    M2M_CT_RECV,
    M2M_CT_REGISTER_FAIL,
    M2M_CT_DEL_SUCCESS,
	M2M_CT_total_data_T50,  //10
	M2M_CT_sos_data_T0,
	M2M_CT_gps_data_T29,
	M2M_CT_hartrate_data_T28,
	M2M_CT_data_T100,
	M2M_CT_data_T101,
	M2M_CT_hartblood_data_T45,
	M2M_CT_state_T51,
	M2M_CT_ack_T86,
	M2M_CT_ack_T87,
	M2M_CT_gettime_data,
	M2M_CT_T71,
	M2M_CT_T47,
	M2M_CT_temp_send_Queue, //15
	M2M_CT_CFUN_OFF,
	M2M_CT_CFUN_ON,
	M2M_DISPLAY_BACKLIGHT,
	M2M_SHUTDOWN,
	M2M_SOS_KEY,
	M2M_MENU_KEY,
	M2M_MENU_SET,
	M2M_VC32S,
	M2M_CT_TEMP_T105,
	M2M_CT_SPO2_T106,
	M2M_MQTT_CREATE,
	M2M_MQTT_CONNECT,
	M2M_MQTT_DISCONNECT,
	M2M_MQTT_DATA_IND,
	M2M_MQTT_TCP_DATA_IND,
	M2M_CT_WIFI_INIT,	
	M2M_send_T15_ACK,
    M2M_CT_MAX
}m2m_ct_msg_enum;

typedef enum
{
	DISPLAY_NONE,
	DISPLAY_BACKLIGHT,
	DISPLYA_UPDATE,
	DISPLAY_ANIMATION,
    DISPLAY_POWEROFF,
	DISPLAY_FM,
}display_msg_enum;

typedef struct {
    uint8_t wan_wei;                                
    uint8_t qian_wei;                                 
    uint8_t bai_wei;                                 
    uint8_t shi_wei;                                  
    uint8_t ge_wei;                                 
} Ch_step_cnt_t;

typedef struct
{
	char m2m_send_Queue1[8][256];
}NVRAM_Read_Write1;

typedef struct
{
	char m2m_send_Queue2[8][256];
}NVRAM_Read_Write2;

typedef struct
{
	char m2m_send_Queue3[8][256];
}NVRAM_Read_Write3;

#define REMIND_MAX	10
typedef struct {
	char remind_time[REMIND_MAX][8];
	bool repeat_flag[REMIND_MAX];
}medicine_time_t;

typedef struct
{
	uint32_t value_steps;              /* 总计步数*/
    int previous_steps;                /* 上一个小时计步数据 */
	int total_steps[13];
	uint32_t temp_step;                /* 前5分钟计步数*/
	uint32_t auto_wifi;                /* 自动开启wifi */
	uint32_t stk8323_reset_mark;
	int step_week_log[7];
	char Hr_log[24]; 
	int exception_auto_reboot_num;     /* 24小时内异常自动重启 次数*/
	int network_fail_record;           /* 网络数据发送连续失败次数 */ 
	int Power_saving_flag;             /* 省电模式 flag */
	int Bright_screen_flag;            /* 抬手亮屏 flag */
	int sedentary_flag;                /* 久坐提醒 flag */
	int Factory_Reset_flag;            /* 抬手亮屏 flag */
	int English_German_flag;           /* 德 英切换 flag */
#if defined(MTK_DATA_SYN_SETTING_SUPPORT)	
	int data_syn_flag;                /* APP同步 */
#endif	
    int inited_flag;				   /* 记录是否有被初始化过,如果不为10086,就代表没有初始化过  定时上传的参数等*/	
	char ble_secret_key[24];           /* 蓝牙秘钥 */
	char ble_mac[12][15];              /* 蓝牙mac地址 */
	char blood_lbp[12];                /* 血压校验值低压 */
	char blood_hbp[12];                /* 血压校验值高压 */
	int fm_gps_flag;                   /* 工程模式GPS  flag */
	int move_time;                     /* 移动时间累计，单位为分 */
	int sedentary_time;                /* 久坐时间累计 */
	int step_interval;                 /* 定位步数间隔 */
	int location_interval;             /* 自动定位上报间隔 */
	int rolling_cumulative;            /* 睡眠时滚动累计次数 */
	int light_sleep_time;	           /* 睡眠浅睡眠状态*/
	int deep_sleep_time;               /* 睡眠深睡眠状态*/
	int sleep_reset_flag;              /* 睡眠清空flag */
	int movement_flag;                 /* 中金育能运动模式flag */
	int waggle_level[12];              /* 终端摇动累计次数 */
	uint16_t low_weather[4];
	uint16_t hight_weather[4];
	uint16_t weather_info[16];
	uint16_t weather_pic[4];
	uint16_t weather_pm25[4];
	uint16_t city_weather[32];
	medicine_time_t Monday;
	medicine_time_t Tuesday;
	medicine_time_t Wednesday;
	medicine_time_t Thursday;
	medicine_time_t Friday;
	medicine_time_t Saturday;
	medicine_time_t Sunday;
	int medicine_set_flag;
#ifdef MTK_fm78100_SUPPORT
    int32_t FM78100_CH1_AVG;//ch1_raw_data_avg
    int32_t FM78100_CH2_AVG;//ch2_raw_data_avg 
    double FM78100NV_cali_k;
    double FM78100NV_cali_b;
    double FM78100NV_cali_b2;
    double FM78100NV_OFFSET_ACTIVE_VALUE[5];//保存5组纠偏数据
#endif
	int temp_highest;				   /* 体温最高值 */
	int temp_mode;					   /* 体温测量模式 */
	int German_English_switch;         /*英文,德文切换标志,0为英文,1为德文*/
	int step_intervallogo;             /*step_interval 定位步数间隔后台设置则显示*/
	signed int offset_time;                /*时间校准偏移量 */
}NVRAM_Read_Write;

typedef struct{
	int hartrate_cnt;
	int spo2_cnt;
	uint8_t blood_lbp;
	uint8_t blood_hbp;
	uint8_t stk8321_raise_mark;        /* 抬手亮屏标识*/
	char data_buffer_hex[2048];
	char imei_num[16];
	char imsi_num[16];
	char iccid[21];
	bool m2mclinew_register_flag;
	char cmd_str[128];
	int idle_show_mark;                /* 界面显示标识*/
	int gps_flag;                
	char MY_Latitude[20];
	char MY_Longitude[20];
	unsigned int total_distance[13];
	unsigned int total_calorie[13];
	int vib_conunt;                    /* 马达控制*/
	int vib_mark;                      /* 马达控制*/
	int vib_tips_conunt;               /* 马达控制*/
	int vib_tips_mark;                 /* 马达控制*/
	int gps_num;                       /* gps 数量*/
	int heart_timing_mark;             /* 定时脉搏标识*/
	int registered_network_flag;       /* 是否注册网络标识*/ 
	int network_sending_flag;          /* 网络正在发送数据 flag  ，防止同一条消息发送多次 */ 
	int sos_mark;                      /* sos 标识*/
	int wifi_timing_flag;              /* wifi 定时标识*/
	int stk8321_driver_task_flag;      /* 计步标识*/
	int Valid_status;                  /*	 GPS是否定位成功标识 */
	int gps_off_on_flag;               /* GPS 开关flag*/ 
	int gps_sos_flag;                  /* GPS  开启SOS flag*/
	char gps_speed[10];                /* GPS  速度*/
	int animation_flag;                /* 开机动画*/
	int shut_down_flag;                /* 关机动画, 1 为关机*/
	int m2m_key_flag;                  /* 按键状态*/
	int show_idle_flag;                /* 0 时钟待机界面，1是计步界面，2是心率界面*/ 
	bool sbit_backlight_flag;          /* 背光flag  ，1为亮屏，0为灭屏*/
	unsigned int step_cont_nvram;      /* 开机读取计步数据值*/
	uint32_t backlight_counter;        /* 背光计时*/
	char build_time[15];               /* 软件版本生成时间*/
	int m2m_send_flag;                 /* 数据发送时flag ，防止连续发送数据 */
	int gHeartRatePwrOn;               /* 心率开关flag */
	int Hr_delay;                      /* 心率开关delay */
	int spo2_delay;                    /* 血氧开关delay */
	int bp_delay;                      /* 血压开关delay */
	int gps_delay;                     /* gps 开关delay */
	int ble_unlocking_flag;            /* 蓝牙正在开锁*/
	int ble_unlock_on_flag;            /* 蓝牙开锁 flag*/
	int ble_unlock_success_flag;       /* 蓝牙开锁成功 flag*/
	int ble_connect_flag;              /* 蓝牙连接*/
	int ble_send_flag;                 /* 蓝牙发送flag*/
	int usb_connect_flag;              /* USB 插拔 flag*/
	int batterr_data;                  /* 电池电量 */
	int charging_complete;             /* 充电完成*/
	int shake_flag;                    /* 终端是否在摇动 flag */
	int step_flag;                     /* 终端产生过最新计步 flag */
	int Sedentary_Remind_Time;         /* 久坐提醒计时器 */
	int Sedentary_idle_flag;           /* 久坐提醒idle界面 flag */
	int set_mode_flag;                 /* 设置模式idle界面 flag */
	int set_mode_select;               /* 设置模式选择 */
	int Low_battery_flag;              /* 低电量关机 flag */
	int Low_battery_warning_flag;      /* 低电量警告 flag */
	char total_buffer_wlap[1024];      /* WIFI 名称列表 */
	int wifi_wlap_success;             /* WIFI 热点获取成功 */     
	int wifi_repeat_init_flag;         /* W IFI 重复 init   flag */             
	int wifi_off_on_flag;              /* W IFI 开关flag */             
	int network_time_cumulative;       /* 网络时间累加，每次发送成功一次就清空为 0 */             
	int stk8321_chip_id;               /* 计步ID   */
	char sim_num[15];                  /* sim 卡号 */
	int csq_num;                       /* 网络CSQ值 */
	int fm_mode;                       /* 工程模式 */
	int registered_nb_flag;            /* 是否注册NB服务器flag */
	char record_buff[40][512];         /* 录音buff */
	char record_buff_flag;             /* 录音队列flag  */
	char temp_buff[600];               /* 录音temp buff */
	char send_temp_Queue[256];         /* 发送队列 temp buff */
	int fm_wifi_flag;                  /* 工程模式wifi  flag */
	int ble_off_on_flag;
	int MTK_off_on_flag;
	int max30102_flag;                 /* 血氧 flag */
	float max30102_sp02;               /* 血氧值 */
	int watch_state;                   /* 手表状态 : 白天值小于300为正常佩戴*/
	int activity_idle_flag;            /* 活动量界面  flag */
	int Flight_mode_flag;              /* 切换过飞行模式  flag */
	int no_service_flag;               /* 无服务  flag */
	int gps_delay_flag;                /* 防止频繁的开启定位  flag */
	int weather_screen_flag;           /* 天气界面  flag */
	int weather_updata_flag;           /* 天气数据更新  flag */
	int weather_delay_flag;            /* 进入天气界面  flag */
	int reset_time;                    /* 重启时间 */
	int fm_hr_flag;                    /* 工程模式心率 flag */
	int recv_flag;                     /* 获取天气、位置、下发指令 flag */
	int send_record_flag[Queue_num];   /* 数据发送记录flag*/
	int temp_delay;
	int temp_auto_delay;
	int gps_sos_mark;

#if defined(MTK_H10S_SUPPORT) 
	int BLE_search_success;            /* WIFI 热点获取成功 */     
	int BLE_search_off_on_flag;        /* 蓝牙搜索 开关flag */             
#endif	

	int Hw_Version;                    /* 硬件版本1 为H10A , 2为 H10S */
    

}Read_Write;


typedef enum
{
	IDLE_SCREEN,
	STEP_SCREEN,
	CAL_DIST_SCREEN,
//	ACTIVITY_SCREEN,
	HEART_RATE_SCREEN,// 4
//	SPO2_SCREEN,	 //  
//	BLOOD_SCREEN,	// 
	//TEMP_SCREEN,
	GPS_SCREEN,
	POWEROFF_SCREEN,
	POWEROFF_ANIMATION_SCREEN,
	IMEI_SCREEN,
//	WEATHER_SCREEN,
	MAX_SCREEN,
}display_screen_enum;

typedef struct
{
	char m2m_send_temp_Queue[Queue_num][256];
}Read_Write1;


extern NVRAM_Read_Write1	NVRAM_info1;
extern NVRAM_Read_Write2	NVRAM_info2;
extern NVRAM_Read_Write3	NVRAM_info3;
extern NVRAM_Read_Write	NVRAM_info;
extern Read_Write temp_info;
extern Read_Write1 temp_info1;
extern int getCSQ (void);
extern void maxim_max30102_onoff(bool on);
extern void hal_max30102_driver_task(void);
extern void hal_stk8321_driver_task(void);
extern void client_timer_create(void);  
extern void sbit_show_meun(void);
extern void sbit_m2m_ct_init(void);
extern void sbit_check_battery_data(void);
extern void H10_send_sos_data_T0(void);
extern void H10_send_hartblood_data_T45();
extern void H10_send_total_data_T50();
extern void H10_send_hartrate_data_T28();
extern void sbit_m2m_ct_send_massege(int msg,char *str,int len,bool isr);
extern void sbit_m2m_ct_send_massege_isr(int msg,char *str,int len,bool isr);
extern void sbit_set_vib_tips(int conunt,int mark);
extern void set_phone_functionality(int32_t fun );
extern bool Hrs3300_chip_init();
extern int get_sbit_show_meun_id();
extern void mmi_stop_heart_rate_count(void);
extern void Latitude_change(char *input,char *output);
extern void Longitude_change(char *input,char *output);
extern void sbit_shut_down(void);
extern void xSTK8321_vTaskResume(void);
extern void convert_build_time();
extern void ctiot_lwm2m_client_send_response(char *pdata);
extern hal_wdt_status_t hal_wdt_feed(uint32_t magic);
extern void sbit_show_Sedentary_idle(void);
extern void H10_translate_step_or_hart_data();
extern hal_keypad_key_state_t get_m2m_key(void);
extern void H10_send_gps_data_T29(void);
extern void WIFI_driver_init(void);
extern void WIFI_power_off();
extern void Wifi_At_Send(void);
extern void sbit_show_fm(void);
extern void sos_PopupBox(void);
extern void Icare_wifi_send(char *buff);
extern void H10_send_ble_mac_T92(void);
extern signed short get_m2m_send_Queue(void);
extern hal_rtc_status_t hal_rtc_enter_rtc_mode(void);
extern hal_gpt_status_t H10_sleep_handler(void);
extern void sbit_show_set_idle();
extern void H10_send_T71_data();
extern void sbit_show_gps_idle();
extern void Health_data_send(void);
extern void Iccid_Get_sim(char *myIccID);
extern void draw_string(char *str, int offset_x, int offset_y, color c);
extern void H10_Factory_Reset_T52(void);
extern char* H10_synctime_T53(void);
extern void n1_mqtt_publish(char *data);

extern unsigned int ch_get_utc_sec();

extern int uart_init_success;
extern int wifi_send;
extern char gps_db[32];
extern int blood_step;
extern int SBIT_DISPLAY_BACKLIGHT_TIME;
extern uint8_t sbit_deep_slee_lock;
extern int exception_auto_reboot_mark;    /* 异常自动重启 标识*/
//__H10_TEMP__

extern int g_mqtt_id;
extern int get_m2m_Queue_count(void);


extern void dbg_print(char *fmt,...);
#endif
