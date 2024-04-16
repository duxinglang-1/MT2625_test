/*
    FreeRTOS V8.2.0 - Copyright (C) 2015 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>!AND MODIFIED BY!<< the FreeRTOS exception.

	***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
	***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
	the FAQ page "My application does not run, what could be wrong?".  Have you
	defined configASSERT()?

	http://www.FreeRTOS.org/support - In return for receiving this top quality
	embedded software for free we request you assist our global community by
	participating in the support forum.

	http://www.FreeRTOS.org/training - Investing in training allows your team to
	be as productive as possible as early as possible.  Now you can receive
	FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
	Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "os_port_callback.h"
#include "hal.h"

#ifdef MTK_OS_CPU_UTILIZATION_ENABLE
#include "hal_gpt.h"
#endif

#if  defined ( __GNUC__ )
#ifndef __weak
#define __weak   __attribute__((weak))
#endif /* __weak */
#endif /* __GNUC__ */

#ifdef HAL_SLEEP_MANAGER_ENABLED
extern  void uart_check_lock_status(void);
#endif

#ifdef MTK_NBIOT_TARGET_BUILD
extern  void modem_idle_check(void);
#endif

void wdt_idle_check(void);

void wdt_idle_check(void)
 {
#ifdef MTK_SYSTEM_HANG_CHECK_ENABLE
    static uint32_t time1_start_count = 0;
    static uint32_t time1_end_count = 0;
    static uint32_t time1_count = 0;

    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &time1_end_count);
    hal_gpt_get_duration_count(time1_start_count, time1_end_count, &time1_count);
    if(time1_count > 32768) {
        hal_wdt_feed(HAL_WDT_FEED_MAGIC);
        hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &time1_start_count);
        time1_count = 0;
    }
#endif
 }


#ifdef MTK_OS_CPU_UTILIZATION_ENABLE
extern uint32_t get_current_count(void);
uint32_t runtime_counter_base;

void vConfigureTimerForRunTimeStats(void)
{
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &runtime_counter_base);
}

uint32_t ulGetRunTimeCounterValue(void)
{
    uint32_t cur_count;
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &cur_count);

    return (cur_count - runtime_counter_base);
}
#endif /* MTK_OS_CPU_UTILIZATION_ENABLE */

#if( configCHECK_FOR_STACK_OVERFLOW > 0 )
__weak void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    printf("stack overflow: %x %s\r\n", (unsigned int)xTask, (portCHAR *)pcTaskName);
    configASSERT(0);
}
#endif

#if ( configUSE_IDLE_HOOK == 1 )
__weak void vApplicationIdleHook(void)
{
  wdt_idle_check();
 #ifdef MTK_NBIOT_TARGET_BUILD
  modem_idle_check();
 #endif
 #ifdef HAL_SLEEP_MANAGER_ENABLED
  uart_check_lock_status();
 #endif
}
#endif
