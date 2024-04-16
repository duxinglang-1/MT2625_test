/* Copyright Statement:
 *
 * (C) 2005-2016  MediaTek Inc. All rights reserved.
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. ("MediaTek") and/or its licensors.
 * Without the prior written permission of MediaTek and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 * You may only use, reproduce, modify, or distribute (as applicable) MediaTek Software
 * if you have agreed to and been bound by the applicable license agreement with
 * MediaTek ("License Agreement") and been granted explicit permission to do so within
 * the License Agreement ("Permitted User").  If you are not a Permitted User,
 * please cease any access or use of MediaTek Software immediately.
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT MEDIATEK SOFTWARE RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES
 * ARE PROVIDED TO RECEIVER ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

#include "nvdm.h"
#include "hal_rtc_external.h"
#include "hal_rtc_internal.h"
#include "hal_sleep_driver.h"

#include "ril.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "task_def.h"

#include "memory_attribute.h"

#include "liblwm2m.h"
#include "er-coap-13.h"
#include "tel_conn_mgr_common.h"

#include "syslog.h"

#include "app_common_header.h"
#include "nb_app_config.h"
#include "tel_conn_mgr_bearer_iprot.h"
#include "tel_conn_mgr_common_def.h"
#include "tel_conn_mgr_app_api.h"
#include "string.h"

#include "util.h"
#include "timers.h"


#define APP_DATA_COLLECT_TASK_QUEUE_SIZE                   (20)
#define APP_DATA_COLLECT_TASK_NAME                         "DACL"
#define APP_DATA_COLLECT_TASK_STACK_SIZE                   (1024 * 4)
#define APP_DATA_COLLECT_TASK_PRIORITY                     (TASK_PRIORITY_NORMAL)


#define APP_BEARER_CONFIGURE_QUEUE_SIZE                    (20)
#define APP_BEARER_CONFIGURE_TASK_NAME                     "ABEC"
#define APP_BEARER_CONFIGURE_TASK_STACK_SIZE               (1024 * 4)
#define APP_BEARER_CONFIGURE_TASK_PRIORITY                 (TASK_PRIORITY_NORMAL)


#define APP_LWM2M_CLIENT_TASK_QUEUE_SIZE                   (2)
#define APP_LWM2M_CLIENT_TASK_NAME                         "LM2M"
#if defined(WITH_MBEDTLS)
#define APP_LWM2M_CLIENT_TASK_STACK_SIZE                   (1024 * 8)
#elif defined(WITH_TINYDTLS)
#define APP_LWM2M_CLIENT_TASK_STACK_SIZE                   (1024 * 6)
#else
#define APP_LWM2M_CLIENT_TASK_STACK_SIZE                   (1024 * 4)
#endif
#define APP_LWM2M_CLIENT_TASK_PRIORITY                     (TASK_PRIORITY_NORMAL) 

#define FLOAT_ZERO                                         (0.00001)

#define MSG_ID_URC_REG_START (0x191)

//#define NOTIFICATION_UPDATE_TIME                           (4 * 60 * 10) //100ms unit


#define NOTIFICATION_UPDATE_TIME                  (240 * 10) //100ms unit

#define __NB_APP_ZERO__                           0

typedef struct nb_app_message_struct
{
    unsigned int message_id;
    void *param;
} nb_app_message_struct;

enum {
    PROMPT_TIME = 1,
    NORMAL_TIME
}; 
 
QueueHandle_t nb_data_control_queue = NULL;
TaskHandle_t nb_data_ctrl_task = NULL;

QueueHandle_t nb_bearer_configure_queue = NULL;
TaskHandle_t nb_bearer_cfg_task = NULL;

nb_app_context g_nb_app_context = {0};

uint32_t g_nb_lwm2m_flag = 0;

static TimerHandle_t data_collect_rtos_timer = NULL; /**< Timer handler. */


const static char *app_lock_sleep_name = "app_2625_locker";
static uint8_t app_lock_sleep_handle;
static uint8_t app_lock_count = 0;

log_create_module(lwm2m_app_c, PRINT_LEVEL_INFO);

#define LWM2MAPPLOG(fmt, args...)  LOG_I(lwm2m_app_c, "[lwm2m_app_c]"fmt, ##args);

//#define LWM2MAPPLOG printf

#define LWM2MAPPPRINT printf


//data rentation data
ATTR_ZIDATA_IN_RETSRAM lwm2m_app_object_data g_collect_data;
ATTR_ZIDATA_IN_RETSRAM uint32_t app_rtc_handle;

static TimerHandle_t g_lwm2m_client_ping_timer;

static uint8_t g_lwm2m_client_sleep_handle;

static bool g_lwm2m_client_is_locking;

/********************************data rentetion*************************************/
typedef enum {
    RUNNING_MODE = 1,
    DEEP_SLEEP_MODE = 2
}RTC_TIMEPOUT_REASON;


#define POWERKEY_GET_SLEEP_HANDLE() \
do{ \
    app_lock_sleep_handle = hal_sleep_manager_set_sleep_handle(app_lock_sleep_name); \
    if(app_lock_sleep_handle == INVALID_SLEEP_HANDLE ) { \
        LWM2MAPPLOG("[app]:get sleep handle failed\r\n"); \
        return false; \
    } \
}while(0)

void app_send_msg_internal(unsigned int msg_id, void *param);

void app_send_msg_internal_from_isr(unsigned int msg_id, void *param);

void lwm2m_client_init();


void lockSleep()
{
    hal_sleep_manager_status_t ret = HAL_SLEEP_MANAGER_OK;
    
    ret = hal_sleep_manager_acquire_sleeplock(app_lock_sleep_handle,
                                        HAL_SLEEP_LOCK_DEEP);
    LWM2MAPPLOG("lock--ret: %d, count: %d", ret, ++app_lock_count);
}

void unlockSleep()
{
    hal_sleep_manager_status_t ret = HAL_SLEEP_MANAGER_OK;

    ret = hal_sleep_manager_release_sleeplock(app_lock_sleep_handle,
                                        HAL_SLEEP_LOCK_DEEP);
    LWM2MAPPLOG("unlock--ret: %d, count: %d", ret, --app_lock_count);
}

/*********************************************************************************/


extern int lwm2m_stringToUri(const char * buffer,
                      size_t buffer_len,
                      lwm2m_uri_t * uriP);


extern void lwm2m_client_quit(char *title, int quit);

/////////////////////////////////// msg common part /////////////////////////////////

void app_send_msg(unsigned int msg_id, void *param)
{
    app_send_msg_internal(msg_id, param);
}

void app_send_msg_from_isr(unsigned int msg_id, void *param)
{
    app_send_msg_internal_from_isr(msg_id, param);
}



/*****************************************************************************
* FUNCTION
*  app_send_msg_internal
* DESCRIPTION
* 
* PARAMETERS
*  event_id        [in]
*  param1          [in]
*  param2          [in]
* RETURNS
*  int32_t
*****************************************************************************/
void app_send_msg_internal(unsigned int msg_id, void *param)
{
     nb_app_message_struct app_message;
     //app_message = pvPortMalloc(sizeof(nb_app_message_struct) * sizeof(unsigned char));
     app_message.message_id = msg_id;
     app_message.param = param;

     LWM2MAPPLOG("App app_send_msg_internal id: %d param: %d\r\n",msg_id, app_message.param);

     if (pdTRUE != xQueueSend(nb_data_control_queue, &app_message, 10)) {
         LWM2MAPPLOG("App app_send_msg_internal queue full\r\n");
     }
}


/*****************************************************************************
* FUNCTION
*  app_send_msg_internal_from_isr
* DESCRIPTION
* 
* PARAMETERS
*  event_id        [in]
*  param1          [in]
*  param2          [in]
* RETURNS
*  int32_t
*****************************************************************************/
void app_send_msg_internal_from_isr(unsigned int msg_id, void *param)
{
     nb_app_message_struct app_message;
	 BaseType_t xHigherPriorityTaskWoken = pdFALSE;
     //app_message = pvPortMalloc(sizeof(nb_app_message_struct) * sizeof(unsigned char));
     app_message.message_id = msg_id;
     app_message.param = param;

     LWM2MAPPLOG("App app_send_msg_internal id: %d param: %d\r\n", msg_id, param);

     if ((nb_data_control_queue) && (pdTRUE != xQueueSendFromISR(nb_data_control_queue, &app_message, &xHigherPriorityTaskWoken))) {
         LWM2MAPPLOG("App app_send_msg_internal queue full\r\n");
     }
}


//////////////////////////////////////////////////////////////////////////////////////


/*****************************************************************************
* FUNCTION
*  set_object_value
* DESCRIPTION
* 
* PARAMETERS
*  context        [in]
*  res_uri_string          [in]
*  val          [in]
* RETURNS
*  void
*****************************************************************************/
static void set_object_value(lwm2m_context_t * context,
                                char* res_uri_string, 
                                double val)
{ 
    char new_value[15] = {0};
    lwm2m_uri_t uri;
    
    if (lwm2m_stringToUri(res_uri_string, strlen(res_uri_string), &uri))
    {
        object_ftoa(val, new_value, 2);
        LWM2MAPPLOG("New Value: %s\n", new_value);
        handle_value_changed(context, &uri, new_value, strlen(new_value));
    }
}


#if __NB_APP_ZERO__
//////////////////////////////////////  barometer part  /////////////////////////////////

/*****************************************************************************
* FUNCTION
*  barometer_set_value
* DESCRIPTION
* 
* PARAMETERS
*  context        [in]
*  value          [in]
* RETURNS
*  void
*****************************************************************************/
static void barometer_set_value(lwm2m_context_t * context,
                              double value)
{
    if (value < 0) value = -value;    
    set_object_value(context, get_barometer_uri_string(), value);
}
#endif


/*****************************************************************************
* FUNCTION
*  check_barometer_data_buffer_is_full
* DESCRIPTION
* 
* PARAMETERS
*
* RETURNS
*  bool
*****************************************************************************/
bool check_barometer_data_buffer_is_full()
{
    if(g_nb_app_context.barometer_blocks.current_use_num >= g_nb_app_context.barometer_blocks.total_num) {
        return true;
    }else {
        return false;
    }
}


/*****************************************************************************
* FUNCTION
*  add_barometer_collect_data
* DESCRIPTION
* 
* PARAMETERS
* value [in]
* RETURNS
*  void
*****************************************************************************/
void add_barometer_collect_data(double value)
{
    nb_barometer_data_blocks *datas = &(g_nb_app_context.barometer_blocks);
    barometer_sensor_data *ptr_sensor_data = NULL;
    char new_value[15] = {0};

    object_ftoa(value, new_value, 2);    
    LWM2MAPPLOG("App current data index: %d, value: %s", datas->current_use_num, new_value);

    ptr_sensor_data = datas->sensor_barometer + datas->current_use_num;

    datas->current_use_num += 1;

    ptr_sensor_data->value = value;
}


/*****************************************************************************
* FUNCTION
*  get_barometer_data
* DESCRIPTION
* get data for test
* PARAMETERS
*
* RETURNS
*  double
*****************************************************************************/
double get_barometer_data()
{
    nb_barometer_data_blocks *datas = &(g_nb_app_context.barometer_blocks);

    return datas->sensor_barometer[0].value;
}



/*****************************************************************************
* FUNCTION
*  barometer_data_collect
* DESCRIPTION
*  barometer sensor event need to rec
* PARAMETERS
* value [In]
* RETURNS
*  void
*****************************************************************************/
void barometer_data_collect(double value)
{
    if(!check_barometer_data_buffer_is_full()) {
        add_barometer_collect_data(value);
    }
    else {
        app_send_msg_internal(msg_data_collect_complete, ((int *)PROMPT_TIME));
    }
}


/*****************************************************************************
* FUNCTION
*  reset_barometer_data
* DESCRIPTION
*  
* PARAMETERS
* value [In]
* RETURNS
*  void
*****************************************************************************/
void reset_barometer_data()
{
    memset(&(g_nb_app_context.barometer_blocks), 0, sizeof(g_nb_app_context.barometer_blocks));
    g_nb_app_context.barometer_blocks.total_num = 1;
}

//////////////////////////////////////  barometer part  End /////////////////////////////////




////////////////////////////////////// accelerometer part ///////////////////////////////////

/*****************************************************************************
* FUNCTION
*  accelerometer_set_value
* DESCRIPTION
*  
* PARAMETERS
* context [In]
* x [In]
* y [In]
* z [In]
* RETURNS
*  void
*****************************************************************************/
static void accelerometer_set_value(lwm2m_context_t * context,
                               double x,
                               double y,
                               double z)
{    
    set_object_value(context, get_accelerometer_uri_string('x'), x);

    set_object_value(context, get_accelerometer_uri_string('y'), y);

    set_object_value(context, get_accelerometer_uri_string('z'), z);
}


/*****************************************************************************
* FUNCTION
*  check_accelerometer_data_buffer_is_full
* DESCRIPTION
*  
* PARAMETERS
*
* RETURNS
*  bool
*****************************************************************************/
bool check_accelerometer_data_buffer_is_full()
{
    if(g_nb_app_context.accelerometer_blocks.current_use_num >= g_nb_app_context.accelerometer_blocks.total_num) {
        return true;
    }else {
        return false;
    }
}


/*****************************************************************************
* FUNCTION
*  check_accelerometer_data_buffer_is_empty
* DESCRIPTION
*  check_accelerometer_data_buffer_is_empty
* PARAMETERS
*
* RETURNS
*  bool
*****************************************************************************/
bool check_accelerometer_data_buffer_is_empty()
{
    if(0 == g_nb_app_context.accelerometer_blocks.current_use_num) {
        return true;
    }else {
        return false;
    }
}



/*****************************************************************************
* FUNCTION
*  add_accelerometer_collect_data
* DESCRIPTION
*  
* PARAMETERS
* context [In]
* x [In]
* y [In]
* z [In]
* RETURNS
*  void
*****************************************************************************/
void add_accelerometer_collect_data(double value_x, double value_y, double value_z)
{
    nb_accelerometer_data_blocks *datas = &(g_nb_app_context.accelerometer_blocks);
    accelerometer_sensor_data *ptr_sensor_data = NULL;

    char new_value_x[15] = {0};
    char new_value_y[15] = {0};
    char new_value_z[15] = {0};
    object_ftoa(value_x, new_value_x, 2);
    object_ftoa(value_y, new_value_y, 2);  
    object_ftoa(value_z, new_value_z, 2);  
    LWM2MAPPLOG("App current data, value_x: %s,  value_y: %s    value_z: %s", 
        new_value_x, new_value_y, new_value_z);

    ptr_sensor_data = datas->sensor_accelerometer + datas->current_use_num;

    datas->current_use_num += 1;

    ptr_sensor_data->value_x = value_x;
    ptr_sensor_data->value_y = value_y;
    ptr_sensor_data->value_z = value_z;
}


/*****************************************************************************
* FUNCTION
*  get_accelerometer_data
* DESCRIPTION
*  
* PARAMETERS
* x [In]
* y [In]
* z [In]
* RETURNS
*  void
*****************************************************************************/
void get_accelerometer_data(double *val_x, double *val_y, double *val_z)
{
    nb_accelerometer_data_blocks *datas = &(g_nb_app_context.accelerometer_blocks);
    char new_value_x[15] = {0};
    char new_value_y[15] = {0};
    char new_value_z[15] = {0};
    
    *val_x = datas->sensor_accelerometer[0].value_x;
    *val_y = datas->sensor_accelerometer[0].value_y;
    *val_z = datas->sensor_accelerometer[0].value_z;


    
    object_ftoa(*val_x, new_value_x, 2);
    object_ftoa(*val_y, new_value_y, 2);  
    object_ftoa(*val_z, new_value_z, 2);  
    LWM2MAPPLOG("get_accelerometer_data value_x: %s,  value_y: %s    value_z: %s", 
        new_value_x, new_value_y, new_value_z);

    return;
}


/*****************************************************************************
* FUNCTION
*  accelerometer_data_collect
* DESCRIPTION
*  
* PARAMETERS
* value_x [In]
* value_y [In]
* value_z [In]
* RETURNS
*  void
*****************************************************************************/
void accelerometer_data_collect(double value_x, 
                                 double value_y, 
                                 double value_z)
{
    if(!check_accelerometer_data_buffer_is_full()) {
        add_accelerometer_collect_data(value_x, value_y, value_z);
    }

	if(check_accelerometer_data_buffer_is_full()) {
        app_send_msg_internal(msg_data_collect_complete, ((int *)PROMPT_TIME));
    }
}


/*****************************************************************************
* FUNCTION
*  reset_accelerometer_data
* DESCRIPTION
*  
* PARAMETERS
*
* RETURNS
*  void
*****************************************************************************/
void reset_accelerometer_data()
{
    memset(&(g_nb_app_context.accelerometer_blocks), 0, sizeof(g_nb_app_context.accelerometer_blocks));
    g_nb_app_context.accelerometer_blocks.total_num = 1;
}


////////////////////////////////////// accelerometer part End /////////////////////////////////



////////////////////////////////////// state machine part ////////////////////////////////////


/*****************************************************************************
* FUNCTION
*  collect_data_complete
* DESCRIPTION
*  
* PARAMETERS
*
* RETURNS
*  void
*****************************************************************************/
void collect_data_complete(void)
{     
     // for test invoke data_collect here
     LWM2MAPPLOG("App collect_data_complete invoked");
     
     //barometer_data_collect((rand() % 1000) * 0.9);

     // test add sensor code, this should be added for sensor trigger
     /*
               accelerometer_data_collect((rand() % 1000) * 0.9,
                                 (rand() % 1000) * 0.7,
                                 (rand() % 1000) * 0.3);
         */

     // disable_accelerometer();
     
     // fomal code here
     LWM2MAPPLOG("collect_data_complete");
     app_send_msg_internal(msg_data_collect_complete, ((int *)NORMAL_TIME));
}
 

/*****************************************************************************
* FUNCTION
*  data_collect_rtos_timer_cb
* DESCRIPTION
*  
* PARAMETERS
*
* RETURNS
*  void
******************************************************************************/
static void data_collect_rtos_timer_cb(TimerHandle_t xTimer)
{
    LWM2MAPPLOG("data_collect_rtos_timer_cb invoke");
    
    xTimerStop(xTimer, 0);
    xTimerDelete(xTimer, 0);
    data_collect_rtos_timer = NULL;
    
    if(NULL != g_nb_app_context.data_result_cb) {
        g_nb_app_context.data_result_cb();
    }
}



/*****************************************************************************
* FUNCTION
*  data_collect_init_timer
* DESCRIPTION
*  
* PARAMETERS
*
* RETURNS
*  void
******************************************************************************/
void data_collect_init_timer(void)
{
    if(data_collect_rtos_timer == NULL) {
        data_collect_rtos_timer = xTimerCreate(
                                 "dc timer", 
                                 g_nb_app_context.data_collect_period / portTICK_PERIOD_MS,
                                 pdFALSE, 
                                 NULL, 
                                 data_collect_rtos_timer_cb);
        if(NULL == data_collect_rtos_timer) {           
         LWM2MAPPLOG("App data collect timer create failed");
        }
    }

    xTimerStop(data_collect_rtos_timer, 0);
    xTimerChangePeriod(data_collect_rtos_timer, g_nb_app_context.data_collect_period / portTICK_PERIOD_MS, 0);
    xTimerReset(data_collect_rtos_timer, 0);
}



/*****************************************************************************
* FUNCTION
*  query_data_result
* DESCRIPTION
*  
* PARAMETERS
*
* RETURNS
*  void
******************************************************************************/
void query_data_result(collect_data_complete_cb data_result_cb)
{
    LWM2MAPPLOG("query_data_result enter");
    g_nb_app_context.data_result_cb = data_result_cb;
    data_collect_init_timer();
    // enable_accelerometer();
    srand(xTaskGetTickCount());
    accelerometer_data_collect((rand() % 1000) * 0.9 + 0.234,
                                (rand() % 1000) * 0.7 + 0.345,
                                (rand() % 1000) * 0.3 + 0.456);
}


/*****************************************************************************
* FUNCTION
*  execute_data_collection
* DESCRIPTION
*  
* PARAMETERS
*
* RETURNS
*  void
******************************************************************************/
void execute_data_collection()
{
    if(APP_DATA_COLLECTING == g_nb_app_context.app_state) {        
        query_data_result(collect_data_complete);
    }
}


/*****************************************************************************
* FUNCTION
*  execute_data_reporting
* DESCRIPTION
*  
* PARAMETERS
*
* RETURNS
*  void
******************************************************************************/
void execute_data_reporting()
{
 if(APP_DATA_REPORTING == g_nb_app_context.app_state) {
    if(NULL != g_nb_app_context.lwm2m_info && g_nb_app_context.lwm2m_info->state == STATE_READY)
    {
        double x = FLOAT_ZERO;
        double y = FLOAT_ZERO;
        double z = FLOAT_ZERO;

        LWM2MAPPLOG("App execute_data_reporting");

        /*
            barometer_set_value(g_nb_app_context.lwm2m_info, get_collect_data());
            // clear data
            reset_barometer_data();
            */

        get_accelerometer_data(&x, &y, &z);

        accelerometer_set_value(g_nb_app_context.lwm2m_info,
                             x, y, z);

        reset_accelerometer_data();

        // for test    
        //LWM2MAPPLOG("App g_nb_app_context.app_state change to APP_DATA_COLLECTING");
        //g_nb_app_context.app_state = APP_DATA_COLLECTING;
        //app_send_msg_internal(msg_app_state_change, (void *)APP_DATA_COLLECTING);  
        g_nb_app_context.not_send_bearer_flag = false;
    }
    else {
        LWM2MAPPLOG("execute_data_reporting lwm2m context is NULL");
		g_nb_app_context.not_send_bearer_flag = true;
        //app_send_msg_internal(msg_data_collect_complete,(void *)(NORMAL_TIME));
    }
 }
 else {
	LWM2MAPPLOG("execute_data_reporting status is :%d", g_nb_app_context.app_state);
 }
}


/*****************************************************************************
* FUNCTION
*  execute_data_recving
* DESCRIPTION
*  
* PARAMETERS
*
* RETURNS
*  void
******************************************************************************/
void execute_data_recving()
{
 
}


/*****************************************************************************
* FUNCTION
*  app_state_machine_handler
* DESCRIPTION
*  
* PARAMETERS
*
* RETURNS
*  void
******************************************************************************/
void app_state_machine_handler()
{
    // lock sleep here.    
    lockSleep();   
    
    if(APP_IDLE == g_nb_app_context.app_state) {
        g_nb_app_context.app_state = APP_DATA_COLLECTING;
        LWM2MAPPLOG("change APP_IDLE to APP_DATA_COLLECTING");
    }
    if(APP_DATA_COLLECTING == g_nb_app_context.app_state){
        LWM2MAPPLOG("execute_data_collection enter");
        execute_data_collection();
    }else if(APP_DATA_REPORTING == g_nb_app_context.app_state) {
        execute_data_reporting();
        /* Reset sensor data retention */
        g_collect_data.used_number = 0;
        app_send_msg(msg_app_send_complete, (void *)1);
    }else if(APP_DATA_RECVING == g_nb_app_context.app_state) {
        execute_data_recving();
    }
}


/******************************************************************************
* FUNCTION
*  store_accelerometer_data
* DESCRIPTION
*  store_accelerometer_data
* PARAMETERS
*  message         [in]
* RETURNS
*  void
*****************************************************************************/
void store_accelerometer_data()
{
#if __NB_APP_ZERO__
    nvdm_status_t ret = NVDM_STATUS_OK;
    int real_total_len = 0;
    char accelerometer_value[total_acceleromter_len] = {0};
    char *p_acc_data = accelerometer_value;
    char *temp_work_acc_data = p_acc_data;
    int test_index = 0;
    accelerometer_sensor_data *tmp_acc_data = &(g_nb_app_context.accelerometer_blocks.sensor_accelerometer[0]);    
    real_total_len = convert_double_to_chars(accelerometer_value, 
                                               tmp_acc_data->value_x, 2);

    LWM2MAPPLOG("store_accelerometer_data x: %s", temp_work_acc_data);
    temp_work_acc_data += real_total_len;
    real_total_len = convert_double_to_chars(temp_work_acc_data, 
                                               tmp_acc_data->value_y, 2);

    LWM2MAPPLOG("store_accelerometer_data y: %s", temp_work_acc_data);
    temp_work_acc_data += real_total_len;
    real_total_len = convert_double_to_chars(temp_work_acc_data, 
                                               tmp_acc_data->value_z, 2);

    LWM2MAPPLOG("store_accelerometer_data z: %s", temp_work_acc_data);

    
    // store sensor data
    /*
       ret = nvdm_write_data_item("NB_SENSOR_APP", "Accelerometer_Data", 
                               NVDM_DATA_ITEM_TYPE_STRING, 
                               p_acc_data, 
                               real_total_len);
                               */
    memset(g_collect_data.accelerometer_value, 0, sizeof(char)* total_acceleromter_len);
	memcpy(g_collect_data.accelerometer_value, p_acc_data, real_total_len);

    if(NVDM_STATUS_OK == ret)
    {
        LWM2MAPPLOG("store_accelerometer_data nvdm_write_data_item success");
    }else {
        LWM2MAPPLOG("store_accelerometer_data nvdm_write_data_item failured");
        return;
    }

    // test
    // reset_accelerometer_data();
    // recover_accelerometer_data();
#endif
    int32_t i = 0;

    for (i = 0; i < g_nb_app_context.accelerometer_blocks.current_use_num; ++i) {
        g_collect_data.sensor_accelerometer[i].value_x = g_nb_app_context.accelerometer_blocks.sensor_accelerometer[i].value_x;
        g_collect_data.sensor_accelerometer[i].value_y = g_nb_app_context.accelerometer_blocks.sensor_accelerometer[i].value_y;
        g_collect_data.sensor_accelerometer[i].value_z = g_nb_app_context.accelerometer_blocks.sensor_accelerometer[i].value_z;
    }

    g_collect_data.used_number = i;

    LWM2MAPPLOG("store_accelerometer_data--used: %d", g_collect_data.used_number);
}


#if __NB_APP_ZERO__
/******************************************************************************
* FUNCTION
*  recover_accelerometer_data
* DESCRIPTION
*  recover_accelerometer_data
* PARAMETERS
*  message         [in]
* RETURNS
*  void
*****************************************************************************/
void recover_accelerometer_data()
{
    nvdm_status_t ret = NVDM_STATUS_OK;
    int real_total_len = total_acceleromter_len, len = 0;
    char accelerometer_value[total_acceleromter_len] = {0};
    char *p_acc_data = accelerometer_value;
    accelerometer_sensor_data *tmp_acc_data = &(g_nb_app_context.accelerometer_blocks.sensor_accelerometer[0]);    
    // for log
    char new_value_x[ACCELEROMETER_TOTAL_LEN_UNITS] = {0};
    char new_value_y[ACCELEROMETER_TOTAL_LEN_UNITS] = {0};
    char new_value_z[ACCELEROMETER_TOTAL_LEN_UNITS] = {0};
 
    // store sensor data
    /*
      ret = nvdm_read_data_item("NB_SENSOR_APP", "Accelerometer_Data", 
                               p_acc_data, 
                               &real_total_len);
                               */
                              
    p_acc_data = g_collect_data.accelerometer_value;
                               

    if(NVDM_STATUS_OK == ret)
    {
        LWM2MAPPLOG("recover_accelerometer_data nvdm_read_data_item success");
    }else {
        LWM2MAPPLOG("recover_accelerometer_data nvdm_read_data_item failured");
        return;
    }

    convert_chars_to_double(p_acc_data, &len, &(tmp_acc_data->value_x));
    p_acc_data = p_acc_data + len + 1;
    convert_chars_to_double(p_acc_data, &len, &(tmp_acc_data->value_y));
    p_acc_data = p_acc_data + len + 1;
    convert_chars_to_double(p_acc_data, &len, &(tmp_acc_data->value_z));

    //for log
    object_ftoa(tmp_acc_data->value_x, new_value_x, 2);
    object_ftoa(tmp_acc_data->value_y, new_value_y, 2);  
    object_ftoa(tmp_acc_data->value_z, new_value_z, 2); 
    
    LWM2MAPPLOG("recover_accelerometer_data nvdm_write_data_item total_len: %d, ret: %d, %s, %s, %s",
                 real_total_len, ret,
                 new_value_x,
                 new_value_y,
                 new_value_z);
}
#endif


/******************************************************************************
* FUNCTION
*  store_app_status
* DESCRIPTION
*  store_app_status
* PARAMETERS
*  message         [in]
* RETURNS
*  void
*****************************************************************************/
void store_app_status()
{
    //nvdm_status_t ret = NVDM_STATUS_OK;
    // store app status
    /*
      ret = nvdm_write_data_item("NB_SENSOR_APP", "Status", 
                               NVDM_DATA_ITEM_TYPE_RAW_DATA, 
                               (uint32_t *)(g_nb_app_context.app_state), 
                               sizeof(uint32_t)); 
                               */

    g_collect_data.app_state = g_nb_app_context.app_state;

    LWM2MAPPLOG("store_app_status--state: %d", g_collect_data.app_state);

    //test
    //recover_app_status();
}


/******************************************************************************
* FUNCTION
*  recover_app_status
* DESCRIPTION
*  recover_app_status
* PARAMETERS
*  message         [in]
* RETURNS
*  void
*****************************************************************************/
void recover_app_status()
{
    nvdm_status_t ret = NVDM_STATUS_OK;
    int len = 4, status = 0;
	
    // store app status
    //ret = nvdm_read_data_item("NB_SENSOR_APP", "Status", (uint8_t *)(status), &len); 
    g_nb_app_context.app_state = g_collect_data.app_state;
	
    if(NVDM_STATUS_OK == ret)
    {
        LWM2MAPPLOG("recover_app_status nvdm_read_data_item buffer: %d, len: %d",status, len);
    }else {
        LWM2MAPPLOG("recover_app_status nvdm_read_data_item read fail :%d", ret);
    }
    g_nb_app_context.app_state = status;
}



/******************************************************************************
* FUNCTION
*  app_data_collect_msg_handler
* DESCRIPTION
*  app_data_collect_msg_handler
* PARAMETERS
*  message         [in]
* RETURNS
*  void
*****************************************************************************/
void app_data_collect_msg_handler(nb_app_message_struct msg)
{

    LWM2MAPPLOG("data_collect_msg_hdr--msg_id: %d", msg.message_id);

	switch (msg.message_id) {
		case msg_data_collect_complete:
			LWM2MAPPLOG("App msg_data_collect_complete enter: %d", msg.param);
			// cancel timer and invoke data collect callback
			if(NULL != data_collect_rtos_timer) {
			LWM2MAPPLOG("App data_collect_rtos_timer_cb enter");
			//data_collect_rtos_timer_cb(data_collect_rtos_timer);
			xTimerStop(data_collect_rtos_timer, 0);
			xTimerDelete(data_collect_rtos_timer, 0);
			data_collect_rtos_timer = NULL;
			}
			//check data is empty or not
			if(false == check_accelerometer_data_buffer_is_empty()) {
			LWM2MAPPLOG("sensor has data to process");
			// data retention
			store_app_status();
			store_accelerometer_data();
			// change to reporting status
			app_send_msg_internal(msg_app_state_change, 
			                   (void *)(APP_DATA_REPORTING));
			}else {
			// data retention
			store_app_status();
			// change to collecting status again
			app_send_msg_internal(msg_app_state_change, 
			                   (void *)(APP_IDLE));
			}
			unlockSleep();
		break;
		case msg_app_state_change: {
			int state = (int)(msg.param);
			g_nb_app_context.app_state = state;
			LWM2MAPPLOG("App msg_app_state_change enter: %d", state);
			app_state_machine_handler();
			}
		break;
		case msg_app_send_complete: {    
			g_nb_app_context.app_state = APP_IDLE;
			LWM2MAPPLOG("msg_app_send_complete: %d", msg.param);

			store_app_status();                
			unlockSleep();
			//in test, we invoke app_state_machine_handler() here, but 
			//in practice, need to invoke it by rtc timer invoke.
			//app_state_machine_handler();                     
			}
		break;

        case LWM2M_MSG_ID_UPDATE_REGISTER:
        {
            if (g_nb_app_context.lwm2m_info && g_nb_app_context.lwm2m_info->state == STATE_READY) {
                int32_t ret = 0;
                ret = lwm2m_update_registration(g_nb_app_context.lwm2m_info, LWM2M_SENSOR_SRV_ID, false);
                LWM2MAPPLOG("data_collect_msg_hdr(update reg)--ret: %d", ret);
            } else {
                LWM2MAPPLOG("data_collect_msg_hdr(update reg fail)--state: %d", (g_nb_app_context.lwm2m_info) ? g_nb_app_context.lwm2m_info->state : 1200);
            }
            break;
        }

        case LWM2M_MSG_ID_LWM2M_INIT: {
            lwm2m_client_init();
        }

        default:
        break;
    }
}


////////////////////// data colltect ////////////////////////////////////////////////////


/******************************************************************************
* FUNCTION
*  nb_app_data_collect_main_loop
* DESCRIPTION
*  nb_app_data_collect_main_loop
* PARAMETERS
*  void *arg
* RETURNS
*  void
*******************************************************************************/
void nb_app_data_collect_main_loop(void *arg)
{
    nb_app_message_struct queue_item;

    LWM2MAPPLOG("nb_app_data_collect_main_loop enter");

    app_state_machine_handler();
    //accelerometer_data_collect(1.23, 3.45, 98.99);

    while (1) {
        if (xQueueReceive(nb_data_control_queue, &queue_item, portMAX_DELAY)) {
            app_data_collect_msg_handler(queue_item);
        }
    }
}
 

void nb_data_collect_prepare()
{
     g_nb_app_context.data_collect_period = DATA_COLLECT_PERIOD;
     g_nb_app_context.app_state = APP_IDLE;
     reset_barometer_data();
     reset_accelerometer_data();
}

 
 /******************************************************************************
  * FUNCTION
  *  nb_data_collect_init
  * DESCRIPTION
  *  nb_data_collect_init
  * PARAMETERS
  *  void
  * RETURNS
  *  void
  *******************************************************************************/
 void nb_data_collect_enter()
 {
     TaskHandle_t task_handle = NULL;    
 
     LWM2MAPPLOG("nb_data_collect_init enter\r\n");
 
     // create task queue
     nb_data_control_queue = xQueueCreate(APP_DATA_COLLECT_TASK_QUEUE_SIZE,
                                            sizeof(nb_app_message_struct) );
 
     if (pdPASS != xTaskCreate(nb_app_data_collect_main_loop,
                               APP_DATA_COLLECT_TASK_NAME,
                               APP_DATA_COLLECT_TASK_STACK_SIZE / sizeof(portSTACK_TYPE),
                               NULL,
                               APP_DATA_COLLECT_TASK_PRIORITY,
                               &task_handle)) {
         LWM2MAPPLOG("App nb_data_collect_init failured\r\n");
         return;
     }

     nb_data_ctrl_task = task_handle;
 }

////////////////////// data colltect End ////////////////////////////////////////////////////


////////////////////// lwm2m client ////////////////////////////////////////////////////

extern int lwm2m_client_main_loop(void * args);


void lwm2m_app_mutex_take(void)
{
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
        if (xSemaphoreTake(g_nb_app_context.bearer_info.app_bearer_semaphore, portMAX_DELAY) == pdFALSE) {
             LWM2MAPPLOG("lwm2m_app_mutex_take error\r\n");
        }        
    }
}

void lwm2m_app_mutex_give(void)
{
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
        if (xSemaphoreGive(g_nb_app_context.bearer_info.app_bearer_semaphore) == pdFALSE) {
            LWM2MAPPLOG("lwm2m_app_mutex_give error\r\n");
        }
   }
}


void lwm2m_bearer_active()
{
    tel_conn_mgr_pdp_type_enum pdp_type = TEL_CONN_MGR_PDP_TYPE_IPV4V6;
    tel_conn_mgr_pdp_type_enum activated_pdp_type = TEL_CONN_MGR_PDP_TYPE_NONE;
    tel_conn_mgr_ret_enum ret = TEL_CONN_MGR_RET_OK;
    //tel_conn_mgr_apn_struct* apn_content = tel_conn_mgr_get_apn_content();

    LWM2MAPPLOG("[lwm2m_bearer_active]");

#if 0
    if (apn_content == NULL){
        //configASSERT(0);
        LWM2MAPPLOG("[lwm2m_bearer_active]--empty apn");
        return false;
    }
#endif

    ret = tel_conn_mgr_activate(TEL_CONN_MGR_BEARER_TYPE_NBIOT,
        TEL_CONN_MGR_SIM_ID_1,
        pdp_type,
        "", //apn_content->apn,
        "", //apn_content->user_name,
        "", //apn_content->password,
        nb_bearer_configure_queue,
        &(g_nb_app_context.bearer_info.app_simat_info.app_id),
        &activated_pdp_type);

#if 0
    ret = tel_conn_mgr_activate(TEL_CONN_MGR_BEARER_TYPE_NBIOT,
                                TEL_CONN_MGR_SIM_ID_1,
                                pdp_type,
                                bearer_info->apn_val,
                                bearer_info->user_val,
                                bearer_info->passwd_val,
                                nb_bearer_configure_queue,
                                &(g_nb_app_context.bearer_info.app_simat_info.app_id),
                                &activated_pdp_type);
#endif

    if (ret == TEL_CONN_MGR_RET_OK) {
        g_nb_app_context.bearer_info.app_bearer_valid_flag = true;
        g_nb_app_context.bearer_try_time = 0;
        lwm2m_client_init();
    }
    LWM2MAPPLOG("[lwm2m_bearer_active]tel_conn_mgr_activate ret: %d app_id: %d", 
                   ret, g_nb_app_context.bearer_info.app_simat_info.app_id);
}


void lwm2m_client_init()
{
    TaskHandle_t task_handle = NULL;
    
#ifdef MTK_LWM2M_CT_SUPPORT
    if (g_nb_app_context.activated_from_command == false || g_nb_app_context.bearer_info.app_bearer_valid_flag == false) {
        return;
    }
#endif

    if (pdPASS != xTaskCreate(lwm2m_client_main_loop,
                               APP_LWM2M_CLIENT_TASK_NAME,
                               APP_LWM2M_CLIENT_TASK_STACK_SIZE / sizeof(portSTACK_TYPE),
                               NULL,
                               APP_LWM2M_CLIENT_TASK_PRIORITY,
                               &task_handle)) {
        LWM2MAPPLOG("App lwm2m_client_init failured\r\n");
        LWM2MAPPPRINT("App lwm2m_client_init failured");
        return;
    }
}


////////////////////// lwm2m client End ////////////////////////////////////////////////////


////////////////////////// //////// bearer configure task /////////////////////////////////////////////

 
/******************************************************************************
* FUNCTION
*  app_bearer_msg_handler
* DESCRIPTION
*  app_bearer_msg_handler
* PARAMETERS
*  message         [in]
* RETURNS
*  void
*****************************************************************************/
void app_bearer_msg_handler(nb_app_message_struct *msg)
{
    if(NULL != msg)
    {
    	LWM2MAPPLOG("msg->message_id: 0x%08x", msg->message_id);
		
        switch (msg->message_id) {
			case MSG_ID_URC_REG_START: {
				LWM2MAPPLOG("app_bearer_msg_handler MSG_ID_URC_REG_START");
				lwm2m_bearer_active();
				}
				break;
            case LWM2M_MSG_ID_CLIENT_RESTART: {
                LWM2MAPPLOG("[app_bearer_msg_handler]client_restart");
                lwm2m_bearer_active();
                break;
            }
            case LWM2M_MSG_ID_CEREG_ACTIVE: {
                LWM2MAPPLOG("[app_bearer_msg_handler]cereg active");
                lwm2m_bearer_active();
                break;
            }
            case MSG_ID_TEL_CONN_MGR_ACTIVATION_RSP: {
                tel_conn_mgr_activation_rsp_struct *bearer_active_msg = (tel_conn_mgr_activation_rsp_struct *)msg;
                //unsigned int index = 0;
                LWM2MAPPLOG("bearer active: %x, result: %d, app_id: %d, cause: %d",
                               bearer_active_msg,
                               bearer_active_msg->result,
                               bearer_active_msg->app_id,
                               bearer_active_msg->cause);
                if (1 == bearer_active_msg->result) {
                    LWM2MAPPLOG("bearer active success app_id: %d, try_time: %d",
                                g_nb_app_context.bearer_info.app_simat_info.app_id,
                                g_nb_app_context.bearer_try_time);
                    g_nb_app_context.bearer_info.app_bearer_valid_flag = true;
                    g_nb_app_context.bearer_try_time = 0;
                    // give mutex let lwm2m_client task can run
                    //lwm2m_app_mutex_give();

                    // create lwm2m task and do upload data to server
                    lwm2m_client_init();

                } else {
                    ++g_nb_app_context.bearer_try_time;
                    if (g_nb_app_context.bearer_try_time <= LWM2M_BEARER_TRY_MAX_TIME) {
                    // notify bearer established failed
                    LWM2MAPPLOG("bearer active failed app_id: %d, try_time: %d", 
                                g_nb_app_context.bearer_info.app_simat_info.app_id,
                                g_nb_app_context.bearer_try_time);
                    nb_app_notify_lwm2m_client_restart();
                    } else {
                        LWM2M_RESET_FLAG(g_nb_lwm2m_flag, LWM2M_FLAG_CEREG_ACTVIE);
                        LWM2MAPPLOG("(final)bearer active failed app_id: %d, try_time: %d", 
                                    g_nb_app_context.bearer_info.app_simat_info.app_id,
                                    g_nb_app_context.bearer_try_time);
                    }
                }
            }
            break;
            case MSG_ID_TEL_CONN_MGR_DEACTIVATION_RSP: {
                tel_conn_mgr_deactivation_rsp_struct *bearer_deactive_msg = (tel_conn_mgr_deactivation_rsp_struct *)msg;
                if (1 == bearer_deactive_msg->result) {
                    g_nb_app_context.bearer_info.app_bearer_valid_flag = false;
                    LWM2MAPPLOG("bearer deactive success app_id: %d\r\n", 
                                g_nb_app_context.bearer_info.app_simat_info.app_id);
                } else {
                    LWM2MAPPLOG("bearer deactive failed app_id: %d\r\n", 
                                g_nb_app_context.bearer_info.app_simat_info.app_id);
                }
            }
            break;
            case MSG_ID_TEL_CONN_MGR_DEACTIVATION_IND: {
                //tel_conn_mgr_deactivation_ind_struct *bearer_deactive_ind = (tel_conn_mgr_deactivation_ind_struct *)(msg->param);
                g_nb_app_context.bearer_info.app_bearer_valid_flag = false;
                LWM2M_RESET_FLAG(g_nb_lwm2m_flag, LWM2M_FLAG_CLIENT_RESTART);
                LWM2M_RESET_FLAG(g_nb_lwm2m_flag, LWM2M_FLAG_CEREG_ACTVIE);
                lwm2m_client_quit("tel deactive", 1);
                LWM2MAPPLOG("MSG_ID_TEL_CONN_MGR_DEACTIVATION_IND\r\n");
            }
            break;			
            default:
                LWM2MAPPLOG("app_bearer_msg_handler id: %id wrong\r\n", msg->message_id);
                break;
        }
        vPortFree(msg);
    } else {
        LWM2MAPPLOG("app_bearer_msg_handler msg null\r\n");
    }
}


/******************************************************************************
* FUNCTION
*  nb_app_bear_configure_main_loop
* DESCRIPTION
*  nb_app_bear_configure_main_loop
* PARAMETERS
*  void *arg
* RETURNS
*  void
*******************************************************************************/
void nb_app_bear_configure_main_loop(void *arg)
{
    nb_app_message_struct *queue_item = NULL;

    LWM2MAPPLOG("nb_app_bear_configure_main_loop enter");

    while (1) {
        if (xQueueReceive(nb_bearer_configure_queue, &queue_item, portMAX_DELAY)) {
            app_bearer_msg_handler(queue_item);
        }
    }
}


/******************************************************************************
* FUNCTION
*  lwm2m_client_bearer_init
* DESCRIPTION
*  lwm2m_client_bearer_init
* PARAMETERS
*  
* RETURNS
*  void
*******************************************************************************/
void lwm2m_client_bearer_init()
{
    simat_bearer_info *temp_bearer_info = &(g_nb_app_context.bearer_info.app_simat_info);
    TaskHandle_t task_handle = NULL;


    g_nb_app_context.bearer_info.app_bearer_semaphore = xSemaphoreCreateBinary();
    if(NULL == g_nb_app_context.bearer_info.app_bearer_semaphore)
    {
        LWM2MAPPLOG("semaphore create failured");
    }
    
    strncpy((char *)(temp_bearer_info->apn_val), (const char *)APN_NAME, strlen(APN_NAME) + 1);
    strncpy((char *)(temp_bearer_info->user_val), (const char *)USER_NAME, strlen(USER_NAME) + 1);
    strncpy((char *)(temp_bearer_info->passwd_val), (const char *)PASSWORD, strlen(PASSWORD) + 1);


    LWM2MAPPLOG("lwm2m_client_bearer_init \r\n");

    // create task queue
    nb_bearer_configure_queue = xQueueCreate(APP_BEARER_CONFIGURE_QUEUE_SIZE,
                                        sizeof(nb_app_message_struct *)); 

    if (pdPASS != xTaskCreate(nb_app_bear_configure_main_loop,
                              APP_BEARER_CONFIGURE_TASK_NAME,
                              APP_BEARER_CONFIGURE_TASK_STACK_SIZE / sizeof(portSTACK_TYPE),
                              NULL,
                              APP_BEARER_CONFIGURE_TASK_PRIORITY,
                              &task_handle)) {
     LWM2MAPPLOG("App lwm2m_client_bearer_init failured\r\n");
     return;
    }

    nb_bearer_cfg_task = task_handle;
}

////////////////////////////// bearer configure task End /////////////////////////////////////


/////////////////////////////////// data retention implement ///////////////////////////////


/******************************************************************************
* FUNCTION
*  save_security_object
* DESCRIPTION
*  save_security_object
* PARAMETERS
*  
* RETURNS
*  void
*******************************************************************************/
void save_security_object(char *server_uri, int server_id)
{
    //sprintf(g_collect_data.server_uri, "%s", server_uri);
    g_collect_data.server_id = server_id;   
}


/******************************************************************************
* FUNCTION
*  save_server_object
* DESCRIPTION
*  save_server_object
* PARAMETERS
*  
* RETURNS
*  void
*******************************************************************************/
void save_server_object(char *bind_type, int life_time)
{
    g_collect_data.life_time = life_time;
    //memcpy(g_collect_data.bind_type, bind_type, MAX_BIND_TYUPE_LEN);
}


/******************************************************************************
* FUNCTION
*  save_client_name
* DESCRIPTION
*  save_client_name
* PARAMETERS
*  
* RETURNS
*  void
*******************************************************************************/
void save_client_name(char *client_name)
{
    //memcpy(g_collect_data.deice_name, client_name, strlen(client_name));
}


#if __NB_APP_ZERO__
/******************************************************************************
* FUNCTION
*  nb_app_data_recover
* DESCRIPTION
*  nb_app_data_recover
* PARAMETERS
*  
* RETURNS
*  void
*******************************************************************************/
void nb_app_data_recover()
{
    // recover g_nb_app_context.app_state first
    g_nb_app_context.app_state = APP_IDLE;
    if(APP_DATA_COLLECTING == g_nb_app_context.app_state)
    {
        recover_accelerometer_data();
    }

    // update new status for next app action
    if(APP_IDLE == g_nb_app_context.app_state)
    {           
        g_nb_app_context.app_state = APP_DATA_COLLECTING;
    }else if(APP_DATA_COLLECTING == g_nb_app_context.app_state)
    {
       g_nb_app_context.app_state = APP_DATA_REPORTING;
    }else if(APP_DATA_REPORTING == g_nb_app_context.app_state)
    {
       g_nb_app_context.app_state = APP_IDLE;
    }else{
        LWM2MAPPLOG("wrong app original state: %d", g_nb_app_context.app_state);
    }
}
#endif


/******************************************************************************
* FUNCTION
*  rtcTimerCallbackFun
* DESCRIPTION
*  rtcTimerCallbackFun
* PARAMETERS
*  
* RETURNS
*  void
*******************************************************************************/
void rtcTimerCallbackFun(void *param)
{
    if (rtc_power_on_result_external() == DEEP_SLEEP || rtc_power_on_result_external() == DEEPER_SLEEP)
    {
        //parameter = DEEP_SLEEP_MODE;
        g_nb_app_context.is_deep_sleep = true;
        LWM2MAPPLOG("DEEP_SLEEP_MODE");
    } else {
        g_nb_app_context.is_deep_sleep = false;
        LWM2MAPPLOG("not DEEP_SLEEP_MODE");
    }

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xTimerStartFromISR(g_lwm2m_client_ping_timer, &xHigherPriorityTaskWoken);
    hal_sleep_manager_acquire_sleeplock(g_lwm2m_client_sleep_handle, HAL_SLEEP_LOCK_DEEP);
    g_lwm2m_client_is_locking = true;

    app_send_msg_from_isr(LWM2M_MSG_ID_UPDATE_REGISTER, NULL);
}


/******************************************************************************
* FUNCTION
*  data_retention_prepare
* DESCRIPTION
*  data_retention_prepare
* PARAMETERS
*  
* RETURNS
*  void
*******************************************************************************/
void data_retention_prepare()
{
    int timer_period = NOTIFICATION_UPDATE_TIME;
    g_nb_app_context.force_upload_period = timer_period;
    int check_timer_val = ( 2 * COAP_MAX_TRANSMIT_WAIT ) * 10; //100ms unit
    nvdm_status_t ret = NVDM_STATUS_OK;
    
    ret = nvdm_init();
    if(NVDM_STATUS_OK != ret)
    {
        LWM2MAPPLOG("NVRAM init failed--ret: %d", ret);
    }

    // check force_upload_period greate than data_collect_period
    if(timer_period < check_timer_val)
    {
        g_nb_app_context.force_upload_period = check_timer_val;       
    }

	LWM2MAPPLOG("data_retention_prepare timer_period current: %d, reconfigure: %d\r\n", 
                timer_period,
                g_nb_app_context.force_upload_period);

    // create and start rtc timer
    ret = rtc_sw_timer_create(&app_rtc_handle, timer_period, true, rtcTimerCallbackFun);
	LWM2MAPPLOG("rtc_sw_timer_start ret: %d\r\n", ret);
	if(0 == ret){
        LWM2MAPPLOG("rtc_sw_timer_create success\r\n");
        // start rtc timer
        //ret = rtc_sw_timer_start(app_rtc_handle);
    }else{
        LWM2MAPPLOG("rtc_sw_timer_create failure\r\n");
    }
}


static int32_t nb_app_ril_cmd_rsp_callback(ril_cmd_response_t *response)
{
    LWM2MAPPLOG("ril_cmd_rsp--id: %d, mode: %d, code: %d", response->cmd_id, response->mode, response->res_code);
    if (RIL_RESULT_CODE_OK == response->res_code) {
        ril_eps_network_registration_status_rsp_t *nw_reg_status = (ril_eps_network_registration_status_rsp_t *)(response->cmd_param);
        LWM2MAPPLOG("cmd_rsp--stat: %d, act: %d, flag: 0x%08x\n", nw_reg_status->stat, nw_reg_status->act, g_nb_lwm2m_flag);
        if (TEL_CONN_MGR_NW_REG_STAT_REGED_HMNW == nw_reg_status->stat ||
            TEL_CONN_MGR_NW_REG_STAT_REGED_ROAMING == nw_reg_status->stat ||
            TEL_CONN_MGR_NW_REG_STAT_SMS_ONLY_HMNW == nw_reg_status->stat ||
            TEL_CONN_MGR_NW_REG_STAT_SMS_ONLY_ROAMING == nw_reg_status->stat) {
            nb_app_message_struct *urc_msg = NULL;
            LWM2M_SET_FLAG(g_nb_lwm2m_flag, LWM2M_FLAG_CEREG_ACTVIE);
            LWM2M_SET_FLAG(g_nb_lwm2m_flag, LWM2M_FLAG_CFUN_READY);
            urc_msg = pvPortMalloc(sizeof(nb_app_message_struct));
            urc_msg->message_id = LWM2M_MSG_ID_CEREG_ACTIVE;
            if (pdTRUE != xQueueSend(nb_bearer_configure_queue, &urc_msg, 10)) {
                LWM2MAPPLOG("ril_cmd_rsp active send msg fail!\n");
            }
        }
    } else {
        LWM2MAPPLOG("ril_cmd_rsp fail");
    }

    return 0;
}


///////////////////////////////////data recover implement End //////////////////////////


//TimerHandle_t test_timer;
static int32_t sensor_app_ril_urc_callback(ril_urc_id_t event_id, void *param, uint32_t param_len)
{    
    nb_app_message_struct *urc_msg = NULL;
    LWM2MAPPLOG("ril_urc_callback:%d!\n", event_id);

    switch(event_id)
    {
       case RIL_URC_ID_CPIN: {
           ril_enter_pin_urc_t * pin = (ril_enter_pin_urc_t*)param;
           LWM2MAPPLOG("app pin->code: %s, flag: 0x%08x\n",pin->code, g_nb_lwm2m_flag);
           if ((strcmp(pin->code, "READY") == 0) && (!(g_nb_lwm2m_flag & LWM2M_FLAG_CFUN_READY))) {
                //send start to task
                LWM2M_SET_FLAG(g_nb_lwm2m_flag, LWM2M_FLAG_CFUN_READY);
                LWM2MAPPLOG("app CPIN ready!\n");

               if (g_nb_lwm2m_flag & LWM2M_FLAG_CLIENT_RUNNING) {
                   LWM2M_SET_FLAG(g_nb_lwm2m_flag, LWM2M_FLAG_CLIENT_RESTART);
                   LWM2MAPPLOG("[ril_urc]later restart lwm2mc");
               } else {
                    int32_t ret = 0;
                    ret = ril_request_eps_network_registration_status(RIL_EXECUTE_MODE,
                                                                     2,
                                                                     NULL,
                                                                     NULL);
                    LWM2MAPPLOG("[ril_urc]cpin ready--ret: %d", ret);
                    #if 0
                    urc_msg = pvPortMalloc(sizeof(nb_app_message_struct));
                    urc_msg->message_id = MSG_ID_URC_REG_START;
                            
                    if (pdTRUE != xQueueSend(nb_bearer_configure_queue, &urc_msg, 10)) {
                       LWM2MAPPLOG("sensor_app_ril_urc_callback send msg fail!\n");
                    }
                    #endif
               }
           } else if ((strcmp(pin->code, "NOT READY") == 0) && (g_nb_lwm2m_flag & LWM2M_FLAG_CFUN_READY)) {
                LWM2M_RESET_FLAG(g_nb_lwm2m_flag, LWM2M_FLAG_CFUN_READY);
                LWM2M_RESET_FLAG(g_nb_lwm2m_flag, LWM2M_FLAG_CLIENT_RESTART);
                LWM2M_RESET_FLAG(g_nb_lwm2m_flag, LWM2M_FLAG_CEREG_ACTVIE);
                lwm2m_client_quit("CPIN not ready", 1);
           }
           break;
       }

       case RIL_URC_ID_CEREG: {
            ril_eps_network_registration_status_urc_t *nw_reg_status = (ril_eps_network_registration_status_urc_t *)param;
            LWM2MAPPLOG("CEREG--stat: %d, act: %d, flag: 0x%08x\n", nw_reg_status->stat, nw_reg_status->act, g_nb_lwm2m_flag);

            if (nb_app_is_nw_registered(nw_reg_status)) {
                if (!(g_nb_lwm2m_flag & LWM2M_FLAG_CEREG_ACTVIE)) {
                    LWM2M_SET_FLAG(g_nb_lwm2m_flag, LWM2M_FLAG_CEREG_ACTVIE);
                    LWM2M_SET_FLAG(g_nb_lwm2m_flag, LWM2M_FLAG_CFUN_READY);
                    urc_msg = pvPortMalloc(sizeof(nb_app_message_struct));
                    urc_msg->message_id = LWM2M_MSG_ID_CEREG_ACTIVE;
                    if (pdTRUE != xQueueSend(nb_bearer_configure_queue, &urc_msg, 10)) {
                       LWM2MAPPLOG("cereg active send msg fail!\n");
                    }
                }
            }
            break;
       }

       case RIL_URC_ID_MATWAKEUP: {
            int32_t ret = 0;
            ret = ril_request_eps_network_registration_status(RIL_READ_MODE,
                                                            RIL_OMITTED_INTEGER_PARAM,
                                                            nb_app_ril_cmd_rsp_callback,
                                                            NULL);
           LWM2MAPPLOG("[ril_urc]wake_up--ret: %d", ret);
           break;
       }

       default:
           break;

    }    
   return 0;
}


/******************************************************************************
* FUNCTION
*  nb_app_enter_internal
* DESCRIPTION
*  nb_app_enter_internal
* PARAMETERS
*  
* RETURNS
*  void
*******************************************************************************/
int nb_app_enter_internal()
{
    ril_status_t ret = RIL_STATUS_SUCCESS;

    // prevent app to enter deep sleep, the code need to remove in official version
    //lockSleep();

    // RTC callback do
    //data_retention_prepare();

    // data collecting and state machine task
    nb_data_collect_prepare();
    nb_data_collect_enter(); 

    // bearer connection manager task
    lwm2m_client_bearer_init();

#if 0
    ret = ril_register_event_callback(RIL_GROUP_MASK_ALL, sensor_app_ril_urc_callback);
    if (ret != RIL_STATUS_SUCCESS){
         LWM2MAPPLOG("register URC into RIL fail!");

    }
#else
    g_nb_app_context.bearer_info.app_bearer_valid_flag = true;
    g_nb_app_context.bearer_try_time = 0;
#endif

    g_nb_app_context.cli_port = LWM2M_CLI_PORT;

    return 1;
}


void nb_app_wakeup_handle(void)
{
    int32_t i = 0;
    ril_status_t ret = RIL_STATUS_SUCCESS;

    LWM2MAPPPRINT("wakeup_handle");

    memset(&g_nb_app_context, 0x00, sizeof(nb_app_context));
    g_nb_app_context.is_deep_sleep = true;

    /* Upload period */
    g_nb_app_context.force_upload_period = (NOTIFICATION_UPDATE_TIME) > ((2 * COAP_MAX_TRANSMIT_WAIT) * 10) ? (NOTIFICATION_UPDATE_TIME) : ((2 * COAP_MAX_TRANSMIT_WAIT) * 10);

    /* Sensor collect data duration */
    g_nb_app_context.data_collect_period = DATA_COLLECT_PERIOD;

    /* Data retention */
    g_nb_app_context.app_state = g_collect_data.app_state;
    g_nb_app_context.accelerometer_blocks.total_num = 1;

    for (i = 0; i < g_collect_data.used_number; ++i) {
        g_nb_app_context.accelerometer_blocks.sensor_accelerometer[i].value_x = g_collect_data.sensor_accelerometer[i].value_x;
        g_nb_app_context.accelerometer_blocks.sensor_accelerometer[i].value_y = g_collect_data.sensor_accelerometer[i].value_y;
        g_nb_app_context.accelerometer_blocks.sensor_accelerometer[i].value_z = g_collect_data.sensor_accelerometer[i].value_z;
    }
    g_nb_app_context.accelerometer_blocks.current_use_num = g_collect_data.used_number;

    /* Create data collect task */
    nb_data_collect_enter(); 

    /* Create bearer connection task */
    lwm2m_client_bearer_init();
    
#if 0
    /* Register ril handler */
    ret = ril_register_event_callback(RIL_GROUP_MASK_ALL, sensor_app_ril_urc_callback);
    if (ret != RIL_STATUS_SUCCESS){
        LWM2MAPPLOG("register URC into RIL fail!");
    }
#else
    g_nb_app_context.bearer_info.app_bearer_valid_flag = true;
    g_nb_app_context.bearer_try_time = 0;
#endif

    g_nb_app_context.cli_port = LWM2M_CLI_PORT;
    g_nb_app_context.lifetime = g_collect_data.life_time;
}


void lwm2m_client_notify_callback(lwm2m_notify_type_t type, lwm2m_notify_code_t code, uint32_t option_info)
{
    LWM2MAPPLOG("lwm2m_client_notify_callback type = %d, code = %d, option_info = %d", (int)type, (int)code, (int)option_info);

    if (type == LWM2M_NOTIFY_TYPE_PING && code == LWM2M_NOTIFY_CODE_SUCCESS && g_lwm2m_client_is_locking) {
        xTimerStop(g_lwm2m_client_ping_timer, 0);
        hal_sleep_manager_release_sleeplock(g_lwm2m_client_sleep_handle, HAL_SLEEP_LOCK_DEEP);
        LWM2MAPPLOG("hal_sleep_manager_release_sleeplock");
        g_lwm2m_client_is_locking = false;
    } else if (type == LWM2M_NOTIFY_TYPE_DEREG && code != LWM2M_NOTIFY_CODE_SUCCESS) {
#ifdef MTK_LWM2M_CT_SUPPORT
        extern void ctiot_lwm2m_client_deregister_failed(void);
        ctiot_lwm2m_client_deregister_failed();
#endif
    } else if (type == LWM2M_NOTIFY_TYPE_LWSTATUS) {
#ifdef MTK_LWM2M_CT_SUPPORT
        extern void ctiot_lwm2m_client_session_abnormal(void);
        ctiot_lwm2m_client_session_abnormal();
#endif
    }
}

static void lwm2m_client_timeout_callback(TimerHandle_t xTimer)
{
    if (g_lwm2m_client_is_locking) {
        hal_sleep_manager_release_sleeplock(g_lwm2m_client_sleep_handle, HAL_SLEEP_LOCK_DEEP);
        LWM2MAPPLOG("hal_sleep_manager_release_sleeplock");
        g_lwm2m_client_is_locking = false;
    }
}

/******************************************************************************
* FUNCTION
*  nb_app_enter
* DESCRIPTION
*  nb_app_enter
* PARAMETERS
*  
* RETURNS
*  void
*******************************************************************************/
int nb_app_enter()
{
    g_lwm2m_client_ping_timer = xTimerCreate("lwm2m_client_timer",
                                      1000 * COAP_MAX_TRANSMIT_WAIT / portTICK_PERIOD_MS, 
                                      pdFALSE,
                                      (void *)0,
                                      lwm2m_client_timeout_callback);
    g_lwm2m_client_sleep_handle = hal_sleep_manager_set_sleep_handle("lwm2m_cli");
    POWERKEY_GET_SLEEP_HANDLE();
    if (rtc_power_on_result_external() != DEEP_SLEEP && rtc_power_on_result_external() != DEEPER_SLEEP)
    {
        LWM2MAPPPRINT("app enter from normal case");
        memset(&g_collect_data, 0x00, sizeof(lwm2m_app_object_data));
        g_nb_app_context.is_deep_sleep = false;
        app_rtc_handle = 0;
        nb_app_enter_internal();
    } else {
        nb_app_wakeup_handle();
    }

    return 1;
}


void nb_app_notify_lwm2m_client_restart(void)
{
    nb_app_message_struct *urc_msg = NULL;

    LWM2MAPPLOG("lwm2mc_restart");

    urc_msg = pvPortMalloc(sizeof(nb_app_message_struct));
    urc_msg->message_id = LWM2M_MSG_ID_CLIENT_RESTART;

    if (pdTRUE != xQueueSend(nb_bearer_configure_queue, &urc_msg, 10)) {
       LWM2MAPPLOG("sensor_app_ril_urc_callback send msg fail!\n");
    }
}


int32_t nb_app_is_nw_registered(ril_eps_network_registration_status_urc_t *nw_reg_status)
{
    if (TEL_CONN_MGR_NW_REG_STAT_REGED_HMNW == nw_reg_status->stat ||
        TEL_CONN_MGR_NW_REG_STAT_REGED_ROAMING == nw_reg_status->stat ||
        TEL_CONN_MGR_NW_REG_STAT_SMS_ONLY_HMNW == nw_reg_status->stat ||
        TEL_CONN_MGR_NW_REG_STAT_SMS_ONLY_ROAMING == nw_reg_status->stat)
    {
        LWM2MAPPLOG("nw registered");
        return 1;
    }

    LWM2MAPPLOG("nw not registered");
    return 0;
}


void nb_lwm2m_sleep_debug_handle(void)
{
    lwm2m_client_quit("sleep debug", 2);

    vTaskDelete(nb_data_ctrl_task);
    vTaskDelete(nb_bearer_cfg_task);
}


void nb_lwm2m_wakeup_debug_handle(void)
{
    int32_t i = 0;
    ril_status_t ret = RIL_STATUS_SUCCESS;

    LWM2MAPPPRINT("wakeup_handle");

    memset(&g_nb_app_context, 0x00, sizeof(nb_app_context));
    g_nb_app_context.is_deep_sleep = true;

    /* Upload period */
    g_nb_app_context.force_upload_period = (NOTIFICATION_UPDATE_TIME) > ((2 * COAP_MAX_TRANSMIT_WAIT) * 10) ? (NOTIFICATION_UPDATE_TIME) : ((2 * COAP_MAX_TRANSMIT_WAIT) * 10);

    /* Sensor collect data duration */
    g_nb_app_context.data_collect_period = DATA_COLLECT_PERIOD;

    /* Data retention */
    g_nb_app_context.app_state = g_collect_data.app_state;
    g_nb_app_context.accelerometer_blocks.total_num = 1;

    for (i = 0; i < g_collect_data.used_number; ++i) {
        g_nb_app_context.accelerometer_blocks.sensor_accelerometer[i].value_x = g_collect_data.sensor_accelerometer[i].value_x;
        g_nb_app_context.accelerometer_blocks.sensor_accelerometer[i].value_y = g_collect_data.sensor_accelerometer[i].value_y;
        g_nb_app_context.accelerometer_blocks.sensor_accelerometer[i].value_z = g_collect_data.sensor_accelerometer[i].value_z;
    }
    g_nb_app_context.accelerometer_blocks.current_use_num = g_collect_data.used_number;

    /* Create data collect task */
    nb_data_collect_enter(); 

    LWM2MAPPPRINT("collect_enter done");

    /* Create bearer connection task */
    lwm2m_client_bearer_init();

    LWM2MAPPPRINT("bearer_init done");

    g_nb_app_context.cli_port = LWM2M_CLI_PORT;

    g_nb_app_context.bearer_info.app_bearer_valid_flag = true;

    //app_send_msg(LWM2M_MSG_ID_LWM2M_INIT, NULL);
}


void nb_lwm2m_debug_init(void)
{
    //app_send_msg(LWM2M_MSG_ID_LWM2M_INIT, NULL);
    LWM2MAPPPRINT("nb_lwm2m_debug_init++");
    lwm2m_client_init();
    LWM2MAPPPRINT("nb_lwm2m_debug_init--");
}

uint32_t nb_lwm2m_rtc_enable = 0;

void nb_lwm2m_start_rtc_timer(void)
{
    int32_t ret = -1;

    if (g_nb_app_context.lifetime < 20) {
        LWM2MAPPLOG("lifetime short: %d\n", g_nb_app_context.lifetime);
        return;
    }

    if (!app_rtc_handle) {
        ret = rtc_sw_timer_create(&app_rtc_handle, (g_nb_app_context.lifetime - 15) * 10, true, rtcTimerCallbackFun);
        if (ret >= 0) {
            rtc_sw_timer_start(app_rtc_handle);
        }
        LWM2MAPPLOG("start_rtc_timer: %d\n", ret);
    } else {
        LWM2MAPPLOG("start_rtc_timer already");
    }
}


void nb_lwm2m_stop_rtc_timer(void)
{
    int32_t ret;

    if (app_rtc_handle) {
        ret = rtc_sw_timer_delete(app_rtc_handle);
        app_rtc_handle = 0;
        LWM2MAPPLOG("stop_rtc_timer: %d\n", ret);
    } else {
        LWM2MAPPLOG("stop_rtc_timer already");
    }
}

