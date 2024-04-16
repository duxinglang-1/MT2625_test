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

#include "hal_nvic.h"

#ifdef HAL_NVIC_MODULE_ENABLED
#include "hal_nvic_internal.h"
//#include "hal_flash_disk_internal.h"
#include "memory_attribute.h"
#include "hal_log.h"

#ifdef HAL_TIME_CHECK_ENABLED
#include "hal_time_check.h"
#endif

#ifdef IRQ_SPEED_DEBUG
#include "n1_hw_phy_timer.h"
#endif

#ifdef MTK_NBIOT_TARGET_BUILD
#include <sys_trace.h>
#endif

#ifdef MTK_SWLA_ENABLE
#include "swla.h"
#endif /* MTK_SWLA_ENABLE */

#ifdef __cplusplus
extern "C" {
#endif

//#define HAL_NVIC_PERFORMANCE_DEBUG 

#ifdef HAL_NVIC_PERFORMANCE_DEBUG
#include "hal_gpt_internal.h"
#endif


typedef struct {
    void (*nvic_callback)(hal_nvic_irq_t irq_number);
} nvic_function_t;

static const uint32_t defualt_irq_priority[IRQ_NUMBER_MAX] = {
    MDSYS_IRQ_0_PRIORITY,             //interrupt 0 
    MDSYS_IRQ_1_PRIORITY,             //interrupt 1 
    MDSYS_IRQ_2_PRIORITY,             //interrupt 2 
    MDSYS_IRQ_3_PRIORITY,             //interrupt 3 
    MDSYS_IRQ_4_PRIORITY,             //interrupt 4 
    MDSYS_IRQ_5_PRIORITY,             //interrupt 5 
    MDSYS_IRQ_6_PRIORITY,             //interrupt 6 
    MDSYS_IRQ_7_PRIORITY,             //interrupt 7 
    L1_SW_IRQ_PRIORITY,               //interrupt 8 
    L2_SW_IRQ_PRIORITY,               //interrupt 9 
    L1_SW_CRC_IRQ_PRIORITY,           //interrupt 10 
    GKI_LOG_PRIORITY,                 //interrupt 11 
    FIRQ_PRIORITY,                    //interrupt 12
    OS_GPT_PRIORITY,                  //interrupt 13
    DMA_MCU_PRIORITY,                 //interrupt 14
    DMA_SENSOR_PRIORITY,              //interrupt 15
    SPI_MST0_PRIORITY,                //interrupt 16
    SPI_MST1_PRIORITY,                //interrupt 17
    SPI_SLV_PRIORITY,                 //interrupt 18
    SDIO_SLV_PRIORITY,                //interrupt 19
    SDIO_MST0_PRIORITY,               //interrupt 20
    SDIO_MST1_PRIORITY,               //interrupt 21
    SDIO_MST0_WAKEUP_PRIORITY,        //interrupt 22
    SDIO_MST1_WAKEUP_PRIORITY,        //interrupt 23
    TRNG_PRIORITY,                    //interrupt 24
    CRYPTO_PRIORITY,                  //interrupt 25
    UART0_PRIORITY,                   //interrupt 26
    UART1_PRIORITY,                   //interrupt 27
    UART2_PRIORITY,                   //interrupt 28
    UART3_PRIORITY,                   //interrupt 29
    I2S0_PRIORITY,                    //interrupt 30
    I2S1_PRIORITY,                    //interrupt 31
    I2C0_PRIORITY,                    //interrupt 32    
    I2C1_PRIORITY,                    //interrupt 33
    I2C2_PRIORITY,                    //interrupt 34
    RTC_PRIORITY,                     //interrupt 35
    GPT_PRIORITY,                     //interrupt 36
    SPM_PRIORITY,                     //interrupt 37
    RGU_PRIORITY,                     //interrupt 38
    PMU_PRIORITY,                     //interrupt 39
    EINT_PRIORITY,                    //interrupt 40
    SFC_PRIORITY,                     //interrupt 41
    SENSOR_PD_PRIORITY,               //interrupt 42
    SENSOR_AO_0_PRIORITY,             //interrupt 43
    SENSOR_AO_1_PRIORITY,             //interrupt 44
    SENSOR_AO_2_PRIORITY,             //interrupt 45
    SENSOR_AO_3_PRIORITY,             //interrupt 46
    KP_PRIORITY,                      //interrupt 47
    USB_PRIORITY,                     //interrupt 48
    USIM_PRIORITY,                    //interrupt 49
    CIPHER_PRIORITY,                  //interrupt 50
    DMA_PWM_PRIORITY,                 //interrupt 51
    ULS_PRIORITY,                     //interrupt 52
    MDSYS_IRQ_9_PRIORITY,             //interrupt 53
    MDSYS_IRQ_10_PRIORITY,            //interrupt 54
    MDSYS_IRQ_11_PRIORITY,            //interrupt 55
    MDSYS_IRQ_12_PRIORITY,            //interrupt 56
    MDSYS_IRQ_13_PRIORITY,            //interrupt 57
    MDSYS_IRQ_14_PRIORITY,            //interrupt 58
    MDSYS_IRQ_15_PRIORITY,            //interrupt 59
    MDSYS_IRQ_16_PRIORITY,            //interrupt 60
    MDSYS_IRQ_17_PRIORITY,            //interrupt 61
    MDSYS_IRQ_18_PRIORITY,            //interrupt 62
    MDSYS_IRQ_19_PRIORITY,            //interrupt 63
    MDSYS_IRQ_20_PRIORITY,            //interrupt 64
    MDSYS_IRQ_21_PRIORITY,            //interrupt 65
    MDSYS_IRQ_22_PRIORITY,            //interrupt 66
    MDSYS_IRQ_23_PRIORITY,            //interrupt 67
    SECURITY_PRIORITY,                //interrupt 68
    BSIBPI0_PRIORITY,                 //interrupt 69
    BSIBPI1_PRIORITY,                 //interrupt 70
    CM4_reserved2_PRIORITY,           //interrupt 71
    CM4_reserved3_PRIORITY,           //interrupt 72
    CSCI_MD_READ_PRIORITY,            //interrupt 73
    CSCI_MD_WRITE_PRIORITY,           //interrupt 74
    CSCI_AP_READ_PRIORITY,            //interrupt 75
    CSCI_AP_WRITE_PRIORITY            //interrupt 76
};

nvic_function_t nvic_function_table[IRQ_NUMBER_MAX];

hal_nvic_status_t hal_nvic_init(void)
{
    static uint32_t priority_set = 0;
    uint32_t i;

    if (priority_set == 0) {
        /* Set defualt priority only one time */
        for (i = 0; i < IRQ_NUMBER_MAX; i++) {
            NVIC_SetPriority((hal_nvic_irq_t)i, defualt_irq_priority[i]);
        }
        priority_set = 1;
    }
    return HAL_NVIC_STATUS_OK;
}

hal_nvic_status_t hal_nvic_enable_irq(hal_nvic_irq_t irq_number)
{
    hal_nvic_status_t status = HAL_NVIC_STATUS_ERROR;

    if (irq_number < (hal_nvic_irq_t)0 || irq_number >= IRQ_NUMBER_MAX) {
        status = HAL_NVIC_STATUS_ERROR_IRQ_NUMBER;
        return status;
    } else {
        NVIC_EnableIRQ(irq_number);
        status = HAL_NVIC_STATUS_OK;
    }

    return status;
}

hal_nvic_status_t hal_nvic_disable_irq(hal_nvic_irq_t irq_number)
{
    hal_nvic_status_t status = HAL_NVIC_STATUS_ERROR;

    if (irq_number < (hal_nvic_irq_t)0 || irq_number >= IRQ_NUMBER_MAX) {
        status = HAL_NVIC_STATUS_ERROR_IRQ_NUMBER;
        return status;
    } else {
        NVIC_DisableIRQ(irq_number);
        status = HAL_NVIC_STATUS_OK;
    }

    return status;
}

uint32_t hal_nvic_get_pending_irq(hal_nvic_irq_t irq_number)
{
    uint32_t ret = 0xFF;

    if (irq_number < (hal_nvic_irq_t)0 || irq_number >= IRQ_NUMBER_MAX) {
        return ret;
    } else {
        ret = NVIC_GetPendingIRQ(irq_number);
    }

    return ret;
}

hal_nvic_status_t hal_nvic_set_pending_irq(hal_nvic_irq_t irq_number)
{
    hal_nvic_status_t status = HAL_NVIC_STATUS_ERROR;

    if (irq_number < (hal_nvic_irq_t)0 || irq_number >= IRQ_NUMBER_MAX) {
        status = HAL_NVIC_STATUS_ERROR_IRQ_NUMBER;
        return status;
    } else {
        NVIC_SetPendingIRQ(irq_number);
        status = HAL_NVIC_STATUS_OK;
    }

    return status;
}

hal_nvic_status_t hal_nvic_clear_pending_irq(hal_nvic_irq_t irq_number)
{
    hal_nvic_status_t status = HAL_NVIC_STATUS_ERROR;

    if (irq_number < (hal_nvic_irq_t)0 || irq_number >= IRQ_NUMBER_MAX) {
        status = HAL_NVIC_STATUS_ERROR_IRQ_NUMBER;
        return status;
    } else {
        NVIC_ClearPendingIRQ(irq_number);
        status = HAL_NVIC_STATUS_OK;
    }

    return status;
}

hal_nvic_status_t hal_nvic_set_priority(hal_nvic_irq_t irq_number, uint32_t priority)
{
    hal_nvic_status_t status = HAL_NVIC_STATUS_ERROR;

    if (irq_number < (hal_nvic_irq_t)0 || irq_number >= IRQ_NUMBER_MAX) {
        status = HAL_NVIC_STATUS_ERROR_IRQ_NUMBER;
        return status;
    } else {
        NVIC_SetPriority(irq_number, priority);
        status = HAL_NVIC_STATUS_OK;
    }

    return status;
}

uint32_t hal_nvic_get_priority(hal_nvic_irq_t irq_number)
{
    uint32_t ret = 0xff;

    if (irq_number < (hal_nvic_irq_t)0 || irq_number >= IRQ_NUMBER_MAX) {
        return ret;
    } else {
        ret = NVIC_GetPriority(irq_number);
    }

    return ret;
}

ATTR_TEXT_IN_TCM uint32_t get_current_irq()
{
    return (((SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) >> SCB_ICSR_VECTACTIVE_Pos) - 16);
}

extern ATTR_TEXT_IN_RAM void Flash_ReturnReady(void);

#ifdef MTK_MINI_DUMP_ENABLE
extern void MiniDump_RecordEnterInterruptInfo(uint32_t IrqID);
extern void MiniDump_RecordExitInterruptInfo(void);
#endif /*MTK_MINI_DUMP_ENABLE*/

ATTR_TEXT_IN_TCM hal_nvic_status_t isrC_main()
{
    hal_nvic_status_t status = HAL_NVIC_STATUS_ERROR;
    hal_nvic_irq_t irq_number;


    //performance debug
    #ifdef HAL_NVIC_PERFORMANCE_DEBUG
    static uint32_t timestamp[10];
    gp_gpt[HAL_GPT_US_PORT]->GPT_CLK = GPT_CLOCK_13MHZ|GPT_DIVIDE_1;
    timestamp[0] = gp_gpt[HAL_GPT_US_PORT]->GPT_COUNT;
    #endif

    Flash_ReturnReady();

    //performance debug
    #ifdef HAL_NVIC_PERFORMANCE_DEBUG
    timestamp[1] = gp_gpt[HAL_GPT_US_PORT]->GPT_COUNT;
    #endif

    #ifdef IRQ_SPEED_DEBUG
    uint32_t start,end;
    start=N1HwPhyTimerCounterRead32Bits();
    #endif

    irq_number = (hal_nvic_irq_t)(get_current_irq());

    #ifdef MTK_SWLA_ENABLE
    SLA_RamLogging((uint32_t)(IRQ_START | irq_number));
    #endif /* MTK_SWLA_ENABLE */

    #ifdef HSL_IRQ_TRACE
    NBIOT_TRACE1(SYS_TRACE_IRQ_IN, HSL_SYS_LOG_DEBUG, irq_number);
    #endif
    
    if (irq_number < (hal_nvic_irq_t)0 || irq_number >= IRQ_NUMBER_MAX) {
        status = HAL_NVIC_STATUS_ERROR_IRQ_NUMBER;
        return status;
    } else if (nvic_function_table[irq_number].nvic_callback == NULL) {
        status = HAL_NVIC_STATUS_ERROR_NO_ISR;
        
        #ifdef READY
        log_hal_error("ERROR: no IRQ handler! \n");
        #endif
        
        return status;
    } else {
    
        #ifdef HAL_TIME_CHECK_ISR_ENABLED
        if((irq_number != EINT_IRQn)&&(irq_number != FIRQ_IRQn)) {
            time_check_start(irq_number);
        }
        #endif
        
        #ifdef MTK_MINI_DUMP_ENABLE
        MiniDump_RecordEnterInterruptInfo(irq_number);
        #endif/*MTK_MINI_DUMP_ENABLE*/ 
        
        nvic_function_table[irq_number].nvic_callback(irq_number);
        status = HAL_NVIC_STATUS_OK;
        
        #ifdef HAL_TIME_CHECK_ISR_ENABLED
        if((irq_number != EINT_IRQn)&&(irq_number != FIRQ_IRQn)){ 
            time_check_end(irq_number, TIME_CHECK_ISR_TIME, (void*)nvic_function_table[irq_number].nvic_callback);
        }
        #endif
    }
    
    #ifdef HSL_IRQ_TRACE
    NBIOT_TRACE1(SYS_TRACE_IRQ_OUT, HSL_SYS_LOG_DEBUG, irq_number);
    #endif

    #ifdef IRQ_SPEED_DEBUG
    end=N1HwPhyTimerCounterRead32Bits();
        /* Trace if over 2ms */
        if ((end-start)>3840)
            {
            NBIOT_TRACE2(SYS_TRACE_IRQ_SPEED_WARNING, HSL_SYS_LOG_DEBUG, ((end-start)/1920),irq_number);
          }
    #endif

    #ifdef MTK_SWLA_ENABLE
    SLA_RamLogging((uint32_t)IRQ_END);
    #endif /* MTK_SWLA_ENABLE */

    #ifdef MTK_MINI_DUMP_ENABLE
    MiniDump_RecordExitInterruptInfo();
    #endif /*MTK_MINI_DUMP_ENABLE*/

    //performance debug
    #ifdef HAL_NVIC_PERFORMANCE_DEBUG
    timestamp[2] = gp_gpt[HAL_GPT_US_PORT]->GPT_COUNT;
    log_hal_info("flash=%d, total=%d\r\n", timestamp[1] - timestamp[0], timestamp[2] - timestamp[0]);
    gp_gpt[HAL_GPT_US_PORT]->GPT_CLK = GPT_CLOCK_13MHZ|GPT_DIVIDE_13;
    #endif

    return status;
}

hal_nvic_status_t hal_nvic_register_isr_handler(hal_nvic_irq_t irq_number, hal_nvic_isr_t callback)
{
    uint32_t mask;

    if (irq_number < (hal_nvic_irq_t)0 || irq_number >= IRQ_NUMBER_MAX || callback == NULL) {
        return HAL_NVIC_STATUS_INVALID_PARAMETER;
    }

    mask = save_and_set_interrupt_mask();
    NVIC_ClearPendingIRQ(irq_number);
    nvic_function_table[irq_number].nvic_callback = callback;
    restore_interrupt_mask(mask);

    return HAL_NVIC_STATUS_OK;
}

ATTR_TEXT_IN_TCM hal_nvic_status_t hal_nvic_save_and_set_interrupt_mask(uint32_t *mask)
{
#if 0
#if defined(__GNUC__)
    uint32_t xLinkRegAddr0 = (uint32_t)__builtin_return_address(0);
    printf("hal_nvic_save_and_set_interrupt_mask calluser:0x%x\r\n",xLinkRegAddr0);
#endif
#endif

    *mask = save_and_set_interrupt_mask();
#if 0
    //set pri priority AP_SIDE_PRIORITY,_START, fault can be handled first as priority is default zero.
    uint32_t priority = AP_SIDE_PRIORITY_START;
    *mask = __get_BASEPRI();
    __set_BASEPRI(((priority << (8 - __NVIC_PRIO_BITS)) & 0xFF));
    __DMB();
    __ISB();
#endif
  
    return HAL_NVIC_STATUS_OK;
}

ATTR_TEXT_IN_TCM hal_nvic_status_t hal_nvic_restore_interrupt_mask(uint32_t mask)
{

#if 0
#if defined(__GNUC__)
    uint32_t xLinkRegAddr0 = (uint32_t)__builtin_return_address(0);
    printf("hal_nvic_restore_interrupt_mask calluser:0x%x\r\n",xLinkRegAddr0);
#endif
#endif

    restore_interrupt_mask(mask);
#if 0
    __set_BASEPRI(mask);
    __DMB();
    __ISB();
#endif
    return HAL_NVIC_STATUS_OK;
}

#ifdef __cplusplus
}
#endif

#endif /* HAL_NVIC_MODULE_ENABLED */

