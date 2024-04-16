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

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* device.h includes */
#include "mt2625.h"

/* hal includes */
#include "hal.h"
#include "memory_attribute.h"


#include "sys_init.h"
#include "task_def.h"
#include "atci.h"

#include "syslog.h"

#ifdef MTK_USB_DEMO_ENABLED
#include "usb.h"
#endif

extern void md_init_phase_2(void);

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
//#define FREERTOS_TEST

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

#define mainCHECK_DELAY ( ( portTickType ) 1000 / portTICK_RATE_MS )

/**
* @brief       Task main function
* @param[in]   pvParameters: Pointer that will be used as the parameter for the task being created.
* @return      None.
*/
static void vTestTask(void *pvParameters)
{
    uint32_t idx = (int)pvParameters;

    portTickType xLastExecutionTime, xDelayTime;
    xLastExecutionTime = xTaskGetTickCount();
    xDelayTime = (1 << idx) * mainCHECK_DELAY;

    while (1) {
        vTaskDelayUntil(&xLastExecutionTime, xDelayTime);
        LOG_I(common, "Hello World from %d at %d\r\n", idx, xTaskGetTickCount());
    }
}

#ifdef FREERTOS_TEST
#include "FreeRTOS_test.h"
void vApplicationTickHook(void)
{
    vFreeRTOS_TestISR();
}
#endif /* FREERTOS_TEST */

/**
 * @brief This function is a task main function for processing the data handled by ATCI module.
 * @param[in] param is the task main function paramter.
 * @return    None
 */
static void atci_def_task(void *param)
{

    LOG_I(common, "enter atci_def_task!!\n\r");
    while (1) {
        atci_processing();
    }
}

int main(void)
{
    int idx;

    /* Do system initialization, eg: hardware, clock. */
    system_init();

    /* dummy function for MD */
    md_init_phase_2();
    
#ifdef MTK_USB_DEMO_ENABLED
	usb_boot_init();
#endif
    
    atci_init(HAL_UART_2);

    /* Add your application code here */
    LOG_I(common, "\r\n\r\n");/* The output UART used by log_hal_info is set by log_uart_init() */
    LOG_I(common, "welcome to main()\r\n");
    LOG_I(common, "pls add your own code from here\r\n");
    LOG_I(common, "\r\n\r\n");
    

#ifdef FREERTOS_TEST
    LOG_I("start to run freertos test.\r\n");
    xTaskCreate(vMainTestTask, FREERTOS_EXAMPLE_TASK_NAME, FREERTOS_EXAMPLE_TASK_STACKSIZE/sizeof(portSTACK_TYPE), NULL, FREERTOS_EXAMPLE_TASK_PRIO, NULL);
#else
    /* Create 4 tasks*/
    for (idx=0; idx<4; idx++) {
        xTaskCreate( vTestTask, FREERTOS_EXAMPLE_TASK_NAME, FREERTOS_EXAMPLE_TASK_STACKSIZE/sizeof(portSTACK_TYPE), (void*)idx, FREERTOS_EXAMPLE_TASK_PRIO, NULL);
    }
#endif /* FREERTOS_TEST */

/* create task for ATCI */
    xTaskCreate(atci_def_task, ATCI_TASK_NAME, ATCI_TASK_STACKSIZE /((uint32_t)sizeof(StackType_t)), NULL, ATCI_TASK_PRIO, NULL); 

    /* Start the scheduler. */
    SysInitStatus_Set();
#ifdef HAL_TIME_CHECK_ENABLED
    hal_time_check_enable();
#endif
    vTaskStartScheduler();
    /* If all is well, the scheduler will now be running, and the following line
    will never be reached.  If the following line does execute, then there was
    insufficient FreeRTOS heap memory available for the idle and/or timer tasks
    to be created.  See the memory management section on the FreeRTOS web site
    for more details. */
    for (;;);
}

