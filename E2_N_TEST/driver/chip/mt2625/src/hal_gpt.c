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

#include "hal_gpt.h"
#include "hal_nvic_internal.h"
#ifdef HAL_GPT_MODULE_ENABLED
#include "hal_gpt_internal.h"
#include "hal_log.h"
#include "memory_attribute.h"
//#include "hal_nvic_internal.h"
#include "hal_sleep_manager.h"
#include "hal_sleep_driver.h"


//Tieming add for FPGA

#ifdef FPGA_ENV
#define MT2625_FPGA_26M_to_13M
#define MT2625_FPGA_32K_to_13M
#endif

#ifdef HAL_SLEEP_MANAGER_ENABLED
const static char *gpt_lock_sleep_name[HAL_GPT_MAX_PORT] = {"GPT0", "GPT1", "GPT2", "GPT3", "GPT4", "GPT5"};
static uint8_t gpt_lock_sleep_handle[HAL_GPT_MAX_PORT];

#define GPT_GET_SLEEP_HANDLE(gpt_port) \
do{ \
    gpt_lock_sleep_handle[gpt_port] = hal_sleep_manager_set_sleep_handle(gpt_lock_sleep_name[gpt_port]); \
    if(gpt_lock_sleep_handle[gpt_port] == INVALID_SLEEP_HANDLE ) { \
        log_hal_error("[GPT%d][init]:get sleep handle failed\r\n",gpt_port); \
        return HAL_GPT_STATUS_ERROR; \
    } \
}while(0)

#define GPT_RELEASE_SLEEP_HANDLE(gpt_port) \
do{ \
    hal_sleep_manager_release_sleep_handle(gpt_lock_sleep_handle[gpt_port]); \
}while(0)

#define GPT_LOCK_SLEEP(gpt_port) \
do{ \
    hal_sleep_manager_acquire_sleeplock(gpt_lock_sleep_handle[gpt_port], HAL_SLEEP_LOCK_ALL); \
}while(0)

#define GPT_UNLOCK_SLEEP(gpt_port) \
do{ \
    hal_sleep_manager_release_sleeplock(gpt_lock_sleep_handle[gpt_port], HAL_SLEEP_LOCK_ALL); \
}while(0)

#else
#define GPT_GET_SLEEP_HANDLE(gpt_port)
#define GPT_RELEASE_SLEEP_HANDLE(gpt_port)
#define GPT_LOCK_SLEEP(gpt_port)
#define GPT_UNLOCK_SLEEP(gpt_port)
#endif

static bool hal_gpt_is_port_valid(hal_gpt_port_t gpt_port)
{
    /*make sure this port just for sw gpt*/
    if ((gpt_sw_context.is_sw_gpt == false) && (gpt_port == HAL_GPT_SW_PORT)) {
        return false;
    }

    if ((gpt_port < HAL_GPT_MAX_PORT) && (gpt_port != HAL_GPT_MS_PORT) && (gpt_port != HAL_GPT_US_PORT)) {
        return true;
    } else {
        return false;
    }
}

hal_gpt_status_t hal_gpt_init(hal_gpt_port_t gpt_port)
{

    if (hal_gpt_is_port_valid(gpt_port) != true) {
        return HAL_GPT_STATUS_ERROR_PORT;
    }

    if ((g_gpt_context[gpt_port].running_status == HAL_GPT_RUNNING) ||
            (g_gpt_context[gpt_port].has_initilized == true)) {

        return HAL_GPT_STATUS_ERROR;
    }

    /*set structure to 0 */
    memset(&g_gpt_context[gpt_port], 0, sizeof(gpt_context_t));

    /*enable pdn power, open clock source*/
    gpt_open_clock_source();
    gp_gpt[gpt_port]->GPT_CON_UNION.GPT_CON &= ~(GPT_CLOCK_GATE);

    /*disable interrutp*/
    gp_gpt[gpt_port]->GPT_IRQ_EN  &= ~GPT_IRQ_ENABLE;

    GPT_GET_SLEEP_HANDLE(gpt_port);
#ifdef GPT_DEBUG_LOG
    log_hal_info("[GPT%d] init OK\r\n", (int)gpt_port);
#endif

    /*set flag respect this port has initlized */
    g_gpt_context[gpt_port].has_initilized = true;

    return HAL_GPT_STATUS_OK;
}

hal_gpt_status_t hal_gpt_deinit(hal_gpt_port_t gpt_port)
{

    if (hal_gpt_is_port_valid(gpt_port) != true) {
        return HAL_GPT_STATUS_ERROR_PORT;
    }

    if (g_gpt_context[gpt_port].running_status == HAL_GPT_RUNNING) {

        return HAL_GPT_STATUS_ERROR;
    }

    /* set structure to 0 */
    memset(&g_gpt_context[gpt_port], 0, sizeof(gpt_context_t));

    /* set flag indicate this port has deinitlized */
    g_gpt_context[gpt_port].has_initilized = false;

    gpt_reset_default_timer((uint32_t)gpt_port);

    GPT_RELEASE_SLEEP_HANDLE(gpt_port);
#ifdef GPT_DEBUG_LOG
    log_hal_info("[GPT%d] deinit OK\r\n", (int)gpt_port);
#endif

    return HAL_GPT_STATUS_OK;
}


ATTR_TEXT_IN_TCM hal_gpt_status_t hal_gpt_get_free_run_count(hal_gpt_clock_source_t clock_source, uint32_t *count)
{

#ifdef MT2625_FPGA_32K_to_13M
    uint32_t temp_count = 0;
#endif
    /* millisecond free run timer */
    if (clock_source == HAL_GPT_CLOCK_SOURCE_32K) {
        if (g_gpt_context[HAL_GPT_MS_PORT].running_status != HAL_GPT_RUNNING) {

            /* set clock source to 32khz, and start timer */
            gpt_start_free_run_timer(gp_gpt[HAL_GPT_MS_PORT], GPT_CLOCK_32KHZ, GPT_DIVIDE_1);

            g_gpt_context[HAL_GPT_MS_PORT].running_status = HAL_GPT_RUNNING;
        }
#ifdef MT2625_FPGA_32K_to_13M
        temp_count = gpt_current_count(gp_gpt[HAL_GPT_MS_PORT]);
        *count = (2 * temp_count + 4 * temp_count / 10 + 6 * temp_count / 100 + temp_count / 1000 + 5 * temp_count / 10000)/1000;
#else
        *count = gpt_current_count(gp_gpt[HAL_GPT_MS_PORT]);
#endif
    } /* microsecond free rum timer */
    else if (clock_source == HAL_GPT_CLOCK_SOURCE_1M) {
        if (g_gpt_context[HAL_GPT_US_PORT].running_status != HAL_GPT_RUNNING) {

            /* set clcok source to 1mhz, and start timer */
            gpt_start_free_run_timer(gp_gpt[HAL_GPT_US_PORT], GPT_CLOCK_13MHZ, GPT_DIVIDE_13);

            g_gpt_context[HAL_GPT_US_PORT].running_status = HAL_GPT_RUNNING;

        }
        *count = gpt_current_count(gp_gpt[HAL_GPT_US_PORT]);
    } else {
        return HAL_GPT_STATUS_INVALID_PARAMETER;
    }

    return HAL_GPT_STATUS_OK;
}

ATTR_TEXT_IN_TCM hal_gpt_status_t hal_gpt_get_duration_count(uint32_t start_count, uint32_t end_count, uint32_t *duration_count)
{
    if (duration_count == NULL) {
        return HAL_GPT_STATUS_INVALID_PARAMETER;
    }

    if (end_count > start_count) {
        *duration_count = end_count - start_count;
    } else {

        *duration_count = (0xffffffff - (start_count - end_count)) + 1;
    }
    return HAL_GPT_STATUS_OK;
}

hal_gpt_status_t hal_gpt_get_running_status(hal_gpt_port_t gpt_port, hal_gpt_running_status_t *running_status)
{
    if (gpt_port >= HAL_GPT_MAX_PORT) {
        return HAL_GPT_STATUS_ERROR_PORT;
    }

    *running_status = g_gpt_context[gpt_port].running_status;

    return HAL_GPT_STATUS_OK;
}

hal_gpt_status_t hal_gpt_register_callback(hal_gpt_port_t    gpt_port,
        hal_gpt_callback_t   callback,
        void                *user_data)
{

    if (hal_gpt_is_port_valid(gpt_port) != true) {
        return HAL_GPT_STATUS_ERROR_PORT;
    }

    if ((g_gpt_context[gpt_port].running_status == HAL_GPT_RUNNING) ||
            (g_gpt_context[gpt_port].has_initilized != true)) {

        return HAL_GPT_STATUS_ERROR;
    }

    if (callback == NULL) {
        return HAL_GPT_STATUS_INVALID_PARAMETER;
    }

#ifdef GPT_DEBUG_LOG
    log_hal_info("[GPT%d] register callback:0x%.8x\r\n", (unsigned int)gpt_port, (unsigned int)callback);
#endif
    g_gpt_context[gpt_port].callback_context.callback  = callback;
    g_gpt_context[gpt_port].callback_context.user_data = user_data;

    gpt_nvic_register();

    return HAL_GPT_STATUS_OK;
}

hal_gpt_status_t hal_gpt_start_timer_ms(hal_gpt_port_t gpt_port, uint32_t timeout_time_ms, hal_gpt_timer_type_t timer_type)
{

    volatile uint32_t mask;
    if (hal_gpt_is_port_valid(gpt_port) != true) {
        return HAL_GPT_STATUS_ERROR_PORT;
    }

    if (timeout_time_ms > HAL_GPT_MAXIMUM_MS_TIMER_TIME) {

        return HAL_GPT_STATUS_INVALID_PARAMETER;
    }
#ifdef GPT_DEBUG_LOG
    if (gpt_port !=  HAL_GPT_SW_PORT) {
        log_hal_info("[GPT%d]hal_gpt_start_timer_ms, time=%d ms,type=%d\r\n", (int)gpt_port, (int)timeout_time_ms, (int)timer_type);
    }
#endif

    mask = save_and_set_interrupt_mask();
    if ((g_gpt_context[gpt_port].running_status == HAL_GPT_RUNNING)
            || (g_gpt_context[gpt_port].has_initilized != true)) {
            
        restore_interrupt_mask(mask);
        return HAL_GPT_STATUS_ERROR;
    }
    
    gpt_open_clock_source();
    gp_gpt[gpt_port]->GPT_IRQ_EN &= ~GPT_IRQ_ENABLE;              /* disable interrupt */
    gp_gpt[gpt_port]->GPT_CON_UNION.GPT_CON_CELLS.EN &= ~GPT_COUNT_START;

    /* set to 32K clock and 1 division,clear counter */
    gp_gpt[gpt_port]->GPT_CLK       = GPT_CLOCK_32KHZ | (uint32_t)GPT_DIVIDE_1;
    gp_gpt[gpt_port]->GPT_COMPARE   = gpt_convert_ms_to_32k_count(timeout_time_ms) ;
    gp_gpt[gpt_port]->GPT_IRQ_ACK   = GPT_IRQ_FLAG_ACK;
    gp_gpt[gpt_port]->GPT_CLR       = GPT_COUNT_CLEAR;
    restore_interrupt_mask(mask);

    while (gp_gpt_glb->GPT_CLRSTA & (1<<gpt_port));

    mask = save_and_set_interrupt_mask();
    gp_gpt[gpt_port]->GPT_CON_UNION.GPT_CON = 0;
    
    /* set to mode, open clock source and start counter */
    if (timer_type) {
        gp_gpt[gpt_port]->GPT_CON_UNION.GPT_CON |= (uint32_t)(GPT_MODE_REPEAT | GPT_COUNT_START);
    } else {
        gp_gpt[gpt_port]->GPT_CON_UNION.GPT_CON |= (uint32_t)(GPT_MODE_ONE_SHOT | GPT_COUNT_START);
    }

    gp_gpt[gpt_port]->GPT_IRQ_EN = GPT_IRQ_ENABLE;
    g_gpt_context[gpt_port].running_status = HAL_GPT_RUNNING;
    restore_interrupt_mask(mask);

    return HAL_GPT_STATUS_OK;
}


ATTR_TEXT_IN_TCM hal_gpt_status_t hal_gpt_delay_ms(uint32_t ms)
{
    /* if free run timer is not open, open it */
    if (g_gpt_context[HAL_GPT_MS_PORT].running_status != HAL_GPT_RUNNING) {

        /* set clcok source to 1mhz, and start timer */
        gpt_start_free_run_timer(gp_gpt[HAL_GPT_MS_PORT], GPT_CLOCK_32KHZ, GPT_DIVIDE_1);
        g_gpt_context[HAL_GPT_MS_PORT].running_status = HAL_GPT_RUNNING;
    }

    gpt_delay_time(gp_gpt[HAL_GPT_MS_PORT], gpt_convert_ms_to_32k_count(ms));
    return HAL_GPT_STATUS_OK;
}


#ifdef HAL_GPT_FEATURE_US_TIMER
hal_gpt_status_t hal_gpt_start_timer_us(hal_gpt_port_t gpt_port, uint32_t timeout_time_us, hal_gpt_timer_type_t timer_type)
{
    volatile uint32_t mask;

    if (hal_gpt_is_port_valid(gpt_port) != true) {
        //log_hal_error("Invalid port: %d. Only port 0 or 1 works as timer.", gpt_port);
        return HAL_GPT_STATUS_ERROR_PORT;
    }

    if ((g_gpt_context[gpt_port].running_status == HAL_GPT_RUNNING)
            || (g_gpt_context[gpt_port].has_initilized != true)) {

        return HAL_GPT_STATUS_ERROR;
    }

#ifdef GPT_DEBUG_LOG
    if (gpt_port !=  HAL_GPT_SW_PORT) {
        log_hal_info("[GPT%d]hal_gpt_start_timer_us, time=%d us,type=%d\r\n", (int)gpt_port, (int)timeout_time_us, (int)timer_type);
    }
#endif

    GPT_LOCK_SLEEP(gpt_port);
    g_gpt_context[gpt_port].is_gpt_locked_sleep = true;

    gpt_open_clock_source();

    gp_gpt[gpt_port]->GPT_IRQ_EN &= ~GPT_IRQ_ENABLE;              /* disable interrupt */
    gp_gpt[gpt_port]->GPT_CON_UNION.GPT_CON_CELLS.EN &= ~GPT_COUNT_START;

    //set to 13MHz clock and 13 division, clear counter,1 us per tick
    gp_gpt[gpt_port]->GPT_CLK       = GPT_CLOCK_13MHZ | (uint32_t)GPT_DIVIDE_13;
    gp_gpt[gpt_port]->GPT_COMPARE   = timeout_time_us;
    gp_gpt[gpt_port]->GPT_IRQ_ACK   = GPT_IRQ_FLAG_ACK;
    gp_gpt[gpt_port]->GPT_CLR       = GPT_COUNT_CLEAR;

    while (gp_gpt_glb->GPT_CLRSTA & (1<<gpt_port));

    gp_gpt[gpt_port]->GPT_CON_UNION.GPT_CON = 0;
    mask = save_and_set_interrupt_mask();
    /* set to mode, open clock source and start counter */
    if (timer_type) {
        gp_gpt[gpt_port]->GPT_CON_UNION.GPT_CON |= (uint32_t)(GPT_MODE_REPEAT | GPT_COUNT_START);
    } else {
        gp_gpt[gpt_port]->GPT_CON_UNION.GPT_CON |= (uint32_t)(GPT_MODE_ONE_SHOT | GPT_COUNT_START);
    }

    gp_gpt[gpt_port]->GPT_IRQ_EN = GPT_IRQ_ENABLE;

    restore_interrupt_mask(mask);

    g_gpt_context[gpt_port].running_status = HAL_GPT_RUNNING;

    return HAL_GPT_STATUS_OK;
}



ATTR_TEXT_IN_TCM hal_gpt_status_t hal_gpt_delay_us(uint32_t us)
{

    /* if free run timer is not open, open it */
    if (g_gpt_context[HAL_GPT_US_PORT].running_status != HAL_GPT_RUNNING) {

        /* set clcok source to 1mhz, and start timer */
        gpt_start_free_run_timer(gp_gpt[HAL_GPT_US_PORT], GPT_CLOCK_13MHZ, GPT_DIVIDE_13);

        g_gpt_context[HAL_GPT_US_PORT].running_status = HAL_GPT_RUNNING;
    }
    gpt_delay_time(gp_gpt[HAL_GPT_US_PORT], us);

    return HAL_GPT_STATUS_OK;
}
#endif /* HAL_GPT_FEATURE_US_TIMER */


hal_gpt_status_t hal_gpt_stop_timer(hal_gpt_port_t gpt_port)
{
    volatile uint32_t mask;

    if (hal_gpt_is_port_valid(gpt_port) != true) {
        return HAL_GPT_STATUS_ERROR_PORT;
    }

#ifdef GPT_DEBUG_LOG
    if (gpt_port !=  HAL_GPT_SW_PORT) {
        log_hal_info("[GPT%d]hal_gpt_stop_timer\r\n", (int)gpt_port);
    }
#endif

    mask = save_and_set_interrupt_mask();

    /*diable interrupt*/
    gp_gpt[gpt_port]->GPT_IRQ_EN &= ~GPT_IRQ_ENABLE;

    /* stop timer */
    gp_gpt[gpt_port]->GPT_CON_UNION.GPT_CON_CELLS.EN &= ~GPT_COUNT_START;
    gp_gpt[gpt_port]->GPT_IRQ_ACK = GPT_IRQ_FLAG_ACK;

    g_gpt_context[gpt_port].running_status = HAL_GPT_STOPPED;
    restore_interrupt_mask(mask);

    if (g_gpt_context[gpt_port].is_gpt_locked_sleep == true) {
        GPT_UNLOCK_SLEEP(gpt_port);
        g_gpt_context[gpt_port].is_gpt_locked_sleep = false;
    }

    return HAL_GPT_STATUS_OK;
}



/**************** software timer for  multiple user *************************/

hal_gpt_status_t hal_gpt_sw_get_timer(uint32_t *handle)
{
    uint32_t timer;
    uint32_t mask;

    if (handle == NULL) {
        return HAL_GPT_STATUS_INVALID_PARAMETER;
    }

    mask = save_and_set_interrupt_mask();
    if (gpt_sw_context.used_timer_count >= HAL_GPT_SW_NUMBER) {
        restore_interrupt_mask(mask);
        return HAL_GPT_STATUS_ERROR;
    }

    timer   = gpt_sw_get_free_timer();
    *handle = timer | HAL_GPT_SW_MAGIC;

    gpt_sw_context.timer[timer].is_used = true;
    gpt_sw_context.used_timer_count++;
    restore_interrupt_mask(mask);
    
    return HAL_GPT_STATUS_OK;
}

hal_gpt_status_t hal_gpt_sw_free_timer(uint32_t handle)
{
    volatile uint32_t timer;
    uint32_t mask;

    if ((handle & HAL_GPT_SW_MAGIC) != HAL_GPT_SW_MAGIC) {
        return HAL_GPT_STATUS_INVALID_PARAMETER;
    }

    timer = handle & HAL_GPT_SW_HANDLE_MASK;

    if (gpt_sw_context.timer[timer].is_running == true) {
        return HAL_GPT_STATUS_ERROR;
    }

    if (gpt_sw_context.timer[timer].is_used != true) {
        return HAL_GPT_STATUS_ERROR;
    }

    mask = save_and_set_interrupt_mask();
    if (gpt_sw_context.used_timer_count == 0) {
        restore_interrupt_mask(mask);
        return HAL_GPT_STATUS_ERROR;
    }
    
    gpt_sw_context.timer[timer].is_used = false;
    gpt_sw_context.used_timer_count--;
    restore_interrupt_mask(mask);

    return HAL_GPT_STATUS_OK;
}

hal_gpt_status_t hal_gpt_sw_start_timer_ms(uint32_t handle, uint32_t timeout_time_ms, hal_gpt_callback_t callback, void *user_data)
{
    uint32_t current_time;
    uint32_t mask;
    uint32_t timer;
    bool is_set_data = false;

    if ((handle & HAL_GPT_SW_MAGIC) != HAL_GPT_SW_MAGIC) {
        return HAL_GPT_STATUS_INVALID_PARAMETER;
    }

    timer = handle & HAL_GPT_SW_HANDLE_MASK;

    if (callback == NULL) {
        return HAL_GPT_STATUS_INVALID_PARAMETER;
    }

    if (gpt_sw_context.timer[timer].is_running == true) {
        return HAL_GPT_STATUS_ERROR;
    }

    if (gpt_sw_context.timer[timer].is_used != true) {
        return HAL_GPT_STATUS_ERROR;
    }

    if (timeout_time_ms > HAL_GPT_MAXIMUM_MS_TIMER_TIME) {

        return HAL_GPT_STATUS_INVALID_PARAMETER;
    }

    mask = save_and_set_interrupt_mask();
    if (gpt_sw_context.is_first_init == false) {
        
        gpt_sw_context.is_first_init = true;
        restore_interrupt_mask(mask);

        gpt_sw_context.is_sw_gpt = true;
        hal_gpt_init(HAL_GPT_SW_PORT);
        hal_gpt_register_callback(HAL_GPT_SW_PORT, (hal_gpt_callback_t)gpt_sw_handler, NULL);
        gpt_sw_context.is_sw_gpt = false;
        
    } else {
        restore_interrupt_mask(mask);
    }

    do {
        gpt_sw_context.is_sw_gpt = true;
        gpt_sw_context.is_set_reentrant_flag = true;
        hal_gpt_stop_timer(HAL_GPT_SW_PORT);

        if(gpt_sw_context.is_start_from_isr == false) {
            current_time = gpt_sw_get_current_time_ms(gp_gpt[HAL_GPT_SW_PORT]);
        } else {
            current_time = 0;
        }

        /*ensure this section not to be interrupt*/
        mask = save_and_set_interrupt_mask();
        if(gpt_sw_context.is_gpt_count_valid == true) {
            gpt_sw_context.is_gpt_count_valid = false;
            gpt_sw_context.absolute_time +=  current_time;
        }
        
        if(is_set_data == false) {
            is_set_data = true;
            gpt_sw_context.timer[timer].is_running      = true;
            gpt_sw_context.timer[timer].callback_context.callback   = callback;
            gpt_sw_context.timer[timer].callback_context.user_data  = user_data;
            gpt_sw_context.running_timer_count++;
            gpt_sw_context.timer[timer].time_out_ms = gpt_sw_context.absolute_time + timeout_time_ms;
        }
        restore_interrupt_mask(mask);
        
        gpt_sw_start_timer();
    }while(gpt_sw_context.is_set_reentrant_flag == false);

    /*restore mask*/
    gpt_sw_context.is_sw_gpt = false;
    gpt_sw_context.is_set_reentrant_flag = false;
    
    return HAL_GPT_STATUS_OK;
}

hal_gpt_status_t hal_gpt_sw_stop_timer_ms(uint32_t handle)
{
    uint32_t current_time;
    uint32_t mask;
    uint32_t timer;
    bool is_set_data = false;

    if ((handle & HAL_GPT_SW_MAGIC) != HAL_GPT_SW_MAGIC) {
        return HAL_GPT_STATUS_INVALID_PARAMETER;
    }

    timer = handle & HAL_GPT_SW_HANDLE_MASK;

    if (gpt_sw_context.timer[timer].is_running != true) {
        return HAL_GPT_STATUS_ERROR;
    }

    if (gpt_sw_context.timer[timer].is_used != true) {
        return HAL_GPT_STATUS_ERROR;
    }
    
    do {
        gpt_sw_context.is_sw_gpt = true;
        gpt_sw_context.is_set_reentrant_flag = true;
        hal_gpt_stop_timer(HAL_GPT_SW_PORT);

        if(gpt_sw_context.is_start_from_isr == false) {
            current_time = gpt_sw_get_current_time_ms(gp_gpt[HAL_GPT_SW_PORT]);
        } else {
            current_time = 0;
        }

        /*ensure this section not to be interrupt*/
        mask = save_and_set_interrupt_mask();
        if(gpt_sw_context.is_gpt_count_valid == true) {
            gpt_sw_context.is_gpt_count_valid = false;
            gpt_sw_context.absolute_time +=  current_time;
        }
        
        if(is_set_data == false) {
            is_set_data = true;
            gpt_sw_context.timer[timer].is_running = false;
            gpt_sw_context.running_timer_count--;
        }
        restore_interrupt_mask(mask);

        if (gpt_sw_context.running_timer_count != 0) {
            gpt_sw_start_timer();
        } else {
            mask = save_and_set_interrupt_mask();
            gpt_sw_context.last_absolute_time = gpt_sw_context.absolute_time;
            restore_interrupt_mask(mask);
        }
    }while(gpt_sw_context.is_set_reentrant_flag == false);
    
    gpt_sw_context.is_sw_gpt = false;
    gpt_sw_context.is_set_reentrant_flag = false;
    
    return HAL_GPT_STATUS_OK;
}


hal_gpt_status_t hal_gpt_sw_get_remaining_time_ms(uint32_t handle, uint32_t *remaing_time)
{
    uint32_t mask;
    uint32_t timer;
    uint32_t count,time_s,time_ms,current_time;

    if ((handle & HAL_GPT_SW_MAGIC) != HAL_GPT_SW_MAGIC) {
        return HAL_GPT_STATUS_INVALID_PARAMETER;
    }

    timer = handle & HAL_GPT_SW_HANDLE_MASK;

    if (gpt_sw_context.timer[timer].is_used != true) {
        return HAL_GPT_STATUS_ERROR;
    }

    /*ensure this section not to be interrupt*/
    mask = save_and_set_interrupt_mask();
    
    count = gp_gpt[HAL_GPT_SW_PORT]->GPT_COUNT;
    time_s = count / 32768;
    time_ms = ((count % 32768) * 1000 + 16384) / 32768;
    current_time = time_s * 1000 + time_ms;
    current_time += gpt_sw_context.absolute_time;

    if (gpt_sw_context.timer[timer].time_out_ms >  current_time) {
        *remaing_time = gpt_sw_context.timer[timer].time_out_ms - current_time;
    } else {
        *remaing_time = 0;
    }

    restore_interrupt_mask(mask);

    return HAL_GPT_STATUS_OK;
}
#endif //HAL_GPT_MODULE_ENABLED


