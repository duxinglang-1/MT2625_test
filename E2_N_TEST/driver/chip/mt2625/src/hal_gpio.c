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

#include "hal_gpio.h"


#ifdef HAL_GPIO_MODULE_ENABLED
#include "hal_gpio_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t save_and_set_interrupt_mask(void);
extern void restore_interrupt_mask(uint32_t mask);

GPIO_BASE_REGISTER_T *gpio_base = (GPIO_BASE_REGISTER_T *)(GPIO_BASE);
GPIO_CFG0_REGISTER_T *gpio_cfg0 = (GPIO_CFG0_REGISTER_T *)(IO_CFG_0_BASE);
GPIO_CFG1_REGISTER_T *gpio_cfg1 = (GPIO_CFG1_REGISTER_T *)(IO_CFG_1_BASE);
TOP_MISC_CFG_T *clk_out_contorl = TOP_MISC_CFG;

/* check if the pin has only one pull-up resister and one pull-down resister*/
/* pin with pullsel means the pin has only one pull-up resister and one pull-down resister*/
bool is_pin_with_pullsel(hal_gpio_pin_t gpio_pin)
{
    /* pin number which has only one pull-up resister and one pull-down resister, it is different on different chips */
    if ((gpio_pin >= HAL_GPIO_24) &&(gpio_pin <= (hal_gpio_pin_t)34)) {
        return true;
    }
    
    if ((gpio_pin == (hal_gpio_pin_t)2) || (gpio_pin == (hal_gpio_pin_t)5)) {
        return true;
    }

    
    return false;
}

bool is_pin_in_cfg0(hal_gpio_pin_t gpio_pin)
{
    if (gpio_pin <= (hal_gpio_pin_t)29) {
       return true;
    }

    return false;
}

bool is_pin_in_cfg1(hal_gpio_pin_t gpio_pin)
{
    if ((gpio_pin >= HAL_GPIO_30) && (HAL_GPIO_0 <= 36)) {
       return true;
    }

    return false;
}




hal_gpio_status_t hal_gpio_init(hal_gpio_pin_t gpio_pin)
{
    return HAL_GPIO_STATUS_OK;
}


hal_gpio_status_t hal_gpio_deinit(hal_gpio_pin_t gpio_pin)
{
    return HAL_GPIO_STATUS_OK;
}



hal_gpio_status_t hal_gpio_set_direction(hal_gpio_pin_t gpio_pin, hal_gpio_direction_t gpio_direction)
{
    uint32_t reg_num;
    uint32_t index_num;
    uint32_t irq_status;
        
    if (gpio_pin >= HAL_GPIO_MAX) {
        return HAL_GPIO_STATUS_ERROR_PIN;
    }

    irq_status = save_and_set_interrupt_mask();
    reg_num   = gpio_pin/GPIO_DIR_REG_CTRL_PIN_NUM;
    index_num = gpio_pin%GPIO_DIR_REG_CTRL_PIN_NUM;
    
    if (gpio_direction == HAL_GPIO_DIRECTION_INPUT) {
        gpio_base->GPIO_DIR.CLR[reg_num] = (GPIO_REG_ONE_BIT_SET_CLR << index_num);
    } else {
        gpio_base->GPIO_DIR.SET[reg_num] = (GPIO_REG_ONE_BIT_SET_CLR << index_num);
    }
    restore_interrupt_mask(irq_status);
    
    return HAL_GPIO_STATUS_OK;

}

hal_gpio_status_t hal_gpio_get_direction(hal_gpio_pin_t gpio_pin, hal_gpio_direction_t *gpio_direction)
{
    uint32_t reg_num;
    uint32_t index_num;
    uint32_t irq_status;
    
    if (gpio_pin >= HAL_GPIO_MAX) {
        return HAL_GPIO_STATUS_ERROR_PIN;
    }

    if (NULL == gpio_direction) {
        return HAL_GPIO_STATUS_INVALID_PARAMETER;
    }

    irq_status = save_and_set_interrupt_mask();
    reg_num   = gpio_pin/GPIO_DIR_REG_CTRL_PIN_NUM;
    index_num = gpio_pin%GPIO_DIR_REG_CTRL_PIN_NUM;

    if (gpio_base->GPIO_DIR.RW[reg_num] &(GPIO_REG_ONE_BIT_SET_CLR<<index_num)) {
        *gpio_direction = HAL_GPIO_DIRECTION_OUTPUT;
    }
    else {
        *gpio_direction = HAL_GPIO_DIRECTION_INPUT;
    }
    restore_interrupt_mask(irq_status);
    
    return HAL_GPIO_STATUS_OK;

}



hal_pinmux_status_t hal_pinmux_set_function(hal_gpio_pin_t gpio_pin, uint8_t function_index)
{
    uint32_t no;
    uint32_t remainder;
    uint32_t irq_status;
    uint32_t temp;

    if (gpio_pin >= HAL_GPIO_MAX) {
        return HAL_PINMUX_STATUS_ERROR_PORT;
    }

    /* check whether the function index is right as one function is corresponding to 4 bits of oen pin */
    if (function_index >= GPIO_MODE_MAX_NUMBER) {
        return HAL_PINMUX_STATUS_INVALID_FUNCTION;
    }

    /* protect the configuration to prevent possible interrupt */
    irq_status = save_and_set_interrupt_mask();
        
    /* get the register number corresponding to the pin as one register can control 8 pins*/
    no = gpio_pin / GPIO_MODE_REG_CTRL_PIN_NUM;

    /* get the bit offset within the register as one register can control 8 pins*/
    remainder = gpio_pin % GPIO_MODE_REG_CTRL_PIN_NUM;

    temp = gpio_base->GPIO_MODE.RW[no];
    temp &= ~(GPIO_REG_FOUR_BIT_SET_CLR << (remainder * GPIO_MODE_FUNCTION_CTRL_BITS));
    temp |= (function_index << (remainder * GPIO_MODE_FUNCTION_CTRL_BITS));
    gpio_base->GPIO_MODE.RW[no] = temp;

    if ((gpio_pin>=30) && (gpio_pin<=34)) {
        if (function_index == 5) {
            /* set analog pin*/
            gpio_cfg1->GPIO_G.CLR = (1<<(gpio_pin-30));
        }
        else {
            /* set digital pin*/
            gpio_cfg1->GPIO_G.SET = (1<<(gpio_pin-30));
        }
    }
    
    restore_interrupt_mask(irq_status);
    
    return HAL_PINMUX_STATUS_OK;

}



hal_gpio_status_t hal_gpio_get_input(hal_gpio_pin_t gpio_pin, hal_gpio_data_t *gpio_data)
{
    uint32_t reg_num;
    uint32_t index_num;
    uint32_t irq_status;
    
    if (gpio_pin >= HAL_GPIO_MAX) {
        return HAL_GPIO_STATUS_ERROR_PIN;
    }

    if (NULL == gpio_data) {
        return HAL_GPIO_STATUS_INVALID_PARAMETER;
    }

    irq_status = save_and_set_interrupt_mask();
    reg_num   = gpio_pin/GPIO_DIN_REG_CTRL_PIN_NUM;
    index_num = gpio_pin%GPIO_DIN_REG_CTRL_PIN_NUM;
    
    if (gpio_base->GPIO_DIN.R[reg_num] & (GPIO_REG_ONE_BIT_SET_CLR<<index_num)) {
        *gpio_data = HAL_GPIO_DATA_HIGH;
    }
    else {
        *gpio_data = HAL_GPIO_DATA_LOW;
    }
    restore_interrupt_mask(irq_status);
    
    return HAL_GPIO_STATUS_OK;

}



hal_gpio_status_t hal_gpio_set_output(hal_gpio_pin_t gpio_pin, hal_gpio_data_t gpio_data)
{
    uint32_t reg_num;
    uint32_t index_num;
    uint32_t irq_status;
    
    if (gpio_pin >= HAL_GPIO_MAX) {
        return HAL_GPIO_STATUS_ERROR_PIN;
    }

    irq_status = save_and_set_interrupt_mask();
    reg_num   = gpio_pin/GPIO_DOUT_REG_CTRL_PIN_NUM;
    index_num = gpio_pin%GPIO_DOUT_REG_CTRL_PIN_NUM;
    
    if (gpio_data) {
        gpio_base->GPIO_DOUT.SET[reg_num] = (GPIO_REG_ONE_BIT_SET_CLR << index_num);
    } else {
        gpio_base->GPIO_DOUT.CLR[reg_num] = (GPIO_REG_ONE_BIT_SET_CLR << index_num);
    }
    restore_interrupt_mask(irq_status);
    
    return HAL_GPIO_STATUS_OK;
}



hal_gpio_status_t hal_gpio_get_output(hal_gpio_pin_t gpio_pin, hal_gpio_data_t *gpio_data)
{
    uint32_t reg_num;
    uint32_t index_num;
    uint32_t irq_status;
    
    if (gpio_pin >= HAL_GPIO_MAX) {
        return HAL_GPIO_STATUS_ERROR_PIN;
    }

    if (NULL == gpio_data) {
        return HAL_GPIO_STATUS_INVALID_PARAMETER;
    }

    irq_status = save_and_set_interrupt_mask();
    reg_num   = gpio_pin/GPIO_DOUT_REG_CTRL_PIN_NUM;
    index_num = gpio_pin%GPIO_DOUT_REG_CTRL_PIN_NUM;
    
    if (gpio_base->GPIO_DOUT.RW[reg_num] & (GPIO_REG_ONE_BIT_SET_CLR << index_num)) {
        *gpio_data = HAL_GPIO_DATA_HIGH;
    }
    else {
        *gpio_data = HAL_GPIO_DATA_LOW;
    }
    restore_interrupt_mask(irq_status);
        
    return HAL_GPIO_STATUS_OK;

}



hal_gpio_status_t hal_gpio_toggle_pin(hal_gpio_pin_t gpio_pin)
{

    uint32_t reg_num = 0;
    uint32_t index_num = 0;
    uint32_t irq_status = 0;
    
    if (gpio_pin >= HAL_GPIO_MAX) {
        return HAL_GPIO_STATUS_ERROR_PIN;
    }

    irq_status = save_and_set_interrupt_mask();
    reg_num   = gpio_pin/GPIO_DOUT_REG_CTRL_PIN_NUM;
    index_num = gpio_pin%GPIO_DOUT_REG_CTRL_PIN_NUM;
        
    if (gpio_base->GPIO_DOUT.RW[reg_num] & (GPIO_REG_ONE_BIT_SET_CLR << index_num)) {
        
        gpio_base->GPIO_DOUT.CLR[reg_num] = (GPIO_REG_ONE_BIT_SET_CLR << index_num);
    } else {
        gpio_base->GPIO_DOUT.SET[reg_num] = (GPIO_REG_ONE_BIT_SET_CLR << index_num);
    }
    restore_interrupt_mask(irq_status);
    
    return HAL_GPIO_STATUS_OK;
}

hal_gpio_status_t hal_gpio_pull_up(hal_gpio_pin_t gpio_pin) 
{
    uint32_t irq_status;
        
    if (gpio_pin >= HAL_GPIO_MAX) {
        return HAL_GPIO_STATUS_ERROR_PIN;
    }

    irq_status = save_and_set_interrupt_mask();

    if (is_pin_with_pullsel(gpio_pin)) {
        if (is_pin_in_cfg0(gpio_pin) == true) {

            if (gpio_pin == (hal_gpio_pin_t)2) {
                gpio_cfg0->GPIO_PD.CLR = (GPIO_REG_ONE_BIT_SET_CLR << 0);
                gpio_cfg0->GPIO_PU.SET = (GPIO_REG_ONE_BIT_SET_CLR << 0);
            }
            else  if(gpio_pin == (hal_gpio_pin_t)5){
                gpio_cfg0->GPIO_PD.CLR = (GPIO_REG_ONE_BIT_SET_CLR << 1);
                gpio_cfg0->GPIO_PU.SET = (GPIO_REG_ONE_BIT_SET_CLR << 1);
            }
                    
            if ((gpio_pin <= (hal_gpio_pin_t)29) && (gpio_pin >= (hal_gpio_pin_t)24)) {
                gpio_cfg0->GPIO_PD.CLR = (GPIO_REG_ONE_BIT_SET_CLR << (gpio_pin - 22));
                gpio_cfg0->GPIO_PU.SET = (GPIO_REG_ONE_BIT_SET_CLR << (gpio_pin - 22));    
            }
        } 
        else if (is_pin_in_cfg1(gpio_pin)  == true) {
            if ((gpio_pin >= (hal_gpio_pin_t)30)&&(gpio_pin <= 34)) {
                gpio_cfg1->GPIO_PD.CLR = (GPIO_REG_ONE_BIT_SET_CLR <<(gpio_pin - HAL_GPIO_30));
                gpio_cfg1->GPIO_PU.SET = (GPIO_REG_ONE_BIT_SET_CLR <<(gpio_pin - HAL_GPIO_30)); 
            }
        }
        else {
            restore_interrupt_mask(irq_status);
            return HAL_GPIO_STATUS_INVALID_PARAMETER;  
        }
    }
    else {
        #ifdef HAL_GPIO_FEATURE_PUPD
        /*pull up 47K*/
        hal_gpio_set_pupd_register(gpio_pin,0,1,0);
        #endif
    }
    
    restore_interrupt_mask(irq_status);
    
    return HAL_GPIO_STATUS_OK;
    
}



hal_gpio_status_t hal_gpio_pull_down(hal_gpio_pin_t gpio_pin)
{
    uint32_t irq_status;
    
    if (gpio_pin >= HAL_GPIO_MAX) {
        return HAL_GPIO_STATUS_ERROR_PIN;
    }

    irq_status = save_and_set_interrupt_mask();

    if (is_pin_with_pullsel(gpio_pin)) {
         if (is_pin_in_cfg0(gpio_pin) == true) {
            if (gpio_pin == (hal_gpio_pin_t)2) {
                gpio_cfg0->GPIO_PU.CLR = (GPIO_REG_ONE_BIT_SET_CLR << 0);
                gpio_cfg0->GPIO_PD.SET = (GPIO_REG_ONE_BIT_SET_CLR << 0);
            }
            else  if(gpio_pin == (hal_gpio_pin_t)5){
                gpio_cfg0->GPIO_PU.CLR = (GPIO_REG_ONE_BIT_SET_CLR << 1);
                gpio_cfg0->GPIO_PD.SET = (GPIO_REG_ONE_BIT_SET_CLR << 1);
            }
                    
            if ((gpio_pin <= (hal_gpio_pin_t)29) && (gpio_pin >= (hal_gpio_pin_t)24)) {
                gpio_cfg0->GPIO_PU.CLR = (GPIO_REG_ONE_BIT_SET_CLR << (gpio_pin - 22));
                gpio_cfg0->GPIO_PD.SET = (GPIO_REG_ONE_BIT_SET_CLR << (gpio_pin - 22));    
            }
        }
        else if (is_pin_in_cfg1(gpio_pin)  == true) {
            if ((gpio_pin >= (hal_gpio_pin_t)30)&&(gpio_pin <= 34)) {
                gpio_cfg1->GPIO_PU.CLR = (GPIO_REG_ONE_BIT_SET_CLR <<(gpio_pin - HAL_GPIO_30));
                gpio_cfg1->GPIO_PD.SET = (GPIO_REG_ONE_BIT_SET_CLR <<(gpio_pin - HAL_GPIO_30)); 
            }
        }
        else {
            restore_interrupt_mask(irq_status);
            return HAL_GPIO_STATUS_INVALID_PARAMETER;
        }
    }
    else{
        #ifdef HAL_GPIO_FEATURE_PUPD
        /*pull down 47k*/
        hal_gpio_set_pupd_register(gpio_pin,1,1,0);
        #endif
    }

    restore_interrupt_mask(irq_status);
    
    return HAL_GPIO_STATUS_OK;
    
}



hal_gpio_status_t hal_gpio_disable_pull(hal_gpio_pin_t gpio_pin)
{
    uint32_t irq_status;

    if (gpio_pin >= HAL_GPIO_MAX) {
        return HAL_GPIO_STATUS_ERROR_PIN;
    }

    irq_status = save_and_set_interrupt_mask();
    if (is_pin_with_pullsel(gpio_pin)) {
        if (is_pin_in_cfg0(gpio_pin) == true) {
            if (gpio_pin == (hal_gpio_pin_t)2) {
                gpio_cfg0->GPIO_PU.CLR = (GPIO_REG_ONE_BIT_SET_CLR << 0);
                gpio_cfg0->GPIO_PD.CLR = (GPIO_REG_ONE_BIT_SET_CLR << 0);
            }
            else  if(gpio_pin == (hal_gpio_pin_t)5){
                gpio_cfg0->GPIO_PU.CLR = (GPIO_REG_ONE_BIT_SET_CLR << 1);
                gpio_cfg0->GPIO_PD.CLR = (GPIO_REG_ONE_BIT_SET_CLR << 1);
            }
                    
            if ((gpio_pin <= (hal_gpio_pin_t)29) && (gpio_pin >= (hal_gpio_pin_t)24)) {
                gpio_cfg0->GPIO_PU.CLR = (GPIO_REG_ONE_BIT_SET_CLR << (gpio_pin - 22));
                gpio_cfg0->GPIO_PD.CLR = (GPIO_REG_ONE_BIT_SET_CLR << (gpio_pin - 22));    
            }
        }
        else if (is_pin_in_cfg1(gpio_pin)  == true) {
            if ((gpio_pin >= (hal_gpio_pin_t)30)&&(gpio_pin <= 34)) {
                gpio_cfg1->GPIO_PU.CLR = (GPIO_REG_ONE_BIT_SET_CLR <<(gpio_pin - HAL_GPIO_30));
                gpio_cfg1->GPIO_PD.CLR = (GPIO_REG_ONE_BIT_SET_CLR <<(gpio_pin - HAL_GPIO_30)); 
            }
        }
        else {
            restore_interrupt_mask(irq_status);
            return HAL_GPIO_STATUS_INVALID_PARAMETER;
        }
    }
    else {
        #ifdef HAL_GPIO_FEATURE_PUPD
        hal_gpio_set_pupd_register(gpio_pin,0,0,0);
        #endif
    }

    restore_interrupt_mask(irq_status);
    
    return HAL_GPIO_STATUS_OK;
}



#ifdef HAL_GPIO_FEATURE_CLOCKOUT
hal_gpio_status_t hal_gpio_set_clockout(hal_gpio_clock_t gpio_clock_num, hal_gpio_clock_mode_t clock_mode)
{
    uint32_t temp;

    if (gpio_clock_num >= HAL_GPIO_CLOCK_MAX) {
        return HAL_GPIO_STATUS_INVALID_PARAMETER;
    }

    if (gpio_clock_num < HAL_GPIO_CLOCK_4 ) {
        temp = clk_out_contorl->GPIO_CLKO_CTRL_A;
        temp &= ~(0xf<<gpio_clock_num);
        temp |= clock_mode<<gpio_clock_num;
        clk_out_contorl->GPIO_CLKO_CTRL_A = temp;
    }
    else {
        temp = clk_out_contorl->GPIO_CLKO_CTRL_B;
        temp &= ~(0xf<<(gpio_clock_num - 4));
        temp |= clock_mode<<gpio_clock_num;
        clk_out_contorl->GPIO_CLKO_CTRL_B = temp;
    }

    return HAL_GPIO_STATUS_OK;
}

#endif

#ifdef HAL_GPIO_FEATURE_SET_SCHMITT
hal_gpio_status_t hal_gpio_set_schmitt(hal_gpio_pin_t gpio_pin)
{
    if (gpio_pin >= HAL_GPIO_MAX) {
        return HAL_GPIO_STATUS_ERROR_PIN;
    }

    if ((gpio_pin <= (hal_gpio_pin_t)29)&&(gpio_pin >= 0)) {
        gpio_cfg0->GPIO_SMT.SET = (GPIO_REG_ONE_BIT_SET_CLR <<gpio_pin);
    }
    else if ((gpio_pin <= (hal_gpio_pin_t)36)&&(gpio_pin >= 30)) {
        gpio_cfg1->GPIO_SMT.SET = (GPIO_REG_ONE_BIT_SET_CLR <<(gpio_pin - 30));
    }
    else {
        return HAL_GPIO_STATUS_INVALID_PARAMETER;
    }
    
    return HAL_GPIO_STATUS_OK;
}

hal_gpio_status_t hal_gpio_clear_schmitt(hal_gpio_pin_t gpio_pin)
{
    if (gpio_pin >= HAL_GPIO_MAX) {
        return HAL_GPIO_STATUS_ERROR_PIN;
    }
      
    if ((gpio_pin <= (hal_gpio_pin_t)29)&&(gpio_pin >= 0)) {
        gpio_cfg0->GPIO_SMT.SET = (GPIO_REG_ONE_BIT_SET_CLR <<gpio_pin);
    }
    else if ((gpio_pin <= (hal_gpio_pin_t)36)&&(gpio_pin >= 30)) {
        gpio_cfg1->GPIO_SMT.SET = (GPIO_REG_ONE_BIT_SET_CLR <<(gpio_pin - 30));
    }
    else {
        return HAL_GPIO_STATUS_INVALID_PARAMETER;
    }
    
    return HAL_GPIO_STATUS_OK;
}
#endif


#ifdef HAL_GPIO_FEATURE_PUPD
hal_gpio_status_t hal_gpio_set_pupd_register(hal_gpio_pin_t gpio_pin, uint8_t gpio_pupd, uint8_t gpio_r0, uint8_t gpio_r1)
{
    uint32_t shift;
    uint32_t irq_status;
    uint32_t reg_flag = 0;

    if ((gpio_pin >= HAL_GPIO_MAX) || (is_pin_with_pullsel(gpio_pin))) {
        return HAL_GPIO_STATUS_ERROR_PIN;
    }

    irq_status = save_and_set_interrupt_mask();

    if ((gpio_pin >= (hal_gpio_pin_t)0) && (gpio_pin <= (hal_gpio_pin_t)1)) {

        shift = gpio_pin;
        reg_flag = 1;
    }
    else if ((gpio_pin >= (hal_gpio_pin_t)3) && (gpio_pin <= (hal_gpio_pin_t)4)) {
        shift = gpio_pin - 1;
        reg_flag = 1;
    }
    else if ((gpio_pin >= (hal_gpio_pin_t)6) && (gpio_pin <= (hal_gpio_pin_t)23)) {
        shift = gpio_pin - 2;
        reg_flag = 1;
    }
    else if ((gpio_pin >= (hal_gpio_pin_t)35) && (gpio_pin <= (hal_gpio_pin_t)36)) {
        shift = gpio_pin - 35;
        reg_flag = 2;
    }
    else {
        return HAL_GPIO_STATUS_INVALID_PARAMETER;
    }

    if (reg_flag == 1) {
        if (gpio_pupd) {
            gpio_cfg0->GPIO_PUPD.SET = (GPIO_REG_ONE_BIT_SET_CLR << shift);
        } else {
            gpio_cfg0->GPIO_PUPD.CLR = (GPIO_REG_ONE_BIT_SET_CLR << shift);
        }

        if (gpio_r0) {
            gpio_cfg0->GPIO_R0.SET = (GPIO_REG_ONE_BIT_SET_CLR << shift);
        } else {
            gpio_cfg0->GPIO_R0.CLR = (GPIO_REG_ONE_BIT_SET_CLR << shift);
        }

        if (gpio_r1) {
            gpio_cfg0->GPIO_R1.SET = (GPIO_REG_ONE_BIT_SET_CLR << shift);
        } else {
            gpio_cfg0->GPIO_R1.CLR = (GPIO_REG_ONE_BIT_SET_CLR << shift);
        }
    }
    else if (reg_flag == 2) {
        if (gpio_pupd) {
            gpio_cfg1->GPIO_PUPD.SET = (GPIO_REG_ONE_BIT_SET_CLR << shift);
        } else {
            gpio_cfg1->GPIO_PUPD.CLR = (GPIO_REG_ONE_BIT_SET_CLR << shift);
        }

        if (gpio_r0) {
            gpio_cfg1->GPIO_R0.SET = (GPIO_REG_ONE_BIT_SET_CLR << shift);
        } else {
            gpio_cfg1->GPIO_R0.CLR = (GPIO_REG_ONE_BIT_SET_CLR << shift);
        }

        if (gpio_r1) {
            gpio_cfg1->GPIO_R1.SET = (GPIO_REG_ONE_BIT_SET_CLR << shift);
        } else {
            gpio_cfg1->GPIO_R1.CLR = (GPIO_REG_ONE_BIT_SET_CLR << shift);
        }
    }

    restore_interrupt_mask(irq_status);
    
    return HAL_GPIO_STATUS_OK;
}
#endif

#ifdef HAL_GPIO_FEATURE_HIGH_Z
hal_gpio_status_t hal_gpio_set_high_impedance(hal_gpio_pin_t gpio_pin)
{

    hal_pinmux_status_t ret1;
    hal_gpio_status_t   ret2;
    
    if (gpio_pin >= HAL_GPIO_MAX) {
        return HAL_GPIO_STATUS_ERROR_PIN;
    }
    
    /* set GPIO mode of pin */
    ret1 = hal_pinmux_set_function(gpio_pin,0);
    if(ret1 != HAL_PINMUX_STATUS_OK) {
        return HAL_GPIO_STATUS_ERROR;
    }

    /* set input direction of pin */
    ret2 = hal_gpio_set_direction(gpio_pin,HAL_GPIO_DIRECTION_INPUT);
    if(ret2 != HAL_GPIO_STATUS_OK) {
        return ret2;
    }

    /* disable input buffer enable function of pin */
    if ((gpio_pin <= (hal_gpio_pin_t)29) && (gpio_pin >= (hal_gpio_pin_t)0)) {
        gpio_cfg0->GPIO_IES.CLR = (GPIO_REG_ONE_BIT_SET_CLR << gpio_pin);
    }
    
    if ((gpio_pin <= (hal_gpio_pin_t)36)&&(gpio_pin >= (hal_gpio_pin_t)30)) {
        gpio_cfg1->GPIO_IES.CLR = (GPIO_REG_ONE_BIT_SET_CLR <<(gpio_pin - 30));
    }

    /* disable pull function of pin */
    ret2 = hal_gpio_disable_pull(gpio_pin);
    if(ret2 != HAL_GPIO_STATUS_OK) {
        return ret2;
    }


    return HAL_GPIO_STATUS_OK;
}


hal_gpio_status_t hal_gpio_clear_high_impedance(hal_gpio_pin_t gpio_pin)
{
    hal_pinmux_status_t ret1;
    hal_gpio_status_t   ret2;
    
    if (gpio_pin >= HAL_GPIO_MAX) {
        return HAL_GPIO_STATUS_ERROR_PIN;
    }

    /* set GPIO mode of pin. */
    ret1 = hal_pinmux_set_function(gpio_pin,0);
    if(ret1 != HAL_PINMUX_STATUS_OK) {
        return HAL_GPIO_STATUS_ERROR;
    }

    /* set input direction of pin. */
    ret2 = hal_gpio_set_direction(gpio_pin,HAL_GPIO_DIRECTION_INPUT);
    if(ret2 != HAL_GPIO_STATUS_OK) {
        return ret2;
    }

    /* Enable input buffer enable function of pin */
    if ((gpio_pin <= (hal_gpio_pin_t)29) && (gpio_pin >= (hal_gpio_pin_t)0)) {
        gpio_cfg0->GPIO_IES.SET = (GPIO_REG_ONE_BIT_SET_CLR << gpio_pin);
    }
    
    if ((gpio_pin <= (hal_gpio_pin_t)36)&&(gpio_pin >= (hal_gpio_pin_t)30)) {
        gpio_cfg1->GPIO_IES.SET = (GPIO_REG_ONE_BIT_SET_CLR <<(gpio_pin - 30));
    }

    /* enable pull down of pin. */
    if (is_pin_with_pullsel(gpio_pin)) {
        ret2 = hal_gpio_pull_down(gpio_pin);
        if(ret2 != HAL_GPIO_STATUS_OK) {
            return ret2;
        }
    }
    #ifdef HAL_GPIO_FEATURE_PUPD
    else {
        ret2 = hal_gpio_set_pupd_register(gpio_pin,1,1,1);
        if(ret2 != HAL_GPIO_STATUS_OK) {
            return ret2;
        }

    }
    #endif

    return HAL_GPIO_STATUS_OK;
}
#endif

#ifdef HAL_GPIO_FEATURE_SET_DRIVING
hal_gpio_status_t hal_gpio_set_driving_current(hal_gpio_pin_t gpio_pin, hal_gpio_driving_current_t driving)
{
    uint32_t irq_status;
    uint32_t temp;
    
    if (gpio_pin >= HAL_GPIO_MAX) {
        return HAL_GPIO_STATUS_ERROR_PIN;
    }
    
    irq_status = save_and_set_interrupt_mask();
    
    if (gpio_pin <= (hal_gpio_pin_t)15) {
        temp = gpio_cfg0->GPIO_DRV.RW[0];
        temp &= ~(GPIO_REG_TWO_BIT_SET_CLR << (gpio_pin*2));
        gpio_cfg0->GPIO_DRV.RW[0] = (driving << (gpio_pin*2)) | temp;
    }
    else if ((gpio_pin <= (hal_gpio_pin_t)29)&&(gpio_pin >= 16)) {
        temp = gpio_cfg0->GPIO_DRV.RW[1];
        temp &= ~(GPIO_REG_TWO_BIT_SET_CLR <<((gpio_pin - 16)*2));
        gpio_cfg0->GPIO_DRV.RW[1] = (driving <<((gpio_pin - 16)*2)) | temp;    
    }
    else if ((gpio_pin <= (hal_gpio_pin_t)36)&&(gpio_pin >= 30)) {
        temp = gpio_cfg1->GPIO_DRV.RW[0];
        temp &= ~(GPIO_REG_TWO_BIT_SET_CLR <<((gpio_pin - 30)*2));
        gpio_cfg1->GPIO_DRV.RW[0] = (driving <<((gpio_pin - 30)*2)) | temp;    
    }

    restore_interrupt_mask(irq_status);
    
    return HAL_GPIO_STATUS_OK;
}


hal_gpio_status_t hal_gpio_get_driving_current(hal_gpio_pin_t gpio_pin, hal_gpio_driving_current_t *driving)
{
    uint32_t irq_status;
    uint32_t temp = 0;
    uint32_t shift;
    
    if (gpio_pin >= HAL_GPIO_MAX) {
        return HAL_GPIO_STATUS_ERROR_PIN;
    }
    
    irq_status = save_and_set_interrupt_mask();
    
    if (gpio_pin <= (hal_gpio_pin_t)15) {
        shift = (gpio_pin*2);
        temp = (gpio_cfg0->GPIO_DRV.RW[0] >> shift) & GPIO_REG_TWO_BIT_SET_CLR;
    }
    else if ((gpio_pin <= (hal_gpio_pin_t)29)&&(gpio_pin >= 16)) {
        shift = ((gpio_pin - 16)*2);   
        temp = (gpio_cfg0->GPIO_DRV.RW[1] >> shift) & GPIO_REG_TWO_BIT_SET_CLR;
    }
    else if ((gpio_pin <= (hal_gpio_pin_t)36)&&(gpio_pin >= 30)) {
        shift = ((gpio_pin - 30)*2);   
        temp = (gpio_cfg1->GPIO_DRV.RW[0] >> shift) & GPIO_REG_TWO_BIT_SET_CLR;
    }
    else {
        return HAL_GPIO_STATUS_INVALID_PARAMETER;
    }

    *driving = (hal_gpio_driving_current_t)(temp);

    restore_interrupt_mask(irq_status);
    
    return HAL_GPIO_STATUS_OK;
    
}
#endif

#ifdef __cplusplus
}
#endif

#endif  /* HAL_GPIO_MODULE_ENABLED */

