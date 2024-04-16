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

#include "hal_nvic_internal.h"

#ifdef HAL_NVIC_MODULE_ENABLED
#include "hal_nvic.h"
#include "memory_attribute.h"

#ifdef HAL_TIME_CHECK_ENABLED
#include"hal_time_check.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif


#ifdef NVIC_USE_BASEPRI
#include "assert.h"
#define NVIC_DISABLE_LEVEL 2
ATTR_ZIDATA_IN_TCM uint32_t nvic_nest_level;
extern bool exception_occur(void);
void nvic_check_interrupt_not_disable(void)
{
    if(!exception_occured())
       assert(nvic_nest_level == 0);
}
#endif

void nvic_clear_all_pending_interrupt(void)
{
    uint8_t i,j;
    uint32_t temp = 0;
    
    for (i = 0; i < (IRQ_NUMBER_MAX/32); i++) {
        NVIC->ICPR[i] = 0xffffffff;
    }

    for (j = 0; j < (IRQ_NUMBER_MAX%32); j++) {
        temp |= 1<<j;
    }

    NVIC->ICPR[i+1] = temp;
}

hal_nvic_status_t nvic_irq_software_trigger(hal_nvic_irq_t irq_number)
{
    if (irq_number < 0 || irq_number >= IRQ_NUMBER_MAX) {
        return HAL_NVIC_STATUS_INVALID_PARAMETER;
    }

    NVIC->STIR = ( irq_number << NVIC_STIR_INTID_Pos ) & NVIC_STIR_INTID_Msk;
    __DSB();
    
    return HAL_NVIC_STATUS_OK;
}

/**
 * @brief This function is used to return the CM4 status.
 * @return    To indicate whether this function call is successful.
 *            If the return value is not zero, the CM4 is executing excetpion handler;
 *            If the return value is zero, the CM4 is executing normal code.
 */
uint32_t hal_nvic_query_exception_number(void)
{
    return ((SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) >> SCB_ICSR_VECTACTIVE_Pos);
}


/**
 * @brief This function saves the current IRQ settings in an variable, and then disables the IRQ by setting the IRQ mask.
 *  It should be used in conjunction with #nvic_restore_all_irq() to protect the critical resources.
 *  This function is defined only for MD and OS!!!
 * @param[out] mask is used to store the current IRQ setting, upon the return of this function.
 * @return    IRQ settings.
 */
ATTR_TEXT_IN_TCM uint32_t save_and_set_interrupt_mask(void)
{
#ifdef NVIC_USE_BASEPRI
    uint32_t mask = __get_BASEPRI();
    __set_BASEPRI(NVIC_DISABLE_LEVEL);
    nvic_nest_level++;
#else
    uint32_t mask = __get_PRIMASK();
    __disable_irq();
#endif    
#ifdef HAL_TIME_CHECK_DISABLE_IRQ_ENABLED
if(mask == 0)
    hal_time_check_disable_irq_start();
#endif
    return mask;
}


/**
 * @brief This function restores the IRQ settings as specified in the mask. It should be used in conjunction with
 *  #nvic_save_and_set_all_irq() to protect critical resources.
 *  This function is defined only for MD and OS!!!
 * @param[in] mask is an unsigned integer to specify the IRQ settings.
 * @return    N/A.
 */
ATTR_TEXT_IN_TCM void restore_interrupt_mask(uint32_t mask)
{
#ifdef HAL_TIME_CHECK_DISABLE_IRQ_ENABLED

#if 0
#if defined(__GNUC__)
    uint32_t xLinkRegAddr0 = (uint32_t)__builtin_return_address(0);
    printf("restore_interrupt_mask calluser:0x%x\r\n",xLinkRegAddr0);
#endif
#endif

    if(mask == 0) {
        hal_time_check_disable_irq_end(TIME_CHECK_DISABLE_IRQ_TIME);
    } 
#endif
#ifdef NVIC_USE_BASEPRI
    nvic_nest_level--;
    __set_BASEPRI(mask);
#else
    __set_PRIMASK(mask);
#endif
}



#ifdef __cplusplus
}
#endif

#endif /* HAL_NVIC_MODULE_ENABLED */


