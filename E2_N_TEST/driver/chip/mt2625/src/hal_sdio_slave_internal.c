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
#include "hal_sdio_slave_internal.h"

#ifdef HAL_SDIO_SLAVE_MODULE_ENABLED
#include "hal_log.h"
#include "assert.h"
#include "hal_platform.h"
#include <string.h>
#include "memory_attribute.h"
#include "hal_spm.h"

sdio_slave_private_t sdio_private;
ATTR_RWDATA_IN_NONCACHED_RAM_4BYTE_ALIGN sdio_slave_gpd_header_t sdio_slave_rx_gpd[SDIO_SLAVE_RX_QUEUE_MAX];
ATTR_RWDATA_IN_NONCACHED_RAM_4BYTE_ALIGN sdio_slave_gpd_header_t sdio_slave_tx_gpd[SDIO_SLAVE_TX_QUEUE_MAX];
ATTR_RWDATA_IN_NONCACHED_RAM_4BYTE_ALIGN sdio_slave_gpd_header_t sdio_slave_gpd_null;
volatile bool sdio_slave_power_enable = false;
volatile hal_sdio_slave_callback_sw_interrupt_parameter_t h2d_sw_interrupt;
volatile hal_sdio_slave_callback_tx_length_parameter_t tx_len;
sdio_slave_interrupt_status_t interrupt_status;
volatile uint32_t rx_done_status = 0;
volatile uint32_t tx_done_status = 0;


typedef enum {
    SDIO_SLAVE_TRANSMISSION_NONE = 0,
    SDIO_SLAVE_TRANSMISSION_SEND_RX_QUEUE_0 = 0x01,    // send: slave send. RX queue: in host view
    SDIO_SLAVE_TRANSMISSION_SEND_RX_QUEUE_1 = 0x02,
    SDIO_SLAVE_TRANSMISSION_RECEIVE_TX_QUEUE_1 = 0x04  // receive: slave receive. TX queue: in host view
} sdio_slave_transmission_state_t;

uint16_t sdio_slave_transmission_state = SDIO_SLAVE_TRANSMISSION_NONE;

#ifdef HAL_SLEEP_MANAGER_ENABLED
    bool already_acquire_sleep_locked = false;
#endif


void sdio_slave_read_mailbox(uint32_t mailbox_number, uint32_t *mailbox_value)
{
    if (0 == mailbox_number) {
        *mailbox_value = SDIO_SLAVE_REG->H2DRM0R;
    } else if (1 == mailbox_number) {
        *mailbox_value = SDIO_SLAVE_REG->H2DRM1R;
    } else {
        log_hal_error("error, read mailbox_number %d invalide.\r\n", mailbox_number);
    }
}


void sdio_slave_write_mailbox(uint32_t mailbox_number, uint32_t mailbox_value)
{
    if (0 == mailbox_number) {
        SDIO_SLAVE_REG->D2HSM0R = mailbox_value;
    } else if (1 == mailbox_number) {
        SDIO_SLAVE_REG->D2HSM1R = mailbox_value;
    } else {
        log_hal_error("error, write mailbox_number %d invalide.\r\n", mailbox_number);
    }
}


void sdio_slave_set_device_to_host_interrupt(uint32_t interrupt_number)
{
    if (24 < interrupt_number) {
        log_hal_error("error, read mailbox_number invalide.\r\n");
    } else {
        SDIO_SLAVE_REG->HWFICR = 0x00000001 << (HWFICR_D2H_SW_INT_SET_OFFSET + interrupt_number);
    }
}


bool sdio_slave_check_fw_own(void)
{
    if (SDIO_SLAVE_REG->HWFICR & HWFICR_FW_OWN_BACK_INT_SET_MASK) {
        return true;
    } else {
        return false;
    }
}


void sdio_slave_give_fw_own(void)
{
    SDIO_SLAVE_REG->HWFICR = HWFICR_FW_OWN_BACK_INT_SET_MASK;
}

void sdio_slave_pdn_set(bool enable)
{
    if (enable) {
        hal_clock_enable(HAL_CLOCK_CG_SDIOSLV);
    } else {
        hal_clock_disable(HAL_CLOCK_CG_SDIOSLV);
    }
}


void sdio_slave_private_init(void)
{
    uint32_t i = 0;

    memset(&sdio_private, 0 , sizeof(sdio_slave_private_t));
    sdio_private.sdio_hw_property.close_pio_clock = true;
    sdio_private.sdio_hw_property.close_ehpi_clock = true;
    sdio_private.sdio_hw_property.close_spi_clock = true;
    sdio_private.sdio_hw_property.close_sdio_clock = false;
    sdio_private.sdio_hw_property.force_high_speed_mode = false;
    sdio_private.sdio_hw_property.not_gate_ahb_clk = true;
    sdio_private.sdio_hw_property.int_mask_at_terminal_cycle = false;
    sdio_private.sdio_hw_property.set_fw_own_back_if_any_interrupt = true;
    sdio_private.sdio_hw_property.interface_mode = 1;
    sdio_private.sdio_hw_property.tx_port_number = 1;
    sdio_private.sdio_hw_property.remove_tx_Header = true;
    sdio_private.sdio_hw_property.remove_rx_redundant_zero = true;

    sdio_private.sdio_property.txq_number = SDIO_SLAVE_MAX_TXQ_NUM;
    sdio_private.sdio_property.rxq_number = SDIO_SLAVE_MAX_RXQ_NUM;
    sdio_private.sdio_property.checksum_enable = true;

    for (i = SDIO_SLAVE_RXQ_START; i < SDIO_SLAVE_MAX_RXQ_NUM; i++) {
        sdio_private.sdio_property.rx_ioc_disable[i] = false;
    }
}

void sdio_slave_hardware_init(void)
{
    uint32_t reg_value = 0;
    uint32_t i = 0;

    if (false == sdio_slave_power_enable) {
#ifdef HAL_SLEEP_MANAGER_ENABLED
        spm_control_mtcmos(SPM_MTCMOS_SDIO_SLV, SPM_MTCMOS_PWR_ENABLE);
#endif
    }

    sdio_slave_pdn_set(true);

    hal_gpt_delay_ms(10);

    SDIO_SLAVE_REG->HGFCR = HGFCR_HINT_AS_FW_OB_MASK | HGFCR_HCLK_NO_GATED_MASK |
                            HGFCR_EHPI_HCLK_DIS_MASK | HGFCR_EHPI_HCLK_DIS_MASK | HGFCR_PB_HCLK_DIS_MASK;


    SDIO_SLAVE_REG->HWFCR = HWFCR_RX_NO_TAIL_MASK | HWFCR_TX_NO_HEADER_MASK;

    /*When vcore level is lower than 0.9V,config the HGFCR_FORCE_SD_HS_MASK to finish the init flow*/
    #if defined HAL_SLEEP_MANAGER_ENABLED
        if (PMU_VCORE_VOL_0P9V >= msdc_get_vcore_voltage()) {
            SDIO_SLAVE_REG->HGFCR |= HGFCR_FORCE_SD_HS_MASK;
        }
    #endif

    reg_value = SDIO_SLAVE_REG->HWFIOCDR;
    /*Disable IOC, means always issue interrupt when GPD done. Notice:For using IOC GPD to trigger interrupt, we default close the RX_DONE interrupt.*/
    for (i = SDIO_SLAVE_RXQ_START; i < (sdio_private.sdio_property.rxq_number + SDIO_SLAVE_RXQ_START); i++) {
        if (sdio_private.sdio_property.rx_ioc_disable[i] == true) {
            reg_value |= SDIO_SLAVE_RXQ_IOC_DISABLE(i);
        }
    }
    SDIO_SLAVE_REG->HWFIOCDR = reg_value;

    SDIO_SLAVE_REG->HSDIOTOCR = 0x3f000;

    /*Set function ready.*/
    SDIO_SLAVE_REG->HWFCR |= HWFCR_W_FUNC_RDY_MASK;

    sdio_slave_tx_queue_count_reset();
    sdio_slave_set_rx_queue_stop(SDIO_SLAVE_RX_QUEUE_0);
    sdio_slave_set_rx_queue_stop(SDIO_SLAVE_RX_QUEUE_1);
    sdio_slave_set_tx_queue_stop(SDIO_SLAVE_TX_QUEUE_1);

}


void sdio_slave_hardware_deinit(void)
{
    if (sdio_slave_power_enable) {
#ifdef HAL_SLEEP_MANAGER_ENABLED
        spm_control_mtcmos(SPM_MTCMOS_SDIO_SLV, SPM_MTCMOS_PWR_DISABLE);
#endif
    }
}

static void __sdio_rx_queue_event_callback(sdio_slave_rx_queue_id_t sdio_slave_rx_queue_id, hal_sdio_slave_callback_event_t hal_sdio_slave_callback_event)
{
    if (sdio_slave_rx_queue_id == SDIO_SLAVE_RX_QUEUE_0) {
        if (sdio_slave_transmission_state & SDIO_SLAVE_TRANSMISSION_SEND_RX_QUEUE_0) {
            sdio_private.sdio_property.sdio_slave_callback(hal_sdio_slave_callback_event, 0, sdio_private.sdio_property.sdio_slave_callback_user_data);
            sdio_slave_transmission_state &= ~SDIO_SLAVE_TRANSMISSION_SEND_RX_QUEUE_0;
        } else {
            log_hal_error("__sdio_rx_queue_event_callback: SDIO_SLAVE_RX_QUEUE_0, something wrong sdio_slave_transmission_state(%d) != SDIO_SLAVE_TRANSMISSION_SEND_RX_QUEUE_0, hal_sdio_slave_callback_event=%d\r\n", sdio_slave_transmission_state, hal_sdio_slave_callback_event);
        }
    } else if  (sdio_slave_rx_queue_id == SDIO_SLAVE_RX_QUEUE_1) {
        if (sdio_slave_transmission_state & SDIO_SLAVE_TRANSMISSION_SEND_RX_QUEUE_1) {
            sdio_private.sdio_property.sdio_slave_callback(hal_sdio_slave_callback_event, 0, sdio_private.sdio_property.sdio_slave_callback_user_data);
            sdio_slave_transmission_state &= ~SDIO_SLAVE_TRANSMISSION_SEND_RX_QUEUE_1;
        } else {
            log_hal_error("__sdio_rx_queue_event_callback: SDIO_SLAVE_RX_QUEUE_1, something wrong sdio_slave_transmission_state(%d) != SDIO_SLAVE_TRANSMISSION_SEND_RX_QUEUE_0, hal_sdio_slave_callback_event=%d\r\n", sdio_slave_transmission_state, hal_sdio_slave_callback_event);
        }
    } else {
        log_hal_error("__sdio_rx_queue_event_callback: something wrong. queue out of range. sdio_slave_rx_queue_id=%d\r\n", sdio_slave_rx_queue_id);
    }
}

static void __sdio_print_interrupt_register_status(void)
{
    log_hal_info("__sdio_print_interrupt_register_status: interrupt_status.hwfte0sr_interrupte_status=%x\r\n", interrupt_status.hwfte0sr_interrupte_status);
    log_hal_info("__sdio_print_interrupt_register_status: interrupt_status.hwfre0sr_interrupte_status=%x\r\n", interrupt_status.hwfre0sr_interrupte_status);
    log_hal_info("__sdio_print_interrupt_register_status: interrupt_status.hwfre1sr_interrupte_status=%x\r\n", interrupt_status.hwfre1sr_interrupte_status);
    log_hal_info("__sdio_print_interrupt_register_status: interrupt_status.global_interrupt_status_mask=%x\r\n", interrupt_status.global_interrupt_status_mask);
    log_hal_info("__sdio_print_interrupt_register_status: interrupt_status.hwfisr_interrupte_status=%x\r\n", interrupt_status.hwfisr_interrupte_status);

}

void sdio_slave_isr(hal_nvic_irq_t irq_number)
{
    uint32_t temp_status;
    uint32_t i;

    sdio_slave_nvic_set(false);

    log_hal_info("sdio_slave_isr.++\r\n");

    /*read interrupt status.*/
    interrupt_status.hwfte0sr_interrupte_status = SDIO_SLAVE_REG->HWFTE0SR & sdio_private.sdio_isr_mask.hwfte0er_interrupte_enable_mask;
    interrupt_status.hwfre0sr_interrupte_status = SDIO_SLAVE_REG->HWFRE0SR & sdio_private.sdio_isr_mask.hwfre0er_interrupte_enable_mask;
    interrupt_status.hwfre1sr_interrupte_status = SDIO_SLAVE_REG->HWFRE1SR & sdio_private.sdio_isr_mask.hwfre1er_interrupte_enable_mask;
    interrupt_status.global_interrupt_status_mask = SDIO_SLAVE_REG->HGFISR & sdio_private.sdio_isr_mask.global_interrupt_enable_mask;

    temp_status = SDIO_SLAVE_REG->HWFISR;
    interrupt_status.sw_interrupte_status_mask = ((uint16_t)((temp_status & HWFIER_H2D_SW_INT_EN_MASK) >> HWFISR_H2D_SW_INT_OFFSET)) &
            sdio_private.sdio_isr_mask.sw_interrupte_enable_mask;
    interrupt_status.fw_interrupte_status_mask = ((uint16_t)(temp_status & 0xffff)) & sdio_private.sdio_isr_mask.fw_interrupte_enable_mask;

    interrupt_status.hwfisr_interrupte_status = temp_status;

    /*clean interrupt status.*/
    SDIO_SLAVE_REG->HWFTE0SR = interrupt_status.hwfte0sr_interrupte_status;
    SDIO_SLAVE_REG->HWFRE0SR = interrupt_status.hwfre0sr_interrupte_status;
    SDIO_SLAVE_REG->HWFRE1SR = interrupt_status.hwfre1sr_interrupte_status;
    SDIO_SLAVE_REG->HGFISR = interrupt_status.global_interrupt_status_mask;
    SDIO_SLAVE_REG->HWFISR = temp_status;


    /*interrupt handle.*/
    if (interrupt_status.fw_interrupte_status_mask & HWFISR_DRV_SET_FW_OWN_MASK) {
        log_hal_info("sdio_slave_isr. driver give firmware own to firmware\r\n");
#ifdef HAL_SLEEP_MANAGER_ENABLED
        if (already_acquire_sleep_locked) {
            if (sdio_slave_transmission_state == SDIO_SLAVE_TRANSMISSION_NONE) {
                if (hal_sleep_manager_release_sleeplock(SLEEP_LOCK_SDIO_SLV, HAL_SLEEP_LOCK_ALL) == HAL_SLEEP_MANAGER_OK){
                    log_hal_info("SLEEP_LOCK_SDIO_SLV unlock sleep success.\r\n");
                    already_acquire_sleep_locked = false;
                } else {
                    log_hal_error("SLEEP_LOCK_SDIO_SLV unlock sleep fail.\r\n");
                }
            } else {
                // it should not be in this case. since if it didn't transmission yet, host should not give own to firmware
                log_hal_error("sdio_slave_isr: sdio_slave_transmission_state(%d) != SDIO_SLAVE_TRANSMISSION_NONE. can't release sleeplock\r\n", sdio_slave_transmission_state);
            }
        }
#endif
    }

    if (interrupt_status.fw_interrupte_status_mask & HWFISR_DRV_CLR_FW_OWN_MASK) {
        log_hal_info("sdio_slave_isr. driver request firmware own. give firmware own to dirver\r\n");
#ifdef HAL_SLEEP_MANAGER_ENABLED
        if (!already_acquire_sleep_locked) {
            if (hal_sleep_manager_acquire_sleeplock(SLEEP_LOCK_SDIO_SLV, HAL_SLEEP_LOCK_ALL) == HAL_SLEEP_MANAGER_OK){
                already_acquire_sleep_locked = true;
                log_hal_info("SLEEP_LOCK_SDIO_SLV lock sleep success. (driver request firmware own)\r\n");
            } else {
                log_hal_info("SLEEP_LOCK_SDIO_SLV lock sleep fail. (driver request firmware own)\r\n");
            }
        } else {
            log_hal_info("already_acquire_sleep_locked, so don't acquire again. (driver request firmware own)\r\n");
        }
#endif
        sdio_slave_give_fw_own();
    }

    if (interrupt_status.fw_interrupte_status_mask & HWFISR_RD_TIMEOUT_INT_MASK) {
        log_hal_error("RD Timeout fail, fw_interrupte_status_mask = %x\r\n", interrupt_status.fw_interrupte_status_mask);
        if (sdio_slave_transmission_state & (SDIO_SLAVE_TRANSMISSION_SEND_RX_QUEUE_0 | SDIO_SLAVE_TRANSMISSION_SEND_RX_QUEUE_1)) {
            sdio_private.sdio_property.sdio_slave_callback(HAL_SDIO_SLAVE_EVENT_ERROR, 0, sdio_private.sdio_property.sdio_slave_callback_user_data);
            sdio_slave_transmission_state &= ~(SDIO_SLAVE_TRANSMISSION_SEND_RX_QUEUE_0 | SDIO_SLAVE_TRANSMISSION_SEND_RX_QUEUE_1);
        } else {
            log_hal_error("fw_interrupte_status_mask fail, sdio_slave_transmission_state(%d) is wrong, interrupt_status.fw_interrupte_status_mask = %x\r\n", sdio_slave_transmission_state, interrupt_status.fw_interrupte_status_mask);
        }

        __sdio_print_interrupt_register_status();
        return;
    }

    if (interrupt_status.fw_interrupte_status_mask & HWFISR_WR_TIMEOUT_INT_MASK) {
        log_hal_error("WR Timeout fail, fw_interrupte_status_mask = %x\r\n", interrupt_status.fw_interrupte_status_mask);
        if (sdio_slave_transmission_state & SDIO_SLAVE_TRANSMISSION_RECEIVE_TX_QUEUE_1) {
            sdio_private.sdio_property.sdio_slave_callback(HAL_SDIO_SLAVE_EVENT_ERROR, 0, sdio_private.sdio_property.sdio_slave_callback_user_data);
            sdio_slave_transmission_state &= ~SDIO_SLAVE_TRANSMISSION_RECEIVE_TX_QUEUE_1;
        } else {
            log_hal_error("fw_interrupte_status_mask fail, sdio_slave_transmission_state(%d) is wrong, interrupt_status.fw_interrupte_status_mask = %x\r\n", sdio_slave_transmission_state, interrupt_status.fw_interrupte_status_mask);
        }

        __sdio_print_interrupt_register_status();
        return;
    }

    if (interrupt_status.hwfte0sr_interrupte_status & 0xffffff00) {
        log_hal_error("Tx fail, hwfte0sr_interrupte_status = %x\r\n", interrupt_status.hwfte0sr_interrupte_status);
        if (interrupt_status.hwfte0sr_interrupte_status & 0x02020200) { // Tx1  error
            if (sdio_slave_transmission_state & SDIO_SLAVE_TRANSMISSION_RECEIVE_TX_QUEUE_1) {
                sdio_private.sdio_property.sdio_slave_callback(HAL_SDIO_SLAVE_EVENT_ERROR, 0, sdio_private.sdio_property.sdio_slave_callback_user_data);
                sdio_slave_transmission_state &= ~SDIO_SLAVE_TRANSMISSION_RECEIVE_TX_QUEUE_1;
            } else {
                log_hal_error("fw_interrupte_status_mask fail, sdio_slave_transmission_state(%d) is wrong, hwfre0sr_interrupte_status = %x\r\n", sdio_slave_transmission_state, interrupt_status.hwfre0sr_interrupte_status);
            }
        } else {
            log_hal_error("Tx1 fail, we didn't support this Tx queue, hwfte0sr_interrupte_status = %x\r\n", interrupt_status.hwfte0sr_interrupte_status);
        }

        __sdio_print_interrupt_register_status();
        return;
    }

    if (interrupt_status.hwfre0sr_interrupte_status & 0xffffff00) {
        log_hal_error("hwfre0sr fail, hwfre0sr_interrupte_status = %x\r\n", interrupt_status.hwfre0sr_interrupte_status);
        if (interrupt_status.hwfre0sr_interrupte_status & 0x01010100) { // Rx0  error
            __sdio_rx_queue_event_callback(SDIO_SLAVE_RX_QUEUE_0, HAL_SDIO_SLAVE_EVENT_ERROR);
        } else if (interrupt_status.hwfre0sr_interrupte_status & 0x02020200) { // Rx1  error
            __sdio_rx_queue_event_callback(SDIO_SLAVE_RX_QUEUE_1, HAL_SDIO_SLAVE_EVENT_ERROR);
        } else {
            log_hal_error("hwfre0sr fail, but we didn't support this rx queue hwfre0sr_interrupte_status = %x\r\n", interrupt_status.hwfre0sr_interrupte_status);
        }

        __sdio_print_interrupt_register_status();
        return;
    }

    if (interrupt_status.hwfre1sr_interrupte_status & 0x0f00) {
        log_hal_error("hwfre1sr fail, hwfre1sr_interrupte_status = %x\r\n", interrupt_status.hwfre1sr_interrupte_status);
        if (interrupt_status.hwfre0sr_interrupte_status & 0x01) { // Rx0  error
            __sdio_rx_queue_event_callback(SDIO_SLAVE_RX_QUEUE_0, HAL_SDIO_SLAVE_EVENT_ERROR);
        } else if (interrupt_status.hwfre0sr_interrupte_status & 0x02) { // Rx1  error
            __sdio_rx_queue_event_callback(SDIO_SLAVE_RX_QUEUE_1, HAL_SDIO_SLAVE_EVENT_ERROR);
        } else {
            log_hal_error("hwfre1sr fail, but we didn't support this rx queue hwfre0sr_interrupte_status = %x\r\n", interrupt_status.hwfre0sr_interrupte_status);
        }

        __sdio_print_interrupt_register_status();
        return;
    }

    if (interrupt_status.global_interrupt_status_mask & HGFISR_CHG_TO_18V_REQ_INT_MASK) {
        SDIO_SLAVE_REG->HGFCR |= HGFCR_CARD_IS_18V_MASK;
        log_hal_info("Host performd CMD11 to change voltage.\r\n");
    }


    if ((interrupt_status.hwfre0sr_interrupte_status & 0x0f)){  // || (interrupt_status.hwfre1sr_interrupte_status & 0x0f)) {
        for (i = SDIO_SLAVE_RXQ_START; i < SDIO_SLAVE_MAX_RXQ_NUM; i++) {
            if (interrupt_status.hwfre0sr_interrupte_status & SDIO_SLAVE_RXQ_DONE(i)) {
                if (NULL == sdio_private.sdio_property.sdio_slave_callback) {
                    assert(0);
                }

                if (i == SDIO_SLAVE_RX_QUEUE_0) {
                    __sdio_rx_queue_event_callback(SDIO_SLAVE_RX_QUEUE_0, HAL_SDIO_SLAVE_EVENT_RX0_DONE);
                } else if (i == SDIO_SLAVE_RX_QUEUE_1) {
                    __sdio_rx_queue_event_callback(SDIO_SLAVE_RX_QUEUE_1, HAL_SDIO_SLAVE_EVENT_RX1_DONE);
                } else {
                    log_hal_error("something wrong, we didn't support this rx queue. interrupt_status.hwfre0sr_interrupte_status%x\r\n", interrupt_status.hwfre0sr_interrupte_status);
                }
            }
        }
    }

    if (interrupt_status.hwfte0sr_interrupte_status & 0xff) {
        if (NULL == sdio_private.sdio_property.sdio_slave_callback) {
            assert(0);
        }

        if (interrupt_status.hwfte0sr_interrupte_status & 0x2) {
            if (sdio_slave_transmission_state & SDIO_SLAVE_TRANSMISSION_RECEIVE_TX_QUEUE_1) {
                tx_len.hal_sdio_slave_tx_length = sdio_slave_tx_gpd[SDIO_SLAVE_TX_QUEUE_1].word_3.length.buffer_length;
                sdio_private.sdio_property.sdio_slave_callback(HAL_SDIO_SLAVE_EVENT_TX1_DONE, (void *)(&tx_len), sdio_private.sdio_property.sdio_slave_callback_user_data);
                sdio_slave_transmission_state &= ~SDIO_SLAVE_TRANSMISSION_RECEIVE_TX_QUEUE_1;
            }
        } else {
            log_hal_error("something wrong, we didn't support this tx queue. interrupt_status.hwfte0sr_interrupte_status=%x\r\n", interrupt_status.hwfte0sr_interrupte_status);
        }
    }

    if (interrupt_status.sw_interrupte_status_mask) {
        if (NULL == sdio_private.sdio_property.sdio_slave_callback) {
            assert(0);
        }
        h2d_sw_interrupt.hal_sdio_slave_sw_interrupt_number = interrupt_status.sw_interrupte_status_mask;
        sdio_private.sdio_property.sdio_slave_callback(HAL_SDIO_SLAVE_EVENT_SW_INTERRUPT, (void *)(&h2d_sw_interrupt), sdio_private.sdio_property.sdio_slave_callback_user_data);
    }

    log_hal_info("sdio_slave_isr.--\r\n");

    sdio_slave_nvic_set(true);
}


sdio_slave_status_t sdio_slave_interrupt_status_polling(void)
{
    uint32_t temp_status;
    sdio_slave_status_t status;

    interrupt_status.hwfte0sr_interrupte_status = SDIO_SLAVE_REG->HWFTE0SR;
    interrupt_status.hwfre0sr_interrupte_status = SDIO_SLAVE_REG->HWFRE0SR;
    interrupt_status.hwfre1sr_interrupte_status = SDIO_SLAVE_REG->HWFRE1SR;
    interrupt_status.global_interrupt_status_mask = SDIO_SLAVE_REG->HGFISR;
    temp_status = SDIO_SLAVE_REG->HWFISR;

    interrupt_status.sw_interrupte_status_mask = ((uint16_t)((interrupt_status.sw_interrupte_status_mask & HWFIER_H2D_SW_INT_EN_MASK) >> HWFISR_H2D_SW_INT_OFFSET)) &
            sdio_private.sdio_isr_mask.sw_interrupte_enable_mask;
    interrupt_status.fw_interrupte_status_mask = ((uint16_t)(interrupt_status.fw_interrupte_status_mask & 0xffff)) & sdio_private.sdio_isr_mask.fw_interrupte_enable_mask;

    /*clean interrupt status.*/
    SDIO_SLAVE_REG->HWFTE0SR = interrupt_status.hwfte0sr_interrupte_status;
    SDIO_SLAVE_REG->HWFRE0SR = interrupt_status.hwfre0sr_interrupte_status;
    SDIO_SLAVE_REG->HWFRE1SR = interrupt_status.hwfre1sr_interrupte_status;
    SDIO_SLAVE_REG->HGFISR = interrupt_status.global_interrupt_status_mask;
    SDIO_SLAVE_REG->HWFISR = temp_status;

    if (interrupt_status.global_interrupt_status_mask & HGFISR_CRC_ERROR_INT_MASK) {
        status = SDIO_SLAVE_STATUS_CRC_ERROR;
        goto error;
    }

    if (interrupt_status.global_interrupt_status_mask & HGFISR_DRV_CLR_DB_IOE_MASK) {
        status = SDIO_SLAVE_STATUS_ABNORMAL_SETTING;
        goto error;
    }

    if (interrupt_status.global_interrupt_status_mask & HGFISR_SDIO_SET_ABT_MASK) {
        status = SDIO_SLAVE_STATUS_USER_TIMEOUT;
        goto error;
    }

    if (temp_status & HWFISR_DRV_SET_FW_OWN_MASK) {
        status = SDIO_SLAVE_STATUS_ABNORMAL_HOST_BEHAVIOR;
        goto error;
    }

    if (temp_status & HWFISR_WR_TIMEOUT_INT_MASK) {
        status = SDIO_SLAVE_STATUS_HW_WRITE_TIMEOUT;
        goto error;
    }

    if (temp_status & HWFISR_RD_TIMEOUT_INT_MASK) {
        status = SDIO_SLAVE_STATUS_HW_READ_TIMEOUT;
        goto error;
    }

    if (interrupt_status.hwfte0sr_interrupte_status & 0x300) {
        status = SDIO_SLAVE_STATUS_TX_OVERFLOW;
        goto error;
    }

    if (interrupt_status.hwfre0sr_interrupte_status & 0xf0) {
        status = SDIO_SLAVE_STATUS_RX_UNDERFLOW;
        goto error;
    }

    if (interrupt_status.hwfre0sr_interrupte_status & 0xf00000) {
        status = SDIO_SLAVE_STATUS_RX_LEN_FIFO_OVERFLOW;
        goto error;
    }

    if (interrupt_status.hwfre0sr_interrupte_status & 0x0f) {
        rx_done_status = interrupt_status.hwfre0sr_interrupte_status & 0x0f;
    }

    if (interrupt_status.hwfte0sr_interrupte_status & 0x02) {
        tx_done_status = interrupt_status.hwfte0sr_interrupte_status & 0xff;
    }

    return SDIO_SLAVE_STATUS_OK;

error:

    sdio_slave_set_rx_queue_stop(SDIO_SLAVE_RX_QUEUE_0);
    sdio_slave_set_rx_queue_stop(SDIO_SLAVE_RX_QUEUE_1);
    sdio_slave_set_tx_queue_stop(SDIO_SLAVE_TX_QUEUE_1);

    /*send error status to host*/
    log_hal_error("status = %d", status);
    log_hal_error("HWFTE0SR : %x \r\n", interrupt_status.hwfte0sr_interrupte_status);
    log_hal_error("HWFRE0SR : %x \r\n", interrupt_status.hwfre0sr_interrupte_status);
    log_hal_error("HWFRE1SR : %x \r\n", interrupt_status.hwfre1sr_interrupte_status);
    log_hal_error("HGFISR : %x \r\n", interrupt_status.global_interrupt_status_mask);
    log_hal_error("HWFISR : %x \r\n", temp_status);

    return status;
}

void sdio_slave_nvic_set(bool enable)
{
    /*should call nvic api to set msdc interrupt enable or disable*/
    if (enable) {
        hal_nvic_enable_irq(SDIO_SLV_IRQn);
    } else {
        hal_nvic_disable_irq(SDIO_SLV_IRQn);
    }
}


void sdio_slave_interrupt_init(void)
{
    uint32_t i = 0;

    hal_nvic_register_isr_handler(SDIO_SLV_IRQn, sdio_slave_isr);

    /*Set SDIO IP layer 1(global) interrupt config.*/
    sdio_private.sdio_isr_mask.global_interrupt_enable_mask = (HGFISR_DRV_CLR_DB_IOE_MASK | HGFISR_DRV_SET_DB_IOE_MASK | HGFISR_SDIO_SET_RES_MASK |
            HGFISR_SDIO_SET_ABT_MASK | HGFISR_DB_INT_MASK | HGFISR_CRC_ERROR_INT_MASK |
            HGFISR_CHG_TO_18V_REQ_INT_MASK | HGFISR_SD1_SET_XTAL_UPD_INT_MASK);  // we didn't include PIO interrupt, since our spec didn't have PIO mode

    SDIO_SLAVE_REG->HGFIER = sdio_private.sdio_isr_mask.global_interrupt_enable_mask;

    /*Set SDIO layer 2(FW & SW) interrupte config.*/    // HWFISR
    sdio_private.sdio_isr_mask.sw_interrupte_enable_mask = 0xffff;
    sdio_private.sdio_isr_mask.fw_interrupte_enable_mask = HWFIER_DRV_SET_FW_OWN_INT_EN_MASK | HWFIER_DRV_CLR_FW_OWN_INT_EN_MASK | HWFIER_D2HSM2R_RD_INT_EN_MASK |
            HWFIER_RD_TIMEOUT_INT_EN_MASK | HWFIER_WR_TIMEOUT_INT_EN_MASK | HWFIER_TX_EVENT_0_INT_EN_MASK |
            HWFIER_RX_EVENT_0_INT_EN_MASK | HWFIER_RX_EVENT_1_INT_EN_MASK;

    SDIO_SLAVE_REG->HWFIER = HWFIER_DRV_SET_FW_OWN_INT_EN_MASK | HWFIER_DRV_CLR_FW_OWN_INT_EN_MASK | HWFIER_D2HSM2R_RD_INT_EN_MASK |
                             HWFIER_RD_TIMEOUT_INT_EN_MASK | HWFIER_WR_TIMEOUT_INT_EN_MASK | HWFIER_TX_EVENT_0_INT_EN_MASK |
                             HWFIER_RX_EVENT_0_INT_EN_MASK | HWFIER_RX_EVENT_1_INT_EN_MASK | HWFIER_H2D_SW_INT_EN_MASK;

    /*Set SDIO layer 3(QMU) related interrupte config.*/    // these is not good writting, but since we only have tx1. so it's ok
    sdio_private.sdio_isr_mask.hwfte0er_interrupte_enable_mask = 0;   // TXx_REDY: 0x2: Tx1_REDY
    sdio_private.sdio_isr_mask.hwfre0er_interrupte_enable_mask = 0;
    sdio_private.sdio_isr_mask.hwfre1er_interrupte_enable_mask = 0;

    for (i = SDIO_SLAVE_TXQ_START; i < (sdio_private.sdio_property.txq_number + SDIO_SLAVE_TXQ_START); i++) {
        sdio_private.sdio_isr_mask.hwfte0er_interrupte_enable_mask |= SDIO_SLAVE_TXQ_READY(i);
        sdio_private.sdio_isr_mask.hwfte0er_interrupte_enable_mask |= SDIO_SLAVE_TXQ_CHKSUM_ERR(i);
        sdio_private.sdio_isr_mask.hwfte0er_interrupte_enable_mask |= SDIO_SLAVE_TXQ_LEN_ERR(i);
    }

    for (i = SDIO_SLAVE_TXQ_START; i < (sdio_private.sdio_hw_property.tx_port_number + SDIO_SLAVE_TXQ_START); i++) {
        sdio_private.sdio_isr_mask.hwfte0er_interrupte_enable_mask |= SDIO_SLAVE_TXQ_OVERFLOW(i);
    }
    SDIO_SLAVE_REG->HWFTE0ER = sdio_private.sdio_isr_mask.hwfte0er_interrupte_enable_mask;

    for (i = SDIO_SLAVE_RXQ_START; i < (sdio_private.sdio_property.rxq_number + SDIO_SLAVE_RXQ_START); i++) {
        sdio_private.sdio_isr_mask.hwfre0er_interrupte_enable_mask |= SDIO_SLAVE_RXQ_DONE(i);
        sdio_private.sdio_isr_mask.hwfre0er_interrupte_enable_mask |= SDIO_SLAVE_RXQ_UNDERFLOW(i);
        sdio_private.sdio_isr_mask.hwfre0er_interrupte_enable_mask |= SDIO_SLAVE_RXQ_CHKSUN_ERR(i);
        sdio_private.sdio_isr_mask.hwfre0er_interrupte_enable_mask |= SDIO_SLAVE_RXQ_OVERFLOW(i);

        sdio_private.sdio_isr_mask.hwfre1er_interrupte_enable_mask |= SDIO_SLAVE_RXQ_OWN_CLEAR(i);
        sdio_private.sdio_isr_mask.hwfre1er_interrupte_enable_mask |= SDIO_SLAVE_RXQ_LEN_ERR(i);
    }

    SDIO_SLAVE_REG->HWFRE0ER = sdio_private.sdio_isr_mask.hwfre0er_interrupte_enable_mask;
    SDIO_SLAVE_REG->HWFRE1ER = sdio_private.sdio_isr_mask.hwfre1er_interrupte_enable_mask;

    sdio_slave_nvic_set(true);
}


void sdio_slave_queue_property_config(void)
{
    if (sdio_private.sdio_property.checksum_enable) {
        SDIO_SLAVE_REG->HWFCR |= HWFCR_TRX_DESC_CHKSUM_EN_MASK;

        if (sdio_private.sdio_property.checksum_use_16bytes) {
            SDIO_SLAVE_REG->HWFCR &= (~HWFCR_TRX_DESC_CHKSUM_12B_MASK);
        } else {
            SDIO_SLAVE_REG->HWFCR |= HWFCR_TRX_DESC_CHKSUM_12B_MASK;
        }
    } else {
        if (HWFCR_TRX_DESC_CHKSUM_EN_MASK == (SDIO_SLAVE_REG->HWFCR & HWFCR_TRX_DESC_CHKSUM_EN_MASK)) {
            SDIO_SLAVE_REG->HWFCR = SDIO_SLAVE_REG->HWFCR & (~HWFCR_TRX_DESC_CHKSUM_EN_MASK);
        }
    }
}


void sdio_slave_init(void)
{
    sdio_slave_private_init();

    sdio_slave_hardware_init();

    sdio_slave_interrupt_init();

    sdio_slave_queue_property_config();
}


void sdio_slave_set_tx_queue_node_add(sdio_slave_tx_queue_id_t queue_id, uint32_t add_queue_number)
{
    SDIO_SLAVE_REG->HWTPCCR = (queue_id << HWTPCCR_TQ_INDEX_OFFSET) | (add_queue_number << HWTPCCR_INC_TQ_CNT_OFFSET);
}

void sdio_slave_tx_queue_count_reset(void)
{
    SDIO_SLAVE_REG->HWTPCCR = HWTPCCR_TQ_CNT_RESET_MASK;
}



void sdio_slave_set_tx_queue_start(sdio_slave_tx_queue_id_t queue_id)
{
    if (SDIO_SLAVE_TX_QUEUE_1 == queue_id) {
        SDIO_SLAVE_REG->HWTDCR = SDIO_SLAVE_TX_QUEUE_START(queue_id);
    } else {
        log_hal_error("queue id invalid.\r\n");
    }
}

void sdio_slave_set_tx_queue_stop(sdio_slave_tx_queue_id_t queue_id)
{
    if (SDIO_SLAVE_TX_QUEUE_1 == queue_id) {
        SDIO_SLAVE_REG->HWTDCR = SDIO_SLAVE_TX_QUEUE_STOP(queue_id);
    } else {
        log_hal_error("queue id invalid.\r\n");
    }
}

void sdio_slave_set_tx_queue_resume(sdio_slave_tx_queue_id_t queue_id)
{
    if (SDIO_SLAVE_TX_QUEUE_1 == queue_id) {
        SDIO_SLAVE_REG->HWTDCR = SDIO_SLAVE_TX_QUEUE_RESUME(queue_id);
    } else {
        log_hal_error("queue id invalid.\r\n");
    }
}


void sdio_slave_set_rx_queue_start(sdio_slave_rx_queue_id_t queue_id)
{
    if (SDIO_SLAVE_RX_QUEUE_0 == queue_id) {
        SDIO_SLAVE_REG->HWRQ0CR = HWRQ0CR_RXQ0_DMA_START_MASK;
    } else if (SDIO_SLAVE_RX_QUEUE_1 == queue_id) {
        SDIO_SLAVE_REG->HWRQ1CR = HWRQ1CR_RXQ1_DMA_START_MASK;
    } else {
        log_hal_error("queue id invalid.\r\n");
    }
}

void sdio_slave_set_rx_queue_stop(sdio_slave_rx_queue_id_t queue_id)
{
    if (SDIO_SLAVE_RX_QUEUE_0 == queue_id) {
        SDIO_SLAVE_REG->HWRQ0CR = HWRQ0CR_RXQ0_DMA_STOP_MASK;
    } else if (SDIO_SLAVE_RX_QUEUE_1 == queue_id) {
        SDIO_SLAVE_REG->HWRQ1CR = HWRQ1CR_RXQ1_DMA_STOP_MASK;
    } else {
        log_hal_error("queue id invalid.\r\n");
    }
}


void sdio_slave_set_rx_queue_resume_and_length(sdio_slave_rx_queue_id_t queue_id, uint32_t length)
{
    if (SDIO_SLAVE_RX_QUEUE_0 == queue_id) {
        SDIO_SLAVE_REG->HWRQ0CR = HWRQ0CR_RXQ0_DMA_RUM_MASK | (length & HWRQ0CR_RXQ0_PACKET_LENGTH_MASK);
    } else if (SDIO_SLAVE_RX_QUEUE_1 == queue_id) {
        SDIO_SLAVE_REG->HWRQ1CR = HWRQ1CR_RXQ1_DMA_RUM_MASK | (length & HWRQ1CR_RXQ1_PACKET_LENGTH_MASK);
    } else {
        log_hal_error("queue id invalid.\r\n");
    }
}


void sdio_slave_set_rx_queue_packet_length(sdio_slave_rx_queue_id_t queue_id, uint32_t length)
{
    if (SDIO_SLAVE_RX_QUEUE_0 == queue_id) {
        SDIO_SLAVE_REG->HWRQ0CR = length & HWRQ0CR_RXQ0_PACKET_LENGTH_MASK;
    } else if (SDIO_SLAVE_RX_QUEUE_1 == queue_id) {
        SDIO_SLAVE_REG->HWRQ1CR = length & HWRQ1CR_RXQ1_PACKET_LENGTH_MASK;
    } else {
        log_hal_error("queue id invalid.\r\n");
    }
}


void sdio_slave_set_tx_queue_start_address(sdio_slave_tx_queue_id_t queue_id, uint32_t gpd_header)
{
    if (SDIO_SLAVE_TX_QUEUE_1 == queue_id) {
        SDIO_SLAVE_REG->HWFTQ1SAR = gpd_header;
    } else {
        log_hal_error("queue id invalid.\r\n");
    }
}


void sdio_slave_set_rx_queue_start_address(sdio_slave_rx_queue_id_t queue_id, uint32_t gpd_header)
{
    if (SDIO_SLAVE_RX_QUEUE_0 == queue_id) {
        SDIO_SLAVE_REG->HWFRQ0SAR = gpd_header;
    } else if (SDIO_SLAVE_RX_QUEUE_1 == queue_id) {
        SDIO_SLAVE_REG->HWFRQ1SAR = gpd_header;
    } else {
        log_hal_error("queue id invalid.\r\n");
    }
}

uint8_t sdio_slave_calculate_checksum_by_len(const uint8_t *data, uint32_t check_len)
{
    uint8_t chksum = 0;

    while (check_len--) {
        chksum += *data ++;
    }

    return (uint8_t)~chksum;
}


sdio_slave_status_t sdio_slave_prepare_gpd(sdio_slave_rx_queue_id_t queue_id, uint32_t data_address, uint32_t data_lenght)
{
    return SDIO_SLAVE_STATUS_OK;
}


sdio_slave_status_t sdio_slave_wait_tx_done(sdio_slave_tx_queue_id_t queue_id)
{
    sdio_slave_status_t status;

    do {
        status = sdio_slave_interrupt_status_polling();

        if (SDIO_SLAVE_STATUS_OK != status) {
            return status;
        }
    } while (!tx_done_status);

    tx_done_status = 0;

    return SDIO_SLAVE_STATUS_OK;
}

sdio_slave_status_t sdio_slave_wait_rx_done(sdio_slave_tx_queue_id_t queue_id)
{
    sdio_slave_status_t status;

    do {
        status = sdio_slave_interrupt_status_polling();

        if (SDIO_SLAVE_STATUS_OK != status) {
            return status;
        }
    } while (!rx_done_status);

    rx_done_status = 0;

    return SDIO_SLAVE_STATUS_OK;
}


sdio_slave_status_t sdio_slave_send(sdio_slave_rx_queue_id_t queue_id, uint32_t data_address, uint32_t data_length)
{
    sdio_slave_gpd_header_t *rx_gpd;

    if (queue_id >= SDIO_SLAVE_RX_QUEUE_MAX) {
        log_hal_error("rx queue id error.\r\n");
        return SDIO_SLAVE_STATUS_INVALID_PARAMETER;
    }

    if (data_address & 0x03) {
        log_hal_error("rx data address 4-byte align error.\r\n");
        return SDIO_SLAVE_STATUS_INVALID_PARAMETER;
    }

    if (data_length > SDIO_SLAVE_MAX_PACKET_LENGTH) {
        log_hal_error("rx data length error.\r\n");
        return SDIO_SLAVE_STATUS_INVALID_PARAMETER;
    }

#ifdef HAL_SLEEP_MANAGER_ENABLED
        sdio_slave_nvic_set(false); // avoid race condition. due to interrupt may interrupt
        if (!already_acquire_sleep_locked) {
            if (hal_sleep_manager_acquire_sleeplock(SLEEP_LOCK_SDIO_SLV, HAL_SLEEP_LOCK_ALL) == HAL_SLEEP_MANAGER_OK){
                already_acquire_sleep_locked = true;
                sdio_slave_nvic_set(true);
                log_hal_info("SLEEP_LOCK_SDIO_SLV lock sleep success. (sdio_slave_send)\r\n");
            } else {
                sdio_slave_nvic_set(true);
                log_hal_info("SLEEP_LOCK_SDIO_SLV lock sleep fail. (sdio_slave_send)\r\n");
            }
        } else {
            sdio_slave_nvic_set(true);
            log_hal_info("already_acquire_sleep_locked, so don't acquire again. (sdio_slave_send)\r\n");
        }
#endif

    sdio_slave_transmission_state |= (queue_id == SDIO_SLAVE_RX_QUEUE_0) ? SDIO_SLAVE_TRANSMISSION_SEND_RX_QUEUE_0 : SDIO_SLAVE_TRANSMISSION_SEND_RX_QUEUE_1;


    rx_gpd = &sdio_slave_rx_gpd[queue_id];
    memset(rx_gpd, 0, sizeof(sdio_slave_gpd_header_t));

    memset(&sdio_slave_gpd_null, 0, sizeof(sdio_slave_gpd_header_t));
    rx_gpd->next = &sdio_slave_gpd_null;

    /*GPD header configuration.*/
    SDIO_SLAVE_GPD_SET_BUFFER_ADDRESS(rx_gpd, data_address);
    SDIO_SLAVE_GPD_SET_BUFFER_LENGTH(rx_gpd, data_length);

    SDIO_SLAVE_GPD_SET_HW0(rx_gpd);
    SDIO_SLAVE_GPD_SET_ALLOW_LENGTH(rx_gpd, 0xfff);
    SDIO_SLAVE_GPD_SET_CHECKSUM(rx_gpd, sdio_slave_calculate_checksum_by_len((const uint8_t *)rx_gpd, 12));

    sdio_slave_set_rx_queue_start_address(queue_id, (uint32_t)rx_gpd);

    /*start queue.*/
    sdio_slave_set_rx_queue_start(queue_id);

    sdio_slave_set_rx_queue_resume_and_length(queue_id, data_length);

    return SDIO_SLAVE_STATUS_OK;

}



sdio_slave_status_t sdio_slave_receive(sdio_slave_tx_queue_id_t queue_id, uint32_t data_address, uint32_t data_length)
{
    sdio_slave_gpd_header_t *tx_gpd;

    if (queue_id != SDIO_SLAVE_TX_QUEUE_1) {
        log_hal_error("tx queue id error.\r\n");
        return SDIO_SLAVE_STATUS_INVALID_PARAMETER;
    }

    if (data_address & 0x03) {
        log_hal_error("tx data address 4-byte align error.\r\n");
        return SDIO_SLAVE_STATUS_INVALID_PARAMETER;
    }

    if (data_length > SDIO_SLAVE_MAX_PACKET_LENGTH) {
        log_hal_error("tx data length error.\r\n");
        return SDIO_SLAVE_STATUS_INVALID_PARAMETER;
    }

#ifdef HAL_SLEEP_MANAGER_ENABLED
        sdio_slave_nvic_set(false); // avoid race condition. due to interrupt may interrupt
        if (!already_acquire_sleep_locked) {
            if (hal_sleep_manager_acquire_sleeplock(SLEEP_LOCK_SDIO_SLV, HAL_SLEEP_LOCK_ALL) == HAL_SLEEP_MANAGER_OK){
                already_acquire_sleep_locked = true;
                sdio_slave_nvic_set(true);
                log_hal_info("SLEEP_LOCK_SDIO_SLV lock sleep success. (sdio_slave_receive)\r\n");
            } else {
                sdio_slave_nvic_set(true);
                log_hal_info("SLEEP_LOCK_SDIO_SLV lock sleep fail. (sdio_slave_receive)\r\n");
            }
        } else {
            sdio_slave_nvic_set(true);
            log_hal_info("already_acquire_sleep_locked, so don't acquire again. (sdio_slave_receive)\r\n");
        }
#endif

    sdio_slave_transmission_state |= SDIO_SLAVE_TRANSMISSION_RECEIVE_TX_QUEUE_1;


    tx_gpd = &sdio_slave_tx_gpd[queue_id];
    memset(tx_gpd, 0, sizeof(sdio_slave_gpd_header_t));

    memset(&sdio_slave_gpd_null, 0, sizeof(sdio_slave_gpd_header_t));
    tx_gpd->next = &sdio_slave_gpd_null;

    /*GPD header configuration.*/
    SDIO_SLAVE_GPD_SET_BUFFER_ADDRESS(tx_gpd, data_address);

    SDIO_SLAVE_GPD_SET_HW0(tx_gpd);
    SDIO_SLAVE_GPD_SET_ALLOW_LENGTH(tx_gpd, 0xfff);
    SDIO_SLAVE_GPD_SET_CHECKSUM(tx_gpd, sdio_slave_calculate_checksum_by_len((const uint8_t *)tx_gpd, 12));

    /*set GPD header to queue start register.*/
    sdio_slave_set_tx_queue_start_address(queue_id, (uint32_t)tx_gpd);

    /*start queue.*/
    sdio_slave_set_tx_queue_start(queue_id);

    /*modify queue count*/
    sdio_slave_set_tx_queue_node_add(queue_id, 1);

    return SDIO_SLAVE_STATUS_OK;
}


#endif /*HAL_SDIO_SLAVE_MODULE_ENABLED*/

