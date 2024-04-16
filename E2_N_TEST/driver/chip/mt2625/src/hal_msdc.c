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

#include "hal.h"
#include "hal_msdc.h"
#include "hal_gpio.h"
#if defined(HAL_SD_MODULE_ENABLED) || defined(HAL_SDIO_MODULE_ENABLED)
#include "hal_log.h"
#include "hal_clock_internal.h"
#include "hal_gpio_internal.h"
#include "hal_eint.h"
#include "assert.h"
#include "msdc_custom_config.h"
#include "hal_sd_internal.h"
#include "hal_sdio_internal.h"
#include "hal_platform.h"

msdc_config_t msdc_config[MSDC_PORT_MAX];
extern volatile uint32_t msdc_interrupt_status;

/*ept tool config*/
extern const char HAL_MSDC_0_CK_PIN;
extern const char HAL_MSDC_0_CM_PIN;
extern const char HAL_MSDC_0_DA0_PIN;
extern const char HAL_MSDC_0_DA1_PIN;
extern const char HAL_MSDC_0_DA2_PIN;
extern const char HAL_MSDC_0_DA3_PIN;

extern const char HAL_MSDC_1_CK_PIN;
extern const char HAL_MSDC_1_CM_PIN;
extern const char HAL_MSDC_1_DA0_PIN;
extern const char HAL_MSDC_1_DA1_PIN;
extern const char HAL_MSDC_1_DA2_PIN;
extern const char HAL_MSDC_1_DA3_PIN;

#if defined(HAL_SD_MODULE_ENABLED) && defined(HAL_SD_CARD_DETECTION)
static volatile bool msdc_card_detection_eint_polarity;
extern const unsigned char HAL_MSDC_EINT;  /*ept tool config*/
extern const unsigned char HAL_MSDC1_EINT;  /*ept tool config*/
#endif

void msdc_wait(uint32_t wait_ms)
{
    hal_gpt_delay_ms(wait_ms);
}

void msdc_power_set(msdc_port_t msdc_port,bool is_power_on)
{
    hal_clock_cg_id clk_id;

    clk_id = (MSDC_PORT_0 == msdc_port) ? HAL_CLOCK_CG_SDIOMST0_BUS : HAL_CLOCK_CG_SDIOMST1_BUS;

    if (is_power_on) {
        hal_clock_enable(clk_id);
    } else {
        hal_clock_disable(clk_id);
    }
}

void msdc_reset(msdc_port_t msdc_port)
{
    msdc_register_t *msdc_register_base;

    msdc_register_base = (MSDC_PORT_0 == msdc_port) ? MSDC0_REG : MSDC1_REG;

    if (!(msdc_register_base->MSDC_CFG & MSDC_CFG_RST)) {
        msdc_register_base->MSDC_CFG = msdc_register_base->MSDC_CFG | MSDC_CFG_RST;
    }

    while (msdc_register_base->MSDC_CFG & MSDC_CFG_RST);
}

void msdc_clear_fifo(msdc_port_t msdc_port)
{
    msdc_register_t *msdc_register_base;

    msdc_register_base = (MSDC_PORT_0 == msdc_port) ? MSDC0_REG : MSDC1_REG;
  
    msdc_register_base->MSDC_FIFOCS = msdc_register_base->MSDC_FIFOCS | MSDC_FIFOCS_CLR;

    while (msdc_register_base->MSDC_FIFOCS & MSDC_FIFOCS_CLR);
}

void msdc_set_bus_width(msdc_port_t msdc_port,msdc_bus_width_t bus_width)
{
    msdc_register_t *msdc_register_base;

    msdc_register_base = (MSDC_PORT_0 == msdc_port) ? MSDC0_REG : MSDC1_REG;

    msdc_register_base->SDC_CFG = (msdc_register_base->SDC_CFG & (~SDC_CFG_BUSWIDTH)) | (bus_width << SDC_CFG_BUSWIDTH_OFFSET);
}

msdc_status_t msdc_set_output_clock(msdc_port_t msdc_port,uint32_t clock)
{
    uint32_t clock_config = 0;
    msdc_register_t *msdc_register_base;
    hal_clock_cg_id clk_id;

    log_hal_info("msdc_set_output_clock++\r\n");

    msdc_register_base = (MSDC_PORT_0 == msdc_port) ? MSDC0_REG : MSDC1_REG;
    clk_id = (MSDC_PORT_0 == msdc_port) ? HAL_CLOCK_CG_SDIOMST0 : HAL_CLOCK_CG_SDIOMST1;

    /*disable msdc clock*/
    hal_clock_disable(clk_id);

    if (clock >= msdc_config[msdc_port].msdc_source_clock) {
        msdc_register_base->MSDC_CFG |= MSDC_CFG_CCKMD;
        msdc_config[msdc_port].output_clock = msdc_config[msdc_port].msdc_source_clock;
        log_hal_info("setting clock (%d) >= msdc_source_clock (%d), let output_clock=msdc_source_clock\r\n", clock, msdc_config[msdc_port].msdc_source_clock);
    } else if (clock >= (msdc_config[msdc_port].msdc_source_clock >> 1)) {
        msdc_register_base->MSDC_CFG &= ~MSDC_CFG_CCKMD;
        msdc_register_base->MSDC_CFG &= ~MSDC_CFG_CKDIV;
        msdc_config[msdc_port].output_clock = msdc_config[msdc_port].msdc_source_clock >> 1;
    } else {
        msdc_register_base->MSDC_CFG &= ~MSDC_CFG_CCKMD;
        clock_config = ((msdc_config[msdc_port].msdc_source_clock + clock - 1) / clock);
        clock_config = (clock_config >> 2) + (((clock_config & 3) != 0) ? 1 : 0);
        msdc_config[msdc_port].output_clock = msdc_config[msdc_port].msdc_source_clock / (4 * clock_config);

        msdc_register_base->MSDC_CFG = (msdc_register_base->MSDC_CFG & (~MSDC_CFG_CKDIV)) | ((clock_config & 0xff) << 8);
    }

    /*config cmd response sample edge.*/
    if (msdc_config[msdc_port].output_clock > SDIO_DEFAULT_MAX_SPEED) {
        msdc_register_base->MSDC_IOCON &= ~MSDC_IOCON_RSPL;
        msdc_register_base->MSDC_IOCON &= ~MSDC_IOCON_DSPL;
        msdc_register_base->MSDC_IOCON &= ~MSDC_IOCON_WDSPL;
    } else {
        msdc_register_base->MSDC_IOCON |= MSDC_IOCON_RSPL;
        msdc_register_base->MSDC_IOCON |= MSDC_IOCON_DSPL;
        msdc_register_base->MSDC_IOCON |= MSDC_IOCON_WDSPL;
    }

    log_hal_info("setting clock is %d, output_clock=%d, msdc_source_clock=%d, MSDC_CFG = %x \r\n", clock, msdc_config[msdc_port].output_clock, msdc_config[msdc_port].msdc_source_clock, msdc_register_base->MSDC_CFG);

    /*enable msdc clock*/
    hal_clock_enable(clk_id);

    uint32_t dowhile_break_count = 20;
    bool clock_stable = false;
    do {
        if (msdc_register_base->MSDC_CFG & MSDC_CFG_CCKSB){
            clock_stable = true;
            break;
        }

        msdc_wait(1);
    } while (dowhile_break_count--);

    if (clock_stable == true) {
        log_hal_info("msdc_set_output_clock--\r\n");
        return MSDC_OK;
    } else {
        log_hal_error("msdc clock not stable, msdc_register_base->MSDC_CFG=%x", msdc_register_base->MSDC_CFG);
        return MSDC_FAIL;
    }
}


uint32_t msdc_get_output_clock(msdc_port_t msdc_port)
{
    return (msdc_config[msdc_port].output_clock);
}


void msdc_sdio_interrupt_set(msdc_port_t msdc_port,bool enable)
{
    msdc_register_t *msdc_register_base;

    msdc_register_base = (MSDC_PORT_0 == msdc_port) ? MSDC0_REG : MSDC1_REG;

    if (enable) {
        msdc_register_base->MSDC_INTEN |= MSDC_INT_SDIOIRQ;
    } else {
        msdc_register_base->MSDC_INTEN &= ~MSDC_INT_SDIOIRQ;
    }
}

void msdc_data_interrupt_handle(msdc_port_t msdc_port,uint32_t status)
{
    // now, this function is just for dma thus we didn't check MSDC_INT_DXFER_DONE.
    if (MSDC_OWNER_SD == msdc_get_owner(msdc_port)) {
#if defined(HAL_SD_MODULE_ENABLED)
        if(status & MSDC_INT_XFER_COMPL) {
                sd_wait_dma_interrupt_transfer_ready((hal_sd_port_t)msdc_port);
        } else if ((status & MSDC_INT_DATTMO) || (status & MSDC_INT_DATCRCERR)) {
            if (msdc_config[msdc_port].msdc_sd_callback) {
                /*call user callback to inform transfer error*/
                if (status & MSDC_INT_DATTMO) {
                    msdc_config[msdc_port].msdc_sd_callback(HAL_SD_EVENT_DATA_TIMEOUT,(void *)0);
                } else if (status & MSDC_INT_DATCRCERR) {
                    msdc_config[msdc_port].msdc_sd_callback(HAL_SD_EVENT_CRC_ERROR,(void *)0);
                }
            }
        }
#endif
        return;
    }

    if (MSDC_OWNER_SDIO == msdc_get_owner(msdc_port)) {
#if defined(HAL_SDIO_MODULE_ENABLED)
        if(status & MSDC_INT_XFER_COMPL) {
            sdio_wait_dma_interrupt_transfer_ready((hal_sdio_port_t)msdc_port);
        } else if ((status & MSDC_INT_DATTMO) || (status & MSDC_INT_DATCRCERR)) {
            if (msdc_config[msdc_port].msdc_sdio_dma_interrupt_callback) {
                /*call user callback to inform transfer error*/
                if (status & MSDC_INT_DATCRCERR) {
                    msdc_config[msdc_port].msdc_sdio_dma_interrupt_callback(HAL_SDIO_EVENT_CRC_ERROR,(void *)0);
                } else if (status & MSDC_INT_DATTMO) {
                    msdc_config[msdc_port].msdc_sdio_dma_interrupt_callback(HAL_SDIO_EVENT_DATA_TIMEOUT,(void *)0);
                }
            }
        }
#endif
        return;
    }
}

void msdc_command_interrupt_handle(msdc_port_t msdc_port,uint32_t status)
{
    // we didn't handle CMDRDY & ACMDRDY to avoid too much interrupt
    if (MSDC_OWNER_SD == msdc_get_owner(msdc_port)) {
#if defined(HAL_SD_MODULE_ENABLED)
        if ((status & MSDC_INT_CMDTMO) || (status & MSDC_INT_RSPCRCERR) || (status & MSDC_INT_ACMDTMO) || (status & MSDC_INT_ACMDCRCERR)) {
            if (msdc_config[msdc_port].msdc_sd_callback) {
                // call user callback to inform transfer error
                if (status & MSDC_INT_CMDTMO) {
                    msdc_config[msdc_port].msdc_sd_callback(HAL_SD_EVENT_CMD_TIMEOUT, (void *)0);
                } else if (status & MSDC_INT_RSPCRCERR) {
                    msdc_config[msdc_port].msdc_sd_callback(HAL_SD_EVENT_RESPONSE_CRC_ERROR, (void *)0);
                } else if (status & MSDC_INT_ACMDTMO) {
                    msdc_config[msdc_port].msdc_sd_callback(HAL_SD_EVENT_CMD_TIMEOUT, (void *)0);
                } else if (status & MSDC_INT_ACMDCRCERR) {
                    msdc_config[msdc_port].msdc_sd_callback(HAL_SD_EVENT_RESPONSE_CRC_ERROR, (void *)0);
                }
            }
        }
#endif
        return;
    }

    // TODO: may add sdio part
}

void msdc_isr(hal_nvic_irq_t irq_number)
{
    uint32_t command_status  = MSDC_INT_CMDRDY | MSDC_INT_CMDTMO | MSDC_INT_RSPCRCERR;
    uint32_t acommand_status = MSDC_INT_ACMDRDY | MSDC_INT_ACMDTMO | MSDC_INT_ACMDCRCERR;
    uint32_t data_status  = MSDC_INT_XFER_COMPL | MSDC_INT_DXFER_DONE | MSDC_INT_DATCRCERR | MSDC_INT_DATTMO;   // MSDC_INT_XFER_COMPL for dma; MSDC_INT_DXFER_DONE for not dma mode
    uint32_t msdc_int;
    uint32_t msdc_inten;
    uint32_t interrupt_status;

    msdc_nvic_set(MSDC_PORT_0,false);

    msdc_int = MSDC0_REG->MSDC_INT;
    msdc_inten = MSDC0_REG->MSDC_INTEN;
    interrupt_status = msdc_int & msdc_inten;

    MSDC0_REG->MSDC_INT |= interrupt_status;

#if defined(HAL_SDIO_MODULE_ENABLED)
    msdc_interrupt_status = msdc_int;   // TODO:  for sdio none blocking dma mode. we may improve this.
#endif

    //log_hal_info("irq handler got msdc_int=%08X msdc_inten=%08X => %08X (in interrupt)\r\n", (unsigned int)msdc_int, (unsigned int)msdc_inten, (unsigned int)interrupt_status);

    /* transfer complete interrupt */
    if (interrupt_status & data_status) {
        msdc_data_interrupt_handle(MSDC_PORT_0, interrupt_status);
    }

    /* command interrupts */
    if (interrupt_status & (command_status | acommand_status)) {
        msdc_command_interrupt_handle(MSDC_PORT_0, interrupt_status);
    }

    /* sdio interrupt */
    if (interrupt_status & MSDC_INT_SDIOIRQ) {
        log_hal_info("SDIOIRQ interrupt\r\n");
    }

    msdc_nvic_set(MSDC_PORT_0, true);
}

void msdc1_isr(hal_nvic_irq_t irq_number)
{
    uint32_t command_status  = MSDC_INT_CMDRDY | MSDC_INT_CMDTMO | MSDC_INT_RSPCRCERR;
    uint32_t acommand_status = MSDC_INT_ACMDRDY | MSDC_INT_ACMDTMO | MSDC_INT_ACMDCRCERR;
    uint32_t data_status  = MSDC_INT_XFER_COMPL | MSDC_INT_DXFER_DONE | MSDC_INT_DATCRCERR | MSDC_INT_DATTMO;
    uint32_t msdc_int;
    uint32_t msdc_inten;
    uint32_t interrupt_status;

    msdc_nvic_set(MSDC_PORT_1,false);

    msdc_int = MSDC1_REG->MSDC_INT;
    msdc_inten = MSDC1_REG->MSDC_INTEN;
    interrupt_status = msdc_int & msdc_inten;

    MSDC1_REG->MSDC_INT |= interrupt_status;

#if defined(HAL_SDIO_MODULE_ENABLED)
    msdc_interrupt_status = msdc_int;
#endif

    //log_hal_info("irq handler got msdc_int=%08X msdc_inten=%08X => %08X (in interrupt)\r\n", (unsigned int)msdc_int, (unsigned int)msdc_inten, (unsigned int)interrupt_status);

    /* transfer complete interrupt */
    if (interrupt_status & data_status) {
        msdc_data_interrupt_handle(MSDC_PORT_1, interrupt_status);
    }

    /* command interrupts */
    if (interrupt_status & (command_status | acommand_status)) {
        msdc_command_interrupt_handle(MSDC_PORT_1, interrupt_status);
    }

    /* sdio interrupt */
    if (interrupt_status & MSDC_INT_SDIOIRQ) {
        log_hal_info("SDIOIRQ interrupt\r\n");
    }

    msdc_nvic_set(MSDC_PORT_1, true);
}


void msdc_nvic_set(msdc_port_t msdc_port,bool enable)
{
    hal_nvic_irq_t irq_number;

    irq_number = (MSDC_PORT_0 == msdc_port) ? SDIO_MST0_IRQn : SDIO_MST1_IRQn;

    /*should call nvic api to set msdc interrupt enable or disable*/
    if (enable) {
        hal_nvic_enable_irq(irq_number);
    } else {
        hal_nvic_disable_irq(irq_number);
    }
}

void msdc_interrupt_init(msdc_port_t msdc_port)
{
    uint32_t reg_value = 0;
    msdc_register_t *msdc_register_base;
    hal_nvic_irq_t irq_number;

    msdc_register_base = (MSDC_PORT_0 == msdc_port) ? MSDC0_REG : MSDC1_REG;
    irq_number = (MSDC_PORT_0 == msdc_port) ? SDIO_MST0_IRQn : SDIO_MST1_IRQn;

    msdc_nvic_set(msdc_port,false);

    if (MSDC_PORT_0 == msdc_port) {
        hal_nvic_register_isr_handler(irq_number, msdc_isr);
    }
    else if (MSDC_PORT_1 == msdc_port) {
        hal_nvic_register_isr_handler(irq_number, msdc1_isr);
    }

    /* Disable and clear all interrupts */
    reg_value = msdc_register_base->MSDC_INTEN;
    msdc_register_base->MSDC_INTEN &= ~reg_value;
    reg_value = msdc_register_base->MSDC_INT;
    msdc_register_base->MSDC_INT |= reg_value;

    msdc_nvic_set(msdc_port,true);
}

#if defined(HAL_SD_MODULE_ENABLED) && defined(HAL_SD_CARD_DETECTION)
void msdc_eint_isr(void *user_data)
{
    hal_eint_trigger_mode_t mode;
    hal_sd_card_event_t sd_event = HAL_SD_EVENT_CARD_INSERTED;

    hal_eint_mask((hal_eint_number_t)HAL_MSDC_EINT);

    msdc_config[MSDC_PORT_0].is_card_present = msdc_card_detection_eint_polarity ? false : true;

    msdc_card_detection_eint_polarity = msdc_card_detection_eint_polarity ? false : true;

    mode = msdc_card_detection_eint_polarity ? HAL_EINT_LEVEL_HIGH : HAL_EINT_LEVEL_LOW;
    hal_eint_set_trigger_mode((hal_eint_number_t)HAL_MSDC_EINT, mode);

    hal_eint_unmask((hal_eint_number_t)HAL_MSDC_EINT);

    if (msdc_config[MSDC_PORT_0].is_card_present == false) {
        msdc_config[MSDC_PORT_0].is_card_plug_out = true;
        sd_event = HAL_SD_EVENT_CARD_REMOVED;
        msdc_reset(MSDC_PORT_0);
        msdc_dma_disable(MSDC_PORT_0);
        msdc_deinit(MSDC_PORT_0);
    } else {
        msdc_config[MSDC_PORT_0].is_card_plug_out = false;
        sd_event = HAL_SD_EVENT_CARD_INSERTED;
    }

    if (msdc_config[MSDC_PORT_0].msdc_card_detect_callback != NULL) {
        msdc_config[MSDC_PORT_0].msdc_card_detect_callback(sd_event, msdc_config[MSDC_PORT_0].card_detect_user_data);
    }
}

void msdc1_eint_isr(void *user_data)
{
    hal_eint_trigger_mode_t mode;
    hal_sd_card_event_t sd_event = HAL_SD_EVENT_CARD_INSERTED;

    hal_eint_mask((hal_eint_number_t)HAL_MSDC_EINT);

    msdc_config[MSDC_PORT_1].is_card_present = msdc_card_detection_eint_polarity ? false : true;

    msdc_card_detection_eint_polarity = msdc_card_detection_eint_polarity ? false : true;

    mode = msdc_card_detection_eint_polarity ? HAL_EINT_LEVEL_HIGH : HAL_EINT_LEVEL_LOW;
    hal_eint_set_trigger_mode((hal_eint_number_t)HAL_MSDC_EINT, mode);

    hal_eint_unmask((hal_eint_number_t)HAL_MSDC_EINT);

    if (msdc_config[MSDC_PORT_1].is_card_present == false) {
        msdc_config[MSDC_PORT_1].is_card_plug_out = true;
        sd_event = HAL_SD_EVENT_CARD_REMOVED;
        msdc_reset(MSDC_PORT_1);
        msdc_dma_disable(MSDC_PORT_1);
        msdc_deinit(MSDC_PORT_1);
    } else {
        msdc_config[MSDC_PORT_1].is_card_plug_out = false;
        sd_event = HAL_SD_EVENT_CARD_INSERTED;
    }

    if (msdc_config[MSDC_PORT_1].msdc_card_detect_callback != NULL) {
        msdc_config[MSDC_PORT_1].msdc_card_detect_callback(sd_event, msdc_config[MSDC_PORT_1].card_detect_user_data);
    }
}

void msdc_eint_registration(msdc_port_t msdc_port)
{
    hal_eint_config_t config;
    unsigned char hal_msdc_eint;

    hal_msdc_eint = (MSDC_PORT_0 == msdc_port) ? HAL_MSDC_EINT : HAL_MSDC_EINT;

    /*HAL_MSDC_EINT is EPT tool config, the HAL_MSDC_EINT value is 0xff means the hot plug eint is not configured in EPT tool*/
    if (0xFF == hal_msdc_eint) {
        assert(0);
    }

    msdc_card_detection_eint_polarity = false;

    config.debounce_time = 500;
    config.trigger_mode = HAL_EINT_LEVEL_LOW;
    config.firq_enable = false;

    hal_eint_mask((hal_eint_number_t)hal_msdc_eint);

    if(HAL_EINT_STATUS_OK != hal_eint_init((hal_eint_number_t)hal_msdc_eint, &config))
    {
        log_hal_error("ERROR:hal_eint_init error!\r\n");
    }

    if(MSDC_PORT_0 == msdc_port) {
        if(HAL_EINT_STATUS_OK != hal_eint_register_callback ((hal_eint_number_t)hal_msdc_eint, msdc_eint_isr, NULL))
        {
            log_hal_error("ERROR:hal_eint_register_callback error!\r\n");
        }
    }
    else if(MSDC_PORT_1 == msdc_port) {
        if(HAL_EINT_STATUS_OK != hal_eint_register_callback ((hal_eint_number_t)hal_msdc_eint, msdc1_eint_isr, NULL))
        {
            log_hal_error("ERROR:hal_eint_register_callback error!\r\n");
        }
    }

    hal_eint_unmask((hal_eint_number_t)hal_msdc_eint);
}
#endif

/*TBD*/
void msdc_io_config(msdc_port_t msdc_port)
{
    if (MSDC_PORT_0 == msdc_port) {
        if (HAL_MSDC_0_CK_PIN != 0xFF) {
            hal_gpio_set_driving_current(HAL_MSDC_0_CK_PIN, (hal_gpio_driving_current_t)MSDC_DATA_LINE_DRIVING_CAPABILITY);
        }
        if (HAL_MSDC_0_CM_PIN != 0xFF) {
            hal_gpio_set_driving_current(HAL_MSDC_0_CM_PIN, (hal_gpio_driving_current_t)MSDC_DATA_LINE_DRIVING_CAPABILITY);
        }
        if (HAL_MSDC_0_DA0_PIN != 0xFF) {
            hal_gpio_set_driving_current(HAL_MSDC_0_DA0_PIN, (hal_gpio_driving_current_t)MSDC_DATA_LINE_DRIVING_CAPABILITY);
        }
        if (HAL_MSDC_0_DA1_PIN != 0xFF) {
            hal_gpio_set_driving_current(HAL_MSDC_0_DA1_PIN, (hal_gpio_driving_current_t)MSDC_DATA_LINE_DRIVING_CAPABILITY);
        }
        if (HAL_MSDC_0_DA2_PIN != 0xFF) {
            hal_gpio_set_driving_current(HAL_MSDC_0_DA2_PIN, (hal_gpio_driving_current_t)MSDC_DATA_LINE_DRIVING_CAPABILITY);
        }
        if (HAL_MSDC_0_DA3_PIN != 0xFF) {
            hal_gpio_set_driving_current(HAL_MSDC_0_DA3_PIN, (hal_gpio_driving_current_t)MSDC_DATA_LINE_DRIVING_CAPABILITY);
        }
    }
    else if (MSDC_PORT_1 == msdc_port) {
        if (HAL_MSDC_1_CK_PIN != 0xFF) {
            hal_gpio_set_driving_current(HAL_MSDC_1_CK_PIN, (hal_gpio_driving_current_t)MSDC_DATA_LINE_DRIVING_CAPABILITY);
        }
        if (HAL_MSDC_1_CM_PIN != 0xFF) {
            hal_gpio_set_driving_current(HAL_MSDC_1_CM_PIN, (hal_gpio_driving_current_t)MSDC_DATA_LINE_DRIVING_CAPABILITY);
        }
        if (HAL_MSDC_1_DA0_PIN != 0xFF) {
            hal_gpio_set_driving_current(HAL_MSDC_1_DA0_PIN, (hal_gpio_driving_current_t)MSDC_DATA_LINE_DRIVING_CAPABILITY);
        }
        if (HAL_MSDC_1_DA1_PIN != 0xFF) {
            hal_gpio_set_driving_current(HAL_MSDC_1_DA1_PIN, (hal_gpio_driving_current_t)MSDC_DATA_LINE_DRIVING_CAPABILITY);
        }
        if (HAL_MSDC_1_DA2_PIN != 0xFF) {
            hal_gpio_set_driving_current(HAL_MSDC_1_DA2_PIN, (hal_gpio_driving_current_t)MSDC_DATA_LINE_DRIVING_CAPABILITY);
        }
        if (HAL_MSDC_1_DA3_PIN != 0xFF) {
            hal_gpio_set_driving_current(HAL_MSDC_1_DA3_PIN, (hal_gpio_driving_current_t)MSDC_DATA_LINE_DRIVING_CAPABILITY);
        }
    }
    else {
        log_hal_error("msdc io config error\r\n");
    }
}

msdc_status_t msdc_init(msdc_port_t msdc_port)
{
    msdc_status_t status = MSDC_OK;
    uint32_t reg_value = 0;
    msdc_register_t *msdc_register_base;
    clock_mux_sel_id clk_mux_sel_id;

    msdc_register_base = (MSDC_PORT_0 == msdc_port) ? MSDC0_REG : MSDC1_REG;
    clk_mux_sel_id = (MSDC_PORT_0 == msdc_port) ? CLK_SDIOMST0_SEL : CLK_SDIOMST1_SEL;

    msdc_power_set(msdc_port,true);

    msdc_io_config(msdc_port);

    if (hal_dvfs_get_cpu_frequency() > 26000) {   // the PLL will be enable
        log_hal_info("cpu frequency > 26M, choose PLL 48M clock\r\n");
        clock_mux_sel(clk_mux_sel_id, 1); //48M   // chose PLL 48M clock
        msdc_config[msdc_port].msdc_source_clock = MSDC_CLOCK_SOURCE_48M;
        // TODO: the EMMC sepc can be 52M, thus we may add here in the feature
    } else {
        log_hal_info("cpu frequency <= 26M, choose XO_CK 26M clock\r\n");
        clock_mux_sel(clk_mux_sel_id, 0); //26M   // chose XO_CK 26M
        msdc_config[msdc_port].msdc_source_clock = MSDC_CLOCK_SOURCE_26M;
    }

    msdc_set_output_clock(msdc_port,240);

    /* Reset */
    msdc_reset(msdc_port);
    msdc_clear_fifo(msdc_port);

    /* Disable card detection */
    msdc_register_base->MSDC_PS = msdc_register_base->MSDC_PS & (~MSDC_PS_CDEN);

    /* Disable and clear all interrupts */
    reg_value = msdc_register_base->MSDC_INTEN;
    msdc_register_base->MSDC_INTEN &= ~reg_value;
    reg_value = msdc_register_base->MSDC_INT;
    msdc_register_base->MSDC_INT |= reg_value;

    msdc_interrupt_init(msdc_port);

    /* Configure to PIO mode */
    msdc_register_base->MSDC_CFG = msdc_register_base->MSDC_CFG | MSDC_CFG_PIO;

    /* Configure to MMC/SD mode */
    msdc_register_base->MSDC_CFG = msdc_register_base->MSDC_CFG | MSDC_CFG_MODE;

    /* write crc timeout detection */
    msdc_register_base->PATCH_BIT0 = msdc_register_base->PATCH_BIT0 | (1 << 30);

    /*switch INCR1 to single burst.*/
    msdc_register_base->PATCH_BIT1 |= (1 << 16);

    /* Configure to default data timeout */
    msdc_register_base->SDC_CFG = (msdc_register_base->SDC_CFG & (~SDC_CFG_DTOC)) | (80 << 24);

    msdc_set_bus_width(msdc_port,MSDC_BUS_WIDTH_1BITS);

    /*set block number.*/
    msdc_register_base->SDC_BLK_NUM = 1;

    msdc_config[msdc_port].is_card_present = true;

#if defined(HAL_SD_MODULE_ENABLED) && defined(HAL_SD_CARD_DETECTION)
    /*card detection eint registration*/
    msdc_eint_registration(msdc_port);
    log_hal_info("msdc eint register done\r\n");
#endif

    log_hal_info("init hardware done\r\n");

    return status;
}

void msdc_deinit(msdc_port_t msdc_port)
{
    uint32_t reg_value = 0;
    msdc_register_t *msdc_register_base;

    msdc_register_base = (MSDC_PORT_0 == msdc_port) ? MSDC0_REG : MSDC1_REG;
  
    /* Disable and clear all interrupts */
    reg_value = msdc_register_base->MSDC_INTEN;
    msdc_register_base->MSDC_INTEN &= ~reg_value;
    reg_value = msdc_register_base->MSDC_INT;
    msdc_register_base->MSDC_INT |= reg_value;
    msdc_power_set(msdc_port,false);

    msdc_config[msdc_port].is_card_present = false;

    log_hal_info("deinit hardware done");
}


bool msdc_card_is_present(msdc_port_t msdc_port)
{
    return msdc_config[msdc_port].is_card_present;
}

void msdc_dma_enable(msdc_port_t msdc_port)
{
    msdc_register_t *msdc_register_base;

    msdc_register_base = (MSDC_PORT_0 == msdc_port) ? MSDC0_REG : MSDC1_REG;

    msdc_register_base->MSDC_CFG &= ~MSDC_CFG_PIO;
}

void msdc_dma_disable(msdc_port_t msdc_port)
{
    msdc_register_t *msdc_register_base;

    msdc_register_base = (MSDC_PORT_0 == msdc_port) ? MSDC0_REG : MSDC1_REG;

    msdc_register_base->MSDC_CFG |= MSDC_CFG_PIO;
}

msdc_owner_t msdc_get_owner(msdc_port_t msdc_port)
{
    return (msdc_config[msdc_port].owner);
}

void msdc_set_owner(msdc_port_t msdc_port,msdc_owner_t owner)
{
    msdc_config[msdc_port].owner = owner;
}

/*this function used to turn on power for card VDD and VDDQ*/
void msdc_card_power_set(msdc_port_t msdc_port,bool is_power_on)
{
    /*card power set.*/
    // mt2625 has no this feature
}

void msdc_clock_init(msdc_port_t msdc_port)
{
    msdc_register_t *msdc_register_base;

    msdc_register_base = (MSDC_PORT_0 == msdc_port) ? MSDC0_REG : MSDC1_REG;

    /*in 50MHZ case, we should set 80 to have at least 100ms timeout,for initial read*/
    msdc_register_base->SDC_CFG = (msdc_register_base->SDC_CFG & (~SDC_CFG_DTOC)) |
                                  (MSDC_DATA_TIMEOUT_COUNT << SDC_CFG_DTOC_OFFSET);

    /*set output clock to 240KHz. The clock should <= 400KHz,*/
    msdc_set_output_clock(msdc_port,MSDC_INIT_CLOCK);

}

void msdc_set_burst_type(msdc_port_t msdc_port,msdc_burst_type_t burst_type)
{
    msdc_register_t *msdc_register_base;

    msdc_register_base = (MSDC_PORT_0 == msdc_port) ? MSDC0_REG : MSDC1_REG;

    msdc_register_base->DMA_CTRL = (msdc_register_base->DMA_CTRL & (~MSDC_DMA_CTRL_BRUSTSZ)) | (burst_type << 12);
}

#if defined HAL_SLEEP_MANAGER_ENABLED

pmu_vcore_voltage_t msdc_get_vcore_voltage(void)
{
    pmu_vcore_voltage_t vcore_voltage;

    vcore_voltage = pmu_get_vcore_voltage();

    return vcore_voltage;
}

#endif

#endif /*HAL_SD_MODULE_ENABLED || HAL_SDIO_MODULE_ENABLED*/

