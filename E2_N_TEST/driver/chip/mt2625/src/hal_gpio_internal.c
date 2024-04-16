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
#include "hal_log.h"

void gpio_get_state(hal_gpio_pin_t gpio_pin, gpio_state_t* gpio_state)
{

    uint32_t mode;
    uint32_t dir;
    uint32_t din;
    uint32_t dout;
    uint32_t pu;
    uint32_t pd;
    uint32_t pupd;
    uint32_t r0;
    uint32_t r1;

    gpio_pull_type_t pull_type;
    uint32_t temp;
    uint32_t shift;
    uint32_t reg_flag;
    uint32_t reg_index;
    uint32_t bit_index;
    uint8_t tdsel;
    hal_gpio_driving_current_t driving_value;

    //const char *direct[2] = {"input", "output"}; 
    //const char *pull_state[8] = {"disable_pull", "PU_75k", "PD_75k", "PU_47K","PD_47K", "PU_23.5K", "PD_23.5K", "PUPD_Error"};

    reg_index = gpio_pin/8;
    bit_index = (gpio_pin%8)*4;
    mode = (gpio_base->GPIO_MODE.RW[reg_index]>>(bit_index)&0xf);

    reg_index = gpio_pin/32;
    bit_index = gpio_pin%32;    
    dir  = (gpio_base->GPIO_DIR.RW[reg_index]>>(bit_index)&0x1);
    din  = (gpio_base->GPIO_DIN.R[reg_index]>>(bit_index)&0x1);
    dout = (gpio_base->GPIO_DOUT.RW[reg_index]>>(bit_index)&0x1);

    pu = 0xf;
    pd = 0xf;
    pupd = 0xf;
    r0   = 0xf;
    r1   = 0xf;

    shift = 0xff;
    reg_flag  = 0xff;
    
    if (gpio_pin == (hal_gpio_pin_t)2) {
        pu = (gpio_cfg0->GPIO_PU.RW >> 0)&0x1;
        pd = (gpio_cfg0->GPIO_PD.RW >> 0)&0x1;
    }
    else  if(gpio_pin == (hal_gpio_pin_t)5){
        pu = (gpio_cfg0->GPIO_PU.RW >> 1)&0x1;
        pd = (gpio_cfg0->GPIO_PD.RW >> 1)&0x1;
    }   
    else if ((gpio_pin <= (hal_gpio_pin_t)29) && (gpio_pin >= (hal_gpio_pin_t)24)) {
        pu = (gpio_cfg0->GPIO_PU.RW >> (gpio_pin - 22))&0x1;
        pd = (gpio_cfg0->GPIO_PD.RW >> (gpio_pin - 22))&0x1;   
    }
    else if ((gpio_pin >= (hal_gpio_pin_t)30)&&(gpio_pin <= 34)) {
        pu = (gpio_cfg1->GPIO_PU.RW >> (gpio_pin - 30))&0x1;
        pd = (gpio_cfg1->GPIO_PD.RW >> (gpio_pin - 30))&0x1;  
    }
    else if ((gpio_pin >= (hal_gpio_pin_t)0) && (gpio_pin <= (hal_gpio_pin_t)1)) {
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
        log_hal_info("get_gpio_state: input error pin number\r\n");
    }

    if (reg_flag == 1) {
        pupd = (gpio_cfg0->GPIO_PUPD.RW >> shift)&0x1;
        r0   = (gpio_cfg0->GPIO_R0.RW >> shift)&0x1;
        r1   = (gpio_cfg0->GPIO_R1.RW >> shift)&0x1;
    }
    else if (reg_flag == 2) {
        pupd = (gpio_cfg1->GPIO_PUPD.RW >> shift)&0x1;
        r0   = (gpio_cfg1->GPIO_R0.RW >> shift)&0x1;
        r1   = (gpio_cfg1->GPIO_R1.RW >> shift)&0x1;
    }

    pull_type = 0;

    temp = (pu<<4) + pd;

    //printf("pu=%d pd=%d, temp=%.3x\r\n",pu,pd,temp);
    
    if (temp == 0x00) {
        pull_type = GPIO_NO_PULL;
    }
    else if (temp == 0x10) {
        pull_type = GPIO_PU_75K;
    }
    else if (temp == 0x01) {
        pull_type = GPIO_PD_75K;
    }
    else if(temp!=0xff) {
        pull_type = GPIO_PUPD_ERR;
        log_hal_info("error pu = %x, pd= %x\r\n",pu,pd);
    }

    temp = (pupd<<8) + (r0<<4) + r1;
    //printf("pupd=%d r0=%d, r1=%d, temp=%.3x\r\n",pupd,r0,r1,temp);
                                 
    if ((temp == 0x001) || (temp == 0x010)) {
        pull_type = GPIO_PU_47K;
    }
    else if ((temp == 0x101) || (temp == 0x110)) {
        pull_type = GPIO_PD_47K;
    }
    else if (temp == 0x011) {
        pull_type = GPIO_PU_23_5K;
    }
    else if (temp == 0x111) {
        pull_type = GPIO_PD_23_5K;
    }
    else if ((temp == 0x100) || (temp == 0x000)) {
        pull_type = GPIO_NO_PULL;
    }
    else if (temp != 0xfff){
        pull_type = GPIO_PUPD_ERR;
        log_hal_info("error pupd-r0-r1 = %x\r\n",temp);
    }

    if (gpio_pin<= 29) {
        temp  = gpio_cfg0->GPIO_TDSEL.RW[gpio_pin/8];
        shift = (gpio_pin%8)*4;
        tdsel = (temp>>shift)*0xf;
    }
    else if ((gpio_pin>=30) && (gpio_pin<=36)) {
        
        temp  = gpio_cfg1->GPIO_TDSEL.RW[0];
        shift = (gpio_pin - 30)*4;
        tdsel = (temp>>shift)*0xf;
    }
    else {
        tdsel = 0xff;
    }

    hal_gpio_get_driving_current((hal_gpio_pin_t)gpio_pin,&driving_value);
        
    gpio_state->mode = mode;
    gpio_state->dir  = dir;
    gpio_state->din  = din;
    gpio_state->dout = dout;
    gpio_state->pull_type = pull_type;
    gpio_state->current_type = (uint8_t)driving_value;
    gpio_state->tdsel = tdsel;

    //printf("LOG%d: GPIO%d, mode=%d, %s, din=%d, dout=%d, %s\r\n",index, gpio_pin, mode, direct[dir], din,dout,pull_state[pull_type]);
        
   // dvt_eint_pirntf("%d-GPIO MODE0      = 0x%x\r\n", index, REG32(GPIO_MODE0));
   // dvt_eint_pirntf("%d-GPIO DOUT0      = 0x%x\r\n", index, REG32(GPIO_DOUT0));
   // dvt_eint_pirntf("%d-GPIO PUPD_CFG0  = 0x%x\r\n", index, REG32(PUPD_CFG0));
   // dvt_eint_pirntf("%d-GPIO R0_CFG0    = 0x%x\r\n", index, REG32(R0_CFG0));
   // dvt_eint_pirntf("%d-GPIO R1_CFG0    = 0x%x\r\n", index, REG32(R1_CFG0));
   // dvt_eint_pirntf("\r\n");

}

#endif

