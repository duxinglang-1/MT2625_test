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

#include "hal_time_check.h"
#include "hal.h"
#include "assert.h"
#include "memory_attribute.h"
#include "hal_log.h"
#include "hal_gpt_internal.h"

#ifdef HAL_TIME_CHECK_ENABLED

#define time_printf      printf

ATTR_ZIDATA_IN_TCM bool is_time_check_enabled;

#ifdef HAL_TIME_CHECK_ISR_ENABLED

typedef struct {
    uint32_t irq_number;
    uint32_t execution_time;
    void*    callback;          //recording the callback for debugging
} time_check_t;

ATTR_ZIDATA_IN_TCM time_check_t time_check_buffer[IRQ_NUMBER_MAX];
ATTR_ZIDATA_IN_TCM int32_t  time_check_buffer_index;
ATTR_ZIDATA_IN_TCM uint32_t last_timestamp;

ATTR_TEXT_IN_TCM void time_check_start(uint32_t irq_number)
{
    uint32_t current_timestamp;
    uint32_t temp_duration_us;
    uint32_t mask = __get_BASEPRI();
  
    if(is_time_check_enabled == false) {
         return;
    }
    __set_BASEPRI(((1 << (8 - __NVIC_PRIO_BITS)) & 0xFF));//disable all IRQ
    __DMB();
    __ISB();
    
    current_timestamp =gp_gpt[HAL_GPT_US_PORT]->GPT_COUNT;
    if (time_check_buffer_index != -1) { // nested
        if( current_timestamp > last_timestamp ) {
            temp_duration_us = current_timestamp - last_timestamp;
        }else {
            temp_duration_us = (0xffffffff - (last_timestamp - current_timestamp)) + 1;
        }
        time_check_buffer[time_check_buffer_index].execution_time += temp_duration_us;
    }
    time_check_buffer_index++;
    time_check_buffer[time_check_buffer_index].execution_time =0;
    time_check_buffer[time_check_buffer_index].irq_number = irq_number;
    last_timestamp = current_timestamp;

    __set_BASEPRI(mask); //enable all IRQ
    __DMB();
    __ISB();
    
    return ;
}

ATTR_TEXT_IN_TCM void time_check_end(uint32_t irq_number,uint32_t limter_us,void* callback)
{
    uint32_t current_timestamp;
    uint32_t temp_duration_us;
    uint32_t mask = __get_BASEPRI();
    
    if(is_time_check_enabled == false) {
         return;
    }
    __set_BASEPRI(((1 << (8 - __NVIC_PRIO_BITS)) & 0xFF));//disable all IRQ
    __DMB();
    __ISB();

    if((time_check_buffer_index == -1) ||(time_check_buffer[time_check_buffer_index].irq_number != irq_number)) {
        #ifdef ENABLE_IRQ_HANDLER_CHECK_ASSERT
        printf("time_check_buffer_index=%d, irq_number = %d not match %d\r\n",time_check_buffer_index, time_check_buffer[time_check_buffer_index].irq_number,irq_number);
        assert(0);//start and end are not match!!!
        #endif
    }    
        
    current_timestamp = gp_gpt[HAL_GPT_US_PORT]->GPT_COUNT;
    if( current_timestamp > last_timestamp ) {
        temp_duration_us = current_timestamp - last_timestamp;
    }else {
        temp_duration_us = (0xffffffff - (last_timestamp - current_timestamp)) + 1;
    }

    temp_duration_us += time_check_buffer[time_check_buffer_index].execution_time ;
     
    time_check_buffer_index--;
    last_timestamp = current_timestamp;
    __set_BASEPRI(mask); //enable all IRQ
    __DMB();
    __ISB();
    
    if(temp_duration_us >= limter_us ) {
        //if time over than limter_us£¬ recording the callback user
        time_check_buffer[time_check_buffer_index+1].callback = callback;
        time_printf("ERROR!!!!!!The IRQ%d execute time too long:%d us, callback addr:0x%x\r\n",irq_number,temp_duration_us,(uint32_t)callback);

        #ifdef ENABLE_IRQ_HANDLER_CHECK_ASSERT
        assert(0);
        #endif
    }
}

#endif

#ifdef HAL_TIME_CHECK_DISABLE_IRQ_ENABLED

ATTR_ZIDATA_IN_TCM uint32_t time_check_disbale_irq_start; 

ATTR_TEXT_IN_TCM void hal_time_check_disable_irq_start(void)
{
    if(is_time_check_enabled == false) {
        return ;
    }
   time_check_disbale_irq_start = gp_gpt[HAL_GPT_US_PORT]->GPT_COUNT;
   return;
}

ATTR_TEXT_IN_TCM bool hal_time_check_disable_irq_end(uint32_t limter_us)
{
    uint32_t temp_time_end,temp_duration_us;
    
    if(is_time_check_enabled == false) {
        return false ;
    }
    
    temp_time_end = gp_gpt[HAL_GPT_US_PORT]->GPT_COUNT;
    if( temp_time_end > time_check_disbale_irq_start ) {
        temp_duration_us = temp_time_end - time_check_disbale_irq_start;
    }else {
        temp_duration_us = (0xffffffff - (time_check_disbale_irq_start - temp_time_end)) + 1;
    }
    //time_printf("Disable IRQ time:%d us\r\n",temp_duration_us);
    if( temp_duration_us >= limter_us ) {
        //printf("NBIOT MT2625 Disable IRQ time start:%d us end:%d us \r\n",time_check_disbale_irq_start,temp_time_end);
        
        //time_printf("\r\nERROR!!!!!! MT2625 Disable IRQ time too long:%d us\r\n",(int)temp_duration_us);
        
        #ifdef ENABLE_REAL_TIME_CHECK_ASSERT
        assert(0);
        #endif
        
        return false;
    }
    //else
     //  printf("NBIOT MT2625 Disable IRQ time is OK:%d us \r\n",temp_duration_us);
    
    return true;
}

#endif

void hal_time_check_enable(void)
{
    uint32_t temp_counter; 
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M,&temp_counter);

    if (is_time_check_enabled == false) {
        #ifdef HAL_TIME_CHECK_ISR_ENABLED
        time_check_buffer_index = -1;
        time_check_buffer[0].execution_time =0;
        #endif
        is_time_check_enabled = true;
    }
}

void hal_time_check_disable(void)
{

    is_time_check_enabled = false;
}

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif /* HAL_TIME_CHECK_ENABLED */


