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

#ifndef __HAL_TIME_CHECK_H__
#define __HAL_TIME_CHECK_H__
#include "hal.h"

#ifdef HAL_TIME_CHECK_ENABLED

//#define HAL_TIME_CHECK_ISR_ENABLED            //ISR handler execution time check feature

#ifdef HAL_TIME_CHECK_ISR_ENABLED
#define TIME_CHECK_ISR_TIME   1000              //Set the ISR execution time 1000us limit
//#define ENABLE_IRQ_HANDLER_CHECK_ASSERT       //enable assert when the ISR handler execution time over than 1000us
#endif


#define HAL_TIME_CHECK_DISABLE_IRQ_ENABLED      //IRQ masked time check feature

#ifdef HAL_TIME_CHECK_DISABLE_IRQ_ENABLED
#define TIME_CHECK_DISABLE_IRQ_TIME  100        //Set the IRQ disable time 100us limit
//#define ENABLE_REAL_TIME_CHECK_ASSERT         //enable assert when the IRQ disable time over than 200
#endif

#include "hal_gpt.h"

#ifdef __cplusplus
extern "C" {
#endif



#ifdef JUST_FOR_PERFORMANCE_MEASURE

extern void init_for_dwt_counter(void);

#define ITM_CONTROL (volatile uint32_t *)0xE0000E80
#define DWT_CONTROL (volatile uint32_t *)0xE0001000
#define DWT_CYCCNT (volatile uint32_t *)0xE0001004
#define DEMCR (volatile uint32_t *)0xE000EDFC


#define CPU_RESET_CYCLECOUNTER() \
do { \
*DEMCR = *DEMCR | 0x01000000; \
*DWT_CYCCNT = 0; \
*DWT_CONTROL = *DWT_CONTROL | 1 ; \
} while(0)



#define DWT_COUNTER_START(time) \
do{ \
time = *DWT_CYCCNT; \
} while(0)	

#define DWT_COUNTER_END(time) \
do{ \
time = *DWT_CYCCNT; \
} while(0)	


/*the read DWT_CYCCNT's SW overhead is 4 cycles*/  
#define DWT_COUNTER_PRINTF(string,start,end) \
do{ \
printf(string); \
printf("   cycle:%d\r\n",end-start-1); \
} while(0)
 

#endif



#if 0


extern uint32_t time_check_disbale_irq_start;

#define TIME_CHECK_START_DISABLE_IRQ() \
do{	\
    time_check_disbale_irq_start = ((GPT_REGISTER_T*)(_GPT4_BASEADDR_))->GPT_COUNT; \
}while(0)


#define TIME_CHECK_END_DISABLE_IRQ(limter_us) \
do{	\
    uint32_t temp_time_end,temp_duration_us;\
    temp_time_end = ((GPT_REGISTER_T*)(_GPT4_BASEADDR_))->GPT_COUNT; \
    if( temp_time_end > time_check_disbale_irq_start ) { \
        temp_duration_us = temp_time_end - time_check_disbale_irq_start; \
    }else { \
        temp_duration_us = (0xffffffff - (time_check_disbale_irq_start - temp_time_end)) + 1; \
    } \
    if( temp_duration_us >= limter_us ) { \
        printf("ERROR!!!!!! NBIOT MT2625 Disable IRQ time too long:%d us \r\n",temp_duration_us); \
        assert(0); \
    } \
}while(0)
    

#endif
   

/*proposal2*start**************************************************************************/
#ifdef HAL_TIME_CHECK_ENABLED
void hal_time_check_enable(void);
void hal_time_check_disable(void);
    

#ifdef HAL_TIME_CHECK_ISR_ENABLED
void  time_check_start(uint32_t irq_number);
void time_check_end(uint32_t irq_number,uint32_t limter_us,void* callback);
#endif

#ifdef HAL_TIME_CHECK_DISABLE_IRQ_ENABLED
void hal_time_check_disable_irq_start(void);
bool hal_time_check_disable_irq_end(uint32_t limter_us);
#endif
#endif


#ifdef __cplusplus
}
#endif

#endif /* HAL_TIME_CHECK_ENABLED*/
#endif /* __HAL_TIME_CHECK_H__ */


